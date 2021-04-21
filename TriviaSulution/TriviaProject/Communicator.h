#pragma once
#include <iostream>
#include <WinSock2.h>
#include "IRequestHandler.h"
#include <map>
#include "LoginRequestHandler.h"
#include <thread>

#define	STARTER_SERVER_MESSAGE "hello"
#define BUFFER_CAPACITY 1024

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

