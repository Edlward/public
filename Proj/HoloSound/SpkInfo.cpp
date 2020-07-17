#include "SpkInfo.h"
#include "Super/Basic/BasicOperation.h"  //for Find sub string



RoomConfig* GetRoomConfig()
{
    static RoomConfig gb_Configroom;
    return &gb_Configroom;
}




//void ReleaseRoomConfig(RoomConfig* pConfigRoom)
//{
//
//}



RoomConfig::RoomConfig()
{
    setDefault();
}

void RoomConfig::setDefault()
{
    //初始化各种参数的值

    grade=1;                            //默认中档
    ScreenWallLength=9.0;    //前墙长度 （荧幕侧）
    TotalSpeakerofScreenWall=3;     //前墙扬声器个数
    TotalSpeakerofLFE=1;                    //前墙低音炮个数

    //LeftRightWall
    LeftRightWallLength=11.0;                       //左右墙长度 
    SpeakersPerLeftRightWall=5;                    //左右墙每侧扬声器个数
    AverangeDistanceOfSpeaker=2.0;           //左右墙.扬声器之间的平均距离
    DistanceWalltoScreen=3.0;                     //左右墙,第一个扬声器到前墙距离
    DistanceFirstRowSeatToBoothWall=LeftRightWallLength-DistanceWalltoScreen;

    //BoothWall
    BoothWallLength=ScreenWallLength;        //后墙长度--默认等于前墙
    TotalSpeakerofBoothWall=4;                      //后墙普通扬声器总个数，不包含低音扬声器               
    TotalSpeakersofSubwoofer=2;                    //后墙低音炮个数

    //ceiling  floor
    FloorElevationat23point=1.2;
    FloorElevationatbooth=2.0;
    CeilingElevationatscreen=6.5;
    CeilingElevationat23point=6.5;
    CeilingElevationatbooth=6.5;
    SpeakersofCeillingPerSide=5;        //顶墙，天花板，左右侧，每侧扬声器个数


    //根据墙面估算银幕尺寸初始值
    FlatWidth=ScreenWallLength*0.95;
    FlatHeight=CeilingElevationatscreen*0.9;
    FlatScreenDisToLeftWall=0;
    ScopeWidth=ScreenWallLength*0.95;
    ScopeHeight=CeilingElevationatscreen*0.9;
    ScopeScreenDisToLeftWall=0;

    ScreenBottomHeight=(CeilingElevationatscreen-ScopeHeight)/2;



    //RoomID对话框的参数信息
    strCompany="LEONIS";
    strCinema="Leonis";
    strAuditorium="HoloSound 1";
    strCity="Beijing";
    strProvince="Beijing";
    strCountry="China";
    strRemark="Test Remark";

    //2019IA-conf tool新增
    strDetailedAddress="海淀区稻香湖路中关村环保科技示范园1号楼5层";          //影厅详细地址
    strContactPerson="杨经理";            //售后联系人
    strContactTel="010-62670467";     //联系人电话
    strContactEmail="xiaohui.yang@leoniscinema.com";       //联系人邮箱

    strCreateDate="2019/12/21";
}


void RoomConfig::setZero()
{
    grade=1;
    ScreenWallLength=0;       //前墙长度 （荧幕侧）
    FlatWidth=0;
    FlatHeight=0;
    ScopeWidth=0;
    ScopeHeight=0;
    ScreenBottomHeight=0;
    FlatScreenDisToLeftWall=0;
    TotalSpeakerofScreenWall=0;     //前墙扬声器个数
    TotalSpeakerofLFE=0;                    //前墙低音炮个数

    //LeftRightWall
    LeftRightWallLength=0;                       //左右墙长度
    SpeakersPerLeftRightWall=0;               //左右墙每侧扬声器个数
    AverangeDistanceOfSpeaker=0;           //左右墙.扬声器之间的平均距离
    DistanceWalltoScreen=0;                     //左右墙,第一个扬声器到前墙距离
    DistanceFirstRowSeatToBoothWall=LeftRightWallLength-DistanceWalltoScreen;

    //BoothWall
    BoothWallLength=0;                       //后墙长度
    TotalSpeakerofBoothWall=0;          //后墙普通扬声器总个数，不包含低音扬声器               
    TotalSpeakersofSubwoofer=0;        //后墙低音炮个数

    //ceiling  floor
    FloorElevationat23point=0;
    FloorElevationatbooth=0;
    CeilingElevationatscreen=0;
    CeilingElevationat23point=0;
    CeilingElevationatbooth=0;
    SpeakersofCeillingPerSide=0;        //顶墙，天花板，左右侧，每侧扬声器个数
}

RoomDim RoomConfig::getRoomDim() const
{
    RoomDim Room;
    //影厅宽度等于前墙后墙的最大值
    Room.width=std::max(BoothWallLength,ScreenWallLength);	
    Room.length=LeftRightWallLength;
    //影厅高度等于天花板高度CeilingElevation的最大值，CeilingElevation大于FloorElevation
    //Room.height=std::max(p->CeilingElevationatscreen,p->CeilingElevationatbooth);
    Room.height=0;
    Room.height=std::max((double)Room.height,CeilingElevationatscreen);
    Room.height=std::max((double)Room.height,CeilingElevationat23point);
    Room.height=std::max((double)Room.height,CeilingElevationatbooth);
    return Room;
}

std::string RoomConfig::gradeToStr() const //档次到字符串
{
    if (grade==0)
    {
        return "Low";
    }
    if (grade==1)
    {
        return "Mid";
    }
    if (grade==2)
    {
        return "High";
    }
    return "Mid";
}

RoomConfig::~RoomConfig()
{
        printf("RoomConfig::~RoomConfig()\n");
}


#include "Super/Basic/BasicOperation.h"

//浮点数==模糊比较
static inline bool myFuzzyCompare(float v1,float v2)
{
    return  fabs(v1-v2)<=0.000001f;
}
static inline bool myFuzzyCompare(double v1,double v2)
{
    return  fabs(v1-v2)<=0.0000000000001;
}


//存放根据功率估算扬声器声压级的参数
struct  PerEstSPL
{
    float power;
    float spl;
    PerEstSPL(float power=100,float spl=105):power(power),spl(spl)
    {

    }
};

struct  EstSPL
{
    std::vector<PerEstSPL> ListEstSPL;
    EstSPL()
    {
        //填充一个根据经验的估算值, 
        ListEstSPL.push_back(PerEstSPL(100,115));   //扬声器100W 的一般最大声压级105dB
        ListEstSPL.push_back(PerEstSPL(150,117));
        ListEstSPL.push_back(PerEstSPL(200,120));
        ListEstSPL.push_back(PerEstSPL(250,122));
        ListEstSPL.push_back(PerEstSPL(300,125));
        ListEstSPL.push_back(PerEstSPL(350,127));
        ListEstSPL.push_back(PerEstSPL(400,128));
        ListEstSPL.push_back(PerEstSPL(450,129));
        ListEstSPL.push_back(PerEstSPL(500,130));
        ListEstSPL.push_back(PerEstSPL(600,132));
        ListEstSPL.push_back(PerEstSPL(700,133));
        ListEstSPL.push_back(PerEstSPL(800,134));
        ListEstSPL.push_back(PerEstSPL(900,134));
        ListEstSPL.push_back(PerEstSPL(1000,135));
        ListEstSPL.push_back(PerEstSPL(1100,135.5));
        ListEstSPL.push_back(PerEstSPL(1200,136));
        ListEstSPL.push_back(PerEstSPL(1300,136.5));
        ListEstSPL.push_back(PerEstSPL(1400,137));
        ListEstSPL.push_back(PerEstSPL(1500,137.5));
        ListEstSPL.push_back(PerEstSPL(1700,138));
        ListEstSPL.push_back(PerEstSPL(1900,138.5));
        ListEstSPL.push_back(PerEstSPL(2100,139));
        ListEstSPL.push_back(PerEstSPL(2300,141));
        ListEstSPL.push_back(PerEstSPL(2500,143));
        ListEstSPL.push_back(PerEstSPL(2700,144));
        ListEstSPL.push_back(PerEstSPL(3000,145));
        ListEstSPL.push_back(PerEstSPL(3300,146));
        ListEstSPL.push_back(PerEstSPL(3600,147));
        ListEstSPL.push_back(PerEstSPL(4000,148));
        ListEstSPL.push_back(PerEstSPL(4500,150));
        ListEstSPL.push_back(PerEstSPL(5000,152));
    }
};


//保证构造顺序用单例函数封装
EstSPL& GetEstSPL()
{
    static EstSPL gEstSPL;
    return gEstSPL;
}



std::string SpeakerTypeNameToDeviceType(const std::string& sSpeakerTypeName)
{
    //if (sSpeakerTypeName==SPKTYPE_LFE)
    if (Comp(sSpeakerTypeName,SPKTYPE_LFE))
    {
        return SPKTYPE_SUBWOOFER;
    }
    return sSpeakerTypeName;
}

ESpkType SpeakerTypeToDeviceType(const ESpkType& spkType)
{
    if (spkType==SpkType_LFE)
    {
        return SpkType_Subwoofer;
    }
    return spkType;
}




void PerSpkInfo::updateValueFromString()
{
    //扬声器枚举值,根据解析到的字符串在此刷新
    SpkType=SpkType_Unknow;

    //扬声器类型某些环节可能是手工填写,宽松容错纠正,排除前缀空格,忽略大小写的区别
    if (Contains(sSpkType,SPKTYPE_SCREEN,false))   //为了兼容性，仅判断是否包含特定字符串
    {
        sSpkType=SPKTYPE_SCREEN;
        SpkType=SpkType_Screen;
    }
    else if (Contains(sSpkType,SPKTYPE_SURROUND,false))
    {
        sSpkType=SPKTYPE_SURROUND;
        SpkType=SpkType_Surround;
    }
    else if (Contains(sSpkType,SPKTYPE_SUBWOOFER,false))
    {
        sSpkType=SPKTYPE_SUBWOOFER;
        SpkType=SpkType_Subwoofer;

        //作为扬声器设备不区分LFE与subwoofer
    }
    else
    {
        sSpkType=SPKTYPE_UNKNOW;
        SpkType=SpkType_Unknow;
    }
    printf("strSpkType:%s SpkType:%d\n",sSpkType,SpkType);



    float powerAll=0;  //不区分频段的功率值
    float powerL=0;
    float powerM=0;
    float powerH=0;
    //解析规则,先搜索CRP,搜索到多个则填充低中高,
    //再搜索LF:CRP MF:CRP: HF:CRP: MHF:CRP: 搜索到则刷新填充对应值
    std::vector<double> ListCRPPower=string_FindAllLabelValueF(sPower,"CRP:");
    if (ListCRPPower.size()>=3)
    {
        powerL=ListCRPPower[0];
        powerM=ListCRPPower[1];
        powerH=ListCRPPower[2];
    }
    else if (ListCRPPower.size()==2)
    {
        powerL=ListCRPPower[0];
        powerM=ListCRPPower[1];
        //powerH=powerM;
    }
    else if (ListCRPPower.size()==1)
    {
        powerL=ListCRPPower[0];
        //powerM=powerL;
        //powerH=powerM;
    }
    ListCRPPower=string_FindAllLabelValueF(sPower,"LF:CRP:");
    if (ListCRPPower.size()>=1)
    {
        powerL=ListCRPPower[0];
    }
    ListCRPPower=string_FindAllLabelValueF(sPower,"MF:CRP:");
    if (ListCRPPower.size()>=1)
    {
        powerM=ListCRPPower[0];
    }
    ListCRPPower=string_FindAllLabelValueF(sPower,"HF:CRP:");
    if (ListCRPPower.size()>=1)
    {
        powerH=ListCRPPower[0];
    }
    ListCRPPower=string_FindAllLabelValueF(sPower,"MHF:CRP:");
    if (ListCRPPower.size()>=1)
    {
        float powertmp=ListCRPPower[0];

        //把功率值估计分给中高频段 暂时如此
        powerM=powertmp*0.7;
        powerH=powertmp*0.3;
    }
    powerAll=powerL+powerM+powerH;

    //如果解析到工程推荐功率值,则覆盖--优先级较高
    ListCRPPower=string_FindAllLabelValueF(sPower,"PRP:");    //Program Recommend Power //
    if (ListCRPPower.size()>=1)
    {
        powerAll=ListCRPPower[0];
    }

    //如果解析到功放推荐功率值,则覆盖--优先级最高
    ListCRPPower=string_FindAllLabelValueF(sPower,"RAP:");    //Program Recommend Power //
    if (ListCRPPower.size()>=1)
    {
        powerAll=ListCRPPower[0];
    }
    //如果以上值都没有解析到就去找连续功率，
    //if (FuzzyCompare(powerAll,0))
    {
        ListCRPPower=string_FindAllLabelValueF(sPower,"CRP-CON:");    //连续功率
        if (ListCRPPower.size()>=1)
        {
            powerAll=ListCRPPower[0];
        }
    }

    //如果以上值都没有解析到就去找2h粉噪功率
    if (myFuzzyCompare(powerAll,0))
    {
        ListCRPPower=string_FindAllLabelValueF(sPower,"PINK2H:");    //2小时粉噪功率
        if (ListCRPPower.size()>=1)
        {
            powerAll=ListCRPPower[0];
        }
    }

    //如果以上值都没有解析到就去找100小时粉噪功率
    if (myFuzzyCompare(powerAll,0))
    {
        ListCRPPower=string_FindAllLabelValueF(sPower,"PINK100H:");    //连续功率
        if (ListCRPPower.size()>=1)
        {
            powerAll=ListCRPPower[0];
        }
    }
    fbPower=FBPower(powerAll,powerL,powerM,powerH);
    power=fbPower.getPower();    //从各种类型功率值取一个典型值用在功放选型参考



    impedance=8;   //解析不到默认8Ω
    //阻抗值一般为4Ω或者8Ω,但是也有区分低中高频段的，用哪个值？
    float ImpedanceL=impedance;
    float ImpedanceM=impedance;
    float ImpedanceH=impedance;
    std::vector<float> ListImpedance=string_FindFloatNum(sImpedance);
    //各种复杂格式完善待续
    if (ListImpedance.size()>=3)
    {
        ImpedanceL=ListImpedance[0];
        ImpedanceM=ListImpedance[1];
        ImpedanceH=ListImpedance[2];
    }
    else if (ListImpedance.size()==2)
    {
        ImpedanceL=ListImpedance[0];
        ImpedanceM=ListImpedance[1];
        ImpedanceH=powerM;
    }
    else if (ListImpedance.size()==1)
    {
        ImpedanceL=ListImpedance[0];
        ImpedanceM=ImpedanceL;
        ImpedanceH=ImpedanceM;
    }
    //impedance=(powerL+powerM+powerH)/3;   不能取平均值,取最小的/中频的？
    //扬声器可能有多个阻抗值,到底用哪个阻抗值去匹配搜索功放功率,三个都比较,模型太复杂了
    //取中频的?  或者取低中高功率最大的那个阻抗值作为扬声器的阻抗值
    impedance=ImpedanceM;    //先默认取中频的
    float powerMax=Max3(powerL,powerM,powerH);
    if (myFuzzyCompare(powerMax,powerL))
    {
        impedance=ImpedanceL;
    }
    else if(myFuzzyCompare(powerMax,powerM))
    {
        impedance=ImpedanceM;
    }
    else if(myFuzzyCompare(powerMax,powerH))
    {
        impedance=ImpedanceH;
    }
    fbImpedance=FBImpedance(ImpedanceL,ImpedanceM,ImpedanceH);




    //声压级,从复杂格式字符串去解析具体值,先复位为一个最低值
    const float SPL_MIN=85;
    SPL=SPL_MIN;
    float splL=SPL_MIN;
    float splM=SPL_MIN;
    float splH=SPL_MIN;
    std::vector<float> ListSpl=string_FindFloatNum(sSPL);
    if (ListSpl.size()==3)
    {
        splL=ListSpl[0];
        splM=ListSpl[1];
        splH=ListSpl[2];
    }
    else if (ListSpl.size()==2)
    {
        splL=ListSpl[0];
        splM=ListSpl[1];
        splH=splM;
    }
    else if (ListSpl.size()==1)
    {
        splL=ListSpl[0];
        splM=splL;
        splH=splM;
    }
    else
    {
        //一下情况下,用户自己添加扬声器，在资料里找不到最大声压级值SPL，根据功率值估算一个,待续
        float estSPL=0;   //功率估计值

        for (size_t n=0;n<GetEstSPL().ListEstSPL.size();n++)
        {
            const PerEstSPL& perEstSpl=GetEstSPL().ListEstSPL.at(n);
            if (power> perEstSpl.power)
            {
                estSPL= perEstSpl.spl;
            }
        }
        splL=splM=splH=estSPL;
    }

    //扬声器最大声压级可能有多个阻抗值的,到底用哪个阻抗值,三个都考虑,模型太复杂
    //取中频的?  或者取低中高功率最大的那个阻抗值的声压级作为扬声器的最大声压级
    //SPL=(splL+splM+splH)/3;    //取平均值
    SPL=splM;                             //先默认取中频的
    powerMax=Max3(powerL,powerM,powerH);
    if (myFuzzyCompare(powerMax,powerL))
    {
        SPL=splL;
    }
    else if(myFuzzyCompare(powerMax,powerM))
    {
        SPL=splM;
    }
    else if(myFuzzyCompare(powerMax,powerH))
    {
        SPL=splH;
    }


    //用新的结构保存更完整数据--具体怎么用待续。与SPL冗余了，
    fbspl=FBSPL(splL,splM,splH);


    //printf("SPL:%f splL:%f splM:%f splH:%f \n",SPL,splL,splM,splH);


}






void PerAmpInfo::updateValueFromString()
{
    float power2ohm=0;
    float power4ohm=0;
    float power8ohm=0;
    float power16ohm=0;
    bool retFind2=false;
    bool retFind4=false;
    bool retFind8=false;
    bool retFind16=false;
    power2ohm=string_FindLabelValueF(sPower,"CRP2:",0,&retFind2);
    power4ohm=string_FindLabelValueF(sPower.c_str(),"CRP4:",0,&retFind4);
    power8ohm=string_FindLabelValueF(sPower.c_str(),"CRP8:",0,&retFind8);
    power16ohm=string_FindLabelValueF(sPower.c_str(),"CRP16:",0,&retFind16);

    //不估算了,没有数据库表格里表示不支持即可
#if 0
    //有些情况数据表格里面没有2Ω的功率值,直接用4Ω的估算
    if (retFind2)
    {
        power2ohm=power4ohm*2;
    }
    //4Ω 8Ω 没有的情况  待续
    //一般情况数据表格里面没有16Ω的功率值,直接用8Ω的估算
    if (!retFind16)
    {
        power16ohm=power8ohm/2;
    }
#endif

    //保存更精确参数
    fiPower=FIPower(power2ohm,power4ohm,power8ohm,power16ohm);
    printf("Power Amp:2:%f 4:%f 8:%f 16:%f\n",power2ohm,power4ohm,power8ohm,power16ohm);




    //桥接功率支持待续,没有暂时用立体声功率估算。待续
    power2ohm=string_FindLabelValueF(sPower,"CRP2:",0,&retFind2);
    power4ohm=string_FindLabelValueF(sPower.c_str(),"CRP4:",0,&retFind4);
    power8ohm=string_FindLabelValueF(sPower.c_str(),"CRP8:",0,&retFind8);
    power16ohm=string_FindLabelValueF(sPower.c_str(),"CRP16:",0,&retFind16);
    fiBridgePower=FIPower(power2ohm,power4ohm,power8ohm,power16ohm);
    printf("BridgePower Amp:2:%f 4:%f 8:%f 16:%f\n",power2ohm,power4ohm,power8ohm,power16ohm);
    //解析到一个值说明支持桥接的，否则不支持,或者在数据库里面用某个标签说明
    if(power2ohm!=0||power4ohm!=0||power8ohm!=0||power16ohm!=0)
    {
        bSupportBridge=true;
    }
    else
    {
        bSupportBridge=false;
    }


    //功放通道数不允许为0
    if (ChannelNum==0)
    {
        ChannelNum=1;
    }

   //解析抵押增益,有3种表现形式,VG:35db  ST:2Ω/1.5V/500W 和ST:2Ω/3dBV/500W--逐步完善待续
    voltageGain=string_FindLabelValueF(sPower,"VG:",0);
}


