#include "QcwMessageBox.h"
#include <QPainter>
#include <QStyle>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>


QcwMessageBox::QcwMessageBox(QString sInfo,int timeCloseDelay/*=3000*/,QWidget *parent /*=0*/)
    :QDialog(parent)
    ,pTimerToDO(new QTimer)
{
    setWindowTitle(tr("Warning"));
    //this->setFont(QFont("黑体",14));


    //QLabel* LabelLogo=new QLabel("");
    QLabel* LabelInfo=new QLabel(this);
    LabelInfo->setAlignment(Qt::AlignCenter);
    LabelInfo->setWordWrap(true);   //自动换行
    LabelInfo->setText(sInfo);

    QHBoxLayout* HLayoutCtrl=new QHBoxLayout;
    QPushButton* pButtonok=new QPushButton(tr("OK"),this);
    //pButtonok->setFont(QFont("黑体",14));
    pButtonok->setStyleSheet("QPushButton{text-align: center;}");  //left  right center;

    QObject::connect(pButtonok,SIGNAL(clicked()),this,SLOT(close()));   
    pButtonok->setFixedSize(80,40);

    HLayoutCtrl->addStretch(1);
    HLayoutCtrl->addWidget(pButtonok);
    HLayoutCtrl->addStretch(1);

#if 0
    QGridLayout* GLayoutMain=new  QGridLayout();
    //约束比例
    for(int i=0;i<6;i++)
    {
        GLayoutMain->setRowStretch(i,1);
    }
    for(int i=0;i<16;i++)
    {
        GLayoutMain->setColumnStretch(i,1);
    }
    //GLayoutMain->addWidget(,0,0,8,5);   
    //GLayoutMain->addLayout(,0,6,8,10);
    this->setLayout(GLayoutMain);
#endif

    QVBoxLayout* VLayoutMain=new  QVBoxLayout();
    VLayoutMain->addStretch(1);
    VLayoutMain->addWidget(LabelInfo);
    VLayoutMain->addSpacing(20);
    VLayoutMain->addLayout(HLayoutCtrl);
    VLayoutMain->addStretch(1);
    this->setLayout(VLayoutMain);

    this->setFixedSize(300,200);
    QObject::connect(pTimerToDO,SIGNAL(timeout()),this,SLOT(slot_TimeToDoSomeThing()));

    if(timeCloseDelay>0)
    {
        pTimerToDO->start(timeCloseDelay);    //3000  延时时间外部传入  定时关闭
    }
}


QcwMessageBox::~QcwMessageBox()
{
    //  delete ui;
    delete pTimerToDO;
}

void QcwMessageBox::slot_TimeToDoSomeThing()
{
    close();
}
