#include "../inc/Client.hpp"

// Client::Client(ServerConfig &serv)
// {
//     _lst_msg = time(NULL);//присваиваю текущее время
// }

Client::Client(ServerConfig &serv)
{
    _response.setServer(serv);
    _request.setMaxBodySize(serv.getMaxBodySize());
    _lst_msg = time(NULL);
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

void Client::updateTime()
{
    _lst_msg = time(NULL);
}