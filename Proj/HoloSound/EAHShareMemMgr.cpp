#include "EAHShareMemMgr.h"

#include <stdio.h>


#ifdef OS_ARM_LINUX
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>
#endif


ShareMemMgr* GetShareMemMgr()
{
	static ShareMemMgr gShareMemMgr;
	return &gShareMemMgr;
}

ShareMemMgr::ShareMemMgr():pSM(nullptr),mem_start(nullptr)
{
     if(pSM==NULL)
    {
    #ifdef OS_ARM_LINUX
        //Open Share memory
        mem_fd = open("/dev/multicore",O_RDWR|O_SYNC);

        //TEST
        //mem_fd = open("/dev/multicore",O_RDWR|O_ASYNC);
        //mem_fd = open("/dev/multicore",O_RDWR|O_DIRECT);  //core dump
        //mem_fd = open("/dev/multicore", O_RDWR|O_SYNC|O_NDELAY);

        if(mem_fd < 0)
        {
            ::printf("Can not open /dev/multicore\n");
            return;
        }
        mem_start = (char*)mmap(NULL,0x02000000,PROT_READ|PROT_WRITE, MAP_SHARED, mem_fd, 0);
        if(mem_start==MAP_FAILED)
        {
            ::printf("share memory mmap failed\n");
            return;
        }

        pSM=(ShareMemOfDCAP_EAH*)mem_start;
        ::printf("open share mem successfull pSM:%p\n",pSM);

        if (pSM->initFlag!=Const_INIT_Key)
        {
              //new(pSM) ShareMemOfDCAP_EAH();     //谁来初始化共享内存，待确认
              pSM->initFlag=Const_INIT_Key;
        }
    #else
        //为了不崩溃先指向一片实际存在的内存地址
        pSM=new ShareMemOfDCAP_EAH;
    #endif
        //DCAP-EAH Global share memory
    }
}



ShareMemMgr::~ShareMemMgr()
{
#ifdef OS_ARM_LINUX
    if(mem_start!=NULL)
    {
        munmap((void*)mem_start,0x10000000);
        mem_start=NULL;
    }
    if(mem_fd>0)
    {
        close(mem_fd);
        mem_fd=0;
    }
#else
    if(pSM!=NULL)
    {
        delete pSM;
        pSM=NULL;
    }
#endif
}


//off_t lseek(int fd, off_t offset, int whence);
//fd：文件描述符
//    offset：偏移的数量
//    whence：从哪里开始偏移
//    SEEK_SET：文件描述符的开始位置
//    SEEK_CUR：文件描述符的当前位置
//    SEEK_END：文件描述符的末尾位置
//    返回值
//    成功：返回当前位置到开始位置的长度
//    失败：返回-1，并设置errno


//1、write()
//函数定义：ssize_t write (int fd, const void * buf, size_t count); 
//函数说明：write()会把参数buf所指的内存写入count个字节到参数放到所指的文件内。
//返回值：如果顺利write()会返回实际写入的字节数。当有错误发生时则返回-1，错误代码存入errno中。


void ShareMemMgr::updateWrire(void* addr,size_t len)
{
  #ifdef OS_ARM_LINUX
    int offset=(char*)addr-(char*)pSM;
    int retlseek=lseek(mem_fd,offset,SEEK_SET);    //注意内核驱动仅仅实现了SEEK_SET SEEK_CUR
    if (retlseek==-1)
    {
        printf("failed! at lseek addr:%p len:%d\n",addr,len);
        return;
    }

    int nwrittern=write(mem_fd,addr,len); 
    if (nwrittern!=len)
    {
        printf("failed! at update mem addr:%p len:%d\n",addr,len);
    }
  #else

  #endif
}

void ShareMemMgr::updateRead(void* addr,size_t len)
{
#ifdef OS_ARM_LINUX
    int offset=(char*)addr-(char*)pSM;
    int retlseek=lseek(mem_fd,offset,SEEK_SET);    //注意内核驱动仅仅实现了SEEK_SET SEEK_CUR
    if (retlseek==-1)
    {
        printf("failed! at lseek addr:%p len:%d\n",addr,len);
        return;
    }
    int nread=read(mem_fd,addr,len); 
    if (nread!=len)
    {
        printf("failed! at update mem addr:%p len:%d\n",addr,len);
    }
#else

#endif
}
