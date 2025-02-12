#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#pragma once

//standart libs
#include <iostream>// для логирования, и работы с консолью
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <exception> // для работы с исключениями
#include <sstream> //для форматирования строк при генерации HTTP ответов
#include <cstdlib> // для exit
#include <fstream> // для работы с файлами
#include <sstream> // для рработы с потоками строк
#include <ctime> 

//Системные библиотеки POSIX
#include <sys/socket.h> // для работы с сокетами
#include <arpa/inet.h>  // Для inet_ntoa
#include <netinet/in.h> // для описания структуры адресации и работы с IP-адресами
#include <unistd.h> // для системных вызовов close() и для read(), write()
#include <fcntl.h> // для управления дескрипторами файлов
#include <signal.h> // для обработки сигналов
#include <cstring> // для работы с С-строками
#include <errno.h> // для обработки ошибок, связанных с системными вызовами


//Пользовательские библиотеки

#include "ServerConfig.hpp"
// #include "Server.hpp"
#include "Logger.hpp"
// #include "HttpRequest.hpp"
#include "Utils.hpp"
// #include "HttpResponse.hpp"
// #include "Client.hpp"


//Библиотеки для CGI

#endif


