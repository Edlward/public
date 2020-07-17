#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H
//整个沉浸式音频项目共用的数据结构，以及一些与具体项目相关的共同操作封装

//public struct share between with Core And UI by lizulin

#include <string>
#include <vector>



//panner 插件坐标系问题待彻底确认
#define USE_DOLBY_AXIS   //if use Dolby Axis





//采样率,帧率每帧采样点数管理类,与时码转换相关的的多处被使用,为了方便独立出来可以作为其他类内部类
struct RateInfo
{
public:
    void setSampleRate(unsigned int n){SampleRate=n;}
    void setFrameRate(unsigned int n){FrameRate=n;}
    unsigned int getSampleRate() const{return SampleRate;}
    unsigned int getFrameRate() const{return FrameRate;}
    unsigned int getPerSample() const{return SampleRate/FrameRate;}   //返回每帧采样点数，根据采样率与帧率
private:
    unsigned int SampleRate;  
    unsigned int FrameRate;
};

//cardinput结构与扬声器实际输出映射互查，可能在多个地方使用。
//为了XMLHelper LHC类不引入复杂依赖，作为单例类。放此处
class SpkChMap
{
public:
    SpkChMap()
    {
        init();
    }
    void init() 
    {
        SpkNum=0;
        const unsigned int ArraryMax=sizeof(SpktoCh)/sizeof(SpktoCh[0]);
        for (int n=0;n<ArraryMax;n++)
        {
            SpktoCh[n]=-1;   //-1表示默认映射无效
            ChtoSpk[n]=-1;
        }
    }
    int getChannelNum(unsigned int SpkNum)const   //查询某实际输出声道在cardinput结构中的声道号，-1表示无效
    {
        if (SpkNum>(sizeof(SpktoCh)/sizeof(SpktoCh[0])))  //256
        {
            return -1;
        }
        return SpktoCh[SpkNum];
    }

    int getSpeakerNum(unsigned int ChNum)const      //查询cardinput结构中的某声道最后实际输出到的声道号，-1表示无效
    {
        if (ChNum>(sizeof(ChtoSpk)/sizeof(ChtoSpk[0])))  //256
        {
            return -1;
        }
        return ChtoSpk[ChNum];
    }
    unsigned int getSpkNum()const {return SpkNum;}
    int SpktoCh[256];         //最大不可能用到256个扬声器，没有用到的赋值为-1，表示无效
    int ChtoSpk[256];
    unsigned int SpkNum; //扬声器数量
};

SpkChMap* GetSpkChMap();



#pragma pack(push,1)     //Align by 1 byte

//注意:
//以下音频对象信息相关结构,Position; PositionInfo; ObjMetaData 
//数据成员,位置与结构大小不可更改,否则会导致与现有PM文件不兼容(不可以再播放/转码之前PM母版文件)

typedef struct _Position_
{
    float x;   //x,y,z范围0-1.0f  为了效率使用过程很多地方不判断，在赋值处保证不溢出
    float y;
    float z;
    _Position_()
    {
        //为了随处例化提高效率 不需要构造函数
        x=y=z=0.0f;
        //必须复位，为了性能不在构造函数复位在其他使用处复位，否则可能坐标溢出
    }
    static void LimitRange(float& fpos)  //调试使用
    {
        if (fpos>1.0)
        {
            //printf("Pos: Out of range: fpos=%f\n",fpos);
            fpos=1.0;
        }
        else if (fpos<0.0)
        {
            //printf("Pos: Out of range: fx=%f y=%f z=%f\n",x,y,z);
            //printf("Pos: Out of range: fpos=%f\n",fpos);
            fpos=0.0;
        }
    }
    void checkRange()
    {
        if(x>1.0f||x<0.0f||y>1.0f||y<0.0f||z>1.0f||z<0.0f)
        {
              printf("Pos: Out of range: x=%f y=%f z=%f\n",x,y,z);
        }
        //LimitPosRange();
    }
    void LimitPosRange() //平滑过程坐标值可能溢出，限制范围
    {
        LimitRange(x);
        LimitRange(y);
        LimitRange(z);
    }
}Position;


class PositionInfoTmp:public Position
{
public:
        //according to the document IABitstream_SMPTE_v3.doc 
        //标准文档数据类型 单位bit
        //PanInfoExists...............................................1
        //ObjectGainPrefix.........................................2
        //ObjectGain[sb] ........................................... 10
        //ObjectPosX[sb]............................................ 16
        //ObjectPosY[sb]............................................. 16
        //ObjectPosZ[sb]............................................. 16
        //ObjectSnap[sb]...............................................1
        //ObjectSnapTolExists .....................................1
        //ObjectSnapTolerance[sb] ............................12
        //ObjectZoneControl.........................................1
        //ZoneGainPrefix ......................................... 2
        //ZoneGain[sb][n].................................... 10
        //ObjectSpreadMode.................................. 2
        //ObjectSpreadX[sb] ........................................ 12
        //ObjectSpreadY[sb] ........................................ 12
        //ObjectSpreadZ[sb] ........................................ 12
        //ObjectDecorCoefPrefix .................................. 2
        //ObjectDecorCoef[sb] ....................................... 8        
    //实际使用数据类型，为了简化大小端,不使用位段，为了效率，重排成员顺序，字节对齐
    //bool PanInfoExists;
    //bool GainPrefix;
    float Gain; // 经检查代码，此 Gain 被用于显示对象动态变化的 Gain，需另定义一个 Gain 用于保存 Bitstream 中的 ObjectGain，在后面定义  [11/14/2018 jaontolt]
    //bool Snap;
    //bool SnapTolExists;
    float SnapTolerance;
    //bool ZoneControl;
    //bool ZoneGainPrefix;
    float ZoneGain[9];
    //char SpreadMode;
    float SpreadX;	// SpreadMode中Spread共用SpreadX表示 [11/16/2018 jaontolt]
    float SpreadY;
    float SpreadZ;
    //bool DecorCoefPrefix;
    float DecorCoef;

    //8个字节--移到后面
    bool PanInfoExists;
    bool GainPrefix;
    bool Snap;
    bool SnapTolExists;
    bool ZoneControl;
//     bool ZoneGainPrefix;
	char ZoneGainPrefix[9]; // ZoneGainPrefix 为2位，不能用bool判断，有4种状态 [11/16/2018 jaontolt]
	//////////////////////////////////////////////////////////////////////////
	// 增加 ZoneDefinition19支持，此内容从ObjectZoneDefinition19中读出 [11/16/2018 jaontolt]
	// 如果存在ZoneDefinition19，上面的ZoneGain无效，用此ZoneGain19替代
	bool ZoneInfoExists;
	char ZoneGainPrefix19[19];
	float ZoneGain19[19];
	//////////////////////////////////////////////////////////////////////////

    char SpreadMode;
    bool DecorCoefPrefix;

    //标准没有，可能会用到的
    float size;      //
    //int recv[4];  //预留4个未来扩充

	// 保存 Bitstream 中的 ObjectGain [11/14/2018 jaontolt]
	float ObjectGain;

    PositionInfoTmp()
    {
        //memset(this,0,sizeof(*this));   //

        //为了随处例化提高效率 不需要构造函数
        //char* pstart=(char*)this;            
        //for (int i=0;i<sizeof(*this);++i)  //clear buf without memset function
        //{
        //	pstart[i]=0;
        //}
    }
    void print()
    {
        printf("x:%0.5f y:%0.5f z:%0.5f\n",x,y,z);
    }
};


//仅仅是为了保证 单个block对象信息大小定长而引入的中间类
//PositionInfoTmp不大于96字节
class PositionInfo:public PositionInfoTmp
{
    char Resv[24*9-sizeof(PositionInfoTmp)];   
};



class ObjMetaDataTmp
{
public:
    int id;
    unsigned int timeInFrameNum;   //用不到暂时保留，填充帧号，而不是时间值，
    unsigned int silent;  //if AudiodataID=0 response a silent audio;
    enum
    {
        NumPanSubBlocksMax=8     //according to document IABitstream_SMPTE_v3.doc the max of NumPanSubBlocks=8,
    };
    unsigned int NumSubBlocks;    //NumPanSubBlocksMax,
    PositionInfo SubPos[NumPanSubBlocksMax];   // 

    //预留4个保留字
    //int recv[4];
    //unsigned int LtcNum; 
    //unsigned int FrameRate; 

    ObjMetaDataTmp()
    {
        //为了随处例化提高效率 不需要构造函数
        //char* pstart=(char*)this;            
        //for (int i=0;i<sizeof(*this);++i)  //clear buf without memset function
        //{
        //	pstart[i]=0;
        //}
        //timeInFrameNum = 0;
        id=-1;  //-1default value   默认无效
    }
    unsigned int getSpeedLevel();   //根据子帧首位坐标求运动速度等级，决定是否进一步填充平滑子帧
    unsigned int getSmoothSubBlocks(unsigned int FrameRate);
    //填充子帧,注意平滑子帧是坐标值可能会溢出
    void FillSmoothBlocks(PositionInfo SmoothSubPos[80],unsigned int NumSubSmoothBlocks);
    void checkRange();    //检查对象数据是否越界，调试使用
    void limitRange();      //抹平越界，调试使用
};


//仅仅是为了保证 整个对象信息大小定长而引入的中间类
//ObjMetaDataTmp不大于800字节
class ObjMetaData:public ObjMetaDataTmp
{
public:
    ObjMetaData()
    {
       // printf("sizeof(PositionInfo)=%d sizeof(ObjMetaData)=%d\n",sizeof(PositionInfo),sizeof(ObjMetaData));
    }
    char Resv[1800-sizeof(ObjMetaDataTmp)];   
};


//本结构参考自juce库"juce_AudioPlayHead.h",很多字段无用暂留。未来扩展需要

//==============================================================================
/** This structure is filled-in by the AudioPlayHead::getCurrentPosition() method.
*/
struct  CurrentTimeCode
{
    enum FrameRateType
    {
        fps24           = 0,
        fps25           = 1,
        fps2997         = 2,
        fps30           = 3,
        fps2997drop     = 4,
        fps30drop       = 5,
        fpsUnknown      = 99
    };

    /** The tempo in BPM */
    double bpm;

    /** Time signature numerator, e.g. the 3 of a 3/4 time sig */
    int timeSigNumerator;
    /** Time signature denominator, e.g. the 4 of a 3/4 time sig */
    int timeSigDenominator;

    /** The current play position, in samples from the start of the edit. */
    unsigned long long timeInSamples;   //old    int64 timeInSamples;

    /** The current play position, in seconds from the start of the edit. */
    double timeInSeconds;

    /** For timecode, the position of the start of the edit, in seconds from 00:00:00:00. */
    double editOriginTime;

    /** The current play position, in pulses-per-quarter-note. */
    double ppqPosition;

    /** The position of the start of the last bar, in pulses-per-quarter-note.
        This is the time from the start of the edit to the start of the current
        bar, in ppq units.

        Note - this value may be unavailable on some hosts, e.g. Pro-Tools. If
        it's not available, the value will be 0.
    */
    double ppqPositionOfLastBarStart;

    /** The video frame rate, if applicable. */
    int frameRate;    //old  FrameRateType frameRate; 用int替代原枚举

    /** True if the transport is currently playing. */
    bool isPlaying;

    /** True if the transport is currently recording.

        (When isRecording is true, then isPlaying will also be true).
    */
    bool isRecording;

    /** The current cycle start position in pulses-per-quarter-note.
        Note that not all hosts or plugin formats may provide this value.
        @see isLooping
    */
    double ppqLoopStart;

    /** The current cycle end position in pulses-per-quarter-note.
        Note that not all hosts or plugin formats may provide this value.
        @see isLooping
    */
    double ppqLoopEnd;

    /** True if the transport is currently looping. */
    bool isLooping;

    //==============================================================================
    //bool operator== (const CurrentPositionInfo& other) const noexcept;
    //bool operator!= (const CurrentPositionInfo& other) const noexcept;
    void resetToDefault()
    {
        //timeInSamples=0;
        //timeInSeconds=0;
        //editOriginTime=0;
    }
    void print()
    {
        printf("timeInSamples:%lld timeInSeconds:%f editOriginTime:%f bpm:%f timeSigNum:%d Denom:%d\n",
            timeInSamples,timeInSeconds,editOriginTime,bpm,timeSigNumerator,timeSigDenominator
            );
        printf("ppqPosition:%f ppqPositionOfLastBarStart:%f ppqLoopStart:%f ppqLoopEnd:%f\n",
            ppqPosition,ppqPositionOfLastBarStart,ppqLoopStart,ppqLoopEnd
            );
        printf("isPlaying:%d isRecording:%d isLooping:%d\n",
            isPlaying,isRecording,isLooping);
        printf("FrameRateType:%d ",frameRate);
    }
};


//网络信息
struct NetAdapterInformation 
{
    int index;
    std::string name;
    std::string ip;
    std::string netmask;
    std::string gateway;
    std::string dns;
};


typedef struct  _Speaker_
{
public:
    float x;
    float y;
    float z;
    float sound_loud;
    int   channel;
    char name[10];     //30*128
    int special;
    char comment[64];
}Speaker;



//use in MPU Core Monitor
typedef struct  _DIM_
{
    float x;
    float y;
    float z;
}DIM;


typedef struct _AUTOEQ_
{
    short speakerId;
    float gain;
    float limitLow;
    float limitHigh;
    float freqA;
    float freqB;
    float Adb;
    float Bdb;
} AUTOEQ;



//原来并没有  需要跟core里面的同名结构同步

//待拷贝文件源与目的路径,为了方便考虑最长路径,用定长数组
typedef struct _copy_path_
{
    char path_src[1024];
    char path_dst[1024];
public:
    _copy_path_()
    {
        memset(path_src, 0,sizeof(path_src));
        memset(path_dst, 0,sizeof(path_dst));
    }
}copy_path;




//传递包含变长字符串结构的新方法--没有必要定义SLV结构和ID然后收发双方每一个字符串依次拼接--by lizulin 20171216
//使用以下辅助类配合即可
class StrToStreamHelper
{
    std::string* pData;                 //point to first string arrary
    const unsigned int Num;       //number of string arrary
public:
    //传递字符串数组首地址与数组大小构造辅助类
    StrToStreamHelper(std::string* pData,unsigned int Num):pData(pData),Num(Num){}
    ~StrToStreamHelper(){}
    unsigned int getTotallength()         //填充流之前取所有字符串长度--非必要
    {
        unsigned int Len=0;
        for (unsigned int n=0;n<Num;n++)       
        {
            Len+=pData[n].length(); 
        }
        return Len;
    }
    //把string的内容串联成一个buffer流,返回已使用的字节数(用在连续拼接多个本结构)       发送方调用
    unsigned int StringtoStream(char* buf)               
    {
        char* pos=buf;
        for (unsigned int n=0;n<Num;n++)       
        {
            *((unsigned int*)pos)=pData[n].length();                //填充字符串长度
            pos+=sizeof(unsigned int);
            memcpy(pos,pData[n].c_str(),pData[n].length());     //填充字符串内容
            pos+=pData[n].length();   
        }
        return pos-buf;
    }
    //从buffer里面还原每一个字符串原内容--返回已使用的字节数(用在连续拼接多个本结构)---接收方调用
    unsigned int StreamtoString(char* buf)                     
    {
        char* pos=buf;
        for (unsigned int n=0;n<Num;n++)       
        {
            unsigned int sLen=*((unsigned int*)pos);         //取字符串长度
            pos+=sizeof(unsigned int);   
            pData[n]=std::string(pos,sLen);                        //从buffer里面还原字符串内容
            pos+=sLen;   
        }
        return pos-buf;
    }
    void print()
    {
        printf("totalLength:%d\n",getTotallength());
        for (unsigned int n=0;n<Num;n++)       
        {
            printf("str[%d]:len=%-3d:%s\n",n,pData[n].length(),pData[n].c_str());
        }
    }
};



struct KDMInfo
{
    enum{
        C_BEGIN=-1,     //辅助计数，决定起始位置
        C_ID,
        C_NAME,
        C_NOTVALID_BEFORE,
        C_NOTVALID_AFTER,
        C_CPLID,
        C_LOCATE,
        C_TOTAL,      //字符串数组总个数
    };
    std::string pData[C_TOTAL];              //字符串原始结构体或者还原后的结构体
};


//使用方法 
//char buf[2000];
//char* pos=buf;
//KDMInfo   KdmInfo;
//StrToStreamHelper SHelper(KdmInfo.sData,KdmInfo.C_TOTAL);
//SHelper.print();  //only test 
//unsigned int lenUse=SHelper.StringtoStream(pos);    //字符串结构体转流
//unsigned int lenUse=SHelper.StreamtoString(pos);    //从流还原字符串结构体


//废弃使用SKL结构传递变长字符串的方法
#if 0   
//SLV
typedef struct _SLV_
{
    unsigned short m_sID;
    unsigned short m_length;
} SLV; 

enum StringID
{
    CONTENT_ID = 0x80,
    CONTENT_NAME,
    CONTENT_PROGRESS,
    CONTENT_STATUS,
    CONTENT_FORMAT,
    CONTENT_FILMLENGTH,
    CONTENT_PROGRAMLENGTH,
    CONTENT_STEREOSCOPIC,
    CONTENT_ISSUER,
    CONTENT_ISSUEDATE,
    CONTENT_TIMERANGE,
    CONTENT_RECVSEGMENT,
    CONTENT_TOTALSEGMENT,
    CONTENT_RECV_DATETIME,
    CONTENT_LOCATE,
//	CONTENT_PATH_SRC,   //?·?
//	CONTENT_PATH_DST,   //?·?
};

enum{
    C_ID = 0,
    C_NAME,
    C_PROGRESS,
    C_STATUS,
    C_FORMAT,
    C_FILM_LENGTH,
    C_PROGRAM_LENGTH,
    C_STEREO,
    C_ISSUER,
    C_ISSUEDATE,
    C_TIMERANGE,
    C_RECVSEGMENT,
    C_TOTAL_SEGMENT,
    C_RECV_DATETIME,
    C_LOCATE,
};
#endif




/*
enum{
    C_ID = 0,
    C_NAME,
    C_HAS_KDM,
    C_NEED_KDM,
    C_FORMAT,
    C_FILM_LENGTH,
    C_PROGRAM_LENGTH,
    C_STEREO,
    C_ISSUER,
    C_ISSUEDATE,
    C_PKL_FILE,
    C_NOT_VALID_BEFORE,
    C_NOT_VALID_AFTER,
    C_RECV_DATETIME,
    C_LOCATE,
   
    //已废弃
    C_PROGRESS,
    C_RECVSEGMENT,
    C_STATUS,
    C_TOTAL_SEGMENT,
    C_TIMERANGE,
};
*/


struct InfoData
{
/*  The details which there are display on UI
    "Film ID:",  0
    "Content Name(Title):", 1
    "HasKdm:", 2
    "NeedKdm:", 3
    "Video Format:", 4
    "Content Size:", 5
    "Duration:", 6 
    "2D/3D:", 7
    "Issuer:", 8
    "Issue Date:",
    "PKL file:",
    "Not Valid Before:",
    "Not Valid After:",
    "Receive Date:",
    "Full Path:",
    "Location",
*/
    enum{
    C_ID = 0,
    C_NAME,
    C_HAS_KDM,
    C_NEED_KDM,
    C_FORMAT,
    C_FILM_LENGTH,
    C_PROGRAM_LENGTH,
    C_STEREO,
    C_ISSUER,
    C_ISSUEDATE,
    C_PKL_FILE,
    C_NOT_VALID_BEFORE,
    C_NOT_VALID_AFTER,
    C_RECV_DATETIME,
    C_LOCATE,

    //新增
    C_COUNTRY,
    C_CREATOR,
    C_TYPE,
    C_DURATION,

    C_TOTAL,   //add by lizulin 
    };
public:
    std::string  pData[C_TOTAL];
}; 




//UI Core
typedef struct _RequestPMAudioData_ 
{
    char pm_name[1024];                  //pm file path
    unsigned int SampleRate;            //PM的采样率 帧率
    unsigned int FrameRate;              
    unsigned int ChannelNum;          //
    unsigned int Begin;                     //
    unsigned int End;                        //
}RequestPMAudioData;


typedef struct _ReponsePMAudioData_ 
{
    RequestPMAudioData Req;
    char failInfo[256];              //
    unsigned int bufferSize;     //
//	float buf[BufferSize];          //float-1.0to1.0
}ReponsePMAudioData;



//即将要播放的PM或MXF媒体文件信息
enum{
    MediaFileType_PM,
    MediaFileType_MXF
};
struct MediaFileInfo
{
    MediaFileInfo()
    {
        for (int i=0;i<sizeof(*this);++i)  //clear buf without memset function
        {
            ((char*)this)[i]=0;
        }
    }
    bool isSuccess;                  //正确标记，否则以下结构无效
    int FileType;                       //文件类型，PM 母版文件或者MXF文件   暂时不填
    char name[1024];               //文件名   暂时不填
    char path[1024];                //路径      暂时不填
    unsigned int SampleRate;  //采样率
    unsigned int FrameRate;    //帧率
    unsigned int TotalFrame;   //总帧数  通过帧率可以求总时间
    void print()
    {
         printf("Curr Media File Info:\n isSuccess:%d path:%s \n TotalFrame:%d SRate:%u FRate:%u \n"
			 ,isSuccess,path,TotalFrame,SampleRate,FrameRate);
    }
};


struct logDate
{
    unsigned short year;
    unsigned char  month;
    unsigned char  day;
};

struct STime
{
    unsigned char hour;
    unsigned char minute;
    unsigned char seconds;
};

struct logRequest
{
    logDate dateAfter;
    logDate datebefore;
    int logType;
};




bool isArrayByName(const char* srcName);   //通过名称判断是否阵列


#pragma pack(pop)       //

#endif
