#include "../inc/ParsedServerConfig.hpp"

std::vector<ParsedServerConfig> serverParsedConfigs;

ParsedServerConfig::ParsedServerConfig() {}

ParsedServerConfig::ParsedServerConfig(const ParsedServerConfig &other)
{
    _port = other._port;
    _host = other._host;
    _max_body_size = other._max_body_size;
	_name = other._name;
    _root = other._root;
    _index = other._index;
    _errorPages = other._errorPages;
    _methods = other._methods;
}

ParsedServerConfig& ParsedServerConfig::operator=(const ParsedServerConfig &other)
{
    if (this != &other)
    {
		_port = other._port;
		_host = other._host;
		_max_body_size = other._max_body_size;
		_name = other._name;
		_root = other._root;
		_index = other._index;
		_errorPages = other._errorPages;
		_methods = other._methods;
    }
    return *this;
}

//SET
void ParsedServerConfig::setPort(u_int16_t port)
{
    _port = port;
}

void ParsedServerConfig::setHost(std::string host)
{
    _host = inet_addr(host.c_str());
    if (_host == INADDR_NONE) {
        std::cerr << "Error: Invalid IP address: " << host << std::endl;
        exit(EXIT_FAILURE);
    }
}
void ParsedServerConfig::setMaxBodySize(long max_body_size)
{
    _max_body_size = max_body_size;
}

void ParsedServerConfig::setName(std::string name)
{
    _name = name;
}

void ParsedServerConfig::setRoot(std::string root)
{
    _root = root;
}

void ParsedServerConfig::setIndex(std::string index)
{
    _index = index;
}

void ParsedServerConfig::setErrorPages(std::map<short, std::string> errorPages)
{
    _errorPages = errorPages;
}
/* 
	3d verstion because every location has its own methods. - roi 0225
*/
void ParsedServerConfig::setMethods(const std::string &location, const std::vector<std::string> &methods)
{
	_methods[location] = methods;
}
// setters of 5 additional attributes in locations - roi 0227
// void ServerConfig::setLocationRoot(const std::string &location, const std::string &root)
// {
//     _locationRoots[location] = root;
// }

/* 
// termorary commented - roi 0227
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
 */
//GETTERS	
std::string ParsedServerConfig::getHost() const
	{
		struct in_addr addr;
		addr.s_addr = _host;
		return std::string(inet_ntoa(addr));
	}
	
u_int16_t ParsedServerConfig::getPort() const
{
	return _port;
}

long ParsedServerConfig::getMaxBodySize() const
{
	return _max_body_size;
}

// setters of 5 additional attributes in locations - roi 0227
/* std::string ServerConfig::getLocationRoot(const std::string &location) const
{
    std::map<std::string, std::string>::const_iterator it = _locationRoots.find(location);
    if (it != _locationRoots.end())
    {
        return it->second;
    }
    return "";
} */
/* 
// temporery commentd roi 0227
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
} */

/* 
	Перегрузка оператора << 
	checks only location and methoes- roi 0225
 */
std::ostream& operator<<(std::ostream &os, const ParsedServerConfig &config)
{
    os << "ParsedServerConfig:\n";
    os << "Host: " << config.getHost() << "\n";
    os << "Port: " << config.getPort() << "\n";
    os << "Max Body Size: " << config.getMaxBodySize() << "\n";
    os << "Server Name: " << config._name << "\n";
    os << "Root: " << config._root << "\n";
    os << "Index: " << config._index << "\n";
    os << "Error Pages:\n";
    for (std::map<short, std::string>::const_iterator it = config._errorPages.begin(); it != config._errorPages.end(); ++it)
    {
        os << "  Error Code: " << it->first << " Page: " << it->second << "\n";
    }
    os << "Methods:\n";
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = config._methods.begin(); it != config._methods.end(); ++it)
    {
        os << "  Location: " << it->first << " Methods: ";
        for (std::vector<std::string>::const_iterator method_it = it->second.begin(); method_it != it->second.end(); ++method_it)
        {
            os << *method_it << " ";
        }
        os << "\n";
    }
    return os;
}

// roi 0225 
const char *ParsedServerConfig::NoFileError::what() const throw()
{
	return "Shit, an Error in roi's part: Could not open the file";
}

/* 
	parsing started with methods - roi 0225
	Если директива allow_methods отсутствует, NGINX обрабатвает все методы HTTP-запросов для этой локации.
	Поэтому такая локация и методы не попадают в мапу methods (std::map<std::string, std::vector <std::string> > 	_methods;)  
 */
void ParsedServerConfig::parseConfig(const std::string &filename)
{
    std::ifstream file(filename.c_str()); // Преобразование std::string в const char* и инициализация входного файлового потока
    if (!file.is_open())
        throw NoFileError();

    std::string line;
    std::string currentLocation;
    bool inLocationBlock = false; // Используется для отслеживания, находимся ли мы внутри блока location.
    bool inServerBlock = false; // Используется для отслеживания, находимся ли мы внутри блока server.
    bool clientMaxBodySizeSet = false;
    ParsedServerConfig currentConfig;

    while (std::getline(file, line)) // Читаем файл построчно
    {
        std::istringstream iss(line); // Для каждой строки создаем поток для разбора строки
        std::string key;
        iss >> key; // Извлекаем первое слово строки и сохраняем его в переменную key

        std::cout << "Processing line: " << line << std::endl; // Debug output - roi 0228
		
		if (key == "server")
        {
            std::cout << "Found server block" << std::endl; // Debug output - roi 0228
			if (inServerBlock)
            {
                if (!clientMaxBodySizeSet)
                    currentConfig.setMaxBodySize(200000);
                serverParsedConfigs.push_back(currentConfig);
                currentConfig = ParsedServerConfig();
                clientMaxBodySizeSet = false;
            }
            inServerBlock = true;
			std::cout << PURPLE << currentLocation << RESET << std::endl; // debug - roi 0228
			currentLocation.clear(); // Сброс значения currentLocation при переходе к новому блоку server
			std::cout << PURPLE << currentLocation << RESET << std::endl; // debug after clear()  - roi 0228
        }
        else if (key == "}")
        {
            std::cout << "Found closing brace" << std::endl; // Debug output - roi 0228
			if (inLocationBlock)
                inLocationBlock = false;
            else if (inServerBlock)
            {
                if (!clientMaxBodySizeSet)
                    currentConfig.setMaxBodySize(200000);
                serverParsedConfigs.push_back(currentConfig);
                inServerBlock = false;
				currentLocation.clear(); // Сброс значения currentLocation при переходе к новому блоку server - roi 0228
				currentConfig._methods.clear(); // Сброс методов для локаций при завершении блока server - roi 0228
            }
        }
        else if (inLocationBlock && key == "allow_methods") // Блок для обработки директивы allow_methods
        {
            std::vector<std::string> methods;
            std::string method;
            while (iss >> method)
            {
                if (!method.empty() && method[method.size() - 1] == ';')
                    method.erase(method.size() - 1); // Удаление точки с запятой в конце строки
                methods.push_back(method);
            }
            currentConfig.setMethods(currentLocation, methods);
        }
        else if (inServerBlock && !inLocationBlock)
        {
            if (key == "listen")
            {
                int port;
                iss >> port;
                currentConfig.setPort(port);
            }
            else if (key == "server_name")
            {
                std::string serverName;
                iss >> serverName;
                if (!serverName.empty() && serverName[serverName.size() - 1] == ';')
                    serverName.erase(serverName.size() - 1); // Удаление точки с запятой в конце строки
                currentConfig.setName(serverName);
            }
            else if (key == "host")
            {
                std::string host;
                iss >> host;
                if (!host.empty() && host[host.size() - 1] == ';')
                    host.erase(host.size() - 1); // Удаление точки с запятой в конце строки
                currentConfig.setHost(host);
            }
            else if (key == "root")
            {
                std::string root;
                iss >> root;
                if (!root.empty() && root[root.size() - 1] == ';')
                    root.erase(root.size() - 1); // Удаление точки с запятой в конце строки
                currentConfig.setRoot(root);
            }
            else if (key == "index")
            {
                std::string index;
                iss >> index;
                if (!index.empty() && index[index.size() - 1] == ';')
                    index.erase(index.size() - 1); // Удаление точки с запятой в конце строки
                currentConfig.setIndex(index);
            }
            else if (key == "error_page")
            {
                short errorCode;
                std::string errorPage;
                iss >> errorCode >> errorPage;
                if (!errorPage.empty() && errorPage[errorPage.size() - 1] == ';')
                    errorPage.erase(errorPage.size() - 1); // Удаление точки с запятой в конце строки
                std::map<short, std::string> errorPages;
                errorPages.insert(std::make_pair(errorCode, errorPage));
                currentConfig.setErrorPages(errorPages);
            }
            else if (key == "client_max_body_size")
            {
                long maxBodySize;
                iss >> maxBodySize;
                currentConfig.setMaxBodySize(maxBodySize);
                clientMaxBodySizeSet = true;
            }
            else if (key == "location")
            {
                iss >> currentLocation;
                inLocationBlock = true;
                if (!currentLocation.empty() && currentLocation[currentLocation.size() - 1] == '{')
                    currentLocation.erase(currentLocation.size() - 1); // Удаление последнего символа '{'
            }
        }
    }

    if (inServerBlock)
    {
        if (!clientMaxBodySizeSet)
            currentConfig.setMaxBodySize(200000);
        serverParsedConfigs.push_back(currentConfig);
    }
    file.close();

}

