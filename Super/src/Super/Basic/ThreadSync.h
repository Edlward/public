#ifndef ThreadSyncSys_H_
#define ThreadSyncSys_H_

#include "global.h"


//各种线程同步锁,以声明方式前置隔离实现,可无害用于模板中  //creation by lizulin 2015-2020
//避免在.h 中引入windows.h 之类系统头文件,大型项目包含复杂后可能遇到很多定义冲突编译错误的问题。 

/*
本文件主要实现跨平台的线程同步操作类，(暂时只考虑windows与Linux)包括:
互斥量:      mutex, 递归锁等,读写互斥锁 写优先 
信号量:      Semaphore,
条件变量:   Condtion Variable
//跨进程跨平台:(进程之间传递数据，消息)互斥锁信号量等
部分实现基于c++11
*/


NAME_SPACE_MAIN_BEGIN

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//拷贝字符串到数组
inline void copyStr(char* dst,size_t dstSizeMax,const char* src)
{
    if (dst==NULL) return;
    memset(dst,0,dstSizeMax);
    if (src==NULL||*src=='\0') return;
    size_t srcLen=::strlen(src);
    size_t copyLen=(srcLen>dstSizeMax)?dstSizeMax:srcLen;
    strncpy(dst,src,copyLen);
    dst[dstSizeMax-1]='\0';
}


//
//typedef union pthread_mutex_t;   //error


//互斥锁 递归锁 对应std::recursive_mutex 
class MutexPrivate;
extern MutexPrivate* Mutex_CreatPrivate();                                         //创建
extern  void Mutex_DestoryPrivate(MutexPrivate* p);                           //销毁
extern  void Mutex_ConstructPrivate(MutexPrivate* p,void*& pHnd);   //构造
extern  void Mutex_DestructPrivate(MutexPrivate* p);                          //析构
extern  void Mutex_Lock(void* pHnd);         //加锁  
extern  bool Mutex_TryLock(void* pHnd);    //非阻塞 枷锁成功返回true 否则返回false  
extern  void Mutex_UnLock(void* pHnd);     //解锁  

class Mutex
{  
public:  
    Mutex()
    {
        CreatPrivate();
    }
    ~Mutex()  
    {  
        DestoryPrivate();
    }
private:
    void CreatPrivate()
    {
        //pimpl=::Mutex_CreatPrivate();
        ::Mutex_ConstructPrivate((MutexPrivate*)pBuf,pHnd);
    }
    void DestoryPrivate()
    {
        //::Mutex_DestoryPrivate(pimpl);
        ::Mutex_DestructPrivate((MutexPrivate*)pBuf);
    }
public:

    //保留与STL同名接口
    void lock(){return Lock();}
    bool try_lock(){return TryLock();}
    void unlock(){return UnLock();}

    void Lock()        //加锁  
    {
         ::Mutex_Lock(pHnd);
    }
    bool TryLock()   //非阻塞 枷锁成功返回true 否则返回false
    {
         return ::Mutex_TryLock(pHnd);
    }
    void UnLock()    //解锁  
    {
         Mutex_UnLock(pHnd);
    }

    //native handle TODO://windows:CRITICAL_SECTION uinux:pthread_mutex_t
    void* native_handle()
    {
        return pHnd;  //NOTE if refactor //重构时注意指向真实的本地句柄值
    }

    //TODO:强制销毁,即使正在被另一个线程使用,也会立即退出 特殊场景使用
    void doForceDestory()
    {
        ::Mutex_DestructPrivate((MutexPrivate*)pBuf);       
    }
public:
    void Reinit()   //为了在特殊用途重新构造，一般情况不需要
    {
        DestoryPrivate();
        CreatPrivate();
    }
public:
private:
    //MutexPrivate* pimpl;
    enum 
    {
        PrivateSizeMax=64,   //
    };
    char pBuf[PrivateSizeMax];  //为了极致性能,不new空间,在类内部构造
    void* pHnd; //极致性能优化，直接记住句柄值，让lock，unclock更快
private:  
    friend MutexPrivate* Mutex_CreatPrivate();
    friend void Mutex_DestoryPrivate(MutexPrivate* p);
    friend void Mutex_ConstructPrivate(MutexPrivate* p,void*& pHnd);
    friend void Mutex_DestructPrivate(MutexPrivate* p);
    friend void Mutex_Lock(void* pHnd);        //加锁
    friend bool Mutex_TryLock(void* pHnd);   //非阻塞 枷锁成功返回true 否则返回false  
    friend void Mutex_UnLock(void* pHnd);    //解锁
private:  
    Mutex(const Mutex&);                   // 禁止拷贝和赋值构造
    Mutex& operator=(const Mutex&);             
};


class MutexNoRecursivePrivate;
extern MutexNoRecursivePrivate* MutexNoRecursive_CreatePrivate();
extern void MutexNoRecursive_DestoryPrivate(MutexNoRecursivePrivate* p);
extern void MutexNoRecursive_ConstructPrivate(MutexNoRecursivePrivate* p,void*& pHnd);
extern void MutexNoRecursive_DestructPrivate(MutexNoRecursivePrivate* p);
extern  void MutexNoRecursive_Lock(void* pHnd);
extern  bool MutexNoRecursive_TryLock(void* pHnd);
extern  void MutexNoRecursive_UnLock(void* pHnd);

//互斥锁 非递归锁 对应std::mutex   
class MutexNoRecursive
{  
public:  
    MutexNoRecursive()
    {
        if(!Create())
        {
            printf("互斥锁创建失败\n");
            PRINT_ERROR_LINE
            //exit(-1);
        }
    }
    ~MutexNoRecursive()  
    {  
        Destory();
    }

    //保留与STL同名接口
    void lock(){return Lock();}
    bool try_lock(){return TryLock();}
    void unlock(){return UnLock();}


    void Lock()        //加锁  
    {
        ::MutexNoRecursive_Lock(pHnd);
    }
    bool TryLock()   //非阻塞 枷锁成功返回true 否则返回false  
    {
        return ::MutexNoRecursive_TryLock(pHnd);
    }
    void UnLock()    //解锁  
    {
        ::MutexNoRecursive_UnLock(pHnd);
    }
public:
    void Reinit()   //为了在特殊用途重新构造，一般情况不需要
    {
        Destory();
        Create();
    }
private:
    bool Create()
    {
        MutexNoRecursive_ConstructPrivate((MutexNoRecursivePrivate*)pBuf,pHnd);
        return true;
    }
    void Destory()
    {
        ::MutexNoRecursive_DestructPrivate((MutexNoRecursivePrivate*)pBuf);
    }
private:
     enum 
     {
         PrivateSizeMax=64,   //
     };
     char pBuf[PrivateSizeMax];  //为了极致性能,不new空间,在类内部构造
    void* pHnd; //极致性能优化，直接记住句柄值，让lock，unclock更快
private:  
    friend MutexNoRecursivePrivate* MutexNoRecursive_CreatePrivate();
    friend void MutexNoRecursive_DestoryPrivate(MutexNoRecursivePrivate* p);
    friend void MutexNoRecursive_ConstructPrivate(MutexNoRecursivePrivate* p,void*& pHnd);
    friend void MutexNoRecursive_DestructPrivate(MutexNoRecursivePrivate* p);
    friend void MutexNoRecursive_Lock(void* pHnd);        //加锁  
    friend bool MutexNoRecursive_TryLock(void* pHnd);   //非阻塞 加锁成功返回true 否则返回false  
    friend void MutexNoRecursive_UnLock(void* pHnd);    //解锁  
private:  
    MutexNoRecursive(const MutexNoRecursive&);
    MutexNoRecursive& operator=(const MutexNoRecursive&);
};


class MutexProcess;
extern void* MutexProcess_Create(MutexProcess* parent);
extern  void MutexProcess_Destory(MutexProcess* parent,void*& pHand);
extern  bool MutexProcess_Lock(void* pHand);        //加锁  
extern  bool MutexProcess_TryLock(void* pHand,unsigned int ms);   //非阻塞 枷锁成功返回true 否则返回false  
extern  bool MutexProcess_UnLock(void* pHand);    //解锁  


//跨进程互斥锁，命名互斥锁。可以用于跨进程。当然进程内也可以
class MutexProcess
{
public:
    //默认创建匿名的互斥
    MutexProcess(const char* name="")
        :pHand(NULL),m_isExist(false)
    {
        m_name[0]='\0';
        copyStr(m_name,sizeof(m_name),name);   //拷贝名称到类内
        if (!Create())
        {
            //::exit(-1);
        }
    }
    ~MutexProcess()
    {
        Destory();
    }
    //保留与STL同名接口
    void lock(){ Lock();}
    bool try_lock(){return TryLock();}
    void unlock(){ UnLock();}

    bool Lock()
    {
        return ::MutexProcess_Lock(pHand);        //加锁  
    }
    bool TryLock(unsigned int ms=5)
    {
        return ::MutexProcess_TryLock(pHand,ms);   //非阻塞 枷锁成功返回true 否则返回false  
    }
    bool UnLock()
    {
        return ::MutexProcess_UnLock(pHand);    //解锁  
    }
    //TODO:强制销毁,即使正在被另一个线程使用,也会立即退出 特殊场景使用
    void doForceDestory()
    {
        ::MutexProcess_Destory(this,pHand);
    }
public:
    void Reinit()   //为了在特殊用途重新构造，一般情况不需要
    {
        Destory();
        Create();
    }
    bool isExist()const {return m_isExist;}

    //返回这个跨进程锁的名字--一些情况需要
    //std::string name()const { return m_name;}
private:
    bool Create()
    {
        pHand=::MutexProcess_Create(this);
        bool bRet=(pHand!=0);
        if (!bRet)
        {
            printf("Mutex::Create failed! name:%s\n",m_name);
            PRINT_ERROR_LINE
        }
        return bRet;
    }
    void Destory()
    {
        ::MutexProcess_Destory(this,pHand);
    }
private:
    void* pHand;
    char m_name[128];   //don't use std::string
    //完善待续
    bool m_isExist;     //是否存在同名命名互斥锁,创建者为false,后来的使用者都为true
private:
    friend void* MutexProcess_Create(MutexProcess* parent);
    friend void MutexProcess_Destory(MutexProcess* parent,void*& pHand);
    friend bool MutexProcess_Lock(void* pHand);        //加锁  
    friend bool MutexProcess_TryLock(void* pHand,unsigned int ms);   //非阻塞加锁
    friend bool MutexProcess_UnLock(void* pHand);    //解锁  
private:
    MutexProcess(const MutexProcess&);
    MutexProcess& operator=(const MutexProcess&);
};




// 加锁辅助工具 --模版实现 
template<class MutexType>  
class LockerGuard  
{  
public:  
    LockerGuard(MutexType& locker )   
        : m_locker(locker)  
    {  
        locker.lock();  
    }  
    ~LockerGuard()  
    {  
        m_locker.unlock();  
    }  
protected:  
    MutexType& m_locker;  
private:
private:
    LockerGuard(const LockerGuard&);
    LockerGuard& operator=(const LockerGuard&);
};  

typedef LockerGuard<Mutex> AutoLocker;
typedef LockerGuard<MutexProcess> AutoProcessLocker; 


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class SyncEvent;
extern void* SyncEvent_ConstructPrivate(SyncEvent* parent);
extern void SyncEvent_DestructPrivate(void*& pHnd);
extern void SyncEvent_signal(void* pHnd);
extern void SyncEvent_reset(void* pHnd);
extern void SyncEvent_wait(void* pHnd);
extern bool SyncEvent_wait(void* pHnd,unsigned int ms);



//改造为共享无害类并测试,待续
class SyncEventPrivate;
class SyncEvent
{
public:
    explicit SyncEvent(bool manual_reset = false, bool signaled = false)
        : _manual_reset(manual_reset),_signaled(signaled),pHnd(0)
    {
        pHnd=SyncEvent_ConstructPrivate(this);
    }
    ~SyncEvent()
    {
        SyncEvent_DestructPrivate(pHnd);
    }
    void signal()
    {
          SyncEvent_signal(pHnd);
    }
    void reset()
    {
          SyncEvent_reset(pHnd);
    }
    void wait()
    {
          SyncEvent_wait(pHnd);
    }
    bool wait(unsigned int ms)
    {
          return SyncEvent_wait(pHnd,ms);
    }
private:
    //SyncEventPrivate* pImpl;
    const bool _manual_reset;
    bool _signaled;
    void* pHnd;
#ifndef _WIN32
    enum 
    {
        PrivateSizeMax=128,   //
    };
    char pBuf[PrivateSizeMax];  //为了极致性能,不new空间,在类内部构造
#endif
private:
    friend void* SyncEvent_ConstructPrivate(SyncEvent* parent);
    friend void SyncEvent_DestructPrivate(void*& pHnd);
    friend void SyncEvent_signal(void* pHnd);
    friend void SyncEvent_reset(void* pHnd);
    friend void SyncEvent_wait(void* pHnd);
    friend bool SyncEvent_wait(void* pHnd,unsigned int ms);
private:
    SyncEvent(const SyncEvent&);
    SyncEvent& operator=(const SyncEvent&);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//跨平台信号量

//C++11以上版本用std::mutex 和std::condition_variable (条件变量) 实现信号量
//失败 待续//(__cplusplus >= 201103L)   //跨进程 信号量不能用C++11实现 必须用系统API

//自定义信号量，跨平台，根据传递name是否为空，决定有名信号量还是命名信号量
//命名信号量可用于跨进程之间的同步


class Semaphore;
extern void* Semaphore_Create(Semaphore* parent);
extern  void Semaphore_Destory(Semaphore* parent,void*& pHand);
extern  bool Semaphore_Wait(Semaphore* parent,void* pHand,unsigned int timeout);
extern  bool Semaphore_Signal(Semaphore* parent,void* pHand,unsigned int lReleaseCount);


class Semaphore
{
public:
    Semaphore(unsigned int maxCount=1,unsigned int initCount=0,const char* name="")
        :m_MaxCount(maxCount),m_InitCount(initCount)
        ,currCount(0)
        ,pHand(NULL)
    {
        m_name[0]='\0';
        copyStr(m_name,sizeof(m_name),name);   //拷贝名称到类内
        if (!this->Create())  //信号里创建失败,退出程序
        {
            //::exit(-1);
        }
    }
    ~Semaphore()
    {
        Destory();
    }
    void ReInit()   //重新初始化，为了在ringbufer中使用是切换状态的时候复位到原来的信号量，一般情况不需要
    {
        Destory();       //关闭之后重新打开
        if (!this->Create())  //信号里创建失败,退出程序
        {
            //::exit(-1);
        }
    }

    void ReSet() //复位信号量为初始值
    {
            for(unsigned int k=0;k<m_MaxCount;k++)
            {
                if (!this->Wait(0))   //使用完所有资源Wait(0)或Wait(1)
                {
                    break;
                }
            }
            for(unsigned int k=0;k<m_InitCount;k++)
            {
                this->Signal();     //恢复初始资源
            }
    }

    bool Wait(unsigned int timeout=UINT32_MAX)   //UINT_MAX=INFINITE表示无限等待
    {
        return  Semaphore_Wait(this,pHand,timeout);
    }
    bool Signal(unsigned int lReleaseCount=1)
    {
        return Semaphore_Signal(this,pHand,lReleaseCount);
    }

    //定义相同功能的接口Acquire  Release更加直观,仿造Qt
    bool Acquire(unsigned int TimesWait=UINT32_MAX)
    {
        return this->Wait(TimesWait);
    }
    bool Release(unsigned int lReleaseCount=1U)
    {
        return this->Signal(lReleaseCount);
    }
    unsigned int AvailableCount()const {return currCount;}

    //TODO:强制销毁,即使正在被另一个线程使用,也会立即退出 特殊场景使用
    void doForceDestory()
    {
        this->Destory();
    }

    //返回这个信号量的名字--一些情况需要
    //std::string name()const { return m_name;}
private:
    bool Create()
    {
        pHand=Semaphore_Create(this);
        bool bRet=(pHand!=0);
        if(!bRet)
        {
            printf("Semaphore::Create failed! name:%s\n",m_name);
            PRINT_ERROR_LINE
        }
        return bRet;
    }
    void Destory()
    {
        ::Semaphore_Destory(this,pHand);
    }
private:
    unsigned int m_MaxCount;    //最大允许值
    unsigned int m_InitCount;      //记住构造时候的传参值，在重新初始化的时候使用

    //注意作为跨进程信号量使用此值获取不正确,但是互斥逻辑是正确的,有空改共享内存实现
    volatile unsigned int currCount;     //当前拥有的信号量,一些只查询,不想获取的应用可能会用到
    char m_name[128];   //std::string?
    void* pHand;  //
    enum 
    {
        PrivateSizeMax=64,   //
    };
    char pBuf[PrivateSizeMax];  //为了极致性能,不new空间,在类内部构造
private:
    Semaphore(const Semaphore&);
    Semaphore& operator=(const Semaphore&);
public:
    //linux 下配合sem_getvalue() 可以实现wait多个信号量,有空待续
    friend void* Semaphore_Create(Semaphore* parent);
    friend void Semaphore_Destory(Semaphore* parent,void*& pHand);
    friend bool Semaphore_Wait(Semaphore* parent,void* pHand,unsigned int timeout);
    friend bool Semaphore_Signal(Semaphore* parent,void* pHand,unsigned int lReleaseCount);
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//基于C++11的各种线程同步机制扩展实现 
#include <climits>       //for UINT_MAX
#include <mutex>
#include <condition_variable>


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//C++11以上版本用std::mutex 和std::condition_variable (条件变量) 实现信号量
//#include <condition_variable>
//#include <mutex>

//逻辑已经实现正确在VS2012下不稳定，待续。20181016 create by lizulin
class SemaphoreCpp11 
{
public:
    SemaphoreCpp11(unsigned int maxCount=1,unsigned int initCount=0,const char* name="")
        :m_MaxCount(maxCount),m_InitCount(initCount)
        ,currCount(0)
    {
        m_name[0]='\0';
        copyStr(m_name,sizeof(m_name),name);   //拷贝名称到类内
    }

    //等待N个--//unsigned int 最多53天
    bool Wait(unsigned int Waitms=UINT_MAX,unsigned int N=1U)  //=UINT_MAX表示无限等待
    {
        if(Waitms==UINT_MAX)   //无限等待的特殊情况考虑
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while(currCount<N)
            {
                m_condition.wait(lock);
            }
            currCount-=N;
            return true;
        }

        std::unique_lock<std::mutex> lock(m_mutex);
        if (currCount<N) 
        { 
            //count>0 // [this]()->bool { return count>0; }
            auto status=m_condition.wait_for(lock,std::chrono::milliseconds(Waitms),[&]()->bool{return currCount>=N;}); 
            if (!status)//status == std::cv_status::timeout) 
            {
                return false;
            }
        }
        currCount-=N;
        return true;
    }

    //如果有阻塞的线程,则随机唤醒一个线程，相当于信号量机制里面的V操作.否则 立即返回.
    bool Signal(unsigned int lReleaseCount=1U)     //返回值没有意义，为了与接口与win版一致暂留  释放多个
    {
        if (currCount+lReleaseCount>m_MaxCount)  //释放之后大于最大允许值,不执行,直接返回
        {
            return false;
        }
        std::lock_guard<std::mutex> lock(m_mutex);   //与wait用的锁类型不同，注意
        for (unsigned int n=0;n<lReleaseCount;n++)     //依次释放
        {
            ++currCount;
            m_condition.notify_one();
        }
        return true;
    }

    //无限等待版本
    //void Wait()
    //{
    //        std::unique_lock<std::mutex> lock(mutex);
    //        while(count<=0) 
    //        {
    //            condition.wait(lock); //suspend and wait ...
    //        }
    //        --count;
    //        return;
    //}
    ////等待一个
    //bool Wait(unsigned int Waitms=0xFFFFFFFF)  //unsigned int 最多53天  阻塞等待
    //{
    //    std::unique_lock<std::mutex> lock(m_mutex);
    //    //boost::unique_lock<boost::mutex> lock(m_mutex);
    //    if (currCount<=0) 
    //    { 
    //        //写法2 无限等待版本不符合接口要求，暂时如此，超时等待版会崩溃
    //        //m_condition.wait(lock);
    //        ////m_condition.wait_for(lock,std::chrono::milliseconds(Waitms));    //std::chrono::milliseconds(Waitms)
    //        //if (count<=0)
    //        //{
    //        //    return false;
    //        //}
    //        //boost 版正确
    //        //auto status=m_condition.wait_for(lock,boost::chrono::milliseconds(Waitms)); //wakesup>0
    //        //if (count<=0)
    //        //{
    //        //    return false;
    //        //}
    //        //无限等待与超时等待分别实现。也许能解决待续
    //        //auto now = std::chrono::system_clock::now();
    //        //auto status=m_condition.wait_until(lock,now+std::chrono::milliseconds(Waitms),
    //        //[&]{ return count>0; }); //wakesup>0
    //        //写法2
    //        //用这句容易在condition.notify_one();崩溃 
    //        auto status=m_condition.wait_for(lock,std::chrono::milliseconds(Waitms),
    //            [&]{ return currCount>0; });
    //        if (!status)//status == std::cv_status::timeout) 
    //        {
    //            return false;
    //        }
    //    }
    //    --currCount;
    //    //--wakeups;
    //    return true;
    //}
    //bool Signal()   //释放一个
    //{
    //    std::lock_guard<std::mutex> lock(m_mutex);        // std::unique_lock 与wait用的锁类型不同，注意
    //    //boost::lock_guard<boost::mutex> lock(m_mutex);  

    //    if (currCount<m_MaxCount)
    //    {
    //        ++currCount;
    //        m_condition.notify_one();
    //        //m_condition.notify_all();
    //        return true;
    //    }
    //    return false;
    //}
    //唤醒所有，作为信号量好像用不着
    //void signalAll() 
    //{
    //      Signal(MaxCount);
    //}


    //定义相同功能的接口Acquire  Release更加直观,仿造Qt
    bool Acquire(unsigned int TimesWait=UINT_MAX)
    {
        return this->Wait(TimesWait);
    }
    bool Release(unsigned int lReleaseCount=1U)
    {
        return this->Signal(lReleaseCount);
    }

    void ReSet()  //复位信号量为初始值
    { 
        unsigned int k;
        for(unsigned int k=0;k<m_MaxCount;k++)
        {
            if (!this->Wait(1))     //使用完所有资源 Wait(0/1)或Wait(1)
            {
                break;
            }
        }
        for(k=0;k<m_InitCount;k++)
        {
            this->Signal();     //恢复初始资源
        }
    }
    unsigned int AvailableCount()const {return currCount;}

    //TODO:强制销毁,即使正在被另一个线程使用,也会立即退出 特殊场景使用
    void doForceDestory()
    {
        this->Destory();
    }
    void ReInit()     //重新初始化，为了在ringbufer中使用是切换状态的时候复位到原来的信号量，一般情况不需要
    {
        Destory();     //关闭之后重新打开
        Create();
    }

    //返回这个信号量的名字--一些情况需要
   std::string name(){ return m_name;}
private:
    void Create()
    {
        new(&m_mutex) std::mutex;
        new(&m_condition) std::condition_variable;   
    }
    void Destory()
    {
         m_mutex.std::mutex::~mutex();
         m_condition.std::condition_variable::~condition_variable();
    }
private:
    unsigned int m_MaxCount;
    unsigned int m_InitCount;      //记住构造时候的传参值，在重新初始化的时候使用
    volatile unsigned int currCount;
    char m_name[128];
    int wakeups;  //与count功能一样，用此变量辅助会更快，具体见 c++11中信号量(semaphore)的实现--待测试确认

    //此代码,VS2012 实现有崩溃问题，到VS2015没有问题，可能是VS2012实现的condition_variable 有bug
    std::mutex m_mutex;
    std::condition_variable m_condition;

    //用boost的没有问题
    //boost::mutex m_mutex;
    //boost::condition_variable m_condition;
private:
    SemaphoreCpp11(const SemaphoreCpp11&);
    SemaphoreCpp11& operator=(const SemaphoreCpp11&);
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//C++11实现写优先的读写锁  C++17，提供了shared_mutex
/*
读锁之间是不冲突的，无论多少个线程申请读锁，不需要等待。但是要想获得写锁，就必须等所有的读锁全部释放完毕。
那么怎么知道所有的读全部结束了呢？只能用一个int变量去记录读锁的总数。这个变量变为0意味着所有的读都结束了。
但这是不完善的，如果其他线程也在写呢？写锁也必须等待写操作结束。那么写锁是否也需要int型来统计总数呢？
不需要。bool型足矣。
读锁通知和写锁通知的时机也不同。读锁要等所有的读全部结束的时候进行通知，写锁则是每个写结束都要通知。
*/

//可以看出用c++11实现读写锁变得非常简洁，在读取量非常大且写入频率很低的时候，通
//过一个简单的写入线程计数可以避免大量的 cond_w.notify_one();减少读取线程因此发生的切换。
//这里需要注意的是对写操作加锁时是先增加写线程计数再判断条件并等待，释放写锁时减少
//写线程计数并判断是否还有写操作等待，如果有只能唤醒一个写等待。

//读写锁不同实现，参考--测试与确认待续
//https://blog.csdn.net/mymodian9612/article/details/52794980


class RWLocker 
{
public:
    RWLocker ()
        :m_ReaderCount(0),bWriterUsing(false)
    {
    }
    void LockRead()
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (bWriterUsing==true)
        {
            cond.wait(lock);
        }
        m_ReaderCount++;
    }
    void UnlockRead()
    {
        std::unique_lock<std::mutex> lock(mtx);
        m_ReaderCount--;
        if (m_ReaderCount==0)
        {
            cond.notify_all();
        }
    }
    void LockWrite()
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (m_ReaderCount!=0||bWriterUsing==true)
        {
            cond.wait(lock);
        }
        bWriterUsing = true;
    }
    void UnlockWriter()
    {
        std::unique_lock<std::mutex> lock(mtx);
        bWriterUsing = false;
        cond.notify_all();     //叫醒所有等待的读和写操作  
    }
private:
    int m_ReaderCount;
    bool bWriterUsing;
    std::mutex mtx;
    std::condition_variable cond;
private:
    RWLocker(const RWLocker&);
    RWLocker& operator=(const RWLocker&);
};

#if 0
class SharedMutex
{
public:
    SharedMutex():
        mReadCount(0),bWriterUsing(false)
    {
    }
    void read()
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_cond.wait(lck,std::bind(&SharedMutex::ReadCond, this));  // 函数可能睡眠
        mReadCount++;   
    }
    void unread()
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        mReadCount--;
        m_cond.notify_all();
    }
    void write()
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        // 匿名函数如果不传指针,当唤醒的时候变量的值依然是睡眠时候的值，
        // 导致函数不能唤醒成功
        m_cond.wait(lck,std::bind([](const bool *is_w, const size_t *read_c) -> bool
        {
            return false == *is_w && 0 == *read_c;
        }, &bWriterUsing, &mReadCount));
        bWriterUsing = true;
    }
    void unwrite()
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        bWriterUsing = false;
        m_cond.notify_all();
    }
private:
    std::mutex m_mutex;
    std::condition_variable m_cond;
    size_t mReadCount;  //读者数量 
    bool bWriterUsing;       //是否在写
    bool ReadCond() const      // 读条件
    {
        return (bWriterUsing==false);
    }
    bool WriteCond() const      // 写条件
    {
        return (bWriterUsing==false)&&(0== mReadCount);
    }
};
#endif



//class WfirstRWLock  
//{  
//public:  
//    WfirstRWLock() = default;  
//    ~WfirstRWLock() = default;  
//public:  
//    void lock_read()  
//    {  
//        std::unique_lock<std::mutex> ulk(counter_mutex);  
//        cond_r.wait(ulk, [=]()->bool {return write_cnt == 0; });  
//        ++read_cnt;  
//    }  
//    void lock_write()  
//    {  
//        std::unique_lock<std::mutex> ulk(counter_mutex);  
//        ++write_cnt;  
//        cond_w.wait(ulk, [=]()->bool {return read_cnt == 0 && !inwriteflag; });  
//        inwriteflag = true;  
//    }  
//    void release_read()  
//    {  
//        std::unique_lock<std::mutex> ulk(counter_mutex);  
//        if (--read_cnt == 0 && write_cnt > 0)  
//        {  
//            cond_w.notify_one();  
//        }  
//    }  
//    void release_write()  
//    {  
//        std::unique_lock<std::mutex> ulk(counter_mutex);  
//        if (--write_cnt == 0)  
//        {  
//            cond_r.notify_all();  
//        }  
//        else  
//        {  
//            cond_w.notify_one();  
//        }  
//        inwriteflag = false;  
//    }  
//
//private:  
//    volatile size_t read_cnt{ 0 };  
//    volatile size_t write_cnt{ 0 };  
//    volatile bool inwriteflag{ false };  
//    std::mutex counter_mutex;  
//    std::condition_variable cond_w;  
//    std::condition_variable cond_r;  
//};  



//辅助自动加速 
class AutoReadLocker
{
private:
    RWLocker& m_sm;
public:
    AutoReadLocker(RWLocker &sm):m_sm(sm)
    {
        m_sm.LockRead();
    }
    ~AutoReadLocker()
    {
        m_sm.UnlockRead();
    }
private:
    AutoReadLocker(const AutoReadLocker&);
    AutoReadLocker& operator=(const AutoReadLocker&);
};

class AutoWriteLocker
{
private:
    RWLocker& m_sm;
public:
    AutoWriteLocker(RWLocker &sm):m_sm(sm)
    {
        m_sm.LockWrite();
    }
    ~AutoWriteLocker()
    {
        m_sm.UnlockWriter();
    }
private:
    AutoWriteLocker(const AutoWriteLocker&);
    AutoWriteLocker& operator=(const AutoWriteLocker&);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//内存屏障，内存顺序封装


//#if 0
//ARM 指令隔离(barrier)指令和存储器隔离指令
//DMB:数据存储器隔离。DMB 指令保证： 仅当所有在它前面的存储器访问操作
//    都执行完毕后，才提交(commit)在它后面的存储器访问操作。
//DSB:数据同步隔离。比 DMB 严格： 仅当所有在它前面的存储器访问操作
//    都执行完毕后，才执行在它后面的指令（亦即任何指令都要等待存储器访 问操作——译者注）
//ISB:指令同步隔离。最严格：它会清洗流水线，以保证所有它前面的指令都执
//    行完毕之后，才执行它后面的指令。
//
//更多参考GCC
//barrier() 编译优化屏障，阻止编译器为了性能优化而进行指令重排。
//mb()      内存屏障(包括读和写)，用于SMP和UP。
//rmb()     读内存屏障，用于SMP和UP。
//wmb()    写内存屏障，用于SMP和UP。
//smp_mb()	用于SMP场合的内存屏障。对于UP不存在memory order的问题，在UP上就是一个优化屏障，确保汇编和C代码的memory order一致。	 
//smp_rmb()   用于SMP场合的读内存屏障。
//smp_wmb()  用于SMP场合的写内存屏障。
//smp_read_barrier_depends()	读依赖屏障。
//#endif



#if 1  //old
#if defined(_MSC_VER)&&_MSC_VER>=1310&&(defined(_M_IX86)||defined(_M_X64)) &&!defined(__c2__)
extern "C" void _mm_pause();
#define SUPER_SMT_PAUSE _mm_pause();
#elif defined(__GNUC__) && ( defined(__i386__) || defined(__x86_64__) )
#define SUPER_SMT_PAUSE __asm__ __volatile__( "rep; nop" : : : "memory" );
#else

//TODO:ARM 
#define SUPER_SMT_PAUSE __asm__ __volatile__("":::"memory");
#endif
#endif


//内存屏障指令
#ifdef OS_ARM_LINUX
#define SUPER_DMB  __asm__ __volatile__("dmb":::"memory");
#define SUPER_DSB   __asm__ __volatile__("dsb":::"memory");
#define SUPER_ISB    __asm__ __volatile__("isb":::"memory");
#else
#define SUPER_DMB
#define SUPER_DSB
#define SUPER_ISB
#endif


//内存屏障指令
//GNU 或asm volatile("" ::: "memory"); __sync_synchronize
//#define SUPER_MemBarrier __asm__ __volatile__ ("":::"memory");   
//#define SUPER_MemBarrier __memory_barrier()                              //Intel_C++编译器使用"full compiler fence
//#define SUPER_MemBarrier _ReadWriteBarrier()                              //Microsoft Visual C++编译器

#define SUPER_MemBarrier  std::atomic_thread_fence(std::memory_order_acq_rel);     //C11/C++11


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Spinbox 自旋锁
//如何设计一个真正高性能的spin_lock?
//郭忠明
//目前多数网站上放出的Spin_lock都不是合格的代码，都是严重误导广大群众的。
// 例如：下面这个 Usage examples - 1.60.0 

#include <atomic>
#include <thread>
//class Spin_lock
//{
//public:
//    Spin_lock( void )
//    {
//        d_atomic_flag.clear( std::memory_order_relaxed );
//        return;
//    }
//    void lock( void )
//    {
//        while( d_atomic_flag.test_and_set( std::memory_order_acquire ) ) {
//        }
//        return;
//    }
//    bool try_lock( void )
//    {
//        if( d_atomic_flag.test_and_set( std::memory_order_acquire ) )   return  false;
//        return  true;
//    }
//    void unlock( void )
//    {
//        d_atomic_flag.clear( std::memory_order_release );
//        return;
//    }
//private:
//    Spin_lock( const Spin_lock& ) = delete;
//    Spin_lock& operator = ( const Spin_lock& ) = delete;
//    std::atomic_flag  d_atomic_flag;
//};

//atomic的exchange/test_and_set/check_and_swap一般都需要serialization，即在执行这条指令前,CPU必须要完成前面所有对memory的访问指令(read and write)。
//    这是非常heavy的操作，使得CPU无法对指令进行reorder，从而优化执行。
//    因为每一次atomic的exchange/test_and_set/check_and_swap都必须读-写 state 这个变量。这就要涉及到多个CPU之间cache coherence的问题。
//    当CPU1读state的时候，如果这个state没有在CPU1的cache中，就需要从memory中读入，因为CPU又需要写这个变量，所以在把这个变量读入cache的时候，如果其他CPU已经cache了这个变量，就需要invalidate它们。这样在CPU1把lock读入自己的cache中时，这块cacheline所cache的lock就是CPU1所独占的，CPU1就可以更改它的值了。如果多个CPU在竞争这个spinlock的话，每一次test_and_set都需要完成以上的操作，在系统总线上会产生大量的traffic，开销是非常大的，而且unlock的CPU还必须同其它正在竞争spinlock的CPU去竞争cacheline ownership. 随着CPU数目的增多，性能会衰减的非常快。
//
//    目前包括C++ 准标准库Boost库在内的spin_lock都存在一些缺陷导致的性能问题。很多版本要么过于简单，出现上面的问题，要么使用了usleep()/nanosleep()/sleep()等代码, 例如：linux下usleep(0)/nanosleep(0)实际测试是50微秒的延迟时间，而我们的锁占用总时长可能只有1-40个微秒。

//作者：郭忠明
//链接：https://www.zhihu.com/question/55764216/answer/146139092
//来源：知乎
//    著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
//    下面是我设计的一个高性能spin_lock,
//    下列代码可以挑战目前所有linux下的spin_lock版本。
//    有不服的可以挑战。
//我们的核心lock()代码中关键的第1行 
//    while( d_atomic_bool.exchange( true, std::memory_order_acquire ) ) 
//        首先是使用了更高效率的exchange, 要比CAS指令有更高的效率，是内存读写模式，每次都要独占cache，并且将其他CPU的cache设置为invalide, 如果只有这1行代码，那么 如果两个以上CPU在等待这把锁，将交替将64字节Cacheline内存反复同步， 数据冲突激烈，影响其他线程的内存操作，降低整体性能, 影响全部CPU的工作效率, 因为产生了内存争用, 产生了总线争用
//        因此我们提供了第2行关键检测代码：
//        if( !d_atomic_flag.load( std::memory_order_relaxed ) ) break
//            该指令主要是为了减少CAS指令循环对内存总线带来的不良影响, 这个循环只有在锁被释放后, 才会跳出循环, 但是这个指令消耗的资源极少。这组指令是 只读模式检查, 只需要锁没有被释放, 这里继续循环, 但是这组spin是没有竞争状态的, 不会对内存和cache产生任何影响, 只要锁不释放, 永远不会内存总线的争夺, 因此大大提高了整体的效率


class SpinLock
{
public:
    SpinLock()
    {
        m_bFlag.store( false, std::memory_order_relaxed );
        return;
    }
    void lock()
    {
        while( m_bFlag.exchange( true, std::memory_order_acquire ) ) 
        {
            while( 1 )
            {
                SUPER_SMT_PAUSE  // pause指令 延迟时间大约是12纳秒

                    if( !m_bFlag.load( std::memory_order_relaxed ) ) 
                    {
                        break; 
                    }

                    std::this_thread::yield(); 
                    // 在无其他线程等待执行的情况下，延迟时间113纳秒
                    // 在有其他线程等待执行情况下，将切换线程
                    if( !m_bFlag.load( std::memory_order_relaxed ) ) 
                    {
                        break; 
                    }
                    continue;
            }
            continue;
        }
        return;
    }
    bool try_lock( void )
    {
        return !m_bFlag.exchange( true, std::memory_order_acquire );
    }
    void unlock( void )
    {
        m_bFlag.store( false, std::memory_order_release ); // 设置为false
        return;
    }
private:
    SpinLock( const SpinLock& R);
    SpinLock& operator =( const SpinLock& R);
    std::atomic<bool> m_bFlag;
};


//@郭忠明
//    说的方法是test and test-and-set lock。
//
//    他的代码的问题是，刚进入lock 就调用
//
//    while( d_atomic_bool.exchange( true, std::memory_order_acquire ) ) 
//        带来了不必要的总线 traffic
//
//
//        正确的做法是 CAS 前先 test
//
//        // code from wiki
//        boolean locked := false // shared lock variable 
//        procedure EnterCritical() {
//            do {
//                while (locked == true) skip or pause // spin until lock seems free
//            } while TestAndSet(locked) // actual atomic locking
//    }
//    不过这都是小问题。test and test-and-set lock 真正的问题是锁没有公平性，会有饥饿。
//
//
//     我自己实现spin lock 倾向于ticket lock。
//
//    std::atomic<uint> ticket; // init as 0
//    std::atomic<uint> serve; // init as 0
//
//    void lock() {
//        auto my_tick = tick++;
//        while (my_tick != serve) {
//            // pause and wait
//        }
//    }
//
//    void unlock() {
//        serve++;
//    }
//    对比test test lock, ticket lock更公平些，实现起来也不难，overhead 适中。
//
//有兴趣的同学可以再看下MCS lock。同样兼顾性能和公平，但是实现起来复杂些，memory 使用上也多一点。

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//spinlock reference boost

class spinlock
{
public:
    std::atomic_flag m_V;
public:
    bool try_lock()
    {
        return !m_V.test_and_set( std::memory_order_acquire );
    }
    void lock()
    {
        for( unsigned k = 0; !try_lock(); ++k )
        {
            yield(k);
        }
    }
    void unlock()
    {
        m_V .clear( std::memory_order_release );
    }

    static inline void yield( unsigned k )
    {
        if( k < 4 )
        {
        }
#if defined(SUPER_SMT_PAUSE )
        else if( k < 16 )
        {
            SUPER_SMT_PAUSE
        }
#endif
        else if( k < 32 )
        {
            ::SleepMs(0);
        }
        else
        {
            ::SleepMs(1);
        }
        //else
        //{
        //    // Sleep isn't supported on the Windows Runtime.
        //    std::this_thread::yield();
        //}
    }
public:
    class scoped_lock
    {
    private:
        spinlock & sp_;
        scoped_lock( scoped_lock const & );
        scoped_lock & operator=( scoped_lock const & );
    public:
        explicit scoped_lock( spinlock & sp ): sp_( sp )
        {
            sp.lock();
        }
        ~scoped_lock()
        {
            sp_.unlock();
        }
    };
};



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



NAME_SPACE_MAIN_END


#ifdef UNIT_TEST
int Test_ThreadSync();
#endif


#endif
