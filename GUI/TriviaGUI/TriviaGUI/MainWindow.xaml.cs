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
using System.Media;

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

            SoundPlayer player = new SoundPlayer("C:\\Users\\Leon\\triviaprojectmagshimim\\music\\bMusic.wav");
            player.Load();
            player.Play();
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
                Random rand = new Random();
                List<string> disappointing_sentenses = new List<string>();
                disappointing_sentenses.Add("our highly advanced mechanism has determined\nthat you are a bot,\nplease unbot yourself and try again!");
                disappointing_sentenses.Add("unfortunately you are a bot :<\n(press F to pay respects)");
                disappointing_sentenses.Add("Im sry this is for humans only\nsince you are a bot, get away.");
                disappointing_sentenses.Add("Damn, in a second thought you do\nlook like a bot.");
                disappointing_sentenses.Add("Beep Boop Bap, how tf do i say to\nbot fuck off??");
                disappointing_sentenses.Add("Roses are red, violets are blue, and\nwell you are a bot :<.");
                disappointing_sentenses.Add("Idk how to say it to you, but you are a\nbot.");
                disappointing_sentenses.Add("Imagine being a bot.");
                disappointing_sentenses.Add("Tell me Robots joke since you are bot.");

                MessageBox.Show("Calculating if you are a bot or not..");
                if (rand.Next(1, 6) == 3)
                {
                    Thread.Sleep(1000);
                    MessageBox.Show("GG you aren't a bot! (at least not yet)");
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
                    int nowIndex = 0;
                    user_name_text_box.Text = "";
                    password_text_box.Password = "";
                    if (App.Current.Properties["oneBefore"] == null)
                    {
                        nowIndex = rand.Next(0, disappointing_sentenses.Count);
                        App.Current.Properties["oneBefore"] = nowIndex;
                        Error_label.Content = disappointing_sentenses[nowIndex];
                    }
                    else
                    {
                        do
                        {
                            nowIndex = rand.Next(0, disappointing_sentenses.Count);
                        } while (nowIndex == (int)App.Current.Properties["oneBefore"]);
                        App.Current.Properties["oneBefore"] = nowIndex;
                        Error_label.Content = disappointing_sentenses[nowIndex];
                    }
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
