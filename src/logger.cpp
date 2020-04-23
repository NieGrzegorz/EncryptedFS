#include "logger.h" 

const std::string Logger::m_logFileName = "/home/greg/fs_logfile.txt";
Logger::Logger()
{
    m_LogFile.open(m_logFileName, std::ios::in | std::ios::app); 

}

Logger::~Logger()
{
    m_LogFile.close();
}

void Logger::Log_trace(std::string &msg)
{
    m_LogFile<<msg;
    m_LogFile<<"\n";
}

void Logger::Log_msg()
{

}

void Logger::Log_error()
{

}

Logger& Logger::getInstance()
{
    static Logger instance; 
    return instance; 
}
