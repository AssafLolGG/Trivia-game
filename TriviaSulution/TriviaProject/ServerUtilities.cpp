#include "ServerUtilities.h"

/*
function that recieves the message from the client.
input: the socket of the client, and a buffer to store within the message
from the client through the socket.
output: if the message recieved or no from the socket.
*/
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

/*
function that creates request info out of buffer sent by a socket.
input: the buffer sent in the socket.
output: the request info describing the request the user 
requested.
*/
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

/*
function that sets a handler to certain client.
input: the handler to set, the clients-handlers map, and the client socket.
output: None.
*/
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

/*
function that gets the current handler of a client.
input: the sockets-handlers map, and the client socket.
output: the handler of the client.
*/
IRequestHandler* getHandlerOfClient(std::map<SOCKET, IRequestHandler*>& clients, SOCKET& client_socket)
{
	for (auto it = clients.begin(); it != clients.end(); ++it) // going through all of the clients
	{
		if (it->first == client_socket) // finding the client that used the function
		{
			return it->second; // inserting the current request handler
		}
	}
}

/*
function that gets the room data of certain room.
input: the room manager, id, and the user logged to the room.
output: The response that is the result of such action.
*/
GetRoomDataResponse getRoomData(RoomManager& room_manager, int room_id, LoggedUser logged)
{
	GetRoomDataResponse roomDataResponse;
	Room* theRoom = room_manager.getRoom(room_id);

	// gets the roomdata, into a structure
	if (room_id != INVALID_INDEX && theRoom)
	{
		std::vector<string> usersInRoom = theRoom->getAllUsers();
		
		// finds if the user is even in the desired room
		if (std::find(usersInRoom.begin(), usersInRoom.end(), logged.getUserName()) != usersInRoom.end())
		{
			RoomData roomdataOfTheRoom = theRoom->GetRoomdata();
			roomDataResponse.status = STATUS_OK;
			roomDataResponse.id = room_id;
			roomDataResponse.isActive = roomdataOfTheRoom.isActive;
			roomDataResponse.maxPlayers = roomdataOfTheRoom.maxPlayers;
			roomDataResponse.name = roomdataOfTheRoom.name;
			roomDataResponse.numOfQuestionsInGame = roomdataOfTheRoom.numOfQuestionsInGame;
			roomDataResponse.timePerQuestion = roomdataOfTheRoom.timePerQuestion;
		}
		else // if the user isn't in the room
		{
			roomDataResponse.status = STATUS_FAIL;
		}
	}
	else
	{
		roomDataResponse.status = STATUS_FAIL;
	}
	return roomDataResponse;
}
