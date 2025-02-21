#include "../inc/HttpRequest.hpp"
#include <algorithm>
#include <unistd.h>
#include <vector>  // Добавляем include для vector
#include <iostream>

HttpRequest::HttpRequest() : method(""), uri(""), httpVersion(""), maxBodySize(0), isChunked(false) {}

HttpRequest::HttpRequest(const std::string& rawRequest) {
    parseRequest(rawRequest);
}

HttpRequest::~HttpRequest() {}

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

bool HttpRequest::parseRequest(const std::vector<char>& buffer, size_t contentLength) 
{
    if (buffer.empty() || contentLength == 0)
        return false;

    // Создаем строку из буфера
    std::string rawRequest(buffer.begin(), buffer.begin() + contentLength);
    
    // Используем stringstream для парсинга
    std::istringstream requestStream(rawRequest);
    std::string firstLine;
    if(!isChunked)
    {
        if (!std::getline(requestStream, firstLine))
            return false;
        
        if (!parseRequestLine(firstLine))
            return false;
        
        if (!parseHeaders(requestStream))
            return false;
    }
    
    isCgiRequest();// check for CGI
    return parseBody(requestStream);
}

bool HttpRequest::parseRequestLine(const std::string& line) {
    std::istringstream lineStream(line);
    lineStream >> method;
    std::string fullUri;
    lineStream >> fullUri;
    lineStream >> httpVersion;
    
    if (lineStream.fail())
        return false;
        
    parseUri(fullUri);
    uri = fullUri;  // сохраняем оригинальный URI
    
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
        
        headers[key] = value;
    }
    
    // Check for chunked transfer encoding after parsing headers
    std::string transferEncoding = getHeader("Transfer-Encoding");
    isChunked = (transferEncoding == "chunked");
    
    return true;
}

bool HttpRequest::parseBody(std::istringstream& requestStream) {
    if(isChunked) 
    {
        return parseChunkedBody(requestStream);
    }
    body.assign(std::istreambuf_iterator<char>(requestStream),
                std::istreambuf_iterator<char>());
    isDone = true;
    return isDone;
}

bool HttpRequest::parseChunkedBody(std::istringstream& buffer) {
    body.append(std::string(std::istreambuf_iterator<char>(buffer), 
                           std::istreambuf_iterator<char>()));

    // Check if body ends with "\r\n0\r\n\r\n" which indicates end of chunked transfer
    isDone = (body.find("\r\n0\r\n\r\n") != std::string::npos);
    return isDone; 
}

void HttpRequest::parseFullChankedBody()
{
    std::string processedBody;
    size_t pos = 0;
    size_t totalSize = 0;

    while (pos < body.length()) {
        // Находим конец строки с размером чанка
        size_t lineEnd = body.find("\r\n", pos);
        if (lineEnd == std::string::npos)
            break;

        // Извлекаем размер чанка в hex формате
        std::string chunkSizeHex = body.substr(pos, lineEnd - pos);
        size_t chunkSize;
        std::istringstream ss(chunkSizeHex);
        ss >> std::hex >> chunkSize;

        // Пропускаем размер чанка и \r\n
        pos = lineEnd + 2;

        // Если размер чанка 0, это конец данных
        if (chunkSize == 0)
            break;

        // Проверяем, достаточно ли данных осталось
        if (pos + chunkSize > body.length())
            break;

        // Копируем чанк напрямую, включая все байты (даже нулевые)
        processedBody.append(body.data() + pos, chunkSize);
        totalSize += chunkSize;

        // Перемещаемся к следующему чанку, пропуская \r\n
        pos += chunkSize + 2;
    }

    body = processedBody;
}

size_t HttpRequest::parseChunkSize(const std::string& line) {
    size_t chunk_size = 0;
    std::istringstream hex_stream(line);
    hex_stream >> std::hex >> chunk_size;
    return chunk_size;
}

const std::string& HttpRequest::getMethod() const {
    return method;
}

const std::string& HttpRequest::getUri() const {
    return uri;
}

const std::string& HttpRequest::getHttpVersion() const {
    return httpVersion;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const {
    return headers;
}

const std::string& HttpRequest::getBody() const {
    return body;
}

std::string HttpRequest::getHeader(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator iter = headers.find(key);
    return (iter != headers.end()) ? iter->second : "";
}

bool HttpRequest::isValidMethod() const {
    static const std::string validMethods[] = {"GET", "POST", "DELETE"};
    const size_t methodCount = sizeof(validMethods) / sizeof(validMethods[0]);
    
    for (size_t i = 0; i < methodCount; ++i) {
        if (method == validMethods[i]) {
            return true;
        }
    }
    return false;
}

void HttpRequest::isCgiRequest()
{
    // Check if path ends with .py or .cgi
    size_t dot = path.find_last_of('.');
    if (dot != std::string::npos) {
        std::string extension = path.substr(dot);
        isCGI = (extension == ".py" || extension == ".cgi");
        return;
    }
    isCGI = false;
}

bool HttpRequest::isValidUri() const {
    return !uri.empty() && uri[0] == '/';
}

bool HttpRequest::isValidHttpVersion() const {
    return httpVersion == "HTTP/1.1" || httpVersion == "HTTP/1.0";
}

void HttpRequest::setMaxBodySize(size_t size) {
    maxBodySize = size;
}

bool HttpRequest::isChunkedTransfer() const 
{
    return isChunked;
}

void HttpRequest::parseUri(const std::string& fullUri) {
    size_t questionPos = fullUri.find('?');
    
    if (questionPos == std::string::npos) {
        path = fullUri;
        return;
    }
    
    path = fullUri.substr(0, questionPos);
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
        queryParams[key] = value;
    }
}

const std::string& HttpRequest::getPath() const {
    return path;
}

const std::map<std::string, std::string>& HttpRequest::getQueryParams() const {
    return queryParams;
}

std::string HttpRequest::getQueryParam(const std::string& key) const {
    std::map<std::string, std::string>::const_iterator it = queryParams.find(key);
    return (it != queryParams.end()) ? it->second : "";
}

bool HttpRequest::hasQueryParam(const std::string& key) const {
    return queryParams.find(key) != queryParams.end();
}

bool HttpRequest::getStatus()
{
    return isDone;
}