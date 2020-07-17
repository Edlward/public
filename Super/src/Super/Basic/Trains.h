#ifndef _A_NewClassTemplate_Header_
#define _A_NewClassTemplate_Header_

#include "global.h" 

#include <memory.h>

NAME_SPACE_MAIN_BEGIN


 //C++���ж������Ƿ�Ϊ��������


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
    // T�Ƿ�Ϊ��������--ͨ����ģ����ػ���ʵ�ֵ�
    if (TypeTraits<T>::PODTYPE::Get())
    {
        // memcpy��ǳ����
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


