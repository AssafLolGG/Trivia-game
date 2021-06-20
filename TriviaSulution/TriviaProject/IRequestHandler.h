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
#define LOGIN_REQUEST_HANDLER_ID 1
#define MENU_REQUEST_HANDLER_ID 2
#define ROOM_MEMBER_REQUEST_HANDLER_ID 3
#define ROOM_ADMIN_REQUEST_HANDLER_ID 4
#define GAME_REQUEST_HANDLER_ID 5

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
	IRequestHandler* new_handler;
	std::vector<uint8_t> response_to_other_players;
	std::vector<SOCKET> players_in_room_sockets;
	std::vector<IRequestHandler*> players_in_room_request_handlers;
};

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo& info) = 0;
	virtual RequestResult handleRequest(RequestInfo& info) = 0;
	virtual int GetRequestHandlerType() = 0;
};

