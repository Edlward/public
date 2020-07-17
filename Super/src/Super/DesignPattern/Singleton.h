#pragma once


//多例模式，待续--自创
class Singleton 
{
public:
    static Singleton* Instance(unsigned int index);
protected:
    Singleton();
private:
    static enum
    {
        Etype_One=0,
        Etype_One,
        Total
    };
};

Singleton* Singleton::Instance(unsigned int index)
{
    static Singleton gSingleton[Total];
    return &gSingleton[Total];
}

