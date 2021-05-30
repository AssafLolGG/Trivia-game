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
    /// <summary>
    /// Interaction logic for RoomMenu.xaml
    /// </summary>
    public partial class RoomMenu : Window
    {
        private bool GetCurrentPlayersInRoom(TcpClient serverConnection, int roomId, out string[] completedData)
        {
            completedData = new string[1000];
            Dictionary<string, object> RoomDitalis = new Dictionary<string, object>();
            RoomDitalis.Add("roomID", roomId.ToString());
            string json_parsed = JsonConvert.SerializeObject(RoomDitalis);
            byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed);
            byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(5, json_byted);

            serverConnection.GetStream().Write(data_encoded, 0, 1000);

            while (serverConnection.Available == 0) ;
            System.Threading.Thread.Sleep(100);
            byte[] serverOutput = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
            Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.ServerFunctions.diserallizeResponse(serverOutput);
            try
            {
                if (jsonReturned["players"].ToString() != "")
                {
                    string[] seperators = { ", " };
                    string[] players = jsonReturned["players"].ToString().Split(seperators, System.StringSplitOptions.RemoveEmptyEntries);
                    completedData = players;
                    return true;
                }
            }
            catch (Exception e)
            { }
            return false;
        }

        /* retrieving a specific room data*/
        private bool GetRoomDataString(TcpClient serverConnection, string roomName, int room_id, out string completedData)
        {
            completedData = "";
            Dictionary<string, object> RoomDitalis = new Dictionary<string, object>();
            RoomDitalis.Add("roomID", room_id.ToString()); // inserting room id to json

            string json_parsed = JsonConvert.SerializeObject(RoomDitalis); // serializing data
            byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed); // encoding json
            byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(9, json_byted); // adding size and code for protocal

            serverConnection.GetStream().Write(data_encoded, 0, 1000);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
            byte[] serverOutput = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
            Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.ServerFunctions.diserallizeResponse(serverOutput); // diserallizing json from server
            try
            {
                string[] players;
                if (jsonReturned["status"].ToString() == "1" && jsonReturned["isActive"].ToString() == "1") // checks if no error has occurred and if the server is active
                {
                    completedData += room_id.ToString() + "        ";

                    if (this.GetCurrentPlayersInRoom(serverConnection, room_id, out players) == true) // getting the current players in room and checking if the number of max player is valid
                    {
                        // writing the room data to a string
                        completedData += roomName + "         ";
                        completedData += players.Length.ToString() + " / " + jsonReturned["maxPlayers"].ToString() + "                                        ";
                        completedData += jsonReturned["questionsNumber"].ToString() + "                                    ";
                        completedData += jsonReturned["timePerQuestion"].ToString();
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }

            }
            catch (Exception e)
            { }
            return false;
        }

        /* refreshing the rooms*/
        public void refreshRoomList()
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"]; // saving socket inside the app
            byte[] data_encoded = { 4 }; // preparing a message with get rooms code (4) as the code
            serverConnection.GetStream().Write(data_encoded, 0, 1);
            System.Threading.Thread.Sleep(1000);

            // getting server json and diserallizing it
            byte[] rooms_names = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
            Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.ServerFunctions.diserallizeResponse(rooms_names);

            // server json will return a json with room names and id's with ',' as a seperator, the name and id will match in the corresponding place
            string rooms_names_string = jsonReturned["rooms"].ToString();
            string[] seperators = { ", " };
            string[] rooms = rooms_names_string.Split(seperators, System.StringSplitOptions.RemoveEmptyEntries); // seperates the room names to a string array
            string rooms_id_string = jsonReturned["rooms_id"].ToString();
            string[] rooms_id = rooms_id_string.Split(seperators, System.StringSplitOptions.RemoveEmptyEntries); // seperates the room id's to a string array

            // creating a list box with all of the rooms with thew corresponding data
            for (int i = 0; i < rooms.Length; i++)
            {
                ListBoxItem item = new ListBoxItem();
                string contentOfItem = "";
                if (GetRoomDataString(serverConnection, rooms[i], int.Parse(rooms_id[i]), out contentOfItem) == true)
                {
                    item.Content = contentOfItem;
                    this.rooms_list.Items.Add(item);
                }
            }
            
        }
        public RoomMenu()
        {
            InitializeComponent();
            refreshRoomList();
        }

        private void join_room_button_Click(object sender, RoutedEventArgs e)
        {

        }

        private void back_to_menu_button_Click(object sender, RoutedEventArgs e)
        {
            MainMenu menu = new MainMenu();
            menu.Show();

            this.Close();
        }

        private void create_room_button_Click(object sender, RoutedEventArgs e)
        {
            CreateMenu create = new CreateMenu();
            create.Show();

            this.Close();
        }
    }
}