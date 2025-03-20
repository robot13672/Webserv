#include "../inc/ParsedServerConfig.hpp"

// Remove global variable definition

ParsedServerConfig::ParsedServerConfig() {}

ParsedServerConfig::ParsedServerConfig(const ParsedServerConfig &other)
{
	_ports = other._ports; // because of _ports vector
	_host = other._host;
	_max_body_size = other._max_body_size;
	_name = other._name;
	_root = other._root;
	_index = other._index;
	_errorPages = other._errorPages;
	_methods = other._methods;
	_locationRoots = other._locationRoots; // added roi 0301
	_locationIndexes = other._locationIndexes;
	_locationCgiPaths = other._locationCgiPaths; // roi 0302
	_locationCgiExts = other._locationCgiExts;
	_locationAutoindex = other._locationAutoindex;
    _return = other._return;
}

ParsedServerConfig &ParsedServerConfig::operator=(const ParsedServerConfig &other)
{
	if (this != &other)
	{
		_ports = other._ports; // because of _ports vector
		_host = other._host;
		_max_body_size = other._max_body_size;
		_name = other._name;
		_root = other._root;
		_index = other._index;
		_errorPages = other._errorPages;
		_methods = other._methods;
		_locationRoots = other._locationRoots; // added roi 0301
		_locationIndexes = other._locationIndexes;
		_locationCgiPaths = other._locationCgiPaths; // roi 0302
		_locationCgiExts = other._locationCgiExts;
		_locationAutoindex = other._locationAutoindex;
        _return = other._return;
	}
	return *this;
}

void ParsedServerConfig::addPort(u_int16_t port)
{
    _ports.push_back(port);
}

const std::vector<u_int16_t>& ParsedServerConfig::getPorts() const
{
    return _ports;
}
// because of _ports vector end
void ParsedServerConfig::setHost(std::string host)
{
	_host = inet_addr(host.c_str());
	if (_host == INADDR_NONE)
	{
		std::cerr << "Error: Invalid IP address: " << host << std::endl;
		exit(EXIT_FAILURE);
	}
}
void ParsedServerConfig::setMaxBodySize(long max_body_size)
{
    if(max_body_size > 60000000)
        _max_body_size = 60000000;
    else
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

void ParsedServerConfig::setErrorPages(short errorCode, std::string errorPage)
{
	_errorPages[errorCode] = errorPage;
}

void ParsedServerConfig::setMethods(const std::string &location, const std::vector<std::string> &methods)
{
	_methods[location] = methods;
}

void ParsedServerConfig::setLocationRoot(const std::string &location, const std::string &root)
{
	_locationRoots[location] = root;
}

void ParsedServerConfig::setLocationRet(const std::string &location, const std::string &root)
{
    _return[location] = root;
}

void ParsedServerConfig::setLocationIndex(const std::string &location, const std::string &index)
{
	_locationIndexes[location] = index;
}

void ParsedServerConfig::setLocationCgiPath(const std::string &location, const std::vector<std::string> &cgiPaths)
{
	_locationCgiPaths[location] = cgiPaths;
}

void ParsedServerConfig::setLocationCgiExt(const std::string &location, const std::vector<std::string> &cgiExts)
{
	_locationCgiExts[location] = cgiExts;
}

void ParsedServerConfig::setLocationAutoindex(const std::string &location, bool autoindex)
{
	_locationAutoindex[location] = autoindex;
}

std::string ParsedServerConfig::getHost() const
{
	struct in_addr addr;
	addr.s_addr = _host;
	return std::string(inet_ntoa(addr));
}

long ParsedServerConfig::getMaxBodySize() const
{
	return _max_body_size;
}

std::string ParsedServerConfig::getLocationRoot(const std::string &location) const
{
	std::map<std::string, std::string>::const_iterator it = _locationRoots.find(location);
	if (it != _locationRoots.end())
	{
		return it->second;
	}
	return "";
}

std::string ParsedServerConfig::getLocationIndex(const std::string &location) const
{
	std::map<std::string, std::string>::const_iterator it = _locationIndexes.find(location);
	if (it != _locationIndexes.end())
	{
		return it->second;
	}
	return "";
}


std::vector<std::string> ParsedServerConfig::getLocationCgiPath(const std::string &location) const
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = _locationCgiPaths.find(location);
	if (it != _locationCgiPaths.end())
	{
		return it->second;
	}
	return std::vector<std::string>();
}

std::vector<std::string> ParsedServerConfig::getLocationCgiExt(const std::string &location) const
{
	std::map<std::string, std::vector<std::string> >::const_iterator it = _locationCgiExts.find(location);
	if (it != _locationCgiExts.end())
	{
		return it->second;
	}
	return std::vector<std::string>();
}

bool ParsedServerConfig::getLocationAutoindex(const std::string &location) const
{
	std::map<std::string, bool>::const_iterator it = _locationAutoindex.find(location);
	if (it != _locationAutoindex.end())
	{
		return it->second;
	}
	return false;
}

const std::vector<ParsedServerConfig>& ParsedServerConfig::getServerParsedConfigs() const {
    return serverParsedConfigs;
}

const char *ParsedServerConfig::NoFileError::what() const throw()
{
	return "Shit, an Error in roi's part: Could not open the file";
}


std::vector<ServerConfig> ParsedServerConfig::getVector()
{
    std::vector<ServerConfig> servers;

    for (std::vector<ParsedServerConfig>::const_iterator it = serverParsedConfigs.begin();
         it != serverParsedConfigs.end(); ++it)
    {
        ServerConfig server;

        server.setName(it->_name);
        server.setRoot(it->_root);
        server.setIndex(it->_index);
        server.setHost(it->getHost());
        const std::vector<u_int16_t>& ports = it->getPorts();
        for (std::vector<u_int16_t>::const_iterator portIt = ports.begin(); portIt != ports.end(); ++portIt)
        {
            server.setPort(*portIt);
        }
        server.setMaxBodySize(it->getMaxBodySize());

        for (std::map<std::string, std::vector<std::string> >::const_iterator methodIt = it->_methods.begin();
             methodIt != it->_methods.end(); ++methodIt)
        {
            server.setMethods(methodIt->first, methodIt->second);
        }

        for (std::map<std::string, std::string>::const_iterator rootIt = it->_locationRoots.begin();
             rootIt != it->_locationRoots.end(); ++rootIt)
        {
            server.setLocationRoot(rootIt->first, rootIt->second);
        }

        for (std::map<std::string, std::string>::const_iterator indexIt = it->_locationIndexes.begin();
             indexIt != it->_locationIndexes.end(); ++indexIt)
        {
            server.setLocationIndex(indexIt->first, indexIt->second);
        }

        for (std::map<std::string, std::vector<std::string> >::const_iterator cgiPathIt = it->_locationCgiPaths.begin();
             cgiPathIt != it->_locationCgiPaths.end(); ++cgiPathIt)
        {
            server.setLocationCgiPath(cgiPathIt->first, cgiPathIt->second);
        }

        for (std::map<std::string, std::vector<std::string> >::const_iterator cgiExtIt = it->_locationCgiExts.begin();
             cgiExtIt != it->_locationCgiExts.end(); ++cgiExtIt)
        {
            server.setLocationCgiExt(cgiExtIt->first, cgiExtIt->second);
        }

        for (std::map<std::string, bool>::const_iterator autoindexIt = it->_locationAutoindex.begin();
             autoindexIt != it->_locationAutoindex.end(); ++autoindexIt)
        {
            server.setLocationAutoindex(autoindexIt->first, autoindexIt->second);
        }
        for (std::map<short, std::string>::const_iterator errorIt = it->_errorPages.begin();
             errorIt != it->_errorPages.end(); ++errorIt)
        {
            server.setErrorPages(errorIt->first, errorIt->second);
        }
        for (std::map<std::string, std::string>::const_iterator errorIt = it->_return.begin();
             errorIt != it->_return.end(); ++errorIt)
        {
            server.setLocationRet(errorIt->first, errorIt->second);
        }
        servers.push_back(server);
    }

    return servers;
}

void ParsedServerConfig::parseConfig(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw NoFileError();

    std::string line;
    std::string currentLocation;
    bool inLocationBlock = false;
    bool inServerBlock = false;
    bool clientMaxBodySizeSet = false;
    ParsedServerConfig currentConfig;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key;
        iss >> key;
        if (key == "server")
        {
            if (inServerBlock)
            {
                if (!clientMaxBodySizeSet)
                    currentConfig.setMaxBodySize(2000000);

                const std::vector<u_int16_t>& ports = currentConfig.getPorts();
                for (size_t i = 0; i < ports.size(); ++i)
                {
                    ParsedServerConfig newConfig = currentConfig;
                    newConfig._ports.clear();
                    newConfig.addPort(ports[i]);
                    serverParsedConfigs.push_back(newConfig);
                }

                currentConfig = ParsedServerConfig();
                clientMaxBodySizeSet = false;
            }
            inServerBlock = true;
            currentLocation.clear();
            currentConfig._errorPages.clear();
			currentConfig._ports.clear();
        }
        else if (key == "}")
        {
            if (inLocationBlock)
                inLocationBlock = false;
            else if (inServerBlock)
            {
                if (!clientMaxBodySizeSet)
                    currentConfig.setMaxBodySize(2000000);

                const std::vector<u_int16_t>& ports = currentConfig.getPorts();
                for (size_t i = 0; i < ports.size(); ++i)
                {
                    ParsedServerConfig newConfig = currentConfig;
                    newConfig._ports.clear();
                    newConfig.addPort(ports[i]);
                    serverParsedConfigs.push_back(newConfig);
                }

                inServerBlock = false;
                currentLocation.clear();
                currentConfig._methods.clear();
                currentConfig._locationRoots.clear();
                currentConfig._locationIndexes.clear();
                currentConfig._locationCgiPaths.clear();
                currentConfig._locationCgiExts.clear();
                currentConfig._locationAutoindex.clear();
                currentConfig._return.clear();
            }
        }
        else if (inLocationBlock && key == "allow_methods")
        {
            std::vector<std::string> methods;
            std::string method;
            while (iss >> method)
            {
                if (!method.empty() && method[method.size() - 1] == ';')
                    method.erase(method.size() - 1);
                methods.push_back(method);
            }
            currentConfig.setMethods(currentLocation, methods);
        }
        else if (inLocationBlock && key == "root")
        {
            std::string root;
            iss >> root;
            if (!root.empty() && root[root.size() - 1] == ';')
                root.erase(root.size() - 1);
            currentConfig.setLocationRoot(currentLocation, root);
        }
        else if (inLocationBlock && key == "return")
        {
            std::string ret;
            iss >> ret;
            if (!ret.empty() && ret[ret.size() - 1] == ';')
                ret.erase(ret.size() - 1);
            currentConfig.setLocationRet(currentLocation, ret);
        }
        else if (inLocationBlock && key == "index")
        {
            std::string index;
            iss >> index;
            if (!index.empty() && index[index.size() - 1] == ';')
                index.erase(index.size() - 1);
            currentConfig.setLocationIndex(currentLocation, index);
        }
        else if (inLocationBlock && key == "cgi_path")
        {
            std::vector<std::string> cgi_paths;
            std::string cgi_path;
            while (iss >> cgi_path)
            {
                if (!cgi_path.empty() && cgi_path[cgi_path.size() - 1] == ';')
                    cgi_path.erase(cgi_path.size() - 1);
                cgi_paths.push_back(cgi_path);
            }
            currentConfig.setLocationCgiPath(currentLocation, cgi_paths);
        }
        else if (inLocationBlock && key == "cgi_ext")
        {
            std::vector<std::string> cgi_exts;
            std::string cgi_ext;
            while (iss >> cgi_ext)
            {
                if (!cgi_ext.empty() && cgi_ext[cgi_ext.size() - 1] == ';')
                    cgi_ext.erase(cgi_ext.size() - 1);
                cgi_exts.push_back(cgi_ext);
            }
            currentConfig.setLocationCgiExt(currentLocation, cgi_exts);
        }
        else if (inLocationBlock && key == "autoindex")
        {
            std::string autoindex;
            iss >> autoindex;
            if (!autoindex.empty() && autoindex[autoindex.size() - 1] == ';')
                autoindex.erase(autoindex.size() - 1);
            bool autoindexValue = (autoindex == "on");
            currentConfig.setLocationAutoindex(currentLocation, autoindexValue);
        }
        else if (inServerBlock && !inLocationBlock)
        {
            if (key == "listen")
            {
                std::string portStr;
                while (iss >> portStr)
                {
                    if (!portStr.empty() && portStr[portStr.size() - 1] == ';')
                        portStr.erase(portStr.size() - 1);
                    u_int16_t port = static_cast<u_int16_t>(atoi(portStr.c_str()));
                    currentConfig.addPort(port);
                }
            }
            else if (key == "server_name")
            {
                std::string serverName;
                iss >> serverName;
                if (!serverName.empty() && serverName[serverName.size() - 1] == ';')
                    serverName.erase(serverName.size() - 1);
                currentConfig.setName(serverName);
            }
            else if (key == "host")
            {
                std::string host;
                iss >> host;
                if (!host.empty() && host[host.size() - 1] == ';')
                    host.erase(host.size() - 1);
                currentConfig.setHost(host);
            }
            else if (key == "root")
            {
                std::string root;
                iss >> root;
                if (!root.empty() && root[root.size() - 1] == ';')
                    root.erase(root.size() - 1);
                currentConfig.setRoot(root);
            }
            else if (key == "index")
            {
                std::string index;
                iss >> index;
                if (!index.empty() && index[index.size() - 1] == ';')
                    index.erase(index.size() - 1);
                currentConfig.setIndex(index);
            }
            else if (key == "error_page")
            {
                std::string errorCodes;
                std::string errorPage;
                std::string token;
                std::vector<std::string> tokens;

                while (iss >> token)
                {
                    tokens.push_back(token);
                }

                if (!tokens.empty())
                {
                    errorPage = tokens.back();
                    tokens.pop_back();
                }

                if (!errorPage.empty() && errorPage[errorPage.size() - 1] == ';')
                {
                    errorPage.erase(errorPage.size() - 1);
                }

                for (size_t i = 0; i < tokens.size(); ++i)
                {
                    std::istringstream errorCodeStream(tokens[i]);
                    short errorCodeInt;
                    errorCodeStream >> errorCodeInt;
                    currentConfig.setErrorPages(errorCodeInt, errorPage);
                }
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
                    currentLocation.erase(currentLocation.size() - 1);
            }
        }
    }

    if (inServerBlock)
    {
        if (!clientMaxBodySizeSet)
            currentConfig.setMaxBodySize(2000000);

        const std::vector<u_int16_t>& ports = currentConfig.getPorts();
        for (size_t i = 0; i < ports.size(); ++i)
        {
            ParsedServerConfig newConfig = currentConfig;
            newConfig._ports.clear();
            newConfig.addPort(ports[i]);
            serverParsedConfigs.push_back(newConfig);
        }
    }
    file.close();
}

std::ostream &operator<<(std::ostream &os, const ParsedServerConfig &config)
{
    os << "ParsedServerConfig:\n";
    os << "Host: " << config.getHost() << "\n";
    os << "Ports: ";
    const std::vector<u_int16_t>& ports = config.getPorts();
    for (std::vector<u_int16_t>::const_iterator portIt = ports.begin(); portIt != ports.end(); ++portIt)
    {
        os << *portIt << " ";
    }
    os << "\n";
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
    os << "Location Roots:\n";
    for (std::map<std::string, std::string>::const_iterator it = config._locationRoots.begin(); it != config._locationRoots.end(); ++it)
    {
        os << "  Location: " << it->first << " Root: " << it->second << "\n";
    }
    os << "Location Indexes:\n";
    for (std::map<std::string, std::string>::const_iterator it = config._locationIndexes.begin(); it != config._locationIndexes.end(); ++it)
    {
        os << "  Location: " << it->first << " Index: " << it->second << "\n";
    }
    os << "Location Cgi Paths:\n";
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = config._locationCgiPaths.begin(); it != config._locationCgiPaths.end(); ++it)
    {
        os << "  Location: " << it->first << " Cgi Path: ";
        for (std::vector<std::string>::const_iterator cgipath_it = it->second.begin(); cgipath_it != it->second.end(); ++cgipath_it)
        {
            os << *cgipath_it << " ";
        }
        os << "\n";
    }
    os << "Location Cgi Extensions:\n";
    for (std::map<std::string, std::vector<std::string> >::const_iterator it = config._locationCgiExts.begin(); it != config._locationCgiExts.end(); ++it)
    {
        os << "  Location: " << it->first << " Cgi Extension: ";
        for (std::vector<std::string>::const_iterator cgiext_it = it->second.begin(); cgiext_it != it->second.end(); ++cgiext_it)
        {
            os << *cgiext_it << " ";
        }
        os << "\n";
    }
    os << "Location Autoindex:\n";
    for (std::map<std::string, bool>::const_iterator it = config._locationAutoindex.begin(); it != config._locationAutoindex.end(); ++it)
    {
        os << "  Location: " << it->first << " Autoindex: " << (it->second ? "on" : "off") << "\n";
    }
    return os;
}

