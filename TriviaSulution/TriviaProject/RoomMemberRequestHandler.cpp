#include "RoomMemberRequestHandler.h"

/**/
RequestResult RoomMemberRequestHandler::leaveGame(RequestInfo& info)
{
	Room* the_room = this->m_room_manager.getRoom(room_id);
	RequestResult result;
	result.new_handler = this->m_handler_factory.createMenuRequestHandler(this->m_user, this->m_client);
	LeaveRoomResponse leave_room;

	if (room_id != INVALID_INDEX && the_room != NULL)
	{
		std::vector<string> users_in_room = the_room->getAllUsers();
		leave_room.status = std::find(users_in_room.begin(), users_in_room.end(), this->m_user.getUserName()) != users_in_room.end() ? STATUS_OK : STATUS_FAIL;
	}
	else
	{
		leave_room.status = STATUS_FAIL;
	}

	result.respone = JsonResponsePacketSerializer::serializeResponse(leave_room);

	if (leave_room.status == 0)
	{
		return result;
	}

	the_room->removeUser(this->m_user, this->m_client);
	return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo& info)
{
	GetRoomDataResponse room_data = getRoomData(this->m_room_manager, this->room_id, this->m_user);
	RequestResult result;

	result.new_handler = new RoomMemberRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(room_data);

	return result;
}

RequestResult RoomMemberRequestHandler::getPlayersInRoom(RequestInfo& info)
{
	Room* the_room = this->m_room_manager.getRoom(this->room_id);
	GetPlayersInRoomResponse players_in_room_response;
	RequestResult result;

	

	players_in_room_response.players = the_room->getAllUsers();

	result.new_handler = new RoomMemberRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(players_in_room_response);

	return result;
}

/* checking if the request code match the codes that the room member accept*/
bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo& info)
{
	if (info.id == GET_QUESTION_CODE)
	{
		std::cout << "HERE!\n";
	}
	return info.id == LEAVE_ROOM_CODE || info.id == GET_ROOMDATA_CODE || info.id == GET_PLAYERS_IN_ROOM_CODE;
}

/* checking the type of request and handling it */
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
	else if (info.id == GET_PLAYERS_IN_ROOM_CODE)
	{
		result = this->getPlayersInRoom(info);
	}

	return result;
}

int RoomMemberRequestHandler::GetRequestHandlerType()
{
	return 3;
}
