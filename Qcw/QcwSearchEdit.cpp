#include "QcwSearchEdit.h"

#include <QHBoxLayout>
#include <QPushButton>


QcwSearchEdit::QcwSearchEdit(QWidget *parent)
    : QLineEdit(parent)
{
    //ui.setupUi(this);
    setUI();
}

QcwSearchEdit::QcwSearchEdit(const QString& str, QWidget* parent/*=0*/)
    : QLineEdit(str,parent)
{
    setUI();
}

QcwSearchEdit::~QcwSearchEdit()
{

}

void QcwSearchEdit::setUI()
{
    //m_pSearchLineEdit = new QLineEdit();

    pSearchButton = new QPushButton(this);

    pSearchButton->setCursor(Qt::PointingHandCursor);   //������� ��״
    pSearchButton->setFixedSize(22,22);
    pSearchButton->setToolTip(tr("Search"));   // ����
    pSearchButton->setStyleSheet("QPushButton{\
                                 border-image:url(:/images/icon_search_normal); background:transparent;}\
                                 QPushButton:hover{border-image:url(:/images/icon_search_hover)} \
                                 QPushButton:pressed{border-image:url(:/images/icon_search_press)}"
                                 );


    //��ֹ�ı�����������λ�ڰ�ť֮��
    //QMargins margins = m_pSearchLineEdit->textMargins();
    //m_pSearchLineEdit->setTextMargins(margins.left(), margins.top(), pSearchButton->width(), margins.bottom());
    //m_pSearchLineEdit->setPlaceholderText(QStringLiteral("��������������"));

    QMargins margins = this->textMargins();
    this->setTextMargins(margins.left(), margins.top(),pSearchButton->width(), margins.bottom());
    this->setPlaceholderText(tr("Enter keyword to search"));   //QStringLiteral("��������������")

    //����������һ��������������ݵİ�ť���пմ���   TODO


    QHBoxLayout *pSearchLayout = new QHBoxLayout();
    pSearchLayout->addStretch();
    pSearchLayout->addWidget(pSearchButton);
    pSearchLayout->setSpacing(2);
    pSearchLayout->setContentsMargins(2,2,2,2);
    //m_pSearchLineEdit->setLayout(pSearchLayout);

    this->setLayout(pSearchLayout);

    connect(pSearchButton, SIGNAL(clicked(bool)), this, SLOT(slot_SearchClicked()));

    connect(this,SIGNAL(editingFinished()),this,SLOT(slot_editingFinished()));
    connect(this,SIGNAL(returnPressed()),this,SLOT(slot_returnPressed()));
    connect(this,SIGNAL(textChanged(const QString &)),this,SLOT(slot_textChanged()));


    //���ü����źŴ�����ʽ��Ĭ��״̬
    setTriggerEnabledOfTextChanged(false);
    setTriggerEnabledOfEditingFinished(true);
    setTriggerEnabledOfReturnPressed(true);
}

void QcwSearchEdit::resizeEvent(QResizeEvent* ev)
{
        int searchIconwith=this->height()-3;
        pSearchButton->setFixedSize(searchIconwith,searchIconwith);
        QMargins margins = this->textMargins();
        this->setTextMargins(margins.left(), margins.top(), pSearchButton->width(), margins.bottom());
}


void QcwSearchEdit::slot_SearchClicked()
{
    //QString strText = m_pSearchLineEdit->text();
    QString strText =this->text();
    //if (!strText.isEmpty())    //�ؼ���Ϊ��Ҳ�����ź�,һЩ��տ����������������ϴ��������,
    {
        //QMessageBox::information(this, QStringLiteral("����"), QStringLiteral("��������Ϊ%1").arg(strText));
        emit sig_Search(strText);
    }
}

void QcwSearchEdit::slot_editingFinished()
{
    if (bTriggerEnabledOfEditingFinished)
    {
        slot_SearchClicked();
    }
}

void QcwSearchEdit::slot_returnPressed()
{
    if (bTriggerEnabledOfReturnPressed)
    {
        slot_SearchClicked();
    }
}

void QcwSearchEdit::slot_textChanged()
{
    if (bTriggerEnabledOfTextChanged)
    {
        slot_SearchClicked();
    }
}
