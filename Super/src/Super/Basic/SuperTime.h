#ifndef SUPERTIME_H
#define SUPERTIME_H

#include "global.h" 
#include <time.h>  
#include <string>  

NAME_SPACE_MAIN_BEGIN

    //int tm_sec;     /* seconds after the minute - [0,59] */
    //int tm_min;     /* minutes after the hour - [0,59] */
    //int tm_hour;    /* hours since midnight - [0,23] */
    //int tm_mday;    /* day of the month - [1,31] */
    //int tm_mon;     /* months since January - [0,11] */
    //int tm_year;    /* years since 1900 */
    //int tm_wday;    /* days since Sunday - [0,6] */
    //int tm_yday;    /* days since January 1 - [0,365] */
    //int tm_isdst;   /* daylight savings time flag */


    //继续完善时间,日期获取与比较相关类
struct StuTime
{
    unsigned int hour;
    unsigned int min;
    unsigned int sec;
    StuTime()
    {
        hour=min=sec=0;
    }
};

struct StuDate
{
    unsigned int year;
    unsigned int mouth;
    unsigned int day;
    unsigned int dayCnt;   //days of Year;
    StuDate()
    {
        year=0;mouth=0;day=0;
    }
};

struct StuDateTime:public StuTime,public StuDate
{
    //unsigned int year;
    //unsigned int mouth;
    //unsigned int day;
    StuDateTime()
    {
        //   year=0;mouth=0;day=0;
    }
    bool operator==(const StuDateTime& R) const
    {
        if (this->year==R.year&&this->mouth==R.mouth&&this->day==R.day
            &&this->hour==R.hour&&this->min==R.min&&this->sec==R.sec)
        {
            return true;
        }
        return false;
    }
    bool operator>(const StuDateTime& R) const
    {
        //better way 
        //if (this->year<dt.year&&this->mouth<dt.mouth&&this->day<dt.day
        //    &&this->hour<dt.hour&&this->min<dt.min&&this->sec<dt.sec)
        //{
        //    return false;
        //}

        if (this->year<R.year)
        {
            return false;
        }
        if (this->year>R.year)
        {
            return true;
        }
        if (this->mouth<R.mouth)
        {
            return false;
        }
        if (this->mouth>R.mouth)
        {
            return true;
        }
        if (this->day<R.day)
        {
            return false;
        }
        if (this->day>R.day)
        {
            return true;
        }
        if (this->hour<R.hour)
        {
            return false;
        }
        if (this->hour>R.hour)
        {
            return true;
        }
        if (this->min<R.min)
        {
            return false;
        }
        if (this->min>R.min)
        {
            return true;
        }
        if (this->sec<R.sec)
        {
            return false;
        }
        if (this->sec>R.sec)
        {
            return true;
        }
        return true;
    }

    bool operator>=(const StuDateTime& dt) const
    {
        return (*this==dt)||operator>(dt);
    }
    bool operator<=(const StuDateTime& dt) const
    {
        return (*this==dt)||!operator>(dt);
    }
    bool operator<(const StuDateTime& date) const
    {
        return  !(*this==date)&&!operator>(date);
    }
    static StuDateTime getDate();

    std::string toString() const
    {
        char tmp[256];
        sprintf(tmp,"%04d%02d%02d-%02d%02d%02d",year,mouth,day,hour,min,sec);
        return std::string(tmp);
    }
    void print() const
    {
        printf("dateTime:%04d-%02d-%02d ",year,mouth,day);
        printf("%02d:%02d:%02d\n",hour,min,sec);
    }
};


StuDateTime getDateTime();
StuDate getDate();
StuTime getTime();




class BASIC_API SuperTime
{
protected:
    SuperTime(const SuperTime &);
    SuperTime&operator=(const SuperTime &);
public:
    SuperTime(const char* name="");  //用一个name作为打印前缀区分，PintFreq打印间隔
    ~SuperTime();
    void setName(const char* name);
    const char* getName()const {return name;}
    void reSet();                                   //复位所有内部计数状态，开始新的计时

    //返回当前系统计数器到现在的时间，以s为单位
    //static double getCurrentTimes(){return highResolutionTicksToSeconds(getHighResolutionTicks());}
    static double getCurrentTimes(){return getStartupToNowSecs();}

    void getBeginTime(){begin=getHighResolutionTicks();}
    void getEndTime(){end=getHighResolutionTicks();}       //取时间并统计
    double ComputeTime();
    void getEndCompute(){getEndTime();ComputeTime();} //计算时间差--内部使用打印End-begin时间差   同时返回耗时，单位是s

    //频繁调用的函数，间隔一定次数再打印，否则影响程序运行
    //打印End-begin时间差   同时返回耗时，单位是s
    //newLine控制是否换行，为了避免刷屏，可以不换行
    double printTime(unsigned int  PintFreq=1,bool newLine=true);    
    //取结束时间  打印耗时并返回，单位是s
    double getEndPrint(unsigned int PintFreq=1,bool newLine=true)
    {
        getEndCompute();return printTime(PintFreq,newLine);
    }
    bool getReCallTime();                   //取与上次调用的时间间隔,首次返回false
    double ComputeReCallTime();      //计算时间差 

     //打印与上次调用的时间间隔，一般作为static类使用
    void printReCallTime(const unsigned int  PintFreq = 1,bool newLine=true);     
    double getUsedTime()const {return usedTime;}        //取本次Begin到End之间时间
    double getAvrgTime()const {return averageTime;}    //取平均时间
    double getAvrgRate()const {return averageRate;}     //取平均帧率

    //一些常用静态方法获取高精度时间的静态方法   
    static uint64_t getStartupToNow()         //取开机到现在的s数
    {
        return  getHighResolutionTicks()/getClockFreq();
    }
    static double getStartupToNowSecs()      //取开机到现在的s数,高精度
    {
        return  ((double)getHighResolutionTicks())/getClockFreq();
    }
    static double getStartupToNowInms()   //取开机到现在的ms数
    {
        return  1000.0*getHighResolutionTicks()/getClockFreq();
    }
    //ns为单位 开机1000*584年后溢出,精度暂时足够
    static uint64_t getStartupToNowInus()     //取开机到现在的us数  微秒microseconds 
    {
        return  1000*1000*getHighResolutionTicks()/getClockFreq();
    }
    //ns为单位 开机584年后溢出,精度暂时足够
    static uint64_t getStartupToNowInns()     //取开机到现在的ns数  纳秒nanosecond
    {
        return  1000*1000*1000*getHighResolutionTicks()/getClockFreq();
    }

    static double usToms(uint64_t  us){return us/1000.0;}                   //ns转ms
    static double usToSeconds(uint64_t  us){return us/1000000.0;}     //ns转seconds

    //以秒格式化时间字符串hh::mm::ss 或大于1天则为 1day:hh:mm:ss
    static void SecondsToStringTime(uint64_t Sec,char buf[32]);

    //refer juce
    /** Returns the current high-resolution counter's tick-count.
    This is a similar idea to getMillisecondCounter(), but with a higher
    resolution.

    @see getHighResolutionTicksPerSecond, highResolutionTicksToSeconds,
    secondsToHighResolutionTicks
    */
    static uint64_t getHighResolutionTicks();   //取高精度计数器当前值

    /** Returns the resolution of the high-resolution counter in ticks per second.

    @see getHighResolutionTicks, highResolutionTicksToSeconds,
    secondsToHighResolutionTicks
    */

    static uint64_t getHighResolutionTicksPerSecond_Win();
    static uint64_t getHighResolutionTicksPerSecond()   //取高精度计数器每秒计数次数，即频率
    {
#ifdef _WIN32
        return getHighResolutionTicksPerSecond_Win();
#else 
         return 1000000;    //for unix (microseconds) 微妙
#endif 
    }

    /** Converts a number of high-resolution ticks into seconds.

    @see getHighResolutionTicks, getHighResolutionTicksPerSecond,
    secondsToHighResolutionTicks
    */
    static double highResolutionTicksToSeconds (uint64_t ticks)       //ticks计数值转成秒
    {
        return (double)ticks/getClockFreq();
    }

    /** Converts a number seconds into high-resolution ticks.
    @see getHighResolutionTicks, getHighResolutionTicksPerSecond,
    highResolutionTicksToSeconds
    */
    static uint64_t secondsToHighResolutionTicks (double seconds)   //秒转成ticks计数值
    {
        return (uint64_t)(seconds*getClockFreq());
    }
    //若作为全局类中使用,构造顺序不确定,改用static函数封装
    static uint64_t getClockFreq()
    {
#ifdef _WIN32
        //高精度计时器频率根据Cpu不同一般是固定的没有必要重复获取       //计数器频率，一般是恒定的,第一次获取即可
        static uint64_t ClockFreq=SuperTime::getHighResolutionTicksPerSecond();
        return ClockFreq;
#else
        return 1000000;    //for unix (microseconds) 微妙
#endif
    }
private:
    //static uint64_t ClockFreq;      //计数器频率，一般是恒定的,第一次获取即可
    uint64_t begin,end,last,current;
    uint64_t CallCounter;        //
    uint64_t clocks;                 //CPU时钟统计     
    double usedTime;              //得到耗时，单位是s
    double rate;                       //帧率
    double SumTime;              //记录总时时间，用在多次求平均值
    uint64_t SumTimeInTicks;
    double averageTime;         //平均时间 单位是s
    double peakUsedTime;      //两次打印间隔之间的最大峰值时间，瞬时性能检测会用到，每次打印完成复位为0
    double maxUsedTime;       //统计整个运行期间耗时最大峰值耗时
    double averageRate;         //平均帧率 单位是s
    bool isFirstRecall;
    char name[128];               //用一个名字作为前缀 在打印时区分
};

//简化调用
namespace TimeToNow
{
    inline double secs(){return SuperTime::getStartupToNowSecs();}
    inline double ms(){return SuperTime::getStartupToNowInms();}
    inline uint64_t us(){return SuperTime::getStartupToNowInus();}
}




//SuperTime打印辅组类
class SuperTimePrintHelper
{
public:
    SuperTimePrintHelper(SuperTime& tm,unsigned int PintFreq=1)
        :tm(tm),m_printFreq(PintFreq)
    {
        tm.getBeginTime();
    }
    ~SuperTimePrintHelper()
    {
        tm.getEndPrint(m_printFreq,true);
    }
private:
    SuperTime& tm;
    unsigned int m_printFreq;
};



//打印某个函数进出耗时
class AutoPrintUseTime
{
public:
    AutoPrintUseTime(const char* name="",double timeout_ms=40):m_name(name),m_timeout(timeout_ms)
    {
        ::printf("Fun:[%s]--[Enter]\n",m_name);
        //RegisterFunTraceKey(m_name);
        enterTime=SuperTime::getStartupToNowInms();   //为了更精确放在打印上一句之前之后？
    }
    ~AutoPrintUseTime()
    {
        //UnRegisterFunTraceKey(m_name);
        double useTime=SuperTime::getStartupToNowInms()-enterTime;
        if (useTime>m_timeout)
        {
            char buftmp[256];
            ::sprintf(buftmp,"Fun:[%s]--[Leave] may timeout:%f ms;\n",m_name,useTime);
            ::fprintf(stdout,buftmp);
            //::fprintf(stderr,buftmp);
        }
        else
        {
            ::printf("Fun:[%s]--[Leave] useTime:%f ms;\n",m_name,useTime);
        }
    }
public:
protected:
private:
    const char* m_name;
    double enterTime;
    const double m_timeout;    //打印超时警告的时间
};


#define PRINT_FUN_TRACE_TIME   AutoPrintUseTime UseTime(__MyFUNC__);


#if 0
//基于C++11
#include <chrono>
class StopWatch   //秒表，跑表
{
public:
    StopWatch(const char* name=""):name(name)
        ,t1(res::zero())
        ,t2(res::zero())
    {
        tic();
    }
    ~StopWatch()
    {
        toc();
        print();
    }
    void tic()
    {
        t1 = clock::now();
    }
    void toc()
    {
        t2 = clock::now();
        usedTime=std::chrono::duration_cast<res>(t2 - t1).count();
    }
    void print()
    {
        printf("%s time:%f\n",(double)usedTime);
        //std::cout << str << " time: " 
        //    << std::chrono::duration_cast<res>(t2 - t1).count() / 1e3 << "ms." << std::endl;
    }
private:
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::microseconds res;
    int64_t usedTime;
    clock::time_point t1;
    clock::time_point t2;
    const char* name;
};
#endif



//使用方法:
//
//{
//    StopWatch watch;
//    for (int i=0;i<50000;i++)
//    {
//        new std::string; // 不用赋值的堆内存分配
//    }
//}
//{
//    StopWatch watch;
//    for (int i=0;i<50000;i++)
//    {
//        std::string s;//创建并销毁本地的自动创建的串
//    }
//}



//获取开机到现在的ms数   跨平台 长时间运行溢出考虑等
inline uint64_t getStartupToNowInms()
{
    //基于C库函数clock()实现, CLOCKS_PER_SEC宏windows下位1000,linux下为1000000
    //ANSI clock有三个问题:
    //1)如果超过一个小时,将要导致溢出.
    //2)函数clock没有考虑CPU被子进程使用的情况.
    //3)也不能区分用户空间和内核空间.

    //clock_t currTime=::clock();
    //return currTime/CLOCKS_PER_SEC;

    return  (uint64_t)SuperTime::getStartupToNowInms();
}

#ifdef UNIT_TEST
BASIC_API  int Test_SuperTime();
#endif



NAME_SPACE_MAIN_END
#endif

