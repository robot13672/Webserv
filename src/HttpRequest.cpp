#include "../inc/HttpRequest.hpp"
#include <algorithm>
#include <unistd.h>
#include <vector>  // Добавляем включение vector
#include <sstream> // Добавляем включение sstream

HttpRequest::HttpRequest(){
    _method = "";
    _uri = "";
    _httpVersion = "";
    _headers = std::map<std::string, std::string>();
    _body = "";
    _isChunked = false;
    _maxBodySize = 0;
    _path = "";
    _queryParams = std::map<std::string, std::string>();
}

HttpRequest::HttpRequest(const HttpRequest& copy) {
    _method = copy._method;
    _uri = copy._uri;
    _httpVersion = copy._httpVersion;
    _headers = copy._headers;
    _body = copy._body;
    _isChunked = copy._isChunked;
    _maxBodySize = copy._maxBodySize;
    _path = copy._path;
    _queryParams = copy._queryParams;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& copy) {
    if (this != &copy) {
        _method = copy._method;
        _uri = copy._uri;
        _httpVersion = copy._httpVersion;
        _headers = copy._headers;
        _body = copy._body;
        _isChunked = copy._isChunked;
        _maxBodySize = copy._maxBodySize;
        _path = copy._path;
        _queryParams = copy._queryParams;
    }
    return *this;
}

HttpRequest::HttpRequest(const std::string& rawRequest) {
    parseRequest(rawRequest);
}

HttpRequest::~HttpRequest() {
    _headers.clear();
    _queryParams.clear();
}

bool HttpRequest::parseRequest(const std::string& rawRequest) 
{
    std::istringstream requestStream(rawRequest);
    std::string firstLine;
    
    if (!std::getline(requestStream, firstLine))
        return false;
    
    if (!parseRequestLine(firstLine))
        return false;
        
    if (!parseHeaders(requestStream))
        return false;
        
    return parseBody(requestStream);
}

bool HttpRequest::parseRequest(int fd, size_t contentLength) 
{
    if (fd < 0 || contentLength == 0)
        return false;

    // Читаем весь запрос в строку
    std::string rawRequest;
    char buffer[16384];
    size_t bytesRead;
    size_t totalRead = 0;

    while (totalRead < contentLength && (bytesRead = read(fd, buffer, sizeof(buffer))) > 0) 
    {
        rawRequest.append(buffer, bytesRead);
        totalRead += bytesRead;
    }

    if (totalRead != contentLength)
        return false;

    // Используем stringstream для парсинга
    std::istringstream requestStream(rawRequest);
    std::string firstLine;
    if (!std::getline(requestStream, firstLine))
        return false;
    if (!parseRequestLine(firstLine))
        return false;    
    if (!parseHeaders(requestStream))
        return false;
        
    return parseBody(requestStream);
}

bool HttpRequest::parseRequestLine(const std::string& line) {
    std::istringstream lineStream(line);
    lineStream >> _method;
    std::string _fullUri;
    lineStream >> _fullUri;
    lineStream >> _httpVersion;
    if (lineStream.fail())
        return false;   
    parseUri(_fullUri);
    _uri = _fullUri;  // сохраняем оригинальный URI
    return true;
}

bool HttpRequest::parseHeaders(std::istringstream& requestStream) {
    std::string line;
    while (std::getline(requestStream, line) && line != "\r" && !line.empty()) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos)
            return false;
        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        // Trim whitespace
        value.erase(0, value.find_first_not_of(" "));
        value.erase(value.find_last_not_of(" \r") + 1);
        _headers[key] = value;
    }
    // Check for chunked transfer encoding after parsing headers
    std::string transferEncoding = getHeader("Transfer-Encoding");
    _isChunked = (transferEncoding == "chunked");
    return true;
}


bool HttpRequest::parseBody(std::istringstream& requestStream) {
    if (_isChunked) {
        return parseChunkedBody(requestStream);
    }

    // Получаем размер контента из заголовков
    std::string contentLength = getHeader("Content-Length");
    if (!contentLength.empty()) {
        size_t length = std::strtoul(contentLength.c_str(), NULL, 10);
        
        // Проверяем, не превышает ли размер максимально допустимый
        if (length > _maxBodySize) {
            _statusCode = 413; // Payload Too Large
            return false;
        }

        // Читаем тело запроса по частям
        char buffer[8192]; // Буфер для чтения по 8KB
        size_t totalRead = 0;
        _body.reserve(length); // Резервируем память заранее

        while (totalRead < length && !requestStream.eof()) {
            size_t remaining = length - totalRead;
            size_t toRead = std::min(remaining, sizeof(buffer));
            
            requestStream.read(buffer, toRead);
            size_t bytesRead = requestStream.gcount();
            
            if (bytesRead == 0) break;
            
            _body.append(buffer, bytesRead);
            totalRead += bytesRead;
        }

        return totalRead == length;
    }
    return true;
}
bool HttpRequest::parseChunkedBody(std::istringstream& requestStream) {
    std::string chunk_line;
    _body.clear();
    size_t totalSize = 0;

    while (std::getline(requestStream, chunk_line)) {
        // Удаляем \r если есть
        if (!chunk_line.empty() && chunk_line[chunk_line.length()-1] == '\r') {
            chunk_line = chunk_line.substr(0, chunk_line.length()-1);
        }

        // Парсим размер чанка
        size_t chunk_size = parseChunkSize(chunk_line);
        
        // Проверяем на последний чанк
        if (chunk_size == 0) {
            return true;
        }

        // Проверяем общий размер
        totalSize += chunk_size;
        if (totalSize > _maxBodySize) {
            _statusCode = 413; // Payload Too Large
            return false;
        }

        // Читаем данные чанка
        std::vector<char> chunk_data(chunk_size);
        requestStream.read(&chunk_data[0], chunk_size);
        
        if (requestStream.gcount() != static_cast<std::streamsize>(chunk_size)) {
            return false;
        }

        // Добавляем данные в тело
        _body.append(chunk_data.begin(), chunk_data.end());

        // Пропускаем CRLF после чанка
        std::getline(requestStream, chunk_line);
    }

    return false; // Неожиданный конец потока
}

// Добавьте в класс новый метод для установки кода состояния
void HttpRequest::setStatusCode(int code) {
    _statusCode = code;
}

size_t HttpRequest::parseChunkSize(const std::string& line) {
    size_t chunk_size = 0;
    std::istringstream hex_stream(line);
    hex_stream >> std::hex >> chunk_size;
    return chunk_size;
}

const std::string& HttpRequest::getMethod() const {
    return _method;
}

const std::string& HttpRequest::getUri() const {
    return _uri;
}

const std::string& HttpRequest::getHttpVersion() const {
    return _httpVersion;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const {
    return _headers;
}

const std::string& HttpRequest::getBody() const {
    return _body;
}

std::string HttpRequest::getHeader(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator iter = _headers.find(key);
    return (iter != _headers.end()) ? iter->second : "";
}

bool HttpRequest::isValidMethod() const {
    static const std::string validMethods[] = {"GET", "POST", "DELETE"};
    const size_t methodCount = sizeof(validMethods) / sizeof(validMethods[0]);
    
    for (size_t i = 0; i < methodCount; ++i) {
        if (_method == validMethods[i]) {
            return true;
        }
    }
    return false;
}

bool HttpRequest::isValidUri() const {
    return !_uri.empty() && _uri[0] == '/';
}

bool HttpRequest::isValidHttpVersion() const {
    return _httpVersion == "HTTP/1.1" || _httpVersion == "HTTP/1.0";
}

void HttpRequest::setMaxBodySize(size_t size) {
    _maxBodySize = size;
}

// bool HttpRequest::isChunkedTransfer() const {
//     return isChunked;
// }

void HttpRequest::parseUri(const std::string& fullUri) {
    size_t questionPos = fullUri.find('?');
    if (questionPos == std::string::npos) {
        _path = fullUri;
        return;
    }
    _path = fullUri.substr(0, questionPos);
    std::string queryString = fullUri.substr(questionPos + 1);
    // Парсим query параметры
    size_t start = 0;
    size_t end;
    while ((end = queryString.find('&', start)) != std::string::npos) {
        parseQueryParam(queryString.substr(start, end - start));
        start = end + 1;
    }
    parseQueryParam(queryString.substr(start));
}

void HttpRequest::parseQueryParam(const std::string& param) {
    size_t equalPos = param.find('=');
    if (equalPos != std::string::npos) {
        std::string key = param.substr(0, equalPos);
        std::string value = param.substr(equalPos + 1);
        _queryParams[key] = value;
    }
}

const std::string& HttpRequest::getPath() const {
    return _path;
}

const std::map<std::string, std::string>& HttpRequest::getQueryParams() const {
    return _queryParams;
}

std::string HttpRequest::getQueryParam(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = _queryParams.find(key);
    return (it != _queryParams.end()) ? it->second : "";
}

bool HttpRequest::hasQueryParam(const std::string& key) const {
    return _queryParams.find(key) != _queryParams.end();
}