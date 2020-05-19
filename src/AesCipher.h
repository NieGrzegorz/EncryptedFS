#pragma once

#include <string>
#include <stdexcept>
#include <vector>

#include "CipherBase.h"

class AesCipher : public CipherBase
{
    public:

        AesCipher(std::string&, std::string&);
        ~AesCipher() noexcept;

        void encrypt(const std::vector<byte> &ptext, std::vector<byte> &ctext) override;
        void decrypt(const std::vector<byte> &ctext, std::vector<byte> &rtext) override;
        void setCipherData(std::string&, std::string&);

    private: 
        std::vector<byte> getCipherData(std::string);

        static constexpr unsigned int m_blockSize = 16;
        std::string m_keyFile;
        std::string m_ivFile;

        std::vector<byte> m_key;
        std::vector<byte> m_iv;
};