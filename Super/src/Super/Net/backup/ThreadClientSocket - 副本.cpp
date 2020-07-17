#include "ThreadClientSocket.h"
#include "Protocol.h"
#include "../SuperClient/my_struct.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

ClientPayloadProcBase* CreateClientPayloadProc()
{
    return new ClientPayloadProcBase;
}


int ThreadClientSocket::ObjCnt=0;

ThreadClientSocket::ThreadClientSocket(void):
    port(Defaultport)
    ,pFunCreat(CreateClientPayloadProc)
{
    memset(ip,0,sizeof(ip));
    ObjCnt++;
    printf("ThreadClientSocket::ThreadClientSocket() ObjCnt:%d\n",ObjCnt);
    CurrObjIndex=ObjCnt;
}


ThreadClientSocket::~ThreadClientSocket(void)
{
    ObjCnt--;
    printf("ThreadClientSocket::~ThreadClientSocket() ObjCnt:%d\n",ObjCnt);
}

void ThreadClientSocket::setServerIPPort(const char* sIP,int port)
{
    unsigned int len=strlen(sIP);
    if (len>sizeof(ip))
    {
        len=sizeof(ip);
    }

    memcpy(ip,sIP,len);
    this->port=port;
}

bool ThreadClientSocket::Init()
{
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
    {
        ErrorHandling("WSAStartup() error!"); 
        return false;
    }
    return true;
}

bool ThreadClientSocket::Connect()
{
    //SOCKET hSocket;
    SOCKADDR_IN servAdr;
    hSocket=socket(PF_INET, SOCK_STREAM, 0);   
    if(hSocket==INVALID_SOCKET)
    {
        ErrorHandling("socket() error");
        return false;
    }

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family=AF_INET;
    servAdr.sin_addr.s_addr=inet_addr(ip); //inet_addr(argv[1]);
    servAdr.sin_port=htons(port);               //htons(atoi(argv[2]));

    if(connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr))==SOCKET_ERROR)
    {
        ErrorHandling("connect() error!");
        return false;
    }
    puts("Connected...........");
    return true;
}



void ThreadClientSocket::run()
{
    printf("ThreadClientSocket::run()--begin\n");

    #define BUF_SIZE 1024
    char message[BUF_SIZE];
    int strLen;

    SendNum=0;
	clientHeart = pFunCreat();
    while(1) 
    {
#if 0
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
            break;
#endif

        Sleep(500) ;

        char  buf[256];
        KL* pKL=(KL*)buf;
        pKL->head=KL_HEAD;
        pKL->key=CMD_TEXT_MESSAGE;
        char* pMsg=buf+sizeof(KL);
        sprintf(pMsg,"Client:%d: Test:Hello World %d\n",CurrObjIndex,SendNum++);
        pKL->len=strlen(pMsg);

        printf("Send Msg: %s", pMsg); 
        send(hSocket, buf, sizeof(KL)+pKL->len,0);

       // send(hSocket, buf, sizeof(buf), 0);
        //strLen=recv(hSocket, message, BUF_SIZE-1, 0);

        strLen=0;
        strLen+=recv(hSocket,buf,sizeof(KL), 0);
        strLen+=recv(hSocket,buf+sizeof(KL),pKL->len,0);
        pMsg=buf+sizeof(KL);
        pMsg[pKL->len]=0;
        printf("Recv Msg: %s", pMsg); 

       // printf("Message from server: %s", message); 
    }
	delete clientHeart;
    closesocket(hSocket);
    WSACleanup();

    printf("ThreadClientSocket::run()--end\n");

    //return 0;
}

void ThreadClientSocket::do_authon()         //发起认证请求
{
	printf("FrameClient::do_authon()--START\n");
	AuthenREQUEST AuthenRequest;
	memset(&AuthenRequest,0,sizeof(AuthenREQUEST));   //初始化，保证用不到的字节为0
	AuthenRequest.kh.pkgHead=PKG_HEAD;
	AuthenRequest.kh.flag_pwd=KEY_HARD;
	AuthenRequest.kh.cmd=CMD_AUTHEN_REQUEST;
	AuthenRequest.kh.cmd_Sub=CMD_ZERO;
	AuthenRequest.kh.cmd_length=sizeof(AuthenREQUEST)-sizeof(KH);
/*        
//认证请求 authentication request
typedef struct _AuthenREQUEST_
{
	KH kh;
	uint32  Machine_ID;           //表示接收机的ID号。本字段不加密。
    char    SoftVersion[16];      //bslbf  128位字段 表示接收机当前接收软件的版本号，由厂家自行定义
    char    Time_Login[20];       //bslbf  160位字段 表示接收机登陆时间 格式为YYYY-MM-DD HH:MM:SS
    uint32  Model_Log;            //日志模式 表示接收机是否上报日志。此字段值为1，表示接收机上报日志；值为2，表示不上报日志。
    uint32  BeatCycle;            //心跳周期heartbeat cycle 表示接收机上报心跳数据报文的周期，单位为秒。如果确认的心跳周期与接收机上报的周期不同，接收机上报周期以此为准。
    uint32  Model_Connect;        //连接方式 表示接收机与招收机远程维护控制子系统保持连接的方式。此字段值为1，表示与发送系统平台保持不间断的长期连接；值为0，表示与发送系统平台建立短连接，即接收机只当有数据传输需要时才与接收机远程维护控制子系统建立连接。
    uint32  HardDiskNum;          //32位无符号整型，表示接收机当前挂载的用于接收电影数字拷贝的硬盘数量。
    uint64  AllDiskCapacity;      //64位无符号整型，表示接收机数据硬盘的容量，单位为字节。
	uint32  CRC32;                 //rpchof 本结构体以上字段加密前的CRC校验值
}AuthenREQUEST;
*/
	/*
	AuthenRequest.Machine_ID=getMachine_ID();//1000;
    char Version[16]="version 0.1";      //getSoftVersion()待实现;
    getSoftVersion(Version);
	PRINTF("Client: getSoftVersion=%s\n",Version);
	//memset(Version,255,16);

	//填充登陆反馈回来的时间--   //待实现
	//填充当前时间
	char Time[20]="2015-12-15 14:25:03";            
	QDateTime DateTime=QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime=DateTime.toString("yyyy-MM-dd hh:mm:ss");
	QByteArray ba = strTime.toLatin1();
    char* ptmp = ba.data();
	//char* ptmp=strTime.toLocal8Bit().data();
	//char* ptmp=strTime.toStdString().c_str();
    memcpy(Time,ptmp,sizeof(Time));
    //strcpy(Time,ptmp);
	char* p1=AuthenRequest.SoftVersion;
    char* p2=AuthenRequest.Time_Login;
    memcpy(p1,Version,sizeof(Version));
    memcpy(p2,Time,sizeof(Time));

    AuthenRequest.Model_Log=1;           
    AuthenRequest.BeatCycle=5;      
    AuthenRequest.Model_Connect=1;  
    AuthenRequest.HardDiskNum=getHardDiskNum();   
	//int getHardDiskNum()         //获取接收机硬盘数量 
    AuthenRequest.AllDiskCapacity=getAllDiskCapacity(); 

	//unsigned long long getAllDiskCapacity()        //获取接收机硬盘总容量  
    AuthenRequest.CRC32=0;  
	char* psend=(char*)&AuthenRequest;
	int   sendsize=sizeof(AuthenREQUEST);
	crc_encry_send(psend,sendsize,sizeof(uint32));
	printf("FrameClient::do_authon()--END\n");
	*/
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