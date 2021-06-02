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
            Mutex server_mutex = (Mutex)App.Current.Properties["server_mutex"];
            string[] players;

            while (true)
            {
                byte[] server_message;

                try
                {
                    lock (App.Current.Properties["send_lock"])
                    {
                        serverConnection.GetStream().Write(client_message, 0, 1);
                    }
                    lock (App.Current.Properties["receive_lock"])
                    {
                        while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
                        server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server
                    }

                    dis.Invoke(() =>
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
                    });
                }
                finally
                {

                }

                Thread.Sleep(3000);
            }
        }
    }
}
