#ifndef SERVER_H
#define SERVER_H

#include "QtTcpSrvResponderBase.h"

#include <QtNetwork>
#include <QHostAddress>
#include <QThread>

#include "Super/Basic/MessageQueue.h"

#include "Super/Net/NetBase.h"   //服务器与其客户线程共享数据访问


//保存连接请求与线程指针的对应关系，方便在断开连接时候删除对应线程指针
struct SrvSocketInfo
{
    int SocketID;                           //
    QtTcpSrvResponderBase*  pSocket;   //新请求对应指针
    QThread* pThread;               //新请求要移动到的线程指针
};

class QtTcpSrvFrame : public QTcpServer		
{
    Q_OBJECT
public:
    explicit QtTcpSrvFrame(int port=0,QObject *parent=0);
   ~QtTcpSrvFrame();
   //类内printf重名函数，为了打印的同时把消息发送到UI显示，注释后自动调用全局::printf打印到控制台
   void printf(const char* fmt, ...);
    bool OpenServer();     //打开服务器,失败就删除当前对象
public:
	int m_port;    //保存即将使用的QTcpServer的端口
     std::vector<SrvSocketInfo> ListClientSocketInfo;
protected:
#if (QT_VERSION>0x050000)                  //必需包含QtCore  才能找到此宏定义
     virtual void incomingConnection(qintptr handle);      //QT4与QT5此函数定义不一样，具体版本不确定
#else
     void incomingConnection(int socketDescriptor);
#endif

signals:
	void sig_PrintMsg(QString sMsg);               //发送打印信息给UI显示
    void sig_DataReady(int socketID);

   //客户端有增减,断开或新连接,发送信号，通知界面,刷新客户端信息列表
    void sig_updateServerUI(QString,int);
    void sig_CMD_TcpServertoUI(int SocketID,int cmdtype); //服务器发给UI
    void sig_CMD_UItoSocket(int SocketID,int cmdtype,QByteArray ba);   //处理UI发过来的命令中转
	void sig_CMD_SocketToUI(int SocketID,int cmdtype,int val,QByteArray ba);   //信号中转

public slots:
    void slot_updateClients(QString,int);
    void slot_ReadData(int socketID);
    void slot_DisConnect(int socketID,QtTcpSrvResponderBase* TcpServerSocket);



    //设置消息处理响应器相关
public:
        void setResponderCreatFun(pMsgResponder pFun){pCreatFun=pFun;}            //设置服务器的业务处理器
private:
        pMsgResponder pCreatFun;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//消息队列相关
public:
    void startPollingMsg();   //开启定时器轮询消息队列是否有消息到来,
    void stopPollingMsg();   //停止轮询定时器
private:
    QTimer tmtodo;
public slots:
    void slot_timetoDo();
public:
    MessageQueue* getMsgQueue(){return &MsgQueue;}

    //提供一个服务器与客户线程访问共享数据的管理类接口,具体用法见SharaDataMgr说明
    SharaDataMgr& getSharaDataMgr(){return m_SharaDataMgr;}
private:
    MessageQueue MsgQueue;
    SharaDataMgr m_SharaDataMgr;
};

#endif

