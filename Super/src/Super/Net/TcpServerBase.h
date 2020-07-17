#ifndef TCPSERVERBASE_Header
#define TCPSERVERBASE_Header

#include "../Basic/Thread.h"
#include "../Basic/ThreadSync.h"   //for mutex or std::mutex
#include "../Basic/MessageQueue.h"
#include "../Basic/Tools.h"

#include <stdio.h>

#include "NetBase.h"



USING_NAME_SPACE_MAIN


typedef struct _SpOBJpFUN_
{
    void* pObject;                          //注册对象地址
    void* pFunSendData;               //注册发送回调函数地址
    void* pFunReadData;               //注册读取回调函数地址
    void* pFunClose;                     //注册主动关闭回调函数地址
}SpOBJpFUN;

//空类 Space Object Jump Function--无用，仅仅是为了引入类函数指针
class  CNoUsetoCAST
{
public:
    int SendData(const char* buf,unsigned int len){}   //发送数据,成功返回0,失败返回未发送字节数，异步发送返回值无意义
    int ReadData(char* buf,unsigned int len){}            //接收数据,成功返回0,失败返回未接收字节数，异步发送返回值无意义
    void CloseConnect();
};

//typedef void (CNoUsetoCAST::*pClassFun)(char*,unsigned int);
typedef int (CNoUsetoCAST::*pClassFun)(const char*,unsigned int);//为何要用类成员指针？
typedef void (CNoUsetoCAST::*pClassFun2)();


class TcpServerBase;
class MessageQueue;
struct Message;


//报文处理器基类，针对不同业务分别实现
class SrvMsgProcessorBase
{
private:
    S_DISABLE_COPY(SrvMsgProcessorBase)
public:
    SrvMsgProcessorBase(TcpServerBase* parent=0)
        :pTcpServerBase(parent)
    {
    }
    virtual ~SrvMsgProcessorBase()
    {
    }
    //被动被调用(接收)
    virtual bool ProcessDatagram(char* buf,unsigned int len){return true;}    //网络报文处理,完整报文到来被调用
    virtual void UnknowProtocol(char* buf,unsigned int len){}      //未知报文

    //处理消息 进程内部不同模块之间通信消息到来,重载后一定调用基类的
    virtual bool ProcessMessage();                                     
    virtual void MessageComing(const Message& msg);    //内部消息到来

    virtual void ClientConnected(){}                                              //客户端首次链接被调用
    virtual void ClientDisConnected(){}                                         //客户端断开被调用

    virtual bool ProcessWakeUp(){return true;}		//客户端超时断开机制，待续


    //一些情况,客户服务线程需要定时处理某业务。由Srv管理层实现
    virtual int startTimer(unsigned int ms);        //返回定时器id
    virtual void stopTimer(int tm_id);                 //停止掉某id定时器,
    virtual void TimeEvent(int tmid,void* pTimerInfo,void* puser=0);
    static int TimeEventConv(int tmid,void* pTimerInfo,void* pUser,void* pData);
    unsigned int time_interval;


    //主动发起--用发送者对象地址与函数指针地址强转实现，实现解耦合
	virtual void CloseConnect() //调用PerClient的CloseConnect();
	{
		pClassFun2 pCallfun=*((pClassFun2*)(&ObjFun.pFunClose));//将void**转为pClassFun2*（类成员指针的地址）
		CNoUsetoCAST* pObject=(CNoUsetoCAST*)ObjFun.pObject;   //强转成类的对象指针，没有转成PerClient？？？？
		(pObject->*pCallfun)();                                //类的对象首地址+函数指针方式调用
	} 
    virtual int SendData(const char* buf,unsigned int len)  //主动发生数据，调用PerClient的SendData
    {
        //将void**转为pClassFun*（类成员指针的地址）？？改变pFunSendData,则pCallfun将会随之变化
        pClassFun pCallfun=*((pClassFun*)(&ObjFun.pFunSendData));
        CNoUsetoCAST* pObject=(CNoUsetoCAST*)ObjFun.pObject;   //强转成类的对象指针，没有转成PerClient
        return (pObject->*pCallfun)(buf,len);                                        //类的对象首地址+函数指针方式调用
    }
    virtual int ReadData(char* buf,unsigned int len)                           //阻塞读数据，异步无用
    {
        //printf("ServerMsgProcessorBase::ReadData()\n");
        return 0;
    }
    virtual void DataRecv(char* buf,unsigned int len)                          //完整的报文数据到来
    {
        //printf("ServerMsgProcessorBase::DataRecv()\n");
    }

    void setPeerPort(unsigned short port){PeerPort=port;}             //创建链接时候被设置
    unsigned short getPeerPort(){return PeerPort;}                         //取客户链接IP地址
    void setPeerIP(const char* sIP);                                         
    char* getPeerIP(){return PeerIP;}

    //登陆时间,上层或Srv框架调用
    void setLoginTime(uint64_t t){LoginTime=t;}
    uint64_t getLoginTime()const {return LoginTime;}

    //最后活动时间
    void setLastAliveTime(uint64_t t){lastAliveTime=t;}
    uint64_t getLastAliveTime()const {return lastAliveTime;}

    //网络框架(IOCP/EPOLL等)应该与具体业务业务报文无关，进一步抽象分离,在具体业务报文处理器里面程序实现以下虚函数
    //不同项目不同报文头协议分别实现，KL头,KH头
    virtual int getHead(char* buf)                 //取报文头标识符常量
    {
        return 0;
    }
    virtual unsigned int getLen(char* buf)   //取报文长度
    {
        return 0;
    }
    virtual int getSizeofHead()       //报文整体大小
    {
        return 0;
    }
    virtual int getConstHeadKey()  //报文头常量值
    {
        return 0;
    }
	int64_t getID(){return socketID;}




//暂时放此处
protected:
    enum{BUFFERSIZE=50*1024};       //音频项目，最大对象帧40多k，所以预留--改模板决定,待续
    char buf[BUFFERSIZE];                  //放到类内
    volatile int m_status;
    volatile bool bRunCompleted;     //是否已经运行完成标记，没有运行过不算
protected:
    //static int ObjCnt;
    TraceClass<SrvMsgProcessorBase> traceClass;
    unsigned int PeerPort;
    char PeerIP[20];
    int64_t LoginTime;       //登陆时间,客户链接到来时保存，用到 1970年1月1日00:00:00到现在的秒数
    int64_t lastAliveTime;   //最后活动时间,可以用来记录最后心跳时间,用开机到现在时间,单位ms


    //无用
	uint64_t socketID;  

    //主动发送数据需要控制反转，记住实际发送的对象地址与函数指针
public:
    void setOBJFUN(SpOBJpFUN ObjFun){this->ObjFun=ObjFun;}
protected:
    SpOBJpFUN ObjFun;

public:
    //返回其服务器指针,为了应用层扩展需要
    TcpServerBase* getParentServer(){return pTcpServerBase;}
    void setParentServer(TcpServerBase* parent){pTcpServerBase=parent;}

    //消息队列，控制Server主动发起消息
    MessageQueue* getMsgQueue(){return &MsgQueue;}

    //提供一个服务器与客户线程访问共享数据的管理类接口,具体用法见SharaDataMgr说明,调用Server的实现
    SharaDataMgr& getSharaDataMgr();
protected:
    MessageQueue MsgQueue;
    TcpServerBase* pTcpServerBase;   //注意不能为NULL,若新建框架,需要在合适的地方设置,否则不要使用共享数据
};


SrvMsgProcessorBase* CreateSrvMsgProcessorBase(TcpServerBase* parent);
void ReleaseSrvMsgProcessorBase(SrvMsgProcessorBase* pMsg);

const int DefaultPort=4360;         //默认端口 Default IP port


enum ESrvPriority
{
    Priority_Low,
    Priority_General,
    Priority_Hight,
};

//CreateResponderOfTcpSrv
//typedef  SrvMsgProcessorBase* (*CreateResponderFun)(TcpServerBase* parent);  

class TcpServerBase:public Thread
{
typedef  SrvMsgProcessorBase* (*CreateResponderFun)(TcpServerBase* parent);  
public:
    TcpServerBase(void);
    virtual ~TcpServerBase(void){}
    void setPort(unsigned short newport)
    {
        port=newport;
    }
    unsigned short getPort()const{return port;}

    //获取服务器所在网络中的IP地址,由上层具体实现提供
    virtual std::string getServerIP(){ return "";}

    //


    //设置当前服务器在进程中的优先级，某些特殊需要负载调节，一般不需要设置
    void setPriority(ESrvPriority p){SrvThreadPriority=p;}  
    ESrvPriority getPriority(){return SrvThreadPriority;}
    void setCreatFun(TcpServerBase::CreateResponderFun pFun){pFunCreat=pFun;}  //设置服务器的业务处理器
    virtual bool InitSocket()=0;                                     //创建socket
    virtual bool Bind()=0;                                             //绑定
    virtual bool Listen()=0;                                           //监听
    virtual bool Open()=0;                                            //以上的封装


    virtual int startTimer(unsigned int ms,SrvMsgProcessorBase* p){return  0;}   //返回定时器id
    virtual void stopTimer(int tm_id){}                                                           //停止掉某id定时器,

private:
    unsigned short port;

protected:
    ESrvPriority SrvThreadPriority;
    CreateResponderFun pFunCreat;

    //消息队列，控制Server主动发起消息
public:
    MessageQueue* getMsgQueue(){return &m_MsgQueue;}

    //提供一个服务器与客户线程访问共享数据的管理类接口,具体用法见SharaDataMgr说明
    SharaDataMgr& getSharaDataMgr(){return m_SharaDataMgr;}
protected:
    MessageQueue m_MsgQueue;
    SharaDataMgr m_SharaDataMgr;
};



//for example
#endif




