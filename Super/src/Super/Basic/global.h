#ifndef _GLOBAL_Header_
#define _GLOBAL_Header_

//#include "Super/Basic/global.h"
//global common
//定义一个全局包含的头文件,定义一些常用宏,平台控制/轻量级数学函数/模板工具等,便于调试,不可引入太多头文件

//#pragma pack(push, 1)
//#pragma pack(pop)

//Instructions
//#include <stdio.h>   //for printf putchar getchar,removed 
//#include <stdlib.h>  //remove
#include <cstdint>
//#include <stddef.h>
//#include <limits.h>  
//#include <typeinfo> 
#include "define.h"
//#include <utility>  //std::swap to be remove

#ifndef _WIN32
#include <stddef.h> //for ptrdiff_t at gcc
#endif


//from stdlib.h 
#ifndef NULL
    #ifdef __cplusplus
        #define NULL  0
    #else
        #define NULL  ((void *)0)
    #endif
#endif


typedef char  int8;
typedef short int16;
typedef int     int32;
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int      uint32;

////C99
typedef long long int64;
typedef unsigned long long uint64;

//For _WIN
//typedef unsigned __int64 uint64;
//typedef __int64          int64;

//For _LINUX
//typedef long   long    int64;
//typedef unsigned long  long  uint64;


//再定义几种种常用的
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef double  qreal;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Macros that control whether use costom nameapace 
//全局命名空间开始与结束宏--使用者可以自定义本库命名空间，create by lizulin
//eg. #define SPACE_NAME myLib

//#ifdef SPACE_NAME
//    #define NAME_SPACE_MAIN_BEGIN namespace SPACE_NAME{
//    #define NAME_SPACE_MAIN_END }
//    #define USING_NAME_SPACE_MAIN using namespace SPACE_NAME;
//#else
//    #define NAME_SPACE_MAIN_BEGIN namespace Super{
//    #define NAME_SPACE_MAIN_END }
//    #define USING_NAME_SPACE_MAIN using namespace Super;
//#endif


//old
//#define NAME_SPACE_MAIN_BEGIN namespace Super{
//#define NAME_SPACE_MAIN_END }
//#define USING_NAME_SPACE_MAIN using namespace Super;

#define NAME_SPACE_MAIN_BEGIN 
#define NAME_SPACE_MAIN_END 
#define USING_NAME_SPACE_MAIN 

////////////////////////////////////////////////////////////////////////////////
//Macros that control whether compile unit test code
//控制是否编译内部单元自测代码,可选项,可以减小库的大小
#ifndef DISABLE_UNIT_TEST
#define UNIT_TEST 
#endif
////////////////////////////////////////////////////////////////////////////////
//Macros about version
#define SP_VERSION_STR "1.0.0"
//VERSION is (major<<16)+(minor<<8)+patch.
//can be used like #if (SP_VERSION>=SP_VERSION_CHECK(1,5,0))

#define SP_VERSION 0x010000
#define SP_VERSION_CHECK(major,minor,patch) ((major<<16)|(minor<<8)|(patch))
#define SP_PACKAGEDATE_STR "2015-05-01"
#define SP_PACKAGE_TAG "445d29dbb8135944ff3a9283b7622de75d68e747"

////////////////////////////////////////////////////////////////////////////////
//TODO:constexpr  support 
# if defined(__cpp_constexpr) && __cpp_constexpr-0 >= 201304
#  define Q_DECL_CONSTEXPR constexpr
# else
# define Q_DECL_CONSTEXPR
#endif
////////////////////////////////////////////////////////////////////////////////
//disable copy class
#define S_DISABLE_COPY(ClassName) \
    private: \
    ClassName(const ClassName&); \
    ClassName& operator=(const ClassName&);public:\

//disable use heap objects   //CN.禁止在堆生成对象
#define DISABLE_HEAP_ALLOC\
   private: \
   static void* operator new (size_t);\
   static void operator delete (void*);public:\

//from qt
//简化pimpl访问
#define S_DECLARE_PRIVATE(Class)\
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(qGetPtrHelper(d_ptr)); }\
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(qGetPtrHelpe(d_ptr));}\
    friend class Class##Private;  

//可以用Q_D和Q_Q两个宏来进一步简化访问：
#define S_D(Class) Class##Private * const d = d_func()
#define S_Q(Class) Class * const q = q_func()
////////////////////////////////////////////////////////////////////////////////

//检测系统是32位，还是64位：只可能检测编译环境，不可能检测机器
//#define NVM_BITS   "[%d bit] ", (sizeof(void*) == 8 ? 64 : 32)

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//Cross-platform byte alignment //跨平台字节对齐
//#define SP_ALIGN(bytes) alignas(bytes)   //c++11 todo
#ifndef SP_ALIGN   
#ifdef _WIN32
#define SP_ALIGN(bytes)   __declspec (align(bytes))
#else
#define SP_ALIGN(bytes)   __attribute__ ((aligned(bytes)))
#endif
#endif
////////////////////////////////////////////////////////////////////////////////
//强制内联 //force inline
#ifdef _WIN32
#define forceinline __forceinline
#else
#define forceinline inline __attribute__((always_inline))
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Common tools

//get array num

#ifndef ARRAY_NUM
#define ARRAY_NUM(a) (sizeof(a)/sizeof(a[0]))
#else
#warning  "Macro definition conflict at: ARRAY_NUM"
#endif


template<class T,size_t N>
inline size_t ArrayNum(T (&arry)[N])
{
    (void) arry;
    (void) sizeof (0[arry]); // This line should cause an error if you pass an object with a user-defined subscript operator
    return N;
}

template<class T,size_t N>
inline size_t ArraySize(T (&arry)[N])
{
    return N*sizeof(T);
}

//Safely delete pointer  //安全删除指针 by lizulin
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) do{ if(p) { delete (p); (p)=0;}}while(0) 
#else
#warning  "Macro definition conflict at: SAFE_DELETE"
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) do{ if(p) { delete[] (p);(p)=0;}}while(0)
#else
#warning  "Macro definition conflict at: SAFE_DELETE_ARRAY"
#endif

//Implementation based on templates
template<class T>
inline void SafeDelete(T*& p){do{ if(p) { delete (p); (p)=0;}}while(0);}
template<class T>
inline void SafeDeleteArray(T*& p){do{ if(p) { delete[] (p);(p)=0;}}while(0);}
////////////////////////////////////////////////////////////////////////////////



//来自juce 可以参考或取类名，继续扩展类对象计数，检测内存泄漏
/*
#define JUCE_LEAK_DETECTOR(OwnerClass) \
    friend class juce::LeakedObjectDetector<OwnerClass>; \
    static const char* getLeakedObjectClassName() noexcept { return #OwnerClass; } \
    juce::LeakedObjectDetector<OwnerClass> JUCE_JOIN_MACRO (leakDetector, __LINE__);
#else
#define JUCE_LEAK_DETECTOR(OwnerClass)
#endif
*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define load8(p)       (*(const uint8_t*) (p))
#define load16(p)     (*(const uint16_t*)(p))
#define load32(p)     (*(const uint32_t*)(p))
#define load64(p)     (*(const uint64_t*)(p))
#define store8(p, v)  (*(uint8_t*) (p) = (v))
#define store16(p, v) (*(uint16_t*)(p) = (v))
#define store32(p, v) (*(uint32_t*)(p) = (v))
#define store64(p, v) (*(uint64_t*)(p) = (v))




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//自定义assert 待续
#if 0
 
#define MY_ENABLE_ASSERT

void assert_fail(const char* file, int line, const char* func,
                 const char* expr, const std::string& msg) {
                     (void)file;
                     (void)line;
                     (void)func;
                     (void)expr;
                     (void)msg;
                     std::string err_msg = std::string("Assertion fails:") + expr + ", message:" + msg +
                         ", at " + file + ":" + std::to_string(line) + ":" + func;
                     std::cout << err_msg << std::endl;
                     std::terminate();
}

#ifdef MY_ENABLE_ASSERT
#define MY_ASSERT(expr)                                                     \
    do {                                                                    \
    if(!(expr)) {                                                       \
    assert_fail(__FILE__, __LINE__, __func__, #expr, "");           \
    }                                                                   \
    } while(0)

#define MY_ASSERT_EQ(a, b)                                                 \
    do {                                                                   \
    using std::to_string;                                              \
    if ((a) != (b)) {                                                  \
    assert_fail(__FILE__, __LINE__, __func__, #a " == " #b,        \
    std::to_string(a) + " != " + std::to_string(b));   \
    }                                                                  \
    } while (0)

#define MY_ASSERT_NE(a, b)                                                 \
    do {                                                                   \
    using std::to_string;                                              \
    if ((a) == (b)) {                                                  \
    assert_fail(__FILE__, __LINE__, __func__, #a " == " #b,        \
    std::to_string(a) + " != " + std::to_string(b));   \
    }                                                                  \
    } while (0)
#else
#define MY_ASSERT(expr) (void)0
#define MY_ASSERT_EQ(expr) (void)0
#define MY_ASSERT_NE(expr) (void)0
#endif

int main(int argc, char** argv) {
    int data[5] = {1,2,3,4,5};
    // int index = 5;
    // MY_ASSERT(index < 5);
    // std::cout << data[index] << std::endl;
    // MY_ASSERT_EQ(3,3);
    MY_ASSERT_NE(2,2);
}

#endif


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//c-library function extern declaration

//本文件多数情况被各.h全局包含,为了加快编译,尽量不要直接包含太多头文件(否则明显拖慢编译速度)
//通过前置声明或自定义中转函数,在cpp里面实现,尤其是高频基础件的模板函数不要直接包含头文件,使用以下封装中转。

//sleep Very commonly used, here is also implemented sleep非常常用在此也实现一份 
extern void SleepMs(unsigned int milliseconds);   //sleep for millseconds
extern void SleepFor(double seconds);                  //sleep for seconds

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    struct	_iobuf;
    typedef	_iobuf FILE;
    #define NO_RETURN_FUN_BEGIN __declspec(noreturn)      //not return function Modifier macro //create by lizulin
    #define NO_RETURN_FUN_END
#else
    struct _IO_FILE;
    typedef struct _IO_FILE FILE;
    #define __cdecl
    #define NO_RETURN_FUN_BEGIN
    #define NO_RETURN_FUN_END __attribute__((noreturn))

#endif

#ifndef _WIN32
    #include <stdio.h>     //gcc to do
    #include <string.h>
    #include <math.h>

//linux TODO:
//#include <sys/cdefs.h>
//extern void *memcpy (void *__restrict __dest, const void *__restrict __src,
//             size_t __n) __THROW __nonnull ((1, 2));

#else
    extern int __cdecl printf(const char* format,...);
    extern int __cdecl sprintf(char * buf, const char *format,...);
    extern int __cdecl fprintf(FILE* fp,const char *format,...);
    extern int __cdecl putchar(int ch);
    extern int __cdecl getchar();
    extern size_t __cdecl strlen(const char* Str);
    extern char* __cdecl strcpy(char* dst,const char* src);
    extern char* __cdecl strncpy(char *dst, const char *src, size_t len);
    extern void* __cdecl memcpy(void* dst,const void* src,size_t len);
    extern void* __cdecl memset(void* dst,int val,size_t len);
    extern void* __cdecl memmove(void* dst, const void* src,size_t len);
    extern int __cdecl memcmp(const void *buf1, const void *buf2, size_t len);
    extern char* __cdecl strcat(char *dst, const char *src);
    extern char* __cdecl strncat(char *dst, const char *src,size_t len);
    extern int __cdecl strcmp(const char *str1, const char *str2);
    extern int __cdecl strncmp(const char *str1, const char *str2,size_t len);
    extern const char* __cdecl strstr(const char *str, const char *substr);
    extern void* __cdecl malloc(size_t size);
    extern void __cdecl free(void *p);
    extern void* __cdecl realloc(void *ptr,size_t size);

    extern NO_RETURN_FUN_BEGIN void __cdecl exit(int _Code)NO_RETURN_FUN_END;
    extern NO_RETURN_FUN_BEGIN void __cdecl _exit(int _Code)NO_RETURN_FUN_END;
    extern NO_RETURN_FUN_BEGIN void __cdecl abort(void)NO_RETURN_FUN_END;

    extern void __cdecl abort(void);
    extern int __cdecl atexit(void (*func)(void));
    extern int __cdecl system(const char *strCmd);
    extern char*__cdecl getenv(const char *varName);

    extern FILE* __cdecl fopen(const char * _Filename,const char * _Mode);
    extern FILE*__cdecl freopen (const char* filename,const char* modes,FILE* stream);
    //extern int __cdecl fsetpos(FILE * _File,const fpos_t * _Pos);
    extern int __cdecl fseek(FILE * _File,long _Offset,int _Origin);
    extern long __cdecl ftell(FILE * _File);
    extern size_t __cdecl fwrite(const void * _Str,size_t _Size,size_t _Count,FILE * _File);
    extern size_t __cdecl fread(void * _DstBuf,size_t _ElementSize,size_t _Count,FILE * _File);
    extern int __cdecl fclose(FILE * _File);
    extern int __cdecl feof(FILE * _File);
    extern int __cdecl ferror(FILE * _File);
    extern int __cdecl fflush(FILE * _File);
    extern int __cdecl fgetc(FILE * _File);
    extern int __cdecl _fgetchar(void);
    //extern int __cdecl fgetpos(FILE * _File ,fpos_t * _Pos);
    extern char * __cdecl fgets(char * _Buf, int _MaxCount, FILE * _File);
    extern int __cdecl fputc(int _Ch, FILE * _File);
    extern int __cdecl fputs(const char * _Str, FILE * _File);
    extern int __cdecl remove(const char * _Filename);
    extern int __cdecl rename(const char * _OldFilename, const char * _NewFilename);
    extern int __cdecl _unlink(const char * _Filename);
    extern int __cdecl scanf(const char * _Format, ...);
    extern int __cdecl sscanf(const char * _Src,const char * _Format, ...);


    //#include <cmath>
    extern void   __cdecl srand(unsigned int _Seed);
    extern int     __cdecl abs(int _X);
    extern long    __cdecl labs(long _X);
    extern long long __cdecl llabs(long long _X);
    extern double  __cdecl cos(double _X);
    extern double  __cdecl cosh(double _X);
    extern double  __cdecl exp(double _X);
    extern double  __cdecl fabs(double _X);
    extern double  __cdecl fmod(double _X, double _Y);
    extern double  __cdecl log(double _X);
    extern double  __cdecl log10(double _X);
    extern double  __cdecl pow(double _X, double _Y);
    extern double  __cdecl sin(double _X);
    extern double  __cdecl sinh(double _X);
    extern double  __cdecl tan(double _X);
    extern double  __cdecl tanh(double _X);
    extern double  __cdecl ceil(double _X);
    extern double  __cdecl floor(double _X);
    
#endif

#ifdef __cplusplus
  } //end extern "C" 
#endif 
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
extern void* Malloc_Aligned(size_t memSize,const int alignBytes=32);   //任意字节对齐版本 by lizulin
extern void Free_Aligned(void* ptr);

extern void* __cdecl malloc_aligned(size_t size, size_t alignment);
extern void* __cdecl realloc_aligned(void *oldptr, size_t newsize, size_t oldsize, size_t alignment);
extern void __cdecl free_aligned(void *ptr);

inline void sp_noop(void){}
void sp_assert(const char *assertion, const char *file, int line);
void sp_assert_x(const char *where, const char *what, const char *file, int line);  //TODO:


#if !defined(S_ASSERT)
#  if defined(S_NO_DEBUG) && !defined(S_FORCE_ASSERTS)
#    define S_ASSERT(cond) do { } while ((false) && (cond))
#  else
#    define S_ASSERT(cond) ((!(cond)) ? sp_assert(#cond,__FILE__,__LINE__) : sp_noop())
#  endif
#endif


#if !defined(S_ASSERT_X)
#  if defined(S_NO_DEBUG) && !defined(S_FORCE_ASSERTS)
#    define S_ASSERT_X(cond, where, what) do { } while ((false) && (cond))
#  else
#    define S_ASSERT_X(cond, where, what) ((!(cond)) ? sp_assert_x(where, what,__FILE__,__LINE__) : sp_noop())
#  endif
#endif

#define S_STATIC_ASSERT(Condition) static_assert(bool(Condition), #Condition)



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//disable warning at VS compiler //Win下VS编译器屏蔽libc不安全版本编译警告
#ifdef _WIN32

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

#endif


//函数名的宏,gcc 下__FUNCTION__类成员函数不带类名,定义一个通用的自定义实现

//#if defined (__GNUC__)
//#  define __MyFUNC__     ((const char*)(__PRETTY_FUNCTION__))
//#elif defined (__STDC_VERSION__)&&__STDC_VERSION__>=19901L
//#  define __MyFUNC__     ((const char*)(__func__))
//#else
//#  define __MyFUNC__     ((const char*)(__FUNCTION__))
//#endif

#if defined (__GNUC__)
#  define __MyFUNC__     __PRETTY_FUNCTION__
#elif defined (__STDC_VERSION__)&&__STDC_VERSION__>=19901L
#  define __MyFUNC__     __func__
#else
#  define __MyFUNC__     __FUNCTION__
#endif



//gcc warning: invalid suffix on literal; C++11 requires a space between literal and string macro
//需要宏前后留出空格 for example 
//#define PRINT(fmt, ...) printf("[" __FILE__ "] [%s] %d ",__MyFUNC__,__LINE__ ); printf(fmt,##__VA_ARGS__);


////////////////////////////////////////////////////////////////////////////////
#define __DEBUG__        //控制是否开启打印信息

//自定义打印宏，在打印信息之前先打印函数名，行号 文件名等
//debug helper //各种辅助调试打印宏
#ifdef  __DEBUG__

    #define PRINTF(fmt,...) ::printf(fmt,##__VA_ARGS__);

    //先打印函数名前缀prefix debug printf
    #define PreDprintf(fmt,...) ::printf("%s() " fmt,__MyFUNC__,##__VA_ARGS__); 

    #define TracePrint(fmt,...)  ::printf("%s()" fmt,__MyFUNC__,##__VA_ARGS__);
    #define TracePrintDetail(fmt,...)  ::printf("Info[%s|%s|%d]\t"fmt"\n",__FILE__,__MyFUNC__,__LINE__,##__VA_ARGS__);
    
     #define PRINT_FUN_NAME ::printf("%s() \n",__MyFUNC__);

    //打印运行到的文件名，行号，函数名。 
    #define PRINT_TRACE ::printf("[RunTo:]Fun:%s() File:%s Line:%04d\n",__MyFUNC__,__FILE__,__LINE__);

    //函数打桩测试，函数入口处用PRINT_TRACE_BEGIN函数结尾用PRINT_TRACE_END。
    #define PRINT_FUN_BEGIN ::printf("%s()--Begin\n",__MyFUNC__);
    #define PRINT_FUN_END ::printf("%s()--End\n",__MyFUNC__);

    #define PRINT_FUN_FAILED ::printf("%s() failed! File:%s Line:%04d\n",__MyFUNC__,__FILE__,__LINE__);

    #define PRINT_ERROR_LINE ::printf("[Error]:Fun:%s() File:%s Line:%04d\n",__MyFUNC__,__FILE__,__LINE__);
    #define PRINT_MAY_ERROR_LINE ::printf("[MayError]:Fun:%s() File:%s Line:%04d\n",__MyFUNC__,__FILE__,__LINE__);

#else

    #define PRINTF(fmt,...)
    #define PreDprintf(fmt,...)
    #define TracePrint(fmt,...)
    #define TracePrintDetail(fmt,...)
    #define PRINT_FUN_NAME  
    #define PRINT_TRACE  
    #define PRINT_FUN_BEGIN  
    #define PRINT_FUN_END  
    #define PRINT_FUN_FAILED
    #define PRINT_ERROR_LINE
    #define PRINT_MAY_ERROR_LINE
#endif


//注册或反注册
extern void RegisterFunTraceKey(const char* name);
extern void UnRegisterFunTraceKey(const char* name);
extern void PrintFunTrace();

//作为全局对象使用,退出时打印函数跟踪,因为全局析构不确定性,可以多处例化
class TraceFunExitPrint
{
public:
    TraceFunExitPrint(){}
    ~TraceFunExitPrint()
    {
//        PrintFunTrace();
    }
};
//static TraceFunExitPrint gTraceFEP;

class TraceFunction
{
public:
    TraceFunction(const char* fun_name):m_FunName(fun_name)
    {
        ::printf("%s()--Begin\n",m_FunName);     //进入函数打印
        //RegisterFunTraceKey(m_FunName);
    }
    ~TraceFunction()
    {
        ::printf("%s()--End\n",m_FunName);       //离开函数打印
        //UnRegisterFunTraceKey(m_FunName);
    }
private:
    const char* m_FunName;
    //可以增加函数进出时间判断，有空待续
};

//打印函数进出跟踪，如果只打印begin,没有end,说明此函数出问题了。
#ifdef  __DEBUG__
    #define PRINT_FUN_TRACE  TraceFunction traceFun(__MyFUNC__);
#else
    #define PRINT_FUN_TRACE  
#endif

//template <typename T>
//inline printNewError()
//{
//    ::printf("");
//};

//print new[] error
#ifdef  __DEBUG__
#define PRINT_NEW_ERROR ::printf("[NewError]:Fun:%s() File:%s Line:%04d\n",__MyFUNC__,__FILE__,__LINE__);
#else
#define PRINT_NEW_ERROR  
#endif



////////////////////////////////////////////////////////////////////////////////
#ifndef SP_PlatformDef_H
#define SP_PlatformDef_H
//platform define

#if defined(_WIN32)&&!defined(OS_WIN32)
#define OS_WIN32
#endif

#if defined(_WIN64)&&!defined(OS_WIN64)
#define OS_WIN64
#endif

#if defined(OS_WIN32)||defined(OS_WIN64)
#   define OS_WINDOWS
#   define OS_WIN
#endif

#if defined(__ANDROID__)||defined(ANDROID)
#  define OS_ANDROID
#  define OS_LINUX
#endif

#if defined(__APPLE__)
#  define OS_MACOS
#endif


#if defined(__linux__)||defined(__linux)
#define OS_LINUX
#endif

//arm platform TODO:
#if defined(__arm__)||defined(__arm64__)||defined(_ARM)||defined(_M_ARM)
||defined(__ARM_ARCH_7__)||defined(__ARM_ARCH_7A__)||defined(__ARM_ARCH_7R__)
    ||defined(__ARM_ARCH_7M__) ||defined(__ARM_ARCH_7S__)

#define OS_ARM
#define OS_ARM_LINUX
#endif


#if defined(OS_WIN)
#  undef OS_UNIX
#elif !defined(OS_UNIX)
#  define OS_UNIX
#endif

//跨平台判断方法
#if defined(__linux__)||defined(__linux)  
#  define OS_LINUX  
#  define OS_UNIX  
#endif

#if defined(_WIN32)||defined(_WIN64)
#  define OS_WIN
#endif

#endif  //SP_PlatformDef_H

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//compiler define

//Macro Determine the byte order of the current platform is Big Endian or Little Endian
//目标平台大小端静态决议,详见ByteOrder.h
//TODO: 

#define IS_BIG_ENDIAN       4321
#define IS_LITTLE_ENDIAN   1234

#ifdef __linux__
#   include <endian.h>
#   define PLATFORM_BYTE_ORDER  __BYTE_ORDER
#else /* __linux__ */
#   if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#       define PLATFORM_BYTE_ORDER   IS_BIG_ENDIAN
#   else
#       define PLATFORM_BYTE_ORDER   IS_LITTLE_ENDIAN
#   endif
#endif /* __linux__ */
//#endif



//error
//#if '\x01\x02\x03\x04'==0x01020304
//// native endian is big-endian
//#define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
//#elif '\x01\x02\x03\x04' == 0x04030201
//// native endian is little-endian
//#define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
//#else
//#error "WTF? What endian do I meet?"
//#endif

enum MByteOrder
{
    E_BigEndian=4321,
    E_LittleEndian=1234,
#if PLATFORM_BYTE_ORDER==IS_LITTLE_ENDIAN
    E_PlatFormEndian=E_LittleEndian,
#else
    E_PlatFormEndian=E_BigEndian,
#endif
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//DLL export/import define
//可以兼容dll创建者/dll使用者/普通文件包含的跨平台导入导出设定--create by lizulin
//dll创建者项目预定义BASIC_API_EXPORT 
//dll使用者项目预定义BASIC_API_IMPORT
//作为普通文件包含使用，不需要预定义

#ifdef _WIN32
    #define EXPORTS_DLL __declspec(dllexport)
    #define IMPORTS_DLL __declspec(dllimport)
#else
    #define EXPORTS_DLL __attribute__((dllexport))
    #define IMPORTS_DLL __attribute__((dllimport))
#endif

//basic module export/import define
#if defined(BASIC_API_EXPORT)
    #define BASIC_API EXPORTS_DLL
#elif defined(BASIC_API_IMPORT)
    #define BASIC_API IMPORTS_DLL
#else
    #define BASIC_API 
#endif

//net module export/import define
#if defined(NET_API_EXPORT)
#define NET_API EXPORTS_DLL
#elif defined(NET_API_IMPORT)
#define NET_API IMPORTS_DLL
#else
#define NET_API 
#endif

//TODO:other module 

////////////////////////////////////////////////////////////////////////////////
//Common tools, put here //CN 极其常用的工具类放此处
//Inherit this class to implement disabled copy class //CN 继承此类,可以禁用拷贝
class nonCopyable
{
protected:
    nonCopyable(){}
    ~nonCopyable(){}
private:
    nonCopyable(const nonCopyable&);
    nonCopyable& operator=(const nonCopyable&);
};


//继承此类可以禁用new 的方式在堆构造对象
class nonHeapObject
{
protected:
    nonHeapObject(){}
    ~nonHeapObject(){}
private:  
    void* operator new(size_t t);     // 注意函数的第一个参数和返回值都是固定的
    void operator delete(void* ptr);  // 重载了new就需要重载delete
};

//继承此类可以禁止在栈中例化对象，只能new,一般用在大结构体，禁用栈对象，避免栈溢出
//TODO:
class nonStackObject
{
public:
    nonStackObject(){}
protected:
    ~nonStackObject(){}
};

#if 0
//For example:
class NoStackObject 
{ 
protected: 
    NoStackObject() { } 
    ~NoStackObject() { } 
public: 
    static NoStackObject* creatInstance() 
    {
        return new NoStackObject() ;//调用保护的构造函数 
    } 
    void destroy() 
    {
        delete this ;//调用保护的析构函数 
    } 
};

//现在可以这样使用NoStackObject类了： 
NoStackObject* hash_ptr = NoStackObject::creatInstance() ; 
hash_ptr->destroy() ; 
hash_ptr = NULL ; //防止使用悬挂指针
#endif


////////////////////////////////////////////////////////////////////////////////
//智能数组,当空间比较小时用定长内部空间，避免new,根据需求会自动增长空间,类似于预设容量的vector
//POD类型拷贝，原地构造析构等优化待续,暂时只考虑POD类型

//resize（n）与容器的size有关，而且n个元素都初始化了。
//reserve（n)与容器的capacity有关，而且没有初始化，不能访问。

static const float ContainerSpaceExpansionFactor=1.5;   //容量扩展系数,VS2012不能放类内
template <class T,size_t  inSize=128/sizeof(T)>   //默认小于一定值用内部空间
class SArray
{
    //避免0字节数组
    static const size_t internalSize=(inSize==0)?1:inSize*sizeof(T);
    //T m_data[internalNum];
    char m_data[internalSize];
private:
    typedef SArray<T,inSize> _Myt;
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
public:
    T *data(){return (pData);}
    const T *data()const {return (pData);}
    reference front(){return (pData[0]);}
    const_reference front() const{ return (pData[0]);}
    reference back(){return (pData[m_size-1]);}
    const_reference back() const{return (pData[m_size - 1]);}
    reference at(size_t _Pos)
    {
        if(_Pos>=m_size){return 0;}   ////_Xran();
        return (pData[_Pos]);
    }
    const_reference at(size_t _Pos) const
    {
        if(_Pos>=m_size){return 0;}   ////_Xran();
        return (pData[_Pos]);
    }
    reference operator[](size_t _Pos){return (pData[_Pos]);}
    const_reference operator[](size_t _Pos) const{return (pData[_Pos]);}
public:
    explicit SArray(size_t nCapacity=1):m_size(0)
    {
        m_capacity=(nCapacity==0)?1:nCapacity;   //容量不允许为0
        if (m_capacity*sizeof(T)<=internalSize)  //为了效率容量需求较小时使用内置空间，不new
        {
            pData=(T*)m_data;
        }
        else
        {
            pData=new T[m_capacity];
        }
    }
    //SArray(size_t _Count, const value_type& _Val)
    //{
    //    _Construct_n(_Count, _STD addressof(_Val));
    //}
    ~SArray()
    {
        if (pData!=(T*)m_data)  //若为new 空间
        {
            delete[] pData;
            pData=(T*)m_data;
        }
    }
    void reserve(size_t nCapacity)
    {
        //拷贝旧数据到内部空间
        if (m_capacity*sizeof(T)<=internalSize)  //inSize
        {
            if (pData!=(T*)m_data)
            {
                for (size_t k=0;k<m_size;k++)  //拷贝数据
                {
                    ((T*)m_data)[k]=pData[k];
                }
                delete[] pData;
            }
            pData=(T*)m_data;
            m_capacity=internalSize/sizeof(T);   //   用内部空间时的容量则算
            return;
        }

        if (nCapacity>m_capacity)
        {
            T* pNew=new T[nCapacity];
             //memcpy(pNew,pData,m_size*sizeof(T));
            for (size_t k=0;k<m_size;k++)
            {
                pNew[k]=pData[k];
            }
            if ((void*)pData!=(void*)m_data)  //若为new 空间
            {
                delete[] pData;
            }
            pData=pNew;
            m_capacity=nCapacity;
        }
        else
        {
             //用new空间情况容量需要缩小时。不操作
        }
    }
    void resize(size_t nSize)
    {
        if (m_capacity<nSize)
        {
            reserve(nSize);
            //for (size_t k=m_size;k<nSize;k++)
            //{
            //    push_back(T());   //填充空数据
            //}
            m_size=nSize;
        }
    }
    void shrink_to_fit()   //缩小容量到刚好合适
    {
        //使用内部空间
        if (m_capacity*sizeof(T)<=internalSize)  //inSize
        {
            return;
        }
        T* pNew=new T[m_size];
        //memcpy(pNew,pData,m_size*sizeof(T));
        for (size_t k=0;k<m_size;k++)
        {
            pNew[k]=pData[k];  //
        }
        pData=pNew;
        m_capacity=m_size;
    }
    size_t size()const {return m_size;}
    size_t capacity()const {return m_capacity;}
    bool empty()const{return m_size==0;}
    bool push_backIfCan(const T& val)
    {
        if (m_size>(m_capacity-1)){return false;}
        pData[m_size++]=val;return true;
    }
    void push_back(const T& val)
    {
        if (m_size>(m_capacity-1))
        {
            reserve((m_capacity+1)*ContainerSpaceExpansionFactor);  //避免m_capacity=1实际不扩充，+1
        }
        pData[m_size++]=val;
    }
    void push_back(const T* arry,size_t num)
    {
        size_t newSize=m_size+num;
        if (newSize>(m_capacity-1))
        {
            reserve(newSize*ContainerSpaceExpansionFactor);  //容量按1.5倍扩展
    }
        const T* pSrc=arry;
        const T* pDst=pData+m_size;
        for (size_t k=0;k<num;k++)
    {
            *(pDst++)=*(pSrc++);
    }
        m_size=newSize;
    }
    template <size_t N>
    void push_back(const T (&arry)[N])
    {
        push_back(arry,N);
    }
    void pop_back()  //移除尾部
    {
        if (!empty())
        {
            --m_size;
        }
    }
    T pop(bool* ok=0)
    {
        if (m_size>0)
        {
            if (ok)
            {
                *ok=true;
            }
            m_size--;
            return pData[m_size];
        }
        if (ok)
        {
            *ok=false;   //或抛异常，待续
        }
        return T();
    }
    void clear(){m_size=0;}
private:
    T* pData;
    size_t m_capacity;
    size_t m_size;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//定长数组 容量不会增长 实现pushback.clear()  一下情况替代vector用于小容量数据类型管理，
template <class T,size_t Capacity=1>
class FixedArray
{
public:
    enum{ARRAY_SIZE=(Capacity == 0?1:Capacity)};
    typedef FixedArray<T,Capacity> _Myt;
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
public:
    FixedArray():m_Size(0){}
    ~FixedArray(){}
    T *data(){return (m_Elems);}
    const T *data()const {return (m_Elems);}
    reference front(){return (m_Elems[0]);}
    const_reference front() const{ return (m_Elems[0]);}
    reference back(){return (m_Elems[m_Size-1]);}
    const_reference back() const{return (m_Elems[m_Size - 1]);}
    reference at(size_t _Pos)
    { 
        if(_Pos>(ARRAY_SIZE-1)){return 0;}   ////_Xran();
        return (m_Elems[_Pos]);
    }
    const_reference at(size_type _Pos) const
    {
        if (_Pos>(ARRAY_SIZE-1)){return 0;} ////_Xran();
        return (m_Elems[_Pos]);
    }
    reference operator[](size_type _Pos){return (m_Elems[_Pos]);}
    const_reference operator[](size_type _Pos) const{return (m_Elems[_Pos]);}

    size_t capacity()const{return ARRAY_SIZE;}
    size_t size() const{return (m_Size);}
    size_t max_size() const{return (ARRAY_SIZE);}
    bool empty()const{return (m_Size == 0);}
    //add new 
    bool push_back(const T& val)
    {
        if (m_Size>(ARRAY_SIZE-1)){return false;}
        m_Elems[m_Size++]=val;return true;
    }
    void Clear(){m_Size=0;}
private:
    T m_Elems[ARRAY_SIZE];
    size_t m_Size;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//from asdcplib

// Non-reference counting container for internal member objects.
// Please do not use this class for any other purpose.
template <class T>
class mem_ptr
{
    T* m_p; // the thing we point to
    mem_ptr(T&);
public:
    mem_ptr() : m_p(0) {}
    mem_ptr(T* p) : m_p(p) {}
    ~mem_ptr(){if (m_p){delete m_p;m_p=0;}}  //improved old { delete m_p; } 
    T&   operator*()  const { return *m_p; }
    T*   operator->() const { return m_p; }
    operator T*()const { return m_p; }
    const mem_ptr<T>& operator=(T* p) { set(p); return *this; }
    T*   set(T* p)          { delete m_p; m_p = p; return m_p; }
    T*   get()        const { return m_p; }
    void release()          { m_p = 0; }
    bool empty()      const { return m_p == 0; }
};
////////////////////////////////////////////////////////////////////////////////

namespace QtMiscUtils {
    inline char toHexUpper(uint value)
    {
        return "0123456789ABCDEF"[value&0xF];
    }

    inline char toHexLower(uint value)
    {
        return "0123456789abcdef"[value&0xF];
    }

    inline int fromHex(uint c)
    {
        return ((c >= '0') && (c <= '9')) ? int(c - '0') :
            ((c >= 'A') && (c <= 'F')) ? int(c - 'A' + 10) :
            ((c >= 'a') && (c <= 'f')) ? int(c - 'a' + 10) :
            /* otherwise */              -1;
    }

    inline char toOct(uint value)
    {
        return '0'+char(value&0x7);
    }

    inline int fromOct(uint c)
    {
        return ((c>='0')&&(c<='7'))?int(c-'0'):-1;
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//极其常用的辅助调试函数 //打印16进制内存数据
inline void printHex(const void* buf,unsigned int len,unsigned int perLineChs=16)  
{
    //assert(buf!=NULL);
    //assert(len>=1);

    const unsigned int newLinePos=perLineChs-1;   //换行位置
    for(unsigned int n=0;n<len;n++) 
    {
        printf("%02X ",((unsigned char*)buf)[n]);
        if(newLinePos==n%perLineChs)    //每行打印多少个后换行
        {
            putchar('\n');
        }
    }
    if ((perLineChs%len)!=0)  //如果已经打印换行符，不再重复
    {
        putchar('\n');
    }
}

inline void printStr(const void* buf,unsigned int len)  //打印字符串遇到\0 继续，直到len长度
{
    for(unsigned int k=0;k<len;k++) 
    {
        char c=((char*)buf)[k];
        ::putchar(c);
        if(k%64==63){::printf("\n");}
    }
}

inline void printBin(const void* buf,unsigned int len)
{
    unsigned char ch;
    for(unsigned int k=0;k<len;k++)
    {
        ch=*(&((unsigned char*)buf)[k]);
        for(int j=7;j>=0;j--)    //j只能为int
        {
            if((ch>>j)&0X01)  
            {::putchar('1');}
            else 
            {::putchar('0');}
        }
        ::putchar(' ');
        if(15 == k%16){::putchar('\n');}
    }
    ::printf("\n");
}

template <class T>
inline void printBin(T tmp)
{
    //tmp=ByteOrderSwap(tmp);
    const unsigned char* buf=&tmp;
    int len=sizeof(tmp);
    int i,j;
    unsigned char ch;
    for(i=0;i<len;i++)
    {
        ch=buf[i];
        for(j=7;j>=0;j--)
        {
            if((ch>>j)&0X01)    //(ch>>j)&0X01
                ::putchar('1');
            else
                ::putchar('0');
        }
        ::putchar(' ');
        if(15 == i%16) ::printf("\n");
    }
    ::printf("\n");
};

//inline void dump(const void * p,unsigned int len)        //可以显示任意地址与二进制内容
//{
//    const char * s = (char*)p;
//    for (size_t i = 0;i<len;i++,s++)
//    {
//        printf("%p: %x\n",s,*s);
//        if(15 == i%16)
//        {
//            printf("\n");
//        }
//    }
//}

inline const char* ToStr(bool flag)
{ 
    return (flag)?("true"):("false");
}

inline bool isNullCstr(const char* pstr) //判断c字符串是否为空 考虑传入NULL的情况
{
    if (pstr==NULL) return true;
    return pstr[0]=='\0';   //return strlen(pstr)==0;
}

inline bool isInValidCstr(const char* pstr){return isNullCstr(pstr);}
inline bool isValidCstr(const char* pstr)
{
    if (pstr==NULL)return false;
    return strlen(pstr)>0;
}

inline void PauseInput()
{
    printf("Press enter key...");
    getchar();
    printf("\n");
}
//下一个回环计数器 在0-(Max-1)之间循环  Max!=0
inline unsigned int getNextCycle(unsigned int nCurr,unsigned int nMax)
{
    return (nCurr+1)%nMax;       //ok  390xhz       //方法1:取余法
    //return (++nCurr)%nMax;     //ok  390xhz      //方法1:取余法
    //return (nCurr++)%nMax;     //error 777xhz   //方法1:取余法

    //ok
    //if(nCurr>nMax)  //正确使用不会出现
    //{
    //    return (nCurr+1)%nMax;
    //}
    //unsigned int tmpCnt=nCurr+1;  //方法2:780XHZ   更快方法 没有取余除法
    //if (tmpCnt>=nMax)
    //{
    //    return 0;   //faster then tmpCnt=0
    //}
    //return tmpCnt;
}
inline uint64_t getNextCycle(uint64_t nCurr,uint64_t nMax)
{
    return (nCurr+1)%nMax;
}

//上一个回环计数器  在(Max-1)-0之间循环 Max!=0
inline unsigned int getPrevCycle(unsigned int nCurr,unsigned int nMax)
{
    return (nCurr+nMax-1)%nMax;  //ok
    //return (nCurr-1)%nMax;          //error 

    //ok
    //if(nCurr>nMax)  //正确使用不会出现
    //{
    //    return (nCurr-1)%nMax;
    //}
    //if (nCurr>=1)  //170xhz
    //{
    //    return nCurr-1;
    //}
    //return nMax-1;
}

inline uint64_t getPrevCycle(uint64_t nCurr,uint64_t nMax)
{
    return (nCurr+nMax-1)%nMax;
}

//下N个回环计数,无符号 在0-(Max-1)之间回环 Max!=0
inline unsigned int getNextNCycle(unsigned int nCurr,unsigned int nOffset,unsigned int nMax)
{
    return (nCurr+nOffset)%nMax;
}

inline uint64_t getNextNCycle(uint64_t nCurr,uint64_t nOffset,uint64_t nMax)
{
    return (nCurr+nOffset)%nMax;
}

//上N个回环计数,无符号  在(Max-1)-0之间回环 Max!=0
inline unsigned int getPrevNCycle(unsigned int nCurr,unsigned int offset,unsigned int nMax)
{
    //return (nCurr-offset)%nMax;  //error

    offset%=nMax;
    return (nMax+nCurr-offset)%nMax;  //ok

    //ok
    //if(nCurr>nMax)  //正确使用不会出现
    //{
    //    return(nCurr-offset)%nMax;
    //}
    //if (offset<=nCurr)
    //{
    //    return nCurr-offset;
    //}
    //return nCurr+nMax-offset;
}

//上N个回环计数,无符号  在(Max-1)-0之间回环
inline uint64_t getPrevNCycle(uint64_t nCurr,uint64_t offset,uint64_t nMax)
{
    offset%=nMax;
    return (nMax+nCurr-offset)%nMax;  //ok
}


//to test
//回环计数器，offset +-表示往后/前 考虑offset 为负 (0,nMax-1)
inline unsigned int getCycleNum(unsigned int nCurr,int offset,unsigned int nMax)
{
    int tmp=(int)nCurr+offset;
    return ((int)nMax+tmp)%nMax;
}
inline uint64_t getCycleNum(uint64_t nCurr,int offset,uint64_t nMax)
{
    int64_t tmp=(int64_t)nCurr+offset;
    return ((int64_t)nMax+tmp)%nMax;
}

////在Min-(Max-1)之间回环,Min可以为负数,步进值可正可负。TODO:test
inline int getCycleNumber(int nCurr,int nOffset,int nMin,int nMax)
{
    int rMax=nMax-nMin;
    nOffset%=rMax;
    nCurr-=nMin;
    nCurr%=rMax;
    return nMin+((rMax+nCurr+nOffset)%(rMax));
}

inline int64_t getCycleNumber(int64_t nCurr,int nOffset,int64_t nMin,int64_t nMax)
{
    int64_t rMax=nMax-nMin;
    nOffset%=rMax;
    nCurr-=nMin;
    nCurr%=rMax;
    return nMin+((rMax+nCurr+nOffset)%(rMax));
}


//以引用方式使用
inline void NextCycle(unsigned int& nCurr,unsigned int nMax){nCurr=getNextCycle(nCurr,nMax);}
inline void PrevCycle(unsigned int& nCurr,unsigned int nMax){nCurr=getPrevCycle(nCurr,nMax);}
inline void NextCycle(uint64_t& nCurr,uint64_t nMax){nCurr=getNextCycle(nCurr,nMax);}
inline void PrevCycle(uint64_t& nCurr,uint64_t nMax){nCurr=getPrevCycle(nCurr,nMax);}



//c++11
//int tmp(std::move(a));
//a = std::move(b);
//b = std::move(tmp);

template <typename T>
inline void Swap(T &v1, T &v2)
{
    //using std::swap;
    //std::swap(v1,v2);
    T tmp=v1;
    v1=v2;
    v2=tmp;
}

//常用数学常数:
#define SP_PI   3.1415926535897932384626433832795
#define SP_2PI  6.283185307179586476925286766559
#define SP_LOG2 0.69314718055994530941723212145818


template <typename T> 
inline const T &Min(const T &a, const T &b) { return (a < b) ? a : b; }

template <typename T> 
inline const T &Max(const T &a, const T &b) { return (a < b) ? b : a; }

template <typename T> 
inline const T &Bound(const T &min, const T &val, const T &max) { return qMax(min, qMin(max, val)); }

template <typename T>
inline T Abs(const T &t) { return t >= 0 ? t : -t; }

//返回四舍五入的整数
inline int Round(double d)
{
    return (int)(d+(d>=0?0.5f:-0.5f));  //opencv
    //return d >= 0.0 ? int(d + 0.5) : int(d - double(int(d-1)) + 0.5) + int(d-1);  //qt
}

//返回四舍五入的整数
inline int Round(float d)
{ 
    return (int)(d+(d>=0?0.5f:-0.5f)); //opencv
    //return d >= 0.0f ? int(d + 0.5f) : int(d - float(int(d-1)) + 0.5f) + int(d-1);  //qt
}

inline int64_t Round64(double d)
{ return d >= 0.0 ? int64_t(d + 0.5) : int64_t(d - double(int64_t(d-1)) + 0.5) + int64_t(d-1);}

inline int64_t Round64(float d)
{ return d >= 0.0f ? int64_t(d + 0.5f) : int64_t(d - float(int64_t(d-1)) + 0.5f) + int64_t(d-1); }


//为什么static ?
//static inline的内联函数，一般情况下不会产生函数本身的代码，而是全部被嵌入在被调用的地方。
//如果不加static，则表示该函数有可能会被其他编译单元所调用，所以一定会产生函数本身的代码。
//所以加了static，一般可令可执行文件变小。内核里一般见不到只用inline的情况，而都是使用static inline。

// 因浮点数不适宜用 == 直接比较，所以采用了精度模糊的方法

static inline bool FuzzyCompare(double p1, double p2)
{  return (Abs(p1 - p2) <= 0.000000000001 * Min(Abs(p1),Abs(p2)));}

static inline bool FuzzyCompare(float p1, float p2)
{
    return (Abs(p1 - p2) * 100000.f <= Min(Abs(p1),Abs(p2)));
}
static inline bool FuzzyIsNull(double d)
{
    return Abs(d) <= 0.000000000001;
}
static inline bool FuzzyIsNull(float f)
{
    return Abs(f) <= 0.00001f;
}

/*
   This function tests a double for a null value. It doesn't
   check whether the actual value is 0 or close to 0, but whether
   it is binary 0, disregarding sign.
*/
static inline bool IsNull(double d)
{
    union U{
        double d;
        uint64_t u;
    };
    U val;
    val.d = d;
    return (val.u&(0x7FFFFFFFFFFFFFFFULL)) == 0;
}

static inline bool IsNull(float f)
{
    union U{
        float f;
        uint32_t u;
    };
    U val;
    val.f = f;
    return (val.u&0x7FFFFFFF)==0;
}


inline int Round( int value )
{
    return value;
}

inline int Floor( float value )
{
    int i = (int)value;
    return i-(i>value);
}

inline  int Floor( int value )
{
    return value;
}

inline  int Ceil( float value )
{
    int i = (int)value;
    return i+(i<value);
}

inline  int Ceil( int value )
{
    return value;
}

inline  bool IsNaN( float value )
{
    union U32f{
        float f;
        uint32_t u;
    };
    U32f ieee754;
    ieee754.f = value;
    return (ieee754.u&0x7fffffff)>0x7f800000;
}

inline  bool IsInf( float value )
{
    union U32f{
        float f;
        uint32_t u;
    };
    U32f ieee754;
    ieee754.f = value;
    return (ieee754.u&0x7fffffff)==0x7f800000;
}


//inline int Floor( double value )  //old
inline int64_t Floor(double value )
{
    int64_t i = (int64_t)value;
    return i - (i > value);
}

inline  int64_t Ceil(double value )
{
    int64_t i =(int64_t)value;
    return i+(i<value);
}

//The function returns true if the argument is a plus or minus infinity (as defined by IEEE754 standard)
inline bool IsNaN( double value )
{
    union U64d {
        double f;
        uint64_t u;
    };
    U64d ieee754;
    ieee754.f = value;
    return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) +
           ((unsigned)ieee754.u != 0) > 0x7ff00000;
}

//The function returns true if the argument is a plus or minus infinity (as defined by IEEE754 standard)
inline bool IsInf(double value)
{
    union U64d {
        double f;
        uint64_t u;
    };
    U64d ieee754;
    ieee754.f = value;
    return ((unsigned)(ieee754.u>>32)&0x7fffffff)==0x7ff00000
        &&(unsigned)ieee754.u==0;
}



////求绝对值
//template<typename T> 
//inline T Abs(const T& t) {return (t>0)?t:(-t);}


//浮点数==模糊比较Fuzz Compare
 inline bool FComp(float v1,float v2)
{
    return  Abs(v1-v2)<=0.000001f;
}
inline bool FComp(double v1,double v2)
{
    return  Abs(v1-v2)<=0.0000000000001;
}

template <typename T> inline const T& Min(const T &a, const T &b);
template <typename T> inline const T& Max(const T &a, const T &b);

//常用N个数的最大值/最小值
template<typename T>
inline const T& Max3(const T& v1,const T& v2,const T& v3)
{
    const T& max=Max(v1,v2);
    if(v3>max) 
    {
        return v3;
    }
    return max;
}

template<typename T>
inline const T& Min3(const T& v1,const T& v2,const T& v3)
{
    const T& min=Min(v1,v2);
    if(v3<min) 
    {
        return v3;
    }
    return min;
}

//C force cast
template<typename To, typename From>
inline To Fcast(From f)
{
    return (To)f;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//取[a,b)的随机整数，用(rand() % (b-a))+ a; 
//取[a,b]的随机整数，用(rand() % (b-a+1))+ a; 
//取(a,b]的随机整数，用(rand() % (b-a))+ a + 1; 
//通用公式:a + rand() % n；其中的a是起始值，n是整数的范围。 
//取0～1.0之间的浮点数，用rand()/double(RAND_MAX)。

//自定义随机数发生器,系统随机数数据范围窄,效率低,且存在平台差异 create by lizulin 20200414
//C库自带效率低,且数据范围有的库实现是0-2^16,有的是0-2^31次方,C库rand返回值虽然是int，却不会返回负数，不符合语义


//线程安全问题考虑,用线程内本地变量.待续
extern unsigned int myRandSeed32;       //随机数种子
const unsigned int myrand_init_value=1u;
#define MyRand_MAX UINT32_MAX
inline void MySrand(unsigned int nSeed)
{
    myRandSeed32=nSeed;
}
inline void ResetSrand()   //复位随机数种子为本库默认值
{
    MySrand(myrand_init_value);
}

inline int MyRand()     //return INT_MIN-INT_MAX
{
    myRandSeed32=myRandSeed32*214013ULL+2531011U; //
    return myRandSeed32;
}
inline unsigned int MyRandu()  //return 0-UINT_MAX
{
    return MyRand();
}


//随机数类，一下情况作为线程安全的便捷工具会用到
class CRand
{
    unsigned int myRandSeed32;       //随机数种子
public:
    CRand()
    {
        //srand(reinterpret_cast<unsigned int>(this));                        //用任意随机数作为默认种子，地址或时间等
        srand(myrand_init_value);
    }
    void srand(unsigned int nSeed)   //设置随机数种子
    {
        myRandSeed32=nSeed;
    }
    unsigned int randu()   //返回0-UINT_MAX之间随机数,仅正整数
    {
        myRandSeed32=myRandSeed32*214013ULL+2531011U; //
        return myRandSeed32;
    }
    int rand()                  //返回INT_MIN 到INT_MAX 之间随机数，有正有负
    {
        return randu();
    }
    float randf(float Max=1.0,float Min=0.0)
    {
        return (float)(Min+(randu()/(double)MyRand_MAX)*(Max-Min));
    }
};

//奇数偶数判断, 负数也有奇数性    //0不是奇数，而是偶数（一个非正非负的特殊偶数）
inline bool isEveNumBer(int n)       //偶数
{
    return (n&0X1)==0;  //faster   old:return (n%2)==0;
}
inline bool isEveNumBer(int64_t n) //偶数
{
    return (n&0X1ULL)==0ULL;
}
inline bool isOddNumBer(int n)      //奇数
{
    //return (n%2)!=0;   //error n=1;
    return (n&0X1)!=0;
}
inline bool isOddNumBer(int64_t n) //奇数
{
    return (n&0X1ULL)!=0ULL;
}

//实现将正数变成对应的负数,将负数变成对应的正数
inline int turnNum(int a){ return ~a+1;}
inline int64_t turnNum(int64_t a){ return ~a+1;}


//from opencv fundam.cpp文件absolute value without jumps
#define  SP_IABS(a) (((a)^((a)<0?-1:0))-((a)<0?-1:0))   //integer

//取绝对值
inline int absV1(int n){ return (n>0)?n:-n;}
inline int64 absV1(int64_t n){ return (n>0)?n:-n;}
inline int absV2(int n){ return (n^(n>>31))-(n>>31);}  //fast（某些机器上，效率比n>0 ? n:-n 高）//经测试与库函数效率一样
inline int64 absV2(int64_t n){ return (n^(n>>63))-(n>>63);}

inline int MyAbs(int n){ return absV2(n);}
inline int64_t MyAbs(int64_t n){ return absV2(n);}

//浮点数绝对值快速方法
inline float fabs_V1(float V){return (V>0)?V:-V;}
inline float fabs_V2(float y)
{
    float x =y;
    *(((int *) &x) + 1) &= 0x7fffffff;
    return x;
}
inline float fabs_V3(float in)
{
    union { float f; int u; } x;
    x.f = in;
    x.u &= 0x7fffffffu;
    return x.f;
}

inline double fabs_V1(double V){return (V>0)?V:-V;}
inline double fabs_V2(double y)
{
    double x=y;
    *(((int64_t*) &x) + 1) &= 0x7fffffffffffffff;
    return x;
}
inline double fabs_V3(double in)
{
    union {double f; int64_t u; } x;
    x.f=in;
    x.u&=0x7fffffffffffffffull;
    return x.f;
}

inline float MyAbs(float x){return fabs_V3(x);}
inline double MyAbs(double x){return fabs_V3(x);}

//fast sqrt 
float SqrtByBisection(float x);         //二分法 
double SqrtByBisection(double x);
float SqrtByNewton(float x);           //牛顿迭代法
double SqrtByNewton(double x);
float SqrtFast(float x);                    //fast version from Quake-III Arena
double SqrtFast(double x);

inline float MySqrt(float x){return SqrtFast(x);}
inline double MySqrt(double x){return SqrtFast(x);}

#ifdef UNIT_TEST
BASIC_API int Test_Global();
#endif

#endif
