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
        /// <summary>
        /// checks if the text contains something that is not numbers
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }
        public CreateMenu()
        {
            InitializeComponent();
        }

        private void back_to_menu_Click(object sender, RoutedEventArgs e)
        {
            RoomMenu menu = new RoomMenu();
            menu.Show();

            this.Close();
        }

        /// <summary>
        /// Creating new room
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void create_room_Click(object sender, RoutedEventArgs e)
        {
            TcpClient server_connection = (TcpClient)App.Current.Properties["server"];
            Dictionary<string, object> romm_to_be_created = new Dictionary<string, object>();
            if(this.roomName_TB.Text != "" && this.MaximumUsers_TB.Text != "" && this.QuestionCount_TB.Text != "" && this.AnswerTimeOut_TB.Text != "") // checks to see if any of the field is empty
            {
                // adding room data to "to be" json
                romm_to_be_created.Add("roomName", this.roomName_TB.Text);
                romm_to_be_created.Add("maximumUsers", this.MaximumUsers_TB.Text);
                romm_to_be_created.Add("questionCount", this.QuestionCount_TB.Text);
                romm_to_be_created.Add("timeToAnswer", this.AnswerTimeOut_TB.Text);

                string json_parsed = JsonConvert.SerializeObject(romm_to_be_created); // serializing data
                byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed); // encoding json
                byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(7, json_byted); // adding size and code for protocal
                server_connection.GetStream().Write(data_encoded, 0, 1000);

                while (server_connection.Available == 0) ; // wait until a new message arrived from the server
                byte[] server_output = ServerFunctions.ServerFunctions.ReadServerMessage(server_connection);
                Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.ServerFunctions.diserallizeResponse(server_output); // diserallizing json from server

                try
                {
                    if (jsonReturned["status"].ToString() == "1") // checks if the room was created.
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
            else
            {
                this.IsCreated_TB.Text = "Some Fields Are missing!";
            }
        }
    }
}