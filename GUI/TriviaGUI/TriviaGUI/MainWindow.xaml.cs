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
        string username = "user";
        string password = "password";
        //int bytes_written = 0;

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
        private void ConnectingToServer()
        {
            if (App.Current.Properties["server"] == null)
            {
                while (true)
                {
                    try
                    {
                        App.Current.Properties["server"] = new TcpClient("127.0.0.1", 9999);
                        string t = "hello";
                        ((TcpClient)App.Current.Properties["server"]).GetStream().Write(System.Text.Encoding.ASCII.GetBytes(t), 0, System.Text.Encoding.ASCII.GetBytes(t).Length);
                        //bytes_written = 5;
                        byte[] b = new byte[10];
                        ((TcpClient)App.Current.Properties["server"]).GetStream().Read(b, 0, 5);
                        return;
                    }
                    catch (Exception e)
                    {

                    }
                }
            }
        }
        public MainWindow()
        {
            InitializeComponent();
            Thread connectThread = new Thread(new ThreadStart(ConnectingToServer));
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
                byte[] data_encoded = this.getCompleteMsg(1, json_byted);

                serverConnection.GetStream().Write(data_encoded, 0, data_encoded.Length);
                System.Threading.Thread.Sleep(100);

                byte[] serverOutput = new byte[1024];
                serverConnection.GetStream().Read(serverOutput, 0, 1000);
                Newtonsoft.Json.Linq.JObject dis = this.diserallizeResponse(serverOutput);

                if (dis.First.First.ToString() == "1")
                {
                    Error_label.Content = "You logged in successfully!";
                }
                else
                {
                    Error_label.Content = "error! username or password are incorrect";
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
