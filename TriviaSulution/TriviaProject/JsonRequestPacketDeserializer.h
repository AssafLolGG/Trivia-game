#pragma once
#include "LoginRequestHandler.h"
#include <iostream>

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
};

