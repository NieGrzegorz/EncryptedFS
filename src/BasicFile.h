#pragma once

#include <fstream>
#include <memory>
#include "AesCipher.h"

class BasicFile
{
    public:
        BasicFile(std::string&, std::string&, std::string&);
        ~BasicFile();
        
        void write(const char *buf, size_t size);
        int read(char *buf, size_t size, off_t offset);

    private:
        void writeBlocks(std::vector<byte> &v);
        void readBlocks(std::vector<byte> &rtext, size_t size, int offset);

        void writeSingleBlock(std::vector<byte> &v);
        std::vector<byte> readSingleBlock(int size, int offset);

    private:
        const int m_chunkSize = 16;

        std::string m_fPath;
        std::fstream m_ioStream;
        std::ifstream m_istream;
        std::ofstream m_ostream; 
        std::shared_ptr<AesCipher> m_cipher;

        std::string keyFile, ivFile;
};