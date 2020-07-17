#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#include <QtNetwork>
#include <QThread>

#include "Super/Basic/MessageQueue.h"

enum CMD_UItoSocket //UI与socket线程的通信命令，UI发命令socket
{
    UI_TEST,                                 //测试命令，重UI发出,命令socket再命令客户端
    UI_SenDataDirect,                  //UI直接发送网络数据
    UI_SenDatatoUI,                    //发送网络数据给UI
    UI_MsgQueueData,               //内部消息队列TcpServer转发给socket
};

enum CMD_TcpServertoUI   //TcpServertoUI
{
    SrvSocket_NEWCOMMING=0X3000,   //有新的client连接到来
    SrvSocket_DISCONNECT           //有socket断开
};


//enum EMConst
//{
//    //Sever端需要处理的命令加前缀S   Client端加C 太乱
//    //CMD_TEST=0X999,                        //自定义测试命令
//    //CMD_ZERO=0x0000,                     //填充cmd_Sub
//    //PKG_HEAD=0x55,                           //数据包头
//    //CMD_REV=0x78563412,                 //自定义保留字
//    //CMD_REV_LEONIS=0x74752485,    //自定义leonis保留字
//};


//是否可以改成线程类
//Qt Server 服务端消息响应器基类,不同业务需要,继承后,重新实现一些虚函数操作即可


class QtTcpSrvFrame;
class QtTcpSrvResponderBase : public QTcpSocket//,public QThread
{
    Q_OBJECT
public:
	void run();
public:
    QtTcpSrvResponderBase(int socketDescriptor,QObject *parent = 0);
     virtual ~QtTcpSrvResponderBase();

     //被动调用
     virtual bool ProcessDatagram(char* buf,unsigned int len);    //网络报文处理,完整报文到来被调用
     virtual void UnknowProtocol(char* buf,unsigned int len);      //未知报文
     //virtual bool ProcessMessage();                                           //处理消息
     virtual bool ProcessMessage(const Message& msg);            //处理消息--程序内部不同模块通信的消息队列
     virtual void ClientConnected();                                              //客户端首次链接被调用
     virtual void ClientDisConnected();                                          //客户端断开被调用

	virtual void CloseConnect(); //主动调用,断开客户端链接


public:
     //类内printf重名函数，为了打印的同时把消息发送到UI显示，注释后自动调用全局::printf打印到控制台
    void printf(const char* fmt, ...);   
    void timerEvent(QTimerEvent * te);
private:
    int socketID;        //保存id 若用this->socketDescriptor()客户端断开会被释放,断开信号得不到正确值
    int timer_toDO;    //定时发起各种任务
public:
    int SendData(char* buf,int size,int val=0);  //填充CRC字段,然后加密发送 val表示加密偏移,返回发送失败的字节数
//几个不方便信号传参结构体,再改进,注册新类型或改全局数组加SL结构体。


signals:
	void sig_PrintMsg(QString sMsg);               //发送打印信息给UI显示
    void sig_DataReady(int socketID);
    void sig_DisConnect(int socketID,QtTcpSrvResponderBase* TcpServerSocket);  //同时传递当前对象指针,
	void sig_updateClients(QString,int);      //处理登陆变化
	void sig_CMD_SocketToUI(int SocketID,int cmdtype,int val,QByteArray ba="");   //socket线程发送给UI的命令
    void sig_getSocketData();    //自己构思的,代替阻塞等待某个命令返回,关联slot_DataReceived()槽

private:
    //读数据方法1: 并处理相关函数改写--保证可靠读取数据
    char bufStack[500*1024];
    bool isblocking;

    int  RecvDataProcess();                     //读取网络缓存的数据并处理,返回读取到的字节数，可以由信号触发或手动调用
    bool WaitForData();                          //等待数据
    int  ClearSocketBuffer();                   //自定义清空Socket读缓存
    int  ReadNbytes(char* buf,int size);  //读取N个字节  改int seconds); //读N个字节 设置超时退出

    //读数据方法2；思路就是每次读取所有有效数据，存到大缓冲区如100M,地址累加偏移,然后while循环处理所有命令
    //思路二有bug,暂时不好解决,如果此时head刚好一部分在buf尾部一部分在头部，非常不好处理。需要加特殊考虑，待续
    enum 
    {
        BufAllSize=10*1024*1024  //总的数据缓存大小
    };
    char pBufBig[BufAllSize];      //没必要new,因为当前类一般new来使用
    char* pBufStart;    //数据缓存起始地址
    char* pBufEnd;      //数据缓存结束地址
    char* pStart;       //未处理报文起始地址
    char* pEnd;         //未处理报文结束地址
    void InitReadMethod2();    //初始化以上变量
    void  RecvAllDataToProc();             //一次获取所有数据
    unsigned int  getRemainBufSize();              //获取剩余缓缓存空间大小
    unsigned int  getUnProcBufSize();              //获取未处理有效报文空间大小
    int  ReadNbytesforWhile(char* buf,int size);  //直接读取N个字节,不等待
    int  ReadDatatoBuf(int readSize);     //根据数据大小读数据到缓存，因为可能尾部地址溢出，要分段读取
    void  ProcessAll();

    //两种读取方法共同调用报文处理函数
    int  FilterProcess(char* buf,int size);      //根据命令类型依次处理

    int SendRandData();                   //随机数报文发送测试对方处理逻辑是否会崩溃
    int getSocketID(){return socketID;}
public slots:
    void slot_DataReceived();      
    void slot_Connected();      //服务端socket的连接由Server框架管理,实际无用，暂留
    void slot_Disconnected();
    void slot_CMDfromUI(int SocketID,int cmdtype,QByteArray ba);   //	处理UI发过来的命令
    void Error(QAbstractSocket::SocketError socketError);
public:
    void do_sendData();                     //发送测试数据函数
    void do_sendJoyStickData();         //虚拟发送操纵杆的坐标数据
    void do_test();                              //测试函数
    void sendDataToUI(char* buf,unsigned int len);
//其他针对不同应用程序的扩展
private:

public:
//与panner
  void process_SetMetadata(char* buf);      //收到panner发过来的SetMetadata
  void do_SetMetadataResp();                //返回一个确认收到SetMetadata命令
  void Resp_RandData(char *buf, unsigned int len);   //随机测试报文


public:
  //提供一个服务器与客户线程访问共享数据的管理类接口,具体用法见SharaDataMgr说明
  SharaDataMgr& getSharaDataMgr();

protected:

};


QtTcpSrvResponderBase* CreateTcpSrvResponderBase(unsigned short socketDescriptor,QtTcpSrvFrame* parent=0);


typedef  QtTcpSrvResponderBase* (*pMsgResponder)(unsigned short socketDescriptor,QtTcpSrvFrame* parent);

#endif 

