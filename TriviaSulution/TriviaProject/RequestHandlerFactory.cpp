#include "RequestHandlerFactory.h"

/*  constractor function that gets the database of the system
    and initaliize his attribiuts.
    input: a database.
    output: an object of RequestHandlerFactory. */
RequestHandlerFactory::RequestHandlerFactory(IDatabase* db) : m_login_manager(LoginManager(db))
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
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this, this->m_login_manager);
}

/* returns a pointer to MenuRequestHandler class*/
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{
    return new MenuRequestHandler(this->m_room_manager, this->m_statistics_manager, *this, user);
}

/* getter to static manager */
StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return this->m_statistics_manager;
}

/* getter to static manager */
RoomManager& RequestHandlerFactory::getRoomManager()
{
    return this->m_room_manager;
}
