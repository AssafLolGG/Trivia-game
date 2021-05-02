#pragma once

#include <ctime>
#include <vector>
#include <cstdint>
#include <string>
#include "..//../single_include///nlohmann/json.hpp"
#include "Converter.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#define STATUS_FAIL 0
#define STATUS_OK 1

using json = nlohmann::json;

class IRequestHandler;
struct RequestInfo;
struct RequestResult;
struct LoginRequest;
struct signupRequest;

struct RequestInfo
{
	int id;
	std::time_t recivalTime;
	std::vector<uint8_t> buffer;
};

struct RequestResult
{
	std::vector<uint8_t> respone;
	IRequestHandler* newHandler;
};

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo& info) = 0;
	virtual RequestResult handleRequest(RequestInfo& info) = 0;
};

