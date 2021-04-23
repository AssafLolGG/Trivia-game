#include "LoginRequestHandler.h"

/*
    checks to see if the message code belongs to:
    "login" or "signup"
    returns: true for "login", and false for "signup
*/
bool LoginRequestHandler::isRequestRelevant(RequestInfo)
{
    return false;
}
