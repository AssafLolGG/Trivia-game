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
using TriviaGUI;

namespace TriviaGUI
{
    /// <summary>
    /// Interaction logic for RoomAdminWindow.xaml
    /// </summary>
    public partial class RoomAdminWindow : Window
    {
        private Thread refresh_players_list_thread;
        private Mutex server_mutex;
        private readonly object sendSyncRoot = new object();
        private readonly object receiveSyncRoot = new object();

        public RoomAdminWindow(int room_id, string room_name, int max_players, int questions_num, int time_per_question)
        {
            InitializeComponent();

            App.Current.Properties["dispatcher"] = this.Dispatcher;
            App.Current.Properties["list_box"] = this.active_players_list;

            server_mutex = new Mutex();
            App.Current.Properties["server_mutex"] = server_mutex;
            App.Current.Properties["send_lock"] = sendSyncRoot;
            App.Current.Properties["receive_lock"] = receiveSyncRoot;

            this.refresh_players_list_thread = new Thread(() => TriviaGUI.PlayerRoomControles.refreshPlayersInRoom());
            this.refresh_players_list_thread.SetApartmentState(ApartmentState.STA);
            this.refresh_players_list_thread.Start();

            prepareText(room_id, room_name, max_players, questions_num, time_per_question);
        }
      
        private void refreshPlayersInRoom()
        {
            if ((bool)App.Current.Properties["isInRoom"] == true)
            {
                ListBoxItem item;
                TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
                byte[] client_message = { 5 }; // get players in room

                while (true)
                {
                    serverConnection.GetStream().Write(client_message, 0, 1);

                    while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
                    byte[] server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server
                    Newtonsoft.Json.Linq.JObject json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);

                    string[] players = json_returned["players"].ToString().Split(',');

                    this.Dispatcher.Invoke(() =>
                    {
                        active_players_list.Items.Clear();
                    });

                    for (int i = 0; i < players.Length; i++)
                    {
                        this.Dispatcher.Invoke(() =>
                        {
                            item = new ListBoxItem();
                            item.Content = players[i];
                            active_players_list.Items.Add(item);
                        });
                    }
                    Thread.Sleep(3000);
                }
            }
        }

        private void prepareText(int room_id, string room_name, int max_players, int questions_num, int time_per_question)
        {
            this.id_text.Text = "Id - " + room_id.ToString();
            this.name_text.Text = "Name - " + room_name;
            this.question_number_text.Text = "Number Of Question - " + questions_num.ToString();
            this.time_per_question_text.Text = "Time Per Question - " + time_per_question.ToString();
        }

        private void start_button_Click(object sender, RoutedEventArgs e)
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] client_message = { 12 }; // start room code

            server_mutex.WaitOne();

            serverConnection.GetStream().Write(client_message, 0, 1);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
            byte[] server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server

            server_mutex.ReleaseMutex();

            Newtonsoft.Json.Linq.JObject json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);

            if(json_returned["status"].ToString() == "1")
            {
                MessageBox.Show("Game Started");
            }
            else
            {
                MessageBox.Show("Game Couldn't Start");
            }
        }

        private void close_button_Click(object sender, RoutedEventArgs e)
        {
            App.Current.Properties["isInRoom"] = false;
            lock (server_mutex)
            {
                TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
                byte[] client_message = { 11 }; // close room code
                byte[] server_message;


                serverConnection.GetStream().Write(client_message, 0, 1);

                while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
                server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server



                Newtonsoft.Json.Linq.JObject json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);

                if (json_returned["status"].ToString() == "1")
                {
                    this.refresh_players_list_thread.Abort();

                    RoomMenu rmenu = new RoomMenu();
                    rmenu.Show();

                    this.Close();
                }
                else
                {
                    MessageBox.Show("Room Closed Request Failed");
                }
            }
        }
    }
}
