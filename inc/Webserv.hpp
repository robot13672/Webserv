#pragma once

//standart libs
#include <iostream>// для логирования, и работы с консолью
#include <string>
#include <vector>
#include <map>
#include <set>
#include <exception> // для работы с исключениями
#include <sstream> //для форматирования строк при генерации HTTP ответов

//Системные библиотеки POSIX
#include <sys/socket.h> // для работы с сокетами
#include <netinet/in.h> // для описания структуры адресации и работы с IP-адресами
#include <unistd.h> // для системных вызовов close() и для read(), write()
#include <fcntl.h> // для управления дескрипторами файлов
#include <signal.h> // для обработки сигналов
#include <cstring> // для работы с С-строками
#include <errno.h> // для обработки ошибок, связанных с системными вызовами

//Пользовательские библиотеки
//e.g.  Logger


//Библиотеки для CGI




