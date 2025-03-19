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
        int                                 _exitStatus; // Exit status of CGI process
        std::string                         _scriptPath;
        std::string                         _requestBody;
    public:
        int inputPipe[2];
        int outputPipe[2];
        CGI();
        ~CGI();
        
        void SetEnv(HttpRequest &request);
        void setScriptPath(const std::string& path) ;
        void setRequestBody(const std::string& body);
        bool handleCgiRequest(HttpRequest& request, HttpResponse& response);

    private:
        std::string executeCgiScript();
        bool isCgiRequest(const std::string& path);
        void parseResponse(const std::string& cgiOutput, HttpResponse& _response);


};
#endif 