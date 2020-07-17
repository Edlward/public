#ifndef TCPSERVERBYIOCP_H
#define TCPSERVERBYIOCP_H

#include "TcpServerBase.h"
//windows 下基基于IOCP的高性能TCP服务器

USING_NAME_SPACE_MAIN

struct PerClient;
class TcpServerByIOCPPrivate;
class TcpServerByIOCP:public TcpServerBase
{
public:
    TcpServerByIOCP(void);
    ~TcpServerByIOCP(void);
    virtual bool InitSocket();     //创建socket
    virtual bool Bind();           //绑定
    virtual bool Listen();         //监听
    virtual bool Open(); 
    virtual void run(); 

    void addClient(PerClient* pC);
    void removeClient(PerClient* pC);
	void CloseRemoveClient(PerClient* pC);
    void PostMsg(PerClient* pC,int MsgType);

	void CleanAllClient();       //线程结束时清理所有剩余的PerClient
	void CleanInvalidClient();   //清理失效PerClient
	bool WorkThreadContinueRun;
private:
public:
    TcpServerByIOCPPrivate* pImpl;
};


#endif



