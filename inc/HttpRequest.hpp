#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <map>
#include <sstream>

class HttpRequest 
{
private:
    std::string _method;
    std::string _uri;
    std::string _httpVersion;
    std::map<std::string, std::string> _headers;
    std::string _body;
    int _statusCode;  // Новый член класса

    size_t _maxBodySize;  // Новый член класса
    bool _isChunked;  // Флаг для chunked передачи
    std::string _path;  // Часть URI до знака ?
    std::map<std::string, std::string> _queryParams;  // Параметры после ?
    
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
    HttpRequest(const HttpRequest& copy);
    HttpRequest& operator=(const HttpRequest& copy);
    
    // Parse methods
    bool parseRequest(const std::string& rawRequest);
    bool parseRequest(int fd, size_t contentLength);
    bool parseRequestLine(const std::string& line);
    bool parseHeaders(std::istringstream& requestStream);
    bool parseBody(std::istringstream& requestStream);
    
    // Getters
    const std::string& getMethod() const;
    const std::string& getUri() const;
    const std::string& getHttpVersion() const;
    const std::map<std::string, std::string>& getHeaders() const;
    const std::string& getBody() const;
    std::string getHeader(const std::string& key) const;
    void setStatusCode(int code);
    int getStatusCode() const;  // Новый геттер для кода состояния
    
    // Новые геттеры для query параметров
    const std::string& getPath() const;
    const std::map<std::string, std::string>& getQueryParams() const;
    std::string getQueryParam(const std::string& key) const;
    bool hasQueryParam(const std::string& key) const;
    
    // Validation methods
    bool isValidMethod() const;
    bool isValidUri() const;
    bool isValidHttpVersion() const;

    // New method
    void setMaxBodySize(size_t size);
    
    // New method
    bool isChunkedTransfer() const;
};

#endif  // HTTP_REQUEST_HPP