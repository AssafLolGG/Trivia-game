#pragma once

#include "IDatabase.h"
#include <vector>

class LoggedUser
{
private:
	std::string _username;
public:
	LoggedUser();
	LoggedUser(std::string username);
	std::string getUserName() const;
};

class LoginManager
{
private:
	IDatabase* _db_access;
	std::vector<LoggedUser> _loggedUsers;
public:
	LoginManager(IDatabase* db);
	void signup(std::string username, std::string password, std::string email);
	bool login(std::string username, std::string password);
	void logout(std::string username);
};

