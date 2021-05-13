#include "StatisticsManager.h"

std::vector<std::string> StatisticsManager::getHighScore()
{
	std::vector<std::string> highScoresVector;
	std::vector<statisticsDB> topStatistics = this->m_database->getTop5Players();
	try
	{
		highScoresVector.push_back("1st place: " + std::to_string(topStatistics[0].highest_score));
		highScoresVector.push_back("2nd place: " + std::to_string(topStatistics[1].highest_score));
		highScoresVector.push_back("3rd place: " + std::to_string(topStatistics[2].highest_score));
		highScoresVector.push_back("4th place: " + std::to_string(topStatistics[3].highest_score));
		highScoresVector.push_back("5th place: " + std::to_string(topStatistics[4].highest_score));
	}
	catch (...)
	{
	}
	return highScoresVector;
}

std::vector<std::string> StatisticsManager::getUserStatistics(std::string username)
{
	std::vector<std::string> statisticsVector;
	int user_id = this->m_database->usernameToID(username);
	statisticsVector.push_back("Average time: " + std::to_string(this->m_database->getPlayerAverageAnswerTime(user_id)));
	statisticsVector.push_back("Correct answers: " + std::to_string(this->m_database->getNumOfCorrectAnswers(user_id)));
	statisticsVector.push_back("Total answers: " + std::to_string(this->m_database->getNumOfTotalAnswers(user_id)));
	statisticsVector.push_back("Games played: " + std::to_string(this->m_database->getNumOfPlayerGames(user_id)));
	return statisticsVector;
}
