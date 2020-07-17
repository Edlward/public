#include "QtStyleCtrl.h"


//常用风格1：蓝色风格
const char* sStyleBlue=
"QWidget,QDialog{background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(20, 33, 76, 255), stop:0.272727 rgba(38, 77, 121, 255), stop:0.511364 rgba(46, 94, 141, 255), stop:0.767045 rgba(33, 75, 116, 255), stop:1 rgba(19, 42, 78, 255));\
}"

"QGroupBox{\
background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(32, 59, 119, 255), stop:0.272727 rgba(49, 99, 148, 255), stop:0.511364 rgba(49, 99, 148, 255), stop:0.761364 rgba(49, 99, 148, 255), stop:1 rgba(29, 64, 118, 255));\
color: white;\
font:75 18pt Book Antiqua;\
}"


#if 0
QGroupBox {
    border-width:1px;   //线的粗细
    border-style:solid;
    border-color:lightGray;   //颜色，
    margin-top: 0.5ex;  //文字在方框中位置的偏离度
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
left:25px;       //线的偏离度
    margin-left: 0px;
padding:0 1px;   //文字在方框中位置的偏离度
}
#endif
//background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(30, 47, 53, 255), stop:0.19774 rgba(54, 73, 91, 255), stop:0.474576 rgba(71, 92, 103, 255), stop:0.740113 rgba(77, 117, 153, 255), stop:1 rgba(73, 125, 137, 255));

"QPushButton{\
color:#000000;\
background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 255, 255), stop:0.0340909 rgba(30, 130, 153, 255), stop:0.176136 rgba(30, 130, 153, 255), stop:0.465909 rgba(255, 255, 255, 255), stop:0.568182 rgba(255, 255, 255, 255), stop:0.886364 rgba(30, 130, 153, 255), stop:0.965909 rgba(30, 130, 153, 255), stop:1 rgba(255, 255, 255, 255));\
font:75 14pt Book Antiqua;\
text-align: left;\
border-width: 2px;\
border-color:#000000;\
border-radius: 5px;\
border-style: outset;\
padding:3px;\
margin-top:0px;\
margin-bottom:0px;\
margin-left:0px;\
margin-right:0px;\
}"
"QPushButton:hover{ background-color: #555566; color: black;\
background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0.107955 rgba(95, 230, 85, 255), stop:0.357955 rgba(255, 255, 255, 255), stop:0.676136 rgba(255, 255, 255, 255), stop:0.971591 rgba(39, 252, 55, 255));\
}"

"QPushButton:pressed{ background-color: rgb(0, 0, 55); color: white;\
border-style: inset; }"

"QPushButton:disabled{ background-color: rgb(100, 100, 100); color: white;}"

"QLabel{color: white;\
background-color:transparent;\
font: 50 14pt Book Antiqua;\
}"

"QCheckBox{\
color: white;\
background-color:transparent;\
font: 50 14pt Book Antiqua;\
}"

"QSpinBox,QDoubleSpinBox,QComboBox,QLineEdit{\
color: black;\
background-color:#FFFFFF;\
font: 50 12pt Book Antiqua;\
}"



#if 1
//QComboBox 以及下拉列表文字颜色
"QComboBox{\
border: 1px solid #636363;\
height: 30px;\
border-radius: 3px;\
padding: 2px;\
background-color: white; color:black;\
font: 75 12pt 黑体\
}"

"QComboBox QAbstractItemView{background:#FFFFFF;color:#000000;}"
"QComboBox QAbstractItemView::item {font: 75 12pt 黑体;}"
//"QComboBox QAbstractItemView::item {min-height: 20px;max-height: 100px;}"
"QComboBox QAbstractItemView::item {height: 30px;}"
"QComboBox QAbstractItemView::item::selected{background:#FFFFFF;color:#0000FF;}"

"QListView{border-radius: 0px;\
selection-background-color:#428BCA;\
background-color: rgba(255,255,255,255);\
color:black;gridline-color: #DDDDDD;\
border:1px solid #DDDDDD;}"
//"QListView::item{min-height:30px;background-color: rgba(255, 255, 255,0);}"
"QListView::item{height:30px;}"
//"QListView::item:hover,QListView::item:selected{min-height:30px;background-color: #428BCA;color: white;border:0px;}"
#endif


"QDateEdit {\
border: 1px;\
border-radius: 3px;\
color: #000000;\
background-color:#FFFFFF;\
}"


#if 0

"QLineEdit {\
border: 1px solid #636363;\
border-radius: 5px;\
padding: 2px;\
background: none;\
}"


#endif
 
//spacing: 5px; height: 30px;
"QRadioButton {\
color: white;\
font: 50 14pt Book Antiqua;\
}"

#if 0
"QRadioButton::indicator { \
width: 17px;\
height: 30px;\
}"
#endif
//QCheckBox,

#if 0
"QCheckBox{background-color: rgb(200,205,200);\
font-size:16px;\
}"
"QRadioButton{background-color: rgb(200,205,200);\
font-size:16px;\
}"

"QCheckBox {\
spacing: 2px;\
}"

"QCheckBox::indicator {\
width: 20px;\
height: 20px;\
}"

"QCheckBox::indicator:unchecked {\
image: url(:/image/checkbox_unchecked.png);\
}"

"QCheckBox::indicator:checked {\
image: url(:/image/checkbox_checked.png); \
}"

//"QComboBox{background-color: white; color: black}"
//"QSpinBox{background-color: white; color: blue}"
"QCheckBox{background-color: gray; color: red}"
"QLineEdit{background-color: gray; color: red}"
"QRadioButton,QListView,{background-color: gray; color: red}"
#endif

//三、设置QTabwidget的TabBar的样式2
"QTabWidget{\
color: white;\
background:rgba(20, 20, 50, 255);\
background-color:rgba(80, 80, 80, 255);\
font: 50 14pt Book Antiqua;\
}"

"QTabWidget::pane{\
color:black;\
background: rgba(10, 10, 150, 255);\
background-color: rgb(120, 120, 120);\
border:none;\
border-width:1px;\
border-color:rgb(48, 104, 151);\
border-style: outset;\
}"

"QTabWidget::tab-bar{\
alignment:left;\
}"
//background:transparent;
"QTabBar::tab{\
color:white;\
background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(30, 47, 53, 255), stop:0.19774 rgba(54, 73, 91, 255), stop:0.474576 rgba(71, 92, 103, 255), stop:0.740113 rgba(77, 117, 153, 255), stop:1 rgba(73, 125, 137, 255));\
min-width:35ex;\
min-height:12ex;\
}"

"QTabBar::tab:hover{\
color: blue;\
background:rgb(255, 255, 255, 100);\
}"

"QTabBar::tab:selected{\
color:black;\
background:white;\
border-color: white;\
}"


//QTableView
"QTableView{background-color:Gainsboro;\
color:black;\
font: 50 16pt Book Antiqua;\
}"
"QHeaderView::section {background-color:Gainsboro;\
color: DodgerBlue;\
border-style:solid;\
padding-left: 0px;\
font: 50 14pt Book Antiqua;\
border: 1px solid #6c6c6c;\
}"

//垂直QSlider样式表
//border:15px solid rgb(25, 38, 58);min-width:4px;max-width:4px; background-color: rgb(25, 38, 58);
#if 0
"QSlider{\
background:transparent;\
}"

"QSlider::add-page:vertical{\
background-color: rgb(37, 168, 198);\
width:4px;\
}"

"QSlider::sub-page:vertical{\
background-color: rgb(150, 150, 150);\
width:4px;\
}"

//
"QSlider::groove:vertical{\
background:transparent;\
width:4px;\
}"
//margin: -0 -4px; width:50px; height:10px;

//"QSlider::handle:vertical{\
//background-color: rgb(5,255,5); color: red;\
//border-width: 2px;\
//border-color:#AAAAAA;\
//border-radius: 6px;\
//border-style: outset;\
//width:28px;\
//height:10px;\
//margin: -0 -8px;\
//}"
/*滑块手柄*/

"QSlider::handle:Horizontal{background-color: rgb(5,255,5); color: red;\
height: 30px; \
width:8px;\
margin: -8 0px; \
}"// height:10px; width: 10px
#endif


"QTextBrowser{background-color: rgb(200,205,200);\
font-size:16px;\
}"

//中英文字体类型有区别，字体大小与类型分贝设置
"QLabel,QPushButton,QLineEdit,QTextEdit,QTextBrowser,QComboBox,QCheckBox,QRadioButton,\
QGroupBox,QWidget,QMainWindow,QDialog,QSpinBox,\
QMenuBar,QAction,QToolButton{\
font-family:Microsoft YaHei;\
}"

//font-size:16pt;\
//font-style:normal;\
//font-weight:bolder;\
//text-align: center;\
//剩余不常用控件，用逗号隔开，使用相同风格
;




//常用风格2：灰黑色风格
const char*  sStyleGrayBlack=
    "QWidget,QDialog{\
    background-color:rgba(100, 100, 100, 255);\
    }"

    "QGroupBox{\
    background-color:rgba(76, 76, 76, 255);\
    font:75 12pt Book Antiqua;\
    }"

    "QPushButton{\
    color:#FFFFFF;\
    background-color:rgba(57, 57, 57, 255);\
    font:75 12pt Book Antiqua;\
    text-align: center;\
    border-width: 2px;\
    border-color:#000000;\
    border-radius: 5px;\
    border-style: outset;\
    padding:3px;\
    margin-top:0px;\
    margin-bottom:0px;\
    margin-left:0px;\
    margin-right:0px;\
    }"

    "QPushButton:hover{ background-color: #555566; color: white;}"
    "QPushButton:pressed{ background-color: rgb(0, 0, 55); color: white;\
    border-style: inset; }"

    "QLabel{color: white;\
    background-color:transparent;\
    font: 50 12pt Book Antiqua;\
    }"

    "QCheckBox{\
    color: white;\
    background-color:transparent;\
    font: 50 14pt Book Antiqua;\
    }"

    "QSpinBox,QDoubleSpinBox,QComboBox,QLineEdit,QDateEdit{\
    color: black;\
    background-color:#FFFFFF;\
    font: 50 14pt Book Antiqua;\
    }"

      //QComboBox下拉列表文字颜色
     "QComboBox QAbstractItemView{background:#FFFFFF;color:#000000;}"
     "QComboBox QAbstractItemView::item::selected{background:#FFFFFF;color:#0000FF;}"

#if 0

    "QLineEdit {\
    border: 1px solid #636363;\
    border-radius: 5px;\
    padding: 2px;\
    background: none;\
    }"

    "QComboBox{\
    border: 1px solid #636363;\
    border-radius: 5px;\
    padding: 2px;\
    background-color: white; color:black;\
    font: 75 16pt 黑体\
    }"
#endif

    //spacing: 5px; height: 30px;
    "QRadioButton {\
    background-color:transparent;\
    color: white;\
    font: 50 14pt Book Antiqua;\
    }"

#if 0
    "QRadioButton::indicator { \
    width: 17px;\
    height: 30px;\
    }"
#endif
    //QCheckBox,

#if 0
    "QCheckBox{background-color: rgb(200,205,200);\
    font-size:16px;\
    }"
    "QRadioButton{background-color: rgb(200,205,200);\
    font-size:16px;\
    }"

    "QCheckBox {\
    spacing: 2px;\
    }"

    "QCheckBox::indicator {\
    width: 20px;\
    height: 20px;\
    }"

    "QCheckBox::indicator:unchecked {\
    image: url(:/image/checkbox_unchecked.png);\
    }"

    "QCheckBox::indicator:checked {\
    image: url(:/image/checkbox_checked.png); \
    }"

    "QComboBox{background-color: white; color: black}"
    //"QSpinBox{background-color: white; color: blue}"
    "QCheckBox{background-color: gray; color: red}"
    "QLineEdit{background-color: gray; color: red}"
    "QRadioButton,QListView,{background-color: gray; color: red}"
#endif

    //三、设置QTabwidget的TabBar的样式2
    "QTabWidget{\
    color: white;\
    background:rgba(20, 20, 50, 255);\
    background-color:rgba(80, 80, 80, 255);\
    font: 50 14pt Book Antiqua;\
    }"

    "QTabWidget::pane{\
    color:black;\
    background: rgba(10, 10, 150, 255);\
    background-color: rgb(120, 120, 120);\
    border:none;\
    border-width:1px;\
    border-color:rgb(48, 104, 151);\
    border-style: outset;\
    }"

    "QTabWidget::tab-bar{\
    alignment:left;\
    }"
    //background:transparent;
    "QTabBar::tab{\
    color:white;\
    background:rgba(80, 80, 80, 255);\
    min-width:40ex;\
    min-height:12ex;\
    }"

    "QTabBar::tab:hover{\
    color: blue;\
    background:rgb(255, 255, 255, 100);\
    }"

    "QTabBar::tab:selected{\
    color:black;\
    background:white;\
    border-color: white;\
    }"

    //设置 TabWidget 四
    //"QTabWidget::pane{
    //border:none;
    //					}"
    //					QTabWidget::tab-bar{
    //alignment:left;
    //					}
    //					QTabBar::tab{
    //background:transparent;
    //color:white;
    //						min-width:30ex;
    //						min-height:10ex;
    //					}
    //					QTabBar::tab:hover{
    //background:rgb(255, 255, 255, 100);
    //					}
    //					QTabBar::tab:selected{
    //						border-color: white;
    //background:white;
    //color:green;
    //					}

    "QTreeWidget{\
    background-color: #EEEEEE;\
    color: black;\
    }"

    /*
    "QTreeWidget::item{\
    margin:13px;\
    background: #5B677A;\
    background-clip: margin;\
    }"
    */

    //第一列头颜色
    //"QTreeWidget::branch{\
    //background:#5B677A;\
    //}"


    "QTreeView::item:hover {\
    background: rgb(69, 160, 120);\
    }"

    "QTreeView::item:selected:active{\
    background: rgb(69, 190, 120);\
    }"

    "QTreeView::item:selected:!active {\
    background: rgb(69, 187, 120);\
    }"


    //QTableView
    "QTableView{background-color:Gainsboro;\
    color:black;\
    font: 50 16pt Book Antiqua;\
    }"
    "QHeaderView::section {background-color:Gainsboro;\
    color: DodgerBlue;\
    border-style:solid;\
    padding-left: 0px;\
    font: 50 14pt Book Antiqua;\
    border: 1px solid #6c6c6c;\
    }"

    /*
    "QTreeWidget::branch:closed:has-children:!has-siblings,\
    QTreeWidget::branch:closed:has-children:has-siblings {\
    border-image: none;\
    image: url(:/image/plus.png);\
    }"


    "QTreeWidget::branch:open:has-children:!has-siblings,\
    QTreeWidget::branch:open:has-children:has-siblings  {\
    border-image: none;\
    image: url(:/image/minus.png);\
    }"

    "QTreeWidget::branch:!has-children:has-siblings:adjoins-item{\
    border-image:none;\
    image:url(:/image/test.png);\
    }"

    "QTreeWidget::branch:!has-children:!has-siblings:adjoins-item{\
    border-image:none;\
    image:url(:/image/test.png);\
    }"
    */


    //垂直QSlider样式表
    //border:15px solid rgb(25, 38, 58);min-width:4px;max-width:4px; background-color: rgb(25, 38, 58);
    //"QSlider{\
    //background:transparent;\
    //}"

    //"QSlider::add-page:vertical{\
    //background-color: rgb(37, 168, 198);\
    //width:4px;\
    //}"

    //"QSlider::sub-page:vertical{\
    //background-color: rgb(150, 150, 150);\
    //width:4px;\
    //}"

    ////
    //"QSlider::groove:vertical{\
    //background:transparent;\
    //width:4px;\
    //}"
    //margin: -0 -4px; width:50px; height:10px;

    //"QSlider::handle:vertical{\
    //background-color: rgb(5,255,5); color: red;\
    //border-width: 2px;\
    //border-color:#AAAAAA;\
    //border-radius: 6px;\
    //border-style: outset;\
    //width:28px;\
    //height:10px;\
    //margin: -0 -8px;\
    //}"

    //"QSlider::handle:Horizontal{background-color: rgb(5,255,5); color: red;\
    //height: 30px; \
    //width:8px;\
    //margin: -8 0px; \
    //}"// height:10px; width: 10px


    "QTextEdit{background-color: rgb(255,255,255);\
    font-size:10px;\
    }"

    "QTextBrowser{background-color: rgb(200,205,200);\
    font-size:16px;\
    }"
    /*滑块手柄*/


    //中英文字体类型有区别，字体大小与类型分贝设置
    "QLabel,QPushButton,QLineEdit,QTextEdit,QTextBrowser,QComboBox,QCheckBox,QRadioButton,\
    QGroupBox,QWidget,QMainWindow,QDialog,QSpinBox,\
    QMenuBar,QAction,QToolButton{\
    font-family:Microsoft YaHei;\
    }"
    //font-size:16pt;\
    //font-style:normal;\
    //font-weight:bolder;\
    //text-align: center;\
    //剩余不常用控件，用逗号隔开，使用相同风格
    ;

 




     









//常用风格3：灰白色风格
const char*  sStyleGrayWhite=
    "QWidget,QDialog{\
    background-color:rgba(200,200,200,255);\
    }"

    //background-color:rgba(76, 76, 76, 255);

    "QGroupBox{\
    background-color:transparent;\
    font:75 12pt Book Antiqua;\
    }"

      //color:#FFFFFF;
      //background-color:rgba(57, 57, 57, 255);

    "QPushButton{\
    font:75 12pt Book Antiqua;\
    text-align: center;\
    border-width: 2px;\
    border-color:#000000;\
    border-radius: 5px;\
    border-style: outset;\
    padding:3px;\
    margin-top:0px;\
    margin-bottom:0px;\
    margin-left:0px;\
    margin-right:0px;\
    }"

    "QPushButton:hover{ background-color: #555566; color: white;}"
    "QPushButton:pressed{ background-color: rgb(0, 0, 55); color: white;\
    border-style: inset; }"

    //color: white;
    "QLabel{color: black;\
    background-color:transparent;\
    font: 50 12pt Book Antiqua;\
    }"

    "QCheckBox{\
    color: white;\
    background-color:transparent;\
    font: 50 14pt Book Antiqua;\
    }"

    "QSpinBox,QDoubleSpinBox,QComboBox,QLineEdit,QDateEdit{\
    color: black;\
    background-color:#FFFFFF;\
    font: 50 14pt Book Antiqua;\
    }"

      //QComboBox下拉列表文字颜色
     "QComboBox QAbstractItemView{background:#FFFFFF;color:#000000;}"
     "QComboBox QAbstractItemView::item::selected{background:#FFFFFF;color:#0000FF;}"

#if 0

    "QLineEdit {\
    border: 1px solid #636363;\
    border-radius: 5px;\
    padding: 2px;\
    background: none;\
    }"

    "QComboBox{\
    border: 1px solid #636363;\
    border-radius: 5px;\
    padding: 2px;\
    background-color: white; color:black;\
    font: 75 16pt 黑体\
    }"
#endif

    //spacing: 5px; height: 30px;
    "QRadioButton {\
    background-color:transparent;\
    color: white;\
    font: 50 14pt Book Antiqua;\
    }"

#if 0
    "QRadioButton::indicator { \
    width: 17px;\
    height: 30px;\
    }"
#endif
    //QCheckBox,

#if 0
    "QCheckBox{background-color: rgb(200,205,200);\
    font-size:16px;\
    }"
    "QRadioButton{background-color: rgb(200,205,200);\
    font-size:16px;\
    }"

    "QCheckBox {\
    spacing: 2px;\
    }"

    "QCheckBox::indicator {\
    width: 20px;\
    height: 20px;\
    }"

    "QCheckBox::indicator:unchecked {\
    image: url(:/image/checkbox_unchecked.png);\
    }"

    "QCheckBox::indicator:checked {\
    image: url(:/image/checkbox_checked.png); \
    }"

    "QComboBox{background-color: white; color: black}"
    //"QSpinBox{background-color: white; color: blue}"
    "QCheckBox{background-color: gray; color: red}"
    "QLineEdit{background-color: gray; color: red}"
    "QRadioButton,QListView,{background-color: gray; color: red}"
#endif

    //三、设置QTabwidget的TabBar的样式2
    "QTabWidget{\
    color: white;\
    background:rgba(20, 20, 50, 255);\
    background-color:rgba(80, 80, 80, 255);\
    font: 50 14pt Book Antiqua;\
    }"

    "QTabWidget::pane{\
    color:black;\
    background: rgba(10, 10, 150, 255);\
    background-color: rgb(120, 120, 120);\
    border:none;\
    border-width:1px;\
    border-color:rgb(48, 104, 151);\
    border-style: outset;\
    }"

    "QTabWidget::tab-bar{\
    alignment:left;\
    }"
    //background:transparent;
    "QTabBar::tab{\
    color:white;\
    background:rgba(80, 80, 80, 255);\
    min-width:40ex;\
    min-height:12ex;\
    }"

    "QTabBar::tab:hover{\
    color: blue;\
    background:rgb(255, 255, 255, 100);\
    }"

    "QTabBar::tab:selected{\
    color:black;\
    background:white;\
    border-color: white;\
    }"

    //设置 TabWidget 四
    //"QTabWidget::pane{
    //border:none;
    //					}"
    //					QTabWidget::tab-bar{
    //alignment:left;
    //					}
    //					QTabBar::tab{
    //background:transparent;
    //color:white;
    //						min-width:30ex;
    //						min-height:10ex;
    //					}
    //					QTabBar::tab:hover{
    //background:rgb(255, 255, 255, 100);
    //					}
    //					QTabBar::tab:selected{
    //						border-color: white;
    //background:white;
    //color:green;
    //					}

    "QTreeWidget{\
    background-color: #EEEEEE;\
    color: black;\
    }"

    /*
    "QTreeWidget::item{\
    margin:13px;\
    background: #5B677A;\
    background-clip: margin;\
    }"
    */

    //第一列头颜色
    //"QTreeWidget::branch{\
    //background:#5B677A;\
    //}"


    "QTreeView::item:hover {\
    background: rgb(69, 160, 120);\
    }"

    "QTreeView::item:selected:active{\
    background: rgb(69, 190, 120);\
    }"

    "QTreeView::item:selected:!active {\
    background: rgb(69, 187, 120);\
    }"


    //QTableView
    "QTableView{background-color:Gainsboro;\
    color:black;\
    font: 50 16pt Book Antiqua;\
    }"
    "QHeaderView::section {background-color:Gainsboro;\
    color: DodgerBlue;\
    border-style:solid;\
    padding-left: 0px;\
    font: 50 14pt Book Antiqua;\
    border: 1px solid #6c6c6c;\
    }"

    /*
    "QTreeWidget::branch:closed:has-children:!has-siblings,\
    QTreeWidget::branch:closed:has-children:has-siblings {\
    border-image: none;\
    image: url(:/image/plus.png);\
    }"


    "QTreeWidget::branch:open:has-children:!has-siblings,\
    QTreeWidget::branch:open:has-children:has-siblings  {\
    border-image: none;\
    image: url(:/image/minus.png);\
    }"

    "QTreeWidget::branch:!has-children:has-siblings:adjoins-item{\
    border-image:none;\
    image:url(:/image/test.png);\
    }"

    "QTreeWidget::branch:!has-children:!has-siblings:adjoins-item{\
    border-image:none;\
    image:url(:/image/test.png);\
    }"
    */


    //垂直QSlider样式表
    //border:15px solid rgb(25, 38, 58);min-width:4px;max-width:4px; background-color: rgb(25, 38, 58);
    //"QSlider{\
    //background:transparent;\
    //}"

    //"QSlider::add-page:vertical{\
    //background-color: rgb(37, 168, 198);\
    //width:4px;\
    //}"

    //"QSlider::sub-page:vertical{\
    //background-color: rgb(150, 150, 150);\
    //width:4px;\
    //}"

    ////
    //"QSlider::groove:vertical{\
    //background:transparent;\
    //width:4px;\
    //}"
    //margin: -0 -4px; width:50px; height:10px;

    //"QSlider::handle:vertical{\
    //background-color: rgb(5,255,5); color: red;\
    //border-width: 2px;\
    //border-color:#AAAAAA;\
    //border-radius: 6px;\
    //border-style: outset;\
    //width:28px;\
    //height:10px;\
    //margin: -0 -8px;\
    //}"

    //"QSlider::handle:Horizontal{background-color: rgb(5,255,5); color: red;\
    //height: 30px; \
    //width:8px;\
    //margin: -8 0px; \
    //}"// height:10px; width: 10px


    "QTextEdit{background-color: rgb(255,255,255);\
    font-size:10px;\
    }"

    "QTextBrowser{background-color: rgb(200,205,200);\
    font-size:16px;\
    }"
    /*滑块手柄*/


    //中英文字体类型有区别，字体大小与类型分贝设置
    "QLabel,QPushButton,QLineEdit,QTextEdit,QTextBrowser,QComboBox,QCheckBox,QRadioButton,\
    QGroupBox,QWidget,QMainWindow,QDialog,QSpinBox,\
    QMenuBar,QAction,QToolButton{\
    font-family:Microsoft YaHei;\
    }"
    //font-size:16pt;\
    //font-style:normal;\
    //font-weight:bolder;\
    //text-align: center;\
    //剩余不常用控件，用逗号隔开，使用相同风格
    ;

 
