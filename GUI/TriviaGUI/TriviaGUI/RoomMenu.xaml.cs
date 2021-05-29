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
    /// Interaction logic for RoomMenu.xaml
    /// </summary>
    public partial class RoomMenu : Window
    {
        public RoomMenu()
        {
            InitializeComponent();
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
