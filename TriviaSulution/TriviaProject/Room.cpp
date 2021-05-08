#include "Room.h"

Room::Room(RoomData roomdata)
{
    this->m_metadata = roomdata;
    this->m_users = std::vector<LoggedUser>();
}

void Room::addUser(LoggedUser userToAdd)
{
    this->m_users.push_back(userToAdd);
}

void Room::removeUSer(LoggedUser userToRemove)
{
    for (auto it = this->m_users.begin(); it != this->m_users.end(); it++)
    {
        if (it->getUserName() == userToRemove.getUserName())
        {
            this->m_users.erase(it);
            return;
        }

    }
}

RoomData Room::GetRoomdata()
{
    return this->m_metadata;
}

std::vector<std::string> Room::getAllUsers()
{
    std::vector<std::string> usersNames;
    for (auto it = this->m_users.begin(); it != this->m_users.end(); it++)
    {
        usersNames.push_back(it->getUserName());
    }
    return usersNames;
}

void RoomManager::createRoom(LoggedUser loggedUserToRoom, RoomData currentRoomdata)
{
    Room room(currentRoomdata);
    room.addUser(loggedUserToRoom);
    this->m_rooms.insert(std::pair<unsigned int, Room>(currentRoomdata.id, room));
}

void RoomManager::deleteRoom(unsigned int ID)
{
    for (auto it = this->m_rooms.begin(); it != this->m_rooms.end(); it++)
    {
        if (it->first == ID)
        {
            this->m_rooms.erase(it);
            return;
        }
    }
}

unsigned int RoomManager::getRoomState(unsigned int ID)
{
    return this->m_rooms[ID].GetRoomdata().isActive;
}

std::vector<RoomData> RoomManager::getRooms()
{
    std::vector<RoomData> activeRooms = std::vector<RoomData>();
    for (auto it = this->m_rooms.begin(); it != this->m_rooms.end(); it++)
    {
        if (this->getRoomState(it->first))
        {
            activeRooms.push_back(it->second.GetRoomdata());
        }
    }
}
