#ifndef ThreadSyncSys_H_
#define ThreadSyncSys_H_

//各种线程同步锁,以声明方式前置隔离实现,可无害用于模板中  //creation by lizulin 2017-2020
//避免在.h 中引入windows.h 之类系统头文件,大型项目包含复杂后可能遇到很多定义冲突编译错误的问题。 



#include "global.h"

NAME_SPACE_MAIN_BEGIN

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//互斥锁 递归锁 对应std::recursive_mutex 
class MutexPrivate;
extern MutexPrivate* Mutex_CreatPrivate();
extern  void Mutex_DestoryPrivate(MutexPrivate* p);
extern  void Mutex_Lock(MutexPrivate* p);       //加锁  
extern  bool Mutex_TryLock(MutexPrivate* p);   //非阻塞 枷锁成功返回true 否则返回false  
extern  void Mutex_UnLock(MutexPrivate* p);    //解锁  

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
        pimpl=::Mutex_CreatPrivate();
    }
    void DestoryPrivate()
    {
        ::Mutex_DestoryPrivate(pimpl);
    }
public:
    void Lock()        //加锁  
    {
         ::Mutex_Lock(pimpl);
    }
    bool TryLock()   //非阻塞 枷锁成功返回true 否则返回false
    {
         return ::Mutex_TryLock(pimpl);
    }
    void UnLock()    //解锁  
    {
         Mutex_UnLock(pimpl);
    }
public:
    void Reinit()   //为了在特殊用途重新构造，一般情况不需要
    {
        DestoryPrivate();
        CreatPrivate();
    }
public:
private:
    MutexPrivate* pimpl;
private:  
    Mutex(const Mutex&){};                   // 禁止拷贝和赋值构造 
    Mutex& operator=( const Mutex& ){return *this;}; 
};



extern void* MutexNoRecursive_Create();
extern  void MutexNoRecursive_Destory(void*& pHand);
extern  void MutexNoRecursive_Lock(void* pHand);       //加锁  
extern  bool MutexNoRecursive_TryLock(void* pHand);   //非阻塞 枷锁成功返回true 否则返回false  
extern  void MutexNoRecursive_UnLock(void* pHand);    //解锁  

//互斥锁 非递归锁 对应std::mutex   
class MutexNoRecursive
{  
public:  
    MutexNoRecursive()
    {
        if(!Create())
        {
            printf("互斥锁创建失败\n");
            PRINT_ERROR_CURR_LINE
            //exit(-1);
        }
    }
    ~MutexNoRecursive()  
    {  
        Destory();
    }
    void Lock()        //加锁  
    {
        ::MutexNoRecursive_Lock(pHand);
    }
    bool TryLock()   //非阻塞 枷锁成功返回true 否则返回false  
    {
        return ::MutexNoRecursive_TryLock(pHand);
    }
    void UnLock()    //解锁  
    {
        ::MutexNoRecursive_UnLock(pHand);
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
        return (pHand=::MutexNoRecursive_Create())!=NULL;
    }
    void Destory()
    {
        ::MutexNoRecursive_Destory(pHand);
    }
private:
     void* pHand;
private:  
    MutexNoRecursive(const MutexNoRecursive& ){};                   // 禁止拷贝和赋值构造 
    MutexNoRecursive& operator = ( const MutexNoRecursive& ){return *this;}
};


class MutexBetweenProcess;
extern void* MutexBetweenProcess_Create(MutexBetweenProcess* parent);
extern  void MutexBetweenProcess_Destory(void*& pHand);
extern  bool MutexBetweenProcess_Lock(void* pHand);        //加锁  
extern  bool MutexBetweenProcess_TryLock(void* pHand,unsigned int ms);   //非阻塞 枷锁成功返回true 否则返回false  
extern  bool MutexBetweenProcess_UnLock(void* pHand);    //解锁  


//跨进程互斥锁，linux下的原来用信号量,改成互斥锁实现。
class MutexBetweenProcess
{
public:
    //默认创建匿名的互斥
    MutexBetweenProcess(const char* name="")
        :pHand(NULL),isExist(false)
    {
        //拷贝文件名到类内
        memset(m_name,0,sizeof(m_name));
        const size_t nameLenMax=sizeof(m_name)-1;
        if (name!=NULL)
        {
            size_t lenName=strlen(name);
            size_t newLen=(lenName>nameLenMax)?nameLenMax:lenName;
            strncpy(m_name,name,newLen);
        }
        m_name[nameLenMax]='\0';
        if (!Create())
        {
            printf("跨进程互斥锁打开失败\n");
            PRINT_ERROR_CURR_LINE
                ::exit(-1);
        }
    }
    ~MutexBetweenProcess()
    {
        Destory();
    }
    bool Lock()
    {
        return ::MutexBetweenProcess_Lock(pHand);        //加锁  
    }
    bool TryLock(unsigned int ms=5)
    {
        return ::MutexBetweenProcess_TryLock(pHand,ms);   //非阻塞 枷锁成功返回true 否则返回false  
    }
    bool UnLock()
    {
        return ::MutexBetweenProcess_UnLock(pHand);    //解锁  
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
        return (pHand=::MutexBetweenProcess_Create(this))!=0;
    }
    void Destory()
    {
        ::MutexBetweenProcess_Destory(pHand);
    }
private:
    void* pHand;
    char m_name[128];
    //完善待续
    bool isCreator;  //创建者
    bool isExist;     //是否存在同名命名互斥锁
private:
    friend void* MutexBetweenProcess_Create(MutexBetweenProcess* parent);
    friend void MutexBetweenProcess_Destory(void*& pHand);
    friend bool MutexBetweenProcess_Lock(void* pHand);        //加锁  
    friend bool MutexBetweenProcess_TryLock(void* pHand,unsigned int ms);   //非阻塞 枷锁成功返回true 否则返回false  
    friend bool MutexBetweenProcess_UnLock(void* pHand);    //解锁  
};




// 加锁辅助工具 --模版实现 
template<class MutexType>  
class LockerGuard  
{  
public:  
    LockerGuard(MutexType& locker )   
        : m_locker(locker)  
    {  
        locker.Lock();  
    }  
    ~LockerGuard()  
    {  
        m_locker.UnLock();  
    }  
protected:  
    MutexType& m_locker;  
};  

typedef LockerGuard<Mutex> AutoLocker;
typedef LockerGuard<MutexBetweenProcess> AutoProcessLocker; 





#if 0
//windows条件变量封装
#include <windows.h>
class ncConditionVariable
{
private:
    CONDITION_VARIABLE _cv;
public:
    ncConditionVariable()
    {
        InitializeConditionVariable(_cv);
    }
    ~ncConditionVariable()
    {
    }

public:
    void lock (CRITICAL_SECTION cs, DWORD dwMilliseconds = INFINITE)
    {
        SleepConditionVariableCS (&_cv, &cs, dwMilliseconds);
    }
    void lock (SRWLOCK srwLock, DWORD dwMilliseconds = INFINITE, ULONG Flags = 0)
    {
        SleepConditionVariableSRW (&_cv, &srwLock, dwMilliseconds, Flags);
    }
    void unlock (bool isAll = false)
    {
        if (isAll) {
            WakeConditionVariable (_cv);
        }
        else {
            WakeAllConditionVariable (_cv);
        }
    }
};
#endif




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
    Semaphore(unsigned int maxCount=1,unsigned int initCount=0,const char* Name="")
        :m_MaxCount(maxCount),m_InitCount(initCount)
        ,currCount(0)
        ,pHand(NULL)
    {
        //拷贝文件名到类内
       memset(m_name,0,sizeof(m_name));
       const size_t nameLenMax=sizeof(m_name)-1;
        if (Name!=NULL)
        {
            size_t lenName=strlen(Name);
            size_t newLen=(lenName>nameLenMax)?nameLenMax:lenName;
            strncpy(m_name,Name,newLen);
        }
        m_name[nameLenMax]='\0';

        if (!this->Create())  //信号里创建失败,退出程序
        {
            printf("信号量打开失败\n");
            PRINT_ERROR_CURR_LINE
            ::exit(-1);
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
            printf("信号量打开失败\n");
            PRINT_ERROR_CURR_LINE
            ::exit(-1);
        }
    }

    void ReSet() //复位信号量为初始值
    {
            for(unsigned int k=0;k<m_MaxCount;k++)
            {
                if (!this->Wait(1))   //使用完所有资源Wait(0)或Wait(1)
                {
                    break;
                }
            }
            for(unsigned int k=0;k<m_InitCount;k++)
            {
                this->Signal();     //恢复初始资源
            }
    }

    bool Wait(unsigned int timeout=UINT_MAX)   //UINT_MAX=INFINITE表示无限等待
    {
        return  Semaphore_Wait(this,pHand,timeout);
    }
    bool Signal(unsigned int lReleaseCount=1)
    {
        return Semaphore_Signal(this,pHand,lReleaseCount);
    }

    //定义相同功能的接口Acquire  Release更加直观,仿造Qt
    bool Acquire(unsigned int TimesWait=UINT_MAX)
    {
        return this->Wait(TimesWait);
    }
    bool Release(unsigned int lReleaseCount=1U)
    {
        return this->Signal(lReleaseCount);
    }
    unsigned int AvailableCount()const {return currCount;}
private:
    bool Create()
    {
        pHand=Semaphore_Create(this);
        return pHand!=0;
    }
    void Destory()
    {
        ::Semaphore_Destory(this,pHand);
    }
private:
    unsigned int m_InitCount;      //记住构造时候的传参值，在重新初始化的时候使用
    unsigned int m_MaxCount;    //最大允许值
    //注意作为跨进程信号里使用此值获取不正确,但是互斥逻辑是正确的,有空改共享内存实现
    volatile unsigned int currCount;     //当前拥有的信号量,一些只查询,不想获取的应用可能会用到

    char m_name[128];  //std::string ?
    void* pHand;  //
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



NAME_SPACE_MAIN_END


#ifdef UNIT_TEST
int Test_ThreadSync();
#endif


#endif
