/**
 * Logger class 
 *
 */

#pragma once

#include <string> 
#include <fstream>
class Logger
{

    public:
        ~Logger();

        static Logger& getInstance();

        void Log_op(std::string &msg);
        void Log_msg(std::string &msg);
        void Log_error(std::string &msg);

    private:
        Logger();

        static const std::string m_logFileName;
        std::ofstream m_LogFile;
};

#define LOG_OP(x) Logger::getInstance().Log_op(x)
#define LOG_MSG(x) Logger::getInstance().Log_msg(x)
#define LOG_ERR(x) Logger::getInstance().Log_error(x)
