#pragma once
#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "Webserv.hpp"

// class HttpResponse
// {
//     private:
//         ServerConfig _server;
        


//     public:
//         void setServer(ServerConfig &serv);


// };



#include <string>
#include <map>

class HttpResponse {
private:
    ServerConfig _server;
    std::string _httpVersion;
    int _statusCode;
    std::string _statusMessage;
    std::map<std::string, std::string> _headers;
    std::string _body;
    bool _chunked;

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
    void handleRequest(const std::string& path, const std::string& method);
    void setErrorResponse(int code, const std::string& message);
    void setRedirectResponse(const std::string& newLocation);

    // Main methods
    std::string toString() const;
    void clear();
    void sendFile(const std::string& filePath);
    //old
    void setServer(ServerConfig &serv);
};

#endif