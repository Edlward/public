#ifndef QcwPassWordEdit_H
#define QcwPassWordEdit_H

//QLineEdit实现可显示密码可隐藏密码--未完待续


#include <QLineEdit>

class  QPushButton;
class QcwPassWordEdit:public QLineEdit
{
    Q_OBJECT
public:
    explicit QcwPassWordEdit(QWidget *parent = 0);
    explicit QcwPassWordEdit(const QString &, QWidget* parent=0);
    ~QcwPassWordEdit();
    void setUI();
protected:
    virtual void resizeEvent(QResizeEvent* ev);
private:
    QPushButton *pBtnEye;
public:

private Q_SLOTS:
     void slot_BtnEyeClicked(bool flag);


//几种常见信号的处理
//void editingFinished()	   //当编辑完成点回车，发射信号。
//void returnPressed()       //光标在行编辑框内点回车，发射信号。
//void selectionChanged()    //选择的文本发生变化时，发射信号。
//void textChanged(const QString & text)  //文本内容改变时，发射信号。
//void textEdited(const QString & text)     //当文本被编辑时，发射该信号。使用setText()也会发射。
};

#endif // QCWSEARCHEDIT_H
