#include "../inc/Webserv.hpp"
#include "../inc/Server.hpp"

Server* globalServer = NULL;

void signalHandler(int signum) 
{
    if (globalServer) 
    {
        delete globalServer;  // Вызов деструктора
        globalServer = NULL;
    }
    logger.closeFile();
    
    exit(signum);  // Завершаем программу
}

int main(int argc, char **argv)
{
	ParsedServerConfig *a = new ParsedServerConfig(); // roi 
	std::string confLoc;
    if(argc == 1 || argc == 2)
    {
        try
        {
			if (argc == 1)
				confLoc = "configs/default.conf";
			else
				confLoc = std::string(argv[1]);

			a->parseConfig(confLoc);
            signal(SIGINT, signalHandler);
            logger.setFile("logger.txt");
            globalServer = new Server();
            globalServer->setupServer(a->getVector());
            delete a;
            a = NULL;
            globalServer->startServers();
            delete globalServer; 
            globalServer = NULL;
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