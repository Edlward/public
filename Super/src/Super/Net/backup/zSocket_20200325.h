#ifndef _ZZSOCKET_H_20080606__
#define _ZZSOCKET_H_20080606__

#include <string>
//#include "socketdef.h"



//#include "socket.h"  //new add by me


//#ifdef DLLDEMO_EXPORTS
//#define DLL_DEMO _declspec( dllexport )
//#else
//#define DLL_DEMO _declspec(dllimport)
//#endif
//extern "C" DLL_DEMO int Add(int a, int b);


//extern "C" {     


	//#include "io.h"
//#include "socket.h"    //直接包含文件内容

//#ifndef SOCKET_H
//#define SOCKET_H
/*=========================================================================*\
* Socket compatibilization module
* LuaSocket toolkit
*
* BSD Sockets and WinSock are similar, but there are a few irritating
* differences. Also, not all *nix platforms behave the same. This module
* (and the associated usocket.h and wsocket.h) factor these differences and
* creates a interface compatible with the io.h module.
*
* RCS ID: $Id: socket.h,v 1.20 2005/11/20 07:20:23 diego Exp $
\*=========================================================================*/

//#include "socketdef.h"	// added by liang 2008.06.06

#ifdef _WIN32
		//#include "wsocket.h"
		/*=========================================================================*\
		* WinSock include files
		\*=========================================================================*/
		//#include <winsock.h>
		#include <WinSock2.h>
        #pragma comment(lib, "WS2_32")    //new add by me 显示包含或者使用者包含ws2_32.lib
		typedef int socklen_t;
		typedef SOCKET t_socket;
		typedef t_socket *p_socket;

		#define SOCKET_INVALID (INVALID_SOCKET)
#else
       //#include "usocket.h"
		//#ifndef _WIN32  //move by me
		/*=========================================================================*\
		* Socket compatibilization module for Unix
		* LuaSocket toolkit
		*
		* RCS ID: $Id: usocket.h,v 1.7 2005/10/07 04:40:59 diego Exp $
		\*=========================================================================*/

		/*=========================================================================*\
		* BSD include files
		\*=========================================================================*/
		/* error codes */
		#include <errno.h>
		/* close function */
		#include <unistd.h>
		/* fnctnl function and associated constants */
		#include <fcntl.h>
		/* struct sockaddr */
		#include <sys/types.h>
		/* socket function */
		#include <sys/socket.h>
		/* struct timeval */
		#include <sys/time.h>
		/* gethostbyname and gethostbyaddr functions */
		#include <netdb.h>
		/* sigpipe handling */
		#include <signal.h>
		/* IP stuff*/
		#include <netinet/in.h>
		#include <arpa/inet.h>
		/* TCP options (nagle algorithm disable) */
		#include <netinet/tcp.h>

		typedef int t_socket;
		typedef t_socket *p_socket;

		#define SOCKET_INVALID (-1)

		//#endif

#endif



        //from #include "timeout.h"	
       // ///* timeout control structure */
       // typedef struct t_timeout_ {
       //     double block;          /* maximum time for blocking calls */
       //     double total;          /* total number of miliseconds for operation */
       //     double start;          /* time of start of operation */
       // } t_timeout;
       // typedef t_timeout *p_timeout;

       // //int timeout_open(lua_State *L);
       // void timeout_init(p_timeout tm, double block, double total);
       // double timeout_get(p_timeout tm);
       // double timeout_getretry(p_timeout tm);
       // p_timeout timeout_markstart(p_timeout tm);
       // double timeout_getstart(p_timeout tm);
       // double timeout_gettime(void);
       // //int timeout_meth_settimeout(lua_State *L, p_timeout tm);
       //#define timeout_iszero(tm)   ((tm)->block == 0.0)


#if 0		// deleted by liang 2008.06.06
	//#include "timeout.h"	
#else		// liang

	typedef long t_timeout;              //remove by lizulin
	typedef t_timeout *  p_timeout;   //
	#define TO_INFINITE				((t_timeout*)(-1))
	#define timeout_iszero(tm)		((void*)tm==0)
	#define timeout_infinite(tm)	    ((void*)tm==TO_INFINITE)
#endif		// liang

/* we are lazy... */
typedef struct sockaddr SA;













/*=========================================================================*\
* Functions bellow implement a comfortable platform independent 
* interface to sockets
\*=========================================================================*/
int socket_open(void);
int socket_close(void);
void socket_destroy(p_socket ps);
void socket_shutdown(p_socket ps, int how); 
int socket_sendto(p_socket ps, const char *data, size_t count, 
        size_t *sent, SA *addr, socklen_t addr_len, p_timeout tm);
int socket_recvfrom(p_socket ps, char *data, size_t count, 
        size_t *got, SA *addr, socklen_t *addr_len, p_timeout tm);

void socket_setnonblocking(p_socket ps);
void socket_setblocking(p_socket ps);
bool socket_setblocking(p_socket ps,unsigned int tmout);

int socket_waitfd(p_socket ps, int sw, p_timeout tm);
int socket_select(t_socket n, fd_set *rfds, fd_set *wfds, fd_set *efds, 
        p_timeout tm);

int socket_connect(p_socket ps, SA *addr, socklen_t addr_len, p_timeout tm); 
int socket_create(p_socket ps, int domain, int type, int protocol);
int socket_bind(p_socket ps, SA *addr, socklen_t addr_len); 
int socket_listen(p_socket ps, int backlog);
int socket_accept(p_socket ps, p_socket pa, SA *addr, 
        socklen_t *addr_len, p_timeout tm);

const char *socket_hoststrerror(int err);
const char *socket_strerror(int err);

/* these are perfect to use with the io abstraction module 
   and the buffered input module */
int socket_send(p_socket ps, const char *data, size_t count, 
        size_t *sent, p_timeout tm);
int socket_recv(p_socket ps, char *data, size_t count, size_t *got, p_timeout tm);
const char *socket_ioerror(p_socket ps, int err);

int socket_gethostbyaddr(const char *addr, socklen_t len, struct hostent **hp);
int socket_gethostbyname(const char *addr, struct hostent **hp);

const char* socket_name(p_socket ps);

//#endif /* SOCKET_H */

//}  //end  extern "C"


//using namespace std;



/*
//new 
#ifdef _WIN32
//#include <winsock.h>
#include <winsock2.h> 
#pragma comment(lib, "WS2_32")    //或者显示包含ws2_32.lib
#endif
*/

 

#define ZSOCKET_API


// CZSocket error code define ->
#define ZSOCKET_ERROR_BEGIN		50000
#define ZSOCKET_ERROR_END		50100
#define ZSE_TEST(a)				(a>=ZSOCKET_ERROR_BEGIN && a<=ZSOCKET_ERROR_END)
#define ZSE_SET(a)				(ZSOCKET_ERROR_BEGIN+a)
#define ZSE_SOCKETINIT			ZSE_SET(1)
#define ZSE_HASBEENOPEND		ZSE_SET(2)		// has been opened
#define ZSE_INETADDR			ZSE_SET(3)		// inet_addr error
// <-

#define ZSOCKET_FAILED(a)		((a)!=0)
#define ZSOCKET_SUCCEED(a)		((a)==0)

class CZSocketExcption;

class ZSOCKET_API CZSocket
{
public:
	// Wait type
	enum{WAIT_READ = 1, WAIT_WRITE = 2, WAIT_EXCEPTION = 4};
	// Wait return
	enum{WAIT_R_DONE=0, WAIT_R_TIMEOUT=-1, WAIT_R_CLOSED=-2, WAIT_R_UNKNOWN=-3};

	bool IsOpened();
	bool Create(int domain, int type, int protocol);
	void Destroy();
	bool Listen(int backlog);
	bool Bind(const sockaddr_in * addr_in);
	void SetBlocking(bool bBlock);

    //add by lizulin 设置是否阻塞,非阻塞,第二个参数表示等待时间
    bool setBlockMode(bool bBlock,unsigned int ms=0);

	int Accept(CZSocket& as, char* ip, int* port, t_timeout* tm = 0);
	int Connect(const sockaddr_in *addr_in, t_timeout* tm = 0);
	int Send(const char* buff, int size, size_t& sent, t_timeout* tm = 0);
	int Receive(char* buff, int size, size_t& getsize, t_timeout* tm = 0);    //阻塞读数据
	int Receive2(char* buff, int size, size_t& getsize, t_timeout* tm = 0);  //非阻塞读数据
	int SendTo(const char* buff, int size, size_t& sent, const sockaddr_in *addr_in, t_timeout* tm = 0);
	int ReceiveFrom( char* buff, int size, size_t& getsize, const sockaddr_in *addr_in, t_timeout* tm = 0);
	int Wait(int type, t_timeout* tm = 0);
	int GetLastError(char* buff = NULL, int bufsize = 0);
	std::string GetSockName();   

    //在Accept成功之后,可以获取当前socket服务器所绑定的网卡在网络种的IP地址
    sockaddr_in getSockAddr()const {return m_addrin;}
    std::string getServerIP();

    //区分localport 与peer port 待续
    unsigned short getPort()
    {
        return ntohs(m_addrin.sin_port);
    }
public:
	t_socket	m_socket;

protected:
	int			m_error;
	sockaddr_in	m_addrin;
    //SA	m_addrin;        //by lizulin 20190624

	int  m_bInitOK;
	int  m_nRefCount;		// all socket objects used count 

	std::string m_sockName;

public:
	CZSocket(void);
	virtual ~CZSocket(void);
	//~CZSocket(void);   //Modify by lizulin
};

class ZSOCKET_API CZSocketExcption
{
	int error;
	 
public:
	CZSocketExcption(int e):error(e)
	{};
	int GetError(){return error;};
	const std::string& GetErrorMsg()
	{
		static std::string s;
		return s;
	}
};


inline void printSocket(const struct sockaddr_in& SocketAddr)
{

#ifdef _WIN32
    unsigned short port=::ntohs(SocketAddr.sin_port);
    char ip[32]={0};
    char* tmp=::inet_ntoa(SocketAddr.sin_addr);
    ::strcpy(ip,tmp);
    ::printf("Socket strIP:%s Port:%d\n",ip,port);
    ::printf("IP Value:%d:%d:%d:%d\n"
        ,SocketAddr.sin_addr.S_un.S_un_b.s_b1
        ,SocketAddr.sin_addr.S_un.S_un_b.s_b2
        ,SocketAddr.sin_addr.S_un.S_un_b.s_b3
        ,SocketAddr.sin_addr.S_un.S_un_b.s_b4);
  #endif
}






#endif//_ZZSOCKET_H_20080606__
