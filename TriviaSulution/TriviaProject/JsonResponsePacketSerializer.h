#pragma once

#include <iostream>
#include "..//../single_include///nlohmann/json.hpp"
#include "Converter.h"
#include "ResponseToSerializer.h"
#include "codes.h"

#define MAX_BITS_SIZE_OF_RESPONSE_CONTENT 32

#define BINARY_BASE 2

using json = nlohmann::json;

std::vector<uint8_t> getCompleteMessage(unsigned char ResponseCode, std::string jsonResponse);
string roomDataToString(std::vector<RoomData> rooms);
string stringVectorToString(std::vector<string> strings);
string roomDataVectorToRoomIdString(std::vector<RoomData> rooms);

class JsonResponsePacketSerializer
{
public:
	static std::vector<uint8_t> serializeResponse(ErrorResponse);
	static std::vector<uint8_t> serializeResponse(LoginResponse);
	static std::vector<uint8_t> serializeResponse(SignupResponse);

	static std::vector<uint8_t> serializeResponse(LogoutResponse);
	static std::vector<uint8_t> serializeResponse(GetRoomsResponse);
	static std::vector<uint8_t> serializeResponse(GetPlayersInRoomResponse);
	static std::vector<uint8_t> serializeResponse(JoinRoomResponse);
	static std::vector<uint8_t> serializeResponse(CreateRoomResponse);
	static std::vector<uint8_t> serializeResponse(GetPersonalStatsResponse);
	static std::vector<uint8_t> serializeResponse(GetRoomDataResponse);
	static std::vector<uint8_t> serializeResponse(TopFivePlayers);
};

