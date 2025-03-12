#pragma once

#include <string>
#include <map>
#include <sstream>
#include <vector>  // Добавляем include для vector

class HttpRequest 
{
private:
    std::string method;
    std::string uri;
    std::string httpVersion;
    std::string query;
    std::map<std::string, std::string> headers;
    std::string body;
    long maxBodySize;  // Новый член класса
    long contentLength;
    bool isChunked;  // Флаг для chunked передачи
    bool isDone;
    bool isCGI;
    bool isTooLarge;
    std::string path;  // Часть URI до знака ?
    std::map<std::string, std::string> queryParams;  // Параметры после ?
    
    // Private helper method for chunked parsing
    bool parseChunkedBody(std::istringstream& requestStream);
    size_t parseChunkSize(const std::string& line);
    
    // Новый приватный метод для парсинга URI
    void parseUri(const std::string& fullUri);
    void parseQueryParam(const std::string& param);  // Добавляем объявление метода
    
public:
    // Constructors and destructor
    HttpRequest();
    explicit HttpRequest(const std::string& rawRequest);
    ~HttpRequest();
    void clear();
    
    // Parse methods
    bool parseRequest(const std::vector<char>& buffer, size_t contentLength);  // Исправляем сигнатуру
    bool parseRequest(const std::string& rawRequest);  // Добавляем обратно эту версию
    bool parseRequestLine(const std::string& line);
    bool parseHeaders(std::istringstream& requestStream);
    bool parseBody(std::istringstream& requestStream);
    void parseFullChankedBody(void);
    
    // Getters
    const std::string& getMethod() const;
    const std::string& getUri() const;
    const std::string& getHttpVersion() const;
    const std::map<std::string, std::string>& getHeaders() const;
    const std::string& getBody() const;
    const std::string& getQuery() const;
    std::string getHeader(const std::string& key) const;
    bool getStatus();
    long getContentLength();
    
    // Новые геттеры для query параметров
    const std::string& getPath() const;
    const std::map<std::string, std::string>& getQueryParams() const;
    std::string getQueryParam(const std::string& key) const;
    bool hasQueryParam(const std::string& key) const;
    
    // Validation methods
    bool isValidMethod() const;
    bool isValidUri() const;
    bool isValidHttpVersion() const;
    void isCgiRequest();

    // New method
    void setMaxBodySize(size_t size);
    bool isChunkedTransfer() const;

    bool isRequestTooLarge() const {
        return isTooLarge;
    }
};