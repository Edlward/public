#ifndef QCWSEARCHEDIT_H
#define QCWSEARCHEDIT_H


#include <QLineEdit>
//#include "ui_qcwsearchedit.h"

class  QPushButton;
class QcwSearchEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit QcwSearchEdit(QWidget *parent = 0);
    explicit QcwSearchEdit(const QString &, QWidget* parent=0);
    ~QcwSearchEdit();
    void setUI();
protected:
    virtual void resizeEvent(QResizeEvent* ev);
private:
    //Ui::QcwSearchEdit ui;
    QPushButton *pSearchButton;
public:

signals:
    void sig_Search(QString str);     //���������ť������ź�

private Q_SLOTS:
     void slot_SearchClicked();


//���ֳ����źŵĴ���
//void editingFinished()	   //���༭��ɵ�س��������źš�
//void returnPressed()       //������б༭���ڵ�س��������źš�
//void selectionChanged()    //ѡ����ı������仯ʱ�������źš�
//void textChanged(const QString & text)  //�ı����ݸı�ʱ�������źš�
//void textEdited(const QString & text)     //���ı����༭ʱ��������źš�ʹ��setText()Ҳ�ᷢ�䡣

     void slot_editingFinished();
     void slot_returnPressed();
     void slot_textChanged();
public:
    void setTriggerEnabledOfTextChanged(bool en){bTriggerEnabledOfTextChanged=en;}
    bool getTriggerEnabledOfTextChanged()const {return bTriggerEnabledOfTextChanged;}
private:
    bool bTriggerEnabledOfTextChanged;

public:
    void setTriggerEnabledOfEditingFinished(bool en){bTriggerEnabledOfEditingFinished=en;}
    bool getTriggerEnabledOfEditingFinished()const {return bTriggerEnabledOfEditingFinished;}
private:
    bool bTriggerEnabledOfEditingFinished;

public:
    void setTriggerEnabledOfReturnPressed(bool en){bTriggerEnabledOfReturnPressed=en;}
    bool getTriggerEnabledORreturnPressed()const {return bTriggerEnabledOfReturnPressed;}
private:
    bool bTriggerEnabledOfReturnPressed;
};

#endif // QCWSEARCHEDIT_H
