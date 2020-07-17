#ifndef ProcessGuard_H
#define ProcessGuard_H

#include "global.h"

//进程监视，崩溃，自动重启，为了简化系统程序数量不用脚本，把监视功能放到升级监视程序中去

#include <string>
#include "Thread.h"


//设计思路:不用脚本实现进程监控重启
//监控进程与工作进程共享一个数据结构，工作进程首次打开时会注册自己的程序路径到共享结构。
//工作进程后台始终运行一个线程，不断刷新自己最后活着的时间
//监控进程不断循环遍历各个进程最后活动时间，超出一定时间则杀进程并重启。


 NAME_SPACE_MAIN_BEGIN

enum ProcessGuardWorkMode
{
    UsedAs_Watcher,    //监控进程
    UsedAs_Worker,      //工作进程
};

struct PerWorker
{   
   volatile bool isUsing;                                                 //有效标记
   volatile uint64_t lastActiveTime;                                //工作进程最后活着的时间
   volatile unsigned int workerAutoUpdateTime;            //工作进程自动喂狗时间间隔
   volatile unsigned int slowTimeMax;                           //最大允许的迟钝时间,当前时间-最后活着时间>此值,重启进程
   char AppPath[1024];                                                  //工作进程本身可执行程序路径路径
   void init();
   void print()const;
};


enum 
{
    MaxWorkerToBeWatcher=10,
};

struct ShareWatcherWorker
{   
    volatile unsigned int initKey;                                //是否初始化标记
    volatile unsigned int WorkerIndexMax;
    volatile bool hasWatcherRuning;                         //有看守进程在运行,防止多个看守者同时运行
    volatile bool bWorkerChanged;                           //被监控工作者发生变化,
    volatile bool bWatcherContinueRun;                   //Worker设置此标记，令Watcher退出
    PerWorker mWorker[MaxWorkerToBeWatcher];  //最大支持监控的工作进程数量
    void init()
    {
        WorkerIndexMax=0;
        hasWatcherRuning=false;
        bWorkerChanged=false;
        bWatcherContinueRun=true;
        for (unsigned int n=0;n<MaxWorkerToBeWatcher;n++)
        {
            mWorker[n].init();
        }
    }
   void print()const;
};


class ShareMemory;
class ProcessGuard:public Thread
{
public:
    ProcessGuard(const char* keyName,ProcessGuardWorkMode wMode);
        //:pShareMem(sizeof(ShareWatcherWorker),keyName)
    ~ProcessGuard();
    virtual void run();

    //作为Watcher,调用
    void setWatcherCheckPeriod(unsigned int ms){WatcherCheckPeriod=ms;}


    //作为Worker,调用
    void setWorkerUpdatePeriod(unsigned int ms);
    bool setSlowTimeMax(unsigned int ms);   //设置最大容忍迟钝时间
    bool doFeedingDog();  //主动喂狗,其实就是刷新最后活着的时间

    void Register(){return addWorker();}   //工作者启动后手动调用
    void UnRegister(){removeWorker();}    //工作者正常退出调用此函数注销

    //worker设置Watcher标记,令其退出
    void DisEnableWatcher();
private:
    void do_UsedAsWatcher();
    void do_UsedAsWorker();
    void addWorker();
    void removeWorker();
private:
    std::string sKey;
    ProcessGuardWorkMode wMode;
    unsigned int currWorkerIndex;              //本工作进程是第几个  >=MaxWorkerToBeWatcher是无效的
    PerWorker* pCurrPerWorker;

    unsigned int WatcherCheckPeriod;       //作为Watcher每次检查的间隔单位ms,默认3000
    unsigned int workerAutoUpdateTime;  //作为Worker,自动喂狗间隔
private:
    ShareMemory* pShareMem;
    ShareWatcherWorker* pShare;
};

//ProcessGuard* GetProcessGuardWorker();



//使用方法
//作为监控者
//ProcessGuard proc("HoloSound_Dcap_System",UsedAs_Watcher);
//proc.start();

//作为使用者
//ProcessGuard proc("HoloSound_Dcap_System",UsedAs_Worke);
//proc.start();






////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////














NAME_SPACE_MAIN_END

#endif








