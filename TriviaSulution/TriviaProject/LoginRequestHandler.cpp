#include "LoginRequestHandler.h"

/*
    checks to see if the message code belongs to:
    "login" or "signup"
    returns: true for "login", and false for "signup"
*/
bool LoginRequestHandler::isRequestRelevant(RequestInfo& info)
{
    return (info.id = LOGIN_CODE);
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo& info)
{
    LoginResponse login;
    login.status = STATUS_OK;
    RequestResult result;
    result.respone = JsonResponsePacketSerializer::serializeResponse(login);
    result.newHandler = nullptr;

    return result;
}
