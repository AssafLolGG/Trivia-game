#include "SqliteDataBase.h"
bool SqliteDataBase::isPasswordMatching = false;
bool SqliteDataBase::isUserExisting = false;

statisticsDB global_stats;
Question q;

/*
callback function that gets users statistics to a vector.
*/
int SqliteDataBase::callbackStatistics(void* data, int argc, char** argv, char** azColName)
{
	isUserExisting = true;
	std::vector<statisticsDB>* dataVector = (std::vector<statisticsDB>*) data;

	// getting the user's qualities from db to an user object.
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "player_id")
		{
			global_stats.player_id = std::stoi(argv[i]);
		}

		else if (std::string(azColName[i]) == "games_played")
		{
			global_stats.games_played = std::stoi(argv[i]);
		}

		else if (std::string(azColName[i]) == "right_answers")
		{
			global_stats.right_answers = std::stoi(argv[i]);
		}

		else if (std::string(azColName[i]) == "total_answers")
		{
			global_stats.total_answers = std::stoi(argv[i]);
		}

		else if (std::string(azColName[i]) == "likeability")
		{
			global_stats.likeability = std::stoi(argv[i]);
		}

		else if (std::string(azColName[i]) == "potential")
		{
			global_stats.potnetial = std::stoi(argv[i]);
		}

		else if (std::string(azColName[i]) == "highest_score")
		{
			global_stats.highest_score = std::stoi(argv[i]);
		}

		else if (std::string(azColName[i]) == "time_played")
		{
			global_stats.time_played = std::stoi(argv[i]);
		}
		else if (std::string(azColName[i]) == "time_played_last_game")
		{
			global_stats.time_played_last_game = std::stoi(argv[i]);
		}
		else if (std::string(azColName[i]) == "score_last_game")
		{
			global_stats.score_last_game = std::stoi(argv[i]);
		}
		else if (std::string(azColName[i]) == "number_of_future_partners")
		{
			global_stats.number_of_future_partners = std::stoi(argv[i]);
		}
		else if (std::string(azColName[i]) == "questions_last_game")
		{
			global_stats.questions_last_game = std::stoi(argv[i]);
		}
	}

	// pushes the user's record to the vector.
	dataVector->push_back(global_stats);
	return 0;
}
/*
function that calls back user/s data into a vector of users.
input: the data to the call back function, number of fields in the current record,
the value of the record in certain index, an array containing the value of the fields.
output: if the function succeded.
*/
int SqliteDataBase::callbackUser(void* data, int argc, char** argv, char** azColName)
{
	isUserExisting = true;
	std::vector<User>* userVector = (std::vector<User>*) data;
	User user;
	// getting the user's qualities from db to an user object.
	for (int i = 0; i < argc; i++) 
	{
		if (std::string(azColName[i]) == "UserName") 
		{
			user.username = argv[i];
		}

		else if (std::string(azColName[i]) == "Password")
		{
			user.password = argv[i];
		}

		else if (std::string(azColName[i]) == "Mail")
		{
			user.mail = argv[i];
		}

		else if(std::string(azColName[i]) == "Id")
		{
			user.id = std::stoi(argv[i]);
		}

		
	}

	// pushes the user's record to the vector.
	userVector->push_back(user);
	return 0;
}

/*
function that calls back user with a certain password which we gave in the sql statement.
input: the data to the call back function, number of fields in the current record,
the value of the record in certain index, an array containing the value of the fields.
output: if the function succeded.
*/
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

/*
callback function that gets the questions and answers to a 
vector passed as parameter.
*/
int SqliteDataBase::callbackQuestionsAndAnswers(void* data, int argc, char** argv, char** azColName)
{
	std::vector<Question>* dataQuestions = (std::vector<Question>*) data;
	Answer ans;
	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "question_id")
		{
			if (dataQuestions->size() != 0 && std::stoi(argv[i]) != dataQuestions->front().id)
			{
				q.answers.clear();
				q.id = std::stoi(argv[i]);
				dataQuestions->push_back(q);
			}
			else if( dataQuestions->size() == 0)
			{
				q.id = std::stoi(argv[i]);
				dataQuestions->push_back(q);
			}
		}
		else if(std::string(azColName[i]) == "question")
		{
			dataQuestions->front().questionText = argv[i];
		}
		if (std::string(azColName[i]) == "answer")
		{
			ans.answerText = std::stoi(argv[i]);
		}
		
		else if (std::string(azColName[i]) == "if_correct")
		{
			ans.isCorrect = std::stoi(argv[i]) == 0 ? false : true;
		}
		dataQuestions->front().answers.push_back(ans);
	}
	return 0;
}

/*
constractor function that opens the db file and initialize the db
storing variable.
input: None.
output: an  sqliteDataBase object.
*/
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
	std::vector<User> users_vector;
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackUser, &users_vector, nullptr);
	if (SqliteDataBase::isUserExisting)
	{
		SqliteDataBase::isUserExisting = false;
		return true;
	}
	return false;
}

/* checks if password matches the password of the username in database
   input: string / username, string / password
   output: if the matches the username */
bool SqliteDataBase::doesPasswordMatch(std::string username, std::string password)
{
	std::string sql_statement = "SELECT * FROM users WHERE UserName = '" + username + "' and Password = '" + password + "';";
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackPasswords, nullptr, nullptr); // callback checks 
	if (SqliteDataBase::isPasswordMatching)
	{
		SqliteDataBase::isPasswordMatching = false;
		return true;
	}
	return false;
}

/* adding new user to dataBase
   input: string / username, string / password, string / email
   output: None */
void SqliteDataBase::addNewUser(std::string username, std::string password, std::string mail, std::string address, std::string phone, std::string birthdate)
{
	std::string sql_statement = "INSERT INTO users(UserName, Password, mail, address, phoneNumber, birthdate) VALUES('" + username + "', '" + password + "', '" + mail + "', '" + address + "', '" + phone + "', '" + birthdate + "');";
	int res = sqlite3_exec(_db, sql_statement.c_str(), nullptr, nullptr, nullptr);
}

/*
function that returns the average answer time of the user in last round.
input: the user's id.
output: the average time that the player took to answer a question.
*/
int SqliteDataBase::getPlayerAverageAnswerTime(int user_id)
{
	std::string sql_statement = "SELECT * FROM Statistics WHERE player_id = " + std::to_string(user_id) + ";";
	std::vector<statisticsDB> usersVector = std::vector<statisticsDB>();
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackStatistics , &usersVector, nullptr);
	return usersVector[0].time_played_last_game / usersVector[0].questions_last_game;
}

/*
function that gets the number of correct answers the user answerd 
in all of his games.
input: the user's id.
output: the right answers the user answered in all of his games.
*/
int SqliteDataBase::getNumOfCorrectAnswers(int user_id)
{
	std::string sql_statement = "SELECT * FROM Statistics WHERE player_id = " + std::to_string(user_id) + ";";
	std::vector<statisticsDB> usersVector = std::vector<statisticsDB>();
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackStatistics, &usersVector, nullptr);
	return usersVector[0].right_answers;
}

/*
function that gets the number of total answers the user in all of his games.
input: the user's id.
output: the count of total answers the user answered in all of his games.
*/
int SqliteDataBase::getNumOfTotalAnswers(int user_id)
{
	std::string sql_statement = "SELECT * FROM Statistics WHERE player_id = " + std::to_string(user_id) + ";";
	std::vector<statisticsDB> usersVector = std::vector<statisticsDB>();
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackStatistics, &usersVector, nullptr);
	return usersVector[0].total_answers;
}

/*
function that get the number of games a user played in.
input: The user id of the player.
output: number of games the user played
*/
int SqliteDataBase::getNumOfPlayerGames(int user_id)
{
	std::string sql_statement = "SELECT * FROM Statistics WHERE player_id = " + std::to_string(user_id) + ";";
	std::vector<statisticsDB> usersVector = std::vector<statisticsDB>();
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackStatistics, &usersVector, nullptr);
	return usersVector[0].games_played;
}

/*
function that converts username to its id and returns it.
input: the username.
output: the id of the username.
*/
int SqliteDataBase::usernameToID(std::string username)
{
	std::string sql_statement = "SELECT * FROM users WHERE UserName = " + username + ";";
	std::vector<User> usersVector = std::vector<User>();
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackUser, &usersVector, nullptr);
	return usersVector[0].id;
}

/*
function that gets the questions from the database to a vector.
input: None.
output: the vector of the questions(also containing their answers).
*/
std::vector<Question> SqliteDataBase::getQuestions()
{
	std::string sql_statement = "SELECT * FROM Answers inner join Questions ON Answers.question_id = Questions.question_id;";
	std::vector<Question> questionsVector = std::vector<Question>();
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackQuestionsAndAnswers, &questionsVector, nullptr);

	return questionsVector;
}

/*
the function returns the top 5 players with the higest score.
input: None.
output: a vector of the top 5 players's(by score) statistics.
*/
std::vector<statisticsDB> SqliteDataBase::getTop5Players()
{
	std::string sql_statement = "SELECT * FROM Statistics ORDER BY highest_score DESC LIMIT 5;";
	std::vector<statisticsDB> usersVector = std::vector<statisticsDB>();
	int res = sqlite3_exec(_db, sql_statement.c_str(), callbackUser, &usersVector, nullptr);
	return usersVector;
}
