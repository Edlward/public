#include "TcpServerByMultiThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>    
#include <time.h>
#include "zSocket.h"
#include "../Basic/Timer.h"    //轮询定时器



class PerNetThreadPrivate
{
public:
    CZSocket m_socket;
};

CZSocket& PerNetThread::getSocket()
{
    return pImpl->m_socket;
}

PerNetThread::PerNetThread(SrvMsgProcessorBase* pProc):pMsgProcessor(pProc),m_status(0)
    ,bufBig(NULL)
    ,pImpl(new PerNetThreadPrivate)
    ,sTime("ReadData:")
{
    bRunCompleted = false;
    //PreDprintf("SrvMsgProcessorBase MQ:%p\n",pProc->getMsgQueue());
    Init();
}


PerNetThread::~PerNetThread()
{
    stopWaitFinish();

    if (bufBig!=NULL)
    {
        delete[] bufBig;
        bufBig=NULL;
    }
    //释放报文处理器
    if(pMsgProcessor)
    {
        ReleaseSrvMsgProcessorBase(pMsgProcessor);//delete pMsgProcessor;
        pMsgProcessor=NULL;
    }

    bRunCompleted=true;
    delete pImpl;
}


void PerNetThread::Init()
{
    m_status = RUN;
    bRunCompleted = false;
    pMsgProcessor->setLoginTime(time(NULL));    //记住登陆时间
}

void PerNetThread::Clean()
{
    Thread::stopWaitFinish();
}

void PerNetThread::run()
{
    PRINT_FUN_TRACE

    ////先清空残留消息队列,待确认
    pMsgProcessor->getMsgQueue()->clearMsg();      //清除未处理完的

    m_status = RUN;
    while(getContinueRun())
    {
        switch(m_status)
        {
        case RUN:
            {
                try
                {
                    ProcessWakeUp();
                }
                catch(int& err)
                {
                    PreDprintf("ProcessWakeUp() catch exception:%d \n",err);
                    //pImpl->m_socket.Destroy();
                    m_status = STOP;
                }
            }
            break;
        case STOP:
            ClientDisConnected();
            getSocket().Destroy();
            bRunCompleted = true;
            printf("NetThread::run() Stop\n");
            //break;
            return;
        case IDLE:
            printf("IDLE status:%d\n",m_status);
            Thread::sleep_for(1);
            break;
        default:
            printf("Unknow status:%d\n",m_status);
            break;
        }
    }

    ClientDisConnected();
    getSocket().Destroy();
    bRunCompleted = true;
}


#include "../Basic/SuperTime.h"


static unsigned int ProcessCnt = 0;
bool PerNetThread::ProcessWakeUp()
{
    Thread::sleep_for(1);
    ProcessMessage();   //消息处理

    const int sizeHead=pMsgProcessor->getSizeofHead();
    const int ConstHead=pMsgProcessor->getConstHeadKey();

    size_t getsize = 0;
    char *pBuf = buf;
    if (ReadData(buf,sizeHead)<0)
    {
        return false;
    }

    //找不到报文头--继续搜寻
    int CurrHead=pMsgProcessor->getHead(buf);
    if(CurrHead!=ConstHead)
    {
        //printf("Cmd Error Head:0X%X key:0X%X len:%d\n",pKH->pkgHead,pKH->cmd,pKH->cmd_length);
        while (1)
        {
           // if (pKH->pkgHead!=PKG_HEAD)
            int CurrHead=pMsgProcessor->getHead(buf);
            if(CurrHead!=ConstHead)
            {
                //for (int i=0;i<sizeof(KH)-1;i++)
                for (int i=0;i<sizeHead-1;i++)
                {
                    buf[i]=buf[i+1]; //数据左移
                }
                //if (ReadData(&buf[sizeof(KH)-1],1)<0)//将最后一个buf字符读进来
                if (ReadData(&buf[sizeHead-1],1)<0)//将最后一个buf字符读进来
                {
                         return false;
                }
                //printf("Find Head Error：0X%X\n",pKH->pkgHead);
            }
            else
            {
                break;//成功读取报文头
            }
        }
    }

    const unsigned int MsgContentLen=pMsgProcessor->getLen(buf);

    //bool useNew = false;
    //if(pKH->cmd_length>0)
    if(MsgContentLen>0)
    {
       // if(pKH->cmd_length>(BUFFERSIZE-sizeof(KH)))
        if(MsgContentLen>(BUFFERSIZE-sizeHead))
        {
            //printf("big Payload:key:0X%X len:%d\n",pKH->cmd,pKH->cmd_length);
            if (bufBig!=NULL)
            {
                delete[] bufBig;
            }
            size_t nLen=MsgContentLen+sizeHead+10;
            bufBig = new(std::nothrow) char[nLen];  //(std::nothrow)    失败返回空，不抛异常
            if(bufBig)
            {
                //useNew = true;
                pBuf = bufBig;
                //memcpy(pBuf, pKH, sizeof(KH));
                memcpy(pBuf,buf, sizeHead);
            }
            else
            {
                //TODO:
                //大报文处理或清空
                PRINT_ERROR_LINE
                printf("msg pyload too large =%llu\n",nLen);
                return false;
            }
        }
        //if (ReadData(pBuf+sizeof(KH), pKH->cmd_length)<0)//读取报文内容
        if (ReadData(pBuf+sizeHead,MsgContentLen)<0)//读取报文内容
        {
                return false;
        }
    }

    //unsigned int DataLen=sizeof(KH)+pKH->cmd_length;
    unsigned int DataLen=sizeHead+MsgContentLen;

    //add by lizulin  屏蔽前面memset(buf, 0, sizeof(this->buf)); 后增加下面这句
    pBuf[DataLen]=0;   //接收缓存后面一位清零，防止取字符串出错

    //printf("Cmd Head:0X%X key:0X%X len:%d\n",pKL->head,pKL->key,pKL->len);
    ProcessDatagram(pBuf,DataLen); 

    if(bufBig)
    {
        delete[] bufBig;
        bufBig= NULL;
    }
    return true;
}

int PerNetThread::ReadData(char* buf,unsigned int len)
{
    int ret=getSocket().ReadData(buf,len);
    if(ret<0)
    {
        throw -1;
    }
    return ret;
}


int PerNetThread::SendData(const char* buf,unsigned int len)
{
    int ret=getSocket().SendData(buf,len);
    if(ret<0)
    {
        throw -1;
    }
    return ret;
}

void PerNetThread::CloseConnect()
{
    m_status = STOP;
    //if (bufBig!=NULL)
    //{
    //    delete[] bufBig;
    //    bufBig=NULL;
    //}
}

void PerNetThread::ClientDisConnected()
{
    pMsgProcessor->ClientDisConnected();

     clearMessageQueue();
}


bool PerNetThread::ProcessDatagram(char* buf,unsigned int len)
{
    //printf("NetThread::ProcessDatagram()\n");
    return pMsgProcessor->ProcessDatagram(buf,len);
}

bool PerNetThread::ProcessMessage()
{
    return pMsgProcessor->ProcessMessage();
    //printf("NetThread::ProcessMessage()\n");
}

void PerNetThread::clearMessageQueue()
{
    //new add tobe confirm网络断开,从父亲消息移除,避免队列消息没有机会被处理而阻塞
    pMsgProcessor->getMsgQueue()->removeFromParent();
    pMsgProcessor->getMsgQueue()->clearMsg();      //清除未处理完的
}



class TcpServerByMultiThreadPrivate
{
public:
    CZSocket m_SrvSocket;
    std::list<PerNetThread*> m_listClient;

    //定时器
    TimerDynamic timer;
};

TcpServerByMultiThread::TcpServerByMultiThread():m_status(0)
    ,pImpl(new TcpServerByMultiThreadPrivate)
{
    pNetThread=NULL;
}

TcpServerByMultiThread::~TcpServerByMultiThread()
{
    Stop();
    //cleanThread();
    delete pImpl;
}

//bool NetServer::Init()
//{
//	return true;
//}

//bool NetServer::Start()
//{
//	//if(!(status() == /*thread_stopped*/4 || status() == /*thread_ready*/0))
//	//{
//	//	return false;
//	//}
//	//if(isStopped())
//	//	cleanThread();
//
//
//	if(!start())
//		return false;
//
//	m_status = RUN;
//	return true;
//}

//void NetServer::cleanThread()
//{
//	m_status = STOP;
//	pImpl->m_SrvSocket.Destroy();
//	//CActiveThread::stop();
//}

CZSocket& TcpServerByMultiThread::getSocket()
{
    return pImpl->m_SrvSocket;
}

bool TcpServerByMultiThread::Stop()
{
   PRINT_FUN_TRACE

    m_status = STOP;
    //Thread::sleep_for(100);
    stopWaitFinish();

    getSocket().Destroy();
    std::list<PerNetThread*>::iterator itor;
    for (itor = pImpl->m_listClient.begin(); itor != pImpl->m_listClient.end(); ++itor)
    {
        PerNetThread* client = *itor;
        //由内部清理
        //client->stop();
        //client->waitFinish();
        //while(!client->isRunning()) 
        //{
        //    Thread::sleep_for(10);
        //}
        delete client;
    }
    pImpl->m_listClient.clear();
    return true;
}


bool TcpServerByMultiThread::InitSocket()
{
    return true;
}

bool TcpServerByMultiThread::Bind()
{
    return true;
}

bool TcpServerByMultiThread::Listen()
{
    return true;
}

bool TcpServerByMultiThread::Open()
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

std::string TcpServerByMultiThread::getServerIP()
{
    //return getSocket().getServerIP();

    return sServerIP;
}

int TcpServerByMultiThread::startTimer(unsigned int ms,SrvMsgProcessorBase* p)
{
    return pImpl->timer.StartTimer(ms,SrvMsgProcessorBase::TimeEventConv,p,0);
}

void TcpServerByMultiThread::stopTimer(int tm_id)
{
        return pImpl->timer.StopTimer(tm_id);
}

void TcpServerByMultiThread::run()
{
    PRINT_FUN_TRACE

    char str[512];
    bool bSuccess = false;//bind success
    int i = 0;
    //while(m_status != STOP && bSuccess == false)
    while(getContinueRun()&& bSuccess == false)
    {
        try
        {
            if((i % 5) == 0)
            {
                if (getSocket().Create(AF_INET, SOCK_STREAM, 0))
                {
                    PRINT_FUN_NAME
                        getSocket().printIPInfo();

                    struct sockaddr_in addr_in;
                    memset(&addr_in, 0, sizeof(sockaddr_in));
                    addr_in.sin_family = AF_INET;
                    addr_in.sin_port = htons(getPort());
                    addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
                    int error = 0;
                    do 
                    {
                       //PRINT_TRACE

                        t_timeout tm = 3000;
                        if(getSocket().Bind(&addr_in))
                        {
                            PRINT_FUN_NAME
                                getSocket().printIPInfo();
                            if (!getSocket().Listen(20))
                            {
                                throw -1;
                            }
                            bSuccess = true;
                        }
                        else
                        {
                            throw -2;
                        }

                    } while (error);
                }
                else
                {
                     PRINT_ERROR_LINE
                     printf("Socket Create failed\n");
                }
                i = 0;
            }
            else
            {
                i++;
                Thread::sleep_for(1000);//等4s
               //PRINT_TRACE
            }
        }

        catch (int &a)
        {
            getSocket().Destroy();
            switch(a)
            {
            case -1:
                printf("Server Socket Listen error:");
                break;
            case -2:
                printf("Server Socket Bind error:");
                break;
            default:
                printf("Server Socket unknow error:");
            }

            i++;
            Thread::sleep_for(1000);
            //PRINT_TRACE
        }
    }


    PRINT_FUN_NAME
    getSocket().printIPInfo();


    m_status=RUN;
    while(getContinueRun())//while(1)
    {
        switch(m_status)
        {
        case RUN:
            {
                if (pNetThread==NULL)
                {
                    //pNetThread = new NetThread;
                    //根据不同的NetServer设置不同的pNetThread(多态),NetServer没有多态，只能用函数指针
                    //pNetThread =(PerNetThread*)pFunCreat();
                    SrvMsgProcessorBase* pMsgProcessor=pFunCreat(this);
                    //pMsgProcessor->setParentSrv(this);   //也记录其服务器指针,方便上层扩展

                    pNetThread=new PerNetThread(pMsgProcessor);

                    //保存对象地址与函数指针到报文处理基类--以便控制反转
                    typedef int (PerNetThread::*pClassFun)(const char*,unsigned int);
                    typedef void (PerNetThread::*pClassFun2)();
                    pClassFun pFunSenddata=&PerNetThread::SendData;
                    pClassFun2 pFunClose=&PerNetThread::CloseConnect;
                    SpOBJpFUN ObjFun;
                    ObjFun.pObject=pNetThread;
                    ObjFun.pFunSendData=*(void**)(&pFunSenddata);
                    ObjFun.pFunClose= *(void**)(&pFunClose);
                    pMsgProcessor->setOBJFUN(ObjFun);  
                    //pMsgProcessor要调用PerClient::SendData和PerClient::CloseConnect

                    //运行之后再添加到消息队列,否则满了之后会导致阻塞推送失败
                    //getMsgQueue()->addChildren(pMsgProcessor->getMsgQueue());
                }
                    //PRINT_TRACE

                    t_timeout tm = 5*1000;    //阻塞等待新链接时间
                    //每个ClientSocket：m_pThread->m_socket放到m_pThread中去运行
                    getSocket().SetBlocking(false);
                    int retAccept=getSocket().Accept(pNetThread->getSocket(),NULL,NULL,&tm);
                    if(retAccept==0)
                    {
                        //sServerIP=getSocket().getServerIP();  //may error
                        sServerIP=pNetThread->getSocket().getPeerIP();
                        std::string sClientIP=pNetThread->getSocket().GetSockName();
                        printf("Accept: ServerIP:%s clientIP:%s\n",sServerIP.c_str(),sClientIP.c_str());

                        //PRINT_FUN_NAME
                        //getSocket().printIPInfo();
                        //pNetThread->getSocket().printIPInfo();

                        std::list<PerNetThread*>::iterator it;
                        for (it = pImpl->m_listClient.begin(); it != pImpl->m_listClient.end();)
                        {
                            PerNetThread* client = *it;
                            if(client!=NULL&&client->isRunCompleted())       ////if(client->bStop)   //client->isRunning()
                            {
                                getMsgQueue()->removeChild(client->pMsgProcessor->getMsgQueue());
                                delete client;
                                it=pImpl->m_listClient.erase(it);  // //除去已停止的线程
                            }
                            else
                            {
                                ++it;
                            }
                        }

                        //运行之后再添加到消息队列,否则满了之后会导致阻塞推送失败
                        //getMsgQueue()->addChildren(pMsgProcessor->getMsgQueue());
                        getMsgQueue()->addChildren(pNetThread->pMsgProcessor->getMsgQueue());

                        pImpl->m_listClient.push_back(pNetThread);
                        pNetThread->Init();

                        pNetThread->start();
                        pNetThread = NULL;
                    }

                else
                {
                    //PRINT_TRACE

                    //printf("Accept failed:%d err:%d\n ",retAccept,GetLastError());
                    //printf("Accept failed:%d err:%d %s\n ",retAccept,errno,strerror(errno));
                }
            }
            break;
        case STOP:
            Stop();
            printf("NetServer Stop\n");
            return;
        case IDLE:
            Thread::sleep_for(40);
            break;
        default:
            printf("Unknow status:%d\n",m_status);
            break;
        }
    }

}
