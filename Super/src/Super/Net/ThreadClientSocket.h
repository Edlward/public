#ifndef THREADCLIENTSOCKET_H
#define THREADCLIENTSOCKET_H

#include "../Basic/Thread.h"
#include "../Basic/Tools.h"


USING_NAME_SPACE_MAIN

//���Ĵ���������Բ�ͬҵ��ֱ�ʵ��
class ClientPyloadProcBase
{
public:
    ClientPyloadProcBase(){}
    ~ClientPyloadProcBase(){}
    virtual void ClientConnected(){}            //�ͻ����״����ӱ�����
    virtual void ClientDisConnected(){}       //�ͻ��˶Ͽ�������
    virtual void ClientDisConnect(){}           //�����Ͽ�������

    virtual void SendData(char* buf,unsigned int len){}    //������������
    virtual void DataRecv(char* buf,unsigned int len){}     //�����ı������ݵ���
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
    void setServerIPPort(const char* sIP,int port); //����IP��˿ں�
    void setCreatFun(ThreadClientSocket::CreateClientMsgProcessFun pFun){pFunCreat=pFun;}  //���÷�������ҵ������
    bool Init();               //��ʼ�����绷��
    bool Connect();       //���ӷ�����
    void run();               //����
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


