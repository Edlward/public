#ifndef _A_NewClassTemplate_Header_
#define _A_NewClassTemplate_Header_

#include "global.h" 

#include <memory.h>

NAME_SPACE_MAIN_BEGIN


 //C++中判断类型是否为内置类型


struct FalseType
{
    const static bool Get()
    {
        return false;
    }
};

struct TrueType
{
    const static bool Get()
    {
        return true;
    }
};

template<class T>
struct TypeTraits
{
    typedef FalseType PODTYPE;
};

template<>
struct TypeTraits<int>
{
    typedef TrueType PODTYPE;
};

template<>
struct TypeTraits<char>
{
    typedef TrueType PODTYPE;
};

template<>
struct TypeTraits<double>
{
    typedef TrueType PODTYPE;
};

template<>
struct TypeTraits<float>
{
    typedef TrueType PODTYPE;
};

template<class T>
void MemCopy(T* dst, const T* src, size_t size)
{
    // T是否为内置类型--通过类模板的特化来实现的
    if (TypeTraits<T>::PODTYPE::Get())
    {
        // memcpy是浅拷贝
        memcpy(dst, src, sizeof(T)*size);
    }
    else
    {
        for (size_t i = 0; i < size; ++i)
        {
            dst[i] = src[i];
        }
    }
}



NAME_SPACE_MAIN_END



#ifdef UNIT_TEST
BASIC_API int Test_A_NewClassTemplate();
#endif

#endif


