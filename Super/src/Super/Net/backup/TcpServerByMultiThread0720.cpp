#include "TcpServerByMultiThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>    
#include "../Basic/zSocket.h"

class SrvMsgProcessorPrivate
{
public:
    CZSocket m_socket;
};

CZSocket& SrvMsgProcessorMultiThread::getSocket()
{
    return pImpl->m_socket;
}

SrvMsgProcessorMultiThread* CreateSrvMsgProcessorMultiThread()
{
    return new SrvMsgProcessorMultiThread;
}


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "TcpServerByMultiThread.h"

#include "../Basic/my_aes.h"
#include "../Basic/crc32.h"
#include "../Proj/Noc/my_struct.h"  //报文结构


#include <time.h>

int SrvMsgProcessorMultiThread::ObjCnt = 0;

SrvMsgProcessorMultiThread::SrvMsgProcessorMultiThread():m_status(0),clientType(0)
    ,bufBig(NULL)
    ,pImpl(new SrvMsgProcessorPrivate)
	,sTime("ReadData:")
{
    ObjCnt++;
    printf("NetThread::NetThread() thread count:%d ThreadSize:%d\n", ObjCnt,sizeof(SrvMsgProcessorMultiThread));
    Init();
}

SrvMsgProcessorMultiThread::~SrvMsgProcessorMultiThread()
{
	stopWaitFinish();

    ObjCnt--;
    printf("NetThread::~NetThread(): thread count:%d\n", ObjCnt);
    if (bufBig!=NULL)
    {
        delete[] bufBig;
        bufBig=NULL;
    }
    delete pImpl;
	bStop = true;
}


void SrvMsgProcessorMultiThread::Init()
{
	m_status = RUN;
	bStop = false;
	setLoginTime(time(NULL));    //
}

//bool NetThread::Start()
//{
//    // 	Stop();
//    m_status = RUN;
//    bStop = false;
//    return true;
//}

//bool NetThread::Stop()
//{
//    m_status = STOP;
//
//    Sleep(100);
//    // 	m_socket.Destroy();
//    // 	CActiveThread::stop();
//    return true;
//}

void SrvMsgProcessorMultiThread::run()//doit()
{
    m_status = RUN;
    while(getContinueRun())
    {
        switch(m_status)
        {
        case RUN:
            {
                try
                {
                    ProcessWakeUp();
                }
                catch(int& err)
                {
					printf("ProcessWakeUp() exception:%d \n",err);
                    //pImpl->m_socket.Destroy();
                    m_status = STOP;
                }
            }
            break;
        case STOP:
			ClientDisConnected();
			pImpl->m_socket.Destroy();
			bStop = true;
			printf("NetThread::run() Stop\n");
			//break;
            return;
        case IDLE:
			printf("IDLE status:%d\n",m_status);
            Sleep(1);
            break;
		default:
			printf("Unknow status:%d\n",m_status);
			break;
        }
    }

	ClientDisConnected();
	pImpl->m_socket.Destroy();
	bStop = true;
	printf("NetThread::run() Finish\n");
}


#include "../Basic/SuperTime.h"


static unsigned int ProcessCnt = 0;
bool SrvMsgProcessorMultiThread::ProcessWakeUp()
{
	Sleep(1);

	//if(ProcessCnt++%10 == 0)
	//{
	//	printf("NetThread::ProcessWakeUp() %u\n", ProcessCnt);
	//}


    ProcessMessage();   //消息处理，暂时屏蔽
    //char buf[30000]={0};       //栈空间不能太大，放到类内

    size_t getsize = 0;
    char *pBuf = buf;
    //t_timeout tm = 200;  //100
    if (ReadData(buf,sizeof(KH))<0)
    {
        return false;
    }
    KH* pKH = (KH*)buf;

    //找不到报文头--继续搜寻
    if (pKH->pkgHead!=PKG_HEAD)
    {
        printf("Cmd Error Head:0X%X key:0X%X len:%d\n",pKH->pkgHead,pKH->cmd,pKH->cmd_length);
        while (1)
        {
            if (pKH->pkgHead!=PKG_HEAD)
            {
                for (int i=0;i<sizeof(KH)-1;i++)
                {
                    buf[i]=buf[i+1]; //数据左移
                }
                if (ReadData(&buf[sizeof(KH)-1],1)<0)//将最后一个buf字符读进来
                {
                         return false;
                }
                printf("Find Head Error：0X%X\n",pKH->pkgHead);
            }
            else
            {
                break;//成功读取报文头
            }
        }
    }

    //printf("Cmd Head:0X%X key:0X%X len:%d  ",pKH->pkgHead,pKH->cmd,pKH->cmd_length);
    //printf("Cmd Head:0X%X key:0X%X len:%d  ",pKL->head,pKL->key,pKL->len);

    //bool useNew = false;
    if(pKH->cmd_length>0)
    {
        if(pKH->cmd_length>(BUFFERSIZE-sizeof(KH)))
        {
            printf("big Payload:key:0X%X len:%d\n",pKH->cmd,pKH->cmd_length);

            if (bufBig!=NULL)
            {
                delete[] bufBig;
            }
            bufBig = new char[pKH->cmd_length+sizeof(KH)+10];  //(std::nothrow)
            if(bufBig)
            {
                //useNew = true;
                pBuf = bufBig;
                memcpy(pBuf, pKH, sizeof(KH));
            }
            else
            {
                //大报文处理或清空
                //pKH->cmd = 0xffff;
                return false;      //new 20180105  
            }
        }
        if (ReadData(pBuf+sizeof(KH), pKH->cmd_length)<0)//读取报文内容
        {
                return false;
        }
    }

	unsigned int DataLen=sizeof(KH)+pKH->cmd_length;

    //add by lizulin  屏蔽前面memset(buf, 0, sizeof(this->buf)); 后增加下面这句
    pBuf[DataLen]=0;   //接收缓存后面一位清零，防止取字符串出错

    //printf("Cmd Head:0X%X key:0X%X len:%d\n",pKL->head,pKL->key,pKL->len);

    ProcessDatagram(pBuf,DataLen); 


    if(bufBig)
    {
        delete[] bufBig;
        bufBig= NULL;
    }
    //printf("NetThread::ProcessWakeUp()--End\n");
    return true;
}

int SrvMsgProcessorMultiThread::ReadData(char* buf,unsigned int readLen)
{
    //printf("NetThread::ProcessWakeUp()--Begin\n");
    unsigned int timeWait=readLen/128+500;    //根据要读的数据大小决定等待时长
    t_timeout tm =timeWait;  //100
    size_t getsize=0;
    //printf("m_socket.Receive() readLen:%d getsize:%d tm:%d begin\n",readLen,getsize,timeWait);

	
	//sTime.getBeginTime();
    int err = pImpl->m_socket.Receive(buf,readLen,getsize,&tm);   //改阻塞才有时间相应消息
	//sTime.getEndPrint(1,true);


    //printf("m_socket.Receive() readLen:%d getsize:%d tm:%d end\n",readLen,getsize,timeWait);

    if (err==0)
    {
        return getsize;  //0
    }
    else 
    {
        if(err == CZSocket::WAIT_R_TIMEOUT)
        {
            //printf("socket.Receive() timeout err=%d\n",err);    // printf("err == CZSocket::WAIT_R_TIMEOUT\n");
            //return false;
            return -1;
        }
        //else if(err < 0)
        else
        {
            printf("socket.Receive() err=%d throw \n",err);         
            throw -1;
        }
        //if(getsize!=readLen)
        //{
        //    printf("NetThread::ReadData() size=%d err = %d\n", getsize, err);
        //    throw -1;
        //}
        printf("socket.Receive() err=%d\n",err);
        return -1;
    }
}


int SrvMsgProcessorMultiThread::SendData(char* buf,unsigned int len)
{
    //printf("SendData() --Begin buf:%p len:%d\n",buf,len);
    size_t sent;
    unsigned int waitTime=len/128+500;
    t_timeout tm =waitTime;  ;//500;

	//static HighPrecisionTime sTime("SendData:");
	//sTime.getBeginTime();

    int err = pImpl->m_socket.Send(buf, len, sent, &tm);	    //再改 根据数据长度决定等待时间
	
	//sTime.getEndPrint(1,true);

    //int err = m_socket.Send(buf, len, sent);
    if(sent<len)
    {
        std::cout<<"NetThread::SendData() remainSize="<<len-sent<<std::endl;
    }
    if(err != 0)
    {
        printf("SendData() throw \n");
        throw -1;
    }
    //printf("SendData() --End\n");
    //return true;
    return 0;
}

void SrvMsgProcessorMultiThread::CloseConnect()
{
    m_status = STOP;
    //if (bufBig!=NULL)
    //{
    //    delete[] bufBig;
    //    bufBig=NULL;
    //}
}

void SrvMsgProcessorMultiThread::ClientDisConnected()
{
    printf("NetThread::DisConnected()：ClientType=%d   ",clientType);
    switch(clientType)
    {
        //case CLIENT_PANNER: 
        //	{
        //	 
        //	}
        //	break;
        //case CLIENT_DESIGNER:
        //	printf("ClientName = Designer");
        //	break;
    //default:
        //printf("ClientName = Unknow");
        //break;
    }
    printf("\n");
}

void SrvMsgProcessorMultiThread::ProcessDatagram(char* buf,unsigned int len)
{
	//printf("NetThread::ProcessDatagram()\n");
}

void SrvMsgProcessorMultiThread::ProcessMessage()
{
	//printf("NetThread::ProcessMessage()\n");
}


class NetServerPrivate
{
public:
    CZSocket m_SrvSocket;
    std::list<SrvMsgProcessorMultiThread*> m_listClient;
};

//TcpServerByMultiThread* GetNetServer(unsigned int nInstance)
//{
//	if (nInstance>=NetSrv_Total)
//	{
//		return 0;
//	}
//	static TcpServerByMultiThread gNetServer[NetSrv_Total];
//	return &gNetServer[nInstance];
//}


TcpServerByMultiThread::TcpServerByMultiThread():m_status(0)
    ,pImpl(new NetServerPrivate)
{

}

TcpServerByMultiThread::~TcpServerByMultiThread()
{
	//cleanThread();
    delete pImpl;
}

//bool NetServer::Init()
//{
//	return true;
//}

//bool NetServer::Start()
//{
//	//if(!(status() == /*thread_stopped*/4 || status() == /*thread_ready*/0))
//	//{
//	//	return false;
//	//}
//	//if(isStopped())
//	//	cleanThread();
//
//
//	if(!start())
//		return false;
//
//	m_status = RUN;
//	return true;
//}

//void NetServer::cleanThread()
//{
//	m_status = STOP;
//	pImpl->m_SrvSocket.Destroy();
//	//CActiveThread::stop();
//}

bool TcpServerByMultiThread::Stop()
{
	m_status = STOP;
	//Sleep(100);
	stopWaitFinish();

	pImpl->m_SrvSocket.Destroy();
	std::list<SrvMsgProcessorMultiThread*>::iterator itor;
	for (itor = pImpl->m_listClient.begin(); itor != pImpl->m_listClient.end(); ++itor)
	{
		SrvMsgProcessorMultiThread* client = *itor;
	
		//client->stop();
		//client->waitFinish();
		//while(!client->isRunning()) 
		//{
		//	Sleep(10);
		//}

		delete client;
	}
	pImpl->m_listClient.clear();
	return true;
}



bool TcpServerByMultiThread::InitSocket()
{
    return true;
}

bool TcpServerByMultiThread::Bind()
{
    return true;
}

bool TcpServerByMultiThread::Listen()
{
    return true;
}

bool TcpServerByMultiThread::Open()
{
    if (!InitSocket())
    {
        return false;
    }
    if (!Bind())
    {
        return false;
    }
    if (!Listen())
    {
        return false;
    }

    return true;
}







void TcpServerByMultiThread::run() //doit()
{
	char str[512];
	bool bSuccess = false;//bind success
	int i = 0;
	//while(m_status != STOP && bSuccess == false)
	while(getContinueRun()&& bSuccess == false) //getContinueRun()??
	{
		try
		{
			if((i % 5) == 0)
			{
				if (pImpl->m_SrvSocket.Create(AF_INET, SOCK_STREAM, 0))
				{
					struct sockaddr_in addr_in;
					memset(&addr_in, 0, sizeof(sockaddr_in));
					addr_in.sin_family = AF_INET;
					addr_in.sin_port = htons(getPort());
					addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
					int error = 0;
					do 
					{
						t_timeout tm = 3000;
						if(pImpl->m_SrvSocket.Bind(&addr_in))
						{
							if (!pImpl->m_SrvSocket.Listen(20))
								throw -1;
							bSuccess = true;
						}
						else
							throw -2;

					} while (error);
				}
				else
				{
					//DPRINTF("Create failure\n");
				}
				i = 0;
			}
			else
			{
				i++;
				Sleep(1000);//等4s
			}
		}
		catch (int &a)
		{
			pImpl->m_SrvSocket.Destroy();
			//switch(a)
			//{
			//case -1:
			//	sprintf(str, "Server Socket Listen error: %d.", status());
			//	break;
			//case -2:
			//	sprintf(str, "Server Socket Bind error: %d.", status());
			//	break;
			//default:
   //             sprintf(str, "Server Socket unknow error %d.", status());
			//}
#ifdef LOG
			if (gLog)
			{
				gLog->Write(LOG_ERROR, str);
			}
#endif
			i++;
			Sleep(1000);
			//		return;
		}
	}
	
	m_status=RUN; //new
	while(getContinueRun())//while(1)
	{
		switch(m_status)
		{
		case RUN:
			{
				if (pNetThread == NULL)
				{
					//pNetThread = new NetThread;
                    //根据不同的NetServer设置不同的pNetThread(多态),NetServer没有多态，只能用函数指针

					pNetThread =(SrvMsgProcessorMultiThread*)pFunCreat();
					getMsgQueue()->addChildren(pNetThread->getMsgQueue());
				}

				t_timeout tm = 100*100;
				//每个ClientSocket：m_pThread->m_socket放到m_pThread中去运行
				int retAccept=pImpl->m_SrvSocket.Accept(pNetThread->getSocket(), NULL, NULL, &tm);
				if(retAccept==0)
				{
					std::list<SrvMsgProcessorMultiThread*>::iterator itor;
					/*
					for (itor = m_listClient.begin(); itor != m_listClient.end(); ++itor)
					{
						NetThread* client = *itor;
						if(client != NULL)
							if(client->bStop)
							{
								delete client;
								m_listClient.erase(itor);
// 								*itor = NULL;
								itor = m_listClient.begin();//除去已停止的线程
							}
					}
					*/

					std::list<SrvMsgProcessorMultiThread*>::iterator endItor = pImpl->m_listClient.end();
					for (itor = pImpl->m_listClient.begin(); itor != endItor;)
					{
						SrvMsgProcessorMultiThread* client = *itor;
						if(client!=NULL&&!client->isRunning())   ////if(client->bStop)
						{
					            getMsgQueue()->removeChild(client->getMsgQueue());
								
								delete client;
								itor=pImpl->m_listClient.erase(itor);  //
								//*itor = NULL;
								//itor = m_listClient.begin();//除去已停止的线程
						}
						else
						{
							++itor;
						}
					}
					pImpl->m_listClient.push_back(pNetThread);
					pNetThread->Init();
					pNetThread->start();
					pNetThread = NULL;
				}
				else
				{
                    printf("Accept failed:%d err:%d\n ",retAccept,GetLastError());
				}
			}
			break;
		case STOP:
			Stop();
			printf("NetServer Stop\n");
			return;
		case IDLE:
			Sleep(40);
			break;
		default:
			printf("Unknow status:%d\n",m_status);
			break;
		}
	}
}

