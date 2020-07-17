#include "ProcessGuard.h"

#include "ShareMemory.h"
#include "BasicOperation.h"

#include <time.h>


#include "SuperTime.h"

//static uint64_t getStartupToNowInms()
//{
//    return SuperTime::getStartupToNowInms();
//}


void PerWorker::init()
{
    isUsing=false;
    lastActiveTime=getStartupToNowInms()+30*1000;    //首次初始化为比当前时间更晚的值,避免刚启动喂狗线程可能没有来得及运行的情况,待确认
    workerAutoUpdateTime=100;
    slowTimeMax=10*1000;
    memset(AppPath,0,sizeof(AppPath));
}

void PerWorker::print() const
{
    ::printf("App Path:%s\n",AppPath);
    ::printf("isUsing:%d lastActiveTime:%llu workerAutoUpdateTime:%u slowTimeMax:%u\n"
        ,isUsing,lastActiveTime,workerAutoUpdateTime,slowTimeMax);
}


void ShareWatcherWorker::print() const
{
    ::printf("CurrWorker Info: WorkerIndexMax:%u WatcherRuning:%d\n",WorkerIndexMax,hasWatcherRuning);

    for (unsigned int n= 0; n<WorkerIndexMax; n++)
    {
        ::printf("WorkerIndex[%u] ",n);
        mWorker[n].print();
    }
}


NAME_SPACE_MAIN_BEGIN

ProcessGuard::ProcessGuard(const char* keyName,ProcessGuardWorkMode nwMode) //:pShareMem(sizeof(ShareWatcherWorker),keyName)
:wMode(nwMode)
{
    //this->wMode=wMode;
    sKey=keyName;
    pShareMem=new ShareMemory(sizeof(ShareWatcherWorker),keyName);
    pShare=(ShareWatcherWorker*)(pShareMem->getAddress());
   
    //暂不加锁
    if (pShare->initKey!=0X08050512)   //是否初始化标记,共享内存首个使用者会清零
    {
        pShare->init();
        pShare->initKey=0X08050512;
    }
 
    //pShare->WorkerIndexMax=0;
    currWorkerIndex=MaxWorkerToBeWatcher;   //无效值
    pCurrPerWorker=NULL;
    WatcherCheckPeriod=50;

    if(this->wMode==UsedAs_Worker)
    {
        PreDprintf("Work as Worker\n");
        //addWorker();
    }
    else
    {
        PreDprintf("Work as Watcher\n");
        pShare->bWatcherContinueRun=true;
    }

    this->setWorkerUpdatePeriod(100);
    this->setSlowTimeMax(20000);             //设置构造时设置为一个比较大的数,等实际避免程序启动一些初始化操作的影响,实际运行前再手动设置
}


ProcessGuard::~ProcessGuard()
{

    if(this->wMode==UsedAs_Worker)   //正常退出是否需要析构？
    {
        removeWorker();
    }

    delete pShareMem;
}

void ProcessGuard::setWorkerUpdatePeriod(unsigned int ms)
{
    PreDprintf("WorkerUpdatePeriod:%u ms\n",ms);

    //if (ProcessorNum<0||ProcessorNum>=MaxWorkerToBeWatcher)
    //{
    //    return;
    //}
    //pShare->mWorker[ProcessorNum].workerAutoUpdateTime=ms;
    workerAutoUpdateTime=ms;
}



bool ProcessGuard::setSlowTimeMax(unsigned int ms) //设置最大容忍迟钝时间
{
    PreDprintf(":%u \n",ms);

    PerWorker* pCurr=pCurrPerWorker;   //先取出来在访问,防止运行过程被注销置空
    if (pCurr==NULL)   //||ProcessorNum>=MaxWorkerToBeWatcher
    {
        ::printf("failed\n");
        return false;
    }
    pCurr->slowTimeMax=ms;
    return true;
}

void ProcessGuard::DisEnableWatcher()
{
    pShare->bWatcherContinueRun=false;
}




void ProcessGuard::run()
{
    PRINT_FUN_TRACE

    switch (wMode)
    {
    case UsedAs_Watcher:
        do_UsedAsWatcher();
        break;
    case UsedAs_Worker:
        do_UsedAsWorker();
        break;
    default:
        break;
    }
}



void ProcessGuard::do_UsedAsWatcher()   //作为监控看守进程
{
    PRINT_FUN_TRACE

    //防止有多个看守线程同时运行,一般也不会，只能运行一个
    //while (getContinueRun()&&pShare->hasWatcherRuning)
    //{
    //    Thread::sleep_for(100);
    //}

    int maxMemUsedPercent=0;
    int lastMemUsedPercent=0;
    SystemMemoryInfo memInfo;

    pShare->hasWatcherRuning=true;
    while (getContinueRun()&&pShare->bWatcherContinueRun)
    {
            uint64_t CurrTime=getStartupToNowInms();   //
            //printf("ProcessGuard::do_UsedAsWatcher() WorkNum:%d Time:%lld\n",pShare->WorkerIndexMax,CurrTime); //单位为miao
       
            getSysMemoryInfo(memInfo);
            int currMemusedPercent=memInfo.MemoryLoad;
            if (maxMemUsedPercent>currMemusedPercent)
            {
                PreDprintf("MaxUsePercent:%d\n",maxMemUsedPercent);
                  fflush(stdout);  //及时刷到打印到文件,已经被重定向
                maxMemUsedPercent=currMemusedPercent;
            }

           int changeRate=::abs(currMemusedPercent- lastMemUsedPercent);
           if (changeRate>10)
           {
               showSystemMemoryInfo();
                fflush(stdout);  //及时刷到打印到文件,已经被重定向
           }
          // printf("lastMemUsedPercent:%d currMemusedPercent:%d\n",lastMemUsedPercent,currMemusedPercent);
           lastMemUsedPercent=currMemusedPercent;


            for (unsigned int N=0;N<=pShare->WorkerIndexMax;N++)
            {
                PerWorker& worker=pShare->mWorker[N];
                if (!worker.isUsing)
                {
                    continue;
                }
                //CurrTime=getStartupToNowInms();   //update currTime？

                if ((worker.lastActiveTime+worker.slowTimeMax)<CurrTime)   //注意被减数,比减法更安全的写法
                {
                    bool doConfirm=false;

                    //为了由于基础类型线程安全的问题数据不完整,防止误操作,多判断几次
                    unsigned int checkCnt=0;
                    while (true)
                    {
                        CurrTime=getStartupToNowInms();
                        uint64_t lastActiveTime=worker.lastActiveTime;
                        unsigned int slowTimeMax=worker.slowTimeMax;
                        //再容差1s,防止slowTimeMax意外为0？
                        if ((lastActiveTime+slowTimeMax+1000)<CurrTime)   //注意被减数,比减法更安全的写法
                        {
                            checkCnt++;
                            if (checkCnt>200)
                            {
                                doConfirm=true;
                                break;
                            }
                        }
                        else
                        {
                            checkCnt=0;
                            doConfirm=false;
                            break;
                        }

                        Thread::sleep_for(1);
                    }

                    //确认要重启
                    if (doConfirm==true)
                    {
                        showCurrProcessMemoryInfo();
                        const char* pCurrAppPath=worker.AppPath;
                        ::printf("KillProcessByPath:%s\n",pCurrAppPath);
                        ::printf("CurrTime:%llu LastActiveTime:%llu slowTimeMax:%u\n",CurrTime,worker.lastActiveTime,worker.slowTimeMax);

                        if(!KillProcessByPath(pCurrAppPath))//杀死进程，根据程序路径。
                        {
                            ::printf("KillProcess Failed! And SystemRestart()\n");

                            fflush(stdout);  //及时刷到打印到文件,已经被重定向

                            SystemRestart();     //杀崩溃的进程失败只能重启？,待续
                            continue;
                        }
                        ::printf("KillProcess Success!\n");
                        ::printf("CreatProcessByPath:%s\n",pCurrAppPath);
                        fflush(stdout);  //及时刷到打印到文件,已经被重定向

                        Thread::sleep_for(2000);
                        if(!CreatProcessByPath(pCurrAppPath))
                        {
                            ::printf("CreatProcess Failed! And SystemRestart()\n");
                            fflush(stdout);  //及时刷到打印到文件,已经被重定向
                            SystemRestart();     //失败只能重启？,待续
                            continue;  //？
                        }
                        ::printf("CreatProcess Success!\n");
                        fflush(stdout);  //及时刷到打印到文件,已经被重定向
                    }
                }


                CurrTime=getStartupToNowInms();   //update currTime？
            }

            //Worker发生变化,尽量在Watcher里面打印
            if (pShare->bWorkerChanged==true)
            {
                ::printf("WorkerChanged:\n");
                pShare->print();
                fflush(stdout);  //及时刷到打印到文件,已经被重定向

                pShare->bWorkerChanged=false;
            }

            Thread::sleep_for(WatcherCheckPeriod);  //3000ms

            
    }
    pShare->hasWatcherRuning=false;
}


void ProcessGuard::do_UsedAsWorker()  //作为工作进程
{
     PRINT_FUN_TRACE

    if (currWorkerIndex>=MaxWorkerToBeWatcher)
    {
        PRINT_FUN_FAILED
        return;
    }

    unsigned int num=0;
    //PerWorker& currPerWorker=pShare->mWorker[currWorkerIndex];

    while (getContinueRun())
    {
          //currPerWorker.lastActiveTime=getStartupToNowInms();
          doFeedingDog();

          //printf("ProcessGuard::do_UsedAsWorker() lastActiveTime:%lld\n",lastActiveTime);  //单位为miao
          Thread::sleep_for(workerAutoUpdateTime);

          //if (num++==100)
          //{
          //    int p;
          //    memset(&p,0,10*1000*1024);  //故意使程序崩溃测试
          //    int a=0;
          //    printf("",100/a);
          //}
    };
}

bool ProcessGuard::doFeedingDog()
{
    //PerWorker* pCurr=pCurrPerWorker;   //先取出来在访问,防止运行过程被注销置空
    //if (pCurr==NULL)   //||ProcessorNum>=MaxWorkerToBeWatcher
    //{
    //    return;
    //}
    //pCurr->lastActiveTime=getStartupToNowInms();

    //更安全？
    unsigned int tmpCurrWorkerIndex=currWorkerIndex;
    if (tmpCurrWorkerIndex>=MaxWorkerToBeWatcher)
    {
        return false;
    }
    pShare->mWorker[tmpCurrWorkerIndex].lastActiveTime=getStartupToNowInms();
    return true;
}


void ProcessGuard::addWorker()
{
     PRINT_FUN_TRACE

    if(this->wMode!=UsedAs_Worker)
    {
        return;
    }

    std::string currAppAbsolutionPath=getCurrentAppPath();

    printf("ProcessGuard::addWorker() getCurrentAppPath:%s\n",currAppAbsolutionPath.c_str());
    bool isExist=false;
    for (unsigned int N=0;N<MaxWorkerToBeWatcher;N++)
    {
            PerWorker& currPerWorker=pShare->mWorker[N];
            if (currPerWorker.isUsing)      //当前索引正在被使用
            {
                if (strcmp(currAppAbsolutionPath.c_str(),currPerWorker.AppPath)==0)
                {
                    currWorkerIndex=N;
                    pCurrPerWorker=&currPerWorker;
                    pShare->bWorkerChanged=true;  
                    //isExist=true;
                    break;
                }
                continue;
            }
            else    //如果不存在则添加新的被监控进程，路径
            {
                strcpy(currPerWorker.AppPath,currAppAbsolutionPath.c_str());
                currPerWorker.isUsing=true;
                pCurrPerWorker=&currPerWorker;
                currWorkerIndex=N;
                if (N>=pShare->WorkerIndexMax)     //若在最后一个添加
                {
                    pShare->WorkerIndexMax=N;
                }
                pShare->bWorkerChanged=true;  
                break;
            }
    }

    //首次注册,刷新
    doFeedingDog();
}



void ProcessGuard::removeWorker()
{
    //PRINT_FUN_TRACE

    if(this->wMode!=UsedAs_Worker)
    {
        return;
    }
    //可能一个进程例化两个实例，实际管理同一个
    if (pCurrPerWorker==NULL||currWorkerIndex>=MaxWorkerToBeWatcher)
    {
        return;
    }

    std::string currAppAbsolutionPath=getCurrentAppPath();
    printf("ProcessGuard::removeWorker() getCurrentAppPath:%s\n",currAppAbsolutionPath.c_str());

    for (unsigned int N=0;N<MaxWorkerToBeWatcher;N++)
    {
        PerWorker& currPerWorker=pShare->mWorker[N];
        if (currPerWorker.isUsing)
        {
            if (strcmp(currAppAbsolutionPath.c_str(),currPerWorker.AppPath)==0)
            {
                //移除,标记为无效即可
                currPerWorker.isUsing=false;
                memset(currPerWorker.AppPath,0,sizeof(currPerWorker.AppPath));
                currWorkerIndex=MaxWorkerToBeWatcher;
                pCurrPerWorker=NULL;
                pShare->bWorkerChanged=true;  
                break;
            }
        }
    }
}


NAME_SPACE_MAIN_END




