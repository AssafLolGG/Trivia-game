#include "Server.h"

void Server::run()
{
	this->m_communicator.startHandleRequests();
}
