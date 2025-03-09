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

/* 
			// oleh 0309 - debug insert start
			{
				// Вывод всех конфигураций серверов
				for (size_t i = 0; i < serverParsedConfigs.size(); ++i) //vbstd::vector<ParsedServerConfig> serverParsedConfigs
				std::cout << YELLOW << "main() - After parseConfig(): " << serverParsedConfigs[i] << std::endl << RESET; // debug - roi 0228
			}			
			// oleh 0309 - debug insert end 
 */

			{
			// Вывод конфигураций через объект ParsedServerConfig
			ParsedServerConfig a;
			a.parseConfig("congifs/default.conf");
			std::cout << YELLOW << "Number of servers: " << a.getServerParsedConfigs().size() << std::endl;
			for(size_t i = 0; i < a.getServerParsedConfigs().size(); ++i) {
				std::cout << "Server " << i << ":\n" << a.getServerParsedConfigs()[i] << std::endl;
			}
			std::cout << RESET;
			}

/* 
 			// Вывод конфигураций через getVector() - debug by oleh 0309
			std::vector<ServerConfig> configs = a.getVector();
			for (size_t i = 0; i < configs.size(); ++i)
			{
			std::cout << YELLOW << "main() - After parseConfig(): Server " << i << ", Port: " << configs[i].getPort() << ", Host: " << configs[i].getHost() << std::endl << RESET;
			} 
 */

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