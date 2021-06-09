using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Windows;

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
            MainWindow login = new MainWindow();
            login.Show();

            this.Close();
        }

        private void signup_button_Click(object sender, RoutedEventArgs e)
        {
            if (App.Current.Properties["server"] != null)
            {
                TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
                
                if (username_control.Text != "" && password_control.Password != "" && email_control.Text != "" &&
                    street_control.Text != "" && apt_control.Text != "" && phone_number_control.Text != "" && city_control.Text != "" && birthdateControl.SelectedDate.HasValue) // check if one of the fields is empty
                {
                    Dictionary<string, string> loginDitails = new Dictionary<string, string>();
                    string address_str = this.street_control.Text + "," + this.apt_control.Text + "," + this.city_control.Text;  

                    // add all fields to the dictionary (Json to be)
                    loginDitails.Add("username", username_control.Text);
                    loginDitails.Add("password", password_control.Password);
                    loginDitails.Add("email", email_control.Text);
                    loginDitails.Add("address", address_str);
                    loginDitails.Add("phone", phone_number_control.Text);
                    loginDitails.Add("birthdate", "");
                    loginDitails["birthdate"] = birthdateControl.SelectedDate.Value.ToString("MM/dd/yyyy");

                    string json_parsed = JsonConvert.SerializeObject(loginDitails);
                    byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed);
                    byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(2, json_byted);

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
                        else // one of the fields does not meet the server requirements
                        {
                            infoBox.Content = "error! check the validty of the fields (username, email, etc..)";
                        }
                    }
                    catch (Exception ex) // runtime error (unidentified)
                    {
                        infoBox.Content = "there is an error.";
                    }
                }
                else // one of the fields is empty error
                {
                    infoBox.Content = "Error! One of the fields in empty!";
                }
            }
            else // the server is not connected
            {
                infoBox.Content = "error! client isn't connected to the server";
            }
        }
    }
}
