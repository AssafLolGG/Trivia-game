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

namespace TriviaGUI
{
    /// <summary>
    /// Interaction logic for Statistics.xaml
    /// </summary>
    public partial class Statistics : Window
    {
        private void getPersonalStatistics()
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] data_encoded = { 4 }; // preparing a message with get rooms code (4) as the code
            serverConnection.GetStream().Write(data_encoded, 0, 1);
            System.Threading.Thread.Sleep(1000);
        }
        public Statistics()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            MainMenu menu = new MainMenu();
            menu.Show();
            this.Close();
        }
    }
}
