#ifndef THREADCLIENTSOCKET_H
#define THREADCLIENTSOCKET_H

#include "../Basic/Thread.h"
#include "../Basic/Tools.h"


USING_NAME_SPACE_MAIN

//报文处理器，针对不同业务分别实现
class ClientPyloadProcBase
{
public:
    ClientPyloadProcBase(){}
    ~ClientPyloadProcBase(){}
    virtual void ClientConnected(){}            //客户端首次链接被调用
    virtual void ClientDisConnected(){}       //客户端断开被调用
    virtual void ClientDisConnect(){}           //主动断开服务器

    virtual void SendData(char* buf,unsigned int len){}    //主动发生数据
    virtual void DataRecv(char* buf,unsigned int len){}     //完整的报文数据到来
};
ClientPyloadProcBase* CreateClientPyloadProc();



const int Defaultport=4360;



class ThreadClientSocketPrivate;




class CZSocket;
class ThreadClientSocket:public Thread
{
typedef  ClientPyloadProcBase* (*CreateClientMsgProcessFun)();  
public:
    ThreadClientSocket(void);
    ~ThreadClientSocket(void);
    void setServerIPPort(const char* sIP,int port); //设置IP与端口号
    void setCreatFun(ThreadClientSocket::CreateClientMsgProcessFun pFun){pFunCreat=pFun;}  //设置服务器的业务处理器
    bool Init();               //初始化网络环境
    bool Connect();       //连接服务器
    void run();               //运行
private:
    char ip[20];
    int port;
    CZSocket& getSocket();
    int SendData(const char* buf,unsigned int len); //
    int ReadData(char* buf,unsigned int len);   //
private:
    ThreadClientSocketPrivate* pImpl;
    CreateClientMsgProcessFun pFunCreat;
    int SendNum;
    int CurrObjIndex;
    TraceClass<ThreadClientSocket> traceClass;
};

#endif


