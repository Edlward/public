#ifndef _QcwMessageBox_H
#define _QcwMessageBox_H

//�����Զ��˳�����Ϣ��ʾ��

//��װ����Ϣ������ʾ�򣬿��Զ�ʱ�Զ��ر�

#include <QDialog>


class QPushButton;
class QTimer;
class QcwMessageBox: public QDialog
{
    Q_OBJECT
public:
    //timeCloseDelay<0 �����Զ��ر�
    explicit QcwMessageBox(QString sInfo,int timeCloseDelay=3000,QWidget *parent =0);
    ~QcwMessageBox();
    // virtual void closeEvent(QCloseEvent *event);
signals:
    // void sig_closeDialog();       //���ڹر�ǰ�������ź�
public: 
    //QPushButton* pButtonUnMute;
private:
    QTimer* pTimerToDO;
private slots:
    void slot_TimeToDoSomeThing();
};

//ʹ�÷���
//MyMessageBox DlgMsg("My MessageBox  Auto Close");
//DlgMsg.exec();


#endif

