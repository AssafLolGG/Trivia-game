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
using System.Threading;
using System.Net.Sockets;
using Newtonsoft.Json;

namespace TriviaGUI
{
    public class playerResult
    {
        public string Username { get; set; }
        public int CorrectAnswersCount { get; set; }
        public int WrongAnswerCount { get; set; }
        public int averageAnswerTime { get; set; }
    }

    public class playersResult
    {
        public List<playerResult> results { get; set; }
        public playersResult()
        {
            results = new List<playerResult>();
        }
    }

    /// <summary>
    /// Interaction logic for TriviaGameRoom.xaml
    /// </summary>
    public partial class TriviaGameRoom : Window
    {
        private int time_out;
        private int questionsLeft;
        private System.Windows.Threading.DispatcherTimer _timer;
        private List<int> vecChoices;
        TimeSpan _time;
        public void ShowQuestionScreen()
        {
            this.Question_Text_TB.Visibility = Visibility.Visible;
            this.Ans_1_TB.Visibility = Visibility.Visible;
            this.Ans_2_TB.Visibility = Visibility.Visible;
            this.Ans_3_TB.Visibility = Visibility.Visible;
            this.Ans_4_TB.Visibility = Visibility.Visible;
        }
        public void HideQuestionScreen()
        {
            this.Question_Text_TB.Visibility = Visibility.Hidden;
            this.Ans_1_TB.Visibility = Visibility.Hidden;
            this.Ans_2_TB.Visibility = Visibility.Hidden;
            this.Ans_3_TB.Visibility = Visibility.Hidden;
            this.Ans_4_TB.Visibility = Visibility.Hidden;
        }
        public void WaitForTimeOut()
        {
            for (int i = 0; i < time_out; i++)
            {
                Thread.Sleep(1000);
                App.Current.Properties["TimeLeft"] = time_out - i;
            }
            if ((bool)App.Current.Properties["QuestionsShown"] == true)
            {
                App.Current.Properties["QuestionsShown"] = false;
                this.Dispatcher.Invoke(() => {
                    HideQuestionScreen();
                    }
                );
            }
            App.Current.Properties["timeOut"] = true;
        }
        public TriviaGameRoom()
        {
            InitializeComponent();
            this.vecChoices = new List<int>();
            object a = App.Current.Properties["numOfQuestions"];
            this.questionsLeft =int.Parse(App.Current.Properties["numOfQuestions"].ToString());
            this.time_out = int.Parse(App.Current.Properties["timeOutPerQuestion"].ToString());
            App.Current.Properties["TimeLeft"] = time_out;
            App.Current.Properties["timeOut"] = false;
            Thread screenSwitchThread = new Thread(new ThreadStart(QuestionsScreenSwitch));
            screenSwitchThread.Start();
            App.Current.Properties["screenChangingThread"] = screenSwitchThread;
        }

        private void QuestionsScreenSwitch()
        {
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] client_message = { 16 };
            byte[] server_message = { };
            Newtonsoft.Json.Linq.JObject json_returned;
            for (int i = 0; i < questionsLeft; i++)
            {
                App.Current.Properties["TimeLeft"] = time_out;
                serverConnection.GetStream().Write(client_message, 0, 1);

                while (serverConnection.Available == 0) ; // wait until a new message arrived from the server

                server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
                json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);

                string questionText = json_returned["question"].ToString();
                string ans1 = json_returned["answers"][0][1].ToString();
                string ans2 = json_returned["answers"][1][1].ToString();
                string ans3 = json_returned["answers"][2][1].ToString();
                string ans4 = json_returned["answers"][3][1].ToString();
                this.Dispatcher.Invoke(() =>
                {
                    this.Question_Text_TB.Text = questionText;
                    this.Ans_1_TB.Content = ans1;
                    this.Ans_2_TB.Content = ans2;
                    this.Ans_3_TB.Content = ans3;
                    this.Ans_4_TB.Content = ans4;
                    this.ShowQuestionScreen();
                    Waiting_TB.Visibility = Visibility.Hidden;
                }
                );
                App.Current.Properties["QuestionsShown"] = true;
                Thread threadTimeOut = new Thread(new ThreadStart(WaitForTimeOut));
                threadTimeOut.Start();

                // creating timer to show the user time left till time out
                _time = TimeSpan.FromSeconds((int)App.Current.Properties["TimeLeft"]);

                _timer = new System.Windows.Threading.DispatcherTimer(new TimeSpan(0, 0, 1), System.Windows.Threading.DispatcherPriority.Normal, delegate
                {
                    timer_TB.Text = "Seconds Left: " + _time.ToString("c") + "s";
                    if (_time == TimeSpan.Zero) _timer.Stop();
                    _time = _time.Add(TimeSpan.FromSeconds(-1));
                }, Application.Current.Dispatcher);

                _timer.Start();

                App.Current.Properties["ThreadTimeOut"] = threadTimeOut;
                while ((bool)App.Current.Properties["QuestionsShown"] == true) ;
                this.Dispatcher.Invoke(() =>
                {
                    HideQuestionScreen();
                    Waiting_TB.Visibility = Visibility.Visible;
                });

                while (_timer.IsEnabled == true) ;
                if (vecChoices.Count < i + 1)
                {
                    vecChoices.Add(1);
                }
                int choice = vecChoices[vecChoices.Count - 1];
                Dictionary<string, string> submitAnswer = new Dictionary<string, string>();
                submitAnswer["ANSWER_ID"] = choice.ToString();
                string json_parsed = JsonConvert.SerializeObject(submitAnswer);
                byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed);
                byte[] data_encoded = ServerFunctions.ServerFunctions.getCompleteMsg(15, json_byted);

                serverConnection.GetStream().Write(data_encoded, 0, 1000);
                while (serverConnection.Available == 0) ; // wait until a new message arrived from the server

                ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
            }

            this.Dispatcher.Invoke(() =>
            {
                Waiting_TB.Visibility = Visibility.Hidden;
            });
            serverConnection = (TcpClient)App.Current.Properties["server"];
            Array.Clear(client_message, 0, client_message.Length);
            client_message[0] = 14;
            serverConnection.GetStream().Write(client_message, 0, 1);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server

            server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
            json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);
            playersResult resultsInList = new playersResult();
            Newtonsoft.Json.Linq.JArray results = (Newtonsoft.Json.Linq.JArray)json_returned["results"];

            for (int i = 0; i < results.Count; i++)
            {
                Newtonsoft.Json.Linq.JToken currentResult = results[i];
                playerResult result = new playerResult();
                result.Username = currentResult["username"].ToString();
                result.WrongAnswerCount = int.Parse(currentResult["num_of_wrong_answers"].ToString());
                result.CorrectAnswersCount = int.Parse(currentResult["num_of_correct_answers"].ToString());
                result.averageAnswerTime = int.Parse(currentResult["average_answer_time"].ToString());
                resultsInList.results.Add(result);
            }

            foreach (playerResult p in resultsInList.results)
            {
                string currentResult = $"name: {p.Username}\r\ncorrect answers: {p.CorrectAnswersCount}\r\n";
                currentResult += $"wrong answers: {p.WrongAnswerCount}\r\nAverage time to answer: {p.averageAnswerTime}";
                this.Dispatcher.Invoke(() =>
                {
                    ListBoxItem item = new ListBoxItem();
                    item.Content = currentResult;
                    this.Results_list.Items.Add(item);
                });
            }
            this.Dispatcher.Invoke(() =>
            {
                this.Results_list.Visibility = Visibility.Visible;
            });
        }

        private void Ans_1_TB_Click(object sender, RoutedEventArgs e)
        {
            vecChoices.Add(1);
            App.Current.Properties["QuestionsShown"] = false;

        }

        private void Ans_2_TB_Click(object sender, RoutedEventArgs e)
        {
            vecChoices.Add(2);
            App.Current.Properties["QuestionsShown"] = false;
        }

        private void Ans_3_TB_Click(object sender, RoutedEventArgs e)
        {
            vecChoices.Add(3);
            App.Current.Properties["QuestionsShown"] = false;
        }

        private void Ans_4_TB_Click(object sender, RoutedEventArgs e)
        {
            vecChoices.Add(4);
            App.Current.Properties["QuestionsShown"] = false;
        }

        private void Exit_button_Click(object sender, RoutedEventArgs e)
        {
            ((Thread)App.Current.Properties["screenChangingThread"]).Abort();
            TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
            byte[] client_message = { 17 };
            byte[] server_message = { };
            serverConnection.GetStream().Write(client_message, 0, 1);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server

            server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);

            RoomMenu rmenu = new RoomMenu();
            rmenu.Show();
            this.Close();

        }
    }
}
