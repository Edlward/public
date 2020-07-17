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
    void sig_Search(QString str);     //点击搜索按钮后发起的信号

private Q_SLOTS:
     void slot_SearchClicked();


//几种常见信号的处理
//void editingFinished()	   //当编辑完成点回车，发射信号。
//void returnPressed()       //光标在行编辑框内点回车，发射信号。
//void selectionChanged()    //选择的文本发生变化时，发射信号。
//void textChanged(const QString & text)  //文本内容改变时，发射信号。
//void textEdited(const QString & text)     //当文本被编辑时，发射该信号。使用setText()也会发射。

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
