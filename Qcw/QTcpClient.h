#ifndef QTCPCLIENT_H
#define QTCPCLIENT_H

#include <QThread>
#include <QtNetwork>
 
#include "MessageRegistrar.h"   //用StuMessage结构


class QTimer;
class QTcpSocket;
class QTcpClient: public QThread
{
    Q_OBJECT
public:
    explicit QTcpClient(QObject *parent = 0);
    ~QTcpClient();
    enum NetState
    {
        NetState_Unknow=0,         //初始化未知状态
        NetState_Connecting,        //正在连接
        NetState_Connected,         //已经连接
        NetState_DisConnected,     //有socket断开
        NetState_Total
    };
    enum SOCKET_CTRL
    {
        SK_CTRL_Begin=NetState_Total,                  //网络状态同时作控制为信号类型枚举。在NetState基础上递增
        SK_DataToSocket,                                       //帮助外部/UI,发送数据
        SK_DataFromSocket,
        //SK_DISCONNECT_FROM_HOST,                //让socket主动断开
        //SK_RECONNECT,                                      //重新连接网络,
    };
public:
    void run();
    void stop();
    void timerEvent(QTimerEvent * te);
    void printName();                                          //打印线程名字，方便多线程调试    //发布时可以删除
    void setServerIPPort(const std::string& sIP,unsigned short port);     //设置IP与端口
    void setAutoConnect(bool flag,unsigned int tm);                             //设置断开重连  //time in milliseconds
	std::string getIp() const{ return sServerIP;}                    //初始化设定的ip
	unsigned short getPort() const{return this->m_port;}    //初始化设定的端口
    std::string getPeerIP()const;  //QTcpSocket内部维护的IP
    //void setClientID(int Id) {ClientID = Id;}          //同时连接多个网络时，互相区分
public:
    QTcpSocket& getQTcpSocket();    //提供外部直接访问QTcpSocket 接口
    std::string getPeerName() const;
    unsigned short getPeerPort() const;
    NetState getNetState()const {return m_NetStatus;}
private:
    QTcpSocket *pTcpSocket;            //
private:
    //int ClientID;                               //同时连接多个网络时，互相区分
    std::string  sThisNane;                 //存放当前线程名字,printf打印出来，当有多个线程的不会混乱
    bool bAutoConnectEnable;           //设置断开自动重连
    unsigned int AutoConnectTime;   //设置断开自动重连
    unsigned int AutoConnectCnt; 
    NetState m_NetStatus;
    unsigned int TimeToDo;
    QTimer* pTimerToDo;
    uint64_t NumDoneCnt;
    std::string sServerIP;             //服务器IP
    unsigned short m_port;             //初始化时使用的端口号
public: 
    //读数据方法1: 并处理相关函数改写--保证可靠读取数据
    enum 
    {
        MsgBufferSize=1*1024*1024
    };
    char bufStack[MsgBufferSize];                 //小报文数据缓存
    unsigned int  RecvDataProcess();             //读取网络缓存的数据并处理,返回读取到的字节数，可以由信号触发或手动调用
    bool WaitForData(int msecs=30000);       //等待数据到来,30s超时
    bool WaitByDataSize(unsigned int size);   //根据数据大小决定等待时长
    void  ClearSocketBuffer();                //自定义清空Socket读缓存
    unsigned int  ReadNbytes(char* buf,unsigned int size);  //读取N个字节  改int seconds); //读N个字节 设置超时退出
    int  FilterProcess1(char* buf,unsigned int size,bool isNew=false);  //根据命令类型依次处理

#if 0
    //读数据方法2；思路就是每次读取所有有效数据，存到大缓冲区如100M,地址累加偏移,然后while循环处理所有命令
    //思路二有bug,暂时不好解决,如果此时head刚好一部分在buf尾部一部分在头部，非常不好处理。需要加特殊考虑，待续
    enum 
    {
        BufAllSize=10*1024*1024  //总的数据缓存大小
    };
    char pBufBig[BufAllSize];      //没必要new,因为当前类一般new来使用
    char* pBufBegin;           //数据缓存起始地址
    char* pBufEnd;              //数据缓存结束地址
    char* pUnProcBegin;     //未处理报文起始地址
    char* pUnProcEnd;        //未处理报文结束地址
    void InitReadMethod2();              //初始化以上变量
    void  RecvAllDataToProc();                                   //一次获取所有数据
    unsigned int  getRemainBufSize() const;              //获取剩余缓缓存空间大小
    unsigned int  getUnProcBufSize() const;              //获取未处理有效报文空间大小
    unsigned int  ReadDatatoBuf(unsigned int readSize);     //根据数据大小读数据到缓存，因为可能尾部地址溢出，要分段读取
    unsigned int  ReadNBytesWithBlock(char* buf,unsigned int size);  //直接读取N个字节,不等待
    void  doProcess();
    //两种读取方法共同调用报文处理函数
    int  FilterProcess2(char* buf,unsigned int size,bool isNew=false);      //根据命令类型依次处理
#endif

    int SendRandData();               //随机数报文发送测试对方处理逻辑是否会崩溃
    void InitSocket();                   //初始化socket,绑定服务器与端口;
    void printNetState();              //打印socket的状态
    void NotifyNetStateChanged(NetState state);
    int SendData(const char* buf,int size); 
public slots:
    void slot_timeToConnect();                      //定时器测试--断开重连
    void slot_timeToDoSomeThing();
    void slot_DataReceived();
    void slot_Connected();
    void slot_Disconnected();
    void slot_Error(QAbstractSocket::SocketError socketError);
    void slot_stateChanged(QAbstractSocket::SocketState nState);
    void slot_Findhost();

//////////////////////////////////////////////////////////////////////////////////////////////////
//用信号去触发清理操作,解决跨线程关闭定时器问题
private:
    bool isClearing;
signals:
    void sig_Clear();
public slots:
    void slot_Clear();
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
public:
    //void SendDataFromOutSide(QByteArray& ba,char* buf,int len);
    void SendDataFromOutSide(char* buf,int len);
signals:
Q_SIGNALS:
    //与外部交互，小数据发送QByteArray,大数据发送静态內存地址与长度,isNew为true表示是new出来的buffer,用完需要delete
    //void sig_CmdToOutSide(int type,QByteArray ba=0,char* buf=0,int len=0,bool isNew=false); 
    void sig_CmdToOutSide(int type,StuMessage msgData); 
public slots:
    //void slot_CmdFromOutSide(int type,QByteArray ba,char* buf,int len,bool isNew);
    void slot_CmdFromOutSide(int type,StuMessage msgData);
    //用自定义结构替代QByteArray ba


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
////TEST 跨线程发送事件 失败
//signals:
//        void sig_send(int type,QByteArray ba,char* buf,int len,bool isNew);
//public slots:
//        void slot_Send(int type,QByteArray ba,char* buf,int len,bool isNew);
//private:
//    virtual bool event(QEvent *event);
//    virtual void customEvent(QEvent *);
//////////////////////////////////////////////////////////////////////////////////////////////////

        //其他针对不同应用程序的扩展
private:
public:
    //各种报文请求函数
    //各种报文处理函数
};






//#include <QEvent>
////自定义事件
//class MyDataEvent : public QEvent
//{
//    QString m_data;
//public:
//    const static Type TYPE = static_cast<Type>(QEvent::User + 0xFF);
//    MyDataEvent(QString data = ""):QEvent(TYPE)
//    {
//        m_data = data;
//    }
//    //virtual ~MyDataEvent(){}
//    QString& data()
//    {
//        return m_data;
//    }
//};

#endif



