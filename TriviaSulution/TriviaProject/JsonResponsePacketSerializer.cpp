#include "JsonResponsePacketSerializer.h"
 
/*
function that serialize Error response.
input: the Error response struct.
output: the serialized error response.
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{
	int jsonSize = 0;
	json responseJson;
	responseJson["message"] = response.message;
	std::string jsonSerialized = responseJson.dump();
	return GetCompleteMessage(ERROR_CODE, jsonSerialized);
}

/*
function that serialize Login response.
input: the Login response struct.
output: the serialized error response.
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
	json responseJson;
	responseJson["status"] = response.status;
	std::string jsonSerialized = responseJson.dump();
	return GetCompleteMessage(LOGIN_CODE, jsonSerialized);
}
/*
function that serialize Sign up response.
input: the Sign up response struct.
output: the serialized error response.
*/
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
	json responseJson;
	responseJson["status"] = response.status;
	std::string jsonSerialized = responseJson.dump();
	return GetCompleteMessage(SIGNUP_CODE, jsonSerialized);
}

/*
function that adds into one byte array the response code, the size of the response , the responce json itself.
input: the response code, and the json of the response.
output: the completed message.
*/
std::vector<unsigned char> GetCompleteMessage(unsigned char ResponseCode, std::string jsonResponse)
{
	int jsonSize = jsonResponse.size();
	unsigned char jsonSizeInBinary[MAX_BITS_SIZE_OF_RESPONSE_CONTENT] = {};
	std::vector<unsigned char> fullBinaryMessage = std::vector<unsigned char>();

	// gets the size of the response's json in binary
	decToBinary(jsonSize, jsonSizeInBinary, MAX_BITS_SIZE_OF_RESPONSE_CONTENT);
	// writes the response code 
	fullBinaryMessage.push_back(ResponseCode);

	// writes the size of the json in 4 bytes.
	for (int i = 0; i < MAX_BITS_SIZE_OF_RESPONSE_CONTENT; i++)
	{
		fullBinaryMessage.push_back(jsonSizeInBinary[i]);
	}
	
	// writes the response's json
	for (auto it = jsonResponse.begin(); it != jsonResponse.end(); it++)
	{
		fullBinaryMessage.push_back(*it);
	}
	return fullBinaryMessage;
}
