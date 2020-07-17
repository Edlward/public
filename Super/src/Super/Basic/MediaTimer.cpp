#include "MediaTimer.h"



#ifdef _WIN32


//Windows 的多媒体定时器，待封装
#include <stdio.h>
#include <windows.h>
#pragma comment(lib,"winmm")
#pragma comment(lib,"winmm.lib")   //安装多媒体定时器支持库,也可手动将winmm.lib添加到工程中。
//#define TIMER_ACCURACY 1000  //定时器分辨率
//#define N 5                





 NAME_SPACE_MAIN_BEGIN
void CALLBACK timerCallFun(UINT uID,UINT uMsg,DWORD_PTR dwUser,DWORD_PTR dw1,DWORD_PTR dw2)
{
    //printf("Timer event callback Current:%lu\n",GetTickCount());
    //printf("uID:%u uMsg:%u dwUser:%lu dw1:%lu dw2:%lu\n",uID,uMsg,dwUser,dw1,dw2);
    
    CMediaTimer* p=(CMediaTimer*)dwUser;
    p->m_fun(p->m_pUser);
    //p->print();
}


//Sleep(N*TIMER_ACCURACY);  //等待定时器线程执行N*TIMER_ACCURACY ms


class CMediaTimerPrivate
{
public:
    //unsigned int TimerPeriod=1000/24;
    UINT wTimerRes;//=TimerPeriod;   //定义时间间隔,单位是ms
    UINT wAccuracy;                  //定义分辨率
    UINT TimerID;                      //定义定时器句柄
    TIMECAPS tc;
    CMediaTimerPrivate()
    {
        TimerID=0;
        wAccuracy=0;
        //tc=0;
    }
};

CMediaTimer::CMediaTimer(void)
{
    pimpl=new CMediaTimerPrivate;
}


CMediaTimer::~CMediaTimer(void)
{
    //timeKillEvent(pimpl->TimerID);
    //timeEndPeriod(pimpl->wAccuracy);
    CloseTimer();
    delete pimpl;
}

void CMediaTimer::CloseTimer()
{
    if (pimpl->TimerID!=0)
    {
    timeKillEvent(pimpl->TimerID);
       pimpl->TimerID=0;
    }
    if (pimpl->wAccuracy!=0)
    {
    timeEndPeriod(pimpl->wAccuracy);
        pimpl->wAccuracy=0;
    }
}

void CMediaTimer::print()
{
    printf("Fun:%p pUser:%p\n",m_fun,m_pUser);
}

bool CMediaTimer::CreateTimer(unsigned int TimerPeriod)
{
    pimpl->wTimerRes=TimerPeriod;
    //this->pUsers=pUser;

    if(timeGetDevCaps(&pimpl->tc,sizeof(TIMECAPS))==TIMERR_NOERROR)
    {
        //对于X86平台而言，最小时钟间隔是0.5ms，最大值大概是15.6001ms。在内部，
        //如果调度时限太小，系统就会频繁切换线程（因为时间片很快用完），从而导致性能降低。而如果调度时限太长，某些对于实时性要求很高的任务则不能接受。

      //设置合适的线程时钟中断频率
      //pimpl->wAccuracy=min(max(pimpl->tc.wPeriodMin,TimerPeriod),pimpl->tc.wPeriodMax); 
      //pimpl->wAccuracy=min(max(pimpl->tc.wPeriodMin,TimerPeriod/10),pimpl->tc.wPeriodMax);//为了精度更高用最小值。
        pimpl->wAccuracy=pimpl->tc.wPeriodMin;        //为了精度更高用最小值。

        printf("timeBeginPeriod()=%u\n",pimpl->wAccuracy);
        timeBeginPeriod(pimpl->wAccuracy);  //设置定时器分辨率
    }
    else
    {
        return false;
    }
    //设置定时器回调事件，回调函数形如fun
    if((pimpl->TimerID=timeSetEvent(pimpl->wTimerRes,pimpl->wAccuracy
        ,(LPTIMECALLBACK)timerCallFun,(DWORD_PTR)this,TIME_PERIODIC))==0)
    {
        printf("Can't count!\n" );
        return false;
    }
    return true;
}

#else

#warning ("Linux platform to be implemented")

#endif



NAME_SPACE_MAIN_END

