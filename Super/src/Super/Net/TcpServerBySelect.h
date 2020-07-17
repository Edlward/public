#ifndef TCPSERVERBYSELECT_H
#define TCPSERVERBYSELECT_H

#include "TcpServerBase.h"

//����select�ķ����� windows ��linux �ֱ�ʵ��

class TcpServerBySelectPrivate;
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
     TcpServerBySelectPrivate* pImpl;
};

#endif

