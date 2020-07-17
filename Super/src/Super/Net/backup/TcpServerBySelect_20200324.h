#ifndef TCPSERVERBYSELECT_H
#define TCPSERVERBYSELECT_H

#include "TcpServerBase.h"

#if defined(_WIN32)
    #include <winsock2.h>
    #pragma comment(lib, "WS2_32")    //��ʽ��������ʹ���߰���ws2_32.lib
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif
//����select�ķ����� windows ��linux �ֱ�ʵ��

class TcpServerBySelect:public TcpServerBase
{
public:
    TcpServerBySelect(void);
    ~TcpServerBySelect(void);
    virtual bool initSoctet();                //����socket
    virtual bool bind();                        //��
    virtual bool listen();                       //����
    virtual void run(); 
private:
#if defined(_WIN32)
        SOCKET hServSock, hClntSock;
#else

#endif
};

#endif
