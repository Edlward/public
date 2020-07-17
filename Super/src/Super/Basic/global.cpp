#include "global.h"

//主要放置无法内联的静态变量,常量数组。

#include <string.h>
#include <memory.h>
#include <stdlib.h>    //for gcc malloc
#include <stdio.h>
#include <math.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif

static const unsigned int kNumMillisPerSecond=1000;
static const unsigned int kNumMicrosPerMilli=1000;
static const unsigned int kNumMicrosPerSecond=kNumMillisPerSecond*1000;
static const unsigned int kNumNanosPerMicro=1000;
static const unsigned int kNumNanosPerSecond=kNumNanosPerMicro*kNumMicrosPerSecond;

extern void SleepMs(unsigned int milliseconds);   //sleep非常常用，在此也实现一份
extern void SleepFor(double seconds);

#ifdef _WIN32
// Window's Sleep takes milliseconds argument.
void SleepMs(unsigned int milliseconds){ ::Sleep(milliseconds);}
void SleepFor(double seconds) 
{
    SleepMs(static_cast<unsigned int>(kNumMillisPerSecond * seconds));
}
#else   //
//void SleepForMicroseconds(unsigned int microseconds)
//{
//    struct timespec sleep_time;
//    sleep_time.tv_sec = microseconds / kNumMicrosPerSecond;
//    sleep_time.tv_nsec = (microseconds % kNumMicrosPerSecond) * kNumNanosPerMicro;
//    while (nanosleep(&sleep_time,&sleep_time)!=0&&errno==EINTR)
//        ;  // Ignore signals and wait for the full interval to elapse.
//}

//高精度基于微妙的实现,暂无必要  //Microsecond precision,Not necessary for now
//void SleepMs(unsigned int milliseconds) 
//{
//    SleepForMicroseconds(milliseconds * kNumMicrosPerMilli);
//}
//void SleepFor(double seconds)
//{
//    SleepForMicroseconds(static_cast<unsigned int>(seconds * kNumMicrosPerSecond));
//}


void SleepMs(unsigned int milliseconds) 
{
      ::usleep(milliseconds*1000);
}
void SleepFor(double seconds)
{
     ::usleep(seconds*1000*1000);
}
#endif




//extern void SleepMs(unsigned int milliseconds);   //sleep for millseconds
//extern void SleepFor(double seconds);                  //sleep for seconds
//extern int __cdecl sp_printf(const char* format,...);
//extern int __cdecl sp_sprintf(char * buf, const char *format,...);
//struct	_iobuf;
//typedef	_iobuf FILE;
//extern int __cdecl sp_fprintf(FILE* fp,const char *format,...);
//extern int __cdecl sp_putchar(int ch);
//extern int __cdecl sp_getchar();
//extern size_t __cdecl sp_strlen(const char* Str);
//extern char* __cdecl sp_strcpy(char* dst,const char* src);
//extern char* __cdecl sp_strncpy(char *dst, const char *src, size_t len);
//extern void* __cdecl sp_memcpy(void* dst,const void* src,size_t len);
//extern void* __cdecl sp_memset(void* dst,int val,size_t len);
//extern void* __cdecl sp_memmove(void* dst, const void* src,size_t len);
//extern int __cdecl sp_memcmp(const void *buf1, const void *buf2, size_t len);
//extern char* __cdecl sp_strcat(char *dst, const char *src);
//extern char* __cdecl sp_strncat(char *dst, const char *src,size_t len);
//extern int __cdecl sp_strcmp(const char *str1, const char *str2);
//extern int __cdecl sp_strncmp(const char *str1, const char *str2,size_t len);
//extern const char* __cdecl sp_strstr(const char *str, const char *substr);
//extern void* __cdecl sp_malloc(size_t size);
//extern void __cdecl sp_free(void *p);
//extern void* __cdecl sp_realloc(void *ptr,size_t size);
//extern void* __cdecl sp_malloc_aligned(size_t size, size_t alignment);
//extern void* __cdecl sp_realloc_aligned(void *oldptr, size_t newsize, size_t oldsize, size_t alignment);
//extern void __cdecl sp_free_aligned(void *ptr);
//extern void __cdecl sp_exit(int value);
//extern void __cdecl sp_abort(void);
//extern int __cdecl sp_atexit(void (*func)(void));
//extern int __cdecl sp_system(const char *strCmd);
//extern char*__cdecl sp_getenv(const char *varName);

#if 0
int __cdecl sp_printf(const char * format, ...)
{
    //return ::printf(format,...);  //error
    va_list args;
    va_start(args,format);
    int res=::vprintf(format,args); //必须用vprintf
    va_end(args);
    return res;
}
int __cdecl sp_sprintf(char * buf, const char *format,...)
{
    va_list args;
    va_start(args,format);
    int res=::vsprintf(buf,format,args);
    va_end(args);
    return res;
}
int __cdecl sp_fprintf(FILE* fp,const char *format,...)
{
    va_list args;
    va_start(args,format);
    int res=::vfprintf(fp,format,args);
    va_end(args);
    return res;
}

int __cdecl sp_putchar(int ch)
{
    return ::putchar(ch);
}
int __cdecl sp_getchar()
{
    return ::getchar();
}

size_t __cdecl sp_strlen(const char* Str)
{
    return ::strlen(Str);
}
char* __cdecl sp_strcpy(char* dst,const char* src)
{
    return ::strcpy(dst,src);
}
char*__cdecl sp_strncpy(char *dst, const char *src, size_t len)
{
    return ::strncpy(dst,src,len);
}

void* __cdecl sp_memcpy(void* dst,const void* src,size_t size)
{
    return ::memcpy(dst,src,size);
}
void* __cdecl sp_memset(void* dst,int val,size_t size)
{
    return ::memset(dst,val,size);
}
void* __cdecl sp_memmove(void* dst, const void* src,size_t len)
{
        return ::memmove(dst,src,len);
}
int __cdecl sp_memcmp(const void *buf1, const void *buf2, size_t len)
{
    return ::memcmp(buf1,buf2,len);
}
char* __cdecl sp_strcat(char *dst, const char *src)
{
    return ::strcat(dst,src);
}
char* __cdecl sp_strncat(char *dst, const char *src,size_t len)
{
    return ::strncat(dst,src,len);
}
int __cdecl sp_strcmp(const char *str1, const char *str2)
{
    return ::strcmp(str1,str2);
}
int __cdecl sp_strncmp(const char *str1, const char *str2,size_t len)
{
    return ::strncmp(str1,str2,len);
}
const char* __cdecl sp_strstr(const char *str, const char *substr)
{
    return ::strstr(str,substr);
}
void* __cdecl sp_malloc(size_t size)
{
    return ::malloc(size);
}
void __cdecl sp_free(void *p)
{
    return ::free(p);
}
void* sp_realloc(void *ptr,size_t size)
{
    return ::realloc(ptr,size);
}

void __cdecl sp_exit(int value)
{
    ::exit(value);
}
void __cdecl sp_abort(void)
{
    return ::abort();
}
int __cdecl sp_atexit(void (*func)(void))
{
    return ::atexit(func);
}
int __cdecl sp_system(const char *strCmd)
{
    return ::system(strCmd);
}
char* __cdecl sp_getenv(const char *varName)
{
    return ::getenv(varName);
}
#endif


#include<map>
#include<mutex>
struct  FunInfo
{
    int num;
    FunInfo()
    {
        num=0;
    }
};

class FunTraceMgr
{
public:
    FunTraceMgr()
    {
        PRINT_FUN_NAME
    }
    ~FunTraceMgr()
    {
        PRINT_FUN_NAME
           //析构时打印剩余的
        this->print();
    }
    void print() const
    {
        PRINT_FUN_NAME
        ::printf("Legacy Fun: size:%d\n",(int)gMapFun.size());
        const std::map<int64_t,FunInfo>& mMap=gMapFun;
        std::map<int64_t,FunInfo>::const_iterator it;
        unsigned int index=0;
        for (it=mMap.begin();it!=mMap.end();it++)
        {
            int64_t key=it->first;
            const char* mfunName=(const char*)key;
            ::printf("Fun[%u]:%s\n",index,mfunName);
            index++;
        }
    }
    void RegisterKey(const char* name)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mtxMapFun);
        int64_t key=(int64)name;   //为了效率把字符串名称首地址作为key保存
        gMapFun.insert(std::map<int64_t,FunInfo>::value_type(key,FunInfo()));  
    }
    void UnRegisterKey(const char* name) 
    {
        std::lock_guard<std::recursive_mutex> lock(m_mtxMapFun);
        int64_t key=(int64)name;  //为了效率把字符串名称首地址作为key保存
        gMapFun.erase(key);
    }
private:
    std::recursive_mutex m_mtxMapFun;
    std::map<int64_t,FunInfo> gMapFun;
};


FunTraceMgr& GetFunTraceMgr()
{
    static FunTraceMgr gFunTraceMgr;
    return gFunTraceMgr;
}

//注册或反注册
void RegisterFunTraceKey(const char* name)
{
    GetFunTraceMgr().RegisterKey(name);
}
void UnRegisterFunTraceKey(const char* name) 
{
    GetFunTraceMgr().UnRegisterKey(name);
}

extern void PrintFunTrace()
{
    GetFunTraceMgr().print();
}

void* Malloc_Aligned(size_t memSize,const int alignBytes)   //任意字节对齐版本 by lizulin
{
    typedef int OffsetType;   //用int 型保存距离偏差
    char *ptrRaw=(char*)::malloc(memSize+alignBytes+sizeof(OffsetType));
    char* ptr_tmp=ptrRaw+sizeof(OffsetType);
    char *ptr_aligned =ptr_tmp+(alignBytes-((size_t)ptr_tmp&(alignBytes-1)));
    OffsetType* ptr_offset_store=(OffsetType*)((char*)ptr_aligned-sizeof(OffsetType));
    *ptr_offset_store=(OffsetType)(ptr_aligned-ptrRaw);
    return  ptr_aligned;
}
void Free_Aligned(void* ptr)
{
    if (ptr==NULL) return;
    typedef int OffsetType;
    OffsetType offset=*((OffsetType*)ptr-1);
    char* pRaw=(char*)ptr-offset;
    ::free(pRaw);
}


void*__cdecl malloc_aligned(size_t size, size_t alignment)
{
    return realloc_aligned(0,size,0,alignment);
}

// from qmalloc.cpp
void* realloc_aligned(void *oldptr, size_t newsize, size_t oldsize, size_t alignment)
{
    // fake an aligned allocation
    void *actualptr = oldptr ? static_cast<void **>(oldptr)[-1] : 0;
    if (alignment <= sizeof(void*)) {
        // special, fast case
        void **newptr = static_cast<void **>(realloc(actualptr, newsize + sizeof(void*)));
        if (!newptr)
            return 0;
        if (newptr == actualptr) {
            // realloc succeeded without reallocating
            return oldptr;
        }

        *newptr = newptr;
        return newptr + 1;
    }

    // malloc returns pointers aligned at least at sizeof(size_t) boundaries
    // but usually more (8- or 16-byte boundaries).
    // So we overallocate by alignment-sizeof(size_t) bytes, so we're guaranteed to find a
    // somewhere within the first alignment-sizeof(size_t) that is properly aligned.

    // However, we need to store the actual pointer, so we need to allocate actually size +
    // alignment anyway.

    void *real = ::realloc(actualptr, newsize + alignment);
    if (!real)
        return 0;

    size_t faked = reinterpret_cast<size_t>(real) + alignment;
    faked &= ~(alignment - 1);
    void **faked_ptr = reinterpret_cast<void **>(faked);


//#define ptrdiff_t int64_t;

    if (oldptr) {
        ptrdiff_t oldoffset = static_cast<char *>(oldptr) - static_cast<char *>(actualptr);
        ptrdiff_t newoffset = reinterpret_cast<char *>(faked_ptr) - static_cast<char *>(real);
        if (oldoffset != newoffset)
            ::memmove(faked_ptr, static_cast<char *>(real)+oldoffset,Min(oldsize, newsize));
    }

    // now save the value of the real pointer at faked-sizeof(void*)
    // by construction, alignment > sizeof(void*) and is a power of 2, so
    // faked-sizeof(void*) is properly aligned for a pointer
    faked_ptr[-1] = real;

    return faked_ptr;
}

void __cdecl free_aligned(void *ptr)
{
    if (!ptr)
    {
       return;
    }

    void **ptr2 = static_cast<void **>(ptr);
    ::free(ptr2[-1]);
}

//一个Sqrt函数引发的血案--作者: 码农1946  来源: 博客园 
//快速开平方
float SqrtByBisection(float x) //用二分法 
{ 
    if(x<0) //小于0的按照你需要的处理 
        return x; 

    const float eps=1e-6;//0.0000001;
    float mid,last; 
    float low,up; 
    low=0, up=x; 
    mid=(low+up)/2; 
    do
    {
        if(mid*mid>x)
            up=mid; 
        else 
            low=mid;
        last=mid;
        mid=(up+low)/2; 
    }while(fabs(mid-last) >eps);//精度控制
    return mid; 
} 

double SqrtByBisection(double x) //用二分法 
{ 
    if(x<0) //小于0的按照你需要的处理 
        return x; 
    const double eps=1e-10;//

    double mid,last; 
    double low,up; 
    low=0, up=x; 
    mid=(low+up)/2; 
    do
    {
        if(mid*mid>x)
            up=mid; 
        else 
            low=mid;
        last=mid;
        mid=(up+low)/2; 
    }while(fabs(mid-last) >eps);
    return mid; 
} 


float SqrtByNewton(float x)  //牛顿迭代法
{
    const float eps=1e-6;//0.0000001;
    float val = x;
    float last;
    do
    {
        last = val;
        val =(val + x/val)/2;
    }while(abs(val-last)>eps);
    return val;
}

double SqrtByNewton(double x)
{
    const double eps=1e-10;//
    double val = x;
    double last;
    do
    {
        last = val;
        val =(val + x/val)/2;
    }while(abs(val-last)>eps);
    return val;
}


//快速开平方 from Quake-III Arena
float SqrtFast(float x)
{
    float xhalf = 0.5f*x;
    int i = *(int*)&x; // get bits for floating VALUE 
    i = 0x5f375a86- (i>>1); // gives initial guess y0
    x = *(float*)&i; // convert bits BACK to float
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    return 1/x;
}

double SqrtFast(double Value)
{
    double x2, y;
    const double threehalfs = 1.5F;
    union
    {
        double d;
        int64_t i;// __int64
    }d;
    x2 = Value * 0.5F;
    y = Value;
    d.d =  y;
    d.i = 0x5fe6ec85e7de30da-(d.i >> 1);
    y = d.d;
    y = y * (threehalfs - (x2 * y * y)); //1stiteration
    y = y * (threehalfs - (x2 * y * y)); //2nditeration, this can be removed
    return y;
}





#if 0
//rand()函数实现原理：线性同余法
//产生整数rand的原理是：   
//    y＝(ax＋b)(mod  n)。其中n一般是一个很大的素数（几万）。a也是大素数，而且a，b，n都是常数。所以rand的产生决定于x，他被称seed。每一个seed都是上一次产生的y的函数。这样，如果直接取seed＝y的话，虽然产生的rand之间相关性甚小，但只要知道某个y，就能推知以后的rand。   为避免这种情况，一般取seed为y和当时计算机的时间的函数，如seed＝y＋t系统里的随机数是利用初等数论中的同余定理来实现的.  比如C中对于rand()函数是如下实现的.   
unsigned long int next = 1;
void srand(unsigned int seed)  
{
    next = seed;
}
int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}


//另外一个高级一点的随机数生成为
unsigned int x = 123456789,y = 362436000,z = 521288629,c = 7654321; /* Seed variables */ 
unsigned int KISS()
{  
    uint64_t t, A = 698769069ULL;  
    x = 69069*x+12345;  
    y ^= (y<<13); 
    y ^= (y>>17); 
    y ^= (y<<5);  
    t = (A*z + c);
    c = (t >> 32);
    z = t;
    return x+y+z;  
}

//居然就只是把/线性同余/移位轮转/带记忆乘法。这3种基本的随机数发生法一起用，便获得很好的效果。
//的确够巧的，从这一点上说，比Mersenne Twister要有趣多了。而且通过它，我们一次就学会3种随机数发生法。
//抽出其中任何1种，都可以单独使用，仿佛一把瑞士军刀。
//线性同余法生成的是伪随机数，粗略符合均匀分布。根据中心极限定理，任何分布的噪声，通过反复相加，就可以成为高斯噪声。

//PHP的实现:0-1.0
//rnd.seed = (rnd.seed*9301+49297) % 233280;
//return rnd.seed/(233280.0);


//C库实现: VS下返回值应该是0-32767，没有负数
//void __cdecl srand (unsigned int seed)
//{
//    _getptd()->_holdrand = (unsigned long)seed;
//}
//
//int __cdecl rand(void)
//{
//    _ptiddata ptd = _getptd();
//    return( ((ptd->_holdrand = ptd->_holdrand * 214013L
//        + 2531011L) >> 16) & 0x7fff );
//}



//C++ std库
mersenne_twister_engine 类模板实现了梅森选择算法，它被这样叫是因为周期长度是一个梅森素数。
梅森素数是 2n-1 形式的素数，因此 7 和 127 是梅森素数；当然，用在这个算法中的梅森素数更大。
因为它可以生成非常高质量的序列，但存在速度相对较慢的缺点。
对于定义具体生成器的 mersenne_twister_engine 的实例有两个类型别名。mtl9937 生成随机的无符号 32 位整数，
mtl9937_64 生成无符号的 64 位整数。mtl9937 随机数生成器的周期长度为 219937-1，它因此得名。

std::random_device rd;
std::mtl9937_64 rng {rd()}; // Generates random 64-bit integers
std::uniform_real_distribution<long double> dist {-5.OL, 5.OL};
for(size_t i {}; i < 8; ++i)
{
    std::cout << std::setw(5)<< dist(rng)<<" "; //-2.57481 3.0546 -1.6438 2.14798 -3.84095 0.973843 -2.98971 -2.1067
}


#endif





unsigned int myRandSeed32=myrand_init_value;
//进一步统计随机器的数据分配概率,
void TestRand()
{
    const int Mode=128;
    unsigned int retCnt[Mode]={0};
    for(unsigned int n=0;n<100000;n++)
    {
        unsigned int randV=MyRand()%Mode;
        //printf("%u ",randV);
        retCnt[randV]++;
    }

    for(unsigned int n=0;n<Mode;n++)
    {
        printf("%u ",retCnt[n]);
    }
}

void sp_assert(const char *assertion, const char *file, int line)
{
    //qFatal("ASSERT: \"%s\" in file %s, line %d", assertion, file, line);
}

/*
  The Q_ASSERT_X macro calls this function when the test fails.
*/
void sp_assert_x(const char *where, const char *what, const char *file, int line)
{
    //qFatal("ASSERT failure in %s: \"%s\", file %s, line %d", where, what, file, line);
}






int Test_Global()
{
    return 0;
}







