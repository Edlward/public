#include "QcwSerialPort.h"

#include <QList>
#include <QDebug>

#ifdef USE_3rd_Library
    #include "qextserialport/qextserialport.h"
#else
    //#include <QSerialPort>
    //#include <QSerialPortInfo>
    #include <QtSerialPort/QSerialPort>
    #include <QtSerialPort/QSerialPortInfo>
#endif



#ifdef USE_3rd_Library
QcwSerialPort::QcwSerialPort() 
{
     pCom=new QextSerialPort;
}

QcwSerialPort::~QcwSerialPort()
{
     delete pCom;
}

bool QcwSerialPort::setBaudRate(int baudRate)
{
   m_baudRate=baudRate;
   pCom->setBaudRate((BaudRateType)baudRate);     //数置相同,直接强转
   return true;
}

bool QcwSerialPort::setDataBits(DataBits dataBits)
{
    m_dataBits=dataBits;
   pCom->setDataBits((DataBitsType)dataBits);     //数置相同,直接强转
   return true;
}

bool QcwSerialPort::setParity(Parity parity)
{
    m_parity=parity;
    //pCom->setParity((ParityType)parity);     //数置不全部系统同,不能直接强转

    ParityType parityType=PAR_ODD;
    switch (parity)
    {
    case QcwSerialPort::NoParity:
        parityType=PAR_NONE;
        break;
    case QcwSerialPort::EvenParity:
        parityType=PAR_EVEN;
        break;
    case QcwSerialPort::OddParity:
        parityType=PAR_ODD;
        break;
    case QcwSerialPort::SpaceParity:
        parityType=PAR_SPACE;
        break;
    case QcwSerialPort::MarkParity:
        parityType=PAR_MARK;
        break;
    case QcwSerialPort::UnknownParity:
        break;
    default:
        break;
    }
    pCom->setParity(parityType);     //数置不全部系统同,不能直接强转
    return true;
}

bool QcwSerialPort::setStopBits(StopBits stopBits)
{
    m_stopBits=stopBits;
    //pCom->setStopBits((StopBitsType)stopBits);    //数置不全部系统同,不能直接强转
    StopBitsType stopBitsType=STOP_1;
    switch (stopBits)
    {
    case QcwSerialPort::OneStop:
        stopBitsType=STOP_1;
        break;
    case QcwSerialPort::OneAndHalfStop:
        stopBitsType=STOP_1_5;
        break;
    case QcwSerialPort::TwoStop:
        stopBitsType=STOP_2;
        break;
    case QcwSerialPort::UnknownStopBits:
        break;
    default:
        break;
    }
   pCom->setStopBits(stopBitsType);    //数置不全部系统同,不能直接强转
   return true;
}

bool QcwSerialPort::setFlowControl(FlowControl flow)
{
    m_flow=flow;
    pCom->setFlowControl((FlowType)flow);     //数置相同,直接强转
   return true;
}


QList<QString> QcwSerialPort::availableDevices()
{
    //没有接口获得设备是否可用,返回固定的10个
    QList<QString> comList;
    for (int i = 1; i <= 10; i++) 
    {
#ifdef _WIN32
        comList<<QString("COM%1").arg(i);    //windows系统串口设备名称
#else
        comList<<QString("ttyS%1").arg(i);       //Linux系统串口设备名称
#endif
    }
    return comList;
}

QList<int> QcwSerialPort::standardBaudRates()
{
    QList<int> BaudRateList;
    BaudRateList<<1200<<2400<<4800<<9600<<19200<<38400<<57600<<115200;
    return BaudRateList;
}


int QcwSerialPort::SendData(const char* buf,size_t len)
{
    return pCom->write(buf,len);
}

int QcwSerialPort::ReadData(char* buf,size_t len)
{
     return pCom->read(buf,len);
}

QByteArray QcwSerialPort::readAll()
{
    return pCom->readAll();
}

qint64 QcwSerialPort::bytesAvailable() const
{
    return pCom->bytesAvailable();
}

void QcwSerialPort::flush()
{
    pCom->flush();
}

#else


QcwSerialPort::QcwSerialPort() 
{
    pCom=new QSerialPort;
}

QcwSerialPort::~QcwSerialPort()
{
    delete pCom;
}

bool QcwSerialPort::setBaudRate(int baudRate)
{
    m_baudRate=baudRate;
    return pCom->setBaudRate(baudRate);     //数置相同,直接强转
}

bool QcwSerialPort::setDataBits(DataBits dataBits)
{
    m_dataBits=dataBits;
    return pCom->setDataBits((QSerialPort::DataBits)dataBits);     //数置相同,直接强转
}

bool QcwSerialPort::setParity(Parity parity)
{
    m_parity=parity;
    return  pCom->setParity((QSerialPort::Parity)parity);                //数置相同,直接强转
}

bool QcwSerialPort::setStopBits(StopBits stopBits)
{
    m_stopBits=stopBits;
    return  pCom->setStopBits((QSerialPort::StopBits)stopBits);     //数置相同,直接强转
}

bool QcwSerialPort::setFlowControl(FlowControl flow)
{
    m_flow=flow;
    return pCom->setFlowControl((QSerialPort::FlowControl)flow);     //数置相同,直接强转
}


QList<QString> QcwSerialPort::availableDevices()
{
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();
    QList<QString> comList;
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos)
    {
        comList<<serialPortInfo.portName();
    }
    return comList;

//    //没有接口获得设备是否可用,返回固定的10个
//    QList<QString> comList;
//    for (int i = 1; i <= 10; i++) 
//    {
//#ifdef _WIN32
//        comList<<QString("COM%1").arg(i);    //windows系统串口设备名称
//#else
//        comList<<QString("ttyS%1").arg(i);       //Linux系统串口设备名称
//#endif
//    }
//    return comList;
}


QList<int> QcwSerialPort::standardBaudRates()
{
    //QList<int> BaudRateList;
    //BaudRateList<<1200<<2400<<4800<<9600<<19200<<38400<<57600<<115200;
    //return BaudRateList;
    return QSerialPortInfo::standardBaudRates();
}


int QcwSerialPort::SendData(const char* buf,size_t len)
{
    return pCom->write(buf,len);
}

int QcwSerialPort::ReadData(char* buf,size_t len)
{
    return pCom->read(buf,len);
}

QByteArray QcwSerialPort::readAll()
{
    return pCom->readAll();
}

qint64 QcwSerialPort::bytesAvailable() const
{
    return pCom->bytesAvailable();
}

int QcwSerialPort::error()
{
    return pCom->error();
}

#endif


bool QcwSerialPort::Open()
{
    pCom->open(QIODevice::ReadWrite);
    m_Opened=pCom->isOpen();
    return m_Opened;
}

void QcwSerialPort::Close()
{
    return pCom->close();
}


void QcwSerialPort::readData()
{

}

//串口消息通信协议 以C语言结构说明
//1.	消息头
struct SPMsgHead
{
    unsigned char Head;     //消息头恒为某个值如0XFE
    unsigned char Key;        //消息类型
    unsigned int Len;          //消息体长度
};


