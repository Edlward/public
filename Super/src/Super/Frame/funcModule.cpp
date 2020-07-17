#include "funcModule.h"
//#include "../../public/Proj/HoloSound/definition.h"
//#include "../basic/dataformat.h"


#include "Super/Basic/ringbuffer.h"
#include <stdio.h>


class AutoPrintRingHelper
{
public:
    AutoPrintRingHelper(SuperTime& tm,unsigned int& CallCnt,unsigned int& UsedCntPeak,unsigned int& Cnt,
        ringbuffer<cardInput *> *pRing)
        :tm(tm),CallCnt(CallCnt),UsedCntPeak(UsedCntPeak),Cnt(Cnt),pRing(pRing)
    {
        CallCnt++;
        tm.getBeginTime();
    }
    ~AutoPrintRingHelper()
    {
        tm.getEndPrint(512);
        if (UsedCntPeak<Cnt)
        {
            UsedCntPeak=Cnt;
        }
        if (CallCnt%512==0)
        {
            //printf("CallCnt:%u Get/Push Cnt:%u PeakMax:%u\n",CallCnt,Cnt,UsedCntPeak);
            printf("UsedCnt:%u Peak:%u at:%s RingState:",Cnt,UsedCntPeak,tm.getName());
            pRing->printStatus();
            UsedCntPeak=0;
        }
    }
private:
    SuperTime& tm;
    unsigned int& CallCnt;
    unsigned int& UsedCntPeak;
    unsigned int& Cnt;
    ringbuffer<cardInput *> *pRing;
};


bool RingBufferMgr::getBlock(cardInput*& pCard)
{
    unsigned int UsedCnt=0;
    //AutoPrintRingHelper autoPrintRing(tmGet,CallCntGet,GetUsedPeak,UsedCnt,inputBuffer);

    while (playStatus== STATUS_PLAY)
    {
        UsedCnt++;
        cardInput *in =NULL;
        bool retGet= ((ringbuffer<cardInput *> *)inputBuffer)->getOnlySingle(in,false);  // 取得输入
        //bool retGet= ((ringbuffer<cardInput *> *)inputBuffer)->getMayMultiple(in,false);  // 取得输入
        if (retGet)//   continue;  //in!=NULL
        {
            pCard=in;
            return true;
        }
    }
    pCard=NULL;
    return false;  //获取失败
}

bool RingBufferMgr::pushBlock(cardInput* pCard)
{
    unsigned int UsedCnt=0;
    //AutoPrintRingHelper autoPrintRing(tmPush,CallCntPush,PushUsedPeak,UsedCnt,outputBuffer);

    //((ringbuffer<cardInput*>*)outputBuffer)->push(in,true);    //5.1 7.1 模式原样输出 推送到render在做remap
    //out->setHasSoundFlag();
    while (playStatus== STATUS_PLAY)
    {
        UsedCnt++;
        bool retpush=((ringbuffer<cardInput*>*)outputBuffer)->pushOnlySingle(pCard,false);
        //bool retpush=((ringbuffer<cardInput*>*)outputBuffer)->pushMayMultiple(pCard,false);
        if (retpush) //break;
        {
            return true;
        }
    }
    return false;
}

bool RingBufferMgr::getBlockMayMultiple(cardInput*& pCard)
{
    while (playStatus == STATUS_PLAY)
    {
        cardInput *in =NULL;
        bool retGet= ((ringbuffer<cardInput *> *)inputBuffer)->getMayMultiple(in,false);  // 取得输入
        if (retGet)//   continue;  //in!=NULL
        {
            pCard=in;
            return true;
        }
    }
    pCard=NULL;
    return false;  //获取失败
}

bool RingBufferMgr::pushBlockMayMultiple(cardInput* pCard)
{
    //((ringbuffer<cardInput*>*)outputBuffer)->push(in,true);    //5.1 7.1 模式原样输出 推送到render在做remap
    //out->setHasSoundFlag();
    while (playStatus == STATUS_PLAY)
    {
        bool retpush=((ringbuffer<cardInput*>*)outputBuffer)->pushMayMultiple(pCard,false);
        if (retpush) //break;
        {
            return true;
        }
    }
    return false;
}

funcModule::funcModule(std::string sName):sName(sName)
    ,timeTest(std::string(sName+"()").c_str())   //加括号 时间打印时可以表示函数
    ,RingBufferMgr(status,sName)   //注意把本级状态status直接引用给其使用
{
    status = STATUS_STOP;
	isDoing=false;
}


void funcModule::run()
{
    while(getContinueRun())
    {
        //int status=getStatus();
        //switch (status)
        //{
        //case STATUS_PLAY:
        //	doLoop();
        //	break;
        //case STATUS_PAUSE:
        //	Thread::sleep_for(1);
        //	break;
        //case STATUS_STOP:
        //	break;
        //default:
        //	break;
        //}

        if(getStatus() == STATUS_PLAY)  // 播放状态
        {
            doLoop();
        }
        else if(getStatus() == STATUS_PAUSE)  // 暂停状态
        {
            Thread::sleep_for(1);//Sleep(1);
        }
        else if(getStatus() == STATUS_STOP)  // 停止状态
        {
            break;
        }
        else
        {
            printf("未知播放状态!\n");
        }
    }
}


void funcModule::setStatus(const int newStatus)  // 设置播放状态，返回错误代码
{
    //if(newStatus != STATUS_PLAY && newStatus != STATUS_PAUSE && newStatus != STATUS_STOP)
    if(newStatus<=STATUS_BEGIN||newStatus>=STATUS_END)
    {
        printf("播放状态设置错误,不能为%d!\n", newStatus);
        return;
    }
    status = newStatus;
    return;
}


int funcModule::WaitModeChangedtoFinish(int lastMode,int lastStatus)
{
    //while((mode==MODE_MONITOR)&&status == STATUS_PLAY) 
    while(mode==lastMode&&status ==lastStatus) 
    {
        Thread::sleep_for(1);
    }
    return 0;
}

void funcModule::printModeStatus()
{
    printf("funcModule 当前处于:Mode=%02d Status=%02d: ",mode,status);
    switch(status)
    {
    case  STATUS_PLAY:   printf("Play    播放状态");break;
    case  STATUS_PAUSE :printf("Pause 暂停状态");break;
    case  STATUS_STOP :  printf("Stop   停止状态");break;
    default:printf("Unknow 未知状态");break;
    }
    printf("\n");
}






void funcModule::getBeginTime()
{
	//性能调试打印，实际发行屏蔽此句
	timeTest.getBeginTime();
}

void funcModule::getEndTimeAndPrint()
{
	//性能调试打印，实际发行屏蔽此句
	timeTest.getEndPrint(PRINT_Freq);
}

void funcModule::printTimetoNow(PStepIndex Step,unsigned long long beginTime,const char* name,const char* name2)
{
#if  1
    HistoryCount& hisCnt=ArrayhisCnt[Step];

    unsigned long long currtime=SuperTime::getHighResolutionTicks();    
    unsigned long long Usetime=currtime-beginTime;//out->curtime;
    if (Usetime>hisCnt.peakUseTimeInTicks)
    {
        hisCnt.peakUseTimeInTicks=Usetime;
    }
    hisCnt.RecallCouter++;
    hisCnt.SumTimeInTicks+=Usetime;
    if (hisCnt.RecallCouter%PRINT_Freq==0)
    //if (hisCnt.RecallCouter%1==0)
    {

        //printf("currtime:%lld last:%lld usetime:%lld\n",currtime,out->curtime,usetime);
        double avrgtimems=SuperTime::highResolutionTicksToSeconds(hisCnt.SumTimeInTicks/hisCnt.RecallCouter)*1000;
        double usetimems=SuperTime::highResolutionTicksToSeconds(Usetime)*1000;
        double peakUsetimems=SuperTime::highResolutionTicksToSeconds(hisCnt.peakUseTimeInTicks)*1000;

        //double cpuUsage=getCurretProcessUsage();

        //double cpuUsage =cpuInfo.getProcessCpuUsage();

        //printf("到%s%s耗时: usetime:%lld clk;%f ms; Peak:%lld clk;%f ms; %u帧平均:%f ms\n",
        //    name,name2,Usetime,usetimems,peakUseTimeInTicks,peakUsetimems,RecallCouter,avrgtimems);


        //打印本身也耗时，远大于1帧时间再打印
        //if (peakUsetimems>50)
        {
        printf("到%s%s耗时: usetime:%f ms; Peak:%f ms; %u帧平均:%f ms\n",
            name,name2,usetimems,peakUsetimems,hisCnt.RecallCouter,avrgtimems);
        }

        //if (peakUsetimems>80)
        //{
        //    //printf("doProcess() May be TimeOut!\n");
        //    char buftmp[256];
        //    sprintf(buftmp,"doProcess() May be TimeOut! useTime:%fms at:%s\n",peakUsetimems,name);
        //    fprintf(stdout,buftmp);
        //    fprintf(stderr,buftmp);
        //}

        hisCnt.peakUseTimeInTicks=0;
    }
#endif
}




//void funcModule::pushSilent(cardInput* pCard)
//{
//    pCard->clearData();
//    //pCard->clearHasSoundFlag();
//    //((ringbuffer<cardInput*>*)outputBuffer)->push(out);
//    while (status == STATUS_PLAY)
//    {
//        bool retpush=((ringbuffer<cardInput*>*)outputBuffer)->push(pCard,false);
//        if (retpush) break;
//    }
//}


 



void funcModule::doProcess(cardInput*& pCard)
{

}

void funcModule::doProcessWaitCondition(cardInput*& pCard)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (isDoing&&(FrameNumReady!=pCard->CurrFrameNum))
    {
        condition.wait(lock);
    }
    isDoing=true;
    doProcess(pCard);
	FrameNumReady++;
    isDoing=false;
    condition.notify_all();   //唤醒所有在等待的
}
