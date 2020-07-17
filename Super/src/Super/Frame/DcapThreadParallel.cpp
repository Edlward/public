#include "DcapThreadParallel.h"
#include "Super/Basic/SuperTime.h"
#include "Super/Basic/ringbuffer.h"

#include <stdio.h>

DcapThreadParallel::DcapThreadParallel(volatile int& SysStatus):WorkStatus(SysStatus)
    ,RingBufferMgr(SysStatus)
{
    isDoingPush=false;
}

void DcapThreadParallel::doPushWaitCondition(cardInput*& pCard)
{
    std::unique_lock<std::mutex> lock(m_mutexPush);
    while (isDoingPush&&(FrameNumReadyPush!=pCard->CurrFrameNum))
    {
        conditionPush.wait(lock);
    }
    isDoingPush=true;
    pushBlockMayMultiple(pCard);
    FrameNumReadyPush++;
    isDoingPush=false;
    conditionPush.notify_all();   //���������ڵȴ���
}

void DcapThreadParallel::doInitState()
{
    for (size_t n=0;n<VfuncModuleCurrMode.size();n++)
    {
        funcModule* pfunModule=VfuncModuleCurrMode.at(n);
        pfunModule->InitState();
        pfunModule->FrameNumCount=0;   //��λ��֡�����ţ�����Ҫ
        pfunModule->FrameNumReady=0;
    }
    this->FrameNumReadyPush=0;
    this->FrameNumCountPush=0;
}

void DcapThreadParallel::doCleanState()
{
    for (size_t n=0;n<VfuncModuleCurrMode.size();n++)
    {
        funcModule* pfunModule=VfuncModuleCurrMode.at(n);
        pfunModule->ClearState();
    }
}


void DcapThreadParallel::run()
{
    //����˳��������
    return do_Work();
}


bool DcapThreadParallel::do_GetAudioCard(cardInput*& pCard)
{
    //if (!getBlock(pCard)) continue;   
    //if (!getBlockMayMultiple(pCard)) continue; //���мܹ����ܴ��ڶ��߳�ͬʱ����һ������ ���ڲ�������Multiple��
#if defined(OnlyOneWorkThread)
    return getBlock(pCard);
#else
    return getBlockMayMultiple(pCard);
#endif
}


void DcapThreadParallel::do_Process(funcModule* pModule,cardInput*& pCard)
{
#if defined(OnlyOneWorkThread)
    pModule->doProcess(pCard);
#else
    pModule->doProcessWaitCondition(pCard);
#endif
}


void DcapThreadParallel::do_PushAudioCard(cardInput*& pCard)
{
    //pushBlock(pCard);   //ͬһʱ��ֻ��һ���߳����е���������߳���ʱ�����ȣ�����Ҫ��Multiple��
    //pushBlockByMultiple(pCard); 
    //doPushWaitCondition(pCard);//�������ȴ������ٷ�֮���ϸ�˳��

#if defined(OnlyOneWorkThread)
    pushBlock(pCard);
    return;
#else
    return doPushWaitCondition(pCard);
#endif
}


void DcapThreadParallel::do_Work()
{
    while(WorkStatus == STATUS_PLAY)
    {
        cardInput *pCard =NULL;
        if (!do_GetAudioCard(pCard)) continue; 
        for (int n=0;n<VfuncModuleCurrMode.size();n++)
        {
            do_Process(VfuncModuleCurrMode[n],pCard);
        }
        do_PushAudioCard(pCard);
    }
}


void DcapThreadParallel::doWorkAsEdle()
{
    //printf("Mode IDLE--Begin\n");   //
    //GetPlayCtrl()->setStatus(STATUS_PLAY);             //ģʽ��ʼʱ��λ״̬
    //GetPlayCtrl()->printModeStatus();
    //int NextState;
    //bool ret=GetPlayCtrl()->getNextMode(&NextState,true);   //�����ȴ�ȡ��һ��״̬  
    //GetPlayCtrl()->setSysModeWait(NextState);
    //while (GetPlayCtrl()->getSysMode()==MODE_IDLE)  //�����ȴ�ֱ��ģʽ�л�
    //{
    //	Thread::sleep_for(1);
    //}
    ////printModeStatus();            
    //printf("Mode IDLE--End\n");   //
}


void DcapThreadParallel::doWorkAsMonitor()
{
    while(WorkStatus == STATUS_PLAY)
    {
        cardInput *pCard =NULL;
        //if (!getBlock(pCard)) continue;   
        if (!getBlockMayMultiple(pCard)) continue; //���мܹ����ܴ��ڶ��߳�ͬʱ����һ������ ���ڲ�������Multiple��
//
//        pAes16Split->doProcessWaitCondition(pCard);
//#if defined(RENDER_SEPARATE)
//        pRendererObj->doProcessWaitCondition(pCard);
//        pRendererBed->doProcessWaitCondition(pCard);
//#else
//        pRenderer->doProcessWaitCondition(pCard);
//#endif
//        pBassMgr->doProcessWaitCondition(pCard);
//
//#if defined(RENDER_SEPARATE)
//        pEqualizerPartA->doProcessWaitCondition(pCard);
//        pEqualizerPartB->doProcessWaitCondition(pCard);
//#else
//        pEqualizer->doProcessWaitCondition(pCard);
//#endif

        //pushBlock(pCard);   //ͬһʱ��ֻ��һ���߳����е���������߳���ʱ�����ȣ�����Ҫ��Multiple��
        //pushBlockByMultiple(pCard); 
        doPushWaitCondition(pCard);//�������ȴ������ٷ�֮���ϸ�˳��
    }
}
