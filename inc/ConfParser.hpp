/* 
	Parser - roi 25.0123
*/
#ifndef CONFPARSER_HPP
# define CONFPARSER_HPP
# include "Webserv.hpp"

class ConfParser
{
	private:
	public:
		ConfParser();
		~ConfParser();
		
		int createCluster(std::string);
};
#endif // CONFPARSER_HPP