#include "GameRequestHandler.h"

RequestResult GameRequestHandler::getQuestion(RequestInfo info)
{
    RequestResult result;
    GetQuestionResponse getQuestion;
    int i = 0;
    for (Answer ans : this->m_game.getUserData(this->m_user).currentQuestion.answers)
    {
        i++;
        getQuestion.Answers.insert(std::pair<int, string>(i, ans.answerText));
    }
	getQuestion.status = this->m_game.getUserData(this->m_user).isThereQuestions == true ? STATUS_OK : STATUS_FAIL;
    getQuestion.question = this->m_game.getUserData(this->m_user).currentQuestion.questionText;

    result.new_handler = new GameRequestHandler(*this);
    result.respone = JsonResponsePacketSerializer::serializeResponse(getQuestion);
    

    return result;
}

RequestResult GameRequestHandler::submitAnswer(RequestInfo info)
{
    RequestResult result;
    SubmitAnswerRequest infoDitails = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info.buffer);
    SubmitAnswerResponse submitAnswer;

    if (infoDitails.answerID >= 1 && infoDitails.answerID <= this->m_game.getUserData(this->m_user).currentQuestion.answers.size())
    {
        bool isAnswerd = this->m_game.submitAnswer(this->m_user, this->m_game.getUserData(this->m_user).currentQuestion.answers[infoDitails.answerID - 1].answerText);
		if (isAnswerd)
		{
			submitAnswer.status = this->m_game.getUserData(this->m_user).isThereQuestions == true ? STATUS_OK : STATUS_FAIL;
			this->m_game.getQuestionForUser(this->m_user);

			// update user's statistics
			if (this->m_game.getUserData(this->m_user).isThereQuestions == false)
			{
				this->m_gameManager.updateStatistics(this->m_user);
			}
		}
		else
		{
			submitAnswer.status = STATUS_FAIL;
		}
	}
    else
    {
        submitAnswer.status = STATUS_FAIL;
    }
    result.new_handler = new GameRequestHandler(*this);
    result.respone = JsonResponsePacketSerializer::serializeResponse(submitAnswer);

    return result;
}

RequestResult GameRequestHandler::getGameResults(RequestInfo info)
{
    RequestResult result;
    GetGameResultsResponse getGameResults;
	bool gameFinished = this->m_game.checkIfFinished();
    getGameResults.results = std::vector<PlayerResults>();
	if (gameFinished)
	{
		for (auto player : this->m_game.getPlayers())
		{
			PlayerResults playerResult;
			playerResult.averageAnswerTime = player.second.averageAnswerTime;
			playerResult.CorrectAnswersCount = player.second.correctAnswerCount;
			playerResult.WrongAnswerCount = player.second.wrongAnswerCount;
			playerResult.Username = player.first.getUserName();
			getGameResults.results.push_back(playerResult);
		}
	}
    getGameResults.status = gameFinished ? STATUS_OK : STATUS_FAIL;
    result.new_handler = new GameRequestHandler(*this);
    result.respone = JsonResponsePacketSerializer::serializeResponse(getGameResults);

    return result;
}

RequestResult GameRequestHandler::leaveGame(RequestInfo info)
{
    RequestResult result;
    LeaveGameResponse leaveGame;
    
    this->m_game.removePlayer(this->m_user);
    leaveGame.status = STATUS_OK;

    result.new_handler = this->m_handlerFactory.createMenuRequestHandler(this->m_user, this->m_socket);
    result.respone = JsonResponsePacketSerializer::serializeResponse(leaveGame);

    return result;
}

bool GameRequestHandler::isRequestRelevant(RequestInfo& info)
{
    return info.id == GET_QUESTION_CODE || info.id == LEAVE_GAME_CODE ||
        info.id == GET_GAME_RESULTS_CODE || info.id == SUBMIT_ANSWER_CODE;
}

RequestResult GameRequestHandler::handleRequest(RequestInfo& info)
{
	RequestResult result;

    if (info.id == GET_QUESTION_CODE)
    {
        result = this->getQuestion(info);
    }
    else if(info.id == LEAVE_GAME_CODE)
    {
        result = this->leaveGame(info);
    }
    else if (info.id == GET_GAME_RESULTS_CODE)
    {
        result = this->getGameResults(info);
    }
    else if (info.id == SUBMIT_ANSWER_CODE)
    {
        result = this->submitAnswer(info);
    }
	return result;
}

int GameRequestHandler::GetRequestHandlerType()
{
	return 5;
}
