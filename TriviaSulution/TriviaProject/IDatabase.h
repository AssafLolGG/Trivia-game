#pragma once
#include <string>
#include <vector>

using std::string;

struct Answer
{
	bool isCorrect;
	std::string answerText;
};
struct Question
{
	int id;
	std::vector<Answer> answers;
	std::string questionText;
};

struct User
{
	std::string username;
	std::string password;
	std::string mail;
	int id;
};

struct statisticsDB
{
	string player_id;
	string games_played;
	string right_answers;
	string total_answers;
	string likeability;
	string potnetial;
	string highest_score;
	string time_played;
	string time_played_last_game;
	string score_last_game;
	string number_of_future_partners;
	string questions_last_game;
};
class IDatabase
{
public:
	virtual bool doesUserExist(std::string username) = 0;
	virtual bool doesPasswordMatch(std::string username, std::string password) = 0;
	virtual void addNewUser(std::string username, std::string password, std::string mail, std::string address, std::string phone, std::string birthdate) = 0;
	virtual int getPlayerAverageAnswerTime(int user_id) = 0;
	virtual int getNumOfCorrectAnswers(int user_id) = 0;
	virtual int getNumOfTotalAnswers(int user_id) = 0;
	virtual int getNumOfPlayerGames(int user_id) = 0;
	virtual int usernameToID(std::string username) = 0;
	virtual statisticsDB getStatistics(int user_id) = 0;
	virtual std::vector<Question> getQuestions() = 0;
	virtual std::vector<User> getTop5Players() = 0;
	virtual std::vector<statisticsDB> getTopFiveScore() = 0;
};

