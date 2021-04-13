#include "Communicator.h"




void Communicator::bindAndListen()
{
	
	while (true)
	{
		// notice that we step out to the global namespace
		// for the resolution of the function accept
		std::cout << "Waiting for connection\n";
		// this accepts the client and create a specific socket from server to this client
		
		SOCKET client_socket = ::accept(this->m_serverSocket, NULL, NULL);

		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		// the function that handle the conversation with the client
		std::thread t(&Communicator::handleNewClient, this, client_socket);
		t.detach();
	}
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	this->m_clients.insert(std::pair<SOCKET, IRequestHandler*>(clientSocket, nullptr));
	try
	{
		while (true)
		{
		}
		
	}
	catch (const std::exception& e)
	{
		closesocket(clientSocket); // closing the client socket
	}

}

Communicator::Communicator()
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(3720); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (bind(this->m_serverSocket, (struct sockaddr*) & sa, sizeof(sa)) == SOCKET_ERROR)
		std::cout << "not working\n";

	// Start listening for incoming requests of clients
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		std::cout << "not working2\n";
	std::cout << "Listening on port 4000 " << std::endl;
	// creates the MT server
	std::thread t(&Communicator::bindAndListen, this);
	t.detach();
}

void Communicator::startHandleRequests()
{
	bindAndListen();
}
