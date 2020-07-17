#ifndef FUNCMODULE_H_
#define FUNCMODULE_H_

#include "Super/Basic/Thread.h"
#include "Super/Basic/SuperTime.h"           //高精度计数器，性能调优使用
#include "Super/Basic/BasicOperation.h"   //CPU占用率用


enum STATUS
{
    STATUS_BEGIN=0X0,                         //BEGIN/END之间的才是有效的 
    STATUS_UNKNOW=STATUS_BEGIN,   //未知状态
    STATUS_PLAY,                                   // 状态，播放
    STATUS_PAUSE,                                 // 状态，暂停
    STATUS_STOP,                                   // 状态，停止
    STATUS_END,
};


class cardInput  //输入给声卡的数据结构
{
public:
    typedef int PcmType;
    enum 
    {
        CardTrackNum=128,
        PCM_TYPE_SIZE=sizeof(PcmType),
        PerFrameSamplesMax=4000
    };
    PcmType data[CardTrackNum][PerFrameSamplesMax];
    void setValidChannel(unsigned int n){ValidChannel=n;}
    void setLength(unsigned int len){length=len;}
    void setFrameRate(unsigned int n){FrameRate=n;}
    unsigned int getValidChannel()const {return ValidChannel;}
    unsigned int getLength() const{return length;}  
    unsigned int getFrameRate() const{return FrameRate;}
    unsigned int getSampleRate() const{return length*FrameRate;}     //采样率等于帧率乘以帧长24*2000=48000

    //辅助调试
    void FillTime();                       //填充当前时间
    void setFrameIndex(unsigned int n){CurrFrameNum=n;}
    unsigned int getFrameIndex()const {return CurrFrameNum;}
    unsigned long long curtime;  //声卡入口获取音频时填充当前时间
    unsigned int CurrFrameNum; //用在标识当前帧号，帧间相关的音频处理做唯一标识符，每推送一帧加一

private:
    unsigned int length;                     //每帧有效采样点数=采样率/帧率
    unsigned int FrameRate;              //帧速率，只能为24/25/30/48/50/60/90/100/120 
    unsigned int ValidChannel;          //
public:
    static unsigned int InOutChannel_Max;  
    bool hasSound[CardTrackNum]; 
    bool bProcessCompleted;
    void setProcessCompleted(bool flag){bProcessCompleted=flag;}
    bool getProcessCompleted()const {return bProcessCompleted;}

    cardInput();
    ~cardInput();
    void init(unsigned int ValidCh=128,unsigned int Len=2000,unsigned int FR=24);
    cardInput(const cardInput& R)  //拷贝构造
    {
        copyFrom(&R);
    }
    cardInput& operator=(const cardInput& R) //赋值构造
    {
        copyFrom(&R);
        return *this;
    }
    //常用辅助操作
    void copyFrom(const cardInput* pCard)
    {
    }
};



//可以兼容dll创建者，dll使用者，直接作为普通文件包含在exe程序里面的导出设定--add by lizulin
//dll创建者项目预定义BASIC_API_EXPORT 
//dll使用者项目预定义BASIC_API_IMPORT
//作为普通文件在exe程序中直接包含使用，不需要预定义


#include <condition_variable>
#include <mutex>

#if defined(_WIN32)
    #if defined(BASIC_API_EXPORT)
        #define BASIC_API __declspec(dllexport)
    #elif defined(BASIC_API_IMPORT)
        #define BASIC_API __declspec(dllimport)
    #else
        #define BASIC_API 
    #endif
#else
    #define BASIC_API 
#endif



class cardInput;
template<typename T> class ringbufferProcess;
template<typename T> class ringbuffer;

//任务队列，获取，推送，多处被复用，从funModule里面提取出来
class RingBufferMgr
{
    std::string sName;
private:
    RingBufferMgr(const RingBufferMgr& rhs);
    RingBufferMgr& operator=(const RingBufferMgr& rhs);
public:
    RingBufferMgr(	volatile int& playStatus,std::string sName=""):sName(sName),playStatus(playStatus)
      ,tmGet(std::string(sName+" RingGet").c_str()),tmPush(std::string(sName+" RingPush").c_str())
    {
        inputBuffer =NULL;
        outputBuffer = NULL;

        //临时
        CallCntGet=0;
        CallCntPush=0;
        GetUsedPeak=0;
        PushUsedPeak=0;
    }
    void setBuffer(void *input,void *output)// 设置输入、输出缓冲区指针
    {
        //inputBuffer = input;
        //outputBuffer = output;
        PRINT_FUN_NAME_GOON
        printf("input:%p output:%p ModuleName:%s\n",input,output,sName.c_str());

        this->inputBuffer= (ringbuffer<cardInput *> *)input;
        this->outputBuffer= (ringbuffer<cardInput *>*)output;
    }
    void* getBufferInput()const {return inputBuffer;}
    void* getBufferOutput()const {return outputBuffer;}
    bool pushBlock(cardInput* pCard);  //推送给下一级，默认阻塞，直到推送成功或播放停止才返回
    bool getBlock(cardInput*& pCard);  //从上一级获取，默认阻塞，直到获取成功或播放停止才返回
    bool pushSilent(cardInput* pCard);  //一般失败操作之后推送无声音频帧

    //可能存在多个线程同时获取/推送的版本，内部实现，为了效率分别实现，内部加锁版
    bool pushBlockMayMultiple(cardInput* pCard);  //推送给下一级，默认阻塞，直到推送成功或播放停止才返回
    bool getBlockMayMultiple(cardInput*& pCard);  //从上一级获取，默认阻塞，直到获取成功或播放停止才返回

protected:
    //临时变量,统计ringbuffer推送/获取过程耗时用
    unsigned int CallCntGet;
    unsigned int CallCntPush;
    unsigned int GetUsedPeak;
    unsigned int PushUsedPeak;
    SuperTime tmGet;
    SuperTime tmPush;

protected:
    //void *inputBuffer;                 // 输入缓冲区指针
    //void *outputBuffer;              // 输出缓冲区指针
public:
    ringbuffer<cardInput *> *inputBuffer;            //输入缓冲区指针
    ringbuffer<cardInput *> *outputBuffer;         
    volatile int& playStatus ;      //注意理解使用，为了在串并两种框架复用，设计为引用，
};






class BASIC_API funcModule:public RingBufferMgr,public Thread
{
    //char resv[10*1024*1024];
private:
    funcModule(const funcModule& rhs);
    funcModule& operator=(const funcModule& rhs);
protected:
    //void setName(std::string s){sName=s;}
    std::string sName;   //不同模块之间打印区分
    volatile int status;    // 播放状态  PLAY PAUSE  STOP 
    volatile int mode;    // 当前播放模式 线程应该无module概念。由外部控制不同mode开启不同线程即可
    //volatile int lastmode;    //上次的模式,根据现有架构模式改变先于清理操作,所以要记住上次的。

    //int sampleRate;     // 采样率
    //int frameRate;       // 帧率
    //int setSampleRate(const int newSampleRate);   // 设置采样率
    //int setFrameRate(const int newFrameRate);       // 设置帧率
public:
    funcModule(std::string sName="");
    virtual ~funcModule(){};  //为了delete 基类的时候派生类也会被调用
    virtual void run();
    virtual int doLoop()=0;
    void setStatus(const int newStatus);                      // 设置播放状态，返回错误代码
    int getStatus()const {return status;}                     // 读取播放状态
    void setMode(const int newMode);                        // 设置模式
    int getMode()const {return mode;}

    //线程模块在开始运行前被调用，可以重载实现一些初始化操作,重载最好也要调用本基类的
    virtual void InitState()
    {
        ResetTimerCount();
    };     
    //线程模块结束后会被调用，可以重载实现一些清理操作 重载最好也要调用本基类的
    virtual void ClearState()
    {

    };     

    //该结束时循环等待直到模式状态切换，如果立即return,模式还没有来得及改变又会重新进入当前模式
    int WaitModeChangedtoFinish(int lastMode,int lastStatus);  
    void printModeStatus();

////////////////////////////////////////////////////////////////////////////////
    //为了在同一个模块中复用printTimetoNow函数打印到不同步骤时间引入,不同打印过程需要不同历史计数
    enum  PStepIndex
    {
        P2N_Begin=0,
        P2N_End,
        P2N_Step3,
        P2N_Step4,
        P2N_Step5,
        P2N_Step6,
        P2N_Step7,
        P2N_Step8,
        P2N_Step9,
        P2N_SIZE,
    };
   //辅助调试使用，主要 统计音频处理耗时
    struct HistoryCount
    {
        unsigned int RecallCouter;
        unsigned long long SumTimeInTicks;
        unsigned long long peakUseTimeInTicks;    //音频处理耗时的峰值，测试用
        HistoryCount()
        {
            reset();
        }
        void reset()
        {
            SumTimeInTicks=0;
            peakUseTimeInTicks=0;
            RecallCouter=0;
        }
    };
    HistoryCount ArrayhisCnt[P2N_SIZE];
    void printTimetoNow(PStepIndex Step,unsigned long long beginTime,const char* name="",const char* name2="");   //打印音频帧从输入处，到某处的耗时。
    void resetPrint2Now()
    {
        for (int n=0;n<P2N_SIZE;n++)
        {
            ArrayhisCnt[n].reset();
        }
    }
protected:
    enum 
    {
        PRINT_Freq=512//*8,//10*24,  //时间打印频率  最好是二次幂
    };
    SuperTime timeTest;
    void getBeginTime();
    void getEndTimeAndPrint();
    void ResetTimerCount()
    {
        resetPrint2Now();
        timeTest.reSet();
    }

    virtual void CallAtProcessBegin(){};
    virtual void CallAtProcessEnd(){};
    //CPUInfo cpuInfo;
////////////////////////////////////////////////////////////////////////////////
public:
//为了适配新框架的分离封装
    virtual void doProcess(cardInput*& pCard);
    void doProcessWaitCondition(cardInput*& pCard);
    volatile unsigned int FrameNumCount;   //若作为音频帧生成级，每次使用完并推送成功+1，每次模式切换复位为0.
    volatile unsigned int FrameNumReady;   //当前操作可以处理的帧号
    volatile bool isDoing;
private:
    std::condition_variable condition;
    std::mutex m_mutex;

public:
    friend class AutoPrintTime;
    friend class AutoPrintTimeToNow;
    friend class AutoCallWhendoProcess;
};

//辅助打印耗时，自动打印funcModule里doProcess处理耗时，避免每次return都打印
class AutoPrintTime
{
//private:
//    S_DISABLE_COPY(AutoPrintTime)
private:
    AutoPrintTime(const AutoPrintTime& rhs);
    AutoPrintTime& operator=(const AutoPrintTime& rhs);
public:
    AutoPrintTime(funcModule* pFm):m_pFm(pFm)
    {
        m_pFm->getBeginTime();
    }
    ~AutoPrintTime()
    {
        m_pFm->getEndTimeAndPrint();
    }
private:
   funcModule* m_pFm;
};


class AutoPrintTimeToNow
{
private:
//    S_DISABLE_COPY(AutoPrintTimeToNow)

    AutoPrintTimeToNow(const AutoPrintTimeToNow& rhs);
    AutoPrintTimeToNow& operator=(const AutoPrintTimeToNow& rhs);
public:
    AutoPrintTimeToNow(funcModule* pFm,unsigned long long beginTime,const char* name):m_pFm(pFm),m_beginTime(beginTime),m_name(name)
    {
         m_pFm->printTimetoNow(funcModule::P2N_Begin,beginTime,name,"开始");
    }
    ~AutoPrintTimeToNow()
    {
         m_pFm->printTimetoNow(funcModule::P2N_End,m_beginTime,m_name,"完成");
    }
private:
    funcModule* m_pFm;
    unsigned long long m_beginTime;
    const char* m_name;
};


class AutoCallWhendoProcess
{
private:
//    S_DISABLE_COPY(AutoCallWhendoProcess)

    AutoCallWhendoProcess(const AutoCallWhendoProcess& rhs);
    AutoCallWhendoProcess& operator=(const AutoCallWhendoProcess& rhs);
public:
    AutoCallWhendoProcess(funcModule* pFm):m_pFm(pFm)
    {
        m_pFm->CallAtProcessBegin();
    }
    ~AutoCallWhendoProcess()
    {
        m_pFm->CallAtProcessEnd();
    }
private:
    funcModule* m_pFm;
};

//批处理辅助类
#include <vector>
class FunModuleHelper
{

public:
    FunModuleHelper()
    {

    }
    void push_back(funcModule* pFun)
    {
        VfuncModule.push_back(pFun);
    }
    void clear()
    {
        VfuncModule.clear();
    }
    void ForeachInitState()
    {
        for (size_t n=0;n<VfuncModule.size();n++)
        {
            VfuncModule.at(n)->InitState();
        }
    }
    void ForeachStart()
    {
        for (size_t n=0;n<VfuncModule.size();n++)
        {
            funcModule* pfun=VfuncModule.at(n);
            pfun->start();
            pfun->setPriority(Thread::High);   //为了保证音频处理的实时性能，设置线程优先级为高  //无差别


            //为了实时性能,重要处理模式把线程与CPU核心绑定,探索中
            //unsigned int CpuCoreIndex=0;
            //VfuncModule.at(n)->SetThreadAffinityNum(n);   //add 20190103
        }
    }


    void ForeachWaitFinish()
    {
        for (size_t n=0;n<VfuncModule.size();n++)
        {
            VfuncModule.at(n)->waitFinish();
        }
    }
    void ForeachCleanState()
    {
        for (size_t n=0;n<VfuncModule.size();n++)
        {
            VfuncModule.at(n)->ClearState();
        }
    }
private:
    std::vector<funcModule*> VfuncModule;

private:
    //S_DISABLE_COPY(FunModuleHelper)
    FunModuleHelper(const FunModuleHelper& rhs);
    FunModuleHelper& operator=(const FunModuleHelper& rhs);
};



#endif  // FUNCMODULE_H_


