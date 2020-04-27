#include "fsutils.h"
#include "logger.h"

extern "C" 
{
    #include <string.h> 
}


void FsInfo::init(int argc, char **argv)
{
    mountPoint = realpath(argv[argc - 1], NULL);

    for(int i = 0; i < argc; ++i)
    {
        std::string s(argv[i]);
        fsArgv.push_back(s);
    }

}

const char* getAbsPath(const char *path, const std::string &mountPoint)
{

    std::string tmp(path);
    std::string absPath(mountPoint);
    absPath.append(tmp);
    if('/' == absPath.back())
    {
        absPath = absPath.substr(0, absPath.size() - 1);
    }
    return strdup(absPath.c_str());
}


const char* getRelPath(const char *path, const char *mountPoint)
{
    return "";
}
