#include "../inc/HttpRequest.hpp"
#include <algorithm>
#include <unistd.h>

HttpRequest::HttpRequest() : method(""), uri(""), httpVersion(""), maxBodySize(0), isChunked(false) {}

HttpRequest::HttpRequest(const std::string& rawRequest) {
    parseRequest(rawRequest);
}

<<<<<<< HEAD
HttpRequest::HttpRequest(){
    _method_str[::GET] = "GET";
    _method_str[::POST] = "POST";
    _method_str[::DELETE] = "DELETE";
    _method_str[::PUT] = "PUT";
    _method_str[::HEAD] = "HEAD";
    _path = "";
    _query = "";
    _fragment = "";
    _body_str = "";
    _error_code = 0;
    _chunk_length = 0;
    _method = NONE;
    _method_index = 1;
    _state = Request_Line;
    _fields_done_flag = false;
    _body_flag = false;
    _body_done_flag = false;
    _chunked_flag = false;
    _body_length = 0;
    _storage = "";
    _key_storage = "";
    _multiform_flag = false;
    _boundary = "";
}

HttpRequest::~HttpRequest(){
    
}
=======
HttpRequest::~HttpRequest() {}
>>>>>>> origin

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

<<<<<<< HEAD
HttpRequest::HttpRequest(HttpRequest const &copy){
    this->_method_str = copy._method_str;
    this->_path = copy._path;
    this->_query = copy._query;
    this->_fragment = copy._fragment;
    this->_body_str = copy._body_str;
    this->_error_code = copy._error_code;
    this->_chunk_length = copy._chunked_flag;
    this->_method = copy._method;
    this->_method_index = copy._method_index;
    this->_state = copy._state;
    this->_fields_done_flag = copy._fields_done_flag;
    this->_body_flag = copy._body_flag;
    this->_body_done_flag = copy._body_done_flag;
    this->_chunked_flag = copy._chunked_flag;
    this->_body_length = copy._body_length;
    this->_storage = copy._storage;
    this->_key_storage = copy._key_storage;
    this->_multiform_flag = copy._multiform_flag;
    this->_boundary = copy._boundary;
}

HttpRequest &HttpRequest::operator=(HttpRequest const &copy){
    if (this != &copy)
    {
        _method_str = copy._method_str;
        _path = copy._path;
        _query = copy._query;
        _fragment = copy._fragment;
        _body_str = copy._body_str;
        _error_code = copy._error_code;
        _chunk_length = copy._chunked_flag;
        _method = copy._method;
        _method_index = copy._method_index;
        _state = copy._state;
        _fields_done_flag = copy._fields_done_flag;
        _body_flag = copy._body_flag;
        _body_done_flag = copy._body_done_flag;
        _chunked_flag = copy._chunked_flag;
        _body_length = copy._body_length;
        _storage = copy._storage;
        _key_storage = copy._key_storage;
        _multiform_flag = copy._multiform_flag;
        _boundary = copy._boundary;
    }
    return *this;
}
=======
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
    if (isChunked) {
        return parseChunkedBody(requestStream);
    }
    body.assign(std::istreambuf_iterator<char>(requestStream),
                std::istreambuf_iterator<char>());
    return true;
}

bool HttpRequest::parseChunkedBody(std::istringstream& requestStream) {
    std::string chunk_line;
    body.clear();
    
    while (std::getline(requestStream, chunk_line)) {
        // Remove \r if present
        if (!chunk_line.empty() && chunk_line[chunk_line.length()-1] == '\r') {
            chunk_line = chunk_line.substr(0, chunk_line.length()-1);
        }
        
        // Parse chunk size
        size_t chunk_size = parseChunkSize(chunk_line);
        if (chunk_size == 0) {
            return true; // End of chunks
        }
        
        // Check body size limit
        if (body.length() + chunk_size > maxBodySize) {
            return false;
        }
        
        // Read chunk data
        std::string chunk_data;
        chunk_data.resize(chunk_size);
        requestStream.read(&chunk_data[0], chunk_size);
        
        if (requestStream.gcount() != static_cast<std::streamsize>(chunk_size)) {
            return false;
        }
        
        body += chunk_data;
        
        // Skip the CRLF after chunk
        std::getline(requestStream, chunk_line);
    }
    
    return false; // Unexpected end of stream
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

bool HttpRequest::isValidUri() const {
    return !uri.empty() && uri[0] == '/';
}

bool HttpRequest::isValidHttpVersion() const {
    return httpVersion == "HTTP/1.1" || httpVersion == "HTTP/1.0";
}

void HttpRequest::setMaxBodySize(size_t size) {
    maxBodySize = size;
}

// bool HttpRequest::isChunkedTransfer() const {
//     return isChunked;
// }

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
>>>>>>> origin
