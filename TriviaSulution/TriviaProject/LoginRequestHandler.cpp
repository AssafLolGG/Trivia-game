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
<<<<<<< HEAD
    loginResponse.status = this->m_loginManager.login(loginRequest.username, loginRequest.password) == true ? TRUE : FALSE;
=======
    loginResponse.status = this->m_login_manager.login(loginRequest.username, loginRequest.password) == true ? 1 : 0;
>>>>>>> 18f90f815aad71a643041d19ea17646e229e7f75
    RequestResult result;
    result.respone = JsonResponsePacketSerializer::serializeResponse(loginResponse);
    // checks if the the user managed to login succeessfully.
    if (loginResponse.status == TRUE)
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
    SignupRequest _signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
	SignupResponse signupResponse;
	signupResponse.status = this->m_loginManager.signup(_signupRequest.username, _signupRequest.password, _signupRequest.email, _signupRequest.address, _signupRequest.phone, _signupRequest.birthdate) == true ? TRUE : FALSE;
    RequestResult result;
    result.respone = JsonResponsePacketSerializer::serializeResponse(signupResponse);
    // checks if the the user managed to signup succeessfully.
    if (signupResponse.status == TRUE)
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
