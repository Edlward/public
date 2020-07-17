#include "QcwSlider.h"

#include <QEvent>
#include <QApplication>
#include <QCoreApplication>
#include <QStyleOption>
#include <QPainter>
#include <QLabel> 

#include "Super/Basic/BasicOperation.h"
 

QcwSlider::QcwSlider(QWidget* parent /*= 0*/) : QSlider(parent)
{
    Init();
}

QcwSlider::QcwSlider(Qt::Orientation orientation, QWidget *parent /*= 0*/) :QSlider(orientation,parent)
{
    Init();
}

void QcwSlider::Init()
{
    displayLabel=new QLabel(this);  
    //displayLabel->setFixedSize(QSize(20,20));  
    //设置游标背景为白色  
    //displayLabel->setAutoFillBackground(true);  
    QPalette palette;  
    //palette.setColor(QPalette::Background, Qt::white);  
    palette.setColor(QPalette::Background, Qt::gray);  
    palette.setColor(QPalette::WindowText,Qt::red);

    displayLabel->setPalette(palette);  
    displayLabel->setAlignment(Qt::AlignCenter);  
    displayLabel->setVisible(false);  
    displayLabel->move(0,9);  

    //不可行 
    //QFont ft;
    //ft.setPointSize(6);
    //displayLabel->setFont(ft);
    //displayLabel->setFont(QFont("",2));

    //displayLabel->setPaletteForeGroundColor(QColor(0,0,255));
    //displayLabel->adjustSize();
    displayLabel->setStyleSheet("QLabel{font-size:10px;color:#FF0000;}");   

    //SampleRate=SRateDefault;
    //FrameRate=FRateDefault;
 
    SampleRate=48000;
    FrameRate=24;
}

void QcwSlider::mousePressEvent(QMouseEvent *ev)
{
#if 0
    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
    QSlider::mousePressEvent(ev);
    //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
    double pos = ev->pos().x() / (double)width();
    int newpos=pos*(maximum() - minimum()) + minimum();
    //blockSignals(true);
    setValue(newpos);
    //blockSignals(false);

    //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
    QCoreApplication::sendEvent(parentWidget(), &evEvent);

#else
    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况  
    //屏蔽拖动，否则飘移
    //QSlider::mousePressEvent(ev);

    int dur =maximum()-minimum();
    int pos =minimum() + dur * ((double)ev->x()/width());
   // if(pos!=sliderPosition())
    //if(pos!=value())
    {
        printf("clicked pos=%d\n",pos);
        setValue(pos);
    }

    if(!displayLabel->isVisible())  
    {  
        displayLabel->setVisible(true);  
        //displayLabel->setText(QString::number(this->value()));    //显示百分比
        char sTimeCode[64];

        TimeCodeNumtoString(this->value(),FrameRate,sTimeCode);
        displayLabel->setText(QString(sTimeCode));                     //显示时码
    }
    QSlider::mousePressEvent(ev);


    //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
    //QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
    //QCoreApplication::sendEvent(parentWidget(), &evEvent);
    //return QObject::eventFilter(this,ev);

    //发送自定义的鼠标单击信号
    //emit costomSliderClicked();
#endif
}

void QcwSlider::mouseReleaseEvent(QMouseEvent *ev)
{
    if(displayLabel->isVisible())  
    {  
        displayLabel->setVisible(false);  
    }  
    QSlider::mouseReleaseEvent(ev);  
}

void QcwSlider::mouseMoveEvent(QMouseEvent *ev)
{
      int dur =maximum()-minimum();
      int pos =minimum() + dur * ((double)ev->x()/width());
      printf("mouseMove pos=%d\n",pos);

      //displayLabel->setText(QString::number(this->value()));  //显示百分比
      char sTimeCode[64];
      TimeCodeNumtoString(this->value(),FrameRate,sTimeCode);    //24Hz
      displayLabel->setText(QString(sTimeCode));                            //显示时码
      displayLabel->move((this->width()-displayLabel->width())*this->value()/(this->maximum()-this->minimum()),9);  
      QSlider::mouseMoveEvent(ev);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


static const double MultiCoeff=1000.0;  //乘法系数
QcwDoubleSlider::QcwDoubleSlider(QWidget* pParent /*= NULL*/) :
    QSlider(pParent)
    //,MultiCoeff(1000.0)
{
    init();
}

QcwDoubleSlider::QcwDoubleSlider(Qt::Orientation orientation, QWidget *parent /*= 0*/)
    :QSlider(orientation,parent)
    //,MultiCoeff(1000.0)
{
    init();
}

void QcwDoubleSlider::paintEvent(QPaintEvent *event)
{
    //QSlider::paintEvent(event);
    //Q_UNUSED(event); 
    QStyleOption styleOpt; 
    styleOpt.init(this); 
    QPainter painter(this); 
    style()->drawPrimitive(QStyle::PE_Widget, &styleOpt, &painter, this);  //CC_Slider  PE_Widget
   QSlider::paintEvent(event);
}

void QcwDoubleSlider::init()
{
    //setObjectName("QcwDoubleSlider");
    //this->setAttribute(Qt::WA_StyledBackground); // 启用 QSS
    //setStyleSheet("QSlider::handle:horizontal{width:80px;height:80px;}");  //

    connect(this,SIGNAL(valueChanged(int)),this,SLOT(innerValueChanged(int)));   //循环触发了 屏蔽
    setSingleStep(1);
    //setOrientation(Qt::Horizontal);
    setFocusPolicy(Qt::NoFocus);
}

//void QDoubleSlider::setValue(int Value)
//{
//    //QSlider::setValue(Value * m_Multiplier);// add new
//
//    emit valueChanged((double)Value/m_Multiplier);
//}


void QcwDoubleSlider::innerValueChanged(int Value)
{
    //::printf("QDoubleSlider::innerValueChanged()%d\n",Value);

    //QSlider::setValue(Value * m_Multiplier);// add new
    emit valueChanged((double)Value/MultiCoeff);
}

//void QcwDoubleSlider::setValue(double Value)
//{
//    setValue(Value,false);
//}


//void QcwDoubleSlider::setValue(double Value, bool BlockSignals)
//{
//    //old
//    //QSlider::blockSignals(BlockSignals);
//    //QSlider::setValue(Value * m_Multiplier);
//    //if (!BlockSignals)
//    //    emit valueChanged(Value);
//    //QSlider::blockSignals(false);
//
//    int oldValue=QSlider::value();
//    int newValue=Value*m_Multiplier;
//
//    //::printf("QDoubleSlider::setValue()%f block:%d old;%d new:%d\n",Value,BlockSignals,oldValue,newValue);
//
//
//    if (oldValue!=newValue)
//    {
//        //QSlider::blockSignals(true);
//        QSlider::blockSignals(BlockSignals);
//        QSlider::setValue(newValue);
//        QSlider::blockSignals(false);
//
//        if (!BlockSignals)
//        {
//            if (!signalsBlocked())   //没有屏蔽才发送信号
//            {
//                 emit valueChanged(Value);
//            }
//        }
//    }
//}

void QcwDoubleSlider::setValue(double Value)
{
    int oldValue=QSlider::value();
    int newValue=Value*MultiCoeff;
    if (oldValue!=newValue)   //值变化才实际调用设定
    {
    QSlider::setValue(newValue);
    if (!signalsBlocked())
    {
        emit valueChanged(Value);
    }
}
}

void QcwDoubleSlider::setValue(double Value, bool BlockSignals)
{
    QSlider::blockSignals(!BlockSignals);
    QcwDoubleSlider::setValue(Value);
    QSlider::blockSignals(false);
}

void QcwDoubleSlider::setRange(double Min, double Max)
{
    QSlider::setRange(Min * MultiCoeff, Max * MultiCoeff);

    emit rangeChanged(Min, Max);
}

void QcwDoubleSlider::setMinimum(double Min)
{
    QSlider::setMinimum(Min * MultiCoeff);
    emit rangeChanged(minimum(), maximum());
}

double QcwDoubleSlider::minimum() const
{
    return QSlider::minimum() / MultiCoeff;
}

void QcwDoubleSlider::setMaximum(double Max)
{
    QSlider::setMaximum(Max * MultiCoeff);
    emit rangeChanged(minimum(), maximum());
}

double QcwDoubleSlider::maximum() const
{
    return QSlider::maximum()/MultiCoeff;
}

double QcwDoubleSlider::value() const
{
    int Value = QSlider::value();
    return (double)Value/MultiCoeff;
}

void QcwDoubleSlider::setSingleStep(double step)
{
    QSlider::setSingleStep(step*MultiCoeff);
}

void QcwDoubleSlider::setPageStep(double step)
{
    QSlider::setPageStep(step*MultiCoeff);
}
