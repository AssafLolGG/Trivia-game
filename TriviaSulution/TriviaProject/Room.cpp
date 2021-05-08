#include "Room.h"


/*
default constractor function which initalize the data of the room and users list.
input: None.
output: a Room object.
*/
Room::Room()
{
    Room(RoomData());
}

/*
constractor function which initalize the data of the room and users list.
input: the data about the room.
output: a Room object.
*/
Room::Room(RoomData roomdata)
{
    this->m_metadata = roomdata;
    this->m_users = std::vector<LoggedUser>();
}

/*
function that adds user to the list of users that are
in the room.
input: the user to add to the list.
output: None.
*/
void Room::addUser(LoggedUser userToAdd)
{
    this->m_users.push_back(userToAdd);
}

/*
function that removes user from the list
of users connected to the room.
input: the user to remove from the list.
output: None.
*/
void Room::removeUSer(LoggedUser userToRemove)
{
    for (auto it = this->m_users.begin(); it != this->m_users.end(); it++) // finds the room to erase through iterators and removes it.
    {
        if (it->getUserName() == userToRemove.getUserName())
        {
            this->m_users.erase(it);
            return;
        }

    }
}

/*
getter function that gets the data of the room.
input: None.
output: the data of the room.
*/
RoomData Room::GetRoomdata()
{
    return this->m_metadata;
}

/*
function that gets list of all users connected to a room.
input: None.
output: list of all users connected to the room.
*/
std::vector<std::string> Room::getAllUsers()
{
    std::vector<std::string> usersNames;
    for (auto it = this->m_users.begin(); it != this->m_users.end(); it++) // adds all users conncected to the room  to a list.
    {
        usersNames.push_back(it->getUserName());
    }
    return usersNames;
}

/*
function that creates room.
input: the room logged usera and the room data.
output: None.
*/
void RoomManager::createRoom(LoggedUser loggedUserToRoom, RoomData currentRoomdata)
{
    Room room(currentRoomdata);
    room.addUser(loggedUserToRoom);
    this->m_rooms.insert(std::pair<unsigned int, Room>(currentRoomdata.id, room));
}

/*
function that erases a room from a list.
input: the room's id.
output: None.
*/
void RoomManager::deleteRoom(unsigned int ID)
{
    for (auto it = this->m_rooms.begin(); it != this->m_rooms.end(); it++) // the function finds the room and removes it.
    {
        if (it->first == ID)
        {
            this->m_rooms.erase(it);
            return;
        }
    }
}

/*
function that checks if a certain room is active or not(his state).
input: the room's id.
output: 
1 - the room's active.
0 - the room isn't active.
*/
unsigned int RoomManager::getRoomState(unsigned int ID)
{
    return this->m_rooms[ID].GetRoomdata().isActive;
}

/*
function that gets all the active rooms in the rooms list.
input: None.
output: a list containing the data of all active rooms in the rooms list.
*/
std::vector<RoomData> RoomManager::getRooms()
{
    std::vector<RoomData> activeRooms = std::vector<RoomData>();
    for (auto it = this->m_rooms.begin(); it != this->m_rooms.end(); it++) // finds all active rooms and added it to the active rooms list.
    {
        if (this->getRoomState(it->first))
        {
            activeRooms.push_back(it->second.GetRoomdata());
        }
    }
    return activeRooms;
}
