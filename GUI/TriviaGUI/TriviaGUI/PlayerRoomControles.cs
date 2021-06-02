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
        public static void refreshPlayersInRoom()
        {
            ListBoxItem item;
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] client_message = { 5 }; // get players in room
            System.Windows.Threading.Dispatcher dis = ((System.Windows.Threading.Dispatcher)App.Current.Properties["dispatcher"]);
            ListBox list_box = (ListBox)App.Current.Properties["list_box"];

            while (true)
            {
                serverConnection.GetStream().Write(client_message, 0, 1);

                while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
                byte[] server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server
                Newtonsoft.Json.Linq.JObject json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);

                string[] players = json_returned["players"].ToString().Split(',');

               dis.Invoke(() =>
                {
                    list_box.Items.Clear();
                });

                for (int i = 0; i < players.Length; i++)
                {
                    ((System.Windows.Threading.Dispatcher)App.Current.Properties["dispatcher"]).Invoke(() =>
                    {
                        item = new ListBoxItem();
                        item.Content = players[i];
                        list_box.Items.Add(item);
                    });
                }
                Thread.Sleep(3000);
            }
        }
    }
}
