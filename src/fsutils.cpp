#include "fsutils.h"
#include "logger.h"

extern "C" 
{
    #include <unistd.h>
    #include <string.h>
    #include <fcntl.h>	
}


void FsInfo::init(int argc, char **argv)
{
    mountPoint = realpath(argv[argc - 1], NULL);

    for(int i = 0; i < argc; ++i)
    {
        std::string s(argv[i]);
        fsArgv.push_back(s);
    }

    chdir(mountPoint.c_str()); 
    mountPointFd = open(".", 0);

    std::string log = "FsInfo init: mountPoint: " + mountPoint; 
    LOG_MSG(log); 


}

const char* getAbsPath(const char *path, const std::string &mountPoint)
{

    std::string tmp(path);
    std::string absPath(mountPoint);
    absPath.append(tmp);
    return strdup(absPath.c_str());
}


const char* getRelPath(const char *path, const std::string &mountPoint)
{
    std::string relPath = ".";
    relPath.append(path); 
    return strdup(relPath.c_str());
}
