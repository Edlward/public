//#include "stdafx.h"
#include "zSocket.h"

//原socket.c内容直接放到此处





#ifdef _WIN32
//wsocket.c




/*=========================================================================*\
* Socket compatibilization module for Win32
* LuaSocket toolkit
*
* The penalty of calling select to avoid busy-wait is only paid when
* the I/O call fail in the first place. 
*
* RCS ID: $Id: wsocket.c,v 1.36 2007/06/11 23:44:54 diego Exp $
\*=========================================================================*/
#include <string.h>

//#include "io.h"

//#include "socket.h"   //delete by me

/* IO error codes */
enum {
    IO_DONE = 0,        /* operation completed successfully */
    IO_TIMEOUT = -1,    /* operation timed out */
    IO_CLOSED = -2,     /* the connection has been closed */
	IO_UNKNOWN = -3
};

/* WinSock doesn't have a strerror... */
static const char *wstrerror(int err);

/*-------------------------------------------------------------------------*\
* Initializes module 
\*-------------------------------------------------------------------------*/
int socket_open(void) {
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 0); 
    int err = WSAStartup(wVersionRequested, &wsaData );
    if (err != 0) return 0;
    if ((LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) &&
        (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)) {
        WSACleanup();
        return 0; 
    }
    return 1;
}

/*-------------------------------------------------------------------------*\
* Close module 
\*-------------------------------------------------------------------------*/
int socket_close(void) {
    WSACleanup();
    return 1;
}

/*-------------------------------------------------------------------------*\
* Wait for readable/writable/connected socket with timeout
\*-------------------------------------------------------------------------*/
#define WAITFD_R        1
#define WAITFD_W       2
#define WAITFD_E        4
#define WAITFD_C        (WAITFD_E|WAITFD_W)

int socket_waitfd(p_socket ps, int sw, p_timeout tm) {
    int ret;
    fd_set rfds, wfds, efds, *rp = NULL, *wp = NULL, *ep = NULL;
    struct timeval tv, *tp = NULL;
    if (timeout_iszero(tm)) return IO_TIMEOUT;  /* optimize timeout == 0 case */
    if (sw & WAITFD_R) 
		{ FD_ZERO(&rfds); FD_SET(*ps, &rfds); rp = &rfds; }
    if (sw & WAITFD_W) 
		{ FD_ZERO(&wfds); FD_SET(*ps, &wfds); wp = &wfds; }
    if (sw & WAITFD_C) 
		{ FD_ZERO(&efds); FD_SET(*ps, &efds); ep = &efds; }
#if 0	// deleted by liang 2008.06.06
    double t;
    if ((t = timeout_get(tm)) >= 0.0) {
        tv.tv_sec = (int) t;
        tv.tv_usec = (int) ((t-tv.tv_sec)*1.0e6);
        tp = &tv;
    }
#else
		if(!timeout_infinite(tm)) {
			tv.tv_sec = (*tm)/1000;
			tv.tv_usec = (*tm)%1000*1000;
			tp = &tv;
		}
#endif
    ret = select(0, rp, wp, ep, tp);
    if (ret == -1) return WSAGetLastError();
    if (ret == 0) return IO_TIMEOUT;
    if ((sw & WAITFD_C) && FD_ISSET(*ps, &efds)) 
		return IO_CLOSED;
    return IO_DONE;
}

/*-------------------------------------------------------------------------*\
* Select with int timeout in ms
\*-------------------------------------------------------------------------*/
int socket_select(t_socket n, fd_set *rfds, fd_set *wfds, fd_set *efds, 
        p_timeout tm) {
    struct timeval tv; 
#if 0	// deleted by liang 2008.06.06
    double t = timeout_get(tm);
    tv.tv_sec = (int) t;
    tv.tv_usec = (int) ((t - tv.tv_sec) * 1.0e6);
    if (n <= 0) {
        Sleep((DWORD) (1000*t));
        return 0;
    } else return select(0, rfds, wfds, efds, t >= 0.0? &tv: NULL);
#else	// liang
	if(tm && *tm > 0) {
		tv.tv_sec = 0;
		tv.tv_usec = 1000*(*tm);
		return select(0, rfds, wfds, efds, &tv);
	} else {
		return select(0, rfds, wfds, efds, NULL);
	}
#endif	// liang
}

/*-------------------------------------------------------------------------*\
* Close and inutilize socket
\*-------------------------------------------------------------------------*/
void socket_destroy(p_socket ps) {
    if (*ps != SOCKET_INVALID) {
        socket_setblocking(ps); /* close can take a long time on WIN32 */
        closesocket(*ps);
        *ps = SOCKET_INVALID;
    }
}

/*-------------------------------------------------------------------------*\
* 
\*-------------------------------------------------------------------------*/
void socket_shutdown(p_socket ps, int how) {
    socket_setblocking(ps);
    shutdown(*ps, how);
    socket_setnonblocking(ps);
}

/*-------------------------------------------------------------------------*\
* Creates and sets up a socket
\*-------------------------------------------------------------------------*/
int socket_create(p_socket ps, int domain, int type, int protocol) {
    *ps = socket(domain, type, protocol);
    if (*ps != SOCKET_INVALID) return IO_DONE;
    else return WSAGetLastError();
}

/*-------------------------------------------------------------------------*\
* Connects or returns error message
\*-------------------------------------------------------------------------*/
int socket_connect(p_socket ps, SA *addr, socklen_t len, p_timeout tm) {
    int err;
    /* don't call on closed socket */
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    /* ask system to connect */
    if (connect(*ps, addr, len) == 0) return IO_DONE;
    /* make sure the system is trying to connect */
    err = WSAGetLastError();
    if (err != WSAEWOULDBLOCK && err != WSAEINPROGRESS) return err;
    /* zero timeout case optimization */
    if (timeout_iszero(tm)) return IO_TIMEOUT;
    /* we wait until something happens */
    err = socket_waitfd(ps, WAITFD_C, tm);
    if (err == IO_CLOSED) {
        int len = sizeof(err);
        /* give windows time to set the error (yes, disgusting) */
        Sleep(10);
        /* find out why we failed */
        getsockopt(*ps, SOL_SOCKET, SO_ERROR, (char *)&err, &len); 
        /* we KNOW there was an error. if 'why' is 0, we will return
        * "unknown error", but it's not really our fault */
        return err > 0? err: IO_UNKNOWN; 
    } else return err;

}

/*-------------------------------------------------------------------------*\
* Binds or returns error message
\*-------------------------------------------------------------------------*/
int socket_bind(p_socket ps, SA *addr, socklen_t len) {
    int err = IO_DONE;
//    socket_setblocking(ps);
    if (bind(*ps, addr, len) < 0) err = WSAGetLastError();
//    socket_setnonblocking(ps);
    return err;
}

/*-------------------------------------------------------------------------*\
* 
\*-------------------------------------------------------------------------*/
int socket_listen(p_socket ps, int backlog) {
    int err = IO_DONE;
//    socket_setblocking(ps);
    if (listen(*ps, backlog) < 0) err = WSAGetLastError();
//    socket_setnonblocking(ps);
    return err;
}

/*-------------------------------------------------------------------------*\
* Accept with timeout
\*-------------------------------------------------------------------------*/
int socket_accept(p_socket ps, p_socket pa, SA *addr, socklen_t *len, 
        p_timeout tm) {
    SA daddr;
    socklen_t dlen = sizeof(daddr);
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    if (!addr) addr = &daddr;
    if (!len) len = &dlen;
    for ( ;; ) {
        int err;
        /* try to get client socket */
        if ((*pa = accept(*ps, addr, len)) != SOCKET_INVALID) return IO_DONE;
        /* find out why we failed */
        err = WSAGetLastError(); 
        /* if we failed because there was no connectoin, keep trying */
        if (err != WSAEWOULDBLOCK && err != WSAECONNABORTED) return err;
        /* call select to avoid busy wait */
        if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err;
    } 
    /* can't reach here */
    return IO_UNKNOWN; 
}

/*-------------------------------------------------------------------------*\
* Send with timeout
* On windows, if you try to send 10MB, the OS will buffer EVERYTHING 
* this can take an awful lot of time and we will end up blocked. 
* Therefore, whoever calls this function should not pass a huge buffer.
\*-------------------------------------------------------------------------*/
int socket_send(p_socket ps, const char *data, size_t count, 
        size_t *sent, p_timeout tm)
{
    int err;
    *sent = 0;
    /* avoid making system calls on closed sockets */
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    /* loop until we send something or we give up on error */
    for ( ;; ) {
        /* try to send something */
		int put = send(*ps, data, (int) count, 0);
        /* if we sent something, we are done */
        if (put > 0) {
            *sent = put;
            return IO_DONE;
        }
        /* deal with failure */
        err = WSAGetLastError(); 
        /* we can only proceed if there was no serious error */
        if (err != WSAEWOULDBLOCK) return err;
        /* avoid busy wait */
        if ((err = socket_waitfd(ps, WAITFD_W, tm)) != IO_DONE) return err;
    } 
    /* can't reach here */
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Sendto with timeout
\*-------------------------------------------------------------------------*/
int socket_sendto(p_socket ps, const char *data, size_t count, size_t *sent, 
        SA *addr, socklen_t len, p_timeout tm)
{
    int err;
    *sent = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        int put = sendto(*ps, data, (int) count, 0, addr, len);
        if (put > 0) {
            *sent = put;
            return IO_DONE;
        }
        err = WSAGetLastError(); 
        if (err != WSAEWOULDBLOCK) return err;
        if ((err = socket_waitfd(ps, WAITFD_W, tm)) != IO_DONE) return err;
    } 
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Receive with timeout
\*-------------------------------------------------------------------------*/

#if 1 //old
int socket_recv(p_socket ps, char *data, size_t count, size_t *got, p_timeout tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        int taken = recv(*ps, data, (int) count, 0);
        if (taken > 0) {
            *got = taken;
            return IO_DONE;
        }
        if (taken == 0)
		{
			err = WSAGetLastError();
			return IO_CLOSED;
		}
        err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) return err;
        if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err;
    }
    return IO_UNKNOWN;
}


#else

//by lizulin 201808
int socket_recv(p_socket ps, char *data, size_t count, size_t *got, p_timeout tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;

#if 0
	//by lizulin 20180319
	int nTimeout =500;//*tm; //
	//设置接收超时为1000ms  
	if (SOCKET_ERROR == setsockopt(*ps, SOL_SOCKET, SO_RCVTIMEO,(char *)&nTimeout, sizeof(int)))  
	{
	     printf("Set SO_RCVTIMEO error !\n");  
		 //err = WSAGetLastError();
		 //return err;
	}  


	int selret = -1;
	long nRet = IO_TIMEOUT;//-2;

	fd_set fd;
	FD_ZERO(&fd);
	FD_SET(*ps, &fd);
	struct timeval  val;
	val.tv_sec  =0;      //*tm/1000;
	val.tv_usec =500000; //*tm*1000;
	//指向一组等待可读性检查到接口
	selret = select(0,&fd,NULL,NULL,&val); //	selret = select(*ps+1,&fd,NULL,NULL,&val);


	//有数据时 selret>0   否则 selret小于0
	if(selret > 0)
	{
		nRet = recv(*ps,(char*)data,count,0);
		if(nRet == 0)
		{
			// 表示连接被关闭了
			//CloseSocket();
			//nRet = 0;
			return IO_CLOSED;
		}
		else if(nRet < 0)
		{
			//nRet<0 表示出错了
			//CloseSocket();
			//nRet = -1;

			return nRet = IO_TIMEOUT;
		}

		*got=nRet;
	}
	else
	{
		*got=0;
	}

    return IO_DONE;





#else


	//by lizulin 20180319
	int nTimeout =*tm; //
	//设置接收超时为1000ms  
	//if (SOCKET_ERROR == setsockopt(*ps, SOL_SOCKET, SO_RCVTIMEO,(char *)&nTimeout, sizeof(int)))  
	//{
	//     printf("Set SO_RCVTIMEO error !\n");  
	//	 //err = WSAGetLastError();
	//	 //return err;
	//}  


    for ( ;; ) 
	{
		//printf("recv--begin  ");

        int taken = recv(*ps, data, (int) count, 0);

		//printf("recv--end taken:%d\n",taken);

        if (taken > 0) 
		{
		    //printf("recv--end taken:%d\n",taken);

            *got = taken;
            return IO_DONE;
        }

        if (taken == 0)
		{
			err = WSAGetLastError();
			return IO_CLOSED;
		}

		//new
		return IO_TIMEOUT;  //by lizulin 20180319  

		//printf("err != WSAEWOULDBLOCK err:%d \n",err);
		
		//old
		err = WSAGetLastError();
		if (err != WSAEWOULDBLOCK) return err;

		//printf("socket_waitfd--begin  err:%d \n",err);
        //if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) 
		//{
			//printf("socket_waitfd--end err:%d\n",err);
		//	return err;
		//}
		//printf("socket_waitfd--end err:%d\n",err);


		err = socket_waitfd(ps, WAITFD_R, tm);
        printf("socket_waitfd err:%d\n",err);
		//added by zn
		if(err < 0){
		return err;
		}
		//return err;
    }
    return IO_UNKNOWN;


#endif

}


#endif



/*-------------------------------------------------------------------------*\
* Recvfrom with timeout
\*-------------------------------------------------------------------------*/
int socket_recvfrom(p_socket ps, char *data, size_t count, size_t *got, 
        SA *addr, socklen_t *len, p_timeout tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        int taken = recvfrom(*ps, data, (int) count, 0, addr, len);
        if (taken > 0) {
            *got = taken;
            return IO_DONE;
        }
        if (taken == 0) return IO_CLOSED;
        err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) return err;
        if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err;
    }
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Put socket into blocking mode
\*-------------------------------------------------------------------------*/
void socket_setblocking(p_socket ps) {
    u_long argp = 0;
    ioctlsocket(*ps, FIONBIO, &argp);
}

/*-------------------------------------------------------------------------*\
* Put socket into non-blocking mode
\*-------------------------------------------------------------------------*/
void socket_setnonblocking(p_socket ps) {
    u_long argp = 1;
    ioctlsocket(*ps, FIONBIO, &argp);
}

//Put socket into non-blocking with timeout   //带超时版控制的非阻塞设置
bool socket_setnonblocking(p_socket ps,unsigned int ms) 
{
    u_long tmWait =ms;
    int ret = ioctlsocket(*ps,FIONBIO,&tmWait);  
    if(ret == SOCKET_ERROR) 
    {
        ::printf("socket_setnonblocking failed ioctlsocket()\n");
        return false;
    }
    return true;
}


/*-------------------------------------------------------------------------*\
* DNS helpers 
\*-------------------------------------------------------------------------*/
int socket_gethostbyaddr(const char *addr, socklen_t len, struct hostent **hp) {
    *hp = gethostbyaddr(addr, len, AF_INET);
    if (*hp) return IO_DONE;
    else return WSAGetLastError();
}

int socket_gethostbyname(const char *addr, struct hostent **hp) {
    *hp = gethostbyname(addr);
    if (*hp) return IO_DONE;
    else return  WSAGetLastError();
}

/*-------------------------------------------------------------------------*\
* I/O error strings
\*-------------------------------------------------------------------------*/
const char *io_strerror(int err) {
    switch (err) {
        case IO_DONE: return NULL;
        case IO_CLOSED: return "closed";
        case IO_TIMEOUT: return "timeout";
        default: return "unknown error"; 
    }
}

/*-------------------------------------------------------------------------*\
* Error translation functions
\*-------------------------------------------------------------------------*/
const char *socket_hoststrerror(int err) {
    if (err <= 0) return io_strerror(err);
    switch (err) {
        case WSAHOST_NOT_FOUND: return "host not found";
        default: return wstrerror(err); 
    }
}

const char *socket_strerror(int err) {
    if (err <= 0) return io_strerror(err);
    switch (err) {
        case WSAEADDRINUSE: return "address already in use";
        case WSAECONNREFUSED: return "connection refused";
        case WSAEISCONN: return "already connected";
        case WSAEACCES: return "permission denied";
        case WSAECONNABORTED: return "closed";
        case WSAECONNRESET: return "closed";
        case WSAETIMEDOUT: return "timeout";
        default: return wstrerror(err);
    }
}

const char *socket_ioerror(p_socket ps, int err) {
	(void) ps;
	return socket_strerror(err);
}

static const char *wstrerror(int err) {
    switch (err) {
        case WSAEINTR: return "Interrupted function call";
        case WSAEACCES: return "Permission denied";
        case WSAEFAULT: return "Bad address";
        case WSAEINVAL: return "Invalid argument";
        case WSAEMFILE: return "Too many open files";
        case WSAEWOULDBLOCK: return "Resource temporarily unavailable";
        case WSAEINPROGRESS: return "Operation now in progress";
        case WSAEALREADY: return "Operation already in progress";
        case WSAENOTSOCK: return "Socket operation on nonsocket";
        case WSAEDESTADDRREQ: return "Destination address required";
        case WSAEMSGSIZE: return "Message too long";
        case WSAEPROTOTYPE: return "Protocol wrong type for socket";
        case WSAENOPROTOOPT: return "Bad protocol option";
        case WSAEPROTONOSUPPORT: return "Protocol not supported";
        case WSAESOCKTNOSUPPORT: return "Socket type not supported";
        case WSAEOPNOTSUPP: return "Operation not supported";
        case WSAEPFNOSUPPORT: return "Protocol family not supported";
        case WSAEAFNOSUPPORT: 
            return "Address family not supported by protocol family"; 
        case WSAEADDRINUSE: return "Address already in use";
        case WSAEADDRNOTAVAIL: return "Cannot assign requested address";
        case WSAENETDOWN: return "Network is down";
        case WSAENETUNREACH: return "Network is unreachable";
        case WSAENETRESET: return "Network dropped connection on reset";
        case WSAECONNABORTED: return "Software caused connection abort";
        case WSAECONNRESET: return "Connection reset by peer";
        case WSAENOBUFS: return "No buffer space available";
        case WSAEISCONN: return "Socket is already connected";
        case WSAENOTCONN: return "Socket is not connected";
        case WSAESHUTDOWN: return "Cannot send after socket shutdown";
        case WSAETIMEDOUT: return "Connection timed out";
        case WSAECONNREFUSED: return "Connection refused";
        case WSAEHOSTDOWN: return "Host is down";
        case WSAEHOSTUNREACH: return "No route to host";
        case WSAEPROCLIM: return "Too many processes";
        case WSASYSNOTREADY: return "Network subsystem is unavailable";
        case WSAVERNOTSUPPORTED: return "Winsock.dll version out of range";
        case WSANOTINITIALISED: 
            return "Successful WSAStartup not yet performed";
        case WSAEDISCON: return "Graceful shutdown in progress";
        case WSAHOST_NOT_FOUND: return "Host not found";
        case WSATRY_AGAIN: return "Nonauthoritative host not found";
        case WSANO_RECOVERY: return "Nonrecoverable name lookup error"; 
        case WSANO_DATA: return "Valid name, no data record of requested type";
        default: return "Unknown error";
    }
}

const char* socket_name(p_socket ps)
{
	static char server_ip[20];
	struct sockaddr_in addr;
	int len;
	memset(server_ip, 0,sizeof(server_ip));
	len = sizeof(addr);
	getsockname(*ps, (struct sockaddr*)&addr, &len);
	return inet_ntoa(addr.sin_addr);
}
















#else
//usocket.c
/*=========================================================================*\
* Socket compatibilization module for Unix
* LuaSocket toolkit
*
* The code is now interrupt-safe.
* The penalty of calling select to avoid busy-wait is only paid when
* the I/O call fail in the first place. 
*
* RCS ID: $Id: usocket.c,v 1.38 2007/10/13 23:55:20 diego Exp $
\*=========================================================================*/
#include <string.h> 
#include <signal.h>

//#include "socket.h"    //move

#ifndef _WIN32

//#include "io.h"     //原文件内容直接替换--如下


//#ifndef IO_H
//#define IO_H
/*=========================================================================*\
* Input/Output abstraction
* LuaSocket toolkit
*
* This module defines the interface that LuaSocket expects from the
* transport layer for streamed input/output. The idea is that if any
* transport implements this interface, then the buffer.c functions
* automatically work on it.
*
* The module socket.h implements this interface, and thus the module tcp.h
* is very simple.
*
* RCS ID: $Id: io.h,v 1.11 2005/10/07 04:40:59 diego Exp $
\*=========================================================================*/
#include <stdio.h>
// #include "lua.h"
//#include "socketdef.h"   delete by me

//#include "timeout.h"		// deleted by liang 2008.06.06

/* IO error codes */
enum {
	IO_DONE = 0,        /* operation completed successfully */
	IO_TIMEOUT = -1,    /* operation timed out */
	IO_CLOSED = -2,     /* the connection has been closed */
	IO_UNKNOWN = -3
};

/* interface to error message function */
typedef const char *(*p_error) (
	void *ctx,          /* context needed by send */
	int err             /* error code */
	);

/* interface to send function */
typedef int (*p_send) (
	void *ctx,          /* context needed by send */
	const char *data,   /* pointer to buffer with data to send */
	size_t count,       /* number of bytes to send from buffer */
	size_t *sent,       /* number of bytes sent uppon return */
	p_timeout tm        /* timeout control */
	);

/* interface to recv function */
typedef int (*p_recv) (
	void *ctx,          /* context needed by recv */
	char *data,         /* pointer to buffer where data will be writen */
	size_t count,       /* number of bytes to receive into buffer */
	size_t *got,        /* number of bytes received uppon return */
	p_timeout tm        /* timeout control */
	);

/* IO driver definition */
typedef struct t_io_ {
	void *ctx;          /* context needed by send/recv */
	p_send send;        /* send function pointer */
	p_recv recv;        /* receive function pointer */
	p_error error;      /* strerror function */
} t_io;
typedef t_io *p_io;

void io_init(p_io io, p_send send, p_recv recv, p_error error, void *ctx);
const char *io_strerror(int err);

//#endif /* IO_H */


///原"io.c"  文件内容直接替换

/*=========================================================================*\
* Exported functions
\*=========================================================================*/
/*-------------------------------------------------------------------------*\
* Initializes C structure
\*-------------------------------------------------------------------------*/
void io_init(p_io io, p_send send, p_recv recv, p_error error, void *ctx) {
	io->send = send;
	io->recv = recv;
	io->error = error;
	io->ctx = ctx;
}

/*-------------------------------------------------------------------------*\
* I/O error strings
\*-------------------------------------------------------------------------*/


//#ifndef _WIN32
//const char *io_strerror(int err) {
//	switch (err) {
//	case IO_DONE: return NULL;
//	case IO_CLOSED: return "closed";
//	case IO_TIMEOUT: return "timeout";
//	default: return "unknown error";
//	}
//}
//#endif


const char *io_strerror(int err) {
	switch (err) {
		case IO_DONE: return NULL;
		case IO_CLOSED: return "closed";
		case IO_TIMEOUT: return "timeout";
		default: return "unknown error"; 
	}
} 


/*-------------------------------------------------------------------------*\
* Wait for readable/writable/connected socket with timeout
\*-------------------------------------------------------------------------*/
#ifdef SOCKET_POLL
#include <sys/poll.h>

#define WAITFD_R        POLLIN
#define WAITFD_W        POLLOUT
#define WAITFD_C        (POLLIN|POLLOUT)
int socket_waitfd(p_socket ps, int sw, p_timeout tm) {
    int ret;
    struct pollfd pfd;
    pfd.fd = *ps;
    pfd.events = sw;
    pfd.revents = 0;
    if (timeout_iszero(tm)) return IO_TIMEOUT;  /* optimize timeout == 0 case */
    do {
#if 0
		int t = (int)(timeout_getretry(tm)*1e3);
#else
		int t = 0;
		if(!timeout_infinite(tm))
			t = (*tm)*1000;
#endif
		ret = poll(&pfd, 1, t >= 0? t: -1);
	} while (ret == -1 && errno == EINTR);
    if (ret == -1) return errno;
    if (ret == 0) return IO_TIMEOUT;
    if (sw == WAITFD_C && (pfd.revents & (POLLIN|POLLERR))) return IO_CLOSED;
    return IO_DONE;
}
#else

#define WAITFD_R        1
#define WAITFD_W        2
#define WAITFD_C        (WAITFD_R|WAITFD_W)

int socket_waitfd(p_socket ps, int sw, p_timeout tm) {
    int ret;
    fd_set rfds, wfds, *rp, *wp;
    struct timeval tv, *tp;
    double t;
    if (timeout_iszero(tm)) return IO_TIMEOUT;  /* optimize timeout == 0 case */
    do {
        /* must set bits within loop, because select may have modifed them */
        rp = wp = NULL;
        if (sw & WAITFD_R) { FD_ZERO(&rfds); FD_SET(*ps, &rfds); rp = &rfds; }
        if (sw & WAITFD_W) { FD_ZERO(&wfds); FD_SET(*ps, &wfds); wp = &wfds; }
#if 0
        t = timeout_getretry(tm);
        tp = NULL;
        if (t >= 0.0) {
            tv.tv_sec = (int)t;
            tv.tv_usec = (int)((t-tv.tv_sec)*1.0e6);
            tp = &tv;
        }
#else
		if(!timeout_infinite(tm)) {
			tv.tv_sec = (*tm)/1000;
			tv.tv_usec = (*tm)%1000*1000;
			tp = &tv;
		}
#endif
        ret = select(*ps+1, rp, wp, NULL, tp);
    } while (ret == -1 && errno == EINTR);
    if (ret == -1) return errno;
    if (ret == 0) return IO_TIMEOUT;
    if (sw == WAITFD_C && FD_ISSET(*ps, &rfds)) return IO_CLOSED;
    return IO_DONE;
}
#endif


/*-------------------------------------------------------------------------*\
* Initializes module 
\*-------------------------------------------------------------------------*/
int socket_open(void) {
    /* instals a handler to ignore sigpipe or it will crash us */
    signal(SIGPIPE, SIG_IGN);
    return 1;
}

/*-------------------------------------------------------------------------*\
* Close module 
\*-------------------------------------------------------------------------*/
int socket_close(void) {
    return 1;
}

/*-------------------------------------------------------------------------*\
* Close and inutilize socket
\*-------------------------------------------------------------------------*/
void socket_destroy(p_socket ps) {
    if (*ps != SOCKET_INVALID) {
        socket_setblocking(ps);
        close(*ps);
        *ps = SOCKET_INVALID;
    }
}

/*-------------------------------------------------------------------------*\
* Select with timeout control
\*-------------------------------------------------------------------------*/
int socket_select(t_socket n, fd_set *rfds, fd_set *wfds, fd_set *efds, 
        p_timeout tm) 
{
    int ret;
    do {
        struct timeval tv;
#if 0
        double t = timeout_getretry(tm);
        tv.tv_sec = (int) t;
        tv.tv_usec = (int) ((t - tv.tv_sec) * 1.0e6);
#else
		double t = timeout_infinite(tm);    //by lizulin because of timeout_getretry() is removed 
		if(tm && *tm > 0) {
			tv.tv_sec = 0;
			tv.tv_usec = 1000*(*tm);
		}
#endif
        /* timeout = 0 means no wait */
        ret = select(n, rfds, wfds, efds, t >= 0.0 ? &tv: NULL);
    } while (ret < 0 && errno == EINTR);
    return ret;
}

/*-------------------------------------------------------------------------*\
* Creates and sets up a socket
\*-------------------------------------------------------------------------*/
int socket_create(p_socket ps, int domain, int type, int protocol) {
    *ps = socket(domain, type, protocol);
    if (*ps != SOCKET_INVALID) return IO_DONE; 
    else return errno; 
}

/*-------------------------------------------------------------------------*\
* Binds or returns error message
\*-------------------------------------------------------------------------*/
int socket_bind(p_socket ps, SA *addr, socklen_t len) {
    int err = IO_DONE;
    socket_setblocking(ps);
    if (bind(*ps, addr, len) < 0) err = errno; 
    socket_setnonblocking(ps);
    return err;
}

/*-------------------------------------------------------------------------*\
* 
\*-------------------------------------------------------------------------*/
int socket_listen(p_socket ps, int backlog) {
    int err = IO_DONE; 
    socket_setblocking(ps);
    if (listen(*ps, backlog)) err = errno; 
    socket_setnonblocking(ps);
    return err;
}

/*-------------------------------------------------------------------------*\
* 
\*-------------------------------------------------------------------------*/
void socket_shutdown(p_socket ps, int how) {
    socket_setblocking(ps);
    shutdown(*ps, how);
    socket_setnonblocking(ps);
}

/*-------------------------------------------------------------------------*\
* Connects or returns error message
\*-------------------------------------------------------------------------*/
int socket_connect(p_socket ps, SA *addr, socklen_t len, p_timeout tm) {
    int err;
    /* avoid calling on closed sockets */
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    /* call connect until done or failed without being interrupted */
    do if (connect(*ps, addr, len) == 0) return IO_DONE;
    while ((err = errno) == EINTR);
    /* if connection failed immediately, return error code */
    if (err != EINPROGRESS && err != EAGAIN) return err; 
    /* zero timeout case optimization */
    if (timeout_iszero(tm)) return IO_TIMEOUT;
    /* wait until we have the result of the connection attempt or timeout */
    err = socket_waitfd(ps, WAITFD_C, tm);
    if (err == IO_CLOSED) {
        if (recv(*ps, (char *) &err, 0, 0) == 0) return IO_DONE;
        else return errno;
    } else return err;
}

/*-------------------------------------------------------------------------*\
* Accept with timeout
\*-------------------------------------------------------------------------*/
int socket_accept(p_socket ps, p_socket pa, SA *addr, socklen_t *len, p_timeout tm) {
    SA daddr;
    socklen_t dlen = sizeof(daddr);
    if (*ps == SOCKET_INVALID) return IO_CLOSED; 
    if (!addr) addr = &daddr;
    if (!len) len = &dlen;
    for ( ;; ) {
        int err;
        if ((*pa = accept(*ps, addr, len)) != SOCKET_INVALID) return IO_DONE;
        err = errno;
        if (err == EINTR) continue;
        if (err != EAGAIN && err != ECONNABORTED) return err;
        if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err;
    }
    /* can't reach here */
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Send with timeout
\*-------------------------------------------------------------------------*/
int socket_send(p_socket ps, const char *data, size_t count, 
        size_t *sent, p_timeout tm)
{
    int err;
    *sent = 0;
    /* avoid making system calls on closed sockets */
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    /* loop until we send something or we give up on error */
    for ( ;; ) {
        long put = (long) send(*ps, data, count, 0);
        /* if we sent anything, we are done */
        if (put > 0) {
            *sent = put;
            return IO_DONE;
        }
        err = errno;
        /* send can't really return 0, but EPIPE means the connection was 
           closed */
        if (put == 0 || err == EPIPE) return IO_CLOSED;
        /* we call was interrupted, just try again */
        if (err == EINTR) continue;
        /* if failed fatal reason, report error */
        if (err != EAGAIN) return err;
        /* wait until we can send something or we timeout */
        if ((err = socket_waitfd(ps, WAITFD_W, tm)) != IO_DONE) return err;
    }
    /* can't reach here */
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Sendto with timeout
\*-------------------------------------------------------------------------*/
int socket_sendto(p_socket ps, const char *data, size_t count, size_t *sent, 
        SA *addr, socklen_t len, p_timeout tm)
{
    int err;
    *sent = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        long put = (long) sendto(*ps, data, count, 0, addr, len);  
        if (put > 0) {
            *sent = put;
            return IO_DONE;
        }
        err = errno;
        if (put == 0 || err == EPIPE) return IO_CLOSED;
        if (err == EINTR) continue;
        if (err != EAGAIN) return err;
        if ((err = socket_waitfd(ps, WAITFD_W, tm)) != IO_DONE) return err;
    }
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Receive with timeout
\*-------------------------------------------------------------------------*/
int socket_recv(p_socket ps, char *data, size_t count, size_t *got, p_timeout tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        long taken = (long) recv(*ps, data, count, 0);
        if (taken > 0) {
            *got = taken;
            return IO_DONE;
        }
        err = errno;
        if (taken == 0) return IO_CLOSED;
        if (err == EINTR) continue;
        if (err != EAGAIN) return err; 
        if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err; 
    }
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Recvfrom with timeout
\*-------------------------------------------------------------------------*/
int socket_recvfrom(p_socket ps, char *data, size_t count, size_t *got, 
        SA *addr, socklen_t *len, p_timeout tm) {
    int err;
    *got = 0;
    if (*ps == SOCKET_INVALID) return IO_CLOSED;
    for ( ;; ) {
        long taken = (long) recvfrom(*ps, data, count, 0, addr, len);
        if (taken > 0) {
            *got = taken;
            return IO_DONE;
        }
        err = errno;
        if (taken == 0) return IO_CLOSED;
        if (err == EINTR) continue;
        if (err != EAGAIN) return err; 
        if ((err = socket_waitfd(ps, WAITFD_R, tm)) != IO_DONE) return err; 
    }
    return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Put socket into blocking mode
\*-------------------------------------------------------------------------*/
void socket_setblocking(p_socket ps) {
    int flags = fcntl(*ps, F_GETFL, 0);
    flags &= (~(O_NONBLOCK));
    fcntl(*ps, F_SETFL, flags);
}

/*-------------------------------------------------------------------------*\
* Put socket into non-blocking mode
\*-------------------------------------------------------------------------*/
void socket_setnonblocking(p_socket ps) {
    int flags = fcntl(*ps, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(*ps, F_SETFL, flags);
}


//add by lizulin no ctrl of timeout  //linux 版没有超时
bool socket_setnonblocking(p_socket ps,unsigned int tmout)
{
    int flags = fcntl(*ps,F_GETFL,0);
    flags |= O_NONBLOCK;
    fcntl(*ps, F_SETFL, flags);
    return true;
}



/*-------------------------------------------------------------------------*\
* DNS helpers 
\*-------------------------------------------------------------------------*/
int socket_gethostbyaddr(const char *addr, socklen_t len, struct hostent **hp) {
    *hp = gethostbyaddr(addr, len, AF_INET);
    if (*hp) return IO_DONE;
    else if (h_errno) return h_errno;
    else if (errno) return errno;
    else return IO_UNKNOWN;
}

int socket_gethostbyname(const char *addr, struct hostent **hp) {
    *hp = gethostbyname(addr);
    if (*hp) return IO_DONE;
    else if (h_errno) return h_errno;
    else if (errno) return errno;
    else return IO_UNKNOWN;
}

/*-------------------------------------------------------------------------*\
* Error translation functions
* Make sure important error messages are standard
\*-------------------------------------------------------------------------*/
const char *socket_hoststrerror(int err) {
    if (err <= 0) return io_strerror(err);
    switch (err) {
        case HOST_NOT_FOUND: return "host not found";
        default: return hstrerror(err);
    }
}

const char *socket_strerror(int err) {
    if (err <= 0) return io_strerror(err);
    switch (err) {
        case EADDRINUSE: return "address already in use";
        case EISCONN: return "already connected";
        case EACCES: return "permission denied";
        case ECONNREFUSED: return "connection refused";
        case ECONNABORTED: return "closed";
        case ECONNRESET: return "closed";
        case ETIMEDOUT: return "timeout";
        default: return strerror(errno);
    }
}

const char *socket_ioerror(p_socket ps, int err) {
    (void) ps;
    return socket_strerror(err);
} 

const char* socket_name(p_socket ps)
{
	static char server_ip[20];
	memset(server_ip, 0,sizeof(server_ip));
	struct sockaddr_in addr;

    //int len = sizeof(addr); 
    //getsockname(*ps, (struct sockaddr*)&addr, (unsigned int*)&len);

    socklen_t len = sizeof(addr);
    getsockname(*ps, (struct sockaddr*)&addr, &len);

	inet_ntop(AF_INET, &addr.sin_addr, server_ip, sizeof(server_ip));
 	return server_ip;
}
#endif

#endif











//move to head file
/*
extern "C" {
//#include "io.h"
#include "socket.h"
}
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXITFUNC_ERR(err)	{return err;}

#ifdef _WIN32
#define snprintf	_snprintf	
#endif


CZSocket::CZSocket(void):m_socket(SOCKET_INVALID), m_error(0)
{
	m_bInitOK = 0;
	m_nRefCount = 0;
	if(!m_bInitOK && !(m_bInitOK = socket_open()))
	{
		m_error = ZSE_SOCKETINIT;
		return;
	}
	m_nRefCount++;
}

CZSocket::~CZSocket(void)
{
	Destroy();
	assert(m_nRefCount>0);
	if(--m_nRefCount==0)
		socket_close();
}

bool CZSocket::Create(int domain, int type, int protocol)
{
#ifdef _WIN32   //new add by me
	//初始化WSA  必须要加这几行代码
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA wsaData;
	if(WSAStartup(sockVersion, &wsaData)!=0)
	{
		return false;
		//return -1;
	}
#endif

	if(!m_bInitOK) return false;
	if(m_socket!=SOCKET_INVALID)
	{
		m_error = ZSE_HASBEENOPEND;		
		return false;
	}
	m_error = socket_create(&m_socket, domain, type, protocol);
	return ZSOCKET_SUCCEED(m_error);
}

void CZSocket::Destroy()
{
	if(m_socket!=SOCKET_INVALID)
	{
		socket_destroy(&m_socket);
		m_socket = SOCKET_INVALID;
	}
}

bool CZSocket::IsOpened()
{
	return m_socket!=SOCKET_INVALID;
}

bool CZSocket::Bind(const sockaddr_in *addr_in)
{
	assert(sizeof(sockaddr_in) == sizeof(SA));
	memcpy(&m_addrin, addr_in, sizeof(sockaddr_in));
	m_error = socket_bind(&m_socket, (SA*)addr_in, sizeof(SA));
	return ZSOCKET_SUCCEED(m_error);
}

bool CZSocket::Listen(int backlog)
{
	m_error = socket_listen(&m_socket, backlog);
	return ZSOCKET_SUCCEED(m_error);
}

int CZSocket::Accept(CZSocket& as, char* address, int* port, t_timeout* tm)
{
	assert(!as.IsOpened());
	SA addr;
	memset(&addr, 0, sizeof(addr));
	socklen_t len = sizeof(addr);
	m_error = socket_accept(&m_socket, &as.m_socket, &addr, &len, tm);
	if(ZSOCKET_FAILED(m_error))
		return m_error;
	memcpy(&as.m_addrin, &addr, sizeof(as.m_addrin));
	sockaddr_in* p = (sockaddr_in*)&addr;
	if(port) *port = ntohs(p->sin_port);
	if(address) strcpy(address, inet_ntoa(p->sin_addr));
	return m_error;
}

int CZSocket::Connect(const sockaddr_in *addr_in, t_timeout* tm)
{
	m_error = socket_connect(&m_socket, (SA*)addr_in, sizeof(sockaddr_in), tm);
	return m_error;
}

int CZSocket::Send(const char* buff, int size, size_t& sent, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	size_t s = size;
	size_t s_sent = 0;
	while(s > 0)
	{
		m_error = socket_send(&m_socket, buff + s_sent, s, &sent, tm);
		s -= sent;
		s_sent += sent;
		sent = s_sent;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	return m_error;
}

#if 1  //old
int CZSocket::Receive(char* buff, int size, size_t& getsize, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	size_t s = size;
	size_t s_get = 0;
	socket_setblocking(&m_socket);
	while(s > 0)
	{
		m_error = socket_recv(&m_socket, buff + s_get, s, &getsize, tm);
		s -= getsize;
		s_get += getsize;
		getsize = s_get;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	getsize = size;
	return m_error;
}

#else
int CZSocket::Receive(char* buff, int size, size_t& getsize, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	size_t s = size;
	size_t s_get = 0;
	socket_setblocking(&m_socket);

	//unsigned int times=0;
	while(s > 0)
	{
		//printf("socket_recv() times:%d size:%d getsize:%d \n",times,size,getsize);

		m_error = socket_recv(&m_socket, buff + s_get, s, &getsize, tm);
		s -= getsize;
		s_get += getsize;
		//getsize = s_get;
		getsize = 0; //by zn
		//*tm--;  //by lizulin 20180319
		//printf("socket_recv() times:%d size:%d getsize:%d \n",times,size,getsize);
		//times++;
		//if(times>3)
		//{
		//	break;
		//}


		if(ZSOCKET_FAILED(m_error))
			break;
	}
	getsize = s_get;
	return m_error;
}

#endif



int CZSocket::Receive2(char* buff, int size, size_t& getsize, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	size_t s = size;
	size_t s_get = 0;
	socket_setnonblocking(&m_socket);
	while(s > 0)
	{
		m_error = socket_recv(&m_socket, buff + s_get, s, &getsize, tm);
		s -= getsize;
		s_get += getsize;
		getsize = s_get;
		if(ZSOCKET_FAILED(m_error))
			break;
			//return m_error;
	}
	getsize = size;
	return m_error;
}

int CZSocket::SendTo(const char* buff, int size, size_t& sent, const sockaddr_in *addr_in, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	socklen_t len = sizeof(sockaddr_in);
	size_t s = size;
	size_t s_sent = 0;
	while(s > 0)
	{
		m_error = socket_sendto(&m_socket, buff + s_sent, s, &sent, (SA*)addr_in, len, tm);
		s -= sent;
		s_sent += sent;
		sent = s_sent;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	return 0;
}

int CZSocket::ReceiveFrom(char* buff, int size, size_t& getsize, const sockaddr_in *addr_in, t_timeout* tm)
{
	assert(buff);
	assert(size>=0);
	socklen_t len = sizeof(sockaddr_in);
	size_t s = size;
	size_t s_get = 0;
	while(s > 0)
	{
        m_error = socket_recvfrom(&m_socket, buff+s_get, s, &getsize, (SA*)addr_in, &len, tm);
       //m_error = socket_recvfrom(&m_socket, buff+s_get, s, &getsize, (SA*)addr_in,(unsigned int*)&len, tm);   //bylizulin

		s -= getsize;
		s_get += getsize;
		getsize = s_get;
		if(ZSOCKET_FAILED(m_error))
			break;
	}
	return m_error;
}

void CZSocket::SetBlocking(bool bBlock)
{
    if (bBlock)
        socket_setblocking(&m_socket);
    else
        socket_setnonblocking(&m_socket);
//     u_long argp = !bBlock;
// 	ioctlsocket(m_socket, FIONBIO, &argp);
}




bool CZSocket::setBlockMode(bool bBlock,unsigned int ms)
{
  if (m_socket == SOCKET_INVALID)
  {
     ::printf("CZSocket::setBlockMode() failed SOCKET_INVALID!\n");
      return false;
  }

    if (bBlock)
    {
        socket_setblocking(&m_socket);
    }
    else
    {
        //socket_setnonblocking(&m_socket);
        return socket_setnonblocking(&m_socket,ms);

    }
    //u_long argp = !bBlock;
    //ioctlsocket(m_socket, FIONBIO, &argp);

    return true;
}


int CZSocket::Wait(int type, t_timeout* tm)
{
	return socket_waitfd(&m_socket, type, tm);
}

int CZSocket::GetLastError(char* buff, int bufsize)
{
	if(buff)
	{
		const char* s = 0;
		if(ZSOCKET_SUCCEED(m_error))
		{
			s = "succeeded";
		}
		else if(ZSE_TEST(m_error))
		{
			switch(m_error)
			{
			case ZSE_SOCKETINIT:
				s = "initialize socket failed";
				break;
			case ZSE_HASBEENOPEND:
				s = "the socket has been opend";
				break;
			case ZSE_INETADDR:
				s = "inet_addr failed";
				break;
			default:
				s = "unknown";
			}
		}
		else
		{
			s = socket_strerror(m_error);
		}
		snprintf(buff, bufsize, "zSocket: %s", s);
	}
	return m_error;
}

std::string CZSocket::GetSockName()
{
	m_sockName = socket_name(&m_socket);
	return m_sockName;
}

std::string CZSocket::getServerIP()
{
    return m_sockName;
     //getsockname(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);//获取sockfd表示的连接上的本地地址
}
