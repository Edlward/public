#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H
//Core与UI共用的结构体，宏开关等
#include <string>
#include <vector>


#define USE_DOLBY_AXIS   //是否兼容dolby坐标系开关





#pragma pack(push,1)     //设置一个字节对齐  



typedef struct _Position_
{
	float x;
	float y;
	float z;
	_Position_()
	{
		x=0.0f;
		y=0.0f;
		z=0.0f;
	}
}Position;


//一个对象某时刻的坐标等其他信息
class PositionInfo:public Position
{
public:
	float size;   //未来可能会支持size属性 Bitstream文档没有size属性   	
	//根据IABitstream_SMPTE_v3.doc文档  未来可能会支持以下更多属性
	bool PanInfoExists;
	bool GainPrefix;
	float Gain;
	bool Snap;
	bool SnapTolExists;
	float SnapTolerance;
	bool ZoneControl;
	int ZoneGainPrefix;
	float ZoneGain;
	int SpreadMode;
	float SpreadX;
	float SpreadY;
	float SpreadZ;
	int DecorCoefPrefix;
	float DecorCoef;
	//float extent;   //其实就是size 可删
	//float coherence;
	//float snap;
	//float zoneGain[19];       //暂时不明确意义待删除
	PositionInfo()
	{
		//memset(this,0,sizeof(*this));   //本结构，包括基类清零
		char* pstart=(char*)this;            
		for (int i=0;i<sizeof(*this);++i)  //本文件不想引入任何头文件，手动实现memset功能
		{
			pstart[i]=0;
		}
	}
};



class ObjMetaData
{
public:
	int id;
	unsigned int timecode;   //改unsigned int
	//new   添加子帧坐标支持
	enum
	{
		NumPanSubBlocksMax=8             //根据IABitstream_SMPTE_v3.doc文档 子块数量NumPanSubBlocks最大为8,
	};
	unsigned int NumSubBlocks;                //当前有效的子块数量,取1到NumPanSubBlocksMax,
	PositionInfo SubPos[NumPanSubBlocksMax];   //   
	ObjMetaData()
	{
		id=-1;      //-1 表示无效ID，默认无效
		timecode = 0;
	}
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

	//以下合并自speakerConfig
	//std::string name;
	//std::string comment;
	int special;
	char comment[64];
}Speaker;



//【room】为MPU Core向Monitor传递的影厅尺寸信息，数据结构定义如下：
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



//与声道定义的SL重命名了，改名为SLV
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

	CONTENT_PATH_SRC,   //源路址
	CONTENT_PATH_DST,   //目路址
};



//【SpeakerList】SpeakerList为MPU Core向Monitor传送的n个音箱（包含10个BED）的XYZ坐标、SoundLoud、channel及音箱名字定义，数据结构如下。
//10个BED的音箱名字定义分别为L，R，C，LFE，Lss，Rss，Lsr，Rsr，Lts，Rts。

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



struct InfoData
{
public:
	std::string  pData[16];
/*  The details which there are display on UI
	"Film ID:",  0
	"Content Name:", 1
	"Progress:", 2
	"Receive Status:", 3
	"Video Format:", 4
	"Content Size:", 5
	"Duration:", 6 
	"2D/3D:", 7
	"Issuer:", 8
	"Issue Date:",
	"Time Range:",
	"PKL file:",
	"Issue Area:",
	"Receive Date:",
	"Full Path:",
	"Creator",

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
*/ 
}; 



//CMD_GET_PM_AUDIO_DATA,              //MPU-UI向Core请求pm母版的一段音频数据
//CMD_GET_PM_AUDIO_DATA_RESP,     //Core返回音频数据


//UI向Core请求某个pm文件一段时间的音频数据
typedef struct _RequestPMAudioData_ 
{
	char pm_name[1024];                             //pm模板文件名字符串
	unsigned int ChannelNum;                   //声道号
	unsigned long long NumFrameBegin;   //从0开始的帧序号   
	unsigned long long NumFrameEnd;      //从0开始的帧序号
}RequestPMAudioData;


 //Core返回音频数据
typedef struct _ReponsePMAudioData_ 
{
	RequestPMAudioData Req;
	char failInfo[256];              //返回失败原因，没有填0，成功可以不填，失败原因可以弹窗提醒UI操作者，如文件不存在，时码超出正常范围
	unsigned int bufferSize;     //返回音频数据大小   为0表明失败   BufferSize!=end-begin  也是失败  
//	float buf[BufferSize];          //音频数据  float型-1.0到1.0
}ReponsePMAudioData;














#pragma pack(pop)       //恢复原来对齐状态

#endif