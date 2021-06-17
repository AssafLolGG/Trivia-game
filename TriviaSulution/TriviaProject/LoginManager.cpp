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

bool LoggedUser::operator<(const LoggedUser& other) const
{
	return this->getUserName() < other.getUserName();
}

bool LoginManager::isUserActive(std::string username)
{
	for (LoggedUser user : this->_loggedUsers)
	{
		if (user.getUserName() == username)
		{
			return true;
		}
	}

	return false;
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
	std::vector<std::string> addressSeperated(
		std::sregex_token_iterator(address.begin(), address.end(), r, -1)
		,std::sregex_token_iterator());
	std::string street, apt, city;
	if(addressSeperated.size() == 3)
	{
		street = addressSeperated[STREET_INDEX], apt = addressSeperated[APT_INDEX], city = addressSeperated[CITY_INDEX];
	}
	else
	{
		return false;
	}
	//checks if user exists
	if (this->_db_access->doesUserExist(username))
	{
		return false;
	}

	// checks if the sign up info is valid or no.
	if (!(std::regex_match(password, std::regex(PASSWORD_REGEX))))
	{
		return false;
	}
	if (!(std::regex_match(email, std::regex(EMAIL_REGEX))))
	{
		return false;
	}
	if (!(std::regex_match(street, std::regex(STREET_REGEX))))
	{
		return false;
	}
	if (!(std::regex_match(apt, std::regex(APT_REGEX))))
	{
		return false;
	}
	if (!(std::regex_match(city, std::regex(CITY_REGEX))))
	{
		return false;
	}
	if (!(std::regex_match(phone, std::regex(PHONE_REGEX))))
	{
		return false;
	}
	if (!(std::regex_match(birthday, std::regex(BIRTHDATE_REGEX))))
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
	if (!this->isUserActive(username))
	{
		if (this->_db_access->doesUserExist(username) &&
			this->_db_access->doesPasswordMatch(username, password));
		{
			this->_loggedUsers.push_back(LoggedUser(username));
			return true;
		}
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
			return;
        }
    }
}
