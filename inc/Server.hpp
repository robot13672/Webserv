#pragma once
#include "Webserv.hpp"
#include "Client.hpp"
#include "HttpResponse.hpp"

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
        ~Server();
        void setupServer(std::vector<ServerConfig> servers);
        bool isDublicateFd(std::vector<ServerConfig>::iterator cur);
        int  findDublicateFr(std::vector<ServerConfig>::iterator cur);

        void startServers(void);
        void clearFdSets(void);
        void initializeServerConnections(void);
        void setupListeningSocket(int fd);
        
        void addNewConnect(ServerConfig &serv);
        void addToSet(int client_sock, fd_set &set);
        void removeFromSet(int client_sock, fd_set &set);
        void readRequest(int &fd, Client &client);
        void sendResponse(int &fd, Client &Client);

        void handleClientDisconnection(int clientFd);
        void handleReadError(int clientFd);
        void closeFd(int fd);
        void processClientData(Client &client, std::vector<char> buffer, int readedBytes);
        void checkTimeout(void);
        std::string createNewTxt(char *buff, int size);// функция для создания файла для буффера
};

