#pragma once

#include <WinSock2.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include "RequestHandlerFactory.h"
#define BUFFER_CAPACITY 1024

class RequestHandlerFactory;

// server utilities functions:

bool receiveMassageFromClient(SOCKET& client_socket, char* message_buffer);
RequestInfo createRequestInfo(std::vector<uint8_t>& buffer_vector);
void insertHandlerToClient(IRequestHandler* new_handler, std::map<SOCKET, IRequestHandler*>& clients, SOCKET& client_socket);
IRequestHandler* getHandlerOfClient(std::map<SOCKET, IRequestHandler*>& clients, SOCKET& client_socket);
GetRoomDataResponse getRoomData(RoomManager& room_manager, int room_id, LoggedUser logged);