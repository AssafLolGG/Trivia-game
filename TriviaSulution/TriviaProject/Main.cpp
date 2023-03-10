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
	
	// gets input from the user, till he wants to exit.
	while (userInput != "EXIT")
	{
		std::cin >> userInput;
	}
	std::cout << "Goodbye :>" << std::endl;

	return 0;
}