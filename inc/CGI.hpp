#include "Webserv.hpp"

class HttpRequest;

class CGI
{
    private:
        std::map<std::string, std::string>  _cgiEnv;
        int                                 _timeout;
        bool                                _isRuninng;
        int                                 _exitStatus; // Exit status of CGI process
    
    public:
        int inputPipe[2];
        int outputPipe[2];
        void SetEnv(HttpRequest &request);





};