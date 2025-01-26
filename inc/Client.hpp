#pragma once

#include "Webserv.hpp"

struct Client
{
    private://change to private
        int _sockfd; //дескриптор сокета
        sockaddr_in _adress; // для хранения адреса(IP, порт)
        time_t _lst_msg;// время последнего сообщения
    public:
        Client(ServerConfig &serv);
        ~Client();
        time_t getLstMsg();
        void updateTime();
};