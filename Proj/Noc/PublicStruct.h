#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H
//Core��UI���õĽṹ�壬�꿪�ص�
#include <string>
#include <vector>


#define USE_DOLBY_AXIS   //�Ƿ����dolby����ϵ����





#pragma pack(push,1)     //����һ���ֽڶ���  



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


//һ������ĳʱ�̵������������Ϣ
class PositionInfo:public Position
{
public:
	float size;   //δ�����ܻ�֧��size���� Bitstream�ĵ�û��size����   	
	//����IABitstream_SMPTE_v3.doc�ĵ�  δ�����ܻ�֧�����¸�������
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
	//float extent;   //��ʵ����size ��ɾ
	//float coherence;
	//float snap;
	//float zoneGain[19];       //��ʱ����ȷ�����ɾ��
	PositionInfo()
	{
		//memset(this,0,sizeof(*this));   //���ṹ��������������
		char* pstart=(char*)this;            
		for (int i=0;i<sizeof(*this);++i)  //���ļ����������κ�ͷ�ļ����ֶ�ʵ��memset����
		{
			pstart[i]=0;
		}
	}
};



class ObjMetaData
{
public:
	int id;
	unsigned int timecode;   //��unsigned int
	//new   �����֡����֧��
	enum
	{
		NumPanSubBlocksMax=8             //����IABitstream_SMPTE_v3.doc�ĵ� �ӿ�����NumPanSubBlocks���Ϊ8,
	};
	unsigned int NumSubBlocks;                //��ǰ��Ч���ӿ�����,ȡ1��NumPanSubBlocksMax,
	PositionInfo SubPos[NumPanSubBlocksMax];   //   
	ObjMetaData()
	{
		id=-1;      //-1 ��ʾ��ЧID��Ĭ����Ч
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

	//���ºϲ���speakerConfig
	//std::string name;
	//std::string comment;
	int special;
	char comment[64];
}Speaker;



//��room��ΪMPU Core��Monitor���ݵ�Ӱ���ߴ���Ϣ�����ݽṹ�������£�
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



//�����������SL�������ˣ�����ΪSLV
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

	CONTENT_PATH_SRC,   //Դ·ַ
	CONTENT_PATH_DST,   //Ŀ·ַ
};



//��SpeakerList��SpeakerListΪMPU Core��Monitor���͵�n�����䣨����10��BED����XYZ���ꡢSoundLoud��channel���������ֶ��壬���ݽṹ���¡�
//10��BED���������ֶ���ֱ�ΪL��R��C��LFE��Lss��Rss��Lsr��Rsr��Lts��Rts��

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



//CMD_GET_PM_AUDIO_DATA,              //MPU-UI��Core����pmĸ���һ����Ƶ����
//CMD_GET_PM_AUDIO_DATA_RESP,     //Core������Ƶ����


//UI��Core����ĳ��pm�ļ�һ��ʱ�����Ƶ����
typedef struct _RequestPMAudioData_ 
{
	char pm_name[1024];                             //pmģ���ļ����ַ���
	unsigned int ChannelNum;                   //������
	unsigned long long NumFrameBegin;   //��0��ʼ��֡���   
	unsigned long long NumFrameEnd;      //��0��ʼ��֡���
}RequestPMAudioData;


 //Core������Ƶ����
typedef struct _ReponsePMAudioData_ 
{
	RequestPMAudioData Req;
	char failInfo[256];              //����ʧ��ԭ��û����0���ɹ����Բ��ʧ��ԭ����Ե�������UI�����ߣ����ļ������ڣ�ʱ�볬��������Χ
	unsigned int bufferSize;     //������Ƶ���ݴ�С   Ϊ0����ʧ��   BufferSize!=end-begin  Ҳ��ʧ��  
//	float buf[BufferSize];          //��Ƶ����  float��-1.0��1.0
}ReponsePMAudioData;














#pragma pack(pop)       //�ָ�ԭ������״̬

#endif