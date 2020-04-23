

#include "fsutils.h"
#include <iostream>
#include "logger.h"

void FsInfo::init(int argc, char **argv)
{
    std::cout<<argc;
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

    Logger::getInstance().Log_trace(absPath);
    return absPath.c_str();
}


const char* getRelPath(const char *path, const char *mountPoint)
{
    return "";
}
