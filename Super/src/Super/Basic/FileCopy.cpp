#include "FileCopy.h"

#if 0
#include <vector>
typedef std::vector<std::string> DIR_LIST;

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/exception.hpp>
//#include <thread/activeThread/activeThread.h>
//#include <thread/activeThread/activeThreadManager_i.h>
//#include "../basic/bruntActiveThread.h"  //activeThread线程相关合并成一个文件方便使用，by lizulin 20180125


namespace fs = boost::filesystem;
static uint64_t scan_dir(std::string dir, DIR_LIST& dirList)   //
{  
    int64_t totalsize=0;
    fs::path p(dir);
    dirList.push_back(dir);
    char str[512];
    try
    {
        fs::directory_iterator end_itr;
        for(fs::directory_iterator itr(p); itr != end_itr; ++itr)
        {
            try
            {
                if(fs::is_directory(*itr))
                {
                    totalsize += scan_dir((*itr).path().string(), dirList);
                }
                else
                {
                    //移到外面更合适，待续 by lizulin 20180810
                    // Add DCPFILTER to ignore video & non atmos mxf [12/19/2017 jaontolt]
                    //if(getDcpFilter()->inDcp((*itr).path().string()) == 1)
                    {
                        dirList.push_back((*itr).path().string());
                        totalsize += fs::file_size((*itr).path());
                    }
                }
            }
            catch(const fs::filesystem_error& e)
            {
                sprintf(str, "[scan_dir] Skip the IO error, except: %s.", e.what());
                printf("%s\n", str);
            }
        }
    }
    catch(const fs::filesystem_error& e)
    {
        sprintf(str, "[scan_dir] except: %s.", e.what());
        printf("%s\n", str);
        //GetLog()->Write(LOG_ERROR, str);
    }
    return  totalsize;
}  


//char iobuf[BUFSIZE];    //不能放函数体内，堆栈空间不够，只能放全局堆，不常用缺占用固定内存
#define BUFSIZE 1024*512   //


static int cpfile(const char *DstPath,const char *SrcPath,uint64_t* totalSize,uint64_t* completeSize)
{
    //char* iobuf=new(std::nothrow) char[BUFSIZE];
    char iobuf[BUFSIZE];
    FILE *source,*target;
    int nread;

    //static int64_t complete_size=0;
    if((source=fopen(SrcPath, "rb"))==NULL)
    {
        printf("Error at cpfile(): Source file open error!\n");
        //GetLog()->Write(LOG_ERROR, "Error at cpfile(): Source file open error!");
        return 1;
    }
    if((target=fopen(DstPath,"wb"))==NULL)
    {
        printf("Error at: cpfile() Target file open error!\n");
        //GetLog()->Write(LOG_ERROR, "Error at cpfile(): Target file open error!");
        fclose(source);
        return 2;
    }

    while((nread=fread(iobuf, 1, BUFSIZE, source))>0)
    {
        if(fwrite(iobuf, 1, nread, target)!=nread)
        {
            printf("Error at cpfile(): Target file write error!\n");
            //GetLog()->Write(LOG_ERROR, "Error at cpfile(): Target file write error!");
            fclose(source);
            fclose(target);
            return 3;
        }
        else
        {
            (*completeSize)+=nread;
            if(*completeSize/(*totalSize) == 1)   //
            {
                printf("complete_size= %lld \n",*completeSize);       
                printf("totalsize= %lld\n",totalSize);                       //
                printf("Copy file to %s ......\n",DstPath);
            }
        }	
    }
    fclose(source);
    fclose(target);

    //delete[] iobuf;
    return 0;
}

static int cpdir(DIR_LIST &dirList, const char *dst_dir,uint64_t* totalSize,uint64_t* completeSize)
{
    //if target dir no exist, create it.
    if(!fs::exists(dst_dir))
    {
        fs::create_directory(dst_dir);
    }
    std::string path = dst_dir;
    for(unsigned int i = 0; i < dirList.size(); i++)
    {
        fs::path p(dirList[i]);
        if(fs::is_directory(p))
        {
            path = path + "\\" + p.filename().string();
            fs::create_directory(path);
        }
        else
        {
            std::string dst = path + "\\" + p.filename().string();
            cpfile((char*)dst.c_str(),(char*)p.string().c_str(),totalSize,completeSize);
        }
    }
    return 0;
}

FileCopyThread::FileCopyThread()
{

}

FileCopyThread::~FileCopyThread()
{
    //Stop();
}

bool FileCopyThread::Init()
{
    printf("CopyThread:Init()\n");
    //Start();
    return true;
}

bool FileCopyThread::Start()
{
    printf("CopyThread: Start()\n");
    //Stop();
    Thread::start();
    return true;
}

bool FileCopyThread::Stop()
{
    printf("CopyThread: Stop()\n");
    Thread::stopWaitFinish();
    return true;
}

void FileCopyThread::run()
{
    while(getContinueRun())
    {
        //printf("CopyThread: Start in loop\n");
        copy_dir(this->sDst.c_str(),this->sSrc.c_str(),&this->total,&this->copied);
        break;
    }
}

int FileCopyThread::copy_dir(const char *dir_dst,const char *dir_src,uint64_t* total,uint64_t* copied)
{
    printf("CopyThread::copy_dir(): dst=%s src=%s \n",dir_dst,dir_src);  

    *total=0;
    *copied=0;
    DIR_LIST dirList;
    *total = scan_dir(dir_src, dirList);

    printf("CopyThread::copy_dir(): toCopying Dir totalsize= %lld\n",*total);  
    if(*total == 0)  return 0;

    cpdir(dirList, dir_dst, total, copied);
    return 0;
}





#endif
