#include "SystemCtrl.h"
#include "ThreadSync.h"


#ifdef OS_UNIX
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>   
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <string>

#ifndef PATH_MAX
#define PATH_MAX 1024   // 默认最大路径长度
#endif

#endif

NAME_SPACE_MAIN_BEGIN

class SingleApplicationPrivate
{
public:
    SingleApplicationPrivate(const char* name="")
        :m_mtxProcess(name)
    {
        isExist=m_mtxProcess.isExist();
#ifdef OS_UNIX
        if(checkSingleInstance(name))
        {
            isExist= false;
        }
        else
        {
            isExist= true;
        }
#endif
    }
    MutexProcess m_mtxProcess;   //基于跨进程的命名互斥锁实现


#ifdef OS_UNIX
    static std::string currentExeName()
    {    
        //const int PATH_MAX=1024;
        char buf[PATH_MAX] = {'\0'};
        int ret = readlink("/proc/self/exe",buf,PATH_MAX);
        if (ret<0||ret>=PATH_MAX)
        {
            return "";
        }

        std::string path(buf);
        int pos = path.find_last_of("/");
        if (pos==-1)
        {
            return "";
        }
        path = path.substr(pos+1, path.size()-1);
        return path;
    }

    //unix系列系统下基于文件锁可能更可靠 to test
    static bool checkSingleInstance(const char* name)
    {
        // 获取当前可执行文件名
        //std::string processName = currentExeName();
        //if (processName.empty())
        //{
        //    exit(1);
        //}
        std::string processName=name;   //文件名格式合法性检查，有空待续 TODO:

        // 打开或创建一个文件
        std::string filePath = std::string("./")+processName+".mutexAccess.pid";
        int fd=open(filePath.c_str(),O_RDWR|O_CREAT,0666);
        if (fd<0)
        {
            printf("Open file failed, error : %s", strerror(errno));
            //exit(1);
            return false;
        }

        // 将该文件锁定
        // 锁定后的文件将不能够再次锁定
        struct flock fl;
        fl.l_type = F_WRLCK; // 写文件锁定
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        int ret = fcntl(fd, F_SETLK, &fl);
        if (ret < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                printf("%s already locked, error: %s\n", filePath.c_str(), strerror(errno));
                close(fd);
                return false;
            }
        }

        // 锁定文件后，将该进程的pid写入文件
        char buf[16] = {'\0'};
        sprintf(buf, "%d", getpid());
        ftruncate(fd, 0);
        ret = write(fd, buf, strlen(buf));
        if (ret < 0)
        {
            printf("Write file failed, file: %s, error: %s\n", filePath.c_str(), strerror(errno));
            close(fd);
            //exit(1);
            return false;
        }

        // 函数返回时不需要调用close(fd)
        // 不然文件锁将失效
        // 程序退出后kernel会自动close
        return true;
    }
#endif
    bool isExist;
};


SingleApplication::SingleApplication(const char* name) :
m_mtxProcess(std::string(std::string(name)+"_SAmutexProcess").c_str()),
m_ShareMem(sizeof(SingleApplicationShareMem),std::string(std::string(name)+"_SAshareMem").c_str())
{
    pShare=(SingleApplicationShareMem*)m_ShareMem.getAddress();
    LockerGuard<MutexProcess> lock(m_mtxProcess);
    pShare->ProcessNum++;
    PreDprintf("Process Num:%d\n",pShare->ProcessNum);

    //方式2：linux预留实现
    pImpl = new SingleApplicationPrivate(name);
}

SingleApplication::~SingleApplication()
{
    LockerGuard<MutexProcess> lock(m_mtxProcess);
    pShare->ProcessNum--;
    PreDprintf("Process Num:%d\n",pShare->ProcessNum);

        if (NULL!=pImpl)
        {
            delete pImpl;
            pImpl=NULL;
        }
}

bool SingleApplication::isRuning() const
{
//#ifdef OS_UNIX
    return pImpl->isExist;
//#else
//    return pShare->ProcessNum>1;    //存在其他进程
//#endif
}

NAME_SPACE_MAIN_END



/*
//共享内存法
//共享变量法
//四，互斥量法
//共享内存的方法,问题多个程序共用一个动态库？

//如果使用OpenMutex等方法，当程序非法终止后，你将不能够再运行一个新的进程，要因为在程序非法终止时，互斥区或信号量均不会自动释放。
安全风险：
    任何进程--即使是最低权限的进程--都能用任何指定的名称来创建一个对象，所以攻击者很容易写一个应用程序来创建一个同名的内核对象。如果它先于单实例应用程序启动，上面“单实例”的应用程序就变成一个“无实例”的应用程序--始终都是已启动就退出。这种攻击就是拒绝服务(DoS)的一种。

    结果：
    对于方法一和方法二（文件法和注册表法），由于有I/O操作，如果恢复设置的代码不被调用，除非手动将设置恢复，否则程序就永远不能再被启动。这也是我不推荐这两种方法的主要原因。
    对于后3种方法（文件映射法，互斥量法和共享变量法），就算进程被异常终止，恢复设置的代码不被调用，也不会影响程序的再次启动。我觉得，这是因为这3种使用的对象（文件映射，互斥量，共享变量）虽然可以夸进程访问，但是他们还是属于创建者进程的，会随着创建者的销毁而销毁，Windows会在进程结束（无论是正常还是意外）的时候释放它们。
    另外，我在MSDN对CreateMutex的介绍中发现了下面这句话：
    The system closes the handle automatically when the process terminates. The mutex object is destroyed when its last handle has been closed.
    可以说这是明确的告诉我们互斥量在程序终止的时候会被自动释放。
    所以，在我介绍的这些让进程只启动一个实例的五种方法中，互斥量法是最好的解决方法。
*/

