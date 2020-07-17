#include "XmlCommon.h"

#include <QFile>
#include <QDomDocument>
//#include <QDomElement>
#include <QTextStream>

void PutToDomElement(QDomDocument &doc,QDomElement& parent,const PerSpkInfo& perSpkInfo,int IdxNum/*=0*/)
{
    QDomText text;
    QDomElement Ele_Item = doc.createElement("SpkInfo");
    Ele_Item.setAttribute("FullName",QString::fromStdString(perSpkInfo.sFullName));

    //#if 0
    //    //扬声器在库中索引号,无实际用处
    //    QDomElement Ele_IndexNum = doc.createElement("IndexNum");
    //    Ele_Item.appendChild(Ele_IndexNum);
    //    text = doc.createTextNode(QString::number(IdxNum));
    //    Ele_IndexNum.appendChild(text);
    //
    //    QDomElement Ele_FactoryShort = doc.createElement("FactoryShort");
    //    Ele_Item.appendChild(Ele_FactoryShort);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sFactoryShort));
    //    Ele_FactoryShort.appendChild(text);
    //
    //    QDomElement Ele_Name = doc.createElement("Name");
    //    Ele_Item.appendChild(Ele_Name);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sName));
    //    Ele_Name.appendChild(text);
    //
    //    QDomElement Ele_SpkType = doc.createElement("SpkType");
    //    Ele_Item.appendChild(Ele_SpkType);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sSpkType));
    //    Ele_SpkType.appendChild(text);
    //
    //    QDomElement Ele_Factory = doc.createElement("Factory");
    //    Ele_Item.appendChild(Ele_Factory);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sFactory));
    //    Ele_Factory.appendChild(text);
    //
    //    QDomElement Ele_Series = doc.createElement("Series");
    //    Ele_Item.appendChild(Ele_Series);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sSeries));
    //    Ele_Series.appendChild(text);
    //
    //
    //    QDomElement Ele_Dimensions = doc.createElement("Dimensions");
    //    Ele_Item.appendChild(Ele_Dimensions);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sDimensions));
    //    Ele_Dimensions.appendChild(text);
    //
    //
    //    QDomElement Ele_Weight = doc.createElement("Weight");
    //    Ele_Item.appendChild(Ele_Weight);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sWeight));
    //    Ele_Weight.appendChild(text);
    //
    //    QDomElement Ele_Power = doc.createElement("Power");
    //    Ele_Item.appendChild(Ele_Power);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sPower));
    //    Ele_Power.appendChild(text);
    //
    //    QDomElement Ele_Impedance = doc.createElement("Impedance");
    //    Ele_Item.appendChild(Ele_Impedance);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sImpedance));
    //    Ele_Impedance.appendChild(text);
    //
    //    QDomElement Ele_SPL = doc.createElement("SPL");
    //    Ele_Item.appendChild(Ele_SPL);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sSPL));
    //    Ele_SPL.appendChild(text);
    //
    //
    //    QDomElement Ele_Cost = doc.createElement("Cost");
    //    Ele_Item.appendChild(Ele_Cost);
    //    text = doc.createTextNode(QString::number(perSpkInfo.cost));
    //    Ele_Cost.appendChild(text);
    //
    //    QDomElement Ele_Remark = doc.createElement("Remark");
    //    Ele_Item.appendChild(Ele_Remark);
    //    text = doc.createTextNode(QString::fromStdString(perSpkInfo.sRemark));
    //    Ele_Remark.appendChild(text);
    //#endif

    AddChildrenElement(doc,Ele_Item,"IndexNum",IdxNum);
    AddChildrenElement(doc,Ele_Item,"FactoryShort",perSpkInfo.sFactoryShort);
    AddChildrenElement(doc,Ele_Item,"Name",perSpkInfo.sName);
    AddChildrenElement(doc,Ele_Item,"SpkType",perSpkInfo.sSpkType);
    AddChildrenElement(doc,Ele_Item,"Factory",perSpkInfo.sFactory);
    AddChildrenElement(doc,Ele_Item,"Series",perSpkInfo.sSeries);
    AddChildrenElement(doc,Ele_Item,"Dimensions",perSpkInfo.sDimensions);
    AddChildrenElement(doc,Ele_Item,"Weight",perSpkInfo.sWeight);
    AddChildrenElement(doc,Ele_Item,"Power",perSpkInfo.sPower);
    AddChildrenElement(doc,Ele_Item,"Impedance",perSpkInfo.sImpedance);
    AddChildrenElement(doc,Ele_Item,"SPL",perSpkInfo.sSPL);
    AddChildrenElement(doc,Ele_Item,"Cost",perSpkInfo.cost);
    AddChildrenElement(doc,Ele_Item,"Sensitivity",perSpkInfo.sSensitivity);
    AddChildrenElement(doc,Ele_Item,"ResponseFreq",perSpkInfo.sResponseFreq);
    AddChildrenElement(doc,Ele_Item,"CrossoverFreq",perSpkInfo.sCrossoverFreq);
    AddChildrenElement(doc,Ele_Item,"Remark",perSpkInfo.sRemark);

    parent.appendChild(Ele_Item);
}

void PutToDomElement(QDomDocument &doc,QDomElement& parent,const PerAmpInfo& perAmpInfo,int IdxNum/*=0*/)
{
    QDomText text;
    QDomElement Ele_Item = doc.createElement("AmpInfo");
    Ele_Item.setAttribute("FullName",QString::fromStdString(perAmpInfo.sFullName));

    //#if 0
    //    //扬声器在库中索引号,无实际用处
    //    QDomElement Ele_IndexNum = doc.createElement("IndexNum");
    //    Ele_Item.appendChild(Ele_IndexNum);
    //    text = doc.createTextNode(QString::number(IdxNum));
    //    Ele_IndexNum.appendChild(text);
    //
    //    QDomElement Ele_FactoryShort = doc.createElement("FactoryShort");
    //    Ele_Item.appendChild(Ele_FactoryShort);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sFactoryShort));
    //    Ele_FactoryShort.appendChild(text);
    //
    //    QDomElement Ele_Name = doc.createElement("Name");
    //    Ele_Item.appendChild(Ele_Name);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sName));
    //    Ele_Name.appendChild(text);
    //
    //    QDomElement Ele_AmpType = doc.createElement("AmpType");
    //    Ele_Item.appendChild(Ele_AmpType);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sAmpType));
    //    Ele_AmpType.appendChild(text);
    //
    //    QDomElement Ele_Factory = doc.createElement("Factory");
    //    Ele_Item.appendChild(Ele_Factory);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sFactory));
    //    Ele_Factory.appendChild(text);
    //
    //    QDomElement Ele_Series = doc.createElement("Series");
    //    Ele_Item.appendChild(Ele_Series);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sSeries));
    //    Ele_Series.appendChild(text);
    //
    //
    //    QDomElement Ele_Dimensions = doc.createElement("Dimensions");
    //    Ele_Item.appendChild(Ele_Dimensions);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sDimensions));
    //    Ele_Dimensions.appendChild(text);
    //
    //
    //    QDomElement Ele_Weight = doc.createElement("Weight");
    //    Ele_Item.appendChild(Ele_Weight);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sWeight));
    //    Ele_Weight.appendChild(text);
    //
    //    QDomElement Ele_Power = doc.createElement("Power");
    //    Ele_Item.appendChild(Ele_Power);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sPower));
    //    Ele_Power.appendChild(text);
    //
    //    QDomElement Ele_Impedance = doc.createElement("Impedance");
    //    Ele_Item.appendChild(Ele_Impedance);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sImpedance));
    //    Ele_Impedance.appendChild(text);
    //
    //    //QDomElement Ele_SPL = doc.createElement("SPL");
    //    //Ele_Item.appendChild(Ele_SPL);
    //    //text = doc.createTextNode(QString::fromStdString(perAmpInfo.sSPL));
    //    //Ele_SPL.appendChild(text);
    //
    //
    //    QDomElement Ele_Cost = doc.createElement("Cost");
    //    Ele_Item.appendChild(Ele_Cost);
    //    text = doc.createTextNode(QString::number(perAmpInfo.cost));
    //    Ele_Cost.appendChild(text);
    //
    //    QDomElement Ele_Remark = doc.createElement("Remark");
    //    Ele_Item.appendChild(Ele_Remark);
    //    text = doc.createTextNode(QString::fromStdString(perAmpInfo.sRemark));
    //    Ele_Remark.appendChild(text);
    //#endif

    AddChildrenElement(doc,Ele_Item,"IndexNum",IdxNum);  //扬声器在库中索引号,无实际用处
    AddChildrenElement(doc,Ele_Item,"FactoryShort",perAmpInfo.sFactoryShort);
    AddChildrenElement(doc,Ele_Item,"Name",perAmpInfo.sName);
    AddChildrenElement(doc,Ele_Item,"AmpType",perAmpInfo.sAmpType);
    AddChildrenElement(doc,Ele_Item,"Factory",perAmpInfo.sFactory);
    AddChildrenElement(doc,Ele_Item,"Series",perAmpInfo.sSeries);
    AddChildrenElement(doc,Ele_Item,"Dimensions",perAmpInfo.sDimensions);
    AddChildrenElement(doc,Ele_Item,"Weight",perAmpInfo.sWeight);
    AddChildrenElement(doc,Ele_Item,"ChannelNum",perAmpInfo.ChannelNum);
    AddChildrenElement(doc,Ele_Item,"Power",perAmpInfo.sPower);
    AddChildrenElement(doc,Ele_Item,"BridgePower",perAmpInfo.sBridgePower);
    AddChildrenElement(doc,Ele_Item,"Impedance",perAmpInfo.sImpedance);
    AddChildrenElement(doc,Ele_Item,"VoltageGain",perAmpInfo.sVoltageGain);

    AddChildrenElement(doc,Ele_Item,"Cost",perAmpInfo.cost);
    AddChildrenElement(doc,Ele_Item,"Remark",perAmpInfo.sRemark);

    parent.appendChild(Ele_Item);
}

void GetFromDomElement(const QDomElement& EleSpkInfo,PerSpkInfo& perSpkInfo)
{
    const QDomElement& EleCurr=EleSpkInfo;

    QDomElement EleTmp;
    if(EleCurr.isElement())
    {
        perSpkInfo.sFullName=EleCurr.attribute("FullName").toStdString();
    }

    //#if 0
    //    //扬声器在库中索引号,无实际用处
    //    EleTmp = EleCurr.firstChildElement("IndexNum");
    //    if(EleTmp.isElement())
    //    {
    //        
    //    }
    //    EleTmp = EleCurr.firstChildElement("FactoryShort");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sFactoryShort= EleTmp.text().toStdString();
    //    }
    //    EleTmp = EleCurr.firstChildElement("Name");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sName= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("SpkType");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sSpkType= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Factory");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sFactory= EleTmp.text().toStdString();
    //    }
    //
    //
    //    EleTmp = EleCurr.firstChildElement("Series");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sSeries= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Dimensions");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sDimensions= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Weight");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sWeight= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Power");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sPower= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Impedance");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sImpedance= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("SPL");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sSPL= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Cost");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.cost= EleTmp.text().toFloat();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Remark");
    //    if(EleTmp.isElement())
    //    {
    //        perSpkInfo.sRemark= EleTmp.text().toStdString();
    //    }
    //#endif

    //getElementValue(EleCurr,"IndexNum",IdxNum);  //扬声器在库中索引号,无实际用处
    getElementValue(EleCurr,"FactoryShort",perSpkInfo.sFactoryShort);
    getElementValue(EleCurr,"Name",perSpkInfo.sName);
    getElementValue(EleCurr,"SpkType",perSpkInfo.sSpkType);
    getElementValue(EleCurr,"Factory",perSpkInfo.sFactory);
    getElementValue(EleCurr,"Series",perSpkInfo.sSeries);
    getElementValue(EleCurr,"Dimensions",perSpkInfo.sDimensions);
    getElementValue(EleCurr,"Weight",perSpkInfo.sWeight);
    getElementValue(EleCurr,"Power",perSpkInfo.sPower);
    getElementValue(EleCurr,"Impedance",perSpkInfo.sImpedance);
    getElementValue(EleCurr,"SPL",perSpkInfo.sSPL);
    getElementValue(EleCurr,"Cost",perSpkInfo.cost);
    getElementValue(EleCurr,"Sensitivity",perSpkInfo.sSensitivity);
    getElementValue(EleCurr,"ResponseFreq",perSpkInfo.sResponseFreq);
    getElementValue(EleCurr,"CrossoverFreq",perSpkInfo.sCrossoverFreq);
    getElementValue(EleCurr,"Remark",perSpkInfo.sRemark);

    perSpkInfo.updateValueFromString();
}

void GetFromDomElement(const QDomElement& parent,PerAmpInfo& perAmpInfo)
{
    //QDomElement EleCurr = ListAmp.at(n).toElement();
    const QDomElement& EleCurr=parent;
    QDomElement EleTmp;
    if(EleCurr.isElement())
    {
        perAmpInfo.sFullName=EleCurr.attribute("FullName").toStdString();
    }

    //#if 0
    //    EleTmp = EleCurr.firstChildElement("IndexNum");
    //    if(EleTmp.isElement())
    //    {
    //
    //    }
    //    EleTmp = EleCurr.firstChildElement("FactoryShort");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sFactoryShort= EleTmp.text().toStdString();
    //    }
    //    EleTmp = EleCurr.firstChildElement("Name");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sName= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("AmpType");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sAmpType= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Factory");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sFactory= EleTmp.text().toStdString();
    //    }
    //
    //
    //    EleTmp = EleCurr.firstChildElement("Series");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sSeries= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Dimensions");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sDimensions= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Weight");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sWeight= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Power");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sPower= EleTmp.text().toStdString();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Impedance");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sImpedance= EleTmp.text().toStdString();
    //    }
    //
    //    //EleTmp = EleCurr.firstChildElement("SPL");
    //    //if(EleTmp.isElement())
    //    //{
    //    //    perAmpInfo.sSPL= EleTmp.text().toStdString();
    //    //}
    //
    //    EleTmp = EleCurr.firstChildElement("Cost");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.cost= EleTmp.text().toFloat();
    //    }
    //
    //    EleTmp = EleCurr.firstChildElement("Remark");
    //    if(EleTmp.isElement())
    //    {
    //        perAmpInfo.sRemark= EleTmp.text().toStdString();
    //    }
    //#endif

    //getElementValue(EleCurr,"IndexNum",IdxNum);
    getElementValue(EleCurr,"FactoryShort",perAmpInfo.sFactoryShort);
    getElementValue(EleCurr,"Name",perAmpInfo.sName);
    getElementValue(EleCurr,"AmpType",perAmpInfo.sAmpType);
    getElementValue(EleCurr,"Factory",perAmpInfo.sFactory);
    getElementValue(EleCurr,"Series",perAmpInfo.sSeries);
    getElementValue(EleCurr,"Dimensions",perAmpInfo.sDimensions);
    getElementValue(EleCurr,"Weight",perAmpInfo.sWeight);
    getElementValue(EleCurr,"ChannelNum",perAmpInfo.ChannelNum);
    getElementValue(EleCurr,"Power",perAmpInfo.sPower);
    getElementValue(EleCurr,"BridgePower",perAmpInfo.sBridgePower);
    getElementValue(EleCurr,"Impedance",perAmpInfo.sImpedance);
    //getElementValue(EleCurr,"SPL",perAmpInfo.sSPL);
    
    getElementValue(EleCurr,"VoltageGain",perAmpInfo.sVoltageGain);
    getElementValue(EleCurr,"Cost",perAmpInfo.cost);
    getElementValue(EleCurr,"Remark",perAmpInfo.sRemark);

    perAmpInfo.updateValueFromString();
}

void GetFromDomElementFromParent(const QDomElement& parent,PerSpkInfo& perSpkInfo)
{
    const QDomElement EleSpkInfo=parent.firstChildElement("SpkInfo");
    if (!EleSpkInfo.isElement())
    {
        //失败在此复位默认值 tobe confirm
        perSpkInfo=PerSpkInfo();
        return;
    }
    return GetFromDomElement(EleSpkInfo,perSpkInfo);
}

void GetFromDomElementFromParent(const QDomElement& parent,PerAmpInfo& perAmpInfo)
{
    const QDomElement EleAmpInfo=parent.firstChildElement("AmpInfo");
    if (!EleAmpInfo.isElement())
    {
        //失败在此复位默认值 tobe confirm
        perAmpInfo=PerAmpInfo();
        return;
    }
    return GetFromDomElement(EleAmpInfo,perAmpInfo);
}
