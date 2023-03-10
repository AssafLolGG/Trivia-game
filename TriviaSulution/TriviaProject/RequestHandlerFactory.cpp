#include "RequestHandlerFactory.h"

/*  constractor function that gets the database of the system
    and initaliize his attribiuts.
    input: a database.
    output: an object of RequestHandlerFactory. */
RequestHandlerFactory::RequestHandlerFactory(IDatabase* db) : m_login_manager(LoginManager(db)), m_game_manager(GameManager(db))
{
    this->m_database = db;
    this->m_statistics_manager = StatisticsManager(this->m_database);
}

/*  function returns the login manager of the request handler factory.
    input: None.
    output: the login Manager of the RequestHandlerFactory. */
LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_login_manager;
}

/*function that creates loginRequestHandler.
input: None.
output : a LoginRequestHandler.*/
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler(SOCKET client_socket)
{
    return new LoginRequestHandler(*this, this->m_login_manager, client_socket);
}

/* returns a pointer to MenuRequestHandler class*/
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user, SOCKET client_socket)
{
    return new MenuRequestHandler(this->m_room_manager, this->m_statistics_manager, *this, user, client_socket);
}

/* function that creates room admin requset handler. */
RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, int room_id, SOCKET client_socket)
{
	return new RoomAdminRequestHandler(this->m_room_manager, room_id, *this, user, client_socket);
}

/* function that creates room member requset handler. */
RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, int room_id, SOCKET client_socket)
{
	return new RoomMemberRequestHandler(this->m_room_manager, room_id, *this, user, client_socket);
}

/* getter to statistics manager */
StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return this->m_statistics_manager;
}

/* getter to static manager */
RoomManager& RequestHandlerFactory::getRoomManager()
{
    return this->m_room_manager;
}

/* function that creates game requset handler. */
GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser user, Game& game, SOCKET client_soc)
{
    return new GameRequestHandler(this->m_game_manager, game, client_soc, *this, user);
}

/* function to get the game manager of the request handler factory. */
GameManager& RequestHandlerFactory::getGameManager()
{
    return this->m_game_manager;
}
