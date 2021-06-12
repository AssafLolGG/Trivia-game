#include "IDatabase.h"

std::vector<Question> IDatabase::getQuestions(int num_of_questions)
{
	std::vector<Question> questions_vec;
	std::vector<Question> all_questions_vec = this->getAllQuestions();
	for (int i = 0; i < all_questions_vec.size() && i < num_of_questions; i++)
	{
		questions_vec.push_back(all_questions_vec[i]);
	}
	return questions_vec;
}

string Question::getCorrectAnswer()
{
	for (Answer ans : this->answers)
	{
		if (ans.isCorrect)
		{
			return ans.answerText;
		}
	}
	return "";
}
