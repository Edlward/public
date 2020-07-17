#pragma once
#include "./Basic/global.h" 

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//��̬��֮�乲��ȫ�ֱ����ķ���: 
//VC
//#pragma data_seg("Shared")
//int gShareVal=0;
//#pragma data_seg()
////#pragma comment(linker,"/SECTION:Shared,RWS")

//GCC
//int gShareVal __attribute__((section("Shared")) =0;  //����ʱΪ����/����ָ����


//
//#if defined(OS_WIN)
//	#define GLOBAL_SHARE_WIN_BEGIN  //#pragma data_seg("Shared")   //�˷�����
//	#define GLOBAL_SHARE_WIN_END   //#pragma data_seg() #pragma comment(linker,"/SECTION:Shared,RWS")
//#else
//	#define GLOBAL_SHARE_WIN_BEGIN   
//	#define GLOBAL_SHARE_WIN_END  
//#endif
//
//#if defined(OS_LINUX)
//  #define GLOBAL_SHARE_LINUX __attribute__((section("Shared"))
//#else
//  #define GLOBAL_SHARE_LINUX
//#endif
//ʹ�÷���:

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if 0 //
#if defined(Q_OS_WIN) && !defined(Q_CC_GNU)
#  define Q_INT64_C(c) c ## i64    /* signed 64 bit constant */
#  define Q_UINT64_C(c) c ## ui64   /* unsigned 64 bit constant */
typedef __int64 qint64;            /* 64 bit signed */
typedef unsigned __int64 quint64;  /* 64 bit unsigned */
#else
#  define Q_INT64_C(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#  define Q_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */
typedef long long qint64;           /* 64 bit signed */
typedef unsigned long long quint64; /* 64 bit unsigned */
#endif

#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#if 0
//����ں����д�ӡ�������ߵĺ������ƣ�
void caller_ONE()
{
    called();
}
void caller_TWO()
{
    called();
}

void called()
{
    //���֪��������������ĺ�������     
}
#define called printf(__MyFUNC__), called   //��ֻ�Ǹ�˼·������Ҫ����ʵ��ʹ��������е���

//#define PrintWhoCallMe(meFun) printf(__MyFUNC__);meFun  //�ں���������ʹ��,�Ľ�����,���������ڷǳ�Ա����
//
//int Funtest();
//PrintWhoCallMe(Funtest);   //˫�غ궨�� ����
#endif



////////////////////////////////////////////////////////////////////////////////
//#define MAX_UINT8   ((uint8_t)~((uint8_t) 0))
//#define MAX_UINT16 ((uint16_t)~((uint16_t)0))
//#define MAX_UINT32 ((uint32_t)~((uint32_t)0))
//#define MAX_UINT64 ((uint64_t)~((uint64_t)0))
//
//static_assert(MAX_UINT8==0xFF,"");
//static_assert(MAX_UINT16==0xFFFF,"");
//static_assert(MAX_UINT32==0xFFFFFFFF,"");
//static_assert(MAX_UINT64==0xFFFFFFFFFFFFFFFF,"");

//#define MAX_INT8    ((int8_t)(MAX_UINT8>>1))
//#define MAX_INT16  ((int16_t)(MAX_UINT16>>1))
//#define MAX_INT32  ((int32_t)(MAX_UINT32>>1))
//#define MAX_INT64  ((int64_t)(MAX_UINT64>>1))
//
//#define MIN_UINT8    (0)
//#define MIN_UINT16  (0)
//#define MIN_UINT32  (0)
//#define MIN_UINT64  (0ULL)
//#define MIN_INT8   ((int8_t)~MAX_INT8)
//#define MIN_INT16  ((int16_t)~MAX_INT16)
//#define MIN_INT32  ((int32_t)~MAX_INT32)
//#define MIN_INT64  ((int64_t)~MAX_INT64)


//by lizulin
//#ifndef INT16_MIN
//#define INT16_MIN (-32768)
//#endif
//#ifndef INT16_MAX
//#define INT16_MAX (32767)
//#endif
//
//#ifndef UINT16_MIN
//#define UINT16_MIN (0)
//#endif
//#ifndef UINT16_MAX
//#define UINT16_MAX (0XFFFF)
//#endif
//
//#ifndef INT32_MIN
//#define INT32_MIN (-2147483648)
//#endif
//#ifndef INT32_MAX
//#define INT32_MAX (2147483647)
//#endif
//
//#ifndef UINT32_MIN
//#define UINT32_MIN (0U)
//#endif
//#ifndef UINT32_MAX
//#define UINT32_MAX (0XFFFFFFFF)
//#endif
//
//#ifndef INT64_MIN
//#define INT64_MIN (-9223372036854775808LL)
//#endif
//#ifndef INT64_MAX
//#define INT64_MAX (9223372036854775807LL)
//#endif
//
//#ifndef UINT64_MIN
//#define UINT64_MIN (0ULL)
//#endif
//#ifndef UINT64_MAX
//#define UINT64_MAX (0XFFFFFFFFFFFFFFFFULL)
//#endif
//
//#ifndef SHRT_MIN
//#define SHRT_MIN INT16_MIN
//#endif
//
//#ifndef SHRT_MAX
//#define SHRT_MAX INT16_MAX
//#endif
//
//#ifndef USHRT_MAX
//#define USHRT_MAX UINT16_MAX
//#endif
//
//#ifndef INT_MIN
//#define INT_MIN INT32_MIN
//#endif
//
//#ifndef INT_MAX
//#define INT_MAX INT32_MAX
//#endif
//
//#ifndef UINT_MAX
//#define UINT_MAX UINT32_MAX
//#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if 0
#ifdef _MSC_VER
#include <intrin.h>
//msc����ʱʹ���ڽ�����(Compiler Intrinsics)
inline __int8 BSR_uint64(unsigned __int64 num)
{
    unsigned long index;
    return _BitScanReverse64(&index,num);
    return index;

    //return _BitScanReverse64(&index,num)?index:-1;  //old
}
inline __int8 BSR_uint32(unsigned __int32 num)
{
    unsigned long index;
    return _BitScanReverse(&index,num);
    return index;
    //return _BitScanReverse(&index,num)?index:-1;  //old
}

#elif __GNUC__
//gcc ����ʱʹ���ڽ�����(Built-in Functions)
inline __int8 BSR_uint64(unsigned __int64 num) {
    return num==0?-1:(sizeof(num)<<3)-1-__builtin_clzll(num);
    //clzll����64λ��ǰ��Ϊ0�ĸ���������1����63(ǰ����63��0)
}
inline __int8 BSR_uint32(unsigned __int32 num) {
    return num==0?-1:(sizeof(num)<<3)-1-__builtin_clz(num);
    //clz����32λ��ǰ��Ϊ0�ĸ���������2����30(ǰ����30��0)
}
#endif
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Defines the same pointer type (raw or smart) to another pointee type

template<class T, class U>
struct pointer_to_other;

template<class T, class U, 
    template<class> class Sp>
struct pointer_to_other< Sp<T>, U >
{
    typedef Sp<U> type;
};

template<class T, class T2, class U, 
    template<class, class> class Sp>
struct pointer_to_other< Sp<T, T2>, U >
{
    typedef Sp<U, T2> type;
};

template<class T, class T2, class T3, class U, 
    template<class, class, class> class Sp>
struct pointer_to_other< Sp<T, T2, T3>, U >
{
    typedef Sp<U, T2, T3> type;
};

template<class T, class U>
struct pointer_to_other< T*, U >
{
    typedef U* type;
};





////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



//static_pointer_cast overload for raw pointers
template<class T, class U>
inline T* static_pointer_cast(U *ptr) throw()
{  
    return static_cast<T*>(ptr);
}

//dynamic_pointer_cast overload for raw pointers
template<class T, class U>
inline T* dynamic_pointer_cast(U *ptr) throw()
{  
    return dynamic_cast<T*>(ptr);
}

//const_pointer_cast overload for raw pointers
template<class T, class U>
inline T* const_pointer_cast(U *ptr) throw()
{  
    return const_cast<T*>(ptr);
}

//reinterpret_pointer_cast overload for raw pointers
template<class T, class U>
inline T* reinterpret_pointer_cast(U *ptr) throw()
{  
    return reinterpret_cast<T*>(ptr);
}


#include <memory>

//static_pointer_cast overload for std::shared_ptr
using std::static_pointer_cast;

//dynamic_pointer_cast overload for std::shared_ptr
using std::dynamic_pointer_cast;

//const_pointer_cast overload for std::shared_ptr
using std::const_pointer_cast;

//reinterpret_pointer_cast overload for std::shared_ptr
template<class T, class U> std::shared_ptr<T> reinterpret_pointer_cast(const std::shared_ptr<U> & r )  throw()
{
    (void) reinterpret_cast< T* >( static_cast< U* >( 0 ) );

    typedef typename std::shared_ptr<T>::element_type E;

    E * p = reinterpret_cast< E* >( r.get() );
    return std::shared_ptr<T>( r, p );
}

//static_pointer_cast overload for std::unique_ptr
template<class T, class U> std::unique_ptr<T> static_pointer_cast( std::unique_ptr<U> && r ) throw()
{
    (void) static_cast< T* >( static_cast< U* >( 0 ) );

    typedef typename std::unique_ptr<T>::element_type E;

    return std::unique_ptr<T>( static_cast<E*>( r.release() ) );
}

//dynamic_pointer_cast overload for std::unique_ptr
template<class T, class U> std::unique_ptr<T> dynamic_pointer_cast( std::unique_ptr<U> && r ) throw()
{
    (void) dynamic_cast< T* >( static_cast<U*>(0));

    //BOOST_STATIC_ASSERT_MSG( boost::has_virtual_destructor<T>::value, "The target of dynamic_pointer_cast must have a virtual destructor." );

    T * p = dynamic_cast<T*>( r.get() );
    if( p ) r.release();
    return std::unique_ptr<T>( p );
}

//const_pointer_cast overload for std::unique_ptr
template<class T, class U> std::unique_ptr<T> const_pointer_cast( std::unique_ptr<U> && r ) throw()
{
    (void) const_cast< T* >( static_cast< U* >( 0 ) );

    typedef typename std::unique_ptr<T>::element_type E;

    return std::unique_ptr<T>( const_cast<E*>( r.release() ) );
}

//reinterpret_pointer_cast overload for std::unique_ptr
template<class T, class U> std::unique_ptr<T> reinterpret_pointer_cast( std::unique_ptr<U> && r ) throw()
{
    (void) reinterpret_cast< T* >( static_cast< U* >( 0 ) );

    typedef typename std::unique_ptr<T>::element_type E;

    return std::unique_ptr<T>(reinterpret_cast<E*>(r.release()));
}






////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//from boost  polymorphic_pointer_cast

//1 dynamic_cast��ָ������ת��ʧ�ܻ᷵�ؿգ�������ת��ʧ�ܻ��׳��쳣����polymorphic_castͳһ�������ֲ�һ�µ���Ϊ��polymorphic_cast�ᶼ�׳��쳣
//
//    2 polymorphic_cast�ĺô��Ǳ�����Ϊ����������ǲ���ת�����ָ���Ƿ�Ϊ�գ���Ϊ���׳��쳣
//
//    3 ����̬ת�͵�ʧ����Ԥ�ڵ�ʱ��ʹ�� dynamic_cast<T*>����һ����̬ת�ͱ���ɹ���ȷ���߼�����ȷ��ʱ��ʹ��polymorphic_cast<T*>.
//
//    4 ����������ִ�ж�̬ת��ʱ��ʹ�� dynamic_cast<T*>

template <class Target, class Source>
inline Target polymorphic_cast(Source* x)
{
    Target tmp = dynamic_cast<Target>(x);
    if ( tmp == 0 ) throw std::bad_cast() ;
    return tmp;
}

#include <assert.h>
template <class Target, class Source>
inline Target polymorphic_downcast(Source* x)
{
    assert( dynamic_cast<Target>(x) == x );  // detect logic error
    return static_cast<Target>(x);
}