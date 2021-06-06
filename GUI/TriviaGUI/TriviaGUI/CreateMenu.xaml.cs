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
using System.Text.RegularExpressions;
using System.Net.Sockets;
using System.Threading;
using Newtonsoft.Json;
using ServerFunctions;

namespace TriviaGUI
{
    /// <summary>
    /// Interaction logic for CreateMenu.xaml
    /// </summary>
    public partial class CreateMenu : Window
    {
        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);

            ((Thread)App.Current.Properties["ThreadOfSound"]).Abort();
            ((Thread)App.Current.Properties["ThreadOfConnecting"]).Abort();
            App.Current.Shutdown();
            Environment.Exit(0);
            this.Close();
        }
        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }
        public CreateMenu()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            RoomMenu menu = new RoomMenu();
            menu.Show();
            this.Close();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            Dictionary<string, object> RoomToBeCreatedDitalis = new Dictionary<string, object>();
            RoomToBeCreatedDitalis.Add("roomName", this.roomName_TB.Text);
            RoomToBeCreatedDitalis.Add("maximumUsers", this.MaximumUsers_TB.Text);
            RoomToBeCreatedDitalis.Add("questionCount", this.QuestionCount_TB.Text);
            RoomToBeCreatedDitalis.Add("timeToAnswer", this.AnswerTimeOut_TB.Text);
            string json_parsed = JsonConvert.SerializeObject(RoomToBeCreatedDitalis); // serializing data
            byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed); // encoding json
            byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(7, json_byted); // adding size and code for protocal
            serverConnection.GetStream().Write(data_encoded, 0, 1000);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
            byte[] serverOutput = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
            Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.ServerFunctions.diserallizeResponse(serverOutput); // diserallizing json from server

            try
            {
                if (jsonReturned["status"].ToString() == "1")
                {
                    this.IsCreated_TB.Text = "The Room was successfully created.";
                    App.Current.Properties["isInRoom"] = true;
                    RoomAdminWindow admin = new RoomAdminWindow(Int32.Parse(jsonReturned["id"].ToString()), this.roomName_TB.Text,
                        Int32.Parse(this.MaximumUsers_TB.Text), Int32.Parse(this.QuestionCount_TB.Text), Int32.Parse(this.AnswerTimeOut_TB.Text));
                    admin.Show();

                    this.Close();
                }
                else
                {
                    this.IsCreated_TB.Text = "The Room wasn't created.";
                }
            }
            catch (Exception ex)
            {
                this.IsCreated_TB.Text = "The Room wasn't created.";
            }
        }
    }
}