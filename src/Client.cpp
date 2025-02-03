#include "../inc/Client.hpp"

Client::Client()
{
    this->_lst_msg = time(NULL);
}

Client::Client(ServerConfig &serv)
{
    _response.setServer(serv);
    _request.setMaxBodySize(serv.getMaxBodySize());
    _lst_msg = time(NULL);
    _sockfd = -1; // Initialize socket file descriptor
}

Client::~Client() {}

void Client::setSocket(int socket)
{
    _sockfd = socket;
}

time_t Client::getLstMsg()
{
    return _lst_msg;
}

HttpResponse Client::getResponse()
{
    return _response;
}

void Client::updateTime()
{
    _lst_msg = time(NULL);
}