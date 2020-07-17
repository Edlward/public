#include "XMLHelper.h"

#include <iostream>
#include <string>
#include <vector>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>
//#include <QThread>


#include "definition.h"    //基础宏定义


#include "Super/Basic/Tstring.h"



//为了避免宏重复定义放到cpp中  by lizulin 20180425
//为了兼容已有配置文件，字符串名称与xml文件结构确认后不可以随意更改  

#define SCHEMALOCATION "http://www.leoniscinema.com/cp/holesound/config_HoleSoundConfigSuiteData.xsd"
#define XMLNS "http://www.leoniscinema.com/cp/holesound/config"
#define OUTCONFIG "DCAP_OBIA_OutputConfiguration"       //侵权 原"cp850OutputConfiguration"  //
#define CONFIGSUITEDATA "HoloSound_ConfigSuiteData"    //侵权 原 "atmosConfigSuiteData"
#define PROCESSORCONFIG "processorConfiguration"
#define ROOMCONFIG "roomConfiguration"
#define NAME "name"
#define ROOM "room"
#define INFO "info"
#define METADATA "metadata"
#define _UUID "uuid"
#define LASTMODIFIEDTIME "lastModifiedTime"
#define LOCALPATHATORIGIN "localPathAtOrigin"
#define COMMENTS "comments"
#define TEXT_ "text"
#define TIME "time"
#define ROOMCONFIGURATION "roomConfiguration"
#define COMPANY "company"
#define THEATERNAME "theaterName"
#define AUDITORIUM "auditorium"
#define CITY "city"
#define STATE "state"
#define COUNTRY "country"
#define DIMENSIONS "dimensions"
#define SCREENWALLWIDTH "screenWallWidth"
#define BOOTHWALLWIDTH "boothWallWidth"
#define HOUSELEFTWALLWIDTH "houseLeftWallWidth"
#define HOUSERIGHTWALLWIDTH "houseRightWallWidth"
#define SCREENHOUSELEFTINTERIORANGLE "screenHouseLeftInteriorAngle"
#define SCREENHOUSERIGHTINTERIORANGLE "screenHouseRightInteriorAngle"
#define BOOTHHOUSELEFTINTERIORANGLE "boothHouseLeftInteriorAngle"
#define BOOTHHOUSERIGHTINTERIORANGLE "boothHouseRightInteriorAngle"
#define FLOORELEVATIONATSCREEN "floorElevationAtScreen"
#define CEILINGELEVATIONATSCREEN "ceilingElevationAtScreen"
#define FLOORELEVATIONATTWOTHIRDS "floorElevationAtTwoThirds"
#define CEILINGELEVATIONATTWOTHIRDS "ceilingElevationAtTwoThirds"
#define FLOORELEVATIONATBOOTH "floorElevationAtBooth"
#define CEILINGELEVATIONATBOOTH "ceilingElevationAtBooth"
#define FLATSCREEN "flatScreen"
#define ROOM_WIDTH "width"
#define ROOM_HEIGHT "height"
#define DISTANCETOLEFTWALL "distanceToLeftWall"
#define POS_X "x"
#define POS_Y "y"
#define POS_Z "z"
#define SCOPESCREEN "scopeScreen"
#define REGION "region"
#define SPEAKERENDPOINT "speakerEndpoint"
#define SPK_ID "id"
#define TYPE "type"
#define ARRAY "array"
#define BASSMANAGEMENT "bassManagement"
#define SUBWOOFERREF "subwooferRef"
#define CROSSOVERFREQUENCYHZ "crossoverFrequencyHz"
#define DELAY "delay"
#define SPECSREF "specsRef"
#define POSITION "position"
#define SPEAKERSPECS "speakerSpecs"
#define MANUFACTURER "manufacturer"
#define MODEL "model"
#define FREQUENCYRESPONSEMINHZ "frequencyResponseMinHz"
#define FREQUENCYRESPONSEMAXHZ "frequencyResponseMaxHz"
#define CROSSOVERFREQUENCYHZ "crossoverFrequencyHz"
#define PEAKINPUTDBUPK "peakInputDbuPk"
#define EQCONFIGURATION "eqConfiguration"
#define ARRAYEQ "arrayEq"
#define DELAYMS "delayMs"
#define GAINDB "gainDb"
#define USERGAINDB "userGainDb"
#define EQRESPONSECURVE "eqResponseCurve"
#define RESPONSEPOINT "responsePoint"
#define FREQUENCYHZ "frequencyHz"
#define ISEQENABLED "isEqEnabled"
#define EQRESPONSECURVE "eqResponseCurve"
#define RESPONSEPOINT "responsePoint"
#define TARGETEQRESPONSECURVE "targetEqResponseCurve"
#define USEDEFAULTXCURVE "useDefaultXCurve"
#define LOWFREQBOOSTLIMITHZ "lowFreqBoostLimitHz"
#define HIGHFREQBOOSTLIMITHZ "highFreqBoostLimitHz"
#define XCURVEPARAMETERS "xCurveParameters"
#define FREQCORNERAHZ "freqCornerAHz"
#define FREQCORNERBHZ "freqCornerBHz"
#define SLOPEADBPEROCT "slopeAdBPerOct"
#define SLOPEBDBPEROCT "slopeBdBPerOct"
#define SPEAKEREQ "speakerEq"
#define SPEAKERENDPOINTREF "speakerEndpointRef"
#define ISLIMITERENABLED "isLimiterEnabled"
#define OUTPUTCONFIGURATION "outputConfiguration"
#define AMPLIFIER "amplifier"
#define VOLTAGEGAINDB "voltageGainDb"
#define PEAKVOLTAGE "peakVoltage"
#define AMPPERCHANNELSPECS "ampPerChannelSpecs"
#define IMPEDANCEOHM "impedanceOhm"
#define POWERWATTS "powerWatts"
#define OUTPUTROUTE "outputRoute"
#define OUTPUTREF "outputRef"
#define SPEAKERENDPOINTREF "speakerEndpointRef"
#define AMPLIFIERREF "amplifierRef"
#define AMPLIFIERTRIMDB "amplifierTrimDb"
#define LIMITERTYPE "limiterType"
#define SPEAKERPROCESSORREF "speakerProcessorRef"
#define SPEAKERPROCESSOR "speakerProcessor"
#define PEAKINPUTDBUPK "peakInputDbuPk"
#define BLULINKOUTPUT "blulinkOutput"
#define PEAKOUTPUTDBUPK "peakOutputDbuPk"
#define ANALOGOUTPUT "analogOutput"
#define ANALOGOUTPUTTRIMDB "analogOutputTrimDb"
#define SCREENFILTERMODE "screenFilterMode"


//扬声器/功放设备名称支持新增
#define SPK_DEVICE_NAME "SpkDeviceName"         //具体扬声器型号名称
#define AMP_DEVICE_NAME "AmpDeviceName"      //具体功放型号名称
#define SPK_AMP_DEVICE_LIB "SpkAmpDeviceLib"
#define SPK_DEVICE_LIST "SpkDeviceList"
#define AMP_DEVICE_LIST "AmpDeviceList"


//新增自定义均衡相关
//#define EQCONFIG		"EQConfig"

//新增自定义均衡
//同名字符用宏来保证读写一致
#define  EQGROUPALLINFO "EqGroupAllInfo"
#define  CURRENT_EQGROUP_NAME "CurrentEqGroupName"
#define  EQGROUP_NAME "EqGroupName"
#define  SPEAKERS "Speakers"
#define  SPEAKER  "Speaker" 
#define  GRAPHICSEQ "graphicsEQ"
#define  SEGMENT  "Segment"
#define  EQTYPE "EQType"
#define  BASS_TREBLE "bass_treble"
#define  BASS "bass"
#define  TREBLE "treble"
#define  NHZ "nHz"
#define  OUTPUTLEVEL "outputlevel"
#define  LEVEL "level"
#define  ARRAYS "Arrays"

//各种不同类型扬声器boostlimit,为了兼容旧的配置文件，
//解析不到设为默认值
//ScreenSpk银幕侧扬声器就是主扬声器,一个概念
//Surround: 80-12.5K
//Main: 50-12.5K
//Sub: 32-125

static const float ScreenSpkBoostLimitLowFreq=50;
static const float ScreenSpkBoostLimitHighFreq=12.5*1000;  //12.5k
static const float SurroundSpkBoostLimitLowFreq=80;
static const float SurroundSpkBoostLimitHighFreq=12.5*1000;   //12.5k
static const float SubwooferSpkBoostLimitLowFreq=32;
static const float SubwooferSpkBoostLimitHighFreq=125;




//根据名称与旧的扬声器类型纠正扬声器类型，若旧类型非法
std::string SpkNameToSpkType(const std::string& sSpkName,const std::string& sSpkTypeOld)
{
    if (sSpkName.empty())
    {
        return "";
    }

     std::string sSpkType=sSpkTypeOld;

    //旧影厅配置文件可能解析到如下形式,且大小写各异不统一,为了兼容,统一扬声器类型的表现形式
    //"screenSpeaker" "surroundSpeaker" "SubwooferSpeaker";
    //扬声器类型某些环节可能是手工填写,宽松容错纠正,排除前缀空格,忽略大小写的区别
    if (Contains(sSpkType,SPKTYPE_SCREEN,false))   
    {
        sSpkType=SPKTYPE_SCREEN;
    }
    else if (Contains(sSpkType,SPKTYPE_SURROUND,false))   
    {
        sSpkType=SPKTYPE_SURROUND;
    }
    else if (Contains(sSpkType,SPKTYPE_SUBWOOFER,false))   
    {
        sSpkType=SPKTYPE_SUBWOOFER;
    }
    else
    {
        sSpkType=SPKTYPE_UNKNOW;
    }

    //若解析不到扬声器类型,或者不规范不是已知类型
    if (sSpkType.empty()
        ||(!Comp(sSpkType,SPKTYPE_SUBWOOFER))    //忽略大小写的字符串比较
        ||(!Comp(sSpkType,SPKTYPE_SURROUND))
        ||(!Comp(sSpkType,SPKTYPE_SCREEN)))
    {
        //根据扬声名称器前缀判断类型,名称以L R C 开头的是主扬声器，注意有空格
        if (startsWith(sSpkName,"L ",false)||startsWith(sSpkName,"R ",false)||startsWith(sSpkName,"C ",false))
        {
            return SPKTYPE_SCREEN;
        }
        else if (startsWith(sSpkName,"LFE",false)||startsWith(sSpkName,"Sub",false))
        {
            return SPKTYPE_SUBWOOFER;
        }
        else if (startsWith(sSpkName,"Lss",false)
            ||startsWith(sSpkName,"Lts",false)
            ||startsWith(sSpkName,"Lrs",false)
            ||startsWith(sSpkName,"Rss",false)
            ||startsWith(sSpkName,"Rts",false)
            ||startsWith(sSpkName,"Rrs",false)
            ||startsWith(sSpkName,"Lw",false)    //左宽
            ||startsWith(sSpkName,"Rw",false)   //右宽
            ||startsWith(sSpkName,"Lc",false)     //?
            ||startsWith(sSpkName,"Rc",false)    //
            ||startsWith(sSpkName,"Crs",false))  //
        {
            //环绕扬声器前缀 待确认
            //const std::string strSurroundName[]={"Lc","Rc","Lw","Rw","Lss","Rss","Lrs","Rrs","Lts","Rts","Crs"};

            return SPKTYPE_SURROUND;
        }
        else
        {
            return  "";    //TODO:新增未定义类型或者空
        }
    }
    return "";
}

//wstring version
std::wstring SpkNameToSpkType(const std::wstring& sSpkName,const std::wstring& sSpkTypeOld)
{
      return StringToWString(SpkNameToSpkType(WStringToString(sSpkName),WStringToString(sSpkTypeOld)));
}





void LHC::_EQTypeAll_::updateInfo()
{
    if (sSpkName.empty())
    {
        return;
    }

    sSpkType=SpkNameToSpkType(sSpkName,sSpkType);

    //若解析不到 或为0，可能是旧的配置文件没有该项目，根据扬声器类型给一个默认值
    if (BoostLimitLowFreq<=0)
    {
        if (Comp(sSpkType,SPKTYPE_SCREEN))
        {
            BoostLimitLowFreq=ScreenSpkBoostLimitLowFreq;
        }
        else if (Comp(sSpkType,SPKTYPE_SURROUND))
        {
            BoostLimitLowFreq=SurroundSpkBoostLimitLowFreq;
        }
        else if (Comp(sSpkType,SPKTYPE_SUBWOOFER))
        {
            BoostLimitLowFreq=SubwooferSpkBoostLimitLowFreq;
        }
    }
    if (BoostLimitHighFreq<=0)
    {
        if (Comp(sSpkType,SPKTYPE_SCREEN))
        {
            BoostLimitHighFreq=ScreenSpkBoostLimitHighFreq;
        }
        else if (Comp(sSpkType,SPKTYPE_SURROUND))
        {
            BoostLimitHighFreq=SurroundSpkBoostLimitHighFreq;
        }
        else if (Comp(sSpkType,SPKTYPE_SUBWOOFER))
        {
            BoostLimitHighFreq=SubwooferSpkBoostLimitHighFreq;
        }
    }


    //溢出修正,声音频率范围是20-20khz
    if (BoostLimitLowFreq<20)
    {
           BoostLimitLowFreq=20;
    }
    else if (BoostLimitLowFreq>20*1000)
    {
        BoostLimitLowFreq=20*1000;
    }

    if (BoostLimitHighFreq<20)
    {
        BoostLimitHighFreq=20;
    }
    else if (BoostLimitHighFreq>20*1000)
    {
        BoostLimitHighFreq=20*1000;
    }
}

LHC::_EQTypeAll_::_EQTypeAll_()
{
    for (int i=0;i<NumBand;i++)
    {
        graphicEQ.gain[i]=0;
    }
    for(int i = 0; i < 4; i++)
    {
        bassTreble[i].bass=0;
        bassTreble[i].treble=0;
        bassTreble[i].nHz=i;
    }
    outPutLevel.level=0;
    channel=65535;   //初始化实际映射声道号为无效值

    BoostLimitEnabled=false;   //使能
    BoostLimitLowFreq=0;
    BoostLimitHighFreq=0;
    spkDelayMs=0;
}


void LHC::_EQTypeAll_::print() const
{
    //printf("EQType: graphicEQ\n");
    for (int i=0;i<NumBand;i++)
    {
        //  printf("%f ",graphicEQ.gain[i]);
    }
    //printf("\n");
    printf("EQType: outputLevel:%f\n",outPutLevel.level);
}


size_t NeedBytesToStream(const LHC::_EQTypeAll& tmp) //计算本结构转换成网络流需要的字节数量
{
    //PRINT_FUN_TRACE

    size_t nBytes=0;
    nBytes+=::NeedBytesToStream(tmp.graphicEQ);
    nBytes+=::NeedBytesToStream(tmp.bassTreble);
    nBytes+=::NeedBytesToStream(tmp.outPutLevel);
    nBytes+=::NeedBytesToStream(tmp.channel);
    nBytes+=::NeedBytesToStream(tmp.sSpkName);
    nBytes+=::NeedBytesToStream(tmp.sSpkType);
    nBytes+=::NeedBytesToStream(tmp.BoostLimitEnabled);
    nBytes+=::NeedBytesToStream(tmp.BoostLimitLowFreq);
    nBytes+=::NeedBytesToStream(tmp.BoostLimitHighFreq);
    nBytes+=::NeedBytesToStream(tmp.spkDelayMs);
    return nBytes;
}

size_t StreamPut(char*& pos,size_t& AccLen,const LHC::_EQTypeAll& tmp)
{
    //PRINT_FUN_TRACE

    char* pBegin=pos;
    StreamPut(pos,AccLen,tmp.graphicEQ);
    StreamPut(pos,AccLen,tmp.bassTreble);
    StreamPut(pos,AccLen,tmp.outPutLevel);
    StreamPut(pos,AccLen,tmp.channel);
    StreamPut(pos,AccLen,tmp.sSpkName);
    StreamPut(pos,AccLen,tmp.sSpkType);
    StreamPut(pos,AccLen,tmp.BoostLimitEnabled);
    StreamPut(pos,AccLen,tmp.BoostLimitLowFreq);
    StreamPut(pos,AccLen,tmp.BoostLimitHighFreq);
    StreamPut(pos,AccLen,tmp.spkDelayMs);
    return pos-pBegin;
}

size_t StreamGet(char*& pos,size_t& AccLen,LHC::_EQTypeAll& tmp)
{
    //PRINT_FUN_TRACE

    char* pBegin=pos;
    StreamGet(pos,AccLen,tmp.graphicEQ);
    StreamGet(pos,AccLen,tmp.bassTreble);
    StreamGet(pos,AccLen,tmp.outPutLevel);
    StreamGet(pos,AccLen,tmp.channel);
    StreamGet(pos,AccLen,tmp.sSpkName);
    StreamGet(pos,AccLen,tmp.sSpkType);
    StreamGet(pos,AccLen,tmp.BoostLimitEnabled);
    StreamGet(pos,AccLen,tmp.BoostLimitLowFreq);
    StreamGet(pos,AccLen,tmp.BoostLimitHighFreq);
    StreamGet(pos,AccLen,tmp.spkDelayMs);
    return pos-pBegin;
}

size_t NeedBytesToStream(const LHC::EQSpeakerOne& tmp) //计算本结构转换成网络流需要的字节数量
{
    //PRINT_FUN_TRACE

    size_t nBytes=0;
    nBytes+=::NeedBytesToStream(tmp.sGroupName);
    nBytes+=::NeedBytesToStream(tmp.sSpkName);
    nBytes+=::NeedBytesToStream(tmp.isArray);
    nBytes+=::NeedBytesToStream(tmp.eqTypeAll);
    return nBytes;
}

size_t StreamPut(char*& pos,size_t& AccLen,const LHC::EQSpeakerOne& tmp)
{
    //PRINT_FUN_TRACE

    char* pBegin=pos;
    StreamPut(pos,AccLen,tmp.sGroupName);
    StreamPut(pos,AccLen,tmp.sSpkName);
    StreamPut(pos,AccLen,tmp.isArray);
    StreamPut(pos,AccLen,tmp.eqTypeAll);
    return pos-pBegin;
}

size_t StreamGet(char*& pos,size_t& AccLen,LHC::EQSpeakerOne& tmp)
{
    //PRINT_FUN_TRACE

    char* pBegin=pos;
    StreamGet(pos,AccLen,tmp.sGroupName);
    StreamGet(pos,AccLen,tmp.sSpkName);
    StreamGet(pos,AccLen,tmp.isArray);
    StreamGet(pos,AccLen,tmp.eqTypeAll);
    return pos-pBegin;
}

void LHC::EQSpeakerOne::print() const
{
    printf("EQSpeakerOne: GroupName:%s SpkName:%s isArray:%d\n",
        WStringToString(sGroupName).c_str(),WStringToString(sSpkName).c_str(),isArray);
}

size_t NeedBytesToStream(const LHC::EQGroupOne& tmp)   //计算结构转换成网络流需要的字节数量
{
    size_t useLen=0;
    NetSizeType size_speakerEQ=tmp.speakerEQ.size();
    useLen+=sizeof(size_speakerEQ);
    std::map<std::wstring,LHC::_EQTypeAll>::const_iterator it;
    for(it=tmp.speakerEQ.begin();it!=tmp.speakerEQ.end();it++)
    {
        const std::wstring& sName=it->first;
        const LHC::_EQTypeAll& tmp=it->second;
        useLen+=::NeedBytesToStream(sName);
        useLen+=::NeedBytesToStream(tmp);
    }
    NetSizeType size_arrayEQ=tmp.arrayEQ.size();
    useLen+=sizeof(size_arrayEQ);
    for(it=tmp.arrayEQ.begin();it!=tmp.arrayEQ.end();it++)
    {
        const std::wstring& sName=it->first;
        const LHC::_EQTypeAll& tmp=it->second;
        useLen+=::NeedBytesToStream(sName);
        useLen+=::NeedBytesToStream(tmp);
    }
    return useLen;
}

size_t StreamPut(char*& pos,size_t& AccLen,const LHC::EQGroupOne& tmp)
{
    char* pBegin=pos;
    NetSizeType size_speakerEQ=tmp.speakerEQ.size();
    StreamPut(pos,AccLen, size_speakerEQ);
    std::map<std::wstring,LHC::_EQTypeAll>::const_iterator it;
    for(it=tmp.speakerEQ.begin();it!=tmp.speakerEQ.end();it++)
    {
        const std::wstring& sSpkName=it->first;
        const LHC::_EQTypeAll& tmpEQTypeAll=it->second;
        StreamPut(pos,AccLen,sSpkName);
        StreamPut(pos,AccLen,tmpEQTypeAll);
    }
    NetSizeType size_arrayEQ=tmp.arrayEQ.size();
    StreamPut(pos,AccLen,size_arrayEQ);
    for(it=tmp.arrayEQ.begin();it!=tmp.arrayEQ.end();it++)
    {
        const std::wstring& sSpkName=it->first;
        const LHC::_EQTypeAll& tmpEQTypeAll=it->second;
        StreamPut(pos,AccLen,sSpkName);
        StreamPut(pos,AccLen,tmpEQTypeAll);
    }
    return pos-pBegin;
}

size_t StreamGet(char*& pos,size_t& AccLen,LHC::EQGroupOne& tmp)
{
    char* pBegin=pos;
    NetSizeType size_speakerEQ;
    StreamGet(pos,AccLen, size_speakerEQ);
    tmp.speakerEQ.clear();
    //std::map<std::wstring, _EQTypeAll>::iterator it;
    for(NetSizeType n=0;n<size_speakerEQ;n++)
    {  
        std::wstring sSpkName;
        LHC::_EQTypeAll tmpEQTypeAll;
        StreamGet(pos,AccLen,sSpkName);
        StreamGet(pos,AccLen,tmpEQTypeAll);
        //map插入方法1  //不存在则创建,已存在则覆盖
        tmp.speakerEQ[sSpkName]=tmpEQTypeAll;
        //map插入方法2  //不存在则创建,已存在键将忽略,而非值覆盖
        //speakerEQ.insert(std::map<std::wstring, _EQTypeAll>::value_type(sSpkName,tmpEQTypeAll));
    }

    NetSizeType size_arrayEQ;
    StreamGet(pos,AccLen, size_arrayEQ);
    tmp.arrayEQ.clear();
    //std::map<std::wstring, _EQTypeAll>::iterator it;
    for(NetSizeType n=0;n<size_arrayEQ;n++)
    {  
        std::wstring sSpkName;
        LHC::_EQTypeAll tmpEQTypeAll;
        StreamGet(pos,AccLen,sSpkName);
        StreamGet(pos,AccLen,tmpEQTypeAll);
        tmp.arrayEQ[sSpkName]=tmpEQTypeAll;
    }
    return pos-pBegin;
}

void LHC::EQGroupOne::print() const
{
    std::map<std::wstring, _EQTypeAll>::const_iterator it;
    printf("speakerEQ:\n");
    for(it=speakerEQ.begin();it!=speakerEQ.end();it++)
    {
        printf("%s ||",WStringToString(it->first).c_str());
        const LHC::_EQTypeAll& tmp=it->second;
        tmp.print();
    }
    printf("\n");
    printf("arrayEQ:\n");
    for(it=arrayEQ.begin();it!=arrayEQ.end();it++)
    {
        printf("%s ||",WStringToString(it->first).c_str());
        const LHC::_EQTypeAll& tmp=it->second;
        tmp.print();
    }
    printf("\n");
}

LHC::EQCoeffInfo::EQCoeffInfo()
{
    isEnabledEQ=true;
    CurrentEQGroupName=StringToWString(sDefaultGroupName);
    EQALL.clear();
}




Lhc* GetLhc()
{
    //单例对象,放函数外更高效,但无法保证使用时已构造完成(main函数前被调用),//若单例之间依赖复杂为保证构造顺序就放函数内,并为构造时线程安全C++11之前要加锁 C++11之后由编译器保证 by lizulin
    static Lhc gLhcClass;
    return &gLhcClass;
}




//往某QDomElement里存取某种结构的封装,多处复用,在此封装
void PutToDomElement(QDomDocument &doc,QDomElement& ElementSpeaker,const LHC::_EQTypeAll& eqSpkOne)
{
    QDomText text;
    QDomElement EleTmp;

    ElementSpeaker.setAttribute("name", QString::fromStdString(eqSpkOne.sSpkName));
    ElementSpeaker.setAttribute("channel",QString::number(eqSpkOne.channel));

    QDomElement ElementEQTypeGraphicEQ=doc.createElement(EQTYPE);
    ElementEQTypeGraphicEQ.setAttribute("name",GRAPHICSEQ);
    for (int band=0;band<NumBand;band++)
    {
        QDomElement ElementSegment=doc.createElement(SEGMENT);
        ElementSegment.setAttribute("band", QString::number(band));   //31个频带
        text=doc.createTextNode(QString::number(eqSpkOne.graphicEQ.gain[band], 'f', 1));
        ElementSegment.appendChild(text);

        ElementEQTypeGraphicEQ.appendChild(ElementSegment);
    }
    ElementSpeaker.appendChild(ElementEQTypeGraphicEQ);

    for(int i = 0; i < 4; i++)
    {
        QDomElement ElementEQTypeBasstreble=doc.createElement(EQTYPE);
        ElementEQTypeBasstreble.setAttribute("name", BASS_TREBLE);
        EleTmp=doc.createElement(BASS);
        text=doc.createTextNode(QString::number(eqSpkOne.bassTreble[i].bass, 'f', 1));
        EleTmp.appendChild(text);
        ElementEQTypeBasstreble.appendChild(EleTmp);

        EleTmp=doc.createElement(TREBLE);
        text=doc.createTextNode(QString::number(eqSpkOne.bassTreble[i].treble, 'f', 1));
        EleTmp.appendChild(text);
        ElementEQTypeBasstreble.appendChild(EleTmp);

        EleTmp=doc.createElement(NHZ);
        text=doc.createTextNode(QString::number(eqSpkOne.bassTreble[i].nHz));
        EleTmp.appendChild(text);
        ElementEQTypeBasstreble.appendChild(EleTmp);
        ElementSpeaker.appendChild(ElementEQTypeBasstreble);
    }

    QDomElement ElementEQTypeOutLevel=doc.createElement(EQTYPE);
    ElementEQTypeOutLevel.setAttribute("name",OUTPUTLEVEL);
    EleTmp=doc.createElement(LEVEL);
    text=doc.createTextNode(QString::number(eqSpkOne.outPutLevel.level, 'f', 1));
    EleTmp.appendChild(text);
    ElementEQTypeOutLevel.appendChild(EleTmp);
    ElementSpeaker.appendChild(ElementEQTypeOutLevel);


    //新增扬声器其他属性
    EleTmp=doc.createElement("SpkName");
    text=doc.createTextNode(QString::fromStdString(eqSpkOne.sSpkName));
    EleTmp.appendChild(text);
    ElementSpeaker.appendChild(EleTmp);

    EleTmp=doc.createElement("SpkType");
    text=doc.createTextNode(QString::fromStdString(eqSpkOne.sSpkType));
    EleTmp.appendChild(text);
    ElementSpeaker.appendChild(EleTmp);

    EleTmp=doc.createElement("SpkDelay");
    text=doc.createTextNode(QString::number(eqSpkOne.spkDelayMs,'f',1));
    EleTmp.appendChild(text);
    ElementSpeaker.appendChild(EleTmp);

    std::string sBoostLimitEnabled=(eqSpkOne.BoostLimitEnabled)?("true"):("false");
    EleTmp=doc.createElement("BoostLimitEnabled");
    text=doc.createTextNode(QString::fromStdString(sBoostLimitEnabled));
    EleTmp.appendChild(text);
    ElementSpeaker.appendChild(EleTmp);

    EleTmp=doc.createElement("BoostLimitLowFreq");
    text=doc.createTextNode(QString::number(eqSpkOne.BoostLimitLowFreq,'f',1));
    EleTmp.appendChild(text);
    ElementSpeaker.appendChild(EleTmp);

    EleTmp=doc.createElement("BoostLimitHighFreq");
    text=doc.createTextNode(QString::number(eqSpkOne.BoostLimitHighFreq,'f',1));
    EleTmp.appendChild(text);
    ElementSpeaker.appendChild(EleTmp);
}


void GetFromDomElement(const QDomElement& EleTmpSpk,LHC::_EQTypeAll& eqTypeTmp)
{
    //QDomElement EleTmpSpk=ListSpeaker.at(i).toElement();
    if (EleTmpSpk.isElement())
    {
        std::wstring sSpkName;
        //LHC::_EQTypeAll eqTypeTmp;  //
        sSpkName=EleTmpSpk.attribute("name").toStdWString();
        eqTypeTmp.sSpkName=WStringToString(sSpkName);
        eqTypeTmp.channel=EleTmpSpk.attribute("channel").toUInt();

        QDomNodeList ListEQType=EleTmpSpk.elementsByTagName(EQTYPE);
        for (int i = 0; i < ListEQType.size(); i++)
        {
            QDomElement  EleTmpType=ListEQType.at(i).toElement();
            if(EleTmpType.attribute("name") == GRAPHICSEQ)
            {
                QDomNodeList  listband = EleTmpType.childNodes();
                for(int k = 0; k < listband.count(); ++k)
                {
                    QDomElement EleTmpband = listband.item(k).toElement();
                    int band=EleTmpband.attribute("band").toUInt();
                    if (band<=NumBand)  //防止越界
                    {
                        eqTypeTmp.graphicEQ.gain[band] = EleTmpband.text().toFloat();
                    }
                }
            }
            else if(EleTmpType.attribute("name") ==BASS_TREBLE)
            {
                QDomElement tt = EleTmpType.firstChildElement(BASS);
                LHC::_bassTreble tbt;
                if(tt.isElement())
                    tbt.bass = tt.text().toFloat();
                else
                    tbt.bass = 0;
                tt = EleTmpType.firstChildElement(TREBLE);
                if(tt.isElement())
                    tbt.treble = tt.text().toFloat();
                else
                    tbt.treble = 0;
                tt = EleTmpType.firstChildElement(NHZ);
                if(tt.isElement())
                    tbt.nHz = tt.text().toUShort();
                else
                    tbt.nHz = 0;

                //防止文件错误导致越界
                if (tbt.nHz<4)
                {
                     eqTypeTmp.bassTreble[tbt.nHz] = tbt;
                }

            }
            else if(EleTmpType.attribute("name") ==OUTPUTLEVEL)
            {
                QDomElement tt = EleTmpType.firstChildElement(LEVEL);
                if(tt.isElement())
                    eqTypeTmp.outPutLevel.level = tt.text().toFloat();
                else
                    eqTypeTmp.outPutLevel.level = 0;
            }
        }


        //新增扬声器其他属性
        QDomElement tt;
        tt=EleTmpSpk.firstChildElement("SpkName");
        if(tt.isElement())
        {
            eqTypeTmp.sSpkName = tt.text().toStdString();
        }
        else
        {
            //eqTypeTmp.sSpkName="";
        }
        tt=EleTmpSpk.firstChildElement("SpkType");
        if(tt.isElement())
        {
            eqTypeTmp.sSpkType = tt.text().toStdString();
        }
        else
        {
            //TODO:为了兼容旧的配置文件没有spktype,根据名字判断类型
            eqTypeTmp.sSpkType="";
        }
        tt=EleTmpSpk.firstChildElement("SpkDelay");
        if(tt.isElement())
        {
            eqTypeTmp.spkDelayMs=tt.text().toFloat();
        }
        else
        {
            eqTypeTmp.spkDelayMs=0;
        }

        tt=EleTmpSpk.firstChildElement("BoostLimitEnabled");
        eqTypeTmp.BoostLimitEnabled=false;
        if(tt.isElement())
        {
            eqTypeTmp.BoostLimitEnabled=tt.text().toStdString()=="true";
        }

        tt=EleTmpSpk.firstChildElement("BoostLimitLowFreq");
        if(tt.isElement())
        {
            eqTypeTmp.BoostLimitLowFreq=tt.text().toFloat();
        }
        else
        {
            eqTypeTmp.BoostLimitLowFreq=0;
        }

        tt=EleTmpSpk.firstChildElement("BoostLimitHighFreq");
        if(tt.isElement())
        {
            eqTypeTmp.BoostLimitHighFreq=tt.text().toFloat();
        }
        else
        {
            eqTypeTmp.BoostLimitHighFreq=0;
        }
    }
}



bool Lhc::Parser_DomToStu(const QDomDocument &doc, LHC::_leonisHoleSoundConfig& lhConfig)
{
    PRINT_FUN_TRACE
    QDomElement ElementRoot = doc.documentElement();
    QDomElement ElementProcessConfig = ElementRoot.firstChildElement(PROCESSORCONFIG);
    QDomElement ElementMetadata = ElementRoot.firstChildElement(METADATA);


    //引用可以大幅缩短单行代码长度，by lizulin 20180425
    LHC::_roomConfiguration& roomConfiguration=lhConfig.processorConfiguration.roomConfiguration;
    LHC::_roomConfiguration::_room& room=lhConfig.processorConfiguration.roomConfiguration.room;
    LHC::_outputConfiguration& outputConfiguration=lhConfig.processorConfiguration.outputConfiguration;
    LHC::_eqConfiguration& eqConfiguration=lhConfig.processorConfiguration.eqConfiguration;
    LHC::_metadata& metadata=lhConfig.metadata;
    LHC::EQCoeffInfo& eqAllInfo=lhConfig.eqALLInfo;
    MainSpeakerCrossoverCoef& mainSCC = lhConfig.mainSpeakerCrossoverCoef;

    //Get metadata from configuration file
    QDomElement EleTmp = ElementMetadata.firstChildElement(_UUID);
    if(EleTmp.isElement())
    {
        metadata.uuid = EleTmp.text().toStdWString();
    }
    else
    {
        metadata.uuid.clear();
    }
    EleTmp = ElementMetadata.firstChildElement(LASTMODIFIEDTIME);
    if (EleTmp.isElement())
    {
        metadata.lastModifiedTime = EleTmp.text().toStdWString();
    }
    else
    {
       metadata.lastModifiedTime.clear();
    }
    EleTmp = ElementMetadata.firstChildElement(LOCALPATHATORIGIN);
    if(EleTmp.isElement())
    {
        metadata.localPathAtOrigin = EleTmp.text().toStdWString();
    }
    else
    {
        metadata.localPathAtOrigin.clear();
    }
    QDomNodeList list = ElementMetadata.elementsByTagName(COMMENTS);
    metadata.commentsVector.clear();
    for(int i = 0; i < list.size(); i++)
    {
        QDomElement t = list.at(i).toElement();
        LHC::_comments comments;
        EleTmp = t.firstChildElement(TEXT_);
        if(EleTmp.isElement())
        {
            comments.text = EleTmp.text().toStdWString();
        }
        else
        {
            comments.text.clear();
        }
        EleTmp = t.firstChildElement(TIME);
        if(EleTmp.isElement())
        {
            comments.time = EleTmp.text().toStdWString();
        }
        else
        {
            comments.time.clear();
        }
        metadata.commentsVector.push_back(comments);
    }

    //room configuration
    QDomElement ElementRoomConfig = ElementProcessConfig.firstChildElement(ROOMCONFIGURATION);
    EleTmp = ElementRoomConfig.firstChildElement(NAME);
    if(EleTmp.isElement())
    {
        roomConfiguration.name = EleTmp.text().toStdWString();
    }
    else
    {
        roomConfiguration.name.clear();
    }
    QDomElement ElementRoomEle = ElementRoomConfig.firstChildElement(ROOM);
    //info
    QDomElement ElementInfo = ElementRoomEle.firstChildElement(INFO);
    EleTmp = ElementInfo.firstChildElement(COMPANY);
    if(EleTmp.isElement())
    {
        room.info.company = EleTmp.text().toStdWString();
    }
    else
    {
        room.info.company.clear();
    }
    EleTmp = ElementInfo.firstChildElement(THEATERNAME);
    if(EleTmp.isElement())
    {
        room.info.theaterName = EleTmp.text().toStdWString();
    }
    else
    {
        room.info.theaterName.clear();
    }
    EleTmp = ElementInfo.firstChildElement(AUDITORIUM);
    if(EleTmp.isElement())
    {
        room.info.auditorium = EleTmp.text().toStdWString();
    }
    else
    {
        room.info.auditorium.clear();
    }
    EleTmp = ElementInfo.firstChildElement(CITY);
    if(EleTmp.isElement())
    {
        room.info.city = EleTmp.text().toStdWString();
    }
    else
    {
        room.info.city.clear();
    }
    EleTmp = ElementInfo.firstChildElement(STATE);
    if(EleTmp.isElement())
    {
        room.info.state = EleTmp.text().toStdWString();
    }
    else
    {
        room.info.state.clear();
    }
    EleTmp = ElementInfo.firstChildElement(COUNTRY);
    if(EleTmp.isElement())
    {
        room.info.country = EleTmp.text().toStdWString();
    }
    else
    {
        room.info.country.clear();
    }
    //dimensions
    ElementInfo = ElementRoomEle.firstChildElement(DIMENSIONS);
    EleTmp = ElementInfo.firstChildElement(SCREENWALLWIDTH);
    if(EleTmp.isElement())
    {
        room.dimensions.screenWallWidth = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.screenWallWidth=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(BOOTHWALLWIDTH);
    if(EleTmp.isElement())
    {
        room.dimensions.boothWallWidth = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.boothWallWidth=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(HOUSELEFTWALLWIDTH);
    if(EleTmp.isElement())
    {
        room.dimensions.houseLeftWallWidth = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.houseLeftWallWidth=0;
    }
    EleTmp = ElementInfo.firstChildElement(HOUSERIGHTWALLWIDTH);
    if(EleTmp.isElement())
    {
        room.dimensions.houseRightWallWidth = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.houseRightWallWidth=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(SCREENHOUSELEFTINTERIORANGLE);
    if(EleTmp.isElement())
    {
        room.dimensions.screenHouseLeftInteriorAngle = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.screenHouseLeftInteriorAngle=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(SCREENHOUSERIGHTINTERIORANGLE);
    if(EleTmp.isElement())
    {
        room.dimensions.screenHouseRightInteriorAngle = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.screenHouseRightInteriorAngle=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(BOOTHHOUSELEFTINTERIORANGLE);
    if(EleTmp.isElement())
    {
        room.dimensions.boothHouseLeftInteriorAngle = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.boothHouseLeftInteriorAngle=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(BOOTHHOUSERIGHTINTERIORANGLE);
    if(EleTmp.isElement())
    {
        room.dimensions.boothHouseRightInteriorAngle = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.boothHouseRightInteriorAngle=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(FLOORELEVATIONATSCREEN);
    if(EleTmp.isElement())
    {
        room.dimensions.floorElevationAtScreen = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.floorElevationAtScreen=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(CEILINGELEVATIONATSCREEN);
    if(EleTmp.isElement())
    {
        room.dimensions.ceilingElevationAtScreen = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.ceilingElevationAtScreen=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(FLOORELEVATIONATTWOTHIRDS);
    if(EleTmp.isElement())
    {
        room.dimensions.floorElevationAtTwoThirds = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.floorElevationAtTwoThirds=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(CEILINGELEVATIONATTWOTHIRDS);
    if(EleTmp.isElement())
    {
        room.dimensions.ceilingElevationAtTwoThirds = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.ceilingElevationAtTwoThirds=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(FLOORELEVATIONATBOOTH);
    if(EleTmp.isElement())
    {
        room.dimensions.floorElevationAtBooth = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.floorElevationAtBooth=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(CEILINGELEVATIONATBOOTH);
    if(EleTmp.isElement())
    {
        room.dimensions.ceilingElevationAtBooth = EleTmp.text().toFloat();
    }
    else
    {
        room.dimensions.ceilingElevationAtBooth=0.0;
    }
    //flatscreen
    ElementInfo = ElementRoomEle.firstChildElement(FLATSCREEN);
    EleTmp = ElementInfo.firstChildElement(ROOM_WIDTH);
    if(EleTmp.isElement())
    {
        room.flatScreen.width = EleTmp.text().toFloat();
    }
    else
    {
        room.flatScreen.width=0.0f;
    }
    EleTmp = ElementInfo.firstChildElement(ROOM_HEIGHT);
    if(EleTmp.isElement())
    {
         room.flatScreen.height = EleTmp.text().toFloat();
    }
    else
    {
        room.flatScreen.height=0.0f;
    }
    QDomElement EleT = ElementInfo.firstChildElement(DISTANCETOLEFTWALL);
    EleTmp = EleT.firstChildElement(POS_X);
    if(EleTmp.isElement())
    {
        room.flatScreen.distanceToLeftWall.x = EleTmp.text().toFloat();
    }
    else
    {
        room.flatScreen.distanceToLeftWall.x=0.0f;
    }
    EleTmp = EleT.firstChildElement(POS_Y);
    if(EleTmp.isElement())
    {
        room.flatScreen.distanceToLeftWall.y = EleTmp.text().toFloat();
    }
    else
    {
         room.flatScreen.distanceToLeftWall.y=0.0f;
    }
    EleTmp = EleT.firstChildElement(POS_Z);
    if(EleTmp.isElement())
    {
        room.flatScreen.distanceToLeftWall.z = EleTmp.text().toFloat();
    }
    else
    {
        room.flatScreen.distanceToLeftWall.z=0;
    }
    //scopeScreen
    ElementInfo = ElementRoomEle.firstChildElement(SCOPESCREEN);
    EleTmp = ElementInfo.firstChildElement(ROOM_WIDTH);
    if(EleTmp.isElement())
    {
        room.scopeScreen.width = EleTmp.text().toFloat();
    }
    else
    {
        room.scopeScreen.width=0;
    }
    EleTmp = ElementInfo.firstChildElement(ROOM_HEIGHT);
    if(EleTmp.isElement())
    {
        room.scopeScreen.height = EleTmp.text().toFloat();
    }
    else
    {
        room.scopeScreen.height=0;
    }
    EleT = ElementInfo.firstChildElement(DISTANCETOLEFTWALL);
    EleTmp = EleT.firstChildElement(POS_X);
    if(EleTmp.isElement())
    {
        room.scopeScreen.distanceToLeftWall.x = EleTmp.text().toFloat();
    }
    else
    {
        room.scopeScreen.distanceToLeftWall.x=0;
    }
    EleTmp = EleT.firstChildElement(POS_Y);
    if(EleTmp.isElement())
    {
        room.scopeScreen.distanceToLeftWall.y = EleTmp.text().toFloat();
    }
    else
    {
        room.scopeScreen.distanceToLeftWall.y=0;
    }
    EleTmp = EleT.firstChildElement(POS_Z);
    if(EleTmp.isElement())
    {
        room.scopeScreen.distanceToLeftWall.z = EleTmp.text().toFloat();
    }
    else
    {
        room.scopeScreen.distanceToLeftWall.z=0;
    }
    //region
    QDomElement ElementRegion = ElementRoomEle.firstChildElement(REGION);
    EleTmp = ElementRegion.firstChildElement(NAME);
    if(EleTmp.isElement())
       room.region.name = EleTmp.text().toStdWString();
    else
        room.region.name.clear();
    //dimensions
    ElementInfo = ElementRegion.firstChildElement(DIMENSIONS);
    EleTmp = ElementInfo.firstChildElement(SCREENWALLWIDTH);
    if(EleTmp.isElement())
    {
       room.region.dimensions.screenWallWidth = EleTmp.text().toFloat();
    }
    else
    {
        room.region.dimensions.screenWallWidth=0;
    }
    EleTmp = ElementInfo.firstChildElement(BOOTHWALLWIDTH);
    if(EleTmp.isElement())
    {
        room.region.dimensions.boothWallWidth = EleTmp.text().toFloat();
    }
    else
    {
        room.region.dimensions.boothWallWidth=0;
    }
    EleTmp = ElementInfo.firstChildElement(HOUSELEFTWALLWIDTH);
    if(EleTmp.isElement())
    {
       room.region.dimensions.houseLeftWallWidth = EleTmp.text().toFloat();
    }
    else
    {
        room.region.dimensions.houseLeftWallWidth=0;
    }
    EleTmp = ElementInfo.firstChildElement(HOUSERIGHTWALLWIDTH);
    if(EleTmp.isElement())
    {
        room.region.dimensions.houseRightWallWidth = EleTmp.text().toFloat();
    }
    else
    {
        room.region.dimensions.houseRightWallWidth=0;
    }
    EleTmp = ElementInfo.firstChildElement(SCREENHOUSELEFTINTERIORANGLE);
    if(EleTmp.isElement())
    {
        room.region.dimensions.screenHouseLeftInteriorAngle = EleTmp.text().toFloat();
    }
    else
    {
        room.region.dimensions.screenHouseLeftInteriorAngle=0;
    }
    EleTmp = ElementInfo.firstChildElement(SCREENHOUSERIGHTINTERIORANGLE);
    if(EleTmp.isElement())
    {
        room.region.dimensions.screenHouseRightInteriorAngle = EleTmp.text().toFloat();
    }
    else
    {
       room.region.dimensions.screenHouseRightInteriorAngle=0;
    }
    EleTmp = ElementInfo.firstChildElement(BOOTHHOUSELEFTINTERIORANGLE);
    if(EleTmp.isElement())
    {
        room.region.dimensions.boothHouseLeftInteriorAngle = EleTmp.text().toFloat();
    }
    else
    {
        room.region.dimensions.boothHouseLeftInteriorAngle=0;
    }
    EleTmp = ElementInfo.firstChildElement(BOOTHHOUSERIGHTINTERIORANGLE);
    if(EleTmp.isElement())
    {
       room.region.dimensions.boothHouseRightInteriorAngle = EleTmp.text().toFloat();
    }
    else
    {
        room.region.dimensions.boothHouseRightInteriorAngle=0;
    }
    EleTmp = ElementInfo.firstChildElement(FLOORELEVATIONATSCREEN);
    if(EleTmp.isElement())
    {
        room.region.dimensions.floorElevationAtScreen = EleTmp.text().toFloat();
    }
    else
    {
        room.region.dimensions.floorElevationAtScreen=0;
    }
    EleTmp = ElementInfo.firstChildElement(CEILINGELEVATIONATSCREEN);
    if(EleTmp.isElement())
    {
        room.region.dimensions.ceilingElevationAtScreen = EleTmp.text().toFloat();
    }
    else
    {
        room.region.dimensions.ceilingElevationAtScreen=0;
    }
    //speakerEndpoint
    list = ElementRegion.elementsByTagName(SPEAKERENDPOINT);
    room.region.speakerEndpointVector.clear();
    for (int i = 0; i < list.size(); i++)
    {
        LHC::_speakerEndpoint speakerEndpoint;
        EleT = list.at(i).toElement();

        //TODO:扬声器属性类型解析失败 待续
        speakerEndpoint.type = EleT.attribute(TYPE).toStdWString();   //error
        //speakerEndpoint.type = EleT.attribute("xsi:"TYPE).toStdWString();   //error
        //speakerEndpoint.type = EleT.attribute("xsi:type").toStdWString();   //error
          QString tmp=EleT.attributeNode("xsi:type").value();
          QString tmp2=EleT.attributeNode("type").value();
           //QString tmp3=EleT.attributeNode("xsi:").value();
         QString tmp3=EleT.text();
         PreDprintf("s1:%s s2:%s s3:%s\n",tmp.toStdString().c_str(),tmp2.toStdString().c_str(),tmp3.toStdString().c_str());


        std::wstring wsid=EleT.attribute(SPK_ID).toStdWString();
        wsid=wsid.erase(0,strlen("SPKREP-"));
        //int id=_wtoi(wsid.c_str());
        int id=WStringToInt(wsid.c_str());

        speakerEndpoint.id =id;   //从文件读取到的字符串去掉SPKREP-前缀

        EleTmp = EleT.firstChildElement(NAME);
        if(EleTmp.isElement())
        {
            speakerEndpoint.name = EleTmp.text().toStdWString();
        }
        else
        {
            speakerEndpoint.name.clear();
        }


        //统一各处实现
        speakerEndpoint.type=SpkNameToSpkType(speakerEndpoint.name,speakerEndpoint.type);

//#if 0 //已经解决,屏蔽 by lizulin 20200608
//        ////////////////////////////////////////////////////////////////////////////
//        // 现在版本的CONFIG文件中的SpeakerEndpoint不知为什么没有type属性了。下面打个补丁 [6/3/2020 jaontolt]
//        if(speakerEndpoint.type.empty()||speakerEndpoint.type==L"")
//        {
//            if(wcsstr(speakerEndpoint.name.c_str(), L"Sub ") != NULL
//                ||wcsstr(speakerEndpoint.name.c_str(), L"LFE "))
//            {
//                speakerEndpoint.type =SPKTYPE_SUBWOOFER_WS;
//            }
//            else if(wcsstr(speakerEndpoint.name.c_str(), L"L ") != NULL 
//                ||wcsstr(speakerEndpoint.name.c_str(), L"R ") != NULL 
//                ||wcsstr(speakerEndpoint.name.c_str(), L"C ") != NULL)
//               // ||wcsstr(speakerEndpoint.name.c_str(), L"LFE ") != NULL)  //参考dolby的分类LFE 不属于主扬声器
//            {
//                speakerEndpoint.type =SPKTYPE_SCREEN_WS;
//            }
//            else
//            {
//                speakerEndpoint.type=SPKTYPE_SURROUND_WS;
//            }
//        }
//        ////////////////////////////////////////////////////////////////////////////
//#endif


        ElementInfo = EleT.firstChildElement(POSITION);
        EleTmp = ElementInfo.firstChildElement(POS_X);
        if(EleTmp.isElement())
        {
            speakerEndpoint.position.x=EleTmp.text().toFloat();
        }
        else
        {
            speakerEndpoint.position.x=0.0f;
        }
        EleTmp = ElementInfo.firstChildElement(POS_Y);
        if(EleTmp.isElement())
        {
            speakerEndpoint.position.y = EleTmp.text().toFloat();
        }
        else
        {
            speakerEndpoint.position.y=0.0f;
        }
        EleTmp = ElementInfo.firstChildElement(POS_Z);
        if(EleTmp.isElement())
        {
            speakerEndpoint.position.z = EleTmp.text().toFloat();
        }
        else
        {
            speakerEndpoint.position.z=0.0f;
        }
        EleTmp = EleT.firstChildElement(DELAY);
        if(EleTmp.isElement())
        {
            speakerEndpoint.delay = EleTmp.text().toFloat();
        }
        else
        {
            speakerEndpoint.delay=0.0f;
        }

        QDomNodeList l = EleT.elementsByTagName(ARRAY);
        for(int j = 0; j < l.size(); j++)
        {
            EleTmp = l.at(j).toElement();
            if(EleTmp.isElement())
            {
                std::wstring ar = EleTmp.text().toStdWString();
                speakerEndpoint.arrayVector.push_back(ar);
            }
        }

        l = EleT.elementsByTagName(BASSMANAGEMENT);
        for(int j = 0; j < l.size(); j++)
        {
            LHC::_bassManagement bm;
            ElementInfo = l.at(j).toElement();
            EleTmp = ElementInfo.firstChildElement(SUBWOOFERREF);
            if(EleTmp.isElement())
            {
                bm.subwooferRef = EleTmp.text().toStdWString();
            }
            else
            {
                bm.subwooferRef.clear();
            }
            EleTmp = ElementInfo.firstChildElement(CROSSOVERFREQUENCYHZ);
            if(EleTmp.isElement())
            {
                bm.crossoverFrequencyHz = EleTmp.text().toInt();
            }
            else
            {
                bm.crossoverFrequencyHz=0;
            }
            EleTmp = ElementInfo.firstChildElement(GAINDB);
            if(EleTmp.isElement())
            {
                bm.gainDb = EleTmp.text().toFloat();
            }
            else
            {
                bm.gainDb=0;
            }
            EleTmp = EleT.firstChildElement(SPECSREF);
            if(EleTmp.isElement())
            {
                speakerEndpoint.specsRef = EleTmp.text().toStdWString();
            }
            else
            {
                speakerEndpoint.specsRef.clear();
            }
            speakerEndpoint.bassManagementVector.push_back(bm);
        }

        EleTmp = EleT.firstChildElement(SPK_DEVICE_NAME);
        if(EleTmp.isElement())
        {
            speakerEndpoint.sSpkDeviceName = EleTmp.text().toStdWString();
        }
        else
        {
            speakerEndpoint.sSpkDeviceName.clear();
        }
        EleTmp = EleT.firstChildElement(AMP_DEVICE_NAME);
        if(EleTmp.isElement())
        {
            speakerEndpoint.sAmpDeviceName= EleTmp.text().toStdWString();
        }
        else
        {
            speakerEndpoint.sAmpDeviceName.clear();
        }
        room.region.speakerEndpointVector.push_back(speakerEndpoint);
    }
    //speakerSpecs
    roomConfiguration.speakerSpecsVector.clear();
    list = ElementRoomConfig.elementsByTagName(SPEAKERSPECS);
    for (int i = 0; i < list.size(); i++)
    {
        LHC::_speakerSpecs speakerSpecs;
        EleT = list.at(i).toElement();
        speakerSpecs.id = EleT.attribute(SPK_ID).toStdWString();
        speakerSpecs.type = EleT.attribute(TYPE).toStdWString();
        EleTmp = EleT.firstChildElement(MANUFACTURER);
        if(EleTmp.isElement())
        {
            speakerSpecs.manufacturer = EleTmp.text().toStdWString();
        }
        else
        {
            speakerSpecs.manufacturer.clear();
        }
        EleTmp = EleT.firstChildElement(MODEL);
        if(EleTmp.isElement())
        {
            speakerSpecs.model = EleTmp.text().toStdWString();
        }
        else
        {
            speakerSpecs.model.clear();
        }
        EleTmp = EleT.firstChildElement(FREQUENCYRESPONSEMINHZ);
        if(EleTmp.isElement())
        {
            speakerSpecs.frequencyResponseMinHz = EleTmp.text().toInt();
        }
        else
        {
            speakerSpecs.frequencyResponseMinHz=0;
        }
        EleTmp = EleT.firstChildElement(FREQUENCYRESPONSEMAXHZ);
        if(EleTmp.isElement())
        {
            speakerSpecs.frequencyResponseMaxHz = EleTmp.text().toInt();
        }
        else
        {
            speakerSpecs.frequencyResponseMaxHz=0;
        }
        EleTmp = EleT.firstChildElement(CROSSOVERFREQUENCYHZ);
        if(EleTmp.isElement())
        {
            speakerSpecs.crossoverFrequencyHz = EleTmp.text().toInt();
             //PreDprintf("speakerSpecs.crossoverFrequencyHz=%d\n",speakerSpecs.crossoverFrequencyHz);
        }
        else
        {
            speakerSpecs.crossoverFrequencyHz=0;
            //PreDprintf("speakerSpecs.crossoverFrequencyHz=%d\n",speakerSpecs.crossoverFrequencyHz);
        }
        EleTmp = EleT.firstChildElement(PEAKINPUTDBUPK);
        if(EleTmp.isElement())
        {
            speakerSpecs.peakInputDbuPk = EleTmp.text().toInt();
        }
        else
        {
            speakerSpecs.peakInputDbuPk=0;
        }
        roomConfiguration.speakerSpecsVector.push_back(speakerSpecs);
    }
    //eqConfiguration
    eqConfiguration.arrayEqVector.clear();
    QDomElement ElementEqConfig = ElementProcessConfig.firstChildElement(EQCONFIGURATION);
    eqConfiguration.isEqEnabled = ElementEqConfig.attribute(ISEQENABLED).toStdWString();
    list = ElementEqConfig.elementsByTagName(ARRAYEQ);
    for(int i = 0; i < list.size(); i++)
    {
        LHC::_arrayEq arrayEq;
        EleT = list.at(i).toElement();
        EleTmp = EleT.firstChildElement(DELAYMS);
        if(EleTmp.isElement())
        {
            arrayEq.delayMs = EleTmp.text().toFloat();
        }
        else
        {
            arrayEq.delayMs=0;
        }
        EleTmp = EleT.firstChildElement(GAINDB);
        if(EleTmp.isElement())
        {
            arrayEq.gainDb = EleTmp.text().toFloat();
        }
        else
        {
            arrayEq.gainDb=0;
        }
        EleTmp = EleT.firstChildElement(USERGAINDB);
        if(EleTmp.isElement())
        {
            arrayEq.userGainDb = EleTmp.text().toFloat();
        }
        else
        {
            arrayEq.userGainDb=0;
        }
        EleTmp = EleT.firstChildElement(EQRESPONSECURVE);
        QDomNodeList l = EleTmp.elementsByTagName(RESPONSEPOINT);
        for (int j = 0; j < l.size(); j++)
        {
            LHC::_responsePoint responsePoint;
            ElementInfo = l.at(j).toElement();
            EleTmp = ElementInfo.firstChildElement(GAINDB);
            if(EleTmp.isElement())
            {
                responsePoint.gainDb = EleTmp.text().toFloat();
            }
            else
            {
                responsePoint.gainDb=0;
            }
            EleTmp = ElementInfo.firstChildElement(FREQUENCYHZ);
            if(EleTmp.isElement())
            {
                responsePoint.frequencyHz = EleTmp.text().toFloat();
            }
            else
            {
                responsePoint.frequencyHz=0;
            }
            arrayEq.eqResponseCurveVector.push_back(responsePoint);
        }
        ElementInfo = EleT.firstChildElement(TARGETEQRESPONSECURVE);
        EleTmp = ElementInfo.firstChildElement(USEDEFAULTXCURVE);
        if(EleTmp.isElement())
        {
            arrayEq.targetEqResponseCurve.useDefaultXCurve = EleTmp.text().toStdWString();
        }
        else
        {
            arrayEq.targetEqResponseCurve.useDefaultXCurve.clear();
        }
        EleTmp = ElementInfo.firstChildElement(LOWFREQBOOSTLIMITHZ);
        if(EleTmp.isElement())
        {
            arrayEq.targetEqResponseCurve.lowFreqBoostLimitHz = EleTmp.text().toInt();
        }
        else
        {
            arrayEq.targetEqResponseCurve.lowFreqBoostLimitHz=0;
        }
        EleTmp = ElementInfo.firstChildElement(HIGHFREQBOOSTLIMITHZ);
        if(EleTmp.isElement())
        {
            arrayEq.targetEqResponseCurve.highFreqBoostLimitHz = EleTmp.text().toInt();
        }
        else
        {
            arrayEq.targetEqResponseCurve.highFreqBoostLimitHz=0;
        }
        ElementInfo = ElementInfo.firstChildElement(XCURVEPARAMETERS);
        EleTmp = ElementInfo.firstChildElement(FREQCORNERAHZ);
        if(EleTmp.isElement())
        {
            arrayEq.targetEqResponseCurve.xCurveParameters.freqCornerAHz = EleTmp.text().toInt();
        }
        else
        {
            arrayEq.targetEqResponseCurve.xCurveParameters.freqCornerAHz=0;
        }
        EleTmp = ElementInfo.firstChildElement(FREQCORNERBHZ);
        if(EleTmp.isElement())
        {
            arrayEq.targetEqResponseCurve.xCurveParameters.freqCornerBHz = EleTmp.text().toInt();
        }
        else
        {
            arrayEq.targetEqResponseCurve.xCurveParameters.freqCornerBHz=0;
        }
        EleTmp = ElementInfo.firstChildElement(SLOPEADBPEROCT);
        if(EleTmp.isElement())
        {
            arrayEq.targetEqResponseCurve.xCurveParameters.slopeAdBPerOct = EleTmp.text().toInt();
        }
        else
        {
            arrayEq.targetEqResponseCurve.xCurveParameters.slopeAdBPerOct=0;
        }
        EleTmp = ElementInfo.firstChildElement(SLOPEBDBPEROCT);
        if(EleTmp.isElement())
        {
            arrayEq.targetEqResponseCurve.xCurveParameters.slopeBdBPerOct = EleTmp.text().toInt();
        }
        else
        {
            arrayEq.targetEqResponseCurve.xCurveParameters.slopeBdBPerOct=0;
        }
        EleTmp = EleT.firstChildElement(ARRAY);
        if(EleTmp.isElement())
        {
            arrayEq.array = EleTmp.text().toStdWString();
        }
        else
        {
            arrayEq.array.clear();
        }
       eqConfiguration.arrayEqVector.push_back(arrayEq);
    }
    //speakerEq
    eqConfiguration.speakerEqVector.clear();
    list = ElementEqConfig.elementsByTagName(SPEAKEREQ);
    for(int i = 0; i < list.size(); i++)
    {
        LHC::_speakerEq speakerEq;
        EleT = list.at(i).toElement();
        EleTmp = EleT.firstChildElement(DELAYMS);
        if(EleTmp.isElement())
        {
            speakerEq.delayMs = EleTmp.text().toFloat();
        }
        else
        {
            speakerEq.delayMs=0;
        }
        EleTmp = EleT.firstChildElement(GAINDB);
        if(EleTmp.isElement())
        {
            speakerEq.gainDb = EleTmp.text().toFloat();
        }
        else
        {
            speakerEq.gainDb=0;
        }
        EleTmp = EleT.firstChildElement(USERGAINDB);
        if(EleTmp.isElement())
        {
            speakerEq.userGainDb = EleTmp.text().toFloat();
        }
        else
        {
            speakerEq.userGainDb=0;
        }
        ElementInfo = EleT.firstChildElement(EQRESPONSECURVE);
        QDomNodeList l = ElementInfo.elementsByTagName(RESPONSEPOINT);
        for(int j = 0; j < l.size(); j++)
        {
            ElementInfo = l.at(j).toElement();
            LHC::_responsePoint responsePoint;
            EleTmp = ElementInfo.firstChildElement(GAINDB);
            if(EleTmp.isElement())
            {
                responsePoint.gainDb = EleTmp.text().toFloat();
            }
            else
            {
                responsePoint.gainDb=0;
            }
            EleTmp = ElementInfo.firstChildElement(FREQUENCYHZ);
            if(EleTmp.isElement())
            {
                responsePoint.frequencyHz = EleTmp.text().toFloat();
            }
            else
            {
                responsePoint.frequencyHz=0;
            }
            speakerEq.eqResponseCurveVector.push_back(responsePoint);
        }
        ElementInfo = EleT.firstChildElement(TARGETEQRESPONSECURVE);
        EleTmp = ElementInfo.firstChildElement(USEDEFAULTXCURVE);
        if(EleTmp.isElement())
        {
            speakerEq.targetEqResponseCurve.useDefaultXCurve = EleTmp.text().toStdWString();
        }
        else
        {
            speakerEq.targetEqResponseCurve.useDefaultXCurve.clear();
        }
        EleTmp = ElementInfo.firstChildElement(LOWFREQBOOSTLIMITHZ);
        if(EleTmp.isElement())
        {
            speakerEq.targetEqResponseCurve.lowFreqBoostLimitHz = EleTmp.text().toInt();
        }
        else
        {
            speakerEq.targetEqResponseCurve.lowFreqBoostLimitHz=0;
        }
        EleTmp = ElementInfo.firstChildElement(HIGHFREQBOOSTLIMITHZ);
        if(EleTmp.isElement())
        {
            speakerEq.targetEqResponseCurve.highFreqBoostLimitHz = EleTmp.text().toInt();
        }
        else
        {
            speakerEq.targetEqResponseCurve.highFreqBoostLimitHz=0;
        }
        ElementInfo = ElementInfo.firstChildElement(XCURVEPARAMETERS);
        EleTmp = ElementInfo.firstChildElement(FREQCORNERAHZ);
        if(EleTmp.isElement())
        {
            speakerEq.targetEqResponseCurve.xCurveParameters.freqCornerAHz = EleTmp.text().toInt();
        }
        else
        {
            speakerEq.targetEqResponseCurve.xCurveParameters.freqCornerAHz=0;
        }
        EleTmp = ElementInfo.firstChildElement(FREQCORNERBHZ);
        if(EleTmp.isElement())
        {
            speakerEq.targetEqResponseCurve.xCurveParameters.freqCornerBHz = EleTmp.text().toInt();
        }
        else
        {
            speakerEq.targetEqResponseCurve.xCurveParameters.freqCornerBHz=0;
        }
        EleTmp = ElementInfo.firstChildElement(SLOPEADBPEROCT);
        if(EleTmp.isElement())
        {
            speakerEq.targetEqResponseCurve.xCurveParameters.slopeAdBPerOct = EleTmp.text().toInt();
        }
        else
        {
            speakerEq.targetEqResponseCurve.xCurveParameters.slopeAdBPerOct=0;
        }
        EleTmp = ElementInfo.firstChildElement(SLOPEBDBPEROCT);
        if(EleTmp.isElement())
        {
            speakerEq.targetEqResponseCurve.xCurveParameters.slopeBdBPerOct = EleTmp.text().toInt();
        }
        else
        {
            speakerEq.targetEqResponseCurve.xCurveParameters.slopeBdBPerOct=0;
        }
        EleTmp = EleT.firstChildElement(SPEAKERENDPOINTREF);
        if(EleTmp.isElement())
        {
            speakerEq.speakerEndpointRef = EleTmp.text().toStdWString();
        }
        else
        {
            speakerEq.speakerEndpointRef.clear();
        }
        eqConfiguration.speakerEqVector.push_back(speakerEq);
    }


    // Add CrossOver configuration [6/19/2020 jaontolt]
    QDomElement ElementCrossOverConfig = ElementProcessConfig.firstChildElement("CrossOverConfiguration");
    if(ElementCrossOverConfig.isElement())
    {
        if(ElementCrossOverConfig.attribute("isEnabledCrossOver") == "true")
            mainSCC.EnableCrossover = true;
        else
            mainSCC.EnableCrossover = false;
        mainSCC.co_speaker.clear();
        list = ElementCrossOverConfig.elementsByTagName("Speaker");
        {
            for (int i = 0; i < list.size(); i++)
            {
                _SPK_PARAM param;
                EleT = list.at(i).toElement();
                EleTmp = EleT.firstChildElement("Cnt");
                if (EleTmp.isElement())
                {
                    param.co_cnt = EleTmp.text().toUInt();
                }
                EleTmp = EleT.firstChildElement("In");
                if (EleTmp.isElement())
                {
                    param.in = EleTmp.text().toUInt();
                }
                EleTmp = EleT.firstChildElement("OutL");
                if (EleTmp.isElement())
                {
                    param.out_L = EleTmp.text().toUInt();
                }
                EleTmp = EleT.firstChildElement("OutM");
                if (EleTmp.isElement())
                {
                    param.out_M= EleTmp.text().toUInt();
                }
                EleTmp = EleT.firstChildElement("OutH");
                if (EleTmp.isElement())
                {
                    param.out_H = EleTmp.text().toUInt();
                }
                EleTmp = EleT.firstChildElement("Lpf");
                if (EleTmp.isElement())
                {
                    param.Lpf = EleTmp.text().toFloat();
                }
                EleTmp = EleT.firstChildElement("Hpf");
                if (EleTmp.isElement())
                {
                    param.Hpf = EleTmp.text().toFloat();
                }
                mainSCC.co_speaker.push_back(param);
            }
        }
    }

    // Add Channel Mapping Configuration [6/20/2020 jaontolt]
    QDomElement ElementChannelMapping = ElementProcessConfig.firstChildElement("ChannelMappingConfiguration");
    if(ElementChannelMapping.isElement())
    {
        QDomNodeList tList = ElementChannelMapping.elementsByTagName("Template");
        int tt = 8;
        if(tList.size() < 8)
            tt = tList.size();
        for (int n = 0; n < tt; n++)
        {
            LHC::_channelMappingConfiguration &channelMapping = lhConfig.channelMappingConfiguration[n];
            QDomElement tEle = tList.at(n).toElement();
            int sz = 16;
            if(tEle.attribute("Name").toStdString().size() < 16)
                sz = tEle.attribute("Name").toStdString().size();
            memset(channelMapping.templateName, 0, 16);
            strncpy(channelMapping.templateName, tEle.attribute("Name").toStdString().c_str(), sz);
            list = tEle.elementsByTagName("Channel");
            {
                for (int i = 0; i < list.size(); i++)
                {
                    EleT = list.at(i).toElement();
                    int input;
                    int mapto;
                    EleTmp = EleT.firstChildElement("Input");
                    if (EleTmp.isElement())
                    {
                        input = EleTmp.text().toUInt();
                        EleTmp = EleT.firstChildElement("MapTo");
                        if(EleTmp.isElement())
                        {
                            mapto = EleTmp.text().toUInt();
                            channelMapping.mapping[input] = mapto;
                        }
                    }
                }
            }
        }
    }


    //outputConfiguration
    QDomElement ElementOutputConfig = ElementProcessConfig.firstChildElement(OUTPUTCONFIGURATION);
    outputConfiguration.isLimiterEnabled = ElementOutputConfig.attribute(ISLIMITERENABLED).toStdWString();
    outputConfiguration.type = ElementOutputConfig.attribute(TYPE).toStdWString();
#ifdef ENABLE_AMPLIFIER
    outputConfiguration.amplifierVector.clear();
    list = ElementOutputConfig.elementsByTagName(AMPLIFIER);
    for(int i = 0; i < list.size(); i++)
    {
        EleT = list.at(i).toElement();
        LHC::_amplifier amplifier;
        amplifier.id = EleT.attribute(SPK_ID).toStdWString();
        EleTmp = EleT.firstChildElement(VOLTAGEGAINDB);
        if(EleTmp.isElement())
        {
            amplifier.voltageGainDb = EleTmp.text().toStdWString();
        }
        else
        {
            amplifier.voltageGainDb.clear();
        }
        EleTmp = EleT.firstChildElement(PEAKVOLTAGE);
        if(EleTmp.isElement())
        {
            amplifier.peakVoltage = EleTmp.text().toStdWString();
        }
        else
        {
            amplifier.peakVoltage.clear();
        }
        QDomNodeList l = EleT.elementsByTagName(AMPPERCHANNELSPECS);
        for(int j = 0; j < l.size(); j++)
        {
            LHC::_ampPerChannelSpecs ampPerChannelSpecs;
            ElementInfo = l.at(j).toElement();
            EleTmp = ElementInfo.firstChildElement(IMPEDANCEOHM);
            if(EleTmp.isElement())
            {
                ampPerChannelSpecs.impedanceOhm = EleTmp.text().toInt();
            }
            else
            {
                ampPerChannelSpecs.impedanceOhm=0;
            }
            EleTmp = ElementInfo.firstChildElement(POWERWATTS);
            if(EleTmp.isElement())
            {
                ampPerChannelSpecs.powerWatts = EleTmp.text().toInt();
            }
            else
            {
                ampPerChannelSpecs.powerWatts=0;
            }
            amplifier.ampPerChannelSpecsVector.push_back(ampPerChannelSpecs);
        }
        EleTmp = EleT.firstChildElement(MANUFACTURER);
        if(EleTmp.isElement())
        {
            amplifier.manufacturer = EleTmp.text().toStdWString();
        }
        else
        {
            amplifier.manufacturer.clear();
        }
        EleTmp = EleT.firstChildElement(MODEL);
        if(EleTmp.isElement())
        {
            amplifier.model = EleTmp.text().toStdWString();
        }
        else
        {
            amplifier.model.clear();
        }
       outputConfiguration.amplifierVector.push_back(amplifier);
    }
#endif // AMPLIFIER

    outputConfiguration.outputRouteVector.clear();
    list = ElementOutputConfig.elementsByTagName(OUTPUTROUTE);
    for(int i = 0; i < list.size(); i++)
    {
        LHC::_outputRoute outputRoute;
        EleT = list.at(i).toElement();
        EleTmp = EleT.firstChildElement(OUTPUTREF);
        if(EleTmp.isElement())
        {
            //outputRoute.outputRef = EleTmp.text().toStdWString();

            std::wstring wsoutputRef=EleTmp.text().toStdWString();
            wsoutputRef=wsoutputRef.erase(0,strlen("OUT-BL-"));   //从文件读取到的字符串去掉OUT-BL-前缀 再读取声道号
            //outputRoute.outputRef =_wtoi(wsoutputRef.c_str());
            outputRoute.outputRef =WStringToInt(wsoutputRef.c_str());

        }
        else
        {
            outputRoute.outputRef=-1;   
        }

        EleTmp = EleT.firstChildElement(SPEAKERENDPOINTREF);
        if(EleTmp.isElement())
        {
            //outputRoute.speakerEndpointRef = EleTmp.text().toStdWString();

            std::wstring wsspeakerEndpointRef=EleTmp.text().toStdWString();
            wsspeakerEndpointRef=wsspeakerEndpointRef.erase(0,strlen("SPKREP-"));   //从文件读取到的字符串去掉SPKREP-前缀 再读取声道号
            //outputRoute.speakerEndpointRef =_wtoi(wsspeakerEndpointRef.c_str());
            outputRoute.speakerEndpointRef =WStringToInt(wsspeakerEndpointRef.c_str());

        }
        else
        {
            outputRoute.speakerEndpointRef=-1;
        }
        EleTmp = EleT.firstChildElement(AMPLIFIERREF);
        if(EleTmp.isElement())
        {
            outputRoute.amplifierRef = EleTmp.text().toStdWString();
        }
        else
        {
            outputRoute.amplifierRef.clear();
        }
        EleTmp = EleT.firstChildElement(AMPLIFIERTRIMDB);
        if(EleTmp.isElement())
        {
            outputRoute.amplifierTrimDb = EleTmp.text().toInt();
        }
        else
        {
            outputRoute.amplifierTrimDb=0;
        }
        EleTmp = EleT.firstChildElement(LIMITERTYPE);
        if(EleTmp.isElement())
        {
            outputRoute.limiterType = EleTmp.text().toStdWString();
        }
        else
        {
            outputRoute.limiterType.clear();
        }
        EleTmp = EleT.firstChildElement(SPEAKERPROCESSORREF);
        if(EleTmp.isElement())
        {
            outputRoute.speakerProcessorRef = EleTmp.text().toStdWString();
        }
        else
        {
            outputRoute.speakerProcessorRef.clear();
        }
        outputConfiguration.outputRouteVector.push_back(outputRoute);
    }
#ifdef ENABLE_AMPLIFIER
   outputConfiguration.speakerProcessorVector.clear();
    list = ElementOutputConfig.elementsByTagName(SPEAKERPROCESSOR);
    for (int i = 0; i < list.size(); i++)
    {
        EleT = list.at(i).toElement();
        LHC::_speakerProcessor speakerProcessor;
        speakerProcessor.id = EleT.attribute(SPK_ID).toStdWString();
        EleTmp = EleT.firstChildElement(MANUFACTURER);
        if(EleTmp.isElement())
        {
            speakerProcessor.manufacturer = EleTmp.text().toStdWString();
        }
        else
        {
            speakerProcessor.manufacturer.clear();
        }
        EleTmp = EleT.firstChildElement(MODEL);
        if(EleTmp.isElement())
        {
            speakerProcessor.model = EleTmp.text().toStdWString();
        }
        else
        {
            speakerProcessor.model.clear();
        }
        EleTmp = EleT.firstChildElement(PEAKINPUTDBUPK);
        if(EleTmp.isElement())
        {
            speakerProcessor.peakInputDbuPk = EleTmp.text().toStdWString();
        }
        else
        {
            speakerProcessor.peakInputDbuPk.clear();
        }
        outputConfiguration.speakerProcessorVector.push_back(speakerProcessor);
    }
#endif // ENABLE_AMPLIFIER
#ifdef ENABLE_AMPLIFIER
    outputConfiguration.blulinkOutputVector.clear();
    list = ElementOutputConfig.elementsByTagName(BLULINKOUTPUT);
    for(int i = 0; i < list.size(); i++)
    {
        EleT = list.at(i).toElement();
        LHC::_blulinkOutput blulinkOutput;
        blulinkOutput.id = EleT.attribute(SPK_ID).toStdWString();
        EleTmp = EleT.firstChildElement(PEAKOUTPUTDBUPK);
        if(EleTmp.isElement())
        {
            blulinkOutput.peakOutputDbuPk = EleTmp.text().toStdWString();
        }
        else
        {
            blulinkOutput.peakOutputDbuPk.clear();
        }
        outputConfiguration.blulinkOutputVector.push_back(blulinkOutput);
    }
#endif // ENABLE_AMPLIFIER
#ifdef ENABLE_AMPLIFIER
    outputConfiguration.analogOutputVector.clear();
    list = ElementOutputConfig.elementsByTagName(ANALOGOUTPUT);
    for (int i = 0; i < list.size(); i++)
    {
        EleT = list.at(i).toElement();
        LHC::_analogOutput analogOutput;
        analogOutput.id = EleT.attribute(SPK_ID).toStdWString();
        EleTmp = EleT.firstChildElement(ANALOGOUTPUTTRIMDB);
        if(EleTmp.isElement())
        {
            analogOutput.analogOutputTrimDb = EleTmp.text().toStdWString();
        }
        else
        {
            analogOutput.analogOutputTrimDb.clear();
        }
        EleTmp = EleT.firstChildElement(SCREENFILTERMODE);
        if(EleTmp.isElement())
        {
            analogOutput.screenFilterMode = EleTmp.text().toStdWString();
        }
        else
        {
            analogOutput.screenFilterMode.clear();
        }
        outputConfiguration.analogOutputVector.push_back(analogOutput);
    }
#endif // ENABLE_AMPLIFIER


    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    //新增扬声器/功放设备信息,本影厅用到的,或者以后保存全部库。20200711 广州项目需求
    QDomElement ElementSpkAmpDeviceLib=ElementProcessConfig.firstChildElement(SPK_AMP_DEVICE_LIB);  
    if (ElementSpkAmpDeviceLib.isElement())
    {
        //扬声器设备列表
        QDomElement ElementSpkDeviceList=ElementSpkAmpDeviceLib.firstChildElement(SPK_DEVICE_LIST);
        QDomNodeList ListSpkDevice=ElementSpkDeviceList.elementsByTagName("SpkInfo");  //扬声器信息元素名称
        lhConfig.mapSpkTable.clear();
        for (int i = 0; i <ListSpkDevice.size(); i++)
        {
                   QDomElement EleSpkDevice=ListSpkDevice.at(i).toElement();
                   PerSpkInfo perSpkInfo;
                   GetFromDomElement(EleSpkDevice,perSpkInfo);
                   if (!perSpkInfo.sFullName.empty())
                   {
                         lhConfig.mapSpkTable[perSpkInfo.sFullName]=perSpkInfo;
                   }
        }

        //功放设备列表
        QDomElement ElementAmpDeviceList=ElementSpkAmpDeviceLib.firstChildElement(AMP_DEVICE_LIST);
        QDomNodeList ListAmpDevice=ElementAmpDeviceList.elementsByTagName("AmpInfo");  //扬声器信息元素名称
        lhConfig.mapAmpTable.clear();
        for (int i = 0; i <ListAmpDevice.size(); i++)
        {
            QDomElement EleAmpDevice=ListAmpDevice.at(i).toElement();
            PerAmpInfo perAmpInfo;
            GetFromDomElement(EleAmpDevice,perAmpInfo);
            if (!perAmpInfo.sFullName.empty())
            {
                lhConfig.mapAmpTable[perAmpInfo.sFullName]=perAmpInfo;
            }
        }
    }



    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    //新增自定义均衡相关
    QDomElement ElementEqGroupAllInfo =ElementProcessConfig.firstChildElement(EQGROUPALLINFO);  
    if (!ElementEqGroupAllInfo.isElement())
    {
      //  return;
    }
    eqAllInfo.isEnabledEQ=true;
    if (ElementEqGroupAllInfo.attribute("isEnabledEq")==QString("false"))
    {
         eqAllInfo.isEnabledEQ=false;
    }
   EleTmp =ElementEqGroupAllInfo.firstChildElement(CURRENT_EQGROUP_NAME);
   if (EleTmp.isElement())
   {
       eqAllInfo.CurrentEQGroupName= EleTmp.text().toStdWString();
   }
   else
   {
        //eqAllInfo.CurrentEQGroupName="";  //获取时候如果为空,则新增Custom并令其为Custom
   }

   PRINT_FUN_NAME
   printf("CurrentEQGroupName:%s\n",WStringToString(eqAllInfo.CurrentEQGroupName).c_str());

    QDomNodeList ListEqGroup=ElementEqGroupAllInfo.elementsByTagName(EQGROUP_NAME);
    eqAllInfo.EQALL.clear();
    for (int i = 0; i < ListEqGroup.size(); i++)
    {
        QDomElement EleEqGroup=ListEqGroup.at(i).toElement();
        std::wstring sEqGroupName=EleEqGroup.attribute("name").toStdWString();
        printf("parser  sEqGroupName:%s\n",WStringToString(sEqGroupName).c_str());
        if (EleEqGroup.isElement())
        {
             LHC::EQGroupOne EqGroupOneTmp;
            //printf("parser  speakers\n");
             QDomElement  ElementSpeakers= EleEqGroup.firstChildElement(SPEAKERS);
             if (ElementSpeakers.isElement())
             {
                    //printf("parser  speaker\n");
//#ifdef OS_ARM_LINUX
                 // 为了分频选择扬声器开放所有的未使用音箱
                 // DCAP-EAH 64ch output max
#ifdef OS_ARM_LINUX
                 int MaxSpk = 64;
#else
                 int MaxSpk = 128;
#endif
                 std::vector<int> totalSpk;
                 for(int i = 0; i < MaxSpk; i++)
                 {
                     totalSpk.push_back(i);
                 }
//                  printf("reserved size %d\n", totalSpk.size());
//#endif
                    QDomNodeList ListSpeaker=ElementSpeakers.elementsByTagName(SPEAKER);
                    for (int i = 0; i < ListSpeaker.size(); i++)
                    {
                        QDomElement EleTmpSpk=ListSpeaker.at(i).toElement();
                        if (EleTmpSpk.isElement())
                        {
                                //NEW
                                LHC::_EQTypeAll eqTypeTmp;  //
                                GetFromDomElement(EleTmpSpk,eqTypeTmp);
                                std::wstring sSpkName=StringToWString(eqTypeTmp.sSpkName);
                                eqTypeTmp.updateInfo();  //兼容旧的配置纠正参数
                                //eqTypeTmp.print();

                                EqGroupOneTmp.speakerEQ[sSpkName]=eqTypeTmp;

                                // 为了分频选择扬声器开放所有的未使用音箱
                                std::vector<int>::iterator itr=totalSpk.begin();
                                while(itr!=totalSpk.end())
                                {
                                    if(*itr==eqTypeTmp.channel)
                                    {
                                        itr=totalSpk.erase(itr);
                                    }
                                    else
                                    {
                                        itr++;
                                    }
                                }
                        }
                    }
// #ifdef OS_ARM_LINUX
                    // 为了分频选择扬声器开放所有的未使用音箱
//                     printf("reserved size %d\n", totalSpk.size());
                    for(int i = 0; i < totalSpk.size(); i++)
                    {
                        std::wstring sSpkName;
                        LHC::_EQTypeAll eqTypeTmp;  //
                        wchar_t buf[64];
                        swprintf(buf,64,L"Undef %d", totalSpk[i]+1);
                        sSpkName=buf;
                        eqTypeTmp.sSpkName=WStringToString(sSpkName);
                        eqTypeTmp.channel=totalSpk[i];
                        eqTypeTmp.updateInfo();

                        EqGroupOneTmp.speakerEQ[sSpkName]=eqTypeTmp;
                    }
// #endif
             }

             QDomElement  ElementArrays= EleEqGroup.firstChildElement(ARRAYS);
             if (ElementArrays.isElement())
             {
                     QDomNodeList ListSpeaker=ElementArrays.elementsByTagName(SPEAKER);
                     for (int i = 0; i < ListSpeaker.size(); i++)
                     {
                         QDomElement EleTmpSpk=ListSpeaker.at(i).toElement();
                         if (EleTmpSpk.isElement())
                         {
                                LHC::_EQTypeAll eqTypeTmp;  //
                                GetFromDomElement(EleTmpSpk,eqTypeTmp);
                                eqTypeTmp.updateInfo();  //兼容旧的配置纠正参数
                                //eqTypeTmp.print();
                                EqGroupOneTmp.arrayEQ[StringToWString( eqTypeTmp.sSpkName)]=eqTypeTmp;
                         }
                    }
             }

             printf("lhcparser() eqAllInfo insert: sEqGroupName:%s \n",WStringToString(sEqGroupName).c_str());
             eqAllInfo.EQALL[sEqGroupName]=EqGroupOneTmp;
        }
    }

    return true;
}

bool Lhc::loadFromFile(const char* fileName)
{
   PRINT_FUN_TRACE
    QFile file((fileName));
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }
    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, true, &errStr, &errLine, &errColumn))
    {
        file.close();
        printf("%s %d %d \n", errStr.toStdString().c_str(), errLine, errColumn);
        return false;
    }
    file.close();

    bool res = Parser_DomToStu(doc, lhConfig);
    processBassMgr();
    buildSpeakerMap();

    updateSpeakerVector();   //刷新扬声器列表
    return res;
}

bool Lhc::loadFromBuf(const char* buf, int length)
{
   PRINT_FUN_TRACE

    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(QByteArray(buf, length),
        true, &errStr, &errLine, &errColumn))
    {
        return false;
    }
    bool res = Parser_DomToStu(doc,lhConfig);

    processBassMgr();
    buildSpeakerMap();
    updateSpeakerVector();   //刷新扬声器列表
    return res;
}

bool Lhc::Parser_StuToDom(QDomDocument &doc, const LHC::_leonisHoleSoundConfig& lhConfig)
{
   PRINT_FUN_TRACE
    QDomProcessingInstruction inst = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");
    doc.appendChild(inst);
    //atmosConfigSuiteData
    QDomElement ElementConfig = doc.createElement(CONFIGSUITEDATA);
    doc.appendChild(ElementConfig);
    ElementConfig.setAttribute("version", "1.1.0");
    ElementConfig.setAttribute("xmlns", XMLNS);
    ElementConfig.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    ElementConfig.setAttribute("xsi:schemaLocation", SCHEMALOCATION);
    QDomElement ElementProcessorConfig = doc.createElement(PROCESSORCONFIG);
    ElementConfig.appendChild(ElementProcessorConfig);

    QDomElement ElementMetadata = doc.createElement(METADATA);
    ElementConfig.appendChild(ElementMetadata);
    //roomConfiguration
    QDomElement ElementRoomConfig = doc.createElement(ROOMCONFIG);
    ElementProcessorConfig.appendChild(ElementRoomConfig);
    QDomElement EleItem = doc.createElement(NAME);

    //引用可以大幅缩短单行代码长度，by lizulin 20180425
    const LHC::_roomConfiguration& roomConfiguration=lhConfig.processorConfiguration.roomConfiguration;
    const LHC::_roomConfiguration::_room& room=lhConfig.processorConfiguration.roomConfiguration.room;
    const LHC::_outputConfiguration& outputConfiguration=lhConfig.processorConfiguration.outputConfiguration;
    const LHC::_eqConfiguration& eqConfiguration=lhConfig.processorConfiguration.eqConfiguration;
    const LHC::_metadata& metadata=lhConfig.metadata;
    const LHC::EQCoeffInfo& eqAllInfo=lhConfig.eqALLInfo;
    const MainSpeakerCrossoverCoef& mainSCC = lhConfig.mainSpeakerCrossoverCoef;

    QDomText text;
    if(!roomConfiguration.name.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(roomConfiguration.name));
        EleItem.appendChild(text);
    }
    ElementRoomConfig.appendChild(EleItem);
    //room

    QDomElement ElementRoom = doc.createElement(ROOM);
    ElementRoomConfig.appendChild(ElementRoom);
    //info
    QDomElement ElementInfo = doc.createElement(INFO);
    ElementRoom.appendChild(ElementInfo);
    EleItem = doc.createElement(COMPANY);
    if(!room.info.company.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(room.info.company));
        EleItem.appendChild(text);
    }
    ElementInfo.appendChild(EleItem);
    EleItem = doc.createElement(THEATERNAME);
    if(!room.info.theaterName.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(room.info.theaterName));
        EleItem.appendChild(text);
    }
    ElementInfo.appendChild(EleItem);
    EleItem = doc.createElement(AUDITORIUM);
    if(!room.info.auditorium.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(room.info.auditorium));
        EleItem.appendChild(text);
    }
    ElementInfo.appendChild(EleItem);
    EleItem = doc.createElement(CITY);
    if(!room.info.city.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(room.info.city));
        EleItem.appendChild(text);
    }
    ElementInfo.appendChild(EleItem);
    EleItem = doc.createElement(STATE);
    if(!room.info.state.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(room.info.state));
        EleItem.appendChild(text);
    }
    ElementInfo.appendChild(EleItem);
    EleItem = doc.createElement(COUNTRY);
    if(!room.info.country.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(room.info.country));
        EleItem.appendChild(text);
    }
    ElementInfo.appendChild(EleItem);
    //dimensions
    QDomElement ElementDimensions = doc.createElement(DIMENSIONS);
    ElementRoom.appendChild(ElementDimensions);
    EleItem = doc.createElement(SCREENWALLWIDTH);
    //if(!room.dimensions.screenWallWidth.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.screenWallWidth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(BOOTHWALLWIDTH);
    //if(!room.dimensions.boothWallWidth.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.boothWallWidth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(HOUSELEFTWALLWIDTH);
    //if(!room.dimensions.houseLeftWallWidth.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.houseLeftWallWidth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(HOUSERIGHTWALLWIDTH);
    //if(!room.dimensions.houseRightWallWidth.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.houseRightWallWidth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(SCREENHOUSELEFTINTERIORANGLE);
    //if(!room.dimensions.screenHouseLeftInteriorAngle.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.screenHouseLeftInteriorAngle,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(SCREENHOUSERIGHTINTERIORANGLE);
    //if(!room.dimensions.screenHouseRightInteriorAngle.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.screenHouseRightInteriorAngle,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(BOOTHHOUSELEFTINTERIORANGLE);
    //if(!room.dimensions.boothHouseLeftInteriorAngle.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.boothHouseLeftInteriorAngle,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(BOOTHHOUSERIGHTINTERIORANGLE);
    //if(!room.dimensions.boothHouseRightInteriorAngle.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.boothHouseRightInteriorAngle,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(FLOORELEVATIONATSCREEN);
    //if(!room.dimensions.floorElevationAtScreen.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.floorElevationAtScreen,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(CEILINGELEVATIONATSCREEN);
    //if(!room.dimensions.ceilingElevationAtScreen.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.ceilingElevationAtScreen,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(FLOORELEVATIONATTWOTHIRDS);
    //if(!room.dimensions.floorElevationAtTwoThirds.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.floorElevationAtTwoThirds,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(CEILINGELEVATIONATTWOTHIRDS);
    //if(!room.dimensions.ceilingElevationAtTwoThirds.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.ceilingElevationAtTwoThirds,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(FLOORELEVATIONATBOOTH);
    //if(!room.dimensions.floorElevationAtBooth.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.floorElevationAtBooth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(CEILINGELEVATIONATBOOTH);
    //if(!room.dimensions.ceilingElevationAtBooth.empty())
    {
        text = doc.createTextNode(QString::number(room.dimensions.ceilingElevationAtBooth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    //flatScreen
    QDomElement ElementFlatScreen = doc.createElement(FLATSCREEN);
    ElementRoom.appendChild(ElementFlatScreen);
    EleItem = doc.createElement(ROOM_WIDTH);
    //if(!room.flatScreen.width.empty())
    {
        text = doc.createTextNode(QString::number(room.flatScreen.width,'f', 2));
        EleItem.appendChild(text);
    }
    ElementFlatScreen.appendChild(EleItem);
    EleItem = doc.createElement(ROOM_HEIGHT);
    //if(!room.flatScreen.height.empty())
    {
        text = doc.createTextNode(QString::number(room.flatScreen.height,'f', 2));
        EleItem.appendChild(text);
    }
    ElementFlatScreen.appendChild(EleItem);
    QDomElement EleTmp = doc.createElement(DISTANCETOLEFTWALL);
    ElementFlatScreen.appendChild(EleTmp);
    EleItem = doc.createElement(POS_X);
    //if(!room.flatScreen.distanceToLeftWall.x.empty())
    {
        text = doc.createTextNode(QString::number(room.flatScreen.distanceToLeftWall.x,'f', 2));
        EleItem.appendChild(text);
    }
    EleTmp.appendChild(EleItem);
    EleItem = doc.createElement(POS_Y);
    //if(!room.flatScreen.distanceToLeftWall.y.empty())
    {
        text = doc.createTextNode(QString::number(room.flatScreen.distanceToLeftWall.y,'f', 2));
        EleItem.appendChild(text);
    }
    EleTmp.appendChild(EleItem);
    EleItem = doc.createElement(POS_Z);
    //if(!room.flatScreen.distanceToLeftWall.z.empty())
    {
        text = doc.createTextNode(QString::number(room.flatScreen.distanceToLeftWall.z,'f', 2));
        EleItem.appendChild(text);
    }
    EleTmp.appendChild(EleItem);
    //scopeScreen
    ElementFlatScreen = doc.createElement(SCOPESCREEN);
    ElementRoom.appendChild(ElementFlatScreen);
    EleItem = doc.createElement(ROOM_WIDTH);
    //if(!room.scopeScreen.width.empty())
    {
        text = doc.createTextNode(QString::number(room.scopeScreen.width,'f', 2));
        EleItem.appendChild(text);
    }
    ElementFlatScreen.appendChild(EleItem);
    EleItem = doc.createElement(ROOM_HEIGHT);
    //if(!room.scopeScreen.height.empty())
    {
        text = doc.createTextNode(QString::number(room.scopeScreen.height,'f', 2));
        EleItem.appendChild(text);
    }
    ElementFlatScreen.appendChild(EleItem);
    EleTmp = doc.createElement(DISTANCETOLEFTWALL);
    ElementFlatScreen.appendChild(EleTmp);
    EleItem = doc.createElement(POS_X);
    //if(!room.scopeScreen.distanceToLeftWall.x.empty())
    {
        text = doc.createTextNode(QString::number(room.scopeScreen.distanceToLeftWall.x,'f', 2));
        EleItem.appendChild(text);
    }
    EleTmp.appendChild(EleItem);
    EleItem = doc.createElement(POS_Y);
    //if(!room.scopeScreen.distanceToLeftWall.y.empty())
    {
        text = doc.createTextNode(QString::number(room.scopeScreen.distanceToLeftWall.y,'f', 2));
        EleItem.appendChild(text);
    }
    EleTmp.appendChild(EleItem);
    EleItem = doc.createElement(POS_Z);
    //if(!room.scopeScreen.distanceToLeftWall.z.empty())
    {
        text = doc.createTextNode(QString::number(room.scopeScreen.distanceToLeftWall.z,'f', 2));
        EleItem.appendChild(text);
    }
    EleTmp.appendChild(EleItem);

    //region


    QDomElement ElementRegion = doc.createElement(REGION);
    ElementRoom.appendChild(ElementRegion);
    EleTmp = doc.createElement(NAME);
    if(!room.region.name.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(room.region.name));
        EleTmp.appendChild(text);
    }
    ElementRegion.appendChild(EleTmp);
    ElementDimensions = doc.createElement(DIMENSIONS);
    ElementRegion.appendChild(ElementDimensions);
    EleItem = doc.createElement(SCREENWALLWIDTH);
    //if(!room.region.dimensions.screenWallWidth.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.screenWallWidth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(BOOTHWALLWIDTH);
    //if(!room.region.dimensions.boothWallWidth.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.boothWallWidth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(HOUSELEFTWALLWIDTH);
    //if(!room.region.dimensions.houseLeftWallWidth.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.houseLeftWallWidth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(HOUSERIGHTWALLWIDTH);
    //if(!room.region.dimensions.houseRightWallWidth.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.houseRightWallWidth,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(SCREENHOUSELEFTINTERIORANGLE);
    //if(!room.region.dimensions.screenHouseLeftInteriorAngle.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.screenHouseLeftInteriorAngle,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(SCREENHOUSERIGHTINTERIORANGLE);
    //if(!room.region.dimensions.screenHouseRightInteriorAngle.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.screenHouseRightInteriorAngle,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(BOOTHHOUSELEFTINTERIORANGLE);
    //if(!room.region.dimensions.boothHouseLeftInteriorAngle.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.boothHouseLeftInteriorAngle,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(BOOTHHOUSERIGHTINTERIORANGLE);
    //if(!room.region.dimensions.boothHouseRightInteriorAngle.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.boothHouseRightInteriorAngle,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(FLOORELEVATIONATSCREEN);
    //if(!room.region.dimensions.floorElevationAtScreen.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.floorElevationAtScreen,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    EleItem = doc.createElement(CEILINGELEVATIONATSCREEN);
    //if(!room.region.dimensions.ceilingElevationAtScreen.empty())
    {
        text = doc.createTextNode(QString::number(room.region.dimensions.ceilingElevationAtScreen,'f', 2));
        EleItem.appendChild(text);
    }
    ElementDimensions.appendChild(EleItem);
    //speakerEndpoint

    for(unsigned int i = 0; i < room.region.speakerEndpointVector.size(); i++)
    {
        LHC::_speakerEndpoint se = room.region.speakerEndpointVector[i];
        QDomElement ElementSpkEndPoint = doc.createElement(SPEAKERENDPOINT);
        ElementRegion.appendChild(ElementSpkEndPoint);
        ElementSpkEndPoint.setAttribute("xsi:"TYPE, QString::fromStdWString(se.type));    
        //ElementDimensions.setAttribute(SPK_ID, QString::fromStdWString(se.id));
        QString qsid="SPKREP-";
        qsid+=QString::number(se.id);
        ElementSpkEndPoint.setAttribute(SPK_ID,qsid);

        EleTmp = doc.createElement(NAME);
        if(!se.name.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(se.name));
            EleTmp.appendChild(text);
        }
        ElementSpkEndPoint.appendChild(EleTmp);

        EleItem = doc.createElement(POSITION);
        ElementSpkEndPoint.appendChild(EleItem);

        EleTmp = doc.createElement(POS_X);
        text = doc.createTextNode(QString::number(se.position.x,'f', 2));
        EleTmp.appendChild(text);
        EleItem.appendChild(EleTmp);

        EleTmp = doc.createElement(POS_Y);
        text = doc.createTextNode(QString::number(se.position.y,'f', 2));
        EleTmp.appendChild(text);
        EleItem.appendChild(EleTmp);

        EleTmp = doc.createElement(POS_Z);
        text = doc.createTextNode(QString::number(se.position.z,'f', 2));
        EleTmp.appendChild(text);
        EleItem.appendChild(EleTmp);

        for (unsigned int j = 0; j < se.arrayVector.size(); j++)
        {
            EleTmp = doc.createElement(ARRAY);
            if(!se.arrayVector[j].empty())
            {
                text = doc.createTextNode(QString::fromStdWString(se.arrayVector[j]));
                EleTmp.appendChild(text);
            }
            ElementSpkEndPoint.appendChild(EleTmp);
        }

        EleTmp = doc.createElement(DELAY);
        text = doc.createTextNode(QString::number(se.delay,'f', 2));
        EleTmp.appendChild(text);
        ElementSpkEndPoint.appendChild(EleTmp);

        for (unsigned int j = 0; j < se.bassManagementVector.size(); j++)
        {
            ElementInfo = doc.createElement(BASSMANAGEMENT);
            ElementSpkEndPoint.appendChild(ElementInfo);
            LHC::_bassManagement bm = se.bassManagementVector[j];
            EleTmp = doc.createElement(SUBWOOFERREF);
            if(!bm.subwooferRef.empty())
            {
                text = doc.createTextNode(QString::fromStdWString(bm.subwooferRef));
                EleTmp.appendChild(text);
            }
            ElementInfo.appendChild(EleTmp);

            EleTmp = doc.createElement(CROSSOVERFREQUENCYHZ);
            text = doc.createTextNode(QString::number(bm.crossoverFrequencyHz));
            EleTmp.appendChild(text);
            ElementInfo.appendChild(EleTmp);

            EleTmp = doc.createElement(GAINDB);
            text = doc.createTextNode(QString::number(bm.gainDb,'f', 2));
            EleTmp.appendChild(text);
            ElementInfo.appendChild(EleTmp);
        }


        EleTmp = doc.createElement(SPECSREF);
        if(!se.specsRef.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(se.specsRef));
            EleTmp.appendChild(text);
            ElementSpkEndPoint.appendChild(EleTmp);
        }

        EleTmp = doc.createElement(SPK_DEVICE_NAME);
        if(!se.sSpkDeviceName.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(se.sSpkDeviceName));
            EleTmp.appendChild(text);
            ElementSpkEndPoint.appendChild(EleTmp);
        }
        EleTmp = doc.createElement(AMP_DEVICE_NAME);
        if(!se.sAmpDeviceName.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(se.sAmpDeviceName));
            EleTmp.appendChild(text);
            ElementSpkEndPoint.appendChild(EleTmp);
        }
    }

    //speakerSpecs
    for (unsigned int i = 0; i < roomConfiguration.speakerSpecsVector.size(); i++)
    {
        const LHC::_speakerSpecs& ss = roomConfiguration.speakerSpecsVector[i];
        ElementInfo = doc.createElement(SPEAKERSPECS);
        ElementRoomConfig.appendChild(ElementInfo);
        ElementInfo.setAttribute("xsi:"TYPE, QString::fromStdWString(ss.type));
        ElementInfo.setAttribute(SPK_ID, QString::fromStdWString(ss.id));
        EleTmp = doc.createElement(MANUFACTURER);
        if(!ss.manufacturer.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ss.manufacturer));
            EleTmp.appendChild(text);
        }
        ElementInfo.appendChild(EleTmp);
        EleTmp = doc.createElement(MODEL);
        if(!ss.model.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ss.model));
            EleTmp.appendChild(text);
        }
        ElementInfo.appendChild(EleTmp);
        EleTmp = doc.createElement(FREQUENCYRESPONSEMINHZ);
        text = doc.createTextNode(QString::number(ss.frequencyResponseMinHz));
        EleTmp.appendChild(text);
        ElementInfo.appendChild(EleTmp);
        EleTmp = doc.createElement(FREQUENCYRESPONSEMAXHZ);
        text = doc.createTextNode(QString::number(ss.frequencyResponseMaxHz));
        EleTmp.appendChild(text);
        ElementInfo.appendChild(EleTmp);
        EleTmp = doc.createElement(CROSSOVERFREQUENCYHZ);
        text = doc.createTextNode(QString::number(ss.crossoverFrequencyHz));
        EleTmp.appendChild(text);

        ElementInfo.appendChild(EleTmp);
        EleTmp = doc.createElement(PEAKINPUTDBUPK);
        text = doc.createTextNode(QString::number(ss.peakInputDbuPk));
        EleTmp.appendChild(text);
        ElementInfo.appendChild(EleTmp);
    }
    //eqConfiguration
    QDomElement ElementEqconfig = doc.createElement(EQCONFIGURATION);
    ElementProcessorConfig.appendChild(ElementEqconfig);
    ElementEqconfig.setAttribute(ISEQENABLED, QString::fromStdWString(eqConfiguration.isEqEnabled));
    //arrayEq
    for (unsigned int i = 0; i <eqConfiguration.arrayEqVector.size(); i++)
    {
        QDomElement ElementArrareq = doc.createElement(ARRAYEQ);
        ElementEqconfig.appendChild(ElementArrareq);
        LHC::_arrayEq ae = eqConfiguration.arrayEqVector[i];
        EleTmp = doc.createElement(DELAYMS);

        text = doc.createTextNode(QString::number(ae.delayMs,'f', 2));
        EleTmp.appendChild(text);
        ElementArrareq.appendChild(EleTmp);
        EleTmp = doc.createElement(GAINDB);

        text = doc.createTextNode(QString::number(ae.gainDb,'f', 2));
        EleTmp.appendChild(text);
        ElementArrareq.appendChild(EleTmp);
        EleTmp = doc.createElement(USERGAINDB);

        text = doc.createTextNode(QString::number(ae.userGainDb,'f', 2));
        EleTmp.appendChild(text);
        ElementArrareq.appendChild(EleTmp);
        ElementInfo = doc.createElement(EQRESPONSECURVE);
        ElementArrareq.appendChild(ElementInfo);
        //eqResponseCurveVector
        for (unsigned int j = 0; j < ae.eqResponseCurveVector.size(); j++)
        {
            LHC::_responsePoint rp = ae.eqResponseCurveVector[j];
            ElementDimensions = doc.createElement(RESPONSEPOINT);
            ElementInfo.appendChild(ElementDimensions);

            EleTmp = doc.createElement(GAINDB);
            text = doc.createTextNode(QString::number(rp.gainDb));
            EleTmp.appendChild(text);
            ElementDimensions.appendChild(EleTmp);

            EleTmp = doc.createElement(FREQUENCYHZ);
            text = doc.createTextNode(QString::number(rp.frequencyHz));
            EleTmp.appendChild(text);
            ElementDimensions.appendChild(EleTmp);
        }
        ElementInfo = doc.createElement(TARGETEQRESPONSECURVE);
        ElementArrareq.appendChild(ElementInfo);
        EleTmp = doc.createElement(USEDEFAULTXCURVE);
        if(!ae.targetEqResponseCurve.useDefaultXCurve.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.targetEqResponseCurve.useDefaultXCurve));
            EleTmp.appendChild(text);
        }
        ElementInfo.appendChild(EleTmp);
        EleTmp = doc.createElement(LOWFREQBOOSTLIMITHZ);

        text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.lowFreqBoostLimitHz));
        EleTmp.appendChild(text);
        ElementInfo.appendChild(EleTmp);

        EleTmp = doc.createElement(HIGHFREQBOOSTLIMITHZ);
        text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.highFreqBoostLimitHz));
        EleTmp.appendChild(text);

        ElementInfo.appendChild(EleTmp);
        ElementDimensions = doc.createElement(XCURVEPARAMETERS);
        ElementInfo.appendChild(ElementDimensions);
        EleTmp = doc.createElement(FREQCORNERAHZ);
        //if(!ae.targetEqResponseCurve.xCurveParameters.freqCornerAHz.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.xCurveParameters.freqCornerAHz));
            EleTmp.appendChild(text);
        }
        ElementDimensions.appendChild(EleTmp);
        EleTmp = doc.createElement(FREQCORNERBHZ);
        //if(!ae.targetEqResponseCurve.xCurveParameters.freqCornerBHz.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.xCurveParameters.freqCornerBHz));
            EleTmp.appendChild(text);
        }
        ElementDimensions.appendChild(EleTmp);
        EleTmp = doc.createElement(SLOPEADBPEROCT);
        //if(!ae.targetEqResponseCurve.xCurveParameters.slopeAdBPerOct.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.xCurveParameters.slopeAdBPerOct));
            EleTmp.appendChild(text);
        }
        ElementDimensions.appendChild(EleTmp);
        EleTmp = doc.createElement(SLOPEBDBPEROCT);
        //if(!ae.targetEqResponseCurve.xCurveParameters.slopeBdBPerOct.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.xCurveParameters.slopeBdBPerOct));
            EleTmp.appendChild(text);
        }
        ElementDimensions.appendChild(EleTmp);
        EleTmp = doc.createElement(ARRAY);
        if(!ae.array.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.array));
            EleTmp.appendChild(text);
        }
        ElementArrareq.appendChild(EleTmp);
    }
    //speakerEq
    for (unsigned int i = 0; i <eqConfiguration.speakerEqVector.size(); i++)
    {
        QDomElement ElementArrayEq = doc.createElement(SPEAKEREQ);
        ElementEqconfig.appendChild(ElementArrayEq);
        LHC::_speakerEq ae =eqConfiguration.speakerEqVector[i];
        EleTmp = doc.createElement(DELAYMS);
        //if(!ae.delayMs.empty())
        {
            text = doc.createTextNode(QString::number(ae.delayMs));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
        EleTmp = doc.createElement(GAINDB);
        //if(!ae.gainDb.empty())
        {
            text = doc.createTextNode(QString::number(ae.gainDb));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
        EleTmp = doc.createElement(USERGAINDB);
        //if(!ae.userGainDb.empty())
        {
            text = doc.createTextNode(QString::number(ae.userGainDb));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
        ElementInfo = doc.createElement(EQRESPONSECURVE);
        ElementArrayEq.appendChild(ElementInfo);
        //responsePoint
        for (unsigned int j = 0; j < ae.eqResponseCurveVector.size(); j++)
        {
            LHC::_responsePoint rp = ae.eqResponseCurveVector[j];
            ElementDimensions = doc.createElement(RESPONSEPOINT);
            ElementInfo.appendChild(ElementDimensions);
            EleTmp = doc.createElement(GAINDB);
            //if(!rp.gainDb.empty())
            {
                text = doc.createTextNode(QString::number(rp.gainDb));
                EleTmp.appendChild(text);
            }
            ElementDimensions.appendChild(EleTmp);
            EleTmp = doc.createElement(FREQUENCYHZ);
            //if(!rp.frequencyHz.empty())
            {
                text = doc.createTextNode(QString::number(rp.frequencyHz));
                EleTmp.appendChild(text);
            }
            ElementDimensions.appendChild(EleTmp);
        }
        ElementInfo = doc.createElement(TARGETEQRESPONSECURVE);
        ElementArrayEq.appendChild(ElementInfo);
        EleTmp = doc.createElement(USEDEFAULTXCURVE);
        if(!ae.targetEqResponseCurve.useDefaultXCurve.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.targetEqResponseCurve.useDefaultXCurve));
            EleTmp.appendChild(text);
        }
        ElementInfo.appendChild(EleTmp);
        EleTmp = doc.createElement(LOWFREQBOOSTLIMITHZ);
        //if(!ae.targetEqResponseCurve.lowFreqBoostLimitHz.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.lowFreqBoostLimitHz));
            EleTmp.appendChild(text);
        }
        ElementInfo.appendChild(EleTmp);
        EleTmp = doc.createElement(HIGHFREQBOOSTLIMITHZ);
        //if(!ae.targetEqResponseCurve.highFreqBoostLimitHz.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.highFreqBoostLimitHz));
            EleTmp.appendChild(text);
        }
        ElementInfo.appendChild(EleTmp);
        ElementDimensions = doc.createElement(XCURVEPARAMETERS);
        ElementInfo.appendChild(ElementDimensions);
        EleTmp = doc.createElement(FREQCORNERAHZ);
        //if(!ae.targetEqResponseCurve.xCurveParameters.freqCornerAHz.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.xCurveParameters.freqCornerAHz));
            EleTmp.appendChild(text);
        }
        ElementDimensions.appendChild(EleTmp);
        EleTmp = doc.createElement(FREQCORNERBHZ);
        //if(!ae.targetEqResponseCurve.xCurveParameters.freqCornerBHz.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.xCurveParameters.freqCornerBHz));
            EleTmp.appendChild(text);
        }
        ElementDimensions.appendChild(EleTmp);
        EleTmp = doc.createElement(SLOPEADBPEROCT);
        //if(!ae.targetEqResponseCurve.xCurveParameters.slopeAdBPerOct.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.xCurveParameters.slopeAdBPerOct));
            EleTmp.appendChild(text);
        }
        ElementDimensions.appendChild(EleTmp);
        EleTmp = doc.createElement(SLOPEBDBPEROCT);
        //if(!ae.targetEqResponseCurve.xCurveParameters.slopeBdBPerOct.empty())
        {
            text = doc.createTextNode(QString::number(ae.targetEqResponseCurve.xCurveParameters.slopeBdBPerOct));
            EleTmp.appendChild(text);
        }
        ElementDimensions.appendChild(EleTmp);
        EleTmp = doc.createElement(SPEAKERENDPOINTREF);
        if(!ae.speakerEndpointRef.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.speakerEndpointRef));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
    }

    // Add CrossOver configuration [6/19/2020 jaontolt]
    QDomElement ElementCrossOverConfig = doc.createElement("CrossOverConfiguration");
    ElementProcessorConfig.appendChild(ElementCrossOverConfig);
    QString sTmp=(mainSCC.EnableCrossover)?("true"):("false");
    ElementCrossOverConfig.setAttribute("isEnabledCrossOver", sTmp);
    for(int i = 0; i < mainSCC.co_speaker.size(); i++)
    {
        const _SPK_PARAM &param = mainSCC.co_speaker[i];
        QDomElement spk = doc.createElement("Speaker");
        ElementCrossOverConfig.appendChild(spk);

        EleTmp = doc.createElement("Cnt");
        spk.appendChild(EleTmp);
        text = doc.createTextNode(QString::number(param.co_cnt));
        EleTmp.appendChild(text);

        EleTmp = doc.createElement("In");
        spk.appendChild(EleTmp);
        text = doc.createTextNode(QString::number(param.in));
        EleTmp.appendChild(text);

        EleTmp = doc.createElement("OutL");
        spk.appendChild(EleTmp);
        text = doc.createTextNode(QString::number(param.out_L));
        EleTmp.appendChild(text);

        EleTmp = doc.createElement("OutM");
        spk.appendChild(EleTmp);
        text = doc.createTextNode(QString::number(param.out_M));
        EleTmp.appendChild(text);

        EleTmp = doc.createElement("OutH");
        spk.appendChild(EleTmp);
        text = doc.createTextNode(QString::number(param.out_H));
        EleTmp.appendChild(text);

        EleTmp = doc.createElement("Lpf");
        spk.appendChild(EleTmp);
        text = doc.createTextNode(QString::number(param.Lpf, 'f', 1));
        EleTmp.appendChild(text);

        EleTmp = doc.createElement("Hpf");
        spk.appendChild(EleTmp);
        text = doc.createTextNode(QString::number(param.Hpf, 'f', 1));
        EleTmp.appendChild(text);
    }

    // Add ChannelMappingConfiguration [6/20/2020 jaontolt]
    QDomElement ElementChannelMappingConfig = doc.createElement("ChannelMappingConfiguration");
    ElementProcessorConfig.appendChild(ElementChannelMappingConfig);
    for (int n = 0; n < 8; n++)
    {
        const LHC::_channelMappingConfiguration& channelMapping = lhConfig.channelMappingConfiguration[n];
        QDomElement tep = doc.createElement("Template");
        tep.setAttribute("Name", QString(channelMapping.templateName));
        ElementChannelMappingConfig.appendChild(tep);

        for(int i = 0; i < 16; i++)
        {
            QDomElement spk = doc.createElement("Channel");
            tep.appendChild(spk);

            EleTmp = doc.createElement("Input");
            text = doc.createTextNode(QString::number(i));
            EleTmp.appendChild(text);
            spk.appendChild(EleTmp);

            EleTmp = doc.createElement("MapTo");
            text = doc.createTextNode(QString::number(channelMapping.mapping[i]));
            EleTmp.appendChild(text);
            spk.appendChild(EleTmp);
        }
    }


    //outputConfiguration
    ElementEqconfig = doc.createElement(OUTPUTCONFIGURATION);
    ElementProcessorConfig.appendChild(ElementEqconfig);
    
    ElementEqconfig.setAttribute(ISLIMITERENABLED, QString::fromStdWString(outputConfiguration.isLimiterEnabled));
    //amplifier
#ifdef ENABLE_AMPLIFIER
    for (unsigned int i = 0; i < outputConfiguration.amplifierVector.size(); i++)
    {
        QDomElement ElementArrayEq = doc.createElement(AMPLIFIER);
        ElementEqconfig.appendChild(ElementArrayEq);
        LHC::_amplifier ae =outputConfiguration.amplifierVector[i];
        ElementArrayEq.setAttribute(SPK_ID, QString::fromStdWString(ae.id));
        EleTmp = doc.createElement(VOLTAGEGAINDB);
        if(!ae.voltageGainDb.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.voltageGainDb));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
        EleTmp = doc.createElement(PEAKVOLTAGE);
        if(!ae.peakVoltage.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.peakVoltage));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
        for (unsigned int j = 0; j < ae.ampPerChannelSpecsVector.size(); j++)
        {
            LHC::_ampPerChannelSpecs rp = ae.ampPerChannelSpecsVector[j];
            ElementDimensions = doc.createElement(AMPPERCHANNELSPECS);
            ElementArrayEq.appendChild(ElementDimensions);
            EleTmp = doc.createElement(IMPEDANCEOHM);
            //if(!rp.impedanceOhm.empty())
            {
                text = doc.createTextNode(QString::number(rp.impedanceOhm));
                EleTmp.appendChild(text);
            }
            ElementDimensions.appendChild(EleTmp);
            EleTmp = doc.createElement(POWERWATTS);
            //if(!rp.powerWatts.empty())
            {
                text = doc.createTextNode(QString::number(rp.powerWatts));
                EleTmp.appendChild(text);
            }
            ElementDimensions.appendChild(EleTmp);
        }
        EleTmp = doc.createElement(MANUFACTURER);
        if(!ae.manufacturer.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.manufacturer));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
        EleTmp = doc.createElement(MODEL);
        if(!ae.model.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.model));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
    }
#endif // ENABLE_AMPLIFIER
    //outputRoute
    for (unsigned int i = 0; i <outputConfiguration.outputRouteVector.size(); i++)
    {
        QDomElement ElementAarrayeq = doc.createElement(OUTPUTROUTE);
        ElementEqconfig.appendChild(ElementAarrayeq);
        LHC::_outputRoute ae = outputConfiguration.outputRouteVector[i];
        EleTmp = doc.createElement(OUTPUTREF);
        //if(!ae.outputRef.empty())
        {
            //text = doc.createTextNode(QString::fromStdWString(ae.outputRef));
            QString soutputRef="OUT-BL-";    //保存到文件加OUT-BL-前缀
            soutputRef+=QString::number(ae.outputRef);
            text = doc.createTextNode(soutputRef);
            EleTmp.appendChild(text);
        }
        ElementAarrayeq.appendChild(EleTmp);
        EleTmp = doc.createElement(SPEAKERENDPOINTREF);
        //if(!ae.speakerEndpointRef.empty())
        {
            //text = doc.createTextNode(QString::fromStdWString(ae.speakerEndpointRef));
            QString sspeakerEndpointRef="SPKREP-";    //保存到文件加SPKREP-前缀
            sspeakerEndpointRef+=QString::number(ae.speakerEndpointRef);
            text = doc.createTextNode(sspeakerEndpointRef);
            EleTmp.appendChild(text);
        }
        ElementAarrayeq.appendChild(EleTmp);
        EleTmp = doc.createElement(AMPLIFIERREF);
        if(!ae.amplifierRef.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.amplifierRef));
            EleTmp.appendChild(text);
        }
        ElementAarrayeq.appendChild(EleTmp);
        EleTmp = doc.createElement(AMPLIFIERTRIMDB);
        //if(!ae.amplifierTrimDb.empty())
        {
            text = doc.createTextNode(QString::number(ae.amplifierTrimDb));
            EleTmp.appendChild(text);
        }
        ElementAarrayeq.appendChild(EleTmp);
        EleTmp = doc.createElement(LIMITERTYPE);
        if(!ae.limiterType.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.limiterType));
            EleTmp.appendChild(text);
        }
        ElementAarrayeq.appendChild(EleTmp);
        EleTmp = doc.createElement(SPEAKERPROCESSORREF);
        if(!ae.speakerProcessorRef.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.speakerProcessorRef));
            EleTmp.appendChild(text);
        }
        ElementAarrayeq.appendChild(EleTmp);
    }
    //speakerProcessor
#ifdef ENABLE_AMPLIFIER
    for (unsigned int i = 0; i <outputConfiguration.speakerProcessorVector.size(); i++)
    {
        QDomElement ElementArrayEq = doc.createElement(SPEAKERPROCESSOR);
        ElementEqconfig.appendChild(ElementArrayEq);
        LHC::_speakerProcessor ae =outputConfiguration.speakerProcessorVector[i];
        ElementArrayEq.setAttribute(SPK_ID, QString::fromStdWString(ae.id));
        EleTmp = doc.createElement(MANUFACTURER);
        if(!ae.manufacturer.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.manufacturer));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
        EleTmp = doc.createElement(MODEL);
        if(!ae.model.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.model));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
        EleTmp = doc.createElement(PEAKINPUTDBUPK);
        if(!ae.peakInputDbuPk.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.peakInputDbuPk));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
    }
#endif // ENABLE_AMPLIFIER
#ifdef ENABLE_AMPLIFIER
    //blulinkOutput
    for (unsigned int i = 0; i < outputConfiguration.blulinkOutputVector.size(); i++)
    {
        QDomElement ElementArrayEq = doc.createElement(BLULINKOUTPUT);
        ElementEqconfig.appendChild(ElementArrayEq);
        LHC::_blulinkOutput ae =outputConfiguration.blulinkOutputVector[i];
        ElementArrayEq.setAttribute(SPK_ID, QString::fromStdWString(ae.id));
        EleTmp = doc.createElement(PEAKOUTPUTDBUPK);
        if(!ae.peakOutputDbuPk.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.peakOutputDbuPk));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
    }
#endif // ENABLE_AMPLIFIER
    //analogOutput
#ifdef ENABLE_AMPLIFIER
    for (unsigned int i = 0; i <outputConfiguration.analogOutputVector.size(); i++)
    {
        QDomElement ElementArrayEq = doc.createElement(ANALOGOUTPUT);
        ElementEqconfig.appendChild(ElementArrayEq);
        LHC::_analogOutput ae =outputConfiguration.analogOutputVector[i];
        ElementArrayEq.setAttribute(SPK_ID, QString::fromStdWString(ae.id));
        EleTmp = doc.createElement(ANALOGOUTPUTTRIMDB);
        if(!ae.analogOutputTrimDb.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.analogOutputTrimDb));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
        EleTmp = doc.createElement(SCREENFILTERMODE);
        if(!ae.screenFilterMode.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(ae.screenFilterMode));
            EleTmp.appendChild(text);
        }
        ElementArrayEq.appendChild(EleTmp);
    }
#endif // ENABLE_AMPLIFIER
    //metadata
    EleTmp = doc.createElement(_UUID);
    if(!metadata.uuid.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(metadata.uuid));
        EleTmp.appendChild(text);
    }
    ElementMetadata.appendChild(EleTmp);
    EleTmp = doc.createElement(LASTMODIFIEDTIME);
    if(!metadata.lastModifiedTime.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(metadata.lastModifiedTime));
        EleTmp.appendChild(text);
    }
    ElementMetadata.appendChild(EleTmp);
    EleTmp = doc.createElement(LOCALPATHATORIGIN);
    if(!metadata.localPathAtOrigin.empty())
    {
        text = doc.createTextNode(QString::fromStdWString(metadata.localPathAtOrigin));
        EleTmp.appendChild(text);
    }
    ElementMetadata.appendChild(EleTmp);
    //comments
    for (unsigned int i = 0; i <metadata.commentsVector.size(); i++)
    {
        ElementInfo = doc.createElement(COMMENTS);
        LHC::_comments c =metadata.commentsVector[i];
        ElementMetadata.appendChild(ElementInfo);
        EleTmp = doc.createElement(TEXT_);
        if(!c.text.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(c.text));
            EleTmp.appendChild(text);
        }
        ElementInfo.appendChild(EleTmp);
        EleTmp = doc.createElement(TIME);
        if(!c.time.empty())
        {
            text = doc.createTextNode(QString::fromStdWString(c.time));
            EleTmp.appendChild(text);
        }
        ElementInfo.appendChild(EleTmp);
    }


    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //新增扬声器/功放设备信息,本影厅用到的,或者以后保存全部库。20200711 广州项目需求
    QDomElement ElementSpkAmpDeviceLib= doc.createElement(SPK_AMP_DEVICE_LIB);
    ElementProcessorConfig.appendChild(ElementSpkAmpDeviceLib);
    //扬声器列表
    QDomElement ElementSpkDeviceList=doc.createElement(SPK_DEVICE_LIST);
    ElementSpkAmpDeviceLib.appendChild(ElementSpkDeviceList);
    for (auto it=lhConfig.mapSpkTable.begin();it!=lhConfig.mapSpkTable.end();it++)
    {
            const PerSpkInfo&  perSpkInfo=it->second;
            PutToDomElement(doc,ElementSpkDeviceList,perSpkInfo);
    }
    //功放列表
    QDomElement ElementAmpDeviceList=doc.createElement(AMP_DEVICE_LIST);
    ElementSpkAmpDeviceLib.appendChild(ElementAmpDeviceList);
    for (auto it=lhConfig.mapAmpTable.begin();it!=lhConfig.mapAmpTable.end();it++)
    {
        const PerAmpInfo&  perAmpInfo=it->second;
        PutToDomElement(doc,ElementAmpDeviceList,perAmpInfo);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //新增合并自定义均衡类到lhc中
    QDomElement ElementEqGroupAllInfo = doc.createElement(EQGROUPALLINFO);
    ElementProcessorConfig.appendChild(ElementEqGroupAllInfo);

    std::string sisEnabledEQ="false";
    if (eqAllInfo.isEnabledEQ)
    {
         sisEnabledEQ="true";
    }
    ElementEqGroupAllInfo.setAttribute("isEnabledEq", QString::fromStdString(sisEnabledEQ));
    QDomElement ElementCurrentEqGroupName=doc.createElement(CURRENT_EQGROUP_NAME);
    text=doc.createTextNode(QString::fromStdWString(eqAllInfo.CurrentEQGroupName));
    ElementCurrentEqGroupName.appendChild(text);
    ElementEqGroupAllInfo.appendChild(ElementCurrentEqGroupName);

    const std::map<std::wstring,LHC::EQGroupOne>& eqALL=eqAllInfo.EQALL;
    std::map<std::wstring,LHC::EQGroupOne>::const_iterator itGroup;      //所有均衡分组数据
    for (itGroup=eqALL.begin();itGroup!=eqALL.end();itGroup++)
    {
            std::wstring sEqGroupName=itGroup->first;
            QDomElement ElementEqGroupName=doc.createElement(EQGROUP_NAME);
            ElementEqGroupName.setAttribute("name", QString::fromStdWString(sEqGroupName));

            QDomElement ElementSpeakers=doc.createElement(SPEAKERS);
            ElementEqGroupName.appendChild(ElementSpeakers);

            const LHC::EQGroupOne& eqGroupOne=itGroup->second;
            std::map<std::wstring, LHC::_EQTypeAll>::const_iterator itGroupOne;

            // 为了分频选择扬声器开放所有的未使用音箱
#ifdef OS_ARM_LINUX
            int MaxSpk = 64;
#else
            int MaxSpk = 128;
#endif
            std::vector<int> totalSpk;
            for(int i = 0; i<MaxSpk; i++)
            {
                totalSpk.push_back(i);
            }
            itGroupOne = eqGroupOne.speakerEQ.begin();
            while(itGroupOne != eqGroupOne.speakerEQ.end())
            {
                const LHC::_EQTypeAll& eqSpkOne=itGroupOne->second;
                std::vector<int>::iterator itr = totalSpk.begin();
                while (itr != totalSpk.end())
                {
                    if(*itr == eqSpkOne.channel)
                    {
                        itr = totalSpk.erase(itr);
                    }
                    else
                    {
                        itr++;
                    }
                }
                itGroupOne++;
            }
            LHC::EQGroupOne EqGroupOneTmp;
            for(int i = 0; i < totalSpk.size(); i++)
            {
                std::wstring sSpkName;
                LHC::_EQTypeAll eqTypeTmp;  //
                wchar_t buf[64];
                swprintf(buf,64,L"Undef %03d", totalSpk[i]+1);
                sSpkName = buf;
                eqTypeTmp.updateInfo();
                eqTypeTmp.channel = totalSpk[i];  //updatedInfo未分配输出通道，手工分配一次
                EqGroupOneTmp.speakerEQ[sSpkName]=eqTypeTmp;
            }
// #endif

            for (itGroupOne=eqGroupOne.speakerEQ.begin();itGroupOne!=eqGroupOne.speakerEQ.end();itGroupOne++)
            {
                const std::wstring& sSpkName=itGroupOne->first;
                const LHC::_EQTypeAll& eqSpkOne=itGroupOne->second;
               
                QDomElement ElementSpeaker=doc.createElement(SPEAKER);
                PutToDomElement(doc,ElementSpeaker,eqSpkOne);
                ElementSpeakers.appendChild(ElementSpeaker);
            }

            QDomElement ElementArrays=doc.createElement(ARRAYS);
            ElementEqGroupName.appendChild(ElementArrays);
            for (itGroupOne=eqGroupOne.arrayEQ.begin();itGroupOne!=eqGroupOne.arrayEQ.end();itGroupOne++)
            {
                const std::wstring& sSpkName=itGroupOne->first;
                const LHC::_EQTypeAll& eqSpkOne=itGroupOne->second;

                QDomElement ElementSpeaker=doc.createElement(SPEAKER);
                PutToDomElement(doc,ElementSpeaker,eqSpkOne);
                ElementArrays.appendChild(ElementSpeaker);
            }
            ElementEqGroupAllInfo.appendChild(ElementEqGroupName);
            //doc.appendChild(ElementEqGroupName);
    }
    return true;
}


bool Lhc::saveToFile(const char* fileName, bool bOnlyEQ)
{
    QDomDocument doc;
    if(!Parser_StuToDom(doc,lhConfig))
    {
        return false;
    }
//     printf("parser ok\n");

    // 改用C API 保存置文件，以加速配置文件的保存速度 [5/20/2020 jaontolt]
    // 从QT的6秒提升到实时保存
    QByteArray buf = doc.toByteArray(2);
//     printf("save to buffer ok\n");

    FILE *fp = fopen(fileName, "wb");
    if(fp)
    {
        fwrite(buf.data(),1,buf.size(), fp);
        fclose(fp);
    }
//     printf("C api save ok\n");

#if 0
    QFile savefile(fileName);
    savefile.open(QFile::ReadWrite|QFile::Text|QFile::Truncate);
    printf("open file ok\n");
    QTextStream out(&savefile);
    printf("stream out ok\n");
    doc.save(out, 2);
    printf("save ok\n");
    savefile.close();
    printf("close ok\n");
#endif // 0

    // 加快EQ保存速度
    if(!bOnlyEQ)
    {
        processBassMgr();
        buildSpeakerMap();
    }

    return true;
}

bool Lhc::loadFromBuf(const std::wstring& buf)
{
    QString s;
    s.fromStdWString(buf);

    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(s/*QByteArray(xmlBuffer.c_str(), xmlBuffer.size())*/,
        true, &errStr, &errLine, &errColumn))
    {
        return false;
    }
    bool res = Parser_DomToStu(doc,lhConfig);

    processBassMgr();
    buildSpeakerMap();
    updateSpeakerVector();   //刷新扬声器列表
    return res;
}

bool Lhc::loadFromBuf(const std::string& buf)
{
    QString s;
    s.fromStdString(buf);

    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(s/*QByteArray(xmlBuffer.c_str(), xmlBuffer.size())*/,
        true, &errStr, &errLine, &errColumn))
    {
        return false;
    }
    bool res = Parser_DomToStu(doc, lhConfig);

    processBassMgr();
    buildSpeakerMap();
    updateSpeakerVector();   //刷新扬声器列表

    return res;
}

bool Lhc::saveToBuf(std::wstring& buf)
{
    QDomDocument doc;
    if(!Parser_StuToDom(doc, lhConfig))
    {
        return false;
    }

    buf = doc.toString().toStdWString();

    //没有必要？
    processBassMgr();
    buildSpeakerMap();
    return true;
}

bool Lhc::saveToBuf(std::string& buf)
{
    QDomDocument doc;
    if(!Parser_StuToDom(doc,lhConfig))
    {
        return false;
    }
    buf = doc.toString().toStdString();

    //没有必要？
    processBassMgr();
    buildSpeakerMap();
    return true;
}

Lhc::Lhc()
{

}

Lhc::~Lhc()
{

}

bool Lhc::autoEqResultFromBuf(char* buf, int length)
{
    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(QByteArray(buf, length),
        true, &errStr, &errLine, &errColumn))
    {
        return false;
    }

    //Clear all element for receive new element from XML buffer
    eqResponseCurveVector.clear();

    QDomElement root = doc.documentElement();
    QDomNodeList l = root.elementsByTagName(RESPONSEPOINT);
    for (int j = 0; j < l.size(); j++)
    {
        LHC::_responsePoint responsePoint;
        QDomElement info = l.at(j).toElement();
        QDomElement tmp = info.firstChildElement(GAINDB);
        if(tmp.isElement())
        {
            responsePoint.gainDb = tmp.text().toFloat();
        }
        else
        {
            responsePoint.gainDb=0;
        }
        tmp = info.firstChildElement(FREQUENCYHZ);
        if(tmp.isElement())
        {
            responsePoint.frequencyHz = tmp.text().toFloat();
        }
        else
        {
            responsePoint.frequencyHz=0;
        }
        eqResponseCurveVector.push_back(responsePoint);
    }
    return true;
}

bool Lhc::autoEqResultToBuf(std::string &buf)
{
    QDomDocument doc;
    QDomElement info = doc.createElement(EQRESPONSECURVE);
    doc.appendChild(info);
    //eqResponseCurveVector
    for (unsigned int j = 0; j < eqResponseCurveVector.size(); j++)
    {
        LHC::_responsePoint rp = eqResponseCurveVector[j];
        QDomElement dimensions = doc.createElement(RESPONSEPOINT);
        info.appendChild(dimensions);
        QDomElement tmp = doc.createElement(GAINDB);
        QDomText txt;
        //if(!rp.gainDb.empty())
        {
            txt = doc.createTextNode(QString::number(rp.gainDb));
            tmp.appendChild(txt);
        }
        dimensions.appendChild(tmp);
        tmp = doc.createElement(FREQUENCYHZ);
        //if(!rp.frequencyHz.empty())
        {
            txt = doc.createTextNode(QString::number(rp.frequencyHz));
            tmp.appendChild(txt);
        }
        dimensions.appendChild(tmp);
    }
    buf = doc.toString().toStdString();
    return true;
}


bool Lhc::processBassMgr()
{
   PRINT_FUN_TRACE

    //引用可以大幅缩短单行代码长度，by lizulin 20180425
    LHC::_roomConfiguration& roomConfiguration=lhConfig.processorConfiguration.roomConfiguration;
    LHC::_roomConfiguration::_room& room=lhConfig.processorConfiguration.roomConfiguration.room;
    LHC::_outputConfiguration& outputConfiguration=lhConfig.processorConfiguration.outputConfiguration;

    LHC::_eqConfiguration& eqConfiguration=lhConfig.processorConfiguration.eqConfiguration;
    LHC::_metadata& metadata=lhConfig.metadata;
    LHC::EQCoeffInfo& eqAllInfo=lhConfig.eqALLInfo;

    LHC::_speakerSpecsVector& speakerSpecsVector=lhConfig.processorConfiguration.roomConfiguration.speakerSpecsVector;
    //Clear bassMgrChn
    bassMgrChn.clear();

    //PRINT_TRACE

    PreDprintf("speakerEndpointVector.size()=%d\n",(int)room.region.speakerEndpointVector.size());
    PreDprintf("outputConfiguration.outputRouteVector.size()=%d\n",(int)outputConfiguration.outputRouteVector.size());

    //Find subWoofer with type
    for(unsigned int i = 0; i <room.region.speakerEndpointVector.size(); i++)
    {
        const LHC::_speakerEndpoint& spkEndpointItem=room.region.speakerEndpointVector[i];

 

        //区分LFE与Sub,仅仅对Sub做低音管理,LFE不需要，忽略大小写
        if (Comp(spkEndpointItem.type,SPKTYPE_SUBWOOFER_WS)
            &&(!QString::fromStdWString(spkEndpointItem.name).startsWith("LFE",Qt::CaseInsensitive)))
        {
 
            //Convert subWoofer spkId to Dout

            for(unsigned int j = 0; j <outputConfiguration.outputRouteVector.size(); j++)
            {
                const LHC::_outputRoute& outputRouteItem=outputConfiguration.outputRouteVector[j];
 

                if(spkEndpointItem.id==outputRouteItem.speakerEndpointRef)
                {
                    LHC::bassManagmentChannal bmc;
                    //swscanf(outputRouteItem.outputRef.c_str(),L"OUT-BL-%d", &bmc.subWooferDout);
                    
                    bmc.subWooferDout=outputRouteItem.outputRef;   //refactor
                     
                    //ARM GCC ERROR
                    //wchar_t tmp[128]={0};
                    //int tmpId=outputRouteItem.speakerEndpointRef;
                    //swprintf(tmp,L"SPKREP-%d",tmpId);
                    //bmc.id =tmp;//outputRouteItem.speakerEndpointRef;

                    char tmp[128]={0};
                    int tmpId=outputRouteItem.speakerEndpointRef;
                    sprintf(tmp,"SPKREP-%d",tmpId);
                    bmc.id =StringToWString(std::string(tmp));
                    bassMgrChn.push_back(bmc);
                    break;
                }
            }
        }
    }



    //Alloc speaker to subWoofer
    for(unsigned int i = 0; i <room.region.speakerEndpointVector.size(); i++)
    {
        const LHC::_speakerEndpoint& spkEndpointItem=room.region.speakerEndpointVector[i];

 
        //if (spkEndpointItem.type != SPKTYPE_SUBWOOFER_WS)
        //区分LFE与Sub,仅仅对Sub做低音管理,LFE不需要，忽略大小写
        if (spkEndpointItem.type!=SPKTYPE_SUBWOOFER_WS)
            //&&(!QString::fromStdWString(spkEndpointItem.name).startsWith("LFE",Qt::CaseInsensitive)))
        {
 

            for(unsigned int j = 0; j < bassMgrChn.size(); j++)
            {
                LHC::bassManagmentChannal& bassItem=bassMgrChn[j];

 
                PreDprintf("spkEndpointItem.bassManagementVector.size()=%d\n",(int)spkEndpointItem.bassManagementVector.size());
   
                for(unsigned int m = 0; m <spkEndpointItem.bassManagementVector.size(); m++)
                {
 
                    const LHC::_bassManagement& bassMgrItem=spkEndpointItem.bassManagementVector[m];
                    if(bassItem.id==bassMgrItem.subwooferRef)
                    {
                        PreDprintf("bassMgrChn[%d].frqHz=%d\n",j,bassMgrItem.crossoverFrequencyHz);
                        bassItem.frqHz.push_back(bassMgrItem.crossoverFrequencyHz);
                        for(unsigned int k = 0; k <outputConfiguration.outputRouteVector.size(); k++)
                        {
                             const LHC::_outputRoute& outputRouteItem=outputConfiguration.outputRouteVector[k];
                            if(spkEndpointItem.id ==outputRouteItem.speakerEndpointRef)
                            {
                                //int dout = 0;
                                //swscanf(outputRouteItem.outputRef.c_str(),L"OUT-BL-%d", &dout);
                                int dout=outputRouteItem.outputRef;
                                bassItem.managementDout.push_back(dout);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    PRINT_TRACE
    //////////////////////////////////////////////////////////////////////////
    printf("-------------------------BassMgrChn:------------------------\n");
    for (size_t i = 0; i < bassMgrChn.size(); i++)
    {
        //error
        //wprintf(L"id: %s frq: %d sub: %d\n", (const wchar_t*)(bassMgrChn[i].id.c_str()), bassMgrChn[i].frqHz, bassMgrChn[i].subWooferDout);

        const LHC::bassManagmentChannal&  bassItem=bassMgrChn[i];
        wprintf(L"id: %s sub: %d ", (const wchar_t*)(bassItem.id.c_str()),bassItem.subWooferDout);
        for (size_t n=0;n<bassItem.frqHz.size();n++)
        {
            printf("freq[%d]:%d ",(int)n,bassItem.frqHz[n]);
        }
        printf("\n");

        for(size_t j= 0; j <bassItem.managementDout.size(); j++)
        {
            printf("Dout[%d]:%d ",(int)j,bassItem.managementDout[j]);
        }
        printf("\n");
    }
    //////////////////////////////////////////////////////////////////////////
    return true;
}


#include <mutex>

bool Lhc::buildSpeakerMap()
{
   PRINT_FUN_TRACE

    ////此法不可靠 再改加锁 by lizulin
    //// 生成音箱->分组映射表 [11/25/2016 killerlife]
    //static int bProcess = 0;
    ////////////////////////////////////////////////////////////////////////////
    ////Muti-Thread Manual Mutex
    //if(bProcess == 1)
    //{
    //    while (bProcess)
    //    {
    //        QThread::msleep(10);
    //    }
    //    return true;
    //}
    //bProcess = 1;
    ////////////////////////////////////////////////////////////////////////////

    static std::mutex m_mtxLHC;
    std::lock_guard<std::mutex> lock(m_mtxLHC);   //构建过程加锁,其他地方待续 

    //引用可以大幅缩短单行代码长度，by lizulin 20180425
    LHC::_roomConfiguration& roomConfiguration=lhConfig.processorConfiguration.roomConfiguration;
    LHC::_roomConfiguration::_room& room=lhConfig.processorConfiguration.roomConfiguration.room;
    LHC::_outputConfiguration& outputConfiguration=lhConfig.processorConfiguration.outputConfiguration;
    LHC::_eqConfiguration& eqConfiguration=lhConfig.processorConfiguration.eqConfiguration;
    LHC::_metadata& metadata=lhConfig.metadata;
    LHC::EQCoeffInfo& eqAllInfo=lhConfig.eqALLInfo;




    speakerMap.clear();
    for(unsigned int i = 0; i <room.region.speakerEndpointVector.size(); i++)
    {
        LHC::_speakerEndpoint& spkEndpoint=room.region.speakerEndpointVector[i];
        //改引用可以大幅缩短代码长度 by lizulin

        LHC::speakerMapPoint smp;

        //Find the array
        for(unsigned int j = 0; j <spkEndpoint.arrayVector.size(); j++)
        {
            const std::wstring& arrayName =spkEndpoint.arrayVector[j];
          
            //低效可以在改进  by lizulin
            if(arrayName == L"L")
                smp.array.push_back(C_L);
            else if(arrayName == L"R")
                smp.array.push_back(C_R);
            else if(arrayName == L"C")
                smp.array.push_back(C_C);
            else if(arrayName == L"LFE")
                smp.array.push_back(C_LFE);
            else if(arrayName == L"Lc")
                smp.array.push_back(C_LC);
            else if(arrayName == L"Rc")
                smp.array.push_back(C_RC);
            else if(arrayName == L"Ls")
                smp.array.push_back(C_LS);
            else if(arrayName == L"Lss")
                smp.array.push_back(C_LSS);
            else if(arrayName == L"Lrs")
                smp.array.push_back(C_LRS);
            else if(arrayName == L"Rrs")
                smp.array.push_back(C_RRS);
            else if(arrayName == L"Rs")
                smp.array.push_back(C_RS);
            else if(arrayName == L"Rss")
                smp.array.push_back(C_RSS);
            else if(arrayName == L"Lts")
                smp.array.push_back(C_LTS);
            else if(arrayName == L"Rts")
                smp.array.push_back(C_RTS);
            else if(arrayName == L"Lw")
                smp.array.push_back(C_LW);
            else if(arrayName == L"Rw")
                smp.array.push_back(C_RW);
            else if(arrayName == L"Ltf")
                smp.array.push_back(C_LTF);
            else if(arrayName == L"Rtf")
                smp.array.push_back(C_RTF);
            else if(arrayName == L"Lh")
                smp.array.push_back(C_LH);
            else if(arrayName == L"Rh")
                smp.array.push_back(C_RH);
            else if(arrayName == L"Ch")
                smp.array.push_back(C_CH);
            else if(arrayName == L"Lsh")
                smp.array.push_back(C_LSH);
            else if(arrayName == L"Rsh")
                smp.array.push_back(C_RSH);
            else if(arrayName == L"VOG")
                smp.array.push_back(C_VOG);
            else if(arrayName == L"W")
                smp.array.push_back(C_W);
            else if(arrayName == L"X")
                smp.array.push_back(C_X);
            else if(arrayName == L"Y")
                smp.array.push_back(C_Y);
            else if(arrayName == L"Z")
                smp.array.push_back(C_Z);

       // DOLBY DAC中有Subwoofer分组，但标准中没有，暂时把Subwoofer归到LFE中管理 [11/25/2016 killerlife]
      // Disable for CFG [4/14/2018 leonis]
       else if(arrayName == L"Sub")
        smp.array.push_back(C_SUB);
            // End [11/25/2016 killerlife]
        }


        //Convert spkId to Dout
        for(unsigned int j = 0; j <outputConfiguration.outputRouteVector.size(); j++)
        {
            if(spkEndpoint.id ==outputConfiguration.outputRouteVector[j].speakerEndpointRef)
            {
                //swscanf(outputConfiguration.outputRouteVector[j].outputRef.c_str(),
                //    L"OUT-BL-%d", &smp.dout);
                smp.dout=outputConfiguration.outputRouteVector[j].outputRef;
                break;
            }
        }



        smp.id=spkEndpoint.id;      //改id为int 类型，保存到文件加SPKREP前缀   20181112 
        smp.x =spkEndpoint.position.x;
        smp.y =spkEndpoint.position.y;
        smp.z =spkEndpoint.position.z;
        smp.delay=spkEndpoint.delay;  

        smp.strName =spkEndpoint.name;
        smp.type=spkEndpoint.type;

        MapSpkNameID[smp.strName]=smp.dout;

        speakerMap.push_back(smp);
    }
    // End [11/25/2016 killerlife]

    // 生成分组->音箱映射表 [11/25/2016 killerlife]
    arrayMap.clear();
    for(int i = 0; i < 0x1c; i++)
    {
        LHC::arrayPoint ap;
        arrayMap.push_back(ap);
    }
    for(unsigned int i = 0; i < speakerMap.size(); i++)
    {
        for (unsigned int j = 0; j < speakerMap[i].array.size(); j++)
        {
            if(speakerMap[i].array[j] == C_SUB)
            {
                break;
            }
            arrayMap[speakerMap[i].array[j]].dout.push_back(speakerMap[i].dout);
        }
    }

    // End [11/25/2016 killerlife]

     bulidMapofSpeakerChannelNum();   // add by lizulin 20171202
#if 0
    printf("L: ");
    for(int i = 0; i < arrayMap[C_L].dout.size(); i++)
        printf("%d ", arrayMap[C_L].dout[i]);
    printf("\n");
    printf("R: ");
    for(int i = 0; i < arrayMap[C_R].dout.size(); i++)
        printf("%d ", arrayMap[C_R].dout[i]);
    printf("\n");
    printf("C: ");
    for(int i = 0; i < arrayMap[C_C].dout.size(); i++)
        printf("%d ", arrayMap[C_C].dout[i]);
    printf("\n");
    printf("LFE: ");
    for(int i = 0; i < arrayMap[C_LFE].dout.size(); i++)
        printf("%d ", arrayMap[C_LFE].dout[i]);
    printf("\n");
    printf("LS: ");
    for(int i = 0; i < arrayMap[C_LS].dout.size(); i++)
        printf("%d ", arrayMap[C_LS].dout[i]);
    printf("\n");
    printf("RS: ");
    for(int i = 0; i < arrayMap[C_RS].dout.size(); i++)
        printf("%d ", arrayMap[C_RS].dout[i]);
    printf("\n");
    printf("LSS: ");
    for(int i = 0; i < arrayMap[C_LSS].dout.size(); i++)
        printf("%d ", arrayMap[C_LSS].dout[i]);
    printf("\n");
    printf("RSS: ");
    for(int i = 0; i < arrayMap[C_RSS].dout.size(); i++)
        printf("%d ", arrayMap[C_RSS].dout[i]);
    printf("\n");
    printf("LRS: ");
    for(int i = 0; i < arrayMap[C_LRS].dout.size(); i++)
        printf("%d ", arrayMap[C_LRS].dout[i]);
    printf("\n");
    printf("RRS: ");
    for(int i = 0; i < arrayMap[C_RRS].dout.size(); i++)
        printf("%d ", arrayMap[C_RRS].dout[i]);
    printf("\n");
    printf("LTS: ");
    for(int i = 0; i < arrayMap[C_LTS].dout.size(); i++)
        printf("%d ", arrayMap[C_LTS].dout[i]);
    printf("\n");
    printf("RTS: ");
    for(int i = 0; i < arrayMap[C_RTS].dout.size(); i++)
        printf("%d ", arrayMap[C_RTS].dout[i]);
    printf("\n");
#endif
    //bProcess = 0;
    return true;
}

void Lhc::printSpeakerMap()
{
    PRINT_FUN_TRACE
    for(unsigned int i = 0; i < this->speakerMap.size();i++)
    {
        const LHC::speakerMapPoint& m=this->speakerMap.at(i);
        //为了显示从1开始 +1;
        printf("Index:%-3d  Speaker: id:%-3d dout:%-3d delay:%f ms ",i,m.id,m.dout,m.delay);
        std::wcout<<" name: "<<m.strName<<std::endl;
    }
}



void Lhc::bulidMapofSpeakerChannelNum()
{
   PRINT_FUN_TRACE
   printf("Lhc::bulidMapofSpeakerChannelNum() 构建映射表\n");
   GetSpkChMap()->init();   //初始化为默认值
   GetSpkChMap()->SpkNum=speakerMap.size();

   for(unsigned int i = 0; i < this->speakerMap.size();i++)
   {
       const LHC::speakerMapPoint& m=this->speakerMap.at(i);

       int ChNum=m.id;          //cardinput声卡声道号码
       int SpkNum=m.dout;    //最后实际输出的扬声器号码
       //存入互相映射的数组
       GetSpkChMap()->SpktoCh[SpkNum]=ChNum;
       GetSpkChMap()->ChtoSpk[ChNum]=SpkNum;
    }
   GetSpkChMap()->updateMaxDout();
   GetSpkChMap()->printSpkMap();
}


int Lhc::getBedout(int bedNum)
{
    static const int ArraySNum[10]={S_L,S_R,S_C,S_LFE,S_LSS,S_RSS,S_LRS,S_RRS,S_LTS,S_RTS};
    static const int ArrayCNum[10]={C_L,C_R,C_C,C_LFE,C_LSS,C_RSS,C_LRS,C_RRS,C_LTS,C_RTS};
    int CNum=ArraySNum[bedNum];
    if (this->arrayMap[CNum].dout.empty())
    {
        return -1;
    }
    return this->arrayMap[CNum].dout[0];
}




//int Lhc::getChannelNum(unsigned int SpkNum)
//{
//    if (SpkNum>(sizeof(SpktoCh)/sizeof(SpktoCh[0])))  //256
//    {
//        return -1;
//    }
//    return SpktoCh[SpkNum];
//}
//
//int Lhc::getSpeakerNum(unsigned int ChNum)
//{
//    if (ChNum>(sizeof(ChtoSpk)/sizeof(ChtoSpk[0])))  //256
//    {
//        return -1;
//    }
//    return ChtoSpk[ChNum];
//}


void Lhc::getRoomDimension(float& length,float& width,float& height)
{
   LHC::_roomConfiguration::_room& room=lhConfig.processorConfiguration.roomConfiguration.room;
   //old
   //width=_wtof(room.dimensions.screenWallWidth.c_str());
   //len=_wtof(room.dimensions.houseLeftWallWidth.c_str());
   //height=_wtof(room.dimensions.ceilingElevationAtScreen.c_str());


   //float BoothWallLength=_wtof(room.dimensions.boothWallWidth.c_str());             //后墙宽度
   //float ScreenWallLength=_wtof(room.dimensions.screenWallWidth.c_str());           //前墙宽度 银幕侧
   //float LeftRightWallLength=_wtof(room.dimensions.houseLeftWallWidth.c_str());   //左右墙高度

   // float  CeilingElevationatscreen=_wtof(room.dimensions.ceilingElevationAtScreen.c_str());        //银幕侧处天花板高度
   // float CeilingElevationat23point=_wtof(room.dimensions.ceilingElevationAtTwoThirds.c_str());  //2/3处天花板高度 
   // float CeilingElevationatbooth=_wtof(room.dimensions.ceilingElevationAtBooth.c_str());           //后墙处天花板高度

    float BoothWallLength=room.dimensions.boothWallWidth;             //后墙宽度
    float ScreenWallLength=room.dimensions.screenWallWidth;           //前墙宽度 银幕侧
    float LeftRightWallLength=room.dimensions.houseLeftWallWidth;   //左右墙高度

    float  CeilingElevationatscreen=room.dimensions.ceilingElevationAtScreen;        //银幕侧处天花板高度
    float CeilingElevationat23point=room.dimensions.ceilingElevationAtTwoThirds;  //2/3处天花板高度 
    float CeilingElevationatbooth=room.dimensions.ceilingElevationAtBooth;           //后墙处天花板高度

    width=std::max((double)BoothWallLength,(double)ScreenWallLength);	
    length=LeftRightWallLength;
    //影厅高度等于天花板高度CeilingElevation的最大值，CeilingElevation大于FloorElevation
    //Room.height=std::max(p->CeilingElevationatscreen,p->CeilingElevationatbooth);

    //在几种高度中找到最大值作为影厅高度
    height=0;
    height=std::max((double)height,(double)CeilingElevationatscreen);
    height=std::max((double)height,(double)CeilingElevationat23point);
    height=std::max((double)height,(double)CeilingElevationatbooth);
}


void Lhc::updateSpeakerVector()
{
    getRoomDimension(roomSpkInfo.roomLength,roomSpkInfo.roomWidth,roomSpkInfo.roomHeight);

    roomSpkInfo.ListSpkInfo.clear();
    for (unsigned int i = 0; i < speakerMap.size(); i++)
    {
        const LHC::speakerMapPoint& sp = speakerMap[i];
        Speaker sc;
        sc.x =sp.x;
        sc.y = sp.y;
        sc.z = sp.z;
        sc.channel=sp.dout;
        sc.dout=sp.dout;
        sc.id=sp.id;
        sc.delay=sp.delay;

        int lenName=sp.strName.length();
        if (lenName>sizeof(sc.name))
        {
            lenName=sizeof(sc.name);
        }
        memcpy(sc.name,sp.strName.c_str(),lenName);
        memcpy(sc.name,sc.name,lenName);   //name 与comment暂时相同

        sc.special = 0;
        for(unsigned int j = 0; j < sp.array.size(); j++)
        {
            if(sp.array[j] == C_LFE)
            {
                sc.special = 1;
            }
            else if(sp.array[j] == C_SUB)
            {
                sc.special = 1;
            }
        }
       roomSpkInfo.ListSpkInfo.push_back(sc);
    }
}


void Lhc::updateSpkName()
{
    PRINT_FUN_TRACE
    std::map<std::wstring,LHC::EQGroupOne>& eqALL=lhConfig.eqALLInfo.EQALL;
    std::map<std::wstring,LHC::EQGroupOne>::iterator it;      //所有均衡分组数据
    for (it=eqALL.begin();it!=eqALL.end();it++)
    {
        LHC::EQGroupOne& eqGroupOne=it->second;
        std::map<std::wstring, LHC::_EQTypeAll>::iterator itone;
        for(size_t n = 0; n < this->speakerMap.size();n++)
        {
            //从影厅布局配置里面刷新扬声器名称与实际输出声道号
            LHC::speakerMapPoint& m=this->speakerMap.at(n);
            LHC::_EQTypeAll tmp=eqGroupOne.speakerEQ[m.strName];     //不存在 会创建
            std::string sSpkName=WStringToString(m.strName);
            tmp.channel=m.dout;   //刷新实际id
            tmp.sSpkName=sSpkName;
            //tmp.spkDelayMs=m.delay;
            tmp.updateInfo();

            eqGroupOne.speakerEQ[m.strName]=tmp;  //存回map

            printf("Lhc::updateSpkName() SpkName:%s SpkCn:%d \n",sSpkName.c_str(),tmp.channel);
        }

        //避免从配置文件读到不合规范的扬声器名称，如其他影厅配置文件混用等造成的，在此清理
        for (itone=eqGroupOne.speakerEQ.begin();itone!=eqGroupOne.speakerEQ.end();itone++)
        {
            bool isRightName=false;
            for(size_t n = 0; n < this->speakerMap.size();n++)
            {
                if (itone->first==this->speakerMap.at(n).strName)
                {
                    isRightName=true;
                    break;
                }
//#ifdef OS_ARM_LINUX
                // 为了分频选择扬声器开放所有的未使用音箱
                else
                {
                    const std::wstring& s = itone->first;
                    if(wcsstr(s.c_str(),L"Undef")!=NULL)
                    {
                        isRightName = true;
                        break;
                    }
                }
//#endif
            }
            if (!isRightName)
            {
                //eqGroupOne.speakerEQ.erase(itone);
                printf("is Error SpkName eraser it! Name:%s\n",itone->first.c_str());
                itone=eqGroupOne.speakerEQ.erase(itone);
            }
        }
        //////////////////////////////////////////////////////////////////////////
        // 对新的配置文件，也需要打开未分配的音箱用于分频处理 [6/10/2020 jaontolt]
#ifdef OS_ARM_LINUX
        int MaxSpk = 64;
#else
        int MaxSpk = 128;
#endif
        std::vector<int> totalSpk;
        for(int i = 0; i<MaxSpk; i++)
        {
            totalSpk.push_back(i);
        }
        itone = eqGroupOne.speakerEQ.begin();
        while(itone != eqGroupOne.speakerEQ.end())
        {
            const LHC::_EQTypeAll& eqSpkOne=itone->second;
            std::vector<int>::iterator itr = totalSpk.begin();
            while (itr != totalSpk.end())
            {
                if(*itr == eqSpkOne.channel)
                {
                    itr = totalSpk.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
            itone++;
        }
        for(int i = 0; i < totalSpk.size(); i++)
        {
            std::wstring sSpkName;
            LHC::_EQTypeAll eqTypeTmp;  //
            wchar_t buf[64];
            swprintf(buf,64, L"Undef %03d", totalSpk[i]+1);
            sSpkName = buf;
            eqTypeTmp.updateInfo();
            eqTypeTmp.channel = totalSpk[i];
            eqGroupOne.speakerEQ[sSpkName]=eqTypeTmp;
        }
        //////////////////////////////////////////////////////////////////////////

        //阵列名称固定为大写   Bed的前4个 L/R /C/LFE 称为main 主扬声器,后六个称为array
        static const std::string sBedName[10]={"L","R","C","LFE","LSS","RSS","LRS","RRS","LTS","RTS"};
        std::wstring wsBedName[10];  //不想每个字符串都加L
        for (int n=0;n<10;n++)
        {
            wsBedName[n]=StringToWString(sBedName[n]);
        }
        for (int n=4;n<10;n++)
        {
            std::wstring sName=wsBedName[n];
            LHC::_EQTypeAll tmp=eqGroupOne.arrayEQ[sName];       //不存在 会创建
            tmp.channel=n;                                                                 //刷新实际id,
            eqGroupOne.arrayEQ[sName]=tmp;                                  //存回map
        }

        //避免从配置文件读到不合规范的阵列名称，在此清理
        for (itone=eqGroupOne.arrayEQ.begin();itone!=eqGroupOne.arrayEQ.end();itone++)
        {
            bool isRightName=false;
            for (int i=4;i<10;i++)
            {
                if (itone->first==wsBedName[i])
                {
                    isRightName=true;
                    break;
                }
            }
            if (!isRightName)
            {
                //eqGroupOne.arrayEQ.erase(itone);
                 printf("is Error ArrayName eraser it! Name:%s\n",itone->first.c_str());
                itone=eqGroupOne.arrayEQ.erase(itone);
            }
        }
    }
}

std::vector<std::string> Lhc::getEQGroupNameList()
{
    PRINT_FUN_TRACE
    std::vector<std::string> VGroupNameList;
    std::map<std::wstring,LHC::EQGroupOne>& eqALL=lhConfig.eqALLInfo.EQALL;
    std::map<std::wstring,LHC::EQGroupOne>::iterator it;      //所有均衡分组数据
    for (it=eqALL.begin();it!=eqALL.end();it++)
    {
        const std::wstring& sGroupName=it->first;
        VGroupNameList.push_back(WStringToString(sGroupName));
    }
    if (VGroupNameList.empty())
    {
        PreDprintf("GroupName isEmpty add DefaultGroupName!\n");
        const std::string sCustom=sDefaultGroupName;
        VGroupNameList.push_back(sCustom);
        LHC::EQGroupOne tmp;
        const std::wstring wsCustom=StringToWString(sCustom);
        eqALL.insert(std::map<std::wstring,LHC::EQGroupOne>::value_type(wsCustom,tmp));
    }
    updateSpkName();  //为新建或已经存在的分组刷新扬声器名称和id dout

    PRINT_FUN_NAME
    for (unsigned int n=0;n<VGroupNameList.size();n++)
    {
        printf("GroupName[%u]=%s\n",n,VGroupNameList.at(n).c_str());
    }
    return VGroupNameList;
}

std::string Lhc::getCurrentEQGroupName() //当前使用的eq配置分组名称,保证有返回
{
   PRINT_FUN_TRACE
    std::wstring sName=lhConfig.eqALLInfo.CurrentEQGroupName;
    if (sName.empty())
    {
        sName=StringToWString(sDefaultGroupName);
        lhConfig.eqALLInfo.CurrentEQGroupName=sName;  //初始化默认配置

        PreDprintf("Name empty set to default=%s\n",WStringToString(sName).c_str());
    }


    PreDprintf("Name:%s\n",WStringToString(sName).c_str());
    return WStringToString(sName);
}

void Lhc::setCurrentEQGroupName(const std::string& s)
{
    lhConfig.eqALLInfo.CurrentEQGroupName=StringToWString(s);
    
    PreDprintf("Name:%s\n",StringToWString(s).c_str());
}

LHC::EQGroupOne Lhc::getEQGroupOne(std::string sEQGroupName)
{
    std::wstring wsEQGroupName=StringToWString(sEQGroupName);
    std::map<std::wstring,LHC::EQGroupOne>& eqALL=lhConfig.eqALLInfo.EQALL;
    std::map<std::wstring,LHC::EQGroupOne>::iterator it;      //所有均衡分组数据
    for (it=eqALL.begin();it!=eqALL.end();it++)
    {
        if (it->first==wsEQGroupName)
        {
            return it->second;
        }
    }

     //不存在则创建,已存在键将忽略,而非值覆盖
    LHC::EQGroupOne tmp;
    //eqALL.insert(std::map<std::wstring,LHC::EQGroupOne>::value_type(wsEQGroupName,tmp));
    eqALL[wsEQGroupName]=tmp;

    printf("Lhc::getEQGroupOne() Not exist and Creat GroupName:%s\n",sEQGroupName.c_str());

    updateSpkName();  //为新建或已经存在的分组刷新扬声器名称和id dout
    lhConfig.eqALLInfo.CurrentEQGroupName=wsEQGroupName;   //初始化一个默认Custom配置

    return  eqALL[wsEQGroupName];
    // printf("Lhc::getEQGroupOne() Failed GroupName:%s\n",sEQGroupName.c_str());
    // return LHC::EQGroupOne();
}

void Lhc::setSpkEqCoeff(const LHC::EQSpeakerOne& tmp)
{
    PRINT_FUN_TRACE
    std::map<std::wstring,LHC::EQGroupOne>& eqALL=lhConfig.eqALLInfo.EQALL;  
    if(tmp.isArray)
    {
        eqALL[tmp.sGroupName].arrayEQ[tmp.sSpkName]=tmp.eqTypeAll;
    }
    else
    {
        eqALL[tmp.sGroupName].speakerEQ[tmp.sSpkName]=tmp.eqTypeAll;

    }
}

void Lhc::addEQGroupName(const std::string& sEQGroupName)
{
    PreDprintf("Name:%s\n",sEQGroupName.c_str());
    std::map<std::wstring,LHC::EQGroupOne>& eqALL=lhConfig.eqALLInfo.EQALL;
    std::map<std::wstring,LHC::EQGroupOne>::iterator it;      //所有均衡分组数据
    LHC::EQGroupOne tmp;
    const std::wstring wsEQGroupName=StringToWString(sEQGroupName);
    //插入新的分组，存作则不做任何存作
    eqALL.insert(std::map<std::wstring,LHC::EQGroupOne>::value_type(wsEQGroupName,tmp));  

    updateSpkName();  //为新建或已经存在的分组刷新扬声器名称和id dout
}

void Lhc::DeleteEQGroupName(const std::string& sEQGroupName)
{
    PreDprintf("Name:%s\n",sEQGroupName.c_str());
    std::wstring wsEQGroupName=StringToWString(sEQGroupName);
    std::map<std::wstring,LHC::EQGroupOne>& eqALL=lhConfig.eqALLInfo.EQALL;
    std::map<std::wstring,LHC::EQGroupOne>::iterator it;      //所有均衡分组数据

    for (it=eqALL.begin();it!=eqALL.end();it++)
    {
        if (it->first==wsEQGroupName)
        {
            //eqALL.erase(it);
            it=eqALL.erase(it);
            break;
        }
    }
}

void Lhc::addSpkInfo(PerSpkInfo perSpkInfo,std::string sSpkDeviceName,ESpkType spkType)
{
    if (sSpkDeviceName.empty())
    {
        return;
    }
    if (!isValidEnum(spkType))
    {
        return;
    }
    if (sSpkDeviceName==perSpkInfo.sFullName)        //名称正确
    {
        lhConfig.mapSpkTable[sSpkDeviceName]=perSpkInfo;   //不存在则创建,存在则覆盖
    }
}

PerSpkInfo Lhc::getSpkInfo(std::string sSpkDeviceName,ESpkType spkType)
{
    if (sSpkDeviceName.empty())
    {
        return PerSpkInfo();
    }
    std::map<std::string,PerSpkInfo>::iterator it;
    it =lhConfig.mapSpkTable.find(sSpkDeviceName);
    if(it!=lhConfig.mapSpkTable.end())
    {
        return it->second;
    }
    return PerSpkInfo();
}

void Lhc::clearSpkInfo()
{
    lhConfig.mapSpkTable.clear();
}

void Lhc::addAmpInfo(PerAmpInfo perAmpInfo,std::string sAmpDeviceName)
{
    if (sAmpDeviceName==perAmpInfo.sFullName)        //名称正确
    {
        lhConfig.mapAmpTable[sAmpDeviceName]=perAmpInfo;   //不存在则创建,存在则覆盖
    }
}

PerAmpInfo Lhc::getAmpInfo(std::string sAmpDeviceName)
{
    if (sAmpDeviceName.empty())
    {
        return PerAmpInfo();
    }

    std::map<std::string,PerAmpInfo>::iterator it;
    it =lhConfig.mapAmpTable.find(sAmpDeviceName);
    if(it!=lhConfig.mapAmpTable.end())
    {
        return it->second;
    }
    return PerAmpInfo();
}

void Lhc::clearAmpInfo()
{
    lhConfig.mapAmpTable.clear();
}

bool Lhc::CopyEQGroupByName(const std::string& destName, const std::string& srcName)
{
    std::wstring srcGroupName = StringToWString(srcName);
    std::map<std::wstring,LHC::EQGroupOne>& eqALL=lhConfig.eqALLInfo.EQALL;
    std::map<std::wstring,LHC::EQGroupOne>::iterator it;      //所有均衡分组数据

    LHC::EQGroupOne srcGroup;
    bool bFound = false;
    for (it=eqALL.begin();it!=eqALL.end();it++)
    {
        if (it->first==srcGroupName)
        {
            srcGroup = it->second;
            bFound = true;
            break;
        }
    }

    //源不存在则返回 false;
    if(!bFound)
        return false;
    std::wstring destGroupName = StringToWString(destName);
    for(it=eqALL.begin(); it!=eqALL.end(); it++)
    {
        if(it->first == destGroupName)
        {
            it->second = srcGroup;
            return true;
        }
    }

    eqALL[destGroupName] = srcGroup;
    return true;
}

//
//void Lhc::setSpkInfo(PerSpkInfo perSpkInfo,std::string sSpkDeviceName,ESpkType spkType)
//{
//    if(sSpkDeviceName=="")
//    {
//        return;
//    }
//    if (sSpkDeviceName=="default")
//    {
//        setDefaultSpkInfo(perSpkInfo,spkType);
//        return;
//    }
//    //不存在则创建,已存在键将忽略,而非值覆盖
//    //mapTableSpecialSpk.insert(std::map<std::string,PerSpkInfo>::value_type(sSpkDeviceName,perSpkInfo));
//
//    if (sSpkDeviceName==perSpkInfo.sFullName)   //名称正确
//    {
//        mapTableSpecialSpk[sSpkDeviceName]=perSpkInfo;   //不存在则创建,存在则覆盖
//    }
//}
//
//PerSpkInfo Lhc::getSpkInfo(std::string sSpkDeviceName,ESpkType spkType)
//{
//    if(sSpkDeviceName=="")
//    {
//        return PerSpkInfo();
//    }
//    if (sSpkDeviceName=="default")
//    {
//        return getDefaultSpkInfo(spkType);
//    }
//    std::map<std::string,PerSpkInfo>::iterator it;
//    it = mapTableSpecialSpk.find(sSpkDeviceName);
//    if(it!=mapTableSpecialSpk.end())
//    {
//        return it->second;
//    }
//    return PerSpkInfo();
//}
//
//void Lhc::setAmpInfo(PerAmpInfo perAmpInfo,std::string sAmpDeviceName,ESpkType spkType)
//{
//    if(sAmpDeviceName=="")
//    {
//        return;
//    }
//    if (sAmpDeviceName=="default")
//    {
//        setDefaultAmpInfo(perAmpInfo,spkType);
//        return;
//    }
//    //不存在则创建,已存在键将忽略,而非值覆盖
//    //mapTableSpecialAmp.insert(std::map<std::string,PerAmpInfo>::value_type(sAmpDeviceName,perAmpInfo));
//
//    if (sAmpDeviceName==perAmpInfo.sFullName)   //名称正确
//    {
//        mapTableSpecialAmp[sAmpDeviceName]=perAmpInfo;   //不存在则创建,存在则覆盖
//    }
//}
//
//PerAmpInfo Lhc::getAmpInfo(std::string sAmpDeviceName,ESpkType spkType)
//{
//    if(sAmpDeviceName=="")
//    {
//        return PerAmpInfo();
//    }
//    if (sAmpDeviceName=="default")
//    {
//        return getDefaultAmpInfo(spkType);
//    }
//    std::map<std::string,PerAmpInfo>::iterator it;
//    it = mapTableSpecialAmp.find(sAmpDeviceName);
//    if(it!=mapTableSpecialAmp.end())
//    {
//        return it->second;
//    }
//    return PerAmpInfo();
//}
//
//void Lhc::setDefaultSpkInfo(PerSpkInfo perSpkInfo,ESpkType spkType)
//{
//    if (!isValidEnum(spkType))
//    {
//        return;
//    }
//    defaultUsedSpk[spkType]=perSpkInfo;
//}
//
//PerSpkInfo Lhc::getDefaultSpkInfo(ESpkType spkType)
//{
//    if (!isValidEnum(spkType))
//    {
//        return PerSpkInfo();
//    }
//    return defaultUsedSpk[spkType];
//}
//
//void Lhc::setDefaultAmpInfo(PerAmpInfo perAmpInfo,ESpkType spkType)
//{
//    if (!isValidEnum(spkType))
//    {
//        return;
//    }
//    defaultUsedAmp[spkType]=perAmpInfo;
//}
//
//PerAmpInfo Lhc::getDefaultAmpInfo(ESpkType spkType)
//{
//    if (!isValidEnum(spkType))
//    {
//        return PerAmpInfo();
//    }
//    return defaultUsedAmp[spkType];
//}

//bool prjFileParser(QDomDocument &doc, PRJ::MPU_PROJECT* prjConfig);
//bool prjToXml(QDomDocument &doc, PRJ::MPU_PROJECT& prjConfig);

mpuprj* GetMpu()
{
    //单例对象,放函数外更高效,但无法保证使用时已构造完成(main函数前被调用),//若单例之间依赖复杂为保证构造顺序就放函数内,并为构造时线程安全C++11之前要加锁 C++11之后由编译器保证 by lizulin
    static mpuprj gmpuprj;
    return &gmpuprj;
}

mpuprj::mpuprj()
{

}

mpuprj::~mpuprj()
{

}




bool mpuprj::loadFromBufTmp(const char* buf, int length)
{
    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(QByteArray(buf, length),
        true, &errStr, &errLine, &errColumn))
    {
        return false;
    }

    //bool res = prjFileParser(doc, tmpProject);
    bool res = Parser_DomToStu(doc,tmpProject);
    return res;
}

bool mpuprj::loadFromFileTmp(const char* fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }
    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, true, &errStr, &errLine, &errColumn))
    {
        file.close();
        printf("%s %d %d \n", errStr.toStdString().c_str(), errLine, errColumn);
        return false;
    }
    file.close();

    //bool res = prjFileParser(doc, tmpProject);
    bool res = Parser_DomToStu(doc,tmpProject);
    return res;
}

bool mpuprj::saveToFileTmp(const char* fileName)
{
    QDomDocument doc;
    if(!Parser_StuToDom(doc, tmpProject))
    {
        return false;
    }

    if (isNullCstr(fileName))
    {
        PreDprintf(" Project driver:%s\n",tmpProject.basic.driver.c_str());
        std::string name = tmpProject.basic.driver + tmpProject.basic.name + "\\" + tmpProject.basic.name + ".mpu";
        QFile savefile(name.c_str());
        savefile.open(QFile::ReadWrite|QFile::Text|QFile::Truncate);
        QTextStream out(&savefile);
        doc.save(out, 2);
        savefile.close();
    }
    else
    {
        QFile savefile(fileName);
        savefile.open(QFile::ReadWrite|QFile::Text|QFile::Truncate);
        QTextStream out(&savefile);
        doc.save(out, 2);
        savefile.close();
    }

    return true;
}

bool mpuprj::saveToBufTmp(std::string& buf)
{
    QDomDocument doc;
    if(!Parser_StuToDom(doc, tmpProject))
    {
        return false;
    }
    buf = doc.toString().toStdString();
    return true;
}

#include <time.h>
//////////////////////////////////////////////////////////////////////////
// 修改支持同步对齐 [8/9/2017 killerlife]
// 2个参数：begin - LTC开始时码转成的帧号，end - LTC结束时码转成的帧号
bool mpuprj::encodeFinish(unsigned int begin,unsigned int end,unsigned int SampleRate,unsigned int FrameRate)
{
    printf("rmuprj::encodeFinish() begin:%u end:%u SRate:%u FRate:%u\n",begin,end,SampleRate,FrameRate);

    std::string src = strPmName;
    src.erase(0, 9);
    std::vector<PRJ::PMITEM>::iterator itor;

    //也存一份到basic
    //sprintf(buf,"%d",SampleRate);    //可以改int to string
    mpuProject.basic.samplerate=SampleRate;
    //sprintf(buf,"%d",FrameRate);   
    mpuProject.basic.framerate=FrameRate;

    bool bFind = false;
    for (itor = mpuProject.pm.begin(); itor != mpuProject.pm.end(); itor++)
    {
        std::string name = (*itor).name;
        if(name.compare(0, 9, "YYYYMMDD_") == 0)
        {
            name.erase(0, 9);
            if(name == src)
            {
                bFind = true;
                PRJ::PMITEM item;
                item.name = strPmName;
                char buf[20]={0};

                item.rec_begin=TimeCodeNumtoString(begin,FrameRate);
                item.rec_end=TimeCodeNumtoString(end,FrameRate);
                printf("strRecordTime: sBegin:%s sEnd:%s nBgein:%u nEnd:%u\n",item.rec_begin.c_str(), item.rec_end.c_str(),begin,end);

                //item.duration = itoa(frames, buf, 10);   //非标准库不可移植 review by lizulin
                int frames = end - begin + 1;
                sprintf(buf,"%d",frames);   //vs sprintf_s  _spnrintf  gcc spnrintf
                item.duration=buf;
                item.create_time = itor->create_time;
                item.description = itor->description;

                item.samplerate=SampleRate;
                item.framerate=FrameRate;

                time_t t;
                time(&t);
                //item.modify_time = itoa(t, buf, 10);
                item.modify_time=IntToString((uint64_t)t);   //todo



                //mpuProject.pm.erase(itor);
                itor=mpuProject.pm.erase(itor);
                std::vector<PRJ::PMITEM>::iterator itor1;
                int vecSize = mpuProject.pm.size();
                int i = 0;
                for(itor1 = mpuProject.pm.begin(); itor1 != mpuProject.pm.end();)
                {
                    if(itor1->name.empty())
                        break;
                    if((*itor1).name == item.name)
                    {
                        itor1=mpuProject.pm.erase(itor1);   //返回下一个有效迭代器  by lizulin
                    }
                    else
                    {
                        itor1++;
                    }
                }
                mpuProject.pm.push_back(item);
                break;
            }
        }
    }
    if(!bFind)
    {
        PRJ::PMITEM item;
        time_t t;
        time(&t);
        char buf[20]={0};
        item.name = strPmName;

        item.rec_begin =TimeCodeNumtoString(begin,FrameRate);
        item.rec_end = TimeCodeNumtoString(end,FrameRate);
        printf("strRecordTime: sBegin:%s sEnd:%s nBgein:%u nEnd:%u\n",item.rec_begin.c_str(), item.rec_end.c_str(),begin,end);

        int frames = end - begin + 1;
        //item.duration = itoa(frames, buf, 10);
        sprintf(buf,"%d",frames);   //vs sprintf_s  _spnrintf  gcc spnrintf
        item.duration=buf;

        //item.create_time = itoa(t, buf, 10);
        item.create_time =IntToString((uint64_t)t);
        item.modify_time = item.create_time;
        item.description = item.name;

        //多帧率支持
        item.samplerate=SampleRate;
        item.framerate=FrameRate;

        std::vector<PRJ::PMITEM>::iterator itor1;
        int vecSize = mpuProject.pm.size();
        int i = 0;
        for(itor1 = mpuProject.pm.begin(); itor1 != mpuProject.pm.end();)
        {
            if(itor1->name.empty())
                break;
            if((*itor1).name == item.name)
            {
                itor1=mpuProject.pm.erase(itor1);   //返回下一个有效迭代器
            }
            else
                itor1++;
        }

        mpuProject.pm.push_back(item);
    }
    return saveToFile();
}
//////////////////////////////////////////////////////////////////////////

std::string mpuprj::getPMorMxfFullPath()
{
    return strPmOrMxf;
}

std::string mpuprj::getPMorMxfName()
{
    return strPmName;
}


bool mpuprj::Parser_DomToStu(const QDomDocument &doc,PRJ::MPU_PROJECT& prjConfig)
{
   // PRJ::MPU_PROJECT& prjConfig=*prjConfigIN;   //引用传参不可靠？

    QDomElement root = doc.documentElement();
    QDomElement basic = root.firstChildElement("Basic");
    QDomElement filter = root.firstChildElement("Filter");
    QDomElement downmix = root.firstChildElement("Downmix");
    QDomElement printmaster = root.firstChildElement("PrintMaster");
    QDomElement mxf = root.firstChildElement("MXF");

    //Get basic info from configuration file
    QDomElement tmp = basic.firstChildElement("Name");
    if(tmp.isElement())
        prjConfig.basic.name = tmp.text().toStdString();
    else
        prjConfig.basic.name.clear();
    tmp = basic.firstChildElement("Description");
    if (tmp.isElement())
        prjConfig.basic.description = tmp.text().toStdString();
    else
        prjConfig.basic.description.clear();
    tmp = basic.firstChildElement("CreateDateTime");
    if(tmp.isElement())
        prjConfig.basic.create_time = tmp.text().toStdString();
    else
        prjConfig.basic.create_time = "0";
    tmp = basic.firstChildElement("ModifyDateTime");
    if(tmp.isElement())
        prjConfig.basic.modify_time = tmp.text().toStdString();
    else
        prjConfig.basic.modify_time = "0";

    //多帧率采样率支持 也存一份到basic
    tmp = basic.firstChildElement("SampleRate");
    if(tmp.isElement())
        prjConfig.basic.samplerate = tmp.text().toInt(); //totoStdString();
    else
        prjConfig.basic.samplerate  =SRateDefault;   //取不到默认48000 兼容以前的工程
    tmp = basic.firstChildElement("FrameRate");
    if(tmp.isElement())
        prjConfig.basic.framerate = tmp.text().toInt(); //totoStdString();
    else
        prjConfig.basic.framerate =SRateDefault;   //取不到默认24HZ 兼容以前的工程

    tmp = basic.firstChildElement("Driver");
    if(tmp.isElement())
    {
        prjConfig.basic.driver = tmp.text().toStdString();
    }
    else
    {
        prjConfig.basic.driver.clear();
    }
    PreDprintf(" Project driver:%s\n",prjConfig.basic.driver.c_str());

    //Get Filter info from configuration file
    tmp = filter.firstChildElement("LPF");
    QDomElement t = tmp.firstChildElement("Frequency");
    if(t.isElement())
        prjConfig.filter.lpf.frequency = t.text().toFloat();
    else
        prjConfig.filter.lpf.frequency = 0.0f;
    t = tmp.firstChildElement("Policy");
    if(t.isElement())
        prjConfig.filter.lpf.policy = t.text().toStdString();
    else
        prjConfig.filter.lpf.policy.clear();
    tmp = filter.firstChildElement("HPF");
    t = tmp.firstChildElement("Frequency");
    if(t.isElement())
        prjConfig.filter.hpf.frequency = t.text().toFloat();
    else
        prjConfig.filter.hpf.frequency = 0.0f;
    t = tmp.firstChildElement("Policy");
    if(t.isElement())
        prjConfig.filter.hpf.policy = t.text().toStdString();
    else
        prjConfig.filter.hpf.policy.clear();

    //Get Downmix info from configuration file
    //TODO:

    //Get PrintMaster info from configuration file
    QDomNodeList list = printmaster.elementsByTagName("PMItem");
    prjConfig.pm.clear();
    for(int i = 0; i < list.size(); i++)
    {
        t = list.at(i).toElement();
        PRJ::PMITEM comments;
        tmp = t.firstChildElement("Name");
        if(tmp.isElement())
            comments.name = tmp.text().toStdString();
        else
            comments.name.clear();
        tmp = t.firstChildElement("Description");
        if(tmp.isElement())
            comments.description = tmp.text().toStdString();
        else
            comments.description.clear();
        tmp = t.firstChildElement("Duration");
        if(tmp.isElement())
            comments.duration = tmp.text().toStdString();
        else
            comments.duration = "0";
        tmp = t.firstChildElement("CreateDateTime");
        if(tmp.isElement())
            comments.create_time = tmp.text().toStdString();
        else
            comments.create_time = "0";
        tmp = t.firstChildElement("ModifyDateTime");
        if(tmp.isElement())
            comments.modify_time = tmp.text().toStdString();
        else
            comments.modify_time = "0";

        //多帧率采样率支持
        tmp = t.firstChildElement("SampleRate");
        if(tmp.isElement())
            comments.samplerate = tmp.text().toInt();   //.toStdString();
        else
            comments.samplerate  =SRateDefault;   //取不到默认48000 兼容以前的工程
        tmp = t.firstChildElement("FrameRate");
        if(tmp.isElement())
            comments.framerate = tmp.text().toInt();   //.toStdString();
        else
            comments.framerate =FRateDefault;   //取不到默认24HZ 兼容以前的工程

        //////////////////////////////////////////////////////////////////////////
        // 修改工程文件以支持同步对齐调整 [8/4/2017 killerlife]
        tmp = t.firstChildElement("beginMark");
        if(tmp.isElement())
            comments.begin_mark = tmp.text().toStdString();
        else
            comments.begin_mark = "00:00:00:00";
        tmp = t.firstChildElement("SyncPop");
        if(tmp.isElement())
            comments.sync_pop = tmp.text().toStdString();
        else
            comments.sync_pop = "00:00:00:00";
        tmp = t.firstChildElement("RecStart");
        if(tmp.isElement())
            comments.rec_begin = tmp.text().toStdString();
        else
            comments.rec_begin = "00:00:00:00";
        tmp = t.firstChildElement("RecEnd");
        if(tmp.isElement())
            comments.rec_end = tmp.text().toStdString();
        else
            comments.rec_end = "00:00:00:00";
        tmp = t.firstChildElement("Delay");
        if(tmp.isElement())
            comments.delay = tmp.text().toStdString();
        else
            comments.delay = "0";
        //////////////////////////////////////////////////////////////////////////
        tmp = t.firstChildElement("SplitPolicy");
        QDomNodeList l = tmp.elementsByTagName("Reel");
        for(int j = 0; j < l.size(); j++)
        {
            QDomElement a = l.at(j).toElement();
            PRJ::SPLITPOLICY sp;
            sp.number = a.attribute("number").toInt();
            QDomElement b = a.firstChildElement("Start");
            if(b.isElement())
                sp.begin = b.text().toStdString();
            else
                sp.begin = "-1";
            b = a.firstChildElement("End");
            if(b.isElement())
                sp.end = b.text().toStdString();
            else
                sp.end = "-1";
            comments.split_policy.push_back(sp);
        }
        prjConfig.pm.push_back(comments);
    }

    //Get MXF info from configuration file
    prjConfig.reel.clear();
    list = mxf.elementsByTagName("Reel");
    for(int i = 0; i < list.size(); i++)
    {
        tmp = list.at(i).toElement();
        PRJ::REELINFO ri;
        ri.number = tmp.attribute("number").toInt();
        t = tmp.firstChildElement("Name");
        if(t.isElement())
            ri.name = t.text().toStdString();
        else
            ri.name.clear();
        t = tmp.firstChildElement("Description");
        if(t.isElement())
            ri.descripion = t.text().toStdString();
        else
            ri.descripion.clear();
        t = tmp.firstChildElement("Duration");
        if (t.isElement())
            ri.duration = t.text().toStdString();
        else
            ri.duration = "0";
        t = tmp.firstChildElement("CreateDateTime");
        if(t.isElement())
            ri.create_time = t.text().toStdString();
        else
            ri.create_time = "0";
        t = tmp.firstChildElement("ModifyDateTime");
        if(t.isElement())
            ri.modify_time = t.text().toStdString();
        else
            ri.modify_time = "0";
        prjConfig.reel.push_back(ri);
    }
    return true;
}

bool mpuprj::Parser_StuToDom(QDomDocument &doc, const PRJ::MPU_PROJECT& prjConfig)
{
    QDomProcessingInstruction inst = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");
    doc.appendChild(inst);
    QDomElement rmu = doc.createElement("mpu");
    doc.appendChild(rmu);
    //Basic
    QDomElement basic = doc.createElement("Basic");
    rmu.appendChild(basic);
    QDomElement tmp = doc.createElement("Name");
    basic.appendChild(tmp);
    QDomText txt;
    if(!prjConfig.basic.name.empty())
    {
        txt = doc.createTextNode(QString::fromStdString(prjConfig.basic.name));
        tmp.appendChild(txt);
    }
    tmp = doc.createElement("Description");
    basic.appendChild(tmp);
    if(!prjConfig.basic.description.empty())
    {
        txt = doc.createTextNode(QString::fromStdString(prjConfig.basic.description));
        tmp.appendChild(txt);
    } 
    tmp = doc.createElement("CreateDateTime");
    basic.appendChild(tmp);
    if(!prjConfig.basic.create_time.empty())
    {
        txt = doc.createTextNode(QString::fromStdString(prjConfig.basic.create_time));
        tmp.appendChild(txt);
    } 
    tmp = doc.createElement("ModifyDateTime");
    basic.appendChild(tmp);
    if(!prjConfig.basic.modify_time.empty())
    {
        txt = doc.createTextNode(QString::fromStdString(prjConfig.basic.modify_time));
        tmp.appendChild(txt);
    } 

    //多帧率采样率支持--basic也存一份
    tmp = doc.createElement("SampleRate");
    basic.appendChild(tmp);
    //if(!prjConfig.basic.samplerate.empty())
    {
        //txt = doc.createTextNode(QString::fromStdString(prjConfig.basic.samplerate));
        txt = doc.createTextNode(QString::number(prjConfig.basic.samplerate));
        tmp.appendChild(txt);
    } 
    tmp = doc.createElement("FrameRate");
    basic.appendChild(tmp);
    //if(!prjConfig.basic.framerate.empty())
    {
        //txt = doc.createTextNode(QString::fromStdString(prjConfig.basic.framerate));
        txt = doc.createTextNode(QString::number(prjConfig.basic.framerate));
        tmp.appendChild(txt);
    } 

    tmp = doc.createElement("Driver");
    basic.appendChild(tmp);
    if(!prjConfig.basic.driver.empty())
    {
        txt = doc.createTextNode(QString::fromStdString(prjConfig.basic.driver));
        tmp.appendChild(txt);
    } 

    //Filter
    QDomElement filter = doc.createElement("Filter");
    rmu.appendChild(filter);
    //info
    QDomElement f = doc.createElement("LPF");
    filter.appendChild(f);
    QDomElement t = doc.createElement("Frequency");
    f.appendChild(t);
    txt = doc.createTextNode(QString::number(prjConfig.filter.lpf.frequency));
    t.appendChild(txt);
    t = doc.createElement("Policy");
    f.appendChild(t);
    if(!prjConfig.filter.lpf.policy.empty())
    {
        txt = doc.createTextNode(QString::fromStdString(prjConfig.filter.lpf.policy));
        t.appendChild(txt);
    }
    f = doc.createElement("HPF");
    filter.appendChild(f);
    t = doc.createElement("Frequency");
    f.appendChild(t);
    txt = doc.createTextNode(QString::number(prjConfig.filter.hpf.frequency));
    t.appendChild(txt);
    t = doc.createElement("Policy");
    f.appendChild(t);
    if(!prjConfig.filter.hpf.policy.empty())
    {
        txt = doc.createTextNode(QString::fromStdString(prjConfig.filter.hpf.policy));
        t.appendChild(txt);
    }

    //Downmix，暂时没想好
    //TODO:
    QDomElement downmix = doc.createElement("Downmix");
    rmu.appendChild(downmix);

    //PrintMaster
    QDomElement printmaster = doc.createElement("PrintMaster");
    rmu.appendChild(printmaster);
    for(unsigned int i = 0; i < prjConfig.pm.size(); i++)
    {
        QDomElement item = doc.createElement("PMItem");
        printmaster.appendChild(item);
        tmp = doc.createElement("Name");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].name.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].name));
            tmp.appendChild(txt);
        }
        tmp = doc.createElement("Description");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].description.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].description));
            tmp.appendChild(txt);
        }
        tmp = doc.createElement("Duration");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].duration.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].duration));
            tmp.appendChild(txt);
        }
        tmp = doc.createElement("CreateDateTime");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].create_time.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].create_time));
            tmp.appendChild(txt);
        }
        tmp = doc.createElement("ModifyDateTime");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].modify_time.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].modify_time));
            tmp.appendChild(txt);
        }

        //多帧率采样率支持
        tmp = doc.createElement("SampleRate");
        item.appendChild(tmp);
        //if(!prjConfig.pm[i].samplerate.empty())
        {
            //txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].samplerate));
            unsigned int SampleRate=prjConfig.pm[i].samplerate;
            txt = doc.createTextNode(QString::number(SampleRate));
            tmp.appendChild(txt);
            //printf("Save toXml: SampleRate:%u str:%s\n",SampleRate,QString::number(SampleRate).toStdString().c_str());
        } 
        tmp = doc.createElement("FrameRate");
        item.appendChild(tmp);
        //if(!prjConfig.pm[i].framerate.empty())
        {
            //txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].framerate));
            txt = doc.createTextNode(QString::number(prjConfig.pm[i].framerate));
            tmp.appendChild(txt);
        } 


        //////////////////////////////////////////////////////////////////////////
        // 修改工程文件格式以支持同步对齐调整 [8/4/2017 killerlife]
        tmp = doc.createElement("StartMark");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].begin_mark.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].begin_mark));
            tmp.appendChild(txt);
        }
        tmp = doc.createElement("SyncPop");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].sync_pop.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].sync_pop));
            tmp.appendChild(txt);
        }
        tmp = doc.createElement("RecStart");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].rec_begin.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].rec_begin));
            tmp.appendChild(txt);
        }
        tmp = doc.createElement("RecEnd");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].rec_end.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].rec_end));
            tmp.appendChild(txt);
        }
        tmp = doc.createElement("Delay");
        item.appendChild(tmp);
        if(!prjConfig.pm[i].delay.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].delay));
            tmp.appendChild(txt);
        }
        //////////////////////////////////////////////////////////////////////////
        tmp = doc.createElement("SplitPolicy");
        item.appendChild(tmp);
        t = doc.createElement("ReelCounts");
        tmp.appendChild(t);
        txt = doc.createTextNode(QString::number(prjConfig.pm[i].split_policy.size()));
        t.appendChild(txt);
        for(unsigned int j = 0; j < prjConfig.pm[i].split_policy.size(); j++)
        {
            QDomElement reel = doc.createElement("Reel");
            tmp.appendChild(reel);
            reel.setAttribute("number", prjConfig.pm[i].split_policy[j].number);
            t = doc.createElement("Start");
            reel.appendChild(t);
            if(!prjConfig.pm[i].split_policy[j].begin.empty())
            {
                txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].split_policy[j].begin));
                t.appendChild(txt);
            }
            t = doc.createElement("End");
            reel.appendChild(t);
            if(!prjConfig.pm[i].split_policy[j].end.empty())
            {
                txt = doc.createTextNode(QString::fromStdString(prjConfig.pm[i].split_policy[j].end));
                t.appendChild(txt);
            }
        }
    }

    //MXF
    QDomElement mxf = doc.createElement("MXF");
    rmu.appendChild(mxf);
    for(unsigned int i = 0; i < prjConfig.reel.size(); i++)
    {
        tmp = doc.createElement("Reel");
        mxf.appendChild(tmp);
        tmp.setAttribute("number", prjConfig.reel[i].number);
        t = doc.createElement("Name");
        tmp.appendChild(t);
        if(!prjConfig.reel[i].name.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.reel[i].name));
            t.appendChild(txt);
        }
        t = doc.createElement("Description");
        tmp.appendChild(t);
        if(!prjConfig.reel[i].descripion.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.reel[i].descripion));
            t.appendChild(txt);
        }
        t = doc.createElement("Duration");
        tmp.appendChild(t);
        if(!prjConfig.reel[i].duration.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.reel[i].duration));
            t.appendChild(txt);
        }
        t = doc.createElement("CreateDateTime");
        tmp.appendChild(t);
        if(!prjConfig.reel[i].create_time.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.reel[i].create_time));
            t.appendChild(txt);
        }
        t = doc.createElement("ModifyDateTime");
        tmp.appendChild(t);
        if(!prjConfig.reel[i].modify_time.empty())
        {
            txt = doc.createTextNode(QString::fromStdString(prjConfig.reel[i].modify_time));
            t.appendChild(txt);
        }
    }
    return true;
}

bool mpuprj::loadFromFile(const char* fileName)
{
    printf("mpuprj::loadFromFile()\n");

    QFile file((fileName));
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        printf(" QFile open() failed()\n");
        return false;
    }
    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, true, &errStr, &errLine, &errColumn))
    {
        file.close();
        printf("%s %d %d \n", errStr.toStdString().c_str(), errLine, errColumn);
        return false;
    }
    file.close();

    bool res = Parser_DomToStu(doc,mpuProject);
    return res;
}

bool mpuprj::loadFromBuf(const char* buf, int length)
{
    printf("mpuprj::loadFromBuf()\n");

    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(QByteArray(buf, length),true, &errStr, &errLine, &errColumn))
    {
        return false;
    }
    bool res = Parser_DomToStu(doc,this->mpuProject);

     PreDprintf("driver:%s name:%s\n",mpuProject.basic.driver.c_str(),mpuProject.basic.name.c_str());
    return res;
}

bool mpuprj::loadFromBuf(const std::wstring& buf)
{
    QString s;
    s.fromStdWString(buf);

    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(s/*QByteArray(xmlBuffer.c_str(), xmlBuffer.size())*/,
        true, &errStr, &errLine, &errColumn))
    {
        return false;
    }
    //bool res = prjFileParser(doc, rmuProject);
    bool res = Parser_DomToStu(doc,mpuProject);
    return res;
}

bool mpuprj::loadFromBuf(const std::string& buf)
{
    QString s;
    s.fromStdString(buf);

    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(s/*QByteArray(xmlBuffer.c_str(), xmlBuffer.size())*/,
        true, &errStr, &errLine, &errColumn))
    {
        return false;
    }
    //bool res = prjFileParser(doc, rmuProject);
    bool res = Parser_DomToStu(doc,mpuProject);
    return res;
}

bool mpuprj::saveToFile(const char* fileName)
{
    printf("mpuprj::saveToFile()\n");

    QDomDocument doc;
    if(!Parser_StuToDom(doc, mpuProject))
    {
        return false;
    }

    if (isNullCstr(fileName))
    {
        PreDprintf(" Project driver:%s\n",mpuProject.basic.driver.c_str());

        std::string name = mpuProject.basic.driver + mpuProject.basic.name + "\\" + mpuProject.basic.name + ".mpu";
        QFile savefile(name.c_str());
        savefile.open(QFile::ReadWrite|QFile::Text|QFile::Truncate);
        QTextStream out(&savefile);
        doc.save(out, 2);
        savefile.close();
    }
    else
    {
        QFile savefile(fileName);
        if(!savefile.open(QFile::ReadWrite|QFile::Text|QFile::Truncate))
        {
            printf(" QFile open() failed()\n");
        }
        QTextStream out(&savefile);
        doc.save(out, 2);
        savefile.close();
    }

    return true;
}

bool mpuprj::saveToBuf(std::wstring& buf)
{
    QDomDocument doc;
    if(!Parser_StuToDom(doc, mpuProject))
    {
        return false;
    }

    buf = doc.toString().toStdWString();
    return true;
}

bool mpuprj::saveToBuf(std::string& buf)
{
    QDomDocument doc;
    if(!Parser_StuToDom(doc, mpuProject))
    {
        return false;
    }
    buf = doc.toString().toStdString();
    return true;
}



setting* Getsetting()
{
    //单例对象,放函数外更高效,但无法保证使用时已构造完成(main函数前被调用),//若单例之间依赖复杂为保证构造顺序就放函数内,并为构造时线程安全C++11之前要加锁 C++11之后由编译器保证 by lizulin
    static setting gSetting;
    return &gSetting;
}

setting::setting()
{

}

setting::~setting()
{

}

bool setting::loadFromFile(const char* fileName)
{
    QString name;
    if(isNullCstr(fileName))
    {
        name = "./setting.xml";
    }
    else
    {
        name = fileName;
    }
    QFile file(name);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }
    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, true, &errStr, &errLine, &errColumn))
    {
        file.close();
        printf("%s %d %d \n", errStr.toStdString().c_str(), errLine, errColumn);
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomElement tmp = root.firstChildElement("GlobalGain");
    if(tmp.isElement())
        set.globalGain = tmp.text().toFloat();
    else
        set.globalGain = 0.0f;
    tmp = root.firstChildElement("GlobalDelay");
    if(tmp.isElement())
        set.globalDelay = tmp.text().toFloat();
    else
        set.globalDelay = 0.0f;
    tmp = root.firstChildElement("LtcChase");
    if(tmp.isElement())
        set.ltcChase = tmp.text().toFloat();
    else
        set.ltcChase = 0.0f;
    tmp = root.firstChildElement("Reel1FFOA");
    if(tmp.isElement())
        set.ltcChase = tmp.text().toFloat();
    else
        set.ltcChase = 0;
    tmp = root.firstChildElement("DcsSync");
    if(tmp.isElement())
        set.ltcChase = tmp.text().toInt();
    else
        set.ltcChase = 0;
    tmp = root.firstChildElement("Reel1StartMark");
    if(tmp.isElement())
        set.ltcChase = tmp.text().toFloat();
    else
        set.ltcChase = 0;
    tmp = root.firstChildElement("AsioFile");
    if(tmp.isElement())
        strncpy(set.asioFile, tmp.text().toStdString().c_str(),
        tmp.text().toStdString().size()>500?500:tmp.text().toStdString().size());
    else
        memset(set.asioFile, 0, sizeof(set.asioFile));
    tmp = root.firstChildElement("KeyFile");
    if(tmp.isElement())
        strncpy(set.keyFile, tmp.text().toStdString().c_str(),
        tmp.text().toStdString().size()>500?500:tmp.text().toStdString().size());
    else
        memset(set.keyFile, 0, sizeof(set.keyFile));
    tmp = root.firstChildElement("AudioProcessorIp");
    if(tmp.isElement())
        strncpy(set.ipObia, tmp.text().toStdString().c_str(),
        tmp.text().toStdString().size()>500?500:tmp.text().toStdString().size());
    else
        memset(set.ipObia, 0, sizeof(set.ipObia));
    tmp = root.firstChildElement("DCSIp");
    if(tmp.isElement())
        strncpy(set.ipCinema, tmp.text().toStdString().c_str(),
        tmp.text().toStdString().size()>500?500:tmp.text().toStdString().size());
    else
        memset(set.ipCinema, 0, sizeof(set.ipCinema));
    tmp = root.firstChildElement("PmDir");
    if(tmp.isElement())
        strncpy(set.pmDirectory, tmp.text().toStdString().c_str(),
        tmp.text().toStdString().size()>500?500:tmp.text().toStdString().size());
    else
        memset(set.pmDirectory, 0, sizeof(set.pmDirectory));
    return true;
}

bool setting::saveToFile(const char* fileName)
{
    QDomDocument doc;

    QDomProcessingInstruction inst = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");
    doc.appendChild(inst);
    QDomElement rmu = doc.createElement("Setting");
    doc.appendChild(rmu);

    //Basic
    QDomText txt;
    QDomElement tmp = doc.createElement("GlobalGain");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString::number(set.globalGain));
    tmp.appendChild(txt);

    tmp = doc.createElement("GlobalDelay");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString::number(set.globalDelay));
    tmp.appendChild(txt);

    tmp = doc.createElement("LtcChase");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString::number(set.ltcChase));
    tmp.appendChild(txt);

    tmp = doc.createElement("DcsSync");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString::number(set.dcssync));
    tmp.appendChild(txt);

    tmp = doc.createElement("Reel1beginMark");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString::number(set.reel1beginmark));
    tmp.appendChild(txt);

    tmp = doc.createElement("Reel1FFOA");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString::number(set.reel1ffoa));
    tmp.appendChild(txt);

    tmp = doc.createElement("AsioFile");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString(set.asioFile));
    tmp.appendChild(txt);

    tmp = doc.createElement("KeyFile");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString(set.keyFile));
    tmp.appendChild(txt);

    tmp = doc.createElement("AudioProcessorIp");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString(set.ipObia));
    tmp.appendChild(txt);

    tmp = doc.createElement("DCSIp");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString(set.ipCinema));
    tmp.appendChild(txt);

    tmp = doc.createElement("PmDir");
    rmu.appendChild(tmp);
    txt = doc.createTextNode(QString(set.pmDirectory));
    tmp.appendChild(txt);

    if(fileName!=NULL)
    {
        QFile savefile(fileName);
        savefile.open(QFile::ReadWrite|QFile::Text|QFile::Truncate);
        QTextStream out(&savefile);
        doc.save(out, 2);
        savefile.close();
    }
    else
    {
        std::string name = "./setting.xml";
        QFile savefile(name.c_str());
        savefile.open(QFile::ReadWrite|QFile::Text|QFile::Truncate);
        QTextStream out(&savefile);
        doc.save(out, 2);
        savefile.close();
    }

    return true;
}



SysKeyInfo* GetSysKeyInfo()
{
    static SysKeyInfo gSysKeyInfo;
    return &gSysKeyInfo;
}

SysKeyInfo::SysKeyInfo()
{
    //loadFromFile(default_UserInfoPath);
}

SysKeyInfo::~SysKeyInfo()
{
    //saveToFile(default_UserInfoPath);
}

bool SysKeyInfo::parser_StuToDom(QDomDocument& doc,const KeyInfo& SKInfo)
{
    QDomProcessingInstruction inst = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");
    doc.appendChild(inst);

    QDomElement ElementRoot = doc.createElement("Root");
    doc.appendChild(ElementRoot);

    QDomElement ElementTitle = doc.createElement("Title");
    ElementRoot.appendChild(ElementTitle);
    QDomText text;
    text = doc.createTextNode("Leonis HoloSound DCAP System User information file!");
    ElementTitle.appendChild(text);

    QDomElement ElementImportant = doc.createElement("Important");
    ElementRoot.appendChild(ElementImportant);
    text = doc.createTextNode("Do not edit by the other tool otherwise the system can not work properly!");
    ElementImportant.appendChild(text);


    QDomElement ElementUserInfo = doc.createElement("UserInfo");
    ElementRoot.appendChild(ElementUserInfo);

    for (std::size_t n=0;n<KeyInfo::User_Total;n++)
    {
        QDomElement ElementItem = doc.createElement("UserItem");
        ElementItem.setAttribute("ID",(unsigned int)(n+1));
        ElementUserInfo.appendChild(ElementItem);
        QDomElement ElementUserName = doc.createElement("UserName");
        ElementItem.appendChild(ElementUserName);
        text = doc.createTextNode(QString::fromStdString(SKInfo.perUser[n].sUserName));
        ElementUserName.appendChild(text);

        QDomElement ElementPassWord = doc.createElement("PassWord");
        ElementItem.appendChild(ElementPassWord);
        text = doc.createTextNode(QString::fromStdString(SKInfo.perUser[n].sPassWord));
        ElementPassWord.appendChild(text);

        QDomElement ElementLastEditTime = doc.createElement("LastEditTime");
        ElementItem.appendChild(ElementLastEditTime);
        text = doc.createTextNode(QString::fromStdString(SKInfo.perUser[n].sLastEditTime));
        ElementLastEditTime.appendChild(text);
    }
    return true;
}



bool SysKeyInfo::parser_DomToStu(const QDomDocument& doc,KeyInfo& SKInfo)
{
    QDomElement ElementRoot = doc.documentElement();
    QDomElement ElementUserInfo = ElementRoot.firstChildElement("UserInfo");


    QDomNodeList list = ElementUserInfo.elementsByTagName("UserItem");
    for(int i = 0; i < list.size(); i++)
    {
        std::string sUserName;
        std::string sPassWord;
        std::string sLastEditTime;

        QDomElement t = list.at(i).toElement();
        QDomElement EleTmp = t.firstChildElement("UserName");
        if(EleTmp.isElement())
            sUserName= EleTmp.text().toStdString();
        EleTmp = t.firstChildElement("PassWord");
        if(EleTmp.isElement())
            sPassWord= EleTmp.text().toStdString();

        EleTmp = t.firstChildElement("LastEditTime");
        if(EleTmp.isElement())
            sLastEditTime= EleTmp.text().toStdString();

        if (sUserName=="admin")
        {
            SKInfo.perUser[KeyInfo::User_admin].sPassWord=sPassWord;
            SKInfo.perUser[KeyInfo::User_admin].sLastEditTime=sLastEditTime;
        }
        else if (sUserName=="users")
        {
            SKInfo.perUser[KeyInfo::User_users].sPassWord=sPassWord;
            SKInfo.perUser[KeyInfo::User_users].sLastEditTime=sLastEditTime;
        }

        //else if (sUserName=="technician")
        //{
        //    SKInfo.perUser[KeyInfo::User_admin].sPassWord=sPassWord;
        //    SKInfo.perUser[KeyInfo::User_admin].sLastEditTime=sLastEditTime;
        //}
        //else if (sUserName=="manager")
        //{
        //    SKInfo.perUser[KeyInfo::User_admin].sPassWord=sPassWord;
        //    SKInfo.perUser[KeyInfo::User_admin].sLastEditTime=sLastEditTime;
        //}
        //else if (sUserName=="operator")
        //{
        //    SKInfo.perUser[KeyInfo::User_admin].sPassWord=sPassWord;
        //    SKInfo.perUser[KeyInfo::User_admin].sLastEditTime=sLastEditTime;
        //}
    }

    return true;
}

bool SysKeyInfo::loadFromFile(const char* fileName)
{
    QFile file((fileName));
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        return false;
    }
    QString errStr;
    int errLine;
    int errColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, true, &errStr, &errLine, &errColumn))
    {
        file.close();
        printf("%s %d %d \n", errStr.toStdString().c_str(), errLine, errColumn);
        return false;
    }
    file.close();
    bool res = parser_DomToStu(doc,keyInfo);
    return res;
}

bool SysKeyInfo::saveToFile(const char* fileName)
{
    QDomDocument doc;
    if(!parser_StuToDom(doc, keyInfo))
    {
        return false;
    }

    QFile savefile(fileName);
    if (!savefile.open(QFile::ReadWrite|QFile::Text|QFile::Truncate))
    {
        return false;
    }
    
    QTextStream out(&savefile);
    doc.save(out, 2);
    savefile.close();
    return true;
}

ELoginPrivileges SysKeyInfo::checkLogin(std::string sUserName,std::string sPassWord)
{
    printf("SysKeyInfo::checkLogin() sUserName:%s sPassWord:%s \n",sUserName.c_str(),sPassWord.c_str());
    for (std::size_t n=0;n<KeyInfo::User_Total;n++)
    {
        if (keyInfo.perUser[n].sUserName==sUserName)
        {
            if (keyInfo.perUser[n].sPassWord==sPassWord)
            {
                return (ELoginPrivileges)n;   //返回匹配类型
            }
        }
    }
    printf("Dcap-UI login failed \n");
    return LoginPri_failed;
}


bool SysKeyInfo::ModifyPassWord(std::string sUserName,std::string sPassWordOld,std::string sPassWordNew)
{
    printf("SysKeyInfo::checkLogin() sUserName:%s sPassWordOld:%s sPassWordNew:%s \n",sUserName.c_str(),sPassWordOld.c_str(),sPassWordNew.c_str());
    for (std::size_t n=0;n<KeyInfo::User_Total;n++)
    {
        if (keyInfo.perUser[n].sUserName==sUserName)
        {
            if (keyInfo.perUser[n].sPassWord==sPassWordOld)
            {
                keyInfo.perUser[n].sPassWord=sPassWordNew;
                bool retsave=saveToFile(default_UserInfoPath);
                return retsave;
                //return true;
            }
        }
    }
    return false;
}


#if 0

XMLHelper::XMLHelper(void)
{
}

XMLHelper::~XMLHelper(void)
{
}


void XMLHelper::UpdateEQXMlFile(QDomDocument &doc, const char *EQXML)
{
    QFile XML(EQXML);
    if (!XML.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        std::cout << "Open EQ File Failed!" << std::endl;
        return ;
    }

    QTextStream out(&XML);
    out.setCodec("UTF-8");

    doc.save(out, 4, QDomNode::EncodingFromTextStream);
    XML.close();
}

void XMLHelper::LoadEQXMLFile(QDomDocument &doc, const char *EQXML)
{
// 	std::cout<<"XMLHelper::LoadEQXMLFile()"<<std::endl;
    QFile XML(EQXML);
    QString error;
    int row = 0, column = 0;

    if(!XML.open(QFile::ReadOnly | QFile::Text))
    {
//         std::cout << "Open EQ XML File Failed!" << std::endl;
        if(!doc.setContent(QString("<EQConfig></EQConfig>"), false, &error, &row, &column))
        {
            std::cout << "<EQConfig></EQConfig> ("<< row << " , " << column << "):" << error.toStdString()  
                << std::endl;
            return ;
        }
    }

    if(!doc.setContent(&XML, false, &error, &row, &column))
    {
// 		std::cout << "Parse EQ.xml error at ("<< row << " , " << column << "):" << error.toStdString()  
// 			<< std::endl;
        if(!doc.setContent(QString("<EQConfig></EQConfig>"), false, &error, &row, &column))
        {
            std::cout << "<EQConfig></EQConfig> ("<< row << " , " << column << "):" << error.toStdString()  
                << std::endl;
            return ;
        }
    }
    XML.close();
}

void XMLHelper::GetAllEQName(char *buf, int &len, const char *EQXML)
{
    QDomDocument doc;
    LoadEQXMLFile(doc, EQXML);

    QDomElement domElement;
    QDomNode node;
    QDomNodeList childList = doc.elementsByTagName("EQ");

    len = 0;
    for(int i=0; i<childList.count(); ++i)
    {
        node = childList.item(i);
        domElement = node.toElement();
        QByteArray arr = domElement.attribute("name").toLocal8Bit();
        if(i < childList.count()-1)
            arr.append('\r');
        memcpy(buf + len, arr.data(), arr.size());
        len += arr.size();
    }
}

//删除EQName对应的在xml中配置
void XMLHelper::DelEQConfig(const std::string EQName, const char *EQXML)
{
    QDomDocument doc;
    LoadEQXMLFile(doc, EQXML);

    QDomElement root=doc.documentElement();  
    QDomNodeList list=doc.elementsByTagName("EQ"); //由标签名定位  
    for(int i=0;i<list.count();i++)  
{
        QDomElement e=list.at(i).toElement();  
        if(e.attribute("name") == EQName.c_str())  
        {
            root.removeChild(list.at(i));  
        }
    } 

    QDomElement oldRoot = doc.documentElement();
    doc.replaceChild(root, oldRoot);

    UpdateEQXMlFile(doc, EQXML);
}

//int setGain(int channel, int band, float gain);
void XMLHelper::GetGraphicEQGain(std::map< unsigned short, std::vector<float> > &gainData,const std::string &EQName, const char *EQXML)
{
    QDomDocument doc;
    LoadEQXMLFile(doc, EQXML);

    QDomElement root = doc.documentElement();  
    QDomNodeList list = doc.elementsByTagName("EQ"); 
    QDomElement e;

    for(int i=0;i<list.count();i++) 
    {
        e = list.at(i).toElement();
        if(e.attribute("name") == EQName.c_str())  
            break;
    }

    list = e.elementsByTagName("speakers"); 
    e = list.at(0).toElement();				//只有一个speakers
    list = e.childNodes();
    QDomNodeList list1;

    for (int i=0; i<list.count(); ++i)
    {
        e = list.at(i).toElement();
        unsigned short channel = e.attribute("channel").toUShort();
        e = e.firstChildElement();
        std::vector<float> gVec;
        list1 = e.childNodes();
        for (int j =0; j<list1.count(); ++j)
        {
            e = list1.item(j).toElement();
            gVec.push_back(e.text().toFloat());
        }
    
        gainData[channel] = gVec;
    }

#if 0
    std::map< unsigned short, std::vector<float> >::iterator it;
    for (it = gainData.begin(); it != gainData.end(); ++it)
    {
        printf("%d--->", it->first);
        for (int i=0; i<it->second.size(); ++i)
        {
            printf("%f ", it->second[i]);
        }
        putchar(10);
    }
#endif
}

//buf为EQName对应的xml配置，转化为的buf
void XMLHelper::GetEQConfig(const std::string EQName, char *buf, unsigned int &len, const char *EQXML)
{
    QDomDocument doc;
    LoadEQXMLFile(doc, EQXML);

    std::map< unsigned short, std::vector<float> > gainData;
    GetGraphicEQGain(gainData, EQName, EQXML);

    QDomElement root = doc.documentElement();  
    QDomNodeList list = doc.elementsByTagName("EQ"); //由标签名定位 
    QByteArray arr;
    for(int i=0;i<list.count();i++)  
    {
        QDomElement e=list.at(i).toElement();  
        if(e.attribute("name") == EQName.c_str())  
        {
            QDomDocument tDoc;
            root = tDoc.createElement("EQConfig");
            tDoc.appendChild(root);
            root.appendChild(e);
            arr = tDoc.toString().toLocal8Bit();
            break;
        }
    } 

    len = arr.size();
    memcpy(buf, arr.data(), len);
}


void XMLHelper::UpdateEQConfig(char *buf, unsigned len, const char *EQXML)
{
    printf("XMLHelper::UpdateEQConfig()--begin\n");

    QString error;
    int row = 0, column = 0;
    
    QDomDocument newDoc;
    if(!newDoc.setContent(QByteArray(buf, len),false, &error, &row, &column))
    {
        std::cout << "UpdateEQConfig: Parse EQ buf error at ("<< row << " , " << column << "):" << error.toStdString()  
            << std::endl;
        return ;
    }

    QDomElement newDomElement = newDoc.documentElement().firstChildElement();
    QString newName = newDomElement.attribute("name");

    QDomDocument doc;
    LoadEQXMLFile(doc, EQXML);

    //判断是否已经存在，不存在插入，存在修改；
    QDomElement root=doc.documentElement();  
    QDomNodeList list=doc.elementsByTagName("EQ"); //由标签名定位 
    for(int i=0;i<list.count();i++)  
    {  
        QDomElement e=list.at(i).toElement();  
        if(e.attribute("name") == newName)  
        {
            root.replaceChild(newDomElement, e);
            QDomElement oldRoot = doc.documentElement();
            doc.replaceChild(root, oldRoot);
            UpdateEQXMlFile(doc, EQXML);

            printf("XMLHelper::UpdateEQConfig()--end exist\n");
            return;
        }
    } 

    
    root.appendChild(newDomElement);
    UpdateEQXMlFile(doc, EQXML);

    printf("XMLHelper::UpdateEQConfig()--end\n");
}

/////////////////////////////////////////////////////////////////////////
void XMLHelper::MacroToXML(std::vector<_Macro> &mVec, std::string &nCurrent, QDomDocument &doc)
{
    QDomElement root = doc.createElement("Macro");
    doc.appendChild(root);

    //增加当前活动的宏的标记字段
    QDomElement current = doc.createElement("active");
    QDomText t = doc.createTextNode(QString::fromStdString(nCurrent));
    current.appendChild(t);
    root.appendChild(current);

    for(size_t i=0; i<mVec.size(); ++i)
    {
        _Macro & m = mVec[i];
        QDomElement macro = doc.createElement("macro");
        macro.setAttribute("name", QString::fromLocal8Bit(m.name.c_str()));
        root.appendChild(macro);

        QDomElement domElement = doc.createElement("GPIO_input");
        QDomText text = doc.createTextNode(QString::number(m.GPIOInput));
        domElement.appendChild(text);
        macro.appendChild(domElement);

        domElement = doc.createElement("fader_preset_switch");
        text = doc.createTextNode(QString::number(m.faderPreset_switch));
        domElement.appendChild(text);
        macro.appendChild(domElement);

        domElement = doc.createElement("fader_preset");
        text = doc.createTextNode(QString::number(m.faderPreset));
        domElement.appendChild(text);
        macro.appendChild(domElement);

        domElement = doc.createElement("delay");
        text = doc.createTextNode(QString::number(m.delay));
        domElement.appendChild(text);
        macro.appendChild(domElement);

        domElement = doc.createElement("input");
        text = doc.createTextNode(m.input.c_str());
        domElement.appendChild(text);
        macro.appendChild(domElement);

        domElement = doc.createElement("format");
        text = doc.createTextNode(m.format.c_str());
        domElement.appendChild(text);
        macro.appendChild(domElement);

        QDomElement supported_bitStreams = doc.createElement("supported_bitStreams");
        macro.appendChild(supported_bitStreams);

        std::vector<std::string> bitStreamKeys;
        bitStreamKeys.push_back("leonis_holoSound");
        bitStreamKeys.push_back("dolby_digtal");
        bitStreamKeys.push_back("dolby_digtal_plus");
        bitStreamKeys.push_back("dolby_E");
        bitStreamKeys.push_back("true_HD");
        bitStreamKeys.push_back("PCM");

        for(int i=0; i<5; ++i)
        {
            domElement = doc.createElement(bitStreamKeys[i].c_str());
            text = doc.createTextNode(QString::number(m.bitStreams[i]));
            domElement.appendChild(text);
            supported_bitStreams.appendChild(domElement);
        }

        QDomElement channel_assignment = doc.createElement("channel_assignment");
        macro.appendChild(channel_assignment);

        domElement = doc.createElement("channel_assignment_set_type");
        text = doc.createTextNode(m.channel_assignment_set_type.c_str());
        domElement.appendChild(text);
        channel_assignment.appendChild(domElement);

        for(int i=0; i<16; ++i)
        {
            domElement = doc.createElement("channel");
            domElement.setAttribute("id", QString::number(i+1));
            text = doc.createTextNode(m.channels[i].c_str());
            domElement.appendChild(text);
            channel_assignment.appendChild(domElement);
        }

        QDomElement sorrunding_assignment = doc.createElement("sorrunding_assignment");
        macro.appendChild(sorrunding_assignment);

        domElement = doc.createElement("stress_switch");
        text = doc.createTextNode(QString::number(m.stress_switch));
        domElement.appendChild(text);
        sorrunding_assignment.appendChild(domElement);

        std::vector<std::string> surroundingTypes;
        surroundingTypes.push_back("2");
        surroundingTypes.push_back("5.1");
        surroundingTypes.push_back("7.1");

        for(int i = 0; i<3; ++i)
        {
            domElement = doc.createElement("surrounding");
            domElement.setAttribute("category", surroundingTypes[i].c_str());
            text = doc.createTextNode(m.surrounding[i].c_str());
            domElement.appendChild(text);
            sorrunding_assignment.appendChild(domElement);
        }

        domElement = doc.createElement("inputTrim");
        text = doc.createTextNode(QString::number(m.inputTrim));
        domElement.appendChild(text);
        macro.appendChild(domElement);

        domElement = doc.createElement("eq_preset");
        text = doc.createTextNode(m.eqPreset.c_str());
        domElement.appendChild(text);
        macro.appendChild(domElement);

        domElement = doc.createElement("note");
        text = doc.createTextNode(QString::fromLocal8Bit(m.note.c_str()));
        domElement.appendChild(text);
        macro.appendChild(domElement);
    }
}

void XMLHelper::MacroFromXML(QDomDocument &doc, std::vector<_Macro> &mVec, std::string &nCurrent)
{
    mVec.clear();
    QDomElement domElement = doc.documentElement();

    QDomNodeList list = domElement.elementsByTagName("macro");

    for(int i=0; i<list.count(); ++i)
    {
        _Macro m;
        QDomNode node = list.item(i);
        domElement = node.toElement();
        m.name = domElement.attribute("name").toLocal8Bit().data();
        QDomNodeList childList = domElement.childNodes();

        node = childList.item(0);
        domElement = node.toElement();
        m.GPIOInput = domElement.text().toInt();

        node = childList.item(1);
        domElement = node.toElement();
        m.faderPreset_switch = domElement.text().toInt();

        node = childList.item(2);
        domElement = node.toElement();
        m.faderPreset = domElement.text().toFloat();

        node = childList.item(3);
        domElement = node.toElement();
        m.delay = domElement.text().toFloat();

        node = childList.item(4);
        domElement = node.toElement();
        m.input = domElement.text().toStdString();

        node = childList.item(5);
        domElement = node.toElement();
        m.format = domElement.text().toStdString();

        node = childList.item(6);
        domElement = node.toElement();
        QDomNodeList childList1 = domElement.childNodes();

        for(int k = 0; k < childList1.count(); ++k)
        {
            node = childList1.item(k);
            domElement = node.toElement();
            m.bitStreams.push_back(domElement.text().toInt());
        }

        node = childList.item(7);
        domElement = node.toElement();
        QDomNodeList childList2 = domElement.childNodes();

        node = childList2.item(0);
        domElement = node.toElement();
        m.channel_assignment_set_type = domElement.text().toLocal8Bit().data();

        for(int k=1; k<childList2.count(); ++k)
        {
            node = childList2.item(k);
            domElement = node.toElement();
            m.channels.push_back(domElement.text().toStdString());
        }

        node = childList.item(8);
        domElement = node.toElement();
        QDomNodeList childList3 = domElement.childNodes();

        node = childList3.item(0);
        domElement = node.toElement();
        m.stress_switch = domElement.text().toInt();

        for(int k=1; k<childList3.count(); ++k)
        {
            node = childList3.item(k);
            domElement = node.toElement();
            m.surrounding.push_back(domElement.text().toStdString());
        }

        node = childList.item(9);
        domElement = node.toElement();
        m.inputTrim = domElement.text().toFloat();

        node = childList.item(10);
        domElement = node.toElement();
        m.eqPreset = domElement.text().toStdString();

        node = childList.item(11);
        domElement = node.toElement();
        m.note = domElement.text().toLocal8Bit().data();

        mVec.push_back(m);
    }
    //增加活动的宏的标记字段
    QDomElement e = domElement.firstChildElement("active");
    if(e.isElement())
        nCurrent = e.text().toStdString();
    else
    {
        nCurrent = mVec.front().name;
    }
}

void XMLHelper::UpdateMacroXMLFile(std::vector<_Macro> &mVec, std::string &nCurrent, const char *MacroXML)
{
    QFile XML(MacroXML);
    if (!XML.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        std::cerr << "Open Macro File Failed!" << std::endl;
        return ;
    }

    QTextStream out(&XML);
    out.setCodec("UTF-8");

    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    MacroToXML(mVec, nCurrent, doc);

    doc.save(out, 4, QDomNode::EncodingFromTextStream);
    XML.close();
}

void XMLHelper::LoadMacroXMLFile(std::vector<_Macro> &mVec, std::string &nCurrent, const char *MacroXML)
{
    QFile XML(MacroXML);

    if(!XML.open(QFile::ReadOnly | QFile::Text))
    {
        std::cerr << "Open Macro XML File Failed!" << std::endl;
        return ;
    }

    QDomDocument doc;
    QString error;
    int row = 0, column = 0;
    if(!doc.setContent(&XML, false, &error, &row, &column))
    {
        std::cerr << "parse file failed at line "<< row << " , " << column << std::endl;
        return ;
    }

    MacroFromXML(doc, mVec, nCurrent);
}

char*XMLHelper:: MacroToBuf(std::vector<_Macro> &mVec, std::string &nCurrent, unsigned int &len)
{
    QDomDocument doc;

    MacroToXML(mVec, nCurrent, doc);

    QString strXML = doc.toString();

    //留出8个字节来保存KL头
    static QByteArray arrXML(8, 0);
    arrXML.resize(8);
    arrXML += strXML;

    len = arrXML.length() - 8;

    char* p = (char*)arrXML.operator const char *();
    
    return p;
}

bool XMLHelper::MacroFromBuf(std::vector<_Macro> &mVec, std::string &nCurrent, char* buf, unsigned int len)
{
    QString errStr;
    int errLine;
    int errColumn;

    QDomDocument doc;
    if (!doc.setContent(QByteArray(buf, len),
        true, &errStr, &errLine, &errColumn))
    {
        return false;
    }
    mVec.clear();

    MacroFromXML(doc, mVec, nCurrent);

    return true;
}

static std::vector<_Macro> gMacroVector;
static std::string gActiveMacro;

std::vector<_Macro>* GetMacroVector()
{
    return &gMacroVector;
}

std::string* GetActiveMacro()
{
    return &gActiveMacro;
}

void SetActiveMacro(char* name)
{
    gActiveMacro = name;
}



///////////////////////////////////////////////////////////////////

void XMLHelper::MonitorMixToXML(_MonitorMixStruct &m, QDomDocument &doc)
{
    QString strHead("version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(doc.createProcessingInstruction("xml", strHead));

    QDomElement root = doc.createElement("MonitorMixConfig");
    doc.appendChild(root);

    QDomElement AES;
    AES = doc.createElement("AES");
    root.appendChild(AES);

    for (int i=0; i<CHANNEL_USABLE; ++i)
    {
        QDomElement domElement = doc.createElement("Channel");
        domElement.setAttribute("id", i);
        QDomText text = doc.createTextNode(QString::number(m.channelChecked[i]));
        domElement.appendChild(text);
        AES.appendChild(domElement);
    }

    QDomElement domElement = doc.createElement("IsMix");
    QDomText text = doc.createTextNode(QString::number(m.isMix));
    domElement.appendChild(text);
    root.appendChild(domElement);
}

void XMLHelper::MonitorMixFromXML(_MonitorMixStruct &m, QDomDocument &doc)
{
    QDomNodeList nodeList = doc.elementsByTagName("Channel");
    for (int i=0; i<nodeList.count(); ++i)
    {
        QDomNode node = nodeList.at(i);
        QDomElement domElement = node.toElement();
        m.channelChecked[i] = domElement.text().toInt();
    }

    nodeList = doc.elementsByTagName("IsMix");
    m.isMix = nodeList.at(0).toElement().text().toInt();
}


char *XMLHelper::MonitorMixToBuf(_MonitorMixStruct &m, unsigned int &len)
{
    QDomDocument doc;
    MonitorMixToXML(m, doc);
    return XMLToBuf(doc, len);
}

void XMLHelper::MonitorMixFromBuf(_MonitorMixStruct &m, char *buf, unsigned int len)
{
    QDomDocument doc;
    BufToXML(buf, len, doc);
    MonitorMixFromXML(m, doc);
}




void XMLHelper::UpdateXMLFile(char *buf, unsigned int len, char *XMLPath)
{
    QDomDocument doc;
    BufToXML(buf, len, doc);
    WriteXML(doc, XMLPath);
}

char * XMLHelper::LoadXMLFile(unsigned int &len, char *XMLPath)
{
    QDomDocument doc;
    ReadXML(doc, XMLPath);
    return XMLToBuf(doc, len);
}

bool XMLHelper::ReadXML(QDomDocument &doc, const char *XMLPath)
{
    QFile XML(XMLPath);

    if(!XML.open(QFile::ReadOnly | QFile::Text))
    {
        std::cout << "Open XML File Failed!" << std::endl;
        return false;
    }

    QString error;
    int row = 0, column = 0;
    if(!doc.setContent(&XML, false, &error, &row, &column))
    {
        std::cout << "Parse XML File error at ("<< row << " , " << column << "):" << error.toStdString()  
            << std::endl;
        return false;
    }

    XML.close();

    return true;
}

bool XMLHelper::WriteXML(QDomDocument &doc, const char *XMLPath)
{
    QFile XML(XMLPath);
    if (!XML.open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
    {
        std::cout << "Open XML file Failed!" << std::endl;
        return false;
    }

    QTextStream out(&XML);
    out.setCodec("UTF-8");
    doc.save(out, 4, QDomNode::EncodingFromTextStream);
    XML.close();

    return true;
}

char * XMLHelper::XMLToBuf(QDomDocument &doc, unsigned int &len)
{
    QString strXML = doc.toString();
    //std::cout << strXML.toStdString() << std::endl;
    QByteArray klHead(8, 0);
    static QByteArray arrXML;				
    arrXML = klHead;				//留出8个字节来保存KL头
    arrXML += strXML.toLocal8Bit();
    len = arrXML.size()-8;

    return (char*)arrXML.operator const char *();
}

bool XMLHelper::BufToXML(char *buf, unsigned int len, QDomDocument &doc)
{
    QString error;
    int row = 0, column = 0;
    if(!doc.setContent(QByteArray(buf, len), false, &error, &row, &column))
    {
        std::cout << "Parse XML buf error at ("<< row << " , " << column << "):" << error.toStdString()  
            << std::endl;
        return false;
    }

    return true;
}
#endif

