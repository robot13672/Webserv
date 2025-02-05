#include "../inc/ServerConfig.hpp"

ServerConfig::ServerConfig(std::string host, u_int16_t port) //для эмуляции отработанного конфиг файла
{
    _host = inet_addr(host.c_str()); // Example IP address
    if (_host == INADDR_NONE) {
        std::cerr << "Error: Invalid IP address: " << host << std::endl;
        exit(EXIT_FAILURE);
    }
    _port = port; // Example port number
    _max_body_size = 20000;
    // std::cout << _host << ":" << _port << "\n";
}
ServerConfig::ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig &other)
{
    *this = other;
}

ServerConfig& ServerConfig::operator=(const ServerConfig &other)
{
    if (this != &other)
    {
        _adress = other._adress;
        _port = other._port;
        _host = other._host;
        _listen_fd = other._listen_fd;
        _max_body_size = other._max_body_size;
        _name = other._name;
        _root = other._root;
        _index = other._index;
        _errorPages = other._errorPages;
        _autoindex = other._autoindex;
        _methods = other._methods;
        _logDirection = other._logDirection;
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
    
    std::cout << "Successfully bound to host:" << _host << ", port:" << _port << " socket:" << _listen_fd << std::endl;
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

void ServerConfig::setErrorPages(std::map<short, std::string> errorPages)
{
    _errorPages = errorPages;
}

void ServerConfig::setMethods(std::vector<std::string> methods)
{
    _methods = methods;
}

void ServerConfig::setLogDirection(std::string logDirection)
{
    _logDirection = logDirection;
}
//GET
in_addr_t ServerConfig::getHost()
{
    return _host;
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
