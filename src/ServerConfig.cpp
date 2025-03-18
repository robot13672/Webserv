#include "../inc/ServerConfig.hpp"


ServerConfig::ServerConfig(std::string host, u_int16_t port) //для эмуляции отработанного конфиг файла
{
    _host = inet_addr(host.c_str()); // Example IP address
    if (_host == INADDR_NONE) {
        std::cerr << "Error: Invalid IP address: " << host << std::endl;
        exit(EXIT_FAILURE);
    }
    _port = port; // Example port number
    _max_body_size = 200000000000;
    // std::cout << _host << ":" << _port << "\n";
}
ServerConfig::ServerConfig() {}

ServerConfig::~ServerConfig()
{
    _methods.clear();
    _errorPages.clear();
    _locationRoots.clear();
    _locationIndexes.clear();
    _locationCgiPaths.clear();
    _locationCgiExts.clear();
    _locationAutoindex.clear();
    
    _name.clear();
    _root.clear();
    _index.clear();
}

ServerConfig::ServerConfig(const ServerConfig &other)
{
    _adress = other._adress;
    _listen_fd = other._listen_fd;
    _port = other._port;
	_host = other._host;
	_max_body_size = other._max_body_size;
	_name = other._name;
	_root = other._root;
	_index = other._index;
	_errorPages = other._errorPages;
	_methods = other._methods;
	_locationRoots = other._locationRoots;  // added roi 0301 
	_locationIndexes = other._locationIndexes;
	_locationCgiPaths = other._locationCgiPaths; // roi 0302
	_locationCgiExts = other._locationCgiExts;
	_locationAutoindex = other._locationAutoindex;
}

ServerConfig& ServerConfig::operator=(const ServerConfig &other)
{
    if (this != &other)
    {
        _adress = other._adress;
        _listen_fd = other._listen_fd;
        _port = other._port;
		_host = other._host;
		_max_body_size = other._max_body_size;
		_name = other._name;
		_root = other._root;
		_index = other._index;
		_errorPages = other._errorPages;
		_methods = other._methods;
		_locationRoots = other._locationRoots;  // added roi 0301 
		_locationIndexes = other._locationIndexes;
		_locationCgiPaths = other._locationCgiPaths; // roi 0302
		_locationCgiExts = other._locationCgiExts;
		_locationAutoindex = other._locationAutoindex;
    }
    return *this;
}
//Settings
void ServerConfig::setupServer()//функция для настройки сервера, создание сокета, привязка к порту
{
    _listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(_listen_fd == -1)
    {
        std::cerr << "Error: Fatal socket allocation" << std::endl;
        exit(EXIT_FAILURE);
    }

    int tmp = 1;
    if (setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) == -1) // устраняет проблемы с занятым портом после завершения работы сокета.
    {
        std::cerr << "Error: setsockopt failed " << std::endl;
        close(_listen_fd); // Закрываем сокет перед завершением
        exit(EXIT_FAILURE);
    } 

    memset(&_adress, 0, sizeof(_adress)); // обнуление структуры адреса
    _adress.sin_family = AF_INET;//установка подключения на  Ip V4
    _adress.sin_addr.s_addr = _host;//установка хоста в структуру
    _adress.sin_port = htons(_port);// установка порта в структуру

    if(bind(_listen_fd, (sockaddr *) &_adress, sizeof(_adress)) == -1)
    {
        logger.writeMessage("Error: Error bind host");
        close(_listen_fd); // Закрываем сокет перед завершением
        exit(EXIT_FAILURE);
    }
    logger.writeMessage("Successfully bound to host:" + getHost() + ", port:" + uint16ToString(getPort()) + " socket:" + intToString(_listen_fd));
}


//SET
void ServerConfig::setFd(int fd)
{
    _listen_fd = fd;
}

void ServerConfig::setPort(u_int16_t port)
{
    _port = port;
}

void ServerConfig::setHost(std::string host)
{
    _host = inet_addr(host.c_str());
    if (_host == INADDR_NONE) {
        std::cerr << "Error: Invalid IP address: " << host << std::endl;
        exit(EXIT_FAILURE);
    }
}

void ServerConfig::setMaxBodySize(long max_body_size)
{
    _max_body_size = max_body_size;
}

void ServerConfig::setName(std::string name)
{
    _name = name;
}

void ServerConfig::setRoot(std::string root)
{
    _root = root;
}

void ServerConfig::setIndex(std::string index)
{
    _index = index;
}

void ServerConfig::setErrorPages(short errorCode, std::string errorPage)
{
    _errorPages[errorCode] = errorPage;
}

void ServerConfig::setMethods(const std::string &location, const std::vector<std::string> &methods)
{
	_methods[location] = methods;
}

void ServerConfig::setLocationRoot(const std::string &location, const std::string &root)
{
    _locationRoots[location] = root;
}

void ServerConfig::setLocationIndex(const std::string &location, const std::string &index)
{
    _locationIndexes[location] = index;
}

void ServerConfig::setLocationCgiPath(const std::string &location, const std::vector<std::string> &cgiPaths)
{
    _locationCgiPaths[location] = cgiPaths;
}

void ServerConfig::setLocationCgiExt(const std::string &location, const std::vector<std::string> &cgiExts)
{
    _locationCgiExts[location] = cgiExts;
}

void ServerConfig::setLocationAutoindex(const std::string &location, bool autoindex)
{
    _locationAutoindex[location] = autoindex;
}

void ServerConfig::setLocationRoots(const std::map<std::string, std::string> &locationRoots)
{
    _locationRoots = locationRoots;
}

void ServerConfig::setLocationIndexes(const std::map<std::string, std::string> &locationIndexes)
{
    _locationIndexes = locationIndexes;
}

void ServerConfig::setLocationCgiPaths(const std::map<std::string, std::vector<std::string> > &locationCgiPaths)
{
    _locationCgiPaths = locationCgiPaths;
}

void ServerConfig::setLocationCgiExts(const std::map<std::string, std::vector<std::string> > &locationCgiExts)
{
    _locationCgiExts = locationCgiExts;
}

void ServerConfig::setLocationAutoindexes(const std::map<std::string, bool> &locationAutoindex)
{
    _locationAutoindex = locationAutoindex;
}

void ServerConfig::setMethods(const std::map<std::string, std::vector<std::string> > &methods)
{
    _methods = methods;
}

//GET
// in_addr_t ServerConfig::getHost()
// {
//     return _host;
// }

std::string ServerConfig::getHost()
{
    struct in_addr addr;
    addr.s_addr = _host;
    return std::string(inet_ntoa(addr));
}

u_int16_t ServerConfig::getPort()
{
    return _port;
}

int ServerConfig::getListenFd()
{
    return _listen_fd;
}

long ServerConfig::getMaxBodySize()
{
    return _max_body_size;
}

std::string ServerConfig::getLocationRoot(const std::string &location) const
{
    std::map<std::string, std::string>::const_iterator it = _locationRoots.find(location);
    if (it != _locationRoots.end())
    {
        return it->second;
    }
    return "";
}

std::string ServerConfig::getLocationIndex(const std::string &location) const
{
    std::map<std::string, std::string>::const_iterator it = _locationIndexes.find(location);
    if (it != _locationIndexes.end())
    {
        return it->second;
    }
    return "";
}

std::vector<std::string> ServerConfig::getLocationCgiPath(const std::string &location) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _locationCgiPaths.find(location);
    if (it != _locationCgiPaths.end())
    {
        return it->second;
    }
    return std::vector<std::string>();
}

std::vector<std::string> ServerConfig::getLocationCgiExt(const std::string &location) const
{
    std::map<std::string, std::vector<std::string> >::const_iterator it = _locationCgiExts.find(location);
    if (it != _locationCgiExts.end())
    {
        return it->second;
    }
    return std::vector<std::string>();
}

bool ServerConfig::getLocationAutoindex(const std::string &location) const
{
    std::map<std::string, bool>::const_iterator it = _locationAutoindex.find(location);
    if (it != _locationAutoindex.end())
    {
        return it->second;
    }
    return false;
}

bool ServerConfig::isAvailibleMethod(std::string path, std::string method)
{
    // Find if there are any methods defined for this path
    std::map<std::string, std::vector<std::string> >::const_iterator it = _methods.find(path);
    
    // If no methods are defined for this path, all methods are allowed (nginx default behavior)
    if (it == _methods.end())
        return true;
        
    // Check if the requested method is in the allowed methods vector
    const std::vector<std::string>& allowedMethods = it->second;
    return std::find(allowedMethods.begin(), allowedMethods.end(), method) != allowedMethods.end();
}

std::string ServerConfig::getErrorPage(int errorCode) const {
    std::map<short, std::string>::const_iterator it = _errorPages.find(errorCode);

    if (it != _errorPages.end()) {
        return it->second;
    }
    return "";
}


std::string ServerConfig::getRoot()
{
    return _root;
}

std::string ServerConfig::getIndex()
{
    return _index;
}