#pragma once
#include <iostream>
#include <WinSock2.h>
#include "IRequestHandler.h"
#include <map>
#include "LoginRequestHandler.h"
#include <thread>
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"
#include "ServerUtilities.h"

#define	STARTER_SERVER_MESSAGE "hello"
#define PORT_NUM 9999
using std::map;

class RequestHandlerFactory;

class Communicator
{
private:
	SOCKET m_serverSocket;
	map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
	void bindAndListen();
	void handleNewClient(SOCKET client_soc);
	bool connectUser(SOCKET& client_socket);
public:
	Communicator(RequestHandlerFactory& handlerFactory);
	void startHandleRequests();
};

