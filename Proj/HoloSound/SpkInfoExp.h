#ifndef SpkInfoExp_Header
#define SpkInfoExp_Header


#include "SpkInfo.h"


#include <QString>  //

//������ҪQt����Ļ����������ͷŴ˴�
QString toDisplayResult(RoomConfig* p);

QString spkTypeToQs(int SpkType,const std::string& sName);  //IA-Conf���ƴ���
QString spkTypeToQs(ESpkType SpkType);    //�ϲ��ع�����

QString conTypeToQs(int conType);

#endif









