#include "../inc/CGI.hpp"
#include "../inc/HttpRequest.hpp"
#include <string>
#include <iostream>
#include <algorithm>

CGI::CGI() :  _exitStatus(0) {}

CGI::~CGI() {}

// bool CGI::handleCgiRequest(HttpRequest& request, HttpResponse& response) {
//     if (!isCgiRequest(request.getPath()))
//         return false;

//     // Fix the path - remove one /cgi-bin/ since it's already in the path
//     std::string path = request.getPath();
//     if (path[0] == '/') {
//         path = path.substr(1); // Remove leading slash if present
//     }
//     setScriptPath(path);  // Add . to make it relative to current directory
    
//     // std::cout << "DEBUG: Script path: " << _scriptPath << std::endl;
    
//     setRequestBody(request.getBody());
//     SetEnv(request);

//     std::string cgiOutput = executeCgiScript();
//     // std::cout << "DEBUG: CGI output length: " << cgiOutput.length() << std::endl;
    
//     parseResponse(cgiOutput, response);
//     return true;
// }

bool CGI::handleCgiRequest(HttpRequest& request, HttpResponse& response) {
    if (!isCgiRequest(request.getPath()))
        return false;
    std::string path = request.getPath();
    if (path[0] == '/') {
        path = path.substr(1); // Remove leading slash if present
    }
    setScriptPath(path);
    SetEnv(request);
    setRequestBody(request.getBody());

    std::string cgiOutput = executeCgiScript();
    if (cgiOutput.empty()) {
        response.setErrorResponse(500, "CGI Script Error");
        return true;
    }

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

void CGI::SetEnv(HttpRequest &request) {
    _cgiEnv["REQUEST_METHOD"] = request.getMethod();
    _cgiEnv["CONTENT_TYPE"] = request.getHeader("Content-Type");
    _cgiEnv["CONTENT_LENGTH"] = request.getHeader("Content-Length");
    _cgiEnv["SCRIPT_NAME"] = _scriptPath;
    _cgiEnv["PATH_INFO"] = request.getPath();
    _cgiEnv["QUERY_STRING"] = request.getQuery();
    _cgiEnv["SERVER_PROTOCOL"] = request.getHttpVersion();
    _cgiEnv["REDIRECT_STATUS"] = "200";
    _cgiEnv["SERVER_SOFTWARE"] = "Webserv/1.0";
    
    // Add additional environment variables
    std::map<std::string, std::string> headers = request.getHeaders();
    for (std::map<std::string, std::string>::iterator it = headers.begin(); 
         it != headers.end(); ++it) {
        std::string name = "HTTP_" + it->first;
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        std::replace(name.begin(), name.end(), '-', '_');
        _cgiEnv[name] = it->second;
    }
}

std::string CGI::executeCgiScript() {
    int input[2], output[2];
    std::string response;

    if (pipe(input) < 0 || pipe(output) < 0) {
        return "Status: 500\r\nContent-Type: text/plain\r\n\r\nInternal Server Error";
    }

    pid_t pid = fork();
    if (pid < 0) {
        return "Status: 500\r\nContent-Type: text/plain\r\n\r\nFork Failed";
    }

    if (pid == 0) {
        // Child process
        close(input[1]);
        close(output[0]);
        dup2(input[0], STDIN_FILENO);
        dup2(output[1], STDOUT_FILENO);
        close(input[0]);
        close(output[1]);

        // Set environment variables
        for (std::map<std::string, std::string>::const_iterator it = _cgiEnv.begin();
             it != _cgiEnv.end(); ++it) {
            setenv(it->first.c_str(), it->second.c_str(), 1);
        }

        // Execute script
        char *args[] = {(char*)"/usr/bin/python3", (char*)_scriptPath.c_str(), NULL};
        execv("/usr/bin/python3", args);
        exit(1);
    }

    // Parent process
    close(input[0]);
    close(output[1]);

    // Send POST data to script if exists
    if (!_requestBody.empty()) {
        write(input[1], _requestBody.c_str(), _requestBody.length());
    }
    close(input[1]);

    // Read response
    char buffer[4096];
    ssize_t bytes;
    while ((bytes = read(output[0], buffer, sizeof(buffer))) > 0) {
        response.append(buffer, bytes);
    }

    close(output[0]);

    // Wait for child process
    int status;
    waitpid(pid, &status, 0);

    return response;
}

// void CGI::parseResponse(const std::string& cgiOutput, HttpResponse& response) {
//     size_t headerEnd = cgiOutput.find("\r\n\r\n");
//     if (headerEnd == std::string::npos) {
//         response.setHeader("Content-Type", "text/html");
//         response.setBody(cgiOutput);
//         response.setStatus(200, "OK");
//         return;
//     }

//     std::string headers = cgiOutput.substr(0, headerEnd);
//     std::string body = cgiOutput.substr(headerEnd + 4);

//     std::istringstream headerStream(headers);
//     std::string line;
//     bool hasContentType = false;
//     bool hasCookie = false;

//     while (std::getline(headerStream, line)) {
//         if (!line.empty() && line[line.length()-1] == '\r') {
//             line = line.substr(0, line.length()-1);
//         }
        
//         if (line.empty()) continue;

//         // // Handle Set-Cookie headers
//         // if (line.find("Set-Cookie:") == 0) {
//         //     std::string cookieValue = line.substr(11);
//         //     cookieValue.erase(0, cookieValue.find_first_not_of(" "));
//         //     response.addCookie(cookieValue);
//         //     continue;
//         // }

//         // if (line.find("Set-Cookie:") == 0) {
//         //     std::string cookieValue = line.substr(11);
//         //     // Remove leading whitespace
//         //     while (!cookieValue.empty() && cookieValue[0] == ' ') {
//         //         cookieValue = cookieValue.substr(1);
//         //     }
//         //     response.setHeader("Set-Cookie", cookieValue);
//         //     continue;
//         // }

//         // size_t colonPos = line.find(": ");
//         // if (colonPos != std::string::npos) {
//         //     std::string key = line.substr(0, colonPos);
//         //     std::string value = line.substr(colonPos + 2);
//         //     response.setHeader(key, value);
//         //     if (key == "Content-Type") hasContentType = true;
//         // }

//         if (line.find("Set-Cookie:") == 0) {
//             if (!hasCookie) { // Only process the first Set-Cookie header
//                 std::string cookieValue = line.substr(11);
//                 while (!cookieValue.empty() && cookieValue[0] == ' ') {
//                     cookieValue = cookieValue.substr(1);
//                 }
//                 response.setHeader("Set-Cookie", cookieValue);
//                 hasCookie = true;
//             }
//             continue;
//         }

//         // Only process Content-Type if we haven't seen it yet
//         size_t colonPos = line.find(": ");
//         if (colonPos != std::string::npos) {
//             std::string key = line.substr(0, colonPos);
//             std::string value = line.substr(colonPos + 2);
//             if (key == "Content-Type" && !hasContentType) {
//                 response.setHeader(key, value);
//                 hasContentType = true;
//             } else if (key != "Content-Type") {
//                 response.setHeader(key, value);
//             }
//         }
//     }

//     if (!hasContentType) {
//         response.setHeader("Content-Type", "text/html");
//     }

//     response.setBody(body);
//     response.setStatus(200, "OK");
// }

void CGI::parseResponse(const std::string& cgiOutput, HttpResponse& response) {
    // Check if the output starts with headers
    if (cgiOutput.empty()) {
        response.setErrorResponse(500, "Empty CGI response");
        return;
    }

    // For plain text responses without headers, just set content type and body
    if (cgiOutput.find(": ") == std::string::npos) {
        response.setHeader("Content-Type", "text/plain");
        response.setBody(cgiOutput);
        response.setStatus(200, "OK");
        return;
    }

    size_t headerEnd = cgiOutput.find("\r\n\r\n");
    if (headerEnd == std::string::npos) {
        headerEnd = cgiOutput.find("\n\n");
    }

    // If no header separator found, treat as plain text
    if (headerEnd == std::string::npos) {
        response.setHeader("Content-Type", "text/plain");
        response.setBody(cgiOutput);
        response.setStatus(200, "OK");
        return;
    }

    // Parse headers and body
    std::string headers = cgiOutput.substr(0, headerEnd);
    std::string body = cgiOutput.substr(headerEnd + (cgiOutput[headerEnd + 1] == '\n' ? 2 : 4));

    std::istringstream headerStream(headers);
    std::string line;
    bool hasContentType = false;

    while (std::getline(headerStream, line)) {
        if (!line.empty() && line[line.length()-1] == '\r') {
            line = line.substr(0, line.length()-1);
        }
        
        if (line.empty()) continue;

        // Handle Set-Cookie headers
        if (line.find("Set-Cookie:") == 0) {
            std::string cookieValue = line.substr(11);
            while (!cookieValue.empty() && (cookieValue[0] == ' ' || cookieValue[0] == '\t')) {
                cookieValue = cookieValue.substr(1);
            }
            response.setHeader("Set-Cookie", cookieValue);
            continue;
        }

        size_t colonPos = line.find(": ");
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            response.setHeader(key, value);
            if (key == "Content-Type") hasContentType = true;
        }
    }

    if (!hasContentType) {
        response.setHeader("Content-Type", "text/plain");
    }

    response.setBody(body);
    response.setStatus(200, "OK");
}


// void CGI::parseResponse(const std::string& cgiOutput, HttpResponse& response) {
//     size_t headerEnd = cgiOutput.find("\r\n\r\n");
//     if (headerEnd == std::string::npos) {
//         headerEnd = cgiOutput.find("\n\n");
//         if (headerEnd == std::string::npos) {
//             response.setErrorResponse(500, "Invalid CGI response format");
//             return;
//         }
//     }

//     std::string headers = cgiOutput.substr(0, headerEnd);
//     std::string body = cgiOutput.substr(headerEnd + 4);

//     std::istringstream headerStream(headers);
//     std::string line;
//     bool hasContentType = false;

//     while (std::getline(headerStream, line)) {
//         if (!line.empty() && line[line.length()-1] == '\r') {
//             line = line.substr(0, line.length()-1);
//         }
        
//         if (line.empty()) continue;

//         // if (line.find("Set-Cookie:") == 0) {
//         //     response.setHeader("Set-Cookie", line.substr(11));
//         //     continue;
//         // }

//         if (line.find("Set-Cookie:") == 0) {
//             std::string cookieValue = line.substr(11);
//             while (!cookieValue.empty() && (cookieValue[0] == ' ' || cookieValue[0] == '\t')) {
//                 cookieValue = cookieValue.substr(1);
//             }
//             response.setHeader("Set-Cookie", cookieValue);
//             continue;
//         }

//         size_t colonPos = line.find(": ");
//         if (colonPos != std::string::npos) {
//             std::string key = line.substr(0, colonPos);
//             std::string value = line.substr(colonPos + 2);
//             response.setHeader(key, value);
//             if (key == "Content-Type") hasContentType = true;
//         }
//     }

//     if (!hasContentType) {
//         response.setHeader("Content-Type", "text/html");
//     }

//     response.setBody(body);
//     response.setStatus(200, "OK");
// }

// void CGI::parseResponse(const std::string& cgiOutput, HttpResponse& response) {
//     size_t headerEnd = cgiOutput.find("\r\n\r\n");
//     if (headerEnd == std::string::npos) {
//         response.setHeader("Content-Type", "text/html");
//         response.setBody(cgiOutput);
//         response.setStatus(200, "OK");
//         return;
//     }

//     std::string headers = cgiOutput.substr(0, headerEnd);
//     std::string body = cgiOutput.substr(headerEnd + 4);

//     std::istringstream headerStream(headers);
//     std::string line;
//     bool hasContentType = false;

//     while (std::getline(headerStream, line)) {
//         if (!line.empty() && line[line.length()-1] == '\r') {
//             line = line.substr(0, line.length()-1);
//         }
        
//         if (line.empty()) continue;

//         // Handle Set-Cookie headers specially
//         if (line.find("Set-Cookie:") == 0) {
//             std::string cookieValue = line.substr(12);
//             // Trim leading whitespace
//             cookieValue.erase(0, cookieValue.find_first_not_of(" "));
//             response.addCookie(cookieValue);
//             continue;
//         }

//         size_t colonPos = line.find(": ");
//         if (colonPos != std::string::npos) {
//             std::string key = line.substr(0, colonPos);
//             std::string value = line.substr(colonPos + 2);
//             response.setHeader(key, value);
//             if (key == "Content-Type") hasContentType = true;
//         }
//     }

//     if (!hasContentType) {
//         response.setHeader("Content-Type", "text/html");
//     }

//     response.setBody(body);
//     response.setStatus(200, "OK");
// }


//2 version
// void CGI::parseResponse(const std::string& cgiOutput, HttpResponse& response) {
//     // std::cout << "DEBUG: Raw CGI output length: " << cgiOutput.length() << std::endl;
//     // std::cout << "DEBUG: Raw CGI output: \n" << cgiOutput << std::endl;

//     // Set default headers first
//     response.setStatus(200, "OK");
//     response.setHeader("Content-Type", "text/html");
    
//     // Find the header-body separator
//     size_t headerEnd = cgiOutput.find("\r\n\r\n");
//     if (headerEnd == std::string::npos) {
//         // No header separator found, treat everything as body
//         // std::cout << "DEBUG: No headers found, using entire output as body" << std::endl;
//         response.setBody(cgiOutput);
//         return;
//     }

//     // Parse headers
//     std::string headers = cgiOutput.substr(0, headerEnd);
//     std::string body = cgiOutput.substr(headerEnd + 4); // Skip \r\n\r\n

//     // std::cout << "DEBUG: Found headers:\n" << headers << std::endl;
//     // std::cout << "DEBUG: Found body:\n" << body << std::endl;

//     // Parse individual headers
//     std::istringstream headerStream(headers);
//     std::string line;
//     while (std::getline(headerStream, line)) {
//         // Remove carriage return if present
//         if (!line.empty() && line[line.length()-1] == '\r') {
//             line = line.substr(0, line.length()-1);
//         }
        
//         // Skip empty lines
//         if (line.empty()) {
//             continue;
//         }

//         size_t colonPos = line.find(": ");
//         if (colonPos != std::string::npos) {
//             std::string key = line.substr(0, colonPos);
//             std::string value = line.substr(colonPos + 2);
//             // std::cout << "DEBUG: Setting header: " << key << " = " << value << std::endl;
//             response.setHeader(key, value);
//         }
//     }

//     // Set body
//     response.setBody(body);
// }
//3 version
// void CGI::parseResponse(const std::string& cgiOutput, HttpResponse& response) {
//     size_t headerEnd = cgiOutput.find("\r\n\r\n");
//     if (headerEnd == std::string::npos) {
//         response.setHeader("Content-Type", "text/html");
//         response.setBody(cgiOutput);
//         response.setStatus(200, "OK");
//         return;
//     }

//     std::string headers = cgiOutput.substr(0, headerEnd);
//     std::string body = cgiOutput.substr(headerEnd + 4);

//     // Process headers
//     std::istringstream headerStream(headers);
//     std::string line;
//     while (std::getline(headerStream, line)) {
//         // Remove carriage return if present
//         if (!line.empty() && line[line.length()-1] == '\r') {
//             line = line.substr(0, line.length()-1);
//         }
        
//         // Skip empty lines
//         if (line.empty()) continue;

//         // Special handling for Set-Cookie headers
//         if (line.find("Set-Cookie:") == 0) {
//             response.setHeader("Set-Cookie", line.substr(11));
//             continue;
//         }

//         size_t colonPos = line.find(": ");
//         if (colonPos != std::string::npos) {
//             std::string key = line.substr(0, colonPos);
//             std::string value = line.substr(colonPos + 2);
//             response.setHeader(key, value);
//         }
//     }

//     response.setBody(body);
// }


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