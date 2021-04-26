#pragma once
#include "IDatabase.h"

class SqliteDataBase : public IDatabase
{
public:
	virtual bool doesUserExist(std::string username);
	virtual bool doesPasswordMatch(std::string username, std::string password);
	virtual void addNewUser(std::string username, std::string password, std::string mail);
};

