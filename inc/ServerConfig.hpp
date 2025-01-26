#pragma once
#include "Webserv.hpp"


class ServerConfig
{
    private:
        struct sockaddr_in              _adress;
        u_int16_t                       _port;
        in_addr_t                       _host;
        int                             _listen_fd;
        long                            _max_body_size; // Возможно нужно будет изменить тип данных
        std::string                     _name;
        std::string                     root;
        std::string                     _index;// начальная страница
        std::map<short, std::string>    _errorPages; // пути на страницы с ошибками
        bool                            _autoindex;
        //std::vector<Location> _locations;
    public:
        ServerConfig();
        ServerConfig(std::string host, u_int16_t port);//для эмуляции отработанного конфиг файла
        
        //settings
        void setupServer(void);
        //SET
        void setFd(int fd);
        //GET
        in_addr_t getHost();
        u_int16_t getPort();
        int getListenFd();

};