#include "QcwPassWordEdit.h"

#include <QHBoxLayout>
#include <QPushButton>


QcwPassWordEdit::QcwPassWordEdit(QWidget *parent)
    : QLineEdit(parent)
{
    //ui.setupUi(this);
    setUI();
}

QcwPassWordEdit::QcwPassWordEdit(const QString& str, QWidget* parent/*=0*/)
    : QLineEdit(str,parent)
{
    setUI();
}

QcwPassWordEdit::~QcwPassWordEdit()
{

}

void QcwPassWordEdit::setUI()
{
    //m_pSearchLineEdit = new QLineEdit();

    pBtnEye = new QPushButton(this);

    pBtnEye->setCursor(Qt::PointingHandCursor);   //������� ��״
    pBtnEye->setFixedSize(22,22);
    pBtnEye->setToolTip(tr("Search"));   // ����


    //pBtnEye->setStyleSheet("QPushButton{\
    //                             border-image:url(:/images/icon_search_normal); background:transparent;}\
    //                             QPushButton:hover{border-image:url(:/images/icon_search_hover)} \
    //                             QPushButton:pressed{border-image:url(:/images/icon_search_press)}"
    //                             );


    //QPixmap::loadFromData  ��ͼ����Դ��������  ���ڴ����ͼ������
    //��������۲�ͬͼ�꣬���ƴ���,
    pBtnEye->setStyleSheet("QPushButton{border-image:url(:/images/password_hide.png);}\
                           QPushButton:ckecked{border-image:url(:/images/password_show.png)}"
                           );


    pBtnEye->setCursor(Qt::PointingHandCursor);
    pBtnEye->setCheckable(true);
    connect(pBtnEye,&QPushButton::clicked, [this](bool checked) {
        if (checked)
        {
            setEchoMode(QLineEdit::Normal);
        }
        else
        {
            setEchoMode(QLineEdit::Password);
        }
    });

    QMargins margins = this->textMargins();
    this->setTextMargins(margins.left(), margins.top(),pBtnEye->width(), margins.bottom());
    //this->setPlaceholderText(tr("Enter keyword to search"));   //QStringLiteral("��������������")

    //����������һ��������������ݵİ�ť���пմ���   TODO


    QHBoxLayout *pSearchLayout = new QHBoxLayout();
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(pBtnEye);
    pSearchLayout->setSpacing(2);
    pSearchLayout->setContentsMargins(2,2,2,2);
    //m_pSearchLineEdit->setLayout(pSearchLayout);

    this->setLayout(pSearchLayout);

    connect(pBtnEye, SIGNAL(clicked(bool)), this, SLOT(slot_BtnEyeClicked(bool)));
}

void QcwPassWordEdit::resizeEvent(QResizeEvent* ev)
{
        int searchIconwith=this->height()-3;
        pBtnEye->setFixedSize(searchIconwith,searchIconwith);
        QMargins margins = this->textMargins();
        this->setTextMargins(margins.left(), margins.top(), pBtnEye->width(), margins.bottom());
}


void QcwPassWordEdit::slot_BtnEyeClicked(bool flag)
{
   
}
