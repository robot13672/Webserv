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
        ~Client();
        ServerConfig _server;// сервер к конфигурации которого подключен
        HttpRequest  _request;// запрос
        HttpResponse _response;// ответ
        Client();
        Client(ServerConfig &serv);
        Client(Client const &cop);
        Client operator=(Client const &cop);
        
        //set
        void setSocket(int socket);

        //get
        time_t getLstMsg();
        void updateTime();
};