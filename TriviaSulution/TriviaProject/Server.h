#pragma once

#include "Communicator.h"
#include "LoginRequestHandler.h"
#include "SqliteDataBase.h"
#include "RequestHandlerFactory.h"

class Server
{
private:
	IDatabase* m_database;
	RequestHandlerFactory m_handlerFactory;
	Communicator m_communicator;
public:
	Server();
	void run();
};

