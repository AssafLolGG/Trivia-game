#pragma once

#include <WinSock2.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include "LoginRequestHandler.h"

#define BUFFER_CAPACITY 1024

bool receiveMassageFromClient(SOCKET& client_socket, char* message_buffer);
RequestInfo createRequestInfo(std::vector<uint8_t>& buffer_vector);
bool isLoggedIn(IRequestHandler* handler, RequestHandlerFactory& handler_factory);
void insertHandlerToClient(IRequestHandler* new_handler, std::map<SOCKET, IRequestHandler*>& clients, SOCKET& client_socket);