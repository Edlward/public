#ifndef SRC_DSP_H_
#define SRC_DSP_H_

#include "struct_define_h.h"
#include <arm_neon.h>

typedef float CoefFloat;			//二阶用float

#define MAX_SSE_VECTORS (10)
#define MAX_VECTORS			(5)		//5个2阶	

//20-20Khz声音划分成31个频段,每个频段中心频率如下:
//static const float band_f031[] =
//{20,25,31.5,40,50,63,80,100,125,160,200,250,315,400,500,630,800,1000,1250,1600,2000,2500,3150,4000,5000,6300,8000,10000,12500,16000,20000};

class DSP
{
	//2阶的参数
	typedef __attribute__((align(32))) struct _XYN2_
	{
		float x[3][4]; // x[n], x[n-1], x[n-2]
		float y[3][4]; // y[n], y[n-1], y[n-2]
	}XYN2;

	typedef struct
	{
		XYN2 sXYN2[MAX_VECTORS];		//混合滤波器
	}sXYData;

	typedef struct _SSECoeFN2_
	{
		float32x4_t beta1;
		float32x4_t alpha1;
		float32x4_t alpha2;
	}SSECoeFN2;

	typedef struct
	{
		SSECoeFN2 SSECoefN2[MAX_SSE_VECTORS];
	}SSECoefficients;


public:
	DSP();
	virtual ~DSP();
	//待续 gain[EQ_BANDS][EQ_CHANNELS] 改 gain[EQ_CHANNELS][EQ_BANDS] 寻址更快速

	void InitZero();  //初始化各种增益系数为0

	void printParam();

	//rafactor by lizulin 20171115
	//int 改unsigned int 不需要考虑小于0的情况
	//新增以声道作为区分合并spk和bed代码，避免重复代码，避免大量宏定义
	//hasSound 表示本帧输入声音 是否有声，如果无声，当某个点输出衰减到0时候直接退出，节约性能
	void EqSpkNSample(int *out, int *in, unsigned int nSpk,unsigned int nSample,bool hasSound=true);   //add by lizulin
	void SetSpkAmp(unsigned int nSpk, float db);
	void SetSpkGain(unsigned int nSpk, float gain[EQ_BANDS]);
	void SetSpkGain(unsigned int nSpk, unsigned int nBand, float db);
	void SetAllSpkGain(float gain[EQ_BANDS][EQ_CHANNELS]);
	void SetAllSpkAmp(float db[EQ_CHANNELS]);
	int ClearSpk(unsigned int nSpk);

	// 返回换算后的音箱音量值 [12/26/2017 jaontolt]
	float GetSpkAmp(unsigned int nSpk);
	float GetBedAmp(unsigned int nBed);

// 	void EqBedNSample(float* out, float *in, unsigned int nBed,unsigned int nSample,bool hasSound=true);   //add by lizulin
// 	void SetBedGain(unsigned int nBed, unsigned int nBand, float db);
// 	void SetBedGain(unsigned int nBed, float gain[EQ_BANDS]);
// 	void SetAllBedGain(float gain[EQ_BANDS][EQ_CHANNELS+EQ_BEDS]);
// 	void SetBedAmp(unsigned int nBed, float db);
// 	void SetAllBedAmp(float db[EQ_BANDS]);
// 	void GetBedGain(unsigned int nBed, unsigned int nBand, float&db);
// 	void GetBedGain(unsigned int nBed, float gain[EQ_BANDS]);
// 	void GetAllBedGain(float gain[EQ_BANDS][EQ_CHANNELS+EQ_BEDS]);
// 	void GetBedAmp(unsigned int nBed, float& db);
// 	void GetAllBedAmp(float db[EQ_BEDS]);
// 	int ClearBed(unsigned int nBed);//nBedΪBED L/R/C/LFE/LSS/RSS/LRS/RRS/LTS/RTS
	//////////////////////////////////////////////////////////////////////////

	//清除EQ残留
	//针对某个扬声器没有被分配到不做EQ，然后可能会突然分配到，EQ的历史参数不连续
	//所以为了节约性能不被分配到不做EQ,但是清除历史参数
	//也可以不用以下函数，然后始终连续每帧做EQ
	void ClearHistoryAll();
	void ClearHistorySpk(unsigned int nSpk);
// 	void ClearHistoryBed(unsigned int nBed);
	void LoadCoeff();
private:
	int loop;   //SIMD加速band循环次数

	float32x4_t SSE_SpkGain[MAX_SSE_VECTORS][EQ_CHANNELS];

	float SpkOutGain[EQ_CHANNELS];                       //=SpkAmp*2.0  主要为了某个采样点节约1次乘法
	float SpkAmp[EQ_CHANNELS];										//某个扬声器的增益(影响所有频段)

	float SpkAbsAmp[EQ_CHANNELS];                       //某个扬声器的增益(影响所有频段) 单位是分贝dB

	float SpkGain[EQ_BANDS][EQ_CHANNELS];				//某个扬声器在某个频道的增益，手动控制
	float SpkAbsGain[EQ_BANDS][EQ_CHANNELS];			//某个扬声器在某个频道的增益，手动控制  单位是分贝dB


#define WideBrandNum 10
	SSECoefficients SSEeq_coeffs[WideBrandNum];   //SIMD 优化的滤波器系数  或直接定义最大8个

	//二阶
	typedef struct _N2_
	{
		unsigned int in[3];
	}N2;

	typedef struct _iN_
	{
		N2 iN2[5];				//5个2阶
	}iN;

	bool bEQ;    //运行时决议,可废弃

	float dither[256]; //随机扰动
	int di;

	iN iNSpk[EQ_CHANNELS];
	sXYData data_historySpk[EQ_BANDS][EQ_CHANNELS];    //10个bed声场和64/128个扬声器
};

DSP* GetDsp();

#endif
