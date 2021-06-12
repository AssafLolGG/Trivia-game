#pragma once
#include "IRequestHandler.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"

class GameRequestHandler : public IRequestHandler
{
private:
	Game m_game;
	LoggedUser m_user;
	GameManager m_gameManager;
	RequestHandlerFactory m_handlerFactory;
	RequestResult getQuestion(RequestInfo info);
	RequestResult submitAnswer(RequestInfo info);
	RequestResult getGameResults(RequestInfo info);
	RequestResult leaveGame(RequestInfo);
public:
	bool isRequestRelevant(RequestInfo& info) override;
	RequestResult handleRequest(RequestInfo& info) override;
};

