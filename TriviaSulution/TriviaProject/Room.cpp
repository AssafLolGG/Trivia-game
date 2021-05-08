#include "Room.h"

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

std::vector<std::string> Room::getAllUsers()
{
    std::vector<std::string> usersNames;
    for (auto it = this->m_users.begin(); it != this->m_users.end(); it++)
    {
        usersNames.push_back(it->getUserName());
    }
    return usersNames;
}
