#ifndef QcwPassWordEdit_H
#define QcwPassWordEdit_H

//QLineEditʵ�ֿ���ʾ�������������--δ�����


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


//���ֳ����źŵĴ���
//void editingFinished()	   //���༭��ɵ�س��������źš�
//void returnPressed()       //������б༭���ڵ�س��������źš�
//void selectionChanged()    //ѡ����ı������仯ʱ�������źš�
//void textChanged(const QString & text)  //�ı����ݸı�ʱ�������źš�
//void textEdited(const QString & text)     //���ı����༭ʱ��������źš�ʹ��setText()Ҳ�ᷢ�䡣
};

#endif // QCWSEARCHEDIT_H
