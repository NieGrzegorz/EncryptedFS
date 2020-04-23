
#include <string> 
#include <fstream>
class Logger
{

    public:
        ~Logger(); 

        static Logger& getInstance(); 

        void Log_trace(std::string &msg); 
        void Log_msg(); 
        void Log_error(); 

    private:
        Logger(); 

        static const std::string m_logFileName; 
        std::ofstream m_LogFile; 
};
