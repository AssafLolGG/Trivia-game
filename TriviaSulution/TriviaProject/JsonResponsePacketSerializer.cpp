#include "JsonResponsePacketSerializer.h"
 
std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{
	int jsonSize = 0;
	json responseJson;
	responseJson["message"] = response.message;
	std::string jsonSerialized = responseJson.dump();
	std::vector<unsigned char> SerializedJson(jsonSerialized.begin(), jsonSerialized.end());
	jsonSize = SerializedJson.size();
	return SerializedJson;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
	json responseJson;
	responseJson["status"] = response.status;
	std::string jsonSerialized = responseJson.dump();
	std::vector<unsigned char> SerializedJson(jsonSerialized.begin(), jsonSerialized.end());
	return SerializedJson;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
	json responseJson;
	responseJson["status"] = response.status;
	std::string jsonSerialized = responseJson.dump();
	std::vector<unsigned char> SerializedJson(jsonSerialized.begin(), jsonSerialized.end());
	return SerializedJson;
}



