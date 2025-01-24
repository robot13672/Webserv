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
        fd_set _request_fd_pool;
        fd_set _response_fd_pool;
        int _max_fd;


    public:
        void setupServer(std::vector<ServerConfig> servers);
        bool isDublicateFd(std::vector<ServerConfig>::iterator cur);
        int findDublicateFr(std::vector<ServerConfig>::iterator cur);

        void startServers(void);
        void clearFdSets(void);
        void initializeServerConnections(void);
        void setupListeningSocket(int fd);
        
};

