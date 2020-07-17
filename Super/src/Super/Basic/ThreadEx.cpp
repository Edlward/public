#include "ThreadEx.h"

#include <stdio.h>


#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

static void MySleep(unsigned int ms)
{
#ifdef _WIN32
    return ::Sleep(ms);
#else
    ::usleep(ms*1000);
#endif
}

NAME_SPACE_MAIN_BEGIN
class ThreadExPrivate
{
public:
    //MyMutex mutex;
private:
};

ThreadEx::ThreadEx()
    :m_thread(NULL),
    m_running(false),
    m_ContinueRun(true)
{
     //pimpl=new ThreadPrivate;
    m_stackSize=0;   //线程栈大小,0一般会使用默认值
}    

ThreadEx::~ThreadEx()
{
    if(m_running)
    {
        stopWaitFinish();  //stop()
    }

#ifdef _WIN32
    if(m_thread)
    {
        ::CloseHandle(m_thread);
        m_thread=NULL;
    }
#endif                  
    //delete pimpl;
}

ThreadEx::Handle ThreadEx::startThread(void* pFunAddr,void* pPara,unsigned int StackSize)
{
#ifdef _WIN32
    unsigned int m_ThreadId;
    typedef unsigned int (__stdcall *pThreadFun)(void*);     //声明windows下 线程函数类型
    pThreadFun pFun=(pThreadFun)pFunAddr;                  //强转

    Handle  m_thread= (HANDLE)_beginthreadex(NULL,StackSize,pFun,pPara,0, &m_ThreadId);   
    if (m_thread==NULL)
    {
        printf("Thread::startThread() 开启线程失败\n");
    }

#else
    Handle  m_thread;
    pthread_attr_t attr;
    //attr.stacksize=StackSize;    //其他属性待续

    if (StackSize!=0)   //use default 
    {
        pthread_attr_setstacksize(&attr,StackSize);        //设置线程栈大小 
    }


    pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);  //不分离 否则不可以join

    typedef void* (*start_routine)(void*);
    start_routine tmpPfun;
    *((void**)&tmpPfun)=pFunAddr;

    PRINT_FUN_TRACE

    //if(pthread_create(&m_thread, &attr,pFunAddr, this) == 1)    //待测
    //if(pthread_create(&m_thread, &attr,&mpPfun,pPara) == 1)    //待测
    {
        //perror("Thread:create failed");
        printf("Thread::startThread() 开启线程失败\n");
        //setRunning(false);
        //return false; 
    }
#endif

    return m_thread;
}

void ThreadEx::waitFinish(Handle m_thread)
{
#ifdef _WIN32  
    //WaitForSingleObject(m_thread, INFINITE);  
    //Thread::sleep_for(1);
    if (NULL!=m_thread)  
    {  
        //if (isRunning())
        //{
            WaitForSingleObject(m_thread, INFINITE);      //如果线程已经结束，不能WaitForSingleObject？
            //std::cout<<"Thread::waitFinish() WaitForSingleObject()"<<std::endl;
            //CloseHandle(m_thread);  
        //}
        CloseHandle(m_thread);  
        m_thread = NULL;  
    }  
#else  

    //pthread_join(m_thread, NULL);  
    //if (isRunning())
    {
        pthread_join(m_thread, NULL);  
    }
    m_thread = 0;  
#endif // WIN32
}


bool ThreadEx::start()
{
    if (isRunning())
    {
        printf("Thread::start() 上次的线程未结束，不能开始新的线程\n");
        return false;
    }

#ifdef _WIN32
    m_thread = (HANDLE)_beginthreadex(NULL,m_stackSize,&ThreadCallFun,this, 0, &m_ThreadId);   
    if (m_thread==NULL)
    {
        printf("Thread::Create() 创建失败\n");
        return false; 
    }

#else
    pthread_attr_t attr;
    pthread_attr_init(&attr);


    if (m_stackSize!=0)   //use default 
    {
        pthread_attr_setstacksize(&attr,m_stackSize);        //设置线程栈大小 
    }
    //pthread_attr_getstacksize(&attr,&m_stackSize);  //获取线程栈大小 

    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //不分离 否则不可以join
    if(pthread_create(&m_thread, &attr, ThreadCallFun, this) == 1)
    {
        //perror("Thread:create failed");
        printf("Thread::Create() 创建失败");
        setRunning(false);
        return false; 
    }
#endif

    setContinueRun(true);
    setRunning(true);   //线程开启成功，立即设置运行状态否则, ThreadCallFun里面设置可能会延时导致waitFinish()失败
    //Thread::sleep_for(1);//Sleep(20);
    return true; 
}


//线程函数
ThreadEx::FunRet_t STDCALL ThreadEx::ThreadCallFun(void* pThis)
{
    //printf("ThreadCallFun()--Begin pThread:%p ",pThis);
    ThreadEx *p = (ThreadEx*)pThis;  //static_cast<lcw_thread*>(arg);//指向派生类指针转化为基类对象指针

    p->setContinueRun(true);
    p->setRunning(true); 
    p->run();              
    p->setRunning(false); //Run()运行完了
    p->setContinueRun(false);


#ifdef _WIN32
    _endthreadex(0);
#endif
    return (FunRet_t)NULL;
}


void ThreadEx::waitFinish()
{
#ifdef _WIN32  
    //Thread::sleep_for(1);
    if (NULL!=m_thread)  
    {  
        if (isRunning())
        {
            WaitForSingleObject(m_thread, INFINITE);      //如果线程已经结束，不能WaitForSingleObject?
            //std::cout<<"Thread::waitFinish() WaitForSingleObject()"<<std::endl;
            //CloseHandle(m_thread);  
        }
        CloseHandle(m_thread);  
        m_thread = NULL;  
    }  
#else  

    //pthread_join(m_thread, NULL);  
    if (isRunning())
    {
        pthread_join(m_thread, NULL);  
    }
    m_thread = 0;  
#endif
    //std::cout<<"Thread::waitFinish() "<<std::endl;
}


bool ThreadEx::stop(unsigned int timeDelayInms)
{
    setContinueRun(false);
    while(isRunning()&&timeDelayInms>0)    //unsigned int 永远大于0，只能减一逼近
    {
            ThreadEx::sleep_for(1);
            timeDelayInms-=1;
    }

    //如果还在运行强制杀线程退出
    if (isRunning())
    {
#ifdef _WIN32
        if (m_thread)
        {
                ::TerminateThread(m_thread,0);    //windows下强制结束线程的函数，不安全
        }
#else

#endif  
        //printf("Thread::stop() 强制结束线程\n");
        m_running=false;     //非正常结束，复位运行状态
        return false;
    }

    //printf("Thread::stop() 正常结束线程\n");
    m_running=false;
    return true;
}

void ThreadEx::stopWaitFinish()
{
    stop();
    waitFinish();
}

void ThreadEx::sleep(unsigned int ms)
{
     ::MySleep(ms);
}


void ThreadEx::yield()
{
#ifdef _WIN32
        ThreadEx::sleep_for(0);  
#else
    //参考zthread
    bool result = false;
#if defined(HAVE_SCHED_YIELD)
    result = sched_yield() == 0;
#endif
   // return result;
#endif  
}

bool ThreadEx::setPriority(Priority NewP)
{
#ifdef _WIN32
    if(m_thread==NULL)
    {
        return false;
    }

    bool result;
    int n;
    switch(NewP) 
    {
    case Idle:
        n = THREAD_PRIORITY_IDLE;
    case Low:
        n = THREAD_PRIORITY_LOWEST;
    case Below:
        n = THREAD_PRIORITY_BELOW_NORMAL;
        break;
    case Normal:
        n = THREAD_PRIORITY_NORMAL;
        break;
    case Above:
        n = THREAD_PRIORITY_ABOVE_NORMAL;
        break;
    case High:
        n = THREAD_PRIORITY_HIGHEST;
        break;
    default:
        n = THREAD_PRIORITY_NORMAL;
        break;
    }

    result = (::SetThreadPriority(m_thread, n) != THREAD_PRIORITY_ERROR_RETURN);
    return result;
#else

    if(m_thread==0)
    {
        return false;
    }

    bool result = true;
    struct sched_param param;
    switch(NewP) 
    {
    case Low:
        param.sched_priority = 0;
        break;
    case Normal:
        param.sched_priority = 5;   //待确认
        break;
    case High:
        param.sched_priority = 10;
        break;
    default:
        param.sched_priority = 5;
    }
    result = pthread_setschedparam(m_thread, SCHED_OTHER, &param) == 0;
    return result;
#endif  
}

bool ThreadEx::getPriority(int* p)const
{
    if(m_thread==0)
    {
        return false;
    }

#ifdef _WIN32
    bool result = true;
    // Convert to one of the PRIORITY values
    switch(::GetThreadPriority(m_thread)) 
    {
    case THREAD_PRIORITY_ERROR_RETURN:
       { 
           *p = Normal;
            result = false;
            break;
       }
    case THREAD_PRIORITY_IDLE:
        *p =Idle;
        break;
    case THREAD_PRIORITY_BELOW_NORMAL:
        *p = Below;
        break;
    case THREAD_PRIORITY_NORMAL:
        *p = Normal;
        break;
    case THREAD_PRIORITY_ABOVE_NORMAL:
        *p = Above;
        break;
    case THREAD_PRIORITY_HIGHEST:
        *p = High;
        break;
    default:
        *p = Normal;
    }
    return result;
#else

    bool result = true;
    struct sched_param param;
    int policy = SCHED_OTHER;
    if(result = (pthread_getschedparam(m_thread, &policy, &param) == 0)) 
    {
        // Convert to one of the PRIORITY values
        if(param.sched_priority < 10)
        {
            *p = Low;
        }
        else if(param.sched_priority == 10)
        {
            *p = Normal;
        }
        else
        {
            *p = High;
        }
    }
    return result;
#endif  
}

//unsigned int Thread::getParentStackSize()
//{
//#ifdef _WIN32
//        return 0;
//#else
//       
//#endif
//}

unsigned int ThreadEx::getStackSize()
{
    return m_stackSize;
}

void ThreadEx::setStackSize(unsigned int nSize)
{
    m_stackSize=nSize;
}

#ifdef _WIN32

static unsigned int getCpuCount()     ////获取CPU核心数量
{  
    SYSTEM_INFO sysInfo;  
    GetSystemInfo(&sysInfo);  
    return sysInfo.dwNumberOfProcessors;  
}

void ThreadEx::SetThreadAffinityMask(int mask)
{
    //return;

    if (m_thread!=NULL)
    {
         ::SetThreadAffinityMask(m_thread,mask);
    }
}
void ThreadEx::SetThreadAffinityNum(unsigned int num)
{
    num=num%getCpuCount();       //取cpu核心数量
    int mask=0X1<<num;                //转成掩码形式
    SetThreadAffinityMask(mask);
}
#else   //暂不实现

//void Thread::SetThreadAffinityMask(int mask)
//{

//}
//void Thread::SetThreadAffinityNum(int num)
//{

//}
#endif



#include <string>
class ThreadTest:public ThreadEx
{
public:
    ThreadTest(std::string sName):sName(sName){};
    ~ThreadTest(){};
    virtual void run()
    {
        int Num=5;
        while (getContinueRun()&&Num)     //通过外部控制shouldExit()标记来决定是否退出线程
        {
            printf("ThreadTest:: run() %s",sName.c_str());
            ThreadEx::sleep_for(3000);
            //Num--;
        }
    }
private:
    std::string sName;
};


#ifdef UNIT_TEST

int Test_ThreadEx()
{
    //Thread* pThread=new ThreadTest(false);
    //pThread->Create();

    ThreadEx* pThread=new ThreadTest("TestA");
    pThread->start();

    ThreadEx* pThread2=new ThreadTest("TestB");
    pThread2->start();

    //Thread::sleep_for(3000);
    //pThread->stop(500);  //强制结束测试

    //线程结束才会运行到wait,可以用以下方法等待多个线程结束
    pThread->waitFinish();
    pThread2->waitFinish();

    printf("ThreadTest::结束\n");
    return 0;
}

#endif


NAME_SPACE_MAIN_END
