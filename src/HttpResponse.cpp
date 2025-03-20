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
    if (getHeader("Content-Length").empty()) {
        std::stringstream ss;
        ss << body.size();
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

void HttpResponse::handleResponse(const HttpRequest request) 
{
    clear();
    this->_path = request.getPath();
    this->_method = request.getMethod();
    _request = request;
    handleRequest();
}

void HttpResponse::handleRequest() 
{
    setHeader("Server", "text/plain");
    setHeader("Date", getCurrentTime());
    setHeader("Connection", "keep-alive");

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

void HttpResponse::handleDelete(const std::string& filename) 
{
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
    {
        if (!this->_server.getIndex().empty())
            localPath = this->_server.getRoot() +this->_server.getIndex();
        else
            localPath = this->_server.getRoot() + "index.html";
    }
    else {
        localPath = this->_server.getRoot();
            if (localPath[localPath.length()-1] == '/')
                localPath = localPath.substr(0, localPath.length()-1);
            localPath.append(_path);
    }       
    //changes files to directories
    if (S_ISDIR(st.st_mode)){
            handleDirectoryListing(localPath);
            return false;
    }
    if (stat(localPath.c_str(), &st)!= 0) {
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

void HttpResponse::handleDirectoryListing(const std::string& path) {
    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> files;
    std::vector<std::string> directories;
    
    if ((dir = opendir(path.c_str())) == NULL) {
        sendErrorPage(500, "Cannot open directory");
        return;
    }
    
    // Получаем список файлов и директорий
    while ((ent = readdir(dir)) != NULL) {
        std::string name = ent->d_name;
        
        // Пропускаем текущую и родительскую директории
        if (name == "." || name == "..") {
            continue;
        }
        
        // Формируем полный путь
        std::string fullPath = path + "/" + name;
        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                directories.push_back(name);
            } else {
                files.push_back(name);
            }
        }
    }
    closedir(dir);
    
    // Сортируем списки файлов и директорий
    std::sort(directories.begin(), directories.end());
    std::sort(files.begin(), files.end());
    
    // Формируем HTML-страницу
    std::stringstream html;
    html << "<!DOCTYPE html>\n"
         << "<html>\n"
         << "<head>\n"
         << "    <title>Index of " << _path << "</title>\n"
         << "    <style>\n"
         << "        body { font-family: Arial, sans-serif; margin: 20px; }\n"
         << "        h1 { margin-bottom: 20px; }\n"
         << "        table { border-collapse: collapse; width: 100%; }\n"
         << "        th, td { text-align: left; padding: 8px; }\n"
         << "        tr:nth-child(even) { background-color: #f2f2f2; }\n"
         << "        a { text-decoration: none; }\n"
         << "        a:hover { text-decoration: underline; }\n"
         << "    </style>\n"
         << "</head>\n"
         << "<body>\n"
         << "    <h1>Index of " << _path << "</h1>\n"
         << "    <hr>\n"
         << "    <table>\n"
         << "        <tr>\n"
         << "            <th>Name</th>\n"
         << "            <th>Last Modified</th>\n"
         << "            <th>Size</th>\n"
         << "        </tr>\n";
    
    // Добавляем ссылку на родительскую директорию
    if (_path != "/") {
        html << "        <tr>\n"
             << "            <td><a href=\"../\">../</a></td>\n"
             << "            <td></td>\n"
             << "            <td>-</td>\n"
             << "        </tr>\n";
    }
    
    // Добавляем директории
    for (size_t i = 0; i < directories.size(); ++i) {
        html << "        <tr>\n"
             << "            <td><a href=\"" << directories[i] << "/\">" << directories[i] << "/</a></td>\n"
             << "            <td></td>\n"
             << "            <td>-</td>\n"
             << "        </tr>\n";
    }
    
    // Добавляем файлы
    for (size_t i = 0; i < files.size(); ++i) {
        struct stat st;
        std::string fullPath = path + "/" + files[i];
        if (stat(fullPath.c_str(), &st) == 0) {
            char timeStr[100];
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&st.st_mtime));
            
            html << "        <tr>\n"
                 << "            <td><a href=\"" << files[i] << "\">" << files[i] << "</a></td>\n"
                 << "            <td>" << timeStr << "</td>\n"
                 << "            <td>" << st.st_size << " bytes</td>\n"
                 << "        </tr>\n";
        }
    }
    
    html << "    </table>\n"
         << "    <hr>\n"
         << "</body>\n"
         << "</html>\n";
    
    // Устанавливаем код ответа и заголовки
    setStatus(200, "OK");
    setHeader("Content-Type", "text/html");
    setBody(html.str());
}

void HttpResponse::sendErrorPage(int code, const std::string& message) {
    // Сначала устанавливаем код ошибки и сообщение
    setStatus(code, message);
    if (code == 404 && _server.getLocationAutoindex(_path)){
        handleDirectoryListing(_server.getRoot());
        return;
    }

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
    {
        if (!this->_server.getIndex().empty())
        localPath = this->_server.getRoot() +this->_server.getIndex();
        else
        localPath = this->_server.getRoot() + "index.html";
    }
    else {
        localPath = this->_server.getRoot();
        if (localPath[localPath.length()-1] == '/')
        localPath = localPath.substr(0, localPath.length()-1);
        localPath.append(_path);
    }
    struct stat st;
    if (stat(localPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
            handleDirectoryListing(localPath);
            return;
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
            else if (ext == "jpeg")
                setHeader("Content-Type", "image/jpeg");
            else if (ext == "js") 
                setHeader("Content-Type", "application/javascript");
            else if (ext == "png") 
                setHeader("Content-Type", "image/png");
            else if (ext == "jpg" || ext == "jpeg") 
                setHeader("Content-Type", "image/jpeg");
            else if (ext == "gif") 
                setHeader("Content-Type", "image/gif");
            else if (ext == "pdf") 
                setHeader("Content-Type", "application/pdf");
            else if (ext == "xml") 
                setHeader("Content-Type", "application/xml");
            else if (ext == "json") 
                setHeader("Content-Type", "application/json");
            else 
                setHeader("Content-Type", "application/octet-stream");
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

std::string HttpResponse::getMethod()
{
    return _method;
}

std::string HttpResponse::getPath()
{
    return _path;
}