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
    /// Interaction logic for SignupScreen.xaml
    /// </summary>
    public partial class SignupScreen : Window
    {
        public SignupScreen()
        {
            InitializeComponent();
        }

        private void back_to_login_Button_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.MainWindow.Show();
            this.Hide();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (App.Current.Properties["server"] != null)
            {

            }
        }
    }
}
