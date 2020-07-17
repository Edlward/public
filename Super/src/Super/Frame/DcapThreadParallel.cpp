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
    conditionPush.notify_all();   //唤醒所有在等待的
}

void DcapThreadParallel::doInitState()
{
    for (size_t n=0;n<VfuncModuleCurrMode.size();n++)
    {
        funcModule* pfunModule=VfuncModuleCurrMode.at(n);
        pfunModule->InitState();
        pfunModule->FrameNumCount=0;   //复位首帧计数号，很重要
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
    //按照顺序批处理
    return do_Work();
}


bool DcapThreadParallel::do_GetAudioCard(cardInput*& pCard)
{
    //if (!getBlock(pCard)) continue;   
    //if (!getBlockMayMultiple(pCard)) continue; //并行架构可能存在多线程同时操作一个队列 用内部带锁的Multiple版
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
    //pushBlock(pCard);   //同一时刻只有一个线程运行到这里？考虑线程随时被调度，还是要用Multiple版
    //pushBlockByMultiple(pCard); 
    //doPushWaitCondition(pCard);//用条件等待决定百分之百严格顺序

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
    //GetPlayCtrl()->setStatus(STATUS_PLAY);             //模式开始时复位状态
    //GetPlayCtrl()->printModeStatus();
    //int NextState;
    //bool ret=GetPlayCtrl()->getNextMode(&NextState,true);   //阻塞等待取下一个状态  
    //GetPlayCtrl()->setSysModeWait(NextState);
    //while (GetPlayCtrl()->getSysMode()==MODE_IDLE)  //阻塞等待直到模式切换
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
        if (!getBlockMayMultiple(pCard)) continue; //并行架构可能存在多线程同时操作一个队列 用内部带锁的Multiple版
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

        //pushBlock(pCard);   //同一时刻只有一个线程运行到这里？考虑线程随时被调度，还是要用Multiple版
        //pushBlockByMultiple(pCard); 
        doPushWaitCondition(pCard);//用条件等待决定百分之百严格顺序
    }
}
