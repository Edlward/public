#ifndef DcapThreadParallel_H
#define DcapThreadParallel_H
#include "Super/Basic/Thread.h"    //跨平台线程

#include<vector>

class cardInput;
class funcModule;
class SignelGenerator;
class SoundPlay;
class PlayBack;
class AES16Split;
//class StreamRecord;
//class StreamPlayBack;
class Renderer;
class RendererObj;
class RendererBed;
class BassMgr;
class Equalizer;
class EqualizerPartA;
class EqualizerPartB;

//新的并行加速框架:注意理解使用 by lizulin  --待补充：
//用并行线程加条件等待实现串联的功能。
//相对旧的串行级联框架，线程利用率更高，只要存在未完成任务，线程就不会停止
//旧框架若前级任务未处理完，而后级处理完了，后级线程只能等待。
//音频帧处理环路中有些流程必须是顺序的，如均衡等是历史相关的，处理完成之后的推送也是必须是顺序的。

#include "funcModule.h"


//不继承funcModule,概念容易混淆
class DcapThreadParallel:public RingBufferMgr,public Thread
{
protected:
    void *inputBuffer;                      // 输入缓冲区指针
    void *outputBuffer;                   // 输出缓冲区指针
    volatile int& WorkStatus;          // 播放状态  STATUS_PLAY STATUS_PAUSE STATUS_STOP 
    int WorkMode;                         // 当前播放模式
public:
    void doPushWaitCondition(cardInput*& pCard);
    volatile unsigned int FrameNumCountPush;   //若作为音频帧生成级，每次使用完并推送成功+1，每次模式切换复位为0.
    volatile unsigned int FrameNumReadyPush;   //当前操作可以处理的帧号
    volatile bool isDoingPush;
private:
    std::condition_variable conditionPush;
    std::mutex m_mutexPush;
public:
    DcapThreadParallel(volatile int& SysStatus);
    ~DcapThreadParallel(void)
    {
    }
    void doInitState();
    void doCleanState();
    virtual void run();

    //获取、处理、推送过程再封装，处理,单线程时的特化
    bool do_GetAudioCard(cardInput*& pCard);
    static void do_Process(funcModule* pModule,cardInput*& pCard);
    void do_PushAudioCard(cardInput*& pCard);
    void do_Work();


    void doWorkAsEdle();
    void doWorkAsEq();
    void doWorkAsMonitor();
    //void doWorkAsRecord();
    void doWorkAsPlayBack();

    int setStatus(const int newStatus);
    int getStatus()const {return WorkStatus;}                     // 读取播放状态

    void setWorkMode(const int newMode);                        // 设置模式
    int getWorkMode()const {return WorkMode;}

    std::vector<funcModule*> VfuncModuleCurrMode;
    //FunModuleHelper VfuncModuleCurrMode;

    SoundPlay* pSoundPlay;
    SignelGenerator* pSignelGenerator; 
    AES16Split* pAes16Split;
    PlayBack* pPlayBack;
    Renderer*  pRenderer;
    RendererObj*  pRendererObj;
    RendererBed*  pRendererBed;
    BassMgr* pBassMgr;
    Equalizer* pEqualizer;
    EqualizerPartA* pEqualizerPartA;
    EqualizerPartB* pEqualizerPartB;
};

#endif

