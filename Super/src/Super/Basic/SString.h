#ifndef _STRINGV2_H_INCLUDED
#define _STRINGV2_H_INCLUDED

#include "global.h"
#include <iostream>

NAME_SPACE_MAIN_BEGIN

//一种隐式共享/写时复制 的string






/*
这个错误还主要是我调用了一个Utils的辅助库，Utils里面有一个头文件叫 String.h, 包含这个String.h的时候应该是编译器把它当成VC库里的那个 string.h了，因为VC对文件名字是不区分大小写的，所以编译时才导致这个错误。
最好的解决办法，就是把Utils库里的文件 String.h给改名字，这样问题就彻底根除了。
*/

/***
String类的部分实现，采用的内存管理策略是：隐式共享，写时复制
实现方法：与智能指针的实现类似
***/



class String
{
    public:
        String();
        String(const String& s);
        String(const char *pc, size_t len);
        String(const char *pc);
        ~String();

        String& operator=(const String &s);
        String& operator=(const char *s);

        String& operator+=(const String &rhs);
        String& operator+=(const char *rhs);

        void clear();
        size_t getLength()const {return _length;}
        const char* cstr()const {return _cstr;}

    private://function
        void _initString(const char *cstr, size_t len);
        void _decUsed();
        char* _renewAndCat(const char *cstr, size_t len);
        void _addString(const char *cstr, size_t len);
        void _addAssignOpt(const char *cstr, size_t len);

    private://data
        char *_cstr;
        size_t *_used;
        size_t _length;
        size_t _capacity;
};

String operator+(const String &lhs, const String &rhs);
std::ostream& operator <<(std::ostream &os, const String &s);
std::istream& operator >>(std::istream &in, String &s);


#ifdef UNIT_TEST

BASIC_API int Test_String();
#endif


NAME_SPACE_MAIN_END
#endif // _STRINGV2_H_INCLUDED
