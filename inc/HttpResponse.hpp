#pragma once
#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "Webserv.hpp"
#include "HttpRequest.hpp"
#include "CGI.hpp"
// #include "Mime.hpp"

// class HttpResponse
// {
//     private:
//         ServerConfig _server;
        


//     public:
//         void setServer(ServerConfig &serv);


// };



#include <string>
#include <map>

class CGI;

class HttpResponse {
private:
    ServerConfig                        _server;
    std::string                         _httpVersion;
    int                                 _statusCode;
    std::string                         _statusMessage;
    std::map<std::string, std::string>  _headers;
    std::string                         _body;
    bool                                _chunked;
    HttpRequest                         _request;
    std::string                         _response;
    std::string                         _path;
    std::string                         _method;
    CGI*                                cgi;
    std::vector<std::string>            _cookies;
    std::string getOriginalFilename(const std::string& body);
    std::string createUniqueFilename(const std::string& dir, const std::string& originalName);

public:
    HttpResponse();
    HttpResponse(const HttpResponse& copy);
    HttpResponse& operator=(const HttpResponse& copy);
    ~HttpResponse();

    // Setters
    void setStatus(int code, const std::string& message);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    void setHttpVersion(const std::string& version);
    void setChunked(bool chunked);

    // Getters
    int getStatusCode() const;
    const std::string& getStatusMessage() const;
    const std::string& getHeader(const std::string& key) const;
    const std::string& getBody() const;
    bool isChunked() const;

    //ERORR
    void handleRequest();
    void setErrorResponse(int code, const std::string& message);
    void setRedirectResponse(const std::string& newLocation);

    bool isFileAccessible() ;
    std::string getCurrentTime();
    void handleDelete(const std::string& path);
    void handlePost();

    // Main methods
    std::string toString() const;
    void clear();
    void sendFile();
    //old
    void setServer(ServerConfig &serv);
    void handleResponse(const HttpRequest request) ;
    void setPath(const std::string& path);
    void setMethod(const std::string& method);
    void handleListFiles();
    void addCookie(const std::string& cookie);
    const std::vector<std::string>& getCookies() const;
};

#endif