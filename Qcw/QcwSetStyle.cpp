#include "QcwSetStyle.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>

//Qt widgets
#include <QGroupBox>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QSlider>
#include <QColorDialog>   //for get color
#include <QFontDialog>    //for get font

#include <QDebug>


#include<QApplication>  
#include <QClipboard>   //剪切板

#include <stdint.h>

//#include "../public/Super/src/Super/Basic/global.h"

#include "Super/Basic/SuperTime.h"


void QcwGetColor::setUI()
{
    QLabel* labelGetcol=new QLabel(tr("getColor:"));
    QPushButton* pBtnStd=new QPushButton(tr("std"));
    QPushButton* pBtnCustom=new QPushButton(tr("Custom"));
    connect(pBtnStd,SIGNAL(clicked()),this,SLOT(slot_getColorBystd()));
    connect(pBtnCustom,SIGNAL(clicked()),this,SLOT(slot_getColorBycostom()));

    QHBoxLayout* HLayoutMain=new QHBoxLayout;
    HLayoutMain->addWidget(labelGetcol);
    HLayoutMain->addWidget(pBtnStd);
    HLayoutMain->addWidget(pBtnCustom);
    this->setLayout(HLayoutMain);
}

void QcwGetColor::slot_getColorBystd()
{
    QColor color = QColorDialog::getColor(Qt::black);
     if (color.isValid())
      {
             qDebug() << color.redF() << color.greenF() << color.blueF();
      }
}


void QcwGetColor::slot_getColorBycostom()
{
    QcwColorTable dlg;
    dlg.exec();
}

//
//void yuv2rgb(uint8_t y, uint8_t u, uint8_t v,uint8_t& r, uint8_t& g, uint8_t& b)
//{
//    r=y + (1.370705 * (v-128));
//    r=y-(0.698001 * (v-128)) - (0.337633 * (u-128));
//    b=y+(1.732446*(u-128));
//}


void yuv2rgb(uint8_t y,int8_t u,int8_t v,uint8_t& r, uint8_t& g, uint8_t& b)
{
    r=y + (1.370705 * (v-128));
    r=y-(0.698001 * (v-128)) - (0.337633 * (u-128));
    b=y+(1.732446*(u-128));
}

void QcwColorTable::setUI()
{

    //把颜色表分成4组,每组16行8列
    const unsigned int Groups=8;
    const unsigned int Rows=16;
    const unsigned int Colums=8;
    
    //每一个颜色标签宽高。
    const unsigned int ColUintWidth=80;
    const unsigned int ColUintHeight=30;


    unsigned char tY=0;
    int tU=16;
    int tV=16;

    unsigned char tR=0;
    unsigned char tG=0;
    unsigned char tB=0;
    ListColorBtn.reserve(576);

    unsigned int Index=1;
    QVBoxLayout* HLayougroupAll=new QVBoxLayout;
    HLayougroupAll->setMargin(0);
    HLayougroupAll->setSpacing(0);

    const QColor txtCol(0,0,0);



    //按照YUV变化的颜色表 待续
    for (int gp= 0; gp<Groups; gp++)
    {
        QHBoxLayout* HLayougroupone=new QHBoxLayout;
        HLayougroupone->setMargin(0);
        tY=0;
        tG=0;
        for (int row = 0;row<Rows;row++)
        {
            QVBoxLayout* VLayoutCol=new QVBoxLayout;
            VLayoutCol->setMargin(0);
            tU=0;
             tB=0;
            for (int colum= 0;colum< Colums;colum++)
            {
                QPushButton* btn=new QPushButton();
                btn->setFixedSize(ColUintWidth,ColUintHeight);
                btn->setContentsMargins(0,0,0,0);
                //btn->setText(QString::number(Index++));
                //btn->setFlat(true);

                //yuv2rgb(tY,tU,tV,tR,tG,tB);
                //QColor brgCol(tR,tG,tB);
                QColor brgCol=QColor::fromHsvF((double)gp/Groups,(double)colum/Colums,(double)row/Rows);


                QVariant var;
                var.setValue(txtCol);
                btn->setProperty("privateDataTextColor",var);

                var.setValue(brgCol);
                btn->setProperty("privateDataBackColor",var);  
                connect(btn,SIGNAL(clicked()),this,SLOT(slot_color_button_clicked()));

                setButtonStyle(btn,txtCol,brgCol);

                //QString sCol=QString("QPushButton{background-color:rgb(%1,%2,%3);}")
                //    .arg(QString::number(tR)).arg(QString::number(tG)).arg(QString::number(tB));
                //btn->setStyleSheet(sCol);

                //char tmpbuf[256];
                //sprintf(tmpbuf,"QPushButton{background-color:rgb(%d,%d,%d);}",tR,tG,tB);
                //btn->setStyleSheet(tmpbuf);

                ListColorBtn.push_back(btn);
                VLayoutCol->addWidget(btn);

                tB+=256/(Colums-1);
                tU+=256/(Colums-1);
            }
            HLayougroupone->addLayout(VLayoutCol);
            tG+=256/(Rows-1);
            tY+=256/(Rows-1);
       }
        HLayougroupAll->addLayout(HLayougroupone);
        tR+=256/(Groups-1);
      tV+=256/(Groups-1);
    }

    //放3个纯色和一个黑白色
    for (int k = 0; k < 4; k++)
    {
        tR=0;
        tG=0;
        tB=0;

         QHBoxLayout* HLayougroupone=new QHBoxLayout;
         for (int j= 0;j<Rows;j++)
         {
             QPushButton* btn=new QPushButton();
             btn->setFixedSize(ColUintWidth,ColUintHeight);
             btn->setContentsMargins(0,0,0,0);
             btn->setText(QString::number(Index++));
             //yuv2rgb(tY,tU,tV,tR,tG,tB);

             unsigned char useR=tR;
             unsigned char useG=tG;
             unsigned char useB=tB;
             if (k==0)
             {
                 useG=useB=0;
             }
             else if(k==1)
             {
                 useR=useB=0;
             }
             else if(k==2)
             {
                 useR=useG=0;
             }

             //QString sCol=QString("QPushButton{background-color:rgb(%1,%2,%3);}")
             //    .arg(QString::number(useR)).arg(QString::number(useG)).arg(QString::number(useB));
             //btn->setStyleSheet(sCol);

             //char tmpbuf[256];
             //sprintf(tmpbuf,"QPushButton{background-color:rgb(%d,%d,%d);}",useR,useG,useB);
             //btn->setStyleSheet(tmpbuf);

             QColor brgCol(useR,useG,useB);

             QVariant var;
             var.setValue(txtCol);
             btn->setProperty("privateDataTextColor",var);

             var.setValue(brgCol);
             btn->setProperty("privateDataBackColor",var);  
             connect(btn,SIGNAL(clicked()),this,SLOT(slot_color_button_clicked()));

             setButtonStyle(btn,txtCol,brgCol);

             ListColorBtn.push_back(btn);
             HLayougroupone->addWidget(btn);

            tR+=256/(Rows-1);
            tG+=256/(Rows-1);
            tB+=256/(Rows-1);
         }
        HLayougroupAll->addLayout(HLayougroupone);
    }

    QLabel* labelPreView=new QLabel(tr("PreView:"));
    LabelPreViewPic=new QLabel;
    //LabelPreViewPic->setFixedSize(120,30);
    LabelPreViewPic->setFixedWidth(300);
    LabelPreViewPic->setFixedHeight(60);
    LabelPreViewPic->setText(tr("Test String")+tr("123 Test"));


    //QLabel* labelCurr=new QLabel("Curr:");
    QLabel* labelTextrgb=new QLabel("Text: RGB");
    lableTextRGBVal=new QLabel();
    lableTextRGBVal->setFixedWidth(80);
    lableTextRGBVal->setTextInteractionFlags(Qt::TextSelectableByMouse);//文本可以复制


    QLabel* labelTexthex=new QLabel("Hex");
    lableTextHexVal=new QLabel();
    lableTextHexVal->setFixedWidth(50);
    lableTextHexVal->setTextInteractionFlags(Qt::TextSelectableByMouse);//文本可以复制
    QPushButton* pBtnCopyTextHexVal=new QPushButton("copy");
    pBtnCopyTextHexVal->setFixedWidth(30);
    connect(pBtnCopyTextHexVal,SIGNAL(clicked()),this,SLOT(slot_CopyTextHexVal()));

    spinBoxTextR=new QSpinBox;
    spinBoxTextR->setRange(0,255);
    spinBoxTextG=new QSpinBox;
    spinBoxTextG->setRange(0,255);
    spinBoxTextB=new QSpinBox;
    spinBoxTextB->setRange(0,255);
    connect(spinBoxTextR,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_changed()));
    connect(spinBoxTextG,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_changed()));
    connect(spinBoxTextB,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_changed()));


    //QLabel* labelCurr=new QLabel("Curr:");
    QLabel* labelrgb=new QLabel("Back: RGB");  //Background
    lableRGBVal=new QLabel();
    lableRGBVal->setFixedWidth(80);
    lableRGBVal->setTextInteractionFlags(Qt::TextSelectableByMouse);//文本可以复制
    QLabel* labelhex=new QLabel("Hex");
    lableHexVal=new QLabel();
    lableHexVal->setFixedWidth(50);
    lableHexVal->setTextInteractionFlags(Qt::TextSelectableByMouse);//文本可以复制
    QPushButton* pBtnCopyHexVal=new QPushButton("copy");
    pBtnCopyHexVal->setFixedWidth(30);
    connect(pBtnCopyHexVal,SIGNAL(clicked()),this,SLOT(slot_CopyHexVal()));

    spinBoxR=new QSpinBox;
    spinBoxR->setRange(0,255);
    spinBoxG=new QSpinBox;
    spinBoxG->setRange(0,255);
    spinBoxB=new QSpinBox;
    spinBoxB->setRange(0,255);
    connect(spinBoxR,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_changed()));
    connect(spinBoxG,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_changed()));
    connect(spinBoxB,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_changed()));



    QHBoxLayout* HLayoutColEditL1=new QHBoxLayout;
    HLayoutColEditL1->addWidget(labelTextrgb);
    HLayoutColEditL1->addWidget(lableTextRGBVal);
    HLayoutColEditL1->addWidget(labelTexthex);
    HLayoutColEditL1->addWidget(lableTextHexVal);
    HLayoutColEditL1->addWidget(pBtnCopyTextHexVal);
    HLayoutColEditL1->addStretch(1);
    HLayoutColEditL1->addWidget(spinBoxTextR);
    HLayoutColEditL1->addWidget(spinBoxTextG);
    HLayoutColEditL1->addWidget(spinBoxTextB);
    HLayoutColEditL1->addStretch(5);

    QHBoxLayout* HLayoutColEditL2=new QHBoxLayout;
    HLayoutColEditL2->addWidget(labelrgb);
    HLayoutColEditL2->addWidget(lableRGBVal);
    HLayoutColEditL2->addWidget(labelhex);
    HLayoutColEditL2->addWidget(lableHexVal);
    HLayoutColEditL2->addWidget(pBtnCopyHexVal);
    HLayoutColEditL2->addStretch(1);
    HLayoutColEditL2->addWidget(spinBoxR);
    HLayoutColEditL2->addWidget(spinBoxG);
    HLayoutColEditL2->addWidget(spinBoxB);
    HLayoutColEditL2->addStretch(5);

     QVBoxLayout* VLayoutColEdit=new QVBoxLayout;
     VLayoutColEdit->addLayout(HLayoutColEditL1);
     VLayoutColEdit->addLayout(HLayoutColEditL2);


    ////文字模式或背景模式
    //QRadioButton* radioButtonbackgrd=new QRadioButton(tr("backgrd"));
    //QRadioButton* radioButtonText=new QRadioButton(tr("Text"));
    //radioButtonbackgrd->setChecked(true);


    QPushButton* btnOK=new QPushButton(tr("OK"));
    QPushButton* btnCancle=new QPushButton(tr("Cancle"));
    connect(btnOK,SIGNAL(clicked()),this,SLOT(slot_ok()));
    connect(btnCancle,SIGNAL(clicked()),this,SLOT(slot_cancel()));

    QPushButton* btnTextColor=new QPushButton(tr("TextColor"));
    connect(btnTextColor,SIGNAL(clicked()),this,SLOT(slot_getTextColor()));

    QPushButton* btnTextFont=new QPushButton(tr("Font"));
    connect(btnTextFont,SIGNAL(clicked()),this,SLOT(slot_getFont()));

    QRadioButton* btnReverse=new QRadioButton(tr("Reverse"));
    connect(btnReverse,SIGNAL(toggled(bool)),this,SLOT(slot_ReverseTxtBackColor()));


    QHBoxLayout* HLayoutCtrl=new QHBoxLayout;
    HLayoutCtrl->addWidget(labelPreView);
    HLayoutCtrl->addWidget(LabelPreViewPic);
    //HLayoutCtrl->addWidget(labelCurr);

    //HLayoutCtrl->addWidget(labelrgb);
    //HLayoutCtrl->addWidget(lableRGBVal);
    //HLayoutCtrl->addWidget(labelhex);
    //HLayoutCtrl->addWidget(lableHexVal);
    //HLayoutCtrl->addWidget(spinBoxR);
    //HLayoutCtrl->addWidget(spinBoxG);
    //HLayoutCtrl->addWidget(spinBoxB);

    HLayoutCtrl->addLayout(VLayoutColEdit);
    HLayoutCtrl->addStretch(10);
    HLayoutCtrl->addWidget(btnOK);
    HLayoutCtrl->addWidget(btnCancle);
    HLayoutCtrl->addStretch(10);

    //HLayoutCtrl->addWidget(radioButtonbackgrd);
    //HLayoutCtrl->addWidget(radioButtonText);
    HLayoutCtrl->addWidget(btnTextColor);
    HLayoutCtrl->addWidget(btnTextFont);
    HLayoutCtrl->addSpacing(30);
    HLayoutCtrl->addWidget(btnReverse);

    QWidget* pWidgetColorTable=new QWidget;
    pWidgetColorTable->setLayout(HLayougroupAll);
    QScrollArea* psc=new QScrollArea;
    psc->setWidget(pWidgetColorTable);
    psc->setFixedWidth(Rows*ColUintWidth+80);  //固定宽度,只允许上下滚动
    psc->setMinimumHeight(600);

    QVBoxLayout* VLayoutMain=new QVBoxLayout;
    VLayoutMain->addWidget(psc);
    VLayoutMain->addLayout(HLayoutCtrl);
    this->setLayout(VLayoutMain);

    //首次刷新
    //spinBoxR->setValue(0);//无效
    //一定要初始化正确
    tR=m_TextColor.red();
    tG=m_TextColor.green();
    tB=m_TextColor.blue();
    spinBoxTextR->blockSignals(true);
    spinBoxTextG->blockSignals(true);
    spinBoxTextB->blockSignals(true);
    spinBoxTextR->setValue(tR);
    spinBoxTextG->setValue(tG);
    spinBoxTextB->setValue(tB);
    spinBoxTextR->blockSignals(false);
    spinBoxTextG->blockSignals(false);
    spinBoxTextB->blockSignals(false);

    int bR=m_Color.red();
    int bG=m_Color.green();
    int bB=m_Color.blue();
    spinBoxR->blockSignals(true);
    spinBoxG->blockSignals(true);
    spinBoxB->blockSignals(true);
    spinBoxR->setValue(bR);
    spinBoxG->setValue(bG);
    spinBoxB->setValue(bB);
    spinBoxR->blockSignals(false);
    spinBoxG->blockSignals(false);
    spinBoxB->blockSignals(false);

    do_update_display();
}


void QcwColorTable::slot_color_button_clicked()
{
     printf("QcwColorTable::slot_color_button_clicked()\n");

    QPushButton* btn=qobject_cast<QPushButton*>(QObject::sender());

    QVariant var;
    var=btn->property("privateDataTextColor");  
    m_TextColor=var.value<QColor>();

    var=btn->property("privateDataBackColor");  
    m_Color=var.value<QColor>();

    int tR=m_TextColor.red();
    int tG=m_TextColor.green();
    int tB=m_TextColor.blue();
    spinBoxTextR->blockSignals(true);
    spinBoxTextG->blockSignals(true);
    spinBoxTextB->blockSignals(true);
    spinBoxTextR->setValue(tR);
    spinBoxTextG->setValue(tG);
    spinBoxTextB->setValue(tB);
    spinBoxTextR->blockSignals(false);
    spinBoxTextG->blockSignals(false);
    spinBoxTextB->blockSignals(false);


    int bR=m_Color.red();
    int bG=m_Color.green();
    int bB=m_Color.blue();
    spinBoxR->blockSignals(true);
    spinBoxG->blockSignals(true);
    spinBoxB->blockSignals(true);
    spinBoxR->setValue(bR);
    spinBoxG->setValue(bG);
    spinBoxB->setValue(bB);
    spinBoxR->blockSignals(false);
    spinBoxG->blockSignals(false);
    spinBoxB->blockSignals(false);
    do_update_display();

    if (!this->signalsBlocked())
    {
        emit sig_ColorChanged(m_Color);
        emit sig_TextColorChanged(m_TextColor);
    }
}


void QcwColorTable::slot_ok()
{
    //m_Color
    this->close();
}

void QcwColorTable::slot_cancel()
{
    //m_Color
    this->close();
}

void QcwColorTable::slot_spinbox_changed()
{
    int bR=spinBoxR->value();
    int bG=spinBoxG->value();
    int bB=spinBoxB->value();
    m_Color=QColor(bR,bG,bB);

    int tR=spinBoxTextR->value();
    int tG=spinBoxTextG->value();
    int tB=spinBoxTextB->value();
    m_TextColor=QColor(tR,tG,tB);

    do_update_display();
}


void QcwColorTable::slot_getTextColor()
{
    QColor color = QColorDialog::getColor(Qt::black);
    if (!color.isValid())
    {
        qDebug() << color.redF() << color.greenF() << color.blueF();
        return;
    }

    m_TextColor=color;
    for (size_t n=0;n<ListColorBtn.size();n++)
    {
        QPushButton* btn=ListColorBtn[n];
        //QColor  txtCol=btn->property("privateDataTextColor").value<QColor>();
        QColor  brgCol=btn->property("privateDataBackColor").value<QColor>();

        //把选中文本颜色保存到所有控件
        QVariant var;
        var.setValue(m_TextColor);
        btn->setProperty("privateDataTextColor",var);

        setButtonStyle(btn,m_TextColor,brgCol);
    }


    do_update_display();
}

void QcwColorTable::slot_getFont()
{
    bool ok;
    QFont font=QFontDialog::getFont(&ok,QFont("Courier New",10, QFont::Bold), this,"Font Dialog");
    m_Font=font;
    LabelPreViewPic->setFont(m_Font);
}


void QcwColorTable::slot_ReverseTxtBackColor()
{
    for (size_t n=0;n<ListColorBtn.size();n++)
    {
        QPushButton* btn=ListColorBtn[n];
        QColor  txtCol=btn->property("privateDataTextColor").value<QColor>();
        QColor  brgCol=btn->property("privateDataBackColor").value<QColor>();
        QVariant var;
        var.setValue(txtCol);
        btn->setProperty("privateDataBackColor",var);
        var.setValue(brgCol);
        btn->setProperty("privateDataTextColor",var);

        setButtonStyle(btn,brgCol,txtCol);     //反转颜色后刷新控件
    }
}

void QcwColorTable::slot_CopyTextHexVal()
{
    QString scopy=lableTextHexVal->text();
    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    //QString originalText = clipboard->text();	                  //获取剪贴板上文本信息
    clipboard->setText(scopy);		                                  //设置剪贴板内容
}

void QcwColorTable::slot_CopyHexVal()
{
    QString scopy=lableHexVal->text();
    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    //QString originalText = clipboard->text();	                  //获取剪贴板上文本信息
    clipboard->setText(scopy);		                                  //设置剪贴板内容
}

void QcwColorTable::do_update_display()
{
    int tR=m_TextColor.red();
    int tG=m_TextColor.green();
    int tB=m_TextColor.blue();

    int bR=m_Color.red();
    int bG=m_Color.green();
    int bB=m_Color.blue();

    //QString shex=QString("#%1%2%3")
    //    .arg(QString::number(R,16))
    //    .arg(QString::number(G,16))
    //    .arg(QString::number(B,16));
    //lableHexVal->setText(shex.toUpper());


    char tmpbuf[256];
    sprintf(tmpbuf,"(%d,%d,%d)",tR,tG,tB);
    lableTextRGBVal->setText(tmpbuf);

    sprintf(tmpbuf,"#%02X%02X%02X",tR,tG,tB);
    lableTextHexVal->setText(tmpbuf);

    sprintf(tmpbuf,"(%d,%d,%d)",bR,bG,bB);
    lableRGBVal->setText(tmpbuf);

    sprintf(tmpbuf,"#%02X%02X%02X",bR,bG,bB);
    lableHexVal->setText(tmpbuf);

    sprintf(tmpbuf,"QLabel{color:rgb(%d,%d,%d);background-color:rgb(%d,%d,%d);}",tR,tG,tB,bR,bG,bB);
    LabelPreViewPic->setStyleSheet(tmpbuf);

    LabelPreViewPic->update();
}

void QcwColorTable::setButtonStyle(QPushButton* pbtn,const QColor& txtCol,const QColor& brgCol)
{
    int tR=txtCol.red();
    int tG=txtCol.green();
    int tB=txtCol.blue();

    int bR=brgCol.red();
    int bG=brgCol.green();
    int bB=brgCol.blue();

    char tmpbuf[256];
    sprintf(tmpbuf,"QPushButton{padding:-1;border:none;color:rgb(%d,%d,%d);background-color:rgb(%d,%d,%d);}",tR,tG,tB,bR,bG,bB);
    pbtn->setStyleSheet(tmpbuf);
}

QcwSetStyle::QcwSetStyle(void)
{
    setUI();
}

QcwSetStyle::~QcwSetStyle(void)
{

}

void QcwSetStyle::setUI()
{
    VWidget.push_back(new QWidgetLabel);
    VWidget.push_back(new QWidgetLabel);
    VWidget.push_back(new QWidgetLabel);
    VWidget.push_back(new QWidgetPushButton);

    for (int n=0;n<VWidget.size();n++)
    {
        connect(VWidget.at(n),SIGNAL(sig_StyleChanged()),this,SLOT(slot_StyleChanged()));
    }


    QGridLayout* GLayoutMain=new QGridLayout;
    unsigned int NumWidget=0;
    for (int m=0;m<16;m++)
    {
        for (int n=0;n<9;n++)
        {
            if (NumWidget==(VWidget.size()))
            {
                break;
            }
            GLayoutMain->addWidget(VWidget.at(NumWidget),m,n,1,1);
            NumWidget++;
        }
    }

    setLayout(GLayoutMain);
}

void QcwSetStyle::slot_StyleChanged()
{
    printf("QcwSetStyle::slot_StyleChanged()\n");
    QString sStyle;
    for (int n=0;n<VWidget.size();n++)
    {
          sStyle+=VWidget.at(n)->getStyleSheet();
    }

    printf("sStyleAll:%s\n",sStyle.toStdString().c_str());
}



void QWidgetSetstyle::setUI()
{
    QCheckBox* CheckBoxEnableColor=new QCheckBox;
    QHBoxLayout* HLayoutL1=new QHBoxLayout;
    HLayoutL1->addWidget(CheckBoxEnableColor);

    QVBoxLayout* HLayoutMain=new QVBoxLayout;
    HLayoutMain->addLayout(HLayoutL1);
    setLayout(HLayoutMain);
}


void QWidgetLabel::setUI()
{
    QGroupBox* GroupBoxLabel=new QGroupBox(tr("QLabel"));
    QVBoxLayout* HLayoutGroup=new QVBoxLayout;
    GroupBoxLabel->setLayout(HLayoutGroup);

    QLabel* Label=new QLabel("test string");
    QPushButton* buttonSetstyle=new QPushButton("Set");
    connect(buttonSetstyle,SIGNAL(clicked()),this,SLOT( slot_showDialog()));


    QHBoxLayout* HLayoutL1=new QHBoxLayout;
    HLayoutL1->addWidget(Label);
    HLayoutL1->addWidget(buttonSetstyle);

    HLayoutGroup->addLayout(HLayoutL1);

    QGridLayout* GLayoutMain=new QGridLayout;
    GLayoutMain->addWidget(GroupBoxLabel,0,0,1,1);
    setLayout(GLayoutMain);
}



void QWidgetAsjustStyle::setUI()
{
    QGroupBox* GroupSubAttribute=new QGroupBox(tr("SubAttribute"));
    QVBoxLayout* VLayoutSubAttribute=new QVBoxLayout;
    GroupSubAttribute->setLayout(VLayoutSubAttribute);
    GroupSubAttribute->setCheckable(true);

    QCheckBox* checkBoxNormal=new QCheckBox(tr("normal"));
    QCheckBox* checkBoxHover=new QCheckBox(tr("hover"));
    QCheckBox* checkBoxPress=new QCheckBox(tr("press"));
    VLayoutSubAttribute->addWidget(checkBoxNormal);
    VLayoutSubAttribute->addWidget(checkBoxHover);
    VLayoutSubAttribute->addWidget(checkBoxPress);

    checkBoxNormal->setEnabled(false);

    QGroupBox* GroupAttribute=new QGroupBox(tr("Attribute"));
    QVBoxLayout* VLayoutAttribute=new QVBoxLayout;

    QGroupBox* GroupBoxFront=new QGroupBox(tr("Front"));
    QHBoxLayout* HLayoutGroupBoxFront=new QHBoxLayout;
    GroupBoxFront->setLayout(HLayoutGroupBoxFront);
    GroupBoxFront->setCheckable(true);
    GroupBoxFront->setChecked(true);
    GroupBoxFront->setEnabled(false);
    QPushButton* pButtonFront=new QPushButton(tr("Select Front"));
    HLayoutGroupBoxFront->addWidget(pButtonFront);

    QGroupBox* GroupBoxFixedWidth=new QGroupBox(tr("FixedWidth"));
    QHBoxLayout* HLayoutGroupBoxFixedWidth=new QHBoxLayout;
    GroupBoxFixedWidth->setLayout(HLayoutGroupBoxFront);
    GroupBoxFixedWidth->setCheckable(true);
    GroupBoxFixedWidth->setChecked(true);
    GroupBoxFixedWidth->setEnabled(false);
    //QSpinBox* SpinBoxFixedWidth=new QSpinBox(tr("FixedWidth"));
    //HLayoutGroupBoxFixedWidth->addWidget(SpinBoxFixedWidth);


   //FixedWidth FixedthHeight
    //borderwidth borderRadius bordershape in/out
    //border-color

    //coror /backgroung-color
    //


    QVBoxLayout* VLayoutRight=new QVBoxLayout;
    VLayoutRight->addWidget(GroupBoxFront);


    QGridLayout *GLayoutMain = new QGridLayout;
    for(int i=0;i<9;i++)
    {
        GLayoutMain->setRowStretch(i,1);
    }
    for(int i=0;i<16;i++)
    {
        GLayoutMain->setColumnStretch(i,1);
    }
    GLayoutMain->addWidget(GroupSubAttribute,0,0,5,2);
    GLayoutMain->addLayout(VLayoutRight,0,2,5,7);

    setLayout(GLayoutMain);
}


WidgetRegister* GetWidgetRegister()
{
    static WidgetRegister gWidgetRegister;
    return &gWidgetRegister;
}

struct wtypeStrPair
{
    int type;
    char* str;
};

enum WidgetType_t
{
    Ew_AllType=0,
    Ew_QWidget,
    Ew_QDialog,
    Ew_QFrame,
    Ew_QLabel,
    Ew_QLineEdit,
    Ew_QTextEdit,
    Ew_QPushButton,
    Ew_QToolButton,     //简单的按钮控件
    Ew_QToolBox,          //成列的QToolButton控件
    Ew_QRadioButton,
    Ew_QButtonGroup,
    Ew_QComboBox,
    Ew_QCheckBox,
    Ew_QGroupbox,
    Ew_QSlider,
    Ew_QSpinBox,
    Ew_QDoubleSpinBox,
    Ew_QTextBrower,
    Ew_QProgressBar,//水平和垂直进度条控件

    //QCommandLinkButton： //windows Vista风格的命令连接按钮
    //QTimeEdit： //编辑或显示时间的控件
    //QDateEdit： //编辑或显示日期的控件
    //QDateTimeEdit： //编辑或者显示时间和日期的控件
    //QDial： //用于描述仪表盘样式
    //QLCDNumber： 显示LCD风格数字的控件
    //QMenu： 可以在菜单栏的菜单控件、Context和弹出菜单中使用的控件
    //QScrollArea： 可以扩展查看其它控件的滚动条控件
    //QScrollBar： 水平和垂直滚动条控件
    //QTabBar： TabBar控件(在标签对话框中使用)
    Ew_Total
};


class QListView;
class QListWidget;
class QTableView;
class QTableWidget;
class QTabWidget;


const char* getEnumKeyStr(WidgetType_t n)
{
    switch (n)
    {
    case Ew_AllType: return "AllType";break;
    case Ew_QWidget: return "QWidget";break;
    case Ew_QDialog:return "QDialog";break;
    case Ew_QFrame:return "QFrame";break;
    case Ew_QLabel: return "QLabel";break;
    case Ew_QLineEdit: return "QLineEdit";break;
    case Ew_QTextEdit:return "QTextEdit";break;
    case Ew_QPushButton:return "QPushButton";break;
    case Ew_QToolButton:return "QToolButton";break;
    case Ew_QToolBox:return "QToolBox";break;
    case Ew_QButtonGroup:return "QButtonGroup";break;
    case Ew_QComboBox:return "QComboBox";break;
    case Ew_QRadioButton:return "QRadioButton";break;
    case Ew_QCheckBox:return "QCheckBox";break;
    case Ew_QGroupbox:return "QGroupbox";break;
    case Ew_QSlider:return "QSlider";break;
    case Ew_QSpinBox:return "QSpinBox";break;
    case Ew_QDoubleSpinBox:return "DoubleSpinBox";break;
    case Ew_QTextBrower:return "QTextBrower";break;
    default:
        return "";
        break;
    }
}

const char* getWidgetTypeStr(int n)
{
    return getEnumKeyStr((WidgetType_t)n);
}

#include <QListView>
#include <QListWidget>
#include <QStackedWidget>

void DialogSetStyleMain::setUI()
{
    this->setWindowTitle(tr("Real Edit QSS"));
    QLabel* labelTypefilter=new QLabel(tr("Type Filter"));
    ComboBoxTypeFilter=new QComboBox;
    ComboBoxTypeFilter->setFixedWidth(150);
    for (int n=0;n<Ew_Total;n++)
    {
        ComboBoxTypeFilter->insertItem(n,QString(getWidgetTypeStr(n)));
    }
    QHBoxLayout* HLayouttype=new QHBoxLayout;
    HLayouttype->addWidget(labelTypefilter);
    HLayouttype->addWidget(ComboBoxTypeFilter);
    QListWidget* listWidgets=new QListWidget;
    QVBoxLayout* VLayoutH1=new QVBoxLayout;
    VLayoutH1->addLayout(HLayouttype);
    VLayoutH1->addWidget(listWidgets);

    pStackWidget=new QStackedWidget;



    QGroupBox* groupBoxCtrl=new QGroupBox(tr("Ctrl"));


    QGridLayout *GLayoutMain = new QGridLayout;
    for(int i=0;i<9;i++)
    {
        GLayoutMain->setRowStretch(i,1);
    }
    for(int i=0;i<16;i++)
    {
        GLayoutMain->setColumnStretch(i,1);
    }
    GLayoutMain->addLayout(VLayoutH1,0,0,6,1);
    GLayoutMain->addWidget(pStackWidget,1,0,7,15);
    GLayoutMain->addWidget(groupBoxCtrl,7,0,2,16);

    //GLayoutMain->addLayout(VLayoutRight,0,2,5,7);
    setLayout(GLayoutMain);
}

void DialogSetStyleMain::initMember()
{

}

