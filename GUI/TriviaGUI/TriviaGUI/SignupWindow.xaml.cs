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
namespace TriviaGUI
{

    /// <summary>
    /// Interaction logic for SignupScreen.xaml
    /// </summary>
    public partial class SignupScreen : Window
    {
        private byte[] getCompleteMsg(byte msgCode, byte[] jsonBytes)
        {
            byte[] data_encoded = new byte[1024];
            int size = jsonBytes.Length;

            data_encoded[0] = msgCode;
            data_encoded[4] = (byte)(size % 256);
            size /= 256;
            data_encoded[3] = (byte)(size % 256);
            size /= 256;
            data_encoded[2] = (byte)(size % 256);
            size /= 256;
            data_encoded[1] = (byte)(size % 256);
            size /= 256;

            for (int i = 0; i < jsonBytes.Length; i++)
            {
                data_encoded[i + 5] = jsonBytes[i];
            }
            return data_encoded;
        }

        Newtonsoft.Json.Linq.JObject diserallizeResponse(byte[] response)
        {
            byte[] dataDecoded = new byte[1024];
            for (int i = 0; i < dataDecoded.Length; i++)
            {
                dataDecoded[i] = response[i + 0];
            }


            int msgSize = 0;
            msgSize = dataDecoded[4];
            msgSize += dataDecoded[3] * 256;
            msgSize += dataDecoded[2] * 256 * 256;
            msgSize += dataDecoded[1] * 256 * 256 * 256;

            for (int i = 0; i < dataDecoded.Length; i++)
            {
                dataDecoded[i] = 0;
            }
            for (int i = 0; i < msgSize; i++)
            {
                dataDecoded[i] = response[i + 5];
            }
            string jsonObjectInString = System.Text.Encoding.ASCII.GetString(dataDecoded);
            return (Newtonsoft.Json.Linq.JObject)JsonConvert.DeserializeObject(jsonObjectInString);
        }
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
                byte[] data_encoded = this.getCompleteMsg(2, json_byted);

                serverConnection.GetStream().Write(data_encoded, 0, data_encoded.Length);
                System.Threading.Thread.Sleep(100);

                byte[] serverOutput = new byte[1024];
                serverConnection.GetStream().Read(serverOutput, 0, 1000);
                Newtonsoft.Json.Linq.JObject dis = this.diserallizeResponse(serverOutput);
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
