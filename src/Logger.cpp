#include "../inc/Logger.hpp"



Logger::Logger() : _file("")
{
    _mode = CONSOLE_OUTPUT;
    // std::cout << getCurrTime() << ": Logger ready in CONSOLE_OUTPUT_MODE" << std::endl;
}

// Logger::Logger(std::string file) : _file(file)
// {
//     _fileStream.open(_file.c_str(), std::ios::app);
//     if(_fileStream)
//     {
//         _mode = FILE_OUTPUT;
//         _fileStream << "-------------------------------------------------" << std::endl;
//         _fileStream << "New session: " << getCurrTime() << std::endl;
//         _fileStream << "-------------------------------------------------" << std::endl;
//         std::cout << getCurrTime() <<": Logger ready in FILE_OUTPUT_MODE" << std::endl;
//     }
//     else
//     {
//         _mode = CONSOLE_OUTPUT;
//         std::cout << getCurrTime() << ":Logger ready in CONSOLE_OUTPUT_MODE" << std::endl;
//     }
// }

Logger::~Logger()
{
    if(_fileStream.is_open())
    {
        _fileStream << "-------------------------------------------------" << std::endl;
        _fileStream << "Session finish: " << getCurrTime() << std::endl;
        _fileStream << "-------------------------------------------------" << std::endl;
        _fileStream.close();
    }
}

void Logger::setFile(std::string _file)
{
    _fileStream.open(_file.c_str(), std::ios::app);
    if(_fileStream)
    {
        _mode = FILE_OUTPUT;
        _fileStream << "-------------------------------------------------" << std::endl;
        _fileStream << "New session: " << getCurrTime() << std::endl;
        _fileStream << "-------------------------------------------------" << std::endl;
        std::cout << getCurrTime() <<": Logger ready in FILE_OUTPUT_MODE" << std::endl;
    }
}

std::string Logger::getCurrTime()
{
    char buffer[20];
    time_t now = time(NULL);
    tm *tm = localtime(&now);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    return std::string(buffer);
}

void Logger::writeMessage(std::string message)
{
    if(_mode == FILE_OUTPUT)
    {
        _fileStream << getCurrTime() << ": " << message << std::endl;
    }
    std::cout << getCurrTime() << ": " << message << std::endl;     
}

void Logger::closeFile()
{
    if(_fileStream.is_open())
    {
        _fileStream << "-------------------------------------------------" << std::endl;
        _fileStream << "Session finish: " << getCurrTime() << std::endl;
        _fileStream << "-------------------------------------------------" << std::endl;
        _fileStream.close();
    }
}

Logger logger;
