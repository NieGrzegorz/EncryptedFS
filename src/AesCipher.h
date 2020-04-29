#include "CipherBase.h"

class AesCipher : public CipherBase
{
    public: 
        AesCipher();
        ~AesCipher();
        std::string encrypt(const std::string &msg);
        std::string decrypt(const std::string &msg);


    private: 

};