#pragma once

#include "Webserv.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"


class Client
{
    private:
        int         _sockfd; //дескриптор сокета
        sockaddr_in _adress; // для хранения адреса(IP, порт)
        time_t      _lst_msg;// время последнего сообщения
    public:
        Client();
        Client(ServerConfig &serv);
        ~Client();

        ServerConfig _server;
        HttpRequest  _request;
        HttpResponse _response;
        
        //set
        void setSocket(int socket);

        //get
        time_t getLstMsg();
        void updateTime();

};