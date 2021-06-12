#pragma once
#include "IRequestHandler.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"

class GameRequestHandler : public IRequestHandler
{
private:
	Game& m_game;
	SOCKET m_socket;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult getQuestion(RequestInfo info);
	RequestResult submitAnswer(RequestInfo info);
	RequestResult getGameResults(RequestInfo info);
public:
	GameRequestHandler(GameManager& game_manager, Game& game, RequestHandlerFactory& handler_factory, LoggedUser m_user_) : m_gameManager(game_manager),
		m_game(game), m_handlerFactory(handler_factory), IRequestHandler()
	{
		this->m_user = m_user_;
	}
	bool isRequestRelevant(RequestInfo& info) override;
	RequestResult handleRequest(RequestInfo& info) override;
	int GetRequestHandlerType() override;
	RequestResult leaveGame(RequestInfo info);
};

