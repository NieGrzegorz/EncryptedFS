#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <stdexcept>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

#include "CipherBase.h"

class AesCipher
{
    public: 

        ~AesCipher() ;
                AesCipher();
                        AesCipher(std::string&, std::string&);
        void encrypt(const std::vector<byte> &ptext, std::vector<byte> &ctext) ;
        void decrypt(const std::vector<byte> &ctext, std::vector<byte> &rtext) ;
        void setCipherData(std::string&, std::string&);
        static AesCipher& getInstance();

    


    private: 
        std::vector<byte> getCipherData(std::string);

        static constexpr unsigned int m_blockSize = 16;
        std::string m_keyFile = "/home/greg/scrt/AesKey";
        std::string m_ivFile  = "/home/greg/scrt/AesIV";

        std::vector<byte> m_key;
        std::vector<byte> m_iv;
};