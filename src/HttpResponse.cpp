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
    _method(""){}

HttpResponse::HttpResponse(const HttpResponse& copy) :
    _server(copy._server),
    _httpVersion(copy._httpVersion),
    _statusCode(copy._statusCode),
    _statusMessage(copy._statusMessage),
    _headers(copy._headers),
    _body(copy._body),
    _chunked(copy._chunked),
    _method(copy._method),
    _path(copy._path) {}

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
    }
    return *this;
}

HttpResponse::~HttpResponse() {
    _headers.clear();
    _body.clear();
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

// void HttpResponse::handleRequest(const std::string& path, const std::string& method) {
//     // Check if method is allowed
//     if (method != "GET" && method != "POST" && method != "DELETE") {
//         setErrorResponse(405, "Method Not Allowed");
//         setHeader("Allow", "GET, POST, DELETE");
//         return;
//     }
//     // Check if path exists
//     std::ifstream file(path.c_str());
//     if (!file) {
//         setErrorResponse(404, "Not Found");
//         return;
//     }
//     // Check permissions
//     if (access(path.c_str(), R_OK) != 0) {
//         setErrorResponse(403, "Forbidden");
//         return;
//     }
//     // Handle different status codes
//     switch (_statusCode) {
//         case 200:
//             sendFile(path);
//             break;
//         case 301:
//             setRedirectResponse(path);
//             break;
//         case 400:
//             setErrorResponse(400, "Bad Request");
//             break;
//         case 403:
//             setErrorResponse(403, "Forbidden");
//             break;
//         case 404:
//             setErrorResponse(404, "Not Found");
//             break;
//         case 405:
//             setErrorResponse(405, "Method Not Allowed");
//             break;
//         case 413:
//             setErrorResponse(413, "Content Too Large");
//             break;
//         case 500:
//             setErrorResponse(500, "Internal Server Error");
//             break;
//         case 501:
//             setErrorResponse(501, "Not Implemented");
//             break;
//         default:
//             setErrorResponse(500, "Internal Server Error");
//     }
// }

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
    
    // Проверяем метод до проверки файла
    if (_method != "GET" && _method != "POST" && _method != "DELETE") {
        setErrorResponse(405, "Method Not Allowed");
        setHeader("Allow", "GET, POST, DELETE");
        return;
    }
    // Для DELETE метода
    if (_method == "DELETE") {
        handleDelete(_path);
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

void HttpResponse::handleDelete(const std::string& path) {
    if (unlink(path.c_str()) == 0) {
        setStatus(200, "OK");
        setHeader("Content-Type", "text/plain");
        setBody("File successfully deleted");
    } else {
        if (errno == EACCES) {
            setErrorResponse(403, "Forbidden");
        } else {
            setErrorResponse(500, "Internal Server Error");
        }
    }
}

void HttpResponse::handlePost() {
    // Проверяем существование директории
    if (_request.getBody().empty()) {
        setErrorResponse(500, "Internal Server Error: No request set");
        _response = toString();
        return;
    }
    
    std::string dir = "upload/"; // Добавляем слеш, чтобы проверить директорию
    struct stat st;
    std::cout << "Dir: " << dir << std::endl;
    if (stat(dir.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) {
        setErrorResponse(404, "Directory Not Found");
        return;
    }
    // Проверяем права на запись
    if (access(dir.c_str(), W_OK) != 0) {
        setErrorResponse(403, "Forbidden");
        return;
    }
    setStatus(201, "Created");
    setHeader("Content-Type", "text/plain");
    setBody("File successfully processed");
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
                localPath = "assets/html" + _path;
            } else {
                localPath = "assets/html" + _path + ".html";
            }
        } else {
            // No extension found, add .html
            localPath = "assets/html" + _path + ".html";
        }
    }
    // else if (path.find_last_of('.'))
    //     localPath = "assets/html/" + path;
    // else 
    //     localPath = "assets/html" + path + ".html";

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
                localPath = "assets/html" + _path;
            } else {
                localPath = "assets/html" + _path + ".html";
            }
        } else {
            // No extension found, add .html
            localPath = "assets/html" + _path + ".html";
        }
    }
    // else if (filePath.find_last_of('.'))
    //     localPath = "assets/html/" + filePath;
    // else 
    //     localPath = "assets/html" + filePath + ".html";
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

std::string HttpResponse::toString() const {
    std::ostringstream response;
    
    response << _httpVersion << " " << _statusCode << " " << _statusMessage << "\r\n";
    
    std::map<std::string, std::string>::const_iterator it;
    for (it = _headers.begin(); it != _headers.end(); ++it) {
        response << it->first << ": " << it->second << "\r\n";
    }
    
    response << "\r\n" << _body;
    return response.str();
}

void HttpResponse::clear() {
    _statusCode = 200;
    _statusMessage = "OK";
    _headers.clear();
    _body.clear();
    _chunked = false;
}
