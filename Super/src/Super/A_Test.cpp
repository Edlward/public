#include "A_Test.h"

#include "./Basic/BasicOperation.h"
#include "./Basic/bitio.h"
#include "./Basic/Test_Basic.h"
#include "./Basic/MyCLib.h"
#include "./Basic/Temp.h"
#include "./Basic/SmartPointer.h"
#include "./Basic/AudioCodec.h"
#include "./Basic/Thread.h"
#include "./Basic/Noise.h"
#include "./Basic/SuperTime.h"
#include "./Basic/PerformanceTest.h"
#include "./Basic/SIMD.h"
#include "./Basic/PID.h"
//#include "Atomic.h"
//#include "ScopedPointer.h"
#include "./Basic/Int24.h"
#include "./Basic/ringbuffer_Test.h"
#include "./Basic/Timer.h"
#include "./Basic/MemoryPool.h"
//#include "MemPoolFast.h"
#include "./Basic/FIFO.h"
//#include "./TestCPP/Test_Cast.h"
#include "./Basic/MyCLib.h"
#include "./DataStruct/Heap.h"
#include "./Frame/ThreadFrame_Test.h"   //各种线程加速框架



#ifdef UNIT_TEST

#include "Basic/SuperTime.h"

int Test_Int24()
{
    SuperTime tm("Test_Int24()");
    Int24 arryA[4000];
    Int24 arryB[4000];

    int ret[4000];
    int sum=0;
    for (int T=0;T<100;T++)
    {
        for (unsigned int k=0;k<4000;k++)
        {
            arryA[4000]=MyRand();
            arryB[4000]=MyRand();
        }
        tm.getBeginTime();
        for (unsigned int n=0;n<100*1000;n++)
        {
            //for (unsigned int k=0;k<4000;k++)
            int tn=3990+MyRand()%1;
            int rv=MyRand();
            for (unsigned int k=0;k<tn;k++)
            {
                //ret[k]=arryA[k]+arryB[k];
                arryA[k]=rv;
                sum+=(int)arryA[k];
            }
        }
        tm.getEndPrint();
        printf("ret:%d\n",ret);
    }
    return 0;
}
#endif




NAME_SPACE_MAIN_BEGIN

//TODO:
//打印系统相关信息
void printSystemInfo()
{

    printf("已运行:%d day| time:%02d:%02d%02d\n");
    printf(">>>系统负载<<<\n");
    printf(">>>运行时间<<<\n");
    printf(">>>版本信息<<<\n");
    printf(">>>磁盘信息<<<\n");
    printf(">>>上下文切换<<<\n");
    printf(">>>线程数目<<<\n");
}

//打印性能测试
void printPeformanceTest(int times=100)  //times测试次数求平均值
{
    //整形加减乘除
    //浮点数加减乘除
    //内存拷贝效率
    //内存申请效率，低中高分块大小
}



#ifdef UNIT_TEST

int Test_SuperLibrary()
{
    printf("Test_SuperLibrary()\n");

    //Test_Cast();
    //Test_Global();
    //Test_publicBasic();
    //Test_BitIO();
    //Test_ringbuffer();
    //Test_Timer();
    //Test_Temp();
    //Test_SmartPtr();
    //Test_AudioCodec();
    //Test_Thread();
    //Test_Noise();
    //Test_SuperTime();
    //Test_MyClib();
    //Test_PerformanceTest();
    //Test_SIMD();
    //Test_Math();
    //Test_Condition();
    //Test_PID();
    //Test_Int24();
    //Test_MemoryPool();
    //Test_MemPoolFast();
    //Test_FIFO();
    //Test_Heap();
    Test_ThreadSync();
    //Test_ThreadFrame();

    return 0;
}

#endif


NAME_SPACE_MAIN_END




#define PRINT_SIZE(type) printf("%-17s = %zu,inner type: %s\n", "sizeof("#type")", sizeof(type), typeid(type).name())

void print_sys_bits()
{
#ifdef _WIN64
    std::cout << "x64:" << std::endl;
#else
    std::cout << "x86:" << std::endl;
#endif
}

//数据类型打印测试
void print_sizes()
{
    PRINT_SIZE(void*);
    PRINT_SIZE(float);
    PRINT_SIZE(double);
    PRINT_SIZE(short);
    PRINT_SIZE(int);
    PRINT_SIZE(long);
    PRINT_SIZE(int64_t);
    PRINT_SIZE(int8_t);
    PRINT_SIZE(uint8_t);
    PRINT_SIZE(int16_t);
    PRINT_SIZE(uint16_t);
    PRINT_SIZE(int32_t);
    PRINT_SIZE(uint32_t);
    PRINT_SIZE(int64_t);
}
