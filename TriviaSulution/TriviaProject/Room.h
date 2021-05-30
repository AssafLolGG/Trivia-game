#pragma once
#include <iostream>
#include <vector>
#include "LoginManager.h"
#include <map>

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
	
public:
	Room();
	Room(RoomData roomdata);
	void addUser(LoggedUser userToAdd);
	void removeUSer(LoggedUser userToRemove);
	RoomData& GetRoomdata();
	std::vector<std::string> getAllUsers();
	bool isUserInRoom(LoggedUser);
};

class RoomManager
{
private:
	std::map<unsigned int, Room> m_rooms;
public:
	void createRoom(LoggedUser loggedUserToRoom, RoomData currentRoomdata);
	int getRoomID(LoggedUser loggedUserToRoom);
	void deleteRoom(unsigned int ID);
	unsigned int getRoomState(unsigned int ID);
	std::vector<RoomData> getRooms();
	bool getRoom(unsigned int id, Room* room);
	std::map<unsigned int, Room>& getAllRooms();
	int getRoomCount() const;
};