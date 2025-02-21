#include "../inc/CGI.hpp"
#include "../inc/HttpRequest.hpp"


void CGI::SetEnv(HttpRequest &request)
{
    std::map<std::string, std::string> env;

    env["SERVER_SOFTWARE"] = "Webserv/1.0";
    env["REQUEST_METHOD"] = request.getMethod();
    env["SERVER_NAME"] = request.getHeader("IP");
    request.getHeader("IP");
    env["SERVER_PROTOCOL"] = request.getHttpVersion();
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SCRIPT_NAME"] = request.getPath(); 
    env["QUERY_STRING"] = request.getQuery();
    env["CONTENT_TYPE"] = request.getHeader("Content-Type");
    env["CONTENT_LENGTH"] = request.getHeader("Content-Length");

    //TODO: convert all headers to CGI variables
}