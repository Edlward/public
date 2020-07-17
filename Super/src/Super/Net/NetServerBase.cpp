#include "NetServerBase.h"

#include <stdio.h>
#include <string.h>

#include "zSocket.h"

//static NetServer gNetServer;
//NetServer* GetNetServer()
//{
//    return &gNetServer;
//}

class NetThreadBasePrivate
{
public:
    CZSocket m_socket;
};

NetThreadBase::NetThreadBase(NetServerBase* parent):pParentServer(parent)
{
    pimpl=new NetThreadBasePrivate;
}

NetThreadBase::~NetThreadBase()
{
     Stop();
    delete pimpl;
}

CZSocket& NetThreadBase::getSocket()
{
    return pimpl->m_socket;
}

unsigned short NetThreadBase::getPort()
{
    return getSocket().getPort();
}

std::string NetThreadBase::getServerIP()
{
    //return getSocket().getServerIP();
    return getSocket().getPeerIP();    //peer ip ���ǶԷ�������IP,��ȷ��
}

SharaDataMgr& NetThreadBase::getSharaDataMgr()
{
    return pParentServer->getSharaDataMgr();
}

bool NetThreadBase::Stop()
{
    m_status = STOP;
    Thread::stopWaitFinish();
    return true;
}

void NetThreadBase::run()
{
    PRINT_FUN_TRACE

    m_status=RUN;
    printf("NetThread::doit()--Begin status=%d\n",m_status);
    while(getContinueRun())  //while(1) 
    {
        switch(m_status)
        {
        case RUN:
            {
                try
                {
                    ProcessFilter();
                }
                catch(int& e)
                {
                    printf("ProcessFilter() catch:%d\n",e);
                    //getSocket().Destroy();
                    m_status = STOP;
                }
            }
            //printf("NetThread::RUN-End\n");
            break;
        case STOP:
            {

            bRunCompleted = true;
            ClientDisConnected();   //new ����Ͽ��������
            printf("NetThread stop & exit\n");
            getSocket().Destroy();

            return;  //return ��while����ѭ��
            }
            break;
        case IDLE:
            printf("IDLE: sleep(1)");//20180105
            Thread::sleep_for(1);
            break;
        default:
            printf("Unknow: status=%d\n",m_status);//20180105
            break;
        }
    }
}



int NetThreadBase::SendData(const char* buf,unsigned int len)
{

    //old ��mac�µ�juce����������Է��������ݣ����������ڴ�


    int ret=getSocket().SendData(buf,len);
    if(ret<0)
    {
        throw -1;
    }
    return ret;
}

int NetThreadBase::ReadData(char* buf,unsigned int len)
{
    int ret=getSocket().ReadData(buf,len);
    if(ret<0)
    {
        throw -1;
    }
    return ret;
}


bool NetThreadBase::ProcessFilter()
{
#if 0
    //char buf[30000];       //ջ�ռ䲻��̫�󣬷ŵ�����
    //��Ӧ��ÿ����գ��˷����ܣ����ǲ���ջ�Ӱ�칤���б��ȡ��������
    //memset(buf, 0, sizeof(this->buf));    //����û�б�Ҫ

    size_t getsize = 0;
    char *pBuf = buf;
    //t_timeout tm = 200;  //100
    if (ReadData(buf,sizeof(KL))<0)
    {
        return -1;
    }
    KL* pKL = (KL*)buf;

    //�Ҳ�������ͷ--������Ѱ
    if (pKL->head!=KL_HEAD)
    {
        printf("Cmd Error Head:0X%X key:0X%X len:%d\n",pKL->head,pKL->key,pKL->len);
        while (1)
        {
            if (pKL->head!=KL_HEAD)
            {
                for (int i=0;i<sizeof(KL)-1;i++)
                {
                    buf[i]=buf[i+1]; //��������
                }
                if (ReadData(&buf[sizeof(KL)-1],1)<0)
                {
                    return -1;
                }
            }
            else
            {
                break;
            }
        }
    }
    //printf("Cmd Head:0X%X key:0X%X len:%d  ",pKL->head,pKL->key,pKL->len);
    //ProcessMessage();   //��ʱ����
    bool useNew = false;
    if(pKL->len>0)
    {
        if(pKL->len>(BUFFERSIZE-sizeof(KL)))
        {
            printf("big Payload:key:0X%X len:%d\n",pKL->key,pKL->len);
            char *pBuf1 = new char[pKL->len+sizeof(KL)+10];  //(std::nothrow)
            if(pBuf1)
            {
                useNew = true;
                pBuf = pBuf1;
                memcpy(pBuf, pKL, sizeof(KL));
                //err = m_socket.Receive(pBuf + sizeof(KL), pKL->len, getsize);
                //err = m_socket.Receive(pBuf + sizeof(KL), pKL->len, getsize,&tm);
            }
            else
            {
                //���Ĵ�������
                pKL->key = 0xffff;
                return -1;  //new 20180105
            }
        }
        if (ReadData(pBuf+sizeof(KL), pKL->len)<0)
        {
            return -1;
        }
    }

    unsigned int msgLen=sizeof(KL)+pKL->len;

    //add by lizulin  ����ǰ��memset(buf, 0, sizeof(this->buf)); �������������
    pBuf[msgLen]=0;   //���ջ������һλ���㣬��ֹȡ�ַ�������
    //printf("Cmd Head:0X%X key:0X%X len:%d\n",pKL->head,pKL->key,pKL->len);

    ProcessDatagram(pBuf,msgLen); 
    if(useNew)
    {
        delete[] pBuf;
    }
    //printf("NetThread::ProcessFilter()--End\n");
    return false;
#endif

    return true;
}

void NetThreadBase::ClientDisConnected()
{
    //    printf("NetThread::ClientDisConnected()��ClientType=%d   ",clientType);
}

bool NetThreadBase::ErrorProtocol(char* buf)
{
    return true;
}

bool NetThreadBase::ProcessMessage()
{
        return true;
}

bool NetThreadBase::ProcessDatagram(char* buf,unsigned int msgLen)
{
    return true;
}

NetThreadBase* CreateNetThreadBase(NetServerBase* parent)
{
    return new NetThreadBase(parent);
}




class NetServerBasePrivate
{
public:
    CZSocket m_SrvSocket;  
};


NetServerBase::NetServerBase()
   :pFunCreat(CreateNetThreadBase)  //��ʼ��һ�����Ĵ���������������Ӧ�ø���ҵ���������ⲿ����
{
    m_status=STOP;
    m_pThread=0;
    pimpl=new NetServerBasePrivate;
}

NetServerBase::~NetServerBase()
{
    //cleanThread();
    Stop();
    delete pimpl;
}

CZSocket& NetServerBase::getSocket()
{
    return pimpl->m_SrvSocket;
}

bool NetServerBase::Init()
{
    return true;
}

bool NetServerBase::Start()
{
    //if(!(status() == /*thread_stopped*/4 || status() == /*thread_ready*/0))
    //{
    //    return false;
    //}
    //if(isStopped())
    //    cleanThread();
    //if(!start())
    //    return false;

    m_status = RUN;
    Thread::start();
    return true;
}

//void NetServerBase::cleanThread()
//{
//    m_status = STOP;
//    getSocket().Destroy();
//    //CActiveThread::stop();
//}

bool NetServerBase::Stop()
{
    m_status = STOP;
    Thread::stopWaitFinish();
    CleanListClient();
    return true;
}


void NetServerBase::CleanListClient()
{
    PRINT_FUN_NAME

    getSocket().Destroy();
    std::list<NetThreadBase*>::iterator itor;
    for (itor = m_listClient.begin(); itor != m_listClient.end(); ++itor)
    {
        NetThreadBase* client =*itor;
        //�ڲ��Լ�����
        //client->Stop();
        //while(!client->isStop())
        //{
        //    Thread::sleep_for(100);
        //}
        //client->stop();   
        delete client;
    }
    m_listClient.clear();
}

void NetServerBase::StopListClient()
{
    //PRINT_FUN_NAME
    PRINT_FUN_TRACE
    std::list<NetThreadBase*>::iterator itor;
    for (itor = m_listClient.begin(); itor != m_listClient.end(); ++itor)
    {
        NetThreadBase* client =*itor;
        //client->stopWaitFinish();
        client->Stop();
    }
}


std::string NetServerBase::getServerIP()
{
    //return getSocket().getServerIP();
     return sServerIP;
}

void NetServerBase::run()
{
    PRINT_FUN_TRACE

    m_status = RUN;
    //char str[512];
    bool bSuccess = false;
    int i = 0;
    while(m_status != STOP && bSuccess == false)
    {
        try
        {
            if((i % 5) == 0)
            {
                if (getSocket().Create(AF_INET, SOCK_STREAM,0))
                {
                    PreDprintf("After Create()");
                    getSocket().printIPInfo();

                    struct sockaddr_in addr_in;
                    memset(&addr_in, 0, sizeof(sockaddr_in));
                    addr_in.sin_family = AF_INET;
                    addr_in.sin_port = htons(port);
                    addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
                    int error = 0;
                    do 
                    {
                        t_timeout tm = 3000;
                        if(getSocket().Bind(&addr_in))
                        {
                                PreDprintf("After Bind()");
                                getSocket().printIPInfo();

                            if (!getSocket().Listen(20))
                            {
                                 PreDprintf("After Listen()");
                                 getSocket().printIPInfo();

                                throw -1;
                            }
                            bSuccess = true;
                        }
                        else
                        {
                            throw -2;
                        }

                    } while (error);
                }
                else
                {
                    //DPRINTF("Create failure\n");
                    PRINT_ERROR_LINE
                    printf("Socket().Create failed\n");
                }
                i = 0;
            }
            else
            {
                i++;
                Thread::sleep_for(1000);
            }
        }
        catch (int &a)
        {
            getSocket().Destroy();
            switch(a)
            {
            case -1:
                ::printf("Server Socket Listen error:\n");
                break;
            case -2:
                ::printf("Server Socket Bind error:\n");
                break;
            default:
               ::printf("Server Socket unknow error:\n");
                break;
            }
            i++;
            Thread::sleep_for(1000);
            //		return;
        }
    }


    while(getContinueRun())         //while(1)
    {
        switch(m_status)
        {
        case RUN:
            {
                if (m_pThread == NULL)
                {
                    m_pThread =pFunCreat(this);   // new NetThreadBase;
                    //m_pThread->setParentServer(this);
                }
                t_timeout tm = 100;
                if(getSocket().Accept(m_pThread->getSocket(), NULL, NULL, &tm) == 0)
                {
                    //sServerIP=m_pThread->getSocket().GetSockName();
                    sServerIP=m_pThread->getSocket().getPeerIP();
                    std::string sClientIP=m_pThread->getSocket().GetSockName();
                    printf("Accept: ServerIP:%s clientIP:%s\n",sServerIP.c_str(),sClientIP.c_str());

                    PRINT_TRACE

                    std::list<NetThreadBase*>::iterator it;
                    for (it = m_listClient.begin(); it != m_listClient.end();)  //++itor  //ע���ڱ�����ɾ����������ȷ�÷�
                    {
                        NetThreadBase* client = *it;
                        if(client != NULL)
                        {
                            if(client->isRunCompleted())
                            {
                                getMsgQueue()->removeChild(client->getMsgQueue());  //&client->MsgQueue
                                //printf("will delete Client =%p\n",client);
                                delete client;
                                it=m_listClient.erase(it);
                            }
                            else
                            {
                                it++;
                            }
                        }
                        else
                        {
                            it++;
                        }
                            //printf("itor =%p\n",(void*)&(*it));
                    }
                    m_listClient.push_back(m_pThread);
                    getMsgQueue()->addChildren(m_pThread->getMsgQueue());   //����Ӷ��� &m_pThread->MsgQueue

                    m_pThread->Init();
                    m_pThread->Start();
                    //if (m_pThread->status() == brunt::thread_ready)
                    //{
                    //    m_pThread->start();
                    //    printf("Start thread %s\n", m_pThread->m_socket.GetSockName());
                    //}
                    m_pThread = NULL;
                }
            }
            break;
        case STOP:
            break;
            //return;
        case IDLE:
            Thread::sleep_for(40);
            break;
        }
    }

    CleanListClient();   //�˳�ʱ�ù�������ͻ���
}

