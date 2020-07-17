#ifndef QcwSerialPort_H
#define QcwSerialPort_H

//兼容QT4,QT5的串口通信类封装 by lizulin 20190330
//QT4用第三方库qextserialport实现。为了紧跟QT官方更新修正的bug,Qt5用Qt自带QSerialPort实现


#include <QObject>


//#define USE_3rd_Library

class QSerialPort;
class QextSerialPort;

class QcwSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit QcwSerialPort();
    ~QcwSerialPort();
    enum Directions
    {
        Input=1,
        Output=2,
        AllDirections=Input|Output
    };
    enum BaudRate {
        Baud1200=1200,
        Baud2400=2400,
        Baud4800=4800,
        Baud9600=9600,
        Baud19200=19200,
        Baud38400=38400,
        Baud57600=57600,
        Baud115200=115200,
        UnknownBaud=-1
    };

    enum DataBits {
        Data5=5,
        Data6=6,
        Data7=7,
        Data8=8,
        UnknownDataBits=-1
    };

    enum Parity {
        NoParity=0,
        EvenParity=2,
        OddParity=3,
        SpaceParity=4,
        MarkParity=5,
        UnknownParity=-1
    };

    enum StopBits {
        OneStop=1,
        OneAndHalfStop=3,
        TwoStop=2,
        UnknownStopBits=-1
    };

    enum FlowControl {
        NoFlowControl,
        HardwareControl,
        SoftwareControl,
        UnknownFlowControl=-1
    };

    void setPortName(const QString& sName){m_sName=sName;}//设置即将打开的设备名
    QString PortName()const{return m_sName;}                             //获取正在使用的设备名称

    bool setBaudRate(int baudRate);                        //设置波特率
    int baudRate() const{return m_baudRate;}         //返回当前设定的波特率

    bool setDataBits(DataBits dataBits);                //设置数据位
    DataBits dataBits() const{return m_dataBits;}   //返回当前设定的数据位

    bool setParity(Parity parity);                           //设置奇偶校验位
    Parity parity()const{return m_parity; }             //返回当前设定的奇偶校验位

    bool setStopBits(StopBits stopBits);                 //设置停止位
    StopBits stopBits() const{return m_stopBits;}   //返回当前设定的停止位

    bool setFlowControl(FlowControl flow);                 //设置流控
    FlowControl flowControl() const{return m_flow;}   //返回当前设定的流控

    static QList<QString> availableDevices();        //返回当前可用的串口设备名称
    static QList<int> standardBaudRates();            //返回当前可用的波特率

    int SendData(const char* buf,size_t len);    //向串口写数据
    int ReadData(char* buf,size_t len);             //从串口读数据
    QByteArray readAll();                                //读取全部数据

    qint64 bytesAvailable() const;                  //有效数据字节数
   void flush();                                               //清空写入流

   bool Open();   //打开串口设备
   void Close();   //关闭串口设备
   bool isOpened(){return m_Opened;}

   int error();

   //定时读数据,或者信号触发,根据打开模式决定,待续
private slots:
    void readData();
signals:
    void sig_dataReady();
private:
    QString m_sName;
    int m_baudRate;
    DataBits m_dataBits;
    Parity m_parity;
    StopBits m_stopBits;
    FlowControl m_flow;

#ifdef USE_3rd_Library
    QextSerialPort *pCom;        //串口通信对象
#else
    QSerialPort* pCom;
#endif
	bool m_Opened;
};

#endif // QcwSerialPort_H

