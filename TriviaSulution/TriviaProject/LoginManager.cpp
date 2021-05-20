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
LoginManager::LoginManager(IDatabase* db)
{
    this->_db_access = db;
}
bool LoginManager::signup(std::string username, std::string password, std::string email, std::string address, std::string phone, std::string birthday)
{
	// gets the street, appartment, city separated by ',' in address.
	std::regex r("\\,");
	std::vector<std::string> out(
		std::sregex_token_iterator(address.begin(), address.end(), r, -1)
		,std::sregex_token_iterator());
	
	// checks if the sign up info is valid or no.
	if (!(std::regex_match(password, std::regex("[a-zA-Z0-9!@#$%^&*]{8,}"))))
	{
		return false;
	}
	if (!(std::regex_match(email, std::regex("[a-zA-Z0-9]{1,}@[a-zA-Z0-9]{1,}(.com$|.co.il$|.cyber.org.il$)"))))
	{
		return false;
	}
	if (!(std::regex_match(out[0], std::regex("^[a-zA-Z ]{0,}[a-zA-Z]{1,}$"))))
	{
		return false;
	}
	if (!(std::regex_match(out[1], std::regex("^[\d]{1,}$"))))
	{
		return false;
	}
	if (!(std::regex_match(out[2], std::regex("^[a-zA-Z ]{0,}[a-zA-Z]{1,}$"))))
	{
		return false;
	}
	if (!(std::regex_match(phone, std::regex("^0(2|3|4|5[2345])-[\d]{6,7}$"))))
	{
		return false;
	}
	if (!(std::regex_match(birthday, std::regex("^([\d]{2}.[\d]{2}.[\d]{4})|([\d]{2}/[\d]{2}/[\d]{4})$"))))
	{
		return false;
	}

	// adds the user , after the function checked his ditails's validility.
    this->_db_access->addNewUser(username, password, email, address, phone, birthday);
    this->_loggedUsers.push_back(LoggedUser(username));
	return true;
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
