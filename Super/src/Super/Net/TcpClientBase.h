#ifndef TCPCLIENTBASE_H
#define TCPCLIENTBASE_H

#include "../Basic/Thread.h"
#include "../Basic/MessageQueue.h"
#include "../Basic/Timer.h"


#include <string>
#include <vector>

USING_NAME_SPACE_MAIN

class CZSocket;

//客户端功能抽象出来，实现完成放到基础类中
class TcpClientBasePrivate;
class TcpClientBase:public Thread,public Timer
{
private:
    S_DISABLE_COPY(TcpClientBase)
public:
    TcpClientBase();
    virtual ~TcpClientBase();
    virtual void run();
    virtual bool ParserDatagram();                                                  //读socket报文流，分割成一个个数据报文
    virtual void ProcessDatagram(char* buf,unsigned int len)=0;   //报文处理
    virtual bool ProcessMessage();                                                 //消息队列处理
   
    virtual void CallAfterNetConnected();         //网络连接成功被调用
    virtual void CallAfterNetDisConnedted();    //网络断开被调用


    bool WaitReplyAndProcess(unsigned int ms=3000);    //阻塞等待响应,发送完请求后，阻塞等待响应。


    void Send(const char* buf,unsigned int len);   //外部主动发送数据方法,
protected:
    bool hasNewDatatoSent;
    std::string waitSent;
    void SendhasData();   //有未发送数据,内部调用发送
protected:
    int ReadData(char* buf,unsigned int len);
    int SendData(const char* buf,unsigned int len);   //内部发数据使用，不可以外部调用
public:
    void setServerIPport(const char* ip,unsigned short port)
    {
        sIP=ip;
        m_port=port;
    }
    std::string getIP()const {return sIP;}
    unsigned short getPort()const {return m_port;}

     //设置/获取,是否断开自动重连
    void setAutoConnect(bool flag){AutoConnect=flag;}
    bool isAutoConnect()const {return AutoConnect;}

     //外部设定网络连接等待时间
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
    bool AutoConnect;  //重服务器断开会重连，默认使能
    unsigned int ConnectTimeout;    //网络连接等待时间,默认10s,可以外部设定
private:
    MessageQueue MsgQueue;
    TcpClientBasePrivate* pimpl;
};

#endif



