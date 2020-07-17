#ifndef SYSTEMCTRL_H
#define SYSTEMCTRL_H

#include "global.h"
#include "ThreadSync.h"
#include "ShareMemory.h"


NAME_SPACE_MAIN_BEGIN

//跨平台  程序只运行一个实例的方法
//仅基于命名互斥锁在windows下没有问题，linux下进程异常结束可能不会自动释放所持有的互斥锁、读写锁和Posix信号量
//改进基于命名互斥锁+共享内存方式， 

//linux下或基于文件锁，需要创建文件，有空待续


struct  SingleApplicationShareMem
{
    int ProcessNum;   //统计进程数量
};

class SingleApplicationPrivate;
class BASIC_API SingleApplication
{
public:
    SingleApplication(const char* name);
    ~SingleApplication();
    bool isRuning()const;
    void printInfo()
    {
    }
private:
   MutexProcess m_mtxProcess;    //跨进程的命名互斥锁实现
   ShareMemory m_ShareMem;     //跨进程的共享内存,创建者会将共享内存清零
   SingleApplicationShareMem* pShare;
   SingleApplicationPrivate* pImpl;
};

NAME_SPACE_MAIN_END

#endif
