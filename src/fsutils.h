/**
 * Bunch of utilities
 */

#pragma once

#include <string>

//Struct containts filesystem information
//moutPoint - mounting point
//keyFile - file containing encryption key 
//ivFile - file containing initialization vector 
//mountPointFd - file descriptor of mountPoint
struct FsInfo
{
    std::string mountPoint;
    std::string keyFile;
    std::string ivFile;
    int mountPointFd; 
    void init(int argc, char **argv);
};

//Transfers path (from mountpoint)
//to absolute path
const char* getAbsPath(const char *path, const std::string &mountPoint);

//Transfers path
//to relative path
const char* getRelPath(const char *path);
