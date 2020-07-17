#include "ShareMemory.h"

NAME_SPACE_MAIN_BEGIN

#if defined(_WIN32)
#include <Windows.h>
#else //for unix
#include <sys/types.h>    //ftok and so on
#include <sys/ipc.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif


#if defined(_WIN32)

void  ShareMemory_CreateNewShareMemory(ShareMemory* parent)
{
    parent->isCreatOK=false;
    std::string sMprintName=TstringTostring(parent->sMemName);
    //printf("MapShareMemory::CreatorNewShareMemory(): Size:%d Byte;Name:%s\n",ShareSize,sMprintName.c_str());

    parent->pBufCreator = NULL;
    //创建一个有名的共享内存
    parent->HandleCreator=CreateFileMapping(
        INVALID_HANDLE_VALUE,     //=0xFFFFFFFF表示创建一个进程间共享的对象
        NULL,
        PAGE_READWRITE,                //读写共享
        0,
        parent->ShareSize,               //共享区间大小
        parent->sMemName.c_str());//映射文件名，即共享内存的名称
    if (NULL == parent->HandleCreator)
    {
        if (ERROR_ALREADY_EXISTS == GetLastError())
        {
            PreDprintf("Already exists!\n");
        }
        else if(ERROR_INVALID_HANDLE==GetLastError())
        {
            //PreDprintf("发现的命名内存空间和现有的内存映射, 互斥量, 信号量, 临界区同名\n");
            PreDprintf("ERROR_INVALID_HANDLE==GetLastError()\n");
        }
        else
        {
            PreDprintf("failed!\n");
        }
        return;
    }

    parent->pBufCreator = (char*)MapViewOfFile(parent->HandleCreator,FILE_MAP_ALL_ACCESS,0,0,parent->ShareSize);
    if (NULL==parent->pBufCreator)
    {
        PreDprintf("failed at MapViewOfFile()\n");
        return;
    }
    parent->isCreatOK=true;               //创建成功
}
void  ShareMemory_GetExistShareMemory(ShareMemory* parent)
{
    parent->isGetOK=false;
    std::string sMprintName=TstringTostring(parent->sMemName);
    //printf("MapShareMemory::UserGetShareMemory() Size=%d Byte;Name:%s\n",ShareSize,sMprintName.c_str());
    //映射到本进程的地址空间 
    parent->HandleUser = OpenFileMapping(FILE_MAP_ALL_ACCESS,false,parent->sMemName.c_str());
    if (NULL == parent->HandleUser)
    {
        PreDprintf("failed at OpenFileMapping() keyName:%s\n",sMprintName.c_str());
        return;
    }
    //读写权限FILE_MAP_ALL_ACCESS FILE_MAP_READ  FILE_MAP_WRITE
    parent->pBufUser = (char*)MapViewOfFile(parent->HandleUser,FILE_MAP_ALL_ACCESS, 0, 0,parent->ShareSize);
    if (NULL == parent->pBufUser)
    {
        PreDprintf("failed at MapViewOfFile()\n");
        return;
    }
    parent->isGetOK=true;
}

void  ShareMemory_UnmapFromCreator(ShareMemory* parent)
{
    //本对象是创建者
    if(parent->pBufCreator!=NULL)
    {
        UnmapViewOfFile(parent->pBufCreator); 
        parent->pBufCreator=NULL;
    }
    //pBufCreator=NULL;  //取消映射后不可以再操作
}
void  ShareMemory_UnmapFromUser(ShareMemory* parent)
{
    //本对象不是创建者
    if(parent->pBufUser!=NULL)
    {
        UnmapViewOfFile(parent->pBufUser); 
        parent->pBufUser=NULL; 
    }
    //pBufUser=NULL;   //取消映射后不可以再操作
}
void ShareMemory_GetShareMemory(ShareMemory* parent)
{

}
void ShareMemory_Close(ShareMemory* parent)
{

}
void ShareMemory_Release(ShareMemory* parent)
{
    //加上反而有问题，待确认
    //if (pimpl->HandleCreator!=NULL)
    //{
    //    CloseHandle(pimpl->HandleCreator);
    //    pimpl->HandleCreator=NULL;
    //}
    //if (pimpl->HandleUser!=NULL)
    //{
    //    CloseHandle(pimpl->HandleUser);
    //    pimpl->HandleUser=NULL;
    //}
}

#else   //LINUX 


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//最简单的共享内存的使用流程
//①ftok函数生成键值
//②shmget函数创建共享内存空间
//③shmat函数获取第一个可用共享内存空间的地址
//④shmdt函数进行分离（对共享存储段操作结束时的步骤，并不是从系统中删除共享内存和结构）
//⑤shmctl函数进行删除共享存储空间


void  ShareMemory_CreateNewShareMemory(ShareMemory* parent)
{
    parent->isCreatOK=false;
    key_t mem_key=(key_t)0;
    // int shmid;  
    void* segptr;  
    std::string sName=TstringTostring(parent->sMemName);

#if 1
    mem_key=(key_t)ShareMemory::getShareMemKey(sName.c_str());  //用自定义key
#else
   //key_t ftok(const char *path ,int id);path为一个已存在的路径名
    //可以用sName创建一个文件再调用，很麻烦，有空待续

    mem_key=ftok(sName.c_str(),1);    //
    if ((key_t)(-1)==mem_key) 
    {
        PreDprintf("failed:ftok()errno=%d %s  keyName:%s\n",errno,strerror(errno),sName.c_str());
        return;
    }
#endif

    if (mem_key==(key_t)0)
    {
        PreDprintf("failed invalid keyName:%s\n",sName.c_str());
        return;
    }

    if((parent->HandleCreator=shmget(mem_key,parent->ShareSize,IPC_CREAT|IPC_EXCL|0666))==-1)  
    {  
        PreDprintf("failed shmget() errno=%d %s keyName:%s\n", errno,strerror(errno),sName.c_str());
        return;
    }

    if((segptr=shmat(parent->HandleCreator,0,0))==(void *)-1)   
    {  
        //PRINT_ERROR_LINE
        PreDprintf("failed shmat() errno=%d %s keyName:%s\n", errno,strerror(errno),sName.c_str());
        return;
    }  
    parent->pBufCreator=(char*)segptr;
    parent->isCreatOK=true;
    return;
}
void  ShareMemory_GetExistShareMemory(ShareMemory* parent)
{
    parent->isGetOK=false;
    key_t mem_key=(key_t)0;
    // int shmid;  
    void* segptr;  
    std::string sName=TstringTostring(parent->sMemName);

#if 1
    mem_key=(key_t)ShareMemory::getShareMemKey(sName.c_str());
#else
    mem_key=ftok(sName.c_str(),1);
    if ((key_t)(-1)==mem_key) 
    {
        PreDprintf("failed ftok() errno=%d %s keyName:%s\n",errno,strerror(errno),sName.c_str());
        return;
    }
#endif

    if (mem_key==(key_t)0)
    {
        PreDprintf("failed invalid keyName:%s\n",sName.c_str());
        return;
    }

	//注意，当创建一个新的共享内存区时，size 的值必须大于 0 ；如果是访问一个已经存在的内存共享区，则置 size 为 0 。

    //if((parent->HandleUser=shmget(mem_key,parent->ShareSize, 0))==-1)
    if((parent->HandleUser=shmget(mem_key,0,IPC_EXCL|0666))==-1)
    {  
        PreDprintf("failed shmget() errno=%d %s keyName:%s\n",errno,strerror(errno),sName.c_str());
        return;
    }   
    if((segptr=shmat(parent->HandleUser,0,0))==(void *)-1)   
    {  
        PreDprintf("failed shmat() errno=%d %s keyName:%s\n",errno,strerror(errno),sName.c_str());
        return;
    }
    parent->pBufUser=(char*)segptr;
    parent->isGetOK=true;
    return;
}
void  ShareMemory_UnmapFromCreator(ShareMemory* parent)
{
    //本对象是创建者
    if(parent->pBufCreator!=NULL)
    {
        shmdt(parent->pBufCreator); //禁止本进程访问此片共享内存
        parent->pBufCreator = NULL;
    }
}
void  ShareMemory_UnmapFromUser(ShareMemory* parent)
{
    //本对象是使用者
    if(parent->pBufUser!=NULL)
    {
        shmdt(parent->pBufUser); //禁止本进程访问此片共享内存
        parent->pBufUser = NULL;
    }
}
void ShareMemory_GetShareMemory(ShareMemory* parent)
{

}
void ShareMemory_Close(ShareMemory* parent)
{

}
void ShareMemory_Release(ShareMemory* parent)
{
    //两个进程都删除会不会有问题？  TODO:最后一个使用者释放共享内存？不需要
    //if (Usershmid!=-1)
    //{
    //    shmctl(Usershmid,IPC_RMID,0);
    //}
    //else if (Creatorshmid!=-1)
    //{
    //    shmctl(Creatorshmid,IPC_RMID,0);
    //}
}

#endif



NAME_SPACE_MAIN_END
