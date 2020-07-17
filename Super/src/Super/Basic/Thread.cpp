#include "Thread.h"

#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
     #include <process.h>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif

    #include<errno.h>
    #include<time.h>

//跨平台 windows/linux sleep线程延时等待函数 by lizulin--可再改进
//方式1:直接使用
//void MySleep(unsigned int ms);

//方式2:宏定义根据平台自动转换--失败：正确的反而被替换 winapi::sleep
//#ifdef _WIN32
//    #define sleep(x) ::MySleep((x)*1000)     
//    #define usleep(x) ::MySleep((x))           
//#else
//    #define MySleep(x) ::MySleep((x))
//#endif


static void MySleep(unsigned int ms)
{
#ifdef _WIN32
          return ::Sleep(ms);
#else
          ::usleep(ms*1000);
#endif
}



static const unsigned int kNumMillisPerSecond=1000;
static const unsigned int kNumMicrosPerMilli=1000;
static const unsigned int kNumMicrosPerSecond=kNumMillisPerSecond*1000;
static const unsigned int kNumNanosPerMicro=1000;
static const unsigned int kNumNanosPerSecond=kNumNanosPerMicro*kNumMicrosPerSecond;


#ifdef _WIN32
// Window's Sleep takes milliseconds argument.
void SleepForMilliseconds(unsigned int milliseconds){ ::Sleep(milliseconds);}
void SleepForSeconds(double seconds) 
{
    SleepForMilliseconds(static_cast<unsigned int>(kNumMillisPerSecond * seconds));
}
#else   //
void SleepForMicroseconds(unsigned int microseconds) 
{
    struct timespec sleep_time;
    sleep_time.tv_sec = microseconds / kNumMicrosPerSecond;
    sleep_time.tv_nsec = (microseconds % kNumMicrosPerSecond) * kNumNanosPerMicro;
    while (nanosleep(&sleep_time, &sleep_time) != 0 && errno == EINTR)
        ;  // Ignore signals and wait for the full interval to elapse.
}
void SleepForMilliseconds(unsigned int milliseconds) 
{
    SleepForMicroseconds(milliseconds * kNumMicrosPerMilli);
}
void SleepForSeconds(double seconds)
{
    SleepForMicroseconds(static_cast<unsigned int>(seconds * kNumMicrosPerSecond));
}
#endif



NAME_SPACE_MAIN_BEGIN
class ThreadPrivate
{
public:
    //MyMutex mutex;
private:
};

Thread::Thread()
    :m_thread(NULL),
    bCreateSuccess(false),
    m_running(false),
    m_ContinueRun(true)
{
     //pimpl=new ThreadPrivate;
    m_stackSize=0;   //线程栈大小,0一般会使用默认值
}    

Thread::~Thread()
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

Thread::Handle Thread::startThread(void* pFunAddr,void* pPara,unsigned int StackSize)
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
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);  //不分离 否则不可以join

    typedef void* (*start_routine)(void*);
    start_routine tmpPfun;
    *((void**)&tmpPfun)=pFunAddr;

    PRINT_FUN_TRACE


    //if(pthread_create(&m_thread, &attr,pFunAddr, this) == 1)    //待测
    if(pthread_create(&m_thread,&attr,tmpPfun,pPara)!=0)    //待测
    {
        //perror("Thread:create failed");
        printf("Thread::startThread() 开启线程失败\n");
        //setRunning(false);
        //return false; 
    }
#endif

    return m_thread;
}

void Thread::waitFinish(Handle m_thread)
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

#if 1
    //TODO:

#else
    //pthread_join(m_thread, NULL);  
    //if (isRunning())
    {
        pthread_join(m_thread, NULL);  
    }
    m_thread = 0;
#endif

#endif // WIN32
}


bool Thread::start()
{
    if (isRunning())
    {
        PreDprintf("failed! Thread::isRuning 上次的线程未结束，不能开始新的线程\n");
        return false;
    }

    bCreateSuccess=true;
    //先设置,创建线程失败再复位
    setContinueRun(true);  //
    setRunning(true);

#ifdef _WIN32
    m_thread = (HANDLE)_beginthreadex(NULL,m_stackSize,&ThreadCallFun,this, 0, &m_ThreadId);   
    if (m_thread==NULL)
    {
        setRunning(false);
        setContinueRun(false);  //
        bCreateSuccess=false;
        PreDprintf("failed! at CreateThread()\n");
        return false; 
    }
    return true;
#else
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (m_stackSize!=0)   //use default 
    {
        pthread_attr_setstacksize(&attr,m_stackSize);        //设置线程栈大小 
    }
    //pthread_attr_getstacksize(&attr,&m_stackSize);  //获取线程栈大小 
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); //不分离 否则不可以join
    //pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);  //不分离,需要每次运行完成调用join才释放线程资源
    //if(pthread_create(&m_thread, &attr,ThreadCallFun,this) == 1)    //
    if(pthread_create(&m_thread,&attr,ThreadCallFun,this)!=0)    //
    {
        setRunning(false);
        setContinueRun(false);  //
        bCreateSuccess=false;
        PreDprintf("failed! at CreateThread()\n");
        return false; 
    }

    //attr就是给pthread_create使用的，当调用了pthread_create后，attr的属性值就设置到了tid的线程数据结构中去了，
    //而不再需要attr了。当然，紧接着就可以pthread_attr_destroy(&attr)了。
    pthread_attr_destroy(&attr);
#endif

    //setContinueRun(true);
    //setRunning(true);   //线程开启成功，立即设置运行状态否则, ThreadCallFun里面设置可能会延时导致waitFinish()失败
    //Thread::sleep_for(1);//Sleep(20);
    return true; 
}



#if 0
//pthread_attr_t attr;
//pthread_t tid;
///*初始化属性值，均设为默认值*/
//pthread_attr_init(&attr); 
//pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
//pthread_create(&tid, &attr, (void *) my_function, NULL);
//3．线程分离状态
//    线程的分离状态决定一个线程以什么样的方式来终止自己。非分离的线程终止时，其线程ID和退出状态将保留，
//直到另外一个线程调用pthread_join.分离的线程在当它终止时，所有的资源将释放，我们不能等待它终止。
//    设置线程分离状态的函数为 pthread_attr_setdetachstate（pthread_attr_t *attr, int detachstate）。第二个参数可选为PTHREAD_CREATE_DETACHED（分离线程）和 PTHREAD _CREATE_JOINABLE（非分离线程）。这里要注意的一点是，
//如果设置一个线程为分离线程，而这个线程运行又非常快，它很可能在 pthread_create函数返回之前就终止了，
//它终止以后就可能将线程号和系统资源移交给其他的线程使用，这样调用pthread_create的线程就得到了错误的线程号。
//要避免这种情况可以采取一定的同步措施，最简单的方法之一是可以在被创建的线程里调用 pthread_cond_timewait函数，
//让这个线程等待一会儿，留出足够的时间让函数pthread_create返回。设置一段等待时间，是在多线程编程里常用的方法。


//纠正一下我上面最后的结论.那个结论曾经是来自于我早先看过的一本书.现在看来是错误的.
//但现在我又查实了一下资料,并经过测试,得到结论,可以让主线程先退出,而让其他的线程继续运行,当然,整个进程也还在运行.
//方法出奇地简单,就是在main函数中,不要使用exit()或者return命令,而是调用pthread_exit,就可以仅仅让让主线程先退出了.
#endif

//#include<atomic>
//#include<mutex>



//线程函数
Thread::FunRet_t STDCALL Thread::ThreadCallFun(void* pThis)
{
   //static std::atomic<int> gThreadCount(0);  //调试用统计线程总数

    //printf("ThreadCallFun()--Begin pThread:%p ",pThis);
    Thread *p = (Thread*)pThis;  //static_cast<lcw_thread*>(arg);//指向派生类指针转化为基类对象指针
    p->setContinueRun(true);
    p->setRunning(true); 
    //PreDprintf("Begin: Count:%d\n",(int)(++gThreadCount));
    p->run();              
    p->setRunning(false); //Run()运行完了
    p->setContinueRun(false);
    //PreDprintf("End: Count:%d\n",(int)(--gThreadCount));

#ifdef _WIN32
    _endthreadex(0);
#endif
    return (FunRet_t)NULL;
}


void Thread::waitFinish()
{
    //PRINT_FUN_TRACE

#ifdef _WIN32  
    //Thread::sleep_for(1);
    if (NULL!=m_thread)  
    {  
        //if (isRunning())
        {
            WaitForSingleObject(m_thread,INFINITE); //如果线程已经结束,不能WaitForSingleObject?
        }
        CloseHandle(m_thread);  
        m_thread=NULL;  
    }  
#else


#if 1
    //分离模式不需要join
    //阻塞等待运行标记即可
    if (bCreateSuccess)   //m_thread无法得知创建时是否成功
    {
        while (isRunning())
        {
            sleep_for(1);
        }
        bCreateSuccess=false;
    }

#else
    //默认情况下通过pthread_create函数创建的线程是非分离属性的，在非分离的情况下，
    //当一个线程结束的时候，它所占用的系统资源并没有完全真正的释放，也没有真正终止。
    //只有在pthread_join函数返回时，该线程才会释放自己的资源。
    //pthread_join(m_thread,NULL);  
    //if(isRunning())
    if (bCreateSuccess)   //m_thread无法得知创建时是否成功
    {
        pthread_join(m_thread,NULL);
        bCreateSuccess=false;
    }
    m_thread = 0;
#endif


#endif
    //std::cout<<"Thread::waitFinish() "<<std::endl;
}


bool Thread::stop(unsigned int timeDelayInms)
{
    setContinueRun(false);
    while(isRunning()&&timeDelayInms>0)    //unsigned int 永远大于0，只能减一逼近
    {
            Thread::sleep_for(1);
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

void Thread::stopWaitFinish()
{
    stop();
    waitFinish();
}

void Thread::sleep(unsigned int ms)
{
#ifdef _WIN32
    return ::Sleep(ms);
#else
    ::usleep(ms*1000);
#endif
}


void Thread::yield()
{
#ifdef _WIN32
        Thread::sleep_for(0);  
#else
    //参考zthread
    bool result = false;
#if defined(HAVE_SCHED_YIELD)
    result = sched_yield() == 0;
#endif
   // return result;
#endif  
}

bool Thread::setPriority(Priority NewP)
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

bool Thread::getPriority(int* p)const
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

unsigned int Thread::getStackSize()
{
    return m_stackSize;
}

void Thread::setStackSize(unsigned int nSize)
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

void Thread::SetThreadAffinityMask(int mask)
{
    //return;

    if (m_thread!=NULL)
    {
         ::SetThreadAffinityMask(m_thread,mask);
    }
}
void Thread::SetThreadAffinityNum(unsigned int num)
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
class ThreadTest:public Thread
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
            Thread::sleep_for(3000);
            //Num--;
        }
    }
private:
    std::string sName;
};


#ifdef UNIT_TEST

int Test_Thread()
{
    //Thread* pThread=new ThreadTest(false);
    //pThread->Create();

    Thread* pThread=new ThreadTest("TestA");
    pThread->start();

    Thread* pThread2=new ThreadTest("TestB");
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
