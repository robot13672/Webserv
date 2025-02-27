#include "../inc/CGI.hpp"
#include "../inc/HttpRequest.hpp"
#include <string>
#include <iostream>
#include <algorithm>

void CGI::SetEnv(HttpRequest &request)
{
    // Clear any existing _cgiEnvironment variables
    _cgiEnv.clear();

    // Standard CGI _cgiEnvironment variables
    _cgiEnv["SERVER_SOFTWARE"]    = "Webserv/1.0";
    _cgiEnv["SERVER_NAME"]        = request.getHeader("IP");
    _cgiEnv["SERVER_PORT"]        = request.getHeader("PORT");
    _cgiEnv["SERVER_PROTOCOL"]    = request.getHttpVersion();
    _cgiEnv["GATEWAY_INTERFACE"]  = "CGI/1.1";

    // Request-specific variables
    _cgiEnv["REQUEST_METHOD"]     = request.getMethod();
    _cgiEnv["REQUEST_URI"]        = request.getUri();
    _cgiEnv["SCRIPT_NAME"]        = request.getPath();
    _cgiEnv["QUERY_STRING"]       = request.getQuery();
    _cgiEnv["PATH_INFO"]          = request.getPath();
    _cgiEnv["PATH_TRANSLATED"]    = std::string(getcwd(NULL, 0)) + request.getPath();

    // Headers
    _cgiEnv["CONTENT_TYPE"]       = request.getHeader("Content-Type");
    _cgiEnv["CONTENT_LENGTH"]     = request.getHeader("Content-Length");
    
    // Convert HTTP headers to CGI variables
    // std::map<std::string, std::string> headers = request.getHeaders();
    // std::map<std::string, std::string>::const_iterator it;
    // for (it = headers.begin(); it != headers.end(); ++it) 
    // {
    //     std::string header_name = it->first;
        
    //     // Convert header name to CGI format (HTTP_*)
    //     for (std::string::iterator str_it = header_name.begin(); 
    //          str_it != header_name.end(); ++str_it)
    //     {
    //         *str_it = toupper(*str_it);
    //     }
    //     header_name = "HTTP_" + header_name;
        
    //     // Replace '-' with '_' in header name
    //     for (std::string::iterator str_it = header_name.begin(); 
    //          str_it != header_name.end(); ++str_it)
    //     {
    //         if (*str_it == '-')
    //             *str_it = '_';
    //     }
        
    //     _cgiEnv[header_name] = it->second;
    // }

    // // Remote information
    // _cgiEnv["REMOTE_ADDR"] = request.getHeader("IP");
    // _cgiEnv["REMOTE_HOST"] = request.getHeader("Host");

    // // Remove any empty variables
    // std::map<std::string, std::string>::iterator map_it = _cgiEnv.begin();
    // while (map_it != _cgiEnv.end()) 
    // {
    //     if (map_it->second.empty())
    //         _cgiEnv.erase(map_it++);
    //     else
    //         ++map_it;
    // }

    for (std::map<std::string, std::string>::const_iterator it = _cgiEnv.begin(); it != _cgiEnv.end(); ++it) {
        setenv(it->first.c_str(), it->second.c_str(), 1);
    }
}

// void CGI::executeScript() {
//     // Устанавливаем все переменные окружения
//     for (std::map<std::string, std::string>::const_iterator it = _cgiEnv.begin();
//          it != _cgiEnv.end(); ++it) {
//         setenv(it->first.c_str(), it->second.c_str(), 1);
//     }

//     // Создаем дочерний процесс для выполнения скрипта
//     pid_t pid = fork();
//     if (pid == 0) {
//         // Дочерний процесс
//         char *args[] = {(char*)_scriptPath.c_str(), NULL};
//         execve(_scriptPath.c_str(), args, environ);
//         exit(1);
//     }
//     // Родительский процесс ждет завершения дочернего
//     int status;
//     waitpid(pid, &status, 0);
// }

CGI::CGI() : _isRunning(false), _exitStatus(0) {}

CGI::~CGI() {}

// bool CGI::handleCgiRequest(HttpRequest& request, HttpResponse& response) {
//     if (!isCgiRequest(request.getPath()))
//         return false;

//     setScriptPath(request.getPath());
//     setRequestBody(request.getBody());
//     SetEnv(request);

//     std::string cgiOutput = executeCgiScript();
//     parseResponse(cgiOutput, response);
//     return true;
// }
bool CGI::handleCgiRequest(HttpRequest& request, HttpResponse& response) {
    if (!isCgiRequest(request.getPath()))
        return false;

    // Fix the path - remove one /cgi-bin/ since it's already in the path
    std::string path = request.getPath();
    if (path[0] == '/') {
        path = path.substr(1); // Remove leading slash if present
    }
    setScriptPath(path);  // Add . to make it relative to current directory
    
    // std::cout << "DEBUG: Script path: " << _scriptPath << std::endl;
    
    setRequestBody(request.getBody());
    SetEnv(request);

    std::string cgiOutput = executeCgiScript();
    // std::cout << "DEBUG: CGI output length: " << cgiOutput.length() << std::endl;
    
    parseResponse(cgiOutput, response);
    return true;
}

bool CGI::isCgiRequest(const std::string& path) {
    return (path.find("/cgi-bin/") == 0 || 
            path.find(".py") != std::string::npos);
}

void CGI::setScriptPath(const std::string& path) {
    _scriptPath = path;
}

std::string CGI::executeCgiScript() {
    // std::cout << "DEBUG: Starting CGI execution with script: " << _scriptPath << std::endl;
    
    int input[2], output[2];
    if (pipe(input) < 0 || pipe(output) < 0) {
        // std::cerr << "ERROR: Failed to create pipes" << std::endl;
        return "";
    }

    pid_t pid = fork();
    if (pid < 0) {
        // std::cerr << "ERROR: Fork failed" << std::endl;
        return "";
    }

    if (pid == 0) {
        // Child process
        close(input[1]);
        close(output[0]);
        dup2(input[0], STDIN_FILENO);
        dup2(output[1], STDOUT_FILENO);

        // Use python3 interpreter explicitly
        char *args[] = {(char*)"/usr/bin/python3", (char*)_scriptPath.c_str(), NULL};
        // std::cout << "DEBUG: Executing python script: " << _scriptPath << std::endl;
        execv("/usr/bin/python3", args);
        std::cerr << "ERROR: execv failed: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Rest of the code remains the same...
    close(input[0]);
    close(output[1]);

    std::string response;
    char buffer[4096];
    ssize_t bytes;
    
    while ((bytes = read(output[0], buffer, sizeof(buffer))) > 0) {
        response.append(buffer, bytes);
    }
    
    close(output[0]);
    close(input[1]);

    int status;
    waitpid(pid, &status, 0);
    _exitStatus = WEXITSTATUS(status);
    
    // std::cout << "DEBUG: CGI script exit status: " << _exitStatus << std::endl;
    // std::cout << "DEBUG: Response length: " << response.length() << std::endl;

    return response;
}

void CGI::parseResponse(const std::string& cgiOutput, HttpResponse& response) {
    // std::cout << "DEBUG: Raw CGI output length: " << cgiOutput.length() << std::endl;
    // std::cout << "DEBUG: Raw CGI output: \n" << cgiOutput << std::endl;

    // Set default headers first
    response.setStatus(200, "OK");
    response.setHeader("Content-Type", "text/html");
    
    // Find the header-body separator
    size_t headerEnd = cgiOutput.find("\r\n\r\n");
    if (headerEnd == std::string::npos) {
        // No header separator found, treat everything as body
        // std::cout << "DEBUG: No headers found, using entire output as body" << std::endl;
        response.setBody(cgiOutput);
        return;
    }

    // Parse headers
    std::string headers = cgiOutput.substr(0, headerEnd);
    std::string body = cgiOutput.substr(headerEnd + 4); // Skip \r\n\r\n

    // std::cout << "DEBUG: Found headers:\n" << headers << std::endl;
    // std::cout << "DEBUG: Found body:\n" << body << std::endl;

    // Parse individual headers
    std::istringstream headerStream(headers);
    std::string line;
    while (std::getline(headerStream, line)) {
        // Remove carriage return if present
        if (!line.empty() && line[line.length()-1] == '\r') {
            line = line.substr(0, line.length()-1);
        }
        
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            // std::cout << "DEBUG: Setting header: " << key << " = " << value << std::endl;
            response.setHeader(key, value);
        }
    }

    // Set body
    response.setBody(body);
}

// void CGI::parseResponse(const std::string& cgiOutput, HttpResponse& response) {
//     size_t headerEnd = cgiOutput.find("\r\n\r\n");
//     if (headerEnd == std::string::npos) {
//         // No headers found, treat entire output as body
//         response.setHeader("Content-Type", "text/html");
//         response.setBody(cgiOutput);
//         response.setStatus(200, "OK");
//         return;
//     }

//     // Parse headers and body
//     std::string headers = cgiOutput.substr(0, headerEnd);
//     std::string body = cgiOutput.substr(headerEnd + 4);

    
//     std::istringstream headerStream(headers);
//     std::string line;
//     while (std::getline(headerStream, line) && !line.empty()) {
//         size_t colonPos = line.find(": ");
//         if (colonPos != std::string::npos) {
//             std::string key = line.substr(0, colonPos);
//             std::string value = line.substr(colonPos + 2);
//             response.setHeader(key, value);
//         }
//     }
    
//     std::cout << "DEBUG: CGI raw output: " << cgiOutput << std::endl;
//     std::cout << "DEBUG: Header end position: " << headerEnd << std::endl;
//     response.setBody(body);
//     response.setStatus(200, "OK");
// }

// void CGI::parseResponse(const std::string& cgiOutput, HttpResponse& response) {
//     size_t headerEnd = cgiOutput.find("\r\n\r\n");
    
//     std::cout << "DEBUG: CGI raw output: " << cgiOutput << std::endl;
//     std::cout << "DEBUG: Header end position: " << headerEnd << std::endl;
    
//     response.setStatus(200, "OK");
//     response.setHeader("Content-Type", "text/html");
//     response.setBody(cgiOutput);
// }