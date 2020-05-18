#pragma once

#include <vector>

typedef unsigned char byte;

class CipherBase
{

    public: 
        CipherBase(){} 
        virtual ~CipherBase(){}

        virtual void encrypt(const std::vector<byte>&, std::vector<byte>&) = 0;
        virtual void decrypt(const std::vector<byte>&, std::vector<byte>&) = 0;
};

