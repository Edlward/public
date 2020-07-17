#ifndef THREADCLIENTSOCKET_H
#define THREADCLIENTSOCKET_H

#if defined(_WIN32)
    #include <winsock2.h>
    #pragma comment(lib, "WS2_32")    //��ʽ��������ʹ���߰���ws2_32.lib
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

#include "../basic/Thread.h"

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

class ThreadClientSocket:public Thread
{
public:
    ThreadClientSocket(void);
    ~ThreadClientSocket(void);
    void setServerIPPort(const char* sIP,int port); //����IP��˿ں�
    void setCreatFun(void* pFun){pFunCreat=(pCreate)pFun;}  //���÷�������ҵ������
    bool Init();               //��ʼ�����绷��
    bool Connect();       //���ӷ�����
    void run();               //����
private:
    char ip[20];
    int port;
    typedef  ClientPyloadProcBase* (*pCreate)();  
    pCreate pFunCreat;
#if defined(_WIN32)
     SOCKET hSocket;
#else
#endif


private:
    int SendNum;
    int CurrObjIndex;
    //static int ObjCnt;
    TraceClass<ThreadClientSocket> traceClass;
};

#endif


