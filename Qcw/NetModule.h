#ifndef _NetManger_H
#define _NetManger_H

#include <QObject>
//#include <QtNetwork>
#include <QTcpSocket>  //访问内部方法可能需要


#include "MessageRegistrar.h"


//新增一个可以从事件回调参数获取NetModule或者Socket指针,以便进行其他操作,以及多服务器交互时区分，未完待续
//
//void CallNetConnected(NetModule* pm);



class QTcpClient;
class NetModule:public QObject
{
	Q_OBJECT
public:
    NetModule(void);
    ~NetModule(void);
    void InstallNetWork(const std::string &sServerIP,unsigned short port);
    void ChangeNetServer(const std::string &sServerIP,unsigned short port);   //更改IP与端口重新初始化
	unsigned short getLocalPort();   //客户端本机端口

	std::string getPeerIP();               //获取对应服务端的IP--连接成功后
	unsigned short getPeerPort();    //获取对应服务端的端口

    void start();
    void stop();
    QTcpSocket& getQtcpSocket();
    void Send(const char* buf,int len);
private:
    void DataFromSocket(char* buf,int len);
    void NetworkStateChanged(int type,QByteArray ba,char* buf,int len);
signals:
    //void sig_CmdReconnect();
    //void sig_CmdToSocket(int type,QByteArray ba=0,char* buf=0,int len=0,bool isNew=false);	
    void sig_CmdToSocket(int type,StuMessage);	
public slots:
    //void slot_CmdFromSocket(int type,QByteArray ba,char* buf,int len,bool isNew);
    void slot_CmdFromSocket(int type,StuMessage);
public:
    MessageRegistrar& getMessageRegistar()
    {
        return mManger;
    }
private:
    QTcpClient* pTcpClient;
    //MessageSender mSenser;          //消息发送器
    MessageRegistrar mManger;      //报文回调注册管理器
};

#endif







