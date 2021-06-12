#pragma once

#include "LoginManager.h"
#include "Room.h"
#include "StatisticsManager.h"
#include "LoginRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"

class GameRequestHandler;
class LoginRequestHandler;
class RequestHandlerFactory;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class RequestHandlerFactory
{
private:
	LoginManager m_login_manager;
	IDatabase* m_database;
	RoomManager m_room_manager;
	StatisticsManager m_statistics_manager;
	GameManager m_game_manager;
public:
	RequestHandlerFactory(IDatabase* db);
	LoginManager& getLoginManager();
	LoginRequestHandler* createLoginRequestHandler(SOCKET client_socket);
	MenuRequestHandler* createMenuRequestHandler(LoggedUser user, SOCKET client_socket);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser user, int room_id, SOCKET client_socket);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser user, int room_id, SOCKET client_socket);
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();
	GameRequestHandler* createGameRequestHandler(LoggedUser user, Game& game);
	GameManager& getGameManager();
};