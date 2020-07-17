#include "SpkInfoExp.h"


#include<QObject>
QString toDisplayResult(RoomConfig* p)
{
    int TotalSpeakers=p->TotalSpeakerofScreenWall+p->TotalSpeakerofLFE;
    TotalSpeakers+=(p->SpeakersPerLeftRightWall*2);
    TotalSpeakers+=(p->TotalSpeakerofBoothWall+p->TotalSpeakersofSubwoofer);
    TotalSpeakers+=(p->SpeakersofCeillingPerSide*2);

    QString strDisplay="";
    strDisplay+=QObject::tr("Room  dimensional:\n");
    strDisplay+=QObject::tr("Width:");
    strDisplay+=QString::number(p->ScreenWallLength);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+=QObject::tr("Length:");
    strDisplay+=QString::number(p->LeftRightWallLength);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+=QObject::tr("Height:");
    strDisplay+=QString::number(p->CeilingElevationatbooth);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+="\n";
    strDisplay+=QObject::tr("Screen image size flat:");
    strDisplay+="\n";
    strDisplay+=QObject::tr("W: ");
    strDisplay+=QString::number(p->FlatWidth);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+=QObject::tr("H: ");
    strDisplay+=QString::number(p->FlatHeight);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+=QObject::tr("Dis To LeftWall:");
    strDisplay+=QString::number(p->FlatScreenDisToLeftWall);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+="\n";

    strDisplay+=QObject::tr("Screen image size scope:");
    strDisplay+="\n";
    strDisplay+=QObject::tr("W:");
    strDisplay+=QString::number(p->ScopeWidth);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+=QObject::tr("H:");
    strDisplay+=QString::number(p->ScopeHeight);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+=QObject::tr("Dis To LeftWall:");
    strDisplay+=QString::number(p->ScopeScreenDisToLeftWall);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+="\n";

    strDisplay+=QObject::tr("Screen bottom height:");
    strDisplay+=QString::number(p->ScreenBottomHeight);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+="\n";



    strDisplay+=QString::number(TotalSpeakers);
    strDisplay+=QObject::tr("Total speakers");
    strDisplay+="\n";
    strDisplay+=QString::number(p->TotalSpeakerofScreenWall);
    strDisplay+=QObject::tr("Screen speakers");
    strDisplay+="\n";
    strDisplay+=QString::number(p->TotalSpeakerofLFE);
    strDisplay+=QObject::tr("LFE channel");
    strDisplay+="\n";

    strDisplay+=QString::number(p->SpeakersPerLeftRightWall);
    strDisplay+=QObject::tr("Suround speakers on left wall");
    strDisplay+="\n";

    strDisplay+=QString::number(p->SpeakersPerLeftRightWall);
    strDisplay+=QObject::tr("Suround speakers on right wall");
    strDisplay+="\n";

    strDisplay+=QString::number(p->TotalSpeakerofBoothWall);
    strDisplay+=QObject::tr("Suround speakers on booth wall");
    strDisplay+="\n";
    strDisplay+=QString::number(p->SpeakersofCeillingPerSide);
    strDisplay+=QObject::tr("Suround speakers on left overhead");
    strDisplay+="\n";
    strDisplay+=QString::number(p->SpeakersofCeillingPerSide);
    strDisplay+=QObject::tr("Suround speakers on right overhead");
    strDisplay+="\n";
    strDisplay+=QString::number(p->TotalSpeakersofSubwoofer);
    strDisplay+=QObject::tr("Bass management subwoofers");
    strDisplay+="\n";

    strDisplay+=QObject::tr("sloped floor:");
    strDisplay+="\n";
    strDisplay+=QObject::tr("floor elevation at screen:");
    strDisplay+=QString::number(0.0);   //荧幕侧地板高度0m
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+="\n";

    strDisplay+=QObject::tr("floor elevation at 2/3 point:");
    strDisplay+=QString::number(p->FloorElevationat23point);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+="\n";

    strDisplay+=QObject::tr("floor elevation at booth wall:");
    strDisplay+=QString::number(p->FloorElevationatbooth);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+="\n";

    strDisplay+=QObject::tr("tapered ceiling:");
    strDisplay+="\n";
    strDisplay+=QObject::tr("ceiling elevation at screen:");
    strDisplay+=QString::number(p->CeilingElevationatscreen);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+="\n";
    strDisplay+=QObject::tr("ceiling elevation at 2/3 point:");
    strDisplay+=QString::number(p->CeilingElevationat23point);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+="\n";

    strDisplay+=QObject::tr("ceiling elevation at booth wall:");
    strDisplay+=QString::number(p->CeilingElevationatbooth);
    strDisplay+=" ";
    strDisplay+=QObject::tr("m");
    strDisplay+=" ";
    strDisplay+="\n";

    return strDisplay;
}



QString spkTypeToQs(int SpkType,const std::string& sName)
{
    if (SpkType==SpkType_Screen)
    {
        return QObject::tr("ScreenSpk");
    }
    if (SpkType==SpkType_Surround)
    {
        return QObject::tr("SurroundSpk");
    }
    if (SpkType==SpkType_Subwoofer)
    {
        //if (sName.find("LFE")!=std::string::npos)
        //{
        //    return  QObject::tr("LFESpk");
        //}
        //else
        //{
            return QObject::tr("SubwooferSpk");
        //}
    }
    if (SpkType==SpkType_LFE)
    {
         return  QObject::tr("LFESpk");
    }

    return QObject::tr("UnknowSpk");
}


QString spkTypeToQs(ESpkType SpkType)
{
    if (SpkType==SpkType_Screen)
    {
        return QObject::tr("ScreenSpk");
    }
    if (SpkType==SpkType_Surround)
    {
        return QObject::tr("SurroundSpk");
    }
    if (SpkType==SpkType_Subwoofer)
    {
            return QObject::tr("SubwooferSpk");
    }

    if (SpkType==SpkType_LFE)
    {
            return  QObject::tr("LFESpk");
    }

    return QObject::tr("UnknowSpk");
}

QString conTypeToQs(int conType)
{
    if (conType==ConType_Stereo)
    {
        return QObject::tr("Stereo");
    }
    if (conType==ConType_Bridge)
    {
        return QObject::tr("Bridge");
    }
    if (conType==ConType_Parallel)
    {
        return QObject::tr("Parallel");
    }
    return QObject::tr("Unknow");
}


