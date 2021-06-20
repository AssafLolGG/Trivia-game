#include "StatisticsManager.h"

StatisticsManager::StatisticsManager()
{
}

/*
function that returns a vector containing the highest score among the players.
input: None.
output: a vector contains the 5 highest score.
*/
StatisticsManager::StatisticsManager(IDatabase* database)
{
	this->m_database = database;
}

TopFivePlayers StatisticsManager::getHighScore()
{
	TopFivePlayers top_five;
	std::vector<std::string> highScoresVector;
	// gets the top 5 players by score in database.
	std::vector<statisticsDB> topStatistics = this->m_database->getTopFiveScore();
	std::vector<User> top_users = this->m_database->getTop5Players();
	// pushing to the vector the highest scores.
	try
	{
		for (int i = 0; i < topStatistics.size() && i < 5; i++)
		{
			top_five.top_score.push_back(topStatistics[i].highest_score);
			top_five.top_players.push_back(top_users[i].username);
		}
	}
	catch (...)
	{
	}
	return top_five;
}


/*
function that gets certain user's statistics
and pushing some of its qualities to a vector
then returning it.
input: the username.
output: some qualities in username pushed inside a vector.
Average time of the user.
Correct answers the user answered.
Total answers the user answered.
the total games the user played.
*/
std::vector<std::string> StatisticsManager::getUserStatistics(std::string username)
{
	std::vector<std::string> statisticsVector;
	int user_id = this->m_database->usernameToID(username);

	statisticsDB stats = this->m_database->getStatistics(user_id);

	// pushes the statistics stats to vector
	statisticsVector.push_back(std::to_string(user_id));
	statisticsVector.push_back(stats.games_played);
	statisticsVector.push_back(stats.right_answers);
	statisticsVector.push_back(stats.total_answers);
	statisticsVector.push_back(stats.likeability);
	statisticsVector.push_back(stats.potnetial);
	statisticsVector.push_back(stats.time_played);
	statisticsVector.push_back(stats.time_played_last_game);
	statisticsVector.push_back(stats.score_last_game);
	statisticsVector.push_back(stats.number_of_future_partners);
	statisticsVector.push_back(stats.questions_last_game);
	statisticsVector.push_back(stats.highest_score);

	return statisticsVector;
}
