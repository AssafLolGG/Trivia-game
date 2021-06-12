#include "LoginRequestHandler.h"

/*  get request from client to sign him, and logs him in.
    input: the request from the client with the login data.
    output: the result as a result of the request.
    can mean- succefull or failed login. */
RequestResult LoginRequestHandler::login(RequestInfo& info)
{
    LoginRequest login_request = JsonRequestPacketDeserializer::deserializeLoginRequest(info.buffer);
    LoginResponse login_response;

    login_response.status = this->m_login_manager.login(login_request.username, login_request.password) == true ? TRUE : FALSE;

    RequestResult result;
    result.respone = JsonResponsePacketSerializer::serializeResponse(login_response);
    // checks if the the user managed to login succeessfully.
    if (login_response.status == TRUE)
    {
        LoggedUser user(login_request.username);
        result.new_handler = result.new_handler = result.new_handler = this->m_request_handler_factory.createMenuRequestHandler(LoggedUser(login_request.username), this->m_socket);
    }
    else
    {
        result.new_handler = nullptr;
    }

    return result;
}

/*  get request from client to sign him, and signs him up.
    input: the request from the client with the signup data.
    output: the result as a result of the request.
    can mean- succefull or failed signup. */
RequestResult LoginRequestHandler::signup(RequestInfo& info)
{
    SignupRequest signup_request = JsonRequestPacketDeserializer::deserializeSignupRequest(info.buffer);
	SignupResponse signup_response;
	signup_response.status = this->m_login_manager.signup(signup_request.username, signup_request.password, signup_request.email, signup_request.address, signup_request.phone, signup_request.birthdate) == true ? TRUE : FALSE;
    RequestResult result;
    result.respone = JsonResponsePacketSerializer::serializeResponse(signup_response);
    // checks if the the user managed to signup succeessfully.
    if (signup_response.status == TRUE)
    {
        result.new_handler = this->m_request_handler_factory.createMenuRequestHandler(LoggedUser(signup_request.username), this->m_socket);
    }
    else
    {
        result.new_handler = nullptr;
    }

    return result;
}

/*  constractor function for LoginRequestHandler.
    input: the handlerFactory, login manager.
    output: An object of LoginRequestHandler. */
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory, LoginManager& loginManager, SOCKET client_socket) : m_request_handler_factory(handlerFactory), m_login_manager(loginManager)
{
	this->m_socket = client_socket;
}

/*  checks to see if the message code belongs to:
    "login" or "signup"
    returns: true for "login", and false for "signup" */
bool LoginRequestHandler::isRequestRelevant(RequestInfo& info)
{
    return info.id == LOGIN_CODE || info.id == SIGNUP_CODE;
}

/*  function that handles request from a user.
    input: the client's request information.
    output: the response to the request. */
RequestResult LoginRequestHandler::handleRequest(RequestInfo& info)
{
    RequestResult result;

    // checking type of request
    if (info.id == LOGIN_CODE)
    {
        result = this->login(info);
    }
    else
    {
        result = this->signup(info);
    }
    return result;
}

int LoginRequestHandler::GetRequestHandlerType()
{
	return 1;
}
