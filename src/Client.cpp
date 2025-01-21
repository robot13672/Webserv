#include "../inc/Client.hpp"

Client::Client(int sockfd, sockaddr_in adress)
{
    _sockfd = sockfd;
    _adress = adress;
    _ip = inet_ntoa(adress.sin_addr);
    _port = ntohs(adress.sin_port);  
}