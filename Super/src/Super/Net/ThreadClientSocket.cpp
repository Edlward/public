//#include "stdafx.h"

#include "ThreadClientSocket.h"
//#include "Protocol.h"

#include "../../../../Proj/ProtocolNetHead.h"      //网络通信协议数据报文头/理论上网络底层不应该依赖报文头结构，暂时如此
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zSocket.h"
#include "../Basic/Thread.h"  //for sleep

//#if defined(_WIN32)
//    #include <winsock2.h>
//#else
//    #include <unistd.h>
//    #include <arpa/inet.h>
//    #include <sys/socket.h>
//#endif

#include <memory.h>

static void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

ClientPyloadProcBase* CreateClientPyloadProc()
{
    return new ClientPyloadProcBase;
}


//int ThreadClientSocket::ObjCnt=0;


class ThreadClientSocketPrivate
{
public:
    CZSocket m_socket;    //zSocket有副作用，会引入系统头文件，用pimpl技法隔离
};


ThreadClientSocket::ThreadClientSocket(void):
    port(Defaultport)
    ,pFunCreat(CreateClientPyloadProc),pImpl(new ThreadClientSocketPrivate)
{
    memset(ip,0,sizeof(ip));
    //ObjCnt++;
    //printf("ThreadClientSocket::ThreadClientSocket() ObjCnt:%d\n",ObjCnt);
    CurrObjIndex=traceClass.ObjCnt;
}


ThreadClientSocket::~ThreadClientSocket(void)
{
    //ObjCnt--;
    //printf("ThreadClientSocket::~ThreadClientSocket() ObjCnt:%d\n",ObjCnt);
}

CZSocket& ThreadClientSocket::getSocket()
{
    return pImpl->m_socket;
}

int ThreadClientSocket::ReadData(char* buf,unsigned int len)
{
    int ret=getSocket().ReadData(buf,len);
    if(ret<0)
    {
        throw -1;
    }
    return ret;
}


int ThreadClientSocket::SendData(const char* buf,unsigned int len)
{
    int ret=getSocket().SendData(buf,len);
    if(ret<0)
    {
        throw -1;
    }
    return ret;
}



void ThreadClientSocket::setServerIPPort(const char* sIP,int port)
{
    size_t len=strlen(sIP);
    if (len>sizeof(ip))
    {
        len=sizeof(ip);
    }
    memcpy(ip,sIP,len);
    this->port=port;
}

bool ThreadClientSocket::Init()
{
    //WSADATA wsaData;
    //if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
    //{
    //    ErrorHandling("WSAStartup() error!"); 
    //    return false;
    //}

    return true;
}

bool ThreadClientSocket::Connect()
{
    ////SOCKET hSocket;
    //SOCKADDR_IN servAdr;
    //hSocket=socket(PF_INET, SOCK_STREAM, 0);   
    //if(hSocket==INVALID_SOCKET)
    //{
    //    ErrorHandling("socket() error");
    //    return false;
    //}

    //memset(&servAdr, 0, sizeof(servAdr));
    //servAdr.sin_family=AF_INET;
    //servAdr.sin_addr.s_addr=inet_addr(ip); //inet_addr(argv[1]);
    //servAdr.sin_port=htons(port);               //htons(atoi(argv[2]));

    //if(connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr))==SOCKET_ERROR)
    //{
    //    ErrorHandling("connect() error!");
    //    return false;
    //}
    //puts("Connected...........");
    //return true;

    sockaddr_in servAdr;
    memset(&servAdr,0,sizeof(servAdr));
    servAdr.sin_family=AF_INET;
    servAdr.sin_addr.s_addr=inet_addr(ip); //inet_addr(argv[1]);
    servAdr.sin_port=htons(port);               //htons(atoi(argv[2]));

    t_timeout tm=3000u;
    int retCon=pImpl->m_socket.Connect(&servAdr,&tm);
    //return retCon==CZSocket::WAIT_R_DONE;  //暂时
    return retCon>0;
}



void ThreadClientSocket::run()
{
    printf("ThreadClientSocket::run()--begin\n");

    #define BUF_SIZE 1024
    char message[BUF_SIZE];
    int strLen;

    SendNum=0;

    while(getContinueRun()) 
    {
#if 0
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
            break;
#endif

        //Sleep(500) ;

        Thread::sleep_for(500);

        char  buf[256];
        KL* pKL=(KL*)buf;
        pKL->head=KL_HEAD;
        pKL->key=CMD_TEXT_MESSAGE;
        char* pMsg=buf+sizeof(KL);
        sprintf(pMsg,"Client:%d: Test:Hello World %d\n",CurrObjIndex,SendNum++);
        pKL->len=strlen(pMsg);

        printf("Send Msg: %s", pMsg); 
        //send(hSocket, buf, sizeof(KL)+pKL->len,0);
        SendData(buf,sizeof(KL)+pKL->len);


       // send(hSocket, buf, sizeof(buf), 0);
        //strLen=recv(hSocket, message, BUF_SIZE-1, 0);

        strLen=0;

        //strLen+=recv(hSocket,buf,sizeof(KL), 0);
        //strLen+=recv(hSocket,buf+sizeof(KL),pKL->len,0);

        strLen+=ReadData(buf,sizeof(KL));
        strLen+=ReadData(buf+sizeof(KL),pKL->len);


        pMsg=buf+sizeof(KL);
        pMsg[pKL->len]=0;
        printf("Recv Msg: %s", pMsg); 

       // printf("Message from server: %s", message); 
    }

    //closesocket(hSocket);
    //WSACleanup();

    printf("ThreadClientSocket::run()--end\n");

    //return 0;
}

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    SOCKET hSocket;
    char message[BUF_SIZE];
    int strLen;
    SOCKADDR_IN servAdr;

    if(argc!=3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
        ErrorHandling("WSAStartup() error!"); 

    hSocket=socket(PF_INET, SOCK_STREAM, 0);   
    if(hSocket==INVALID_SOCKET)
        ErrorHandling("socket() error");

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family=AF_INET;
    servAdr.sin_addr.s_addr=inet_addr(argv[1]);
    servAdr.sin_port=htons(atoi(argv[2]));

    if(connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr))==SOCKET_ERROR)
        ErrorHandling("connect() error!");
    else
        puts("Connected...........");

    while(1) 
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
            break;

        send(hSocket, message, strlen(message), 0);
        strLen=recv(hSocket, message, BUF_SIZE-1, 0);
        message[strLen]=0;
        printf("Message from server: %s", message);
    }

    closesocket(hSocket);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
#endif



#if 0   //linux
#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;

    struct sockaddr_in serv_adr, from_adr;
    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock=socket(PF_INET, SOCK_DGRAM, 0);   
    if(sock==-1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));

    while(1)
    {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);     
        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
            break;

        sendto(sock, message, strlen(message), 0, 
            (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        adr_sz=sizeof(from_adr);
        str_len=recvfrom(sock, message, BUF_SIZE, 0, 
            (struct sockaddr*)&from_adr, &adr_sz);

        message[str_len]=0;
        printf("Message from server: %s", message);
    }	
    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
#endif
