#include "../inc/HttpResponse.hpp"
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

HttpResponse::HttpResponse() : 
    _httpVersion("HTTP/1.1"),
    _statusCode(200),
    _statusMessage("OK"),
    _headers(),
    _body(),
    _chunked(false),
    _method(""),
    _path(""),
    _request(),
    _response(),
    cgi(new CGI()),
    _cookies() {}

HttpResponse::HttpResponse(const HttpResponse& copy) :
    _server(copy._server),
    _httpVersion(copy._httpVersion),
    _statusCode(copy._statusCode),
    _statusMessage(copy._statusMessage),
    _headers(copy._headers),
    _body(copy._body),
    _chunked(copy._chunked),
    _method(copy._method),
    _path(copy._path),
    _request(copy._request),
    _response(copy._response),
    cgi(new CGI(*copy.cgi)),
    _cookies(copy._cookies) {}

HttpResponse& HttpResponse::operator=(const HttpResponse& copy) {
    if (this != &copy) {
        _server = copy._server;
        _httpVersion = copy._httpVersion;
        _statusCode = copy._statusCode;
        _statusMessage = copy._statusMessage;
        _headers = copy._headers;
        _body = copy._body;
        _chunked = copy._chunked;
        _path = copy._path;
        _method = copy._method;
        _request = copy._request;
        _response = copy._response;
        delete cgi;  // Delete old CGI before assigning new one
        cgi = new CGI(*copy.cgi);
        _cookies = copy._cookies;
    }
    return *this;
}

HttpResponse::~HttpResponse() {
    _headers.clear();
    _body.clear();
    delete cgi;
}

void HttpResponse::setServer(ServerConfig &serv) {
    _server = serv;
}

void HttpResponse::setStatus(int code, const std::string& message) {
    _statusCode = code;
    _statusMessage = message;
}

void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    _headers[key] = value;
}

void HttpResponse::setBody(const std::string& body) {
    _body = body;
    if (!_chunked) {
        std::stringstream ss;
        ss << body.length();
        setHeader("Content-Length", ss.str());
    }
}

void HttpResponse::setHttpVersion(const std::string& version) {
    _httpVersion = version;
}

void HttpResponse::setChunked(bool chunked) {
    _chunked = chunked;
}

int HttpResponse::getStatusCode() const {
    return _statusCode;
}

const std::string& HttpResponse::getStatusMessage() const {
    return _statusMessage;
}

const std::string& HttpResponse::getHeader(const std::string& key) const {
    static const std::string empty;
    std::map<std::string, std::string>::const_iterator it = _headers.find(key);
    return (it != _headers.end()) ? it->second : empty;
}

const std::string& HttpResponse::getBody() const {
    return _body;
}

bool HttpResponse::isChunked() const {
    return _chunked;
}

void HttpResponse::handleResponse(const HttpRequest request) {
    this->_path = request.getPath();
    this->_method = request.getMethod();
    _request = request;
    handleRequest();
}

void HttpResponse::handleRequest() {
    clear();
    setHeader("Server", "text/plain");
    setHeader("Date", getCurrentTime());
    setHeader("Connection", "keep-alive");

    // if (_request.isRequestTooLarge()) {
    //     sendErrorPage(413, "Content Too Large");
    //     return;
    // }
    if (_request.getContentLength() >= _server.getMaxBodySize())
    {
        sendErrorPage(413, "Payload Too Large");
        return;
    }

    if (_server.isAvailibleMethod(_path, _method) == false) {
        sendErrorPage(405, "Method Not Allowed");
        setHeader("Allow", "GET, POST, DELETE");
        return;
    }
    if (_method != "GET" && _method != "POST" && _method != "DELETE") {
        sendErrorPage(405, "Method Not Allowed");
        setHeader("Allow", "GET, POST, DELETE");
        return;
    }
    if (_path == "/list-files") {
        handleListFiles();
        return;
    }
    if (_path.find("/cgi-bin/") == 0) {
        
        CGI cgi;
        if (cgi.handleCgiRequest(_request, *this)) {
            return;
        }
    }

    if (_method == "DELETE" && _path == "/delete-file") {
        std::string filename = _request.getBody();
        handleDelete(filename);
        return;
    }

    if (!isFileAccessible()) {
        return;
    }
    if (_method == "GET") {
        sendFile();
    } else if (_method == "POST") {
        handlePost();
    }
}

void HttpResponse::handleListFiles() {
    DIR *dir;
    struct dirent *ent;
    std::vector<std::map<std::string, std::string> > files;
    
    setHeader("Access-Control-Allow-Origin", "*");
    setHeader("Access-Control-Allow-Methods", "GET, DELETE, OPTIONS");
    setHeader("Content-Type", "application/json");
    
    if ((dir = opendir("upload/")) == NULL) {
        if (mkdir("upload/", 0777) != 0) {
            sendErrorPage(500, "Cannot create directory");
            return;
        }
        dir = opendir("upload/");
        if (dir == NULL) {
            sendErrorPage(500, "Cannot open directory");
            return;
        }
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        if (ent->d_type == DT_REG) {
            struct stat st;
            std::string fullPath;
            fullPath.append("upload/");
            fullPath.append(ent->d_name);
            
            if (stat(fullPath.c_str(), &st) == 0) {
                std::map<std::string, std::string> file;
                file["name"] = ent->d_name;
                std::stringstream ss;
                ss << st.st_size;
                file["size"] = ss.str();
                files.push_back(file);
            }
        }
    }
    closedir(dir);

    std::stringstream jsonResponse;
    jsonResponse << "[";
    for (size_t i = 0; i < files.size(); ++i) {
        if (i > 0) {
            jsonResponse << ",";
        }
        jsonResponse << "{\"name\":\"";
        jsonResponse << files[i]["name"];
        jsonResponse << "\",\"size\":\"";
        jsonResponse << files[i]["size"];
        jsonResponse << "\"}";
    }
    jsonResponse << "]";

    setStatus(200, "OK");
    setBody(jsonResponse.str());
}

void HttpResponse::handleDelete(const std::string& filename) {

    std::string fullPath;
    fullPath.append("upload/");
    fullPath.append(filename);
    
    if (access(fullPath.c_str(), F_OK) != 0) {
        sendErrorPage(404, "File not found");
        return;
    }

    if (unlink(fullPath.c_str()) == 0) {
        setStatus(200, "OK");
        setHeader("Content-Type", "text/plain");
        setHeader("Access-Control-Allow-Origin", "*");
        setHeader("Access-Control-Allow-Methods", "DELETE");
        setBody("File successfully deleted");
    } else {
        if (errno == EACCES) {
            sendErrorPage(403, "Forbidden");
        } else {
            sendErrorPage(500, "Internal Server Error");
        }
    }

}

std::string HttpResponse::getOriginalFilename(const std::string& body) {
    std::string filename;
    size_t filenameStart = body.find("filename=\"");
    
    if (filenameStart != std::string::npos) {
        filenameStart += 10;
        size_t filenameEnd = body.find("\"", filenameStart);
        if (filenameEnd != std::string::npos) {
            filename = body.substr(filenameStart, filenameEnd - filenameStart);
        }
    }
    return filename;
}

std::string HttpResponse::createUniqueFilename(const std::string& dir, const std::string& originalName) {
    std::string baseName;
    std::string extension;
    size_t dotPos = originalName.find_last_of('.');
    
    if (dotPos != std::string::npos) {
        baseName = originalName.substr(0, dotPos);
        extension = originalName.substr(dotPos);
    } else {
        baseName = originalName;
        extension = "";
    }

    std::string fullPath;
    fullPath.append(dir);
    fullPath.append(originalName);
    if (access(fullPath.c_str(), F_OK) != 0) {
        return fullPath;
    }

    size_t openParenPos = baseName.find_last_of('(');
    size_t closeParenPos = baseName.find_last_of(')');
    int counter = 1;
    std::string newPath;
    if (openParenPos != std::string::npos && closeParenPos != std::string::npos){
        std::string counterStr = baseName.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
        counter = atoi(counterStr.c_str());
        counter++;
        baseName = baseName.substr(0, openParenPos);
    }
    do {
        std::stringstream ss;
        ss << dir << baseName << "(" << counter << ")" << extension;
        newPath = ss.str();
        counter++;
    } while (access(newPath.c_str(), F_OK) == 0);

    return newPath;
}

void HttpResponse::handlePost() {

    struct stat st;
    std::string dir = "upload/";
    
    if (stat(dir.c_str(), &st) != 0) {
        if (mkdir(dir.c_str(), 0777) != 0) {
            sendErrorPage(500, "Cannot create directory");
            return;
        }
    }

    if (access(dir.c_str(), W_OK) != 0) {
        sendErrorPage(403, "Forbidden: Cannot write to directory");
        return ;
    }

    // Check Content-Length early to avoid processing large files
    std::string contentLengthStr = _request.getHeader("Content-Length");
    if (!contentLengthStr.empty()) {
        long contentLength = atol(contentLengthStr.c_str());
        if (contentLength > _server.getMaxBodySize()) {
            sendErrorPage(413, "Content Too Large");
            return;
        }
    }

    std::string contentType = _request.getHeader("Content-Type");
    if (contentType.empty()) {
        sendErrorPage(400, "Bad Request: Missing Content-Type");
        return;
    }

    size_t boundaryPos = contentType.find("boundary=");
    if (boundaryPos == std::string::npos) {
        sendErrorPage(400, "Bad Request: No boundary in multipart/form-data");
        return;
    }
    
    std::string boundary;
    if (boundaryPos != std::string::npos) {
        boundary.append("--");
        boundary.append(contentType.substr(boundaryPos + 9));
    }

    const std::string& body = _request.getBody();
    
    std::string originalName = getOriginalFilename(body);
    if (originalName.empty()) {
        sendErrorPage(400, "Bad Request: No filename found");
        return;
    }

    size_t headerEnd = body.find("\r\n\r\n");
    if (headerEnd == std::string::npos) {
        sendErrorPage(400, "Bad Request: Invalid multipart format");
        return;
    }
    
    size_t fileStart = headerEnd + 4;
    size_t fileEnd = body.find(boundary, fileStart) - 4;
    if (fileEnd == std::string::npos) {
        sendErrorPage(400, "Bad Request: Invalid file format");
        return;
    }
    
    std::string fileContent = body.substr(fileStart, fileEnd - fileStart);

    
    std::string finalPath = createUniqueFilename(dir, originalName);
    std::ofstream outFile;
    outFile.open(finalPath.c_str(), std::ios::binary);
    if (!outFile.is_open()) {
        sendErrorPage(500, "Internal Server Error: Cannot create file");
        return;
    }

    outFile.write(fileContent.c_str(), fileContent.length());
    outFile.close();

    setStatus(201, "Created");
    setHeader("Content-Type", "text/plain");
    setBody("File successfully uploaded as: " + finalPath);
}

bool HttpResponse::isFileAccessible() {
    struct stat st;
    
    std::string localPath;
    if (_path == "/")
        localPath = "assets/html/index.html";
    else {
        size_t dotPos = _path.find_last_of('.');
        if (dotPos != std::string::npos) {

            std::string extension = _path.substr(dotPos);
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                localPath = "assets/html";
                localPath.append(_path);
            } else {
                localPath = "assets/html";
                localPath.append(_path);
                localPath.append(".html");
            }
        } else {
            localPath = "assets/html";
            localPath.append(_path);
            localPath.append(".html");
        }
    }          
    if (stat(localPath.c_str(), &st) != 0) {
        sendErrorPage(404, "Not Found");
        return false;
    }
    if (access(localPath.c_str(), R_OK) != 0) {
        sendErrorPage(403, "Forbidden");
        return false;
    }
    return true;
}

std::string HttpResponse::getCurrentTime() {
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tm);
    return std::string(buf);
}

void HttpResponse::sendErrorPage(int code, const std::string& message) {
    // Сначала устанавливаем код ошибки и сообщение
    setStatus(code, message);

    // 1. Проверяем наличие страницы ошибки в конфиге
    std::string configPath = _server.getErrorPage(code);
    std::string localPath;
    bool errorPageFound = false;
    
    if (!configPath.empty()) {
        localPath = "assets/error_pages" + configPath;
        std::ifstream file(localPath.c_str(), std::ios::binary);
        if (file) {
            // Нашли страницу ошибки в конфиге
            std::stringstream buffer;
            buffer << file.rdbuf();
            setHeader("Content-Type", "text/html");
            setBody(buffer.str());
            errorPageFound = true;
        }
    }
    
    // 2. Если в конфиге нет, проверяем стандартные страницы ошибок
    if (!errorPageFound) {
        localPath = "assets/error_pages/" + intToString(code) + ".html";
        std::ifstream file(localPath.c_str(), std::ios::binary);
        if (file) {
            // Нашли стандартную страницу ошибки
            std::stringstream buffer;
            buffer << file.rdbuf();
            setHeader("Content-Type", "text/html");
            setBody(buffer.str());
            errorPageFound = true;
        }
    }
    // 3. Если ничего не сработало, используем простую HTML страницу
    if (!errorPageFound) {
        std::stringstream ss;
        ss << "<!DOCTYPE html>\n<html>\n<head>\n";
        ss << "<title>Error " << code << "</title>\n";
        ss << "<style>body{font-family:Arial,sans-serif;background:#1a1a1a;color:#fff;";
        ss << "display:flex;justify-content:center;align-items:center;height:100vh;margin:0;}</style>\n";
        ss << "</head>\n<body>\n";
        ss << "<div><h1>Error " << code << "</h1>\n";
        ss << "<p>" << message << "</p>\n";
        ss << "<a href='/' style='color:#00ff9d;text-decoration:none;'>Return to Home</a></div>\n";
        ss << "</body>\n</html>";
        setHeader("Content-Type", "text/html");
        setBody(ss.str());
    }
}

void HttpResponse::sendFile() {
    std::string localPath;
    if (_path == "/")
        localPath = "assets/html/index.html";
    else {
        size_t dotPos = _path.find_last_of('.');
        if (dotPos != std::string::npos) {
            std::string extension = _path.substr(dotPos);
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                localPath = "assets/html";
                localPath.append(_path);
            }
            else if(extension == ".py") {
                localPath = "assets/html";
                localPath.append(_path);}
              else {
                localPath = "assets/html";
                localPath.append(_path);
                localPath.append(".html");
            }
        } else {
            localPath = "assets/html";
            localPath.append(_path);
            localPath.append(".html");
        }
    
    }
    std::ifstream file(localPath.c_str(), std::ios::binary);
    if (!file) {
        sendErrorPage(404, "Not Found");
        return;
    }
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    if (fileSize > _server.getMaxBodySize()) {
        sendErrorPage(413, "Content Too Large");
        return;
    }
    try {
        std::stringstream buffer;
        buffer << file.rdbuf();
        setBody(buffer.str());

        size_t dot = _path.find_last_of('.');
        if (dot != std::string::npos) {
            std::string ext = _path.substr(dot + 1);
            if (ext == "html") 
                setHeader("Content-Type", "text/html");
            else if (ext == "css") 
                setHeader("Content-Type", "text/css");
            else if (ext == "txt") 
                setHeader("Content-Type", "text/plain");
            else if (ext == "js") 
                setHeader("Content-Type", "application/javascript");
            else if (ext == "json") 
                setHeader("Content-Type", "application/json");
            else if (ext == "xml") 
                setHeader("Content-Type", "application/xml");
            else if (ext == "jpg") 
                setHeader("Content-Type", "image/jpg");
            else if (ext == "jpeg") 
                setHeader("Content-Type", "image/jpeg");
            else if (ext == "png") 
                setHeader("Content-Type", "image/png");
            else if (ext == "gif") 
                setHeader("Content-Type", "image/gif");
            else if (ext == "mp3") 
                setHeader("Content-Type", "audio/mpeg");
            else if (ext == "mp4") 
                setHeader("Content-Type", "video/mp4");
            else if (ext == "pdf") 
                setHeader("Content-Type", "application/pdf");
            else if (ext == "zip") 
                setHeader("Content-Type", "application/zip");
            else if (ext == "doc") 
                setHeader("Content-Type", "application/msword");
            else if (ext == "docx") 
                setHeader("Content-Type", "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
            else if (ext == "xls") 
                setHeader("Content-Type", "application/vnd.ms-excel");
            else if (ext == "xlsx") 
                setHeader("Content-Type", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
            else if (ext == "bin") 
                setHeader("Content-Type", "application/octet-stream");
            else 
                setHeader("Content-Type", "application/txt");
        }
        setStatus(200, "OK");
    } catch (const std::exception& e) {
        sendErrorPage(500, "Internal Server Error");
    }
}

std::string HttpResponse::toString() const {
    std::stringstream response;
    
    response << _httpVersion << " " << _statusCode << " " << _statusMessage << "\r\n";
    
    std::map<std::string, std::string>::const_iterator it;
    for (it = _headers.begin(); it != _headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    
    std::vector<std::string>::const_iterator cookieIt;
    for (cookieIt = _cookies.begin(); cookieIt != _cookies.end(); ++cookieIt) {
        response << "Set-Cookie: " << *cookieIt << "\r\n";
    }
    
    response << "\r\n";
    
    response << _body;
    
    return response.str();
}

void HttpResponse::clear() {
    _statusCode = 200;
    _statusMessage = "OK";
    _headers.clear();
    _body.clear();
    _chunked = false;
    _path = "";
    _method = "";
    _response = "";
    _cookies.clear();
}

void HttpResponse::setPath(const std::string& path) { 
    _path = path;
}
void HttpResponse::setMethod(const std::string& method) { 
    _method = method; 
}
void HttpResponse::addCookie(const std::string& cookie) { 
    _cookies.push_back(cookie); 
}
const std::vector<std::string>& HttpResponse::getCookies() const { 
    return _cookies; 
}