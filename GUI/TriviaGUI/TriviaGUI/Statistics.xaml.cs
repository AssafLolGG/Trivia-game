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
using Newtonsoft.Json;

namespace TriviaGUI
{
    /// <summary>
    /// Interaction logic for Statistics.xaml
    /// </summary>
    public partial class Statistics : Window
    {
        private void getPersonalStatistics()
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] client_message = { 8 };

            serverConnection.GetStream().Write(client_message, 0, 1);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
            byte[] personal_statistics_json = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server
            Newtonsoft.Json.Linq.JObject server_json = ServerFunctions.ServerFunctions.diserallizeResponse(personal_statistics_json);

            string[] personal_statistics = server_json["user_statistics"].ToString().Replace("\"", "").Replace("]", "").Replace("[","").Replace("\n", "").Replace(", ",",").Split(',');

            // writing personal statistics by the order they were received
            this.player_id_text.Text = personal_statistics[0];
            this.games_played_text.Text = personal_statistics[1];
            this.right_answers_text.Text = personal_statistics[2];
            this.total_answers_text.Text = personal_statistics[3];
            this.likeability_text.Text = personal_statistics[4];
            this.potential_text.Text = personal_statistics[5];
            this.time_played_text.Text = personal_statistics[6];
            this.time_played_last_game_text.Text = personal_statistics[7];
            this.score_last_game_text.Text = personal_statistics[8];
            this.number_of_future_partners_text.Text = personal_statistics[9];
            this.questions_last_game_text.Text = personal_statistics[10];
            this.player_score_text.Text = " Your highest score: " + personal_statistics[11].Replace("\r", "").Replace(" ", "");
        }

        /// <summary>
        /// Getting top five players with the highest score
        /// </summary>
        private void getTopFive()
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] client_message = { 10 };

            serverConnection.GetStream().Write(client_message, 0, 1);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
            byte[] personal_statistics_json = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection); // reading json from server
            Newtonsoft.Json.Linq.JObject server_json = ServerFunctions.ServerFunctions.diserallizeResponse(personal_statistics_json);

            // splitting the players and score json to an array of strings containing one score / player each index
            string[] top_users = server_json["players"].ToString().Replace(", ", ",").Split(',');
            string[] top_score = server_json["score"].ToString().Replace(", ", ",").Split(',');

            string connected_top = "";

            // writing the players and the score to a string
            for(int i = 0; i < top_users.Length; i++)
            {
                connected_top += top_users[i] + " - " + top_score[i] + "\n";
            }
            connected_top = connected_top.Substring(0, connected_top.Length - 1); // removing the finale '\n'

            this.top_text.Text = connected_top; // writing the string of players and score to the textBox
        }

        public Statistics()
        {
            InitializeComponent();
            this.getTopFive();
            this.getPersonalStatistics();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            MainMenu menu = new MainMenu();
            menu.Show();
            this.Close();
        }
    }
}
