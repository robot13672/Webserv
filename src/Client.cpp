#include "../inc/Client.hpp"

Client::Client(ServerConfig &serv)
{
    //new logic for Client
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