#pragma once
#include <iostream>
#include <vector>
#include "LoginManager.h"
struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
};

class Room
{
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
	
public:
	void addUser(LoggedUser userToAdd);
	void removeUSer(LoggedUser userToRemove);
	std::vector<std::string> getAllUsers();
};
