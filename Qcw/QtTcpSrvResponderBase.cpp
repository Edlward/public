#include "QtTcpSrvResponderBase.h"

#include "Super/Basic/BasicOperation.h"
#include "Super/Basic/crc.h"

#include "../public/Proj/ProtocolNetHead.h"                   //����ͨ��Э�����ݱ���ͷ
//#include "../public/Proj/QtNetFrame/Protocol.h"           //����ͨ��Э��


#include "QtTcpSrvFrame.h"   //Ϊ���ܷ��ʷ�����һЩ״̬��������,�����໥����,�Ա�����ת��


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

    //socket�ڲ��ź�
    connect(this,SIGNAL(readyRead()),this,SLOT(slot_DataReceived()));           //��ȡ��ͨ�����Լ��ж�, 

    //�Լ���˼��,���������ȴ�ĳ�������,����slot_DataReceived()��
    connect(this,SIGNAL(sig_getSocketData()),this,SLOT(slot_DataReceived())); 

    //�����socket��������Server��ܹ���,ʵ�����ã�����
    connect(this,SIGNAL(connected()),this,SLOT(slot_Connected()));


    connect(this,SIGNAL(disconnected()),this,SLOT(slot_Disconnected()));   //�ر�����ʱ�����ͶϿ������ź�
    // connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));        //�ر�����ʱ�������Զ�ɾ�� 
    //���ܹ���������ڴ�й©�� ����new�����Ķ�����Ҫ

    //��ʱ������Ϊ����Qtcpserver�����,
    //connect(&socket,SIGNAL(connected()),this,SLOT(slot_Connected()));

    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
        this, SLOT(Error(QAbstractSocket::SocketError)));

    //timer_toDO=startTimer(40);       //24HZ=41.666MS
    isblocking=false;                        //����������־λ
    InitReadMethod2();                   //���Ľ��շ�ʽ2��ʼ��
}


QtTcpSrvResponderBase::~QtTcpSrvResponderBase()
{

}

SharaDataMgr& QtTcpSrvResponderBase::getSharaDataMgr()
{

    //��̬����ת��,����ȷ�� ����
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
    va_list args;                      //����һ��va_list���͵ı������������浥������
    va_start(args,fmt);                //ʹargsָ��ɱ�����ĵ�һ������  
    //ֱ�Ӵ�ӡԭ�ַ���
    //::printf(fmt, args);             //����ȫ��ȷ��%d��ӡ���γ���
    //::printf(fmt,*args);             //��ȷ
    ::vprintf(fmt,args);               //��ȷ--��׼�÷�

    //ȡ�ô�ӡ���ַ���buf
    //::sprintf(buf,fmt,args);       //����ȫ��ȷ��%d��ӡ���γ���
    //::sprintf(buf,fmt,*args);      //��ȷ��ֱ�ӷ�װsprintf�����Լ�����Fmt�����еĲ������������ͣ������޷���ȡ������
    ::vsprintf(buf,fmt,args);        ///��ȷ--��׼�÷�   ������vprintf�ȴ�v��    ::sprintf(buf,fmt,...);  vprintf(cmd,args);
    va_end(args);                    //�����ɱ�����Ļ�ȡ

    // ::printf("New:%s\n",buf);

    QString sMsg(buf);
    emit sig_PrintMsg(sMsg);   		 //�õ��ַ������͸�UI��ʾ
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
    //�Ͽ�����ʱ�����ͶϿ��ź�
    emit sig_DisConnect(this->socketID,this);    
    //�Ͽ��ĸ�����,ͬʱ���������ָ��,������QTcpserver�����������Ӧָ��
}


#include<time.h>
int QtTcpSrvResponderBase::SendRandData()   //��������ķ��Ͳ��ԶԷ������߼��Ƿ�����
{
    //1.��ȫ���������--
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
    //2.��ȷ����ͷ��������ݲ���
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

    RecvDataProcess();           //����1�����ñ���
    //RecvAllDataToProc();      //����2������bug,��ʱû�к÷������ 20190411
}



void QtTcpSrvResponderBase::slot_Connected()
{
    PRINT_FUN_NAME
}


//�����ݷ���1
int QtTcpSrvResponderBase::RecvDataProcess()
{
    const int sizeMsgHead=sizeof(KL);      //����ͷ��С
    char* bufHead=this->bufStack;           //����ͷ����
    KL* pKL = (KL*)bufHead;

    int sumRead=0;                          //��¼���ִ���һ����ȡ�����������ֽ���
    //�ȴ�����ͷ������һ�����˳�
    while(bytesAvailable()<sizeMsgHead)
    {
        return 0;
        bool ret=WaitForData();
        if(!ret)
        {
            printf("Wait:No new data is coming: Quit û��������\n");
        }
    }

    while(bytesAvailable()>=sizeMsgHead)
    {
        sumRead+=ReadNbytes(bufHead,sizeMsgHead);  //��ȡ����ͷ

        //������ͷ����ȷ,ѭ����ȡ������,ֱ����ȷ
        while(pKL->head!=KL_HEAD)
        {
            printf("Msg head Error =0X%X\n",pKL->head);
            print_hex(bufHead,sizeMsgHead);         //��ӡ����ͷ
            char tailBytes;
            int nr=ReadNbytes(&tailBytes,1);
            if (nr==1)
            {
                char* p=bufHead;
                for(int i=0;i<sizeMsgHead-1;i++)
                {
                    p[i]=p[i+1];
                }
                p[sizeMsgHead-1]=tailBytes;  //�¶�������䵽β��
            }
            else
            {
                //
            }
        }

        int sizeMsgBody=pKL->len;                       //ʣ�౨���峤�ȡ�
        int bufsize=sizeMsgBody+sizeMsgHead;    //�����ݴ�С
        bool useNewBuf=false;
        char* buf=this->bufStack;
        if(bufsize>sizeof(this->bufStack))
        {
            buf=new(std::nothrow) char[bufsize];
            if(buf==NULL)
            {
                printf("����̫�� �ڴ治��\n");     //�����ж�����������������ݿ��ܻ�����ܴ��ڴ棬�������
                return sumRead;
            }
            useNewBuf=true;
            memcpy(buf,bufHead,sizeMsgHead);                //��������ͷ���µĵ�ַ
        }
        sumRead+=ReadNbytes(buf+sizeMsgHead,sizeMsgBody);   //��ʣ����ֽ�
        FilterProcess(buf,bufsize);                              //���δ����������

        if(true==useNewBuf)
        {
            delete[] buf;                                     //����ʹ���˶��ڴ棬�ͷ�
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
    int nread=size;           //��Ҫ��ȡ�����ݴ�С
    char* pos=buf;            //�����ַ
    while(nread>0)
    {
        if(bytesAvailable()==0)        //���û����Ч����
        {
            bool ret=WaitForData();
            if(!ret)
            {
                //ClearSocketBuffer();
                return sumRecv;  //return;
            }
        }
        int i=read(pos,nread);   //��ȡ������С�ķ���
        sumRecv+=i;  //�ۼӵ���ȡ��������
        pos+=i;
        nread-=i;
    }
    return sumRecv;
}







//�����ݷ���2
void QtTcpSrvResponderBase::InitReadMethod2()
{
    pBufStart=this->pBufBig;                        //���ݻ�����ʼ��ַ
    pBufEnd=this->pBufBig+BufAllSize-1;     //���ݻ��������ַ ��Ҫ��1,����Խ��

    pStart=this->pBufBig;   //δ��������ʼ��ַ
    pEnd=this->pBufBig;     //δ�����Ľ�����ַ
}


void QtTcpSrvResponderBase::RecvAllDataToProc()
{
    int SocketDataSize=bytesAvailable();
    if(SocketDataSize==0)   return;   //û������ֱ�ӷ��أ���������

    int readSize=SocketDataSize;
    int  BufRemainSize=getRemainBufSize();
    if(SocketDataSize>BufRemainSize)   //���ݴ���ʣ��ռ�
    {
        readSize=BufRemainSize;        //ֻ��ȡʣ�������ɵ�
    }
    ReadDatatoBuf(readSize);
    //����һ��
    ProcessAll();    //���δ����������
}


unsigned int QtTcpSrvResponderBase::getRemainBufSize()
{
    return this->BufAllSize-getUnProcBufSize();   //����ʹ�ñ���δ������Ŀռ�
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
    int wantRead=size;           //��Ҫ��ȡ�����ݴ�С
    char* tmp=buf;            //�����ַ
    while(wantRead>0)            //��Ϊ֮ǰ�Ѿ��ж������ݣ�����Ӧ������ȫ��ȡ
    {
        int nR=read(tmp,wantRead);   //��ȡ������С�ķ���
        sumRecv+=nR;           //�ۼӵ���ȡ��������
        tmp+=nR;
        wantRead-=nR;
    }
    return sumRecv;
}


int QtTcpSrvResponderBase::ReadDatatoBuf(int readSize)
{
    //�������pEnd=pStart,�����ǿգ�  ������
    int SumRecv=0;
    int nread=0;
    if(pEnd>=pStart)
    {
        int sizeToTail=pBufEnd-pEnd;            //������β����ʣ���С
        if(sizeToTail>=readSize)                //ʣ��β����װ
        {
            nread=ReadNbytesforWhile(pEnd,readSize);
            pEnd+=nread;
            SumRecv+=nread;
        }
        else        //�ֶζ�ȡ
        {
            nread=ReadNbytesforWhile(pEnd,sizeToTail);   //�ȶ����ֵ�β��
            pEnd+=nread;
            SumRecv+=nread;
            int Remainsize=readSize-sizeToTail;
            nread=ReadNbytesforWhile(pBufStart,Remainsize);  //�ٶ�ʣ��ŵ���ǰ
            pEnd=pBufStart+nread;                            //����β����ַ��ǰ����
            SumRecv+=nread;
        }
    }
    else
    {
        int sizeToTail=pStart-pEnd;              //������β����ʣ���С
        if(sizeToTail>=readSize)                 //ʣ��β����װ
        {
            nread=ReadNbytesforWhile(pEnd,readSize);
            pEnd+=nread;
            SumRecv+=nread;
        }
        else{} //������ ֮ǰ�Ѿ��ж�
    }
    return SumRecv;
}



void QtTcpSrvResponderBase::ProcessAll()
{
    while(1)
    {
        int SizeHead=sizeof(KL);
        KL* pKL = (KL*)pStart;               //��ʼ��������
        if(getUnProcBufSize()<SizeHead)    //���δ�������Ч����С��һ������ͷ
        {
            return;
        }
        if(pKL->head!=KL_HEAD)         //����ͷ��ȷ
        {
            printf("Client: pKL->head Error =%X\n",pKL->head);
            print_hex(pStart,SizeHead);         //��ӡ����ͷ
        }

        while(1)                           //ֱ��ֱ����ȷ����ͷ--��������Լ��㶪��
        {
            if(pKL->head==KL_HEAD)         //����ͷ��ȷ
                break;

            //            printf("Client: pKL->head Error =%X\n",pKL->head);
            //            print_hex(pStart,SizeHead);         //��ӡ����ͷ

            pStart++;
            if(pStart>pBufEnd)             //��Ѱ��β��
                pStart=pBufStart;

            if(pStart==pEnd)              //��Ч��������
                break;

            pKL = (KL*)pStart;                 //����ӳ��ָ������
        }
        if(getUnProcBufSize()<SizeHead)    //���δ�������Ч����С��һ������ͷ
        {
            return;
        }

        int SizePyloadContent=pKL->len;            //
        if(pKL->len>BufAllSize)  //100*1024*100
        {
            //���ⳬ���Ĵ���--����ļ���һ�β����ܴ������
            //��ȡʣ�����ݣ�Ȼ��ӹ��׽��ֶ�ȡʣ������
        }

        int SizeUnProc=getUnProcBufSize();         //δ�����Ĵ�С
        int SizePyload=SizePyloadContent+SizeHead; //���α��Ĵ�С
        if(SizePyload>SizeUnProc)    //������Ĳ�������ֱ���˳����ȴ��´δ���
        {
            break;
        }

        char* buf=pStart;
        int tailsize=(pBufEnd-pStart);     //β��ʣ��
        bool useNewBuf=false;
        if(SizePyload>tailsize)           //���Ŀ��ܵ�ַ��������ǰ��һ�κ���һ��    //�������¿ռ�
        {
            int requestSize=SizePyload;
            if(requestSize>this->BufAllSize)  //������,���������������ʣ������
                requestSize=this->BufAllSize;

            buf=new(std::nothrow) char[requestSize];        //�϶���ɹ�
            //            if(NULL==buf)
            //            {
            //                printf("����̫�� �ڴ治��\n");
            //                return;
            //            }
            useNewBuf=true;
            memcpy(buf,pStart,tailsize);
            memcpy(buf+tailsize,pBufStart,(requestSize-tailsize));
        }

        FilterProcess(buf,SizePyload);    //����������

        if(true==useNewBuf)       //���ʹ���˶��ڴ�
        {
            delete[] buf;
        }

        //�Ѿ��������ַ�����ƶ�
        char* pStartNew=pStart+SizePyload;
        if(pStartNew>pBufEnd)
        {
            pStartNew=pBufStart+(SizePyload-(pBufEnd-pStart));
        }
        pStart=pStartNew;
    }
}




//����������
int QtTcpSrvResponderBase::FilterProcess(char *buf,int size)
{
    KL* pKL = (KL*)buf;

    //�õ��������ݰ�,�����ڴ˵��ý��ܺ���,Ȼ����CRCУ�飬
    //���ݼ������͵��õ�����Ӧ�Ľ�����Կ
    //printf("Client:�յ�����Cmdtype=%X ���ĳ���len=%d \n",pKL->key,pKL->len);

    //print_hex(buf,sizeof(KL));          //��ӡ���б���
    //�����������͵�����Ӧ������

    //�ٸĽ�--��ٱ���ͷ���������ĳ��Ⱥܴ�û��ʵ������

    if(size>this->BufAllSize)   //���������ļ����ͣ��ڴ������������ʣ������
    {
        //�粻�����������ȡʣ���������
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


    //�յ�����֮����CRC32У����
    //    unsigned int CRC32=Get_CRC32_fromBuffer(0,(uint8*)(buf+sizeof(KL)),pKL->len);
    //    if(CRC32!=pKL->CRC32)
    //    {
    //        qDebug()<<"���ݴ������� ԭCRC32= "<<pKL->CRC32<<" Recv CRC32="<<CRC32;
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
            //printf("Server: δ֪���� Unknow CMD:pKH->key=%X ����=%d\n",pKL->key,pKL->len);

            ProcessDatagram(buf,size);

            break;
        }
    }
    return 0;
}






//���CRC�ֶ�,Ȼ����ܷ��� val��ʾ����ƫ��,���ط���ʧ�ܵ��ֽ���
int QtTcpSrvResponderBase::SendData(char* bufin,int size,int val)   //���CRC�ֶ�,Ȼ����ܷ��� val��ʾ����ƫ��
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
    char*  buf=bufin;                //�����㷨����16�ֽڶ����ˣ�û�б�Ҫ�����ڴ棬ֱ��ʹ�ô��������ڴ�
#endif

    KL* pKL=(KL*)buf;

    //����֮ǰ���CRC32У����--ע�ⲻ�ֶܷη��ͣ����߷���ǰ������CRC32�ֶ�
    //pKL->CRC32=Get_CRC32_fromBuffer(0,(uint8*)buf+sizeof(KL),pKL->len);


    //write(buf,size);
    write(buf,size);
    flush();

    //pTcpSocket->waitForBytesWritten(-1);
    int delay=size/128+5000;      //��ò�Ҫ���޵ȴ�������Ҫ�������ݴ�С����������ʱ,�����Լ10kb/s�����ʼ�����ʱ
    waitForBytesWritten(delay);


    //new Ҳ����Լӿ����д����
    //socket.bytesToWrite ()�������ڵȴ���д�����ݵ��ֽ�����Ҳ�����������Ĵ�С��Ҳ����дѭ���ж����ֵΪ0,�����Ѿ�����

    //printf("Server: �������� CmdType=%X ���ĳ���len=%d \n",pKL->key,pKL->len);
    //print_hex(buf,sizeof(KL));         //��ӡ����ͷ


    //    printf("Sever: bytesToWrite=%d\n",bytesToWrite());
    //    printf("Sever: crc_encry_send()--END\n");

#if 0
    if(true==useHeapbuf)
        free(buf);
#endif
    //qDebug("Server: SendEnd: socket.State=%d\n",state()); //��ӡsocket״̬
    if(state()!=3)
        return -1;           //����������˳�

    return  bytesToWrite();
}






void QtTcpSrvResponderBase::do_test()//���Ժ���
{

}

void QtTcpSrvResponderBase::sendDataToUI(char *buf, unsigned int len)
{
    // printf("TcpServerSocket::sendDataToUI()\n");

    QByteArray ba(buf,len);
    emit sig_CMD_SocketToUI(this->socketID,UI_SenDatatoUI,0,ba);   //socket�̷߳��͸�UI������
}



void QtTcpSrvResponderBase::Resp_RandData(char *buf, unsigned int len)
{

}



#include <exception>  //ʹ���쳣
#include <new>  
//using namespace std; 


//���ַ������Σ� char* buf,int size
void QtTcpSrvResponderBase::slot_CMDfromUI(int SocketID,int cmdtype,QByteArray ba)   //	����UI������������
{
    if(SocketID!=this->socketDescriptor())  
    {
        printf("this->socketDescriptor()=%d\n",this->socketDescriptor());
        //if(SocketID!=this->socketID)
        return;         //������Ƿ�����ǰsocket�̵߳��������˳�
    }

    //printf("TcpServerSocket::slot_CMDfromUI()\n");

    switch(cmdtype)
    {

    case UI_TEST:
        do_test();              break; //���Ժ���

    case UI_SenDataDirect:
        printf("UI_SenDataDirect\n");
        SendData(ba.data(),ba.size(),0);
        break;

    case UI_MsgQueueData:
        {
            //��QByteArray�����������Ϣ����,��������Ϣ������
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







