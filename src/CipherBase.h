
class CipherBase
{

    public: 
        CipherBase(); 
        virtual ~CipherBase();


        virtual void encrypt() = 0;
        virtual void decrypt() = 0;
};

