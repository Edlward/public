#ifndef TCPSERVERBYSELECT_H
#define TCPSERVERBYSELECT_H

#include "TcpServerBase.h"

#if defined(_WIN32)
    #include <winsock2.h>
    #pragma comment(lib, "WS2_32")    //显式包含或者使用者包含ws2_32.lib
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif
//基于select的服务器 windows 与linux 分别实现

class TcpServerBySelect:public TcpServerBase
{
public:
    TcpServerBySelect(void);
    ~TcpServerBySelect(void);
    virtual bool initSoctet();                //创建socket
    virtual bool bind();                        //绑定
    virtual bool listen();                       //监听
    virtual void run(); 
private:
#if defined(_WIN32)
        SOCKET hServSock, hClntSock;
#else

#endif
};

#endif
