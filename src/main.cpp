#include "../inc/Webserv.hpp"
#include "../inc/Server.hpp"


void signalHandler(int signum) {

    logger.closeFile();

    exit(signum);  // Завершаем программу
}

int main(int argc, char **argv)
{
	ParsedServerConfig a; // roi 
	std::string confLoc;
    if(argc == 1 || argc == 2)
    {
        try
        {
			if (argc == 1)
				confLoc = "congifs/default.conf";
			else
				confLoc = std::string(argv[1]);

			a.parseConfig(confLoc);
			
            signal(SIGINT, signalHandler);
            logger.setFile("logger.txt");//нужно будет ставить реальный с конфига
            Server Webservers;
            // std::vector<ServerConfig> servers;
            // servers.push_back(ServerConfig("127.0.0.1", 8084));
            // servers.push_back(ServerConfig("127.0.0.1", 8085));
            // servers.push_back(ServerConfig("127.0.0.1", 8084));
            Webservers.setupServer(a.getVector());
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
		return 1;
    }
    return 0;
}