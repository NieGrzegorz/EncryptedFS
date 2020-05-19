#include "fsutils.h"
#include "logger.h"
#include <iostream>

extern "C" 
{
    #include <unistd.h>
    #include <string.h>
    #include <fcntl.h>
}

constexpr int minArgc = 4;

void FsInfo::init(int argc, char **argv)
{
    if(minArgc > argc)
    {
        mountPoint= strdup(realpath(argv[argc - 3], NULL));
        keyFile= strdup(realpath(argv[argc - 2], NULL));
        ivFile= strdup(realpath(argv[argc - 1], NULL));

        chdir(mountPoint.c_str()); 
        mountPointFd = open(".", 0);

        std::string log = "FsInfo init: mountPoint: " + mountPoint;
        LOG_MSG(log);
    }
    else
    {
        printUsage(); 
        throw std::runtime_error("Not enough arguments");
    }
}

const char* getAbsPath(const char *path, const std::string &mountPoint)
{
    std::string tmp(path);
    std::string absPath(mountPoint);
    absPath.append(tmp);
    return strdup(absPath.c_str());
}

const char* getRelPath(const char *path)
{
    std::string relPath = ".";
    relPath.append(path); 
    return strdup(relPath.c_str());
}

void printUsage()
{
    std::cout<<"Usage: \n";
    std::cout<<"./encfs [fuseFlags (optional)] [mounpoint] [AES key file] [AES IV file]\n";
}



