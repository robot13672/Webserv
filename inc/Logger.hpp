#pragma once

#include "Webserv.hpp"

enum Mode {CONSOLE_OUTPUT, FILE_OUTPUT };

class Logger
{
    private:
        std::string             _file;
        std::ofstream           _fileStream;   
        Mode                    _mode;

    public:
        Logger();
        // Logger(std::string file);
        ~Logger();
        std::string getCurrTime();
        void setFile(std::string _file);
        void writeMessage(std::string message);
        void closeFile(void);
             
};

extern Logger logger;
