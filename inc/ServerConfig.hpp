#pragma once
#include "Webserv.hpp"


class ServerConfig
{
    private:
        struct sockaddr_in                                  _adress;		// is not taken from conf file!!
        u_int16_t                                           _port;
        in_addr_t                                           _host;
        int                                                 _listen_fd;		// is not taken from conf file!!
        long                                                _max_body_size; // Возможно нужно будет изменить тип данных - client_max_body_size, nach sch21 video max 20k. the same if is not incicated in conf. file
        std::string                                         _name;			//имя сервера или домена
        std::string                                         _root;			//корневая директория
        std::string                                         _index;			// начальная страница
        std::map<short, std::string>                        _errorPages; 	// пути на страницы с ошибками
        std::map<std::string, std::vector <std::string> > 	_methods;        //все допустимые методы

        std::map<std::string, std::string>                  _locationRoots;
        std::map<std::string, std::string>                  _locationIndexes;
        std::map<std::string, std::vector<std::string> >    _locationCgiPaths;
        std::map<std::string, std::vector<std::string> >    _locationCgiExts;
        std::map<std::string, bool>                         _locationAutoindex;
        public:
        std::map<std::string, std::string> _return; 
        ~ServerConfig();
        ServerConfig();
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
        void setErrorPages(short errorCode, std::string errorPage);//нужно передавать уже заполненую мапу
        void setMethods(const std::string &location, const std::vector<std::string> &methods);
        void setLocationRoot(const std::string &location, const std::string &root);
        void setLocationIndex(const std::string &location, const std::string &index);
        void setLocationCgiPath(const std::string &location, const std::vector<std::string> &cgiPaths);
        void setLocationCgiExt(const std::string &location, const std::vector<std::string> &cgiExts);
        void setLocationAutoindex(const std::string &location, bool autoindex);
        void setLocationRoots(const std::map<std::string, std::string> &locationRoots);
        void setLocationIndexes(const std::map<std::string, std::string> &locationIndexes);
        void setLocationCgiPaths(const std::map<std::string, std::vector<std::string> > &locationCgiPaths);
        void setLocationCgiExts(const std::map<std::string, std::vector<std::string> > &locationCgiExts);
        void setLocationAutoindexes(const std::map<std::string, bool> &locationAutoindex);
        void setMethods(const std::map<std::string, std::vector<std::string> > &methods);
        void setLocationRet(const std::string &location, const std::string &root);

        //GET
        std::string getHost();
        u_int16_t getPort(void);
        int getListenFd(void);
        long getMaxBodySize(void);

        std::string getLocationRoot(const std::string &location) const;
		std::string getLocationIndex(const std::string &location) const;

        std::vector<std::string> getLocationCgiPath(const std::string &location) const;
		std::vector<std::string> getLocationCgiExt(const std::string &location) const;
        bool getLocationAutoindex(const std::string &location) const;

        bool isAvailibleMethod(std::string path, std::string method);
        std::string getErrorPage(int errorCode) const;
        void parseErrorPage(const std::string& line);
        std::string getRoot();
        std::string getIndex();
        std::string getReturn(std::string _path);

        
};