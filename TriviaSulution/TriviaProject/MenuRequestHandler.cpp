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

	return RequestResult();
}

RequestResult MenuRequestHandler::signout(RequestInfo info)
{
	return RequestResult();
}

RequestResult MenuRequestHandler::getRooms(RequestInfo info)
{
	return RequestResult();
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo info)
{
	return RequestResult();
}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo info)
{
	return RequestResult();
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo info)
{
	return RequestResult();
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo info)
{
	return RequestResult();
}

RequestResult MenuRequestHandler::createRoom(RequestInfo info)
{
	return RequestResult();
}
