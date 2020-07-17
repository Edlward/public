#include "TcpServerByIOCP.h"

//#include "Protocol.h"
//#include "../Proj/Noc/my_struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <list>

//δ�����:�ο���������IOCP�����Ӵ�����
//����piggy�Ȱ�����ACCEPT���̸Ĳ���Ͷ������¼��ķ�ʽ�����ͻ������������


#if defined(_WIN32)

#include <process.h>
#include <winsock2.h>   //<winsock2.h>ͷ�ļ�����֮ǰ�����Ȱ��� <windows.h>
#include <windows.h>

#include "../Basic/ThreadSync.h"   //����windows.h��

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
    int isRightAccept;    //add by lizulin �������߳������ʶ����ȷ�Ŀͻ�����

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

typedef struct  tagPER_IO_DATA    // buffer infoÿ���������Ͷ�� wsasend, wsarecv
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
    bool isComplete;    //�Ƿ���յ���������
    tagPER_IO_DATA():bufBig(0),pBuf(0),len(0)
    {
        wsaBuf.buf=buffer;
        //wsaBuf.len=sizeof(KH);  //�ŵ�PerClient�����ʼ��
    }
    ~tagPER_IO_DATA()
    {
        if (bufBig)
        {
            delete[] bufBig;
        }
    }
}PER_IO_DATA, *LPPER_IO_DATA;


Thread::FunRet_t STDCALL ThreadCtrl(void* p);      //�����ʱ��Socket
Thread::FunRet_t STDCALL ThreadWork(void* pComPort);//��ɶ˿ڴ����߳�

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
    PER_HANDLE_DATA handleInfo; //����hClntSock��Ϣ  
    PER_IO_DATA IODataRead;
    PER_IO_DATA IODataWrite;
    bool shouldContinue;    //��δ��������
    SrvMsgProcessorBase* pMsgProcessor;   //���汨�Ĵ�����ָ��
    TcpServerByIOCP* pSrv;                //����Servָ�����ֱ�Ӳ���
    int ClientID;
    Mutex mutex;
    bool hasbeenPostWillCloseMsg;

    void ClientConnected()   //�ͻ����״����ӱ�����
    {
        pMsgProcessor->ClientConnected();  //ûʲô����
    }                                           
    void ClientDisConnected()  //�ͻ��˶Ͽ������� ����
    {
        pMsgProcessor->ClientDisConnected();
        //pMsgProcessor->CloseConnect();
    }                                      
    void CloseConnect()        //�����������Ͽ��ͻ���
    {
        shouldContinue=false;
    }  

    void do_CloseConnect()        //�����������Ͽ��ͻ���
    {
        printf("PerClient::do_CloseConnect()\n");
        ClientDisConnected();    //�пͻ����ӶϿ����� ����
        closesocket(handleInfo.hSockClient);
        //pSrv->getMsgQueue()->removeChild(pMsgProcessor->getMsgQueue());  //���ӶϿ�����Ϣ���������Ƴ�
        //pMsgProcessor->getMsgQueue()->setNoUse(true);  //����ʹ��
        //handleInfo.hSockClient=INVALID_SOCKET;
        shouldContinue=false;
    }

    //������ȡ����ͷ���Ǳ�������
    void InitDefault()//��ʼ����¶�����ͷ
    {
        IODataRead.rwMode=Mode_READ;
        IODataRead.wsaBuf.buf=IODataRead.buffer;
        IODataRead.wsaBuf.len=pMsgProcessor->getSizeofHead();   //sizeof(KH);//sizeof(KL);//BUF_SIZE
        IODataRead.RemainSize=IODataRead.wsaBuf.len; //added by zn
        memset(&(IODataRead.overlapped), 0, sizeof(OVERLAPPED));		
        IODataRead.DataType=Data_Head;
    }
    void UpdateState()//���¶����ĵ�״̬��������ͷ���Ǳ������ݣ�
    {
        //pMsgProcessor->getSizeofHead();

#if 0
        unsigned int sizeHead=sizeof(KH);                  //sizeof(KH); //sizeof(KL)//����ͷ��С
        KH* pKH=(KH*)IODataRead.buffer;                  //KL* pKL=(KL*)IODataRead.buffer; //IODataRead.wsaBuf.buf    
        unsigned int sizeContext=pKH->cmd_length;   //pKH->cmd_length;          //�������ݴ�С    
        unsigned int ConstHead=PKG_HEAD;               //PKG_HEAD; KL_HEAD  //����ͷ����ֵ
        unsigned int CurrHead=pKH->pkgHead;         //pKH->pkgHead;   //pKL->head; //��ǰ����ֵ
#else

        //IOCP���Ӧ�������ҵ��ҵ�����޹أ���һ���������,�ھ���ҵ���Ĵ������������ʵ�������麯��
        char* bufhead=IODataRead.buffer;
        unsigned int sizeHead=pMsgProcessor->getSizeofHead();
        unsigned int sizeContext=pMsgProcessor->getLen(bufhead);
        unsigned int ConstHead=pMsgProcessor->getConstHeadKey();
        unsigned int CurrHead=pMsgProcessor->getHead(bufhead);
#endif

        //����ͷ����ȷ����λ������
        if(CurrHead!=ConstHead)
        {
            printf("Error ConstHead:0X%X pkgHead:0X%X\n",ConstHead,CurrHead);
            char* buf=IODataRead.wsaBuf.buf;
            for (unsigned int i=0;i<sizeHead-1;i++)
            {
                buf[i]=buf[i+1]; //��������
            }
            //ReadData(&buf[sizeof(KH)-1],1)
            IODataRead.isComplete=false;   
            IODataRead.wsaBuf.buf=&buf[sizeHead-1];  //������ȡ1�ֽڣ�Ͷ�ݵ�������wsaBuf.buf��wsaBuf.len����
            IODataRead.wsaBuf.len=1;
            IODataRead.RemainSize=IODataRead.wsaBuf.len;
            return;
        }

        IODataRead.isComplete=false;
        if (IODataRead.DataType==Data_Head)	//���걨��ͷ����ʼͶ����һ�����񣨱��������һ������ͷ��
        {
            //printf("IORead.ReadLenType==Read_Head len:%d\n",sizeContext);
            if(sizeContext==0)//��Ϣֻ�б���ͷ
            {
                IODataRead.isComplete=true;
                IODataRead.pBuf=IODataRead.buffer;
                IODataRead.len=sizeHead;//��һ�θ��»��Ƕ�Head,��������IODataRead.DataType��IODataRead.wsaBuf
            }
            else//Ͷ�ݶ����������¼�
            {
                IODataRead.DataType=Data_Context;
                //���Ŀ����µ��ڴ�ռ� 
                if (sizeContext>(sizeof(IODataRead.buffer)-sizeHead)) 
                {
                     if (IODataRead.bufBig)
                     {
                          delete[]  IODataRead.bufBig;
                          IODataRead.bufBig=NULL;
                     }
                     IODataRead.bufBig=new char[sizeHead+sizeContext]; 
                     memcpy(IODataRead.bufBig,IODataRead.buffer,sizeHead); 
                     IODataRead.wsaBuf.buf=IODataRead.bufBig+sizeHead; //�´�Ͷ�ݵ�����
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
        else if(IODataRead.DataType==Data_Context)//���걨���壬��ʼͶ�ݱ���ͷ
        {
            IODataRead.DataType=Data_Head;
            IODataRead.wsaBuf.buf=IODataRead.buffer;
            IODataRead.wsaBuf.len=sizeHead; 
            IODataRead.isComplete=true;
        }
        else{
            printf("Unknow Type:%d\n",IODataRead.DataType);
        }

       //�����������ǰ����ʼ��ʣ�����ݴ�С
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
        pMsgProcessor->ProcessDatagram(IODataRead.pBuf,IODataRead.len);//�����������������

        //�����걨���ͷŴ��Ŀռ�
        if (IODataRead.bufBig)
        {
            delete[]  IODataRead.bufBig;
            IODataRead.bufBig=NULL;
        }
    }
    bool ProcessMessage() //�������ڲ��շ���Ϣ
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

        //�첽��������������������
        int  nBytesSent=WSASend(handleInfo.hSockClient,&(IODataWrite.wsaBuf), 1, NULL, 0, &(IODataWrite.overlapped), NULL);
        if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError()))   
        {   
            printf("WSASend() failed\n");

            shouldContinue=false;
        } 
        return 0;  //�첽ģʽ������ֵ������
    }
};


struct  StuShareThread   //�̼߳乲��hComPort
{
    HANDLE hComPort;	       //Ϊ�����߳�����ȡServer������ָ�룬�ŵ��ṹ������
    //bool isAccept;                 //�����ӵ������
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
    //HANDLE hComPort;	   //Ϊ�����߳�����ȡServer������ָ�룬�ŵ��ṹ������
    StuShareThread ComPort;
    SYSTEM_INFO sysInfo;
    LPPER_IO_DATA pIoInfo;
    LPPER_HANDLE_DATA pHandleInfo;
    SOCKET hSockServ;
    SOCKADDR_IN AddrServ;
    
    std::vector<Thread::Handle> VWorkThreadHandle;

    //�ͻ����б�
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
            it=pImpl->ListClient.erase(it);  //������һ����Ч�������������һ
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
    pC->ClientDisConnected();   //�пͻ����ӶϿ�����???����
    getMsgQueue()->removeChild(pC->pMsgProcessor->getMsgQueue());  //���ӶϿ�����Ϣ���������Ƴ�
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
//	ULONG_PTR lpCompleteKey=(ULONG_PTR)&pC->handleInfo;  //64λ
//#else
//	LPDWORD lpCompleteKey=(LPDWORD)&pC->handleInfo;        //32λ
//#endif

    ULONG_PTR lpCompleteKey=(ULONG_PTR)&pC->handleInfo;  //32 64λ������һ���ģ�
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

        //to do �Ƴ���Ϣ����
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
            pC->do_CloseConnect();  //��ʱ�Ƿ�Ҫclosesocket??
            //Thread::sleep_for(1000);
            getMsgQueue()->removeChild(pC->pMsgProcessor->getMsgQueue());  //���ӶϿ�����Ϣ���������Ƴ�
            
            //removeClient(pC);
            it=pImpl->ListClient.erase(it);  //������һ����Ч�������������һ

            //delete pC;
            //printf("CleanInvalidClient() delete pC:%p\n",pC);

            VPerClientCanDelete.push_back(pC);
        }
        else
        {
            ++it;
        }
    }
    Thread::sleep_for(3000);//3s��ɾ������ֹpCɾ����IOCP��Ȼ֪ͨ�˿�����¼�?????
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

    if((pImpl->ComPort.hComPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0))==NULL)//������ɶ˿�
    {
        printf("CreateIoCompletionPort() error!\n");
        return false;
    }  

    GetSystemInfo(&pImpl->sysInfo);

    unsigned int ThreadWorkNum=pImpl->sysInfo.dwNumberOfProcessors;
   
    if (SrvThreadPriority==ESrvPriority::Priority_Low)   //�ϵ����ȼ�������ֻ��һ�������߳�
    {
        ThreadWorkNum=1;
    }
    //ThreadWorkNum=1;

    Thread::Handle handleClean=Thread::startThread((void*)ThreadCtrl,this);//�����̣߳�1��
    if (handleClean==NULL)
    {
        return false;
    }
    pImpl->VWorkThreadHandle.push_back(handleClean);



    pImpl->ComPort.pSrv=this;    //��ס������ָ��
    WorkThreadContinueRun=true;
    for(unsigned int i=0; i<ThreadWorkNum;i++) //�����̳߳أ�����������
    {
        //_beginthreadex(NULL, 0, EchoThreadMain, (LPVOID)hComPort, 0, NULL);          //old
        Thread::Handle handle=Thread::startThread((void*)ThreadWork,&pImpl->ComPort.hComPort);//StuShareThread ComPort���׵�ַ
        if (handle==NULL)
        {
            WorkThreadContinueRun=false;
            return false;
        }
        pImpl->VWorkThreadHandle.push_back(handle);
    }


    pImpl->hSockServ=WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);//��������˼����׽���
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
        //pMsgProcessor->setParentServer(this);   //Ҳ��¼�������ָ��,�����ϲ���չ

        //pMsgProcessor->getSizeofHead();
        pMsgProcessor->setPeerIP(inet_ntoa(clntAdr.sin_addr));      //����ͻ����ӵ�IP��˿�
        pMsgProcessor->setPeerPort(ntohs(clntAdr.sin_port));         
        pMsgProcessor->setLoginTime(time(NULL));    
 

        static int ClientID=0;
        PerClient* pC=new PerClient(pMsgProcessor,this);
        pC->InitDefault();
        pC->ClientConnected();         //�пͻ������ϵ��� ����
        pC->ClientID=ClientID++;       //Ψһ��ʶ�� ����

        //��������ַ�뺯��ָ�뵽���Ĵ������--�Ա���Ʒ�ת
        typedef int (PerClient::*pClassFun)(const char*,unsigned int);
        typedef void (PerClient::*pClassFun2)();
        pClassFun pFunSenddata=&PerClient::SendData;
        pClassFun2 pFunClose=&PerClient::CloseConnect;
        SpOBJpFUN ObjFun;
        ObjFun.pObject=pC;
        ObjFun.pFunSendData=*(void**)(&pFunSenddata);
        ObjFun.pFunClose= *(void**)(&pFunClose);
        pMsgProcessor->setOBJFUN(ObjFun);   //pMsgProcessorҪ����PerClient::SendData��PerClient::CloseConnect


        memcpy(&(pC->handleInfo.AddrClient), &clntAdr, addrLen);
        pC->handleInfo.hSockClient=hClntSock;
        pC->handleInfo.isRightAccept=hClntSock;

        //FileHandle����Ч���ļ������INVALID_HANDLE_VALUE��
        //ExistingCompletionPort���Ѿ����ڵ���ɶ˿ڡ����ΪNULL����Ϊ�½�һ��IOCP��
        //CompletionKey�Ǵ��͸��������Ĳ�����
        //NumberOfConcurrentThreads���ж��ٸ��߳��ڷ��������Ϣ���С�������ExistingCompletionPort��Ϊ0��ʱ��ϵͳ���Ըò��������ò���Ϊ0��ʾ����ͬʱ�����Ŀ�ڴ������������̷߳��ʸ���Ϣ���С�
        //���Դ���CPU���ĸ�����
        //HANDLE han=CreateIoCompletionPort((HANDLE)hClntSock, pImpl->hComPort, (DWORD)pImpl->handleInfo, 0);

#if defined(_WIN64)
        ULONG_PTR lpCompleteKey=(ULONG_PTR)&pC->handleInfo;  //64λ
#else
        DWORD lpCompleteKey=(DWORD)&pC->handleInfo;             //32λ
#endif

        HANDLE han=CreateIoCompletionPort((HANDLE)hClntSock, pImpl->ComPort.hComPort,lpCompleteKey,0);//pC������ɶ˿ڣ�pImpl->ComPort.hComPort����תΪStuShareThread *��lpCompleteKey����תΪpC

        //printf("lpCompleteKey:%p p:%p hSockClient:%d\n",&pC->handleInfo,pC,pC->handleInfo.hSockClient);
        if (han==NULL)
        {
            printf("CreateIoCompletionPort()\n");          
            delete pC;
            return;
        }

        //Ϊ����������Ӷ���
        this->getMsgQueue()->addChildren(pMsgProcessor->getMsgQueue());
        this->addClient(pC);

       
        //�첽��������������������
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
    //�ȴ������߳̽���,һ�㲻��ִ�е��˴�
    for (int n=0;n<pImpl->VWorkThreadHandle.size();n++)
    {
        Thread::waitFinish(pImpl->VWorkThreadHandle.at(n));
    }

    //CloseIoCompletionPort();//�ر���ɶ˿�

    PostQueuedCompletionStatus(pImpl->ComPort.hComPort,0xFFFFFFFF,0,NULL);  //????
    CloseHandle(pImpl->ComPort.hComPort);  
    closesocket(pImpl->hSockServ);  
    //7.����Windows Socket��  
    WSACleanup();  

    CleanAllClient();

    printf("TcpServerByIOCP::run()-end\n");
    return ;//0;
}


enum EPostMsgType
{
    EPType_Begin=0XFFFFFFFF-4,
    EPType_MsgProcess,          //ͳһ������Ϣ
    EPType_WakeUp,              //���ѻ���
    EPType_WillClose,           //�����ر�socket
    EPType_ConfirmClose,        //ȷ�Ϲر�socket
};

Thread::FunRet_t STDCALL ThreadCtrl(void* p)
{
#if 0
    TcpServerByIOCP* pSrv=(TcpServerByIOCP*)p;        //��ȡ��������ַ
    unsigned int Num=0;
    while(pSrv->WorkThreadContinueRun)
    { 
        pSrv->processMsg();         //ͳһ������Ϣ
        pSrv->processWakeUp();    //���ѻ���
        Num++;
        if (Num%16==0)
        {
             pSrv->CleanInvalidClient();//ÿ��20s����һ��
        }
        Thread::sleep_for(1000);
    }
    return 0;
#endif

     TcpServerByIOCP* pSrv=(TcpServerByIOCP*)p;        //��ȡ��������ַ
     while(pSrv->WorkThreadContinueRun)
     { 
        {
            LockerGuard<Mutex> lock(pSrv->pImpl->mutex);
            std::list<PerClient*>::iterator it;
            for (it=pSrv->pImpl->ListClient.begin();it!=pSrv->pImpl->ListClient.end();it++)
            {
                PerClient* pC=*it;	

                //���ټ�����ʱ��Ҫpost��Ϣ�ˣ�����workthread����رղ�delete pC֮������ּ���ȡ���¼���������Խ��
                if (pC->shouldContinue)
                {
                    pSrv->PostMsg(pC,EPType_MsgProcess);
                    pSrv->PostMsg(pC,EPType_WakeUp);
                }
                else //��ֻ֤postһ��EPType_ConfirmClose
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
//�����̷߳���: 	Thread::Handle handle=Thread::startThread((void*)ThreadStatusUpdate,(void*)this);

//DWORD WINAPI EchoThreadMain(LPVOID pComPort)
Thread::FunRet_t STDCALL ThreadWork(void* pComPort)
{
    //printf("ThreadWork -begin\n");
    //HANDLE hComPort=(HANDLE)pComPort;
    HANDLE hComPort=((StuShareThread*)pComPort)->hComPort;
    TcpServerByIOCP* pSrv=((StuShareThread*)pComPort)->pSrv;        //��ȡ��������ַ

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

        //���ó�ʱ
        //32λ/64λ���� lpCompleteKey���Ͳ�һ��
#if defined(_WIN64)
        PULONG_PTR lpCompleteKey=(PULONG_PTR)&pHandleInfo;  //64λ
#else
        LPDWORD lpCompleteKey=(LPDWORD)&pHandleInfo;             //32λ
#endif
       BOOL retGet=GetQueuedCompletionStatus(hComPort,&bytesTrans,lpCompleteKey,(LPOVERLAPPED*)&pIoInfo,1000);   //���ó�ʱ  INFINITE���޵ȴ�  3
       //hComPort��ת��ΪStuShareThread*, *lpCompleteKey����ת��ΪPerClient*,�׵�ַ��ԱΪpHandleInfo
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
        //��accept�����ñ�ǣ����ڴ˴��ж��Ƿ���Ч����
        if (pHandleInfo->hSockClient!=pHandleInfo->isRightAccept)  //��������,�տ�ʼû������ʱ,handleInfo�����ֵ��
        {
            continue;
        }
        sock=pHandleInfo->hSockClient;
        PerClient* pC=(PerClient*)(pHandleInfo);
        

        switch (bytesTrans)
        {
        case EPType_WakeUp:
            {		
                pC->ProcessWakeUp();    //���ѻ��ƣ�����ʱ�Ͽ�
                continue;
            }
            break;
        case EPType_MsgProcess:
            {	
                pC->ProcessMessage(); //�����ڲ���Ϣ
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
                //Thread::sleep_for(2*1000);  //�����Ӻ�ȴ��������й����߳�ִ������ɾ��
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

        //pC->ProcessMessage();     //������Ϣ
        //printf("WSA buf:%p len:%d\n", pIoInfo->wsaBuf.buf,pIoInfo->wsaBuf.len);
        if(pIoInfo->rwMode==Mode_READ)  //READ
        {
            //PerClient* pC=(PerClient*)(ioInfo);
            //puts("Message Received: ");
            //pC->IODataRead.RemainSize=pIoInfo->wsaBuf.len-bytesTrans;       
            //pC->IODataRead.RecvTimes++;
            //printf("WSA buf:%p len:%d bytesTrans:%d RemainSize:%d \n", pIoInfo->wsaBuf.buf,pIoInfo->wsaBuf.len,bytesTrans,pC->IODataRead.RemainSize);

            if(bytesTrans==0)    // EOF,�ر���Ӧ��socket
            {
                printf("bytesTrans==0 ServerPort:%d retGetCP=%d, dwErr=%d SockAlive=%d\n",pC->pSrv->getPort(),retGet, dwErr,IsSocketAlive(sock));
                
                //closesocket(sock);
                //free(handleInfo); 
                //free(ioInfo);
                //shutdown(sock,SD_SEND);    //by me
                //pSrv->CloseRemoveClient(pC);

                pC->shouldContinue=false;

                //printf("shall delete pC:%p\n",pC);
                //pHandleInfo=0;   //��Ӧ�����λ��
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
                pC->UpdateState();//����wsaBuf,Ԥ�������´α��ĵĶ�ȡ
            }

             //pC->UpdateState();
             pC->FilterProcess();//������
             if(!pC->shouldContinue)
             {
                 //pSrv->CloseRemoveClient(pC);
                 continue;
             }
             //pC->ProcessWakeUp();    //���ѻ���
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

            //�첽��������������������
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
            //PerClient* pC=(PerClient*)((char*)ioInfo-sizeof(PER_IO_DATA));   //��ȥƫ��ֵ�ŵõ�
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
