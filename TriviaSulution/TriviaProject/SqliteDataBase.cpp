#include "SqliteDataBase.h"
bool SqliteDataBase::isPasswordMatching = false;
bool SqliteDataBase::isUserExisting = false;

int SqliteDataBase::callbackUser(void* data, int argc, char** argv, char** azColName)
{
	isUserExisting = false;
	for (int i = 0; i < argc; i++) 
	{
		if (std::string(azColName[i]) == "UserName") 
		{
			//checks if user exists in the db
			(std::string)(argv[i]) == "" ? SqliteDataBase::isUserExisting = false : SqliteDataBase::isUserExisting = true;
		}
	}
	return 0;
}

int SqliteDataBase::callbackPasswords(void* data, int argc, char** argv, char** azColName)
{
	isUserExisting = false;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "UserName")
		{
			(std::string)(argv[i]) == "" ? SqliteDataBase::isUserExisting = false : SqliteDataBase::isPasswordMatching = true; // checks to see if there any user that matches the password
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

/* checks if user exist in database
   input: string / username
   output: bool / if users exists in database */
bool SqliteDataBase::doesUserExist(std::string username)
{
	std::string sql_statement = "SELECT * FROM users WHERE UserName = '" + username + "';";
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackUser, nullptr, nullptr);
	return SqliteDataBase::isUserExisting;
}

/* checks if password matches the password of the username in database
   input: string / username, string / password
   output: if the matches the username */
bool SqliteDataBase::doesPasswordMatch(std::string username, std::string password)
{
	std::string sql_statement = "SELECT * FROM users WHERE UserName = '" + username + "' and Password = '" + password + "';";
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackPasswords, nullptr, nullptr); // callback checks 
	return SqliteDataBase::isPasswordMatching;
}

/* adding new user to dataBase
   input: string / username, string / password, string / email
   output: None */
void SqliteDataBase::addNewUser(std::string username, std::string password, std::string mail)
{
	std::string sql_statement = "INSERT INTO users(UserName, Password, mail) VALUES('" + username + "', '" + password + "', '" + mail + "');";
	int res = sqlite3_exec(_db, sql_statement.c_str(), nullptr, nullptr, nullptr);
}
