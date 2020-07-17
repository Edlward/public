#ifndef _Timer_Header_
#define _Timer_Header_

#include "global.h"

#include <vector>
#include <mutex>

#include "Thread.h"

//各种用途下的定时器  create by lizulin 20180903

NAME_SPACE_MAIN_BEGIN

class Timer;
//通用定时器,可以同时维护多种不同定时频率，回调间隔时间精度为1ms。

class MediaTimer;
//一种高精度的定时器，可以用在多媒体播放的精确回调,保证长时间高精度运行,回调间隔高度精确
//例如电影播放 要求声音视频 回调间隔必须为24/25hz等。


//一般定时器回调函数的形式
//返回值作用是在回调函数里控制结束,返回0,继续运行，其他则结束
//回调传入参数:timerID定时器id,times已运行次数，pUser,pData为用户设置定时器时传入的私有参数值
typedef int  (*TimerCallFun)(int timerID,uint32_t times,void* pUser,void* pData);


//时间计数器类型
//typedef uint32_t TSize_t ;
//enum 
//{
//    INVALID_NUM=UINT32_MAX,
//};

struct  TimerInfo
{
    int timerID;                       //每个定时器返回唯一ID
    double peroid;                  //定时器回调周期,单位ms
    double begin_time;           //本定时器开始时的绝对时间,首次使用刷新
    double calltime;               //回调到期时间
    uint32_t timerCount;        //本定时器会运行几次  默认0xFFFFFFFF;表示永远运行,为0将不会被运行
    uint32_t alreadyCount;     //已经运行了多少次
    TimerCallFun pCbFun;      //回调函数,默认为null则调用内部事件处理虚函数。否则调用外部设定的
    void* pUser;                     //存放用户私用数据
    void* pData;                     //存放用户私用数据--预留
    void initTime();
    void updataNextTime();                 //刷新内部计数准备下一次
    TimerInfo()
    {
        peroid=0;
        calltime=0;
        timerCount=0;
        alreadyCount=0;
        timerID=0;
        pCbFun=0;
        pUser=0;
        pData=0;
    }
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//通用定时器,可以同时维护多种不同定时频率，回调间隔时间精度为1ms。
class Timer//:public Thread
{
    //为了避免线程类多继承使用冲突，用组合
    class TimerThread:public Thread
    {
    public:
        TimerThread(Timer* ptimer):ParentTimer(ptimer){}
        ~TimerThread(){}
        virtual void run();
        Timer* ParentTimer;
    };
    TimerThread timerThread;
private:
    S_DISABLE_COPY(Timer)
public:
    Timer(void):timerThread(this)
    {
        TimerIDbegin=0;
    }
    virtual ~Timer(void)
    {
    }

    //以下所有方法都必须在定时器停止的状态运行，否则非线程安全

    //设置定时器，返回timerID  //只有一个定时器
    int SetTimer(double PeriodInms,TimerCallFun pCbFun=0,void* pUser=0,void* pData=0,uint32_t times=UINT32_MAX)
    {
        ListTimer.clear();                //只保留一个
        return AddTimer(PeriodInms,pCbFun,pUser,pData,times);
    }
    //增加定时器，返回timerID PeriodInms是周期 times运行次数 默认永久运行
    int AddTimer(unsigned int PeriodInms,TimerCallFun pCbFun=0,void* pUser=0,void* pData=0,uint32_t times=UINT32_MAX)        //以毫秒为单位的周期
    {
        return AddTimer((double)PeriodInms,pCbFun,pUser,pData,times);
    }
    int AddTimerHz(unsigned int FreqHz,TimerCallFun pCbFun=0,void* pUser=0,void* pData=0,uint32_t times=UINT32_MAX)         //直接指定周期
    {
        return AddTimer(1000.0/FreqHz,pCbFun,pUser,pData,times);
    }
    //外部调用
    void RemovTimer(int timerID);
    //精度更高的方法
    int AddTimer(double PeriodInms,TimerCallFun pCbFun=0,void* pUser=0,void* pData=0,uint32_t times=UINT32_MAX);

    //开始定时器
    void StartTimer(){timerThread.start();}
    //停止定时器
    void StopTimer(){timerThread.stopWaitFinish();}

    //定时器是否在运行
    bool isTimerRuning()const{return timerThread.isRunning();}

    //移除所有定时器,重启定时器生效
    void CleanTimer();     

    //定时器回调函数，回调定时器唯一id,以及本定时器以及运行多少次了。
    virtual void TimerEvent(int id,uint32_t times,void* pUser=0,void* pData=0);    
private:
    //run 线程内部调用
    //static void RemovTimer(int timerID,std::vector<TimerInfo>& ListTimer);
    int TimerIDbegin;
    std::vector<TimerInfo> ListTimer;    //主要是遍历操作,少有删除操作用vector
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//高精度多媒体定时器,用在多媒体播放的精确回调,保证长时间高精度运行,回调间隔高度精确
//例如电影播放 要求声音视频 回调间隔必须为24/25hz等。

//只维护一种定时器频率，保证高精度的定时回调间隔,ns级的精度，取决于系统最高时钟精度
class MediaTimer
{
    //为了避免线程类继承使用冲突，用组合
    class TimerThread:public Thread
    {
    public:
        TimerThread(MediaTimer* ptimer):ParentTimer(ptimer){}
        ~TimerThread(){}
        virtual void run();
        MediaTimer* ParentTimer;
    };
    TimerThread timerThread;
private:
    S_DISABLE_COPY(MediaTimer)
public:
    MediaTimer(void):timerThread(this)
    {
        TimerIDbegin=0;
    }
    virtual ~MediaTimer(void)
    {
    }

    //以下所有方法都必须在定时器停止的状态运行，否则非线程安全
    //设置定时器 PeriodInms是周期单位ms times运行次数 默认永久运行 pUser存放用户数据
    void SetTimer(double PeriodInms,TimerCallFun pCbFun=0,void* pUser=0,void* pData=0,uint32_t times=UINT32_MAX);

    //设置定时器，指定周期
    void SetTimerHz(unsigned int FreqHz,TimerCallFun pCbFun=0,void* pUser=0,void* pData=0,uint32_t times=UINT32_MAX)         //直接指定周期
    {
        return SetTimer(1000.0/FreqHz,pCbFun,pUser,pData,times);
    }

    //开始定时器
    void StartTimer(){timerThread.start();}
    //停止定时器
    void StopTimer(){timerThread.stopWaitFinish();}

    //定时器是否在运行
    bool isTimerRuning()const{return timerThread.isRunning();}

    //定时器回调函数，回调定时器唯一id,以及本定时器以及运行多少次了。
    virtual void TimerEvent(int id,uint32_t times,void* pUser=0,void* pData=0);    
private:
    int TimerIDbegin;
    TimerInfo m_TimerInfo;
};





////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



//线程安全的定时器
//特殊用途会用到的,比如网络服务器中每个客户线程需要每隔一定时间做某种操作，
//如果每个线程开一个轮询定时器太浪费资源,完全可以全局共用一个定时器。
//但是定时器的添加/移除需要加锁。



struct  TimerInfoV2:public TimerInfo
{
    double begin_time;  //定时器添加时间,随机增删线程安全定时器会用到
    bool bContinue;       //是否继续运行,
};


#include "../Basic/MessageQueue.h"  //消息队列,用在与后台定时器工作线程通信

class TimerDynamic
{
    enum EMsgType
    {
        Msg_AddTimer=BaseMsg_User+1,
        Msg_RemoveTimer,
    };

    //为了避免线程类继承使用冲突，用组合
    class TimerThread:public Thread
    {
    public:
        TimerThread(TimerDynamic* ptimer):ParentTimer(ptimer)
        {
            //为了启动效率,初始化一些空间
            ListTimer.reserve(100);
            ListTmp.reserve(100);
        }
        ~TimerThread(){}
        virtual void run();
        TimerDynamic* ParentTimer;
        MessageQueue m_msgQueue;

        //为了启动效率，放类内
        std::vector<TimerInfoV2> ListTimer;
        std::vector<TimerInfoV2> ListTmp;
    };
    TimerThread timerThread;
private:
    S_DISABLE_COPY(TimerDynamic)
public:
    TimerDynamic(void):timerThread(this),TimerIDbegin(0){}
    virtual ~TimerDynamic(void){}

    //开始定时器，返回timerID PeriodInms是周期单位ms times运行次数 默认永久运行 pUser存放用户数据
    int StartTimer(double PeriodInms,TimerCallFun pCbFun=0,void* pUser=0,void* pData=0,uint32_t times=UINT32_MAX)
    {
        return AddTimer(PeriodInms,pCbFun,pUser,pData,times);
    }
     //根据timerID停止某定时器
    void StopTimer(int timerID);

    //停止所有定时器
    void StopAllTimer()
    {
        timerThread.stopWaitFinish();
    }
    //定时器是否在运行
    bool isTimerRuning()const{return timerThread.isRunning();}

    //定时器回调函数，回调定时器唯一id,以及本定时器以及运行多少次了。
    virtual void TimerEvent(int id,uint32_t times,void* pUser=0,void* pData=0);
private:
    void StartTimer(){timerThread.start();}
    int AddTimer(double PeriodInms,TimerCallFun pCbFun=0,void* pUser=0,void* pData=0,uint32_t times=UINT32_MAX);
    //线程内部调用
    //static void RemovTimer(int timerID,std::vector<TimerInfoV2>& ListTimer);
    int TimerIDbegin;
};

//全局单例访问接口
TimerDynamic& GetTimerDynamic();


#ifdef UNIT_TEST
BASIC_API int Test_Timer();
#endif



NAME_SPACE_MAIN_END
#endif
