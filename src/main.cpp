#include "../inc/Webserv.hpp"
#include "../inc/Server.hpp"


void signalHandler(int signum) {

    logger.closeFile();

    exit(signum);  // Завершаем программу
}

int main(int argc, char **argv)
{
    if(argc == 1 || argc == 2)
    {
        try
        {
            signal(SIGINT, signalHandler);
            logger.setFile("logger.txt");//нужно будет ставить реальный с конфига
            Server Webservers;
            std::vector<ServerConfig> servers;
            servers.push_back(ServerConfig("127.0.0.1", 8082));
            servers.push_back(ServerConfig("127.0.0.1", 8083));
            servers.push_back(ServerConfig("127.0.0.1", 8082));
            Webservers.setupServer(servers);
            Webservers.startServers();
            
        }
        catch(const std::exception& e)
        {
            logger.writeMessage(e.what());
            return 1;
        }
       
    }
    else
    {
        logger.writeMessage("Error: Wrong arguments");
    }
    return 0;
}