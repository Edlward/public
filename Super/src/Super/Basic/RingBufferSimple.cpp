#include "RingBufferSimple.h"

#include <iostream>
#include <string>
#include <stdlib.h>
#include <memory.h>

#include "ThreadSync.h"
#include "ShareMemory.h"

NAME_SPACE_MAIN_BEGIN

class RingBufferSimple::Impl
{
public:
    Impl(const unsigned int bufferSize = 8):
        semEmpty(bufferSize,bufferSize)
        ,semFull(bufferSize,0)
    {
    }
    ~Impl(){};
    Semaphore semEmpty;
    Semaphore semFull;
    Mutex writeMutex;
    Mutex readMutex;
};


RingBufferSimple::RingBufferSimple(const unsigned int DataSize,const unsigned int bufferNum) :
    DataSize(DataSize)
{
    pImpl=new Impl(bufferNum);
    Capacity = bufferNum;
    RingCountMax=Capacity+1;  //多开一个，否则windex/rindex无法表示刚好等于满容量的情况
    WriteIndex = 0;
    ReadIndex = 0;
    Count = 0;
    pData = new char[RingCountMax*DataSize];
    char* pBegin=(char*)pData;
    for(unsigned int i = 0; i < Capacity*DataSize; i++)  //sizeof()
    {
        pBegin[i]=0;   //memset 
    }
}

RingBufferSimple::~RingBufferSimple()
{
    //for(unsigned int i = 0; i < BufferSize; i++)  // 若放的不是指针，这句话删去
    //{
    //	//if(data[i] != NULL)
    //	//delete data[i];
    //}
    delete[] pData;
    delete pImpl;
}

void RingBufferSimple::pushData(const void* cargo)
{
    memcpy((void*)(pData+WriteIndex*DataSize),cargo,DataSize);
    WriteIndex=getNextCycle(WriteIndex,RingCountMax);
    //Count++;
}

void RingBufferSimple::getData(void* cargo)
{
    memcpy(cargo,(void*)(pData+ReadIndex*DataSize),DataSize);
    ReadIndex=getNextCycle(ReadIndex,RingCountMax);
    //Count--;
}

bool RingBufferSimple::pushOnlySingle(const void* cargo, bool flag) // 往缓冲区放数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushOnlySingle(cargo,timeout);
}

bool RingBufferSimple::pushOnlySingle(const void* cargo,unsigned int timeout)
{
    if(pImpl->semEmpty.Wait(timeout)==false)
    {
        return false;
    }
    {
        pushData(cargo);
    }
    pImpl->semFull.Signal();
    return true;
}

bool RingBufferSimple::getOnlySingle(void* cargo,bool flag) // 取出缓冲区数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getOnlySingle(cargo,timeout);
}

bool RingBufferSimple::getOnlySingle(void* cargo,unsigned int timeout)
{
    if(pImpl->semFull.Wait(timeout)==false)
    {
        return false; //return NULL;
    }
    {
        getData(cargo);
    }
    pImpl->semEmpty.Signal();
    return true;
}


bool RingBufferSimple::pushByMultiple(const void* cargo, bool flag) // 往缓冲区放数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushByMultiple(cargo,timeout);
}

bool RingBufferSimple::pushByMultiple(const void* cargo,unsigned int timeout)
{
    if(pImpl->semEmpty.Wait(timeout)==false)
    {
        return false;
    }
    {
        LockerGuard<Mutex> lock(pImpl->writeMutex);
        pushData(cargo);
    }
    pImpl->semFull.Signal();
    return true;
}

bool RingBufferSimple::getByMultiple(void* cargo,bool flag) // 取出缓冲区数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getByMultiple(cargo,timeout);
}

bool RingBufferSimple::getByMultiple(void* cargo,unsigned int timeout)
{
    if(pImpl->semFull.Wait(timeout)==false)
    {
        return false; //return NULL;
    }
    {
       LockerGuard<Mutex> lock(pImpl->readMutex);
       getData(cargo);
    }
    pImpl->semEmpty.Signal();
    return true;
}


bool RingBufferSimple::clear(bool flag) // 清空缓冲区
{
    //可能设计不合理，待续

    //WriteIndex/ReadIndex分别用读/写 锁保护 应该同时互斥才正确
    LockerGuard<Mutex> lockR(pImpl->readMutex);
    LockerGuard<Mutex> lockW(pImpl->writeMutex);   

    pImpl->semEmpty.ReSet();
    pImpl->semFull.ReSet();

    //读写位置复位
    WriteIndex = 0;
    ReadIndex = 0;
    Count = 0;

    return true;
}






class ringbufferProcessSimple::Impl
{
public:
    Impl(unsigned int  DataSize,unsigned int bufferSize = 8,const char* name=""):
        semEmpty(bufferSize,bufferSize,std::string(std::string(name)+"_semEmpty").c_str())
        ,semFull(bufferSize,0,std::string(std::string(name)+"_semFull").c_str())
        ,writeMutex(std::string(std::string(name)+"_writeMutex").c_str())
        ,readMutex(std::string(std::string(name)+"_readMutex").c_str())
        ,m_ShareMem(bufferSize*DataSize+sizeof(ShareCount),std::string(std::string(name)+"_privateMem").c_str())
    {
    }
    ~Impl(){};
    Semaphore semEmpty;
    Semaphore semFull;
    MutexProcess writeMutex;
    MutexProcess readMutex;
    ShareMemory m_ShareMem;
};

ringbufferProcessSimple::ringbufferProcessSimple(unsigned int DataSize,unsigned int bufferNum,const char* name/*=NULL*/) : 
    Capacity(bufferNum)
    ,DataSize(DataSize)
{
    pImpl=new Impl(DataSize,bufferNum,name);

    Capacity = bufferNum;
    RingCountMax=Capacity+1;  //多开一个，否则windex/rindex无法表示刚好等于满容量的情况

    ////用共享内存构造对象
    void* pShare=pImpl->m_ShareMem.getAddress();
    ////pData=operator new(pShare) T[BufferSize];
    //pData=(T*)pShare;

    pData=(char*)pShare;

    //进程间共享计数器的初始化
    pSC=(ShareCount*)((char*)pShare+bufferNum*DataSize);  //sizeof(T)
    if (pSC->initKey!=0X08250512)
    {
        pSC->WriteIndex=0;                                   // 当前写入位置
        pSC->ReadIndex=0;                                   // 当前读取位置
		pSC->Capacity=Capacity;
        //pSC->Count=0;	
        pSC->initKey=0X08250512;
    }
}

ringbufferProcessSimple::~ringbufferProcessSimple() 
{
    //delete[]  pData;
    //共享内存不需要释放，只需要构造析构函数即可--一般也不需要，因为在只能在进程中传递基础类型，不能传递std::string之类的结构

    //for(int i = 0; i < BufferSize; i++)  // 若放的不是指针，这句话删去
    //{
    //	pData[i].~T();    //如何显示调用模板类型的析构函数
    //}

    delete pImpl;
}

void ringbufferProcessSimple::pushData(const void* cargo)
{
    memcpy((pData+pSC->WriteIndex*DataSize),cargo,DataSize);
    pSC->WriteIndex=getNextCycle(pSC->WriteIndex,RingCountMax);
    //pSC->Count++;
}

void ringbufferProcessSimple::getData(void* cargo)
{
    memcpy(cargo,(pData+pSC->ReadIndex*DataSize),DataSize);
    pSC->ReadIndex=getNextCycle(pSC->ReadIndex,RingCountMax);
    //pSC->Count--;	
}

bool ringbufferProcessSimple::pushOnlySingle(const void* cargo, bool flag) // 往缓冲区放数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushOnlySingle(cargo,timeout);
}

bool ringbufferProcessSimple::pushOnlySingle(const void* cargo,unsigned int timeout)
{
    if(pImpl->semEmpty.Wait(timeout)==false)
    {
        return false;
    }

    {
        pushData(cargo);
    }

    pImpl->semFull.Signal();
    return true;
}

bool ringbufferProcessSimple::getOnlySingle(void* cargo,bool flag) // 取出缓冲区数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getOnlySingle(cargo,timeout);
}

bool ringbufferProcessSimple::getOnlySingle(void* cargo,unsigned int timeout)
{
    if(pImpl->semFull.Wait(timeout)==false)
    {
        return false;
    }
    {
       getData(cargo);
    }
    pImpl->semEmpty.Signal();
    return true;
}

bool ringbufferProcessSimple::pushByMultiple(const void* cargo, bool flag) // 往缓冲区放数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushByMultiple(cargo,timeout);
}

bool ringbufferProcessSimple::pushByMultiple(const void* cargo,unsigned int timeout)
{
    if(pImpl->semEmpty.Wait(timeout)==false)
    {
        return false;
    }
    {
       LockerGuard<MutexProcess> lock(pImpl->writeMutex);
       pushData(cargo);
    }
    pImpl->semFull.Signal();
    return true;
}

bool ringbufferProcessSimple::getByMultiple(void* cargo,bool flag) // 取出缓冲区数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getByMultiple(cargo,timeout);
}

bool ringbufferProcessSimple::getByMultiple(void* cargo,unsigned int timeout)
{
    if(pImpl->semFull.Wait(timeout)==false)
    {
        return false;
    }
    {
        LockerGuard<MutexProcess> lock(pImpl->readMutex);
        getData(cargo);
    }
    pImpl->semEmpty.Signal();
    return true;
}



bool ringbufferProcessSimple::clear(bool flag /*= true*/) // 清空缓冲区
{
    //可能设计不合理，待续
    AutoProcessLocker lockR(pImpl->readMutex);
    AutoProcessLocker lockW(pImpl->writeMutex);

    pImpl->semEmpty.ReSet();
    pImpl->semFull.ReSet();

    //读写位置复位
    pSC->WriteIndex = 0;
    pSC->ReadIndex = 0;
    //pSC->Count=0;	

    return true;
}


#ifdef UNIT_TEST

int Test_ringbufferSimple()
{
        std::cout<<"Test_ringbufferSimple()"<<std::endl;
    {

        std::cout<<"Test_ringbufferSimple()"<<std::endl;

        RingBufferSimple ring(sizeof(double),1000);
        for (int i=0;i<50;i++)
        {
            double v=i;
            ring.pushOnlySingle(&v,false);
            std::cout<<"put v= "<<v<<std::endl;
        }
        for (int i=0;i<50;i++)
        {
            double v;
            ring.getOnlySingle(&v,false);
            std::cout<<"get v= "<<v<<std::endl;
        }
    }


    {
        std::cout<<"ringbufferProcessSimple()"<<std::endl;

        ringbufferProcessSimple ring(sizeof(double),1000,"name");
        for (int i=0;i<50;i++)
        {
            double v=i;
            ring.pushOnlySingle(&v,false);
            std::cout<<"put v= "<<v<<std::endl;
        }


        {   
            //两个实例，模拟跨进程
            ringbufferProcessSimple ring(sizeof(double),1000,"name");
            for (int i=0;i<50;i++)
            {
                double v;
                ring.getOnlySingle(&v,false);
                std::cout<<"get v= "<<v<<std::endl;
            }
        }

    }

    return 0;
}

#endif

NAME_SPACE_MAIN_END
