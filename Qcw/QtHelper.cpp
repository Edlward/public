#include "QtHelper.h"
#include <string>

#include <QDomDocument>
//#include <QDomElement>
#include <QTextStream>


QtHelper* GetQtHelper()
{
    static QtHelper gQtHelper;
    return &gQtHelper;
}

void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const QString& sTag,const QString& sData)
{
    QDomElement Ele_Children=doc.createElement(sTag);
    QDomText text = doc.createTextNode(sData);
    Ele_Children.appendChild(text);
    parent.appendChild(Ele_Children);
    return;
}

void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const char* sTag,const char* sData,bool lutf8/*=false*/)
{
    QString qsData=(lutf8)?(toQSbyLutf8(sData)):(QString::fromStdString(sData));
    return AddChildrenElement(doc,parent,QString::fromStdString(sTag),qsData);
}

void AddChildrenElement(QDomDocument &doc,QDomElement& parent,const std::string& sTag,const std::string& sData,bool lutf8/*=false*/)
{
    QString qsData=(lutf8)?(toQSbyLutf8(sData)):(QString::fromStdString(sData));
    return AddChildrenElement(doc,parent,QString::fromStdString(sTag),qsData);
}

bool getElementValue(const QDomElement& parent,const QString& sTag,QString& Out,const QString& defaultVal/*=""*/)
{
    QDomElement EleTmp=parent.firstChildElement(sTag);
    if(!EleTmp.isElement())
    {
        Out=defaultVal;
        return false;
    }
    Out=EleTmp.text();
    return true;
}

bool getElementValue(const QDomElement& parent,const QString& sTag,std::string& Out)
{
    QDomElement EleTmp=parent.firstChildElement(sTag);
    if(!EleTmp.isElement())
    {
        Out="";
        return false;
    }
    Out=EleTmp.text().toStdString();
    return true;
}

bool getElementValue(const QDomElement& parent,const std::string& sTag,std::string& Out,const std::string& defaultVal/*=""*/,bool lutf8/*=false*/)
{
    QString sRes;
    bool isOk=getElementValue(parent,QString::fromStdString(sTag),sRes);
    if(!isOk)
    {
        Out=defaultVal;
        return false;
    }
    //Out=sRes.toStdString();
    Out=(lutf8)?(fromQsbyLutf8(sRes)):(sRes.toStdString());
    return isOk;
}

bool getElementValue(const QDomElement& parent,const char* sTag,std::string& Out,const std::string& defaultVal/*=""*/,bool lutf8/*=false*/)
{
    QString sRes;
    bool isOk=getElementValue(parent,QString::fromStdString(sTag),sRes);
    if(!isOk)
    {
        Out=defaultVal;
        return false;
    }
    //Out=sRes.toStdString();
    Out=(lutf8)?(fromQsbyLutf8(sRes)):(sRes.toStdString());
    return isOk;
}

bool getElementValue(const QDomElement& parent,const QString& sTag,int& Out,int defaultVal/*=0*/)
{
    QString sRes;
    bool isOk=getElementValue(parent,sTag,sRes);
    if(!isOk)
    {
        Out=defaultVal;
        return false;
    }
    Out=sRes.toInt(&isOk);
    if (!isOk)
    {
        Out=defaultVal;
    }
    return isOk;
}

bool getElementValue(const QDomElement& parent,const QString& sTag,unsigned int& Out,unsigned int defaultVal/*=0*/)
{
    QString sRes;
    bool isOk=getElementValue(parent,sTag,sRes);
    if(!isOk)
    {
        Out=defaultVal;
        return false;
    }
    Out=sRes.toUInt(&isOk);
    if (!isOk)
    {
        Out=defaultVal;
    }
    return isOk;
}

bool getElementValue(const QDomElement& parent,const QString& sTag,float& Out,float defaultVal/*=0*/)
{
    QString sRes;
    bool isOk=getElementValue(parent,sTag,sRes);
    if(!isOk)
    {
        Out=defaultVal;
        return false;
    }
    Out=sRes.toFloat(&isOk);
    if (!isOk)
    {
        Out=defaultVal;
    }
    return isOk;
}

bool getElementValue(const QDomElement& parent,const QString& sTag,double& Out,double defaultVal/*=0*/)
{
    QString sRes;
    bool isOk=getElementValue(parent,sTag,sRes);
    if(!isOk)
    {
        Out=defaultVal;
        return false;
    }
    Out=sRes.toDouble(&isOk);
    if (!isOk)
    {
        Out=defaultVal;
    }
    return isOk;
}

bool getElementValue(const QDomElement& parent,const QString& sTag,bool& Out,bool defaultVal/*=false*/)
{
    QString sRes;
    bool isOk=getElementValue(parent,sTag,sRes);
    if(!isOk)
    {
        Out=defaultVal;
        return false;
    }
    Out=(sRes=="true");
    //if (!isOk)
    //{
    //    Out=defaultVal;
    //}
    return isOk;
}

QtHelper::QtHelper(void)
{

}

QtHelper::~QtHelper(void)
{
}

 
