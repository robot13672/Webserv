#pragma once
#include "Webserv.hpp"


class ServerConfig
{
    private:
        struct sockaddr_in              _adress;		// is not taken from conf file!!
        u_int16_t                       _port;
        in_addr_t                       _host;
        int                             _listen_fd;		// is not taken from conf file!!
        int                             _max_body_size; // Возможно нужно будет изменить тип данных - client_max_body_size, nach sch21 video max 20k. the same if is not incicated in conf. file
        std::string                     _name;			//имя сервера или домена
        std::string                     _root;			//корневая директория
        std::string                     _index;			// начальная страница
        std::map<short, std::string>    _errorPages; 	// пути на страницы с ошибками
        bool                            _autoindex;
        //std::vector<Location> _locations;
    public:
        ServerConfig();
        ServerConfig(std::string host, u_int16_t port);//для эмуляции отработанного конфиг файла
        
        //settings
        void setupServer(void);
        //SET
        void setFd(int fd);
        void setPort(u_int16_t port);
        void setHost(std::string host);
        void setMaxBodySize(int max_body_size);
        void setName(std::string name);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setErrorPages(std::map<short, std::string> errorPages);//нужно передавать уже заполненую мапу
        //GET
        in_addr_t getHost();
        u_int16_t getPort();
        int getListenFd();

};