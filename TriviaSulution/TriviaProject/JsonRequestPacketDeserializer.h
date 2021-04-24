#pragma once
#include "LoginRequestHandler.h"
#include <iostream>
#include "..//../single_include///nlohmann/json.hpp"

using json = nlohmann::json;

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

class JsonRequestPacketDeserializer : public LoginRequestHandler
{
	static LoginRequest deserializeLoginRequest(static std::vector<unsigned char>);
	static SignupRequest deserializeSignupRequest(static std::vector<unsigned char>);
};

