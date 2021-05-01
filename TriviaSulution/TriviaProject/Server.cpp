#include "Server.h"

Server::Server() : m_database(new SqliteDataBase()), m_handlerFactory(RequestHandlerFactory(this->m_database)), m_communicator(m_handlerFactory)
{
}

void Server::run()
{
	this->m_communicator.startHandleRequests();
}
