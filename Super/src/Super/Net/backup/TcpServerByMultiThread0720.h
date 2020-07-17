#ifndef TCPSERVERBYMULTITHREAD_H
#define TCPSERVERBYMULTITHREAD_H

//可以兼容dll创建者，dll使用者，直接作为普通文件包含在exe程序里面的导出设定--add by lizulin
//dll创建者项目预定义BASIC_API_EXPORT 
//dll使用者项目预定义BASIC_API_IMPORT
//作为普通文件在exe程序中直接包含使用，不需要定义

#if defined(_WIN32)
	#if defined(BASIC_API_EXPORT)
		#define BASIC_API __declspec(dllexport)
	#elif defined(BASIC_API_IMPORT)
		#define BASIC_API __declspec(dllimport)
	#else
		#define BASIC_API 
	#endif
#else
	#define BASIC_API 
#endif

#include <string>
#include <list>

#include "../Basic/Thread.h"
//#include "../Basic/MessageQueue.h"  //消息队列
#include "../Basic/SuperTime.h"

#include "TcpServerBase.h"


enum NET_RUN_STATUS
{
	IDLE = 0,
	RUN,
	STOP,
};


class CZSocket;
class MessageQueue;
class SrvMsgProcessorPrivate;
class BASIC_API SrvMsgProcessorMultiThread: public SrvMsgProcessorBase,public Thread
//class  NetThread: public brunt::CActiveThread
{
public:
    SrvMsgProcessorMultiThread();
    virtual ~SrvMsgProcessorMultiThread();
    void Init();
    //bool Start();
    //bool Stop();
    //int GetStatus() {return m_status;}
    bool isStop() {return bStop;}
public:
    virtual void ProcessDatagram(char* buf,unsigned int len);
    virtual void ProcessMessage();                //处理消息

    //一般不需要重载
    virtual void ClientDisConnected();      //网络断开时被调用，根据客户端类型不同做不同清理操作
    virtual bool ProcessWakeUp();     
    virtual void CloseConnect();      //主动断开链接
public:
    //MessageQueue* getMsgQueue(){return &MsgQueue;}
    CZSocket& getSocket();
private:
protected:
    //	virtual void doit();
    virtual void run();//doit()
    virtual int SendData(char* buf,unsigned int len); //封装数据发送函数，不需要每次调用内部m_socket去Send
    virtual int ReadData(char* buf,unsigned int readLen);     //失败返回-1，非阻塞

public:
    int clientType;
    bool bStop;
 //   int workFor;                                //登录，工作，UI
private:
protected:
    int m_status;
    enum{
        BUFFERSIZE=1*1024
    };
    char buf[BUFFERSIZE];   //放到类内或全局，提高效率
    char* bufBig;
	//unsigned long long LoginTime;
    //MessageQueue MsgQueue;
    SrvMsgProcessorPrivate* pImpl;
    static int ObjCnt;//计数作用，多少个线程
	HighPrecisionTime sTime;
};

SrvMsgProcessorMultiThread* CreateSrvMsgProcessorMultiThread();


class SrvMsgProcessorMultiThread;
class MessageQueue;
class CZSocket;
class NetServerPrivate;
class BASIC_API TcpServerByMultiThread: public TcpServerBase
//class  NetServer: public brunt::CActiveThread
{
public:
	TcpServerByMultiThread();
	virtual ~TcpServerByMultiThread();
	bool Stop();

    //暂不分开实现，放到run里面
    virtual bool InitSocket();                                    //创建socket
    virtual bool Bind();                                            //绑定
    virtual bool Listen();                                          //监听
    virtual bool Open();                                          //以上的封装

	//bool Init();
	//bool Start();
	//int GetStatus() {return m_status;};
    //void setPort(int n){port=n;};
    //MessageQueue* getMsgQueue(){return &MsgQueue;}
    //void setCreatFun(void* pFun){pFunCreat=(pCreate)pFun;}
private:
    //MessageQueue MsgQueue;
private:
//	virtual void doit();
	virtual void run();//doit()
//	void cleanThread();
    //int port;
    //typedef  SrvMsgProcessorMultiThread* (*pCreate)();  
    //pCreate pFunCreat;
private:
	int m_status; 
    SrvMsgProcessorMultiThread* pNetThread;
    NetServerPrivate* pImpl;
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


