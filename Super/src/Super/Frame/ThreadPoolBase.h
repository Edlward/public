#ifndef ThreadPoolBase_H
#define ThreadPoolBase_H

#include <functional>

#include "../Basic/RingBuffer.h"


enum ETPSTATUS
{
    TPSTATUS_BEGIN=0X0,          //BEGIN/END之间的才是有效的 
    TPSTATUS_Stoped,                 //初始状态,或已经停止,可以重新开始
    TPSTATUS_RUN,                     //运行状态
    TPSTATUS_PAUSE,                  //控制暂停状态
    TPSTATUS_STOP,                    //控制准备停止,当状态为STATUS_Stoped才完全停止
    TPSTATUS_END,
};


//Task封装函数和函数参数,大多数算法需要传递数据指针，预留一个父对象指针,两个数据参数
//int* 控制线程 暂停 停止 退出等状态,
typedef std::function<void(void*,void*,int*)> TaskFun;


//算法操作函数及其传参的封装
class FunObjWrap
{
public:
    //TaskFun mfun=0,会报奇怪错误
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
        //if (this->m_taskFun!=NULL)   //每次判断浪费性能,在添加任务时判断即可
        {
            this->m_taskFun(p0,pd1,pCtrl);
        }
    }
};

//用户管理的操作步骤可以设置给ThreadPoolSerial使用
//一步步设定算法需要操作的步骤
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

