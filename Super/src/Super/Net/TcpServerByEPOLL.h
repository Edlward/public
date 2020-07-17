#ifndef TCPSERVERBYEPOLL_H
#define TCPSERVERBYEPOLL_H

#include "TcpServerBase.h"
//linux 下基于EPOLL的高性能TCP服务器

USING_NAME_SPACE_MAIN

class TcpServerByEPOLL:public TcpServerBase
{
public:
    TcpServerByEPOLL(void);
    ~TcpServerByEPOLL(void);
};

#endif


