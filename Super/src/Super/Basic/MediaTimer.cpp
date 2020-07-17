#include "MediaTimer.h"



#ifdef _WIN32


//Windows �Ķ�ý�嶨ʱ��������װ
#include <stdio.h>
#include <windows.h>
#pragma comment(lib,"winmm")
#pragma comment(lib,"winmm.lib")   //��װ��ý�嶨ʱ��֧�ֿ�,Ҳ���ֶ���winmm.lib��ӵ������С�
//#define TIMER_ACCURACY 1000  //��ʱ���ֱ���
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


//Sleep(N*TIMER_ACCURACY);  //�ȴ���ʱ���߳�ִ��N*TIMER_ACCURACY ms


class CMediaTimerPrivate
{
public:
    //unsigned int TimerPeriod=1000/24;
    UINT wTimerRes;//=TimerPeriod;   //����ʱ����,��λ��ms
    UINT wAccuracy;                  //����ֱ���
    UINT TimerID;                      //���嶨ʱ�����
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
        //����X86ƽ̨���ԣ���Сʱ�Ӽ����0.5ms�����ֵ�����15.6001ms�����ڲ���
        //�������ʱ��̫С��ϵͳ�ͻ�Ƶ���л��̣߳���Ϊʱ��Ƭ�ܿ����꣩���Ӷ��������ܽ��͡����������ʱ��̫����ĳЩ����ʵʱ��Ҫ��ܸߵ��������ܽ��ܡ�

      //���ú��ʵ��߳�ʱ���ж�Ƶ��
      //pimpl->wAccuracy=min(max(pimpl->tc.wPeriodMin,TimerPeriod),pimpl->tc.wPeriodMax); 
      //pimpl->wAccuracy=min(max(pimpl->tc.wPeriodMin,TimerPeriod/10),pimpl->tc.wPeriodMax);//Ϊ�˾��ȸ�������Сֵ��
        pimpl->wAccuracy=pimpl->tc.wPeriodMin;        //Ϊ�˾��ȸ�������Сֵ��

        printf("timeBeginPeriod()=%u\n",pimpl->wAccuracy);
        timeBeginPeriod(pimpl->wAccuracy);  //���ö�ʱ���ֱ���
    }
    else
    {
        return false;
    }
    //���ö�ʱ���ص��¼����ص���������fun
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

