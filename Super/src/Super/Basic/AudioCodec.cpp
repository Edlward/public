#include "AudioCodec.h"
#include <memory.h>  //qlib 需要
#include <stdlib.h>  //qlib 需要
#include "BasicOperation.h"     //自定义的基础操作类

#include <iostream>
#include<fstream> 
#include<math.h>
using namespace std;

NAME_SPACE_MAIN_BEGIN

#include "Int24.h"

AudioCodec::AudioCodec(void)
{
}


AudioCodec::~AudioCodec(void)
{
}




//生成特定频率音频的方法

//#include <windows.h> 
//#include <mmsystem.h>


#define WAVE_HEAD_LENGTH 44//wav头文件长度
#define m_samplefreq 44100
#define m_channels 2 
#define m_channelbits 8
#define MATH_PI 3.14159265358979


//typedef unsigned long DWORD;
//typedef unsigned short WORD;




void MakeWaveData(int rate, int freq, int amp, char* p, int len)//采样率、频率、音量、采样点数
{
    int flag = 0;
    if (m_channelbits == 16)        //16位
    {
        if (m_channels == 1)
        {
            for (int i = 0; i < len; i++)
            {
                unsigned short  v = amp/100*32768 * sin(2 * MATH_PI * freq * i / rate);
                *(p + flag) = v & 0xFF;//低8位
                *(p + flag + 1) = (v >> 8) & 0xFF;//16bit量化 高8位
                flag += 2;
            }
        }
        else
        {
            for (int i = 0; i < len; i++)
            {
               unsigned short vl = amp / 100 * 32768 * sin(2 * MATH_PI * freq * i / rate) ;
               unsigned short vr = amp / 100 * 32768 * sin((2 * MATH_PI * freq * (i+5) )/ rate) ;
                *(p + flag) = (vl & 0xFF);      
                *(p + flag + 1) = ((vl >> 8) & 0xFF);
                *(p + flag + 2) = (vr & 0xFF);
                *(p + flag + 3) = ((vr >> 8) & 0xFF);
                flag += 4;
            }
        }
    }
    else
    {   
        if (m_channels == 1)
        {
            for (int i = 0; i < len; i++)
            {
                *(p + i) = sin(i * (MATH_PI * 2) / rate * freq) * amp * 128 / 100 + 128;
            }
        }
        else
        {
            //flag=0;//NEW
            for (int i = 0; i < len; i++)
            {
                *(p + flag)=sin(i * (MATH_PI * 2) / rate * freq) * amp * 128 / 100+128;
                *(p + flag + 1)=sin((i+5) * (MATH_PI * 2) / rate * freq) * amp * 128 / 100+128;
                flag += 2;
            }
        }
    }
}
int Create(int freq, int volume, int durations)//频率、音量、持续时间
{
    WaveHeader *pHeader = new WaveHeader;
    unsigned int totalLen = (m_samplefreq * m_channels * m_channelbits / 8) * durations + 44;//文件总长度=(采样率 * 通道数 * 比特数 / 8) * 持续时间(s)
    pHeader->ChunkIDRIFF[0] = 'R';
    pHeader->ChunkIDRIFF[1] = 'I';
    pHeader->ChunkIDRIFF[2] = 'F';
    pHeader->ChunkIDRIFF[3] = 'F';
    pHeader->ChunkSizeRIFF = totalLen - 8;        //文件的总长度-8bits   应该-4bits?

    pHeader->chWAVE[0] = 'W';
    pHeader->chWAVE[1] = 'A';
    pHeader->chWAVE[2] = 'V';
    pHeader->chWAVE[3] = 'E';

    pHeader->ChunkIDFMT[0] = 'f';
    pHeader->ChunkIDFMT[1] = 'm';
    pHeader->ChunkIDFMT[2] = 't';
    pHeader->ChunkIDFMT[3] = ' ';

    pHeader->ChunkSizeFMT = 0x0010;//一般情况下Size为16，如果为18则最后多了2个字节的附加信息
    pHeader->FmtInfo.nFormatTag = 0x0001;//编码方式
    pHeader->FmtInfo.nChannels = m_channels; //1为单通道，2为双通道
    pHeader->FmtInfo.nSamplesPerSec = m_samplefreq;  //=44.1KHz
    pHeader->FmtInfo.nBytesPerSec = m_samplefreq * m_channels * m_channelbits / 8;//每秒所需字节数
    pHeader->FmtInfo.nBytesPerSample = m_channels * m_channelbits / 8;//一个采样的字节数
    pHeader->FmtInfo.nBitsPerSample = m_channelbits;//16位，即设置PCM的方式为16位立体声(双通道)

    pHeader->ChunkIDDATA[0] = 'd';
    pHeader->ChunkIDDATA[1] = 'a';
    pHeader->ChunkIDDATA[2] = 't';
    pHeader->ChunkIDDATA[3] = 'a';
    pHeader->ChunkSizeDATA = totalLen - WAVE_HEAD_LENGTH;//数据的长度，=文件总长度-头长度(44bit)

    char *pWaveBuffer = new(std::nothrow) char[totalLen]; //音频数据
    memcpy(pWaveBuffer, pHeader, WAVE_HEAD_LENGTH);

    MakeWaveData(pHeader->FmtInfo.nSamplesPerSec, freq, volume, pWaveBuffer+ WAVE_HEAD_LENGTH, m_samplefreq*durations);//采样点数

#if 1
    ofstream ocout;
    ocout.open("C:\\newWave.wav", ios::out | ios::binary);//以二进制形式打开文件
    if (ocout)
        ocout.write(pWaveBuffer, totalLen);
    else
        return 0;
    ocout.close();
#endif

    delete(pHeader);
    return 1;
}
int main_test()
{
    if (Create(500, 100, 10*1))
        cout << "创建成功！" << endl;
    else
        cout << "创建失败！" << endl;
    return 0;
}





WavCodec::WavCodec(void)
{
    fp=NULL;
    pBuf=NULL;                 
    pBuf2=NULL;                     
    channels=0;  
    samplerate=0; 
    filesize=0;
    memset(&header,0,sizeof(header));
}

WavCodec::~WavCodec(void)
{
    if (fp)
    {
        fclose(fp);
    }
    if (pBuf)
    {
        delete[] pBuf;
        pBuf=NULL;
    }
    if (pBuf2)
    {
        delete[] pBuf2;
        pBuf2=NULL;
    }
}

bool WavCodec::OpenFile(const char* filename)
{
    fp=fopen(filename,"rb");
    if (fp==NULL)
    {
        return false;  
    }
    filesize=getFileSize(filename);
    if (filesize<=sizeof(WaveHeader))   //文件太小肯定不是完整文件
    {
        return false;
    }

    size_t nread=fread(&header,1,sizeof(WaveHeader),fp);
    //CheckWaveHead(header);

    return nread==sizeof(WaveHeader);
}


bool CheckWaveHead(const WaveHeader& header,WavInfo& wavInfo)
{
    //header.chRIFF[0] = 'R';
    //header.chRIFF[1] = 'I';
    //header.chRIFF[2] = 'F';
    //header.chRIFF[3] = 'F';
//    unsigned int* pRIFF=(unsigned int*)&header.ChunkIDRIFF[0];
//    if (*pRIFF!=('RIFF')&&*pRIFF!=('FFIR'))
//    {
//        return false;
//    }

    const char* p=header.ChunkIDRIFF;
    if(p[0]!='R'||p[1]!='I'||p[2]!='F'||p[3]!='F')
    {
        return false;
    }


    unsigned int RIFFLen=header.ChunkSizeRIFF;// = totalLen - 8;//文件的总长度-8bits

    //header.chWAVE[0] = 'W';
    //header.chWAVE[1] = 'A';
    //header.chWAVE[2] = 'V';
    //header.chWAVE[3] = 'E';
//    unsigned int* pchWAVE=(unsigned int*)&header.chWAVE[0];
//    if (*pchWAVE!='WAVE'&&*pchWAVE!='EVAW')
//    {
//        return false;
//    }

    p=header.chWAVE;
    if(p[0]!='W'||p[1]!='A'||p[2]!='V'||p[3]!='E')
    {
        return false;
    }


    //header.chFMT[0] = 'f';
    //header.chFMT[1] = 'm';
    //header.chFMT[2] = 't';
    //header.chFMT[3] = ' ';

//    unsigned int* pchFMT=(unsigned int*)&header.ChunkIDFMT[0];
//    if (*pchFMT!='fmt '&&*pchFMT!=' tmf')
//    {
//        return false;
//    }
    p=header.ChunkIDFMT;
    if(p[0]!='f'||p[1]!='m'||p[2]!='t'||p[3]!=' ')
    {
        return false;
    }


    //header.dwFMTLen = 0x0010;//一般情况下Size为16，如果为18则最后多了2个字节的附加信息

    if(header.FmtInfo.nFormatTag!=0x0001)// = 0x0001;   //编码方式01为pcm格式
    {
        PRINTF("Error at: WavCodec::CheckWaveHead() FormatTag %d\n",header.FmtInfo.nFormatTag);
        return false;
    }

    //声道数量一般不会大于2
    if(header.FmtInfo.nChannels>2)// = m_channels; //1为单通道，2为双通道
    {
        PRINTF("Error at: WavCodec::CheckWaveHead() Channels out of range %d\n",header.FmtInfo.nChannels);
        return false;
    }

    //enum Samples
    //{
    //	Samples
    //};
    //有效的采样率
     unsigned int ValidSampleArray[]=
    {
        8000,
        8000,
        11025,
        22050,
        32000,
        44100,
        47250,
        48000,
        50000,
        96000,
        192000,
        384000
    };

    ////暂时只支持48k
    //unsigned int ValidSampleArray[]=
    //{
    //    48000,
    //    96000
    //};

    unsigned int SampleRate=header.FmtInfo.nSamplesPerSec;// = m_samplefreq;  //=44.1KHz
    bool isValidSampleRate=false;
    for (int n=0;n<sizeof(ValidSampleArray)/sizeof(ValidSampleArray[0]);n++)
    {
        if (SampleRate==ValidSampleArray[n])
        {
            isValidSampleRate=true;
            break;
        }
    }

    if (isValidSampleRate==false)
    {
        PRINTF("Error at: WavCodec::CheckWaveHead() SamplesPerSec=%u\n",SampleRate);
        return false;
    }

    //header.pwf.wf.nAvgBytesPerSec=m_samplefreq*m_channels*m_channelbits/8;    //每秒所需字节数
    //header.pwf.wf.nBlockAlign=m_channels * m_channelbits/8;    //一个采样的字节数
    //header.pwf.wBitsPerSample=m_channelbits;                           //16位，即设置PCM的方式为16位立体声(双通道)

    //大小端情况考虑待续
    wavInfo.FormatTag=header.FmtInfo.nFormatTag;
    wavInfo.Channels=header.FmtInfo.nChannels;
    wavInfo.SampleRate=header.FmtInfo.nSamplesPerSec;
    wavInfo.PerSampleBits=header.FmtInfo.nBitsPerSample;
    wavInfo.PerSampleBytes=header.FmtInfo.nBitsPerSample/8;
    wavInfo.PerChSamples=header.ChunkSizeDATA/wavInfo.PerSampleBytes;
    wavInfo.rawPcmSize=header.ChunkSizeDATA;


    //pHeader->chDATA[0] = 'd';
    //pHeader->chDATA[1] = 'a';
    //pHeader->chDATA[2] = 't';
    //pHeader->chDATA[3] = 'a';
//    unsigned int* pchDATA=(unsigned int*)&header.ChunkIDDATA[0];
//    if (*pchDATA!='data'&&*pchDATA!='atad')
//    {
//        return false;
//    }

    p=header.ChunkIDDATA;
    if(p[0]!='d'||p[1]!='a'||p[2]!='t'||p[3]!='a')
    {
        return false;
    }

    return true;
}


WavReader::WavReader(void)
{
    filesize=0;
    //pBuf=NULL;
    //pBuf2=NULL;
    //channels=0;
    //SampleRate=0;
    //PerChSamples=0;

    for (int n=0;n<Ch_Max;n++)
    {
        pData[n]=NULL;
    }

    posGet=0;
}


WavReader::~WavReader(void)
{
    this->Close();
}

bool WavReader::OpenFile(const char* filename)
{

        FILE* fp=::fopen(filename,"rb");
        if (fp==NULL)
        {
            return false;  
        }
        filesize=getFileSize(filename);

        WaveHeader header;
        if (filesize<sizeof(WaveHeader))   //文件太小肯定不是完整文件
        {
            ::fclose(fp);
            return false;
        }
        size_t wantRead=sizeof(WaveHeader);
        size_t nRead=::fread(&header,1,sizeof(WaveHeader),fp);
        if (nRead!=wantRead)
        {
            ::fclose(fp);
            return false;
        }
        if (!CheckWaveHead(header,wavInfo))
        {
            return false;
        }

        //加载pcm数据到内存,暂不支持大于100m的数据
        if (wavInfo.rawPcmSize>50*1024*1024)
        {
            ::fclose(fp);
            return false;
        }

        char* pRawPcm=new char[wavInfo.rawPcmSize];
        if (pRawPcm==NULL)
        {
            ::fclose(fp);
            return false;
        }

        wantRead=wavInfo.rawPcmSize;
        nRead=::fread(pRawPcm,1,wantRead,fp);
        if (nRead!=wantRead)
        {
            ::fclose(fp);
            delete pRawPcm;
            return false;
        }

        for (unsigned int ch=0;ch<wavInfo.Channels;ch++)
        {
            pData[ch]=new int[wavInfo.PerChSamples];
        }

        if (!FillPerChSamples(pData,pRawPcm,wavInfo.Channels,
            wavInfo.PerChSamples,wavInfo.PerSampleBytes))
        {
            ::fclose(fp);
            delete pRawPcm;
            return false;
        }

        posGet=0;
        fclose(fp);
        return true;
}


void WavReader::Close()
{
    for (int n=0;n<Ch_Max;n++)
    {
        if (pData[n]!=NULL)
        {
            delete pData[n];
            pData[n]=NULL;
        }
        pData[n]=NULL;
    }
    //PerChSamples=0;
}


bool WavReader::getFrame(int* pcmOut,unsigned int samples,bool* isTail)
{
    //复位是否最尾帧标记
    if (isTail)
    {
        *isTail=false;
    }
    unsigned int PerChSamplesMax=wavInfo.PerChSamples;

    if ((posGet+samples)>PerChSamplesMax)  //复位0，数据仍然不够
    {
        PreDprintf("failed! posGet:%u samples:%u wavInfo.PerChSamples:%u\n",posGet,samples,PerChSamplesMax);
        posGet=0;
        ::memset(pcmOut,0,samples*sizeof(int));
        return false;
    }
    int* pCurr=pData[0]+posGet;  //只取其中一个声道
    for (unsigned int n=0;n<samples;n++)
    {
        pcmOut[n]=pCurr[n];
    }
    if ((posGet+samples)>=PerChSamplesMax)
    {
        //PreDprintf("failed! posGet:%u samples:%u wavInfo.PerChSamples:%u\n",posGet,samples,PerChSamplesMax);

        posGet=0;
        if (isTail)
        {
            *isTail=true;
        }
        //return false;
    }
    else
    {
        posGet+=samples;
    }
    return true;
}



void WavReader::resetPos()
{
    posGet=0;
}

bool WavReader::savePerChToFile()
{
    for (int n=0;n<Ch_Max;n++)
    {
        if (pData[n]!=NULL)
        {
            char FileName[128];
            sprintf(FileName,"TestCh%02d.pcm",n);
            FILE* fp=::fopen(FileName,"wb");
            if (fp==NULL)
            {
                return false;  
            }
            size_t wantWrite=wavInfo.PerChSamples*sizeof(int);
            size_t nWritten=fwrite(pData[n],1,wantWrite,fp);
            fclose(fp);
            if (wantWrite!=nWritten)
            {
                return false;
            }
        }
    }
    return true;
}


bool WavReader::FillPerChSamples(int* pData[Ch_Max],char* pcmRaw ,uint32_t Chs,uint32_t PerChSamples,uint32_t PerSampleBytes)
{
    //int* pD=pData
    switch (PerSampleBytes)
    {
    case 1:
        {
            char* pS=pcmRaw;
            for (uint32_t j=0;j<PerChSamples;j++)
            {
                for (uint32_t ch=0;ch<Chs;ch++)
                {
                    int tmp=*(pS++);
                    tmp<<=24;
                    pData[ch][j]=tmp;
                }
            }
        }
        break;
    case 2:
        {
            short* pS=(short*)pcmRaw;
            for (uint32_t j=0;j<PerChSamples;j++)
            {
                for (uint32_t ch=0;ch<Chs;ch++)
                {
                    int tmp=*(pS++);
                    tmp<<=16;
                    pData[ch][j]=tmp;
                }
            }
        }
        break;
    case 3:
        {
            Int24* pS=(Int24*)pcmRaw;
            for (uint32_t j=0;j<PerChSamples;j++)
            {
                for (uint32_t ch=0;ch<Chs;ch++)
                {
                    int tmp=*(pS++);
                    tmp<<=8;
                    pData[ch][j]=tmp;
                }
            }
        }
        break;
    case 4:
        {
            int* pS=(int*)pcmRaw;
            for (uint32_t j=0;j<PerChSamples;j++)
            {
                for (uint32_t ch=0;ch<Chs;ch++)
                {
                    int tmp=*(pS++);
                    //tmp<<16;
                    pData[ch][j]=tmp;
                }
            }
        }
        break;
    default:
        return false;
        break;
    }

    return true;
}






#if defined(UNIT_TEST)

void Test_AudioCodec()
{
    //main_test();
    //getchar(); //system("pause");不跨平台

    //WavReader wR;
    //if(!wR.OpenFile("pinkNew.wav"))
    //{
    //    return;
    //}
    //wR.savePerChToFile();

    SignalGenaratorSimple<SineWave> sg(48000,4);
    //SignalGenaratorSimple<TriangleWave> sg(48000,4);
    //SignalGenaratorSimple<SawtoothWave> sg(48000,4);
    //SignalGenaratorSimple<SquareWave> sg(48000,4);

    int pcm[2000];

    FILE* fp=::fopen("test.pcm","wb");
    for (int n=0;n<24;n++)
    {
        //memset(pcm,0,sizeof(pcm));


        sg.setFreq(4*n+1);
        for (int k=0;k<24;k++)
        {
            sg.getFrame(pcm,2000,0.5);
            fwrite(pcm,sizeof(int),2000,fp);
        }

    }
    ::fclose(fp);
}

#endif




NAME_SPACE_MAIN_END


