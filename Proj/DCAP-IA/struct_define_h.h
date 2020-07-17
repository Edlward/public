#ifndef _STURCT_DEFINE_H_
#define _STURCT_DEFINE_H_


#define MAX_INT24			(8388607)
#define MIN_INT24			(-8388608)

//////////////////////////////////////////////////////////////////////////
// EQ buffer definition

enum EQSTATE_CODE
{
	EQ_BYPASS = 0x00,
	EQ_ENABLE,
};

enum EQCOMMAND_CODE
{
	EQ_DONOTHING = 0x00,
	EQ_UPDATE,
};

#define EQ_BANDS			(31)
#define EQ_CHANNELS			(16)

typedef struct _STRUCT_EQ_CMD
{
    int cmd;               //系数改变时设置为EQ_UPDATE,使用者更新后置为EQ_DONOTHING,
    int eqState;         //EQ是否使能EQ_BYPAS或EQ_ENABLE,
	float spkBandGain[EQ_BANDS][EQ_CHANNELS];    //为了兼容dsp接口,以db为单位
	float spkOutGain[EQ_CHANNELS];                           //为了兼容dsp接口,以db为单位
} EQ_CMD;



typedef struct _FADE_BUF
{
	float outFade;             //以下几种gain,为了效率直接使用实际乘法增益系数 0-1.x
	float micInGain1;
	float micInGain2;
}FADE_BUF;

typedef struct _PINK_CTRL_
{
    //0 - is disable output pink noise
    //1~16 is 16 channel output pink noise, only 1 channel select each time
    int channel;
} PINK_CTRL;

typedef struct _HAS_SOUND_
{
    char in[64];
    char out[64];
} HAS_SOUND;

typedef struct _CH_VOLUME_
{
    float fpercent[16];
} CH_VOLUME;

typedef struct _MINI_UI_DATA
{
	char bSettingState;	// 0 - 非设置状态，1- 设置状态
	char ItemId;	// 当前选中：0- 主音量，1- Mic1音量，2- Mic2音量
	char bMute;	// 0 - 非静音，1 - 静音
	char bDim;	// 0 - 非Dim，- Dim
	char nLang;	// 语言：- 中文，- 英文
	float fade;	// 主音量值，.0~10.0
	float mic1;	// Mic1音量值，,0~10.0
	float mic2;	// Mic2音量值，.0~10.0
	char strIp[16];	// 以\0 结束的字符串，如"192.168.64.104"
	char strSn[8];	// 以\0 结束的字符串，如"1005001"
	char strVer[9];	// 以\0 结束的字符串，如"20191231"
} MINI_UI_DATA;

//////////////////////////////////////////////////////////////////////////
//	Audio buffer definition

#define AudioTrack			(18)
#define PerFrameSampleMax	(2000)

typedef struct _STRUC_ADUIO_BUF {
	int data[AudioTrack][PerFrameSampleMax];
	union 
	{
		volatile int wide32;
		struct  
		{
			volatile char c0;
			volatile char c1;
			volatile char c2;
			volatile char c3;
		} wide8;
	} rwFlag;
} STRUC_AUDIO_BUF;

#endif