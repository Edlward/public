#ifndef XmlCommon_Header
#define XmlCommon_Header


#include "../../../public/Qcw/QtHelper.h"

#include "SpkInfoExp.h"     //扬声器属性

//不同子项目相同XML元素的解析放此处,若修改请保证在所有子项目测试确认



//PerSpkInfo SubwooferSpk;     //选中的低音扬声器
//PerAmpInfo AmpInfo; 

//往某QDomElement里存取PerSpkInfo结构的封装,多处复用,在此封装
void PutToDomElement(QDomDocument &doc,QDomElement& parent,const PerSpkInfo& perSpkInfo,int IdxNum=0);

//往某QDomElement里存取PerSpkInfo结构的封装,多处复用,在此封装
void GetFromDomElement(const QDomElement& EleSpkInfo,PerSpkInfo& perSpkInfo);

void GetFromDomElementFromParent(const QDomElement& parent,PerSpkInfo& perSpkInfo);


//往某QDomElement里存取PerAmpInfo结构的封装,多处复用,在此封装
void PutToDomElement(QDomDocument &doc,QDomElement& parent,const PerAmpInfo& perAmpInfo,int IdxNum=0);

//往某QDomElement里存取PerAmpInfo结构的封装,多处复用,在此封装
void GetFromDomElement(const QDomElement& parent,PerAmpInfo& perAmpInfo);


void GetFromDomElementFromParent(const QDomElement& parent,PerAmpInfo& perAmpInfo);




#endif // !XMLHELPER_H
