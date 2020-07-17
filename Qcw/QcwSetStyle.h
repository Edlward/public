#ifndef  QCWSETSTYLE_H_
#define  QCWSETSTYLE_H_


#include <QtWidgets/QDialog>
#include <vector>

//��ɫ�� rgb����
//����ɫ,����ɫ��

class QLabel;
class QPushButton;
class QSpinBox;
class QcwGetColor:public QWidget
{
    Q_OBJECT
public:
    QcwGetColor()
    {
        setUI();
    }
    ~QcwGetColor(){}
    void setUI();
    QColor getColor()const {return m_Color;}

public slots:
        void slot_getColorBystd();
        void slot_getColorBycostom();
private:
    QColor m_Color;
};


//Ч���е��,��̫��,Ӧ����QLabel�Ի�ʵ��,�пմ���
//���״δ�Ĭ����ɫ
//��ȡ����,����

class QcwColorTable:public QDialog
{
    Q_OBJECT
public:
    QcwColorTable(const QColor& initCol=Qt::white,QWidget* parent=0)
        :QDialog(parent)
    {
        m_Color=initCol;
        m_TextColor=Qt::black;
        setUI();
    }
    ~QcwColorTable(){}
    void setUI();
    QColor getColor()const {return getBackgroundColor();}        //Ĭ�Ϸ��ر�����ɫ
    QColor getTextColor()const {return m_TextColor;}                //������ɫ
    QColor getBackgroundColor()const {return m_Color;}           //������ɫ
    QFont getFont()const {return m_Font;}  
signals:
    void sig_ColorChanged(QColor col);
    void sig_TextColorChanged(QColor col);
    void sig_FontChanged(QFont font);
public slots:
    void slot_color_button_clicked();
    void slot_ok();
    void slot_cancel();
    void slot_spinbox_changed();
    void slot_getTextColor();
    void slot_getFont();
    void slot_ReverseTxtBackColor();   //��ת�����뱳��ɫ

    //�����ַ��������а�
    void slot_CopyTextHexVal();   
    void slot_CopyHexVal();         
private:
      void do_update_display();
      static void setButtonStyle(QPushButton* pbtn,const QColor& txtCol,const QColor& brgCol);
private:
    QFont m_Font;
    QColor m_Color;
    QLabel* lableRGBVal;
    QLabel* lableHexVal;
    QSpinBox* spinBoxR;
    QSpinBox* spinBoxG;
    QSpinBox* spinBoxB;
    //QSpinBox* spinBoxA;
    QColor m_TextColor;    //�����ı���ɫ
    QLabel* lableTextRGBVal;
    QLabel* lableTextHexVal;
    QSpinBox* spinBoxTextR;
    QSpinBox* spinBoxTextG;
    QSpinBox* spinBoxTextB;

    QLabel* LabelPreViewPic;
    std::vector<QPushButton*> ListColorBtn;
};



class QWidgetBase;
class QcwSetStyle:public QDialog
{
    Q_OBJECT
public:
    explicit QcwSetStyle(void);
    ~QcwSetStyle(void);
    void setUI();
public slots:
    void slot_StyleChanged();

private:
    std::vector<QWidgetBase*> VWidget;
};


class QWidgetSetstyle:public QDialog
{
    Q_OBJECT
public:
    QWidgetSetstyle()
    {
        setUI();
    }
    ~QWidgetSetstyle(){}
    void setUI();
    QString getStyleSheet(){return "";}
};


class QWidgetBase:public QWidget
{
    Q_OBJECT
public:
    QWidgetBase()
    {
        dlg.hide();
    }
    ~QWidgetBase()
    {

    }
    QString getStyleSheet(){return dlg.getStyleSheet();}
signals:
    void sig_StyleChanged();
private:
    QWidgetSetstyle dlg;
public slots:
   void slot_showDialog()
   {
       //QWidgetSetstyle dlg;
       dlg.show();
       dlg.exec();
   }
};


class QWidgetLabel:public QWidgetBase
{
    Q_OBJECT
public:
    QWidgetLabel()
    {
        setUI();
    }
    ~QWidgetLabel()
    {
    }
    void setUI();
};

class QWidgetPushButton:public QWidgetBase
{
    Q_OBJECT
public:
    QWidgetPushButton()
    {
        setUI();
    }
    ~QWidgetPushButton()
    {
    }
    void setUI()
    {
    }
};


//struct AttributeEnableState
//{
//    bool bEn_Normal;
//    bool bEn_Hover;
//    bool bEn_Press;
//};



class QWidgetAsjustStyle:public QDialog
{
    Q_OBJECT
public:
    QWidgetAsjustStyle()
    {
        setUI();
    }
    ~QWidgetAsjustStyle()
    {
    }
    void setUI();
};


//

#include <map>
#include <set>

class WidgetRegister
{
    //Q_OBJECT
public:
    WidgetRegister()
    {
        
    }
    ~WidgetRegister()
    {
    }

    ////�������÷�,�Զ�����ע��,�����Զ��崰�壬��Ҫ��������ϵͳ�ȵ�֧�֣��пմ���
    //template<class T>
    //void ResgesterWidget(T* p)
    //{
    //     const char* tname=typeid(T).name();
    //     int key=
    //}

    void ResgesterWidget(QWidget* p)
    {
        setWidget.insert(p);
    }
    void UnResgesterWidget(QWidget* p)
    {
        setWidget.erase(p);
    }
private:
    std::set<QWidget*> setWidget;
};
WidgetRegister* GetWidgetRegister();



class QComboBox;
class QStackedWidget;
class DialogSetStyleMain:public QDialog
{
    Q_OBJECT
public:
    DialogSetStyleMain()
    {
        setUI();
    }
    ~DialogSetStyleMain()
    {
    }
    void setUI();
    void initMember();
private:
    QComboBox* ComboBoxTypeFilter;
    QStackedWidget* pStackWidget;
};



#endif

