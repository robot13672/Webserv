#ifndef CGI_HPP
#define CGI_HPP

#include "Webserv.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

class HttpRequest;
class HttpResponse;

class CGI {
    private:
        std::map<std::string, std::string>  _cgiEnv;
        // int                                 _timeout;
        // bool                                _isRunning;
        int                                 _exitStatus; // Exit status of CGI process
        std::string                         _scriptPath;
        std::string                         _requestBody;
        // std::vector<std::string> _cookies;
    public:
        int inputPipe[2];
        int outputPipe[2];
        // void executeScript();
        CGI();
        ~CGI();
        
        void SetEnv(HttpRequest &request);
        void setScriptPath(const std::string& path) ;
        void setRequestBody(const std::string& body);
        bool handleCgiRequest(HttpRequest& request, HttpResponse& response);
        // void addCookie(const std::string& cookie) { _cookies.push_back(cookie); }
        std::string executeCgiScript();
        // const std::vector<std::string>& getCookies() const { return _cookies; }
        void parseResponse(const std::string& cgiOutput, HttpResponse& _response);
    private:
        // void SetEnv(HttpRequest& _request);
        bool isCgiRequest(const std::string& path);


};
#endif 