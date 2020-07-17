#ifndef XMLHelper_Header
#define XMLHelper_Header

#include "Super/Basic/BasicOperation.h"

#include "struct_define.h"


#include "definition.h"
#include "PublicStruct.h"
#include "SpkInfoExp.h"     //扬声器属性
#include "XmlCommon.h"


#include <string>
#include <vector>
#include <map>
 


#if defined(_WIN32)
    #if defined(FUNCLASS_API_EXPORT)
        #define FUNCLASS_API __declspec(dllexport)
    #elif defined(FUNCLASS_API_IMPORT)
        #define FUNCLASS_API __declspec(dllimport)
    #else
        #define FUNCLASS_API 
    #endif
#else
    #define FUNCLASS_API 
#endif


// Define Channel Type [11/22/2016 killerlife]

 

class QDomDocument;
static const std::string sDefaultGroupName="Custom";


enum 
{
#ifdef BAND61
    NumBand=61  //
#else
    NumBand=31  //
#endif
};




namespace LHC
{
    typedef struct _bassManagement_
    {
        std::wstring subwooferRef;             
        //std::wstring crossoverFrequencyHz;
        //std::wstring gainDb;
        float gainDb;
        int crossoverFrequencyHz;
    }_bassManagement;
    typedef struct _speakerEndpoint_
    {
        //std::wstring id;
        //std::wstring type;
        //std::wstring name;
        //struct _position
        //{
        //    std::wstring x;
        //    std::wstring y;
        //    std::wstring z;
        //}position;
        //std::wstring delay;    //add by lizulin

       //为了效率并方便比较与赋值，改为int类型，refactor by lizulin 20181112
        int id;    //改id为int 类型，保存到文件加SPKREP前缀   20181112 
        std::wstring type;         //扬声器类型 银幕/环绕/低音
        std::wstring name;
        struct _position
        {
            float x;
            float y;
            float z;
        }position;
        float delay;
        std::vector<std::wstring> arrayVector;
        std::vector<_bassManagement> bassManagementVector;

        std::wstring specsRef;

        //广州项目需求新增,20200702
        //值为"default" 表示根据其类型查找对应设备信息，为空表示未设定
        std::wstring sSpkDeviceName;     //实际选配的扬声器型号名称 根据名称获取实际信息
        std::wstring sAmpDeviceName;   //实际选配的功放型号名称     根据名称获取实际信息
    }_speakerEndpoint;

    typedef std::vector<_speakerEndpoint> _speakerEndpointVector;

    typedef struct _speakerSpecs_
    {
        std::wstring id;
        std::wstring type;
        std::wstring manufacturer;
        std::wstring model;
        //std::wstring frequencyResponseMinHz;
        //std::wstring frequencyResponseMaxHz;
        //std::wstring crossoverFrequencyHz;
        //std::wstring peakInputDbuPk;

        int frequencyResponseMinHz;
        int frequencyResponseMaxHz;
        int crossoverFrequencyHz;
        int peakInputDbuPk;
    }_speakerSpecs;

    typedef std::vector<_speakerSpecs> _speakerSpecsVector;

    typedef struct _roomConfiguration{
        std::wstring name;
        //struct
         typedef struct _room_
        {
            struct {
                std::wstring company;
                std::wstring theaterName;
                std::wstring auditorium;
                std::wstring city;
                std::wstring state;
                std::wstring country;
            } info;
            struct  
            {
                float screenWallWidth;
                float boothWallWidth;
                float houseLeftWallWidth;
                float  houseRightWallWidth;
                float  screenHouseLeftInteriorAngle;
                float  screenHouseRightInteriorAngle;
                float  boothHouseLeftInteriorAngle;
                float  boothHouseRightInteriorAngle;
                float  floorElevationAtScreen;
                float  ceilingElevationAtScreen;
                float  floorElevationAtTwoThirds;
                float  ceilingElevationAtTwoThirds;
                float  floorElevationAtBooth;
                float  ceilingElevationAtBooth;

            }dimensions;
            struct 
            {
                float width;
                float height;
                struct  
                {
                    float x;
                    float y;
                    float z;
                }distanceToLeftWall;
            }flatScreen;
            struct 
            {
                //std::wstring width;
                //std::wstring height;
                float width;
                float height;
                struct  
                {
                    float x;
                    float y;
                    float z;
                }distanceToLeftWall;
            }scopeScreen;

            struct  
            {
                std::wstring name;
                struct
                {
                    float screenWallWidth;
                    float boothWallWidth;
                    float houseLeftWallWidth;
                    float houseRightWallWidth;
                    float screenHouseLeftInteriorAngle;
                    float screenHouseRightInteriorAngle;
                    float boothHouseLeftInteriorAngle;
                    float boothHouseRightInteriorAngle;
                    float floorElevationAtScreen;
                    float ceilingElevationAtScreen;
                }dimensions;
                _speakerEndpointVector speakerEndpointVector;
            }region;
        }_room;
         _room room;   //为了便于引用，用 typedef by lizulin
        _speakerSpecsVector speakerSpecsVector;
    }roomConfiguration;

    typedef struct _responsePoint_
    {
        float gainDb;
        float frequencyHz;
    } _responsePoint;

    typedef std::vector<_responsePoint> _eqResponseCurveVector;

    typedef struct _arrayEq_
    {
        float delayMs;
        float gainDb;
        float userGainDb;
        _eqResponseCurveVector eqResponseCurveVector;
        struct  
        {
            std::wstring useDefaultXCurve;
            int lowFreqBoostLimitHz;
            int highFreqBoostLimitHz;
            struct 
            {
                int freqCornerAHz;
                int freqCornerBHz;
                int slopeAdBPerOct;
                int slopeBdBPerOct;
            }xCurveParameters;
        }targetEqResponseCurve;
        std::wstring array;
    }_arrayEq;

    typedef std::vector<_arrayEq> _arrayEqVector;

    typedef struct _speakerEq_
    {
        float delayMs;
        float gainDb;
        float userGainDb;
        _eqResponseCurveVector eqResponseCurveVector;
        struct  
        {
            std::wstring useDefaultXCurve;
            int lowFreqBoostLimitHz;
            int highFreqBoostLimitHz;
            struct 
            {
                int freqCornerAHz;
                int freqCornerBHz;
                int slopeAdBPerOct;
                int slopeBdBPerOct;
            }xCurveParameters;
        }targetEqResponseCurve;
        std::wstring speakerEndpointRef;   //注意 与_outputRoute_里的同名 不同类型，待续20181112
    }_speakerEq;

    typedef std::vector<_speakerEq> _speakerEqVector;

    typedef struct _eqConfiguration_
    {
        std::wstring isEqEnabled;
        //bool isEqEnabled;
        _arrayEqVector arrayEqVector;
        _speakerEqVector speakerEqVector;
    }_eqConfiguration;

    typedef struct _ampPerChannelSpecs_
    {
        int impedanceOhm;
        int powerWatts;
    }_ampPerChannelSpecs;

    typedef std::vector<_ampPerChannelSpecs> _ampPerChannelSpecsVector;

    typedef struct _amplifier_
    {
        std::wstring id;
        std::wstring voltageGainDb;
        std::wstring peakVoltage;
        _ampPerChannelSpecsVector ampPerChannelSpecsVector;
        std::wstring manufacturer;
        std::wstring model;
    }_amplifier;

    typedef std::vector<_amplifier> _amplifierVector;

    typedef struct _outputRoute_
    {
        //std::wstring outputRef;						//OUT-BL-
        //std::wstring speakerEndpointRef;       //SPKREP-
        
    //为了效率方便比较与赋值，改为int类型， by lizulin 20181112
    //参照Dolby 保存到文件再分别添加OUT-BL- /SPKREP-前缀  从文件读取注意去掉前缀   
    int outputRef;
    int speakerEndpointRef;

        std::wstring amplifierRef;
        //std::wstring amplifierTrimDb;
        int amplifierTrimDb;
        std::wstring limiterType;
        std::wstring speakerProcessorRef;
    } _outputRoute;

    typedef std::vector<_outputRoute> _outputRouteVector;

    typedef struct _speakerProcessor_
    {
        std::wstring id;
        std::wstring manufacturer;
        std::wstring model;
        std::wstring peakInputDbuPk;
    }_speakerProcessor;

    typedef std::vector<_speakerProcessor> _speakerProcessorVector;

    typedef struct _blulinkOutput_
    {
        std::wstring id;
        std::wstring peakOutputDbuPk;
    }_blulinkOutput;

    typedef std::vector<_blulinkOutput> _blulinkOutputVector;

    typedef struct _analogOutput_
    {
        std::wstring id;
        std::wstring analogOutputTrimDb;
        std::wstring screenFilterMode;
    }_analogOutput;

    typedef std::vector<_analogOutput> _analogOutputVector;

    typedef struct _outputConfiguration_
    {
        std::wstring isLimiterEnabled;
        std::wstring type;

        // 宏开关ENABLE_AMPLIFIER用于打开关闭配置文件中不用项，以加速配置文件的保存速度 [5/20/2020 jaontolt]
#ifdef ENABLE_AMPLIFIER
        _amplifierVector amplifierVector;
#endif
        _outputRouteVector outputRouteVector;
#ifdef ENABLE_AMPLIFIER
        _speakerProcessorVector speakerProcessorVector;
#endif // ENABLE_AMPLIFIER
#ifdef ENABLE_AMPLIFIER
        _blulinkOutputVector blulinkOutputVector;
#endif // ENABLE_AMPLIFIER
#ifdef ENABLE_AMPLIFIER
        _analogOutputVector analogOutputVector;
#endif // ENABLE_AMPLIFIER
    }_outputConfiguration;

    typedef struct _comments_
    {
        std::wstring text;
        std::wstring time;
    }_comments;

    typedef std::vector<_comments> _commentsVector;

    typedef struct _metadata_
    {
        std::wstring uuid;
        std::wstring lastModifiedTime;
        std::wstring localPathAtOrigin;
        _commentsVector commentsVector;
    }_metadata;

    typedef struct _channelMappingConfiguration_
    {
        char mapping[16]; //16个AES输入
        char templateName[16];
        _channelMappingConfiguration_()
        {
            for(int i = 0; i < 16; i++)
                mapping[i] = i;
            sprintf(templateName, "Passthrough");
        }
    } _channelMappingConfiguration;

//原均衡文件合并到LHC类,Dolby的均衡相关设计太复杂了，简化设计  by lizulin 20180425
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//注意:
//在windos平台wchar_t是2个字节，可以正好存放一个unicode字符，可是linux平台的wchar_t却是4个字节
//以后DCAP做跨平台时注意此处,网络流最好wstring转string后再传输。收到后再还原。网络流交互统一用std::string

//或者可以在使用 gcc 编译程序的时候再后面跟上 -fshort-wchar 来解决这个问题。
//还有一点，不能贸然的添加gcc 编译项 -fshort-wchar，强制将Linux平台下的wchar_t指定成两个字节，
//因为这样做，只会改变你在代码中自己实现的部分，而内部库或者是第三方库中用到的接口和函数都是没有变的，
//仍然采用的是4字节编码。如，std::wstring, QT中的QString等。


//注意:为了手动均衡调节的实时性，不能每一个操作变化UI都打包整个结构成xml流发送到Core端再还原，
//LHC类结构太大，导致效率太低。只能使用自定义流。


        typedef struct _graphicEQ_
        {
            float gain[NumBand];  //固定31个频段  31 61
        }_graphicEQ;
        typedef struct _bassTreble_
        {
            float bass;
            float treble;
            unsigned int nHz;	  //Hz所对应的含义  1 1KHz  2 2KHz  3 3KHz 4 4KHz
        }_bassTreble;
        typedef struct _outPutLevel_
        {
            float level;
        }_outPutLevel;
        //注意_EQTypeAll_成员不能新增string之类非POD结构--已经加了正确实现StreamPut/Get就可以
        typedef struct _EQTypeAll_
        {
             int channel;      //对应实际输出声卡声道号码dout  //小于0或大于最大声道号无效,播放静音
            _graphicEQ graphicEQ;
            _bassTreble bassTreble[4]; //广州的SB要求启用，并扩展到和Dolby一样的4个Band
            _outPutLevel outPutLevel;


            //原dolby targetEqResponseCurvearrayEq有相似概念,在此实现
            std::string sSpkName;   //扬声器名称,在此也保存一份
            std::string sSpkType;    //扬声器类型 L"SubwooferSpeaker" L"surroundSpeaker" L"screenSpeaker"
            //扬声器延时,dobly有此概念,可能通过发出特定脉冲然后由麦克风采集得到扬声器发声延时的
            //与speakerEndpoint里的delay是一个概念,若以后算法支持此值,保存完成后刷新到speakerEndpoint
            float spkDelayMs;   //以ms为单位的扬声器延时

            //扬声器BoostLimit限制扬声器可调均衡的频段范围,低于LowFreq或高于HighFreq不可调大于0dB。
            bool BoostLimitEnabled;   //使能--预留暂不判断
            float BoostLimitLowFreq;
            float BoostLimitHighFreq;


            //为了兼容旧的配置文件没有保存BoostLimit,SpkType信息，
            //根据扬声器名称判断扬声器类型SpkType,根据类型给BoostLimit设置默认值
            void updateInfo();

            //避免异常值，在构造函数初始化
             _EQTypeAll_();
             void print() const;
        }_EQTypeAll;

        //单个扬声器的均衡数据,为了简化设计,UI每一个Slider数据变化都发送以下结构的网络流。Core收到后还原流在保存lhc类里面
        struct EQSpeakerOne
        {
            std::wstring sGroupName;     //所在分组名称
            std::wstring sSpkName;         //扬声器或阵列
            bool isArray;                          //是阵列还是扬声器
            _EQTypeAll eqTypeAll;
            void print() const;
        };
        //每个均衡分组名称下的所有系数
        struct EQGroupOne
        {
            std::map<std::wstring, _EQTypeAll> speakerEQ;   //保存speaker中每个音箱的三种均衡的参数
            std::map<std::wstring, _EQTypeAll> arrayEQ;	     //保存array中每个音箱的三种均衡的参数
            void print() const;
        };

        //所有分组的系数保存在此结构中,只保存在Core的Lhc类中,UI不需要,没有必要转成流
        struct EQCoeffInfo
        {
             bool isEnabledEQ;    //是否开启均衡
             std::wstring CurrentEQGroupName;                           //当前正在使用的分组名称
             std::map<std::wstring,LHC::EQGroupOne> EQALL;   //所有均衡分组数据
             EQCoeffInfo();
        };


        //EQ均衡界面的一些开关状态,封装成一个结构发送，简化网络报文命令字
        struct  EQCtrlSwitchOther
        {
                bool RTA_switch;                   //RTA话筒幻想电源开关
                int WaveType;                       //EWaveType
                bool update_switch;              //更新曲线定时器的开关
                bool rotate;                           //平滑式选择
                bool snap;                             //抓取式选择
        };

    typedef struct _LHC_
    {
        struct _processorConfiguration
        {
            _roomConfiguration roomConfiguration;
            _eqConfiguration eqConfiguration;    //仿照Dolby的均衡xml文件--无用
            _outputConfiguration outputConfiguration;
        } processorConfiguration;
        _metadata metadata;

        //项目中用到的扬声器或功放信息在此保存一份。也可以全部保存,待定 库加起来大约不到200k
        std::map<std::string,PerSpkInfo> mapSpkTable;        //保存特别指定的扬声器信息
        std::map<std::string,PerAmpInfo> mapAmpTable;    //保存特别指定的功放信息

        LHC::EQCoeffInfo eqALLInfo;   //新增自定义的均衡xml文件,放到最后
        MainSpeakerCrossoverCoef mainSpeakerCrossoverCoef;
        _channelMappingConfiguration channelMappingConfiguration[8];
    }_leonisHoleSoundConfig;

    typedef struct _bassManagementChannel_
    {
        int subWooferDout;
        // 支持128个音箱单独用不同的频率做高通滤波，以前的结构定义是所有音箱共用1个频率做高通滤波，现改为VECTOR，不同的音箱可用不同的频率 [6/3/2020 jaontolt]
        std::vector<int>frqHz;   
        std::wstring id;
        std::vector<int> managementDout;
    }bassManagmentChannal;
    typedef std::vector<bassManagmentChannal> _bassManagementChannel;


    //确定映射关系之后的扬声器属性？基本上与_speakerEndpoint相同,重构待续
    typedef struct _speakerMapPoint_
    {
        int id;              //在配置文件中的扬声器id号
        int dout;         //扬声器实际输出，对应声卡的声道号码
        float x;
        float y;
        float z;
        float delay;   //解析到的每个扬声器延时存放在这里  //add by lizulin

        std::wstring strName;   //扬声器名称
        std::wstring type;         //扬声器类型 银幕/环绕/低音
        std::vector<int> array;


        //广州项目需求新增,20200702
        //值为"default" 表示根据其类型查找对应设备信息，为空表示未设定
        std::wstring sSpkDeviceName;     //实际选配的扬声器型号名称 根据名称获取实际信息
        std::wstring sAmpDeviceName;   //实际选配的功放型号名称     根据名称获取实际信息

    }speakerMapPoint;
    typedef std::vector<speakerMapPoint> speakerMapVector;

    typedef struct _arrayPoint_
    {
        std::vector<int> dout;
    }arrayPoint;
    typedef std::vector<arrayPoint> arrayMapVector;
};

//内部结构比较复杂,重载网络流收发函数
size_t NeedBytesToStream(const LHC::_EQTypeAll& tmp);                              //计算结构转换成网络流需要的字节数量
size_t StreamPut(char*& pos,size_t& AccLen,const LHC::_EQTypeAll& tmp);   //
size_t StreamGet(char*& pos,size_t& AccLen,LHC::_EQTypeAll& tmp);

//内部结构比较复杂,重载网络流收发函数
size_t NeedBytesToStream(const LHC::EQSpeakerOne& tmp);                              //计算结构转换成网络流需要的字节数量
size_t StreamPut(char*& pos,size_t& AccLen,const LHC::EQSpeakerOne& tmp);   //
size_t StreamGet(char*& pos,size_t& AccLen,LHC::EQSpeakerOne& tmp);




//ARM-GCC 编译器bug 函数重载推导有问题，在此实现一份   windows是没有必要 BasicOperation.h已实现通用模版
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::vector<LHC::EQSpeakerOne>& Vs)
{
    char* pBegin=pos;
    NetSizeType Vsize=Vs.size();
    StreamPut(pos,AccLen,Vsize);
    for (NetSizeType n=0;n<Vsize;n++)
    {
        StreamPut(pos,AccLen,Vs[n]);  //Vs.at(n)  const T& item=Vs.at(n);
    }
    return pos-pBegin;
}

BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,std::vector<LHC::EQSpeakerOne>& Vs)
{
    char* pBegin=pos;
    NetSizeType Vsize=0;
    Vs.clear();
    StreamGet(pos,AccLen,Vsize);
    Vs.reserve(Vsize);
    for (NetSizeType n=0;n<Vsize;n++)
    {
        LHC::EQSpeakerOne tmp;
        StreamGet(pos,AccLen,tmp);
        Vs.push_back(tmp);
    }
    return pos-pBegin;
}




//内部结构比较复杂,重载网络流收发函数
size_t NeedBytesToStream(const LHC::EQGroupOne& tmp);                              //计算结构转换成网络流需要的字节数量
size_t StreamPut(char*& pos,size_t& AccLen,const LHC::EQGroupOne& tmp);
size_t StreamGet(char*& pos,size_t& AccLen,LHC::EQGroupOne& tmp);



//////////////////////////////////////////////////////////////////////////
// leonisHoleSoundConfig 配置文件格式
// 可参考DAC文件，组织格式与DAC文件一一对应
//////////////////////////////////////////////////////////////////////////
// _leonisHoleSoundConfig
//   |-->processorConfiguration
//   |     |-->roomConfiguration
//   |     |     |-->name				//Dolby显示unknow
//   |     |     |-->room				//房间配置
//   |     |     |     |-->info			//相关信息，内含国家省市公司影院影厅名，见结构体定义
//   |     |     |     |-->dimensions	//房间大小形状定义，见结构体定义
//   |     |     |     |-->flatScreen	//遮幕银幕定义，见结构体定义
//   |     |     |     |-->scopeScreen	//遮幅银幕定义，见结构体定义
//   |     |     |     |-->region		//区域定义，含尺寸音箱，见结构体定义
//   |     |     |-->speakerSpecs		//音箱参数定义，频响范围等，见结构体定义
//   |     |-->eqConfiguration
//   |     |     |-->arrayEq			//分组EQ曲线定义，见结构体定义
//   |     |     |-->speakerEq			//音箱EQ曲线定义，见结构体定义
//   |     |-->CrossOverConfiguration
//   |     |     |-->Speaker			//分组每只音箱的分频参数，见结构体定义
//   |     |-->ChannelMappingConfiguration
//   |     |     |-->Channel			//分组每个通道的映射参数，见结构体定义
//   |     |-->outputConfiguration
//   |           |-->amplifier			//功放参数定义，见结构体定义
//   |           |-->outputRoute		//输出路由定义，见结构体定义
//   |           |-->speakerProcessor	//音频处理器参数定义，见结构体定义
//   |           |-->blulinkOutput		//数字输出参数定义，见结构体定义
//   |           |-->analogOutput		//模拟输出参数定义，见结构体定义
//   |-->processorConfiguration
//         |-->uuid						//UUID
//         |-->lastModifiedTime	    //本文件最后修改时间
//         |-->localPathAtOrigin		//本文件在本地存储的路径
//         |-->comments					//注释，可有多条
//               |-->text				//注释的文本
//               |-->time				//注释写入的时间



//配置文件读写类
class FUNCLASS_API Lhc
{
public:
    Lhc();
    ~Lhc();
    //结构体转dom流
    static bool Parser_StuToDom(QDomDocument &doc, const LHC::_leonisHoleSoundConfig& lhConfig);
    //dom流还原结构体
    static bool Parser_DomToStu(const QDomDocument &doc, LHC::_leonisHoleSoundConfig& lhConfig);

    bool loadFromFile(const char* fileName);			//从XML配置文件中读数据初始化结构
    bool loadFromBuf(const char* buf, int length);	//从XML buffer中读数据生成读数据初始化结构

    bool loadFromBuf(const std::wstring& buf);	
    bool loadFromBuf(const std::string& buf);

    bool saveToFile(const char* fileName, bool bOnlyEQ = false);//生成XML配置文件
    bool saveToBuf(std::wstring& buf);    //从HoleSoundConfig取出数据生成XML buffer，用于从网络等非文件方式写配置文件
    bool saveToBuf(std::string& buf);	       //从HoleSoundConfig取出数据生成XML buffer，用于从网络等非文件方式写配置文件

    LHC::_leonisHoleSoundConfig lhConfig;  //HoleSoundConfig数据结构，执行loadFromXXX后，可从此读出数据
    //执行saveToXXX前，可向此写入数据，以存成配置文件

    // AutoEq网络通讯相关部分 [11/2/2016 killerlife]
    bool autoEqResultFromBuf(char* buf, int length);			//用于把MPU core传回来的AutoEq XML Buffer转化为数组结构
    LHC::_eqResponseCurveVector eqResponseCurveVector;	//用于存放AutoEq后的返回曲线值
    //注意：需先调用autoEqResultFromBuf，然后才能读取正确数据

    bool autoEqResultToBuf(std::string& buf);			//从eqResponseCurveVector读出数据，存为XML BUFFER
    //注意：只供MPU AUTOEQ模块调用

    bool processBassMgr();								//用于从配置文件中生成BassManagment的映射表，无需显式调用，由Load&Save函数自动调用
    LHC::_bassManagementChannel bassMgrChn;		//BassManagment映射表

    bool buildSpeakerMap();							//用于生成音箱分组映射表，无需显式调用，由Load/Save自动调用
    LHC::speakerMapVector speakerMap;	    //音箱与分组的对应表
    LHC::arrayMapVector arrayMap;				//分组与音箱的对应表
    void printSpeakerMap();                           //打印音响映射表，便于调试

    //构建扬声器号与cardinput的声道号之间的映射互查  add by lizulin 20171202
    void bulidMapofSpeakerChannelNum();
    //int getChannelNum(unsigned int SpkNum);   //查询某实际输出声道在cardinput结构中的声道号，-1表示失败
    //int getSpeakerNum(unsigned int ChNum);     //查询cardinput结构中的某声道最后实际输出到的声道号，-1表示失败
    //int SpktoCh[256];  //最大不可能用到256个扬声器，没有用到的赋值为-1，表示无效
    //int ChtoSpk[256];
    //SpkChMap spkChMap;

    //typedef std::vector<int> dout;
    //std::vector<dout> Vdout;
    int getBedout(int bedNum);

    //获取影厅长，宽，高，接口   add by lizulin
    void getRoomDimension(float& length,float& width,float& height);
    std::map<std::wstring,int> MapSpkNameID;

////////////////////////////////////////////////////////////////////////////////////////////////////
    //新增延时功能，UI修改延时,Core同步刷新到结构体，前提是存在对应id的扬声器。
    bool setSpeakerDelay(int spkID,float delayTimes)
    {
        LHC::_speakerEndpointVector& spkVector=lhConfig.processorConfiguration.roomConfiguration.room.region.speakerEndpointVector;
        for(unsigned int i = 0; i <spkVector.size(); i++)
        {
            LHC::_speakerEndpoint& _spkEndpoint=spkVector.at(i);
            if (spkID==_spkEndpoint.id)
            {
                _spkEndpoint.delay=delayTimes;
                PreDprintf("id:%d delay:%f\n",_spkEndpoint.id,_spkEndpoint.delay);
                return true;
            }
        }
        return false;
    }
private:
    // std::vector<Speaker> VSpeker;
    RoomSpkInfo roomSpkInfo;
     void updateSpeakerVector();
public:
    //获取影厅房间尺寸与扬声器位置等属性
    const RoomSpkInfo& getRoomSpkInfo()const {return roomSpkInfo;}
    //获取当前影厅扬声器位置等各种属性描述列表
    const std::vector<Speaker>& getSpeakerList()const {return roomSpkInfo.ListSpkInfo;}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//均衡系数调节功能合并到lhc
private:
    std::string sDefaultFilePath;   //默认保存路径
public:
   void setDefaultFilePath(const std::string& s){sDefaultFilePath=s;}
   std::string getDefaultFilePath(){return sDefaultFilePath;}

   //从影厅配置刷新扬声器名称与id到,均衡配置.避免首次使用时候，扬声器列表为空的情况
    void updateSpkName();

    //取所有配置分组,如果为空,创建一个Custom,并返回Custom字符串列表
    std::vector<std::string> getEQGroupNameList();
    std::string getCurrentEQGroupName();   //当前使用的eq配置分组名称,保证有返回
    void setCurrentEQGroupName(const std::string& s);

    //取某个分组下的eq配置
    LHC::EQGroupOne getEQGroupOne(std::string sEQGroupName);
    void setSpkEqCoeff(const LHC::EQSpeakerOne& tmp);

    //新增或删除EQ配置分组
    void addEQGroupName(const std::string& sEQGroupName);
    void DeleteEQGroupName(const std::string& sEQGroupName);

    bool CopyEQGroupByName(const std::string& destName, const std::string& srcName);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//广州项目需求新增扬声器/功放具体型号属性查询功能 
//旧xml配置没有此项目，以后维护要注意兼容判断 20200711  by lizulin

public:
    //会自动去重复
    void addSpkInfo(PerSpkInfo perSpkInfo,std::string sSpkDeviceName,ESpkType spkType);
    PerSpkInfo getSpkInfo(std::string sSpkDeviceName,ESpkType spkType);
    void clearSpkInfo();

    void addAmpInfo(PerAmpInfo perAmpInfo,std::string sAmpDeviceName);
    PerAmpInfo getAmpInfo(std::string sAmpDeviceName);
    void clearAmpInfo();
private:
    ////项目中用到的扬声器或功放信息在此保存一份。也可以全部保存,待定 库加起来大约不到200k
    //std::map<std::string,PerSpkInfo> mapSpkTable;        //保存特别指定的扬声器信息
    //std::map<std::string,PerAmpInfo> mapAmpTable;    //保存特别指定的功放信息

    //std::map<std::string,PerSpkInfo> mapTableSpecialSpk;        //保存特别指定的扬声器信息
    //std::map<std::string,PerAmpInfo> mapTableSpecialAmp;    //保存特别指定的功放信息
    //根据扬声器设备型号名称查询对应的具体设备信息,
    //若设备名称为default,则根据扬声器类型查询默认的
    //std::string sSpkDeviceName; std::string sAmpDeviceName; 
public:
    //void setSpkInfo(PerSpkInfo perSpkInfo,std::string sSpkDeviceName,ESpkType spkType);
    //PerSpkInfo getSpkInfo(std::string sSpkDeviceName,ESpkType spkType);

    //void setAmpInfo(PerAmpInfo perAmpInfo,std::string sAmpDeviceName,ESpkType spkType);
    //PerAmpInfo getAmpInfo(std::string sAmpDeviceName,ESpkType spkType);

    //void setDefaultSpkInfo(PerSpkInfo perSpkInfo,ESpkType spkType);
    //PerSpkInfo getDefaultSpkInfo(ESpkType spkType);
    //void setDefaultAmpInfo(PerAmpInfo perAmpInfo,ESpkType spkType);
    //PerAmpInfo getDefaultAmpInfo(ESpkType spkType);
private:
    ////选中的各种类型扬声器与功放类型
    //PerSpkInfo defaultUsedSpk[SpkType_Total];
    //PerAmpInfo defaultUsedAmp[SpkType_Total];

    //PerSpkInfo UsedSrceenSpk;
    //PerSpkInfo UsedSurroundSpk;
    //PerSpkInfo UsedSubwooferSpk;
    //PerSpkInfo UsedLFESpk;
    //PerAmpInfo UsedSrceenSpkAmp;
    //PerAmpInfo UsedSurroundSpkAmp;
    //PerAmpInfo UsedSubwooferSpkAmp;
    //PerAmpInfo UsedLFESpkAmp;
};

FUNCLASS_API Lhc* GetLhc();	//返回全局lhc类的指针，用于MPU Core的网络/VBAP/MpuSystem等模块共用一个全局类


namespace PRJ
{
    typedef struct _BASIC_	//工程基本描述信息
    {
        std::string name;			   //工程名
        std::string description;	   //工程描述
        std::string create_time;   	//创建时间，注意为34525这样的1980-1-1至今的秒数
        std::string modify_time;	//修改时间，同上
        std::string driver;			    //工程存储的驱动器盘符

        //多帧率采样率支持--20180410 主要实在PM里面 basic也存一份，一般情况一样的 用unsigned int 方便直接使用
        unsigned int samplerate;  //采样率
        unsigned int framerate;     //帧率
    } BASIC;
    typedef struct _FILTER_DEF_
    {
        float frequency;	//截止频率
        std::string policy;	//内容为LFE或Drop，LFE为滤波后的结果送到低音，Drop为扔掉
    } FILTER_DEF;
    typedef struct _FILTER_
    {
        FILTER_DEF lpf;		//低通滤波器
        FILTER_DEF hpf;		//高通滤波器
    } FILTER;
    typedef struct _DOWNMIX_	//DownMix分配置信息，暂时没想好，祖林补充
    {

    } DOWNMIX;
    typedef struct _SPLITPOLICY_	//PM分割为MXF定义
    {
        int number;					//MXF的reel number
        //////////////////////////////////////////////////////////////////////////
        // 修改begin/end定义 [8/4/2017 killerlife]
        // 修改为时间码方式，不使用帧计数
        // begin 对应 FFOA
        // end 对应 LFOA
        // 计数 TFOA 公式为: TFOA = (AA*3600 + BB*60 + CC) * 24 + DD + 1
#if 0
        std::string begin;			//开始时码，以帧计数表示
        std::string end;			//结束时码，以帧计数表示
#else
        std::string begin;			//FFOA, 开始时码，格式为AA:BB:CC:DD，AA为小时，BB为分，CC为秒，DD为帧数，从0-23(24fps为例）
        std::string end;			    //LFOA, 结束时码，同上
#endif // 0
        //////////////////////////////////////////////////////////////////////////
    } SPLITPOLICY;
    typedef struct _PMITEM_			//单个PM成员定义
    {
        std::string name;			//PM名
        std::string description;	//PM描述
        std::string duration;		//PM长度，以帧计数定义的长度
        std::string create_time;	//同BASIC
        std::string modify_time;	//同BASIC

        //多帧率采样率支持--20180410
        unsigned int samplerate;   //采样率
        unsigned int framerate;     //帧率

        //////////////////////////////////////////////////////////////////////////
        // 修改工程文件格式以支持同步对齐调整 [8/4/2017 killerlife]
        std::string begin_mark;	   //带版头的开始时码，格式为AA:BB:CC:DD，AA为小时，BB为分，CC为秒，DD为帧数，从0-23（24fps为例）
        std::string sync_pop;      //版头的标识时码，一般在比正式开始提前2秒
                                              //和声的设置为begin_mask与sync_pop一致，以版头标识为开始
                                              //与DOLBY的不一致，DOLBY手册上是begin_mark比sync_pop提前8秒
        std::string rec_begin;     //工程录制的开始时间码，格式同上
        std::string rec_end;       //工程录制的结束时间码，格式同上
        std::string delay;           //版头对齐后的偏移值，单位为1个24bit采样点，1帧有2000个24bit点
                                             //正值表示声音比画面早，声音需向后移，实际在数据帧中要向前偏移
                                             //负值表示声音比画面晚，声音需向前移，实现在数据帧中要向后偏移
        //////////////////////////////////////////////////////////////////////////

        std::vector<SPLITPOLICY> split_policy;	//分卷策略
    } PMITEM;
    typedef struct _REELINFO_		//Reel命名策略
    {
        int number;                  //reel number
        std::string name;         //参见SPLITPOLICY
        std::string descripion;
        std::string duration;
        std::string create_time;
        std::string modify_time;
    } REELINFO;
    typedef struct _PROJECT_DEF_
    {
        BASIC basic;
        FILTER filter;
        DOWNMIX downmix;
        std::vector<PMITEM> pm;         //目前只支持处理一个PM，预留扩展
        std::vector<REELINFO> reel;
    } MPU_PROJECT;
}


//工程文件读写类
class FUNCLASS_API mpuprj
{
public:
    mpuprj();
    ~mpuprj();

    static bool Parser_DomToStu(const QDomDocument &doc,PRJ::MPU_PROJECT& prjConfig);
    static bool Parser_StuToDom(QDomDocument &doc,const PRJ::MPU_PROJECT& prjConfig);

    bool loadFromFile(const char* fileName); //从XML工程文件中读出数据生成MPU_PROJECT结构，用于本地工程文件读
    bool loadFromBuf(const char* buf, int length);  //从XML buffer中读出数据生成MPU_PROJECT结构，用于从网络等非文件方式读工程文件


    //从字符串加载,以下两种写法再加载mpu工程时失败,完善待续 20191130.
    bool loadFromBuf(const std::wstring& buf); 
    bool loadFromBuf(const std::string& buf); 

    //若传入字符串为空,会根据工程名保存
    bool saveToFile(const char* fileName=NULL);  //从MPU_PROJECT取出数据生成XML配置文件，用于写本地工程文件
    bool saveToBuf(std::wstring& buf);         //从MPU_PROJECT取出数据生成XML buffer，用于从网络等非文件方式写工程文件
    bool saveToBuf(std::string& buf);	           //从MPU_PROJECT取出数据生成XML buffer，用于从网络等非文件方式写工程文件

    bool loadFromBufTmp(const char* buf, int length);
    bool loadFromFileTmp(const char* fileName);
    bool saveToFileTmp(const char* fileName);
    bool saveToBufTmp(std::string& buf);

    //by lizulin 20180801 
    //mpu的xml工程文件driver字段,检查，若driver字段与文件所在盘符不一致，修正为真实的，可以避免一些奇怪问题。
   static std::string getDriveFromPtah(std::string& path)    //从完整路径提取盘符 windows盘符
    {
        const char* p=strchr(path.c_str(),'\\' );   //取第一个反斜杠位置
        if (p==NULL)  
        {
            p=strchr(path.c_str(), '/' );   //正斜杠
        }
        if (p)
        {
            int len=(int)(p-path.c_str());
            return std::string(path.c_str(),len);
        }
        return std::string();
    }
    static void CheckProject(std::string& prjpath)
    {
        mpuprj mpu;
        mpu.loadFromFile(prjpath.c_str());
        std::string sdrive=getDriveFromPtah(prjpath);
        sdrive+="\\";
        printf("getDriveFromPtah() path:%s driver:%s\n",prjpath.c_str(),sdrive.c_str());
        if (mpu.mpuProject.basic.driver!=sdrive)   //工程文件的盘符与真实不符合
        {
            printf("mpuProject.basic.driver:%s\n",mpu.mpuProject.basic.driver.c_str());
            mpu.mpuProject.basic.driver=sdrive;   //修正driver为真实盘符然后保存
            mpu.saveToFile(prjpath.c_str());
        }
    }

    //bool encodeFinish(long long frames);
    //////////////////////////////////////////////////////////////////////////
    // 修改支持同步对齐 [8/9/2017 killerlife]
    // 2个参数：begin - LTC开始时码转成的帧号，end - LTC结束时码转成的帧号
    bool encodeFinish(unsigned int begin,unsigned int end,unsigned int SampleRate,unsigned int FrameRate);     
    //////////////////////////////////////////////////////////////////////////

    std::string getPMorMxfFullPath();		//返回当前活动的PM或MXF全路径
    std::string getPMorMxfName();

    PRJ::MPU_PROJECT&  getmpuProject(){return this->mpuProject;}   //提供外部访问方法

    //--为了导出dll，改非static
    PRJ::MPU_PROJECT tmpProject;      //非全局临时MPU_PROJECT数据结构，执行loadFromBufTmp后，可从此读出数据
                                                            //执行saveToXXX前，可向此写入数据，以存成工程文件
    PRJ::MPU_PROJECT mpuProject;     //MPU_PROJECT数据结构，执行loadFromXXX后，可从此读出数据
    std::string strPmOrMxf;                   //存放当前活动的PM或MXF的全路径名
    std::string strPmName;	                   //存放当前活动的PM或MXF名
    PM_SPLIT_POLICY pmSplitPolicy; //存放PM分卷策略结构
private:
};


FUNCLASS_API mpuprj* GetMpu(); //返回全局mpuprj类的指针，用于MPU Core的网络/MPU UI/MPU Render/MPU Encoder等模块共用一个全局类


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//Setting文件读写类
class FUNCLASS_API setting
{
public:
    setting();
    ~setting();
    bool loadFromFile(const char* fileName="");   //从XML工程文件中读出数据生成MPU_PROJECT结构，用于本地工程文件读
    bool saveToFile(const char* fileName="");      //从setting取出数据生成XML配置文件，用于写本地工程文件
    Setting set;
private:
};

FUNCLASS_API setting* Getsetting(); //返回全局setting类的指针，用于MPU Core的网络/MPU UI/MPU Render/MPU Encoder等模块共用一个全局类




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//用户名密码管理文件读写类,简化设计直接用文本文件存放用户名密码，有空可以考虑把密码做hash再保存，密码存进数据库。

struct  KeyInfo
{
    struct  PerUser
    {
        std::string sUserName;
        std::string sPassWord;
        std::string sLastEditTime;   //最后一次密码修改实际
    };
    enum EUserType
    {
        User_admin=0,           
        User_users,      
        
        //精简用户类型，
        //User_technician,    
        //User_manager,      
        //User_operator,   
        User_Total
    };
   PerUser perUser[User_Total];   //简化采用固定用户名称数量，不可以注册，没有必要
    //std::vector<PerUser> VUsers;
   KeyInfo()
   {
       perUser[User_admin].sUserName="admin";
       perUser[User_admin].sPassWord="123456";
       perUser[User_users].sUserName="users";
       perUser[User_users].sPassWord="123456";

       //perUser[User_technician].sUserName="technician";
       //perUser[User_technician].sPassWord="123456";
       //perUser[User_manager].sUserName="manager";
       //perUser[User_manager].sPassWord="123456";
       //perUser[User_operator].sUserName="operator";
       //perUser[User_operator].sPassWord="123456";
   }
};

static const char* default_UserInfoPath=USERINFO_FILE_PATH;
class FUNCLASS_API SysKeyInfo
{
public:
    SysKeyInfo();
    ~SysKeyInfo();
    static bool parser_StuToDom(QDomDocument& doc,const KeyInfo& SKInfo);//结构体转dom->xml
    static bool parser_DomToStu(const QDomDocument& doc,KeyInfo& SKInfo);         //xml->dom还原结构体
    bool loadFromFile(const char* fileName =default_UserInfoPath);
    bool saveToFile(const char* fileName =default_UserInfoPath);
    ELoginPrivileges checkLogin(std::string sUserName,std::string sPassWord);   //检查用户名密码是否存在，并返回取得的登陆权限
    bool ModifyPassWord(std::string sUserName,std::string sPassWordOld,std::string sPassWordNew);
    KeyInfo keyInfo;
private:
};
FUNCLASS_API SysKeyInfo* GetSysKeyInfo();



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////原设计废弃
//#if 0
//#define CHANNEL_TOTAL 16
//#define CHANNEL_USABLE 10
//typedef struct _MonitorMixStruct_
//{
//    bool channelChecked[CHANNEL_TOTAL];
//    bool isMix;
//}_MonitorMixStruct;
//
////////////////////////////////////
//typedef struct _graphicEQ_
//{
//    float gain[31];
//}_graphicEQ;
//
//typedef struct _bassTreble_
//{
//    float bass;
//    float treble;
//
//    /**************
//     * Hz所对应的含义
//     * 1 1KHz
//     * 2 2KHz
//     * 3 3KHz
//     * 4 4KHz
//    ***************/
//    unsigned short Hz;	
//}_bassTreble;
//
//typedef struct _outPutLevel_
//{
//    float level;
//}_outPutLevel;
//
//
////曲线点结构
//typedef struct _responsePoint_
//{
//    std::wstring gainDb;
//    std::wstring frequencyHz;
//} _responsePoint;
//
//typedef std::vector<_responsePoint> _eqResponseCurveVector;
//
//typedef struct _speaker_
//{
//    bool byPass_AutoEQ_switch;
//
//    typedef struct _speaker_data_
//    {
//        int channel;
//
//        _eqResponseCurveVector eqResponseCurves;
//        _graphicEQ graphicEQ;
//        _bassTreble_ bassTreble;
//        _outPutLevel outPutLevel;
//
//    }_speaker_data;
//
//    std::vector<_speaker_data> dataList;
//
//}_speaker;
//
//typedef struct _array_
//{
//    bool byPass_AutoEQ_switch;
//
//    typedef struct _array_data_
//    {
//        std::wstring arrName;
//
//        _eqResponseCurveVector eqResponseCurves;
//        _graphicEQ graphicEQ;
//        _bassTreble_ bassTreble;
//        _outPutLevel outPutLevel;
//    }_array_data;
//
//    std::vector<_array_data> dataList;
//}_array;
//
//
//typedef struct _micro_
//{
//    std::wstring name;
//    /***********************************************
//     *  sound值对应如下：
//     *  0 粉红噪声(pink noise)
//     *  1 100Hz正弦波(100Hz sine)
//     *  2 1Hz正弦波(1Hz sine)
//     *  3 10Hz正弦波(10Hz sine)
//     *  4 低音噪声(thump)
//     *  5 扫描信号音(Sweep)
//    ************************************************/
//    bool sound_switch;
//    unsigned int sound;
//
//    bool rotate;                           //平滑式选择
//    bool snap;                             //抓取式选择
//
//    bool RTA_switch;                       //RTA话筒幻想电源开关
//
//    _speaker speaker;                      //音响数据
//    _array array;                          //阵列数据
//}_Micro;                                                             
//
//typedef struct _Macro_
//{
//    std::string name;                                                   //宏名称
//    unsigned char GPIOInput;                 //前面板按钮和GPIO输入
//    float faderPreset;                       //音量预设
//    bool faderPreset_switch;                 //音量预设开关
//    float delay;                             //全局声音延时(毫秒)
//    std::string input;                       //输入
//    std::string format;                      //格式
//    std::vector<bool> bitStreams;            //输入支持的码流(6种码流)
//    std::string channel_assignment_set_type; //声道分配设置类型
//    std::vector<std::string> channels;       //声道分配(大小为16)
//    bool stress_switch;                      //重音开关
//    std::vector<std::string> surrounding;    //环绕声分配
//    float inputTrim;                         //输入微调
//    std::string eqPreset;                    //均衡预设
//    std::string note;                        //注释
//}_Macro;
//
//#endif
//
//#if 0
//
//#endif
//
//

//#if 0
//class FUNCLASS_API XMLHelper
//{
//public:
//    XMLHelper(void);
//    ~XMLHelper(void);
//
//    //static void EQToXML(std::vector<_Micro> &vec, QDomDocument &doc);
//    //static void EQFromXML(QDomDocument &doc, std::vector<_Micro> &vec);
//    //static void UpdateEQXMLFile(std::vector<_Micro> &vec, const char *EQXML);
//    //static void LoadEQXMLFile(std::vector<_Micro> &vec, const char *EQXML);
//    //static char* EQToBuf(std::vector<_Micro> &vec, unsigned int &len);
//    //static bool EQFromBuf(std::vector<_Micro> &vec, char* buf, unsigned int len);
//
//    //static void EQToXML(_EQUIStruct &e, QDomDocument &doc); 
//    //static void EQFromXML(QDomDocument &doc, _EQUIStruct &e);
//    //static int EQToBuf(_EQUIStruct &e, char *buf);
//    //static bool EQFromBuf(_EQUIStruct &e, char* buf, unsigned int len);
//    //static void UpdateEQXMLFile(std::vector<_EQUIStruct> &vec, const char *EQXML);
//    static void UpdateEQXMlFile(QDomDocument &doc, const char *EQXML);
//    static void LoadEQXMLFile(QDomDocument &doc, const char *EQXML);
//
//    static void GetAllEQName(char *buf, int &len, const char *EQXML);
//    static void DelEQConfig(const std::string EQName, const char *EQXML);
//    static void GetGraphicEQGain(std::map< unsigned short, std::vector<float> > &gainData,const std::string &EQName, const char *EQXML);
//    static void GetEQConfig(const std::string EQName, char *buf, unsigned int &len, const char *EQXML);
//    static void UpdateEQConfig(char *buf, unsigned len, const char *EQXML);
//#if 1	
//    //////////////////////////////////////////////////////////////////////////
//    //Add by Killerlife
//    //Get EQStruct with giving EQ name
//    //static void GetEQConfig(_EQUIStruct &e, const std::string EQName, const char* EQXML);
//    //////////////////////////////////////////////////////////////////////////
//#endif
//    static void MacroToXML(std::vector<_Macro> &mVec, std::string &nCurrent, QDomDocument &doc);
//    static void MacroFromXML(QDomDocument &doc, std::vector<_Macro> &mVec, std::string &nCurrent);
//    static void UpdateMacroXMLFile(std::vector<_Macro> &mVec, std::string &nCurrent, const char *MacroXML);
//    static void LoadMacroXMLFile(std::vector<_Macro> &mVec, std::string &nCurrent, const char *MacroXML);
//    static char* MacroToBuf(std::vector<_Macro> &mVec, std::string &nCurrent, unsigned int &len);
//    static bool MacroFromBuf(std::vector<_Macro> &mVec, std::string &nCurrent, char* buf, unsigned int len);
//
//    /////////////////////////////////////////////////////////////////////
//    static void MonitorMixToXML(_MonitorMixStruct &m, QDomDocument &doc);
//    static void MonitorMixFromXML(_MonitorMixStruct &m, QDomDocument &doc);
//
//    static char * MonitorMixToBuf(_MonitorMixStruct &m, unsigned int &len);
//    static void MonitorMixFromBuf(_MonitorMixStruct &m, char *buf, unsigned int len);
//
//    static void UpdateXMLFile(char *buf, unsigned int len, char *XMLPath);
//    static char *LoadXMLFile(unsigned int &len, char *XMLPath);
//
//    static bool ReadXML(QDomDocument &doc, const char *XML);
//    static bool WriteXML(QDomDocument &doc, const char *XML);
//
//    static char * XMLToBuf(QDomDocument &doc, unsigned int &len);
//    static bool BufToXML(char *buf, unsigned int len, QDomDocument &doc);
//};
//
//FUNCLASS_API std::vector<_Macro>* GetMacroVector();
//FUNCLASS_API std::string* GetActiveMacro();
//FUNCLASS_API void SetActiveMacro(char* name);
//#endif




#endif // !XMLHELPER_H
