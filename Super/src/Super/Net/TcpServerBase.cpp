#include "TcpServerBase.h"

#include <stdio.h>
#include <string.h>

#include "../Basic/BasicOperation.h"


SrvMsgProcessorBase* CreateSrvMsgProcessorBase(TcpServerBase* parent=0)
{
    return new SrvMsgProcessorBase(parent);
}


void ReleaseSrvMsgProcessorBase(SrvMsgProcessorBase* pMsg)
{
	delete pMsg;
}


TcpServerBase::TcpServerBase(void):
port(DefaultPort)
,pFunCreat(CreateSrvMsgProcessorBase)
{
    SrvThreadPriority=Priority_General;
}

bool SrvMsgProcessorBase::ProcessMessage()
{
    Message msg;
    //循环处理消息
    while (1)
    {
        //读取消息队列，不等等，没有立即返回失败退出
        if (!getMsgQueue()->getMsg(msg,0U))
        {
            return false;
        }

        //如果是定时器超时事件直接处理
        switch (msg.type)
        {
        case BaseMsg_TimeOut:   
            {
                //char* pos=msg.buf;
                //int id=*((int*)pos);
                //pos+=sizeof(int);
                //unsigned int times=*((int*)pos);
                //pos+=sizeof(int);
                //msg.msglen=2*sizeof(int);

                char* pos=msg.buf;
                size_t useLen=0;
                int tmid;
                void* pTimerInfo;
                StreamGet(pos,useLen,tmid);
                StreamGet(pos,useLen,pTimerInfo);

                TimeEvent(tmid,pTimerInfo,0);    //从消息里面解出定时器超时事件才是线程安全的
                continue;
            }
            break;
        }

         MessageComing(msg);
    }
}


void SrvMsgProcessorBase::MessageComing(const Message& msg)
{
    PRINT_FUN_NAME
}

//bool SrvMsgProcessorBase::ProcessMessage(const Message& msg)
//{
//
//}

int SrvMsgProcessorBase::startTimer(unsigned int ms)
{
    return pTcpServerBase->startTimer(ms,this);
}

void SrvMsgProcessorBase::stopTimer(int tm_id)
{
    return pTcpServerBase->stopTimer(tm_id);
}

void SrvMsgProcessorBase::setPeerIP(const char* sIP)
{
    size_t len=strlen(sIP);
    if (len>sizeof(PeerIP)-1)
    {
        len=sizeof(PeerIP)-1;
    }
    strncpy(PeerIP,sIP,len);
}




int SrvMsgProcessorBase::TimeEventConv(int tmid,void* pTimerInfo,void* pUser,void* pData)
{
    SrvMsgProcessorBase* parent=(SrvMsgProcessorBase*)pUser;
    //parent->TimeEvent(tmid,times,0);

    //不能之间调用，有线程问题，通过消息中转
    Message msg;
    msg.type=BaseMsg_TimeOut;
    char* pos=msg.buf;
    size_t useLen=0;
    StreamPut(pos,useLen,tmid);
    StreamPut(pos,useLen,pTimerInfo);
    msg.msglen=useLen;
    parent->getMsgQueue()->pushMsg(msg,false);
    return 0;
}

void SrvMsgProcessorBase::TimeEvent(int tmid,void* pTimerInfo,void* puser)
{

}

SharaDataMgr& SrvMsgProcessorBase::getSharaDataMgr()
{
    return pTcpServerBase->getSharaDataMgr();
}


 
 


//void* SrvMsgProcessorBase::getShareData() const
//{
//    if (pTcpServerBase==NULL)
//    {
//        return NULL;
//    }
//    return pTcpServerBase->getShareData();
//}
//
//Mutex& SrvMsgProcessorBase::getShareDataMutex()
//{
//    return pTcpServerBase->getShareDataMutex();
//}

//void SrvMsgProcessorBase::setShareDataCreateDestoryFun(ShareDataCreateFun creatFun,ShareDataDestoryFun destoryFun,bool bCover)
//{
//    if (pTcpServerBase==NULL)
//    {
//        return ;
//    }
//    return pTcpServerBase->setShareDataCreateDestoryFun(creatFun,destoryFun,bCover);
//}

// SOCKADDR_IN结构取IP地址
//a.S_un.S_addr = inet_addr("192.168.0.74");
//char*p = inet_ntoa(a);
//printf("ip addr is %s\n",p);


/*
Windows上如何玩非阻塞的connect？---让程序员自定义connect函数的超时时间
我们知道， 对于阻塞的socket而言， connect函数也是阻塞的， 我在Windows上测试过，
对于阻塞的socket而言， connect的阻塞时间约为25s(linux上是75s吧， 各个平台都不一样).  
也就是说， 很多时候， 客户端需要等25s才继续往下执行。 我们想象一下， 用户肯定
会不满意啊， 得罪了用户， 那就糟糕了。 那能不能搞个自己设置超时时间的connect函数呢？ 
完全可以！ 在本文中， 我们来学习一下非阻塞connect函数的实现---让程序员自定义connect函数的超时时间。
*/


/*
#include <stdio.h>  
#include <winsock2.h>  
#pragma comment(lib, "ws2_32.lib")  

int main()  
{  
        // 网络初始化  
        WORD wVersionRequested;  
        WSADATA wsaData;  
        wVersionRequested = MAKEWORD(2, 2);  
        WSAStartup( wVersionRequested, &wsaData );  

        // 创建客户端socket(默认为是阻塞socket)  
        SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);  

        // 设置为非阻塞的socket  
        int iMode = 1;  
        ioctlsocket(sockClient, FIONBIO, (u_long FAR*)&iMode);   

        // 定义服务端  
        SOCKADDR_IN addrSrv;  
        addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  
        addrSrv.sin_family = AF_INET;  
        addrSrv.sin_port = htons(8888);  

        // 超时时间  
        struct timeval tm;  
        tm.tv_sec  = 5;  
        tm.tv_usec = 0;  
        int ret = -1;  

        // 尝试去连接服务端  
        if (-1 != connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)))  
        {  
                ret = 1; // 连接成功  
        }  
        else  
        {  
                fd_set set;  
                FD_ZERO(&set);  
                FD_SET(sockClient, &set);  

                if (select(-1, NULL, &set, NULL, &tm) <= 0)  
                {  
                        ret = -1; // 有错误(select错误或者超时)  
                }  
                else  
                {  
                    int error = -1;  
                    int optLen = sizeof(int);  
                    getsockopt(sockClient, SOL_SOCKET, SO_ERROR, (char*)&error, &optLen);   

                    // 之所以下面的程序不写成三目运算符的形式， 是为了更直观， 便于注释  
                    if (0 != error)  
                    {  
                         ret = -1; // 有错误  
                    }  
                    else  
                    {  
                         ret = 1;  // 无错误  
                    }  
        }  
}  


// 设回为阻塞socket  
iMode = 0;  
ioctlsocket(sockClient, FIONBIO, (u_long FAR*)&iMode); //设置为阻塞模式  


// connect状态  
printf("ret is %d\n", ret);  

// 发送数据到服务端测试以下  
if(1 == ret)  
{  
send(sockClient, "hello world", strlen("hello world") + 1, 0);  
}  


// 释放网络连接  
closesocket(sockClient);  
WSACleanup();  

return 0;  
}  

*/


