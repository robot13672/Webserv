#pragma once
#include "Webserv.hpp"

class ServerConfig;
class Client;

class Server
{
    private:
        std::vector<ServerConfig>   _servers;
        std::map<int, ServerConfig> _allServers;
        std::map<int, Client>       _allClients;
        fd_set                      _request_fd_pool;
        fd_set                      _response_fd_pool;
        int                         _max_fd;


    public:
        void setupServer(std::vector<ServerConfig> servers);
        bool isDublicateFd(std::vector<ServerConfig>::iterator cur);
        int findDublicateFr(std::vector<ServerConfig>::iterator cur);

        void startServers(void);
        void clearFdSets(void);
        void initializeServerConnections(void);
        void setupListeningSocket(int fd);
        
        void addNewConnect(ServerConfig &serv);
        void addToSet(int client_sock, fd_set &set);
        void removeFromSet(int client_sock, fd_set &set);
        
};

