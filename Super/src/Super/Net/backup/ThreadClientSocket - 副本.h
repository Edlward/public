#ifndef THREADCLIENTSOCKET_H
#define THREADCLIENTSOCKET_H

#if defined(_WIN32)
    #include <winsock2.h>
    #pragma comment(lib, "WS2_32")    //显式包含或者使用者包含ws2_32.lib
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

#include "../basic/Thread.h"

//报文处理器，针对不同业务分别实现
class ClientPayloadProcBase
{
public:
    ClientPayloadProcBase(){}
    ~ClientPayloadProcBase(){}
    virtual void ClientConnected(){}            //客户端首次链接被调用
    virtual void ClientDisConnected(){}       //客户端断开被调用
    virtual void ClientDisConnect(){}           //主动断开服务器

    virtual void SendData(char* buf,unsigned int len){}    //主动发生数据
    virtual void DataRecv(char* buf,unsigned int len){}     //完整的报文数据到来
};
ClientPayloadProcBase* CreateClientPayloadProc();



const int Defaultport=4360;

class ThreadClientSocket:public Thread
{
public:
    ThreadClientSocket(void);
    ~ThreadClientSocket(void);
    void setServerIPPort(const char* sIP,int port); //设置IP与端口号
    void setCreatFun(void* pFun){pFunCreat=(pCreate)pFun;}  //设置服务器的业务处理器
    bool Init();               //初始化网络环境
    bool Connect();       //连接服务器
    void run();               //运行
private:
    char ip[20];
    int port;
    typedef  ClientPayloadProcBase* (*pCreate)();  
    pCreate pFunCreat;
	ClientPayloadProcBase* clientHeart;

	void ThreadClientSocket::do_authon();         //发起认证请求


#if defined(_WIN32)
     SOCKET hSocket;
#else
#endif


private:
    int SendNum;
    static int ObjCnt;
    int CurrObjIndex;
};

#endif


