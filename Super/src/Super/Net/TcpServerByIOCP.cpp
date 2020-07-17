#include "TcpServerByIOCP.h"

//#include "Protocol.h"
//#include "../Proj/Noc/my_struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <list>

//未完待续:参考其他成熟IOCP框架添加错误处理
//仿照piggy等案例，ACCEPT过程改并发投递完成事件的方式，提高突发访问能力。


#if defined(_WIN32)

#include <process.h>
#include <winsock2.h>   //<winsock2.h>头文件包含之前不能先包含 <windows.h>
#include <windows.h>

#include "../Basic/ThreadSync.h"   //放在windows.h后

enum ERWMode{
    Mode_READ,
    Mode_WRITE
};

enum EDataType{
    Data_Head,
    Data_Context,
};

typedef struct tagPER_HANDLE_DATA   // socket info
{
    SOCKET hSockClient;
    SOCKADDR_IN AddrClient;
    int isRightAccept;    //add by lizulin 用来在线程里面标识是正确的客户链接

    tagPER_HANDLE_DATA()
    {
        hSockClient=0;
        isRightAccept=0;
    }
    ~tagPER_HANDLE_DATA()
    {
        hSockClient=0;
        isRightAccept=0;
    }
} PER_HANDLE_DATA,*LPPER_HANDLE_DATA;

typedef struct  tagPER_IO_DATA    // buffer info每次输入输出投递 wsasend, wsarecv
{
    enum{BUF_SIZE=1024*5};
    OVERLAPPED overlapped;
    WSABUF wsaBuf;
    unsigned int RemainSize;
    char buffer[BUF_SIZE];
    unsigned int len; //length of an entire message
    char* pBuf; //one entire message(head+content)
    char* bufBig;
    int rwMode;         // READ or WRITE
    int DataType;       //head or Content
    //unsigned int RecvTimes;
    bool isComplete;    //是否接收到完整报文
    tagPER_IO_DATA():bufBig(0),pBuf(0),len(0)
    {
        wsaBuf.buf=buffer;
        //wsaBuf.len=sizeof(KH);  //放到PerClient里面初始化
    }
    ~tagPER_IO_DATA()
    {
        if (bufBig)
        {
            delete[] bufBig;
        }
    }
}PER_IO_DATA, *LPPER_IO_DATA;


Thread::FunRet_t STDCALL ThreadCtrl(void* p);      //清除超时的Socket
Thread::FunRet_t STDCALL ThreadWork(void* pComPort);//完成端口处理线程

//Thread::FunRet_t STDCALL ThreadWork(void* pComPort);
void ErrorHandling(char *message);
bool IsSocketAlive(SOCKET s);
struct PerClient
{
    PerClient(SrvMsgProcessorBase* pMsgProcessor, TcpServerByIOCP* pSrv)
        :pMsgProcessor(pMsgProcessor),pSrv(pSrv)
        ,shouldContinue(true)
    {
        hasbeenPostWillCloseMsg=false;
        IODataRead.wsaBuf.len=pMsgProcessor->getSizeofHead();  //sizeof(KH);  //
        IODataWrite.wsaBuf.len=pMsgProcessor->getSizeofHead();
    }
    ~PerClient()
    {
        if(pMsgProcessor)
        {
            ReleaseSrvMsgProcessorBase(pMsgProcessor);//delete pMsgProcessor;
            pMsgProcessor=NULL;
        }
    }
    PER_HANDLE_DATA handleInfo; //包含hClntSock信息  
    PER_IO_DATA IODataRead;
    PER_IO_DATA IODataWrite;
    bool shouldContinue;    //有未发送数据
    SrvMsgProcessorBase* pMsgProcessor;   //保存报文处理器指针
    TcpServerByIOCP* pSrv;                //保存Serv指针便于直接操作
    int ClientID;
    Mutex mutex;
    bool hasbeenPostWillCloseMsg;

    void ClientConnected()   //客户端首次链接被调用
    {
        pMsgProcessor->ClientConnected();  //没什么卵用
    }                                           
    void ClientDisConnected()  //客户端断开被调用 无用
    {
        pMsgProcessor->ClientDisConnected();
        //pMsgProcessor->CloseConnect();
    }                                      
    void CloseConnect()        //服务器主动断开客户端
    {
        shouldContinue=false;
    }  

    void do_CloseConnect()        //服务器主动断开客户端
    {
        printf("PerClient::do_CloseConnect()\n");
        ClientDisConnected();    //有客户链接断开调用 无用
        closesocket(handleInfo.hSockClient);
        //pSrv->getMsgQueue()->removeChild(pMsgProcessor->getMsgQueue());  //链接断开从消息队列里面移除
        //pMsgProcessor->getMsgQueue()->setNoUse(true);  //不再使用
        //handleInfo.hSockClient=INVALID_SOCKET;
        shouldContinue=false;
    }

    //决定读取报文头还是报文内容
    void InitDefault()//初始情况下读报文头
    {
        IODataRead.rwMode=Mode_READ;
        IODataRead.wsaBuf.buf=IODataRead.buffer;
        IODataRead.wsaBuf.len=pMsgProcessor->getSizeofHead();   //sizeof(KH);//sizeof(KL);//BUF_SIZE
        IODataRead.RemainSize=IODataRead.wsaBuf.len; //added by zn
        memset(&(IODataRead.overlapped), 0, sizeof(OVERLAPPED));		
        IODataRead.DataType=Data_Head;
    }
    void UpdateState()//更新读报文的状态：读报文头还是报文内容？
    {
        //pMsgProcessor->getSizeofHead();

#if 0
        unsigned int sizeHead=sizeof(KH);                  //sizeof(KH); //sizeof(KL)//报文头大小
        KH* pKH=(KH*)IODataRead.buffer;                  //KL* pKL=(KL*)IODataRead.buffer; //IODataRead.wsaBuf.buf    
        unsigned int sizeContext=pKH->cmd_length;   //pKH->cmd_length;          //报文内容大小    
        unsigned int ConstHead=PKG_HEAD;               //PKG_HEAD; KL_HEAD  //报文头常量值
        unsigned int CurrHead=pKH->pkgHead;         //pKH->pkgHead;   //pKL->head; //当前报文值
#else

        //IOCP框架应该与具体业务业务报文无关，进一步抽象分离,在具体业务报文处理器里面程序实现以下虚函数
        char* bufhead=IODataRead.buffer;
        unsigned int sizeHead=pMsgProcessor->getSizeofHead();
        unsigned int sizeContext=pMsgProcessor->getLen(bufhead);
        unsigned int ConstHead=pMsgProcessor->getConstHeadKey();
        unsigned int CurrHead=pMsgProcessor->getHead(bufhead);
#endif

        //报文头不正确，移位继续读
        if(CurrHead!=ConstHead)
        {
            printf("Error ConstHead:0X%X pkgHead:0X%X\n",ConstHead,CurrHead);
            char* buf=IODataRead.wsaBuf.buf;
            for (unsigned int i=0;i<sizeHead-1;i++)
            {
                buf[i]=buf[i+1]; //数据左移
            }
            //ReadData(&buf[sizeof(KH)-1],1)
            IODataRead.isComplete=false;   
            IODataRead.wsaBuf.buf=&buf[sizeHead-1];  //继续读取1字节，投递的任务由wsaBuf.buf和wsaBuf.len决定
            IODataRead.wsaBuf.len=1;
            IODataRead.RemainSize=IODataRead.wsaBuf.len;
            return;
        }

        IODataRead.isComplete=false;
        if (IODataRead.DataType==Data_Head)	//读完报文头，开始投递下一个任务（报文体或下一个报文头）
        {
            //printf("IORead.ReadLenType==Read_Head len:%d\n",sizeContext);
            if(sizeContext==0)//消息只有报文头
            {
                IODataRead.isComplete=true;
                IODataRead.pBuf=IODataRead.buffer;
                IODataRead.len=sizeHead;//下一次更新还是读Head,不用重置IODataRead.DataType和IODataRead.wsaBuf
            }
            else//投递读报文内容事件
            {
                IODataRead.DataType=Data_Context;
                //大报文开辟新的内存空间 
                if (sizeContext>(sizeof(IODataRead.buffer)-sizeHead)) 
                {
                     if (IODataRead.bufBig)
                     {
                          delete[]  IODataRead.bufBig;
                          IODataRead.bufBig=NULL;
                     }
                     IODataRead.bufBig=new char[sizeHead+sizeContext]; 
                     memcpy(IODataRead.bufBig,IODataRead.buffer,sizeHead); 
                     IODataRead.wsaBuf.buf=IODataRead.bufBig+sizeHead; //下次投递的任务
                     IODataRead.wsaBuf.len=sizeContext;  
                     //IODataRead.useNewBuf=true;
                     IODataRead.pBuf=IODataRead.bufBig;
                     IODataRead.len=sizeHead+sizeContext; 
                }
                else
                {
                    IODataRead.wsaBuf.buf=IODataRead.buffer+sizeHead; 
                    IODataRead.wsaBuf.len=sizeContext;   
                    //IODataRead.useNewBuf=false;
                    IODataRead.pBuf=IODataRead.buffer;
                    IODataRead.len=sizeHead+sizeContext;
                }
            }
        }
        else if(IODataRead.DataType==Data_Context)//读完报文体，开始投递报文头
        {
            IODataRead.DataType=Data_Head;
            IODataRead.wsaBuf.buf=IODataRead.buffer;
            IODataRead.wsaBuf.len=sizeHead; 
            IODataRead.isComplete=true;
        }
        else{
            printf("Unknow Type:%d\n",IODataRead.DataType);
        }

       //重新填充数据前，初始化剩余数据大小
       IODataRead.RemainSize=IODataRead.wsaBuf.len;
    }
    void FilterProcess()
    {
        if (!IODataRead.isComplete)
        {
            return;
        }

        //KL* pKL=(KL*)IORead.wsaBuf.buf;
        //switch (pKL->key)
        //{
        //case CMD_TEXT_MESSAGE:
        //    {
        //        char* pMsg=(char*)pKL+sizeof(KL);
        //        printf("Recv Msg:%s",pMsg);
        //        SendData((char*)pKL,sizeof(KL)+pKL->len);
        //        break;
        //    }
        //default:
        //    break;
        //}

        //KL* pKL=(KL*)IORead.wsaBuf.buf;
        pMsgProcessor->ProcessDatagram(IODataRead.pBuf,IODataRead.len);//处理读到的完整报文

        //处理完报文释放大报文空间
        if (IODataRead.bufBig)
        {
            delete[]  IODataRead.bufBig;
            IODataRead.bufBig=NULL;
        }
    }
    bool ProcessMessage() //服务器内部收发消息
    {
        return pMsgProcessor->ProcessMessage();
    }
    void ProcessWakeUp()
    {
        //printf("ProcessWakeUp() pC:%p pMsg:%p\n",this,pMsgProcessor);
        pMsgProcessor->ProcessWakeUp();
    }
    int SendData(const char* buf,unsigned int len)
    {
        //printf("WSASend() buf:%p len:%u\n",buf,len);
        
        //memcpy(IODataWrite.buffer,buf,len);
        //IODataWrite.wsaBuf.buf=IODataWrite.buffer;
     
        //way 2:
        IODataWrite.rwMode=Mode_WRITE;
        IODataWrite.wsaBuf.buf=const_cast<char*>(buf);
        IODataWrite.wsaBuf.len=len;
        memset(&(IODataWrite.overlapped), 0, sizeof(OVERLAPPED));		

        //异步发送数据请求，立即返回
        int  nBytesSent=WSASend(handleInfo.hSockClient,&(IODataWrite.wsaBuf), 1, NULL, 0, &(IODataWrite.overlapped), NULL);
        if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError()))   
        {   
            printf("WSASend() failed\n");

            shouldContinue=false;
        } 
        return 0;  //异步模式，返回值无意义
    }
};


struct  StuShareThread   //线程间共享hComPort
{
    HANDLE hComPort;	       //为了在线程里面取Server服务器指针，放到结构体里面
    //bool isAccept;                 //有链接到来标记
    //unsigned int ThreadNum;
    TcpServerByIOCP* pSrv;
     StuShareThread()
     {
     }
     ~StuShareThread()
     {
     }
};

class TcpServerByIOCPPrivate
{
public:
    TcpServerByIOCPPrivate()
    {
    }
    ~TcpServerByIOCPPrivate()
    {
        LockerGuard<Mutex> lock(mutex);
        std::list<PerClient*>::iterator it;
        for (it=ListClient.begin();it!=ListClient.end();it++)
        {
            PerClient* pC=*it;
            delete pC;
        }
        ListClient.clear();
    };
    WSADATA	wsaData;
    //HANDLE hComPort;	   //为了在线程里面取Server服务器指针，放到结构体里面
    StuShareThread ComPort;
    SYSTEM_INFO sysInfo;
    LPPER_IO_DATA pIoInfo;
    LPPER_HANDLE_DATA pHandleInfo;
    SOCKET hSockServ;
    SOCKADDR_IN AddrServ;
    
    std::vector<Thread::Handle> VWorkThreadHandle;

    //客户端列表
    std::list<PerClient*> ListClient;
    Mutex mutex;
};

TcpServerByIOCP::TcpServerByIOCP(void)
     :pImpl(new TcpServerByIOCPPrivate)
{
    WorkThreadContinueRun=true;
}

TcpServerByIOCP::~TcpServerByIOCP(void)
{
    delete pImpl;
}

void TcpServerByIOCP::addClient(PerClient* pC)
{
    LockerGuard<Mutex> lock(pImpl->mutex);
    pImpl->ListClient.push_back(pC);
}

void TcpServerByIOCP::removeClient(PerClient* pC)
{
    //printf("TcpServerByIOCP::removeClient() pC:%p\n",pC);
    LockerGuard<Mutex> lock(pImpl->mutex);
    std::list<PerClient*>::iterator it;
    for (it=pImpl->ListClient.begin();it!=pImpl->ListClient.end();)
    {
        if (*it==pC)
        {
            //printf("match to remove:\n");
            it=pImpl->ListClient.erase(it);  //返回下一个有效迭代器，无需加一
            break;
        }
        else
        {
            ++it;
        }
    }
}

void TcpServerByIOCP::CloseRemoveClient(PerClient* pC)
{
    if(pC->handleInfo.hSockClient!=INVALID_SOCKET)
    {
        closesocket(pC->handleInfo.hSockClient);
        pC->handleInfo.hSockClient=INVALID_SOCKET;
    }
    pC->ClientDisConnected();   //有客户链接断开调用???无用
    getMsgQueue()->removeChild(pC->pMsgProcessor->getMsgQueue());  //链接断开从消息队列里面移除
    removeClient(pC);
    delete pC;
    printf("CloseRemoveClient() delete pC:%p\n",pC);
    //pC=NULL?
}


inline void TcpServerByIOCP::PostMsg(PerClient* pC,int MsgType)
{
    HANDLE CompletlonPort=pImpl->ComPort.hComPort;  
    //DWORD dwNumberOfBytesTrlansferred=0XFFFFFFFF-1;  

//#if defined(_WIN64)
//	ULONG_PTR lpCompleteKey=(ULONG_PTR)&pC->handleInfo;  //64位
//#else
//	LPDWORD lpCompleteKey=(LPDWORD)&pC->handleInfo;        //32位
//#endif

    ULONG_PTR lpCompleteKey=(ULONG_PTR)&pC->handleInfo;  //32 64位参数是一样的？
    LPOVERLAPPED lpoverlapped=&pC->IODataWrite.overlapped;
    PostQueuedCompletionStatus(CompletlonPort,MsgType,lpCompleteKey,lpoverlapped);
}


void TcpServerByIOCP::CleanAllClient()
{
    LockerGuard<Mutex> lock(pImpl->mutex);
    std::list<PerClient*>::iterator it;
    for (it=pImpl->ListClient.begin();it!=pImpl->ListClient.end();it++)
    {
        PerClient* pC=*it;
        delete pC;

        //to do 移除消息队列
    }
    pImpl->ListClient.clear();
}


void TcpServerByIOCP::CleanInvalidClient()
{
#if 0
    MyAutoLocker lock(pImpl->mutex);
    std::vector<PerClient*> VPerClientCanDelete;
    std::list<PerClient*>::iterator it;
    for (it=pImpl->ListClient.begin();it!=pImpl->ListClient.end();)
    {     
        PerClient* pC=*it;
        if(!pC->shouldContinue)
        {
            pC->do_CloseConnect();  //此时是否要closesocket??
            //Thread::sleep_for(1000);
            getMsgQueue()->removeChild(pC->pMsgProcessor->getMsgQueue());  //链接断开从消息队列里面移除
            
            //removeClient(pC);
            it=pImpl->ListClient.erase(it);  //返回下一个有效迭代器，无需加一

            //delete pC;
            //printf("CleanInvalidClient() delete pC:%p\n",pC);

            VPerClientCanDelete.push_back(pC);
        }
        else
        {
            ++it;
        }
    }
    Thread::sleep_for(3000);//3s后删除，防止pC删除后，IOCP仍然通知端口完成事件?????
    for (int i=0;i<VPerClientCanDelete.size();i++)
    {
        PerClient* pC=VPerClientCanDelete.at(i);
        delete pC;
        printf("CleanInvalidClient() delete pC:%p\n",pC);
    }
    //isCleaning=false;
#endif
}


bool TcpServerByIOCP::InitSocket()
{
    if(WSAStartup(MAKEWORD(2, 2), &pImpl->wsaData) != 0)
    {
      //ErrorHandling("WSAStartup() error!"); 
        printf("WSAStartup() error!\n");
        return false;
    }

    if((pImpl->ComPort.hComPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0))==NULL)//创建完成端口
    {
        printf("CreateIoCompletionPort() error!\n");
        return false;
    }  

    GetSystemInfo(&pImpl->sysInfo);

    unsigned int ThreadWorkNum=pImpl->sysInfo.dwNumberOfProcessors;
   
    if (SrvThreadPriority==ESrvPriority::Priority_Low)   //较低优先级服务器只开一个工作线程
    {
        ThreadWorkNum=1;
    }
    //ThreadWorkNum=1;

    Thread::Handle handleClean=Thread::startThread((void*)ThreadCtrl,this);//清理线程，1个
    if (handleClean==NULL)
    {
        return false;
    }
    pImpl->VWorkThreadHandle.push_back(handleClean);



    pImpl->ComPort.pSrv=this;    //记住服务器指针
    WorkThreadContinueRun=true;
    for(unsigned int i=0; i<ThreadWorkNum;i++) //工作线程池，处理器核数
    {
        //_beginthreadex(NULL, 0, EchoThreadMain, (LPVOID)hComPort, 0, NULL);          //old
        Thread::Handle handle=Thread::startThread((void*)ThreadWork,&pImpl->ComPort.hComPort);//StuShareThread ComPort的首地址
        if (handle==NULL)
        {
            WorkThreadContinueRun=false;
            return false;
        }
        pImpl->VWorkThreadHandle.push_back(handle);
    }


    pImpl->hSockServ=WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);//创建服务端监听套接字
    memset(&pImpl->AddrServ, 0, sizeof(pImpl->AddrServ));
    pImpl->AddrServ.sin_family=AF_INET;
    pImpl->AddrServ.sin_addr.s_addr=htonl(INADDR_ANY);
    pImpl->AddrServ.sin_port=htons(getPort());                 //htons(4360);//getPort();//htons(atoi(argv[1]));

    return true;
}

bool TcpServerByIOCP::Bind()
{
    if(::bind(pImpl->hSockServ, (SOCKADDR*)&pImpl->AddrServ, sizeof(pImpl->AddrServ))==SOCKET_ERROR)
    {   
        printf("bind() failed \n");  //ErrorHandling("bind() error");
        return false;
    }
    printf("bind() successfull port:%d\n",getPort());
    return true;
}

bool TcpServerByIOCP::Listen()
{

    if (::listen(pImpl->hSockServ,SOMAXCONN)==SOCKET_ERROR)  //listen(m_sockListen,SOMAXCONN))   //old 5
    {
        printf("listen() failed \n");//ErrorHandling("listen() error");
        return false;
    }
    return true;
}


bool TcpServerByIOCP::Open()
{
    if (!InitSocket())
    {
        return false;
    }
    if (!Bind())
    {
        return false;
    }
    if (!Listen())
    {
        return false;
    }
    return true;
}


void TcpServerByIOCP::run()
{
    printf("void TcpServerByIOCP::run()--begin\n");
    int recvBytes=0;
    int flags=0;

    WorkThreadContinueRun=true;
    while(getContinueRun())//while(1)
    {	
        //printf("accept() begin\n");
        SOCKET hClntSock;
        SOCKADDR_IN clntAdr;

        int addrLen=sizeof(clntAdr);
        hClntSock = accept(pImpl->hSockServ,(SOCKADDR*)&clntAdr,&addrLen); 

        printf("accept socket:%d ip:%s port:%d \n",hClntSock,inet_ntoa(clntAdr.sin_addr),ntohs(clntAdr.sin_port));

        SrvMsgProcessorBase* pMsgProcessor=pFunCreat(this);
        //pMsgProcessor->setParentServer(this);   //也记录其服务器指针,方便上层扩展

        //pMsgProcessor->getSizeofHead();
        pMsgProcessor->setPeerIP(inet_ntoa(clntAdr.sin_addr));      //保存客户链接的IP与端口
        pMsgProcessor->setPeerPort(ntohs(clntAdr.sin_port));         
        pMsgProcessor->setLoginTime(time(NULL));    
 

        static int ClientID=0;
        PerClient* pC=new PerClient(pMsgProcessor,this);
        pC->InitDefault();
        pC->ClientConnected();         //有客户链接上调用 无用
        pC->ClientID=ClientID++;       //唯一标识符 无用

        //保存对象地址与函数指针到报文处理基类--以便控制反转
        typedef int (PerClient::*pClassFun)(const char*,unsigned int);
        typedef void (PerClient::*pClassFun2)();
        pClassFun pFunSenddata=&PerClient::SendData;
        pClassFun2 pFunClose=&PerClient::CloseConnect;
        SpOBJpFUN ObjFun;
        ObjFun.pObject=pC;
        ObjFun.pFunSendData=*(void**)(&pFunSenddata);
        ObjFun.pFunClose= *(void**)(&pFunClose);
        pMsgProcessor->setOBJFUN(ObjFun);   //pMsgProcessor要调用PerClient::SendData和PerClient::CloseConnect


        memcpy(&(pC->handleInfo.AddrClient), &clntAdr, addrLen);
        pC->handleInfo.hSockClient=hClntSock;
        pC->handleInfo.isRightAccept=hClntSock;

        //FileHandle是有效的文件句柄或INVALID_HANDLE_VALUE。
        //ExistingCompletionPort是已经存在的完成端口。如果为NULL，则为新建一个IOCP。
        //CompletionKey是传送给处理函数的参数。
        //NumberOfConcurrentThreads是有多少个线程在访问这个消息队列。当参数ExistingCompletionPort不为0的时候，系统忽略该参数，当该参数为0表示允许同时相等数目于处理器个数的线程访问该消息队列。
        //可以传入CPU核心个数，
        //HANDLE han=CreateIoCompletionPort((HANDLE)hClntSock, pImpl->hComPort, (DWORD)pImpl->handleInfo, 0);

#if defined(_WIN64)
        ULONG_PTR lpCompleteKey=(ULONG_PTR)&pC->handleInfo;  //64位
#else
        DWORD lpCompleteKey=(DWORD)&pC->handleInfo;             //32位
#endif

        HANDLE han=CreateIoCompletionPort((HANDLE)hClntSock, pImpl->ComPort.hComPort,lpCompleteKey,0);//pC加入完成端口，pImpl->ComPort.hComPort可以转为StuShareThread *，lpCompleteKey可以转为pC

        //printf("lpCompleteKey:%p p:%p hSockClient:%d\n",&pC->handleInfo,pC,pC->handleInfo.hSockClient);
        if (han==NULL)
        {
            printf("CreateIoCompletionPort()\n");          
            delete pC;
            return;
        }

        //为服务器添加子队列
        this->getMsgQueue()->addChildren(pMsgProcessor->getMsgQueue());
        this->addClient(pC);

       
        //异步接收数据请求，立即返回
        int nBytesRecv=WSARecv(pC->handleInfo.hSockClient,&(pC->IODataRead.wsaBuf),1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(pC->IODataRead.overlapped), NULL);	

        if ((SOCKET_ERROR == nBytesRecv)&&(WSA_IO_PENDING!=WSAGetLastError()))   
        {   
              printf("WSARecv() failed\n");
              //CloseRemoveClient(pC);
              pC->shouldContinue=false;
        }   
        //printf("WSARecv hSockClient:%d\n",pC->handleInfo.hSockClient);		
    }

    WorkThreadContinueRun=false;
    //等待工作线程结束,一般不会执行到此处
    for (int n=0;n<pImpl->VWorkThreadHandle.size();n++)
    {
        Thread::waitFinish(pImpl->VWorkThreadHandle.at(n));
    }

    //CloseIoCompletionPort();//关闭完成端口

    PostQueuedCompletionStatus(pImpl->ComPort.hComPort,0xFFFFFFFF,0,NULL);  //????
    CloseHandle(pImpl->ComPort.hComPort);  
    closesocket(pImpl->hSockServ);  
    //7.清理Windows Socket库  
    WSACleanup();  

    CleanAllClient();

    printf("TcpServerByIOCP::run()-end\n");
    return ;//0;
}


enum EPostMsgType
{
    EPType_Begin=0XFFFFFFFF-4,
    EPType_MsgProcess,          //统一处理消息
    EPType_WakeUp,              //唤醒机制
    EPType_WillClose,           //即将关闭socket
    EPType_ConfirmClose,        //确认关闭socket
};

Thread::FunRet_t STDCALL ThreadCtrl(void* p)
{
#if 0
    TcpServerByIOCP* pSrv=(TcpServerByIOCP*)p;        //改取服务器地址
    unsigned int Num=0;
    while(pSrv->WorkThreadContinueRun)
    { 
        pSrv->processMsg();         //统一处理消息
        pSrv->processWakeUp();    //唤醒机制
        Num++;
        if (Num%16==0)
        {
             pSrv->CleanInvalidClient();//每隔20s清理一次
        }
        Thread::sleep_for(1000);
    }
    return 0;
#endif

     TcpServerByIOCP* pSrv=(TcpServerByIOCP*)p;        //改取服务器地址
     while(pSrv->WorkThreadContinueRun)
     { 
        {
            LockerGuard<Mutex> lock(pSrv->pImpl->mutex);
            std::list<PerClient*>::iterator it;
            for (it=pSrv->pImpl->ListClient.begin();it!=pSrv->pImpl->ListClient.end();it++)
            {
                PerClient* pC=*it;	

                //不再继续的时候不要post消息了，否则workthread里面关闭并delete pC之后可能又继续取到事件并处理导致越界
                if (pC->shouldContinue)
                {
                    pSrv->PostMsg(pC,EPType_MsgProcess);
                    pSrv->PostMsg(pC,EPType_WakeUp);
                }
                else //保证只post一次EPType_ConfirmClose
                {
                    if (!pC->hasbeenPostWillCloseMsg)
                    {
                        //pSrv->PostMsg(pC,EPType_ConfirmClose);
                        pSrv->PostMsg(pC,EPType_WillClose);
                    }
                    pC->hasbeenPostWillCloseMsg=true;
                }
    
            }
        }
        Thread::sleep_for(500);		
     }
     return 0;
}


//Thread::FunRet_t STDCALL ThreadFun(void* pPara);
//开启线程方法: 	Thread::Handle handle=Thread::startThread((void*)ThreadStatusUpdate,(void*)this);

//DWORD WINAPI EchoThreadMain(LPVOID pComPort)
Thread::FunRet_t STDCALL ThreadWork(void* pComPort)
{
    //printf("ThreadWork -begin\n");
    //HANDLE hComPort=(HANDLE)pComPort;
    HANDLE hComPort=((StuShareThread*)pComPort)->hComPort;
    TcpServerByIOCP* pSrv=((StuShareThread*)pComPort)->pSrv;        //改取服务器地址

    SOCKET sock=0;
    PER_HANDLE_DATA* pHandleInfo=NULL;
    LPPER_IO_DATA pIoInfo=NULL;
    DWORD bytesTrans=0;
    DWORD flags=0;

   ULONG_PTR CompleteKey;  
   //handleInfo=NULL;  //while(1)
    while(pSrv->WorkThreadContinueRun)
    { 
        //GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);

        //设置超时
        //32位/64位参数 lpCompleteKey类型不一样
#if defined(_WIN64)
        PULONG_PTR lpCompleteKey=(PULONG_PTR)&pHandleInfo;  //64位
#else
        LPDWORD lpCompleteKey=(LPDWORD)&pHandleInfo;             //32位
#endif
       BOOL retGet=GetQueuedCompletionStatus(hComPort,&bytesTrans,lpCompleteKey,(LPOVERLAPPED*)&pIoInfo,1000);   //设置超时  INFINITE无限等待  3
       //hComPort可转换为StuShareThread*, *lpCompleteKey可以转换为PerClient*,首地址成员为pHandleInfo
       DWORD dwErr = 9999;
       if(retGet == FALSE )    
       {    
           if(NULL == pIoInfo)  
               continue;  

           //pOverlapped !=NULL  
           dwErr= GetLastError(); 
           printf("dwErr==%d", dwErr);
           if(pHandleInfo == NULL)  
           {  
               continue;  
           }  	    
       }       

       if(pHandleInfo==NULL||pIoInfo==NULL)  //||pIoInfo==NULL
       {
           continue;
       }
     
        //printf("socketAccept:%d\n",pHandleInfo->isRightAccept);
        //在accept处设置标记，用在此处判断是否有效链接
        if (pHandleInfo->hSockClient!=pHandleInfo->isRightAccept)  //经过测试,刚开始没有链接时,handleInfo是随机值。
        {
            continue;
        }
        sock=pHandleInfo->hSockClient;
        PerClient* pC=(PerClient*)(pHandleInfo);
        

        switch (bytesTrans)
        {
        case EPType_WakeUp:
            {		
                pC->ProcessWakeUp();    //唤醒机制，处理超时断开
                continue;
            }
            break;
        case EPType_MsgProcess:
            {	
                pC->ProcessMessage(); //处理内部消息
                continue;
            }
            break;
        case EPType_WillClose:
            {
                pSrv->PostMsg(pC,EPType_ConfirmClose);
            }
            break;
        case EPType_ConfirmClose:
            {
                printf("bytesTrans=EPType_CheckClose\n");
                //Thread::sleep_for(2*1000);  //故意延后等待其他并行工作线程执行完再删？
                pSrv->CloseRemoveClient(pC);    			
                continue;
            }
            break;
        default:
            break;
        }

        if(!pC->shouldContinue)
        {

            continue;
        }


        //printf("GetIOCP:%d bytesTrans:%u pIoInfo:%p pHandleInfo:%p socket:%d \n",retGet,bytesTrans,pIoInfo,pHandleInfo,sock);

        //pC->ProcessMessage();     //处理消息
        //printf("WSA buf:%p len:%d\n", pIoInfo->wsaBuf.buf,pIoInfo->wsaBuf.len);
        if(pIoInfo->rwMode==Mode_READ)  //READ
        {
            //PerClient* pC=(PerClient*)(ioInfo);
            //puts("Message Received: ");
            //pC->IODataRead.RemainSize=pIoInfo->wsaBuf.len-bytesTrans;       
            //pC->IODataRead.RecvTimes++;
            //printf("WSA buf:%p len:%d bytesTrans:%d RemainSize:%d \n", pIoInfo->wsaBuf.buf,pIoInfo->wsaBuf.len,bytesTrans,pC->IODataRead.RemainSize);

            if(bytesTrans==0)    // EOF,关闭相应的socket
            {
                printf("bytesTrans==0 ServerPort:%d retGetCP=%d, dwErr=%d SockAlive=%d\n",pC->pSrv->getPort(),retGet, dwErr,IsSocketAlive(sock));
                
                //closesocket(sock);
                //free(handleInfo); 
                //free(ioInfo);
                //shutdown(sock,SD_SEND);    //by me
                //pSrv->CloseRemoveClient(pC);

                pC->shouldContinue=false;

                //printf("shall delete pC:%p\n",pC);
                //pHandleInfo=0;   //对应句柄置位空
                continue;		
            }

            //memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
            //ioInfo->wsaBuf.len=bytesTrans;
            //ioInfo->rwMode=WRITE;
            pC->IODataRead.RemainSize-=bytesTrans;
            if(pC->IODataRead.RemainSize>0)
            {
                pC->IODataRead.wsaBuf.buf+=bytesTrans;
                pC->IODataRead.wsaBuf.len-=bytesTrans;
                pC->IODataRead.isComplete=false;
            }
            else
            {
                pC->UpdateState();//更新wsaBuf,预先设置下次报文的读取
            }

             //pC->UpdateState();
             pC->FilterProcess();//处理报文
             if(!pC->shouldContinue)
             {
                 //pSrv->CloseRemoveClient(pC);
                 continue;
             }
             //pC->ProcessWakeUp();    //唤醒机制
             //if(!pC->shouldContinue)
             //{
                // pSrv->CloseRemoveClient(pC);
                // continue;
             //}

            //ioInfo=(LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
            //memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
            //ioInfo->wsaBuf.len=BUF_SIZE; 
            //ioInfo->wsaBuf.buf=ioInfo->buffer;
            //ioInfo->rwMode=READ;

            //printf(" WSARecv -begin  ");
            pC->IODataRead.rwMode=Mode_READ;
            memset(&(pC->IODataRead.overlapped), 0, sizeof(OVERLAPPED));

            //WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);

            //异步接收数据请求，立即返回
            int nBytesRecv=WSARecv(sock, &(pC->IODataRead.wsaBuf), 1, NULL, &flags, &(pC->IODataRead.overlapped), NULL);
            //printf(" WSARecv -end\n");
            if ((SOCKET_ERROR == nBytesRecv)&&(WSA_IO_PENDING!=WSAGetLastError()))   
            {   
                printf("WSARecv() failed\n");

                //pSrv->CloseRemoveClient(pC);
                pC->shouldContinue=false;

                continue;
            }   
        }
        else if (pIoInfo->rwMode==Mode_WRITE)
        {
            //puts("Message Sent! ");
            //PerClient* pC=(PerClient*)((char*)ioInfo-sizeof(PER_IO_DATA));   //减去偏移值才得到
            //free(ioInfo);
        }
        else
        {
            printf("Unknow rwMode:%d\n",pIoInfo->rwMode);
        }
    } //end while

    printf("ThreadWork -end\n");
    return 0;
}


//void ErrorHandling(char *message)
//{
//    fputs(message, stderr);
//    fputc('\n', stderr);
//    exit(1);
//}

bool IsSocketAlive(SOCKET s)  
{  
    int nByteSent=send(s,"",0,0);  
    if (-1 == nByteSent)  
        return false;  
    return true;  
}  
#endif
