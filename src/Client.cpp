#include "../inc/Client.hpp"

Client::Client(int sockfd, sockaddr_in adress)
{
    _sockfd = sockfd;
    _adress = adress;
    _lst_msg = time(NULL);
}

Client::~Client() {}

time_t Client::getLstMsg()
{
    return _lst_msg;
}

void Client::updateTime()
{
    _lst_msg = time(NULL);
}