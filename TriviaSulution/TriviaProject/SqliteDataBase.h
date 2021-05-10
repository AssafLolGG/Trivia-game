#pragma once
#include "IDatabase.h"
#include "..//..//sqlite-amalgamation-3350500/sqlite3.h"

struct statisticsDB
{
	int player_id;
	int games_played;
	int right_answers;
	int likeability;
	int potnetial;
	int highest_score;
	int time_played;
	int time_played_last_game;
	int score_last_game;
	int number_of_future_partners;
	int questions_last_game;
};

class SqliteDataBase : public IDatabase
{
private:
	sqlite3* _db;

	static int callbackUser(void* data, int argc, char** argv, char** azColName);
	static int callbackPasswords(void* data, int argc, char** argv, char** azColName);
	static int callbackStatistics(void* data, int argc, char** argv, char** azColName);
public:
	static bool isUserExisting;
	static bool isPasswordMatching;
	SqliteDataBase();
	~SqliteDataBase();
	bool doesUserExist(std::string username);
	bool doesPasswordMatch(std::string username, std::string password);
	void addNewUser(std::string username, std::string password, std::string mail);
	int getPlayerAVerageAnswerTime(int user_id);
	int getNumOfCorrectAnswers(int user_id);
	int getNumOfTotalAnswers(int user_id);
};

