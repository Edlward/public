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
   pCom->setBaudRate((BaudRateType)baudRate);     //������ͬ,ֱ��ǿת
   return true;
}

bool QcwSerialPort::setDataBits(DataBits dataBits)
{
    m_dataBits=dataBits;
   pCom->setDataBits((DataBitsType)dataBits);     //������ͬ,ֱ��ǿת
   return true;
}

bool QcwSerialPort::setParity(Parity parity)
{
    m_parity=parity;
    //pCom->setParity((ParityType)parity);     //���ò�ȫ��ϵͳͬ,����ֱ��ǿת

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
    pCom->setParity(parityType);     //���ò�ȫ��ϵͳͬ,����ֱ��ǿת
    return true;
}

bool QcwSerialPort::setStopBits(StopBits stopBits)
{
    m_stopBits=stopBits;
    //pCom->setStopBits((StopBitsType)stopBits);    //���ò�ȫ��ϵͳͬ,����ֱ��ǿת
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
   pCom->setStopBits(stopBitsType);    //���ò�ȫ��ϵͳͬ,����ֱ��ǿת
   return true;
}

bool QcwSerialPort::setFlowControl(FlowControl flow)
{
    m_flow=flow;
    pCom->setFlowControl((FlowType)flow);     //������ͬ,ֱ��ǿת
   return true;
}


QList<QString> QcwSerialPort::availableDevices()
{
    //û�нӿڻ���豸�Ƿ����,���ع̶���10��
    QList<QString> comList;
    for (int i = 1; i <= 10; i++) 
    {
#ifdef _WIN32
        comList<<QString("COM%1").arg(i);    //windowsϵͳ�����豸����
#else
        comList<<QString("ttyS%1").arg(i);       //Linuxϵͳ�����豸����
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
    return pCom->setBaudRate(baudRate);     //������ͬ,ֱ��ǿת
}

bool QcwSerialPort::setDataBits(DataBits dataBits)
{
    m_dataBits=dataBits;
    return pCom->setDataBits((QSerialPort::DataBits)dataBits);     //������ͬ,ֱ��ǿת
}

bool QcwSerialPort::setParity(Parity parity)
{
    m_parity=parity;
    return  pCom->setParity((QSerialPort::Parity)parity);                //������ͬ,ֱ��ǿת
}

bool QcwSerialPort::setStopBits(StopBits stopBits)
{
    m_stopBits=stopBits;
    return  pCom->setStopBits((QSerialPort::StopBits)stopBits);     //������ͬ,ֱ��ǿת
}

bool QcwSerialPort::setFlowControl(FlowControl flow)
{
    m_flow=flow;
    return pCom->setFlowControl((QSerialPort::FlowControl)flow);     //������ͬ,ֱ��ǿת
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

//    //û�нӿڻ���豸�Ƿ����,���ع̶���10��
//    QList<QString> comList;
//    for (int i = 1; i <= 10; i++) 
//    {
//#ifdef _WIN32
//        comList<<QString("COM%1").arg(i);    //windowsϵͳ�����豸����
//#else
//        comList<<QString("ttyS%1").arg(i);       //Linuxϵͳ�����豸����
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

//������Ϣͨ��Э�� ��C���Խṹ˵��
//1.	��Ϣͷ
struct SPMsgHead
{
    unsigned char Head;     //��Ϣͷ��Ϊĳ��ֵ��0XFE
    unsigned char Key;        //��Ϣ����
    unsigned int Len;          //��Ϣ�峤��
};


