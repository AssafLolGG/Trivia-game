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
#include "../../single_include/utf-8/utf8_v2_3_4/source/utf8.h"
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
public:
	Communicator(RequestHandlerFactory& handlerFactory);
	void startHandleRequests();
};

