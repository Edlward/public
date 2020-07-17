#pragma once

//拷贝/移动 大文件，目录，可以显示进度条

#include "global.h" 


#if 0
#include "Basic/Thread.h"
#include <string>
class FileCopyThread: public Thread
{
public:
    FileCopyThread();
    virtual ~FileCopyThread();
    bool Init();
    bool Start();
    bool Stop();
private:
    virtual void run();
    int copy_dir(const char *dir_dst,const char *dir_src,uint64_t* c_total,uint64_t* c_copied);
public:
    void setPath(std::string sDst,std::string sSrc)
    {
        this->sDst=sDst;
        this->sSrc=sSrc;
    }
    uint64_t getTotal(){return total;}
    uint64_t getCopied(){return copied;}
private:
    std::string sDst;
    std::string sSrc;
    uint64_t total;
    uint64_t copied; 
};


#endif
