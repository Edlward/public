#ifndef TCPSERVERBYIOCP_H
#define TCPSERVERBYIOCP_H

#include "TcpServerBase.h"
//windows �»�����IOCP�ĸ�����TCP������

USING_NAME_SPACE_MAIN

struct PerClient;
class TcpServerByIOCPPrivate;
class TcpServerByIOCP:public TcpServerBase
{
public:
    TcpServerByIOCP(void);
    ~TcpServerByIOCP(void);
    virtual bool InitSocket();     //����socket
    virtual bool Bind();           //��
    virtual bool Listen();         //����
    virtual bool Open(); 
    virtual void run(); 

    void addClient(PerClient* pC);
    void removeClient(PerClient* pC);
	void CloseRemoveClient(PerClient* pC);
    void PostMsg(PerClient* pC,int MsgType);

	void CleanAllClient();       //�߳̽���ʱ��������ʣ���PerClient
	void CleanInvalidClient();   //����ʧЧPerClient
	bool WorkThreadContinueRun;
private:
public:
    TcpServerByIOCPPrivate* pImpl;
};


#endif



