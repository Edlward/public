#include "QTcpClient.h"

#include "../Proj/ProtocolNetHead.h"      //网络通信协议数据报文头

#include "Super/Basic/global.h"

static void print_hex(const void* buf,int len)
{
    for(int i=0;i<len;i++)
    {
        printf("%02X ",(unsigned char)((char*)buf)[i]);
        if(15 == i%16)
            printf("\n");
    }
    printf("\n");
}


//注意理解与使用--为了线程外部关闭可以快速响应退出
//QThread::requestInterruption(); // 请求终止线程
// 耗时操作里判断while (!isInterruptionRequested()) 可以优雅退出线程



#include <QMetaType>
QTcpClient::QTcpClient(QObject *parent /*= 0*/) : QThread(parent)
{
    qDebug()<<"QTcpClient::QTcpClient() threadID:"<<QThread::currentThreadId(); 
    sThisNane="QThread TCP Client";
    pTcpSocket=NULL;
    pTimerToDo=NULL;
    TimeToDo=1000/24;
    NumDoneCnt=0;
    m_NetStatus=NetState_Unknow;
    isClearing=false;
    setAutoConnect(true,4000);
    setServerIPPort("127.0.0.1",8080);                             //初始化IP与端口

    qRegisterMetaType<StuMessage>("StuMessage");   //注册自定义消息类型
    //InitRead();                   //报文接收缓存地址初始化,读方式2需要暂时屏蔽
    moveToThread(this);     //调用这句,UI主线程发送信号，对应的槽函数才会在网络线程运行
}

QTcpClient::~QTcpClient()
{
    PRINT_FUN_TRACE
    stop();
}

void QTcpClient::stop()
{
    PRINT_FUN_TRACE

    //解决了警告，QObject::killTimer: Timers cannot be stopped
#if 1
    qDebug()<<"QTcpClient::stop() begin() threadID:"<<QThread::currentThreadId(); 
    if (isRunning())   //在运行用信号去触发,否则直接调用槽函数,避免跨线程关闭定时器警告
    {
        printf("emit sig_Clear()\n");
        emit sig_Clear(); 
    }
    else
    {
        slot_Clear();
    }

    while(isRunning())   //阻塞等待退出
    {
        //quit();  //不能在此退出,等待槽函数有机会运行,否则重连失败
        wait(100);
    }
    return;
#endif


#if 0
     qDebug()<<"QTcpClient::stop() begin() threadID:"<<QThread::currentThreadId(); 
     if (!isRunning())   //避免不在运行时连续调用stop()，导致重复调用slot_Close()
     {
         if(pTcpSocket!=NULL)         
         {	 
             delete pTcpSocket; 
             pTcpSocket=NULL;
         }
         if(pTimerToDo!=NULL)
         {
             delete pTimerToDo;
             pTimerToDo=NULL;
         }
         printf("QTcpClient::stop()--End !isRunning() return!\n");
         return;
     }

     isClearing=true;
     printf("QTcpClient::stop() emit sig_Clear()\n");
     emit sig_Clear();  //本函数将被主线程直接调用 不能直接跨线程关闭定时器等--先停止线程就安全了？
     //wait(100);             

     unsigned int cycNum=0;
     while (isClearing)  //等待关闭槽函数执行完成  
     {
         //quit();        //退出线程
         wait(10); 
         if (cycNum++>10)  //若窗体模块先执行qApp->quit(), 槽函数没有机会执行，会阻塞在此不退出，最多十次直接退出
         {
             break;
         }
     }

    while(isRunning())
    {
        printf("while(isRunning())\n");
        quit();        //退出线程
        wait(20);    //阻塞等待，  wait();  //无限等待
        //QCoreApplication::processEvents();
        //pTcpClient->deleteLater();
    }
#endif
   qDebug()<<"QTcpClient::stop() end()";
}


void QTcpClient::slot_Clear()
{
    PRINT_FUN_TRACE

    // 请求终 while (!isInterruptionRequested())  判断是否终止
    QThread::requestInterruption(); //不再继续运行

     isClearing=true;
     qDebug()<<"QTcpClient::slot_Close() threadID:"<<QThread::currentThreadId(); 
     if(pTcpSocket!=NULL)         
     {	 
         //disconnect(pTcpSocket,SIGNAL(connected()),this,SLOT(slot_Connected()));
         //disconnect(pTcpSocket,SIGNAL(disconnected()),this,SLOT(slot_Disconnected()));
         //disconnect(pTcpSocket,SIGNAL(readyRead()),this, SLOT(slot_DataReceived()));
         
         pTcpSocket->abort();
         pTcpSocket->close();         
         //pTcpSocket->deleteLater();
         delete pTcpSocket;
         pTcpSocket=NULL;
     }

     if(pTimerToDo!=NULL)
     {
         //理论上一个类的清理操作应该在析构函数考虑未关闭的情况，而不是由外部控制
         if(pTimerToDo->isActive())
         {
             pTimerToDo->stop();
         }
         //pTimerToDo->deleteLater();
         delete  pTimerToDo;
         pTimerToDo=NULL;
     }

     this->QThread::quit();

     //while(this->QThread::isRunning())   //阻塞等待退出
     //{
     //    this->QThread::quit();
     //    //quit();
     //    //wait(20);
     //}

    isClearing=false;
}


void QTcpClient::run()
{
    PRINT_FUN_TRACE
    PRINT_FUN_NAME
    printName() ; 
    qDebug()<<"QTcpClient::run()--Begin() threadID:"<<QThread::currentThreadId(); 
    pTcpSocket = new QTcpSocket();

    connect(pTcpSocket,SIGNAL(hostFound()),this,SLOT(slot_Findhost()),Qt::DirectConnection);     //先查看host是否存在
    connect(pTcpSocket,SIGNAL(connected()),this,SLOT(slot_Connected()),Qt::DirectConnection);
    connect(pTcpSocket,SIGNAL(disconnected()),this,SLOT(slot_Disconnected()),Qt::DirectConnection);
    connect(pTcpSocket,SIGNAL(readyRead()),this, SLOT(slot_DataReceived()),Qt::DirectConnection);

    //connect(m_pClient,SIGNAL(disconnected()),this,SLOT(deleteLater())); //避免出现内存泄漏,new出来的对象才需要

    int type1=qRegisterMetaType<QAbstractSocket::SocketError>("SocketError");
    connect(pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(slot_Error(QAbstractSocket::SocketError)),Qt::DirectConnection);

        int type2=qRegisterMetaType<QAbstractSocket::SocketState>("SocketState");
    connect(pTcpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
        this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)),Qt::DirectConnection);


    //if (AutoConnect)
    //{
    //    pTimerToConnect = new QTimer();      //new QTimer(this)会错误;
    //    connect(pTimerToConnect,SIGNAL(timeout()),this,SLOT(timer_toConnect()),Qt::DirectConnection);
    //    pTimerToConnect->start(AutoConnectTime);
    //}

    pTimerToDo= new QTimer();
    connect(pTimerToDo,SIGNAL(timeout()),this,SLOT(slot_timeToDoSomeThing()),Qt::DirectConnection);
    pTimerToDo->setInterval(TimeToDo);
    
    //pTcpSocket->moveToThread(this);
    //pTimerToDo->moveToThread(this);

    pTimerToDo->start();

    //TEST此法不能把跨线程信号槽转到本线程运作  
    //connect(this,SIGNAL(sig_send(int,int,QByteArray,char*,int,bool)),this,
    //    SLOT(slot_Send(int,int,QByteArray,char*,int,bool)));//DirectConnection  QueuedConnection

    connect(this,SIGNAL(sig_Clear()),this,SLOT(slot_Clear()));//DirectConnection  QueuedConnection
    InitSocket();    //首次初始化
    QThread::exec();
}

void QTcpClient::printName()
{
    printf("%s",sThisNane.c_str());
}

void QTcpClient::timerEvent(QTimerEvent * te)
{
 // printf("QTcpClient::timerEvent()\n");
 //if(te->timerId() ==m_timer_login)
    //{
    //}
}

void QTcpClient::setServerIPPort(const std::string& sIP,unsigned short port)
{
    //IP是否符合范围判断
    //端口范围判断
    this->sServerIP=sIP;
    this->m_port=port;
}


void QTcpClient::setAutoConnect(bool flag,unsigned int tm) //time in milliseconds
{
    bAutoConnectEnable=flag;
    AutoConnectTime=tm;
    AutoConnectCnt=AutoConnectTime/TimeToDo;
    if (AutoConnectCnt<1)   //取余分母不能为0
    {
        AutoConnectCnt=1;
    }
}

QTcpSocket& QTcpClient::getQTcpSocket()
{
    return *pTcpSocket;
}

std::string QTcpClient::getPeerName() const
{
    if (pTcpSocket!=NULL)
    {
        return pTcpSocket->peerName().toStdString();
    }
      return "";
}

std::string QTcpClient::getPeerIP()const
{
    if (pTcpSocket!=NULL)
    {
        return pTcpSocket->peerAddress().toString().toStdString();
    }
    return "0.0.0.0"; //无效ip
}

unsigned short QTcpClient::getPeerPort() const
{
    if (pTcpSocket!=NULL)
    {
        return pTcpSocket->peerPort();
    }
    return 0; //端口无效值
}

 

void QTcpClient::slot_timeToConnect()
{
    //printName() ;
    //printf("QTcpClient::slot_timeToConnect()\n");
    //printNetState();
    if(m_NetStatus!=NetState_Connected)     //如果网络没有断开,判断并不绝对可靠
    {
         InitSocket();
    }
}


void QTcpClient::slot_timeToDoSomeThing()
{
    ++NumDoneCnt;
    if (bAutoConnectEnable)
    {
        if (NumDoneCnt%AutoConnectCnt==0)
        {
             slot_timeToConnect();
        }
    }

     //定时请求两种数据
     //SendRandData();   //随机数报文发送测试对方处理逻辑是否会崩溃

    //在此发送从UI传过来的数据
    //QMutexLocker lock(&mutex);
    //if(ReadySend.isEmpty())   //如果数据为空
    //    return;

    //SendData(ReadySend.data(),ReadySend.size(),0);
    ////ReadySend.resize(0);  //会频繁分配释放内存
    //ReadySend.clear();       //仅仅把数据首地址复位，重新append
}


//打印socket的状态
void QTcpClient::printNetState()
{
    printName(); 
    if (pTcpSocket==NULL)
    {
        return;
    }

    int netStatus=pTcpSocket->state();
    switch(netStatus)
    {
        case QAbstractSocket::UnconnectedState:
        printf("SoctetState=UnconnectedState 未连接\n");
        break;
        case QAbstractSocket::HostLookupState:
        printf("SoctetState=HostLookupState	正在查找主机\n");
        break;
        case QAbstractSocket::ConnectingState:
        printf("SoctetState=ConnectingState  正在连接\n");
        break;
        case QAbstractSocket::ConnectedState:
        printf("SoctetState=ConnectedState	已经连接\n");
        break;
        case QAbstractSocket::BoundState:
        printf("SoctetState=BoundState       套接字已绑定到一个地址和端口(服务器)\n");
        break;
        case QAbstractSocket::ClosingState:
        printf("SoctetState=ClosingState	    连接已关闭(数据可能仍在等待写入)\n");
        break;
        case QAbstractSocket::ListeningState:
        printf("SoctetState=ListeningState	监听状态(内部使用)\n");
        break;
        default:  break;
    }
}


void QTcpClient::NotifyNetStateChanged(NetState state)
{
    //emit sig_CmdToOutSide(state,0,0,0);
    emit sig_CmdToOutSide(state,StuMessage());
}

void QTcpClient::InitSocket()     //初始化socket,绑定服务器与端口;
{
    PRINT_FUN_TRACE

    printName() ; 
    printNetState();
    if (pTcpSocket==NULL)
    {
        printf("Client:init_socket () Failed! pTcpSocket=NULL 初始化连接失败\n");
        return;
    }
    printf("Client:init_socket () 初始化连接 IP=%s Port=%d\n",sServerIP.c_str(),m_port);
    pTcpSocket->abort();      //立即终止连接，丢掉还未发送的数据
    pTcpSocket->close();      //关闭套接字

    QHostAddress hostAddress;

    if(!hostAddress.setAddress(QString::fromStdString(sServerIP)))
    {
        printName() ; 
        printf("Server ip address error!\n");
        return;
    }

    pTcpSocket->connectToHost (hostAddress, m_port);
    m_NetStatus=NetState_Connecting;   //正在连接
    printNetState();
    NotifyNetStateChanged(NetState_Connecting);
}


void QTcpClient::slot_Findhost()
{
    printName() ; 
    qDebug()<<"Client: Has find host";
}


void QTcpClient::slot_Error(QAbstractSocket::SocketError socketError)
{
    printName() ; 
    printNetState();
    QString string=pTcpSocket->errorString();             //显示错误信息
    qDebug()<<"QTcpClient::slot_Error():"<<string;
    //m_pClient->close();                                            //关闭连接
}


void QTcpClient::slot_stateChanged(QAbstractSocket::SocketState nState)
{
    printf("QTcpClient::slot_stateChanged()\n");
    printNetState();

    switch(nState)
    {
    case QAbstractSocket::ConnectedState:
    case QAbstractSocket::ConnectingState:
    case QAbstractSocket::ClosingState:
        //emit sigCommStateUpdate(state);
        break;
    case QAbstractSocket::UnconnectedState:
        //emit sigCommStateUpdate(state);
        break;
    case QAbstractSocket::ListeningState:
    case QAbstractSocket::BoundState:
    case QAbstractSocket::HostLookupState:
    default:
        break;
    }
}

void QTcpClient::slot_Connected()
{
    PRINT_FUN_TRACE

    m_NetStatus=NetState_Connected;
    printName() ; 
    printf("QTcpClient::slot_Connected()--已经连接\n");

    //pTimerToDo->start(1000/TimeToDoFreq);       //定时器帧率;
    //发送正在连接的信号--同时发送服务器IP
    std::string stdstrip=sServerIP;//.toStdString();
    std::string stdstrHostName=getPeerName();//pTcpSocket->peerName().toStdString();
    std::string serverInfo=stdstrip+"--"+stdstrHostName+"--";
    QByteArray ba(serverInfo.c_str(),serverInfo.size());

    NotifyNetStateChanged(NetState_Connected);

    //没有必要
    if (!pTimerToDo->isActive())
    {
            pTimerToDo->start();        //停止发送
    }
}

void QTcpClient::slot_Disconnected()
{
    PRINT_FUN_TRACE

    m_NetStatus=NetState_DisConnected;
    printName() ; 
    printf("QTcpClient::slot_Disconnected()--断开连接\n");
    printNetState();


    pTcpSocket->close();       //是否有必要 或 m_pClient->abort();

    //pTimerToDo->stop();        //停止发送数据

    //发送断开连接的信号
    NotifyNetStateChanged(NetState_DisConnected);
}


void QTcpClient::slot_DataReceived()
{
//  qDebug()<<"Qthread::slot_DataReceived() threadID:"<<QThread::currentThreadId();

         RecvDataProcess();           //方法1：永久保留
         //RecvAllDataToProc();      //方法2：存在bug,暂时没有好方法解决 20190411
}


//读数据方法1
unsigned int QTcpClient::RecvDataProcess()
{
    ////等待数据头，不够一个就退出
    //const unsigned int sizeMsgHead=sizeof(KL);      //消息头长度
    //while(pTcpSocket->bytesAvailable()<sizeMsgHead)
    //{
    //    if(!WaitForData())
    //    {
    //        printf("Wait: has no data: Quit 没有新数据 退出\n");
    //        //   ClearSocketBuffer();
    //        //   return sumRecv;
    //    }
    //}

    unsigned int sumRead=0;                    //记录本轮处理一共读取到的数据总字节数
    const unsigned int sizeMsgHead=sizeof(KL);      //消息头长度
    while(!isInterruptionRequested()&&pTcpSocket->bytesAvailable()>=sizeMsgHead)
    {
        unsigned int wantRead=sizeMsgHead;   //想要读的数据长度
        unsigned int nRead;                               //实际读到的数据长度
        char* pos=this->bufStack;
        nRead=ReadNbytes(pos,wantRead);     //读取报文头,已经判断bytesAvailable()可以肯定能读到指定长度
        sumRead+=nRead;

        //while (!isInterruptionRequested()) //注意理解此用法,为了外部随时关闭,耗时操作判断此标记

        KL* pKL = (KL*)pos;
        //若数据头不正确,循环读取并左移,直到正确
        while(!isInterruptionRequested()&&pKL->head!=KL_HEAD)
        {
            printf("Msg head Error =0X%X\n",pKL->head);
            print_hex(pos,sizeMsgHead);         //打印报文头
            char tailBytes;
            int nr=pTcpSocket->read(&tailBytes,1);
            if (nr==1)
            {
                char* p=pos;
                for(int i=0;i<sizeMsgHead-1;i++)
                {
                    p[i]=p[i+1];
                }
                p[sizeMsgHead-1]=tailBytes;  //新读到的填充到尾部
            }
            else
            {
                //
            }
        }

        unsigned int sizeMsgContent=pKL->len;                             //得到剩余消息体内容长度。
        unsigned int sizeMsgAll=sizeMsgContent+sizeMsgHead;    //整个消息长度
        bool useNewbuf=false;
        char* pBufNew=this->bufStack;
        if(sizeMsgAll>MsgBufferSize)                                  //长报文消息开辟更大内存
        {
            pBufNew=new(std::nothrow) char[sizeMsgAll];   //为了应对虚假报文攻击,用不抛异常版new
            if(pBufNew==NULL)
            {
                printf("报文缓存申请失败报文太长:%u\n",sizeMsgAll);   //必须判断如果是乱码或恶意数据可能会申请很大内存，程序崩溃
                return sumRead;
            }
            memcpy(pBufNew,this->bufStack,sizeMsgHead);           //拷贝报文头到新的地址
            useNewbuf=true;
        }
        wantRead=sizeMsgContent;
        nRead=ReadNbytes(pBufNew+sizeMsgHead,wantRead);   //读剩余的字节
        sumRead+=nRead;
        if(nRead!=wantRead)   //读不到数据不完整
        {
            return sumRead;   
        }

        FilterProcess1(pBufNew,sizeMsgAll);      
                                                        //依次处理各种命令
        //printf("Read:\n");
        //print_hex(pBufNew,sizeMsgAll);


        //大报文使用了新的堆内存，释放--改到处理完在释放？  不行，槽函数在新的线程，可能还没有开始处理又会被覆盖
        if(useNewbuf)
        {
            delete[] pBufNew;                                     
        }
    }
    return sumRead;
}

int QTcpClient::FilterProcess1(char* buf,unsigned int size,bool isNew/*=false*/)
{
    //读数据方法1  用QByteArray封装数据借助隐式共享效率不会太低  buf 可能是new出来的传到ui线程时可能已经被delete，用Qbytearary传递
    //emit sig_CmdToOutSide(SK_DataFromSocket,QByteArray(buf,size),0,0,false);    //其他  命令直接发送给UI处理

    //用自定义结构替代QByteArray
    emit  sig_CmdToOutSide(SK_DataFromSocket,StuMessage(buf,size));

    //读数据方法2
    //emit sig_CmdToOutSide(SK_DataFromSocket,0,buf,size,isNew);    //其他  命令直接发送给UI处理

    //KL* pKL = (KL*)buf;
    //switch(pKL->key)
    //{
    //    //case CMD_WINLOG_LOG_RESP:                             //返回日志内容RESPONSE
    //    //	 break;     
    //default:
    //    {	
    //        break;
    //    }
    //    //printf("Client: 未知报文 Unknow CMD:pKH->key=%X 长度=%d\n",pKL->key,pKL->len);
    //    //print_hex(buf,sizeof(KL));
    //    //break;
    //    //}
    //}
    return 0;
}


bool QTcpClient::WaitForData(int msecs)
{
    //isblocking=true;
    bool ret=pTcpSocket->waitForReadyRead(msecs);
    //isblocking=false;
    return ret;
}

bool QTcpClient::WaitByDataSize(unsigned int size)
{
    //int waitTime=size/8+3000;    //根据数据大小决定等待时间,按最低约10kb/s的速率计算延时
    int waitTime=size+30000;        //根据数据大小决定等待时间,按最低约1kb/s的速率计算延时
    bool ret=pTcpSocket->waitForReadyRead(waitTime);
    if (!ret)
    {
        printf("QTcpClient::WaitByDataSize() waitForReadyRead() failed! size:%u\n",size);
    }

    return ret;
}

void QTcpClient::ClearSocketBuffer()
{
    //while(pTcpSocket->bytesAvailable()>0)
    //{
    //    printf("QTcpClient::ClearSocketBuffer() While read to Clear Socket Buf!\n");
    //    char buf[1024];
    //    pTcpSocket->read(buf,sizeof(buf));
    // }
    if (pTcpSocket->bytesAvailable()>0)
    {
        printf("QTcpClient::ClearSocketBuffer() read to Clear Socket Buf size:%lld ",pTcpSocket->bytesAvailable());
        QByteArray da=pTcpSocket->readAll();
        printf("after read size:%lld ",pTcpSocket->bytesAvailable());
    }
    return;
}

unsigned int QTcpClient::ReadNbytes(char *buf,unsigned int size)
{
    unsigned int sumRead=0;
    int64_t nRemainSize=size;             //想要读取的数据大小
    char* pos=buf;                                  //缓存地址
    while(!isInterruptionRequested()&&nRemainSize>0)                       //读取完整大小的方法
    {
        unsigned int nRead=pTcpSocket->read(pos,nRemainSize);   
        sumRead+=nRead;                //累加到读取的总数据
        if(nRead!=nRemainSize)        //读的数据不完整,等待
        {
            if(!WaitForData())
            //if(!WaitByDataSize(nRemainSize-nRead))   //根据剩余数据决定等待时长
            {
                return sumRead;
            }
        }
        pos+=nRead;
        nRemainSize-=nRead;
    }
    return sumRead;
}





#if 0

//读数据方法2
void QTcpClient::InitReadMethod2()
{
    printf("BufAllSize=%d\n",BufAllSize);
    pBufBegin=this->pBufBig;                       //数据缓存起始地址
    pBufEnd=this->pBufBig+BufAllSize-1;     //数据缓存结束地址 需要减1,否则越界
    pUnProcBegin=this->pBufBig;                 //未处理报文起始地址
    pUnProcEnd=this->pBufBig;                    //未处理报文结束地址
}


void QTcpClient::RecvAllDataToProc()
{
    unsigned int SocketDataSize=pTcpSocket->bytesAvailable();
    if(SocketDataSize==0)   return;                    //没有数据直接返回，基本不会

    unsigned int readSize=SocketDataSize;
    unsigned int  BufRemainSize=getRemainBufSize();
    if(SocketDataSize>BufRemainSize)                //数据大于剩余空间
    {
        readSize=BufRemainSize;                          //只读取剩余能容纳的
    }
    ReadDatatoBuf(readSize);
    //读完一次
    doProcess();    //依次处理各个命令
}


unsigned int QTcpClient::getRemainBufSize() const
{
    return this->BufAllSize-getUnProcBufSize();        //正在使用报文未处理完的空间
    //posEnd=posBegin表示空的情况,所以总容量需要-1
    //return this->BufAllSize-1-getUnProcBufSize();       //正在使用报文未处理完的空间
}

unsigned int QTcpClient::getUnProcBufSize() const //环形缓冲区中未处理数据大小
{
    if(pUnProcEnd>=pUnProcBegin)
    {
        return pUnProcEnd-pUnProcBegin;    //int BufInUseSize=0;
    }
    return (pBufEnd-pUnProcBegin)+(pUnProcEnd-pBufBegin);
}




unsigned int QTcpClient::ReadDatatoBuf(unsigned int readSize)
{
    //特殊情况pEnd=pStart,数据是空 数据满?
    unsigned int SumRead=0;
    if(pUnProcEnd>=pUnProcBegin)
    {
        unsigned int sizeToTail=pBufEnd-pUnProcEnd;            //到缓存尾部的剩余大小
        if(sizeToTail>=readSize)                                              //剩余尾部够装
        {
           unsigned int nRead=ReadNBytesWithBlock(pUnProcEnd,readSize);
           pUnProcEnd+=nRead;
           SumRead+=nRead;
        }
        else        //分段读取
        {
           unsigned int nRead=ReadNBytesWithBlock(pUnProcEnd,sizeToTail);   //先读部分到尾部
           pUnProcEnd+=nRead;
           SumRead+=nRead;
           unsigned int RemainSize=readSize-sizeToTail;
           nRead=ReadNBytesWithBlock(pBufBegin,RemainSize);  //再读剩余放到最前
           pUnProcEnd=pBufBegin+nRead;                                  //报文尾部地址到前面了
           SumRead+=nRead;
        }
    }
    else
    {
        unsigned int sizeToTail=pUnProcBegin-pUnProcEnd;     //到缓存尾部的剩余大小
        if(sizeToTail>=readSize)                                                //剩余尾部够装
        {
           int nread=ReadNBytesWithBlock(pUnProcEnd,readSize);
           pUnProcEnd+=nread;
           SumRead+=nread;
        }
        else{} //不可能 之前已经判断
    }
    return SumRead;
}

unsigned int QTcpClient::ReadNBytesWithBlock(char *buf,unsigned int size)
{
    unsigned int sumRead=0;
    unsigned int nRemainSize=size;             //想要读取的数据大小
    char* pos=buf;                        //缓存地址
    while(!isInterruptionRequested()&&nRemainSize>0)            //因为之前已经判断有数据，这里应该能完全读取
    {
        int nRead=pTcpSocket->read(pos,nRemainSize);   //读取完整大小的方法,阻塞
        sumRead+=nRead;                                                //累加到读取的总数据
        pos+=nRead;
        nRemainSize-=nRead;
    }
    return sumRead;
}


void QTcpClient::doProcess()
{  
    while(!isInterruptionRequested())
    {
        unsigned int SizeHead=sizeof(KL);
        if(getUnProcBufSize()<SizeHead)    //如果未处理的有效数据小于一个报文头
        {
            return;
        }

        KL* pKL = (KL*)pUnProcBegin;               //开始处理数据
        if(pKL->head!=KL_HEAD)                      //报文头不正确
        {
            printf("Client: pKL->head Error =%X\n",pKL->head);
            print_hex(pUnProcBegin,SizeHead);         //打印报文头
        }

        while(1)                           //直到直到正确报文头--在这里可以计算丢包
        {
            if(pKL->head==KL_HEAD)         //报文头正确
              break;


            //思路二有bug,暂时不好解决,如果此时head刚好一部分在buf尾部一部分在头部，非常不好处理。

            //有bug,可能越界不好处理
            pUnProcBegin++;
            if(pUnProcBegin>pBufEnd)             //搜寻到尾部
            {
              pUnProcBegin=pBufBegin;
            }

            if(pUnProcBegin==pUnProcEnd)              //有效数据用完
            {
                break;
            }

            pKL = (KL*)pUnProcBegin;                 //重新映射指针必需的
        }
        if(getUnProcBufSize()<SizeHead)    //如果未处理的有效数据小于一个报文头
        {
            return;
        }

        unsigned int SizePyloadContent=pKL->len;            //
        if(pKL->len>BufAllSize)  //100*1024*100
        {
            //特殊超大报文处理--如大文件等一次不可能传送完成
            //先取剩余数据，然后接管套接字读取剩余数据
        }

        unsigned int SizeUnProc=getUnProcBufSize();                //未处理报文大小
        unsigned int SizePyload=SizePyloadContent+SizeHead; //本次报文大小
        if(SizePyload>SizeUnProc)    //如果报文不完整就直接退出，等待下次处理
        {
            break;
        }

        char* buf=pUnProcBegin;
        unsigned int tailsize=(pBufEnd-pUnProcBegin);     //尾部剩余
        bool useHeapbuf=false;
        if(SizePyload>tailsize)                //报文可能地址不连续，前面一段后面一段    //拷贝到新空间
        {
            unsigned int requestSize=SizePyload;
            if(requestSize>this->BufAllSize)  //超大报文,到处理函数里面接收剩余数据
            {
                requestSize=this->BufAllSize;
            }

            //方式1:
            //buf=new(std::nothrow) char[requestSize];        //肯定会成功
            //useHeapbuf=true;
//            if(NULL==buf)
//            {
//                printf("报文太长 内存不足\n");
//                return;
//            }



            //方式2:
            //buf=buftmp;  //拷贝到另一块内存
            //memcpy(buf,pUnProcBegin,tailsize);
            //memcpy(buf+tailsize,pBufBegin,(requestSize-tailsize));

            printf("报文递增不连续,重新移位\n");

            //待确认
            char* buftmp=new char[requestSize-tailsize];
            memcpy(buftmp,pBufBegin,requestSize-tailsize);
            memcpy(buf,pUnProcBegin,tailsize);
            memcpy(buf+tailsize,buftmp,requestSize-tailsize);
            delete[] buftmp;

        }

        FilterProcess2(buf,SizePyload,useHeapbuf);    //处理单个报文

        if(useHeapbuf==true)       //如果使用了堆内存
        {
           // delete[] buf;    //改 ui线程使用完再delete
        }

        //已经处理过地址往后移动
        char* pStartNew=pUnProcBegin+SizePyload;
        if(pStartNew>pBufEnd)
        {
            pStartNew=pBufBegin+(SizePyload-(pBufEnd-pUnProcBegin));
        }
        pUnProcBegin=pStartNew;
    }
}


//处理单个报文
int QTcpClient::FilterProcess2(char *buf,unsigned int size,bool isNew)
{
    KL* pKL = (KL*)buf;
    //printf("Client: 处理报文 key=%X 长度=%d\n",pKL->key,pKL->len);
    //得到完整数据包,可以在此调用解密函数,然后做CRC校验，
    //根据加密类型调用调用相应的解密密钥
    //根据命令类型调用相应处理函数
  

    //再改进--虚假报文头攻击，报文长度很大，没有实际数据
    if(size>(this->BufAllSize))   //超大报文如文件传送，在处理函数里面接收剩余数据
    {
        printf("Client: 超大报文 key=%X 长度=%d\n",pKL->key,pKL->len);
        //如不处理在这里读取剩余大报文数据
        unsigned int remainSize=size-this->BufAllSize;
        int64_t nread=remainSize;
        while(!isInterruptionRequested()&&nread>0)
        {
            char tmp[2];
            int i=pTcpSocket->read(tmp,1);
            if(i==0)
            {
                //isblocking=true;
                bool ret=pTcpSocket->waitForReadyRead(1000);  //30000
                //isblocking=false;
            }
            nread-=i;
        }
        switch(pKL->key)
        {
        }
        return 0;
    }

    switch(pKL->key)
    {
        //case CMD_WINLOG_LOG_RESP:                             //返回日志内容RESPONSE
        //	 break;     
        default:
               {	
                   //读数据方法1  用QByteArray封装数据借助隐式共享效率不会太低
                   //emit sig_CmdToOutSide(SK_DataFromSocket,QByteArray(buf,size),0,0,false);    //其他  命令直接发送给UI处理

                   emit sig_CmdToOutSide(SK_DataFromSocket,StuMessage(buf,size));    

                   //读数据方法2
                   //emit sig_CmdToOutSide(SK_DataFromSocket,0,buf,size,isNew);    //其他  命令直接发送给UI处理

                    //buf 可能是new出来的传到ui线程时可能已经被delete，用Qbytearary传递
                    //QByteArray ba(buf,size);
                    //emit sig_CmdToUI(SK_DATA_FROM_SOCKET,ClientID,ba,0,0); 
                    break;
                 }

                //printf("Client: 未知报文 Unknow CMD:pKH->key=%X 长度=%d\n",pKL->key,pKL->len);
                //print_hex(buf,sizeof(KL));
                //break;
               //}
    }
     return 0;
}
#endif



#include<time.h>
int QTcpClient::SendRandData()   //随机数报文发送测试对方处理逻辑是否会崩溃
{
    char buf[1024];
    srand(time(NULL));
    for(int i=0;i<sizeof(buf);i++)
    {
        buf[i]=rand()%256;
    }
    SendData(buf,sizeof(buf));
    return 0;
}


int QTcpClient::SendData(const char* pBuf,int size)
{
    //Socket没有初始化或网络状态不正确,直接返回
    if (pTcpSocket==NULL||m_NetStatus!=NetState_Connected)
    {
        //PreDprintf("failed pTcpSocket==NULL or Disconnected\n");

        return size;
    }

    //printf("QTcpClient::SendData()\n");
    //printHex(pBuf,size);

    pTcpSocket->write(pBuf,size);
    pTcpSocket->flush();             //也许可以加快清空写缓存
    int delay=size/8+3000;         //最好不要无限等待，根据要发送数据大小决定阻塞延时,按最低约10kb/s的速率计算延时
    pTcpSocket->waitForBytesWritten(delay);
    unsigned int unSentBytes=pTcpSocket->bytesToWrite();

    if(unSentBytes!=0)
    {
        printf(" 发送失败: 剩余 %u Byte\n",unSentBytes);
    }

    //if(pTcpSocket->state()!=3)
    //{
    //    return -1;           //网络非正常退出
    //}

    return  unSentBytes;        //返回未发送的字节数
}


//void QTcpClient::SendDataFromOutSide(QByteArray& ba,char* buf,int len)
//{
//    //printName();
//    //printf("slot_SendDataFromUI()\n");
//    //qDebug()<<"Qthread::SendDataFromUI() threadID:"<<QThread::currentThreadId();
//
//
//#if 0
//    //继续研究关联信号与槽 方式，最好在网络线程里面运行
//    QMutexLocker lock(&mutex);
//    ReadySend.append(ba);
//    if (ReadySend.size()>300*1024*1024)   //某些情况网络线程没有来得及及时发送并清空数据
//    {
//        ReadySend.clear();
//    }
//    return;
//#endif
//
//
//    //直接发送从UI传过来的数据--此时网络不一定连接上-但是也会触发此函数
//    //若当前线程没有start(),触发此线程会崩溃;
//    char* bufSend=ba.data();
//    int bufsize=ba.size();
//    if(buf!=0)   //使用指针与长度指示静态内存
//    {
//          bufSend=buf;
//          bufsize=len;
//    }
//    //print_hex(bufSend,bufsize);
//    SendData(bufSend,bufsize);
//}


//
//void QTcpClient::slot_CmdFromOutSide(int type,QByteArray ba,char* buf,int len,bool isNew)
//{
//    printf("QTcpClient::slot_CmdFromOutSide() type:%d buf:%p len:%d\n",type,buf,len);
//
//    switch(type)
//    {
//    //case  SK_RECONNECT: //重新连接网络 
//    //    InitSocket();
//        break;   
//    case SK_DataToSocket:   
//        //emit sig_send(type,ClientID,ba,buf,len,isNew);
//        SendDataFromOutSide(ba,buf,len);
//        break;
//    default: break;
//    }
//}
void QTcpClient::SendDataFromOutSide(char* buf,int len)
{
       SendData(buf,len);   //直接转发数据
}


void QTcpClient::slot_CmdFromOutSide(int type,StuMessage msgData)
{
    //printf("QTcpClient::slot_CmdFromOutSide() type:%d\n",type);

     //PRINT_FUN_NAME

    switch(type)
    {
        //case  SK_RECONNECT: //重新连接网络 
        //    InitSocket();
        break;   
    case SK_DataToSocket:   
        SendDataFromOutSide(msgData.pData,msgData.dataLen);   //直接转发数据
        msgData.DestoryNoLongerUse();                      //不再使用此结构释放数据
        break;
    default: break;
    }
}

#if 0
const QEvent::Type MyEvent = (QEvent::Type)1234;
void QTcpClient::slot_Send(int type,int ClientID,QByteArray ba,char* buf,int len,bool isNew)
{
    //qDebug()<<"Qthread::slot_Send()() this: "<<(void*)this<<" threadID:"<<QThread::currentThreadId();
    //主线程调用此函数在此发送事件,事件函数QTcpClient::event函数会在主线程运行 movetothread之后未测试。
    //QCoreApplication::postEvent(dynamic_cast<QThread*>(this), new MyDataEvent);
    //return;
    qDebug()<<"QTcpClient::slot_Send() threadID:"<<QThread::currentThreadId();
    SendData(ba.data(),ba.size(),0);
}

bool QTcpClient::event(QEvent* event)
{
    //qDebug()<<"Qthread:::event() threadID:"<<QThread::currentThreadId();
    //if (event->type() == MyEvent)
    //{
    //    return true;
    //}
    return QObject::event(event);
}

void QTcpClient::customEvent(QEvent* event)
{
    qDebug()<<"Qthread:::customEvent() threadID:"<<QThread::currentThreadId();
}
#endif

//各种报文请求函数
//各种报文处理函数
