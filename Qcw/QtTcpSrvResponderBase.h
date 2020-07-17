#ifndef TCPSERVERSOCKET_H
#define TCPSERVERSOCKET_H

#include <QtNetwork>
#include <QThread>

#include "Super/Basic/MessageQueue.h"

enum CMD_UItoSocket //UI��socket�̵߳�ͨ�����UI������socket
{
    UI_TEST,                                 //���������UI����,����socket������ͻ���
    UI_SenDataDirect,                  //UIֱ�ӷ�����������
    UI_SenDatatoUI,                    //�����������ݸ�UI
    UI_MsgQueueData,               //�ڲ���Ϣ����TcpServerת����socket
};

enum CMD_TcpServertoUI   //TcpServertoUI
{
    SrvSocket_NEWCOMMING=0X3000,   //���µ�client���ӵ���
    SrvSocket_DISCONNECT           //��socket�Ͽ�
};


//enum EMConst
//{
//    //Sever����Ҫ����������ǰ׺S   Client�˼�C ̫��
//    //CMD_TEST=0X999,                        //�Զ����������
//    //CMD_ZERO=0x0000,                     //���cmd_Sub
//    //PKG_HEAD=0x55,                           //���ݰ�ͷ
//    //CMD_REV=0x78563412,                 //�Զ��屣����
//    //CMD_REV_LEONIS=0x74752485,    //�Զ���leonis������
//};


//�Ƿ���Ըĳ��߳���
//Qt Server �������Ϣ��Ӧ������,��ͬҵ����Ҫ,�̳к�,����ʵ��һЩ�麯����������


class QtTcpSrvFrame;
class QtTcpSrvResponderBase : public QTcpSocket//,public QThread
{
    Q_OBJECT
public:
	void run();
public:
    QtTcpSrvResponderBase(int socketDescriptor,QObject *parent = 0);
     virtual ~QtTcpSrvResponderBase();

     //��������
     virtual bool ProcessDatagram(char* buf,unsigned int len);    //���籨�Ĵ���,�������ĵ���������
     virtual void UnknowProtocol(char* buf,unsigned int len);      //δ֪����
     //virtual bool ProcessMessage();                                           //������Ϣ
     virtual bool ProcessMessage(const Message& msg);            //������Ϣ--�����ڲ���ͬģ��ͨ�ŵ���Ϣ����
     virtual void ClientConnected();                                              //�ͻ����״����ӱ�����
     virtual void ClientDisConnected();                                          //�ͻ��˶Ͽ�������

	virtual void CloseConnect(); //��������,�Ͽ��ͻ�������


public:
     //����printf����������Ϊ�˴�ӡ��ͬʱ����Ϣ���͵�UI��ʾ��ע�ͺ��Զ�����ȫ��::printf��ӡ������̨
    void printf(const char* fmt, ...);   
    void timerEvent(QTimerEvent * te);
private:
    int socketID;        //����id ����this->socketDescriptor()�ͻ��˶Ͽ��ᱻ�ͷ�,�Ͽ��źŵò�����ȷֵ
    int timer_toDO;    //��ʱ�����������
public:
    int SendData(char* buf,int size,int val=0);  //���CRC�ֶ�,Ȼ����ܷ��� val��ʾ����ƫ��,���ط���ʧ�ܵ��ֽ���
//�����������źŴ��νṹ��,�ٸĽ�,ע�������ͻ��ȫ�������SL�ṹ�塣


signals:
	void sig_PrintMsg(QString sMsg);               //���ʹ�ӡ��Ϣ��UI��ʾ
    void sig_DataReady(int socketID);
    void sig_DisConnect(int socketID,QtTcpSrvResponderBase* TcpServerSocket);  //ͬʱ���ݵ�ǰ����ָ��,
	void sig_updateClients(QString,int);      //�����½�仯
	void sig_CMD_SocketToUI(int SocketID,int cmdtype,int val,QByteArray ba="");   //socket�̷߳��͸�UI������
    void sig_getSocketData();    //�Լ���˼��,���������ȴ�ĳ�������,����slot_DataReceived()��

private:
    //�����ݷ���1: ��������غ�����д--��֤�ɿ���ȡ����
    char bufStack[500*1024];
    bool isblocking;

    int  RecvDataProcess();                     //��ȡ���绺������ݲ�����,���ض�ȡ�����ֽ������������źŴ������ֶ�����
    bool WaitForData();                          //�ȴ�����
    int  ClearSocketBuffer();                   //�Զ������Socket������
    int  ReadNbytes(char* buf,int size);  //��ȡN���ֽ�  ��int seconds); //��N���ֽ� ���ó�ʱ�˳�

    //�����ݷ���2��˼·����ÿ�ζ�ȡ������Ч���ݣ��浽�󻺳�����100M,��ַ�ۼ�ƫ��,Ȼ��whileѭ��������������
    //˼·����bug,��ʱ���ý��,�����ʱhead�պ�һ������bufβ��һ������ͷ�����ǳ����ô�����Ҫ�����⿼�ǣ�����
    enum 
    {
        BufAllSize=10*1024*1024  //�ܵ����ݻ����С
    };
    char pBufBig[BufAllSize];      //û��Ҫnew,��Ϊ��ǰ��һ��new��ʹ��
    char* pBufStart;    //���ݻ�����ʼ��ַ
    char* pBufEnd;      //���ݻ��������ַ
    char* pStart;       //δ��������ʼ��ַ
    char* pEnd;         //δ�����Ľ�����ַ
    void InitReadMethod2();    //��ʼ�����ϱ���
    void  RecvAllDataToProc();             //һ�λ�ȡ��������
    unsigned int  getRemainBufSize();              //��ȡʣ�໺����ռ��С
    unsigned int  getUnProcBufSize();              //��ȡδ������Ч���Ŀռ��С
    int  ReadNbytesforWhile(char* buf,int size);  //ֱ�Ӷ�ȡN���ֽ�,���ȴ�
    int  ReadDatatoBuf(int readSize);     //�������ݴ�С�����ݵ����棬��Ϊ����β����ַ�����Ҫ�ֶζ�ȡ
    void  ProcessAll();

    //���ֶ�ȡ������ͬ���ñ��Ĵ�����
    int  FilterProcess(char* buf,int size);      //���������������δ���

    int SendRandData();                   //��������ķ��Ͳ��ԶԷ������߼��Ƿ�����
    int getSocketID(){return socketID;}
public slots:
    void slot_DataReceived();      
    void slot_Connected();      //�����socket��������Server��ܹ���,ʵ�����ã�����
    void slot_Disconnected();
    void slot_CMDfromUI(int SocketID,int cmdtype,QByteArray ba);   //	����UI������������
    void Error(QAbstractSocket::SocketError socketError);
public:
    void do_sendData();                     //���Ͳ������ݺ���
    void do_sendJoyStickData();         //���ⷢ�Ͳ��ݸ˵���������
    void do_test();                              //���Ժ���
    void sendDataToUI(char* buf,unsigned int len);
//������Բ�ͬӦ�ó������չ
private:

public:
//��panner
  void process_SetMetadata(char* buf);      //�յ�panner��������SetMetadata
  void do_SetMetadataResp();                //����һ��ȷ���յ�SetMetadata����
  void Resp_RandData(char *buf, unsigned int len);   //������Ա���


public:
  //�ṩһ����������ͻ��̷߳��ʹ������ݵĹ�����ӿ�,�����÷���SharaDataMgr˵��
  SharaDataMgr& getSharaDataMgr();

protected:

};


QtTcpSrvResponderBase* CreateTcpSrvResponderBase(unsigned short socketDescriptor,QtTcpSrvFrame* parent=0);


typedef  QtTcpSrvResponderBase* (*pMsgResponder)(unsigned short socketDescriptor,QtTcpSrvFrame* parent);

#endif 

