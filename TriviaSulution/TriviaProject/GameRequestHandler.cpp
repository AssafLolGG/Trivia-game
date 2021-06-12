#include "GameRequestHandler.h"

RequestResult GameRequestHandler::getQuestion(RequestInfo info)
{
    return RequestResult();
}

RequestResult GameRequestHandler::submitAnswer(RequestInfo info)
{
    return RequestResult();
}

RequestResult GameRequestHandler::getGameResults(RequestInfo info)
{
    return RequestResult();
}

RequestResult GameRequestHandler::leaveGame(RequestInfo)
{
    return RequestResult();
}

bool GameRequestHandler::isRequestRelevant(RequestInfo& info)
{
    return info.id == GET_QUESTION_CODE || info.id == LEAVE_GAME_CODE ||
        info.id == GET_GAME_RESULTS_CODE || info.id == SUBMIT_ANSWER_CODE;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo& info)
{
    return RequestResult();
}
