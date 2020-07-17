#include  <time.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <iostream>

#include "Test_list.h"

#include "Super/Basic/Thread.h"
#include "Super/Basic/SuperTime.h"
#include "Super/Basic/Test_Basic.h"
#include "Super/A_Test.h"
#include "Super/Basic/BasicOperation.h"
#include "TestTmp.h"
//#include "TestTmp2.h"

//VS windows 下单句printf语句是加锁的，一个线程没有打印完成不会被另一个线程切换。
void testp(const char* pstr)
{
    while (true)
    {
        printf("%s\n",pstr);
    }
}

void testPrintf()
{
    Thread::startThread(testp,"thread1");
    Thread::startThread(testp,"thread2");
    Thread::Handle hnd=Thread::startThread(testp,"thread3");
    Thread::waitFinish(hnd);
}


#ifdef _WIN32
//临界区一个锁住时可以被另一个摧毁
#include <windows.h>
RTL_CRITICAL_SECTION cs;
void testm1(const char* pstr)
{
        PRINT_FUN_TRACE
        EnterCriticalSection(&cs);
        //LeaveCriticalSection(&cs);
}

void testm2(const char* pstr)
{
    PRINT_FUN_TRACE
    Thread::sleep_for(100);
    //EnterCriticalSection(&cs);
    //LeaveCriticalSection(&cs);


    unsigned int cnt=0;
    while (true)
    {
        BOOL res=TryEnterCriticalSection(&cs);
        if (res!=TRUE)
        {
            printf("TryEnterCriticalSection() failed Cnt:%u\n",++cnt);
            Thread::sleep_for(50);

            if (cnt>10)
            {
                    DeleteCriticalSection(&cs);
                    printf("DeleteCriticalSection() Cnt:%u\n",++cnt);
                    InitializeCriticalSection(&cs);
                    printf("InitializeCriticalSection()\n");
                    break;
            }
        }
    }
    DeleteCriticalSection(&cs);
    printf("DeleteCriticalSection\n");

    InitializeCriticalSection(&cs);
    printf("InitializeCriticalSection\n");

    EnterCriticalSection(&cs);
    LeaveCriticalSection(&cs);
}


void testMutex()
{
    InitializeCriticalSection(&cs);

    Thread::startThread(testm1,"thread2");
    Thread::Handle hnd=Thread::startThread(testm2,"thread3");
    Thread::waitFinish(hnd);

    DeleteCriticalSection(&cs);
}

#endif


static SuperTime tm("AlignWave");

//#include <emmintrin.h>   //This is SSE for double
//#include <immintrin.h>    //This is SSE for float

#include <stdint.h>
static FILE* fp=fopen("result.txt","w+");
void FindWaveMatchPos(int* Dst,int* src,int dstLen,int srclen)
{

    int64_t sumDiff=0;
    int64_t diffmin=INT64_MAX;
    int diffminpos=0;
    int cmpLen=dstLen;
    //int cmpoffsetBegin=7*24*2000;//dstLen/20;
    //int cmpoffsetEnd=15*24*2000;//dstLen/20;
    int cmpoffsetBegin=5*24*2000;//dstLen/20;
    //int cmpoffsetBegin=336000;//dstLen/20;
    int cmpoffsetEnd=10*24*2000;//dstLen/20;

    PreDprintf("dstlen:%d srclen:%d cmpoffsetMax:%d\n",dstLen,srclen,cmpoffsetEnd);

    int cnt=0;
    for (int d=cmpoffsetBegin;d<cmpoffsetEnd;d+=1)   //d+=200
     {
        if ((cmpLen+d)>=srclen)
         {
             break;
         }

        sumDiff=0;
        int* psrc=src+d;
        tm.getBeginTime();
        for (int k=0;k<cmpLen;k+=4)
        {
            //int diff=abs(Dst[k]-psrc[k]);
            //sumDiff+=diff;

            unsigned int diff1=abs(Dst[k]-psrc[k]);
            unsigned int diff2=abs(Dst[k+1]-psrc[k+1]);
            unsigned int diff3=abs(Dst[k+2]-psrc[k+2]);
            unsigned int diff4=abs(Dst[k+3]-psrc[k+3]);
            sumDiff+=(diff1+diff2+diff3+diff4);

        }
        if (sumDiff<diffmin)
        {
            diffmin=sumDiff;
            diffminpos=d;
            printf("diffmin:%lld pos:%d\n",diffmin,diffminpos);
     }
        tm.getEndPrint(128);
        if(cnt++%128==127)
        {
            printf("offset:max%d curr:%d percent:%f\n",cmpoffsetEnd,d,d/(double)cmpoffsetEnd);
        }
    }
    printf("diff min:%lld pos:%d\n",diffmin,diffminpos);
    fprintf(fp,"diff min:%lld pos:%d\n",diffmin,diffminpos);
    fflush(fp);
    fclose(fp);
}

void ReadWaveToBuf()
{
    PRINT_FUN_NAME

    //const char* file1="F:\\frozen2_5.1_ch.A1.snd";
    //const char* file2="F:\\frozen2_HOLOSOUND_CH1.snd";

    const char* file1="F:\\pirate_5.1_ch.1.snd";
    const char* file2="F:\\pirate_HOLOSOUND.L.snd";


    size_t len1=getFileSize(file1);
    size_t len2=getFileSize(file2);
    
    size_t pcmlen1=len1/sizeof(int16);
    size_t pcmlen2=len2/sizeof(int16);
    int16* pcmraw1=new int16[pcmlen1];
    int16* pcmraw2=new int16[pcmlen2];
    int32* pcm1=new int32[pcmlen1];
    int32* pcm2=new int32[pcmlen2];

    readFiletoBuf(file1,(char*)pcmraw1,len1);
    readFiletoBuf(file2,(char*)pcmraw2,len2);

    for (int k=0;k<pcmlen1;k++)
    {
        pcm1[k]=pcmraw1[k];
    }
    for (int k=0;k<pcmlen2;k++)
    {
        pcm2[k]=pcmraw2[k];
    }
    FindWaveMatchPos(pcm1,pcm2,pcmlen1,pcmlen2);
    delete[] pcmraw1;
    delete[] pcmraw1;
    delete[] pcm1;
    delete[] pcm2;
}






int main(int argc, char* argv[])
{
    //ReadWaveToBuf();

    //testMutex();
    //Test_SuperLibrary();
    //Test_publicBasic();
    Test_Tmp();
    //Test_Tmp2();

    getchar(); //system("pause");不跨平台
    return 0;
}



