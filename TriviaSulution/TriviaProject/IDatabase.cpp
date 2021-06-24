#include "IDatabase.h"

/*
function that gets number of question out of total questions.
input: the number of the questions(if this number is larger
than the size of all the questions -> it sends all the questions.)
output: list of questions that is in the size of num_of_questions or the 
all questions, the lower among them.
*/
std::vector<Question> IDatabase::getQuestions(int num_of_questions)
{
	std::vector<Question> questions_vec;
	std::vector<Question> all_questions_vec = this->getAllQuestions();

	// gets the amount of questions the caller requested
	for (int i = 0; i < all_questions_vec.size() && i < num_of_questions; i++)
	{
		questions_vec.push_back(all_questions_vec[i]);
	}

	// returns the list of questions.
	return questions_vec;
}

/*
function that gets the correct answer of the question.
input: None.
output: the right answer of the question.
*/
string Question::getCorrectAnswer()
{
	// passing on the answers
	for (Answer ans : this->answers)
	{
		// checks if the answer is correct, than if so, returns it.
		if (ans.isCorrect)
		{
			return ans.answerText;
		}
	}
	return "";
}
