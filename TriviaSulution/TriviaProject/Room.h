#pragma once
#include <iostream>
#include <vector>
#include "LoginManager.h"
#include <map>
#include <WinSock2.h>

#define INVALID_INDEX -1

using std::string;

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
};

class Room
{
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
	std::vector<SOCKET> m_sockets;
	
public:
	Room();
	Room(RoomData roomdata);
	bool addUser(LoggedUser userToAdd, SOCKET userSocket);
	void removeUser(LoggedUser userToRemove, SOCKET socketToRemove);
	RoomData& GetRoomdata();
	std::vector<std::string> getAllUsers();
	std::vector<SOCKET>& getAllSockets();
	bool isUserInRoom(LoggedUser);
};

class RoomManager
{
private:
	std::map<unsigned int, Room> m_rooms;
public:
	void createRoom(LoggedUser loggedUserToRoom, RoomData currentRoomdata, SOCKET userSocket);
	int getRoomID(LoggedUser loggedUserToRoom);
	void deleteRoom(unsigned int ID);
	unsigned int getRoomState(unsigned int ID);
	std::vector<RoomData> getRooms();
	Room* getRoom(unsigned int id);
	std::map<unsigned int, Room>& getAllRooms();
	int getRoomCount() const;
	
};