#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"

#define FALSE 0
#define TRUE 1

class RequestHandlerFactory;
class LoginRequestHandler;

class LoginRequestHandler : public IRequestHandler 
{
private:
	LoginManager& m_login_manager;
	RequestHandlerFactory& m_request_handler_factory;
	RequestResult login(RequestInfo& info);
	RequestResult signup(RequestInfo& info);
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory, LoginManager& loginManager);
	bool isRequestRelevant(RequestInfo& info) override;
	RequestResult handleRequest(RequestInfo& info) override;
};


