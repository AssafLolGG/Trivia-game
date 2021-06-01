#include "MenuRequestHandler.h"

/* function that checks if the request is relevant
	input: the client's request information.
	output: if the request is relevant or not. */
bool MenuRequestHandler::isRequestRelevant(RequestInfo& info)
{
	return info.id == LOGOUT_CODE || info.id == GET_STATISTICS_CODE || info.id == GET_JOIN_ROOMS_CODE || info.id == GET_TOP_FIVE_CODE
		|| info.id == GET_PLAYERS_IN_ROOM_CODE || info.id == GET_ROOMS_CODE || info.id == GET_CREATE_ROOMS_CODE || info.id == GET_ROOMDATA_CODE;
}

/*	function that handles client request
	input: the client's request information.
	output: the response to the user about the user's request. */
RequestResult MenuRequestHandler::handleRequest(RequestInfo& info)
{
	RequestResult result;

	if (info.id == LOGOUT_CODE)
	{
		result = this->signout(info);
	}
	else if (info.id == GET_STATISTICS_CODE)
	{
		result = this->getPersonalStats(info);
	}
	else if (info.id == GET_JOIN_ROOMS_CODE)
	{
		result = this->joinRoom(info);
	}
	else if (info.id == GET_PLAYERS_IN_ROOM_CODE)
	{
		result = this->getPlayersInRoom(info);
	}
	else if (info.id == GET_ROOMS_CODE)
	{
		result = this->getRooms(info);
	}
	else if (info.id == GET_CREATE_ROOMS_CODE)
	{
		result = this->createRoom(info);
	}
	else if (info.id == GET_ROOMDATA_CODE)
	{
		result = this->getRoomData(info);
	}
	else if (info.id == GET_TOP_FIVE_CODE)
	{
		result = this->topFivePlayer(info);
	}

	return result;
}

/* signing out */
RequestResult MenuRequestHandler::signout(RequestInfo info)
{
	LogoutResponse logout;
	RequestResult result;
	Room* room = new Room;
	logout.status = STATUS_OK; // double check that one.

	result.new_handler = this->m_handler_factory.createLoginRequestHandler(this->m_socket); // return login request after signing out
	result.respone = JsonResponsePacketSerializer::serializeResponse(logout);
	for (auto it = this->m_room_manager.getAllRooms().begin(); it != this->m_room_manager.getAllRooms().end(); it++)
	{
		it->second.removeUser(this->m_user, this->m_socket);
	}
	this->m_handler_factory.getLoginManager().logout(this->m_user.getUserName());
	
	return result;
}

/* getting all the rooms */
RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
	GetRoomsResponse get_rooms;
	RequestResult result;
	
	get_rooms.status = STATUS_OK;
	get_rooms.rooms = m_room_manager.getRooms();
	

	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(get_rooms);

	return result;
}

/* getting number of players in a room */
RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
	GetPlayersInRoomResponse players_response;
	GetPlayersInRoomRequest players_request;
	RequestResult result;
	Room* room = new Room;
	players_request = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(info.buffer);
	if (this->m_room_manager.getRoom(players_request.room_id, room))
	{
		players_response.players = (room)->getAllUsers();
	}
	else
	{
		players_response.players = std::vector<string>();
	}
	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(players_response);

	return result;
}

/* getting user personal stats */
RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info)
{
	GetPersonalStatsResponse stats;
	RequestResult result;

	stats.statistics = this->m_statistics_manager.getUserStatistics(this->m_user.getUserName());
	stats.status = STATUS_OK;

	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(stats);

	return result;
}

///* getting user high score */
//RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
//{
//	GetPersonalStatsResponse stats;
//	RequestResult result;
//
//	stats.statistics = this->m_statistics_manager.getHighScore();
//	stats.status = STATUS_OK;
//
//	result.newHandler = new MenuRequestHandler(*this);
//	result.respone = JsonResponsePacketSerializer::serializeResponse(stats);
//
//	return result;
//}

/* joining room */
RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
	JoinRoomResponse join_room;
	RequestResult result;
	JoinRoomRequest join_room_request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
	Room* room = nullptr;
	if (this->m_room_manager.getRoom(join_room_request.room_id, room))
	{
		std::vector<string> all_users_in_room = (room)->getAllUsers();


		if (all_users_in_room.size() < (room)->GetRoomdata().maxPlayers &&
			(std::find(all_users_in_room.begin(), all_users_in_room.end(), this->m_user.getUserName()) == all_users_in_room.end())) // if user name NOT found - assaf had pron
		{
			(room)->addUser(this->m_user, this->m_socket); // adding user to room
			join_room.status = STATUS_OK;
		}
		else
		{
			join_room.status = STATUS_FAIL;
		}
	}
	else
	{
		join_room.status = STATUS_FAIL;
	}
	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(join_room);

	return result;
}

/* creating room */
RequestResult MenuRequestHandler::createRoom(RequestInfo info)
{
	CreateRoomResponse create_room_response; // making a response object
	RequestResult result;
	RoomData room_data; // making room data object, to fill in the data in create room function
	CreateRoomRequest create_room_request; // making create room request object, to get the info from the deserializer

	create_room_request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer); // getting info to CreateRoomRequest object

	// filling in the data for RoomData object
	room_data.id = this->m_room_manager.getRoomCount() + 1;
	room_data.isActive = true;
	room_data.maxPlayers = create_room_request.max_users;
	room_data.name = create_room_request.room_name;
	room_data.timePerQuestion = create_room_request.answer_time_out;
	room_data.numOfQuestionsInGame = create_room_request.question_count;

	this->m_room_manager.createRoom(this->m_user, room_data, this->m_socket); // creating new room

	create_room_response.status = STATUS_OK;

	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(create_room_response); // creating response

	return result;
}

RequestResult MenuRequestHandler::getRoomData(RequestInfo info)
{
	GetRoomDataRequest roomDataRequest;
	GetRoomDataResponse roomDataResponse;
	RequestResult result;
	Room* theRoom = new Room();
	

	roomDataRequest = JsonRequestPacketDeserializer::deserializeRoomDataRequest(info.buffer);
	int theRoomId = roomDataRequest.room_id;
	if (theRoomId != INVALID_INDEX && this->m_room_manager.getRoom(theRoomId, theRoom))
	{
		RoomData roomdataOfTheRoom = theRoom->GetRoomdata();
		roomDataResponse.status = STATUS_OK;
		roomDataResponse.id = theRoomId;
		roomDataResponse.isActive = roomdataOfTheRoom.isActive;
		roomDataResponse.maxPlayers = roomdataOfTheRoom.maxPlayers;
		roomDataResponse.name = roomdataOfTheRoom.name;
		roomDataResponse.numOfQuestionsInGame = roomdataOfTheRoom.numOfQuestionsInGame;
		roomDataResponse.timePerQuestion = roomdataOfTheRoom.timePerQuestion;
	}
	else
	{
		roomDataResponse.status = STATUS_FAIL;
	}

	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(roomDataResponse);

	return result;
}

RequestResult MenuRequestHandler::topFivePlayer(RequestInfo info)
{
	TopFivePlayers top_five;
	RequestResult result;

	top_five = this->m_statistics_manager.getHighScore();

	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(top_five);

	return result;
}
