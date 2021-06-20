#pragma once
#include "IDatabase.h"
#include <map>
#include "LoginManager.h"
#include "Room.h"

class GameManager;
class Game;
struct GameData;

class GameManager
{
private:
	IDatabase* m_database;
	std::vector<Game> m_games;
public:
	GameManager(IDatabase* db);
	std::vector<Game> getGames() const;
	Game& getGame(LoggedUser);
	Game& createGame(Room roomInGame);
	void deleteGame(Game gameToRemove);
	void updateStatistics(LoggedUser user);

};

struct GameData
{
	bool isThereQuestions;
	unsigned int currentQuestionID;
	Question currentQuestion;
	int correctAnswerCount;
	int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;
	unsigned int m_timeOut;
public:
	Game(std::vector<string> users, std::vector<Question> questions, unsigned int gameTimeOut);
	bool operator==(const Game& other) const;
	void getQuestionForUser(LoggedUser User);
	bool submitAnswer(LoggedUser User, string answer);
	void removePlayer(LoggedUser User);
	statisticsDB getUserNewStatistics(LoggedUser user, statisticsDB currentStatistics);
	bool isUserInGame(LoggedUser);
	GameData getUserData(LoggedUser user);
	std::map<LoggedUser, GameData> getPlayers();
	bool checkIfFinished();
};

