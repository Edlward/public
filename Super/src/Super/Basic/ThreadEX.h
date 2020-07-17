 #ifndef __THREADEX_H_
#define __THREADEX_H_


//TODO:
// Expend of std::thread
//or base of std::thread

#include "global.h"

#include <thread>





 
//�̵߳Ļ��� 
//�̳д�Thread���࣬����ʵ��run����������start�����ɿ�ʼ�߳�
//����stopֹͣ�̣߳����ǲ�һ�����������������������˲��������б��
//����

NAME_SPACE_MAIN_BEGIN

//class ThreadPrivate;
class BASIC_API ThreadEx
{
#ifdef _WIN32
    typedef unsigned int FunRet_t;       //�̺߳�������ֵ
    typedef void* Handle;                  //�����̵߳ķ��ؾ��
   #define STDCALL __stdcall            //����Լ��
#else
    typedef void* FunRet_t;
    typedef unsigned long int pthread_t;
    typedef pthread_t Handle;                    
    #define STDCALL
#endif

public:
    ThreadEx();
    virtual ~ThreadEx();
    //�÷�1������һ��������ַ�����̣߳����ؾ�����߳�ջ��С(Ϊ0��ʹ��ϵͳĬ��),�þ���ȴ��������ȴ��߳���Ȼ����������ֱ�ӿ���
    static Handle startThread(void* pFunAddr,void* pPara,unsigned int StackSize=0);   //�����̺߳���ָ�루ǿת��void*�����̲߳���
    static void waitFinish(Handle m_thread);


    //ʹ�÷���
    //��ƽ̨�̺߳��������ο�:  ThreadFunRet_t STDCALL ThreadFun(void* pPara);
    //�����̷߳���: 	ThreadHandle handle=Thread::startThread((void*)ThreadStatusUpdate,(void*)this);     //��ƽ̨�����̺߳���ָ�봴���̵߳ķ���
    //�ȴ�����:        Thread::waitFinish(handle); 



    //�÷�2���̳б���ʵ��run(),����start()��ʼ�̣߳�
    //����waitFinish(),�ȴ��߳̽���,����stop()ֹͣ�߳�,stopWaitFinish()ֹͣ�̲߳��ȴ�����
    //run��ʵ����һ��Ҫ��getContinueRun(),�ж��Ƿ�������У������ⲿ������ֹͣ�������ⲿ�޷������߳�ֹͣ
    bool start();
    void waitFinish();     //�����ȴ��߳̽���
    static FunRet_t STDCALL ThreadCallFun(void* pPara);
    virtual void run()=0;  //���麯������������ʵ��

    Handle getThread(){return m_thread;}
    Handle m_thread;  
#ifdef _WIN32
    unsigned int getThreadId(){return m_ThreadId;}
    unsigned int m_ThreadId;
    //HANDLE getThread(){return m_thread;}
#else
    //pthread_t getThread(){return m_thread;}
#endif
    
    //ֹͣ�̣߳����������ò��������б�ǣ���������ֹͣ
    void stop()
    {
        setContinueRun(false);
    }                                              
    bool stop(unsigned int timeDelayInms);   //ֹͣ�̣߳���ʱ��ǿ��ɱ�߳�
    void stopWaitFinish();
    bool isRunning() const{return m_running;}
    bool isStoped()const {return !isRunning();}

    //mutex �Ƿ��б�Ҫ����
    //dispatch
    //�߳����ȼ���ʱ����windows��
    enum Priority{
    Idle,              //����
    Low,              //���
    Below,           //��������
    Normal,         //����
    Above,          //��������
    High,            //������ȼ�
    };


//һЩ����������sleep define��ϵͳSleep,���³�ͻ
#ifdef sleep
#undef sleep
#endif

    static void sleep(unsigned int ms);         //���ÿ�ƽ̨��ʱ����

    //һЩ��������������sleep define�ɱ�� ������ͬ����sleep��ʱ����
    static void sleep_for(unsigned int ms){return sleep(ms);} 

    void yield();                                     //�ó��߳�
    bool setPriority(Priority p);               //�����߳����ȼ�
    bool getPriority(int* p)const;           //��ȡ�߳����ȼ�


    //��ȡ/�����߳�ջ��С
    //unsigned int getParentStackSize();      //��ȡ���߳�(ʹ����)ջ��С 
    unsigned int getStackSize();                  //��ȡ��ǰ�߳�ջ��С
    void setStackSize(unsigned int nSize);   //�����µ�ջ��С


    //Ϊ�����ʵʱ���ܣ����߳���CPU���İ�
    void SetThreadAffinityMask(int mask);                   //�ö������������ͬʱָ���������
    void SetThreadAffinityNum(unsigned int num);     //ָ����һCpu�������

public:
    //ȡ�ڲ�����״̬������,һ�������Ҫʵʱ̽����õ�
    volatile bool& getIsRuningRef(){return m_running;}
    volatile bool& getContinueRunRef(){return m_ContinueRun;}

protected:
    void setRunning(bool flag){m_running = flag;}              //�������ⲿ����
    bool getContinueRun()const {return m_ContinueRun;}
    void setContinueRun(bool b){m_ContinueRun=b;}

private:
    ThreadEx(const ThreadEx& rhs);
    ThreadEx& operator=(const ThreadEx& rhs);
    //Ϊ�˸ı������ɼ��� volatile 
    volatile bool m_running;                  //�Ƿ��������б��,�����߳�����״̬
    volatile bool m_ContinueRun;          //�Ƿ�������б�ǣ�����ͨ�����ô�ֵ��ֹͣ�߳�
    unsigned int m_stackSize;   //�����߳�ջ��������С��Ϊ0ʹ��Ĭ��ֵ��
private:
    //���ϸ���̰߳�ȫҪ�� start ��stop������Ҫ��ͬһ������������ʵ��
   //ThreadPrivate* pimpl;

   //�ݲ�
   //void join();
   //void detach();
};


#ifdef UNIT_TEST
BASIC_API int Test_ThreadEx();
#endif



NAME_SPACE_MAIN_END

#endif

