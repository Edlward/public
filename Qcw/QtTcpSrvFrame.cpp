#include <QtNetwork>
#include "QtTcpSrvFrame.h"
#include <QDebug>


QtTcpSrvFrame::QtTcpSrvFrame(int Port,QObject *parent)
    : m_port (Port)
    ,QTcpServer(parent)
    ,pCreatFun(CreateTcpSrvResponderBase)  //Ĭ�ϵ�ҵ��������������
{
   // this->port=port;                   //���洫�ݽ����Ķ˿ڣ�
    ::printf("CTcpServer::CTcpServer()\n");

    //����Ĭ�ϵ�ҵ��������������
    //setResponderCreatFun(CreateTcpSrvResponderBase);
}


QtTcpSrvFrame::~QtTcpSrvFrame()
{
    this->close();
}

void QtTcpSrvFrame::printf(const char* fmt, ...)
{
    char buf[1024]={0};
    va_list args;                         //����һ��va_list���͵ı������������浥������  
    va_start(args,fmt);                //ʹargsָ��ɱ�����ĵ�һ������  
    //ֱ�Ӵ�ӡԭ�ַ���
    //::printf(fmt, args);              //����ȫ��ȷ��%d��ӡ���γ���
    //::printf(fmt,*args);             //��ȷ
    ::vprintf(fmt,args);               //��ȷ--��׼�÷�

    //ȡ�ô�ӡ���ַ���buf
    //::sprintf(buf,fmt,args);       //����ȫ��ȷ��%d��ӡ���γ���
    //::sprintf(buf,fmt,*args);      //��ȷ��ֱ�ӷ�װsprintf�����Լ�����Fmt�����еĲ������������ͣ������޷���ȡ������
    ::vsprintf(buf,fmt,args);        ///��ȷ--��׼�÷�   ������vprintf�ȴ�v��    ::sprintf(buf,fmt,...);  vprintf(cmd,args);
    va_end(args);                      //�����ɱ�����Ļ�ȡ 

    // ::printf("New:%s\n",buf);
    QString sMsg(buf);
    emit sig_PrintMsg(sMsg);   		 //�õ��ַ������͸�UI��ʾ
}


bool QtTcpSrvFrame::OpenServer()
{
    //�ٸĲ�һ���ɹ��Ĳ�������ò�Ҫ���ڹ��캯������
    //�����ƶ�����ĳ��IP ��˿�
    //QT5 Ĭ�ϼ���IPV6��ַ
#if (QT_VERSION>0x050000)                  //�������QtCore  �����ҵ��˺궨��
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
    //�趨����������������ĿΪ(Ĭ��30),������������������󣬿ͻ����Ծɿ������ӷ���ˣ����Ƿ���˲��ڽ������ӣ�����ϵͳ�����Щ���ӱ�����һ�������С�
    return true;
}


//�µ����ӵ���

#if (QT_VERSION>0x050000)                  //�������QtCore  �����ҵ��˺궨��
void QtTcpSrvFrame::incomingConnection(qintptr socketDescriptor)      //QT4��QT5�˺������岻һ��������汾��ȷ��
#else
void QtTcpSrvFrame::incomingConnection(int socketDescriptor)
#endif
{
    //printf("CTcpServer::incomingConnection()\n");

    //�̳���QTcpSocket()
    //TcpSrvResponderBase *pSocket = new TcpSrvResponderBase(socketDescriptor);  //this,

    //��һ������
    QtTcpSrvResponderBase *pSocket=pCreatFun(socketDescriptor,this);
    QThread* pThread=new QThread(pSocket);


    //���Խ�һ���޸�ÿ�����ٸ�����Ŵ���һ���߳�
    //static int NumThread=0;
    //��������localPort()����server�Ķ˿ںţ�peerport�Ǹ������




    connect(pSocket,SIGNAL(sig_updateClients(QString,int)),this,SLOT(slot_updateClients(QString,int)));

    connect(pSocket,SIGNAL(sig_DisConnect(int,QtTcpSrvResponderBase*)),this,SLOT(slot_DisConnect(int,QtTcpSrvResponderBase*)));

    //�źŹ����ź�
    //����CTcpServer��ת���ź�,����socket�߳� 
    connect(this,SIGNAL(sig_CMD_UItoSocket(int,int,QByteArray)),pSocket,SLOT(slot_CMDfromUI(int,int,QByteArray)));


    //����CTcpServer��ת���ź�  ��ת����socket�̵߳��ź�, ����UI����
    connect(pSocket,SIGNAL(sig_CMD_SocketToUI(int,int,int,QByteArray)),this,SIGNAL(sig_CMD_SocketToUI(int,int,int,QByteArray)));

    //���ʹ�ӡ��Ϣ��UI��ʾ
    connect(pSocket,SIGNAL(sig_PrintMsg(QString)),this,SIGNAL(sig_PrintMsg(QString)));


    //Ҫ��moveToThread,���󴴽�ʱ�����и�ָ��
    //TcpServerSocket->setParent(NULL);                       //��parent��object�ǲ��ܱ��ƶ��������߳��е�
    pSocket->moveToThread(pThread);               //��tcp���ƶ����µ��߳�
    pThread->start();//�߳̿�ʼ����

    //���������������߳�ָ��Ķ�Ӧ��ϵ�������ڶϿ�����ʱ��ɾ����Ӧ�߳�ָ��
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

    //�ڴ˿��Ը��µ�¼��IP��ַ�б�
    //�ĳ���֤����ɹ���,��UI������ʾ�ͻ����б�
    //emit signal_clientChange();   //�ͻ���������,�Ͽ���������,�����źţ�֪ͨ����,ˢ�¿ͻ�����Ϣ�б�
    emit sig_CMD_TcpServertoUI(socketDescriptor,SrvSocket_NEWCOMMING); //����������UI
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

            //�Ͽ�֮ǰ�����������ź�  ͨ���ⲻ�Ǳ���ģ���Ϊ�ڶ���ɾ��ʱQt���Զ�ɾ�������е��������ӡ�
            //disconnect
            QtTcpSrvResponderBase *pTcpServerSocket=socketInfo.pSocket;
            disconnect(pTcpServerSocket,SIGNAL(sig_updateClients(QString,int)),this,SLOT(slot_updateClients(QString,int)));
            disconnect(pTcpServerSocket,SIGNAL(sig_DisConnect(int,QtTcpSrvResponderBase*)),this,SLOT(slot_DisConnect(int,QtTcpSrvResponderBase*)));
            disconnect(this,SIGNAL(sig_CMD_UItoSocket(int,int,QByteArray)),pTcpServerSocket,SLOT(slot_CMDfromUI(int,int,QByteArray)));
            disconnect(pTcpServerSocket,SIGNAL(sig_CMD_SocketToUI(int,int,int,QByteArray)),this,SIGNAL(sig_CMD_SocketToUI(int,int,int,QByteArray)));


           // ��˵����Ϊ����������¼�ѭ���д��� ����ֱ��ɾ ��������Ϣѭ����Ͷ��һ����Ϣ��ȫ��ɾ��
           //��Ȼ���绹������Ƕ������Ϣ�����������˵Ļ�����ͱ�����
            //����1
//            pthread->exit();    //quit()�ڲ�����exit();������һ���� void QThread::quit(){ exit(); }
//            pthread->deleteLater();
            //delete pthread;                    //���ӶϿ�,�ͷŶ�Ӧ�߳�
            //����2
            pthread->exit();
            pthread->wait(1000);
            delete pthread;

            //�ͷ�socket��
            delete pTcpServerSocket;    //������������

            printf("delete Qthread of cleint=%X\n",pthread);
            //ListClientSocketINFO.removeAt(i);    //���б����Ƴ�������
           // ListClientSocketINFO.remove(i);    //���б����Ƴ�������

            ListClientSocketInfo.erase(it);
            ++it;

            break;
        }
        else
        {
            ++it;
        }
    }

    //�ͻ���������,�Ͽ���������,�����źţ�֪ͨ����,ˢ�¿ͻ�����Ϣ�б�
    emit sig_CMD_TcpServertoUI(SocketID,SrvSocket_DISCONNECT); //����������UI
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

    //�յ��ⲿģ����Ϣ,ͨ���źŲ�ת����socket,��������socket����

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

