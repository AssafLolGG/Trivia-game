#pragma once
#include "IDatabase.h"
#include "ResponseToSerializer.h"
#include <vector>

class StatisticsManager
{
private:
	IDatabase* m_database;
public:
	StatisticsManager();
	StatisticsManager(IDatabase* database);
	TopFivePlayers getHighScore();
	std::vector<std::string> getUserStatistics(std::string username);
};

