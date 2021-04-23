#pragma once

#include "IRequestHandler.h"
#include <vector>

struct RequestResult
{
	std::vector<unsigned char> respone;
	IRequestHandler* newHandler;
};

class LoginRequestHandler : public IRequestHandler 
{
	bool isRequestRelevant(RequestInfo);
	RequestResult handleRequest(RequestInfo);
};

