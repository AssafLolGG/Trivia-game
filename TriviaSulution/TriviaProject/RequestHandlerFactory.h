#pragma once

#include "LoginManager.h"
#include "Room.h"
#include "StatisticsManager.h"
#include "LoginRequestHandler.h"

class LoginRequestHandler;
class RequestHandlerFactory;

class RequestHandlerFactory
{
private:
	LoginManager m_login_manager;
	IDatabase* m_database;
	RoomManager m_room_manager;
	StatisticsManager m_statistics_manager;
public:
	RequestHandlerFactory(IDatabase* db);
	LoginManager& getLoginManager();
	LoginRequestHandler* createLoginRequestHandler();
};