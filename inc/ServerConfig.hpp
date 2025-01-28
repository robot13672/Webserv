#pragma once
#include "Webserv.hpp"

/* 
by roi
 */
class LocationConfig
{
    private:
        std::string                     _path;
        std::string                     _root;			//корневая директория
        std::string                     _index;			// начальная страница
        bool                            _autoindex;
        std::vector<std::string>        _methods;		//все допустимые методы
        std::string                     _return;
        std::vector<std::string>        _cgi_path;
        std::vector<std::string>        _cgi_ext;
};

// std::map Servers<int, ServerConfig>;
class ServerConfig
{
    private:
        struct sockaddr_in              _adress;		// is not taken from conf file!!
        u_int16_t                       _port;
        in_addr_t                       _host;
        int                             _listen_fd;		// is not taken from conf file!!
        long                            _max_body_size; // Возможно нужно будет изменить тип данных - client_max_body_size, nach sch21 video max 20k. the same if is not incicated in conf. file
        std::string                     _name;			//имя сервера или домена
		std::vector<LocationConfig>     _locations;     // конфигурации блоков location	 by roi
		std::map<short, std::string>    _errorPages; 	// пути на страницы с ошибками
        std::string                     _logDirection;  //директория для логирование
    public:
        ServerConfig();
        ServerConfig(std::string host, u_int16_t port);//для эмуляции отработанного конфиг файла
        
        //settings
        void setupServer(void);
        //SET
        void setFd(int fd);
        void setPort(u_int16_t port);
        void setHost(std::string host);
        void setMaxBodySize(long max_body_size);
        void setName(std::string name);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setErrorPages(std::map<short, std::string> errorPages);//нужно передавать уже заполненую мапу
        void setMethods(std::vector<std::string> methods);
        void setLogDirection(std::string logDirection);
        //GET
        in_addr_t getHost(void);
        u_int16_t getPort(void);
        int getListenFd(void);
        long getMaxBodySize(void);

};