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
namespace TriviaGUI
{
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
            //App.Current.Properties["numOfQuestions"] = room_data["questionsNumber"].ToString();
            //App.Current.Properties["timeOutPerQuestion"] = room_data["timePerQuestion"].ToString();
            object a = App.Current.Properties["numOfQuestions"];
            this.questionsLeft =int.Parse(App.Current.Properties["numOfQuestions"].ToString());
            this.time_out = int.Parse(App.Current.Properties["timeOutPerQuestion"].ToString());
            App.Current.Properties["TimeLeft"] = time_out;
            App.Current.Properties["timeOut"] = false;
            Thread screenSwitchThread = new Thread(new ThreadStart(QuestionsScreenSwitch));
            screenSwitchThread.Start();
        }

        private void QuestionsScreenSwitch()
        {
            for (int i = 0; i < questionsLeft; i++)
            {
                App.Current.Properties["TimeLeft"] = time_out;

                TcpClient serverConnection = (TcpClient)App.Current.Properties["server"];
                byte[] client_message = { 16 };
                serverConnection.GetStream().Write(client_message, 0, 1);

                while (serverConnection.Available == 0) ; // wait until a new message arrived from the server

                byte[] server_message = ServerFunctions.ServerFunctions.ReadServerMessage(serverConnection);
                Newtonsoft.Json.Linq.JObject json_returned = ServerFunctions.ServerFunctions.diserallizeResponse(server_message);

                string questionText = json_returned["question"].ToString();
                string ans1 = json_returned["answers"][0][1].ToString();
                string ans2 = json_returned["answers"][1][1].ToString();
                string ans3 = json_returned["answers"][2][1].ToString();
                string ans4 = json_returned["answers"][3][1].ToString();
                this.Dispatcher.Invoke(() =>
                {
                    this.Question_Text_TB.Content = questionText;
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
            }
            this.Dispatcher.Invoke(() =>
            {
                this.End_TB.Visibility = Visibility.Visible;
                Waiting_TB.Visibility = Visibility.Hidden;
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
    }
}
