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
using System.Net;
using System.Net.Sockets;
using System.Threading;
using Newtonsoft.Json;
using ServerFunctions;

namespace TriviaGUI
{
    public class ServerMessage
    {
        public int Status { get; set; }
    }
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    ///
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            Thread connectThread = new Thread(new ThreadStart(ServerFunctions.ServerFunctions.ConnectingToServer));
            connectThread.Start();
        }

        private void cancel_button_Click(object sender, RoutedEventArgs e)
        {
            user_name_text_box.Text = string.Empty;
            password_text_box.Password = string.Empty;
        }

        private void login_button_Click(object sender, RoutedEventArgs e)
        {
            if (App.Current.Properties["server"] != null)
            {
                TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];

                Dictionary<string, string> loginDitails = new Dictionary<string, string>();
                loginDitails.Add("username", user_name_text_box.Text);
                loginDitails.Add("password", password_text_box.Password);

                string json_parsed = JsonConvert.SerializeObject(loginDitails);
                byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed);
                byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(1, json_byted);

                serverConnection.GetStream().Write(data_encoded, 0, 1000);
                System.Threading.Thread.Sleep(100);

                byte[] serverOutput = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
                Newtonsoft.Json.Linq.JObject dis = ServerFunctions.ServerFunctions.diserallizeResponse(serverOutput);
                try
                {
                    if (dis.First.First.ToString() == "1")
                    {
                        MainMenu menu = new MainMenu();
                        menu.Show();

                        this.Close();
                    }
                    else
                    {
                        Error_label.Content = "error! username or password are incorrect";
                    }
                }
                catch (Exception ex)
                {

                }
            }
            else
            {
                Error_label.Content = "error! client isn't connected to the server";
            }
        }

        private void signup_button_Click(object sender, RoutedEventArgs e)
        {
            if (App.Current.Properties["server"] != null)
            {
                SignupScreen signup_window = new SignupScreen();
                signup_window.Show();
                Application.Current.MainWindow.Hide();
            }
        }
    }
}
