#ifndef SpkInfo_Header
#define SpkInfo_Header

//基础数据类型其他非Qt项目可能会用到，
//废弃基础数据类型Qt依赖,在Qt项目中请使用SpkInfoExp.h


//#include <QColor>
//#include <QString>


#include <string>
#include <vector>

//typedef unsigned int QRgb;         // RGB triplet
typedef unsigned int MyRgb;         //与QRgb相同,废弃QColor后,表示颜色值,便于与QColor互相转换



class  RoomDim
{
public:
    RoomDim(float l=0,float w=0,float h=0)
        :length(l),width(w),height(h)
    {
    }
    float width;
    float length;
    float height;
    std::string toSrt() const
    {
        char tmpstr[64];
        sprintf(tmpstr,"L:%.2fm W:%.2fm H:%.2fm",length,width,height);
        return std::string(tmpstr);
    }
};


//扬声器类型名称宏定义,Core与UI中同名宏保存一致 rafactor by lizulin 20200707
//历史遗留，为了兼容旧影厅配置，已固定不可任意修改.判断相等时忽略大小写。

////扬声器类型简写形式
//扬声器类型字符串
#define SPKTYPE_SCREEN		"Screen"
#define SPKTYPE_SURROUND	"Surround"
#define SPKTYPE_SUBWOOFER	"Subwoofer"
#define SPKTYPE_LFE	"LFE"  //存为设备类型时转成   "Subwoofer"
#define SPKTYPE_UNKNOW	"UnKnow"


//扬声器类型宽字符串形式
#define SPKTYPE_SCREEN_WS		L"Screen"
#define SPKTYPE_SURROUND_WS	L"Surround"
#define SPKTYPE_SUBWOOFER_WS	L"Subwoofer"
#define SPKTYPE_LFE_WS	"LFE"  //存为设备类型时转成   "Subwoofer"
#define SPKTYPE_UNKNOW_WS	L"UnKnow"

//扬声器类型完整形式,不再使用
////扬声器类型字符串
//#define SPKTYPE_SCREEN_LONG		"screenSpeaker"
//#define SPKTYPE_SURROUND_LONG	"surroundSpeaker"
//#define SPKTYPE_SUBWOOFER_LONG	"SubwooferSpeaker"
//
////扬声器类型宽字符串形式
//#define SPKTYPE_SCREEN_LONG_WS		L"screenSpeaker"
//#define SPKTYPE_SURROUND_LONG_WS	L"surroundSpeaker"
//#define SPKTYPE_SUBWOOFER_LONG_WS	L"SubwooferSpeaker"


//在Begin-End之间的才是正确扬声器类型
enum ESpkType
{
    SpkType_Unknow=-1,         //未知扬声器
    SpkType_Begin=SpkType_Unknow,
    SpkType_Screen,           //银幕侧主扬声器
    SpkType_Surround,       //环绕扬声器

    //LFE扬声器也是低音扬声器,只有与硬件关联时查找具体设备时才当成subwoofer,其他时候单独作为一类,
    //注意此差异是否造成bug
    SpkType_Subwoofer,     //低音扬声器 
    SpkType_LFE,                 

    SpkType_End,
    SpkType_Total=SpkType_End,
};

//判断扬声器枚举值是否有效
inline bool isValidEnum(ESpkType spkType){ return (spkType>SpkType_Begin)&&(spkType<SpkType_End);}



//记录扬声器位置名称等其他信息  形状 低音扬声器旋转90度显示
struct  SpeakerBaseInfo
{
    SpeakerBaseInfo()
    {
        m_x=0; 
        m_y=0;    
        m_z=0;
        m_delay=0; 
        m_SpkType=SpkType_Unknow;
    }
    float m_x;    //扬声器位置
    float m_y;    
    float m_z;

    //注意仅专用于判断平面坐标是否重合,
    //bool operator ==(const SpeakerBaseInfo& R) const
    //{
    //    return (this->m_x == R.m_x && this->m_y == R.m_y);
    //}
    //refactor 20200706 为了不破坏语义,用专门函数实现
    bool isSamePos(const SpeakerBaseInfo& R)  //左边是否相同
    {
            return (this->m_x == R.m_x && this->m_y == R.m_y);
    }


    std::string toPosStr(int Decimal=2)const    //转换成x*y*z 格式的字符串 (x,y,z)
    {
        char tmp[64];
        if (Decimal==2)
        {
            sprintf(tmp,"(%.2f,%.2f,%.2f)\n",m_x,m_y,m_z);
        }
        else
        {
            sprintf(tmp,"(%.1f,%.1f,%.1f)\n",m_x,m_y,m_z);
        }
        return std::string(tmp);
    }
    std::string sName;                     //扬声器名称
    std::string sArrayTypeName;     //扬声器所属阵列名称
    double m_delay;                        //扬声器延时相位延时值

   //扬声器类型名称如screenSpeaker(银幕侧主扬声器), surroundSpeaker(环绕扬声器) SubwooferSpeaker(低音扬声器)
    std::string sSpeakerTypeName;  
    //int m_SpkType;                              //扬声器类型枚举值,ESpkType
    ESpkType m_SpkType;                     //扬声器类型枚举值,ESpkType
};


//准备存到配置文件时扬声器类型LFE要转成Subwoofer,其他类型不变,硬件设备库不支持LFE类型概念
std::string SpeakerTypeNameToDeviceType(const std::string& sSpeakerTypeName);
ESpkType SpeakerTypeToDeviceType(const ESpkType& spkType);


class RoomItem;
struct BassManagerInfo
{
    RoomItem* pItem;               //加载的时候无用，保存的时候也许会用 预留
    std::string  strID;		            //低通扬声器ID名称
    MyRgb SubwooferColor;      //低通扬声器的背景色
    int crossoverFrequencyHz;
    int gainDb;
};


//大改  传递扬声器的信息结构体
struct SpeakerInfo :public SpeakerBaseInfo
{
    SpeakerInfo()
    {
        m_delay=0.0;
        m_ishighLight=false;
        m_DoutNum=-1;
        m_AoutNum=-1;
        m_Number=0;
        m_isSubwoofer=false;
        m_isAutoEQDone=false;
    }
    //SpeakerInfo& operator=(const SpeakePosInfo& R)
    //{
    //    return *this;
    //}

    bool m_ishighLight;    //是否高亮

    //Routing相关
    std::string sOutputRef;  //映射布局信息 如SPKREP-0  与<outputRef>OUT-BL-0直接的映射关系 跟随DoutNum变化
    std::string sAmplifierRef;               //>AMP-CUSTOM-29</amplifierRef>
    std::string sSpeakerProcessorRef;  //>PROC-CUSTOM-29</speakerProcessorRef>
    std::string sAnalogOutput;

    MyRgb SubwooferBassManagerColor;    //低通扬声器只有一种固定颜色，初始化时候决定，作为低音扬声器时候的背景色
    std::vector<MyRgb>  VBassManagerColor;   //作为普通扬声器根据颜色容器，渐变自绘  
    std::vector<BassManagerInfo>  VBassManagerInfo;   //保存当前扬声器分配给了哪些低通扬声器

    int m_DoutNum;   //数字输出--有一个类管理有效ID,-1表示无效，未设定
    int m_AoutNum;   //模拟输出--暂时无用
    int m_Number;    //在view添加的时候给每一个item一个序号。//由view维护保证item唯一，但是可以删除后空缺不连续


    bool  m_isSubwoofer;               //低音扬声器标记，旋转90度显示
    bool  m_isAutoEQDone;            //完成AutoEQ标志，绘制不同颜色以便区别
    MyRgb BackgroundColor;      
    std::string sID;                         //扬声器ID字符串    如"SPKREP-1""SPKREP-2" 根据m_Number生成

    std::vector<std::string> VArrayList;  //分组 二级分组容器,
    std::string sSpecsRef;


    //new add 20200711 广州项目需求新增
    std::string sSpkDeviceName;     //实际选配的扬声器型号名称 根据库决定
    std::string sAmpDeviceName;   //实际选配的功放型号名称    根据库决定
};


enum EFreq
{
    E_LF,       //低频
    E_MF,     //中频
    E_HF,      //高频
    E_FSize,
    //E_VHF,    //有的扬声器会有,暂不支持此概念
};

enum EOhm
{
    Ohm2,         //2欧姆
    Ohm4,         //4欧姆
    Ohm8,         //8欧姆
    Ohm16,       //16欧姆
    Ohm_Size,
};

inline int OhmValue(EOhm eo)
{
    switch (eo)
    {
    case Ohm2:return 2;break;
    case Ohm4:return 4;break;
    case Ohm8:return 8;break;
    case Ohm16:return 16;break;
    default:return 0;break;
    }
}


//全波段的功率值FullBandPower
struct  FBPower
{
    float powerAll;              //对于环绕，一般只有一个功率
    float  power[E_FSize];    //低中高频段的功率
    FBPower()
    {
          setValue(0,0,0,0);
    }

    //各频段的概率值全部相等,一般用于不区分
    bool isAllEqual() const
    {
        float val=power[0];
        for (int n=0;n<E_FSize;n++)
        {
            if (power[n]!=val)
            {
                return false;
            }
        }
        return true;
    }

   //FBPower(float power=0)
   //{
   //       setValue(power,power,power,power);
   //}
   //FBPower(float powerAll,float powerL,float powerM)
   //{
   //       setValue(powerAll,powerL,powerM,powerM);
   //}
   FBPower(float powerAll,float powerL,float powerM,float powerH)
   {
          setValue(powerAll,powerL,powerM,powerH);
   }
   //void operator=(float power)
   //{
   //         setValue(power,power,power);
   //}
   void setValue(float powerAll,float powerL,float powerM,float powerH)
   {
        this->powerAll=powerAll;
        power[E_LF]=powerL;
        power[E_MF]=powerM;
        power[E_HF]=powerH;
   }

   //扬声器功率是取平均值/最大值/总和,理论上作为功放选型参考应该是低中高频率总和合适。  待定
   float getPower() const
   {
       //power=(powerL+powerM+powerH)/3;      //功率取平均值
       //power=powerL+powerM+powerH;           //功率低中高频率总和
       //power=Max3(powerL,powerM,powerH);    //功率取低中高频率最大值

       //若存在总频段值则直接返回,
       if (powerAll>1)
       {
           return powerAll;
       }

       //返回各个频段值相加
       return  power[E_LF]+power[E_MF]+power[E_HF];
   }
};



//全波段声压级值FullBandSPL spl
struct  FBSPL
{
    float  spl[E_FSize];

    //各频段的功率值全部相等,一般用于不区分
    bool isAllEqual() const
    {
        float val=spl[0];
        for (int n=0;n<E_FSize;n++)
        {
            if (spl[n]!=val)
            {
                return false;
            }
        }
        return true;
    }

    FBSPL(float spl=0)
    {
        setValue(spl,spl,spl);
    }
    FBSPL(float splL,float splM)
    {
        setValue(splL,splM,splM);
    }
    FBSPL(float splL,float splM,float splH)
    {
        setValue(splL,splM,splH);
    }
    void operator=(float spl)
    {
        setValue(spl,spl,spl);
    }
    void setValue(float splL,float splM,float splH)
    {
        spl[E_LF]=splL;
        spl[E_MF]=splM;
        spl[E_HF]=splH;
    }
};



//全波段的阻抗值FullBandImpedance 
struct  FBImpedance
{
    int  impedance[E_FSize];
    //各频段的概率值全部相等,一般用于不区分
    bool isAllEqual() const
    {
        int val=impedance[0];
        for (int n=0;n<E_FSize;n++)
        {
            if (impedance[n]!=val)
            {
                return false;
            }
        }
        return true;
    }

    FBImpedance(int impedance=0)
    {
        setValue(impedance,impedance,impedance);
    }
    FBImpedance(int impedanceL,int impedanceM)
    {
        setValue(impedanceL,impedanceM,impedanceM);
    }
    FBImpedance(int impedanceL,int impedanceM,int impedanceH)
    {
        setValue(impedanceL,impedanceM,impedanceH);
    }
    void operator=(int impedance)
    {
        setValue(impedance,impedance,impedance);
    }
    void setValue(int impedanceL,int impedanceM,int impedanceH)
    {
        impedance[E_LF]=impedanceL;
        impedance[E_MF]=impedanceM;
        impedance[E_HF]=impedanceH;
    }
    //阻抗值转成字符串4|8|8 形式
    std::string toStr() const
    {
        char tmp[128];
        if (impedance[E_LF]==impedance[E_MF]&&impedance[E_MF]==impedance[E_HF])
        {
            sprintf(tmp,"%d",(int)impedance[E_LF]);
        }
        //if (tmp,impedance[E_MF]!=impedance[E_HF])
        else
        {
            sprintf(tmp,"%d|%d|%d",(int)impedance[E_LF],(int)impedance[E_MF],(int)impedance[E_HF]);
        }
        return std::string(tmp);
    }
};





//全阻抗的功率值FullImpedancePower,为0表示不支持此阻抗值
struct  FIPower
{
    float  power[Ohm_Size];
    bool isAllEqual() const
    {
        float val=power[0];
        for (int n=0;n<Ohm_Size;n++)
        {
            if (power[n]!=val)
            {
                return false;
            }
        }
        return true;
    }

    FIPower(float power2=0,float power4=0,float power8=0,float power16=0)
    {
        setValue(power2,power4,power8,power16);
    }
    void setValue(float power2=0,float power4=0,float power8=0,float power16=0)
    {
        power[Ohm2]=power2;
        power[Ohm4]=power4;
        power[Ohm8]=power8;
        power[Ohm16]=power16;
    }
    void setPower2(float power2)
    {
        power[Ohm2]=power2;
    }
    void setPower4(float power4)
    {
        power[Ohm4]=power4;
    }
    void setPower8(float power8)
    {
        power[Ohm8]=power8;
    }
    void setPower16(float power16)
    {
        power[Ohm16]=power16;
    }

    std::string toPowerStr()const
    {
        std::string s;
        int pw;   //old float pw;
        pw=power[Ohm2];
        if (pw>0)
        {
            s+="2Ω:";
            s+=IntToString(pw);
            s+="W ";
        }
         pw=power[Ohm4];
        if (pw>0)
        {
            s+="4Ω:";
            s+=IntToString(pw);
            s+="W ";
        }
        pw=power[Ohm8];
        if (pw>0)
        {
            s+="8Ω:";
            s+=IntToString(pw);
            s+="W ";
        }
        pw=power[Ohm16];
        if (pw>0)
        {
            s+="16Ω:";
            s+=IntToString(pw);
            s+="W ";
        }
        return s;
    }

    float getPowerOfImpedance(int ohm) const   //float ohm
    {
        float powerSelectd=0;
        //if ((int)ohm==2)
        if (abs(ohm-2) <0.001)
        {
            powerSelectd=power[Ohm2];
        }
        else if (abs(ohm-4) <0.001) //if ((int)ohm==4)
        {
            powerSelectd=power[Ohm4];
        }
        else if (abs(ohm-8) <0.001) //if ((int)ohm==8)
        {
            powerSelectd=power[Ohm8];
        }
        else if (abs(ohm-16)<0.001) //if ((int)ohm==16)
        {
            powerSelectd=power[Ohm16];
        }
        //PreDprintf("Impedance:%f power:%f\n",ohm,powerSelectd);
        return powerSelectd;
    }


    static std::string FloatToString(float val)
    {
        char buf[64];
        sprintf(buf,"%.1f",val);
        std::string tmp(buf);
        return tmp;
    }
    static std::string IntToString(int val)
    {
        char buf[64];
        sprintf(buf,"%d",val);
        std::string tmp(buf);
        return tmp;
    }
};




//一些概念说明
//LFE(Low Frequency Effects) 低频效果声道
//Subwoofer  低音炮 重低音
//booth 参考dolby 一般指的是后墙 


//扬声器类型说明:参考dolby
//Screen,代表银幕侧主扬声器包含L/R/C
//Surround代表环绕扬声器,包含LSS/RSS LTS/RTS LRS/RRS
//Subwoofer扬声器选型的时候LFE 与Subwoofer 用同一种

#include "Super/Basic/Tstring.h"


//单个扬声器属性描述结构体
struct  PerSpkInfo
{
    std::string sFullName;          //扬声器唯一区分名称,由厂家缩写+名称拼接生成，不显示
    std::string sFactoryShort;     //厂家缩写，必须英文
    std::string sName;               //扬声器名称,相同厂家必须保证唯一

    std::string sSpkType;           //扬声器类型名称
    ESpkType SpkType;             //扬声器类型枚举值

    std::string sFactory;             //厂家,可以是中文,不填则自动生成与缩写的一致
    std::string sSeries;               //扬声器在厂家所属系列,无用预留,可以为空,

    //一些属性太乱，同时维护字符串和数值
    std::string  sDimensions;     //外形尺寸字符串
    std::string  sWeight;
    std::string  sPower;
    std::string  sImpedance;
    std::string  sSPL;
    float length,width,height;    //长宽高,单位mm
    float weight;                       //重量单位kg
    float power;                        //功率单位W
    int impedance;                //阻抗单位欧姆
    float SPL;                           //声压级,单位dB

    //
    std::string  sSensitivity;               //灵敏度
    std::string  sResponseFreq;         //响应频率
    std::string  sCrossoverFreq;         //分频点
    //值建模待续
    float Sensitivity;
    float LowResponseFreq;
    float HeightResponseFreq;
    float CrossoverFreq;


    float cost;                          //物料成本,单位元
    std::string  sRemark;         //备注

    //更严谨的支持各持各种参数的精确值--完善待续
    FBPower fbPower;                         //各频段的功率值
    FBImpedance fbImpedance;         //各频段的阻抗值
    FBSPL  fbspl;                                 //各频段的声压级值
    PerSpkInfo()
    {
        length=width=height=0.0f;
        weight=0.0f;
        power=0.0f;
        impedance=0;
        cost=0.0f;
        SPL=105;
    }

    //从字符串信息,解析刷新各种属性的实际值,以便于编程直接使用
    void updateValueFromString();
    void print()const
    {
        printf("Spk:fullName:%s\n",sFullName.c_str());
        printf("SpkType:%d str:%s\n",SpkType,sSpkType.c_str());
        //printf("Factory:%s Series:%s\n",sFactory.c_str(),sSeries.c_str());
        //printf("Dimensions:%s Weight:%s\n",sDimensions.c_str(),sWeight.c_str());
        //printf("Power:%s Impedance:%s\n",sPower.c_str(),sImpedance.c_str());
        //printf("SPL:%s Remark:%s\n",sSPL.c_str(),sRemark.c_str());

        //xml文件存储的是utf8编码,转成控制台gb2312之后再显示
        printf("Factory:%s Series:%s\n",utf8_to_gb2312(sFactory).c_str(),utf8_to_gb2312(sSeries).c_str());
        printf("Dimensions:%s Weight:%s\n",utf8_to_gb2312(sDimensions).c_str(),utf8_to_gb2312(sWeight).c_str());
        printf("Power:%s Impedance:%s\n",utf8_to_gb2312(sPower).c_str(),utf8_to_gb2312(sImpedance).c_str());
        
        printf("SPL:%s Remark:%s\n",utf8_to_gb2312(sSPL).c_str(),utf8_to_gb2312(sRemark).c_str());

        printf("Sensitivity:%s \n",utf8_to_gb2312(sSensitivity).c_str());
        printf("ResponseFreq:%s \n",utf8_to_gb2312(sResponseFreq).c_str());
        printf("CrossoverFreq:%s \n",utf8_to_gb2312(sCrossoverFreq).c_str());

        printf("Dim: L:%f W:%f H:%f Weight:%f Cost:%f\n",length,width,height,weight,cost);
        printf("power: %f impedance:%d SPL:%f\n",power,impedance,SPL);
    }
};





//功放与扬声器的连接方式
enum EConnectType
{
    ConType_Stereo,       //立体声模式,每个功放通道连接一个扬声器
    ConType_Bridge,       //桥接模式，每两个功放通道连接一个扬声器
    ConType_Parallel,     //并联模式，暂不支持
    ConType_Unknow,    //
};



//功率放大器数据库管理,库不会太大,基于xml实现。


//单个功率放大器属性描述结构体
struct  PerAmpInfo
{
    std::string sFullName;      //功放唯一区分名称,由厂家缩写+名称拼接生成，不显示
    std::string sFactoryShort; //厂家缩写，必须英文
    std::string sName;           //功放名称,相同厂家必须保证唯一

    std::string sAmpType;     //功放类型名称,暂时只有一种
    //ESpkType AmpType;      //扬声器类型

    std::string sFactory;             //厂家,可以是中文,不填则自动生成与缩写的一致
    std::string sSeries;               //扬声器在厂家所属系列,无用预留,可以为空,

    //一些属性太乱，同时维护字符串和数值
    std::string  sDimensions;     //外形尺寸字符串
    std::string  sWeight;
    unsigned int ChannelNum;    //通道数数量,一般功放2通道,LEONIS的有32通道
    std::string  sPower;
    std::string  sBridgePower;     //桥接功率很多时候并非立体声功率的2倍,用单独列表示
    std::string  sImpedance;


    float length,width,height;    //长宽高,单位mm
    float weight;                        //重量单位kg
    //float power;                      //功率单位W
    int impedance;                   //阻抗单位欧姆

    std::string sVoltageGain;   //电压增益Voltage Gain(dB)
    float voltageGain;

    //
    float cost;                       //物料成本,单位元
    std::string  sRemark;      //备注


    //功放与扬声器的连接模式，占用的通道数，暂时放此处，
    EConnectType conType;       //连接模式
    unsigned int perSpkUseChs;          //每一个扬声器的占用功放的通道数

    //更严谨的支持各持各种参数的精确值--完善待续
    FIPower fiPower;                           //各阻抗值对应的功率,为0表示不支持此阻抗

    //高级支持待续
    bool bSupportBridge;                     //是否支持桥接模式
    FIPower fiBridgePower;                 //桥接模式,各阻抗值对应的功率,为0表示不支持此阻抗

    //简化情况暂时令桥接功率等于立体声功率的2倍,除非在库里解析到明确参数

    PerAmpInfo()
    {
        length=width=height=0.0f;
        weight=0.0f;
        //power=0.0f;
        impedance=0.0f;
        cost=0.0f;

        voltageGain=0;


        conType=ConType_Stereo;       //连接模式
        perSpkUseChs=1;                     //每一个扬声器的占用功放的通道数
    }

    //从各种字符串解析到具体值，用于实际运算
    void updateValueFromString();

    //获取采用立体声接法时某个阻抗(2/4/816)连续额定功率值,找不到返回0
    float getPowerOfImpedance(float ohm) const
    {
        return fiPower.getPowerOfImpedance(ohm);
    }
    //获取采用桥接接法时某个阻抗(2/4/816)连续额定功率值,找不到返回0
    float getBridgePowerOfImpedance(float ohm) const
    {
        return fiBridgePower.getPowerOfImpedance(ohm);
    }


    float powerSelectd;          //用在已经选择扬声器的排序,暂时不用原power
    int impedanceSelectd;     //选用的阻抗值,用在显示

    void print()const
    {
        printf("Spk:fullName:%s\n",sFullName.c_str());
        //printf("Factory:%s Series:%s\n",sFactory.c_str(),sSeries.c_str());
        //printf("Dimensions:%s Weight:%s\n",sDimensions.c_str(),sWeight.c_str());
        //printf("Power:%s Impedance:%s\n",sPower.c_str(),sImpedance.c_str());
        //printf("Remark:%s\n",sRemark.c_str());

        //xml文件存储的是utf8编码,转成控制台gb2312之后再显示
        printf("Factory:%s Series:%s\n",utf8_to_gb2312(sFactory).c_str(),utf8_to_gb2312(sSeries).c_str());
        printf("Dimensions:%s Weight:%s\n",utf8_to_gb2312(sDimensions).c_str(),utf8_to_gb2312(sWeight).c_str());
        printf("Power:%s\n",utf8_to_gb2312(sPower).c_str());
        printf("BridgePower:%s\n",utf8_to_gb2312(sBridgePower).c_str());
        printf("Impedance:%s\n",utf8_to_gb2312(sImpedance).c_str());
        printf("VoltageGain:%s\n",utf8_to_gb2312(sVoltageGain).c_str());
        printf("Remark:%s\n",utf8_to_gb2312(sRemark).c_str());

        printf("Dim: L:%f W:%f H:%f Weight:%f Channels:%u Cost:%f\n"
            ,length,width,height,weight,ChannelNum,cost);

        printf("powerSelectd: %d impedance:%f\n",powerSelectd,impedance);
    }

};







////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////本类主要用来保存RoomConfig窗体的配置参数，方便在不同模块修改传递
//class  SpeakerItem
//{
//public:
//	SpeakerItem();
//	~SpeakerItem();
//};





class  RoomConfig
{
public:
	RoomConfig();    //使用默认值
	~RoomConfig();
	void setDefault();    //复位为0
	void setZero();    //复位为0

    //从配置信息获取简化的扬声器尺寸,旧designer模型
    RoomDim getRoomDim()const;

    //旧版Designer需要手动指定,影厅每一侧侧扬声器个数,
    //IA-conf tool，根据长宽高,自动根据公式计算每一侧扬声器个数  2019
    //大部分属性相同复用本结构

    //2019IA-conf tool新版参数
    //简化的影厅房间长宽高,
    //double roomWidth;          //前后墙的最大值
    //double roomLength;        //左右墙长度,一般是等长的
    double roomHeight;        //前中后,最大高度,一般取扬声器的最大高度位置
    //double roomScreenWidth;   //可能会构建前后墙不等长的影厅模型,改进版
    //double roomBoothWidth;
    unsigned int grade;               //自动影厅配置档次,低中高,分别为0,1,2
    std::string gradeToStr()const;    //档次到字符串


    //旧版Designer参数
	//ScreenWall
	double ScreenWallLength;       //前墙长度 （荧幕侧）
	double FlatWidth;                   //遮幅银幕宽度
	double FlatHeight;                 //遮幅银幕高度
    //银幕距离左墙位置偏移,参考dolby
    double FlatScreenDisToLeftWall;          

	double ScopeWidth;                  //宽幅银幕宽度
	double ScopeHeight;                 //宽幅银幕高度
    double ScreenBottomHeight;    //地板到银幕底部高度
    //银幕距离左墙位置偏移,参考dolby
    double ScopeScreenDisToLeftWall;          

	unsigned int TotalSpeakerofScreenWall;     //前墙扬声器个数  银幕侧主扬声器
	int TotalSpeakerofLFE;                                //LFE低音炮个数  


	//LeftRightWall
	double LeftRightWallLength;                  //左右墙长度
	unsigned int SpeakersPerLeftRightWall;  //左右墙每侧扬声器个数
	double AverangeDistanceOfSpeaker;      //左右墙.扬声器之间的平均距离
	double DistanceWalltoScreen;                //左右墙,第一个扬声器到前墙距离

    //观众席长度,第一排座椅到后墙距离,用来以后确定观众席坡度等  new add 
    //暂时等于 LeftRightWallLength-DistanceWalltoScreen
	double DistanceFirstRowSeatToBoothWall;                


	//BoothWall
	double BoothWallLength;                         //后墙长度
	unsigned int TotalSpeakerofBoothWall;    //后墙普通扬声器总个数，不包含低音扬声器               
	unsigned int TotalSpeakersofSubwoofer;  //后墙低音炮个数


	//ceiling  floor 各种影厅高度
	double FloorElevationat23point;
	double FloorElevationatbooth;
	double CeilingElevationatscreen;                    //在银幕处高度
	double CeilingElevationat23point;                  //在2/3处高度
	double CeilingElevationatbooth;                    //在后墙处高度
	unsigned int SpeakersofCeillingPerSide;        //顶墙，天花板，左右侧，每侧扬声器个数


    //用城市名+影院名+影厅名
    std::string CinemaName()const
    {
        return strCity+" "+strCinema+" "+strAuditorium;
    }

    //RoomID对话框的参数信息
    std::string strCompany;         //影院所属公司
    std::string strCinema;            //影院名称
	std::string strAuditorium;      //影院内影厅名称
	std::string strCity;                 //所在城市
	std::string strProvince;          //所在省
	std::string strCountry;           //所在国家/地区
	std::string strRemark;       //其他备注信息

    //2019IA-conf tool新增
    std::string strDetailedAddress;   //影厅详细地址
    std::string strContactPerson;     //售后联系人
    std::string strContactTel;           //联系人电话
    std::string strContactEmail;       //联系人邮箱

    std::string strCreateDate;            //生成日期 点击确认后自动生成,   待续
};

RoomConfig* GetRoomConfig();
//void ReleaseRoomConfig(RoomConfig* pConfigRoom);

//#pragma pack(pop)       //恢复原来对齐状态


#endif





