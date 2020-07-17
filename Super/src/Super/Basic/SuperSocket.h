
#ifndef _SUPERSOCKET_H_
#define _SUPERSOCKET_H_

#include "global.h"

/*
linux and windows
*/
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#ifdef _WIN32
#include <winsock2.h>


#pragma comment(lib,"ws2_32.lib")
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include<netinet/tcp.h>


#define SOCKET int
#endif

#pragma warning (disable: 4786)

#include <string>
#include <vector>
#include <map>
using namespace std;

#define SOCKET_TIMEOUT 1
#define MAX_RECV_BUFFERSIZE 1024*2
//延迟
#define WAIT_FOREVER -1


NAME_SPACE_MAIN_BEGIN

class SuperSocket
{
public:
	SOCKET m_sock;
    int a;
public:
    SuperSocket();
    ~SuperSocket();  //add by lizulin
    bool         Create(bool bUDP=false);
    virtual bool Connect(const string& host,unsigned short port);
    virtual bool isConnected();
    int         IsConnectOK(unsigned long ulTimeOut = 0);
    virtual bool isUDP();
    virtual bool Bind(unsigned short nPort);
    virtual bool Accept(SuperSocket& client);
    virtual void CloseSocket();
    virtual long Send(const void* buf,long buflen);
    virtual long Recv(void* buf,long buflen,long lTimeOut = 0);
    virtual long SendTo(const char* buf,int len,const struct sockaddr_in* toaddr,int tolen);
    virtual long RecvFrom(char* buf,int len,struct sockaddr_in* fromaddr,int* fromlen);
    virtual bool GetPeerName(string& strIP,unsigned short &nPort);
    virtual bool GetClientHostName(string& strClientHostName);
    //设置接收缓存区的大小
    bool    SetRecvBuffSizeof(int iBuffSizeof);
    //获取接收缓存区大小
    int    GetRecvBuffSizeof();

    SOCKET       GetHandle();
private:
    bool           m_bUDP;
    bool           m_bConnected;
    string         m_strHost;
    unsigned short m_nPort;
};

/*
struct arpreq{
  struct sockaddr arp_pa; //协议地址
  struct sockaddr arp_ha; //硬件地址
  int arp_flags;//标志位
  struct sockaddr arp_netmask;//网络掩码
  char arp_dev[16];
}
*/


NAME_SPACE_MAIN_END
#endif

