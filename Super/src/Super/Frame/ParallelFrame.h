#ifndef ParallelFrame_H_
#define ParallelFrame_H_

#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <stdexcept>
//#include <array>
#include <memory>   //for share_ptr
#include "../Basic/RingBuffer.h"
#include "ThreadPoolBase.h"


//帧顺序无关的并行处理框架
//1,帧顺序输入，顺序输出
//2.乱序输入，无序输出，先完成先输出
//数据与任务分开设置推送，或者打包合并的版本执行的版本

 
class SeqOut;       //是否按照输入顺序串行输出,否则根据完成先后顺序输出
class NoSeqOut;
 
//并联+条件等待模拟串联的实现
template<typename DataType,class OutType=SeqOut>
class ThreadPoolParallel
{
    struct  DataFrameNun
    {
        DataType data;
        unsigned int FrameNum;  
    };
    class SerialTaskMgr
    {
    public:
        SerialTaskMgr(){}
        ~SerialTaskMgr()
        {
            clearSteps();
        }
        FixedArray<FunObjWrap,50> ListSteps;
        void addStep(const FunObjWrap& fun)
        {
            ListSteps.push_back(fun);
        }
        void clearSteps()
        {
            ListSteps.Clear();
        }
        void doCallFun(DataFrameNun& dataF)
        {
            for (size_t n=0;n<ListSteps.size();n++)
            {
                FunObjWrap& p=ListSteps[n];
                p.m_taskFun(p.p0,&dataF.data,p.pCtrl);
            }
        }
        void buildFromUserSerialTaskMgr(const UserSerialTaskMgr& ur,ThreadPoolParallel* parent)
        {
            clearSteps();
            for (int k=0;k<ur.ListSteps.size();k++)
            {
                FunObjWrap tmp=ur.ListSteps[k];
                tmp.pCtrl=&parent->stateCtrl;   //记住控制状态制作,为了外部算法能立即响应退出
                addStep(tmp);
            }
        }
    };

    unsigned int FrameNumCurr;       //帧序号,每次重启复位为0
    std::vector<std::thread> ListThreadPool;                //线程池
    //std::queue<DataFrameNun> m_DataQueue;         //待操作的数据，线程安全待续
    RingBuffer<DataFrameNun,200> m_DataQueue;    //待操作的数据，线程安全环形队列
    RingBuffer<DataType,200> m_ResDataQueue;       //处理完成的数据，线程安全环形队列

    std::mutex m_lock;                                         //同步
    std::condition_variable cv_task;                     //条件阻塞
    volatile bool stoped;                                       //是否关闭提交  std::atomic<bool> 
    int stateCtrl;
    std::atomic<unsigned int>  idlThreadNum;    //空闲线程数量
    unsigned int m_WorkThreadNum;
    SerialTaskMgr m_SerialTaskMgr;

private:
    volatile unsigned int FrameNumCountPush;   //若作为音频帧生成级，每次使用完并推送成功+1，每次模式切换复位为0.
    volatile unsigned int FrameNumReadyPush;   //当前操作可以处理的帧号
    volatile bool isDoingPush;
    std::condition_variable conditionPush;
    std::mutex m_mutexPush;
public:
     ThreadPoolParallel(uint32_t bufferSize=8,unsigned int  threadNum=4)
         :stoped(false),m_DataQueue(bufferSize)
    {
        FrameNumCurr=0;
        FrameNumReadyPush=0;
        stateCtrl=TPSTATUS_Stoped;
        setThreadNum(threadNum);
        idlThreadNum=m_WorkThreadNum;
    }
     ~ThreadPoolParallel()
    {
        stopWaitFinish();
    }
    void setThreadNum(unsigned int  threadNum)  //suggested value 2->8
    {
        //限制最大线程数量,太多太小没有意义,避免理解错误误用
        if (threadNum==0)
        {
            PreDprintf("warning threadNum:%u\n",threadNum);
            threadNum=1;
        }
        if (threadNum>10)
        {
            PreDprintf("warning threadNum:%u is too large!\n",threadNum);
            threadNum=10;
        }
        m_WorkThreadNum=threadNum;
    }
    //空闲线程数量
    unsigned int idlThreadCount() { return idlThreadNum;}
    void setSerialTaskMgr(const UserSerialTaskMgr& ns)
    {
        m_SerialTaskMgr.buildFromUserSerialTaskMgr(ns,this);
    }
    bool pushData(const DataType& data,unsigned int timeout=100u)
    {
        //printf("pushdata: pc:%p\n",(void*)data);
        //DataFrameNun tmp;
        //tmp.data=data;
        //tmp.FrameNum=FrameNumCurr++;
        //return m_DataQueue.pushOnlySingle(tmp,timeout);

        //高效推送数据，直接填充到目的地,避免拷贝 对于大的结构体，加速效果明显。
        DataFrameNun* pdst;
        bool retS=m_DataQueue.SearchPush(pdst,timeout);
        if (!retS)
        {
            return false;
        }
        pdst->data=data;
        pdst->FrameNum=FrameNumCurr++;
        m_DataQueue.ReleasePush();
        return true;
    }
    bool getResData(DataType& data,unsigned int timeout=100u)
    {
        return m_ResDataQueue.getOnlySingle(data,timeout);
    }

    //各线程的处理结果顺序放入结果队列,
    void doSequentialPush(DataFrameNun dataf)
    {
        std::unique_lock<std::mutex> lock(m_mutexPush);
        while (FrameNumReadyPush!=dataf.FrameNum)
        {
            conditionPush.wait(lock);
        }
        pushBlockMayMultiple(dataf.data);
        FrameNumReadyPush++;
        conditionPush.notify_all();   //唤醒所有在等待的
    }
    bool pushBlockMayMultiple(const DataType& pCard)
    {
        while (stateCtrl==TPSTATUS_RUN)
        {
            bool retpush=m_ResDataQueue.pushMayMultiple(pCard,false);
            if (retpush) //break;
            {
                return true;
            }
        }
        return false;
    }

    void Start()
    {
        if (this->stateCtrl!=TPSTATUS_Stoped)
        {
            printf("ThreadPool Start failed! still runing!\n");
            return;
        }

        this->stateCtrl=TPSTATUS_RUN;
        this->stoped=false;
        for (unsigned int  n= 0;n< m_WorkThreadNum;++n)
        {
            //初始化线程数量
            ListThreadPool.emplace_back(
                [this]
            {
                printf("thread is runing num\n");
                //工作线程函数
                while(!this->stoped)
                {
                    if(this->stateCtrl==TPSTATUS_RUN)
                    {
                        DataFrameNun tmpdata;
                        if (!m_DataQueue.getMayMultiple(tmpdata,true))
                        {
                            continue;
                        }
                        m_SerialTaskMgr.doCallFun(tmpdata);

                        //根据模板设定,需要根据数据输入顺序串行顺序输出
                        if (std::is_same<SeqOut,OutType>::value)
                        {
                               //处理完成放到结果队列,顺序要求待续。
                              doSequentialPush(tmpdata);
                        }
                        else
                        {
                              pushBlockMayMultiple(tmpdata.data);   //不管顺序,直接输出
                        }
                        //idlThreadNum++;
                    }
                    else if(this->stateCtrl==TPSTATUS_PAUSE)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));  //this 线程？
                    }
                    else if(this->stateCtrl==TPSTATUS_STOP)
                    {
                        return;
                    }
                }
            }
            );
        }
    }
    bool isRuning()
    {
        return stateCtrl==TPSTATUS_Stoped;
    }
    void pause()      //暂停处理
    {
        stateCtrl=TPSTATUS_PAUSE;
    }
    void resume()    //恢复
    {
        stateCtrl=TPSTATUS_RUN;
    }
    void stop()       //停止
    {
        stoped=true;
        stateCtrl=TPSTATUS_STOP;
    }
    void waitFinish()  //等待结束
    {
        for (std::thread& thread : ListThreadPool) 
        {
            //thread.detach(); // 让线程“自生自灭”
            if(thread.joinable())
            {
                thread.join(); // 等待任务结束， 前提：线程一定会执行完
            }
        }
        //所有任务结束复位
        FrameNumCurr=0;
        FrameNumReadyPush=0;
        stateCtrl=TPSTATUS_Stoped;
    }
    void stopWaitFinish() //手动停止并等待结束
    {
        stop();
        waitFinish();
    }
};









#endif





