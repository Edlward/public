#ifndef  _SuperLite_LOG_
#define _SuperLite_LOG_

#include <vector>
#include <list>
#include <string>
#include "global.h"
#include "Thread.h"
#include "RingBuffer.h"          //跨线程环形队列


NAME_SPACE_MAIN_BEGIN

typedef enum 
{
    LOG_ALL = 0,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_SYSTEM,
    LOG_PLAY,
    LOG_END
}LOGTYPE;

//typedef enum 
//{
//    LOG_CAT_ALL = 0,
//    LOG_CAT_END
//}LOGCAT;

typedef enum 
{
    EXPORT_GDC=0,
    EXPORT_EXCEL,
    EXPORT_TEXT
}EXPORTTYPE;

typedef struct logDateTime
{
    unsigned short year;
    unsigned char  month;
    unsigned char  day;
}LOGDATETIME;

typedef struct logTime
{
    unsigned char hour;
    unsigned char minute;
    unsigned char seconds;
}LOGDAYTIME;

typedef struct logQueryResult
{
    int type;
    //int cat;
    std::string     time; //yyyy-mm-dd/hh:mm:ss   
    std::string     text;   
}LOGQUERYRESULT;


typedef std::vector<LOGQUERYRESULT> TLogQueryResultArray;
typedef std::list<std::string> TLogExportContextList;


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//新的轻量高效日志类重构
class LogFileReader
{
public:
    LogFileReader(void);
    ~LogFileReader(void);
public:
    bool OpenFile(const char* file);
    void CloseFile();
    int Read(const int type, TLogQueryResultArray & result);
    int Read(const int type, std::string & result);
private:
    FILE* m_fp;
};




//template <typename T> class RingBuffer;
struct BufferLog
{
    char buf[1*1024];
    unsigned int len;
    bool FlushNow;
    BufferLog();
    BufferLog(const char* buf,unsigned int len,bool FlushNow=false);

    BufferLog& operator=(const BufferLog& r);
};


//自己实现一个新的Log类
//多线程同时写，轻量高效线程库
class BASIC_API Log :public Thread
{
public:
    Log();
    ~Log();
    void run();
public:
    void setLogPath(const char* path);  //日志根目录路径，不设置采用默认
    int Delete(LOGDATETIME * timeAfter, LOGDATETIME * timeBefore);       //删除日志，暂不实现
    int Export(TLogQueryResultArray & contextList);                                     //导出到一个文件，暂没有必要实现

    //查询日志，已结构返回或已字符串返回
    int Query(int type,LOGDATETIME* timeAfter, LOGDATETIME* timeBefore, TLogQueryResultArray & result);
    int Query(int type, LOGDATETIME* timeAfter, LOGDATETIME* timeBefore, std::string& sLogResult);

    //模式1--独立写线程模式 FlushNow为true 表示立即写入，一般为了效率没有必要
    int Write(int type,const char *text,bool FlushNow=false);
    void flush();        //立即推送给清空流
    bool isFlushAndQuit;
private:
    std::string sRootPath;                //日志根目录
    LogFileReader m_LogReader;
    FILE* getWriteFILEHandle(size_t& ExistsFileSize); //同时返回文件写指针和已经存在的文件大小
    RingBuffer<BufferLog>  ringbufferLog;
    class impl;
    impl* pimpl;
};

BASIC_API Log* GetLog();

#ifdef UNIT_TEST
BASIC_API int Test_Log();
#endif


NAME_SPACE_MAIN_END
#endif

