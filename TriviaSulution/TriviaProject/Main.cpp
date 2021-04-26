#pragma comment (lib, "ws2_32.lib")
#include "WSAInitializer.h"
#include "Server.h"
#include "JsonResponsePacketSerializer.h"
#include "SqliteDataBase.h"

int main()
{
	WSAInitializer wsa;
	Server ser;

	ser.run();
	SqliteDataBase myDB;
	std::string userInput = "";
	myDB.addNewUser("xxxIdolxxx", "123", "@getScam");
	myDB.doesUserExist("xxxIdolxxx");
	myDB.doesPasswordMatch("xxxIdolxxx", "123");
	while (userInput != "EXIT")
	{
		std::cin >> userInput;
		
	}
	std::cout << "Goodbye :>" << std::endl;

	return 69;
}