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


//֡˳���޹صĲ��д�����
//1,֡˳�����룬˳�����
//2.�������룬�������������������
//����������ֿ��������ͣ����ߴ���ϲ��İ汾ִ�еİ汾

 
class SeqOut;       //�Ƿ�������˳�������,�����������Ⱥ�˳�����
class NoSeqOut;
 
//����+�����ȴ�ģ�⴮����ʵ��
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
                tmp.pCtrl=&parent->stateCtrl;   //��ס����״̬����,Ϊ���ⲿ�㷨��������Ӧ�˳�
                addStep(tmp);
            }
        }
    };

    unsigned int FrameNumCurr;       //֡���,ÿ��������λΪ0
    std::vector<std::thread> ListThreadPool;                //�̳߳�
    //std::queue<DataFrameNun> m_DataQueue;         //�����������ݣ��̰߳�ȫ����
    RingBuffer<DataFrameNun,200> m_DataQueue;    //�����������ݣ��̰߳�ȫ���ζ���
    RingBuffer<DataType,200> m_ResDataQueue;       //������ɵ����ݣ��̰߳�ȫ���ζ���

    std::mutex m_lock;                                         //ͬ��
    std::condition_variable cv_task;                     //��������
    volatile bool stoped;                                       //�Ƿ�ر��ύ  std::atomic<bool> 
    int stateCtrl;
    std::atomic<unsigned int>  idlThreadNum;    //�����߳�����
    unsigned int m_WorkThreadNum;
    SerialTaskMgr m_SerialTaskMgr;

private:
    volatile unsigned int FrameNumCountPush;   //����Ϊ��Ƶ֡���ɼ���ÿ��ʹ���겢���ͳɹ�+1��ÿ��ģʽ�л���λΪ0.
    volatile unsigned int FrameNumReadyPush;   //��ǰ�������Դ����֡��
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

    //���̵߳Ĵ�����˳�����������,
    void doSequentialPush(DataFrameNun dataf)
    {
        std::unique_lock<std::mutex> lock(m_mutexPush);
        while (FrameNumReadyPush!=dataf.FrameNum)
        {
            conditionPush.wait(lock);
        }
        pushBlockMayMultiple(dataf.data);
        FrameNumReadyPush++;
        conditionPush.notify_all();   //���������ڵȴ���
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
            //��ʼ���߳�����
            ListThreadPool.emplace_back(
                [this]
            {
                printf("thread is runing num\n");
                //�����̺߳���
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

                        //����ģ���趨,��Ҫ������������˳����˳�����
                        if (std::is_same<SeqOut,OutType>::value)
                        {
                               //������ɷŵ��������,˳��Ҫ�������
                              doSequentialPush(tmpdata);
                        }
                        else
                        {
                              pushBlockMayMultiple(tmpdata.data);   //����˳��,ֱ�����
                        }
                        //idlThreadNum++;
                    }
                    else if(this->stateCtrl==TPSTATUS_PAUSE)
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
        stateCtrl=TPSTATUS_Stoped;
    }
    void stopWaitFinish() //�ֶ�ֹͣ���ȴ�����
    {
        stop();
        waitFinish();
    }
};









#endif





