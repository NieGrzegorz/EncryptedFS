#include "logger.h" 

const std::string Logger::m_logFileName = "/home/greg/fs_logfile.txt";
Logger::Logger()
{
    m_LogFile.open(m_logFileName, std::ios::in); 

}

Logger::~Logger()
{
    m_LogFile.close();
}

void Logger::Log_op(std::string &msg)
{
    m_LogFile<<"[OP]: " + msg;
    m_LogFile<<"\n";
}

void Logger::Log_msg(std::string &msg)
{
    m_LogFile<<"[MSG]: " + msg; 
    m_LogFile<<"\n";
}

void Logger::Log_error(std::string &msg)
{
    m_LogFile<<"[ERR]: " + msg; 
    m_LogFile<<"\n"; 
}

Logger& Logger::getInstance()
{
    static Logger instance; 
    return instance; 
}
