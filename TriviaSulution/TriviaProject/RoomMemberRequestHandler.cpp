#include "RoomMemberRequestHandler.h"

/**/
RequestResult RoomMemberRequestHandler::leaveGame(RequestInfo& info, bool is_exit_from_program)
{
	Room* the_room = this->m_room_manager.getRoom(room_id);
	RequestResult result;

	// checks if the user wants to leave the game but still want to stay in the program
	if (!is_exit_from_program)
	{
		result.new_handler = this->m_handler_factory.createMenuRequestHandler(this->m_user, this->m_client);
	}
	else
	{
		result.new_handler = this->m_handler_factory.createLoginRequestHandler(this->m_client);
		
		// logout the logged user list in the login manager
		this->m_handler_factory.getLoginManager().logout(this->m_user.getUserName()); 
	}

	LeaveRoomResponse leave_room;

	// gets if the room the user attempting to leave, is valid to leave from(is the user even in the room?)
	if (room_id != INVALID_INDEX && the_room != NULL)
	{
		// gets all the users in the room, and gets if the user is one of them
		std::vector<string> users_in_room = the_room->getAllUsers();
		leave_room.status = std::find(users_in_room.begin(), users_in_room.end(), this->m_user.getUserName()) != users_in_room.end() ? STATUS_OK : STATUS_FAIL;
	}
	else
	{
		leave_room.status = STATUS_FAIL;
	}

	// serializing the response to the user
	result.respone = JsonResponsePacketSerializer::serializeResponse(leave_room);

	// gets if the leave room attempt failed and then returns from the function.
	if (leave_room.status == STATUS_FAIL)
	{
		return result;
	}

	// removes the user from the user's players list.
	the_room->removeUser(this->m_user, this->m_client);
	return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(RequestInfo& info)
{
	// gets all the data about the room the user's in.
	GetRoomDataResponse room_data = getRoomData(this->m_room_manager, this->room_id, this->m_user);
	RequestResult result;

	result.new_handler = new RoomMemberRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(room_data);

	return result;
}

RequestResult RoomMemberRequestHandler::getPlayersInRoom(RequestInfo& info)
{
	// gets the room, which the user's currently in
	Room* the_room = this->m_room_manager.getRoom(this->room_id);
	GetPlayersInRoomResponse players_in_room_response;
	RequestResult result;

	// and gets all the users within this room.
	players_in_room_response.players = the_room->getAllUsers();

	result.new_handler = new RoomMemberRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(players_in_room_response);

	return result;
}

/* checking if the request code match the codes that the room member accept*/
bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo& info)
{
	return info.id == LEAVE_ROOM_CODE || info.id == GET_ROOMDATA_CODE || info.id == GET_PLAYERS_IN_ROOM_CODE;
}

/* checking the type of request and handling it */
RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo& info)
{
	RequestResult result;

	// checking type of request, and handling it, depending it's type.
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

/*
function to get the type of the request handler.
input: None.
output: the id of the handler.
*/
int RoomMemberRequestHandler::GetRequestHandlerType()
{
	return ROOM_MEMBER_REQUEST_HANDLER_ID;
}
