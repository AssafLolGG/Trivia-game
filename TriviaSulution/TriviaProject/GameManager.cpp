#include "GameManager.h"

Game::Game(std::vector<string> users, std::vector<Question> questions, unsigned int gameTimeOut)
{
	this->m_questions = questions;
	this->m_timeOut = gameTimeOut;
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
		for (auto iter_this = this->m_players.begin(), iter_other = other.m_players.begin(); iter_this !=
			this->m_players.end() && iter_other != this->m_players.end(); iter_this++, iter_other++)
		{
			if (iter_this->first.getUserName() != iter_other->first.getUserName())
			{
				return false;
			}
		}
		return true;
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
		if ((userIter->second.currentQuestionID + 1) < this->m_questions.size())
		{
			userIter->second.currentQuestionID++;
			userIter->second.currentQuestion = this->m_questions[userIter->second.currentQuestionID];
		}
	}
}

bool Game::submitAnswer(LoggedUser User, string answer)
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
		if ((userIter->second.currentQuestionID + 1) >= this->m_questions.size())
		{
			userIter->second.isThereQuestions = false;
		}
		return true;
	}
	return false;
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

statisticsDB Game::getUserNewStatistics(LoggedUser user, statisticsDB currentStatistics)
{
	for (auto iter = this->m_players.begin(); iter != this->m_players.end(); iter++)
	{
		// gets if it's this iterator contain the user data
		if (user.getUserName() == iter->first.getUserName())
		{
			// adds the stats from last game, to the current user's stats
			currentStatistics.games_played = std::to_string(std::stoi(currentStatistics.games_played) + 1);
			currentStatistics.questions_last_game = std::to_string(this->m_questions.size());
			currentStatistics.right_answers = std::to_string(std::stoi(currentStatistics.right_answers) + iter->second.correctAnswerCount);
			currentStatistics.score_last_game = std::to_string(iter->second.correctAnswerCount - iter->second.wrongAnswerCount);
			
			// checks if the score last game is higher than the highest score, and if so, replace him with the score from last game
			if (std::stoi(currentStatistics.score_last_game) > std::stoi(currentStatistics.highest_score))
			{
				currentStatistics.highest_score = currentStatistics.score_last_game;
			}
			
			// adds more stats to the new updated stats
			currentStatistics.time_played_last_game = std::to_string(this->m_timeOut);
			currentStatistics.time_played = std::to_string(std::stoi(currentStatistics.time_played) + this->m_timeOut);
			currentStatistics.total_answers = std::to_string(std::stoi(currentStatistics.total_answers) + this->m_questions.size());
			
			// returned the updated statistics
			return currentStatistics;
		}
	}
}

bool Game::isUserInGame(LoggedUser user)
{
	// searches if the user is in the game or not in the game's player list.
	for (auto iter = this->m_players.begin(); iter != this->m_players.end(); iter++)
	{
		if (iter->first.getUserName() == user.getUserName())
		{
			return true;
		}
	}
	return false;
}

GameData Game::getUserData(LoggedUser user)
{
	// gets the current user's gamedata, which contains data related to his game performance
	for (auto iter = this->m_players.begin(); iter != this->m_players.end(); iter++)
	{
		if (iter->first.getUserName() == user.getUserName())
		{
			return iter->second;
		}
	}
	return GameData();
}

/*
returns a map containing all the players and their gamedata.
input: None.
output: the map of users and their game stats.
*/
std::map<LoggedUser, GameData> Game::getPlayers()
{
	return this->m_players;
}

bool Game::checkIfFinished()
{
	// check if every user finished his list of question
	for (auto iter = this->m_players.begin(); iter != this->m_players.end(); iter++)
	{
		if (iter->second.isThereQuestions)
		{
			return false;
		}
	}
	return true;
}



GameManager::GameManager(IDatabase* db)
{
	this->m_database = db;
}

std::vector<Game> GameManager::getGames() const
{
	return this->m_games;
}

Game& GameManager::getGame(LoggedUser user)
{
	for (Game& game : this->m_games)
	{
		if (game.isUserInGame(user))
		{
			return game;
		}
	}
}



Game& GameManager::createGame(Room roomInGame)
{
	this->m_games.push_back(Game(roomInGame.getAllUsers(), this->m_database->getQuestions(roomInGame.GetRoomdata().numOfQuestionsInGame), roomInGame.GetRoomdata().timePerQuestion));
	return this->m_games.back();
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

void GameManager::updateStatistics(LoggedUser user)
{
	Game& userGame = this->getGame(user);
	this->m_database->updateStatistics(userGame.getUserNewStatistics(user, this->m_database->getStatistics(this->m_database->usernameToID(user.getUserName()))));
}
