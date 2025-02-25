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

ServerConfig::ServerConfig(const ServerConfig &other)
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
    // _autoindex = other._autoindex; // commented by roi 0221
    _methods = other._methods;
    _logDirection = other._logDirection;
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
        // _autoindex = other._autoindex;	// commented by roi 0221
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

void ServerConfig::setErrorPages(std::map<short, std::string> errorPages)
{
    _errorPages = errorPages;
}



void ServerConfig::setLogDirection(std::string logDirection)
{
	_logDirection = logDirection;
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

// made by Kirill
// void ServerConfig::setMethods(std::vector<std::string> methods)
// {
//     _methods = methods;
// }

/* 
	3d verstion because every location has its own methods. - roi 0225
*/
void ServerConfig::setMethods(const std::string &location, const std::vector<std::string> &methods)
{
	_methods[location] = methods;
}

// roi 0225 
const char *ServerConfig::NoFileError::what() const throw()
{
	return "Shit, an Error in roi's part: Could not open the file";
}

/* 
	parsing started with methods - roi 0225
	Если директива allow_methods отсутствует, NGINX обрабатвает все методы HTTP-запросов для этой локации.
	Поэтому такая локация и методы не попадают в мапу methods (std::map<std::string, std::vector <std::string> > 	_methods;)  
 */
void ServerConfig::parseConfig(const std::string &filename)
{
	// std::cout << "roi debug: string(argv[1]) = " << filename << std::endl;  // debug
	std::ifstream file(filename.c_str()); // Преобразование std::string в const char* and initiation of input file stream
    if (!file.is_open())
	{
        throw NoFileError();
    }

    std::string line;
    std::string currentLocation;
    while (std::getline(file, line)) // Читаем файл построчно с помощью
	{
        std::istringstream iss(line); //4каждой строки создаем поток 4 разбора строки
        std::string key;
        iss >> key; //Извлекаем первое слово строки и сохраняем его в var key

        if (key == "location")
        {
			iss >> currentLocation;
			// std::cout << RED << "Locatioin found: " << currentLocation << std::endl << RESET; //debug
            if (!currentLocation.empty() && currentLocation[currentLocation.size() - 1] == '{')
			{
                currentLocation.erase(currentLocation.size() - 1); // Удаление последнего символа '{'
            }
        } else if (key == "allow_methods")
				{
					std::vector<std::string> methods;
					std::string method;
					while (iss >> method)
					{
						methods.push_back(method);
					}
					// std::cout << GREEN << currentLocation << std::endl << RESET; //debug
					// std::cout << RED << methods << std::endl << RESET; //debug - is not to b working. no operatoin<< implemented jet 0225
					setMethods(currentLocation, methods);
        		}
    }

    file.close();
}

/* 
	Перегрузка оператора << - roi 0225
 */
std::ostream& operator<<(std::ostream &os, const ServerConfig &config)
{
    os << "ServerConfig Methods:\n";
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = config._methods.begin(); it != config._methods.end(); ++it)
	{
        os << "Location: " << it->first << " Methods: ";
        for (std::vector<std::string>::const_iterator method_it = it->second.begin(); method_it != it->second.end(); ++method_it)
		{
            os << *method_it << " ";
        }
        os << "\n";
    }
    return os;
}
