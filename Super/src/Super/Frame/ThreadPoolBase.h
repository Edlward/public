#ifndef ThreadPoolBase_H
#define ThreadPoolBase_H

#include <functional>

#include "../Basic/RingBuffer.h"


enum ETPSTATUS
{
    TPSTATUS_BEGIN=0X0,          //BEGIN/END֮��Ĳ�����Ч�� 
    TPSTATUS_Stoped,                 //��ʼ״̬,���Ѿ�ֹͣ,�������¿�ʼ
    TPSTATUS_RUN,                     //����״̬
    TPSTATUS_PAUSE,                  //������ͣ״̬
    TPSTATUS_STOP,                    //����׼��ֹͣ,��״̬ΪSTATUS_Stoped����ȫֹͣ
    TPSTATUS_END,
};


//Task��װ�����ͺ�������,������㷨��Ҫ��������ָ�룬Ԥ��һ��������ָ��,�������ݲ���
//int* �����߳� ��ͣ ֹͣ �˳���״̬,
typedef std::function<void(void*,void*,int*)> TaskFun;


//�㷨�����������䴫�εķ�װ
class FunObjWrap
{
public:
    //TaskFun mfun=0,�ᱨ��ִ���
    FunObjWrap(TaskFun mfun=nullptr,void* pv0=0,void* pv1=0,int* ctrl=0)
        :m_taskFun(mfun),p0(pv0),pd1(pv1),pCtrl(ctrl)
    {
    }
    ~FunObjWrap(){}
    TaskFun m_taskFun;
    void* p0;
    void* pd1;
    int* pCtrl;
    void doCallFun()
    {
        //if (this->m_taskFun!=NULL)   //ÿ���ж��˷�����,���������ʱ�жϼ���
        {
            this->m_taskFun(p0,pd1,pCtrl);
        }
    }
};

//�û�����Ĳ�������������ø�ThreadPoolSerialʹ��
//һ�����趨�㷨��Ҫ�����Ĳ���
class UserSerialTaskMgr
{
public:
    UserSerialTaskMgr(){}
    ~UserSerialTaskMgr(){}
    void addStep(const FunObjWrap& fun)
    {
        ListSteps.push_back(fun);
    }
    void clearSteps()
    {
        ListSteps.Clear();
    }
    FixedArray<FunObjWrap,50> ListSteps;
};

#endif

