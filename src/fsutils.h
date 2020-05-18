/**
 * Bunch of utilities
 */

#pragma once

#include <string>
#include <vector>

extern "C"
{
    #include <fuse.h>
}

//Struct containts filesystem information
//moutPoint - mounting point
//fsArgs - argument count
//fsArgv - vector containtnig arguments
struct FsInfo
{
    std::string mountPoint;
    std::string keyFile;
    std::string ivFile;
    int mountPointFd; 
    void init(int argc, char **argv);
};

//Transfers relative path (from mountpoint)
//to absolute path
const char* getAbsPath(const char *path, const std::string &mountPoint);

//Transfers absolute path
//to relative path
const char* getRelPath(const char *path);

void printUsage();