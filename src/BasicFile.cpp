#include "BasicFile.h"
#include "AesCipher.h"
#include "fsutils.h"

extern "C"
{
    #include <cstring>
}

BasicFile::BasicFile(std::string &filePath, std::string &keyFile, std::string &ivFile):
m_fPath(filePath)
{
    m_ioStream.open(m_fPath, std::fstream::binary|std::fstream::out|std::fstream::in|std::fstream::app);
    if(!m_ioStream.is_open())
    {
        throw std::runtime_error("BasicFile: Could not open IOStream");
    }
}

BasicFile::~BasicFile()
{
    m_ioStream.close();
}

void BasicFile::write(const char *buf, size_t size)
{
    std::vector<byte> ptext(buf, buf + size);
    writeBlocks(ptext);
}

int BasicFile::read(char *buf, size_t size, off_t offset)
{
    int retVal = 0;
    std::vector<byte> rtext;

    readBlocks(rtext, size, offset);
        
    memcpy(buf, (char*)rtext.data(), rtext.size());
    retVal = strlen(buf);
    return retVal;
}

void BasicFile::writeBlocks(std::vector<byte> &v)
{
    size_t vectorSize = v.size();
    std::vector<byte> ctext;

    for(int i = 0; i < vectorSize; i += m_chunkSize)
    {
        ctext.clear();
        if(i + m_chunkSize <= vectorSize)
        {
            std::vector<byte>::iterator begin = v.begin() + i;
            std::vector<byte>::iterator end = begin + m_chunkSize;

            std::vector<byte> temp(begin, end);

            AesCipher::getInstance().encrypt(temp, ctext);
            writeSingleBlock(ctext);
        }
    }

    if(vectorSize % m_chunkSize)
    {
        const byte nullByte = 0;
        int fillerBits = (vectorSize + m_chunkSize) - (vectorSize % m_chunkSize);
        int bitsNum = fillerBits - vectorSize;
        for(int i = 0; i < bitsNum; i++)
        {
            v.push_back(nullByte);
        }
        
        std::vector<byte>::iterator end = v.end();
        std::vector<byte>::iterator begin = v.end() - m_chunkSize;
            
        std::vector<byte> temp(begin, end);

        AesCipher::getInstance().encrypt(temp, ctext);
        writeSingleBlock(ctext);
    }
}

void BasicFile::readBlocks(std::vector<byte> &rtext, size_t size, int offset)
{
    for(int i = 0; i < size; i += m_chunkSize)
    {
        std::vector<byte> temp;
        AesCipher::getInstance().decrypt(readSingleBlock(m_chunkSize, offset + i), temp);
        rtext.insert(rtext.end(), temp.begin(), temp.end());
    }

}

void BasicFile::writeSingleBlock(std::vector<byte> &v)
{
    m_ioStream.write((char*) v.data(), v.size());
    m_ioStream.flush();
}

std::vector<byte> BasicFile::readSingleBlock(int size, int offset)
{
    std::streampos fileSize;

    m_ioStream.seekg(offset, std::ios::end);
    fileSize = m_ioStream.tellg();
    m_ioStream.seekg(offset, std::ios::beg);

    std::vector<byte> fileData(size);
    if((size+offset) <= int(fileSize))
    {
        m_ioStream.read((char*) fileData.data(), size);
        m_ioStream.sync();
    }
    return fileData;
}