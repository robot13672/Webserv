#include "../inc/Webserv.hpp"
#include "../inc/Server.hpp"


void signalHandler(int signum) {

	logger.closeFile();

	exit(signum);  // Завершаем программу
}

int main(int argc, char **argv)
{
	ServerConfig a;
	std::string confLoc;
	if(argc == 1 || argc == 2)
	{
		try
		{
			/* roi 0225 - insert start */
			if (argc == 1)
				confLoc = "congifs/default.conf";
			else
				confLoc = std::string(argv[1]);
				a.parseConfig(confLoc); // roi 0224 Явное преобразование char* в std::string
				std::cout << YELLOW << "main() - After parseConfig(): " << a << std::endl << RESET; // debug
			/* roi 0225 - insert end */

			signal(SIGINT, signalHandler);
			logger.setFile("logger.txt");//нужно будет ставить реальный с конфига
			Server Webservers;
			std::vector<ServerConfig> servers;
			servers.push_back(ServerConfig("127.0.0.1", 8084));
			servers.push_back(ServerConfig("127.0.0.1", 8085));
			servers.push_back(ServerConfig("127.0.0.1", 8084));
			Webservers.setupServer(servers);
			Webservers.startServers();
			
		}
		catch(const std::exception& e)
		{
			// std::cerr << e.what() << std::endl; // roi 0224 - temporary to check fileopen error
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