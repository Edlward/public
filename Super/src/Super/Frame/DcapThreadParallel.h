#ifndef DcapThreadParallel_H
#define DcapThreadParallel_H
#include "Super/Basic/Thread.h"    //��ƽ̨�߳�

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

//�µĲ��м��ٿ��:ע�����ʹ�� by lizulin  --�����䣺
//�ò����̼߳������ȴ�ʵ�ִ����Ĺ��ܡ�
//��ԾɵĴ��м�����ܣ��߳������ʸ��ߣ�ֻҪ����δ��������߳̾Ͳ���ֹͣ
//�ɿ����ǰ������δ�����꣬���󼶴������ˣ����߳�ֻ�ܵȴ���
//��Ƶ֡����·����Щ���̱�����˳��ģ�����������ʷ��صģ��������֮�������Ҳ�Ǳ�����˳��ġ�

#include "funcModule.h"


//���̳�funcModule,�������׻���
class DcapThreadParallel:public RingBufferMgr,public Thread
{
protected:
    void *inputBuffer;                      // ���뻺����ָ��
    void *outputBuffer;                   // ���������ָ��
    volatile int& WorkStatus;          // ����״̬  STATUS_PLAY STATUS_PAUSE STATUS_STOP 
    int WorkMode;                         // ��ǰ����ģʽ
public:
    void doPushWaitCondition(cardInput*& pCard);
    volatile unsigned int FrameNumCountPush;   //����Ϊ��Ƶ֡���ɼ���ÿ��ʹ���겢���ͳɹ�+1��ÿ��ģʽ�л���λΪ0.
    volatile unsigned int FrameNumReadyPush;   //��ǰ�������Դ����֡��
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

    //��ȡ���������͹����ٷ�װ������,���߳�ʱ���ػ�
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
    int getStatus()const {return WorkStatus;}                     // ��ȡ����״̬

    void setWorkMode(const int newMode);                        // ����ģʽ
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

