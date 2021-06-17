#pragma once

#include "IDatabase.h"
#include <vector>
#include <regex>

#define PASSWORD_REGEX "[a-zA-Z0-9!@#$%^&*]{8,}"
#define EMAIL_REGEX "[a-zA-Z0-9]{1,}@[a-zA-Z0-9]{1,}(.com$|.co.il$|.cyber.org.il$)"
#define STREET_REGEX "^[a-zA-Z ]{0,}[a-zA-Z]{1,}$"
#define APT_REGEX "^[0-9]{1,}"
#define CITY_REGEX "^[a-zA-Z ]{0,}[a-zA-Z]{1,}$"
#define PHONE_REGEX "^0(2|3|4|5[2345])-[0-9]{6,7}$"
#define BIRTHDATE_REGEX "^([0-9]{2}.[0-9]{2}.[0-9]{4})|([0-9]{2}/[0-9]{2}/[0-9]{4})$"

#define STREET_INDEX 0
#define APT_INDEX 1
#define CITY_INDEX 2

class LoggedUser
{
private:
	std::string _username;
public:
	LoggedUser();
	LoggedUser(std::string username);
	std::string getUserName() const;
	bool operator<(const LoggedUser& other) const;
};

class LoginManager
{
private:
	IDatabase* _db_access;
	std::vector<LoggedUser> _loggedUsers;

	bool isUserActive(std::string username);
public:
	LoginManager(IDatabase* db);
	bool signup(std::string username, std::string password, std::string email, std::string address, std::string phone, std::string birthday);
	bool login(std::string username, std::string password);
	void logout(std::string username);
};

