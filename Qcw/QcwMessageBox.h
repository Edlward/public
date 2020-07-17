#ifndef _QcwMessageBox_H
#define _QcwMessageBox_H

//可以自动退出的信息提示框

//封装的消息警告提示框，可以定时自动关闭

#include <QDialog>


class QPushButton;
class QTimer;
class QcwMessageBox: public QDialog
{
    Q_OBJECT
public:
    //timeCloseDelay<0 不会自动关闭
    explicit QcwMessageBox(QString sInfo,int timeCloseDelay=3000,QWidget *parent =0);
    ~QcwMessageBox();
    // virtual void closeEvent(QCloseEvent *event);
signals:
    // void sig_closeDialog();       //窗口关闭前发出此信号
public: 
    //QPushButton* pButtonUnMute;
private:
    QTimer* pTimerToDO;
private slots:
    void slot_TimeToDoSomeThing();
};

//使用方法
//MyMessageBox DlgMsg("My MessageBox  Auto Close");
//DlgMsg.exec();


#endif

