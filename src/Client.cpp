#include "../inc/Client.hpp"

Client::Client()
{
    this->_lst_msg = time(NULL);
}

Client::Client(ServerConfig &serv)
{
    _server = serv; // Initialize _server with the provided ServerConfig object
    _lst_msg = time(NULL); // Initialize _lst_msg with the current time
    _sockfd = -1; // Initialize socket file descriptor
    _request.setMaxBodySize(serv.getMaxBodySize());
}

Client::~Client()
{
    // if (_sockfd > 0) 
    // {
    //     close(_sockfd);
    //     _sockfd = -1;
    // }
    _request.clear();
    _response.clear();
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