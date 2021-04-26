#pragma once
#include <iostream>
#include "..//../single_include///nlohmann/json.hpp"
#include "Converter.h"

using json = nlohmann::json;

#define MESSAGE_SIZE_PLACE 1
#define MESSAGE_SIZE 4

#define PASSWORD "password"
#define USERNAME "username"
#define EMAIL "email"

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

class JsonRequestPacketDeserializer
{
	static LoginRequest deserializeLoginRequest(std::vector<uint8_t> buffer);
	static SignupRequest deserializeSignupRequest(std::vector<uint8_t> buffer);
};

