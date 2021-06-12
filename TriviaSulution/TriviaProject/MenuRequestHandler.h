#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;
class MenuRequestHandler;

class MenuRequestHandler : public IRequestHandler
{
	LoggedUser m_user;
	SOCKET m_socket;
	RoomManager& m_room_manager;
	StatisticsManager& m_statistics_manager;
	RequestHandlerFactory& m_handler_factory;

	RequestResult signout(RequestInfo info);
	RequestResult getRooms(RequestInfo info);
	RequestResult getPlayersInRoom(RequestInfo info);
	RequestResult getPersonalStats(RequestInfo info);
	RequestResult joinRoom(RequestInfo info);
	RequestResult createRoom(RequestInfo info);
	RequestResult getRoomData(RequestInfo info);
	RequestResult topFivePlayer(RequestInfo info);
public:
	MenuRequestHandler(RoomManager& room_manager, StatisticsManager& statistics_manager, RequestHandlerFactory& handler_factory, LoggedUser m_user_, SOCKET client_socket) : m_room_manager(room_manager),
		m_statistics_manager(statistics_manager), m_handler_factory(handler_factory), IRequestHandler()
	{
		this->m_socket = client_socket;
		this->m_user = m_user_;
	}
	bool isRequestRelevant(RequestInfo& info) override;
	RequestResult handleRequest(RequestInfo& info) override;
	int GetRequestHandlerType() override;
};

