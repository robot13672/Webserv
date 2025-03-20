#pragma once
#include "Webserv.hpp"

class ServerConfig;

class ParsedServerConfig
{
    private:
        std::vector<u_int16_t> _ports; // listen-changed 2 vector 0315
        in_addr_t _host; // hosthost 127.0.0.1
        long _max_body_size; // client_max_body_size
        std::string _name; // server_name
        std::string _root; // root
        std::string _index; // index
        std::map<short, std::string> _errorPages; // error_page
        std::map<std::string, std::vector<std::string> > _methods; // allow_methods
        std::map<std::string, std::string> _locationRoots; // location root
        std::map<std::string, std::string> _locationIndexes; // location index
        std::map<std::string, std::vector<std::string> > _locationCgiPaths; // location cgi_path
        std::map<std::string, std::vector<std::string> > _locationCgiExts; // location cgi_ext
        std::map<std::string, bool> _locationAutoindex; // location autoindex
        std::vector<ParsedServerConfig> serverParsedConfigs; // vector of server configurations
        std::map<std::string, std::string> _return; 

    public:
        ParsedServerConfig();
        ParsedServerConfig(const ParsedServerConfig &other); // Copy constructor
        ParsedServerConfig& operator=(const ParsedServerConfig &other); // Copy assignment operator

        // SETTERS
        void addPort(u_int16_t port); // because of _ports vectors
        void setHost(std::string host);
        void setMaxBodySize(long max_body_size);
        void setName(std::string name);
        void setRoot(std::string root);
        void setIndex(std::string index);
        void setErrorPages(short errorCode, std::string errorPage);
        void setMethods(const std::string &location, const std::vector<std::string> &methods);
        void setLocationRoot(const std::string &location, const std::string &root);
        void setLocationIndex(const std::string &location, const std::string &index);
        void setLocationCgiPath(const std::string &location, const std::vector<std::string> &cgiPaths);
        void setLocationCgiExt(const std::string &location, const std::vector<std::string> &cgiExts);
        void setLocationAutoindex(const std::string &location, bool autoindex);
        void setLocationRet(const std::string &location, const std::string &root);

        // GETTERS
        std::string getHost() const;
        const std::vector<u_int16_t>& getPorts() const; // because of _ports vectors
        long getMaxBodySize() const;
        std::string getLocationRoot(const std::string &location) const;
        std::string getLocationIndex(const std::string &location) const;
        std::vector<std::string> getLocationCgiPath(const std::string &location) const;
        std::vector<std::string> getLocationCgiExt(const std::string &location) const;
        bool getLocationAutoindex(const std::string &location) const;

        std::vector<ServerConfig> getVector();
        void parseConfig(const std::string &filename);
        friend std::ostream& operator<<(std::ostream &os, const ParsedServerConfig &config);
        const std::vector<ParsedServerConfig>& getServerParsedConfigs() const;

        class NoFileError : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
};