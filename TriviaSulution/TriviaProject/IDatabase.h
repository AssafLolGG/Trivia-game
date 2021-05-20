#pragma once
#include <string>
#include <vector>

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
	int player_id;
	int games_played;
	int right_answers;
	int total_answers;
	int likeability;
	int potnetial;
	int highest_score;
	int time_played;
	int time_played_last_game;
	int score_last_game;
	int number_of_future_partners;
	int questions_last_game;
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
	virtual std::vector<Question> getQuestions() = 0;
	virtual std::vector<statisticsDB> getTop5Players() = 0;
	
};

