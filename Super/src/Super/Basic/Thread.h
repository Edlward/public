 #ifndef __THREAD_Header_
#define __THREAD_Header_

//跨平台简单轻量级线程类 by lizulin
//参考了juce zthread 等的部分实现
//Cross-platform simple lightweight thread class
//Reference library juce/zthread/QT

#include "global.h"

void SleepForMilliseconds(unsigned int milliseconds);
void SleepForSeconds(double seconds);


//线程的基类 
//继承此Thread基类，程序实现run函数，调用start，即可开始线程
//调用stop停止线程，但是不一定立即结束，仅仅是设置了不继续运行标记
//调用

NAME_SPACE_MAIN_BEGIN

//class ThreadPrivate;
class BASIC_API Thread
{
public:
#ifdef _WIN32
    typedef unsigned int FunRet_t;      //线程函数返回值
    typedef void* Handle;                   //创建线程的返回句柄   //typedef void* HANDLE; //去windows.h包含 自定义
#define STDCALL __stdcall                //调用约定
#else
    typedef void* FunRet_t;
    typedef unsigned long int pthread_t;
    typedef pthread_t Handle;
#define STDCALL
#endif

public:
    Thread();
    virtual ~Thread();
    //用法1：传入一个函数地址开启线程，返回句柄，线程栈大小(为0则使用系统默认),用句柄等待结束，等待线程自然结束，不能直接控制
    static Handle startThread(void* pFunAddr,void* pPara,unsigned int StackSize=0);   //传入线程函数指针（强转成void*）和线程参数
    static void waitFinish(Handle m_thread);

    //使用方法
    //跨平台线程函数声明参考:  Thread::FunRet STDCALL ThreadFun(void* pPara);
    //开启线程方法: 	Thread::Handle handle=Thread::startThread((void*)ThreadStatusUpdate,(void*)this);     //跨平台传入线程函数指针创建线程的方法
    //等待结束:        Thread::waitFinish(handle); 

    //用法2：继承本类实现run(),调用start()开始线程，
    //调用waitFinish(),等待线程结束,调用stop()停止线程,stopWaitFinish()停止线程并等待结束
    //run的实现里一般要用getContinueRun(),判断是否继续运行，方便外部控制其停止，否则外部无法控制线程停止
    bool start();
    void waitFinish();     //阻塞等待线程结束
    static FunRet_t STDCALL ThreadCallFun(void* pPara);
    virtual void run()=0;  //纯虚函数，由派生类实现

    Handle getThread(){return m_thread;}
    Handle m_thread;
    volatile bool bCreateSuccess;
#ifdef _WIN32
    unsigned int getThreadId(){return m_ThreadId;}
    unsigned int m_ThreadId;
    //HANDLE getThread(){return m_thread;}
#else
    //pthread_t getThread(){return m_thread;}
#endif
    
    //停止线程，仅仅是设置不继续运行标记，不会立即停止
    void stop()
    {
        setContinueRun(false);
    }                                              
    bool stop(unsigned int timeDelayInms);   //停止线程，超时则强制杀线程
    void stopWaitFinish();
    bool isRunning() const{return m_running;}
    bool isStoped()const {return !isRunning();}

    //mutex 是否有必要加锁
    //dispatch
    //线程优先级暂时考虑windows的
    enum Priority{
    Idle,              //空闲
    Low,              //最低
    Below,           //低于正常
    Normal,         //正常
    Above,          //高于正常
    High,            //最高优先级
    };


//一些第三方库会把sleep define成系统Sleep,导致冲突
#ifdef sleep
#undef sleep
#endif

    static void sleep(unsigned int ms);         //常用跨平台延时函数

    //一些垃圾第三方库会把sleep define成别的 定义相同功能sleep延时函数
    static void sleep_for(unsigned int ms){return sleep(ms);} 

    void yield();                                     //让出线程
    bool setPriority(Priority p);               //设置线程优先级
    bool getPriority(int* p)const;           //获取线程优先级


    //获取/设置线程栈大小
    //unsigned int getParentStackSize();      //获取父线程(使用者)栈大小 
    unsigned int getStackSize();                  //获取当前线程栈大小
    void setStackSize(unsigned int nSize);   //设置新的栈大小


    //为了提高实时性能，让线程与CPU核心绑定
    void SetThreadAffinityMask(int mask);                   //用二进制掩码可以同时指定多个核心
    void SetThreadAffinityNum(unsigned int num);     //指定单一Cpu核心序号

public:
    //取内部运行状态的引用,一起情况需要实时探测会用到
    volatile bool& getIsRuningRef(){return m_running;}
    volatile bool& getContinueRunRef(){return m_ContinueRun;}

protected:
    void setRunning(bool flag){m_running = flag;}              //不能由外部控制
    bool getContinueRun()const {return m_ContinueRun;}
    void setContinueRun(bool b){m_ContinueRun=b;}

private:
    Thread(const Thread& rhs);
    Thread& operator=(const Thread& rhs);
    //为了改变立即可见用 volatile 
    volatile bool m_running;                  //是否正在运行标记,表明线程运行状态
    volatile bool m_ContinueRun;          //是否继续运行标记，可以通过设置此值来停止线程
    unsigned int m_stackSize;   //开启线程栈传参数大小，为0使用默认值。
private:
    //更严格的线程安全要求 start 与stop过程需要用同一把锁加锁，待实现
   //ThreadPrivate* pimpl;

   //暂不
   //void join();
   //void detach();


    //参考自其他库实现，暂时无用--废弃
    //bool isReleased();
    //void setRelease(bool flag);
    //bool isAutoDeleteOnExit();                 //取退出时是否自动析构
    //void setDeleteOnExit(bool flag=true);    //退出时是否自动销毁
    //bool isDestructor();                           //是否已经调用了析构函数
    //bool m_release;
    //bool m_AutoDelete;     //线程结束释放自动释放 暂时无用
    //bool m_isDestructor;    //是否已经运行了析构函数标记，可移除
};


#ifdef UNIT_TEST
BASIC_API int Test_Thread();
#endif



NAME_SPACE_MAIN_END

#endif








 








#if 0    //可以继续参考以下案例改成函数指针调用方式

基于对象风格的线程类封装

基于对象风格的编程思想和面向对象的编程思想不同。面向对象使用纯虚函数为派生类提抽象接口，是一种抽象类（实现回调）；而基于对象使用具体类，在类里面通过注册函数实现回调；若是C风格的编程思想，则是定义全局的注册函数指针。 
基于对象风格的线程类封装代码如下： 
类头文件：

//start from the very beginning,and to create greatness
//@author: Chuangwei Lin
//@E-mail：979951191@qq.com
//@brief： 基于对象风格的线程类封装
#include <pthread.h>
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#ifndef _LCW_THREAD_H_
#define _LCW_THREAD_H_
    //线程基类
class lcw_thread
{
public:
    //重定义新类型
    typedef boost::function<void ()> LCW_Function;
    //构造函数，关键字explicit使其只能显式调用
    explicit lcw_thread(const LCW_Function& func);
    ~lcw_thread();//析构函数   
    void Start();
    void Join();
    void SetAutoDelete(bool AutoDelete);
private:
    static void* Thread_Routine(void* arg);
    void Run();
    LCW_Function func_;
    pthread_t lcwid;    
    bool AutoDelete;//是否自动销毁
};
#endif

//类实现函数：
//start from the very beginning,and to create greatness
//@author: Chuangwei Lin
//@E-mail：979951191@qq.com
//@brief： 基于对象风格的线程类封装
#include "lcw_thread.h"
using namespace std;
lcw_thread::lcw_thread(const LCW_Function& func) :func_(func) ,AutoDelete(false)
{
    cout << "构造函数lcw_thread" << endl;
}
lcw_thread::~lcw_thread()
{
    cout << "析构函数lcw_thread" << endl;
}
void lcw_thread::Start()//启动线程
{
    pthread_create(&lcwid,NULL,Thread_Routine,this);//this指针是参数
}
void lcw_thread::Join()
{//函数pthread_join用来等待一个线程的结束
    pthread_join(lcwid,NULL);
}
void* lcw_thread::Thread_Routine(void* arg)
{//参数arg其实是this指针
    lcw_thread* lcwthread = static_cast<lcw_thread*>(arg);//指向派生类指针转化为基类对象指针
    lcwthread->Run();
    if(lcwthread->AutoDelete)//
    {
        delete lcwthread;//销毁线程
    }
    return NULL;
}
void lcw_thread::SetAutoDelete(bool AutoDelete_)  
{
    AutoDelete = AutoDelete_;//设置是否自动销毁
}
void lcw_thread::Run()
{
    func_();
}
 
//回调函数和测试函数：
    //start from the very beginning,and to create greatness
    //@author: Chuangwei Lin
    //@E-mail：979951191@qq.com
    //@brief： 基于对象风格的线程类封装
#include "lcw_thread.h"
#include <unistd.h>

using namespace std;
//线程回调函数
void lcw1()
{
    cout << "lcw1回调" << endl;
} 
void lcw2(int count)
{
    for(int i = 1;i <= count;i++)
    {
        cout << "lcw2第"<< i << "次调用" <<endl;
    }
}
int main(int argc, char const *argv[])
{
    //创建线程,lcw为初始化函数
    lcw_thread t1(lcw1);
    lcw_thread t2(boost::bind(lcw2,5));

    t1.Start();
    t2.Start();

    t1.Join();
    t2.Join();
    return 0;
}
#endif
