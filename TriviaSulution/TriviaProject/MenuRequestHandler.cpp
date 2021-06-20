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

	if (info.id == LOGOUT_CODE) // logout
	{
		result = this->signout(info);
	}
	else if (info.id == GET_STATISTICS_CODE) // getting personal stats.
	{
		result = this->getPersonalStats(info);
	}
	else if (info.id == GET_JOIN_ROOMS_CODE) // joining certain room.
	{
		result = this->joinRoom(info);
	}
	else if (info.id == GET_PLAYERS_IN_ROOM_CODE) // gets all the players in certain room.
	{
		result = this->getPlayersInRoom(info);
	}
	else if (info.id == GET_ROOMS_CODE) // gets all the rooms existed.
	{
		result = this->getRooms(info);
	}
	else if (info.id == GET_CREATE_ROOMS_CODE) // create room, for user.
	{
		result = this->createRoom(info);
	}
	else if (info.id == GET_ROOMDATA_CODE) // gets the data about a certain room.
	{
		result = this->getRoomData(info);
	}
	else if (info.id == GET_TOP_FIVE_CODE) // gets the top five scores in the statistics of users. 
	{
		result = this->topFivePlayer(info);
	}

	return result;
}

/*
function to get the type of the request handler.
input: None.
output: the id of the handler.
*/
int MenuRequestHandler::GetRequestHandlerType()
{
	return MENU_REQUEST_HANDLER_ID;
}

/* signing out */
RequestResult MenuRequestHandler::signout(RequestInfo info)
{
	LogoutResponse logout;
	RequestResult result;
	Room* room = new Room;
	logout.status = STATUS_OK; // double check that one.

	// gets the new handler(login handler), and serializing the response to the user. 
	result.new_handler = this->m_handler_factory.createLoginRequestHandler(this->m_socket); 
	result.respone = JsonResponsePacketSerializer::serializeResponse(logout);

	// logs the user out of the login manager list of logged users
	this->m_handler_factory.getLoginManager().logout(this->m_user.getUserName());
	
	return result;
}

/* getting all the rooms */
RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
	GetRoomsResponse get_rooms;
	RequestResult result;
	
	// gets a list of all the rooms.
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
	
	// deserializing the user request to get the room id, to get the players from
	players_request = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(info.buffer);

	// gets the room
	Room* room = this->m_room_manager.getRoom(players_request.room_id);

	// gets if the room existed
	if (room != nullptr)
	{
		// gets all the players in the room
		players_response.players = (room)->getAllUsers();
	}
	else
	{
		// returns empty vector of players if the room is non-existend
		players_response.players = std::vector<string>();
	}
	
	// sets the user's new handler, and his response.
	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(players_response);

	return result;
}

/* getting user personal stats */
RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info)
{
	GetPersonalStatsResponse stats;
	RequestResult result;

	// gets the user's statistics
	stats.statistics = this->m_statistics_manager.getUserStatistics(this->m_user.getUserName());
	stats.status = STATUS_OK;

	// sets the user's new handler, and serializing his response related
	// to the get personal stats request.
	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(stats);

	return result;
}

/* joining room */
RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
	JoinRoomResponse join_room;
	RequestResult result;
	JoinRoomRequest join_room_request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.buffer);
	
	// get's the room's the user trying to log into
	Room* room = this->m_room_manager.getRoom(join_room_request.room_id);
	
	// gets if the room exists
	if (room != nullptr)
	{
		// getting all the users within the room
		std::vector<string> all_users_in_room = (room)->getAllUsers();

		// checking if the room is valid, to join(isn't max capacity etc..)
		if (all_users_in_room.size() < (room)->GetRoomdata().maxPlayers &&
			(std::find(all_users_in_room.begin(), all_users_in_room.end(), this->m_user.getUserName()) == all_users_in_room.end())) // if user name NOT found
		{
			// adding user to room
			join_room.status = (room)->addUser(this->m_user, this->m_socket) == true ? STATUS_OK : STATUS_FAIL; 
		}
		else // if the room isn't joinable, return that the join room attempt failed
		{
			join_room.status = STATUS_FAIL;
		}
	}
	else // if the room is non-existened, return that the join room request failed.
	{
		join_room.status = STATUS_FAIL;
	}

	// if the user succefully joined the room.
	if (join_room.status == STATUS_OK) 
	{
		result.new_handler = this->m_handler_factory.createRoomMemberRequestHandler(this->m_user, join_room_request.room_id, this->m_socket);
	}
	else // if the user didn't joined the desired room
	{
		result.new_handler = new MenuRequestHandler(*this);
	}

	// serializing the user's response related to his join-room request.
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

	// checks if the room has max player greater than 0, time to answer till time out greater than 0 seconds, and
	// question count that is greater than 0.
	if (create_room_request.max_users > 0 && create_room_request.answer_time_out > 0 && create_room_request.question_count > 0) 
	{
		this->m_room_manager.createRoom(this->m_user, room_data, this->m_socket); // creating new room

		create_room_response.status = STATUS_OK;
		create_room_response.id = room_data.id;
		result.new_handler = this->m_handler_factory.createRoomAdminRequestHandler(this->m_user, room_data.id, this->m_socket);
	}
	else // if the room's stats aren't valid.
	{
		create_room_response.status = STATUS_FAIL;
		create_room_response.id = 0;
		result.new_handler = new MenuRequestHandler(*this);
	}
	result.respone = JsonResponsePacketSerializer::serializeResponse(create_room_response); // creating response

	return result;
}

RequestResult MenuRequestHandler::getRoomData(RequestInfo info)
{
	GetRoomDataRequest roomDataRequest;
	GetRoomDataResponse roomDataResponse;
	RequestResult result;

	// deserializing the get-room-data request to get the id of the room.
	roomDataRequest = JsonRequestPacketDeserializer::deserializeRoomDataRequest(info.buffer);
	int theRoomId = roomDataRequest.room_id;

	// gets the room to get the data from.
	Room* theRoom = this->m_room_manager.getRoom(theRoomId);

	// checks if the room exists
	if (theRoomId != INVALID_INDEX && theRoom != nullptr)
	{
		// insert the room data inside a structure.
		RoomData roomdataOfTheRoom = theRoom->GetRoomdata();
		roomDataResponse.status = STATUS_OK;
		roomDataResponse.id = theRoomId;
		roomDataResponse.isActive = roomdataOfTheRoom.isActive;
		roomDataResponse.maxPlayers = roomdataOfTheRoom.maxPlayers;
		roomDataResponse.name = roomdataOfTheRoom.name;
		roomDataResponse.numOfQuestionsInGame = roomdataOfTheRoom.numOfQuestionsInGame;
		roomDataResponse.timePerQuestion = roomdataOfTheRoom.timePerQuestion;
	}
	else // if the room's is non existence, returns that the attempt to get data failed.
	{
		roomDataResponse.status = STATUS_FAIL;
	}

	// sets the user's new handler, and his serialized response
	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(roomDataResponse);

	return result;
}

/* function to get the top five players with the highest score. */
RequestResult MenuRequestHandler::topFivePlayer(RequestInfo info)
{
	TopFivePlayers top_five;
	RequestResult result;

	// gets the top five scores among all users.
	top_five = this->m_statistics_manager.getHighScore();

	// sets the user's new handler, and his serialized response
	result.new_handler = new MenuRequestHandler(*this);
	result.respone = JsonResponsePacketSerializer::serializeResponse(top_five);

	return result;
}
