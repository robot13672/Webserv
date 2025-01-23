#pragma once
#include "Webserv.hpp"

class ServerConfig;
class Client;

class Server
{
    private:
        std::vector<ServerConfig> _servers;
        std::map<int, ServerConfig> allServers;
        std::map<int, Client> allClients;
        fd_set request_fd_pool;
        fd_set response_fd_pool;
        int max_fd;


    public:
        void setupServer(std::vector<ServerConfig> servers);
        bool isDublicateFd(std::vector<ServerConfig>::iterator cur);
        int findDublicateFr(std::vector<ServerConfig>::iterator cur);
};

