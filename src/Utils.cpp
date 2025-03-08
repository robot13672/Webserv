#include "../inc/Utils.hpp"

std::string uint16ToString(u_int16_t value) 
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string intToString(int value) 
{
    std::stringstream ss;
    ss << value;  
    return ss.str(); 
}

std::string sockaddrToString(struct sockaddr_in addr) 
{
    // char ip[INET_ADDRSTRLEN];  
    const char* ip_str = inet_ntoa(addr.sin_addr);  
    // uint16_t port = ntohs(addr.sin_port); 

    std::ostringstream ss;
    ss << ip_str; 
    return ss.str();
}