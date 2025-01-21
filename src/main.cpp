#include "../inc/Webserv.hpp"

int main(int argc, char **argv)
{
    if(argc == 1 || argc == 2)
    {
        //logic
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1)
        {
            std::cerr << "Error: Error with socket";
            return (1);
        }
        sockaddr_in address = {0};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;   
        address.sin_port = htons(8080);

        if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)))
        {
            std::cerr << "Error: Error with bind";
            return (1);
        }
        listen(server_fd, 10);
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!"; // ответ
        while(1)
        {
            int client_fd = accept(server_fd, nullptr, nullptr); //изменить nullptr, если нужно хранить информацию про клиента
            if(client_fd == -1)
            {
                std::cerr << "Error: Error with listening";
                return (1);
            }
            std::cout << "Client connected: " << client_fd << std::endl;
            send(client_fd, response.c_str(), response.size(), 0);
            close(client_fd);
        }
        close(server_fd);
    }
    else
    {
        std::cerr << "Error: Wrong arguments"; //Change to Logger class
    }
    return 0;
}