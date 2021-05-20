#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"

#define FALSE 0
#define TRUE 1

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler 
{
private:
	LoginManager& m_loginManager;
	RequestHandlerFactory& m_handlerFactory;
	RequestResult login(RequestInfo& info);
	RequestResult signup(RequestInfo& info);
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory, LoginManager& loginManager);
	bool isRequestRelevant(RequestInfo& info) override;
	RequestResult handleRequest(RequestInfo& info) override;
};

class RequestHandlerFactory
{
private:
	LoginManager m_loginManager;
	IDatabase* m_database;
public:
	RequestHandlerFactory(IDatabase* db);
	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginManager();
};


