#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "LoginManager.h"
#include "ServerUtilities.h"
class RequestHandlerFactory;
class RoomMemberRequestHandler : public IRequestHandler 
{
private:
	SOCKET m_client;
	LoggedUser m_user;
	RoomManager& m_room_manager;
	int room_id;
	RequestHandlerFactory& m_handler_factory;

	RequestResult getRoomState(RequestInfo& info);
	RequestResult getPlayersInRoom(RequestInfo& info);

public:
	RoomMemberRequestHandler(RoomManager& room_manager, int _room_id, RequestHandlerFactory& handler_factory, LoggedUser m_user_, SOCKET client_socket) : m_room_manager(room_manager), m_handler_factory(handler_factory)
	{
		this->m_client = client_socket;
		this->room_id = _room_id;
		this->m_user = m_user_;
	}
	bool isRequestRelevant(RequestInfo& info) override;
	RequestResult handleRequest(RequestInfo& info) override;
	int GetRequestHandlerType() override;
	RequestResult leaveGame(RequestInfo& info);
};

