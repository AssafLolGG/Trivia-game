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
	std::vector<Game> getGames() const;
	Game createGame(Room roomInGame);
	void deleteGame(Game gameToRemove);
};

struct GameData
{
	bool isThereQuestions;
	unsigned int currentQuestionID;
	Question currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

class Game
{
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;

public:
	Game(std::vector<string> users, std::vector<Question> questions);
	bool operator==(const Game& other) const;
	void getQuestionForUser(LoggedUser User);
	void submitAnswer(LoggedUser User, string answer);
	void removePlayer(LoggedUser User);
};

