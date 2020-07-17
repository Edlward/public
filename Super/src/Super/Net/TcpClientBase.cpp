#include "TcpClientBase.h"

#include "zSocket.h"
#include "../Basic/ThreadSync.h"
#include "../Basic/SuperTime.h"

#include <stdio.h>
#include <stdlib.h>

class TcpClientBasePrivate
{
public:
    CZSocket Socket;
    Mutex mutexSend;
};

TcpClientBase::TcpClientBase(void)
{
    pimpl=new TcpClientBasePrivate;
    AutoConnect=true;
    sIP="127.0.0.1";
    m_port=4200;
    ConnectTimeout=10*1000;  
}

TcpClientBase::~TcpClientBase(void)
{
    delete pimpl;
}

CZSocket& TcpClientBase::getSocket()
{
    return pimpl->Socket;
}

void TcpClientBase::run()
{
     PRINT_FUN_TRACE
   CZSocket& socket=getSocket();


    while (getContinueRun())
    {
            if (!socket.Create(AF_INET, SOCK_STREAM, 0))
            {
                PreDprintf("socket create failed\n");
                return;
            }

             sockaddr_in servAdr;
            memset(&servAdr, 0, sizeof(servAdr));
            servAdr.sin_family=AF_INET;
            servAdr.sin_addr.s_addr=inet_addr(sIP.c_str());  //inet_addr(argv[1]);
            servAdr.sin_port=htons(m_port);                       //htons(atoi(argv[2]));

            //不断重连直到成功
            while (getContinueRun())
            {
                t_timeout tm=ConnectTimeout;//2000;
                int errCode;
                sockaddr_in addr=servAdr;   //防止connet失败会破坏结构，每次重新赋值
                //if ((errCode=socket.Connect(&addr,&tm)==0))
                errCode=socket.Connect(&addr,&tm);
                if (errCode==0)
                {
                    break;   //连接成功--进入下一步
                }
                else
                {
                    PreDprintf("socket connect failed\n");
                    printSocket(addr);

                    continue;   //return;失败重连，不退出
                }
            }

           
            //首次网络连接成功,异常考虑待续
            CallAfterNetConnected();


            while (getContinueRun())
            {
                ProcessMessage();
                try
                {
                    if (hasNewDatatoSent)
                    {
                        SendhasData();
                    }
                    //读消息并解析分割成一个个报文去处理
                    ParserDatagram();
                }
                catch(int& err)   //发生异常则重连
                {
                    PreDprintf("ProcessThrow() exception:%d \n",err);
                    CallAfterNetDisConnedted();
                    socket.Destroy();

                    if (AutoConnect)   //如果自动重连功能开启的话
                    {
                          PreDprintf("ProcessThrow() Reconnect\n");
                          break;    //跳出解析报文循环,重新进入链接循环
                    }
                    else
                    {
                        PreDprintf("ProcessThrow() exit TcpClient\n");
                        
                        socket.Destroy();
                        return;    //否则退出,
                    }

                }
            }
    }

    socket.Destroy();
}


bool TcpClientBase::ParserDatagram()
{
    return true;
}

 

int TcpClientBase::ReadData(char* buf,unsigned int len)
{
    int ret=getSocket().ReadData(buf,len);
    if(ret<0)
    {
        throw -1;
    }
    return ret;
}

int TcpClientBase::SendData(const char* buf,unsigned int len)
{
    int ret=getSocket().SendData(buf,len);
    if(ret<0)
    {
        throw -1;
    }
    return ret;
}

bool TcpClientBase::ProcessMessage()
{
    //return;

    //从消息队列里面读消息并处理,一般不设超时等待。没有消息立即退出，等待下次
    Message msg;
    if (!getMsgQueue()->getMsg(msg,0U))
    {
        return false;
    }


    printf("MsgQueue.getMsg() type=%d\n",msg.type);
    char buftmp[1024];

    switch (msg.type)   
    {
    default:break;
    }
    return true;
}

void TcpClientBase::CallAfterNetConnected()
{

}

void TcpClientBase::CallAfterNetDisConnedted()
{

}

bool TcpClientBase::WaitReplyAndProcess(unsigned int ms)
{
     PRINT_FUN_TRACE

    uint64_t begin=getStartupToNowInms();
    uint64_t used;
    //while (1)                           //读取并处理完一个消息或者超时就退出
    while (getContinueRun())    //为了可以随时响应退出
    {
        if (ParserDatagram())
        {
            return true;
        }
        Thread::sleep_for(10);

        used=getStartupToNowInms()-begin;
        if (used>=ms)
        {
            return false;
        }
    }

    return false;
}

void TcpClientBase::Send(const char* buf,unsigned int len)
{
    LockerGuard<Mutex> locker(pimpl->mutexSend);
    waitSent.append(buf,len);
    hasNewDatatoSent=true;
}

void TcpClientBase::SendhasData()
{
    LockerGuard<Mutex> locker(pimpl->mutexSend);
    SendData(waitSent.data(),waitSent.length());
    waitSent.clear();
    hasNewDatatoSent=false;
}
