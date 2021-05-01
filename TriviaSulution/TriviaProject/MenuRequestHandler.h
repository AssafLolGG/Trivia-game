#pragma once
#include "IRequestHandler.h"
class MenuRequestHandler : public IRequestHandler
{
public:
	bool isRequestRelevant(RequestInfo& info) override;
	RequestResult handleRequest(RequestInfo& info) override;
};

