#ifndef MediaTimer_H_
#define MediaTimer_H_

#include "global.h"

//windows多媒体定时器，高精度定时器，需要高精度回调的情况，如控制音视频播放等 
//封装自windows API Linux下待实现


NAME_SPACE_MAIN_BEGIN

typedef void (*MediaTimerCallBackFun)(void* pUser);

class CMediaTimerPrivate;
class CMediaTimer
{
public:
    CMediaTimer(void);
    ~CMediaTimer(void);
    void setCallBackFuntion(MediaTimerCallBackFun fun,void* pUser)
    {
        m_fun=fun;
        m_pUser=pUser;
    }
    bool CreateTimer(unsigned int TimerPeriod);
    void CloseTimer();    //重新打开之前必须close
    void print();
public:
   MediaTimerCallBackFun m_fun;
   void* m_pUser;
private:
    CMediaTimerPrivate* pimpl;
};


NAME_SPACE_MAIN_END
#endif // 
