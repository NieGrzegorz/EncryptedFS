
#include <fstream>
#include <memory>
#include <openssl/err.h>
#include <openssl/evp.h>

#include "AesCipher.h"

using EvpCtx = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

AesCipher::AesCipher(std::string &keyFile, std::string &ivFile)
{
    EVP_add_cipher(EVP_aes_256_cbc());
    m_key = getCipherData(keyFile);
    m_iv = getCipherData(ivFile);
}

AesCipher::~AesCipher()
{
}

void AesCipher::encrypt(const std::vector<byte> &ptext, std::vector<byte> &ctext)
{
    EvpCtx ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);

    int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, m_key.data(), m_iv.data());
    if (rc != 1)
    {
        throw std::runtime_error("AesCipher: EVP_EncryptInit_ex failed");
    }

    EVP_CIPHER_CTX_set_padding(ctx.get(), 0);

    ctext.resize(ptext.size() + m_blockSize);
    int out_len1 = (int)ctext.size();

    rc = EVP_EncryptUpdate(ctx.get(), ctext.data(), &out_len1, ptext.data(), (int)ptext.size());
    if (rc != 1)
    {
        throw std::runtime_error("AesCipher: EVP_EncryptUpdate failed");
    }

    int out_len2 = (int)ctext.size() - out_len1;
    rc = EVP_EncryptFinal_ex(ctx.get(), ctext.data() + out_len1, &out_len2);
    if (rc != 1)
    {
        throw std::runtime_error("AesCipher: EVP_EncryptFinal_ex failed");
    }

    ctext.resize(out_len1 + out_len2);
    EVP_CIPHER_CTX_cleanup(ctx.get());
}

void AesCipher::decrypt(const std::vector<byte> &ctext, std::vector<byte> &rtext)
{
    EvpCtx ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);

    int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, m_key.data(), m_iv.data());
    if (rc != 1)
    {
        throw std::runtime_error("AesCipher: EVP_DecryptInit_ex failed");
    }

    EVP_CIPHER_CTX_set_padding(ctx.get(), 0);
    rtext.resize(ctext.size());
    int out_len1 = (int)rtext.size();

    rc = EVP_DecryptUpdate(ctx.get(), rtext.data(), &out_len1, ctext.data(), (int)ctext.size());
    if (rc != 1)
    {
        throw std::runtime_error("AesCipher: EVP_DecryptUpdate failed");
    }
  
    int out_len2 = (int)rtext.size() - out_len1;
    rc = EVP_DecryptFinal_ex(ctx.get(), rtext.data() + out_len1, &out_len2);
    if (rc != 1)
    {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error("AesCipher: EVP_DecryptFinal_ex failed");
    }

    rtext.resize(out_len1 + out_len2);
    EVP_CIPHER_CTX_cleanup(ctx.get());
}

void AesCipher::setCipherData(std::string &keyFile, std::string &ivFile)
{
    m_keyFile = keyFile; 
    m_ivFile = ivFile;

    m_key = getCipherData(m_keyFile);
    m_iv = getCipherData(m_ivFile);
}

std::vector<byte> AesCipher::getCipherData(std::string filename)
{
    std::streampos fileSize;
    std::ifstream istream;
    istream.open(filename, std::ifstream::binary|std::ifstream::in);
    if(istream.is_open())
    {
        istream.seekg(0, std::ios::end);
        fileSize = istream.tellg();
        istream.seekg(0, std::ios::beg);

        std::vector<byte> fileData(fileSize);

        istream.read((char*) fileData.data(), fileSize);
        istream.sync();
        istream.close();
        return fileData;
    }
    else
    {
        throw std::runtime_error("AesCipher: Failed to read Aes key/iv");
    }
}

