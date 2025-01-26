#pragma once

#include "Webserv.hpp"

class ServerConfig;

void handleReadEvent(ServerConfig &serv, int fd);
void handleWriteEvent(ServerConfig &serv, int fd);
