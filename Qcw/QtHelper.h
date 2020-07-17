#ifndef QtHelper_Header
#define QtHelper_Header

#include <string>
#include <QString>


inline QString toQS(const std::string& str)
{
    return QString::fromStdString(str);
    //return QString::fromLocal8Bit(str.data());
}

inline std::string fromQS(const QString& str)
{
    return str.toStdString();
    //QByteArray cdata = str.toLocal8Bit();
    //return std::string(cdata);
}

inline QString toQSbyLutf8(const std::string& str)
{
    return QString::fromLocal8Bit(str.data());
}

inline std::string fromQsbyLutf8(const QString& qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return std::string(cdata);
}


//std::wstring std::string utf8 互转,基于QT,待续
inline std::wstring toWSbyqtLutf8(const std::string& str)
{
     return  toQSbyLutf8(str).toStdWString();
}

inline std::string fromWSbyqtLutf8(const std::wstring& wstr)
{
     QByteArray cdata=QString::fromStdWString(wstr).toLocal8Bit();
    return std::string(cdata);
}


inline QString OhmStr()
{
    return  QString::fromLocal8Bit("Ω");
}


class QtHelper
{
public:
    QtHelper(void);
    ~QtHelper(void);
};

QtHelper* GetQtHelper();




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Qt-XML解析辅助操作--逐步补全待续
class QDomDocument;
class QDomElement;

//往某个Element添加子元素及其值的封装
void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const QString& sTag,const QString& sData);

//为了优先匹配常量字符串
void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const char* sTag,const char* sData,bool lutf8=false);

void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const std::string& sTag,const std::string& sData,bool lutf8=false);

inline void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const std::string& sTag,int Data)
{
    return AddChildrenElement(doc,parent,QString::fromStdString(sTag),QString::number(Data));
}
inline void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const std::string& sTag,unsigned int Data)
{
    return AddChildrenElement(doc,parent,QString::fromStdString(sTag),QString::number(Data));
}
inline void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const std::string& sTag,float Data)
{
    return AddChildrenElement(doc,parent,QString::fromStdString(sTag),QString::number(Data));
}
inline void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const std::string& sTag,double Data)
{
    return AddChildrenElement(doc,parent,QString::fromStdString(sTag),QString::number(Data));
}

inline void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const std::string& sTag,bool bEnabled)
{
    QString sV=(bEnabled)?("true"):("false");
    return AddChildrenElement(doc,parent,QString::fromStdString(sTag),sV);
}

//从某个Element取值 ,失败则存为default值
bool getElementValue(const QDomElement& parent,const QString& sTag,QString& Out,const QString& defaultVal="");
bool getElementValue(const QDomElement& parent,const QString& sTag,std::string& Out);

bool getElementValue(const QDomElement& parent,const std::string& sTag,std::string& Out,const std::string& defaultVal="",bool lutf8=false);
bool getElementValue(const QDomElement& parent,const char* sTag,std::string& Out,const std::string& defaultVal="",bool lutf8=false);

bool getElementValue(const QDomElement& parent,const QString& sTag,int& Out,int defaultVal=0);
bool getElementValue(const QDomElement& parent,const QString& sTag,unsigned int& Out,unsigned int defaultVal=0);
bool getElementValue(const QDomElement& parent,const QString& sTag,float& Out,float defaultVal=0);
bool getElementValue(const QDomElement& parent,const QString& sTag,double& Out,double defaultVal=0);
bool getElementValue(const QDomElement& parent,const QString& sTag,bool& Out,bool defaultVal=false);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



#endif


