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

namespace TriviaGUI
{
    /// <summary>
    /// Interaction logic for TriviaGameRoom.xaml
    /// </summary>
    public partial class TriviaGameRoom : Window
    {
        private int time_out;
        public void ShowQuestionScreen()
        {
            this.Question_Text_TB.Visibility = Visibility.Visible;
            this.timer_TB.Visibility = Visibility.Visible;
            this.Ans_1_TB.Visibility = Visibility.Visible;
            this.Ans_2_TB.Visibility = Visibility.Visible;
            this.Ans_3_TB.Visibility = Visibility.Visible;
            this.Ans_4_TB.Visibility = Visibility.Visible;
        }
        public void HideQuestionScreen()
        {
            this.Question_Text_TB.Visibility = Visibility.Hidden;
            this.timer_TB.Visibility = Visibility.Hidden;
            this.Ans_1_TB.Visibility = Visibility.Hidden;
            this.Ans_2_TB.Visibility = Visibility.Hidden;
            this.Ans_3_TB.Visibility = Visibility.Hidden;
            this.Ans_4_TB.Visibility = Visibility.Hidden;
        }
        public void WaitForTimeOut()
        {
            Thread.Sleep(1000 * this.time_out);
            if ((bool)App.Current.Properties["QuestionsShown"] == true)
            {
                App.Current.Properties["QuestionsShown"] = false;
                this.Dispatcher.Invoke(() => {
                    HideQuestionScreen();
                    }
                );
            }
        }
        public TriviaGameRoom()
        {
            InitializeComponent();
            this.time_out = 13;
            int questionsLeft = 1;
            for (int i = 0; i < questionsLeft; i++)
            {
                App.Current.Properties["QuestionsShown"] = true;
                Thread t = new Thread(new ThreadStart(WaitForTimeOut));
                t.Start();
            }
        }

        private void Ans_1_TB_Click(object sender, RoutedEventArgs e)
        {
            App.Current.Properties["QuestionsShown"] = false;
            HideQuestionScreen();
        }

        private void Ans_2_TB_Click(object sender, RoutedEventArgs e)
        {
            App.Current.Properties["QuestionsShown"] = false;
            HideQuestionScreen();
        }

        private void Ans_3_TB_Click(object sender, RoutedEventArgs e)
        {
            App.Current.Properties["QuestionsShown"] = false;
            HideQuestionScreen();
        }

        private void Ans_4_TB_Click(object sender, RoutedEventArgs e)
        {
            App.Current.Properties["QuestionsShown"] = false;
            HideQuestionScreen();
        }
    }
}
