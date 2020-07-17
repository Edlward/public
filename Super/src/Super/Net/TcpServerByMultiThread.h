#ifndef TCPSERVERBYMULTITHREAD_H
#define TCPSERVERBYMULTITHREAD_H

#include <string>
#include <list>

#include "../Basic/Thread.h"
#include "../Basic/Tools.h"
#include "../Basic/SuperTime.h"
//#include "../Basic/MessageQueue.h"  //消息队列


#include "TcpServerBase.h"


USING_NAME_SPACE_MAIN


class CZSocket;
class MessageQueue;
class PerNetThreadPrivate;
//class BASIC_API SrvMsgProcessorMultiThread: public SrvMsgProcessorBase,public Thread
class BASIC_API PerNetThread: public Thread
{
    enum RUN_STATUS
    {
        IDLE = 0,
        RUN,
        STOP,
    };
public:
    PerNetThread(SrvMsgProcessorBase* pProc);
    virtual ~PerNetThread();
    void Clean();
    void Init();
    bool isRunCompleted() {return bRunCompleted;}
public:
     bool ProcessDatagram(char* buf,unsigned int len);
     bool ProcessMessage();                //处理消息

    void ClientDisConnected();      //网络断开时被调用，根据客户端类型不同做不同清理操作
    bool ProcessWakeUp();     
    void CloseConnect();                //主动断开链接
    void clearMessageQueue();
public:
public:
    SrvMsgProcessorBase* pMsgProcessor;   //保存报文处理器指针
    CZSocket& getSocket();
private:
protected:
public:
    //	virtual void doit();
    virtual void run();//doit()
    int SendData(const char* buf,unsigned int len); //封装数据发送函数，不需要每次调用内部m_socket去Send
    int ReadData(char* buf,unsigned int len);     //失败返回-1，非阻塞
public:
    int clientType;
    bool bRunCompleted;
private:
protected:
    int m_status;
    enum{
        BUFFERSIZE=50*1024,    //10k 改模板或根据传参决定,待续
    };
    char buf[BUFFERSIZE];
    char* bufBig;
    PerNetThreadPrivate* pImpl;
    TraceClass<PerNetThread> traceClass;
    SuperTime sTime;
};


//PerNetThread* CreateSrvMsgProcessorMultiThread();

class PerNetThread;
class MessageQueue;
class CZSocket;
class TcpServerByMultiThreadPrivate;
class BASIC_API TcpServerByMultiThread: public TcpServerBase
{
    enum RUN_STATUS
    {
        IDLE = 0,
        RUN,
        STOP,
    };
public:
    TcpServerByMultiThread();
    virtual ~TcpServerByMultiThread();
    //暂不分开实现，放到run里面
    virtual bool InitSocket();                                    //创建socket
    virtual bool Bind();                                            //绑定
    virtual bool Listen();                                          //监听
    virtual bool Open();                                          //以上的封装

    //获取服务器所在网络中的IP地址
    virtual std::string getServerIP();


    virtual int startTimer(unsigned int ms,SrvMsgProcessorBase* p);                 //返回定时器id
    virtual void stopTimer(int tm_id);                                                                 //停止掉某id定时器,

    bool Stop();
    CZSocket& getSocket();
    std::string sServerIP;
private:
private:
    virtual void run();
private:
    int m_status; 
    PerNetThread* pNetThread;
    TcpServerByMultiThreadPrivate* pImpl;
};


//enum NetServerInstanceIndex
//{
//	//NetSrv_BEGIN=0,
//	NetSrv_Login=0,
//	NetSrv_Work,
//	NetSrv_UI,
//	NetSrv_Total,
//};
//TcpServerByMultiThread* GetNetServer(unsigned int nInstance);

#endif


