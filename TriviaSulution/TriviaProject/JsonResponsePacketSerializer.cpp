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
	responseJson["status"] = std::to_string(rooms.status);
	responseJson["rooms"] = roomDataToString(rooms.rooms);
	responseJson["rooms_id"] = roomDataVectorToRoomIdString(rooms.rooms);

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
	responseJson["id"] = croom.id;
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
	responseJson["id"] = roomData.id;
	responseJson["name"] = roomData.name;
	responseJson["questionsNumber"] = roomData.numOfQuestionsInGame;
	responseJson["maxPlayers"] = roomData.maxPlayers;
	responseJson["timePerQuestion"] = roomData.timePerQuestion;
	responseJson["isActive"] = roomData.isActive;

	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(GET_ROOMDATA_CODE, jsonSerialized);
}

std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(TopFivePlayers top_five)
{
	json responseJson;
	responseJson["players"] = stringVectorToString(top_five.top_players);
	responseJson["score"] = stringVectorToString(top_five.top_score);
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(GET_TOP_FIVE_CODE, jsonSerialized);
}

std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse closeRoom)
{
	json responseJson;
	responseJson["status"] = closeRoom.status;
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(CLOSE_ROOM_CODE, jsonSerialized);
}

std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(StartRoomResponse startRoom)
{
	json responseJson;
	responseJson["status"] = startRoom.status;
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(START_ROOM_CODE, jsonSerialized);
}

std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse leaveRoom)
{
	json responseJson;
	responseJson["status"] = leaveRoom.status;
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(LEAVE_ROOM_CODE, jsonSerialized);
}

std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse getGameResults)
{
	json responseJson;
	responseJson["status"] = getGameResults.status;
	responseJson["results"] = getGameResults.results;
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(GET_GAME_RESULTS_CODE, jsonSerialized);
}

std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse submitAnswer)
{
	json responseJson;
	responseJson["status"] = submitAnswer.status;
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(SUBMIT_ANSWER_CODE, jsonSerialized);
}

std::vector<uint8_t> JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse getQuestion)
{
	json responseJson;
	responseJson["status"] = getQuestion.status;
	responseJson["question"] = getQuestion.question;
	responseJson["answers"] = getQuestion.Answers;
	std::string jsonSerialized = responseJson.dump();

	return getCompleteMessage(GET_QUESTION_CODE, jsonSerialized);
}


void to_json(json& j, const PlayerResults& result)
{
	j += 
	{
		{"username", result.Username},
		{"num_of_correct_answers", result.CorrectAnswersCount},
		{"num_of_wrong_answers", result.WrongAnswerCount},
		{"average_answer_time", result.averageAnswerTime}
	};
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

string roomDataVectorToRoomIdString(std::vector<RoomData> rooms)
{
	std::vector<string> rooms_id_vector;
	for (auto iter = rooms.begin(); iter != rooms.end(); iter++)
	{
		rooms_id_vector.push_back(std::to_string(iter->id));
	}
	return stringVectorToString(rooms_id_vector);
}
