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
using System.Threading;
using Newtonsoft.Json;
using ServerFunctions;

namespace TriviaGUI
{
    public partial class RoomMenu : Window
    {
        private Thread refresh_room_list;
        private readonly object lock_name = new object();

        public RoomMenu()
        {
            InitializeComponent();

            App.Current.Properties["room_list"] = this.rooms_list;
            App.Current.Properties["dispatcher"] = this.Dispatcher;

            this.refresh_room_list = new Thread(() => this.refreshRoomList());
            this.refresh_room_list.SetApartmentState(ApartmentState.STA);
            this.refresh_room_list.Start();
        }

        /// <summary>
        /// refreshing the rooms
        /// </summary>
        public void refreshRoomList()
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] data_encoded = { 4 }; // preparing a message with get rooms code (4) as the code
            System.Windows.Threading.Dispatcher dis = ((System.Windows.Threading.Dispatcher)App.Current.Properties["dispatcher"]);
            Mutex server_mutex = (Mutex)App.Current.Properties["server_mutex"];

            while (true)
            {
                serverConnection.GetStream().Write(data_encoded, 0, 1);

                // getting server json and diserallizing it
                byte[] rooms_names = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
                Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.ServerFunctions.diserallizeResponse(rooms_names);

                // server json will return a json with room names and id's with ',' as a seperator, the name and id will match in the corresponding place
                try
                {
                    if (jsonReturned.ContainsKey("rooms"))
                    {
                        string rooms_names_string = jsonReturned["rooms"].ToString(); // bug here, check for null
                        string[] seperators = { ", " };
                        string[] rooms = rooms_names_string.Split(seperators, System.StringSplitOptions.RemoveEmptyEntries); // seperates the room names to a string array
                        string rooms_id_string = jsonReturned["rooms_id"].ToString();
                        string[] rooms_id = rooms_id_string.Split(seperators, System.StringSplitOptions.RemoveEmptyEntries); // seperates the room id's to a string array

                        dis.Invoke(() =>
                        {
                            ((ListBox)App.Current.Properties["room_list"]).Items.Clear();

                            // creating a list box with all of the rooms with thew corresponding data
                            for (int i = 0; i < rooms.Length; i++)
                            {
                                string contentOfItem = "";

                                if (ServerFunctions.ServerFunctions.GetRoomDataString(serverConnection, rooms[i], int.Parse(rooms_id[i]), out contentOfItem) == true)
                                {
                                    ListBoxItem item = new ListBoxItem();
                                    string[] split_items = contentOfItem.Split(',');

                                    contentOfItem = "";

                                    contentOfItem += split_items[0] + "        ";                                                                        // id
                                    contentOfItem += split_items[1] + "         ";                                                          // room name
                                    contentOfItem += split_items[2] + " / " + split_items[3] + "                                        ";  // current players / max players
                                    contentOfItem += split_items[4] + "                                    ";                               // question num
                                    contentOfItem += split_items[5];                                                                        // time per question

                                    item.Content = contentOfItem;

                                    ((ListBox)App.Current.Properties["room_list"]).Items.Add(item);                                    
                                }
                            }
                        });
                    }
                }
                catch (Exception e)
                { }

                Thread.Sleep(3000); // refresh every 3 seconds
            }
        }
        
        /// <summary>
        /// Join a room
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void join_room_button_Click(object sender, RoutedEventArgs e)
        {
            this.refresh_room_list.Abort();

            if(rooms_list.SelectedItem != null)
            {
                TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
                Dictionary<string, object> join_room_request = new Dictionary<string, object>();

                string room_string = this.rooms_list.SelectedItem.ToString().Split(' ')[1]; // getting the room data string from the selected room

                int room_id = Int32.Parse(room_string[0].ToString()); // getting the id from the room data string

                join_room_request.Add("roomID", room_id.ToString()); // adding room 

                string json_parsed = JsonConvert.SerializeObject(join_room_request); // serializing data
                byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed); // encoding json
                byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(6, json_byted); // adding size and code for protocal
                serverConnection.GetStream().Write(data_encoded, 0, 1000);

                while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
                byte[] serverOutput = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
                Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.ServerFunctions.diserallizeResponse(serverOutput); // diserallizing json from server

                try
                {
                    if (jsonReturned["status"].ToString() == "1")
                    {
                        App.Current.Properties["isInRoom"] = true;

                        RoomMemberWindow member = new RoomMemberWindow();
                        member.Show();

                        this.Close();
                    }
                    else
                    {
                        this.refresh_room_list.Start();
                    }
                }
                catch (Exception ex)
                {
                }
            }
            
        }

        private void back_to_menu_button_Click(object sender, RoutedEventArgs e)
        {
            this.refresh_room_list.Abort();

            MainMenu menu = new MainMenu();
            menu.Show();

            this.Close();
        }

        private void create_room_button_Click(object sender, RoutedEventArgs e)
        {
            this.refresh_room_list.Abort();

            CreateMenu create = new CreateMenu();
            create.Show();

            this.Close();
        }
    }
}