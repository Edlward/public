#ifndef TCPSERVERBYSELECT_H
#define TCPSERVERBYSELECT_H

#include "TcpServerBase.h"

//基于select的服务器 windows 与linux 分别实现

class TcpServerBySelectPrivate;
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
     TcpServerBySelectPrivate* pImpl;
};

#endif

