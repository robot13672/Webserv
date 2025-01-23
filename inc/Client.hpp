#pragma once

#include "Webserv.hpp"

struct Client
{
    private://change to private
        int _sockfd; //дескриптор сокета
        sockaddr_in _adress; // для хранения адреса(IP, порт)
        time_t _lst_msg;// время последнего сообщения
    public:
        Client(int sockfd, sockaddr_in adress);
        ~Client();
        time_t getLstMsg();
        void updateTime();
};