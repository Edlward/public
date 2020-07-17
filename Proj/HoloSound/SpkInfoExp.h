#ifndef SpkInfoExp_Header
#define SpkInfoExp_Header


#include "SpkInfo.h"


#include <QString>  //

//各种需要Qt翻译的基础数据类型放此处
QString toDisplayResult(RoomConfig* p);

QString spkTypeToQs(int SpkType,const std::string& sName);  //IA-Conf完善待续
QString spkTypeToQs(ESpkType SpkType);    //合并重构待续

QString conTypeToQs(int conType);

#endif









