#include "RoomAdminRequestHandler.h"

RequestResult RoomAdminRequestHandler::startGame(RequestInfo& info)
{
	return RequestResult();
}

RequestResult RoomAdminRequestHandler::closeGame(RequestInfo& info)
{
	return RequestResult();
}

RequestResult RoomAdminRequestHandler::getRoomState(RequestInfo& info)
{
	return RequestResult();
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
