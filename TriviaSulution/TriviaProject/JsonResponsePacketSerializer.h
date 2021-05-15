#pragma once

#include <iostream>
#include "..//../single_include///nlohmann/json.hpp"
#include "Converter.h"
#include "ResponseToSerializer.h"
#include "codes.h"

#define MAX_BITS_SIZE_OF_RESPONSE_CONTENT 32

#define BINARY_BASE 2

using json = nlohmann::json;

std::vector<unsigned char> getCompleteMessage(unsigned char ResponseCode, std::string jsonResponse);
string roomDataToString(std::vector<RoomData> rooms);
string stringVectorToString(std::vector<string> strings);

class JsonResponsePacketSerializer
{
public:
	static std::vector<unsigned char> serializeResponse(ErrorResponse);
	static std::vector<unsigned char> serializeResponse(LoginResponse);
	static std::vector<unsigned char> serializeResponse(SignupResponse);

	static std::vector<unsigned char> serializeResponse(LogoutResponse);
	static std::vector<unsigned char> serializeResponse(GetRoomsResponse);
	static std::vector<unsigned char> serializeResponse(GetPlayersInRoomResponse);
	static std::vector<unsigned char> serializeResponse(JoinRoomResponse);
	static std::vector<unsigned char> serializeResponse(CreateRoomResponse);
	//static std::vector<unsigned char> serializeResponse(GetStatistics)
};

