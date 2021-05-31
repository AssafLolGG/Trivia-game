#include "ServerUtilities.h"

bool receiveMassageFromClient(SOCKET& client_socket, char* message_buffer)
{
	int received = recv(client_socket, message_buffer, BUFFER_CAPACITY - 1, NULL);

	if (received == SOCKET_ERROR)
	{
		std::cout << "Error: " << WSAGetLastError();
		throw(std::exception());
		return false;
	}

	if (received == 0)
	{
		throw(std::exception());
	}

	return true;
}

RequestInfo createRequestInfo(std::vector<uint8_t>& buffer_vector)
{
	// checks the wanted request from user, to send a proper response.
	RequestInfo request;

	request.buffer = buffer_vector;
	request.id = buffer_vector[0];
	auto nowTime = std::chrono::system_clock::now();
	std::time_t nowTime_t = std::chrono::system_clock::to_time_t(nowTime);
	request.recivalTime = nowTime_t;

	return request;
}
 
bool isLoggedIn(IRequestHandler* new_handler, RequestHandlerFactory& handler_factory)
{
	if (new_handler != nullptr) // if an error doesn't occurs
	{
		return true;
	}
	else
	{
		new_handler = handler_factory.createLoginRequestHandler();
		return false;
	}
}

void insertHandlerToClient(IRequestHandler* new_handler, std::map<SOCKET, IRequestHandler*>& clients, SOCKET& client_socket)
{
	for (auto it = clients.begin(); it != clients.end(); ++it) // going through all of the clients
	{
		if (it->first == client_socket) // finding the client that used the function
		{
			it->second = new_handler; // inserting the current request handler
		}
	}
}