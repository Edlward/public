#include "Log.h"

#include "BasicOperation.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ThreadSync.h"
#include <algorithm>



#define PATH_MAX 1024
#define TRUE 1
#define FALSE 0
//#define LOGROOT		getConfig().getLogRootPath()	
#define LOG_ROOT_PATH	"./Log"    //
#define USBROOT	"e:"


NAME_SPACE_MAIN_BEGIN

using namespace std;
USING_NAME_SPACE_MAIN

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
LogFileReader::LogFileReader(void)
{
    m_fp=NULL;
}

LogFileReader::~LogFileReader(void)
{
    CloseFile();
}

bool LogFileReader::OpenFile(const char* file)
{
    m_fp = fopen(file,"r");   //modify to rb ?
    if (m_fp == NULL)
    {
        return false;
    }
    return true;
}

void LogFileReader::CloseFile()
{
    if (m_fp != NULL)
    {
        fclose(m_fp);
        m_fp=NULL;
    }
    return;
}

int LogFileReader::Read(const int type,TLogQueryResultArray & result)
{
    char* pfile = NULL;
    char* pMove = NULL;
    const int BUFFER_SIZE=1024;
    char buf[BUFFER_SIZE] = {0};
    char str[BUFFER_SIZE] = {0};

    LOGQUERYRESULT res;
    while ((pfile = fgets(buf,BUFFER_SIZE,m_fp)))
    {
        res.type = (LOGTYPE)atoi(buf);
        pfile = strstr(buf," ");
        //res.cat=atoi(pfile);
        pfile++;
        pfile = strstr(pfile," ");
        pfile++;
        pMove = strstr(pfile," ");
        memcpy(str,pfile,pMove-pfile);
        res.time = str;
        res.text = ++pMove;
        if ((LOGTYPE)type == LOG_ALL || (LOGTYPE)type == res.type)
        {
            result.push_back(res);
        }
    }
    return 0;
}


int LogFileReader::Read(const int type, std::string & result)
{
    char* pfile = NULL;
    char* pMove = NULL;
    char buf[PATH_MAX] ={0};
    char str[PATH_MAX]={0};

    LOGQUERYRESULT res;
    while ((pfile = fgets(buf,PATH_MAX,m_fp)))
    {
        if(buf[0] == '\0')    //?
            break;
        res.type = (LOGTYPE)atoi(buf);
        //strstr(str1,str2) 若str2是str1的子串,则返回str2在str1的首次出现的地址；否则则返回NULL。
        pfile = strstr(buf," ");
        pMove = strstr(++pfile," ");
        memcpy(str,pfile,pMove-pfile);
        res.time = str;
        res.text = ++pMove;
        if ((LOGTYPE)type == LOG_ALL||(LOGTYPE)type == res.type)
        {
            //编译出错
            //result = result + res.time + " " + res.text + "\n";
            //改
            result +=res.time; 
            result +=" ";
            result +=res.text;
            //result +="\n";
        }
    }
    return 0;
}


BufferLog::BufferLog()
{
    len=0;
    FlushNow=false;
}

BufferLog::BufferLog(const char* buf,unsigned int len,bool FlushNow)
{
    this->FlushNow=FlushNow;
    this->len=std::min<int>(len,sizeof(this->buf));
    memcpy(this->buf,buf,this->len);
}

BufferLog& BufferLog::operator=(const BufferLog& r)
{
    this->len=r.len;
    this->FlushNow=r.FlushNow;
    memcpy(this->buf,r.buf,r.len);
    return *this;
}


static Log gLog;
Log* GetLog()
{
    return &gLog;
}

class Log::impl
{
public:
    impl(){};
    ~impl(){};
    //MyMutex mutex;
};

Log::Log():ringbufferLog(8)
{
    pimpl=new impl;
    //ringbufferLog=new RingBuffer<BufferLog>(8);
    setLogPath(LOG_ROOT_PATH);
}

Log::~Log()
{
    flush();
    stop(1000);
    //delete ringbufferLog;
    delete pimpl;
}

void Log::setLogPath(const char* path)
{
    if (path==NULL)
    {
           return;
    }
    std::string tmp(path);
    if (tmp=="")
    {
        return;
    }
    sRootPath=tmp;
}


int Log::Delete(LOGDATETIME * timeAfter, LOGDATETIME * timeBefore)
{
    return 0;
}

int Log::Export(TLogQueryResultArray & contextList)
{
    return 0;
}


int Log::Query(int type,LOGDATETIME* timeAfter, LOGDATETIME* timeBefore, TLogQueryResultArray & result)
{
    char buft[PATH_MAX] = {0};
    //std::string path = LOG_ROOT_PATH;
    std::string path = sRootPath;

    //%s\\%04d-%02d"
    sprintf(buft,"%s/%04d-%02d",path.c_str(),timeAfter->year,timeAfter->month);
    if (!isDirectory(buft))
    {
        return 0;
    }

    int day = timeAfter->day;
    int month = timeAfter->month;
    int year = timeAfter->year;

    while (!((day - 1) == timeBefore->day 
        && month == timeBefore->month 
        && year == timeBefore->year))
    {
        memset(buft,0,PATH_MAX);
        sprintf(buft,"%s/%04d-%02d/%02d.log.txt",path.c_str(),year,month,day++);
        if (m_LogReader.OpenFile(buft))
        {
            m_LogReader.Read(type,result);
            m_LogReader.CloseFile();
        }
        if(result.size()>2000)
            break;//最多返回2000条记录

        if (day == 31)
        {
            day = 1;
            month++;
            if (month > 12)
            {
                month = 1;
                year++;
                if (year > timeBefore->year)
                {
                    break;
                }
            }
        }
    }
    return 0;
}

int Log::Query(int type, LOGDATETIME* timeAfter, LOGDATETIME* timeBefore, std::string& sLogResult)
{
    char buft[PATH_MAX] = {0};
    //std::string path = LOG_ROOT_PATH;
    std::string path = sRootPath;

    sprintf(buft,"%s/%04d-%02d",path.c_str(),timeAfter->year,timeAfter->month);
    
    //相对路径转绝对路径  待续
    //if (!isDirectory(buft))
    //{
    //    return 0;
    //}

    int day = timeAfter->day;
    int month = timeAfter->month;
    int year = timeAfter->year;

    while (!((day - 1) == timeBefore->day 
        && month == timeBefore->month 
        && year == timeBefore->year))
    {
        memset(buft,0,PATH_MAX);
        sprintf(buft,"%s/%04d-%02d/%02d.log.txt",path.c_str(),year,month,day++);
        if (m_LogReader.OpenFile(buft))
        {
            m_LogReader.Read(type,sLogResult);
            m_LogReader.CloseFile();
        }
        //if(result.size()>2000)
        //    break;//最多返回2000条记录

        if (day == 31)
        {
            day = 1;
            month++;
            if (month > 12)
            {
                month = 1;
                year++;
                if (year > timeBefore->year)
                {
                    break;
                }
            }
        }
    }
    return 0;
}

int Log::Write(int type,const char *text,bool FlushNow)
{
    time_t timep;
    tm* p;
    time	(&timep);
    p = localtime(&timep);
    char buf[10*1024];
    sprintf(buf,"%02d %04d%02d%02d-%02d:%02d:%02d %s\r\n",
        type,
        (1900+p->tm_year),
        (1+p->tm_mon),
        (p->tm_mday),
        p->tm_hour,
        p->tm_min,
        p->tm_sec,
        text);

    BufferLog buffLog(buf,strlen(buf),FlushNow);
    ringbufferLog.pushMayMultiple(buffLog,false);   //true false可能丢失日志 true可能会阻塞
    return 0;
}

void Log::flush()
{
   // isFlushAndQuit=true;
    //推送一个buffer .FlushNow=true即可命令写线程清空流。
    BufferLog buffLog;
    buffLog.FlushNow=true;
    ringbufferLog.pushMayMultiple(buffLog,false);
}


void Log::run()
{
    while (getContinueRun())
    {
            //若打开的是已经存在的日志文件
            size_t orangelFileSize=0;
            FILE* fp=getWriteFILEHandle(orangelFileSize);
            size_t isWrittenSize=orangelFileSize;
            while (getContinueRun())
            {
                //BufferLog* in=NULL;
                //ringbufferLog->get(&in,true);       //阻塞等待直到有日志
                BufferLog in;
                //ringbufferLog->get(in,true);          //阻塞等待直到有日志--再改进
                while (getContinueRun())
                {
                    bool retGet=ringbufferLog.getOnlySingle(in,( unsigned int )2000);   //为了能随时退出，非阻塞
                    if (retGet) break;
                }

                isWrittenSize+=fwrite(in.buf,1,in.len,fp);
                if (in.FlushNow)   //立即刷新流到文件
                {
                    ::fflush(fp);	
                }

                if (isWrittenSize>10*1024*1024)    //
                {
                    ::fclose(fp);
                    fp=getWriteFILEHandle(orangelFileSize);
                    isWrittenSize=orangelFileSize;
                }
            }
            fclose(fp);
        }
}


#if 0
//指定文件流的缓冲区函数
void setbuf(FILE *fp, char *buf);
void setvbuf(FILE *fp, char *buf, int type, unsigned size);
setbuf函数的定义中，参数buf指定的缓冲区大小由stdio.h中定义的宏BUFSIZE的值决定，缺省值default为512字节。
而当buf为NULL时，setbuf函数将使文件I/O不带缓冲区。
而对setvbuf函数，则由malloc函数来分配缓冲区。参数size指明了缓冲区的长度(必须大于0)，而type表明了缓冲的类型，
type 值
_IOFBF 文件全部缓冲，即缓冲区装满后，才能对文件读写
_IOLBF 文件行缓冲，即缓冲区接收到一个换行符时，才能对文件读写
_IONBF 文件不缓冲，此时忽略buf,size的值，直接读写文件，不再经过文件缓冲区缓冲
#endif




//取文件句柄，管理文件大小 日期 创建新日志文件
FILE* Log::getWriteFILEHandle(size_t& ExistsFileSize)
{
    //std::string logPath="C:";  //LOG_ROOT_PATH;
    std::string logPath=sRootPath;
    time_t timep;
    tm* p;
    time	(&timep);
    p = localtime(&timep);
    char buft[PATH_MAX] = {0};
    sprintf(buft,"%s\\%04d-%02d\\%02d.log.txt",logPath.c_str(),(1900+p->tm_year),(1+p->tm_mon),p->tm_mday);		
    createDirectory(buft);  //路径不一定存在,先创建，若存在则直接返回

    std::string Path(buft);
    ExistsFileSize=0;
    if (isExistFile(Path.c_str()))
    {
        ExistsFileSize=getFileSize(Path.c_str());
        if (ExistsFileSize>100*1024*1024)     //如果已有日志文件大于100Mb 用新的日志名称并添加后缀-01 -02等 待续
        {

        }
    }

    FILE* fp=fopen(Path.c_str(),"ab");
    if(fp==NULL) 
    {
        printf("Error at: open log file %s\n",Path.c_str());	
        //return NULL;
        exit(-1);
    }
    return fp;
}

USING_NAME_SPACE_MAIN

#ifdef UNIT_TEST

int Test_Log()
{
    //createDirectory(".\\123\\456\\789\\");
    GetLog()->start();
    for (int i=0;i<100*1024;i++)
    {
        char str[500];
        sprintf(str, "Log Test1234567890=%d",i);
        GetLog()->Write(LOG_SYSTEM,str);
    }

    return 0;
}
#endif


NAME_SPACE_MAIN_END
