#pragma comment (lib, "ws2_32.lib")
#include "WSAInitializer.h"
#include "Server.h"
#include "JsonResponsePacketSerializer.h"
int main()
{
	WSAInitializer wsa;
	Server ser;
	ser.run();
	std::string userInput = "";
	while (userInput != "EXIT")
	{
		//SignupResponse err{ 1};
		//auto v = JsonResponsePacketSerializer::serializeResponse(err);
		//std::string SerializerString(v.begin(), v.end());
		//std::cout << SerializerString << "\n";
		std::cin >> userInput;
	}
	std::cout << "Goodbye :>" << std::endl;
	return 69;
}