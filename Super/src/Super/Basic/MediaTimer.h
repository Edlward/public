#ifndef MediaTimer_H_
#define MediaTimer_H_

#include "global.h"

//windows��ý�嶨ʱ�����߾��ȶ�ʱ������Ҫ�߾��Ȼص�����������������Ƶ���ŵ� 
//��װ��windows API Linux�´�ʵ��


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
    void CloseTimer();    //���´�֮ǰ����close
    void print();
public:
   MediaTimerCallBackFun m_fun;
   void* m_pUser;
private:
    CMediaTimerPrivate* pimpl;
};


NAME_SPACE_MAIN_END
#endif // 
