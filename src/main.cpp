#include "../inc/Webserv.hpp"
#include "../inc/Server.hpp"

int main(int argc, char **argv)
{
    if(argc == 1 || argc == 2)
    {
        try
        {
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
            std::cerr << e.what() << '\n';
            return 1;
        }
       
    }
    else
    {
        std::cerr << "Error: Wrong arguments" << std::endl; //Change to Logger class
    }
    return 0;
}