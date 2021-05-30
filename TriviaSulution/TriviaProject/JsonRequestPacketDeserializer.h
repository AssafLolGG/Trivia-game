#pragma once
#include <iostream>
#include "..//../single_include///nlohmann/json.hpp"
#include "Converter.h"
#include "RequestToDeserializer.h"

using json = nlohmann::json;

#define MESSAGE_SIZE_PLACE 1
#define MESSAGE_SIZE 4

#define PASSWORD "password"
#define USERNAME "username"
#define EMAIL "email"
#define ADDRESS "address"
#define PHONE "phone"
#define BIRTHDATE "birthdate"

#define ROOM_ID "roomID"
#define ROOM_NAME "roomName"
#define MAX_USERS_IN_ROOM "maximumUsers"
#define QUESTION_COUNT "questionCount"
#define TIME_TO_ANSWER "timeToAnswer"

class JsonRequestPacketDeserializer
{
private:
	static json getJson(std::vector<uint8_t> buffer);

public:
	static LoginRequest deserializeLoginRequest(std::vector<uint8_t> buffer);
	static SignupRequest deserializeSignupRequest(std::vector<uint8_t> buffer);

	static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(std::vector<uint8_t> buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(std::vector<uint8_t> buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(std::vector<uint8_t> buffer);
	static GetRoomDataRequest deserializeRoomDataRequest(std::vector<uint8_t> buffer);

};

