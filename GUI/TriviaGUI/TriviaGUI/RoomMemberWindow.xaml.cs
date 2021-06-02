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
        public RoomMemberWindow(int room_id, string room_name, int max_players, int questions_num, int time_per_question)
        {
            InitializeComponent();

            Thread t = new Thread(new ThreadStart(refreshPlayersInRoom));
            t.SetApartmentState(ApartmentState.STA);
            t.Start();

            prepareText(room_id, room_name, max_players, questions_num, time_per_question);
        }

        private void leave_button_Click(object sender, RoutedEventArgs e)
        {

        }

        private void leaveRoom()
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

        private void refreshPlayersInRoom()
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

        private void prepareText(int room_id, string room_name, int max_players, int questions_num, int time_per_question)
        {
            this.id_text.Text = "Id - " + room_id.ToString();
            this.name_text.Text = "Name - " + room_name;
            this.question_number_text.Text = "Number Of Question - " + questions_num.ToString();
            this.time_per_question_text.Text = "Time Per Question - " + time_per_question.ToString();
        }
    }
}
