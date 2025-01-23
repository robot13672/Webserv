#include "../inc/Webserv.hpp"

int main(int argc, char **argv)
{
    if(argc == 1 || argc == 2)
    {
        try
        {
            //config 
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 1;
        }
       
    }
    else
    {
        std::cerr << "Error: Wrong arguments"; //Change to Logger class
    }
    return 0;
}