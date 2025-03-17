#include "../inc/Server.hpp"

Server::~Server()
{
    std::map<int, Client>::iterator it = _allClients.begin();
    while (it != _allClients.end())
    {
        _allClients.erase(it++);
    }

    // // Close all server sockets
    // for (std::map<int, ServerConfig>::iterator it = _allServers.begin(); it != _allServers.end(); ++it) {
    //     close(it->first);
    // }

    // _servers.clear();
    // _allServers.clear();
    // _allClients.clear();
    
    // // Clear fd sets
    // FD_ZERO(&_request_fd_pool);
    // FD_ZERO(&_response_fd_pool);
}

void Server::setupServer(std::vector<ServerConfig> servers) 
{
    _servers = servers;
    for (std::vector<ServerConfig>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        if(isDublicateFd(it))
            it->setFd(findDublicateFr(it));//ищу дублирующий дескриптор и подставляю его в текущий сервер
        else
            it->setupServer();
        logger.writeMessage("Server created with host:" + it->getHost() + ", port:" + uint16ToString(it->getPort()));
        // std::cout << "Server created with host:" << it->getHost() << ", port:" << it->getPort() << std::endl; //Изменить это, и выводить через логер
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
        ready = select(_max_fd + 1, &request_fd_cpy, &response_fd_cpy, NULL, &timer);

        if(ready < 0)//ожидание события на дескрипторах
        {
            logger.writeMessage("Error: Error with select: " + std::string(strerror(errno)));
            std::cout << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
         for(int i = 3; i <= _max_fd ; i++)
        {   
            if(FD_ISSET(i, &request_fd_cpy) && _allServers.count(i))//Если нужно добавлять нового клиента и обязательно проверять, что это серверный сокет
            {
               addNewConnect(_allServers.find(i)->second);
            }
            else if(FD_ISSET(i, &request_fd_cpy) && _allClients.count(i)) //Если нужно читать запрос
            {
                readRequest(i, _allClients[i]);
            }
            else if(FD_ISSET(i, &response_fd_cpy) && _allClients.count(i))//Если нужно отправить респонс
            {
                sendResponse(i, _allClients[i]);
                // return;
            }

        }
        checkTimeout();
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
        logger.writeMessage("Error: Listening error");
        exit(EXIT_FAILURE);
    }
    if(fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
    {
        logger.writeMessage("Error: Non-blocking mode error");
        exit(EXIT_FAILURE);
    }
}

void Server::readRequest(int &fd, Client &client)
{
    const long BUFFER_SIZE = 10485760; 
    std::vector<char> buffer(BUFFER_SIZE);
    int readedBytes = read(fd, buffer.data(), BUFFER_SIZE);
    

    if(readedBytes == 0)
    {
        handleClientDisconnection(fd);
        return;
    }
    if(readedBytes < 0)
    {
        handleReadError(fd);
        return;
    }

    buffer[readedBytes] = '\0';
    processClientData(client,buffer, readedBytes);
    buffer.clear();
    logger.writeMessage("New message from " + intToString(fd));
    if(client._request.getStatus() || client._request.IsBodyTooBig())//Проверка на полное чтение запроса
    {
        if(client._request.isChunkedTransfer() && client._request.getContentLength() == 0)
        {
            client._request.parseFullChankedBody();
        }
        removeFromSet(fd, _request_fd_pool);
        addToSet(fd, _response_fd_pool);
    }
    
    // const int BUFFER_SIZE = 16384; // 16 kb
    // char buffer[BUFFER_SIZE + 1];
    // int readedBytes = read(fd, buffer, BUFFER_SIZE);
    
    // if(readedBytes == 0)
    // {
    //     handleClientDisconnection(fd);
    //     return;
    // }
    // if(readedBytes < 0)
    // {
    //     handleReadError(fd);
    //     return;
    // }

    // buffer[readedBytes] = '\0';

    // std::string file = createNewTxt(buffer, readedBytes);
    // int buff_fd = open(file.c_str(), O_RDONLY | O_APPEND);
    // processClientData(client, buff_fd, readedBytes);
    // memset(buffer, 0, readedBytes + 1);
    // close(buff_fd);
    // // if(!remove(file.c_str()))
    // //     logger.writeMessage("Deleted file: " + file);
    // logger.writeMessage("New message from " + intToString(fd));
    // removeFromSet(fd, _request_fd_pool);
    // addToSet(fd, _response_fd_pool);
}

void Server::sendResponse(int &fd, Client &client)
{
    int sendedBytes = 0;
    //TODO поменять на реальный ответ
    // std::string response = "HTTP/1.1 200 OK\r\n"
    //        "Content-Type: text/plain\r\n"
    //        "Content-Length: 13\r\n"
    //        "\r\n"
    //        "Hello, World!";
    // //TODO: дописать все конструкторы копирования, что бы можно было обратиться к макс боти сайз
    // // if(response.length() >= client._server.getMaxBodySize())
    // //     std::cout << "Error 413";
    // sendedBytes = write(fd, response.c_str(), response.length());

    
    client._response.setServer(client._server);
    client._response.setHttpVersion("HTTP/1.1");
    
    // Get the path and method from client's request
    // response.setPath(client._request.getPath());    // Use setter instead of direct access
    // response.setMethod(client._request.getMethod()); 


    // Handle the request based on method and path
    client._response.handleResponse(client._request);
    if(client._request.IsBodyTooBig())
        client._request.setBodyTooBig(false);
    else
        client._request.clear(); //очистка запроса
    
    // Check max body size
    // if (client._request.getContentLength() >= client._server.getMaxBodySize())
    // {
    //     response.setErrorResponse(413, "Payload Too Large");
    // }
    
    // Convert response to string
    std::string responseStr = client._response.toString();
    sendedBytes = write(fd, responseStr.c_str(), responseStr.length());
    
    if (sendedBytes < 0)
    {
        logger.writeMessage("Error: Failed to send response to client " + intToString(fd));
        handleClientDisconnection(fd);
        return;
    }
    removeFromSet(fd, _response_fd_pool);
    addToSet(fd, _request_fd_pool);
}

void Server::handleClientDisconnection(int clientFd)
{
    logger.writeMessage("webserv: Conenction with " + intToString(clientFd) + "fd closed!");
    closeFd(clientFd);
}

void Server::handleReadError(int clientFd)
{
    logger.writeMessage("Error: error with read message from: " + intToString(clientFd));
    closeFd(clientFd);
}

void Server::processClientData(Client &client, std::vector<char> buffer, int readedBytes)
{
    client.updateTime();
    client._request.parseRequest(buffer, readedBytes);
    //TODO: отправить реквест который получили от клиента
    
    
}
void Server::closeFd(int fd)
{
    if(FD_ISSET(fd, &_request_fd_pool))
    {
        removeFromSet(fd, _request_fd_pool);
    }
    if(FD_ISSET(fd, &_response_fd_pool))
    {
        removeFromSet(fd, _response_fd_pool);
    }
    close(fd);
    _allClients.erase(fd);
}
void Server::addNewConnect(ServerConfig &serv)
{
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    Client client(serv);
    // char buff[INET_ADDRSTRLEN];//INET_ADDRSTRLEN - Это константа, она задает максимальную длину строки, необходимую для хранения IP-адреса в текстовом виде
    int client_sock = accept(serv.getListenFd(), (struct sockaddr *)&client_address, &client_address_len);
    client.setSocket(client_sock);
    if(client_sock == -1)
    {
        logger.writeMessage("Error: Error with listening server " + serv.getHost() + ":" + uint16ToString(serv.getPort()) + "-");
        return;
    }
    logger.writeMessage("New connection from: " + sockaddrToString(client_address) + ", with socket " + intToString(client_sock));
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

void Server::checkTimeout()
{
    for(int i = 0; i <= _max_fd; i++)
    {
        if(_allClients.count(i))
            if (time(NULL) - _allClients.find(i)->second.getLstMsg() > 6000)//TODO: изменить на 60
                handleClientDisconnection(i);
    }
}

std::string Server::createNewTxt(char *buff, int size)
{
    std::string fileName = intToString(time(NULL)) + ".txt";
    std::ofstream outFile(fileName.c_str());
    if (outFile.is_open()) 
    {
        outFile.write(buff, size);
        outFile.close();
    } 
    else 
    {
        logger.writeMessage("Error: Unable to open file " + fileName);
    }
    logger.writeMessage("Created file: " + fileName);
    return fileName;
}