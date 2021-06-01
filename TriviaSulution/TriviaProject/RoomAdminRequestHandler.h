#pragma once
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"

class RoomAdminRequestHandler : public IRequestHandler
{
private:
	LoggedUser m_user;
	RoomManager& m_room_manager;
	int room_id;
	RequestHandlerFactory& m_handler_factory;

	RequestResult startGame(RequestInfo& info);
	RequestResult closeGame(RequestInfo& info);
	RequestResult getRoomState(RequestInfo& info);

public:
	RoomAdminRequestHandler(RoomManager& room_manager,int _room_id, RequestHandlerFactory& handler_factory, LoggedUser m_user_) : m_room_manager(room_manager),
		room_id(_room_id), m_handler_factory(handler_factory), IRequestHandler()
	{
		this->m_user = m_user_;
	}
	bool isRequestRelevant(RequestInfo& info) override;
	RequestResult handleRequest(RequestInfo& info) override;
	
};

