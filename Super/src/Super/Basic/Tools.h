#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "global.h"
#include <typeinfo>
#include <type_traits>
#include <stdio.h> //for printf
#include <vector>
#include <atomic> 



NAME_SPACE_MAIN_BEGIN


struct _FalseType{};
struct _TrueType{};

template <class T>
struct _TypeTraits
{
    typedef _FalseType IsPODType;//内嵌类型重定义
};
template<>
struct _TypeTraits<int>//内嵌类型
{
    typedef _TrueType IsPODType;
};

template<class T>
void copy_impl(T* dst, const T* str, size_t n, _FalseType)
{
    for (size_t i = 0; i < n; i++)
    {
        dst[i] = str[i];
    }
}
template<class T>
void copy_impl(T* dst, const T* str, size_t n, _TrueType)
{
    ::memcpy(dst, str, n*sizeof(str));
}
template<class T>
void Copy(T* dst, const T* str, size_t n)
{
    copy_impl(dst, str, n, _TypeTraits<T>::IsPODType());
}


namespace detail 
{

    struct FalseType
    {
        //const static bool Get()
        //{
        //    return false;
        //}
        const static bool value=false;
    };

    struct TrueType
    {
        //const static bool Get()
        //{
        //    return true;
        //}
        const static bool value=true;
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
    //template <class T>
    //void copyData_Impl(T* dst,const T* src,size_t size)
    //{
    //    for (size_t n=0;n<size;n++)
    //    {
    //        dst[n]=src[n];
    //    }
    //}
    //template <class T>
    //void copyData_Impl<TrueType>(T* dst,const T* src,size_t size)
    //{
    //    //::memcpy(dst,src,size*sizeof(T));
    //}
    //template <class T>
    //void copyData_Impl<T>(T* dst,const T* src,size_t size,TrueType)
    //{
    //    //::memcpy(dst,src,size*sizeof(T));
    //}
    //template <class T>
    //void copyData_Impl<FalseType>(T* dst,const T* src,size_t size)
    //{
    //    for (size_t n=0;n<size;n++)
    //    {
    //        dst[n]=src[n];
    //    }
    //}
    //template <class T>
    //void copyData(T* dst,const T* src,size_t size)
    //{
    //   //copyData_Impl<std::is_pod<T>(dst,src,size);
    //    //copyData_Impl<TypeTraits<T>::PODTYPE>(dst,src,size);
    //}

//    // 因为在函数模版里面压根就没有偏特化的概念
//    //相当于函数重载，改进待续
//    template<typename T>
//    struct Identity
//    {
//        typedef T type;

//        Identity(){}
//        ~Identity(){}
//        char tmp;  //nouse
//    };
//    template <class T>
//    void copyData_Impl(T* dst,const T* src,size_t size)
//    {
//        for (size_t n=0;n<size;n++)
//        {
//            dst[n]=src[n];
//        }
//    }
//    template <class T>
//    void copyData_Impl(T* dst,const T* src,size_t size,Identity<FalseType>)
//    {
//        for (size_t n=0;n<size;n++)
//        {
//            dst[n]=src[n];
//        }
//    }
//    template <class T>
//    void copyData_Impl(T* dst,const T* src,size_t size,Identity<TrueType>)
//    {
//        //::memcpy(dst,src,size*sizeof(T));
//    }
//    template <class T>
//    void copyData(T* dst,const T* src,size_t size)
//    {
//        copyData_Impl(dst,src,size,Identity<TypeTraits<T>::PODTYPE>);
//    }

    //Method3 TODO:借助类特化,失败,待续
    //template <class T>
    //class MemHelper
    //{
    //    static void copyData_Impl(T* dst,const T* src,size_t size)
    //    {
    //        for (size_t n=0;n<size;n++)
    //        {
    //            dst[n]=src[n];
    //        }
    //    }
    //    template<>
    //    static void copyData_Impl<FalseType>(T* dst,const T* src,size_t size)
    //    {
    //        for (size_t n=0;n<size;n++)
    //        {
    //            dst[n]=src[n];
    //        }
    //    }
    //    template<>
    //    static void copyData_Impl<TrueType>(T* dst,const T* src,size_t size,TrueType)
    //    {
    //        ::memcpy(dst,src,size*sizeof(T));
    //    }
    //    template <class T>
    //    static void copyData(T* dst,const T* src,size_t size)
    //    {
    //        copyData_Impl<TypeTraits<T>::PODTYPE>(dst,src,size);
    //    }
    //};

}





template <typename T> inline T *GetPtrHelper(T *ptr) { return ptr; }
template <typename Ptr> inline auto GetPtrHelper(const Ptr &ptr) -> decltype(ptr.operator->()) { return ptr.operator->(); }



//void qt_check_pointer(const char *, int);
//#ifdef QT_NO_EXCEPTIONS
//#  if defined(QT_NO_DEBUG) && !defined(QT_FORCE_ASSERTS)
//#    define Q_CHECK_PTR(p) qt_noop()
//#  else
//#    define Q_CHECK_PTR(p) do {if (!(p)) qt_check_pointer(__FILE__,__LINE__);} while (false)
//#  endif
//#else
//#  define Q_CHECK_PTR(p) do { if (!(p)) qBadAlloc(); } while (false)
//#endif




namespace detail 
{
    template <class T>
    struct AlignOfHelper
    {
        char c;
        T type;
        AlignOfHelper();
        ~AlignOfHelper();
    };

    template <class T>
    struct AlignOf_Default
    {
        enum {Value=sizeof(AlignOfHelper<T>)-sizeof(T)};
    };

    template <class T> struct AlignOf : AlignOf_Default<T> { };
    template <class T> struct AlignOf<T &> : AlignOf<T> {};
    template <size_t N, class T> struct AlignOf<T[N]> : AlignOf<T> {};

#ifdef Q_COMPILER_RVALUE_REFS
    template <class T> struct AlignOf<T &&> : AlignOf<T> {};
#endif
    // GCC and Clang seem to disagree wrt to alignment of arrays
    template <size_t N> struct AlignOf<double[N]>   : AlignOf_Default<double> {};
    template <size_t N> struct AlignOf<int64_t[N]>   : AlignOf_Default<int64_t> {};
    template <size_t N> struct AlignOf<uint64_t[N]>  : AlignOf_Default<uint64_t> {};
}

// namespace QtPrivate


//#define QT_EMULATED_ALIGNOF(T) \
//    (size_t(QT_PREPEND_NAMESPACE(QtPrivate)::AlignOf<T>::Value))
//
//#ifndef Q_ALIGNOF
//#define Q_ALIGNOF(T) QT_EMULATED_ALIGNOF(T)
//#endif




//todo: from qglobal.h
#if 0
template <int> struct QIntegerForSize;
template <>    struct QIntegerForSize<1> { typedef uint8_t  Unsigned; typedef int8_t  Signed; };
template <>    struct QIntegerForSize<2> { typedef uint16_t Unsigned; typedef int16_t Signed; };
template <>    struct QIntegerForSize<4> { typedef uint32_t Unsigned; typedef int32_t Signed; };
template <>    struct QIntegerForSize<8> { typedef uint64_t Unsigned; typedef int64_t Signed; };

//#if defined(Q_CC_GNU) && defined(__SIZEOF_INT128__)
//template <>    struct QIntegerForSize<16> { __extension__ typedef unsigned __int128 Unsigned; __extension__ typedef __int128 Signed; };
//#endif

template <class T> struct QIntegerForSizeof: QIntegerForSize<sizeof(T)> { };
//typedef QIntegerForSize<Q_PROCESSOR_WORDSIZE>::Signed qregisterint;
//typedef QIntegerForSize<Q_PROCESSOR_WORDSIZE>::Unsigned qregisteruint;
typedef QIntegerForSizeof<void*>::Unsigned quintptr;
typedef QIntegerForSizeof<void*>::Signed qptrdiff;
typedef qptrdiff qintptr;
using qsizetype = QIntegerForSizeof<std::size_t>::Signed;


#endif



template <class T>
inline void printTypeSize()
{
    const unsigned int sizeBytes=sizeof(T);
    const double sizeMBytes=(double)sizeBytes/(1024*1024);
    ::printf("TypeInfo:sizeof(%s)=%u B=%.1f MB\n",typeid(T).name(),sizeBytes,sizeMBytes);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//参考自C++沉思录 改进对象跟踪统计，调试是作为某个类内部成员使用，调试时会自动跟踪打印对象个数。
//improve by lizulin 2017
template <class T> 
class TraceClass
{
public:
    TraceClass(const char* objname=""):m_name(objname)
    {
        currIndex=UsedCnt++;
        ObjCnt++;
        //类型名称GCC与VS不一致，还不如用宏
        ::printf("%s Construct() ObjCnt:%d curr:Index:%u name:%s\n",typeName(),ObjCnt.load(),currIndex,m_name);   
    }
    ~TraceClass()
    {
        ObjCnt--;
        ::printf("%s ~Destruct() ObjCnt:%d curr:Index:%u name:%s\n",typeName(),ObjCnt.load(),currIndex,m_name);   
    }
    const char* typeName()const {return typeid(T).name();}
    //static volatile int ObjCnt;
    //static volatile unsigned int UsedCnt;

    static volatile std::atomic<int> ObjCnt;                      //本类对象个数统计,多线程加锁考虑或用atomic
    static volatile std::atomic<unsigned int> UsedCnt;    //本类被使用次数(构造次数)统计,只增不减。
private:
    unsigned int currIndex;
    const char* m_name;  //可以构造处传入对象名称,便于追踪
};
template <class T> volatile std::atomic<int> TraceClass<T>::ObjCnt(0);
template <class T> volatile std::atomic<unsigned int> TraceClass<T>::UsedCnt(0);


//最终对象个数的同时,显示占用内容大小,仅适用于POD类型,非POD类型如何实现？
template <class T> 
class TraceClassMemPOD
{
public:
    TraceClassMemPOD()
    {
        ObjCnt++;
        printf("%s Construct() ObjCnt:%d memUsed:%llu\n",typeName(),ObjCnt.load(),memUsed());
    }
    ~TraceClassMemPOD()
    {
        ObjCnt--;
        printf("%s Construct()~ ObjCnt:%d memUsed:%llu\n",typeName(),ObjCnt.load(),memUsed());
    }
    const char* typeName()const {return typeid(T).name();}
    static size_t memUsed(){return ObjCnt.load()*sizeof(T);}
    //static volatile int ObjCnt;
    static volatile std::atomic<int> ObjCnt;
};
template <class T> 
volatile std::atomic<int> TraceClassMemPOD<T>::ObjCnt(0);



//使用方法
/*
class Ctest
{
    TraceClass<Ctest> test;   //作为某个类的内部成员变量，即可跟踪打印某个类的对象个数
};
*/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//任意字节对齐的内存申请
template<typename T,unsigned int alignByte>
class AlignedAllocator
{
public:
    inline static T* Malloc(size_t size)
    {
        //dbg_code(++counter);
        char *ptr=new char[size*sizeof(T)+alignByte+sizeof(int)];
        char *ptr2 =ptr+sizeof(int);
        char *aligned_ptr = ptr2+(alignByte-((size_t)ptr2&(alignByte-1)));
        ptr2=aligned_ptr-sizeof(int);
        *((int *)ptr2)=(int)(aligned_ptr-ptr);

        new(aligned_ptr) T;

        return(T*)(aligned_ptr);
    }

    inline static void Free(T* ptr)
    {
        int *ptr2=(int *)ptr - 1;
        char*const m=(char*)ptr - *ptr2;
        delete[] m;
    }
private:
    //STACK_ONLY;
    //ZTD_NO_COPYABLE(AlignedAllocator);
};




#if 0

//任意字节对齐的内存申请
template<typename T,unsigned int alignByte>
class AlignedAllocator
{
public:
    inline static T* Malloc(size_t size)
    {
        //dbg_code(++counter);
        char *ptr=new char[size*sizeof(T)+alignByte+sizeof(char*)];
        char *ptr2 =ptr+sizeof(int);
        char *aligned_ptr = ptr2+(alignByte-((size_t)ptr2&(alignByte-1)));
        ptr2=aligned_ptr-sizeof(int);
        *((char*)ptr2)=(aligned_ptr-ptr);

        //pData=operator new(pShare) T[BufferSize];

        new(aligned_ptr) T;

        return(T*)(aligned_ptr);
    }

    inline static void Free(T* ptr)
    {
        char *ptr2=(char*)ptr - 1;
        char*const m=(char*)ptr - *ptr2;
        delete[] m;
    }
private:
    //STACK_ONLY;
    //ZTD_NO_COPYABLE(AlignedAllocator);
};

#endif


//自定义pair 
template<class T1,class T2>
struct Pair
{
    Pair(T1 _first,T2 _second)
        :first(_first)
        ,second(_second)
    {}
    T1 first;
    T2 second;
};

//自定义make_pair
template<class T1,class T2>
Pair<T1, T2> Make_Pair(T1 first, T2 second)
{
    return Pair<T1, T2>(first, second);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//from funtional For learning use only by lizulin
#if 0
//一个一元函数对象的基类，只定义了参数和返回值的类型，不重载（）操作符，这个任务应该交由派生类去完成。
template<class _Arg,class _Result>
struct unary_function
{	// base class for unary functions
    typedef _Arg argument_type;
    typedef _Result result_type;
};

//一个二元函数对象的基类，只定义了参数和返回值的类型，不重载（）操作符，交由派生类去完成。
template<class _Arg1,class _Arg2,class _Result>
struct binary_function
{	// base class for binary functions
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};

template<class _Ty>
struct equal_to: public binary_function<_Ty, _Ty, bool>
{
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left == _Right);
    }
};

template<class _Ty> 
struct less: public binary_function<_Ty, _Ty, bool>
{
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left < _Right);
    }
};

template<class _Ty>
struct greater: public binary_function<_Ty, _Ty, bool>
{
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left > _Right);
    }
};


template<class _Ty>
struct less_equal: public binary_function<_Ty, _Ty, bool>
{
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left <= _Right);
    }
};

template<class _Ty>
struct greater_equal : public binary_function<_Ty, _Ty, bool>
{
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left >= _Right);
    }
};

template<class _Ty> 
struct plus: public binary_function<_Ty, _Ty, _Ty>
{
    _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left + _Right);
    }
};

template<class _Ty>
struct minus: public binary_function<_Ty, _Ty, _Ty>
{
    _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left - _Right);
    }
};


template<class _Ty>
struct multiplies: public binary_function<_Ty, _Ty, _Ty>
{
    _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left * _Right);
    }
};


template<class _Ty>
struct divides: public binary_function<_Ty, _Ty, _Ty>
{
    _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left / _Right);
    }
};


template<class _Ty>
struct modulus: public binary_function<_Ty, _Ty, _Ty>
{
    _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left % _Right);
    }
};


template<class _Ty>
struct negate: public unary_function<_Ty, _Ty>
{
    _Ty operator()(const _Ty& _Left) const
    {
        return (-_Left);
    }
};

template<class _Ty>
struct not_equal_to: public binary_function<_Ty, _Ty, bool>
{
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left != _Right);
    }
};


template<class _Ty>
struct logical_and: public binary_function<_Ty, _Ty, bool>
{
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left && _Right);
    }
};

template<class _Ty>
struct logical_or: public binary_function<_Ty, _Ty, bool>
{
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left || _Right);
    }
};

template<class _Ty>
struct logical_not: public unary_function<_Ty, bool>
{
    bool operator()(const _Ty& _Left) const
    {
        return (!_Left);
    }
};


template<class _Ty>
struct bit_and
    : public binary_function<_Ty, _Ty, _Ty>
{
    _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left & _Right);
    }
};


template<class _Ty>
struct bit_or
    : public binary_function<_Ty, _Ty, _Ty>
{
    _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left | _Right);
    }
};


template<class _Ty>
struct bit_xor
    : public binary_function<_Ty, _Ty, _Ty>
{
    _Ty operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return (_Left ^ _Right);
    }
};
#endif



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//std::addressof

//为什么需要 Addressof ？如果某个类重载了&,则取地址符失效。
//class CTest 
//{
//    CTest* operator &() {return 0;}
//};
//int test_fun() {
//    CTest a;
//    std::cout << &a << '\n';                // Print 0
//    std::cout << std::addressof(a);       // Prints it's actual address
//}
//template<typename T>
//constexpr T* addressof(T &arg) noexcept; // (2) 从C++17开始支持
//template <class T>
//const T* addressof(const T&&) = delete; // (3) 从C++17开始支持

template <typename T> 
inline T* Addressof (T& arg) 
{
    return reinterpret_cast<T*>(&reinterpret_cast<char&> (arg));
}

template <typename T> 
inline T const* Addressof (T const& arg) {
    return reinterpret_cast<T const*>(&reinterpret_cast<char const&> (arg));
}
////////////////////////////////////////////////////////////////////////////////


NAME_SPACE_MAIN_END


#endif
