#include "RoomAdminRequestHandler.h"

RequestResult RoomAdminRequestHandler::startGame(RequestInfo& info)
{
	Room* theRoom = new Room();
	RequestResult result;
	result.newHandler = new RoomAdminRequestHandler(*this);
	StartRoomResponse startRoom;
	
	if (room_id != INVALID_INDEX && this->m_room_manager.getRoom(room_id, theRoom))
	{
		std::vector<string> usersInRoom = theRoom->getAllUsers();
		startRoom.status = std::find(usersInRoom.begin(), usersInRoom.end(), this->m_user.getUserName()) != usersInRoom.end() ? STATUS_OK : STATUS_FAIL ;
		if (startRoom.status != STATUS_FAIL)
		{
			result.socketsToSendTo = theRoom->getAllSockets();
		}
	}
	else
	{
		startRoom.status = STATUS_FAIL;
	}
	result.respone = JsonResponsePacketSerializer::serializeResponse(startRoom);
	result.responseToOthers = JsonResponsePacketSerializer::serializeResponse(startRoom);
	for (int i = 0; i < result.socketsToSendTo.size(); i++)
	{
		LoggedUser logged(theRoom->getAllUsers()[i]);
		result.RequestHandlerToAll.push_back(this->m_handler_factory.createMenuRequestHandler(logged, result.socketsToSendTo[i]));
	}
	return result;
}

RequestResult RoomAdminRequestHandler::closeGame(RequestInfo& info)
{
	Room* theRoom = new Room();
	RequestResult result;
	result.newHandler = this->m_handler_factory.createMenuRequestHandler(this->m_user, this->m_client);
	CloseRoomResponse closeRoom;

	if (room_id != INVALID_INDEX && this->m_room_manager.getRoom(room_id, theRoom))
	{
		std::vector<string> usersInRoom = theRoom->getAllUsers();
		closeRoom.status = std::find(usersInRoom.begin(), usersInRoom.end(), this->m_user.getUserName()) != usersInRoom.end() ? STATUS_OK : STATUS_FAIL;
		if (closeRoom.status != STATUS_FAIL)
		{
			result.socketsToSendTo = theRoom->getAllSockets();
		}
	}
	else
	{
		closeRoom.status = STATUS_FAIL;
	}
	result.respone = JsonResponsePacketSerializer::serializeResponse(closeRoom);
	result.responseToOthers = JsonResponsePacketSerializer::serializeResponse(closeRoom);
	if (closeRoom.status == STATUS_FAIL)
	{
		return result;
	}
	for (int i = 0; i < result.socketsToSendTo.size(); i++)
	{
		LoggedUser logged(theRoom->getAllUsers()[i]);
		result.RequestHandlerToAll.push_back(this->m_handler_factory.createMenuRequestHandler(logged, result.socketsToSendTo[i]));
	}
	for (int i = 0; i < result.socketsToSendTo.size(); i++)
	{
		LoggedUser logged(theRoom->getAllUsers()[i]);
		theRoom->removeUser(logged, result.socketsToSendTo[i]);
	}
	return result;
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo& info)
{
	GetRoomDataResponse roomdata = getRoomData(this->m_room_manager, this->room_id, this->m_user);
	RequestResult result;
	result.newHandler = new RoomAdminRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(roomdata);
	return result;
}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo& info)
{
	return info.id == CLOSE_ROOM_CODE || info.id == START_ROOM_CODE || info.id == GET_ROOMDATA_CODE;
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo& info)
{
	RequestResult result;

	// checking type of request
	if (info.id == START_ROOM_CODE)
	{
		result = this->startGame(info);
	}
	else if (info.id == GET_ROOMDATA_CODE)
	{
		result = this->getRoomState(info);
	}
	else if(info.id == CLOSE_ROOM_CODE)
	{
		result = this->closeGame(info);
	}
	return result;
}
