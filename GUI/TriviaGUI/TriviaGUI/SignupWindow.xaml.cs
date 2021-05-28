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
                TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];

                Dictionary<string, string> loginDitails = new Dictionary<string, string>();
                string address_str = this.street_control.Text + "," + this.apt_control.Text + "," + this.city_control.Text;
                loginDitails.Add("username", username_control.Text);
                loginDitails.Add("password", password_control.Password);
                loginDitails.Add("email", email_control.Text);
                loginDitails.Add("address", address_str);
                loginDitails.Add("phone", phone_number_control.Text);
                loginDitails.Add("birthdate", "");
                if (birthdateControl.SelectedDate.HasValue)
                {
                    loginDitails["birthdate"] = birthdateControl.SelectedDate.Value.ToString("MM/dd/yyyy");
                }
                

                string json_parsed = JsonConvert.SerializeObject(loginDitails);
                byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed);
                byte[] data_encoded =  ServerFunctions.ServerFunctions.getCompleteMsg(2, json_byted);

                serverConnection.GetStream().Write(data_encoded, 0, data_encoded.Length);
                System.Threading.Thread.Sleep(100);

                byte[] serverOutput = new byte[1024];

                serverConnection.GetStream().Read(serverOutput, 0, serverOutput.Length);

                Newtonsoft.Json.Linq.JObject dis = ServerFunctions.ServerFunctions.diserallizeResponse(serverOutput);
                try
                {
                    if (dis.First.First.ToString() == "1")
                    {
                        infoBox.Content = "You logged in successfully!";
                    }
                    else
                    {
                        infoBox.Content = "error! check the validty of the fields(username, email, etc..)";
                    }
                }
                catch (Exception ex)
                {
                    infoBox.Content = "there is an error.";
                }
            }
            else
            {
                infoBox.Content = "error! client isn't connected to the server";
            }
        }
    }
}
