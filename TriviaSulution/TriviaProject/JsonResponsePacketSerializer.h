#pragma once
#include <iostream>
#include <vector>
#include "..//../single_include///nlohmann/json.hpp"
#include "Converter.h"

#define MAX_BITS_SIZE_OF_RESPONSE_CONTENT 32

#define ERROR_CODE 1
#define LOGIN_CODE 2
#define SIGNUP_CODE 3

#define BINARY_BASE 2

using json = nlohmann::json;
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

std::vector<unsigned char> GetCompleteMessage(unsigned char ResponseCode, std::string jsonResponse);

class JsonResponsePacketSerializer
{
public:
	static std::vector<unsigned char> serializeResponse(ErrorResponse);
	static std::vector<unsigned char> serializeResponse(LoginResponse);
	static std::vector<unsigned char> serializeResponse(SignupResponse);
};

