#include "SqliteDataBase.h"

int SqliteDataBase::callbackUser(void* data, int argc, char** argv, char** azColName)
{
	isUserExisting = false;
	std::string* userToFind = (std::string*)data;
	for (int i = 0; i < argc; i++) 
	{
		if (std::string(azColName[i]) == "UserName") 
		{
			// checks if user exists in the db
			//(std::string)(argv[i]) == "" ? SqliteDataBase::isUserExisting = false : SqliteDataBase::isUserExisting = true;
		}
	}
	return 0;
}

int SqliteDataBase::callbackPasswords(void* data, int argc, char** argv, char** azColName)
{
	isUserExisting = false;
	std::string* userToFind = (std::string*)data;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "UserName")
		{
			//(std::string)(argv[i]) == "" ? SqliteDataBase::isUserExisting = false : SqliteDataBase::isPasswordMatching = true;
		}
	}
	return 0;
}

SqliteDataBase::SqliteDataBase()
{
	std::string dbFileName = "..//..//dataBase//triviaDB.sqlite";
	sqlite3_open(dbFileName.c_str(), &this->_db);
}

SqliteDataBase::~SqliteDataBase()
{
}

bool SqliteDataBase::doesUserExist(std::string username)
{
	std::string sql_statement = "SELECT * FROM users WHERE UserName = " + username + ";";
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackUser, nullptr, nullptr);
	return isUserExisting;
}

void SqliteDataBase::addNewUser(std::string username, std::string password, std::string mail)
{
	std::string sql_statement = "INSERT INTO users(UserName, Password, Email) VALUES('" + username + "', '" + password + "', '" + mail + "');";
	int res = sqlite3_exec(_db, sql_statement.c_str(), nullptr, nullptr, nullptr);
}

bool SqliteDataBase::doesPasswordMatch(std::string username, std::string password)
{
	std::string sql_statement = "SELECT * FROM users WHERE UserName = '" + username + "' and Password = '" + password + "';";
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackPasswords, nullptr, nullptr);
	return isPasswordMatching;
}
