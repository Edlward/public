#ifndef  SHAREMEMORY_H
#define  SHAREMEMORY_H

#include "global.h"
#include "Tstring.h"
#include <stdio.h>


//to be confirm
//进程结束后会立即自动断开与共享内存的连接吗？
//进程结束后,共享内存的引用计数会减1.当引用计数为0时,共享内存自动释放.所以结论是会自动断开.
//PS:windows系统是如此,其他系统不清楚. linux好像程序崩溃不会释放共享内存
//自己加引用计数控制释放？ 好像没有必要暂留


//为了避免包含系统头文件,用外部声明
class ShareMemory;
extern void ShareMemory_CreateNewShareMemory(ShareMemory* parent); 
extern void ShareMemory_GetExistShareMemory(ShareMemory* parent); 
extern void ShareMemory_UnmapFromCreator(ShareMemory* parent); 
extern void ShareMemory_UnmapFromUser(ShareMemory* parent); 
extern void ShareMemory_GetShareMemory(ShareMemory* parent);
extern void ShareMemory_Close(ShareMemory* parent);
extern void ShareMemory_Release(ShareMemory* parent);


NAME_SPACE_MAIN_BEGIN

//基于map的共享内存
class BASIC_API ShareMemory
{
#pragma pack(push,1)     //设置一个字节对齐 保存对齐状态
    //多申请sizeof(SHAREMEM)作为自身引用计计数,其他辅组功能等
    struct ShareMemRef
    {
        volatile	int SumInUse;   //共享内存的引用计数，多一个使用者加一，最后一个使用者释放
    };
#pragma pack(pop)

    //winwows下转为HANDLE,linux下作为为int
#if defined(_WIN32)
    void* HandleCreator;            //创建返回句柄，用此句柄释放共享内存，创建者才可以？
    void* HandleUser;                //使用者句柄
#else
    int HandleCreator;            //创建返回句柄
    int HandleUser;                //使用者句柄
#endif

private:
    ShareMemory(const ShareMemory& r); 
    ShareMemory& operator=(const ShareMemory& r);
public:  //共享内存大小可以根据传参设定,不同进程或相同进程中例化,只用key名称相同大小相同，实际会访问同一片共享内存
    ShareMemory (unsigned int memSize,std::string sKey=("keyName"),bool ClearFirstUse=true)
        :ShareSize(memSize+sizeof(ShareMemRef)),isClearFirstUse(ClearFirstUse)  //sMemName(sKey),
    {
#if defined(_WIN32)
        HandleCreator=NULL;
        HandleUser=NULL;
#else
        HandleCreator=-1;
        HandleUser=-1;
#endif

        sMemName=stringToTstring(sKey);
        pBufCreator=NULL;
        pBufUser=NULL;                      //使用者读写指针
        pBufShare=NULL;
        isCreatOK=false;
        isGetOK=false;
        pShareMemSelf=NULL;
        GetShareMemory();
    }
    ~ShareMemory()
    {
        Close();  //取消本进程的映射，根据是否是最后一个使用者释放共享内存
    }
    char* getAddress() const //可以直接读写的共享内存地址
   {
        return (char*)pBufShare;
   }
   unsigned int getSize() const              //已经申请的共享内存内存大小
   {
       return ShareSize-sizeof(ShareMemRef);   //减去引用计数结构大小
   }
    void Print(unsigned int nsize=UINT32_MAX)const    //如果已经获取，打印内存空间前n个字节,默认打印全部
    {   
        if(NULL==pBufShare)
        {
            return;
        }
        if(nsize>ShareSize)
        {
            nsize=ShareSize;
        }
        printHex(pBufShare,nsize);
    }
private:
    unsigned int ShareSize;                //共享的内存大小
    Tstring sMemName;                    //共享内存块文件名
    char* pBufCreator;                        //创建者的读写指针,创建成功可以用这个指针读写数据
    char* pBufUser;                            //使用者读写指针, 获取成功可以用这个指针读写数据
    char* pBufShare;                          //任何进程可以读可以写的指针 
    ShareMemRef* pShareMemSelf;  //多申请一点内存放尾部作为使用者的引用计数
    bool isClearFirstUse;                     //共享内存首次参加者是否清空内存标记
    bool  isCreatOK;                           //创建成功标志
    bool  isGetOK;                              //获取成功标志
protected:
    ////改创建者使用者模式，只需要一个进程中创建一次，在其他多个子进程中可以多获取获取
    //void  CreateNewShareMemory();           //创建新的内存共享区域
    //void  GetExistShareMemory();               //获取已经存在的内存共享区域
    //void  UnmapFromCreator();                  //创建者取消本进程地址空间的映射
    //void  UnmapFromUser();                       //使用者取消本进程地址空间的映射
    ////再次封装，不分创建和使用，任何子进程，先获取看有没有存在的共享内存,没有就创建
    //void GetShareMemory();
    //void Close();                                         //取消本进程的映射，根据是否是最后一个使用者释放内存
    //void Release();

    //改创建者使用者模式，只需要一个进程中创建一次，在其他多个子进程中可以多获取获取
    void  CreateNewShareMemory()           //创建新的内存共享区域
    {
        ShareMemory_CreateNewShareMemory(this); 
    }
    void  GetExistShareMemory()               //获取已经存在的内存共享区域
    {
        ShareMemory_GetExistShareMemory(this); 
    }
    void  UnmapFromCreator()                  //创建者取消本进程地址空间的映射
    {
        ShareMemory_UnmapFromCreator(this); 
    }
    void  UnmapFromUser()                       //使用者取消本进程地址空间的映射
    {
        ShareMemory_UnmapFromUser(this); 
    }

    //再次封装，不分创建和使用，任何子进程，先获取看有没有存在的共享内存,没有就创建
    void GetShareMemory()
    {
        GetExistShareMemory();
        if(isGetOK==true)
        {
            pBufShare=pBufUser;
            pShareMemSelf=(ShareMemRef*)(pBufShare+ShareSize);
            pShareMemSelf->SumInUse++;       //使用者加一，保存在共享内存
        }
        else
        {
            CreateNewShareMemory();
            if(isCreatOK==true)
            {
                pBufShare=pBufCreator;
                //创建者初始化内存为0  根据需要决定
                if (isClearFirstUse)
                {
                    memset(pBufShare,0,ShareSize);
                }
                pShareMemSelf=(ShareMemRef*)(pBufShare+ShareSize);
                pShareMemSelf->SumInUse=0;        //new 使用者初始化为0
                pShareMemSelf->SumInUse++;       //使用者加一，保存在共享内存
            }
            else
            {
                pBufShare=NULL;
                pShareMemSelf=NULL;

                PRINT_ERROR_LINE 
                throw "get sharememfailed";
            }
        }
        //printf("MapShareMemory::GetShareMemory() Users Num:%d\n",pShareMemSelf->SumInUse);
    }

    void Close()                 //取消本进程的映射，根据是否是最后一个使用者释放内存
    {
        //if(NULL!=pShareMemSelf)
        //{
        //    pShareMemSelf->SumInUse--;             //使用者-1，保存在共享内存
        //    if (pShareMemSelf->SumInUse<=0)    //最后使用者删除这片共享内存,好像没有必要，内核已经用引用计数管理了
        //    {
        //        Release();
        //    }
        //    printf("MapShareMemory::ReleaseMemory() Users Num:%d\n",pShareMemSelf->SumInUse);
        //    pBufShare==NULL;
        //    pShareMemSelf=NULL;  //？
        //}
        UnmapFromCreator();
        UnmapFromUser();
        Release();
    }

    void Release()
    {
       ShareMemory_Release(this); 
    }

private:
    //经典times33算法如下：对于字符串
    static uint32_t  hash_times33(const char* key)
    {
        uint32_t  nHash = 0;
        while (*key)
        {
            nHash=(nHash<<5)+nHash+*key++;
        }
        return nHash;
    }
    static uint32_t hash_times33(const char* buf,size_t len)
    {
        unsigned int nHash=0;
        const char* pos=buf;
        for (size_t n=0;n<len;n++)
        {
            nHash=(nHash<<5)+nHash+(*pos++);
        }
        return nHash;
    }
    //自定义共享内存字符串名称到key的方法
    static uint32_t getShareMemKey(const char* sKeyName)
    {
        size_t keylen=strlen(sKeyName);
        uint32_t key=hash_times33(sKeyName,keylen)+(uint32_t)keylen;
        if (key==0)  //todo:
        {

        }
        return key;
    }
public:
    friend void  ShareMemory_CreateNewShareMemory(ShareMemory* parent); 
    friend void  ShareMemory_GetExistShareMemory(ShareMemory* parent); 
    friend void  ShareMemory_UnmapFromCreator(ShareMemory* parent); 
    friend void  ShareMemory_UnmapFromUser(ShareMemory* parent); 
    friend void ShareMemory_GetShareMemory(ShareMemory* parent);
    friend void ShareMemory_Close(ShareMemory* parent);
    friend void ShareMemory_Release(ShareMemory* parent);
};



#if 0
class PipeShameMemoryPrivate;
class BASIC_API PipeShameMemory
{
public:
    PipeShameMemory ();
    ~PipeShameMemory () ;

    void OnPipeCreate();
    void OnSend();
    void Onrecieve();
    void OnPipeConnect();
    void  Onrecieve2();
    void OnSend2();
private:
    PipeShameMemoryPrivate* pimpl;
};
#endif
//4．管道方式
//管道的类型有两种：匿名管道和命名管道。匿名管道是不命名的，它最初用于本地系统中父进程与
//它启动的子进程之间的通信。命名管道则高级一些，通过一个名字进行标识，使客户端和服务端
//应用程序可以通过该管道进行通信。Win32命名管道甚至可以在不同系统的进程间使用，这使它
//成为许多客户/服务器应用程序的理想之选。
//现在我们用命名管道实现进程间的通信，具体实现过程如下。
//（1）创建命名管道，具体代码如下

#if 0

class PipeShameMemoryPrivate;
class BASIC_API PipeShameMemory
{
public:
    PipeShameMemory ();
    ~PipeShameMemory () ;

    void OnPipeCreate();
    void OnSend();
    void Onrecieve();
    void OnPipeConnect();
    void  Onrecieve2();
    void OnSend2();
private:
    PipeShameMemoryPrivate* pimpl;
};


class PipeShameMemoryPrivate
{
public:
    HANDLE hPipe;
};
PipeShameMemory::PipeShameMemory ()
{
    pimpl=new PipeShameMemoryPrivate;
}
PipeShameMemory::~PipeShameMemory () 
{
    delete pimpl;
}

void PipeShameMemory::OnPipeCreate()
{
    pimpl->hPipe=CreateNamedPipe(("\\\\.\\pipe\\MyPipe"),
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        0,1,1024,1024,0,NULL);
    //PIPE_ACCESS_DUPLEX 管道是双向的
    //FILE_FLAG_OVERLAPPED 允许（但不要求）用这个管道进行异步（重叠式）操作
    //nMaxInstances Long，这个管道能够创建的最大实例数量。必须是1到常数PIPE_UNLIMITED_INSTANCES间的一个值。它对于管道的所有实例来说都应是相同的
    //nOutBufferSize Long，建议的输出缓冲区长度；零表示用默认设置
    //nInBufferSize Long，建议的输入缓冲区长度；零表示用默认设置
    //nDefaultTimeOut Long，管道的默认等待超时。对一个管道的所有实例来说都应相同

    if(INVALID_HANDLE_VALUE==pimpl->hPipe)
    {
        // MessageBox("创建命名管道失败！");
        printf("创建命名管道失败!\n");

        pimpl->hPipe=NULL;
        return;
    }
    HANDLE hEvent;
    hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
    if(!hEvent)
    {
        //  MessageBox("创建事件对象失败！");
        CloseHandle(pimpl->hPipe);
        pimpl->hPipe=NULL;
        return;
    }
    OVERLAPPED ovlap;
    ZeroMemory(&ovlap,sizeof(OVERLAPPED));
    ovlap.hEvent=hEvent;
    if(!ConnectNamedPipe(pimpl->hPipe,&ovlap))
    {
        if(ERROR_IO_PENDING!=GetLastError())
        {
            // MessageBox("等待客户端连接失败！");
            printf("等待客户端连接失败！\n");

            CloseHandle(pimpl->hPipe);
            CloseHandle(hEvent);
            pimpl->hPipe=NULL;
            return;
        }
    }

    if(WAIT_FAILED==WaitForSingleObject(hEvent,INFINITE))
    {
        //MessageBox("等待对象失败！");
        printf("等待对象失败！\n");

        CloseHandle(pimpl->hPipe);
        CloseHandle(hEvent);
        pimpl->hPipe=NULL;
        return;
    }
    CloseHandle(hEvent);
}


//（2）写入数据，对于命名管道的数据写入操作，与上面匿名管道的写入操作也是相同的，代码如下
void PipeShameMemory::OnSend()
{
    // UpdateData(TRUE);
    // char * buf = (char*)(LPCTSTR)m_strsend;
    char  buf[256];
    memset(buf,99,sizeof(buf));

    DWORD dwWrite;
    if(!WriteFile(pimpl->hPipe,buf,100,&dwWrite,NULL))
    {
        //MessageBox("写入数据失败！");
        printf("写入数据失败！\n");
        return;
    }

    //写入的数据
    printf("写入的数据:\n");
    print_hex(buf,sizeof(buf));
}

//（3）读取数据，对于命名管道的数据读取操作，与上面匿名管道的读取操作是一样的，代码如下
void PipeShameMemory::Onrecieve()
{
    char buf[256];
    DWORD dwRead;
    if(!ReadFile(pimpl->hPipe,buf,100,&dwRead,NULL))
    {
        //MessageBox(_T("读取数据失败！"));
        printf("读取数据失败！\n");
        return;
    }

    //读取到的数据
    printf("读取到的数据:\n");
    print_hex(buf,sizeof(buf));

    //m_strrecieve=buf;
    // UpdateData(FALSE);
}


//（4）连接命名管道。客户端在连接服务器端程序创建的命名管道之前，首先应判断一下，
//是否有可以利用的命名管道，这可以通过调用WaitNamedPipe()函数实现，该函数会一直
//等待，直到指定的超时间隔已过，或者指定了命名管道的实例可以用来连接了，也就是说该
//管道的服务器进程有了一个未决的ConnectNamedPipe操作。如果当前命名管道的实例
//可以使用，那么客户端就可以调用CreateFile函数打开这个命名管道，与服务端进程进行
//通信了。客户端的连接命名管道的代码如下
//HANDLE hPipe;
void PipeShameMemory::OnPipeConnect()
{
    if(!WaitNamedPipe(("\\\\.\\pipe\\MyPipe"),NMPWAIT_WAIT_FOREVER))
    {
        // MessageBox(_T("当前没有可利用的命名管道实例！"));
        printf("当前没有可利用的命名管道实例！\n");

        return;
    }
    pimpl->hPipe=CreateFile(("\\\\.\\pipe\\MyPipe"),GENERIC_READ | GENERIC_WRITE,
        0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(INVALID_HANDLE_VALUE==pimpl->hPipe)
    {
        //   MessageBox(_T("打开命名管道失败！"));
        printf("打开命名管道失败！\n");

        pimpl->hPipe=NULL;
        return;
    }
}
//（5）读取数据。如果客户端成功打开了指定的命名管道，那么就可以进行读取和写入操作了，具体代码如下
void  PipeShameMemory::Onrecieve2()
{
    char buf[256];
    DWORD dwRead;
    if(!ReadFile(pimpl->hPipe,buf,100,&dwRead,NULL))
    {
        // MessageBox(_T("读取数据失败！"));
        printf("读取数据失败！\n");

        return;
    }
    // m_strrecieve=buf;
    //  UpdateData(FALSE);
}

//（6）写入数据。客户端向命名管道写入数据与上面服务器端向命名管道写入数据一样，具体代码如下

void PipeShameMemory::OnSend2()
{
    //UpdateData(TRUE);
    // char * buf = (char*)(LPCTSTR)m_strsend;
    char  buf[256];
    DWORD dwWrite;
    if(!WriteFile(pimpl->hPipe,buf,100,&dwWrite,NULL))
    {
        //MessageBox("写入数据失败！");
        printf("写入数据失败！\n");

        return;
    }
}
#endif





NAME_SPACE_MAIN_END
#endif
