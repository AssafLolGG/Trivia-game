#include "LoginManager.h"

/*constructor function that initialize the value to an empty string
  input: none 
  output: object of LoggedUser */
LoggedUser::LoggedUser()
{
    this->_username = "";
}

/*constructor function that initialize the value to username
  input: string / username
  output: object of LoggedUser */
LoggedUser::LoggedUser(std::string username)
{
    this->_username = username;
}

/*getting username
  input: none
  output: string / username */
std::string LoggedUser::getUserName() const
{
    return this->_username;
}

/*getting username
  input: string username, string password, string email
  output: none */
void LoginManager::signup(std::string username, std::string password, std::string email)
{
    this->_db_access->addNewUser(username, password, email);
    this->_loggedUsers.push_back(LoggedUser(username));
}

/*logging username to system
  input: string / username, string / password
  output: if the login attempt failed */
bool LoginManager::login(std::string username, std::string password)
{
    if (this->_db_access->doesUserExist(username) &&
        this->_db_access->doesPasswordMatch(username, password))
    {
        this->_loggedUsers.push_back(LoggedUser(username));
        return true;
    }
    return false;
}

/*logging user out
  input: string / username
  output: none */
void LoginManager::logout(std::string username)
{
    for (auto it = this->_loggedUsers.begin(); it != this->_loggedUsers.end(); ++it)
    {
        if (it->getUserName() == username)
        {
            this->_loggedUsers.erase(it);
        }
    }
}
