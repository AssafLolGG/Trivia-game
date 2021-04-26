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
	LoginRequestHandler * client_handler = new LoginRequestHandler;
	std::string server_message = STARTER_SERVER_MESSAGE;
	char* message_buffer = new char[BUFFER_CAPACITY];
	std::vector<uint8_t> buffer_vector;
	this->m_clients.insert(std::pair<SOCKET, IRequestHandler*>(client_socket, client_handler));
	try
	{
		// sending starter message to client
		send(client_socket, server_message.c_str(), server_message.size(), 0);
		recv(client_socket, message_buffer, BUFFER_CAPACITY - 1, 0);
		std::cout << message_buffer << std::endl;
		while (true)
		{
			recv(client_socket, message_buffer, BUFFER_CAPACITY - 1, 0);
			CharPointerToVector(message_buffer, BUFFER_CAPACITY, buffer_vector);
			// checks the wanted request from user, to send a proper response.
			if (buffer_vector[0] == LOGIN_CODE)
			{
				LoginRequest loginRequestFromClient = JsonRequestPacketDeserializer::deserializeLoginRequest(buffer_vector);
				LoginResponse theLoginResponse{ STATUS_OK }; // CHECKS if the login request from the client is valid.
				// Serializing the response.
				std::vector<uint8_t> serializedResponse = JsonResponsePacketSerializer::serializeResponse(theLoginResponse);
				char serializedResponseInCharArray[BUFFER_CAPACITY] = { 0 };
				vectorToCharArray(serializedResponseInCharArray, serializedResponse);
				// sends the serialized response from the server to the client, after analyizing his response.
				send(client_socket, serializedResponseInCharArray, BUFFER_CAPACITY, 0);
			}
			else if (buffer_vector[0] == SIGNUP_CODE)
			{
				SignupRequest loginRequestFromClient = JsonRequestPacketDeserializer::deserializeSignupRequest(buffer_vector);
				SignupResponse theLoginResponse{ STATUS_OK }; // CHECKS if the login request from the client is valid.
				// Serializing the response.
				std::vector<uint8_t> serializedResponse = JsonResponsePacketSerializer::serializeResponse(theLoginResponse);
				char serializedResponseInCharArray[BUFFER_CAPACITY] = { 0 };
				vectorToCharArray(serializedResponseInCharArray, serializedResponse);
				// sends the serialized response from the server to the client, after analyizing his response.
				send(client_socket, serializedResponseInCharArray, BUFFER_CAPACITY, 0);
			}

		}
		
	}
	catch (const std::exception& e)
	{
		closesocket(client_socket); // closing the client socket
	}

}

Communicator::Communicator()
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(6969); // port that server will listen for
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
