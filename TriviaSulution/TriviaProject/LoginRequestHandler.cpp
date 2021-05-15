#include "LoginRequestHandler.h"

/*
get request from client to sign him, and logs him in.
input: the request from the client with the login data.
output: the result as a result of the request.
can mean- succefull or failed login.
*/
RequestResult LoginRequestHandler::login(RequestInfo& info)
{
    LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
    LoginResponse loginResponse;
    loginResponse.status = this->m_login_manager.login(loginRequest.username, loginRequest.password) == true ? 1 : 0;
    RequestResult result;
    result.respone = JsonResponsePacketSerializer::serializeResponse(loginResponse);
    // checks if the the user managed to login succeessfully.
    if (loginResponse.status == 1)
    {
        result.newHandler = new MenuRequestHandler(this->m_request_handler_factory.getRoomManager(), this->m_request_handler_factory.getStatisticsManager(), this->m_request_handler_factory);
    }
    else
    {
        result.newHandler = this->m_request_handler_factory.createLoginRequestHandler();
    }
    return result;
}

/*
get request from client to sign him, and signs him up.
input: the request from the client with the signup data.
output: the result as a result of the request.
can mean- succefull or failed signup.
*/
RequestResult LoginRequestHandler::signup(RequestInfo& info)
{
    SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
    SignupResponse signupResponse;
    this->m_login_manager.signup(signupRequest.username, signupRequest.password, signupRequest.email);
    signupResponse.status = 1;
    RequestResult result;
    result.respone = JsonResponsePacketSerializer::serializeResponse(signupResponse);
    // checks if the the user managed to signup succeessfully.
    if (signupResponse.status == 1)
    {
        result.newHandler = this->m_request_handler_factory.createLoginRequestHandler();
    }
    else
    {
        result.newHandler = nullptr;
    }
    return result;
}

/*
constractor function for LoginRequestHandler.
input: the handlerFactory, login manager.
output: An object of LoginRequestHandler.
*/
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory, LoginManager& loginManager) : m_request_handler_factory(handlerFactory), m_login_manager(loginManager)
{

}

/*
    checks to see if the message code belongs to:
    "login" or "signup"
    returns: true for "login", and false for "signup"
*/
bool LoginRequestHandler::isRequestRelevant(RequestInfo& info)
{
    return (info.id == LOGIN_CODE);
}

/*
function that handles request from a user.
input: the client's request information.
output: the response to the request.
*/
RequestResult LoginRequestHandler::handleRequest(RequestInfo& info)
{
    RequestResult result;
	// checks if the request is relevant.
    if (this->isRequestRelevant(info))
    {
        result = this->login(info);
    }
    else
    {
        result = this->signup(info);
    }
    return result;
}