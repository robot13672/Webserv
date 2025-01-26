#include "../inc/Server.hpp"

void Server::setupServer(std::vector<ServerConfig> servers) 
{
    _servers = servers;
    for (std::vector<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        if(isDublicateFd(it))
            it->setFd(findDublicateFr(it));//ищу дублирующий дескриптор и подставляю его в текущий сервер
        else
            it->setupServer();
        std::cout << "Server created with host:" << it->getHost() << ", port:" << it->getPort() << std::endl; //Изменить это, и выводить через логер
    }
}

bool Server::isDublicateFd(std::vector<ServerConfig>::iterator cur)
{
    for(std::vector<ServerConfig>::iterator i = _servers.begin(); i != cur; i++)
    {
        if(i->getHost() == cur->getHost() && i->getPort() == cur->getPort())
            return (true);
    }
    return (false);
}

int Server::findDublicateFr(std::vector<ServerConfig>::iterator cur)
{
    for(std::vector<ServerConfig>::iterator i = _servers.begin(); i != cur; i++)
    {
        if(i->getHost() == cur->getHost() && i->getPort() == cur->getPort())
            return (i->getListenFd());
    }
    return -1; // эта строка не должна сработать, так как дублирование точно есть
}

void Server::startServers()// функция основного цикла сервера
{
    int ready;// переменна для хранения количества файловых дескрипторов, которые готовы для чтения
    struct timeval timer; //использую для того, что бы select возвращал значение каждую секунду, и что бы не появлялось зависание сервера
    fd_set request_fd_cpy;
    fd_set response_fd_cpy;
    _max_fd = 0;
    initializeServerConnections();
    
    while(true)//основной цикл сервера
    {
        timer.tv_sec = 1;
        timer.tv_usec = 0;
        request_fd_cpy = _request_fd_pool;
        response_fd_cpy = _response_fd_pool;
        
        if(ready = select(_max_fd + 1, &request_fd_cpy, &response_fd_cpy, NULL, &timer) < 0)
        {
            std::cout << "Error: Error with select" << std::endl;
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i < _max_fd + 1; i++)
        {
            if(FD_ISSET(i, &request_fd_cpy))
                int f = 0;//Handler::read...
        }
    }
    

}

void Server::initializeServerConnections()//инициализация наборов дескрипторов, настройка серверных сокетов и обновление данных
{
    clearFdSets();
    for(std::vector<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        setupListeningSocket(it->getListenFd());
        FD_SET(it->getListenFd(),&_request_fd_pool);
        allServers[it->getListenFd()] = *it;// добавление сервера в список всех серверов с ключевым значением FD
    }
    _max_fd = _servers.back().getListenFd();//Обновляю максимальный FD
}

void Server::clearFdSets()//очиста пула файловых дескрипторов
{
    FD_ZERO(&_request_fd_pool);
    FD_ZERO(&_response_fd_pool);
}

void Server::setupListeningSocket(int fd)
{
    if(listen(fd, 512) == -1)
    {
        std::cout << "Error: Listening error" << std::endl;//Поменять на логер
        exit(EXIT_FAILURE);
    }
    if(fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cout << "Error: Non-blocking mode error" << std::endl;//Поменять на логер
        exit(EXIT_FAILURE);
    }
}

void Server::addNewConnect()
{
    struct sockaddr_in client_address;
    long client_size = sizeof(client_address);
    int client_sock;
    
}