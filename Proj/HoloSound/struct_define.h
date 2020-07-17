#ifndef _STURCT_DEFINE_H_
#define _STURCT_DEFINE_H_

//#include "../../../public/Super/src/Super/Basic/Int24.h"

#include <stdio.h>

//#include "xil_types.h"                  //remove this dependency  by lizulin
typedef unsigned char u8;             //
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

#define MAX_INT24			(8388607)
#define MIN_INT24			(-8388608)



#ifndef MY_ALIGN   //��ƽ̨�ֽڶ���
#ifdef _WIN32
#define MY_ALIGN(bytes)   __declspec (align(bytes))
#else
#define MY_ALIGN(bytes)   __attribute__ ((aligned(bytes)))
#endif
#endif

typedef struct MY_ALIGN(1) _INT24_
{
	u8 m_btyes[3];

	//      operator u32() const
	//      {
	//    	  return operator int();
	//      }
	operator s32() const
	{
#if defined(TO_I2S)
        int integer=(s32)((s32)m_btyes[2]<<24)|((s32)m_btyes[1]<<16)|((s32)m_btyes[0]<<8);  //
        //int integer=((m_btyes[2]<<24)&0XFF000000)|((m_btyes[1]<<16)&0x00FF0000)|((m_btyes[0]<<8)&0X0000FF00);
#else
		int integer=(m_btyes[2]<<16)|(m_btyes[1]<<8)|m_btyes[0];
		if (m_btyes[2]&0x80)     //�˾��� (integer&0x00800000) ����������
		{
            integer|=~0xffffff;
            //integer|=0xFF000000;  //һ����
		}
#endif
		return integer;
	}
    _INT24_& operator =(const s32 input)
    {
          m_btyes[0]=((unsigned char*)&input)[0];
          m_btyes[1]=((unsigned char*)&input)[1];
          m_btyes[2]=((unsigned char*)&input)[2];

//        m_btyes[0]=(input&0x000000ff);
//        m_btyes[1]=(input&0x0000ff00)>>8;
//        m_btyes[2]=(input&0x00ff0000)>>16;

        return *this;
    }
    _INT24_& operator =(const u32 input)
    {
          m_btyes[0]=((unsigned char*)&input)[0];
          m_btyes[1]=((unsigned char*)&input)[1];
          m_btyes[2]=((unsigned char*)&input)[2];

//        m_btyes[0]=(input&0x000000ff);
//        m_btyes[1]=(input&0x0000ff00)>>8;
//        m_btyes[2]=(input&0x00ff0000)>>16;
        return *this;
    }
}INT24;





//////////////////////////////////////////////////////////////////////////
//	Audio buffer definition

#define AudioTrack		(128)
#define PerFrameSampleMax	(2000)


#define USE_INT24

//gcc error volatile
typedef  struct _STRUC_ADUIO_BUF 
{
#if defined(USE_INT24)
	typedef INT24 PcmType;
#else
	typedef int PcmType;
#endif

    PcmType data[PerFrameSampleMax*AudioTrack];
	//volatile int data[AudioTrack][PerFrameSampleMax];   
    //bool hasSound[AudioTrack];

	volatile union
	{
		volatile int wide32;
		volatile struct
		{
			volatile char c0;
			volatile char c1;
			volatile char c2;
			volatile char c3;
		} wide8;
	} rwFlag;
    //buffer����д���rwFlag��Ϊ0x01010101����ʾ���ݿɱ�����
    //buffer���ݶ����rwFlag��Ϊ0����ʾbuffer������д���µ�����
    _STRUC_ADUIO_BUF()
    {
        //memset(data,0,sizeof(data));

        //for (int k=0;k<PerFrameSampleMax*AudioTrack;k++)
        //{
        //    data[k]=0;
        //}

		rwFlag.wide32=0;
    }

    volatile unsigned long long currFrameCnt;       //��ǰ֡��
    volatile long long checkSum;                            //fast check sum
    volatile int Resv[8];                                            //Ԥ��

    long long getCheckSum()
    {
        long long sum=0;
        for (int k=0;k<PerFrameSampleMax*20;k+=11)   //chs<20
        {
            int tmp=(int)data[k];
            sum+=(tmp>>8);
        }
        return sum;
    }
    void fillCheckSum()
    {
        checkSum=getCheckSum();
    }
    void doCheck()
    {
        long long currChecksum=getCheckSum();
        if (currChecksum!=this->checkSum)
        {
            printf("bad data!\n");
            //xil_printf("bad data!\n");
        }
    }
} STRUC_AUDIO_BUF;


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



#define EQ_BANDS         (31)
//#define EQ_CHANNELS	       (64)  //macro definition conflict
#define EAH_EQ_CHANNELS	   (64)   //or 128��

typedef struct _STRUCT_EQ_CMD
{
	int cmd;             // ϵ���ı�ʱ����ΪEQ_UPDATE,ʹ���߸��º���ΪEQ_DONOTHING,
	int eqState;         // EQ�Ƿ�ʹ��EQ_BYPAS��EQ_ENABLE,
	float spkBandGain[EQ_BANDS][EAH_EQ_CHANNELS];    // Ϊ�˼���dsp�ӿ�,��dbΪ��λ��ע����spkOutGain������
	float spkOutGain[EAH_EQ_CHANNELS];               // Ϊ�����Ч�ʣ�Ϊʵ������0~1 floatֵ��������dbΪ��λ
} EQ_CMD;

typedef struct _STRUCT_BT_CMD
{
    int cmd;
    float spkBass[4][EAH_EQ_CHANNELS];
    float spkTreble[4][EAH_EQ_CHANNELS];
}BT_CMD;

typedef struct _FADE_BUF
{
	float outFade;             //0-1.x
	float micInGain1;
	float micInGain2;
}FADE_BUF;


typedef struct _MONITOR_SELECT
{
	float monitor[64];
}MONITOR_SELECT;


typedef struct _MINI_UI_DATA{
	char bSettingState;		// 0 - ������״̬��- ����״̬
	char ItemId;			// ��ǰѡ�У�- ��������- Mic1������- Mic2����
	char bMute;				// 0 - �Ǿ�����1 - ����
    char bMic1Mute;
    char bMic2Mute;
	char bDim;				// 0 - ��Dim��- Dim
	char nLang;				// ���ԣ�- ���ģ�- Ӣ��
    char Reserved;          // ��������UI�� 0x75
	float fade;				// ������ֵ��.0~10.0
	float mic1;				// Mic1����ֵ��,0~10.0
	float mic2;				// Mic2����ֵ��.0~10.0
    char strIp1[16];			// ��\0 �������ַ�������"192.168.64.104"
    char strIp2[16];
	char strSn[8];			// ��\0 �������ַ�������"1005001"
	char strVer[9];			// ��\0 �������ַ�������"20191231"
}MINI_UI_DATA;



//////////////////////////////////////////////////////////////////////////
// I2C EQ PARAMETER
typedef struct _STRUCT_EQ_CMD_IIC_32CH
{
    u16 subAddr;
    char Reserved[2];
	int cmd;             // ϵ���ı�ʱ����ΪEQ_UPDATE,ʹ���߸��º���ΪEQ_DONOTHING,
	int eqState;         // EQ�Ƿ�ʹ��EQ_BYPAS��EQ_ENABLE,
	float spkBandGain[EQ_BANDS][32];    // Ϊ�˼���dsp�ӿ�,��dbΪ��λ��ע����spkOutGain������
	float spkOutGain[32];               // Ϊ�����Ч�ʣ�Ϊʵ������0~1 floatֵ��������dbΪ��λ
    u32 sum;
	_STRUCT_EQ_CMD_IIC_32CH()
	{
        subAddr = 0x0000;
	}
} EQ_CMD_IIC_32CH;

typedef struct _STRUCT_BT_CMD_IIC_32CH
{
    u16 subAddr;
    char Reserved[2];
    int cmd;             // ϵ���ı�ʱ����ΪEQ_UPDATE,ʹ���߸��º���ΪEQ_DONOTHING,
    float spkBass[4][32];    // Ϊ�˼���dsp�ӿ�,��dbΪ��λ��ע����spkOutGain������
    float spkTreble[4][32];               // Ϊ�����Ч�ʣ�Ϊʵ������0~1 floatֵ��������dbΪ��λ
    u32 sum;
    _STRUCT_BT_CMD_IIC_32CH()
    {
        subAddr = 0x0003;
    }
}BT_CMD_IIC_32CH;

typedef struct _FADE_BUF_IIC
{
    u16 subAddr;
    char Reserved[2];
	float outFade;             //0-1.x not dB
	float micInGain1;
	float micInGain2;
    u32 sum;
	_FADE_BUF_IIC()
	{
        subAddr = 0x0001;
		outFade = 0.0f;
		micInGain1 = 0.0f;
		micInGain2 = 0.0f;
	}
}FADE_BUF_IIC;

typedef struct _MONITOR_SELECT_IIC
{
    u16 subAddr;
    char Reserved[2];
	float monitor[64];
    u32 sum;
	_MONITOR_SELECT_IIC()
	{
        subAddr = 0x0002;
	}
}MONITOR_SELECT_IIC;



typedef struct _STRUCT_EQ_BAND_GAIN_MODIFY
{
    u16 subAddr;
    unsigned char nChannel;
    unsigned char nBand;
    float fValue;
    u32 sum;
    _STRUCT_EQ_BAND_GAIN_MODIFY()
    {
        subAddr = 0x1000;
    }
}EQ_BAND_GAIN_MODIFY;

typedef struct _STRUCT_EQ_SPK_GAIN_MODIFY
{
    u16 subAddr;
    unsigned char nChannel;
    unsigned char reserved;
    float fValue;
    u32 sum;
    _STRUCT_EQ_SPK_GAIN_MODIFY()
    {
        subAddr = 0x1001;
    }
}EQ_SPK_GAIN_MODIFY;


typedef struct _STRUCT_BT_SPK_BASS_MODIFY
{
    u16 subAddr;
    unsigned char nChannel;
    unsigned char nBand;
    float fValue;
    u32 sum;
    _STRUCT_BT_SPK_BASS_MODIFY()
    {
        subAddr = 0x3000;
    }
}BT_SPK_BASS_MODIFY;

typedef struct _STRUCT_BT_SPK_TREBLE_MODIFY
{
    u16 subAddr;
    unsigned char nChannel;
    unsigned char nBand;
    float fValue;
    u32 sum;
    _STRUCT_BT_SPK_TREBLE_MODIFY()
    {
        subAddr = 0x3001;
    }
}BT_SPK_TREBLE_MODIFY;


typedef struct _CROSSOVER_PARAM{
    u16 enable;
    u16 co_segment;
    u8 ch_in;
    u8 ch_out_low;
    u8 ch_out_mid;
    u8 ch_out_hig;
    float lpf;
    float hpf;
}CROSSOVER_PARAM;

typedef struct _CROSSOVER_IIC
{
    u16 subAddr;
    u8 Reserved[2];
    CROSSOVER_PARAM param[64];
    u32 sum;
    _CROSSOVER_IIC()
    {
        subAddr = 0x0003;
    }
}CROSSOVER_IIC;

typedef struct _CROSSOVER_SPK_MODIFY
{
    u16 subAddr;
    u8 enable;
    u8 co_segment;
    u8 ch_in;
    u8 ch_out_low;
    u8 ch_out_mid;
    u8 ch_out_hig;
    float lpf;
    float hpf;
    u32 sum;
    _CROSSOVER_SPK_MODIFY()
    {
        subAddr = 0x1002;
    }
}CROSSOVER_SPK_MODIFY;

typedef struct _SRAM_DATA
{
    u32 serialNumber;
    u32 publicLen;
    u32 privateLen;
    u8 pub[4096];
    u8 key[4096];
} SRAM_DATA;

#define SHARE_RING_BUF_NUM 6

//DCAP-EAH�潻�����ƹ����ڴ����ݽṹ
enum 
{
    Const_INIT_Key=0x75852020
};

typedef enum _FRAME_RATE_
{
    FR_24 = 0,
    FR_25,
    FR_30,
    FR_48,
    FR_50,
    FR_60,
    FR_96,
    FR_100,
    FR_120,
} _FRAME_RATE;

struct ShareMemOfDCAP_EAH
{
	volatile int initFlag;  //Const_INIT_Key   //�״�ʹ�ñ������ڴ�ṹ������initFlag!=Const_INIT_Key,���ù��캯�����챾��

    volatile unsigned int ReOpenFlag;            //Set-- 0x7121, success(0x7222),failed(0x7332)
    volatile unsigned int SampleRate;            //48000 or96000
    volatile unsigned int FrameRate;              //_FRAME_RATE: 24-25-30-48-50-60-96-100-120
    volatile unsigned int PerFrameSamples;  //2000 1920
    volatile unsigned int InputFrameCnt;       //
    volatile unsigned int OutputFrameCnt;    //
    volatile unsigned int BufInPos;
    volatile unsigned int BufOutPos;
	volatile unsigned int InputChs;
	volatile unsigned int OutputChs;
    volatile int ResvCtrl[16];       

    STRUC_AUDIO_BUF AudioToA53[SHARE_RING_BUF_NUM];
    STRUC_AUDIO_BUF AudioToR5[SHARE_RING_BUF_NUM];

	//Append data for MiniUI
	MINI_UI_DATA miniUiData;
    SRAM_DATA sramData;

    //�������Ժ����
    EQ_CMD eqCmd;
    FADE_BUF fadeBuf;
    MONITOR_SELECT monitor;
    //////////////////////////////////////////////////////////////////////////

    ShareMemOfDCAP_EAH()
	{
		BufInPos=0;
		BufOutPos=0;
	}
};



#if 0
//�������EAH���ⲿģ�鹲���ڴ�Ԥ��32MB
//Ϊ�˱�֤�����ڴ��С������������м���,�ڲ��ı��ܴ�С�������,����������Ա��Tmp�ṹβ����
//����Ŀ�ȶ��������������� by lizulin 20200619
struct ShareMemOfDCAP_EAH:public ShareMemOfDCAP_EAHTmp
{
    //char resv[30*1024*1024-sizeof(ShareMemOfDCAP_EAHTmp)];
};
#endif



extern ShareMemOfDCAP_EAH* pGlobalSM;    //used at different modules,set as global value 

const int size1=sizeof(STRUC_AUDIO_BUF);
const int size2=sizeof(ShareMemOfDCAP_EAH);

 //�״�ʹ�ñ������ڴ�ṹ������initFlag!=Const_INIT_Key,���ù��캯�����챾��
//if (pGlobalSM->initFlag!=Const_INIT_Key)
//{
//    new(pGlobalSM) ShareMemOfDCAP_EAH;
//   pGlobalSM->initFlag=Const_INIT_Key;
//}




#define AUDIO_BUFFER_BASE		(0x70000000)

#ifdef __DATA_FOR_MAIN__

STRUC_AUDIO_BUF *AudioIn[2] = {(STRUC_AUDIO_BUF*)(AUDIO_BUFFER_BASE),
	(STRUC_AUDIO_BUF*)(sizeof(STRUC_AUDIO_BUF) + AUDIO_BUFFER_BASE)
};

STRUC_AUDIO_BUF *AudioOut[2] = {(STRUC_AUDIO_BUF*)(sizeof(STRUC_AUDIO_BUF)*2 + AUDIO_BUFFER_BASE),
	(STRUC_AUDIO_BUF*)(sizeof(STRUC_AUDIO_BUF)*3 + AUDIO_BUFFER_BASE)
};


#endif



#endif
