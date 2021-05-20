#pragma once

#include "IDatabase.h"
#include <vector>
#include <regex>

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
	bool signup(std::string username, std::string password, std::string email, std::string address, std::string phone, std::string birthday);
	bool login(std::string username, std::string password);
	void logout(std::string username);
};

