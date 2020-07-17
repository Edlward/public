#include <errno.h>
#include "SuperSocket.h"
#include<stdio.h>
#ifndef _WIN32
#define strnicmp strncasecmp
#else
#endif

//#include"BaseDefine.h"


#include<string.h>

NAME_SPACE_MAIN_BEGIN

//using namespace std;
SuperSocket::SuperSocket()
{
    m_bUDP = false;
    m_sock = -1;
    m_nPort = 0;
    m_bConnected = false;
}

SuperSocket::~SuperSocket()
{
	CloseSocket();   //析构前关闭  add by 
}

SOCKET SuperSocket::GetHandle()
{
    return m_sock;
}

void SuperSocket::CloseSocket()
{

    if(m_sock != -1)
    {
#ifdef _WIN32
        shutdown(m_sock,SD_BOTH);
        closesocket(m_sock);	
		WSACleanup();    //清理网络环境    add me
#else
        shutdown(m_sock, SHUT_RDWR);
        close(m_sock);
#endif
        m_sock = -1;
    }
    m_bConnected = false;
}

bool SuperSocket::Create(bool bUDP)
{

#ifdef _WIN32   //add me
	//初始化WSA  必须要加这几行代码
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA wsaData;
	if(WSAStartup(sockVersion, &wsaData)!=0)
	{
		return false;
		//return -1;
	}
#endif


    m_bUDP = bUDP;
    if(!m_bUDP)
    {
        m_sock = socket(AF_INET,SOCK_STREAM,0);   //IPPROTO_IP);
        //LOGW( "Create m_sock :%d", m_sock);
    }
    else
    {
        m_sock = socket(AF_INET,SOCK_DGRAM,0);  //IPPROTO_UDP);
		printf("socket(AF_INET,SOCK_DGRAM,0)=%d\n",m_sock);
        if(m_sock!=-1)
        {
            m_bConnected =  true;
        }
    }

    return (m_sock!=-1);
}

bool SuperSocket::Connect(const string& host,unsigned short port)
{
    if(m_sock==-1)
        return false;

    m_strHost=host;
    m_nPort=port;

    struct hostent * he=gethostbyname(host.c_str());
    if(he==NULL)
    {
        CloseSocket();
        return false;
    }
    struct sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_addr=*((struct   in_addr*)he->h_addr);
    memset(sin.sin_zero,0,8);
    sin.sin_port=htons(port);
    if(connect(m_sock ,(struct sockaddr *)&sin,sizeof(sin)))
    {
        CloseSocket();
        return false;
    }
    //unsigned long rb=1;
    //ioctlsocket(m_sock,FIONBIO,&rb);

    int bufsize = MAX_RECV_BUFFERSIZE;
    setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char*)&bufsize, sizeof(bufsize));
    //int timeout= 20000;
    //setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    m_bConnected =  true;
    return true;
}

long SuperSocket::Send(const void* buf,long buflen)
{

    if(m_sock==-1)
    {
        return -1;
    }
    long nRet = -1;
    nRet = send(m_sock,(const char*)buf,buflen,0);
    return nRet;
    /* fd_set fd;
    FD_ZERO(&fd);
    FD_SET(m_sock, &fd);
    struct timeval  val = {SOCKET_TIMEOUT,0};
    int selret = select(m_sock+1,NULL,&fd,NULL,&val);
    if (selret <= 0)
    {
    return selret;
    }
    */
}

long SuperSocket::Recv(void* buf,long buflen,long lTimeOut)
{
    //Windows
    if(m_sock==-1)
    {
        return -1;
    }
    int selret = -1;
    long nRet = -2;
    if(lTimeOut == WAIT_FOREVER)
    {
        // 需要永久等待,就不需要对其进行查询是否有数据可以读取了
        selret = 1;
    }
    else
    {

        fd_set fd;
        FD_ZERO(&fd);
        FD_SET(m_sock, &fd);
        struct timeval  val;
        //val.tv_sec  = SOCKET_TIMEOUT ;
        val.tv_sec  = lTimeOut ;
        val.tv_usec = 0;
        //指向一组等待可读性检查到接口
        selret = select(m_sock+1,&fd,NULL,NULL,&val);
    }
    //有数据时 selret>0   否则 selret小于0
    if((selret > 0) && (buf != NULL) && (buflen > 0))
    {
        nRet = recv(m_sock,(char*)buf,buflen,0);
        if(nRet == 0)
        {
            // 表示连接被关闭了
            CloseSocket();
            nRet = 0;
        }
        else if(nRet < 0)
        {
            //nRet<0 表示出错了
            CloseSocket();
            nRet = -1;
        }
    }
//    else
//   {
//        nRet = -1;
//    }
    return nRet;


    //Linux
/*
   if(m_sock==-1)
    {
        return -1;
    }
    fd_set fd;
    FD_ZERO(&fd);
    FD_SET(m_sock, &fd);
    struct timeval  val = {SOCKET_TIMEOUT,0};
    int selret = select(m_sock+1,&fd,NULL,NULL,&val);
    if (selret <= 0)
    {
        return selret;
    }
    int len=recv (m_sock,buf,buflen,0);
    //printf("Recv: %.*s\r\n",len,buf);
    return len;
*/
}

bool SuperSocket::GetPeerName(string& strIP,unsigned short &nPort)
{
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    int addrlen=sizeof(addr);

#ifdef _WIN32
    if(getpeername(m_sock,(struct sockaddr*)&addr,&addrlen)!=0)
#else
    if(getpeername(m_sock,(struct sockaddr*)&addr,(socklen_t*)&addrlen)!=0)
#endif
        return false;

    char szIP[64];
#ifdef _WIN32
    sprintf(szIP,"%u.%u.%u.%u",addr.sin_addr.S_un.S_addr&0xFF,(addr.sin_addr.S_un.S_addr>>8)&0xFF,(addr.sin_addr.S_un.S_addr>>16)&0xFF,(addr.sin_addr.S_un.S_addr>>24)&0xFF);
#else
    sprintf(szIP,"%u.%u.%u.%u",addr.sin_addr.s_addr&0xFF,(addr.sin_addr.s_addr>>8)&0xFF,(addr.sin_addr.s_addr>>16)&0xFF,(addr.sin_addr.s_addr>>24)&0xFF);
#endif
    strIP=szIP;
    nPort=ntohs(addr.sin_port);
    return true;
}


bool SuperSocket::GetClientHostName(string &strClientHostName)
{
    return false;
}

bool SuperSocket::SetRecvBuffSizeof(int iBuffSizeof)
{
     if( setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, (char *)&iBuffSizeof, sizeof(iBuffSizeof) ) < 0 )
     {
            perror("getsockopt: ");
            return false;
     }
     else
         return true;
}

int SuperSocket::GetRecvBuffSizeof()
{

    int rcvbuf_len;
#ifdef _WIN32
    int len = sizeof(rcvbuf_len);
#else
    socklen_t len=sizeof(int);
#endif
    if( getsockopt( m_sock, SOL_SOCKET, SO_RCVBUF, (char *)&rcvbuf_len, &len ) < 0 ){

       perror("getsockopt: ");

        }

   return rcvbuf_len;
}


bool SuperSocket::isConnected()
{
    return (m_sock!=-1)&&m_bConnected;
}

int SuperSocket::IsConnectOK(unsigned long ulTimeOut)
{
    bool bRet = 0;
    #ifndef _WIN32
    ulTimeOut = ulTimeOut/1000;
    #else
    #endif


    if(m_sock==-1)
    {
        return -1;
    }
    fd_set WriteSet;
    FD_ZERO(&WriteSet);
    FD_SET(m_sock,&WriteSet);
    struct timeval  val ;
    val.tv_sec = ulTimeOut;
    val.tv_usec = 0;
    //指向一组待可写性检查到接口
    int Select = select(m_sock+1,NULL,&WriteSet,NULL,&val);
    if(Select > 0)
    {
        bRet = Select;
    }
    else
         bRet = Select;

    return bRet;
}

bool SuperSocket::isUDP()
{
    return m_bUDP;
}

long SuperSocket::SendTo(const char* buf,int len,const struct sockaddr_in* toaddr,int tolen)
{
    if(m_sock==-1)
    {
        return -1;
    }

	int ret=sendto(m_sock,buf,len,0,(const struct sockaddr*)toaddr,tolen);
	
	if (ret<0)
	{
        //printf("m_sock=%d sendtoERR=%d \n",m_sock,WSAGetLastError());   //add me


		//错误编号 10038 使用原始套接字需要管理员权限，用管理员权限运行exe 就好了
	}

	return ret;
}

long SuperSocket::RecvFrom(char* buf,int len,struct sockaddr_in* fromaddr,int* fromlen)
{

    if(m_sock==-1)
    {
        return -1;
    }

	//printf("m_sock=%d\n",m_sock);
	int ret=0;
#ifdef _WIN32
    //return 
	ret=recvfrom(m_sock,buf,len,0,(struct sockaddr*)fromaddr,fromlen);
#else
    //return 
	ret=recvfrom(m_sock,buf,len,0,(struct sockaddr*)fromaddr,(socklen_t*)fromlen);
#endif

	if (ret<0)
	{
        // printf("m_sock=%d  recvfrom=%d \n",m_sock,WSAGetLastError());
	}

	return ret;
}

bool SuperSocket::Bind(unsigned short nPort)
{
    if(m_sock==-1)
    {
        return false;
    }

    struct sockaddr_in sin;
    sin.sin_family=AF_INET;
#ifdef _WIN32
    //sin.sin_addr.S_un.S_addr=0;            //old
	 sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);  
	 sin.sin_addr.s_addr=htonl(INADDR_ANY) ;// 接收任意IP发来的数据
#else
    sin.sin_addr.s_addr=0;
#endif
    memset(sin.sin_zero,0,8);
    sin.sin_port=htons(nPort);

	
    if(bind(m_sock,(sockaddr*)&sin,sizeof(sockaddr))!=0)
    //if(bind(m_sock,(sockaddr*)&sin,sizeof(sockaddr_in))!=0)
        return false;


	if(m_bUDP==false)   //如果是UDP不需要listen，modify me
	{
		listen(m_sock,1024);
		//m_bConnected = true;
	}

	m_bConnected = true;
    return true;
}


bool SuperSocket::Accept(SuperSocket& client)
{
    if(m_sock==-1)
    {
        return false;
    }

    client.m_sock = accept(m_sock,NULL,NULL);
    client.m_bConnected = true;

    return (client.m_sock != -1);
}


NAME_SPACE_MAIN_END
