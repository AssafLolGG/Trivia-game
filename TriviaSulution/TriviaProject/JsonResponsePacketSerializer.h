#pragma once
#include <iostream>
#include <vector>
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

class JsonResponsePacketSerializer
{
	std::vector<unsigned char> serializeResponse(ErrorResponse);
	std::vector<unsigned char> serializeResponse(LoginResponse);
	std::vector<unsigned char> serializeResponse(SignupResponse);
};

