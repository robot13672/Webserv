#include "../inc/Webserv.hpp"

int main(int argc, char **argv)
{
    if(argc == 1 || argc == 2)
    {
        // ================================> roi
		try
		{
			ConfParser cluster;
			cluster.createCluster("default.conf");
		}
		catch (std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return 1;
		}
		// ================================> roi

		
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

		std::cout << "TEST: " << std::endl;	// ======================> roi

        if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)))
        {
            std::cerr << "Error: Error with bind";
            return (1);
        }
        listen(server_fd, 10);
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!"; // ответ
        while(1)
        {
            sockaddr_in client_adr; // Тут запишется информация про клиента
            socklen_t client_len = sizeof(client_adr); // размер клиента

            int client_fd = accept(server_fd, (struct sockaddr*) &client_adr, &client_len); 
            if(client_fd == -1)
            {
                std::cerr << "Error: Error with listening";
                return (1);
            }
            Client client = Client(client_fd, client_adr);// инициализация клиента
            std::cout << client._sockfd << "\n" << client._ip << "\n" << client._port << std::endl;
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