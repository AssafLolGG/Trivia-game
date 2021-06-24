#include "RoomAdminRequestHandler.h"

/**/
RequestResult RoomAdminRequestHandler::startGame(RequestInfo& info)
{
	Room* the_room = this->m_room_manager.getRoom(this->room_id);
	RequestResult result;
	StartRoomResponse start_room;
	if (this->room_id != INVALID_INDEX && the_room != nullptr) // checking if the room id is valid, getting the current room for the server and checking if the room exists
	{
		std::vector<string> users_in_room = the_room->getAllUsers();

		// checks if the player that created the room is in the room
		start_room.status = std::find(users_in_room.begin(), users_in_room.end(), this->m_user.getUserName()) != users_in_room.end() ? STATUS_OK : STATUS_FAIL;

		// gets if the user is even in the room
		if (start_room.status != STATUS_FAIL)
		{
			the_room->GetRoomdata().isActive = 0;

			// gets the game the admin created, in order to start the game.
			Game& gameCreated = this->m_handler_factory.getGameManager().createGame(*the_room);
			
			// gets data for the result of the request
			result.players_in_room_sockets = the_room->getAllSockets();
			result.response_to_other_players = JsonResponsePacketSerializer::serializeResponse(start_room);
			result.new_handler = this->m_handler_factory.createGameRequestHandler(this->m_user, gameCreated, this->m_client);
			
			// assigning new handlers to all players
			for (int i = 0; i < result.players_in_room_sockets.size(); i++)
			{
				// give all the new players game request handler since the room's admin started the game.
				LoggedUser logged(the_room->getAllUsers()[i]);
				result.players_in_room_request_handlers.push_back(this->m_handler_factory.createGameRequestHandler(logged, gameCreated, result.players_in_room_sockets[i]));
			}
		}
	}
	else // if the room isn't valid.
	{
		start_room.status = STATUS_FAIL;
	}

	// serializing the response to the user.
	result.respone = JsonResponsePacketSerializer::serializeResponse(start_room);

	return result;
}

/* closing the game and the room */
RequestResult RoomAdminRequestHandler::closeGame(RequestInfo& info, bool is_exit_from_program)
{
	// function that gets the room, the user's in
	Room* the_room = this->m_room_manager.getRoom(room_id);
	RequestResult result;

	// checks if the user wants to exit the game, and not the program
	if (!is_exit_from_program)
	{
		result.new_handler = this->m_handler_factory.createMenuRequestHandler(this->m_user, this->m_client);
	}
	else // if the user wants to exit the program
	{
		result.new_handler = this->m_handler_factory.createLoginRequestHandler(this->m_client);
		this->m_handler_factory.getLoginManager().logout(this->m_user.getUserName());
	}
	CloseRoomResponse close_room;

	// checking if the room id is valid, getting the current room for the server and checking if the room exists
	if (room_id != INVALID_INDEX && the_room != nullptr)
	{
		// checks if the user is in the room, he requesting.
		std::vector<string> users_in_room = the_room->getAllUsers();

		close_room.status = std::find(users_in_room.begin(), users_in_room.end(), this->m_user.getUserName()) != users_in_room.end() ? STATUS_OK : STATUS_FAIL;

		if (close_room.status != STATUS_FAIL)
		{
			result.players_in_room_sockets = the_room->getAllSockets();
		}
	}
	else // if the room's the user requesting to close isn't valid to close(the user isn't the room etc..)
	{
		close_room.status = STATUS_FAIL;
	}

	// serializing the response to the user and other users.
	result.respone = JsonResponsePacketSerializer::serializeResponse(close_room);
	result.response_to_other_players = JsonResponsePacketSerializer::serializeResponse(close_room);

	if (close_room.status == STATUS_FAIL)
	{
		return result;
	}

	// sets the new handlers to the other users, in the result of the request.
	for (int i = 0; i < result.players_in_room_sockets.size(); i++)
	{
		LoggedUser logged(the_room->getAllUsers()[i]);
		result.players_in_room_request_handlers.push_back(this->m_handler_factory.createMenuRequestHandler(logged, result.players_in_room_sockets[i]));
	}

	// Remove all users from room
	for (int i = 0; the_room->getAllUsers().size() != 0; i++)
	{
		LoggedUser logged(the_room->getAllUsers()[0]);
		the_room->removeUser(logged, result.players_in_room_sockets[i]);
	}

	return result;
}

/* getting room state, can be active or not active */
RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo& info)
{
	GetRoomDataResponse roomdata = getRoomData(this->m_room_manager, this->room_id, this->m_user);
	RequestResult result;

	result.new_handler = new RoomAdminRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(roomdata);
	return result;
}

RequestResult RoomAdminRequestHandler::getPlayersInRoom(RequestInfo& info)
{
	// gets all the players in the room, the user's in.
	Room* the_room = this->m_room_manager.getRoom(this->room_id);
	GetPlayersInRoomResponse players_in_room_response;
	RequestResult result;

	players_in_room_response.players = the_room->getAllUsers();

	result.new_handler = new RoomAdminRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(players_in_room_response);

	return result;
}

/* checking if the request code match the codes that the room admin accept */
bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo& info)
{
	return info.id == CLOSE_ROOM_CODE || info.id == START_ROOM_CODE || info.id == GET_ROOMDATA_CODE || info.id == GET_PLAYERS_IN_ROOM_CODE;
}

/* checking the type of request and handling it */
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
int RoomAdminRequestHandler::GetRequestHandlerType()
{
	return ROOM_ADMIN_REQUEST_HANDLER_ID;
}


