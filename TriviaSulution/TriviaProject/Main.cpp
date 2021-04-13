#pragma comment (lib, "ws2_32.lib")
#include "WSAInitializer.h"
#include "Server.h"
int main()
{
	WSAInitializer wsa;
	Server ser;
	ser.run();
	
	return 69;
}