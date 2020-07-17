#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H
//整个沉浸式音频项目共用的数据结构，以及一些与具体项目相关的共同操作封装

//public struct share between with Core And UI by lizulin

#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include "definition.h"


//panner 插件坐标系问题待彻底确认
#define USE_DOLBY_AXIS   //if use Dolby Axis


#pragma pack(push,1)     //Align by 1 byte

typedef struct _FtpServerInfo
{
	char szFtpServerIP[32] ;
	unsigned short ftpPort;
	char szFtpUser[32];
	char szFtpPassword[32];
	char szFtpDcpDir[256];
}FtpServerInfo;

typedef struct _CopyProgress
{
	long long allFileSize;
	long long allFileCopiedSize;
	int       allFileCopiedPercent;
	long long currentFileSize;
	long long currentFileCopiedSize;
	int       currentFileCopiedPercent;
	int       CopyStatus;
}CopyProgress;


enum ESTATUS
{
	UNKNOW=0,
	UPDATE_CMD,
	DONE_CMD,
};


struct ShareMemoryDCAP_Other
{
	int  unpateFlag; 

	int doMountFTP;
	char MountCmd[1024];

	int doUnmountFTP;
	char UnmounCmd[1024];
};
#pragma pack(pop)       //



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
        const unsigned int ArraryNum=sizeof(SpktoCh)/sizeof(SpktoCh[0]);
        for (unsigned int n=0;n<ArraryNum;n++)
        {
            SpktoCh[n]=-1;   //-1表示默认映射无效
            ChtoSpk[n]=-1;
        }
    }
    int getChannelNum(unsigned int SpkNum)const   //查询某实际输出声道在cardinput结构中的声道号，-1表示无效
    {
        const unsigned int ArraryMax=sizeof(SpktoCh)/sizeof(SpktoCh[0]);
        if (SpkNum>=ArraryMax)  //256
        {
            return -1;
        }
        return SpktoCh[SpkNum];
    }

    int getSpeakerNum(unsigned int ChNum)const      //查询cardinput结构中的某声道最后实际输出到的声道号，-1表示无效
    {
        const unsigned int ArraryNum=sizeof(SpktoCh)/sizeof(SpktoCh[0]);
        if (ChNum>=ArraryNum)  //256
        {
            return -1;
        }
        return ChtoSpk[ChNum];
    }
    unsigned int getSpkNum()const {return SpkNum;}    //返回配置文件中的扬声器个数
    
    //返回一个声道数作为当前系统最大使用声道数
    //有时候某个影厅只有8个扬声器，却被路由到25-32，所以要取spk 与ch中的最大值
    unsigned int getSpkDoutMax()const 
    {
        return MaxDout+1; //+1是因为dout从0开始计数，转到最大扬声器数量需要+1；
    }
    void updateMaxDout()
    {
        MaxDout=0;
        for (unsigned int n=0;n<SpkNum;n++)
        {
            if (ChtoSpk[n]>MaxDout)
            {
                MaxDout=ChtoSpk[n];
            }
        }
        printf("Spk MaxDout:%d\n",MaxDout);
    }
    void printSpkMap()
    {
        //unsigned int SpkNum=this->speakerMap.size();  //扬声器个数
        printf("扬声器个数为:%d\n",SpkNum);
        printf("声卡cardinput声道号与实际输出映射关系如下:\n");
        printf("Ch是cardinput中的声道号，Spk实际要映射到的输出扬声器:\n");//       //为了显示从1开始 +1;
        for (unsigned int n=0;n<SpkNum;n++)
        {
            printf("Ch=%-3d spk=%-3d || ",n,ChtoSpk[n]);
            printf("Spk=%-3d Ch=%-3d\n",n,SpktoCh[n]);
        }
    }


    int SpktoCh[256];         //最大不可能用到256个扬声器，没有用到的赋值为-1，表示无效
    int ChtoSpk[256];
    unsigned int SpkNum; //扬声器数量
    unsigned int MaxDout;
};

SpkChMap* GetSpkChMap();



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//与panner操纵杆,monitor等相关网络协议结构移到此处 20191127


//#pragma pack(push,1)  //保存对齐状态
//#pragma pack(1)

//typedef enum
//{
//    CMD_PH_HEAD=0x74752485,           //自定义leonis保留字
//    CMD_BEDDATA_KEY=0x02,             //自定义leonis保留字
//    CMD_OBJECTDATA_KEY=0x05           //自定义leonis保留字
//}OBIA;


////OBIA通信协议结构体
//typedef struct _PH_             //HEADER 简写HD  Package Header 简写KH  pKH比pPH好看
//{
//    uint32_t Key;	       //Value is 0x74758524, it means the package is a meta-data command package.
//    uint32_t Length;	   //The length of total struct in bytes, including the key and length.
//    //Value variable Meta-data
//}PH;


//typedef struct _BEDDATA_
//{
//    uint32_t Key;	    //Value is 0x2.
//    uint32_t Length;	//The length of total struct in bytes, including the key and length.
//    uint32_t CH_id;	//It’s means audio track number, such as 1, 2, 10.
//    uint32_t Time;    //Time code, it is the index of frame.
//    uint32_t Routing_destination;	//See below(routing_destination table)
//}BEDDATA;


/*
Routing_destination table
VALUE	LOCATION
0	Left
1	Right
2	Center
3	LFE
4	Side Left
5	Side Right
6	Rear Left
7	Rear Right
8	Height  Left
9	Height  Right
*/
//typedef struct _OBJECTDATA_
//{
//    uint32_t Key;            //Value is 0x5.
//    uint32_t Length;      //The length of total struct in bytes, including the key and length.
//    uint32_t CH_id;	     //It’s object id number, such as 11,12 to 128.
//    uint32_t Time;         //Time code, it is the index of frame.
//    float X;                  //	Float(-1~1)	Left wall: -1 Right wall: 1
//    float Y;                  //	Float(-1~1)	Screen: -1 Back wall: 1
//    float Z;                  //	Float(-1~1)	Ceiling: 1 Floor: -1
//    float Extent;           //(non-negative)	The extent(size) of the object in the room, measured as radius from the object position. 0 indicating a point source.
//    float Gain;             //(non-negative)	This linear gain scales the level the audio is presented at, which is otherwise determined by the waveform level and renderer.
//    float Coherence;   //(0~1)	For objects with non-zero extent, coherence affects the coherence of sound elements arriving from different directions. 1 indicating full coherence.
//    float Snap;            //(non-negative)	Snap arranges for an object to jump to a speaker if one is nearby. 0 indicating no snap.
//}OBJECTDATA;


////与操纵杆JOYSTICK交互相关结构体
//typedef struct _KHJOY_        //一般命令三个字节
//{
//    unsigned char KeyHead;
//    unsigned char Key2;
//    unsigned char Key3;
//}KHJOY;
//
//typedef struct _KHJOYPOS_   //传递坐标值的命令五个字节
//{
//    KHJOY KHjoy;
//    unsigned char Key4;
//    unsigned char Key5;
//}KHJOYPOS;
//
//typedef struct _POS_    //保存操纵杆回传的坐标值结构体
//{
//    uint32_t CH_id;	 //It’s object id number, such as 11,12 to 128.
//    uint32_t Time;     //Time code, it is the index of frame.
//    float posX;
//    float posY;
//    float posZ;
//}POS;
//
//
//
////网络通讯协议报文基本结构
////嵌入以下所有报文前面,作为报文头 BASEprotocol
//typedef struct _KH_             //HEADER 简写HD  Package Header 简写KH  pKH比pPH好看
//{
//    uint8_t   pkgHead;            //preamble;//8位无符号整型，起始同步字段，恒为(0x55)
//    uint8_t   flag_pwd;           //加密标记 0	报文未加密 1报文采用硬件指纹加密 2	报文采用会唔密钥加密
//    uint32_t  rev;                //reserved保留字 
//    uint16_t  cmd;                //该报文的主命令字
//    uint16_t  cmd_Sub;            //该报文的辅助命令字 Sub_cmd
//    uint32_t  cmd_length;         //Payload_length;//报文载荷内容长度，(包含其内容本身+其CRC32校验值)
//    //uint8  Payload_data_byte;  
//}KH;

//#pragma pack(pop)       //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////





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
        init();
    }
    void init()
    {
        //为了随处例化提高效率 不需要构造函数
        x=y=z=0.0f;
        //必须复位，为了性能不在构造函数复位在其他使用处复位，否则可能坐标溢出
    }
    static void LimitRange(float& fpos)  //调试使用
    {
        if (fpos<=0.0)
        {
            //printf("Pos: Out of range: fpos=%f\n",fpos);
            fpos=0.0;
        }
        else if (fpos>1.0)
        {
            //printf("Pos: Out of range: fx=%f y=%f z=%f\n",x,y,z);
            //printf("Pos: Out of range: fpos=%f\n",fpos);
            fpos=1.0;
        }
    }
    bool checkOutRange() const
    {
        if(x>1.0f||x<0.0f||y>1.0f||y<0.0f||z>1.0f||z<0.0f)
        {
              printf("Pos: Out of range: x=%f y=%f z=%f\n",x,y,z);
              return true;
        }
        //LimitPosRange();
        return false;
    }

    void LimitRange() //平滑过程坐标值可能溢出，限制范围
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
    //ZoneGainPrefix ............................................... 2
    //ZoneGain[sb][n]................................................10
    //ObjectSpreadMode...........................................2
    //ObjectSpreadX[sb] ........................................ 12
    //ObjectSpreadY[sb] ........................................ 12
    //ObjectSpreadZ[sb] ........................................ 12
    //ObjectDecorCoefPrefix .................................. 2
    //ObjectDecorCoef[sb] ....................................... 8
    //实际使用数据类型，为了简化网络传输大小端,不使用位段，为了效率重排成员顺序、确定后不可修改 20190130

    //bool PanInfoExists;
    //char ObjectGainPrefix;
    float ObjectGain;
    //bool Snap;
    //bool SnapTolExists;
    float SnapTolerance;

    //char SpreadMode;
    float SpreadX;
    float SpreadY;
    float SpreadZ;
    //char DecorCoefPrefix;
    float DecorCoef;

    //bool WorkAsZone19;  //标准没有,自定义用在区分是否工作在zone19模式,否则为zone9复用以下结构。
    //bool ZoneControl;
    //char ZoneGainPrefix[19];
    //float ZoneGain[19];

    //为了效率与空间、1字节成员放到后面 共8个
    bool PanInfoExists;
    char ObjectGainPrefix;
    bool Snap;
    bool SnapTolExists;
    char SpreadMode;
    char DecorCoefPrefix;
    bool ZoneControl;
    //bool WorkAsZone19;          //标准没有,自定义用在区分是否工作在zone19模式,否则为zone9复用以下结构。


#if 1  //相当于原来WorkAsZone19,不影响
    //////////////////////////////////////////////////////////////////////////
    // 增加 ZoneDefinition19支持，此内容从ObjectZoneDefinition19中读出 [11/16/2018 jaontolt]
    // 如果存在ZoneDefinition19，上面的ZoneGain无效，用此ZoneGain19替代
    char ZoneMode; // 用于标记Zone19中存的是哪种Zone数据，0 - 为标准Zone9模式，1为Zone19模式 [12/12/2018 jaontolt]
#endif  

    float ZoneGain[19];             //可能变化,放到最后
    char ZoneGainPrefix[19];    

    //////////////////////////////////////////////////////////////////////////
    //标准没有，可能会用到的
    //size可能对应Spread待整理--待废弃
    float size;      //
    //int recv[4];  //预留4个未来扩充

    PositionInfoTmp()
    {
        //memset(this,0,sizeof(*this));            //
        //为了随处例化提高效率 不需要构造函数
        //char* pstart=(char*)this;            
        //for (int i=0;i<sizeof(*this);++i)        //clear buf without memset function
        //{
        // pstart[i]=0;
        //}
    }
    void print() const
    {
        printf("x:%f y:%f z:%f\n",x,y,z);
    }
};


//仅仅是为了保证 单个block对象信息大小定长而引入的中间类
//PositionInfoTmp不大于160字节
class PositionInfo:public PositionInfoTmp
{
    char Resv[160-sizeof(PositionInfoTmp)];   
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
        init();
    }
    void init()
    {
        //为了随处例化提高效率 不需要构造函数
        //char* pstart=(char*)this;            
        //for (int i=0;i<sizeof(*this);++i)  //clear buf without memset function
        //{
        //	pstart[i]=0;
        //}
        for(unsigned int n=0;n<NumPanSubBlocksMax;n++)
        {
            SubPos[n].init();
        }
        id=-1;  //-1default value   默认无效
    }

    unsigned int getSpeedLevel() const;   //根据子帧首位坐标求运动速度等级，决定是否进一步填充平滑子帧
    unsigned int getSmoothSubBlocks(unsigned int FrameRate) const;
    //填充子帧,注意平滑子帧是坐标值可能会溢出
    void FillSmoothBlocks(PositionInfo SmoothSubPos[80],unsigned int NumSubSmoothBlocks);
    void checkRange() const;    //检查对象数据是否越界，调试使用
    void limitRange();      //抹平越界，调试使用
    void print() const
    {
        ::printf("Obj: FrameNum:%u id:%d x:%f y:%f z:%f\n",timeInFrameNum,id,SubPos[0].x,SubPos[0].y,SubPos[0].z);
    }
};


//仅仅是为了保证 整个对象信息大小定长而引入的中间类
//ObjMetaDataTmp不大于1400字节
class ObjMetaData:public ObjMetaDataTmp
{
public:
    ObjMetaData()
    {
        //printf("sizeof(PositionInfoTmp)=%d sizeof(ObjMetaDataTmp)=%d\n",sizeof(PositionInfoTmp),sizeof(ObjMetaDataTmp));
        //printf("sizeof(PositionInfo)=%d sizeof(ObjMetaData)=%d\n",sizeof(PositionInfo),sizeof(ObjMetaData));
    }
    char Resv[1400-sizeof(ObjMetaDataTmp)];   
};






//BED相关信息,由标准文档决定,不可任意删减或更改成员变量顺序,
//否则,造成已经录制的PM文件可能无法播放  by lizulin 20190515
//


//参考IABitstream文档的BedDefinition,BedRemapDefinitionInfo,基本是一一映射的。保存其解码结果
struct BedDataInfo
{
    enum {Ch_MAX=16};    //ChannelCount最大值等于16？
    unsigned int MetaID;
    unsigned int ConditionalBed;
    unsigned int BedUseCase;
    unsigned int ChannelCount;
    unsigned int ChannelID[Ch_MAX];
    unsigned int AudioDataID[Ch_MAX];
    unsigned int ChannelGainPrefix[Ch_MAX];
    //unsigned int ChannelGain[Ch_MAX];
    float ChannelGain[Ch_MAX];
    unsigned int ChannelDecorInfoExists[Ch_MAX];
    unsigned int Reserved[Ch_MAX];
    unsigned int ChannelDecorCoefPrefix[Ch_MAX];
    unsigned int ChannelDecorCoef[Ch_MAX];
    unsigned int Reserved2[Ch_MAX];
    unsigned int Reserved3;  //(set to 0x180) 	10bit
    BedDataInfo()
    {
            setDefaultInfo();
    }
    void setDefaultInfo()  //恢复默认配置
    {
        //构造Bed映射关系，基本是固定的
        unsigned int AudioDataIDIndex=100;//仿照Dolby用100作为bed声道的起始值
        ChannelID[0]=C_L;      AudioDataID[0]=AudioDataIDIndex++;
        ChannelID[1]=C_R;     AudioDataID[1]=AudioDataIDIndex++;
        ChannelID[2]=C_C,     AudioDataID[2]=AudioDataIDIndex++;
        ChannelID[3]=C_LFE;  AudioDataID[3]=AudioDataIDIndex++;
        ChannelID[4]=C_LS;   AudioDataID[4]=AudioDataIDIndex++;
        ChannelID[5]=C_RS;   AudioDataID[5]=AudioDataIDIndex++;
        ChannelID[6]=C_LRS,  AudioDataID[6]=AudioDataIDIndex++;
        ChannelID[7]=C_RRS;  AudioDataID[7]=AudioDataIDIndex++;
        ChannelID[8]=C_LTS;  AudioDataID[8]=AudioDataIDIndex++;
        ChannelID[9]=C_RTS;  AudioDataID[9]=AudioDataIDIndex++;

        //ChannelID[10]=C_RRS,AudioDataID[10]=AudioDataIDIndex++;
        //ChannelID[11]=C_LTS;AudioDataID[11]=AudioDataIDIndex++;
        //ChannelID[12]=C_RTS;AudioDataID[12]=AudioDataIDIndex++;
        //ChannelID[13]=C_LFE; AudioDataID[13]=AudioDataIDIndex++;

        ConditionalBed=0x1;
        BedUseCase=0x20;
        ChannelCount=10; //仿照Dolby设置为10 
        for (unsigned int n=0;n<Ch_MAX;++n)
        {
            ChannelGainPrefix[n]=0x0;ChannelGain[n]=0;ChannelDecorInfoExists[n]=0x0;Reserved[n]=0;
            ChannelDecorCoefPrefix[n]=0x0;  ChannelDecorCoef[n]=0;
        }
        Reserved3=0X180;     //(set to 0x180) 	10bit
    }
    void printInfo()
    {
        printf("BedDefinitionInfo::printInfo()\n");

        printf("ChannelCount=%d \n",ChannelCount);
        for (unsigned int n=0;n<ChannelCount;++n)
        {
            printf("ChannelID[%d]=%d AudioDataID[%d]=%d\n",n,ChannelID[n],n,AudioDataID[n]);
        }
    }
};

struct BedRemapDataInfo
{
    enum {
        subBlk_Max=16,
        Chan_Max=16,
    };
    unsigned int MetaID;
    unsigned int RemapUseCase	;
    unsigned int SourceChannels;
    unsigned int DestinationChannels	;
    unsigned int RemapInfoExists[subBlk_Max];
    unsigned int DestinationChannelID[subBlk_Max][Chan_Max];
    unsigned int RemapGainPrefix[subBlk_Max][Chan_Max];
    //unsigned int RemapGain[subBlk_Max][Chan_Max][Chan_Max];
    float RemapGain[subBlk_Max][Chan_Max][Chan_Max];
    BedRemapDataInfo()
    {
            setDefaultInfo();
    }
    void setDefaultInfo()             //恢复默认配置
    {
        MetaID=0;
        RemapUseCase=0	;
        SourceChannels=0;
        DestinationChannels	=0;
        for (int subBlk=0;subBlk<subBlk_Max;++subBlk)
        {
            RemapInfoExists[subBlk]=0;
            for (int oChan=0;oChan<Chan_Max;++oChan)
            {
                DestinationChannelID[subBlk_Max][oChan]=0;
                for (int iChan=0;iChan<Chan_Max;++iChan)
                {
                    RemapGain[subBlk][oChan][iChan]=0;
                }
            }
        }
    }
};

//为了扩展性封装一个与ObjMetaData平行概念的BedMetaData,
//而不是把BedMetaData放入ObjMetaData结构里面
class BedMetaData
{
public:
    BedDataInfo bedInfo;
    BedRemapDataInfo bedRemapInfo;
    void init()
    {
        bedInfo.setDefaultInfo();
        bedRemapInfo.setDefaultInfo();
    }
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
    void print() const
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
    std::string dhcp;
};



//NOTE:为了效率与简洁，本结构设计为POD结构,暂不加std::string 之类成员
struct  Speaker
{
public:
    char name[32];                //扬声器名称
    char spkTypeName[32];   //扬声器类型名称    "Screen"   "Surround" "Subwoofer"
    int   channel;            //扬声器实际路由输出号对应SpkEndpoint的dout
    int id;                       //内部编号意义用处不大暂留
    int dout;                   //对应SpkEndpoint的dout

    //扬声器坐标,实际尺寸,单位是米  //>0小于影厅尺寸,超出部分被截断
    float x;                   
    float y;
    float z;
    float delay;              //扬声器延时值,单位是ms>=0
    float sound_loud;

    int special;                //特殊扬声器标记为1表示低音？ 待续
    char comment[64];   //备注
    Speaker()
    {
        x=y=z=0;
        delay=0;
        channel=0;
        id=0;
        dout=0;

        memset(name,0,sizeof(name));
        memset(spkTypeName,0,sizeof(spkTypeName));
        memset(comment,0,sizeof(comment));
    }
};


//use in MPU Core Monitor
typedef struct  _DIM_
{
    float x;
    float y;
    float z;
}DIM;



//描述影厅长宽高与,扬声器列表坐标等详细信息,方便各处使用
struct  RoomSpkInfo
{
    //影厅长宽高,实际尺寸,单位米
    float roomLength;
    float roomWidth;
    float roomHeight;
    std::vector<Speaker> ListSpkInfo;    //各个扬声器名称与实际布局信息
    RoomSpkInfo()
    {
        roomLength=0;
        roomWidth=0;
        roomHeight=0;
    }
};
//内部结构比较复杂,重载网络流收发函数
size_t NeedBytesToStream(const RoomSpkInfo& Info);
size_t StreamPut(char*& pos,size_t& AccLen,const RoomSpkInfo& Info);
size_t StreamGet(char*& pos,size_t& AccLen,RoomSpkInfo& Info);




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
        memset(path_src,0,sizeof(path_src));
        memset(path_dst,0,sizeof(path_dst));
    }
    void print()const
    {
        printf("src path:%s\n",path_src);
        printf("dst path:%s\n",path_dst);
    }
}copy_path;



//传递包含变长字符串结构的新方法--没有必要定义SLV结构和ID然后收发双方每一个字符串依次拼接--by lizulin 20171216
//废弃 直接用StreamPut/StreamGet可以编解string数组的流 by lizulin 201910102


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
    std::string mStr[C_TOTAL];              //字符串原始结构体或者还原后的结构体
    void print()const
    {
        printf("KDMInfo:ID:%s\nName:%s\nPath:%s\nCPLID:%s\nValidDate From:%s to %s\n"
            ,mStr[C_ID].c_str(),mStr[C_NAME].c_str(),mStr[C_LOCATE].c_str(),mStr[C_CPLID].c_str(),mStr[C_NOTVALID_BEFORE].c_str(),mStr[C_NOTVALID_AFTER].c_str());
    }
};
//内部结构比较复杂,重载网络流收发函数
size_t NeedBytesToStream(const KDMInfo& kInfo);
size_t StreamPut(char*& pos,size_t& AccLen,const KDMInfo& kInfo);
size_t StreamGet(char*& pos,size_t& AccLen,KDMInfo& kInfo);


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

//注意若要修改此结构,需要同步修改StreamPut/StreamGet里的实现
struct AtomsData
{
    std::string strAtomsMxf;         //Mxf文件路径
    uint64_t MxfCheckSum;           //Mxf文件校验码,用来刷新时判断文件是否变化,优化预处理速度,不变化不重复预处理
    std::string strAtomsUuid;        //Mxf的uuid
    std::string strUuid1; // 5.1MXF uuid of DCP A [12/8/2017 jaontolt]  //一般情况uuid1,uuid2只有一个有效,另一个为空
    std::string strUuid2; // 5.1MXF uuid of DCP B [12/8/2017 jaontolt]  //uuid2是针对中国巨幕的特殊格式
    std::string strHash;

    std::string strKeyId;                          // KeyId  KDM ID
    std::string strKeyFilePath;                 //KDM文件路径
    std::string strKeyNotValidBefore;      //KDM有效期开始
    std::string strKeyNotValidAfter;         //KDM有效期结束

    char binAssetID[16];            //二进制形式的strAtomsUuid
    char binAssetHash[20];       //二进制形式的strHash


    //是否已经从KDM文件找到密钥并填充如下binAesKey,仅仅是找到,不一定正确,解码器打开并解码出加密帧才算
    bool bFindAesKey;                   
    unsigned char binAesKey[16];  //二进制形式的kdm密钥

    unsigned int TotalFrames;        //mxf文件实际帧数,预留
    unsigned int EntryPoint;           //mxf有影片起始位置,由CPL文件<EntryPoint>字段描述,大多数情况为0,
    unsigned int nDuration;           //影片时长,以帧为单位,由CPL文件<Duration>字段描述,一般小于mxf实际总帧数。
    unsigned int FrameRate;          //影片帧率,可以从CPL文件文件解析得到。或者由Mxf解码器得到。
	std::string strContentName;    //电影名称

    AtomsData()
    {
        //add by lizulin 20180116
        for (unsigned int i=0;i<sizeof(binAssetID);i++)
        {
            binAssetID[i]=0;
        }
        for (unsigned int i=0;i<sizeof(binAssetHash);i++)
        {
            binAssetHash[i]=0;
        }

        bFindAesKey=false;
        for (unsigned int i=0;i<sizeof(binAesKey);i++)
        {
            binAesKey[i]=0;
        }
        MxfCheckSum=0;
        TotalFrames=0;
        EntryPoint=0;
        nDuration=0;
        FrameRate=24;
    }
    ~AtomsData(){}
    void print() const;
};

//ATMOST_DATA
//内部结构比较复杂,重载网络流收发函数
size_t NeedBytesToStream(const AtomsData& ad);
size_t StreamPut(char*& pos,size_t& AccLen,const AtomsData& ad);
size_t StreamGet(char*& pos,size_t& AccLen,AtomsData& ad);


struct DcpInfo
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
    std::string toDetail() const  //显示详细信息
    {
        std::string ret;
        ret+="ID: ";
        ret+=mStr[C_ID]+"\n";
        ret+="Name: ";
        ret+=mStr[C_NAME]+"\n";
        ret+="Format: ";
        ret+=mStr[C_FORMAT]+"    ";
        ret+="Stereo: ";
        ret+=mStr[C_STEREO]+"\n";
        ret+="Haskdm: ";
        ret+=mStr[C_HAS_KDM]+"    ";
        ret+="Needkdm: ";
        ret+=mStr[C_NEED_KDM]+"\n";
        ret+="FilmLength:";
        ret+=mStr[C_FILM_LENGTH]+"    ";
        ret+="ProgramLength: ";
        ret+=mStr[C_PROGRAM_LENGTH]+"\n";
        ret+="Issuer: ";
        ret+=mStr[C_ISSUER]+"    ";
        ret+="Issuedate:";
        ret+=mStr[C_ISSUEDATE]+"\n";
        ret+="PklFile: ";
        ret+=mStr[C_PKL_FILE]+"\n";
        ret+="NotValidBefore: ";
        ret+=mStr[C_NOT_VALID_BEFORE]+"    ";
        ret+="NotValidAfter: ";
        ret+=mStr[C_NOT_VALID_AFTER]+"\n";
        ret+="RecvDatetime: ";
        ret+=mStr[C_RECV_DATETIME]+"\n";
        ret+="Locate: ";
        ret+=mStr[C_LOCATE]+"\n";
        return ret;
    }
    enum{
    C_ID = 0,
    C_NAME,
    C_TITLE,  //Title 和Name好像是一个概念
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
    C_LOCATE,   //rootpath

    //新增
    C_COUNTRY,
    C_CREATOR,
    C_TYPE,
    C_DURATION,
    C_TOTAL_FILE_SIZE,  //add by lizulin 从PKL文件解析出DCP包各种文件总大小

    //为了支持ST43014/17协议新增   std::string sCplXmlText; std::string sKmdXmlText; 
    //xml格式文件内容会比较大,在UI端没有用,为了效率,UI收到后置为空即可。待续
    C_CPL_XML_TEXT,           //DCP包文件xml格式的CPL文件内容本身。
    C_CPL_ID,                       //DCP包文件xml格式的CPL文件uuid

    C_KDM_XML_TEXT,         //DCP包对应的本机KDM文件,xml格式的KDM文件内容本身。
    C_KDM_ID,                      //DCP包对应的本机KDM文件,xml格式的KDM文件uuid  实现待续

    C_TOTAL,   //add by lizulin 
    };
public:

    void print() const;
    std::string  mStr[C_TOTAL];
    //为了简化播放流程,把当前dcp包的每个卷文件路径,总帧数,uuid与kdm信息放入，放入此结构
    std::vector<AtomsData> ListSubReelInfo;


    //当前DCP包路径,同目录下所有文件名称与大小,调试定位与详情显示用


    DcpInfo(){}
    ~DcpInfo(){}
}; 

//内部结构比较复杂,重载网络流收发相关函数
size_t NeedBytesToStream(const DcpInfo& di);
size_t StreamPut(char*& pos,size_t& AccLen,const DcpInfo& di);
size_t StreamGet(char*& pos,size_t& AccLen,DcpInfo& di);



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



//当前放映输入片源格式类型
enum EInputMediaType
{
    EIMType_Default=0,       //未设置
    EIMType_HoloSound=1,  //沉浸式音频
    EIMType_Ch5_1=2,          //5.1/7.1/9.1等
    EIMType_Ch7_1=3,
    EIMType_Ch9_1=4,
    EIMType_MIC=5      //NoSync？
};


//信号发生器生成的信号类型
enum EWaveType
{
    Wave_PinkNoise,              //粉红噪声
    Wave_DigitalNumVoice,    //语音报数001-002
    Wave_TestMusic1,            //测试音乐1,比较短,用于快速遍历
    Wave_TestMusic2,            //测试音乐2
    //Wave_Sine,                       //频率可变正弦波,暂不实现

    //以下暂暂时未实现
    Wave_SineWave100Hz,  //100Hz正弦波(100Hz sine)
    Wave_SineWave1KHz,    //K1Hz正弦波(1Hz sine)
    Wave_SineWave10KHz,  //10KHz正弦波(10Hz sine)
    Wave_thump,                //低音噪声(thump)
    Wave_sweep                 //扫描信号音(Sweep)
};



//即将要播放的PM或MXF媒体文件信息
enum{
    MediaFileType_Unknow,
    MediaFileType_PM,
    MediaFileType_MXF
};


//描述PM或Mxf格式媒体文件的结构,主要用在与播放控制相关的,新增成员要同步到StreamPut/StreamGet函数中
//为了便于使用,封装在一起
struct MediaFileInfo
{
    MediaFileInfo()
    {
        reInit();
    }
    void reInit()
    {
        //for (int i=0;i<sizeof(*this);++i)  //clear buf without memset function
        //{
        //    ((char*)this)[i]=0;
        //}
        isSuccess=false;
        FileType=MediaFileType_Unknow;
        SampleRate=48000;
        FrameRate=24;
        TotalFrame=0;
        NeedKDM=false;
        memset(mxfbinAesKey,0,sizeof(mxfbinAesKey));
        sName.clear();
        sDriver.clear();
        sProjectName.clear();
        sDir.clear();
        sFullPath.clear();
    }

    bool isSuccess;                               //正确标记，否则以下结构无效
    int FileType;                                    //文件类型，PM 母版文件或者MXF文件
    unsigned int SampleRate;               //采样率
    unsigned int FrameRate;                 //帧率
    unsigned int TotalFrame;                //总帧数  可以通过帧率可以求总时间
    bool NeedKDM;                             //若媒体为MXF格式的话,是否需要KDM--暂时无用
    unsigned char mxfbinAesKey[16];  //KMD密钥二进制

    std::string sName;                        //媒体文件名包含,pm或.mxf后缀
    std::string sDriver;                        //所在驱动器
    std::string sProjectName;             //工程名,在mpu中用到
    std::string sDir;                             //除了文件名外的路径
    std::string sFullPath;                     //媒体文件完整路径
    std::string strMediaType() const
    {
        if (FileType==MediaFileType_MXF)
        {
            return "MXF";
        }
        if (FileType==MediaFileType_PM)
        {
            return "PM";
        }
        return "";
    }
    void print() const
    {
         printf("Current Media File Info:\nisSuccess:%d path:%s\nTotalFrame:%u SRate:%u FRate:%u\n"
             ,isSuccess,sFullPath.c_str(),TotalFrame,SampleRate,FrameRate);
    }
};
//内部结构比较复杂,重载网络流收发相关函数
size_t NeedBytesToStream(const MediaFileInfo& mediaInfo);
size_t StreamPut(char*& pos,size_t& AccLen,const MediaFileInfo& mediaInfo);
size_t StreamGet(char*& pos,size_t& AccLen,MediaFileInfo& mediaInfo);

typedef struct _SETTING_
{
    float globalGain;
    float globalDelay;
    float ltcChase;
    int dcssync;
    int reel1beginmark;
    int reel1ffoa;
    char asioFile[512];
    char keyFile[512];
    char ipObia[512];
    char ipCinema[512];
    char pmDirectory[512];
    _SETTING_()
    {
        memset(asioFile,0,sizeof(asioFile));
        memset(keyFile,0,sizeof(keyFile));
        memset(ipObia,0,sizeof(ipObia));
        memset(ipCinema,0,sizeof(ipCinema));
        memset(pmDirectory,0,sizeof(pmDirectory));
    }
}Setting;


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


//主扬声器分频系数
struct _SPK_PARAM
{
    int co_cnt;
    int in;
    int out_L, out_M, out_H;
    float Lpf, Hpf;
} ;

struct MainSpeakerCrossoverCoef
{
    bool EnableCrossover;
    std::vector<_SPK_PARAM> co_speaker;
    bool operator==(const MainSpeakerCrossoverCoef& rVal)
    {
        if(this->EnableCrossover != rVal.EnableCrossover)
            return false;
        if(this->co_speaker.size() != rVal.co_speaker.size())
            return false;
        for(int i = 0; this->co_speaker.size(); i++)
        {
            if(this->co_speaker[i].co_cnt != rVal.co_speaker[i].co_cnt
                || this->co_speaker[i].Hpf != rVal.co_speaker[i].Hpf
                || this->co_speaker[i].in != rVal.co_speaker[i].in
                || this->co_speaker[i].Lpf != rVal.co_speaker[i].Lpf
                || this->co_speaker[i].out_L != rVal.co_speaker[i].out_L
                || this->co_speaker[i].out_M != rVal.co_speaker[i].out_M
                || this->co_speaker[i].out_H != rVal.co_speaker[i].out_H)
                return false;
        }
        return true;
    }
#if 0
    bool operator==(const MainSpeakerCrossoverCoef& rVal)
    {
        if(this->EnableCrossover!=rVal.EnableCrossover)
        {
            return false;
        }
        if(this->LeftCnt!=rVal.LeftCnt&&this->RightCnt!=rVal.RightCnt
            &&this->CenterCnt!=rVal.CenterCnt&&this->LfeCnt!=rVal.LfeCnt)
        {
            return false;
        }
        if(this->LeftLpf!=rVal.LfeLpf&&this->RightLpf!=rVal.RightLpf
            &&this->CenterLpf!=rVal.CenterLpf&&this->LfeLpf!=rVal.LfeLpf)
        {
            return false;
        }
        if(this->LeftHpf!=rVal.LfeHpf&&this->RightHpf!=rVal.RightHpf
            &&this->CenterHpf!=rVal.CenterHpf&&this->LfeHpf!=rVal.LfeHpf)
        {
            return false;
        }
        return true;
    }
#endif
    bool operator!=(const MainSpeakerCrossoverCoef& rVal)
    {
        return !operator==(rVal);
    }
};

size_t NeedBytesToStream(const MainSpeakerCrossoverCoef& mccc);
size_t StreamPut(char*& pos,size_t& AccLen,const MainSpeakerCrossoverCoef& mccc);
size_t StreamGet(char*& pos,size_t& AccLen,MainSpeakerCrossoverCoef& mccc);



bool isArrayByName(const char* srcName);   //通过名称判断是否阵列



//当前正在播放DCP 电影名称,kdm有效期等信息
//注意若要修改此结构,需要同步修改StreamPut/StreamGet里的实现
struct ProgramInfo
{
    std::string sCurrentPlayingPromgramName;
    std::string sCurrKeyFilePath;
    std::string sCurrKeyNotValidBefore;
    std::string sCurrKeyNotValidAfter;
};

//内部结构比较复杂,重载网络流收发相关函数
size_t NeedBytesToStream(const ProgramInfo& Info);
size_t StreamPut(char*& pos,size_t& AccLen,const ProgramInfo& Info);
size_t StreamGet(char*& pos,size_t& AccLen,ProgramInfo& Info);





////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//PM编码分卷策略相关,在MPU-Core/UI都用到，放到此处，

//长久的未来帧号不可能超过unsigned int 的表示范围，所有帧号由long long改unsigned int 约40亿够录制5年的 by lizulin
typedef struct _REEL_INFO 
{
    unsigned int begin;
    unsigned int end;
} REEL_INFO;

typedef struct _PM_INFO
{
    std::string pmFullPath;
    //////////////////////////////////////////////////////////////////////////
    // 修改支持同步对齐 [8/5/2017 killerlife]
    // 增加SEEK POS 
    // 单位为24位的采样点，1表示1个Int24数据
    // >0 时表示声音比画面快，需要延时
    // <0 时表示声音比画面慢，需要提前
    long long seekPos;
    //////////////////////////////////////////////////////////////////////////
    std::vector<REEL_INFO> reelInfo;
} PM_INFO;

typedef struct _PM_SPLIT_POLICY_
{
    std::vector<PM_INFO> pmInfo;
}PM_SPLIT_POLICY;


//内部结构比较复杂,重载网络流收发相关函数
size_t NeedBytesToStream(const PM_INFO& Info);
size_t StreamPut(char*& pos,size_t& AccLen,const PM_INFO& Info);
size_t StreamGet(char*& pos,size_t& AccLen,PM_INFO& Info);

//内部结构比较复杂,重载网络流收发相关函数
size_t NeedBytesToStream(const PM_SPLIT_POLICY& Info);
size_t StreamPut(char*& pos,size_t& AccLen,const PM_SPLIT_POLICY& Info);
size_t StreamGet(char*& pos,size_t& AccLen,PM_SPLIT_POLICY& Info);

////////////////////////////////////////////////////////////////////////////////


#pragma pack(pop)       //








////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//DCAP-EAH嵌入式均衡算法放到外部arm核后,均衡系数交互控制相关

#define EQ_CHANNELS	 (128)	
#define EQ_BANDS (31)  //31个band

//#pragma pack(push,1)     //Align by 1 byte

//单个扬声器的均衡系数
struct  PerChEQCoeff
{
    float bandgain[EQ_BANDS];     //各个频段增益 db为单位
    float outgain;                          //总增益           db为单位
    float fBass[4], fTreble[4];         //广州SB要求启用并扩展到4个Band

    PerChEQCoeff()
    {
        outgain=0;
        for (int n=0;n<EQ_BANDS;n++)
        {
            this->bandgain[n]=0;
        }
        for (int n = 0; n < 4; n++)
        {
            fBass[n] = fTreble[n] = 0;
        }
    }

    bool operator ==(const PerChEQCoeff& R) const
    {
        if (this->outgain!=R.outgain)
        {
            return false;
        }
        for (int n=0;n<EQ_BANDS;n++)
        {
            if (this->bandgain[n]!=R.bandgain[n])
            {
                return false;
            }
        }
        for(int n = 0; n < 4; n++)
        {
            if(this->fBass[n] != R.fBass[n])
                return false;
            if(this->fTreble[n] != R.fTreble[n])
                return false;
        }
        return true;
    }
    bool operator !=(const PerChEQCoeff& R) const
    {
        return !this->operator==(R);
    }
    void print()const
    {
        for (int nband=0;nband<EQ_BANDS;nband++)
        {
            printf("%.1f ",bandgain[nband]);
        }
        for(int n = 0; n < 4; n++)
            printf("[%.1f %.1f] ", fBass[n], fTreble[n]);
        printf("level:%.1f\n",outgain);
    }
};

inline void printChanged(const PerChEQCoeff& newCoef,const PerChEQCoeff& oldCoef)
{
    for (int nband=0;nband<EQ_BANDS;nband++)
    {
        if (newCoef.bandgain[nband]!=oldCoef.bandgain[nband])
        {
            printf("band[%d] gain:%.2f ",nband,newCoef.bandgain[nband]);
        }
    }
    for(int n = 0; n < 4; n++)
    {
        if(newCoef.fBass[n] != oldCoef.fBass[n])
            printf("bass[%d] gain:%.1f", n, newCoef.fBass[n]);
        if(newCoef.fTreble[n] != oldCoef.fTreble[n])
            printf("treble[%d] gain:%.1f", n, newCoef.fTreble[n]);
    }
    if (newCoef.outgain!=oldCoef.outgain)
    {
        printf("level:%.2f",newCoef.outgain);
    }
}

//所有扬声器16个通道的均衡系数
struct  EQCoeffAll
{
    bool bEQEnabled;   //均衡使能
    bool bBTEnabled;
    enum 
    {
        NUM_CH=EQ_CHANNELS,
        NUM_BAND=EQ_BANDS,
    };

    PerChEQCoeff chCoeff[NUM_CH];
    bool operator ==(const EQCoeffAll& R)
    {
        for (int n=0;n<NUM_CH;n++)
        {
            if (this->chCoeff[n]!=R.chCoeff[n])
            {
                return false;
            }
        }
        return true;
    }
    bool operator !=(const EQCoeffAll& R)
    {
        return !this->operator==(R);
    }
    void print()const
    {
        printf("EqCoefAll:isEqEnabled:%d\n",bEQEnabled);
        for (int ch=0;ch<NUM_CH;ch++)
        {
            printf("ch[%d]",ch);
            for (int nband=0;nband<NUM_BAND;nband++)
            {
                printf("%.1f ",chCoeff[ch].bandgain[nband]);
            }
            printf(" BassTreble: ");
            for(int n = 0; n < 4; n++)
                printf("[%.1f %.1f] ", chCoeff[ch].fBass[n], chCoeff[ch].fTreble[n]);
            printf("level:%.1f \n",chCoeff[ch].outgain);
        }
    }
};




class  EQCoeffAllMgr
{
public:
    EQCoeffAllMgr();
    ~EQCoeffAllMgr();
    //////////////////////////////////////////////////////////////////////
    //均衡系数配置相关:
public:
    void setEQEnabled(bool flag)
    {
        eqCoeffAll.bEQEnabled=flag;
    }
    bool getEQEnabled()const{return eqCoeffAll.bEQEnabled;}
    void setEQCoeffAll(const EQCoeffAll& coeff){eqCoeffAll=coeff;}
    EQCoeffAll  getEQCoeffAll()const{return eqCoeffAll;}
    void setEQCoeff(unsigned int Ch,const PerChEQCoeff& perChCoeff)
    {
        if(Ch>=EQCoeffAll::NUM_CH)
        //if(Ch>EQCoeffAll::NUM_CH)
        {
            return;
        }
        //PRINT_FUN_NAME
        //仅打印设置时发生变化的声道
        //Do not printf this, arm printf this to serial port, too slow
        //printf("setEQCoeff() Ch:%d ",Ch);
        //printChanged(perChCoeff,eqCoeffAll.chCoeff[Ch]);
        //printf("\n");
        eqCoeffAll.chCoeff[Ch]=perChCoeff;
    }
    PerChEQCoeff getEQCoeff(unsigned int Ch)
    {
        if (Ch>=EQCoeffAll::NUM_CH)
        //if(Ch>EQCoeffAll::NUM_CH)
        {
            return PerChEQCoeff();
        }
        return eqCoeffAll.chCoeff[Ch];
    }
    bool saveConf()
    {
    }
private:
    EQCoeffAll eqCoeffAll;
};
EQCoeffAllMgr* GetEQCoeffAllMgr();




#endif

