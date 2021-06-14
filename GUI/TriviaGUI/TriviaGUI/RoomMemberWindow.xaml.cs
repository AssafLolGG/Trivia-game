using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Net.Sockets;
using Newtonsoft.Json;
using System.Threading;
using ServerFunctions;

namespace TriviaGUI
{
    /// <summary>
    /// Interaction logic for RoomMemberWindow.xaml
    /// </summary>
    public partial class RoomMemberWindow : Window
    {
        private Thread refresh_players_list_thread;

        public RoomMemberWindow()
        {
            InitializeComponent();

            App.Current.Properties["dispatcher"] = this.Dispatcher;
            App.Current.Properties["list_box"] = this.active_players_list;

            // creating thread to check exit
            this.refresh_players_list_thread = new Thread(() => this.listenIfExit());
            this.refresh_players_list_thread.SetApartmentState(ApartmentState.STA);
            this.refresh_players_list_thread.Start();

            Newtonsoft.Json.Linq.JObject room_data = getRoomData();

            while(room_data == null)
            {
                room_data = getRoomData();
            }

            // getting string of roomData
            string room_id = room_data["id"].ToString();
            string room_name = room_data["name"].ToString();
            string max_players = room_data["maxPlayers"].ToString();
            string question_num = room_data["questionsNumber"].ToString();
            string time_per_question = room_data["timePerQuestion"].ToString();

            App.Current.Properties["numOfQuestions"] = room_data["questionsNumber"].ToString();
            App.Current.Properties["timeOutPerQuestion"] = room_data["timePerQuestion"].ToString();

            prepareText(room_id, room_name, max_players, question_num, time_per_question);
        }

        private void leave_button_Click(object sender, RoutedEventArgs e)
        {
            App.Current.Properties["isInRoom"] = false;
            leaveRoom(e);
        }
        private Newtonsoft.Json.Linq.JObject getRoomData()
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] client_message = { 9 };
            byte[] personal_statistics_json;

            serverConnection.GetStream().Write(client_message, 0, 1);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
            personal_statistics_json = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server

            Newtonsoft.Json.Linq.JObject server_json = ServerFunctions.ServerFunctions.diserallizeResponse(personal_statistics_json);


            if (server_json.ContainsKey("status") && server_json["status"].ToString() == "1")
            {
                return server_json;
            }

            return null;
        }

        private void leaveRoom(EventArgs event_e)
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] client_message = { 13 }; // leave room code

            serverConnection.GetStream().Write(client_message, 0, 1);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
            byte[] server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server
            Newtonsoft.Json.Linq.JObject json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);

            if (json_returned["status"].ToString() == "1")
            {
                RoomMenu rmenu = new RoomMenu();
                rmenu.Show();

                this.Close();
            }
            else
            {
                MessageBox.Show("Couldn't Leave Room");
            }
        }

        private void prepareText(string room_id, string room_name, string max_players, string questions_num, string time_per_question)
        {
            this.id_text.Text = "Id - " + room_id;
            this.name_text.Text = "Name - " + room_name;
            this.question_number_text.Text = "Number Of Question - " + questions_num;
            this.time_per_question_text.Text = "Time Per Question - " + time_per_question;
        }

        private void listenIfExit()
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            Mutex server_mutex = (Mutex)App.Current.Properties["server_mutex"];
            ListBoxItem item;
            byte[] client_message = { 5 }; // get players in room
            System.Windows.Threading.Dispatcher dis = ((System.Windows.Threading.Dispatcher)App.Current.Properties["dispatcher"]);
            ListBox list_box = (ListBox)App.Current.Properties["list_box"];
            string[] players;
            while ((bool)App.Current.Properties["isInRoom"] == true)
            {
                byte[] server_message;

                try
                {
                    serverConnection.GetStream().Write(client_message, 0, 1);
                    while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
                    server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server

                    if (dis.HasShutdownFinished == false)
                    {
                        dis.Invoke(() =>
                        {
                            if (server_message[0] == 5)
                            {
                                Newtonsoft.Json.Linq.JObject json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);
                                players = json_returned["players"].ToString().Split(',');

                                if (players != null)
                                {
                                    list_box.Items.Clear();

                                    for (int i = 0; i < players.Length; i++)
                                    {
                                        ((System.Windows.Threading.Dispatcher)App.Current.Properties["dispatcher"]).Invoke(() =>
                                        {
                                            item = new ListBoxItem();
                                            item.Content = players[i];
                                            list_box.Items.Add(item);
                                        });
                                    }
                                }
                            }
                            else if (server_message[0] == 11) // if the room closed
                            {
                                App.Current.Properties["isInRoom"] = false;
                                while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
                                server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server
                                RoomMenu rmenu = new RoomMenu();
                                rmenu.Show();

                                this.Close();
                                return;
                            }
                            else if (server_message[0] == 12) // if the room started
                            {
                                App.Current.Properties["isInRoom"] = false;
                                while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
                                server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server
                                TriviaGameRoom triviaGame = new TriviaGameRoom();
                                triviaGame.Show();

                                this.Close();

                                return;
                            }
                        });
                        if(server_message[0] == 12 || server_message[0] == 11)
                        {
                            return;
                        }
                        Thread.Sleep(3000);
                    }
                }
                catch(Exception E)
                {
                    App.Current.Properties["isInRoom"] = false;
                    return;
                }
                finally
                {

                }

            }
        }
    }
}
