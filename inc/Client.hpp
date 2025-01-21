#pragma once

#include "Webserv.hpp"

struct Client
{
    public://change to private
        int _sockfd; //дескриптор сокета
        sockaddr_in _adress; // для хранения адреса(IP, порт)
        std::string _ip;
        unsigned short _port;// порт для более быстрого обращения
    public:
        Client(int sockfd, sockaddr_in adress);
};