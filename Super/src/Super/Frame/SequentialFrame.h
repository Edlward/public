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


//˳���㷨ģ�Ⲣ�м��ٿ��

//�µĲ��м��ٿ��:ע�����ʹ�� by lizulin  --�����䣺
//�ò����̼߳������ȴ�ʵ�ִ����Ĺ��ܡ�
//��ԾɵĴ��м�����ܣ��߳������ʸ��ߣ�ֻҪ����δ��������߳̾Ͳ���ֹͣ
//�ɿ����ǰ������δ�����꣬���󼶴������ˣ����߳�ֻ�ܵȴ���
//��Ƶ֡����·����Щ���̱�����˳��ģ�����������ʷ��صģ��������֮�������Ҳ�Ǳ�����˳��ġ�

//֡����˳����Ĵ��м������
//˳�����룬˳�����


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
        while ((FrameNumReady!=CurrFrameNum))  // isDoing&&���ܷǱ���
        {
            condition.wait(lock);
        }
        //isDoing=true;
        doProcess(pdata);
        FrameNumReady++;
        //isDoing=false;
        condition.notify_all();   //���������ڵȴ���
    }
    std::condition_variable condition;
    std::mutex m_mutex;
    volatile unsigned int FrameNumReady;   //��ǰ�������Դ����֡��
    //volatile bool isDoing;
};


//����+�����ȴ�ģ�⴮����ʵ��
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
                ListSteps[k]=nullptr;   //��ʼ��
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
                p->doProcessWaitCondition(&data.data,data.FrameNum);// TODO ���֡��
            }
        }

        void buildFromUserSerialTaskMgr(const UserSerialTaskMgr& ur,ThreadPoolSerial* parent)
        {
            clearSteps();
            for (int k = 0; k <ur.ListSteps.size(); k++)
            {
                FunObjWrap tmp=ur.ListSteps[k];

                tmp.pCtrl=&parent->stateCtrl;   //��ס����״̬����,Ϊ���ⲿ�㷨��������Ӧ�˳�
                addStep(tmp);
            }
        }
    };

    unsigned int FrameNumCurr;       //֡���,ÿ��������λΪ0
    std::vector<std::thread> ListThreadPool;                //�̳߳�
    //std::queue<DataFrameNun> m_DataQueue;         //�����������ݣ��̰߳�ȫ����
    RingBuffer<DataFrameNun,200> m_DataQueue;          //�����������ݣ��̰߳�ȫ���ζ���
    RingBuffer<DataType,200> m_ResDataQueue;     //������ɵ����ݣ��̰߳�ȫ���ζ���

    std::mutex m_lock;                                         //ͬ��
    std::condition_variable cv_task;                     //��������
    volatile bool stoped;                                                   //�Ƿ�ر��ύ  std::atomic<bool> 
    int stateCtrl;
    std::atomic<unsigned int>  idlThreadNum;    //�����߳�����
    unsigned int m_WorkThreadNum;
    SerialTaskMgr m_SerialTaskMgr;
    bool bHasThreadRuning;
private:
    volatile unsigned int FrameNumCountPush;   //����Ϊ��Ƶ֡���ɼ���ÿ��ʹ���겢���ͳɹ�+1��ÿ��ģʽ�л���λΪ0.
    volatile unsigned int FrameNumReadyPush;   //��ǰ�������Դ����֡��
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
        //��������߳�����,̫��̫Сû������,��������������
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
    //�����߳�����
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

        //��Ч�������ݣ�ֱ����䵽Ŀ�ĵ�,���⿽�� ���ڴ�Ľṹ�壬����Ч�����ԡ�
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

    //��ȡ�����������ķ���������̳߳���Ľ������,�ٶ�������������ȡ���������
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
    //���̵߳Ĵ�����˳�����������,
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
        conditionPush.notify_all();   //���������ڵȴ���
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

            //��ʼ���߳�����
            ListThreadPool.emplace_back(
                [this]
            {

               printf("thread is runing num\n");
               bHasThreadRuning=true;

                //�����̺߳���
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
                        //������ɷŵ��������,˳��Ҫ�������
                        doSequentialPush(tmpdata);

                        //m_ResDataQueue.pushMayMultiple(tmpdata.data,false);

                        idlThreadNum++;
                    }

                    if(this->stateCtrl==TPSTATUS_PAUSE)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));  //this �̣߳�
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
    void pause()      //��ͣ����
    {
        stateCtrl=TPSTATUS_PAUSE;
    }
    void resume()    //�ָ�
    {
        stateCtrl=TPSTATUS_RUN;
    }
    void stop()       //ֹͣ
    {
        stoped=true;
        stateCtrl=TPSTATUS_STOP;
    }
    void waitFinish()  //�ȴ�����
    {
        for (std::thread& thread : ListThreadPool) 
        {
            //thread.detach(); // ���̡߳���������
            if(thread.joinable())
            {
                thread.join(); // �ȴ���������� ǰ�᣺�߳�һ����ִ����
            }
        }
        //�������������λ
        FrameNumCurr=0;
        FrameNumReadyPush=0;
    }
    void stopWaitFinish() //�ֶ�ֹͣ���ȴ�����
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

    //����������������������ָ��
    void setInOutBuffer(RingBuffer<DataType>* in,RingBuffer<DataType>* out)
    {
        pTaskBufferInput=in;
        pTaskBufferInput=out;
    }
    RingBuffer<DataType>* pTaskBufferInput;
    RingBuffer<DataType>* pTaskBufferOutput;
};


//������·ʵ�� ˼·�� �ڲ�����RingBuffer���̳߳�ʵ��
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
            //    ListSteps[k]=nullptr;   //��ʼ��
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

               // tmp.pCtrl=&parent->stateCtrl;   //��ס����״̬����,Ϊ���ⲿ�㷨��������Ӧ�˳�
                addStep(tmp);
            }
        }
    };
    //����������������������ָ��,�̰߳�ȫ���ζ���
    void setInOutBuffer(RingBuffer<DataType>* in,RingBuffer<DataType>* out)
    {
        pbufferInput=in;
        pbufferInput=out;
    }
    unsigned int m_bufferSize;
    RingBuffer<DataType>* pbufferInput;
    RingBuffer<DataType>* pbufferOutput;

    //���õ��������������,�ⲿû���趨�������õ�,�̰߳�ȫ���ζ���
    RingBuffer<DataType> m_bufferInput;
    RingBuffer<DataType> m_bufferOutput;
    std::vector<std::thread> ListThreadPool;       //�̳߳�
    volatile bool stoped;                                        //�Ƿ�ر��ύ  std::atomic<bool> 
    volatile int stateCtrl;
    SerialTaskMgr m_SerialTaskMgr;

    std::vector<RingBuffer<DataType>*> listTmpringbuffer;

    //����ʱ���м仺����ȡ���ݣ���ǰ�����newָ��Ҫ�����л����ͷţ�����
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

        //û��Ҫִ�е��㷨
        if (m_SerialTaskMgr.ListSteps.empty())
        {
            printf("ThreadPool restart failed! have no Task!\n");
            return;
        }
        //������·ͷβָ��
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

           //������·��һ�����㷨��������ͷ
           if (n==0)
           {
               pcurrItem.pTaskBufferInput=pbufferHead;
               pcurrItem.pTaskBufferOutput=pbufferTail;
               continue;
           }

           //������·�м伶�������������һ�������
           if (n>0&&n<m_SerialTaskMgr.ListSteps.size())
           {
                 RingBuffer<DataType>* pbufferTmp=new RingBuffer<DataType>(m_bufferSize);
                 listTmpringbuffer.push_back(pbufferTmp);

                 FunObjSerialWrap<DataType>& plastItem=m_SerialTaskMgr.ListSteps[n-1];

                 //��һ����������������룬Ϊͬһ��������
                 plastItem.pTaskBufferOutput=pbufferTmp;
                 pcurrItem.pTaskBufferInput=pbufferTmp;

                 pcurrItem.pTaskBufferOutput=pbufferTail;  //��û����һ��������ֱ��������
           }
           ////������·���һ���㷨����β
           //if ((n+1)==m_SerialTaskMgr.ListSteps.size())  
           //{
           //    pcurrItem->pbufferOutput=pbufferTail;
           //}
        }




        for (unsigned int  n= 0;n<m_SerialTaskMgr.ListSteps.size();++n)
        {   
            FunObjSerialWrap<DataType>& pcurrItem=m_SerialTaskMgr.ListSteps[n];

           //�����̣߳�����,��̬����ʵ�֡�
            ////��ʼ���߳�����
            ListThreadPool.emplace_back(
                [this,&pcurrItem]
            {
                printf("thread is runing num\n");
             
                //�����̺߳���
                while(!this->stoped)
                {
                    while(this->stateCtrl==TPSTATUS_RUN)
                    {
                         pcurrItem.doProcess(this->stateCtrl);
                    }

                    if(this->stateCtrl==TPSTATUS_PAUSE)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));  //this �̣߳�
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
    void pause()      //��ͣ����
    {
        stateCtrl=STATUS_PAUSE;
    }
    void resume()    //�ָ�
    {
        stateCtrl=TPSTATUS_RUN;
    }
    void stop()       //ֹͣ
    {
        stoped=true;
        stateCtrl=TPSTATUS_STOP;
    }

    void waitFinish()  //�ȴ��̳߳��ڲ������߳����н���
    {
        for (std::thread& thread : ListThreadPool) 
        {
            //thread.detach(); // ���̡߳���������
            if(thread.joinable())
            {
                thread.join(); // �ȴ���������� ǰ�᣺�߳�һ����ִ����
            }
        }
        stateCtrl=TPSTATUS_Stoped;   //��ȫֹͣ���

        //�����м仺����
        for (int k = 0; k < listTmpringbuffer.size(); k++)
        {
            RingBuffer<DataType>*&  pbufferTmp=listTmpringbuffer.at(k);
            delete pbufferTmp;
        }
        listTmpringbuffer.clear();
    }
    void stopWaitFinish() //�ֶ�ֹͣ���ȴ�����
    {
        stop();
        waitFinish();
    }
};


#endif

