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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TriviaGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string username = "user";
        string password = "password";
        public MainWindow()
        {
            InitializeComponent();
        }

        private void cancel_button_Click(object sender, RoutedEventArgs e)
        {
            user_name_text_box.Text = string.Empty;
            password_text_box.Password = string.Empty;
        }

        private void enter_button_Click(object sender, RoutedEventArgs e)
        {
            if (user_name_text_box.Text == username && password_text_box.Password == password)
            {
               // Window1 win1 = new Window1();
               // win1.Show();
                Error_label.Content = string.Empty;
                Application.Current.MainWindow.Close();
            }
            else
            {
                Error_label.Content = "error! username or password are incorrect";
            }
        }
    }
}
