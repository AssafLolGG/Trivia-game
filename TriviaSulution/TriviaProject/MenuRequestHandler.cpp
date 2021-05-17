#include "MenuRequestHandler.h"

/* function that checks if the request is relevant
	input: the client's request information.
	output: if the request is relevant or not. */
bool MenuRequestHandler::isRequestRelevant(RequestInfo& info)
{
	return info.id == LOGOUT_CODE || info.id == GET_STATISTICS_CODE || info.id == GET_JOIN_ROOMS_CODE
		|| info.id == GET_PLAYERS_IN_ROOM_CODE || info.id == GET_ROOMS_CODE || info.id == GET_CREATE_ROOMS_CODE;
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

	return result;
}

/* signing out */
RequestResult MenuRequestHandler::signout(RequestInfo info)
{
	LogoutResponse logout;
	RequestResult result;

	logout.status = STATUS_OK; // double check that one.

	result.newHandler = this->m_handler_factory.createLoginRequestHandler(); // return login request after signing out
	result.respone = JsonResponsePacketSerializer::serializeResponse(logout);

	return result;
}

/* getting all the rooms */
RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
	GetRoomsResponse get_rooms;
	RequestResult result;
	
	get_rooms.rooms = m_room_manager.getRooms();

	result.newHandler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(get_rooms);

	return result;
}

/* getting number of players in a room */
RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
	GetPlayersInRoomResponse players_response;
	RequestResult result;

	players_response.players = this->m_room_manager.getRoom(info.id).getAllUsers();

	result.newHandler = new MenuRequestHandler(*this);
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

	result.newHandler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(stats);

	return result;
}

/* getting user high score */
RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
	GetPersonalStatsResponse stats;
	RequestResult result;

	stats.statistics = this->m_statistics_manager.getHighScore();
	stats.status = STATUS_OK;

	result.newHandler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(stats);

	return result;
}

/* joining room */
RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
	JoinRoomResponse jroom;
	RequestResult result;

	this->m_room_manager.getRoom(info.id).addUser(this->m_user);
	jroom.status = STATUS_OK;

	result.newHandler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(jroom);

	return result;
}

/* creating room */
RequestResult MenuRequestHandler::createRoom(RequestInfo info)
{
	CreateRoomResponse croom_response;
	RequestResult result;
	RoomData room_data;
	CreateRoomRequest croom_request;

	croom_request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.buffer);

	room_data.id = info.id;
	room_data.isActive = true;
	room_data.maxPlayers = croom_request.max_users;
	room_data.name = croom_request.room_name;
	room_data.timePerQuestion = croom_request.answer_time_out;
	room_data.numOfQuestionsInGame = croom_request.question_count;

	this->m_room_manager.createRoom(this->m_user, room_data);

	croom_response.status = STATUS_OK;

	result.newHandler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(croom_response);

	return result;
}
