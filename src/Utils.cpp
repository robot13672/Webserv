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