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
    int cmd;               //ϵ���ı�ʱ����ΪEQ_UPDATE,ʹ���߸��º���ΪEQ_DONOTHING,
    int eqState;         //EQ�Ƿ�ʹ��EQ_BYPAS��EQ_ENABLE,
	float spkBandGain[EQ_BANDS][EQ_CHANNELS];    //Ϊ�˼���dsp�ӿ�,��dbΪ��λ
	float spkOutGain[EQ_CHANNELS];                           //Ϊ�˼���dsp�ӿ�,��dbΪ��λ
} EQ_CMD;



typedef struct _FADE_BUF
{
	float outFade;             //���¼���gain,Ϊ��Ч��ֱ��ʹ��ʵ�ʳ˷�����ϵ�� 0-1.x
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
	char bSettingState;	// 0 - ������״̬��1- ����״̬
	char ItemId;	// ��ǰѡ�У�0- ��������1- Mic1������2- Mic2����
	char bMute;	// 0 - �Ǿ�����1 - ����
	char bDim;	// 0 - ��Dim��- Dim
	char nLang;	// ���ԣ�- ���ģ�- Ӣ��
	float fade;	// ������ֵ��.0~10.0
	float mic1;	// Mic1����ֵ��,0~10.0
	float mic2;	// Mic2����ֵ��.0~10.0
	char strIp[16];	// ��\0 �������ַ�������"192.168.64.104"
	char strSn[8];	// ��\0 �������ַ�������"1005001"
	char strVer[9];	// ��\0 �������ַ�������"20191231"
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