#include "GameManager.h"

Game::Game(std::vector<string> users, std::vector<Question> questions)
{
	this->m_questions = questions;

	for (string user : users)
	{
		GameData gData = GameData();

		gData.averageAnswerTime = 0;
		gData.correctAnswerCount = 0;
		gData.currentQuestionID = 0;
		gData.currentQuestion = this->m_questions[0];
		gData.isThereQuestions = true;
		gData.wrongAnswerCount = 0;

		this->m_players.insert(std::pair<LoggedUser, GameData>(LoggedUser(user), gData));
	}
}

bool Game::operator==(const Game& other) const
{
	if (this != &other)
	{
		return this->m_players == other.m_players;
	}
	else
	{
		return true;
	}
}

void Game::getQuestionForUser(LoggedUser User)
{
	auto userIter = this->m_players.end();
	for (auto iter = this->m_players.begin(); iter != this->m_players.end(); iter++)
	{
		if (User.getUserName() == iter->first.getUserName())
		{
			userIter = iter;
		}
	}

	if (userIter != this->m_players.end())
	{
		if (userIter->second.currentQuestionID + 1 < this->m_questions.size())
		{
			userIter->second.currentQuestionID++;
			userIter->second.currentQuestion = this->m_questions[userIter->second.currentQuestionID];
		}
		else
		{
			userIter->second.isThereQuestions = false;
		}
	}
}

void Game::submitAnswer(LoggedUser User, string answer)
{
	auto userIter = this->m_players.end();
	for (auto iter = this->m_players.begin(); iter != this->m_players.end(); iter++)
	{
		if (User.getUserName() == iter->first.getUserName())
		{
			userIter = iter;
		}
	}

	if (userIter != this->m_players.end())
	{
		if (userIter->second.currentQuestion.getCorrectAnswer() == answer)
		{
			userIter->second.correctAnswerCount++;
		}
		else
		{
			userIter->second.wrongAnswerCount++;
		}
	}
}

void Game::removePlayer(LoggedUser User)
{
	for (auto iter = this->m_players.begin(); iter != this->m_players.end(); iter++)
	{
		if (User.getUserName() == iter->first.getUserName())
		{
			this->m_players.erase(iter);
			return;
		}
	}
}

std::vector<Game> GameManager::getGames() const
{
	return this->m_games;
}

Game GameManager::createGame(Room roomInGame)
{
	this->m_games.push_back(Game(roomInGame.getAllUsers(), this->m_database->getQuestions(roomInGame.GetRoomdata().numOfQuestionsInGame)));
}

void GameManager::deleteGame(Game gameToRemove)
{
	for (auto iter = this->m_games.begin(); iter != this->m_games.end(); iter++)
	{
		if (gameToRemove == *iter)
		{
			this->m_games.erase(iter);
			return;
		}
	}
}