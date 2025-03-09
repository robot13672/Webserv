#pragma once
#include "Webserv.hpp"
/* 
by roi
 */

class ServerConfig;

class ParsedServerConfig
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
		// Индексация по location(string), а значение будет вектор, который хранит все допустимые методы, _methods["root"][1] = {"GET", "POST", "DELETE"} 
		
		// Новые атрибуты для хранения директив внутри блоков location - roi 0227
		std::map<std::string, std::string> 					_locationRoots; //location /cgi-bin { /n root ./;
		std::map<std::string, std::string> 					_locationIndexes; // location /tours { /n 	index tours1.html;
	
		// temporally commented - roi 0227 - 0302
		std::map<std::string, std::vector<std::string> > 	_locationCgiPaths; // location /cgi-bin { /n cgi_path /usr/bin/python3 /bin/bash;
		std::map<std::string, std::vector<std::string> > 	_locationCgiExts; // 	location /red { /n	return /tours;
		std::map<std::string, bool> 						_locationAutoindex; // Для хранения значения директивы autoindex для каждой локации likeL: 'autoindex off;'
		std::vector<ParsedServerConfig> 					serverParsedConfigs; // implemented by Illia instead of extern std::vector<ParsedServerConfig> serverParsedConfigs; - commented by oleh 0309
		public:
		ParsedServerConfig();
        ParsedServerConfig(const ParsedServerConfig &other); // Copy constructor
        ParsedServerConfig& operator=(const ParsedServerConfig &other); // Copy assignment operator
        //SETTERS
        void setPort(u_int16_t port);
        void setHost(std::string host);
        void setMaxBodySize(long max_body_size);
        void setName(std::string name);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setErrorPages(short errorCode, 
		// void setErrorPages(std::map<short, std::string> errorPages);//нужно передавать уже заполненую мапу
		std::string errorPage);// edited by main && ros
		void setMethods(const std::string &location, const std::vector<std::string> &methods);
		// setters of 5 additional attributes in locations
		void setLocationRoot(const std::string &location, const std::string &root);
		void setLocationIndex(const std::string &location, const std::string &index);
		
		// temporally commented - roi 0227 - 0302
		void setLocationCgiPath(const std::string &location, const std::vector<std::string> &cgiPaths);
		void setLocationCgiExt(const std::string &location, const std::vector<std::string> &cgiExts);
		void setLocationAutoindex(const std::string &location, bool autoindex);
        //GETTERS
        // in_addr_t getHost(void);
        std::string getHost() const;
        u_int16_t getPort(void) const;
        int getListenFd(void);
        long getMaxBodySize(void) const;
		// setters of 5 additional attributes in locations
		std::string getLocationRoot(const std::string &location) const;
		std::string getLocationIndex(const std::string &location) const;
		
		// temporally commented - roi 0227 - 0302
		std::vector<std::string> getLocationCgiPath(const std::string &location) const;
		std::vector<std::string> getLocationCgiExt(const std::string &location) const;
		bool getLocationAutoindex(const std::string &location) const;

		std::vector<ServerConfig> getVector(); // implemented by main because std::vector<ParsedServerConfig> serverParsedConfigs is not extern anymore - commented by oleh 0309

		void parseConfig(const std::string &filename); //start with methods first - roi 0224
        friend std::ostream& operator<<(std::ostream &os, const ParsedServerConfig &config); // Перегрузка оператора << rpi 0224
		
		const std::vector<ParsedServerConfig>& getServerParsedConfigs() const; // To allow operator<<  after std::vector<ParsedServerConfig> serverParsedConfigs; is not extern anymore but became an attribute of the class ParsedServerConfig - commented by oleh 0309
		class NoFileError : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
};

// Определение переменной serverParsedConfigs. Deleted by Illiya
// extern std::vector<ParsedServerConfig> serverParsedConfigs;