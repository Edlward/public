#include <QtNetwork>
#include "QtTcpSrvFrame.h"
#include <QDebug>


QtTcpSrvFrame::QtTcpSrvFrame(int Port,QObject *parent)
    : m_port (Port)
    ,QTcpServer(parent)
    ,pCreatFun(CreateTcpSrvResponderBase)  //默认的业务处理器创建方法
{
   // this->port=port;                   //保存传递进来的端口，
    ::printf("CTcpServer::CTcpServer()\n");

    //设置默认的业务处理器创建方法
    //setResponderCreatFun(CreateTcpSrvResponderBase);
}


QtTcpSrvFrame::~QtTcpSrvFrame()
{
    this->close();
}

void QtTcpSrvFrame::printf(const char* fmt, ...)
{
    char buf[1024]={0};
    va_list args;                         //定义一个va_list类型的变量，用来储存单个参数  
    va_start(args,fmt);                //使args指向可变参数的第一个参数  
    //直接打印原字符串
    //::printf(fmt, args);              //不完全正确，%d打印整形出错
    //::printf(fmt,*args);             //正确
    ::vprintf(fmt,args);               //正确--标准用法

    //取得打印后字符串buf
    //::sprintf(buf,fmt,args);       //不完全正确，%d打印整形出错
    //::sprintf(buf,fmt,*args);      //正确，直接封装sprintf必须自己解释Fmt参数中的参数个数和类型，否则无法提取参数？
    ::vsprintf(buf,fmt,args);        ///正确--标准用法   必须用vprintf等带v的    ::sprintf(buf,fmt,...);  vprintf(cmd,args);
    va_end(args);                      //结束可变参数的获取 

    // ::printf("New:%s\n",buf);
    QString sMsg(buf);
    emit sig_PrintMsg(sMsg);   		 //得到字符串发送给UI显示
}


bool QtTcpSrvFrame::OpenServer()
{
    //再改不一定成功的操作，最好不要放在构造函数里面
    //可以制定监听某个IP 与端口
    //QT5 默认监听IPV6地址
#if (QT_VERSION>0x050000)                  //必需包含QtCore  才能找到此宏定义
    if(!listen(QHostAddress::AnyIPv4,m_port))   //QHostAddress::Any,port  QHostAddress::AnyIPv4
#else
    if(!listen(QHostAddress::Any,port))   //QHostAddress::Any,port  QHostAddress::AnyIPv4
#endif
    {
        this->close();
        ::printf("CTcpServer::CTcpServer():listen error\n");
        return false;
    }

    setMaxPendingConnections(300);
    //设定待处理的连接最大数目为(默认30),当超过了最大连接数后，客户端仍旧可以连接服务端，但是服务端不在接受连接，操作系统会把这些链接保存在一个队列中。
    return true;
}


//新的连接到来

#if (QT_VERSION>0x050000)                  //必需包含QtCore  才能找到此宏定义
void QtTcpSrvFrame::incomingConnection(qintptr socketDescriptor)      //QT4与QT5此函数定义不一样，具体版本不确定
#else
void QtTcpSrvFrame::incomingConnection(int socketDescriptor)
#endif
{
    //printf("CTcpServer::incomingConnection()\n");

    //继承自QTcpSocket()
    //TcpSrvResponderBase *pSocket = new TcpSrvResponderBase(socketDescriptor);  //this,

    //进一步抽象
    QtTcpSrvResponderBase *pSocket=pCreatFun(socketDescriptor,this);
    QThread* pThread=new QThread(pSocket);


    //可以进一步修改每来多少个请求才创建一个线程
    //static int NumThread=0;
    //经过测试localPort()才是server的端口号，peerport是个随机数




    connect(pSocket,SIGNAL(sig_updateClients(QString,int)),this,SLOT(slot_updateClients(QString,int)));

    connect(pSocket,SIGNAL(sig_DisConnect(int,QtTcpSrvResponderBase*)),this,SLOT(slot_DisConnect(int,QtTcpSrvResponderBase*)));

    //信号关联信号
    //关联CTcpServer中转的信号,操作socket线程 
    connect(this,SIGNAL(sig_CMD_UItoSocket(int,int,QByteArray)),pSocket,SLOT(slot_CMDfromUI(int,int,QByteArray)));


    //关联CTcpServer中转的信号  中转来着socket线程的信号, 交给UI处理
    connect(pSocket,SIGNAL(sig_CMD_SocketToUI(int,int,int,QByteArray)),this,SIGNAL(sig_CMD_SocketToUI(int,int,int,QByteArray)));

    //发送打印信息给UI显示
    connect(pSocket,SIGNAL(sig_PrintMsg(QString)),this,SIGNAL(sig_PrintMsg(QString)));


    //要想moveToThread,对象创建时不能有父指针
    //TcpServerSocket->setParent(NULL);                       //有parent的object是不能被移动到其他线程中的
    pSocket->moveToThread(pThread);               //把tcp类移动到新的线程
    pThread->start();//线程开始运行

    //保存连接请求与线程指针的对应关系，方便在断开连接时候删除对应线程指针
    SrvSocketInfo ClientSocketinfo;
    ClientSocketinfo.pSocket=pSocket;
    ClientSocketinfo.SocketID=socketDescriptor;
    ClientSocketinfo.pThread=pThread;
     ListClientSocketInfo.push_back(ClientSocketinfo);


     std::string sPeerIP=pSocket->peerAddress().toString().toStdString();
     std::string sPeerName=pSocket->peerName().toStdString();
     short peerPort=pSocket->peerPort();

     std::string sLocalIP=pSocket->localAddress().toString().toStdString();
     short LocalPort=pSocket->localPort();

     printf("New client coming:Peer:IP:%s port:%d Name:%s Local: IP:%s port:%d Count:%d\n",sPeerIP.c_str(),peerPort,sPeerName.c_str(),sLocalIP.c_str(),LocalPort,(int)ListClientSocketInfo.size());

     //printf("ListClientSocketInfo.size()=%d\n",ListClientSocketInfo.size());

    //在此可以更新登录的IP地址列表
    //改成认证请求成功后,在UI里面显示客户端列表
    //emit signal_clientChange();   //客户端有增减,断开或新连接,发送信号，通知界面,刷新客户端信息列表
    emit sig_CMD_TcpServertoUI(socketDescriptor,SrvSocket_NEWCOMMING); //服务器发给UI
}


void QtTcpSrvFrame::slot_updateClients(QString msg,int length)
{

}

void QtTcpSrvFrame::slot_ReadData(int socketID)
{
    emit sig_DataReady(socketID);
}

void QtTcpSrvFrame::slot_DisConnect(int SocketID,QtTcpSrvResponderBase *pTcpServerSocket)
{
       std::vector<SrvSocketInfo>::iterator it;
       for(it=ListClientSocketInfo.begin();it!=ListClientSocketInfo.end();)
      //for (int i=0;i<ListClientSocketINFO.size();i++)
    {
        //const SrvSocketInfo& ClientSocketinfo=ListClientSocketINFO.at(i);
        SrvSocketInfo socketInfo=(*it);

        if (socketInfo.SocketID==SocketID)
        {
            QThread* pthread=socketInfo.pThread;

            //断开之前关联的所有信号  通常这不是必须的，因为在对象删除时Qt会自动删除对象中的所有连接。
            //disconnect
            QtTcpSrvResponderBase *pTcpServerSocket=socketInfo.pSocket;
            disconnect(pTcpServerSocket,SIGNAL(sig_updateClients(QString,int)),this,SLOT(slot_updateClients(QString,int)));
            disconnect(pTcpServerSocket,SIGNAL(sig_DisConnect(int,QtTcpSrvResponderBase*)),this,SLOT(slot_DisConnect(int,QtTcpSrvResponderBase*)));
            disconnect(this,SIGNAL(sig_CMD_UItoSocket(int,int,QByteArray)),pTcpServerSocket,SLOT(slot_CMDfromUI(int,int,QByteArray)));
            disconnect(pTcpServerSocket,SIGNAL(sig_CMD_SocketToUI(int,int,int,QByteArray)),this,SIGNAL(sig_CMD_SocketToUI(int,int,int,QByteArray)));


           // 据说是因为对象可能在事件循环中存在 不能直接删 而是往消息循环中投递一个消息安全的删除
           //不然假如还有针对那对象的消息而对象被销毁了的话程序就崩溃了
            //方法1
//            pthread->exit();    //quit()内部调用exit();所以是一样的 void QThread::quit(){ exit(); }
//            pthread->deleteLater();
            //delete pthread;                    //连接断开,释放对应线程
            //方法2
            pthread->exit();
            pthread->wait(1000);
            delete pthread;

            //释放socket类
            delete pTcpServerSocket;    //放在这里正常

            printf("delete Qthread of cleint=%X\n",pthread);
            //ListClientSocketINFO.removeAt(i);    //从列表中移除该连接
           // ListClientSocketINFO.remove(i);    //从列表中移除该连接

            ListClientSocketInfo.erase(it);
            ++it;

            break;
        }
        else
        {
            ++it;
        }
    }

    //客户端有增减,断开或新连接,发送信号，通知界面,刷新客户端信息列表
    emit sig_CMD_TcpServertoUI(SocketID,SrvSocket_DISCONNECT); //服务器发给UI
}


void QtTcpSrvFrame::startPollingMsg()
{
    connect(&tmtodo,SIGNAL(timeout()),this,SLOT(slot_timetoDo()));
    tmtodo.start(20);
}


void QtTcpSrvFrame::stopPollingMsg()
{
    tmtodo.stop();
}

void QtTcpSrvFrame::slot_timetoDo()
{
    Message msg;
    if (!getMsgQueue()->getMsg(msg,0U))
    {
        return;
    }

    //收到外部模块消息,通过信号槽转发给socket,遍历所有socket发送

    QByteArray ba((char*)&msg,sizeof(msg));
    for(int i=0;i<ListClientSocketInfo.size();i++)
    {
        const SrvSocketInfo& ClientInfo=ListClientSocketInfo.at(i);
        if(ClientInfo.pSocket!=NULL)
        {
                int SocketID=ClientInfo.SocketID;
                sig_CMD_UItoSocket(SocketID,UI_MsgQueueData,ba);
        }
    }
}

