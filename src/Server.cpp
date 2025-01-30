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
        
        if(ready = select(_max_fd + 1, &request_fd_cpy, &response_fd_cpy, NULL, &timer) < 0)//ожидание события на дескрипторах
        {
            std::cout << "Error: Error with select" << std::endl;//change to logger
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i <= _max_fd ; i++)
        {   
            if(FD_ISSET(i, &request_fd_cpy) && _allServers.count(i))//Если нужно добавлять нового клиента и обязательно проверять, что это серверный сокет
            {
               addNewConnect(_allServers.find(i)->second);
            }
            else if(FD_ISSET(i, &request_fd_cpy) && _allClients.count(i)) //Если нужно читать запрос
            {
                readRequest(_allClients.find(i)->second._server.getMaxBodySize());
            }
        }
    }
    

}

void Server::initializeServerConnections()//инициализация наборов дескрипторов, настройка серверных сокетов и обновление данных
{
    clearFdSets();
    for(std::vector<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        int fd = it->getListenFd();
        if (fd == -1) {
            std::cerr << "Error: Invalid file descriptor for server with host:" << it->getHost() << ", port:" << it->getPort() << std::endl;
            continue;
        }
        setupListeningSocket(fd);
        FD_SET(fd, &_request_fd_pool);
        _allServers[fd] = *it;
    }
    _max_fd = _servers.back().getListenFd();
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

void Server::readRequest(long max_body_size)
{
    std::vector<char> buffer(max_body_size);
    int read_bytes = 0;
    read_bytes = read(i)
}

void Server::addNewConnect(ServerConfig &serv)
{
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    Client client(serv);
    char buff[INET_ADDRSTRLEN];//INET_ADDRSTRLEN - Это константа, она задает максимальную длину строки, необходимую для хранения IP-адреса в текстовом виде
    int client_sock = accept(serv.getListenFd(), (struct sockaddr *)&client_address, &client_address_len);
    if(client_sock == -1)
    {
        std::cerr << "Error: Error with listening server " << serv.getHost() << ":" << serv.getPort() << " - " << strerror(errno) << std::endl;//change to loger
        return;
    }
    std::cout << "New connection from: " << inet_ntop(AF_INET, &client_address, buff, INET_ADDRSTRLEN) << ", with socket " << client_sock << std::endl;//change to loger
    addToSet(client_sock, _request_fd_pool);
    if (fcntl(client_sock, F_SETFL, O_NONBLOCK)) //F_SETFL - указывает, то что я буду изменять флаги, O_NONBLOCK - флаг, который ставит сокет в неблокирующий режим
    {
        std::cerr << "Error: Error with FCNL " << strerror(errno) << std::endl; // change to loger
        removeFromSet(client_sock, _request_fd_pool);
        close(client_sock);
        return;
    }
    client.setSocket(client_sock);
    if(_allClients.count(client_sock) != 0)//проверка есть ли уже сокет клиента, если да, то его нужно удалить и переписать
        _allClients.erase(client_sock);//само удаление клиента.
    _allClients.insert(std::make_pair(client_sock, client));//добавление нового клиента в мапу
}


void Server::addToSet(int client_sock, fd_set &set)
{
    FD_SET(client_sock, &set);//функция которая принимает FD и сет, и добавляет его в сет
    if(client_sock > _max_fd)
        _max_fd = client_sock;
}

void Server::removeFromSet(int client_sock, fd_set &set)
{
    FD_CLR(client_sock, &set);//функция которая принимает FD и сет, и убирает его из сета
    if(client_sock == _max_fd)
        _max_fd -= 1;
}