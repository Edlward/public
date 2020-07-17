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


//#if __cplusplus >= 201103L  
//#include <random>
//#endif 


////跨平台的快速锁--windows使用临界区实现不能跨进程  允许加锁线程递归加锁
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
    CRITICAL_SECTION    m_Cs;        //临界区  windows临界区默认只能递归,
#elif defined(OS_LINUX)  
    pthread_mutex_t     m_mutex;                        //互斥锁  
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
void Mutex_Lock(MutexPrivate* pimpl)
{
#if defined(OS_WIN)  
    EnterCriticalSection(&pimpl->m_Cs);  
#else // defined(OS_LINUX)  
    pthread_mutex_lock(&pimpl->m_mutex);
#endif
}
bool Mutex_TryLock(MutexPrivate* pimpl)
{
#if defined(OS_WIN)  
    if (!TryEnterCriticalSection(&pimpl->m_Cs))  
    {  
        return false;  
    }  
#else //defined(OS_LINUX)  
    if(pthread_mutex_trylock(&pimpl->m_mutex)!=0)  
    {  
        return false;  
    }  
#endif  
    return true;
}
void Mutex_UnLock(MutexPrivate* pimpl)
{
#if defined(OS_WIN)  
    LeaveCriticalSection(&pimpl->m_Cs);  
#else //defined(OS_LINUX)  
    pthread_mutex_unlock(&pimpl->m_mutex);  
#endif
}
 


void* MutexNoRecursive_Create()
{
    void* pHand=NULL;
#if defined(OS_WIN)  
    pHand=CreateMutex(NULL,FALSE,NULL);
#else

    //对于pthread_mutex_t类型的全局变量，只能用PTHREAD_MUTEX_INITIALIZER来初始化。
    //对于pthread_mutex_t类型的静态变量，使用pthread_mutex_init(pthread_mutex_t&, NULL)来初始化。

    pHand=new pthread_mutex_t;
    pthread_mutexattr_t attr;  
    pthread_mutexattr_init(&attr);  
    pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);   //recursive lock
    pthread_mutex_init((pthread_mutex_t*)pHand,&attr);  
#endif
    return pHand;
}
void MutexNoRecursive_Destory(void*& pHand)
{
#if defined(OS_WIN)  
    if (pHand!=NULL)
    {
        CloseHandle(pHand);
        pHand=NULL;
    }
#else // defined(OS_LINUX)  
    if (pHand!=NULL)
    {
        pthread_mutex_destroy((pthread_mutex_t*)pHand);
        delete (pthread_mutex_t*)pHand;
        pHand=NULL;
    }
#endif
}
void MutexNoRecursive_Lock(void* pHand)       //加锁  
{
    if (pHand==NULL)
    {
        return;
    }

#if defined(OS_WIN)  
    DWORD nRet = WaitForSingleObject(pHand,INFINITE);
    if (nRet!=WAIT_OBJECT_0)
    {
        printf("Error at: ProcessMutex::Lock()\n");
        //return false;
    }
    //return true;
#else // defined(OS_LINUX)  
    pthread_mutex_lock((pthread_mutex_t*)pHand);  
#endif
}
bool MutexNoRecursive_TryLock(void* pHand)   //非阻塞 枷锁成功返回true 否则返回false  
{
    if (pHand==NULL)
    {
        return false;
    }

#if defined(OS_WIN)  
    DWORD nRet = WaitForSingleObject(pHand,5);  //暂时默认5ms
    if (nRet !=WAIT_OBJECT_0)
    {
        //printf("Error at: ProcessMutex::Lock()\n");
        return false;
    }
    return true;
#else //defined(OS_LINUX)  
    if(pthread_mutex_trylock((pthread_mutex_t*)pHand)!=0)
    {  
        return false;  
    }  
#endif  
    return true;
}
void MutexNoRecursive_UnLock(void* pHand)    //解锁  
{
    if (pHand==NULL)
    {
        return;
    }
#if defined(OS_WIN)  
    ReleaseMutex(pHand);
#else //defined(OS_LINUX)  
    pthread_mutex_unlock((pthread_mutex_t*)pHand);  
#endif
}



#if defined(OS_WIN)  
void* MutexBetweenProcess_Create(MutexBetweenProcess* parent)
{
    Tstring sName=FromCstr(parent->m_name);
    void* pHand = OpenMutex(MUTEX_ALL_ACCESS,FALSE,sName.c_str());
    if (pHand==NULL)
    {
        pHand = CreateMutex(NULL,FALSE,sName.c_str());
        if(pHand==NULL)
        {
            printf("创建新的跨进程互斥锁失败! :%s\n",TstringTostring(sName).c_str());
            //return false;  //exit(-1);
             return pHand;
        }
    }
    else
    {
        parent->isExist=true;  //已经存在
    }
    //return true;
    return pHand;
}

void MutexBetweenProcess_Destory(void*& pHand)
{
    if (pHand!=NULL)
    {
        CloseHandle(pHand);
        pHand=NULL;
    }
}
bool MutexBetweenProcess_Lock(void* pHand)        //加锁  
{
    //互斥锁创建失败
    if (pHand==NULL)
    {
        return false;
    }
    DWORD nRet = WaitForSingleObject(pHand,INFINITE);
    if (nRet!=WAIT_OBJECT_0)
    {
        printf("Error at: ProcessMutex::Lock()\n");
        return false;
    }
    return true;
}
bool MutexBetweenProcess_TryLock(void* pHand,unsigned int ms)   //非阻塞 枷锁成功返回true 否则返回false  
{
    //互斥锁创建失败
    if (pHand==NULL)
    {
        return false;
    }
    DWORD nRet = WaitForSingleObject(pHand,ms);
    if (nRet!=WAIT_OBJECT_0)
    {
        printf("Error at: ProcessMutex::Lock()\n");
        return false;
    }
    return true;
}
bool MutexBetweenProcess_UnLock(void* pHand)   //解锁  
{
    if (pHand==NULL)
    {
        return false;
    }
    return (ReleaseMutex(pHand)!=0);
}
 

#else //OS_LINUX


void* MutexBetweenProcess_Create(MutexBetweenProcess* parent)
{
       //不存在再创建 测试待续
       void* pHand = (void*)sem_open(m_name,0)
        if (pHand==NULL)
        {
            pHand = (void*)sem_open(m_name,O_RDWR|O_CREAT,0644,1);    //作为互斥锁,信号量设置为1
            if (pHand==NULL)
            {
                return NULL;  //return false;
            }
        }
        else
        {
            isExist=true;
        }
        //return true;
        return pHand;
}

void MutexBetweenProcess_Destory(void*& pHand)
{
    if (pHand!=NULL)
    {
        int ret =sem_close((sem_t*)pHand);
        if (ret!=0)
        {
            printf("sem_close error %d\n",ret);
        }
        sem_unlink(m_name);
        pHand=NULL;
    }
}
bool MutexBetweenProcess_Lock(void* pHand)       //加锁  
{
    if (pHand==NULL)
    {
        return false;
    }

    int ret = sem_wait((sem_t*)pHand);
    if (ret!=0)
    {
        return false;
    }
    return true;
}
bool MutexBetweenProcess_TryLock(void* pHand,unsigned int ms)   //非阻塞 枷锁成功返回true 否则返回false  
{
    if (pHand==NULL)
    {
        return false;
    }
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME,&ts)<0)
    {
        return false;
    }

    //用的是UTC时间,正在等待的时候管理员调整时间会出问题 系统bug，其他办法研究待续 测试待续
    int32 nano_sec=(ms%1000)*1000;  //ms 转s和ns 
    int32 sec=ms/1000;

    ts.tv_sec+=sec;
    ts.tv_nsec+=nano_sec;

    //#define NSECTOSEC    1000000000 
    ts.tv_sec += ts.tv_nsec/NSECTOSEC; //Nanoseconds [0 .. 999999999]
    ts.tv_nsec = ts.tv_nsec%NSECTOSEC;

    int ret = sem_timedwait((sem_t*)pHand,&ts);
    if (ret!=0)
    {
        return false;
    }
    return true;
}

bool MutexBetweenProcess_UnLock(void* pHand)    //解锁  
{
    if (pHand==NULL)
    {
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
            printf("创建新的跨进程信号量失败! :%s\n",TstringTostring(sName).c_str());
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

void* Semaphore_Create(Semaphore* parent)
{
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

    void* pHand=NULL;
    if (isNullCstr(parent->m_name))   //无名信号量的实现
    {
        pHand=(sem_t*)new sem_t;
        if (pHand==NULL)
        {
             return NULL;//return false;
        }
        int res=sem_init((sem_t*)pHand,0,parent->m_InitCount);
        if (res!=0)
        {
             return NULL;//return false;
        }
        return pHand;// return true;
    }

    //进程间可见的命名信号里
    pHand = (void*)sem_open(parent->m_name,O_CREAT|O_RDWR,0644,parent->m_InitCount);
    if (pHand==NULL)
    {
        return NULL;//return false;
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
            delete (sem_t*)pHand;
        }
        else
        {
            int ret =sem_close((sem_t*)pHand);  //若成功则返回0，否则返回-1。
            if (ret!=0)
            {
                printf("sem_close error %d\n",ret);
            }
            sem_unlink(parent->m_name);  //若成功则返回0，否则返回-1。
        }
        pHand=NULL;
    }
}


//时间a的值增加ms毫秒 悟空的博客www.7es.cn
inline void timeraddMS(struct timeval *p, uint32_t ms)
{
    p->tv_usec +=ms*1000;
    if(p->tv_usec >= 1000000)
    {
        p->tv_sec+=p->tv_usec/1000000;
        p->tv_usec%=1000000;
    }
}

inline void add_timer_ms(struct timespec *p, uint32_t ms)
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
    return false;
}
bool Semaphore_Signal(Semaphore* parent,void* pHand,unsigned int lReleaseCount)
{
    if (pHand==NULL)
    {
        return false;
    }
    int res=sem_post((sem_t*)pHand);//成功时返回 0；错误时，信号量的值没有更改，-1 被返回，并设置 errno 来指明错误。
    if (res==0)
    {
        ++parent->currCount;
        return true;
    }
    return false;
}
#endif



NAME_SPACE_MAIN_END