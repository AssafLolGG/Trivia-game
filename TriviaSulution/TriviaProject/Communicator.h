#pragma once
#include <iostream>
#include <WinSock2.h>
#include "IRequestHandler.h"
#include <map>
#include <thread>
using std::map;
class Communicator
{
private:
	SOCKET m_serverSocket;
	map<SOCKET, IRequestHandler*> m_clients;
	void bindAndListen();
	void handleNewClient(SOCKET client_soc);
public:
	Communicator();
	void startHandleRequests();
};

