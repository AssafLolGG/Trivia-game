#pragma once

#include <vector>
#include "Room.h"

using std::string;

struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct ErrorResponse
{
	std::string message;
};

struct LogoutResponse
{
	unsigned int status;
};

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
	std::vector<string> players;
};

struct GetHighScoreResponse
{
	unsigned int status;
	std::vector<string> statistics;
};

struct GetPersonalStatsResponse
{
	unsigned int status;
	std::vector<string> statistics;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
	unsigned int id;
};

struct GetRoomDataResponse
{
	unsigned int status;
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
};

struct TopFivePlayers
{
	std::vector<std::string> top_players;
	std::vector<std::string> top_score;
};

struct CloseRoomResponse
{
	int status;
};

struct StartRoomResponse
{
	int status;
};

struct LeaveRoomResponse
{
	int status;
};

struct PlayerResults
{
	string Username;
	int CorrectAnswersCount;
	int WrongAnswerCount;
	int averageAnswerTime;

};

struct GetGameResultsResponse
{
	int status;
	std::vector<PlayerResults> results;
};

struct GetQuestionResponse
{
	int status;
	string question;
	std::map<int, string> Answers;
};

struct SubmitAnswerResponse
{
	int status;
};