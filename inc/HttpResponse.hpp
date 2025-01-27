#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#pragma once

#include "Webserv.hpp"

class HttpResponse
{
    private:
        ServerConfig _server;

    public:
        void setServer(ServerConfig &serv);


};

#endif