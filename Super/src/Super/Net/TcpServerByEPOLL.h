#ifndef TCPSERVERBYEPOLL_H
#define TCPSERVERBYEPOLL_H

#include "TcpServerBase.h"
//linux �»���EPOLL�ĸ�����TCP������

USING_NAME_SPACE_MAIN

class TcpServerByEPOLL:public TcpServerBase
{
public:
    TcpServerByEPOLL(void);
    ~TcpServerByEPOLL(void);
};

#endif


