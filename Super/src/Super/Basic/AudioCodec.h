#ifndef AUDIOCODEC_H
#define AUDIOCODEC_H

#include "global.h" 
#include <stdio.h>
#include <math.h>


//音频编解码类 WAV读写，WAV与Mp3互转
//音频播放,音频信号发生器，白噪声，粉红噪声，音频滤波，音频频谱自动均衡
//音频低通，高通滤波
//混音算法Mix  
//回声消除、降噪
//音频拼接，根据音响特性，声音不能突然跳变--待续

NAME_SPACE_MAIN_BEGIN


//TODO:完整支持WAVE MP3 等音频格式编解码器
class AudioCodec
{
public:
	AudioCodec(void);
	~AudioCodec(void);
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#pragma  pack(push,1)
//必须确定字节对齐1字节，否则生成的文件不能播放
struct FMTINFO
{
    unsigned short    nFormatTag;       //format type  PCM音频数据的值为1
    unsigned short    nChannels;          //声道数，1：单声道，2：双声道
    unsigned int   nSamplesPerSec;      //音频数据的采样率,如44.1K 48K等
    unsigned int   nBytesPerSec;          //ByteRate每秒数据字节数 = SampleRate * NumChannels * BitsPerSample / 8
    unsigned short nBytesPerSample;  //每个采样所需的字节数 = NumChannels * BitsPerSample / 8
	unsigned short nBitsPerSample;     //每个采样点存储的bit数，8：8bit，16：16bit，32：32bit
};


//.wav文件的文件头结构,原始结构
typedef struct
{
    //Chunk RIFF
    char ChunkIDRIFF[4];                 //标识常数  'RIFF' (0x52494646)
    unsigned int ChunkSizeRIFF;      //fileSize - 8 整个文件的长度减去ID和Size的长度
    char chWAVE[4];                        //标识常数  'WAVE'(0x57415645)

    //Chunk FMT
    char ChunkIDFMT[4];                //标识常数  'fmt ' (0x666D7420)
    unsigned int ChunkSizeFMT;     //Size表示该区块数据的长度（不包含ID和Size的长度）
    FMTINFO FmtInfo;

    //ChunkData
    char ChunkIDDATA[4];               //标识常数'data' (0x64617461)
    unsigned int ChunkSizeDATA;    //此后的数据大小
    //UINT8* pBufer;
}WaveHeader;

#pragma  pack(pop)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



//解析得到的音频描述信息
struct WavInfo
{
    unsigned short  FormatTag;        /* format type */
    unsigned int Channels;               //声道数量
    unsigned int SampleRate;           //采样率
    unsigned int PerSampleBytes;     //每个采样点的字节数
    unsigned int PerSampleBits;        //每个采样点的位数
    unsigned int PerChSamples;        //每个声道采样点个数
    unsigned int rawPcmSize;            //原始PCM数据大小
};



/*bool CheckWaveHead(WaveHeader& header);    //检查文件头是否符合标准*/

class WavCodec
{
public:
	WavCodec(void);
	~WavCodec(void);
	bool OpenFile(const char* filename);
private:
	FILE* fp;
	uint64_t filesize;                 //文件大小
	float* pBuf;                        //文件缓存
	float* pBuf2;                      //文件缓存如果有右声道，存放右声道音频缓存
	unsigned int channels;       //声道数量
	unsigned int samplerate;    //采样速率
	WaveHeader header;
};




//一个简化的Wav格式解码器,打开立即加载文件第一声道内容到内存。
//暂时只考虑小端的情况,暂时最大支持2声道,默认只加载第一声道

//暂不支持,fact扩展以及24bit以上的格式,有空参考以下补全
//https://www.cnblogs.com/wangguchangqing/p/5957531.html

class WavReader
{
    enum 
    {
        Ch_Max=2,
    };
public:
    WavReader(void);
    ~WavReader(void);
    bool OpenFile(const char* filename);
    void Close();

    //取一帧n个点的音频数据,获取失败或者到结尾了返回false
    bool getFrame(int* pcm,unsigned int samples,bool* isEnd=0);
    void resetPos();

    unsigned int posGet;

    bool savePerChToFile();
private:
    //bool loadPcm();
    bool saveBuftoFile(char* buf,size_t len);

    bool FillPerChSamples(int* pData[Ch_Max],char* pcmRaw
        ,uint32_t Chs,uint32_t PerChSamples,uint32_t PerSampleBytes);

    size_t filesize;          //文件大小

    //int* pBuf;                           //文件缓存
    //int* pBuf2;                         //文件缓存如果有右声道，存放右声道音频缓存
    WavInfo wavInfo;



    int* pData[Ch_Max];                         //指向左右声道的数据指针。


    //unsigned int channels;                //声道数量
    //unsigned int SampleRate;            //采样速率
    //unsigned int PerChSamples;        //每个声道总的采样点数
};






int Mix(int a,int b);
float Mix(float a,float b);
double Mix(double a,double b);





class SineWave;           //正弦波
class TriangleWave;     //三角波
class SawtoothWave;   //锯齿波
class SquareWave;       //方波


//取余法不严谨，待续 0-1999 1999/2000!=1.0

static const double PI=3.1415926535897932384626;
static const double PI_M2=2*PI;

template<typename T>
struct Identity 
{ 
    typedef T type; 

    Identity(){}
    ~Identity(){}
    char tmp;  //nouse
};


template<class waveType=SineWave>
class SignalGenaratorSimple
{
public:
    SignalGenaratorSimple(size_t  nSampleRate=48000,size_t  nFreq=1000)
        :SampleRate(nSampleRate),Freq(nFreq)
    {
        setSampleRate(nSampleRate);
        setFreq(nFreq);
        reSetPos();
    }
    ~SignalGenaratorSimple()
    {

    }
    void setNewRate(size_t  nSampleRate,size_t  nFreq)
    {
        setSampleRate(nSampleRate);
        setFreq(nFreq);
    }
    void setSampleRate(size_t  nSampleRate)
    {
         this->SampleRate=nSampleRate;
    }
    void setFreq(size_t  nFreq)
    {
         this->Freq=nFreq;
         //if freq>sampleRate 合理范围判断,待续
        this->PeriodSamples=SampleRate/Freq;   //周期,为了实现任意频率,用浮点数
        // setCoef<waveType>();

       setCoef(Identity<waveType>());
    }
    void reSetPos()      //重新从相位0处生成
    {
         this->currPos=0;
    }



    //template<class SineWave>
    //void setCoef()
    //{
    //    this->tmpValue=PI_M2/this->PeriodSamples;
    //}
    //template<>
    //void  setCoef<TriangleWave>()
    //{
    //      this->tmpValue=this->PeriodSamples/2.0;
    //}
    //template<>
    //void  setCoef<SawtoothWave>()
    //{

    //}
    //template<>
    //void  setCoef<SquareWave>()
    //{
    //      this->tmpValue=this->PeriodSamples/2.0;
    //}

    //template<class SineWave>
    //double nextPoint()
    //{

    //    //double gain=0.2*::sin(2*PI*currPos++/(SampleRate/Freq));     //限制振幅

    //    size_t tmppos=(currPos++)%(PeriodSamples);
    //    //double gain=::sin(2.0*PI*tmppos/(SampleRate/Freq));

    //    double gain=::sin(2.0*PI*tmppos/(PeriodSamples-1));   //更精确?

    //    return gain;
    //}

    //template<>
    //double nextPoint<TriangleWave>()
    //{
    //    double PeriodSamples_half=this->tmpValue;
    //    size_t tmppos=(currPos++)%PeriodSamples;
    //    //double tmp=4.0*(tmppos/(double)PeriodSamples);
    //    double tmp=4.0*(tmppos/(double)(PeriodSamples-1));   //更精确?

    //    if (tmppos>PeriodSamples_half)
    //    {
    //        tmp=4.0-tmp;
    //    }
    //    tmp=tmp-1.0;
    //    return tmp;
    //}
    //template<>
    //double nextPoint<SawtoothWave>()
    //{
    //    size_t tmppos=(currPos++)%PeriodSamples;
    //    //double res=(2.0*tmppos/(double)PeriodSamples)-1.0;   
    //    double res=(2.0*tmppos/((double)PeriodSamples-1))-1.0; //更精确?
    //    return res;
    //}
    //template<>
    //double nextPoint<SquareWave>()
    //{
    //    double PeriodSamples_half=this->tmpValue;
    //    size_t tmppos=(currPos++)%PeriodSamples;
    //    if (tmppos<PeriodSamples_half)
    //    {
    //        return 1.0;
    //    }
    //    else
    //    {
    //        return -1.0;
    //    }
    //}


    //C++这样的类成员函数模板特化,以上写法只能在VS编译通过,不符合C++标准,gcc编译错误
    //改以下写法用函数参数重载实现,性能略有损失
    //方法一：
    void setCoef(Identity<SineWave>)
    {
        this->tmpValue=PI_M2/this->PeriodSamples;
    }
    void  setCoef(Identity<TriangleWave>)
    {
          this->tmpValue=this->PeriodSamples/2.0;
    }
    void  setCoef(Identity<SawtoothWave>)
    {
    }
    void  setCoef(Identity<SquareWave>)
    {
        this->tmpValue=this->PeriodSamples/2.0;
    }
    double nextPoint(Identity<SineWave>)
    {
        //double gain=0.2*::sin(2*PI*currPos++/(SampleRate/Freq));     //限制振幅
        size_t tmppos=(currPos++)%(PeriodSamples);
        //double gain=::sin(2.0*PI*tmppos/(SampleRate/Freq));
        double gain=::sin(2.0*PI*tmppos/(PeriodSamples-1));   //更精确?
        return gain;
    }
    double nextPoint(Identity<TriangleWave>)
    {
        double PeriodSamples_half=this->tmpValue;
        size_t tmppos=(currPos++)%PeriodSamples;
        //double tmp=4.0*(tmppos/(double)PeriodSamples);
        double tmp=4.0*(tmppos/(double)(PeriodSamples-1));   //更精确?
        if (tmppos>PeriodSamples_half)
        {
            tmp=4.0-tmp;
        }
        tmp=tmp-1.0;
        return tmp;
    }
    double nextPoint(Identity<SawtoothWave>)
    {
        size_t tmppos=(currPos++)%PeriodSamples;
        //double res=(2.0*tmppos/(double)PeriodSamples)-1.0;   
        double res=(2.0*tmppos/((double)PeriodSamples-1))-1.0; //更精确?
        return res;
    }
    double nextPoint(Identity<SquareWave>)
    {
        double PeriodSamples_half=this->tmpValue;
        size_t tmppos=(currPos++)%PeriodSamples;
        if (tmppos<PeriodSamples_half)
        {
            return 1.0;
        }
        else
        {
            return -1.0;
        }
    }

    //方法二:用静态模板推断+编译器优化实现
    //void setCoef()
    //{
    //    //虽然看起来像是运行时决议影响性能,但是if内表达式是编译器确定的,无用代码会被优化掉
    //    if (std::is_same<waveType,SineWave>::value)
    //    {
    //         this->tmpValue=PI_M2/this->PeriodSamples;
    //    }
    //    if (std::is_same<waveType,TriangleWave>::value)
    //    {
    //        this->tmpValue=this->PeriodSamples/2.0;
    //    }
    //    if (std::is_same<waveType,SawtoothWave>::value)
    //    {
    //    }
    //    if (std::is_same<waveType,SquareWave>::value)
    //    {
    //        this->tmpValue=this->PeriodSamples/2.0;
    //    }
    //}


    //方法三:



    //获取Ns个采样点的声音信号,范围为-1.0到1.0之间
    void getFrame(double* pcmOut,unsigned int Ns,float gain=0.5)
    {
        double* pos=pcmOut;
        double mg=gain;//*(INT_MAX);
        for (unsigned int n=0;n<Ns;n++)
        {
            //double tmp=nextPoint<waveType>();
            double tmp=nextPoint(Identity<waveType>());
            tmp*=mg;
            *(pos++)=tmp;    //tmp*gain
        }
    }
    //获取Ns个采样点的声音信号,范围为-INT_MAX到INT_MAX之间
    void getFrame(int* pcmOut,unsigned int samples,float gain=0.5)
    {
        int* pos=pcmOut;
        double mg=gain*(INT32_MAX);
        for (unsigned int n=0;n<samples;n++)
        {
            //double tmp=nextPoint<waveType>();
            double tmp=nextPoint(Identity<waveType>());
            tmp*=mg;
            *(pos++)=tmp;    //tmp*gain
        }
    }
private:
    uint64_t SampleRate;       //1S的采样点数
    uint64_t  Freq;                   //频率,整数单位是hz
    uint64_t  PeriodSamples;   //周期,为了实现任意频率,用浮点数
    uint64_t  currPos;         
    double tmpValue;                         
};




//template<class waveType>
//void SignalGenaratorSimple<waveType>::setCoef<SineWave>()
//{
//    this->tmpValue=PI_M2/this->PeriodSamples;
//}
//

//
//template<class SineWave>
//inline void SignalGenaratorSimple<SineWave>::setCoef<SineWave>()
//{
//    this->tmpValue=PI_M2/this->PeriodSamples;
//}
//template<class TriangleWave>
//void  SignalGenaratorSimple<TriangleWave>::setCoef<TriangleWave>()
//{
//        this->tmpValue=this->PeriodSamples/2.0;
//}
//template<class waveType,class SawtoothWave>
//void  SignalGenaratorSimple<waveType>::setCoef<SawtoothWave>()
//{
//
//}
//template<class T,class SquareWave>
//void  SignalGenaratorSimple<T>::setCoef<SquareWave>()
//{
//        this->tmpValue=this->PeriodSamples/2.0;
//}
//
//template<class T,class SineWave>
//double SignalGenaratorSimple<T,SineWave>::nextPoint()
//{
//
//    //double gain=0.2*::sin(2*PI*currPos++/(SampleRate/Freq));     //限制振幅
//
//    size_t tmppos=(currPos++)%(PeriodSamples);
//    //double gain=::sin(2.0*PI*tmppos/(SampleRate/Freq));
// 
//    double gain=::sin(2.0*PI*tmppos/(PeriodSamples-1));   //更精确?
//
//    return gain;
//}
//
//template<class T,class TriangleWave>
//double SignalGenaratorSimple<T>::nextPoint<TriangleWave>()
//{
//    double PeriodSamples_half=this->tmpValue;
//    size_t tmppos=(currPos++)%PeriodSamples;
//    //double tmp=4.0*(tmppos/(double)PeriodSamples);
//    double tmp=4.0*(tmppos/(double)(PeriodSamples-1));   //更精确?
//
//    if (tmppos>PeriodSamples_half)
//    {
//        tmp=4.0-tmp;
//    }
//    tmp=tmp-1.0;
//    return tmp;
//}
//template<class T,class SawtoothWave>
//double SignalGenaratorSimple<T>::nextPoint<SawtoothWave>()
//{
//    size_t tmppos=(currPos++)%PeriodSamples;
//    //double res=(2.0*tmppos/(double)PeriodSamples)-1.0;   
//    double res=(2.0*tmppos/((double)PeriodSamples-1))-1.0; //更精确?
//    return res;
//}
//template<class T,class SquareWave>
//double SignalGenaratorSimple<T>::nextPoint<SquareWave>()
//{
//    double PeriodSamples_half=this->tmpValue;
//    size_t tmppos=(currPos++)%PeriodSamples;
//    if (tmppos<PeriodSamples_half)
//    {
//        return 1.0;
//    }
//    else
//    {
//        return -1.0;
//    }
//}




#ifdef UNIT_TEST
void Test_AudioCodec();
#endif

NAME_SPACE_MAIN_END

#endif


/*
语音信号处理
音频和视频是二大信息传播的形式，随着计算机技术的发展。音频和视频技术也得到了广泛的应用。
本人硕士研究的主要方向是语音数字信号处理、语音识别和语音编码。毕业后继续从事语音数字信号处理工作。目前主要工作是对各种语音(speech)和音频(audio)编码标准算法进行学习，进而进行实际应用的移植和优化。

接触了语音数字信号处理这专业后，自己深深地喜欢上这个方向，也将自己的职业方向定于此。平时涉足语音数字信号处理的各各方面：主要有：语音识别、语音编码、语音增强，音频特效(回声，3D等)。还对语音端点检测各种算法进行了深入的研究。

要在这一方向上有所成绩，应该学习以下几门基础课程：
    一、数字信号处理
    二、随机过程
    三、几门专门讲述语音信号处理的书籍
   除此之外，还要多阅读相关的中英文文献，对各种算法进行认真的演算和相互的编程练习。编程语言可以根据自己的专长，我平时所用的就是: C 和 matlab.

工作一年半来，主要是做语音/音频编解码方面的工作。 先后接触过以下的编解码：
   WMA encoder；WMA decoder 包括standerd, profession and lossless,；MP3 encoder/deocder, AMR_NB encoder/decoder；G.729 encoder/decoder, FLAC(Free lossless audio codec) decoder；AAC plus；AC3.
   主要是基于ARM core做项目开发，所经历的指令集： ARMv4~ ARMv6， 接下来要好好学习ARMv7指令了，适应时代的要求啊。

  总结起来，在所有语音信号处理系统中，音频编码的深入研究学习对自己的能力提升会很快的，编码是在充分利用语音的数学模型和语音的相关时域的特性来去除冗余。
   speech codec 主要算法： 1、线性预测模型(LP) ---（Levison--Durbin）算法；2、LP--LSP--LSF 一些的系数转换；3、矢量量化；4、后滤波 等。
   代表codec: G.726, G.729系列， AMR_NB/WB, SVM 等变速率编码

     Audio Codec: 主要算法： 1、子带滤波；2、MDCT/IMDCT；3、量化(量化步长的自适应商定)；4， huffman encoder/decoder；5，multi-channel 的编码方式，最主要有：双声道 M/S(利用两声道数据的相关性)；多声道有mutli-channel transform ,也就是乘以一矩阵，能把2channel 变成多声道，相反操作也行。
   代表codec: WMA 系列；AAC 系列；MP3系列。

      上面只是大概说说主要的算法模型，还有很多算法细节要去好好学习，如不同的codec的帧长长度变化很大：从32samples到8192samples，甚至不定长的。等。 总之一句，很多算法都是可以reuse的，学通了可以多次应用，好好学习很重要啊。
*/


/*
如何实现音频淡入淡出效果

	淡入效果是指音频选区的起始音量很小甚至无声，而最终音量相对效大，则在一段时间范围内音量逐渐增大。
	淡出效果是指音频选区的起始音量相对较大，而最终音量很小甚至无声，则在一段时间范围内音量逐渐降低。

	音量的逐渐变化是一个线性代数问题，可通过如下代码去对每一帧数据进行线性处理：
	While(!音乐结束)
{
	获取一帧音频数据放入缓存;
	short* pData = (short*)一帧音频数据缓存;// 这里我们处理16位的采样数据，                                     
	//因此需要将BYTE类型转换成short类型

	double rate = 1.0f;
	if(音乐当前播放位置-音乐开始播放位置)<=淡入时间长度)//淡入
	{
		rate = (double)( 音乐当前播放位置-音乐开始播放位置)/ 淡入时间长度;
	}
	else if(音乐结束位置-音乐当前播放位置<=淡出时间长度)//淡出
	{
		rate = (double)( 音乐结束位置-音乐当前播放位置)/( 淡出时间长度);
	}
	if (rate<0.0f)//防止计算误差   
	{
		rate = 0.0f;
	}
	if (rate<1.0f&&rate>=0.0f)
	{

		for (int i=0;i<一帧音频数据缓存大小/2;i++)
		{
			pData[i]*=rate;//对每个采样数据进行线性转换
		}
	}
	播放一帧音频数据;
}
*/

/*
在数字音频领域，常用的采样率有： 
8,000 Hz - 电话所用采样率, 对于人的说话已经足够 
11,025 Hz 
22,050 Hz - 无线电广播所用采样率 
32,000 Hz - miniDV 数码视频 camcorder、DAT (LP mode)所用采样率 
44,100 Hz - 音频 CD, 也常用于 MPEG-1 音频（VCD, SVCD, MP3）所用采样率 
47,250 Hz - Nippon Columbia (Denon)开发的世界上第一个商用 PCM 录音机所用采样率 
48,000 Hz - miniDV、数字电视、DVD、DAT、电影和专业音频所用的数字声音所用采样率 
50,000 Hz - 二十世纪七十年代后期出现的 3M 和 Soundstream 开发的第一款商用数字录音机所用采样率 50,400 Hz - 三菱 X-80 数字录音机所用所用采样率 
96,000 或者 192,000 Hz - DVD-Audio、一些 LPCM DVD 音轨、BD-ROM（蓝光盘）音轨、和 HD-DVD （高清晰度 DVD）音轨所用所用采样率 
2.8224 MHz - SACD、 索尼 和 飞利浦 联合开发的称为 Direct Stream Digital 的 1 位 sigma-delta modulation 过程所用采样率。
*/