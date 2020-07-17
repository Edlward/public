#include "TestTmp.h"
#include <stdio.h>
#include <stdlib.h>
#include "Super/A_Test.h"
#include "Super/Basic/global.h"
#include "Super/Basic/Math.h"
#include "Super/Basic/BasicOperation.h"


//#include "Super/Basic/Test_Basic.h"
#include "Super/A_Test.h"
#include "Super/Basic/BasicOperation.h"
#include "Super/Basic/SuperTime.h"

#include  <mutex>
#include  <thread>

//#ifdef _MSC_VER
#include <intrin.h>


static bool isSlash(char c)
{
    return (c=='/'||c=='\\');
}

//各种奇怪格式文件路径整理成合法格式,反斜杠统一整理成斜杠
// C:\dir/test//\sub2\\sub3/\sub4  等

void myFormatPath(char* dst,const char* src)
{
    if (isNullCstr(src))
    {
        return;
    }
    unsigned int outIndex=0;
    bool isSlashLast=false;   //上一个是否斜线
    while (1)
    {
        char tmp=*src++;
        if (tmp=='\0')
        {
            break;
        }
         bool bSlash=isSlash(tmp);
        if (tmp=='\\')
        {
            tmp='/';
        }
        bool conSlash=bSlash&&isSlashLast;   //连续斜线
        if (!conSlash)
        {
            dst[outIndex++]=tmp;
        }
        isSlashLast=bSlash;
    }
    dst[outIndex]='\0';
}



int Test_Tmp()
{
    const char* src1="C:\\dir/test//\\sub2\\sub3/\\sub4////SUB5\\SUB6";
    char pd[1024];
    myFormatPath(pd,src1);
    printf("str dst:%s\n",pd);
    return 0;

    //Test_DevideStrategy();

    //unsigned long long ret=2;
    //for (int n=0;n<64;n++)
    //{
    //    //ret*=2;
    //    ret<<=1;
    //    //printf("%llu,",ret);   //%llu
    //    //printf("[%d]:%I64u\n",n,ret);   //%I64d %I64u
    //}
    //for (int n=0;n<ARRAY_NUM(tablepow2_uint32);n++)
    //{
    //    printf("table_uint32[%d]:%llu\n",n,tablepow2_uint32[n]);
    //}
    //for (int n=0;n<ARRAY_NUM(tablepow2_uint64);n++)
    //{
    //        printf("table_uint64[%d]:%llu\n",n,tablepow2_uint64[n]);
    //}

    SuperTime tm("TestTmp");
    const int TmNum=10;
    const int LoopNum=100*1000;
    tm.reSet();
    unsigned int retSum=0;
    for (int T=0;T<TmNum;T++)
    {
        unsigned int nVar=2;
        tm.getBeginTime();
        for (int K=0;K<LoopNum;K++)
        {
            //nVar=MyRand();
            for (unsigned int n=0;n<1*1000;n++)
            {
                //uint32_t h1pos=getHightBit1Pos(n);
                //retSum+=getRightNBit<1>(n,h1pos);
                //retSum+=ByteOrderSwapFloat(n);
                //retSum+=ByteOrderSwap32(n);
                retSum+=ByteOrderSwap32(n);
            }
        }
        tm.getEndPrint();
    }
    tm.printTime();
    printf("ret:%u \n",retSum);   //仅仅避免运算过程被优化掉
    return 0;
}



