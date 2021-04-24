#pragma once

#include <ctime>
#include <vector>
#include <cstdint>
#include <string>
#include "..//../single_include///nlohmann/json.hpp"
#include "Converter.h"

using json = nlohmann::json;

// can be changed later, to avoid conflict with simular defines.
#define LOGIN 1
#define SIGNUP 2

class IRequestHandler;
struct RequestInfo;
struct RequestResult;

struct RequestInfo
{
	int id;
	std::time_t recivalTime;
	std::vector<uint8_t> buffer;
};

struct RequestResult
{
	std::string respone;
	IRequestHandler* newHandler;
};

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo& info) = 0;
	virtual RequestResult handleRequest(RequestInfo& info) = 0;
};

