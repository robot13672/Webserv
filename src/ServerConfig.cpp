#include "../inc/ServerConfig.hpp"

ServerConfig::ServerConfig(std::string host, u_int16_t port) //для эмуляции отработанного конфиг файла
{
    _host = inet_addr(host.c_str()); // Example IP address
    _port = port; // Example port number
    std::cout << _host << ":" << _port << "\n";
}
ServerConfig::ServerConfig() {}


//Settings
void ServerConfig::setupServer()//функция для настройки сервера, создание сокета, привязка к порту
{
    _listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(_listen_fd == -1)
    {
        std::cerr << "Error: Fatal socket allocation for host:" << _host << std::endl;// Выводить сообщение об ошибке логером.
        exit(EXIT_FAILURE);
    }

    int tmp = 1;
    if (setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) == -1) // устраняет проблемы с занятым портом после завершения работы сокета.
    {
        //вывести сообщение через логер
        close(_listen_fd); // Закрываем сокет перед завершением
        exit(EXIT_FAILURE);
    } 

    memset(&_adress, 0, sizeof(_adress)); // обнуление структуры адреса
    _adress.sin_family = AF_INET;//установка подключения на  Ip V4
    _adress.sin_addr.s_addr = _host;//установка хоста в структуру
    _adress.sin_port = htons(_port);// установка порта в структуру

    if(bind(_listen_fd, (sockaddr *) &_adress, sizeof(_adress)) == -1)
    {
        std::cout << "Error: Error bind host:" << _host << std::endl;//вывести ошибку через логер об неуспешной привязке сокета к порту
        close(_listen_fd); // Закрываем сокет перед завершением
        exit(EXIT_FAILURE);
    }
    //вывести информацию об успешном бинде через логер
}



//SET
void ServerConfig::setFd(int fd)
{
    _listen_fd = fd;
}



//GET
in_addr_t ServerConfig::getHost()
{
    return _host;
}

u_int16_t ServerConfig::getPort()
{
    return _port;
}

int ServerConfig::getListenFd()
{
    return _listen_fd;
}