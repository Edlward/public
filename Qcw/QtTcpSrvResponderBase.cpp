#include "QtTcpSrvResponderBase.h"

#include "Super/Basic/BasicOperation.h"
#include "Super/Basic/crc.h"

#include "../public/Proj/ProtocolNetHead.h"                   //网络通信协议数据报文头
//#include "../public/Proj/QtNetFrame/Protocol.h"           //网络通信协议


#include "QtTcpSrvFrame.h"   //为了能访问服务器一些状态或共享数据,必须相互包含,以便类型转换


QtTcpSrvResponderBase* CreateTcpSrvResponderBase(unsigned short socketDescriptor,QtTcpSrvFrame* parent)
{
    return new QtTcpSrvResponderBase(socketDescriptor,parent);
}

#include <QHostAddress>
QtTcpSrvResponderBase::QtTcpSrvResponderBase(int socketDescriptor,QObject *parent) :
    QTcpSocket(parent),socketID(socketDescriptor)

{   
    this->setSocketDescriptor(socketDescriptor);

    ::printf("New Connect comming:ThreadID=%d\n",QThread::currentThreadId());


    this->socketID=socketDescriptor;

    //socket内部信号
    connect(this,SIGNAL(readyRead()),this,SLOT(slot_DataReceived()));           //读取普通数据自己判断, 

    //自己构思的,代替阻塞等待某个命令返回,关联slot_DataReceived()槽
    connect(this,SIGNAL(sig_getSocketData()),this,SLOT(slot_DataReceived())); 

    //服务端socket的连接由Server框架管理,实际无用，暂留
    connect(this,SIGNAL(connected()),this,SLOT(slot_Connected()));


    connect(this,SIGNAL(disconnected()),this,SLOT(slot_Disconnected()));   //关闭连接时，发送断开连接信号
    // connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));        //关闭连接时，对象自动删除 
    //就能够避免出现内存泄漏。 不是new出来的对象不需要

    //暂时不用因为这是Qtcpserver管理的,
    //connect(&socket,SIGNAL(connected()),this,SLOT(slot_Connected()));

    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(Error(QAbstractSocket::SocketError)));

    //timer_toDO=startTimer(40);       //24HZ=41.666MS
    isblocking=false;                        //正在阻塞标志位
    InitReadMethod2();                   //报文接收方式2初始化
}


QtTcpSrvResponderBase::~QtTcpSrvResponderBase()
{

}

SharaDataMgr& QtTcpSrvResponderBase::getSharaDataMgr()
{

    //动态类型转换,测试确认 待续
    QtTcpSrvFrame* pQtTcpSrvFrame=static_cast<QtTcpSrvFrame*>(parent());
    return pQtTcpSrvFrame->getSharaDataMgr();
}



bool QtTcpSrvResponderBase::ProcessDatagram(char* buf,unsigned int len)
{
    return true;
}


void QtTcpSrvResponderBase::UnknowProtocol(char* buf,unsigned int len)
{

}


//bool QtTcpSrvResponderBase::ProcessMessage()
//{
//    return true;
//}




void QtTcpSrvResponderBase::ClientConnected()
{

}


void QtTcpSrvResponderBase::ClientDisConnected()
{

}


void QtTcpSrvResponderBase::CloseConnect()
{

}

void QtTcpSrvResponderBase::run()
{

}

void QtTcpSrvResponderBase::printf(const char* fmt, ...)
{
    char buf[1024]={0};
    va_list args;                      //定义一个va_list类型的变量，用来储存单个参数
    va_start(args,fmt);                //使args指向可变参数的第一个参数  
    //直接打印原字符串
    //::printf(fmt, args);             //不完全正确，%d打印整形出错
    //::printf(fmt,*args);             //正确
    ::vprintf(fmt,args);               //正确--标准用法

    //取得打印后字符串buf
    //::sprintf(buf,fmt,args);       //不完全正确，%d打印整形出错
    //::sprintf(buf,fmt,*args);      //正确，直接封装sprintf必须自己解释Fmt参数中的参数个数和类型，否则无法提取参数？
    ::vsprintf(buf,fmt,args);        ///正确--标准用法   必须用vprintf等带v的    ::sprintf(buf,fmt,...);  vprintf(cmd,args);
    va_end(args);                    //结束可变参数的获取

    // ::printf("New:%s\n",buf);

    QString sMsg(buf);
    emit sig_PrintMsg(sMsg);   		 //得到字符串发送给UI显示
}


void QtTcpSrvResponderBase::timerEvent(QTimerEvent * te)
{
    if(te->timerId() ==timer_toDO)
    {
        //SendRandData();
    }
}

void QtTcpSrvResponderBase::Error(QAbstractSocket::SocketError socketError)
{
    //m_ConnectStatus = 0;
}


void QtTcpSrvResponderBase::slot_Disconnected()
{
    //断开连接时，发送断开信号
    emit sig_DisConnect(this->socketID,this);    
    //断开哪个连接,同时发送其对象指针,方便在QTcpserver的链表里面对应指针
}


#include<time.h>
int QtTcpSrvResponderBase::SendRandData()   //随机数报文发送测试对方处理逻辑是否会崩溃
{
    //1.完全随机数测试--
#if 0
    const unsigned int BSIZE=10*1024;
    char buf[BSIZE];
    ::srand(::time(NULL));
    unsigned int msgSize=::rand()%BSIZE;
    char* pos=buf;
    for(int n=0;n<msgSize;n++)
    {
        pos[n]=::rand()%256;
    }
    return SendData(buf,msgSize,0);
#else
    //2.正确报文头的随机数据测试
    const unsigned int BSIZE=10*1024;
    const unsigned int sizeHead=sizeof(KL);

    char buf[sizeHead+BSIZE];
    KL* pKL=(KL*)buf;
    pKL->head=KL_HEAD;
    pKL->key=CMD_RAND_DATA;
    ::srand(::time(NULL));
    unsigned int msgSize=::rand()%BSIZE;
    char* pos=buf+sizeHead;
    for(int n=0;n<msgSize;n++)
    {
        pos[n]=::rand()%256;
    }
    pKL->len=msgSize;
    return SendData((char*)pKL,sizeHead+msgSize,0);
#endif
}


void QtTcpSrvResponderBase::slot_DataReceived()
{

    //PRINT_FUN_NAME

    RecvDataProcess();           //方法1：永久保留
    //RecvAllDataToProc();      //方法2：存在bug,暂时没有好方法解决 20190411
}



void QtTcpSrvResponderBase::slot_Connected()
{
    PRINT_FUN_NAME
}


//读数据方法1
int QtTcpSrvResponderBase::RecvDataProcess()
{
    const int sizeMsgHead=sizeof(KL);      //报文头大小
    char* bufHead=this->bufStack;           //报文头缓存
    KL* pKL = (KL*)bufHead;

    int sumRead=0;                          //记录本轮处理一共读取到的数据总字节数
    //等待数据头，不够一个就退出
    while(bytesAvailable()<sizeMsgHead)
    {
        return 0;
        bool ret=WaitForData();
        if(!ret)
        {
            printf("Wait:No new data is coming: Quit 没有新数据\n");
        }
    }

    while(bytesAvailable()>=sizeMsgHead)
    {
        sumRead+=ReadNbytes(bufHead,sizeMsgHead);  //读取报文头

        //若数据头不正确,循环读取并左移,直到正确
        while(pKL->head!=KL_HEAD)
        {
            printf("Msg head Error =0X%X\n",pKL->head);
            print_hex(bufHead,sizeMsgHead);         //打印报文头
            char tailBytes;
            int nr=ReadNbytes(&tailBytes,1);
            if (nr==1)
            {
                char* p=bufHead;
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

        int sizeMsgBody=pKL->len;                       //剩余报文体长度。
        int bufsize=sizeMsgBody+sizeMsgHead;    //总数据大小
        bool useNewBuf=false;
        char* buf=this->bufStack;
        if(bufsize>sizeof(this->bufStack))
        {
            buf=new(std::nothrow) char[bufsize];
            if(buf==NULL)
            {
                printf("报文太长 内存不足\n");     //必须判断如果是乱码或恶意数据可能会申请很大内存，程序崩溃
                return sumRead;
            }
            useNewBuf=true;
            memcpy(buf,bufHead,sizeMsgHead);                //拷贝报文头到新的地址
        }
        sumRead+=ReadNbytes(buf+sizeMsgHead,sizeMsgBody);   //读剩余的字节
        FilterProcess(buf,bufsize);                              //依次处理各种命令

        if(true==useNewBuf)
        {
            delete[] buf;                                     //大报文使用了堆内存，释放
        }
    }
    return sumRead;
}


bool QtTcpSrvResponderBase::WaitForData()
{
    isblocking=true;
    bool ret=waitForReadyRead(1000);  //30000
    isblocking=false;
    return ret;
}

int QtTcpSrvResponderBase::ClearSocketBuffer()
{
    while(bytesAvailable()>0)
    {
        printf("While read to Clear Socket Buf:\n");
        char buf[1024];
        read(buf,1024);
    }
    return 0;
}


int QtTcpSrvResponderBase::ReadNbytes(char *buf, int size)
{
    int sumRecv=0;
    int nread=size;           //想要读取的数据大小
    char* pos=buf;            //缓存地址
    while(nread>0)
    {
        if(bytesAvailable()==0)        //如果没有有效数据
        {
            bool ret=WaitForData();
            if(!ret)
            {
                //ClearSocketBuffer();
                return sumRecv;  //return;
            }
        }
        int i=read(pos,nread);   //读取完整大小的方法
        sumRecv+=i;  //累加到读取的总数据
        pos+=i;
        nread-=i;
    }
    return sumRecv;
}







//读数据方法2
void QtTcpSrvResponderBase::InitReadMethod2()
{
    pBufStart=this->pBufBig;                        //数据缓存起始地址
    pBufEnd=this->pBufBig+BufAllSize-1;     //数据缓存结束地址 需要减1,否则越界

    pStart=this->pBufBig;   //未处理报文起始地址
    pEnd=this->pBufBig;     //未处理报文结束地址
}


void QtTcpSrvResponderBase::RecvAllDataToProc()
{
    int SocketDataSize=bytesAvailable();
    if(SocketDataSize==0)   return;   //没有数据直接返回，基本不会

    int readSize=SocketDataSize;
    int  BufRemainSize=getRemainBufSize();
    if(SocketDataSize>BufRemainSize)   //数据大于剩余空间
    {
        readSize=BufRemainSize;        //只读取剩余能容纳的
    }
    ReadDatatoBuf(readSize);
    //读完一次
    ProcessAll();    //依次处理各个命令
}


unsigned int QtTcpSrvResponderBase::getRemainBufSize()
{
    return this->BufAllSize-getUnProcBufSize();   //正在使用报文未处理完的空间
}

unsigned int QtTcpSrvResponderBase::getUnProcBufSize()
{
    if(pEnd>=pStart)
        return pEnd-pStart;    //int BufInUseSize=0;
    else
        return (pEnd-pBufStart)+(pBufEnd-pStart);
}

int QtTcpSrvResponderBase::ReadNbytesforWhile(char *buf, int size)
{
    int sumRecv=0;
    int wantRead=size;           //想要读取的数据大小
    char* tmp=buf;            //缓存地址
    while(wantRead>0)            //因为之前已经判断有数据，这里应该能完全读取
    {
        int nR=read(tmp,wantRead);   //读取完整大小的方法
        sumRecv+=nR;           //累加到读取的总数据
        tmp+=nR;
        wantRead-=nR;
    }
    return sumRecv;
}


int QtTcpSrvResponderBase::ReadDatatoBuf(int readSize)
{
    //特殊情况pEnd=pStart,数据是空，  数据满
    int SumRecv=0;
    int nread=0;
    if(pEnd>=pStart)
    {
        int sizeToTail=pBufEnd-pEnd;            //到缓存尾部的剩余大小
        if(sizeToTail>=readSize)                //剩余尾部够装
        {
            nread=ReadNbytesforWhile(pEnd,readSize);
            pEnd+=nread;
            SumRecv+=nread;
        }
        else        //分段读取
        {
            nread=ReadNbytesforWhile(pEnd,sizeToTail);   //先读部分到尾部
            pEnd+=nread;
            SumRecv+=nread;
            int Remainsize=readSize-sizeToTail;
            nread=ReadNbytesforWhile(pBufStart,Remainsize);  //再读剩余放到最前
            pEnd=pBufStart+nread;                            //报文尾部地址到前面了
            SumRecv+=nread;
        }
    }
    else
    {
        int sizeToTail=pStart-pEnd;              //到缓存尾部的剩余大小
        if(sizeToTail>=readSize)                 //剩余尾部够装
        {
            nread=ReadNbytesforWhile(pEnd,readSize);
            pEnd+=nread;
            SumRecv+=nread;
        }
        else{} //不可能 之前已经判断
    }
    return SumRecv;
}



void QtTcpSrvResponderBase::ProcessAll()
{
    while(1)
    {
        int SizeHead=sizeof(KL);
        KL* pKL = (KL*)pStart;               //开始处理数据
        if(getUnProcBufSize()<SizeHead)    //如果未处理的有效数据小于一个报文头
        {
            return;
        }
        if(pKL->head!=KL_HEAD)         //报文头正确
        {
            printf("Client: pKL->head Error =%X\n",pKL->head);
            print_hex(pStart,SizeHead);         //打印报文头
        }

        while(1)                           //直到直到正确报文头--在这里可以计算丢包
        {
            if(pKL->head==KL_HEAD)         //报文头正确
                break;

            //            printf("Client: pKL->head Error =%X\n",pKL->head);
            //            print_hex(pStart,SizeHead);         //打印报文头

            pStart++;
            if(pStart>pBufEnd)             //搜寻到尾部
                pStart=pBufStart;

            if(pStart==pEnd)              //有效数据用完
                break;

            pKL = (KL*)pStart;                 //重新映射指针必需的
        }
        if(getUnProcBufSize()<SizeHead)    //如果未处理的有效数据小于一个报文头
        {
            return;
        }

        int SizePyloadContent=pKL->len;            //
        if(pKL->len>BufAllSize)  //100*1024*100
        {
            //特殊超大报文处理--如大文件等一次不可能传送完成
            //先取剩余数据，然后接管套接字读取剩余数据
        }

        int SizeUnProc=getUnProcBufSize();         //未处理报文大小
        int SizePyload=SizePyloadContent+SizeHead; //本次报文大小
        if(SizePyload>SizeUnProc)    //如果报文不完整就直接退出，等待下次处理
        {
            break;
        }

        char* buf=pStart;
        int tailsize=(pBufEnd-pStart);     //尾部剩余
        bool useNewBuf=false;
        if(SizePyload>tailsize)           //报文可能地址不连续，前面一段后面一段    //拷贝到新空间
        {
            int requestSize=SizePyload;
            if(requestSize>this->BufAllSize)  //超大报文,到处理函数里面接收剩余数据
                requestSize=this->BufAllSize;

            buf=new(std::nothrow) char[requestSize];        //肯定会成功
            //            if(NULL==buf)
            //            {
            //                printf("报文太长 内存不足\n");
            //                return;
            //            }
            useNewBuf=true;
            memcpy(buf,pStart,tailsize);
            memcpy(buf+tailsize,pBufStart,(requestSize-tailsize));
        }

        FilterProcess(buf,SizePyload);    //处理单个报文

        if(true==useNewBuf)       //如果使用了堆内存
        {
            delete[] buf;
        }

        //已经处理过地址往后移动
        char* pStartNew=pStart+SizePyload;
        if(pStartNew>pBufEnd)
        {
            pStartNew=pBufStart+(SizePyload-(pBufEnd-pStart));
        }
        pStart=pStartNew;
    }
}




//处理单个报文
int QtTcpSrvResponderBase::FilterProcess(char *buf,int size)
{
    KL* pKL = (KL*)buf;

    //得到完整数据包,可以在此调用解密函数,然后做CRC校验，
    //根据加密类型调用调用相应的解密密钥
    //printf("Client:收到命令Cmdtype=%X 报文长度len=%d \n",pKL->key,pKL->len);

    //print_hex(buf,sizeof(KL));          //打印所有报文
    //根据命令类型调用相应处理函数

    //再改进--虚假报文头攻击，报文长度很大，没有实际数据

    if(size>this->BufAllSize)   //超大报文如文件传送，在处理函数里面接收剩余数据
    {
        //如不处理在这里读取剩余大报文数据
        int remainSize=size-this->BufAllSize;
        int nread=remainSize;
        while(nread>0)
        {
            char tmp[2];
            int i=read(tmp,1);
            if(i==0)
            {
                isblocking=true;
                bool ret=waitForReadyRead(1000);  //30000
                isblocking=false;
            }
            nread-=i;
        }

        switch(pKL->key)
        {
            //             case  CMD_GET_METADATA_LIST:        break;
            //             default:   break;
        }
        return 0;
    }


    //收到数据之后做CRC32校验码
    //    unsigned int CRC32=Get_CRC32_fromBuffer(0,(uint8*)(buf+sizeof(KL)),pKL->len);
    //    if(CRC32!=pKL->CRC32)
    //    {
    //        qDebug()<<"数据传输误码 原CRC32= "<<pKL->CRC32<<" Recv CRC32="<<CRC32;
    //        print_hex(buf,pKL->len);
    //        return -1;
    //    }


    switch(pKL->key)
    {
    case CMD_RAND_DATA:
        Resp_RandData(buf,size);   //
        break;

    default:
        {
            //sendDataToUI(buf,sizeof(KL)+pKL->len);

            //print_hex(buf,sizeof(KL));
            //printf("Server: 未知报文 Unknow CMD:pKH->key=%X 长度=%d\n",pKL->key,pKL->len);

            ProcessDatagram(buf,size);

            break;
        }
    }
    return 0;
}






//填充CRC字段,然后加密发送 val表示加密偏移,返回发送失败的字节数
int QtTcpSrvResponderBase::SendData(char* bufin,int size,int val)   //填充CRC字段,然后加密发送 val表示加密偏移
{
    //	   printf("Sever: crc_encry_send()--START\n");
#if 0
    //char* buf=new(std::nothrow) char[size];
    char bufStack[1024];
    char* buf=bufStack;
    bool useHeapbuf=false;
    if(size>sizeof(bufStack))
    {
        buf=(char*)malloc(size);  //buf=(char*)malloc(size+16);
        useHeapbuf=true;
    }

    //char* buf=(char*)malloc(size);
    memcpy(buf,bufin,size);
#else
    char*  buf=bufin;                //加密算法不做16字节对齐了，没有必要扩充内存，直接使用传进来的内存
#endif

    KL* pKL=(KL*)buf;

    //发送之前填充CRC32校验码--注意不能分段发送，或者发送前填充好了CRC32字段
    //pKL->CRC32=Get_CRC32_fromBuffer(0,(uint8*)buf+sizeof(KL),pKL->len);


    //write(buf,size);
    write(buf,size);
    flush();

    //pTcpSocket->waitForBytesWritten(-1);
    int delay=size/128+5000;      //最好不要无限等待，根据要发送数据大小决定阻塞延时,按最低约10kb/s的速率计算延时
    waitForBytesWritten(delay);


    //new 也许可以加快清空写缓存
    //socket.bytesToWrite ()返回正在等待被写的数据的字节数，也就是输出缓存的大小。也可以写循环判断这个值为0,表明已经发送

    //printf("Server: 发送命令 CmdType=%X 报文长度len=%d \n",pKL->key,pKL->len);
    //print_hex(buf,sizeof(KL));         //打印报文头


    //    printf("Sever: bytesToWrite=%d\n",bytesToWrite());
    //    printf("Sever: crc_encry_send()--END\n");

#if 0
    if(true==useHeapbuf)
        free(buf);
#endif
    //qDebug("Server: SendEnd: socket.State=%d\n",state()); //打印socket状态
    if(state()!=3)
        return -1;           //网络非正常退出

    return  bytesToWrite();
}






void QtTcpSrvResponderBase::do_test()//测试函数
{

}

void QtTcpSrvResponderBase::sendDataToUI(char *buf, unsigned int len)
{
    // printf("TcpServerSocket::sendDataToUI()\n");

    QByteArray ba(buf,len);
    emit sig_CMD_SocketToUI(this->socketID,UI_SenDatatoUI,0,ba);   //socket线程发送给UI的命令
}



void QtTcpSrvResponderBase::Resp_RandData(char *buf, unsigned int len)
{

}



#include <exception>  //使用异常
#include <new>  
//using namespace std; 


//加字符串传参？ char* buf,int size
void QtTcpSrvResponderBase::slot_CMDfromUI(int SocketID,int cmdtype,QByteArray ba)   //	处理UI发过来的命令
{
    if(SocketID!=this->socketDescriptor())  
    {
        printf("this->socketDescriptor()=%d\n",this->socketDescriptor());
        //if(SocketID!=this->socketID)
        return;         //如果不是发给当前socket线程的命令则退出
    }

    //printf("TcpServerSocket::slot_CMDfromUI()\n");

    switch(cmdtype)
    {

    case UI_TEST:
        do_test();              break; //测试函数

    case UI_SenDataDirect:
        printf("UI_SenDataDirect\n");
        SendData(ba.data(),ba.size(),0);
        break;

    case UI_MsgQueueData:
        {
            //从QByteArray流里解析出消息数据,并调用消息处理函数
            Message msg;
            memcpy(&msg,ba.data(),sizeof(Message));    
            ProcessMessage(msg);
        }
        break;
    default:	break;
    }
}

bool QtTcpSrvResponderBase::ProcessMessage(const Message& msg)
{
    return true;
}







