#pragma once
#include "Webserv.hpp"
/* 
by roi
 */
// class LocationConfig
// {
//     private:
//         std::string                     _path;
//         std::string                     _root;			//корневая директория
//         std::string                     _index;			// начальная страница
//         bool                            _autoindex;
//         std::vector<std::string>        _methods;		//все допустимые методы
//         std::string                     _return;
//         std::vector<std::string>        _cgi_path;
//         std::vector<std::string>        _cgi_ext;
// };
// std::map Servers<int, ServerConfig>;

class LocationConfig;

class ServerConfig
{
    private:
		u_int16_t						 					_port;			// listen
		in_addr_t						 					_host; // hosthost 127.0.0.1
		long												_max_body_size; // Возможно нужно будет изменить тип данных - client_max_body_size, nach sch21 video max 20k. the same if is not incicated in conf. file
		std::string											_name;			//имя сервера или домена - server_name
		std::string											_root;			// dir where index.html is, root ./;
		std::string											_index;			//indexfile name like index.html - roi 0221
		std::map<short, std::string> 						_errorPages; 	// пути на страницы с ошибками
		// std::vector<LocationConfig>		_locations;     // конфигурации блоков location	 by roi
		std::map<std::string, std::vector <std::string> > 	_methods;     	// допустимые методы	 by roi
		// Индексация будет по location(string), а значение будет вектор, который хранит все допустимые методы, _methods["root"][1] = {"GET", "POST", "DELETE"} 
		
		struct sockaddr_in									_adress;		// is not taken from conf file!!
		int													_listen_fd;		// is not taken from conf file!!
		std::string											_logDirection;  //директория для логирование

    public:
        ServerConfig();
        ServerConfig(std::string host, u_int16_t port);//для эмуляции отработанного конфиг файла
        ServerConfig(const ServerConfig &other); // Copy constructor
        ServerConfig& operator=(const ServerConfig &other); // Copy assignment operator
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
        void setLogDirection(std::string logDirection);
        //GET
        // in_addr_t getHost(void);
        std::string getHost() const;
        u_int16_t getPort(void) const;
        int getListenFd(void);
        long getMaxBodySize(void) const;
		
		
        // void setMethods(const std::vector<std::string> &methods); // made by Kirrill b4 methods a map became - comment by roi 0224
		void setMethods(const std::string &location, const std::vector<std::string> &methods);
		void parseConfig(const std::string &filename); //start with methods first - roi 0224
        friend std::ostream& operator<<(std::ostream &os, const ServerConfig &config); // // Перегрузка оператора << rpi 0224
		class NoFileError : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};




};
