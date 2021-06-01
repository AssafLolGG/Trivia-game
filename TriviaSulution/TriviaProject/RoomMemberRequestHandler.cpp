#include "RoomMemberRequestHandler.h"

RequestResult RoomMemberRequestHandler::leaveGame(RequestInfo& info)
{
	Room* theRoom = new Room();
	RequestResult result;
	result.newHandler = this->m_handler_factory.createMenuRequestHandler(this->m_user, this->m_client);
	LeaveRoomResponse leaveRoom;

	if (room_id != INVALID_INDEX && this->m_room_manager.getRoom(room_id, theRoom))
	{
		std::vector<string> usersInRoom = theRoom->getAllUsers();
		leaveRoom.status = std::find(usersInRoom.begin(), usersInRoom.end(), this->m_user.getUserName()) != usersInRoom.end() ? STATUS_OK : STATUS_FAIL;
	}
	else
	{
		leaveRoom.status = STATUS_FAIL;
	}
	result.respone = JsonResponsePacketSerializer::serializeResponse(leaveRoom);
	if (leaveRoom.status == 0)
	{
		return result;
	}
	theRoom->removeUser(this->m_user, this->m_client);
	return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo& info)
{
	GetRoomDataResponse roomdata = getRoomData(this->m_room_manager, this->room_id, this->m_user);
	RequestResult result;
	result.newHandler = new RoomMemberRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(roomdata);
	return result;
}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo& info)
{
	return info.id == LEAVE_ROOM_CODE || info.id == GET_ROOMDATA_CODE;
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo& info)
{
	RequestResult result;

	// checking type of request
	if (info.id == GET_ROOMDATA_CODE)
	{
		result = this->getRoomState(info);
	}
	else if (info.id == LEAVE_ROOM_CODE)
	{
		result = this->leaveGame(info);
	}
	return result;
}
