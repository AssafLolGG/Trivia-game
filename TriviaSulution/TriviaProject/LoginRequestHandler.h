#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RequestHandlerFactory.h"
#include "MenuRequestHandler.h"
class LoginRequestHandler : public IRequestHandler 
{
private:
	LoginManager& m_loginManager;
	RequestHandlerFactory& m_handlerFactory;
	RequestResult login(RequestInfo& info);
	RequestResult signup(RequestInfo& info);
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory);
	bool isRequestRelevant(RequestInfo& info);
	RequestResult handleRequest(RequestInfo& info);
};

class RequestHandlerFactory
{
private:
public:

};


