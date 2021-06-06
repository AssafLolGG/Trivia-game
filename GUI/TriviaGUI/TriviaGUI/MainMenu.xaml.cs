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
using ServerFunctions;

namespace TriviaGUI
{
    /// <summary>
    /// Interaction logic for MainMenu.xaml
    /// </summary>
    public partial class MainMenu : Window
    {
        public MainMenu()
        {
            InitializeComponent();
        }

        private void room_menu_button_Click(object sender, RoutedEventArgs e)
        {
            RoomMenu room_menu = new RoomMenu();
            room_menu.Show();

            this.Close();
        }

        private void statistics_buttons_Click(object sender, RoutedEventArgs e)
        {
            Statistics stats = new Statistics();
            stats.Show();

            this.Close();
        }

        private void log_out_button_Click(object sender, RoutedEventArgs e)
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] data_encoded = { 3 };
            serverConnection.GetStream().Write(data_encoded, 0, 1);
            System.Threading.Thread.Sleep(100);

            ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
            MainWindow login_screen = new MainWindow();
            login_screen.Show();

            this.Close();
        }
    }
}