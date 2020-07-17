#include "SuperTime.h"
#include <stdio.h>
//#include <iostream>
#include <memory.h>


#if defined(OS_WIN)
   #include <windows.h>  //高精度计时头文件
#endif


//跨平台的获取时间戳【win32和linux】
//工作需要，就写了个时间戳获取方法，主要针对Win32和Linux，理论iOS只要使用code编译出来的静态库即可
//头文件  
#include <time.h>  
#include <stdio.h>  
#ifdef _WIN32  
#include <sys/timeb.h>  
#else  
#include <sys/time.h>  
#endif  


//#include< cfloat>
#undef  max
#undef  min
#include <limits>  //max min 宏定义重复,编译错误


NAME_SPACE_MAIN_BEGIN

StuDateTime getDateTime()
{
    time_t timep;
    struct tm *p;
    time(&timep);
    //p=gmtime(&timep);  //old 取得的hour需要+8
    p=localtime(&timep); 

    //printf("%d\n",p->tm_yday); /*从今年1月1日算起至今的天数，范围为0-365*/
    //printf("Year:   %d\n",1900+p->tm_year);/*获取当前年份,从1900开始，所以要加1900*/
    //printf("Month: %d\n",1+p->tm_mon);/*获取当前月份,范围是0-11,所以要加1*/
    //printf("Day:  %d\n",p->tm_mday);/*获取当前月份日数,范围是1-31*/
    //printf("Hour: %d\n",p->tm_hour);/*获取当前时,这里获取西方的时间,刚好相差八个小时*/   //old 8+p->tm_hour
    //printf("Min:  %d\n",p->tm_min); /*获取当前分*/
    //printf("Sec:  %d\n",p->tm_sec); /*获取当前秒*/

    //printf("Data:%04d-%02d-%02d Time:%02d:%02d:%02d\n",1900+p->tm_year,1+p->tm_mon,
    //    p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

    StuDateTime tm;
    tm.year=1900+p->tm_year;
    tm.mouth=1+p->tm_mon;
    tm.day=p->tm_mday;
    tm.hour=p->tm_hour;
    tm.min=p->tm_min;
    tm.sec=p->tm_sec;
    return tm;
}

StuDate getDate()
{
    StuDateTime tm=getDateTime();
    return tm;
}

StuTime getTime()
{
    StuDateTime tm=getDateTime();
    return tm;
}



void print_time()
{
    //功能实现  
    //get the timestamp  
    time_t tt = time(NULL);  
    struct tm* ptr;  
    ptr = localtime(&tt);    //C语言localtime()函数：获取当前时间和日期并转换为本地时间
    printf("time: %d \n", tt);  
    char str[80];  
    strftime(str, sizeof(str),"%Y-%m-%d %H:%M:%S",ptr);  

    #ifdef _WIN32  
        struct  timeb   tp;  
        ftime(&tp);  
        printf("now time: %s.%03d \n", str, tp.millitm);  
    #else  
        struct timeval tmv;  
        gettimeofday(&tmv, NULL);  
        printf("now time: %s.%03d \n", str, tmv.tv_usec/1000);  
    #endif  
}



StuDateTime StuDateTime::getDate()
{
    time_t timep;  
    struct tm *p;  
    ::time(&timep);  
    p =::localtime(&timep);     //此函数获得的tm结构体的时间，是已经进行过时区转化为本地时间  
    //p = gmtime(&timep); //把日期和时间转换为格林威治(GMT)时间的函数  

    //printf("Year:  %d\n", 1900+p->tm_year);  
    //printf("Month:  %d\n", 1+p->tm_mon);  
    //printf("Day:  %d\n", p->tm_mday);  
    //printf("Hour:  %d\n", p->tm_hour);  
    //printf("Minute:  %d\n", p->tm_min);  
    //printf("Second:  %d\n",  p->tm_sec);  
    //printf("Weekday:  %d\n", p->tm_wday);  
    //printf("Days:  %d\n", p->tm_yday);  
    //printf("Isdst:  %d\n", p->tm_isdst);  

    StuDateTime Curr;
    Curr.year=1900+p->tm_year;
    Curr.mouth=1+p->tm_mon;
    Curr.day=p->tm_mday;
    Curr.hour=p->tm_hour;
    Curr.min=p->tm_min;
    Curr.sec=p->tm_sec;
    return Curr;
}

SuperTime::SuperTime(const char* name)
{
    setName(name);
    reSet();
}

SuperTime::~SuperTime()
{

}

void SuperTime::setName(const char* name)
{
    memset(this->name,0,sizeof(this->name));
    if (name!=NULL)
    {
        strcpy(this->name,name);
    }
    //this->name[127]=0;
    this->name[sizeof(this->name)-1]='\0';
}


void SuperTime::SecondsToStringTime(uint64_t Sec,char buf[32])
{
    unsigned int hour=Sec/3600;
    unsigned int min=(Sec%3600)/60;
    unsigned int sec=Sec%60;
    if (hour<24)
    {
         sprintf(buf,"%02d:%02d:%02d",hour, min,sec);
    }
    else
    {
        unsigned int day=hour/24;
        hour=hour%24;
         sprintf(buf,"%d day:%02d:%02d:%02d",day,hour, min,sec);
    }
}

uint64_t SuperTime::getHighResolutionTicks()
{
#if defined(OS_WIN)
    LARGE_INTEGER Ltmp;
    QueryPerformanceCounter(&Ltmp);   //查询定时器的计数值
    return Ltmp.QuadPart;
#else
    timespec t;
    clock_gettime(CLOCK_MONOTONIC,&t);
    return (t.tv_sec*1000000)+(t.tv_nsec/1000);
#endif
}


uint64_t SuperTime::getHighResolutionTicksPerSecond_Win()
{
#if defined(OS_WIN)
    LARGE_INTEGER Ltmp;
    //获得机器内部计时器的时钟频率,机器启动之后就会是固定的,不会因为当前CPU频率发生变化
    //原型：BOOL QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency);
    //作用：返回硬件支持的高精度计数器的频率。返回零。硬件不支持。读取失败。//返回值判断与处理待续

    QueryPerformanceFrequency(&Ltmp);    
    //printf("QueryPerformanceFrequency():%llu\n",Ltmp.QuadPart);   //win10 I5-7400 returns 10000000
    return Ltmp.QuadPart; 
#else  
    return 1000000;      //defined(OS_LINUX) // (microseconds) 微妙
#endif
}


void SuperTime::reSet()
{
    begin=end=last=current=0;
    clocks=0;
    CallCounter=0;
    usedTime=0.0;
    rate=0.0;
    SumTime=0.0;
    SumTimeInTicks=0;
    averageTime=0.0;
    peakUsedTime=0.0;
    maxUsedTime=0.0;
    averageRate=0.0;
    isFirstRecall=true;
}


double SuperTime::ComputeTime()
{
    CallCounter++;

    clocks =end-begin;   
    //两次取时间间隔太近导致end==begin的考虑,此后运算结果没有意义
    if (clocks==0)
    {
        //printf("clocks==0\n");
        //printf("operation failed! sampling interval is too small\n");

        usedTime=0;
        //rate=std::numeric_limits<double>::max();   //DBL_MAX;  //gcc 4.8.5 error
        rate=-1; //理论上usedTime=0,rate=∞;没有意义,会打印一个巨大的数刷屏,所以设为-1

        return 0.0;
    }

    uint64_t ClockFreq=getClockFreq();
    usedTime =clocks/(double)ClockFreq;                   //转成以秒为单位的时间
    rate=1.0/usedTime;
    SumTime+=usedTime;
    SumTimeInTicks+=clocks;

    //Counter可能会溢出等于零导致除零异常崩溃--但是用64位寄存器一般到不了,暂时无需考虑
    averageTime=SumTimeInTicks/((double)ClockFreq*CallCounter);      //用原始ticks计算时间平均值应该更精确
    averageRate=1.0/averageTime;                //平均帧率

    if (usedTime>peakUsedTime)
    {
         peakUsedTime=usedTime;
    }

    if (usedTime>maxUsedTime)
    {
        maxUsedTime=usedTime;
    }
    //printf("begin=%lld end=%lld Time:=%f s;Cpu频率=%lld;Clk=%lld;帧率F=%f HZ 平均F=%f HZ\n",begin,end,useTime,freq, SumClock, FrameRate, avrgRate);
    return usedTime;
}


double SuperTime::printTime(unsigned int PintFreq/*=1*/,bool newLine)
{
    if (CallCounter%PintFreq==0)    //间隔一定频率打印
    {
       //依次为耗时/帧率 打印间隔的峰值耗时 最大耗时 调用次数 平均耗时 平均帧率

        printf("[Time]:%.3fms=%.3fHz;peak:%.2fms;max:%.2fms;%llu次平均%.9fms=%.9fHz %s", 
            usedTime*1000,rate,peakUsedTime*1000,maxUsedTime*1000,CallCounter,averageTime*1000,averageRate,name);

        peakUsedTime=0;   
        if (newLine)
        {
            printf("\n");
        }
        else
        {
           printf("\r");
        }
    }
    return usedTime;  //useTime
}


bool SuperTime::getReCallTime()
{
    current=getHighResolutionTicks();
    //首次调用判断 刷新last time，放在构造函数处不精确
    if (isFirstRecall==true)
    {
        last=current;
        SumTime=0;
        SumTimeInTicks=0;
        isFirstRecall=false;
        return false;
    }
    return true;
}

double SuperTime::ComputeReCallTime()
{
    CallCounter++;

    clocks =current-last;   //间隔太小 SumClock可能=0
    if (clocks==0)
    {
        usedTime=0;
        //rate=std::numeric_limits<double>::max();   //DBL_MAX;  //gcc 4.8.5 error
        rate=-1; //理论上usedTime=0,rate=∞;没有意义,会打印一个巨大的数刷屏,所以设为-1
        return 0.0;
    }

    uint64_t ClockFreq=getClockFreq();
    usedTime =clocks /(double)ClockFreq;
    rate =1.0/usedTime;
    SumTime +=usedTime;
    SumTimeInTicks+=clocks;

    //Counter可能会溢出等于零导致除零异常崩溃--但是用64位寄存器一般到不了,暂时无需考虑
    averageTime = (double)SumTimeInTicks/(ClockFreq*CallCounter);      //用原始ticks计算时间平均值应该更精确
    averageRate=1.0/averageTime;            //平均帧率

     if (usedTime>peakUsedTime)
    {
         peakUsedTime=usedTime;
    }

     if (usedTime>maxUsedTime)
     {
         maxUsedTime=usedTime;
     }

    return usedTime;
}

void SuperTime::printReCallTime(unsigned int PintFreq /*= 1*/,bool newLine)
{
    if (!getReCallTime())    //第一次直接返回
    {
        return;
    }

    ComputeReCallTime();     
    if (CallCounter%PintFreq==0)    //间隔一定频率打印
    {
       //依次为耗时/帧率 打印间隔的峰值耗时 最大耗时 调用次数 平均耗时 平均帧率
        printf("[ReCall]:%.3fms=%.3fHz;peak:%.2fms;max:%.2fms;%llu次平均%.9fms=%.9fHz %s", 
            usedTime*1000,rate,peakUsedTime*1000,maxUsedTime*1000,CallCounter,averageTime*1000,averageRate,name);

        peakUsedTime=0;   

        if (newLine)
        {
            printf("\n");
        }
        else
        {
            printf("\r");
        }
    }
    last=current;
}



#if defined(UNIT_TEST)

USING_NAME_SPACE_MAIN

int Test_SuperTime()
{

    //SuperTime使用方法，传
    static SuperTime Time("Test");
    Time.getBeginTime();
    //std::cout<<"test print"<<std::endl;
    Time.getEndTime();
    Time.printTime();


    //使用方法2--用来记录前后两次调用某个函数的间隔
    //static SuperTime Time("Test");
    //Time.printReCallTime(24);

    //piint_time();
    return 0;
}

#endif


NAME_SPACE_MAIN_END

