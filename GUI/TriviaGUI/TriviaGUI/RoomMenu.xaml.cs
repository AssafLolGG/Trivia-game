﻿using System;
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
            completedData = new string[1];
            Dictionary<string, object> RoomDitalis = new Dictionary<string, object>();
            RoomDitalis.Add("roomID", roomId);
            string json_parsed = JsonConvert.SerializeObject(RoomDitalis);
            byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed);
            byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(5, json_byted);

            serverConnection.GetStream().Write(data_encoded, 0, data_encoded.Length);
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
        private bool GetRoomDataString(TcpClient serverConnection, string roomName, out string completedData)
        {
            completedData = "";
            Dictionary<string, object> RoomDitalis = new Dictionary<string, object>();
            RoomDitalis.Add("roomName", roomName);
            string json_parsed = JsonConvert.SerializeObject(RoomDitalis);
            byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed);
            byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(9, json_byted);

            serverConnection.GetStream().Write(data_encoded, 0, 1000);
            System.Threading.Thread.Sleep(100);

            byte[] serverOutput = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
            Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.ServerFunctions.diserallizeResponse(serverOutput);
            try
            {
                string[] players;
                if (jsonReturned["status"].ToString() == "1" && jsonReturned["isActive"].ToString() == "1")
                {
                    completedData += jsonReturned["id"].ToString() + "        ";
                    if (this.GetCurrentPlayersInRoom(serverConnection, int.Parse(jsonReturned["id"].ToString()), out players) == true)
                    {
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
        public void refreshRoomList()
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] data_encoded = { 4 };
            serverConnection.GetStream().Write(data_encoded, 0, 1);
            System.Threading.Thread.Sleep(100);

            byte[] rooms_names = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
            Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.ServerFunctions.diserallizeResponse(rooms_names);

            
            string rooms_names_string = jsonReturned.First.First.ToString();
            string[] seperators = { ", " };
            string[] rooms = rooms_names_string.Split(seperators, System.StringSplitOptions.RemoveEmptyEntries);
            foreach (string room in rooms)
            {
                ListBoxItem item = new ListBoxItem();
                string contentOfItem = "";
                if (GetRoomDataString(serverConnection, room, out contentOfItem) == true)
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

        private void joine_room_button_Click(object sender, RoutedEventArgs e)
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