#include "ThreadSync.h"
#include "Tstring.h"   //String Wstring 与宽字符串转换与编码相关

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//不同的平台不同的头文件定义。最好不要在.h 引入系统头文件，放到cpp里
#ifdef OS_WIN
    #include <windows.h>
#else
    #include <pthread.h>
    #include <unistd.h>
    #include <semaphore.h>  //Posix信号量
    #include <fcntl.h>
    #include <signal.h>
    #include <sys/time.h>   //for gettimeofday
#endif



////跨平台的快速锁--windows使用临界区实现不能跨进程  允许加锁线程递归加锁
//前置声明
//#if defined(OS_WIN)
//    struct _RTL_CRITICAL_SECTION;
//    typedef struct _RTL_CRITICAL_SECTION RTL_CRITICAL_SECTION;
//    typedef RTL_CRITICAL_SECTION CRITICAL_SECTION;
//#else
//    #include <pthread.h>
//    //union pthread_mutex_t;
//#endif


NAME_SPACE_MAIN_BEGIN

class MutexPrivate
{
public:
#if defined(OS_WIN)  
    CRITICAL_SECTION   m_Cs;        //临界区  windows临界区默认只能递归,
#elif defined(OS_LINUX)  
    pthread_mutex_t     m_mutex;    //unix互斥锁  
#endif  
    MutexPrivate()
    {
#if defined(OS_WIN)  
        InitializeCriticalSection(&m_Cs);  
#else
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);    //为了接口一致，默认递归锁
        pthread_mutex_init(&m_mutex,&attr);  
#endif
    }
    ~MutexPrivate()
    {
#if defined(OS_WIN)  
        DeleteCriticalSection(&m_Cs);  
#else // defined(OS_LINUX)  
        pthread_mutex_destroy(&m_mutex);  
#endif
    }
};



MutexPrivate* Mutex_CreatPrivate()
{
    return new MutexPrivate;
}
void Mutex_DestoryPrivate(MutexPrivate* pimpl)
{
    delete pimpl;
}
void Mutex_ConstructPrivate(MutexPrivate* p,void*& pHnd)
{
    const int size=sizeof(MutexPrivate);       //可以通过编译器提示获取大小并留余量
    static_assert(Mutex::PrivateSizeMax>=sizeof(MutexPrivate),"Not enough reserved space");
    new(p) MutexPrivate;   //在原地址构造

    //直接记住内部指针，此后操作更快
#if defined(OS_WIN)  
    pHnd=&p->m_Cs;
#else
    pHnd=&p->m_mutex;
#endif
}
void Mutex_DestructPrivate(MutexPrivate* p)
{
    p->~MutexPrivate();   //显示析构
}

void Mutex_Lock(void* pHnd)
{
#if defined(OS_WIN)  
    EnterCriticalSection((CRITICAL_SECTION*)pHnd);  
#else // defined(OS_LINUX)  
    pthread_mutex_lock((pthread_mutex_t*)pHnd);
#endif
}

bool Mutex_TryLock(void* pHnd)
{
#if defined(OS_WIN)  
    if (!TryEnterCriticalSection((CRITICAL_SECTION*)pHnd))  
    {  
        return false;  
    }  
#else //defined(OS_LINUX)  
    if(pthread_mutex_trylock((pthread_mutex_t*)pHnd)!=0)  
    {  
        return false;  
    }  
#endif  
    return true;
}
void Mutex_UnLock(void* pHnd)
{
#if defined(OS_WIN)  
    LeaveCriticalSection((CRITICAL_SECTION*)pHnd);  
#else //defined(OS_LINUX)  
    pthread_mutex_unlock((pthread_mutex_t*)pHnd);  
#endif
}
 
class MutexNoRecursivePrivate
{
public:
#if defined(OS_WIN)  
    MutexNoRecursivePrivate()
    {
            pHnd=CreateMutex(NULL,FALSE,NULL);
    }
    ~MutexNoRecursivePrivate()
    {
        if (pHnd!=NULL)
        {
            CloseHandle(pHnd);
            pHnd=NULL;
        }
    }
    HANDLE pHnd;
#else
    MutexNoRecursivePrivate()
    {
        pthread_mutexattr_t attr;  
        pthread_mutexattr_init(&attr);  
        pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_NORMAL);   //not recursive lock
        pthread_mutex_init(&m_mtx,&attr);  
    }
    ~MutexNoRecursivePrivate()
    {
       pthread_mutex_destroy(&m_mtx);
    }
   pthread_mutex_t m_mtx;
#endif
};

MutexNoRecursivePrivate* MutexNoRecursive_CreatePrivate()
{
     return new MutexNoRecursivePrivate;
}
void MutexNoRecursive_DestoryPrivate(MutexNoRecursivePrivate* p)
{
    delete p;
}

void MutexNoRecursive_ConstructPrivate(MutexNoRecursivePrivate* p,void*& pHnd)
{
    const int size=sizeof(MutexNoRecursivePrivate);//可以通过编译器提示获取大小并留余量
    static_assert(MutexNoRecursive::PrivateSizeMax>=sizeof(MutexNoRecursivePrivate),"Not enough reserved space");
    new(p) MutexNoRecursivePrivate;   //在原地址构造

    //直接记住内部指针，此后操作更快
#if defined(OS_WIN)  
    pHnd=p->pHnd;
#else
    pHnd=&p->m_mtx;
#endif
}

void MutexNoRecursive_DestructPrivate(MutexNoRecursivePrivate* p)
{
        p->~MutexNoRecursivePrivate();   //显示析构
}


void MutexNoRecursive_Lock(void* pHnd)       //加锁  
{
#if defined(OS_WIN)  
    if (pHnd==NULL) return;  //
    DWORD nRet = WaitForSingleObject(pHnd,INFINITE);
    if (nRet!=WAIT_OBJECT_0)
    {
        printf("Error at: ProcessMutex::Lock()\n");
        //return false;
    }
    //return true;
#else // defined(OS_LINUX)  
    pthread_mutex_lock((pthread_mutex_t*)pHnd);  
#endif
}
bool MutexNoRecursive_TryLock(void* pHnd)   //非阻塞 枷锁成功返回true 否则返回false  
{
#if defined(OS_WIN)  
    if (pHnd==NULL) return false;  //
    DWORD nRet = WaitForSingleObject(pHnd,5);  //暂时默认5ms
    if (nRet !=WAIT_OBJECT_0)
    {
        //printf("Error at: ProcessMutex::Lock()\n");
        return false;
    }
    return true;
#else //defined(OS_LINUX)  
    if(pthread_mutex_trylock((pthread_mutex_t*)pHnd)!=0)
    {
        return false;
    }
#endif  
    return true;
}
void MutexNoRecursive_UnLock(void* pHnd)    //解锁  
{
#if defined(OS_WIN)
    if (pHnd==NULL) return;  //
    ReleaseMutex(pHnd);
#else //defined(OS_LINUX)  
    pthread_mutex_unlock((pthread_mutex_t*)pHnd);  
#endif
}



#if defined(OS_WIN)  
void* MutexProcess_Create(MutexProcess* parent)
{
    Tstring sName=FromCstr(parent->m_name);
    void* pHand = OpenMutex(MUTEX_ALL_ACCESS,FALSE,sName.c_str());
    if (pHand==NULL)
    {
        pHand = CreateMutex(NULL,FALSE,sName.c_str());
        if(pHand==NULL)
        {
            PreDprintf("failed! 跨进程互斥锁创建失败!:%s\n",TstringTostring(sName).c_str());
            //return false;  //exit(-1);
             return pHand;
        }
    }
    else
    {
        parent->m_isExist=true;  //已经存在
    }
    //return true;
    return pHand;
}

void MutexProcess_Destory(MutexProcess* parent,void*& pHand)
{
    if (pHand!=NULL)
    {
        CloseHandle(pHand);
        pHand=NULL;
    }
}
bool MutexProcess_Lock(void* pHand)        //加锁  
{
    //互斥锁创建失败
    if (pHand==NULL)
    {
        return false;
    }
    DWORD nRet = WaitForSingleObject(pHand,INFINITE);
    if (nRet!=WAIT_OBJECT_0)
    {
        PRINT_ERROR_LINE
        return false;
    }
    return true;
}
bool MutexProcess_TryLock(void* pHand,unsigned int ms)   //非阻塞 枷锁成功返回true 否则返回false  
{
    //互斥锁创建失败
    if (pHand==NULL)
    {
        return false;
    }
    DWORD nRet = WaitForSingleObject(pHand,ms);
    if (nRet!=WAIT_OBJECT_0)
    {
        PRINT_ERROR_LINE
        return false;
    }
    return true;
}
bool MutexProcess_UnLock(void* pHand)   //解锁  
{
    if (pHand==NULL)
    {
        return false;
    }
    return (ReleaseMutex(pHand)!=0);
}
 

#else //OS_LINUX


void* MutexProcess_Create(MutexProcess* parent)
{
      const char* key_name=parent->m_name;
       void* pHand = (void*)sem_open(key_name,0);
        if (pHand==NULL)           //不存在才创建 测试待续
        {
            printf("sem_open() name:%s errno:%d %s\n",key_name,errno,strerror(errno));
            //O_RDWR|O_CREAT  O_CREAT|O_EXCL
            pHand = (void*)sem_open(key_name,O_RDWR|O_CREAT,0644,1);    //作为互斥锁,信号量设置为1  O_RDWR|
            if (pHand==NULL)
            {
                PRINT_ERROR_LINE
                printf("sem_open() failed name:%s errno:%d %s\n",key_name,errno,strerror(errno));
                return NULL;  //return false;
            }
        }
        else
        {
            parent->m_isExist=true;
        }
        //return true;
        return pHand;
}

void MutexProcess_Destory(MutexProcess* parent,void*& pHand)
{
    //PRINT_TRACE
    if (pHand!=NULL)
    {
        int ret =sem_close((sem_t*)pHand);
        if (ret!=0)
        {
            PRINT_ERROR_LINE
            printf("sem_close() failed ret:%d errno:%d %s\n",ret,errno,strerror(errno));
        }
        sem_unlink(parent->m_name);
        pHand=NULL;
    }
}
bool MutexProcess_Lock(void* pHand)       //加锁  
{
    if (pHand==NULL)
    {
     PRINT_ERROR_LINE
        return false;
    }

    int ret = sem_wait((sem_t*)pHand);
    if (ret!=0)
    {
        PRINT_ERROR_LINE
        return false;
    }
    return true;
}
bool MutexProcess_TryLock(void* pHand,unsigned int ms)   //非阻塞 枷锁成功返回true 否则返回false  
{
    if (pHand==NULL)
    {
        PRINT_ERROR_LINE
        return false;
    }
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME,&ts)<0)
    {
        PRINT_ERROR_LINE
        return false;
    }

    //用的是UTC时间,正在等待的时候管理员调整时间会出问题 系统bug，其他办法研究待续 测试待续
    int32 nano_sec=(ms%1000)*1000;  //ms 转s和ns 
    int32 sec=ms/1000;

    ts.tv_sec+=sec;
    ts.tv_nsec+=nano_sec;

   // #define NSECTOSEC    1000000000

    ts.tv_sec += ts.tv_nsec/1000000000; //Nanoseconds [0 .. 999999999]
    ts.tv_nsec = ts.tv_nsec%1000000000;

    int ret = sem_timedwait((sem_t*)pHand,&ts);
    if (ret!=0)
    {
                PRINT_ERROR_LINE
        return false;
    }
    return true;
}

bool MutexProcess_UnLock(void* pHand)    //解锁  
{
    if (pHand==NULL)
    {
        PRINT_ERROR_LINE
        return false;
    }
    int ret = sem_post((sem_t*)pHand);
    if (ret != 0)
    {
        return false;
    }
    return true;
}

#endif


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class SyncEventPrivate
{
public:
#ifndef _WIN32
    pthread_cond_t _cond;
    Mutex _mutex;
    bool _manual_reset;
    bool _signaled;
    int _consumer;
    SyncEventPrivate()
    {
#ifdef __linux__
        pthread_condattr_t attr;
        int r = pthread_condattr_init(&attr);
        //assert(r == 0);
        pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
        r = pthread_cond_init(&_cond, &attr);
        //assert(r == 0);
        pthread_condattr_destroy(&attr);
#else
        int r = pthread_cond_init(&_cond, NULL);
        //assert(r == 0);
#endif
    }
   ~SyncEventPrivate()
   {
       pthread_cond_destroy(&_cond);
   }

   void signal()
   {
       LockerGuard<Mutex> lock(_mutex);
       if (!_signaled) {
           _signaled = true;
           pthread_cond_broadcast(&_cond);
       }
   }
   void reset()
   {
       LockerGuard<Mutex> lock(_mutex);
       _signaled = false;
   }
   void wait()
   {
       LockerGuard<Mutex> lock(_mutex);
       if (!_signaled) {
           ++_consumer;
           pthread_cond_wait(&_cond,(pthread_mutex_t*)_mutex.native_handle());  //todo
           --_consumer;
           //assert(_signaled);
       }
       if (!_manual_reset &&_consumer == 0) _signaled = false;
   }
   bool wait(unsigned ms)
   {
       LockerGuard<Mutex> lock(_mutex);
       if (!_signaled) 
       {
#ifdef __linux__
           struct timespec ts;
           clock_gettime(CLOCK_MONOTONIC, &ts);
#else
           struct timeval tv;
           gettimeofday(&tv, NULL);
           struct timespec ts;
           ts.tv_sec = tv.tv_sec;
           ts.tv_nsec = tv.tv_usec * 1000;
#endif

           if (ms < 1000) {
               ts.tv_nsec += ms * 1000000;
           } else {
               unsigned int x = ms / 1000;
               ts.tv_nsec += (ms - x * 1000) * 1000000;
               ts.tv_sec += x;
           }

           if (ts.tv_nsec > 999999999) {
               ts.tv_nsec -= 1000000000;
               ++ts.tv_sec;
           }

           ++_consumer;
           int r = pthread_cond_timedwait(&_cond,(pthread_mutex_t*)_mutex.native_handle(),&ts);
           --_consumer;

           if (r == ETIMEDOUT) return false;
           //assert(r == 0);
           //assert(_signaled);
       }

       if (!_manual_reset && _consumer == 0) _signaled = false;
       return true;
   }
#endif
};

void* SyncEvent_ConstructPrivate(SyncEvent* parent)
{
#ifdef _WIN32
    HANDLE ph= CreateEvent(0,parent->_manual_reset,parent->_signaled,0);
    assert(ph!= INVALID_HANDLE_VALUE);
    return ph;
#else

    const int size=sizeof(SyncEventPrivate);//可以通过编译器提示获取大小并留余量
    static_assert(SyncEvent::PrivateSizeMax>=sizeof(SyncEventPrivate),"Not enough reserved space");
    new(parent->pBuf) SyncEventPrivate;   //在原地址构造
    SyncEventPrivate* p=(SyncEventPrivate*)parent->pBuf;
    p->_manual_reset=parent->_manual_reset;
    p->_signaled=parent->_signaled;
    p->_consumer=0;//new add
    return parent->pBuf;
#endif
}

void SyncEvent_DestructPrivate(void*& pHnd)
{
#ifdef _WIN32
    if (pHnd!=NULL)
    {
        CloseHandle(pHnd);
        pHnd=NULL;
    }
#else
    //pthread_cond_destroy(&_cond);
    ((SyncEventPrivate*)pHnd)->~SyncEventPrivate();
#endif
}
void SyncEvent_signal(void* pHnd)
{
#ifdef _WIN32
    SetEvent(pHnd);
#else
    SyncEventPrivate* p=(SyncEventPrivate*)pHnd;
    p->signal();
#endif
}
void SyncEvent_reset(void* pHnd)
{
#ifdef _WIN32
    ResetEvent(pHnd);
#else
    SyncEventPrivate* p=(SyncEventPrivate*)pHnd;
    p->reset();
#endif
}
void SyncEvent_wait(void* pHnd)
{
#ifdef _WIN32
    WaitForSingleObject(pHnd, INFINITE);
#else
     SyncEventPrivate* p=(SyncEventPrivate*)pHnd;
     p->wait();
#endif
}
bool SyncEvent_wait(void* pHnd,unsigned int ms)
{
#ifdef _WIN32
    return WaitForSingleObject(pHnd,ms) == WAIT_OBJECT_0;

#else
    SyncEventPrivate* p=(SyncEventPrivate*)pHnd;
    return p->wait(ms);
#endif
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#if defined(OS_WIN)  

void* Semaphore_Create(Semaphore* parent)
{
    void* pHand=NULL;
    Tstring sName=FromCstr(parent->m_name);
    //获取是否存在信号量
    pHand =  OpenSemaphore(SEMAPHORE_ALL_ACCESS,FALSE,sName.c_str());	
    //不存在则创建新的信号量
    if (pHand==NULL)
    {
        pHand=CreateSemaphore(NULL,parent->m_InitCount,parent->m_MaxCount,sName.c_str());
        if (pHand==NULL)
        {
            PreDprintf("failed! 创建新的跨进程信号量失败!:%s\n",TstringTostring(sName).c_str());
            //return false;  //::exit(-1);
            return pHand;
        }
    }
    parent->currCount=parent->m_InitCount;
    //return true;
    return pHand;
}

void Semaphore_Destory(Semaphore* parent,void*& pHand)
{
    if (pHand!=NULL)
    {
        CloseHandle(pHand);
        pHand=NULL;
        parent->currCount=0;
    }
}
bool Semaphore_Wait(Semaphore* parent,void* pHand,unsigned int timeout)
{
    if (pHand==NULL)
    {
        return false;
    }

    DWORD  ret=WaitForSingleObject (pHand,timeout);
#if 0
    switch (ret)
    {
    case WAIT_OBJECT_0://表示等待的对象（比如线程、互斥体）已的正常执行完成或完成释放。
        break;
    case WAIT_TIMEOUT://表示你等待的对象没完成之前，由 WaitForSingleObject 设置的时间已经超时。
        break;
    case WAIT_FAILED://针对等待对象是互斥体的情况，当互斥体对象虽然没有被占用它的线程释放，
        //但是占用它的线程已提前中止时，WaitForSingleObject 就返回此值。// 函数调用失败，比如传递了一个无效的句柄
        printf("MySemaphore Wait() WAIT_FAILED\n");
        break;
    }
#endif
    if(ret==WAIT_OBJECT_0)
    {
        --parent->currCount;
        return true;
    }
    return false;
}
bool Semaphore_Signal(Semaphore* parent,void* pHand,unsigned int lReleaseCount)
{
    if (pHand==NULL)
    {
        return false;
    }

    long* lpPreviousCount = NULL;
    BOOL ret=ReleaseSemaphore(pHand, lReleaseCount,lpPreviousCount);
    if (ret==TRUE)
    {
        ++parent->currCount;
        //--currCount;
        return true;
    }
    return false;
}

#else

/*
函数原型：sem_t *sem_open(const char *name,int oflag,mode_t mode,unsigned int value);
name 信号灯的外部名字
oflag 选择创建或打开一个现有的信号灯
mode 权限位
value 信号灯初始值
返回值：
成功时返回指向信号灯的指针,出错时为SEM_FAILED
oflag参数可以是0、O_CREAT（创建一个信号量）或O_CREAT|O_EXCL（如果没有指定的信号量就创建），如果指定了O_CREAT，那么第三个和第四个参数是需要的；其中mode参数指定权限位，value参数指定信号量的初始值，通常用来指定共享资源的书面。该初始不能超过SEM_VALUE_MAX，这个常值必须低于为32767。二值信号量的初始值通常为1，计数信号量的初始值则往往大于1。
如果指定了O_CREAT（而没有指定O_EXCL），那么只有所需的信号量尚未存在时才初始化它。所需信号量已存在条件下指定O_CREAT不是一个错误。该标志的意思仅仅是“如果所需信号量尚未存在，那就创建并初始化它”。但是所需信号量等已存在条件下指定O_CREAT|O_EXCL却是一个错误。
sem_open返回指向sem_t信号量的指针，该结构里记录着当前共享资源的数目。

int sem_init(sem_t *sem, int pshared, unsigned int value);
sem ：指向信号量对象
pshared : 指明信号量的类型。不为0时此信号量在进程间共享，否则只能为当前进程的所有线程共享。
value : 指定信号量值的大小
返回值
sem_init() 成功时返回 0；错误时，返回 -1，并把 errno 设置为合适的值。

//2,Posix的有名信号量一般用于进程同步, 有名信号量是内核持续的. 有名信号量的api为：sem_open、sem_close、sem_unlink
//3,Posix的无名信号量一般用于线程同步, 无名信号量是进程持续的, 无名信号量的api为：sem_init、sem_destroy
*/

void* Semaphore_Create(Semaphore* parent)
{
    void* pHand=NULL;
    const char* key_name=parent->m_name;
    if (isNullCstr(key_name))   //无名信号量的实现
    {
        //pHand=(sem_t*)new sem_t;
        //if (pHand==NULL)
        //{
        //    return NULL;//return false;
        //}

        const int size=sizeof(sem_t);//可以通过编译器提示获取大小并留余量
        static_assert(Semaphore::PrivateSizeMax>=sizeof(sem_t),"Not enough reserved space");

        new(parent->pBuf) sem_t;   //不new 空间，在类内原地构造
        pHand=parent->pBuf;
        int res=sem_init((sem_t*)pHand,0,parent->m_InitCount);
        if (res!=0)
        {
             PRINT_ERROR_LINE
            printf("sem_init() failed name:%s initCount:%u errno:%d %s\n",key_name,parent->m_InitCount,errno,strerror(errno));
             return NULL;//return false;
        }
        return pHand;// return true;
    }

    pHand = (void*)sem_open(parent->m_name,0);
    if (pHand==NULL)
    {
        //printf("sem_open() failed name:%s errno:%d %s\n",key_name,errno,strerror(errno));

        //进程间可见的命名信号实现  O_CREAT|O_EXCL   //|O_RDWR
        pHand = (void*)sem_open(parent->m_name,O_RDWR|O_CREAT,0644,parent->m_InitCount);
        if (pHand==NULL) //#define SEM_FAILED      ((sem_t *) 0)
        {
            PRINT_ERROR_LINE
                printf("sem_open() failed name:%s initCount:%u errno:%d %s\n",key_name,parent->m_InitCount,errno,strerror(errno));
            //return NULL;//return false;
        }
    }
     return pHand;// return true;
}

void Semaphore_Destory(Semaphore* parent,void*& pHand)
{
    if(pHand!=NULL)
    {
        if (isNullCstr(parent->m_name))   //无名信号量的实现
        {
            sem_destroy((sem_t*)pHand);
            // error 'void*' is not a pointer-to-object type    //don'n need destruct
            //(sem_t*)pHand->~sem_t();     //delete (sem_t*)pHand;  //原地析构
        }
        else   //有名信号量的实现
        {
            int ret =sem_close((sem_t*)pHand);  //若成功则返回0，否则返回-1。
            if (ret!=0)
            {
                   printf("sem_close() failed ret:%d errno:%d %s\n",ret,errno,strerror(errno));
            }
            sem_unlink(parent->m_name);  //若成功则返回0，否则返回-1。
        }
        pHand=NULL;
    }
}


//时间a的值增加ms毫秒 悟空的博客www.7es.cn
static inline void timeraddMS(struct timeval *p, uint32_t ms)
{
    p->tv_usec +=ms*1000;
    if(p->tv_usec >= 1000000)
    {
        p->tv_sec+=p->tv_usec/1000000;
        p->tv_usec%=1000000;
    }
}

static inline void add_timer_ms(struct timespec *p, uint32_t ms)
{
    struct timeval now;
    gettimeofday(&now,NULL);
    timeraddMS(&now,ms);
    p->tv_sec=now.tv_sec;
    p->tv_nsec=now.tv_usec*1000;
}

bool Semaphore_Wait(Semaphore* parent,void* pHand,unsigned int timeout)
{
    if (pHand==NULL)
    {
         PRINT_ERROR_LINE
        return false;
    }

    //bug 备忘 待续
    //更完善的方法参见CSDN C/C++ 修改系统时间，导致sem_timedwait 一直阻塞的问题解决和分析

    if (timeout==UINT_MAX)   //无限等待的实现方式
    {
        int res=sem_wait((sem_t*)pHand);
        if (res==0)
        {
            --parent->currCount;
            return true;
        }
        return false;
    }
    struct timespec outtime;
    add_timer_ms(&outtime,timeout);
    int ret = sem_timedwait((sem_t*)pHand,&outtime); //成功返回0. 出错返回-1，并设置errno指明具体的错误
    if (ret==0)
    {
        --parent->currCount;
        return true;
    }
    else
    {
            //PRINT_ERROR_LINE
            //printf("sem_timewait() failed ret:%d errno:%d %s\n",ret,errno,strerror(errno));
    }
    return false;
}
bool Semaphore_Signal(Semaphore* parent,void* pHand,unsigned int lReleaseCount)
{
    if (pHand==NULL)
    {
        PRINT_ERROR_LINE

        return false;
    }
    int res=sem_post((sem_t*)pHand);//成功时返回 0；错误时，信号量的值没有更改，-1 被返回，并设置 errno 来指明错误。
    if (res==0)
    {
        ++parent->currCount;
        return true;
    }
    PRINT_ERROR_LINE
    return false;
}
#endif


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//基于C++11的各种线程同步机制扩展实现 
//https://www.zhihu.com/question/31555101/answer/117537944
//C++11 中没有信号量，如何进行同步或互斥？
//condition_variable+mutex啊，pthread世界一直是那么用的。


//QSemaphore实现参考
#if 0
class Q_CORE_EXPORT QSemaphore
{
public:
    explicit QSemaphore(int n = 0);
    ~QSemaphore();
    void acquire(int n = 1);
    bool tryAcquire(int n = 1);
    bool tryAcquire(int n, int timeout);
    void release(int n = 1);
    int available() const;
private:
    Q_DISABLE_COPY(QSemaphore)
    QSemaphorePrivate *d;
};

class QSemaphorePrivate {
public:
    inline QSemaphorePrivate(int n) : avail(n) { }
    QMutex mutex;
    QWaitCondition cond;
    int avail;
};

/*!
    Creates a new semaphore and initializes the number of resources
    it guards to \a n (by default, 0).

    \sa release(), available()
*/
QSemaphore::QSemaphore(int n)
{
    Q_ASSERT_X(n >= 0, "QSemaphore", "parameter 'n' must be non-negative");
    d = new QSemaphorePrivate(n);
}

/*!
    Destroys the semaphore.

    \warning Destroying a semaphore that is in use may result in
    undefined behavior.
*/
QSemaphore::~QSemaphore()
{ delete d; }

/*!
    Tries to acquire \c n resources guarded by the semaphore. If \a n
    > available(), this call will block until enough resources are
    available.

    \sa release(), available(), tryAcquire()
*/
void QSemaphore::acquire(int n)
{
    Q_ASSERT_X(n >= 0, "QSemaphore::acquire", "parameter 'n' must be non-negative");
    QMutexLocker locker(&d->mutex);
    while (n > d->avail)
        d->cond.wait(locker.mutex());
    d->avail -= n;
}

/*!
    Releases \a n resources guarded by the semaphore.

    This function can be used to "create" resources as well. For
    example:

    \snippet code/src_corelib_thread_qsemaphore.cpp 1

    \sa acquire(), available()
*/
void QSemaphore::release(int n)
{
    Q_ASSERT_X(n >= 0, "QSemaphore::release", "parameter 'n' must be non-negative");
    QMutexLocker locker(&d->mutex);
    d->avail += n;
    d->cond.wakeAll();
}

/*!
    Returns the number of resources currently available to the
    semaphore. This number can never be negative.

    \sa acquire(), release()
*/
int QSemaphore::available() const
{
    QMutexLocker locker(&d->mutex);
    return d->avail;
}

/*!
    Tries to acquire \c n resources guarded by the semaphore and
    returns \c true on success. If available() < \a n, this call
    immediately returns \c false without acquiring any resources.

    Example:

    \snippet code/src_corelib_thread_qsemaphore.cpp 2

    \sa acquire()
*/
bool QSemaphore::tryAcquire(int n)
{
    Q_ASSERT_X(n >= 0, "QSemaphore::tryAcquire", "parameter 'n' must be non-negative");
    QMutexLocker locker(&d->mutex);
    if (n > d->avail)
        return false;
    d->avail -= n;
    return true;
}

/*!
    Tries to acquire \c n resources guarded by the semaphore and
    returns \c true on success. If available() < \a n, this call will
    wait for at most \a timeout milliseconds for resources to become
    available.

    Note: Passing a negative number as the \a timeout is equivalent to
    calling acquire(), i.e. this function will wait forever for
    resources to become available if \a timeout is negative.

    Example:

    \snippet code/src_corelib_thread_qsemaphore.cpp 3

    \sa acquire()
*/
bool QSemaphore::tryAcquire(int n, int timeout)
{
    Q_ASSERT_X(n >= 0, "QSemaphore::tryAcquire", "parameter 'n' must be non-negative");
    QMutexLocker locker(&d->mutex);
    if (timeout < 0) {
        while (n > d->avail)
            d->cond.wait(locker.mutex());
    } else {
        QElapsedTimer timer;
        timer.start();
        while (n > d->avail) {
            const qint64 elapsed = timer.elapsed();
            if (timeout - elapsed <= 0
                || !d->cond.wait(locker.mutex(), timeout - elapsed))
                return false;
        }
    }
    d->avail -= n;
    return true;
}
#endif






NAME_SPACE_MAIN_END


#include <thread>
//读写锁测试代码
//static shared_mutex sm;
//void read(int id)
//{
//    AutoReadLocker lck(sm);
//    printf("%d r \n",id);
//    std::this_thread::sleep_for(chrono::milliseconds(300));
//    printf("%d r end \n",id);
//}
//
//void write(int id)
//{
//    AutoWriteLocker lck(sm);
//    printf("%d w \n", id);
//    std::this_thread::sleep_for(chrono::milliseconds(300));
//    printf("%d w end \n", id);
//}
//
//int test_read_write()
//{
//    thread t1(read,1);
//    thread t2(write,2);
//    thread t3(write,3);
//    thread t4(read,4);
//    t1.join();
//    t2.join();
//    t3.join();
//    t4.join();
//    return 0;
//}
//



static const int kIncNum =100*1000000;   //每个线程自增次数
static const int kWorkerNum=4;                //线程数
static unsigned int tCount=0;                     //自增计数器

//static SpinLock m_mutexInc;     //自旋锁 //4.6s
//static std::mutex m_mutexInc;    //104s--比系统的慢
//static std::recursive_mutex m_mutexInc;    //
//static Mutex m_mutexInc;        //52s
static spinlock m_mutexInc;        //52s


//WINDOWS VS2012  std::mutex比Mutex慢一倍
//ARM  std::mutex比Mutex快1.2倍

//每个线程的工作函数
static void IncCounter()
{
    for (int i = 0; i < kIncNum; ++i)
    {
        m_mutexInc.lock();
        tCount++;
        m_mutexInc.unlock();
    }
}

#include <vector>
#include "SuperTime.h"
static bool testMutex()
{
    std::vector<std::thread> ListThread;
    printf("Mutex Test Begin\n");
    tCount = 0;

    printf("Start: workThreadNum:%d every worker inc:%d count:%u\n",kWorkerNum,kIncNum,tCount);

    //自动耗时打印
    SuperTime tm("testMutex");
    SuperTimePrintHelper print(tm);

    //创建10个工作线程进行自增操作
    for (int i = 0; i < kWorkerNum; ++i)
    {
        ListThread.push_back(std::move(std::thread(IncCounter)));
    }
    for (auto it = ListThread.begin(); it != ListThread.end(); it++)
    {
        it->join();
    }

    printf("End: count:%u\n",tCount);

    //验证结果
    if (tCount==kIncNum*kWorkerNum)
    {
        printf("Mutex Test passed\n");
        return true;
    }
    else
    {
        printf("Mutex Test failed\n");
        return false;
    }
}


#ifdef UNIT_TEST
 int Test_ThreadSync()
{
    testMutex();
    return 0;
}

#endif

