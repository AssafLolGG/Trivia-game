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

void Communicator::handleNewClient(SOCKET client_socket)
{
	RequestInfo request;
	RequestResult result;
	ErrorResponse error;
	std::string server_message = STARTER_SERVER_MESSAGE;
	char* message_buffer = new char[BUFFER_CAPACITY], serialized_response_in_char_array[BUFFER_CAPACITY] = { 0 };
	std::vector<uint8_t> buffer_vector;
	IRequestHandler* request_handler = this->m_handlerFactory.createLoginRequestHandler(client_socket);
	int count = 0;

	this->m_clients.insert(std::pair<SOCKET, IRequestHandler*>(client_socket, request_handler));

	try
	{
		// sending starter message to client
		send(client_socket, server_message.c_str(), server_message.size(), 0);
		recv(client_socket, message_buffer, BUFFER_CAPACITY - 1, 0);

		while (true)
		{
			request_handler = getHandlerOfClient(this->m_clients, client_socket);
			receiveMassageFromClient(client_socket, message_buffer);

			CharPointerToVector(message_buffer, BUFFER_CAPACITY, buffer_vector); // converting char array to vector

			request = createRequestInfo(buffer_vector);

			if (request_handler->isRequestRelevant(request))
			{
				result = request_handler->handleRequest(request);
				if (result.new_handler == nullptr)
				{
					result.new_handler = request_handler;
				}
				else
				{
					delete request_handler;
					request_handler = result.new_handler;
				}

				insertHandlerToClient(result.new_handler, this->m_clients, client_socket);
			}
			else
			{
				error.message = "failed";
				result.respone = JsonResponsePacketSerializer::serializeResponse(error);
			}
			
			vectorToCharArray(serialized_response_in_char_array, result.respone);

			// sends the serialized response from the server to the client, after analyizing his response.
			send(client_socket, serialized_response_in_char_array, BUFFER_CAPACITY, 0);

			count = 0;

			for (SOCKET current_player_socket: result.players_in_room_sockets) // iter over the sockets of the player in room and sending them messages
			{
				if (current_player_socket != client_socket)
				{
					memset(serialized_response_in_char_array, 0, BUFFER_CAPACITY);

					vectorToCharArray(serialized_response_in_char_array, result.response_to_other_players);

					send(current_player_socket, serialized_response_in_char_array, BUFFER_CAPACITY, 0);
					receiveMassageFromClient(current_player_socket, message_buffer);

					insertHandlerToClient(result.players_in_room_request_handlers[count], this->m_clients, current_player_socket);
				}
				count++;
			}

			// reset char array for future use
			memset(serialized_response_in_char_array, 0, BUFFER_CAPACITY);

			// clearing vectors for future usage
			buffer_vector.clear();
		}
	}
	
	catch (...)
	{
		std::cout << "connection stopped" << std::endl;

		for (auto it = this->m_clients.begin(); it != this->m_clients.end(); ++it) // python tester.py
		{
			if (it->first == client_socket)
			{
				this->m_clients.erase(it);
				closesocket(client_socket); // closing the client socket

				return;
			}
		}
	}
}

Communicator::Communicator(RequestHandlerFactory& handlerFactory): m_handlerFactory(handlerFactory)
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(PORT_NUM); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// again stepping out to the global namespace
	// Connects between the socket and the configuration (port and etc..)
	if (bind(this->m_serverSocket, (struct sockaddr*) & sa, sizeof(sa)) == SOCKET_ERROR)
		std::cout << "not working\n";

	// Start listening for incoming requests of clients
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		std::cout << "not working2\n";
	// creates the MT server
}

void Communicator::startHandleRequests()
{
	std::thread t(&Communicator::bindAndListen, this);
	t.detach();
}
