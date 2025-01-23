/* 
	Parser - roi 25.0123
*/
#include "../inc/ConfParser.hpp"

ConfParser::ConfParser() {}
ConfParser::~ConfParser() {}

/* 
roi 
should check and read config file, split servers to strings and create vector of servers 
*/
void ConfParser::createCluster(const std::string &conf_file)
{
	std::cout << "int ConfParser::createCluster(\"" << conf_file << "\")" << std::endl;
	return 1;
}