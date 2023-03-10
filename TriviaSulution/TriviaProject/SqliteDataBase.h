#pragma once
#include "IDatabase.h"
#include "..//..//sqlite-amalgamation-3350500/sqlite3.h"
#include <vector>
#include <iostream>


class SqliteDataBase : public IDatabase
{
private:
	sqlite3* _db;

	static int callbackStatistics(void* data, int argc, char** argv, char** azColName);
	static int callbackUser(void* data, int argc, char** argv, char** azColName);
	static int callbackPasswords(void* data, int argc, char** argv, char** azColName);
	static int callbackQuestionsAndAnswers(void* data, int argc, char** argv, char** azColName);

	void clearStatisticsDB(statisticsDB stats);
public:
	static bool isUserExisting;
	static bool isPasswordMatching;
	SqliteDataBase();
	~SqliteDataBase();
	bool doesUserExist(std::string username);
	bool doesPasswordMatch(std::string username, std::string password);
	void addNewUser(std::string username, std::string password, std::string mail, std::string address, std::string phone, std::string birthdate);

	int getPlayerAverageAnswerTime(int user_id);
	int getNumOfCorrectAnswers(int user_id);
	int getNumOfTotalAnswers(int user_id);
	int getNumOfPlayerGames(int user_id);
	
	int usernameToID(std::string username);
	std::vector<Question> getAllQuestions();
	std::vector<User> getTop5Players();
	std::vector<statisticsDB> getTopFiveScore();
	statisticsDB getStatistics(int user_id);
	void updateStatistics(statisticsDB newStatistics);
};

