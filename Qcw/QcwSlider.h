#ifndef QCWSLIDER_H
#define QCWSLIDER_H

//可以实现点击快进的播放器进度条空间 一般用在音视频播放器


#include <QMouseEvent>
#include <QSlider>


//type必须是大于或者等于QEvent::User。一个很好的做法是定义枚举类型：
//typedef enum _MYEVENT_  
//{
//    QEvent::User,
//    MYEVENTA,  
//    MYEVENTB,
//    YEVENT,
//};

class  QLabel;
class QcwSlider : public QSlider
{
    Q_OBJECT
public:
   explicit QcwSlider(QWidget* parent = 0);
   explicit QcwSlider(Qt::Orientation orientation, QWidget *parent = 0);
   void Init();
protected:
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);  
    virtual void mouseMoveEvent(QMouseEvent *ev);
signals:
    void costomSliderClicked();//自定义的鼠标单击信号，用于捕获并处理
private:
    QLabel* displayLabel;

public:
    void setSampleRate(unsigned int n){SampleRate=n;}
    void setFrameRate(unsigned int n){FrameRate=n;}
    unsigned int getSampleRate() const{return SampleRate;}
    unsigned int getFrameRate() const{return FrameRate;}
    unsigned int getPerSmple() const{return SampleRate/FrameRate;}   //返回每帧采样点数，根据采样率与帧率
private:
    unsigned int SampleRate;  
    unsigned int FrameRate;
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


class QcwDoubleSlider : public QSlider
{
    Q_OBJECT
public:
    QcwDoubleSlider(QWidget* pParent=NULL);
    explicit QcwDoubleSlider(Qt::Orientation orientation,QWidget *parent=0);

    //需要重载paintEvent函数,否则QSS设置无效
    virtual void paintEvent(QPaintEvent *event);

    void init();
    void setRange(double Min, double Max);
    void setMinimum(double Min);
    double minimum() const;
    void setMaximum(double Max);
    double maximum() const;
    double value() const;
    void setSingleStep(double step);
    void setPageStep(double step);
public slots:
    //void setValue(int value);
    void innerValueChanged(int value);
    void setValue(double Value);
private:
    void setValue(double Value,bool BlockSignals);  //BlockSignals = false
private slots:
signals :
    void valueChanged(double Value);
    void rangeChanged(double Min,double Max);
private:
    //double MultiCoeff;   //乘法系数
};

#endif



