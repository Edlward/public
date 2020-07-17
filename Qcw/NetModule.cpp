#include "NetModule.h"

#include "QTcpClient.h"


#include "../Proj/ProtocolNetHead.h"      //网络通信协议数据报文头/理论上网络底层不应该依赖报文头结构，暂时如此




NetModule::NetModule(void)
{
    PRINT_FUN_TRACE
    pTcpClient=new QTcpClient;
    qRegisterMetaType<StuMessage>("StuMessage");
}

NetModule::~NetModule(void)
{
    PRINT_FUN_TRACE
    //在内部完成清理操作
    if (pTcpClient!=NULL)
    {
        delete pTcpClient; 
        pTcpClient=NULL;
    }
}

void NetModule::InstallNetWork(const std::string &sServerIP, unsigned short port)
{
    PRINT_FUN_TRACE
    printf("NetModule::InstallNetWork() IP:%s Port:%d\n",sServerIP.c_str(),port);

    //QTcpClient* pTcpClient = new QTcpClient();
    pTcpClient->setServerIPPort(sServerIP, port);

    //关联Socket线程发送命令给UI
    //connect(pTcpClient,SIGNAL(sig_CmdToOutSide(int,QByteArray,char*,int,bool)),this,
    //    SLOT(slot_CmdFromSocket(int,QByteArray,char*,int,bool)),Qt::UniqueConnection);
    //connect(this,SIGNAL(sig_CmdToSocket(int,QByteArray,char*,int,bool)),
    //    pTcpClient,SLOT(slot_CmdFromOutSide(int,QByteArray,char*,int,bool)),Qt::UniqueConnection);     

    //改进版,暂时保留两份
    connect(pTcpClient,SIGNAL(sig_CmdToOutSide(int,StuMessage)),this,
        SLOT(slot_CmdFromSocket(int,StuMessage)),Qt::UniqueConnection);
    connect(this,SIGNAL(sig_CmdToSocket(int,StuMessage)),
        pTcpClient,SLOT(slot_CmdFromOutSide(int,StuMessage)),Qt::UniqueConnection);     

    //pTcpClient->start(); 
}


void NetModule::ChangeNetServer(const std::string &sServerIP,unsigned short port)
{
    PRINT_FUN_TRACE
    printf("NetModule::ChangeNetServer() IP:%s Port:%d\n",sServerIP.c_str(),port);
    pTcpClient->setServerIPPort(sServerIP,port); //设置IP与端口
    pTcpClient->stop();
    pTcpClient->start();
}


unsigned short NetModule::getLocalPort()
{
	return pTcpClient->getPort();
}


std::string NetModule::getPeerIP()
{
    return    pTcpClient->getIp();
}

unsigned short NetModule::getPeerPort()
{
    return   pTcpClient->getPeerPort();
}

void NetModule::start()
{
    pTcpClient->start(); 
}

void NetModule::stop()
{
    pTcpClient->stop(); 
}

QTcpSocket& NetModule::getQtcpSocket()
{
    return pTcpClient->getQTcpSocket();
}

void NetModule::Send(const char* buf,int len)
{ 
        //printHex(buf,len);
       //网络线程不在运行,不用发数据否则消息会被推送到事件循环，容易栈溢出
        if (!pTcpClient->isRunning()&&pTcpClient->getNetState()!=QTcpClient::NetState_Connected)
        {
            //printf("NetModule::Send() break!\n");
            return;
        }


        //QByteArray ba(buf,len);
        //emit sig_CmdToSocket(QTcpClient::SK_DataToSocket,ba);  

        //用自定义结构更高效
        emit sig_CmdToSocket(QTcpClient::SK_DataToSocket,StuMessage(buf,len));  
}

//
//void NetModule::slot_CmdFromSocket(int type,QByteArray ba,char* buf,int len,bool isNew)
//{
//    switch(type)
//    {
//    case QTcpClient::NetState_Connecting:       
//    case QTcpClient::NetState_Connected:        
//    case QTcpClient::NetState_DisConnected:       
//        NetworkStateChanged(type,ba,buf,len);
//        break;
//    case QTcpClient::SK_DataFromSocket: 
//        {
//            char*  bufData=buf;
//            int dataLen=len;
//            if (buf==NULL)
//            {
//                bufData=ba.data();
//                dataLen=ba.size();
//            }
//            DataFromSocket(bufData,dataLen);
//            //DataFromSocket(buf,len);
//        }
//        break;
//    default:   break;
//    }
//
//    if (isNew)       //传过来的buf是new出来的在此delete[]
//    {
//        delete[] buf;
//    }
//}


void NetModule::slot_CmdFromSocket(int type,StuMessage msgData)
{
    //printf("NetModule::slot_CmdFromSocket() StuMessage\n");
    //PRINT_FUN_NAME


    switch(type)
    {
    case QTcpClient::NetState_Connecting:       
    case QTcpClient::NetState_Connected:        
    case QTcpClient::NetState_DisConnected:       
        NetworkStateChanged(type,0,0,0);
        break;
    case QTcpClient::SK_DataFromSocket: 
        {
            //return;

            DataFromSocket(msgData.pData,msgData.dataLen);
            msgData.DestoryNoLongerUse();   //处理完成,不再传递,手动释放
        }
        break;
    default:   break;
    }
}

void NetModule::DataFromSocket(char* buf,int len)
{
    //printf("NetModule::DataFromSocket()\n");
    //printHex(buf,len);

    KL* pKL = (KL*)buf;
    //// 对接收数据做正确性检查 [12/19/2016 killerlife]  没有必要
    //if(pKL->head != 0x7585)
    //	return;

    //////////////////////////////////////////////////////////////////////////
    unsigned short Key=pKL->key;
    //MessageManager& Msm=MessageManager::getInstance();
    //Msm.doAutoCallback(Key,buf,len);   //回调模拟

    mManger.doEvenvCallofMsgReceived(Key,buf,len);   //回调模拟
    return;
}

void NetModule::NetworkStateChanged(int type,QByteArray ba,char* buf,int len)
{
    PRINT_FUN_NAME

    switch(type)
    {
    case QTcpClient::NetState_Connecting:
        {
            printf("Connecting...\n");	
        }
        break;
    case QTcpClient::NetState_Connected:  //网络连接成功后的操作
        {
            mManger.doEventCallofNetConnected();
            printf("Connected...\n");
        }
        break;
    case QTcpClient::NetState_DisConnected: //网络连接断开后的操作
        {
            mManger.doEventCallofNetDisConnected();
            printf("Disconnect...\n");
        }
        break;
    default: 
        break;
    }
}

