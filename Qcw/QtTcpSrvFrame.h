#ifndef SERVER_H
#define SERVER_H

#include "QtTcpSrvResponderBase.h"

#include <QtNetwork>
#include <QHostAddress>
#include <QThread>

#include "Super/Basic/MessageQueue.h"

#include "Super/Net/NetBase.h"   //����������ͻ��̹߳������ݷ���


//���������������߳�ָ��Ķ�Ӧ��ϵ�������ڶϿ�����ʱ��ɾ����Ӧ�߳�ָ��
struct SrvSocketInfo
{
    int SocketID;                           //
    QtTcpSrvResponderBase*  pSocket;   //�������Ӧָ��
    QThread* pThread;               //������Ҫ�ƶ������߳�ָ��
};

class QtTcpSrvFrame : public QTcpServer		
{
    Q_OBJECT
public:
    explicit QtTcpSrvFrame(int port=0,QObject *parent=0);
   ~QtTcpSrvFrame();
   //����printf����������Ϊ�˴�ӡ��ͬʱ����Ϣ���͵�UI��ʾ��ע�ͺ��Զ�����ȫ��::printf��ӡ������̨
   void printf(const char* fmt, ...);
    bool OpenServer();     //�򿪷�����,ʧ�ܾ�ɾ����ǰ����
public:
	int m_port;    //���漴��ʹ�õ�QTcpServer�Ķ˿�
     std::vector<SrvSocketInfo> ListClientSocketInfo;
protected:
#if (QT_VERSION>0x050000)                  //�������QtCore  �����ҵ��˺궨��
     virtual void incomingConnection(qintptr handle);      //QT4��QT5�˺������岻һ��������汾��ȷ��
#else
     void incomingConnection(int socketDescriptor);
#endif

signals:
	void sig_PrintMsg(QString sMsg);               //���ʹ�ӡ��Ϣ��UI��ʾ
    void sig_DataReady(int socketID);

   //�ͻ���������,�Ͽ���������,�����źţ�֪ͨ����,ˢ�¿ͻ�����Ϣ�б�
    void sig_updateServerUI(QString,int);
    void sig_CMD_TcpServertoUI(int SocketID,int cmdtype); //����������UI
    void sig_CMD_UItoSocket(int SocketID,int cmdtype,QByteArray ba);   //����UI��������������ת
	void sig_CMD_SocketToUI(int SocketID,int cmdtype,int val,QByteArray ba);   //�ź���ת

public slots:
    void slot_updateClients(QString,int);
    void slot_ReadData(int socketID);
    void slot_DisConnect(int socketID,QtTcpSrvResponderBase* TcpServerSocket);



    //������Ϣ������Ӧ�����
public:
        void setResponderCreatFun(pMsgResponder pFun){pCreatFun=pFun;}            //���÷�������ҵ������
private:
        pMsgResponder pCreatFun;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//��Ϣ�������
public:
    void startPollingMsg();   //������ʱ����ѯ��Ϣ�����Ƿ�����Ϣ����,
    void stopPollingMsg();   //ֹͣ��ѯ��ʱ��
private:
    QTimer tmtodo;
public slots:
    void slot_timetoDo();
public:
    MessageQueue* getMsgQueue(){return &MsgQueue;}

    //�ṩһ����������ͻ��̷߳��ʹ������ݵĹ�����ӿ�,�����÷���SharaDataMgr˵��
    SharaDataMgr& getSharaDataMgr(){return m_SharaDataMgr;}
private:
    MessageQueue MsgQueue;
    SharaDataMgr m_SharaDataMgr;
};

#endif

