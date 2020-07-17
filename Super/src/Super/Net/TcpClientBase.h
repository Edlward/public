#ifndef TCPCLIENTBASE_H
#define TCPCLIENTBASE_H

#include "../Basic/Thread.h"
#include "../Basic/MessageQueue.h"
#include "../Basic/Timer.h"


#include <string>
#include <vector>

USING_NAME_SPACE_MAIN

class CZSocket;

//�ͻ��˹��ܳ��������ʵ����ɷŵ���������
class TcpClientBasePrivate;
class TcpClientBase:public Thread,public Timer
{
private:
    S_DISABLE_COPY(TcpClientBase)
public:
    TcpClientBase();
    virtual ~TcpClientBase();
    virtual void run();
    virtual bool ParserDatagram();                                                  //��socket���������ָ��һ�������ݱ���
    virtual void ProcessDatagram(char* buf,unsigned int len)=0;   //���Ĵ���
    virtual bool ProcessMessage();                                                 //��Ϣ���д���
   
    virtual void CallAfterNetConnected();         //�������ӳɹ�������
    virtual void CallAfterNetDisConnedted();    //����Ͽ�������


    bool WaitReplyAndProcess(unsigned int ms=3000);    //�����ȴ���Ӧ,����������������ȴ���Ӧ��


    void Send(const char* buf,unsigned int len);   //�ⲿ�����������ݷ���,
protected:
    bool hasNewDatatoSent;
    std::string waitSent;
    void SendhasData();   //��δ��������,�ڲ����÷���
protected:
    int ReadData(char* buf,unsigned int len);
    int SendData(const char* buf,unsigned int len);   //�ڲ�������ʹ�ã��������ⲿ����
public:
    void setServerIPport(const char* ip,unsigned short port)
    {
        sIP=ip;
        m_port=port;
    }
    std::string getIP()const {return sIP;}
    unsigned short getPort()const {return m_port;}

     //����/��ȡ,�Ƿ�Ͽ��Զ�����
    void setAutoConnect(bool flag){AutoConnect=flag;}
    bool isAutoConnect()const {return AutoConnect;}

     //�ⲿ�趨�������ӵȴ�ʱ��
    void setNetConnectWaitTime(unsigned int ms){ConnectTimeout=ms;}
    unsigned int getNetConnectWaitTime()const {return ConnectTimeout;}

    CZSocket& getSocket();
    MessageQueue* getMsgQueue(){return &MsgQueue;}
protected:
    enum
    {
        BUFFERSIZE=10*1024
    };
    char buf[BUFFERSIZE];
    char* bufBig;
    unsigned short m_port;
    std::string sIP;
    bool AutoConnect;  //�ط������Ͽ���������Ĭ��ʹ��
    unsigned int ConnectTimeout;    //�������ӵȴ�ʱ��,Ĭ��10s,�����ⲿ�趨
private:
    MessageQueue MsgQueue;
    TcpClientBasePrivate* pimpl;
};

#endif



