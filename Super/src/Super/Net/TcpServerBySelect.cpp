#include "TcpServerBySelect.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
    #include <winsock2.h>
    #pragma comment(lib, "WS2_32")    //显式包含或者使用者包含ws2_32.lib
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

static void ErrorHandling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

class TcpServerBySelectPrivate
{
public:
#if defined(_WIN32)
    SOCKET hServSock, hClntSock;
#else

#endif
};


TcpServerBySelect::TcpServerBySelect(void):pImpl(new TcpServerBySelectPrivate)
{

}

TcpServerBySelect::~TcpServerBySelect(void)
{

}


bool TcpServerBySelect::initSoctet()
{
#ifdef _WIN32
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
    {
        ErrorHandling("WSAStartup() error!"); 
        return false;
    }
#else

#endif

    return true;
}

bool TcpServerBySelect::bind()
{
#ifdef _WIN32

    //SOCKET hServSock;
    SOCKADDR_IN servAdr;
 //   int adrSz;
//    int strLen, fdNum, i;
    pImpl->hServSock=socket(PF_INET, SOCK_STREAM,0);
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family=AF_INET;
    servAdr.sin_addr.s_addr=htonl(INADDR_ANY);
    int port=getPort();
    servAdr.sin_port=htons(port);   //(atoi(argv[1]));
    if(::bind(pImpl->hServSock,(SOCKADDR*) &servAdr, sizeof(servAdr))==SOCKET_ERROR)
    {
        ErrorHandling("bind() error");
        return false;
    }
#else

#endif
    return true;
}


bool TcpServerBySelect::listen()
{
#ifdef _WIN32

    if(::listen(pImpl->hServSock,5)==SOCKET_ERROR)
    {
        ErrorHandling("listen() error");
        return false;
    }
#endif

    return true;
}

void TcpServerBySelect::run()
{
#ifdef _WIN32

    //简单Select模式，Select+多线程模式待续
   #define BUF_SIZE 1024
    SOCKADDR_IN clntAdr;
    TIMEVAL timeout;
    fd_set reads, cpyReads;
    int adrSz;
    int strLen, fdNum, i;
    char buf[BUF_SIZE];

    FD_ZERO(&reads);
    FD_SET(pImpl->hServSock, &reads);

    while(1)
    {
        cpyReads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=5000;

        if((fdNum=select(0, &cpyReads, 0, 0, &timeout))==SOCKET_ERROR)
            break;

        if(fdNum==0)
            continue;

        for(i=0; i<reads.fd_count; i++)
        {
            if(FD_ISSET(reads.fd_array[i], &cpyReads))
            {
                if(reads.fd_array[i]==pImpl->hServSock)     // connection request!
                {
                    adrSz=sizeof(clntAdr);
                    pImpl->hClntSock=::accept(pImpl->hServSock, (SOCKADDR*)&clntAdr, &adrSz);
                    FD_SET(pImpl->hClntSock, &reads);
                    printf("connected client: %d \n", pImpl->hClntSock);
                }
                else    // read message!
                {
                    strLen=recv(reads.fd_array[i], buf, BUF_SIZE-1, 0);
                    if(strLen==0)    // close request!
                    {
                        FD_CLR(reads.fd_array[i], &reads);
                        closesocket(cpyReads.fd_array[i]);
                        printf("closed client: %d \n", cpyReads.fd_array[i]);
                    }
                    else
                    {
                        send(reads.fd_array[i], buf, strLen, 0);    // echo!
                    }
                }
            }
        }
    }
    closesocket(pImpl->hServSock);
    WSACleanup();
    //return 0;
#else

#endif
}





#if 0
#define BUF_SIZE 1024
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
    WSADATA wsaData;
    SOCKET hServSock, hClntSock;
    SOCKADDR_IN servAdr, clntAdr;
    TIMEVAL timeout;
    fd_set reads, cpyReads;

    int adrSz;
    int strLen, fdNum, i;
    char buf[BUF_SIZE];

    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
        ErrorHandling("WSAStartup() error!"); 

    hServSock=socket(PF_INET, SOCK_STREAM, 0);
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family=AF_INET;
    servAdr.sin_addr.s_addr=htonl(INADDR_ANY);
    servAdr.sin_port=htons(atoi(argv[1]));

    if(bind(hServSock, (SOCKADDR*) &servAdr, sizeof(servAdr))==SOCKET_ERROR)
        ErrorHandling("bind() error");
    if(listen(hServSock, 5)==SOCKET_ERROR)
        ErrorHandling("listen() error");

    FD_ZERO(&reads);
    FD_SET(hServSock, &reads);

    while(1)
    {
        cpyReads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=5000;

        if((fdNum=select(0, &cpyReads, 0, 0, &timeout))==SOCKET_ERROR)
            break;

        if(fdNum==0)
            continue;

        for(i=0; i<reads.fd_count; i++)
        {
            if(FD_ISSET(reads.fd_array[i], &cpyReads))
            {
                if(reads.fd_array[i]==hServSock)     // connection request!
                {
                    adrSz=sizeof(clntAdr);
                    hClntSock=
                        accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
                    FD_SET(hClntSock, &reads);
                    printf("connected client: %d \n", hClntSock);
                }
                else    // read message!
                {
                    strLen=recv(reads.fd_array[i], buf, BUF_SIZE-1, 0);
                    if(strLen==0)    // close request!
                    {
                        FD_CLR(reads.fd_array[i], &reads);
                        closesocket(cpyReads.fd_array[i]);
                        printf("closed client: %d \n", cpyReads.fd_array[i]);
                    }
                    else
                    {
                        send(reads.fd_array[i], buf, strLen, 0);    // echo!
                    }
                }
            }
        }
    }
    closesocket(hServSock);
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


