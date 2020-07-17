#ifndef SequentialFrame_H_
#define SequentialFrame_H_

#include "../Basic/global.h"
#include "ThreadPoolBase.h"
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <array>
#include <memory>   //for share_ptr


//顺序算法模拟并行加速框架

//新的并行加速框架:注意理解使用 by lizulin  --待补充：
//用并行线程加条件等待实现串联的功能。
//相对旧的串行级联框架，线程利用率更高，只要存在未完成任务，线程就不会停止
//旧框架若前级任务未处理完，而后级处理完了，后级线程只能等待。
//音频帧处理环路中有些流程必须是顺序的，如均衡等是历史相关的，处理完成之后的推送也是必须是顺序的。

//帧必须顺序处理的串行级联框架
//顺序输入，顺序输出


class FunObjSyncWrap
{
public:
    FunObjSyncWrap()
    {
        FrameNumReady=0;
    }
    ~FunObjSyncWrap(){}
    FunObjWrap funWrap;
    void doProcess(void* pdata)
    {
        //funWrap.doCallFun();
        funWrap.m_taskFun(funWrap.p0,pdata,funWrap.pCtrl);
    }
    void doProcessWaitCondition(void* pdata,unsigned int CurrFrameNum)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while ((FrameNumReady!=CurrFrameNum))  // isDoing&&可能非必须
        {
            condition.wait(lock);
        }
        //isDoing=true;
        doProcess(pdata);
        FrameNumReady++;
        //isDoing=false;
        condition.notify_all();   //唤醒所有在等待的
    }
    std::condition_variable condition;
    std::mutex m_mutex;
    volatile unsigned int FrameNumReady;   //当前操作可以处理的帧号
    //volatile bool isDoing;
};


//并联+条件等待模拟串联的实现
template<typename DataType>
class ThreadPoolSerial
{

    struct  DataFrameNun
    {
        DataType data;
        unsigned int FrameNum;  
    };
    class SerialTaskMgr
    {
    public:
        SerialTaskMgr()
        {
            for (int k = 0; k<ListSteps.capacity(); k++)
            {
                ListSteps[k]=nullptr;   //初始化
            }
        }
        ~SerialTaskMgr()
        {
            clearSteps();
        }
        FixedArray<FunObjSyncWrap*,50> ListSteps;
        void addStep(const FunObjWrap& fun)
        {
            FunObjSyncWrap* pfunop=new FunObjSyncWrap;
            pfunop->funWrap=fun;
            ListSteps.push_back(pfunop);
        }
        void clearSteps()
        {
            for (size_t n=0;n<ListSteps.size();n++)
            {
                FunObjSyncWrap* p=ListSteps[n];
                delete[] p;
            }
            ListSteps.Clear();
        }
        void doCallFun(DataFrameNun& data)
        {
            for (size_t n=0;n<ListSteps.size();n++)
            {
                FunObjSyncWrap* p=ListSteps[n];
                p->doProcessWaitCondition(&data.data,data.FrameNum);// TODO 填充帧号
            }
        }

        void buildFromUserSerialTaskMgr(const UserSerialTaskMgr& ur,ThreadPoolSerial* parent)
        {
            clearSteps();
            for (int k = 0; k <ur.ListSteps.size(); k++)
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
    RingBuffer<DataFrameNun,200> m_DataQueue;          //待操作的数据，线程安全环形队列
    RingBuffer<DataType,200> m_ResDataQueue;     //处理完成的数据，线程安全环形队列

    std::mutex m_lock;                                         //同步
    std::condition_variable cv_task;                     //条件阻塞
    volatile bool stoped;                                                   //是否关闭提交  std::atomic<bool> 
    int stateCtrl;
    std::atomic<unsigned int>  idlThreadNum;    //空闲线程数量
    unsigned int m_WorkThreadNum;
    SerialTaskMgr m_SerialTaskMgr;
    bool bHasThreadRuning;
private:
    volatile unsigned int FrameNumCountPush;   //若作为音频帧生成级，每次使用完并推送成功+1，每次模式切换复位为0.
    volatile unsigned int FrameNumReadyPush;   //当前操作可以处理的帧号
    volatile bool isDoingPush;
    std::condition_variable conditionPush;
    std::mutex m_mutexPush;
public:
    ThreadPoolSerial(unsigned int BufferSize=8,unsigned int  threadNum=4):stoped(false)
        ,m_DataQueue(BufferSize)
    {
        FrameNumCurr=0;
        FrameNumReadyPush=0;
        stateCtrl=TPSTATUS_Stoped;
        bHasThreadRuning=false;
        setThreadNum(threadNum);
        idlThreadNum=m_WorkThreadNum;
    }
    ~ThreadPoolSerial()
    {
        stopWaitFinish();
    }
    void setThreadNum(unsigned int  threadNum) //suggested value 2->8
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
    unsigned int idlThreadCount(){ return idlThreadNum;}
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

    //用取所有输出缓存的方法来清空线程池里的结果队列,假定不再输入的情况取所有输出。
    void getAllToClear(unsigned int timeout=100u)
    {
        DataType data;
        while (true)
        {
             bool retGet=m_ResDataQueue.getOnlySingle(data,timeout);
             if (!retGet)
             {
                 return;
             }
        }
    }
    //各线程的处理结果顺序放入结果队列,
    void doSequentialPush(DataFrameNun dataf)
    {
        std::unique_lock<std::mutex> lock(m_mutexPush);
        while (FrameNumReadyPush!=dataf.FrameNum)   //(isDoingPush&&
        {
            conditionPush.wait(lock);
        }
        //isDoingPush=true;
        pushBlockMayMultiple(dataf.data);
        FrameNumReadyPush++;
        //isDoingPush=false;
        conditionPush.notify_all();   //唤醒所有在等待的
    }
    bool pushBlockMayMultiple(const DataType& pCard)
    {
        while (stateCtrl==TPSTATUS_RUN)
        {
            //bool retpush=m_ResDataQueue.pushMayMultiple(pCard,false);
            bool retpush=m_ResDataQueue.pushMayMultiple(pCard,100u);
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
               bHasThreadRuning=true;

                //工作线程函数
                while(!this->stoped)
                {
                    while(this->stateCtrl==TPSTATUS_RUN)
                    {
                        DataFrameNun tmpdata;
                        if (!m_DataQueue.getMayMultiple(tmpdata,true))
                        {
                            continue;
                        }
                        idlThreadNum--;

                        //printf("get to process: pc:%p\n",(void*)tmpdata.data);
                        m_SerialTaskMgr.doCallFun(tmpdata);
                        //处理完成放到结果队列,顺序要求待续。
                        doSequentialPush(tmpdata);

                        //m_ResDataQueue.pushMayMultiple(tmpdata.data,false);

                        idlThreadNum++;
                    }

                    if(this->stateCtrl==TPSTATUS_PAUSE)
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
    }
    void stopWaitFinish() //手动停止并等待结束
    {
        this->stop();
        this->waitFinish();
    }
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<typename DataType>
class FunObjSerialWrap
{
public:
    FunObjSerialWrap()
    {
    }
    ~FunObjSerialWrap(){}
    FunObjWrap funWrap;

    DataType data;
    void doProcess(volatile int& state)
    {
         //DataType data;
        if(!BlockGet(data,state))
        {
            return;
        }

        //funWrap.m_taskFun(funWrap.p0,&data,funWrap.pCtrl);
        funWrap.m_taskFun(funWrap.p0,&data,0);

        BlockPush(data,state);
    }
    bool BlockGet(DataType& data,volatile int& state)
    {
        while (state==TPSTATUS_RUN)
        {
            bool retGet=pTaskBufferInput->getOnlySingle(data,false);
            if (retGet)
            {
                return true;
            }
        }
        return false;
    }

    bool BlockPush(DataType& data,volatile int& state)
    {
        while (state==TPSTATUS_RUN)
        {
            bool retGet=pTaskBufferOutput->pushOnlySingle(data,false);
            if (retGet)
            {
                return true;
            }
        }
        return false;
    }

    //本处理过程输入输出缓冲区指针
    void setInOutBuffer(RingBuffer<DataType>* in,RingBuffer<DataType>* out)
    {
        pTaskBufferInput=in;
        pTaskBufferInput=out;
    }
    RingBuffer<DataType>* pTaskBufferInput;
    RingBuffer<DataType>* pTaskBufferOutput;
};


//串联环路实现 思路二 内部串联RingBuffer加线程池实现
template<typename DataType>
class ThreadPoolSerialV2
{

    class SerialTaskMgr
    {
    public:
        SerialTaskMgr()
        {
            //for (int k = 0; k<ListSteps.capacity(); k++)
            //{
            //    ListSteps[k]=nullptr;   //初始化
            //}
        }
        ~SerialTaskMgr()
        {
            clearSteps();
        }
        FixedArray<FunObjSerialWrap<DataType>,50> ListSteps;
        void addStep(const FunObjWrap& fun)
        {
            //FunObjSerialWrap<DataType>* funop=new FunObjSerialWrap<DataType>;
            FunObjSerialWrap<DataType> funop;
            funop.funWrap=fun;
            ListSteps.push_back(funop);
        }
        void clearSteps()
        {
            for (size_t n=0;n<ListSteps.size();n++)
            {
                //FunObjSerialWrap<DataType>* p=ListSteps[n];
                //delete[] p;
            }
            ListSteps.Clear();
        }
        void buildFromUserSerialTaskMgr(const UserSerialTaskMgr& ur,ThreadPoolSerialV2<DataType>* parent)
        {
            clearSteps();
            for (int k = 0; k <ur.ListSteps.size(); k++)
            {
                FunObjWrap tmp=ur.ListSteps[k];

               // tmp.pCtrl=&parent->stateCtrl;   //记住控制状态制作,为了外部算法能立即响应退出
                addStep(tmp);
            }
        }
    };
    //本处理过程输入输出缓冲区指针,线程安全环形队列
    void setInOutBuffer(RingBuffer<DataType>* in,RingBuffer<DataType>* out)
    {
        pbufferInput=in;
        pbufferInput=out;
    }
    unsigned int m_bufferSize;
    RingBuffer<DataType>* pbufferInput;
    RingBuffer<DataType>* pbufferOutput;

    //内置的输入输出缓冲区,外部没有设定就用内置的,线程安全环形队列
    RingBuffer<DataType> m_bufferInput;
    RingBuffer<DataType> m_bufferOutput;
    std::vector<std::thread> ListThreadPool;       //线程池
    volatile bool stoped;                                        //是否关闭提交  std::atomic<bool> 
    volatile int stateCtrl;
    SerialTaskMgr m_SerialTaskMgr;

    std::vector<RingBuffer<DataType>*> listTmpringbuffer;

    //清理时从中间缓冲区取数据，以前情况用new指针要让其有机会释放，待续
    std::vector<DataType> clear()
    {
        return std::vector<DataType>();
    }
private:
public:
     ThreadPoolSerialV2(size_t bufferSize=8):stoped(false)
         ,m_bufferSize(bufferSize)
        ,m_bufferInput(bufferSize),m_bufferOutput(bufferSize)
    {
        stateCtrl=TPSTATUS_Stoped;
        pbufferInput=&m_bufferInput;
        pbufferOutput=&m_bufferOutput;
    }
    ~ThreadPoolSerialV2()
    {
        stopWaitFinish();
    }
    void setSerialTaskMgr(const UserSerialTaskMgr& ns)
    {
        m_SerialTaskMgr.buildFromUserSerialTaskMgr(ns,this);
    }
    bool pushData(const DataType& data,unsigned int timeout=100u)
    {
         return pbufferInput->pushOnlySingle(data,timeout);
    }
    bool getResData(DataType& data,unsigned int timeout=100u)
    {
         return pbufferOutput->getOnlySingle(data,timeout);
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

        //没有要执行的算法
        if (m_SerialTaskMgr.ListSteps.empty())
        {
            printf("ThreadPool restart failed! have no Task!\n");
            return;
        }
        //串联环路头尾指针
        RingBuffer<DataType>* pbufferHead=this->pbufferInput;
        RingBuffer<DataType>* pbufferTail=this->pbufferOutput;

        for (int k = 0; k < listTmpringbuffer.size(); k++)
        {
              RingBuffer<DataType>*&  pbufferTmp=listTmpringbuffer.at(k);
              delete pbufferTmp;
        }
        listTmpringbuffer.clear();

        for (unsigned int  n= 0;n<m_SerialTaskMgr.ListSteps.size();++n)
        {   
           FunObjSerialWrap<DataType>& pcurrItem=m_SerialTaskMgr.ListSteps[n];

           //串联环路第一个级算法输入连接头
           if (n==0)
           {
               pcurrItem.pTaskBufferInput=pbufferHead;
               pcurrItem.pTaskBufferOutput=pbufferTail;
               continue;
           }

           //串联环路中间级的输入就是其上一级的输出
           if (n>0&&n<m_SerialTaskMgr.ListSteps.size())
           {
                 RingBuffer<DataType>* pbufferTmp=new RingBuffer<DataType>(m_bufferSize);
                 listTmpringbuffer.push_back(pbufferTmp);

                 FunObjSerialWrap<DataType>& plastItem=m_SerialTaskMgr.ListSteps[n-1];

                 //上一级的输出给本级输入，为同一个缓冲区
                 plastItem.pTaskBufferOutput=pbufferTmp;
                 pcurrItem.pTaskBufferInput=pbufferTmp;

                 pcurrItem.pTaskBufferOutput=pbufferTail;  //若没有下一级，本级直接输出输出
           }
           ////串联环路最后一级算法连接尾
           //if ((n+1)==m_SerialTaskMgr.ListSteps.size())  
           //{
           //    pcurrItem->pbufferOutput=pbufferTail;
           //}
        }




        for (unsigned int  n= 0;n<m_SerialTaskMgr.ListSteps.size();++n)
        {   
            FunObjSerialWrap<DataType>& pcurrItem=m_SerialTaskMgr.ListSteps[n];

           //开启线程，待续,静态函数实现。
            ////初始化线程数量
            ListThreadPool.emplace_back(
                [this,&pcurrItem]
            {
                printf("thread is runing num\n");
             
                //工作线程函数
                while(!this->stoped)
                {
                    while(this->stateCtrl==TPSTATUS_RUN)
                    {
                         pcurrItem.doProcess(this->stateCtrl);
                    }

                    if(this->stateCtrl==TPSTATUS_PAUSE)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));  //this 线程？
                        continue;
                    }
                    else if(this->stateCtrl==TPSTATUS_STOP)
                    {
                         printf("thread will finish\n");
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
        stateCtrl=STATUS_PAUSE;
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

    void waitFinish()  //等待线程池内部所有线程运行结束
    {
        for (std::thread& thread : ListThreadPool) 
        {
            //thread.detach(); // 让线程“自生自灭”
            if(thread.joinable())
            {
                thread.join(); // 等待任务结束， 前提：线程一定会执行完
            }
        }
        stateCtrl=TPSTATUS_Stoped;   //完全停止标记

        //清理中间缓冲区
        for (int k = 0; k < listTmpringbuffer.size(); k++)
        {
            RingBuffer<DataType>*&  pbufferTmp=listTmpringbuffer.at(k);
            delete pbufferTmp;
        }
        listTmpringbuffer.clear();
    }
    void stopWaitFinish() //手动停止并等待结束
    {
        stop();
        waitFinish();
    }
};


#endif

