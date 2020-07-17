#ifndef NetServerBase_H
#define NetServerBase_H

//简单版的TcpServer封装
//原NetSever 与NetThread的简易封装，主要为了复用NetServer,


#if defined(_WIN32)
#if defined(NET_API_EXPORT)
#define NET_API __declspec(dllexport)
#elif defined(NET_API_IMPORT)
#define NET_API __declspec(dllimport)
#else
#define NET_API 
#endif
#else
#define NET_API 
#endif


#include <string>
#include <list>
#include "../Basic/MessageQueue.h"
#include "../Basic/Thread.h"

#include "NetBase.h"



USING_NAME_SPACE_MAIN

class CZSocket;
class NetThreadBase;
class NetThreadBasePrivate;
class NetServerBase;


class NetThreadBase: public Thread
{
public:
    enum RUN_STATUS
    {
        IDLE = 0,
        RUN,
        STOP,
    };
private:
    S_DISABLE_COPY(NetThreadBase)
public:
     NetThreadBase(NetServerBase* parent=0);
     virtual ~NetThreadBase();
     virtual void Init(){}
     virtual bool Start()
     {
         return Thread::start();
     }
     bool Stop();
     CZSocket& getSocket();

     unsigned short getPort();;
     std::string getServerIP();

     bool isRunCompleted() {return bRunCompleted;}
     void doDisConnect()   //服务端主动断开链接--待确认
     {
         Stop();
     }

     //登陆时间,上层或Srv框架调用
     void setLoginTime(uint64_t t){LoginTime=t;}
     uint64_t getLoginTime()const {return LoginTime;}

     //最后活动时间
     void setLastAliveTime(uint64_t t){lastAliveTime=t;}
     uint64_t getLastAliveTime()const {return lastAliveTime;}

public:
    //返回其服务器指针,为了应用层扩展需要
    NetServerBase* getParentServer()const {return pParentServer;}    
    void setParentServer(NetServerBase* pS){pParentServer=pS;}
    MessageQueue* getMsgQueue(){return &MsgQueue;}   //消息队列

    //提供一个服务器与客户线程访问共享数据的管理类接口,具体用法见SharaDataMgr说明
    SharaDataMgr& getSharaDataMgr();
protected:
    MessageQueue MsgQueue;
    NetServerBase* pParentServer;
protected:
     enum{BUFFERSIZE=50*1024};       //音频项目，最大对象帧40多k，所以预留--改模板决定,待续
     char buf[BUFFERSIZE];                  //放到类内
     volatile int m_status;
     volatile bool bRunCompleted;   //是否已经运行完成标记，没有运行过不算

    int64_t LoginTime;       //登陆时间,客户链接到来时保存，可能用到 1970年1月1日00:00:00到现在的秒数
    int64_t lastAliveTime;   //最后活动时间,可以用来记录最后心跳时间,用开机到现在时间,单位ms
public:
    virtual void run();
    virtual bool ProcessFilter();     
    int SendData(const char* buf,unsigned int len);         //封装数据发送函数，不需要每次调用 内部m_socket去Send
    int ReadData(char* buf,unsigned int len);           //失败返回-1，非阻塞

    virtual void ClientDisConnected();                                  //网络被动断开时被调用，根据客户端类型不同做不同清理操作
    virtual bool ErrorProtocol(char* buf);                             //网络协议错误被调用
    //virtual bool UnknowFunction(char* buf);                       //未知报文到来被调用
    virtual bool ProcessMessage();                                      //处理消息
    virtual bool ProcessDatagram(char* buf,unsigned int msgLen);  //数据报文到来被调用
protected:
     NetThreadBasePrivate* pimpl;
     friend class NetServerBase;
};

NetThreadBase* CreateNetThreadBase(NetServerBase* parent=0);



//CreateResponderOfNetSrv

//typedef  NetThreadBase* (*CreateResponderFun)(NetServerBase* parent);




class NetServerBasePrivate;
class  NET_API NetServerBase: public Thread
{

typedef  NetThreadBase* (*CreateResponderFun)(NetServerBase* parent);

public:
    enum RUN_STATUS
    {
        IDLE = 0,
        RUN,
        STOP,
    };
private:
    S_DISABLE_COPY(NetServerBase)
public:
    NetServerBase();
    virtual ~NetServerBase();
    bool Init();
    bool Start();
    bool Stop();
    void CleanListClient(); //清理释放所以客户处理线程
    void StopListClient();

    int GetStatus() {return m_status;}


    //获取服务器所在网络中的IP地址,具体实现可以由上层覆盖
    virtual std::string getServerIP();

    //set get Server Port
    void setPort(unsigned short n){port=n;}
    unsigned short getPort(){return port;}
private:
    virtual void run();
protected:
    int m_status;
    CZSocket& getSocket();//避免包含CZSocket内部包含windows.h用pimpl技法 by lizulin 
    std::string sServerIP;
    unsigned short port;
    std::list<NetThreadBase*> m_listClient;
    NetThreadBase *m_pThread;
public:
    MessageQueue* getMsgQueue(){return &MsgQueue;}
    void setCreatFun(NetServerBase::CreateResponderFun pFun){pFunCreat=pFun;}            //设置服务器的业务处理器

    //提供一个服务器与客户线程访问共享数据的管理类接口,具体用法见SharaDataMgr说明
    SharaDataMgr& getSharaDataMgr(){return m_SharaDataMgr;}
protected:
    MessageQueue MsgQueue;
    SharaDataMgr m_SharaDataMgr;
    CreateResponderFun pFunCreat;
    NetServerBasePrivate* pimpl;
};

//NET_API NetServer* GetNetServer();

#endif






