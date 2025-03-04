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
    _chunked(false),
    _path(""),
    _method(""), 
    cgi(new CGI()){}

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
    _request(copy._request) {}

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
    // Добавляем базовые заголовки
    setHeader("Server", "text/plain");
    setHeader("Date", getCurrentTime());
    setHeader("Connection", "keep-alive");
    
    

    std::cout << "Method: " << _method << std::endl;
    // Проверяем метод до проверки файла
    if (_method != "GET" && _method != "POST" && _method != "DELETE") {
        setErrorResponse(405, "Method Not Allowed");
        setHeader("Allow", "GET, POST, DELETE");
        return;
    }
    // Специальная обработка для list-files API endpoint
    if (_path == "/list-files") {
        handleListFiles();
        return;
    }
    if (_path.find("/cgi-bin/") == 0) {
        CGI cgi;
        std::cout << "DEBUG: Handling CGI request for path: " << _path << std::endl;
        if (cgi.handleCgiRequest(_request, *this)) {
            return;
        }
    }

    if (_method == "DELETE" && _path == "/delete-file") {
        std::string filename = _request.getBody();  // Get filename from request
        handleDelete(filename);
        return;
    }

    // Проверяем существование и доступность файла
    if (!isFileAccessible()) {
        return; // isFileAccessible устанавливает соответствующий код ошибки
    }
    // Для GET и POST
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
            setErrorResponse(500, "Cannot create directory");
            return;
        }
        dir = opendir("upload/");
        if (dir == NULL) {
            setErrorResponse(500, "Cannot open directory");
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
                // Use stringstream instead of to_string (C++11)
                std::stringstream ss;
                ss << st.st_size;
                file["size"] = ss.str();
                files.push_back(file);
            }
        }
    }
    closedir(dir);

    // Create JSON response using C++98 string concatenation
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
    
    // Check if file exists
    if (access(fullPath.c_str(), F_OK) != 0) {
        setErrorResponse(404, "File not found");
        return;
    }

    // Try to delete the file
    if (unlink(fullPath.c_str()) == 0) {
        setStatus(200, "OK");
        setHeader("Content-Type", "text/plain");
        setHeader("Access-Control-Allow-Origin", "*");
        setHeader("Access-Control-Allow-Methods", "DELETE");
        setBody("File successfully deleted");
    } else {
        if (errno == EACCES) {
            setErrorResponse(403, "Forbidden");
        } else {
            setErrorResponse(500, "Internal Server Error");
        }
    }

}

std::string HttpResponse::getOriginalFilename(const std::string& body) {
    std::string filename;
    size_t filenameStart = body.find("filename=\"");
    
    if (filenameStart != std::string::npos) {
        filenameStart += 10; // length of 'filename="'
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

    // std::string fullPath = dir + originalName;
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
    // 1. Check if request has Content-Type header

    struct stat st;
    std::string dir = "upload/";
    
    // Проверка существования директории
    if (stat(dir.c_str(), &st) != 0) {
        // Создаем если не существует
        if (mkdir(dir.c_str(), 0777) != 0) {
            setErrorResponse(500, "Cannot create directory");
            return;
        }
    }
    
    // Проверка прав на запись
    if (access(dir.c_str(), W_OK) != 0) {
        setErrorResponse(403, "Forbidden: Cannot write to directory");
        return ;
    }
    std::string contentType = _request.getHeader("Content-Type");
    if (contentType.empty()) {
        setErrorResponse(400, "Bad Request: Missing Content-Type");
        return;
    }

    // 2. Parse multipart form data boundary
    size_t boundaryPos = contentType.find("boundary=");
    if (boundaryPos == std::string::npos) {
        setErrorResponse(400, "Bad Request: No boundary in multipart/form-data");
        return;
    }
    
    // std::string boundary = "--" + contentType.substr(boundaryPos + 9);
    std::string boundary;
    if (boundaryPos != std::string::npos) {
        boundary.append("--");
        boundary.append(contentType.substr(boundaryPos + 9));
    }

    const std::string& body = _request.getBody();
    
    std::string originalName = getOriginalFilename(body);
    if (originalName.empty()) {
        setErrorResponse(400, "Bad Request: No filename found");
        return;
    }

    // 3. Find start of file data
    size_t headerEnd = body.find("\r\n\r\n");
    if (headerEnd == std::string::npos) {
        setErrorResponse(400, "Bad Request: Invalid multipart format");
        return;
    }
    
    // 4. Get actual file content
    size_t fileStart = headerEnd + 4;
    size_t fileEnd = body.find(boundary, fileStart) - 4;
    if (fileEnd == std::string::npos) {
        setErrorResponse(400, "Bad Request: Invalid file format");
        return;
    }
    
    std::string fileContent = body.substr(fileStart, fileEnd - fileStart);

    // 5. Save file
   // std::string fileName = dir + intToString(time(NULL)) + ".png";
    
   std::string finalPath = createUniqueFilename(dir, originalName);
//    std::ofstream outFile(fileName.c_str(), std::ios::binary);
    std::ofstream outFile;
    outFile.open(finalPath.c_str(), std::ios::binary);
    if (!outFile.is_open()) {
        setErrorResponse(500, "Internal Server Error: Cannot create file");
        return;
    }

    outFile.write(fileContent.c_str(), fileContent.length());
    outFile.close();

    // 6. Set success response
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
            // If path has an extension (e.g. /about.html)
            std::string extension = _path.substr(dotPos);
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                // localPath = "assets/html" + _path;
                localPath = "assets/html";
                localPath.append(_path);
            } else {
                localPath = "assets/html";
                localPath.append(_path);
                localPath.append(".html");
            }
        } else {
            // No extension found, add .html
            localPath = "assets/html";
            localPath.append(_path);
            localPath.append(".html");
        }
    }
    std::cout << "Path: " << _path << std::endl;           
    if (stat(localPath.c_str(), &st) != 0) {
        setErrorResponse(404, "Not Found");
        return false;
    }
    if (access(localPath.c_str(), R_OK) != 0) {
        setErrorResponse(403, "Forbidden");
        return false;
    }
    std::cout << "local Path " << localPath << std::endl;
    std::cout << "File is accessible" << std::endl;
    return true;
}

std::string HttpResponse::getCurrentTime() {
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tm);
    return std::string(buf);
}

void HttpResponse::setErrorResponse(int code, const std::string& message) {
    setStatus(code, message);
    std::stringstream ss;
    ss << "<html><body><h1>" << message << "</h1>";
    ss << "<p>Error " << code << ": " << message << "</p></body></html>";
    setHeader("Content-Type", "text/html");
    setBody(ss.str());
}

void HttpResponse::setRedirectResponse(const std::string& newLocation) {
    setStatus(301, "Moved Permanently");
    setHeader("Location", newLocation);
    std::string redirectPage = "<html><body><h1>301 Moved Permanently</h1>";
    redirectPage += "<p>The document has moved <a href=\"" + newLocation + "\">here</a>.</p></body></html>";
    setHeader("Content-Type", "text/html");
    setBody(redirectPage);
}

void HttpResponse::sendFile() {
    // std::ifstream file(filePath.c_str(), std::ios::binary);
    std::string localPath;
    if (_path == "/")
        localPath = "assets/html/index.html";
    else {
        size_t dotPos = _path.find_last_of('.');
        if (dotPos != std::string::npos) {
            // If path has an extension (e.g. /about.html)
            std::string extension = _path.substr(dotPos);
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
                // localPath = "assets/html" + _path;
                localPath = "assets/html";
                localPath.append(_path);
            }
            else if(extension == ".py") {
                localPath = "assets/html";
                localPath.append(_path);}
              else {
                // localPath = "assets/html" + _path + ".html";
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
        setErrorResponse(404, "Not Found");
        return;
    }
    // Check file size
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    if (fileSize > _server.getMaxBodySize()) {
        setErrorResponse(413, "Content Too Large");
        return;
    }
    // Read and set file content
    try {
        std::stringstream buffer;
        buffer << file.rdbuf();
        setBody(buffer.str());

        // Set content type based on file extension
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
        setErrorResponse(500, "Internal Server Error");
    }
}

// std::string HttpResponse::toString() const {
//     std::stringstream response;
    
//     response << _httpVersion << " " << _statusCode << " " << _statusMessage << "\r\n";
    
//     std::map<std::string, std::string>::const_iterator it;
//     for (it = _headers.begin(); it != _headers.end(); ++it) {
//         response << it->first << ": " << it->second << "\r\n";
//     }
    
//     response << "\r\n" << _body;
//     return response.str();
// }

// std::string HttpResponse::toString() const {
//     std::stringstream response;
    
//     // Status line
//     response << _httpVersion << " " << _statusCode << " " << _statusMessage << "\r\n";
    
//     // Headers
//     for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
//          it != _headers.end(); ++it) {
//         response << it->first << ": " << it->second << "\r\n";
//     }
    
//     // Cookies
//     for (std::vector<std::string>::const_iterator it = _cookies.begin();
//          it != _cookies.end(); ++it) {
//         response << "Set-Cookie: " << *it << "\r\n";
//     }
    
//     // Empty line separating headers from body
//     response << "\r\n";
    
//     // Body
//     response << _body;
    
//     return response.str();
// }

std::string HttpResponse::toString() const {
    std::stringstream response;
    
    // Status line
    response << _httpVersion << " " << _statusCode << " " << _statusMessage << "\r\n";
    
    // Headers
    std::map<std::string, std::string>::const_iterator it;
    for (it = _headers.begin(); it != _headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    
    // Cookies - each cookie gets its own Set-Cookie header
    std::vector<std::string>::const_iterator cookieIt;
    for (cookieIt = _cookies.begin(); cookieIt != _cookies.end(); ++cookieIt) {
        response << "Set-Cookie: " << *cookieIt << "\r\n";
    }
    
    // Empty line separating headers from body
    response << "\r\n";
    
    // Body
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

