using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using Newtonsoft.Json;
using System.Threading;
using ServerFunctions;
using System.Windows.Controls;

namespace TriviaGUI
{
    class PlayerRoomControles
    {
        /// <summary>
        /// Refreshing the players in the room
        /// </summary>
        public static void refreshPlayersInRoom()
        {
            if ((bool)App.Current.Properties["isInRoom"] == true)
            {
                ListBoxItem item;
                TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
                byte[] client_message = { 5 }; // get players in room
                System.Windows.Threading.Dispatcher dis = ((System.Windows.Threading.Dispatcher)App.Current.Properties["dispatcher"]);
                ListBox list_box = (ListBox)App.Current.Properties["list_box"];
                string[] players;

                while ((bool)App.Current.Properties["isInRoom"] == true)
                {
                    byte[] server_message;

                    serverConnection.GetStream().Write(client_message, 0, 1);
                    while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
                    server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server

                    dis.Invoke(() =>
                    {
                        Newtonsoft.Json.Linq.JObject json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);
                        players = json_returned["players"].ToString().Split(new char[] { ',', ' ' }, StringSplitOptions.RemoveEmptyEntries);

                        if (players != null) // players will be null if an error in the server has happend
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
                    });

                    Thread.Sleep(3000); // refreshing every 3 seconds
                }
            }
        }
    }
}
