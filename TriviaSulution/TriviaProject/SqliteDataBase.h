#pragma once
#include "IDatabase.h"
#include "..//..//sqlite-amalgamation-3350500/sqlite3.h"

class SqliteDataBase : public IDatabase
{
private:
	sqlite3* _db;

	static int callbackUser(void* data, int argc, char** argv, char** azColName);
	static int callbackPasswords(void* data, int argc, char** argv, char** azColName);
public:
	static bool isUserExisting;
	static bool isPasswordMatching;
	SqliteDataBase();
	~SqliteDataBase();
	bool doesUserExist(std::string username);
	bool doesPasswordMatch(std::string username, std::string password);
	void addNewUser(std::string username, std::string password, std::string mail);
};

