#include "../inc/CGI.hpp"
#include "../inc/HttpRequest.hpp"
#include <string>
#include <iostream>
#include <algorithm>

void CGI::SetEnv(HttpRequest &request)
{
    // Clear any existing _cgiEnvironment variables
    _cgiEnv.clear();

    // Standard CGI _cgiEnvironment variables
    _cgiEnv["SERVER_SOFTWARE"]    = "Webserv/1.0";
    _cgiEnv["SERVER_NAME"]        = request.getHeader("IP");
    _cgiEnv["SERVER_PORT"]        = request.getHeader("PORT");
    _cgiEnv["SERVER_PROTOCOL"]    = request.getHttpVersion();
    _cgiEnv["GATEWAY_INTERFACE"]  = "CGI/1.1";

    // Request-specific variables
    _cgiEnv["REQUEST_METHOD"]     = request.getMethod();
    _cgiEnv["REQUEST_URI"]        = request.getUri();
    _cgiEnv["SCRIPT_NAME"]        = request.getPath();
    _cgiEnv["QUERY_STRING"]       = request.getQuery();
    _cgiEnv["PATH_INFO"]          = request.getPath();
    _cgiEnv["PATH_TRANSLATED"]    = std::string(getcwd(NULL, 0)) + request.getPath();

    // Headers
    _cgiEnv["CONTENT_TYPE"]       = request.getHeader("Content-Type");
    _cgiEnv["CONTENT_LENGTH"]     = request.getHeader("Content-Length");
    
    // Convert HTTP headers to CGI variables
    std::map<std::string, std::string> headers = request.getHeaders();
    std::map<std::string, std::string>::const_iterator it;
    for (it = headers.begin(); it != headers.end(); ++it) 
    {
        std::string header_name = it->first;
        
        // Convert header name to CGI format (HTTP_*)
        for (std::string::iterator str_it = header_name.begin(); 
             str_it != header_name.end(); ++str_it)
        {
            *str_it = toupper(*str_it);
        }
        header_name = "HTTP_" + header_name;
        
        // Replace '-' with '_' in header name
        for (std::string::iterator str_it = header_name.begin(); 
             str_it != header_name.end(); ++str_it)
        {
            if (*str_it == '-')
                *str_it = '_';
        }
        
        _cgiEnv[header_name] = it->second;
    }

    // Remote information
    _cgiEnv["REMOTE_ADDR"] = request.getHeader("IP");
    _cgiEnv["REMOTE_HOST"] = request.getHeader("Host");

    // Remove any empty variables
    std::map<std::string, std::string>::iterator map_it = _cgiEnv.begin();
    while (map_it != _cgiEnv.end()) 
    {
        if (map_it->second.empty())
            _cgiEnv.erase(map_it++);
        else
            ++map_it;
    }
}