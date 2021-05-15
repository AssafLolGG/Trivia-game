#include "RequestHandlerFactory.h"

/*  constractor function that gets the database of the system
    and initaliize his attribiuts.
    input: a database.
    output: an object of RequestHandlerFactory. */
RequestHandlerFactory::RequestHandlerFactory(IDatabase* db) : m_login_manager(LoginManager(db))
{
    this->m_database = db;
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