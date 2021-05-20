#pragma once

#include <string>

using std::string;

struct LoginRequest
{
	string username;
	string password;
};

struct SignupRequest
{
	string username;
	string password;
	string email;
	string address;
	string phone;
	string birthdate;
};

struct GetPlayersInRoomRequest
{
	unsigned int room_id;
};

struct JoinRoomRequest
{
	unsigned int room_id;
};

struct CreateRoomRequest
{
	string room_name;
	unsigned int max_users;
	unsigned int question_count;
	unsigned int answer_time_out;
};