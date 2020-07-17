#ifndef NetServerBase_H
#define NetServerBase_H

//�򵥰��TcpServer��װ
//ԭNetSever ��NetThread�ļ��׷�װ����ҪΪ�˸���NetServer,


#if defined(_WIN32)
#if defined(NET_API_EXPORT)
#define NET_API __declspec(dllexport)
#elif defined(NET_API_IMPORT)
#define NET_API __declspec(dllimport)
#else
#define NET_API 
#endif
#else
#define NET_API 
#endif


#include <string>
#include <list>
#include "../Basic/MessageQueue.h"
#include "../Basic/Thread.h"

#include "NetBase.h"



USING_NAME_SPACE_MAIN

class CZSocket;
class NetThreadBase;
class NetThreadBasePrivate;
class NetServerBase;


class NetThreadBase: public Thread
{
public:
    enum RUN_STATUS
    {
        IDLE = 0,
        RUN,
        STOP,
    };
private:
    S_DISABLE_COPY(NetThreadBase)
public:
     NetThreadBase(NetServerBase* parent=0);
     virtual ~NetThreadBase();
     virtual void Init(){}
     virtual bool Start()
     {
         return Thread::start();
     }
     bool Stop();
     CZSocket& getSocket();

     unsigned short getPort();;
     std::string getServerIP();

     bool isRunCompleted() {return bRunCompleted;}
     void doDisConnect()   //����������Ͽ�����--��ȷ��
     {
         Stop();
     }

     //��½ʱ��,�ϲ��Srv��ܵ���
     void setLoginTime(uint64_t t){LoginTime=t;}
     uint64_t getLoginTime()const {return LoginTime;}

     //���ʱ��
     void setLastAliveTime(uint64_t t){lastAliveTime=t;}
     uint64_t getLastAliveTime()const {return lastAliveTime;}

public:
    //�����������ָ��,Ϊ��Ӧ�ò���չ��Ҫ
    NetServerBase* getParentServer()const {return pParentServer;}    
    void setParentServer(NetServerBase* pS){pParentServer=pS;}
    MessageQueue* getMsgQueue(){return &MsgQueue;}   //��Ϣ����

    //�ṩһ����������ͻ��̷߳��ʹ������ݵĹ�����ӿ�,�����÷���SharaDataMgr˵��
    SharaDataMgr& getSharaDataMgr();
protected:
    MessageQueue MsgQueue;
    NetServerBase* pParentServer;
protected:
     enum{BUFFERSIZE=50*1024};       //��Ƶ��Ŀ��������֡40��k������Ԥ��--��ģ�����,����
     char buf[BUFFERSIZE];                  //�ŵ�����
     volatile int m_status;
     volatile bool bRunCompleted;   //�Ƿ��Ѿ�������ɱ�ǣ�û�����й�����

    int64_t LoginTime;       //��½ʱ��,�ͻ����ӵ���ʱ���棬�����õ� 1970��1��1��00:00:00�����ڵ�����
    int64_t lastAliveTime;   //���ʱ��,����������¼�������ʱ��,�ÿ���������ʱ��,��λms
public:
    virtual void run();
    virtual bool ProcessFilter();     
    int SendData(const char* buf,unsigned int len);         //��װ���ݷ��ͺ���������Ҫÿ�ε��� �ڲ�m_socketȥSend
    int ReadData(char* buf,unsigned int len);           //ʧ�ܷ���-1��������

    virtual void ClientDisConnected();                                  //���类���Ͽ�ʱ�����ã����ݿͻ������Ͳ�ͬ����ͬ�������
    virtual bool ErrorProtocol(char* buf);                             //����Э����󱻵���
    //virtual bool UnknowFunction(char* buf);                       //δ֪���ĵ���������
    virtual bool ProcessMessage();                                      //������Ϣ
    virtual bool ProcessDatagram(char* buf,unsigned int msgLen);  //���ݱ��ĵ���������
protected:
     NetThreadBasePrivate* pimpl;
     friend class NetServerBase;
};

NetThreadBase* CreateNetThreadBase(NetServerBase* parent=0);



//CreateResponderOfNetSrv

//typedef  NetThreadBase* (*CreateResponderFun)(NetServerBase* parent);




class NetServerBasePrivate;
class  NET_API NetServerBase: public Thread
{

typedef  NetThreadBase* (*CreateResponderFun)(NetServerBase* parent);

public:
    enum RUN_STATUS
    {
        IDLE = 0,
        RUN,
        STOP,
    };
private:
    S_DISABLE_COPY(NetServerBase)
public:
    NetServerBase();
    virtual ~NetServerBase();
    bool Init();
    bool Start();
    bool Stop();
    void CleanListClient(); //�����ͷ����Կͻ������߳�
    void StopListClient();

    int GetStatus() {return m_status;}


    //��ȡ���������������е�IP��ַ,����ʵ�ֿ������ϲ㸲��
    virtual std::string getServerIP();

    //set get Server Port
    void setPort(unsigned short n){port=n;}
    unsigned short getPort(){return port;}
private:
    virtual void run();
protected:
    int m_status;
    CZSocket& getSocket();//�������CZSocket�ڲ�����windows.h��pimpl���� by lizulin 
    std::string sServerIP;
    unsigned short port;
    std::list<NetThreadBase*> m_listClient;
    NetThreadBase *m_pThread;
public:
    MessageQueue* getMsgQueue(){return &MsgQueue;}
    void setCreatFun(NetServerBase::CreateResponderFun pFun){pFunCreat=pFun;}            //���÷�������ҵ������

    //�ṩһ����������ͻ��̷߳��ʹ������ݵĹ�����ӿ�,�����÷���SharaDataMgr˵��
    SharaDataMgr& getSharaDataMgr(){return m_SharaDataMgr;}
protected:
    MessageQueue MsgQueue;
    SharaDataMgr m_SharaDataMgr;
    CreateResponderFun pFunCreat;
    NetServerBasePrivate* pimpl;
};

//NET_API NetServer* GetNetServer();

#endif






