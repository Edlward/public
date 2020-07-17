#ifndef QCWSLIDER_H
#define QCWSLIDER_H

//����ʵ�ֵ������Ĳ������������ռ� һ����������Ƶ������


#include <QMouseEvent>
#include <QSlider>


//type�����Ǵ��ڻ��ߵ���QEvent::User��һ���ܺõ������Ƕ���ö�����ͣ�
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
    void costomSliderClicked();//�Զ������굥���źţ����ڲ��񲢴���
private:
    QLabel* displayLabel;

public:
    void setSampleRate(unsigned int n){SampleRate=n;}
    void setFrameRate(unsigned int n){FrameRate=n;}
    unsigned int getSampleRate() const{return SampleRate;}
    unsigned int getFrameRate() const{return FrameRate;}
    unsigned int getPerSmple() const{return SampleRate/FrameRate;}   //����ÿ֡�������������ݲ�������֡��
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

    //��Ҫ����paintEvent����,����QSS������Ч
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
    //double MultiCoeff;   //�˷�ϵ��
};

#endif



