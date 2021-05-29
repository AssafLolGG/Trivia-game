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
                item.Content = room;
                this.rooms_list.Items.Add(item);
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