#ifndef _NetManger_H
#define _NetManger_H

#include <QObject>
//#include <QtNetwork>
#include <QTcpSocket>  //�����ڲ�����������Ҫ


#include "MessageRegistrar.h"


//����һ�����Դ��¼��ص�������ȡNetModule����Socketָ��,�Ա������������,�Լ������������ʱ���֣�δ�����
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
    void ChangeNetServer(const std::string &sServerIP,unsigned short port);   //����IP��˿����³�ʼ��
	unsigned short getLocalPort();   //�ͻ��˱����˿�

	std::string getPeerIP();               //��ȡ��Ӧ����˵�IP--���ӳɹ���
	unsigned short getPeerPort();    //��ȡ��Ӧ����˵Ķ˿�

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
    //MessageSender mSenser;          //��Ϣ������
    MessageRegistrar mManger;      //���Ļص�ע�������
};

#endif







