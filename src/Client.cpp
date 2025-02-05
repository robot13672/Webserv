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

Client::Client(const Client &cop)
{
    this->_sockfd = cop._sockfd;
    this->_lst_msg = cop._lst_msg;
    this->_response = cop._response;
    this->_request = cop._request;
    this->_server = cop._server;
    this->_adress = cop._adress;
}

Client Client::operator=(const Client &cop)
{
    if (this != &cop)
    {
        this->_sockfd = cop._sockfd;
        this->_lst_msg = cop._lst_msg;
        this->_response = cop._response;
        this->_request = cop._request;
    }
    return *this;
}


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