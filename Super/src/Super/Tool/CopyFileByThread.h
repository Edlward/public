#ifndef  CopyFileByThread_H
#define  CopyFileByThread_H

#include "../Basic/Thread.h"
#include <string>
#include <vector>

//typedef  bool (*FilterFun)(const char* filePath, std::string&cplid);              //扫描文件夹时候的过滤条件
typedef  bool (*FilterFun)(const char* filePath);              //扫描文件夹时候的过滤条件
typedef  void (*callAtCopyingFun)(void* parameter);     //拷贝过程回调函数


//拷贝状态
enum ECopyStatus
{
	CopyStatus_IDLE=0,                 //空闲等待，拷贝完成或失败 后告知客户端后回到空闲状态
    CopyStatus_isCopying=1,         //正在拷贝
    CopyStatus_CopyFailed=2,       //拷贝失败,
    CopyStatus_CopyFinish=3,       //拷贝结束,
};


struct CopyHelp
{
    volatile ECopyStatus Status;
    volatile bool Continue;
    volatile bool* bContinue;   //外部控制拷贝过程可以随时退出
    volatile bool Pause;
    volatile bool* bPause;         //控制拷贝过程暂停相关

    //所有文件的拷贝进度
    volatile long long totalAllFileSize;
    volatile long long totalCopied;
    volatile int AllCopyPercent;
    volatile int lastAllCopyPercent;

    //当前文件拷贝进度
    volatile long long currFileSize;
    volatile long long currFileCopied;
    volatile int currFileCopyPercent;
    volatile int lastcurrFileCopyPercent;

    //正在拷贝文件名,不同线程读写,可能存在读写不同步问题，不用线程锁,特殊设计
    char sCurrCopyingFileName[1024];   //当前正在拷贝的文件名,为了效率不用std::string
    volatile int FileNameLen;  //文件名长度，读取时判断保证不越界
    volatile int FileNameID;    //用某种校验值来唯一标识文件名ID,一般ID变化文件名才变化,用在使用处判断变化才刷新

    callAtCopyingFun pFunCallAtCopying;
    void* parameterofCopyingFun;
    enum 
    {
        BUFFER_SIZE=20*1024*1024
    };
    //char pBufIO[BUFFER_SIZE];   //FILE文件读取中间缓存,为了更灵活并节约内存需要时再申请
    char* pBufIO;   //FILE文件读取中间缓存,为了更灵活并节约内存需要时再申请
    bool CreateIOBuffer()
    {
        if (pBufIO==NULL)   //不存在才创建
        {
            pBufIO=new(std::nothrow) char[BUFFER_SIZE];
            return pBufIO!=NULL;
        }
        return true;
    }
    void ReleaseIOBuffer()
    {
        if (pBufIO)
        {
            delete[] pBufIO;
            pBufIO=NULL;
        }
    }
    CopyHelp():pBufIO(NULL)
    {
        init();
    }
    ~CopyHelp()
    {
        ReleaseIOBuffer();
    }
    void init()
    {
        Continue=true;
        bContinue=&Continue;//没有外部设置则用默认的

        Pause=false;           //默认不暂停
        bPause=&Pause;    //没有外部设置则用默认的

        totalAllFileSize=0;
        totalCopied=0; 
        AllCopyPercent=0;
        lastAllCopyPercent=0;    //记录拷贝
        Status=CopyStatus_IDLE;
        pFunCallAtCopying=NULL;
        parameterofCopyingFun=NULL;
    }
};


struct pathPair
{
    pathPair(const std::string sDst="",const std::string sSrc="")
    {
        this->sDst=sDst;
        this->sSrc=sSrc;
    }
    std::string sDst;
    std::string sSrc;
};


class CopyFileByThread: public Thread
{
private:
    S_DISABLE_COPY(CopyFileByThread)
public:
    CopyFileByThread()
    {
        dirDcpCopyFilter=NULL;
    }
    virtual ~CopyFileByThread()
    {

    }

    virtual void CallBeforeCopy(){}          //虚函数拷贝线程开始前调用
    //virtual void CallAtCopying(){};            //拷贝过程调用,可以用在通知大文件拷贝进度
    virtual void CallAfterCopyFinish(){}    //拷贝线程结束调用


    //暂停会恢复拷贝控制相关
    void pause(){bPause=true;}            //暂停
    void resume(){bPause=false;}         //恢复
    bool isPause()const {return bPause;}       //是否处于暂停状态

    void stop()
	{
	   resume();
	   Thread::stop();
	}
	void stopWaitFinish()
	{
		this->stop();
		Thread::stopWaitFinish();
	}

    volatile bool bPause;
private:
    virtual void run();
    void copy_dir(const char *dir_dst,const char *dir_src,CopyHelp& copyStatus);
public:
    void setPath(std::string sDst,std::string sSrc)
    {
        //this->sDst=sDst;
        //this->sSrc=sSrc;

        //只有一个的情况
        ListCopyPath.clear();
        pathPair path(sDst,sSrc);
        ListCopyPath.push_back(path);

        printf("CopyThread::setPath() \nPathDst:%s \nPathSrc:%s\n",sDst.c_str(),sSrc.c_str());
    }

    //多组拷贝任务
    void setPath(std::vector<pathPair>& nListCopyPath)
    {
        this->ListCopyPath=nListCopyPath;
        for (size_t n=0;n<this->ListCopyPath.size();n++)
        { 
             const pathPair& tmp=this->ListCopyPath.at(n);
             printf("CopyThread::setPath() Index:%d\nPathDst:%s \nPathSrc:%s\n",(int)n,tmp.sDst.c_str(),tmp.sSrc.c_str());
        }
    }

    //所有文件拷贝进度
    long long getAllTotalFileSize()const{return copyHelp.totalAllFileSize;}
    long long getAllFileCopied()const {return copyHelp.totalCopied;}
    int getAllCopyPercent()const {return copyHelp.AllCopyPercent;}  //0-100

    //当前文件拷贝进度
    long long getCurrFileSize()const {return copyHelp.currFileSize;}
    long long getCurrFileCopied()const {return copyHelp.currFileCopied;}
    int getCurrCopyPercent()const {return copyHelp.currFileCopyPercent;}   //0-100

    void setDirCopyFilter(const FilterFun p){this->dirDcpCopyFilter=p;}
    void setCallBackAtCopying(callAtCopyingFun pFun,void* para)
    {
        copyHelp.pFunCallAtCopying=pFun;
        copyHelp.parameterofCopyingFun=para;
    }
    CopyHelp copyHelp;
private:
//     std::string sDst;
//     std::string sSrc;

    std::vector<pathPair> ListCopyPath;

    FilterFun dirDcpCopyFilter;
    //long long total;
    //long long copied;
    //int copyStatus;
	
public:
	// use cplid as destination directory name [4/9/2020 jaontolt]
	//static std::string strCplId;
};

#endif


