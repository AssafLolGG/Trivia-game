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

        }
    }
}
