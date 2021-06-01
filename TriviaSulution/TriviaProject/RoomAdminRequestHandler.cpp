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
	}
	else
	{
		startRoom.status = STATUS_FAIL;
	}
	result.respone = JsonResponsePacketSerializer::serializeResponse(startRoom);
	return result;
}

RequestResult RoomAdminRequestHandler::closeGame(RequestInfo& info)
{
	Room* room = new Room();
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
		
	}
	else if (info.id == GET_ROOMDATA_CODE)
	{
	}
	else if(info.id == CLOSE_ROOM_CODE)
	{
	}
	return result;
}
