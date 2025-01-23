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
#include "Client.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include "ServerConfig.hpp"


//Библиотеки для CGI




