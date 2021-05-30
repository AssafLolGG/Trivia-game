#include "JsonResponsePacketSerializer.h"
 
/*	function that serialize Error response.
	input: the Error response struct.
	output: the serialized error response. */
std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{
	int jsonSize = 0;
	json responseJson;
	responseJson["message"] = response.message;
	std::string jsonSerialized = responseJson.dump();
	return getCompleteMessage(ERROR_CODE, jsonSerialized);
}

/*	function that serialize Login response.
	input: the Login response struct.
	output: the serialized error response. */
std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
	json responseJson;
	responseJson["status"] = response.status;
	std::string jsonSerialized = responseJson.dump();
	return getCompleteMessage(LOGIN_CODE, jsonSerialized);
}
/*	function that serialize Sign up response.
	input: the Sign up response struct.
	output: the serialized error response. */
std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
	json responseJson;
	responseJson["status"] = response.status;
	std::string jsonSerialized = responseJson.dump();
	return getCompleteMessage(SIGNUP_CODE, jsonSerialized);
}

/*	function that serialize Logout response.
	input: the Logout up response struct.
	output: the serialized error response. */
std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(LogoutResponse response)
{
	json responseJson;
	responseJson["status"] = response.status;
	std::string jsonSerialized = responseJson.dump();
	return getCompleteMessage(LOGOUT_CODE, jsonSerialized);
}

/* serialize response for "GetRoomsResponse" request */
std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse rooms)
{
	json responseJson;
	responseJson["status"] = rooms.status;
	responseJson["rooms"] = roomDataToString(rooms.rooms);

	std::string jsonSerialized = responseJson.dump();
	
	return getCompleteMessage(GET_ROOMS_CODE, jsonSerialized);
}

/* serialize response for "GetPlayersInRoomResponse" request */
std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse players)
{
	json responseJson;
	responseJson["players"] = stringVectorToString(players.players);

	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(GET_PLAYERS_IN_ROOM_CODE, jsonSerialized);
}

/* serialize response for "jroom" request */
std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse jroom)
{
	json responseJson;
	responseJson["status"] = jroom.status;
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(GET_JOIN_ROOMS_CODE, jsonSerialized);
}

/* serialize response for "croom" request */
std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse croom)
{
	json responseJson;
	responseJson["status"] = croom.status;
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(GET_CREATE_ROOMS_CODE, jsonSerialized);
}

/* serialize response for statstics request*/
std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(GetPersonalStatsResponse stats)
{
	json responseJson;
	responseJson["user_statistics"] = stats.statistics;
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(GET_STATISTICS_CODE, jsonSerialized);
}

std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(GetRoomDataResponse roomData)
{
	json responseJson;
	responseJson["status"] = roomData.status;
	responseJson["name"] = roomData.name;
	responseJson["questionsNumber"] = roomData.numOfQuestionsInGame;
	responseJson["maxPlayers"] = roomData.maxPlayers;
	responseJson["timePerQuestion"] = roomData.timePerQuestion;
	responseJson["isActive"] = roomData.isActive;

	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(GET_ROOMDATA_CODE, jsonSerialized);
}

/*	function that adds into one byte array the response code, the size of the response and the responce json itself.
	input: the response code, and the json of the response.
	output: the completed message. */
std::vector<uint8_t> getCompleteMessage(unsigned char ResponseCode, std::string jsonResponse)
{
	int jsonSize = jsonResponse.size();
	uint8_t jsonSizeInBinary[MAX_BITS_SIZE_OF_RESPONSE_CONTENT] = {};
	std::vector<uint8_t> fullBinaryMessage = std::vector<uint8_t>();

	// gets the size of the response's json in binary
	decToBinary(jsonSize, jsonSizeInBinary, MAX_BITS_SIZE_OF_RESPONSE_CONTENT); // 0x32 0x8 0x8
	// writes the response code 
	fullBinaryMessage.push_back(ResponseCode);

	// writes the size of the json in 4 bytes.
	for (int i = 0; i < 4; i++)
	{
		fullBinaryMessage.push_back((uint8_t)binToDecimal(bitsToBytes(i*8, jsonSizeInBinary)));
	}
	
	// writes the response's json
	for (auto it = jsonResponse.begin(); it != jsonResponse.end(); it++)
	{
		fullBinaryMessage.push_back(*it);
	}
	return fullBinaryMessage;
}

/* converts a vector of rooms into one string, only uses rooms names with ", " between each name */
string roomDataToString(std::vector<RoomData> rooms)
{
	string rooms_string = "";

	for (auto it = rooms.begin(); it != rooms.end(); ++it)
	{
		rooms_string += it->name + ", ";
	}
	if (rooms_string != "")
	{
		rooms_string.erase(rooms_string.size() - 2);
	}
	return rooms_string;
}

/* converts a vector of strings into one string with ", " between each string */
string stringVectorToString(std::vector<string> strings)
{
	string str = "";
	string temp;

	for (int i = 0; i < strings.size(); i++)
	{
		str += strings[i] + ", ";
	}

	if (str != "")
	{
		str.erase(str.size() - 2);
	}
	return str;
}
