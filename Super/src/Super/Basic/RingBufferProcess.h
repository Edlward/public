#ifndef RINGBUFFERPROCESS_H_
#define RINGBUFFERPROCESS_H_
//#include "RingBufferProcess.h"   //跨进程环形缓冲区

#include "global.h" 

//refactoring  by lizulin 李祖林 重构
//重构改封装跨平台的信号量，互斥量 
#include <iostream>
#include <string>

#include "ThreadSync.h"
#include "ShareMemory.h"


 NAME_SPACE_MAIN_BEGIN

//改通过命名信号量，实现跨进程的ringbufer，一个进程里面push，另一个进程里面get，
//通过共享内存实现数据交互
template <class T>
class RingBufferProcess
{
private://disable copy
    RingBufferProcess(const RingBufferProcess&);
    RingBufferProcess& operator=(const RingBufferProcess&);
private:
    T *pData;                                            // 存放数据的指针
    unsigned int Capacity;                        // 缓冲区大小
    unsigned int RingCountMax;              //为了区分能区分WriteIndex=ReadIndex 是满还是空的情况，多开一个空间
    //读写可能在不同线程，需要把一些计数器变量在进程间共享，考虑避免多次初始化问题
    struct ShareCount
    {
        int initKey;                                        //创建者标记，决定是否初始化以下变量
        volatile unsigned int WriteIndex;       // 当前写入位置
        volatile unsigned int ReadIndex;        // 当前读取位置
        volatile unsigned int CurrCount;         //记录有效货物数量 A count for judge buffer empty or not
    };
    ShareCount* pSC;
    ShareMemory m_ShareMem;
    Semaphore semEmpty;
    Semaphore semFull;

    //跨进程使用进程间互斥锁，当本类作为跨进程使用时候
    MutexProcess writeMutex;
    MutexProcess readMutex;

    MutexProcess initMutex;  //构造共享变量保护锁，避免极低概率，不同进程同时初始化
public:
    //注意:不同进程共享双方构造传参必须一致。
    inline RingBufferProcess(const unsigned int bufferSize=8,const char* name="");
    inline ~RingBufferProcess();

    //默认版本--内部调用多生产者消费者版本,若不理解以下版本设计意图,可能效率稍低,但是肯定不会出错
    bool push(const T& cargo, bool flag){return pushMayMultiple(cargo,flag);}
    bool get(T& cargo,bool flag){return getMayMultiple(cargo,flag);}
    bool push(const T& cargo,unsigned int timeout=Wait_Dafault){return pushMayMultiple(cargo,timeout);}
    bool get(T& cargo,unsigned int timeout=Wait_Dafault){return getMayMultiple(cargo,timeout);}


    //单生产者单消费者版本
    // 往缓冲区写数据,flag为true，则无限等待，false则有限时间内没有空位则丢弃数据
    inline bool pushOnlySingle(const T& cargo, bool flag);           //为了效率用引用
    inline bool getOnlySingle(T& cargo,bool flag);           
    //T get(bool flag = true); //原设计，返回NULL不科学，如果是整数缓冲区null为0是有意义的

    //往缓冲区读写数据 以timeout作为延时,time=0XFFFFFFFF则无限等待
    inline bool pushOnlySingle(const T& cargo,unsigned int timeout=Wait_Dafault);      //不能设定默认参数，否则调用不明确
    inline bool getOnlySingle(T& cargo,unsigned int timeout=Wait_Dafault);     

    //多生产者或多消费者版本
    bool pushMayMultiple(const T& cargo, bool flag);
    bool getMayMultiple(T& cargo,bool flag);
    bool pushMayMultiple(const T& cargo,unsigned int timeout=Wait_Dafault);
    bool getMayMultiple(T& cargo,unsigned int timeout=Wait_Dafault);     

/*
    //跨进程传递共享内存指针使用,推送指针前转成相对地址在推送,取到指针后再映射成本进程指针可以直接使用--只能在跨进程中传递同名共享内存指针
    inline bool PushShareMemPoint(const T cargo, bool flag = true); // 往缓冲区写数据,flag为true，则无限等待，false则有限时间内没有空位则丢弃数据
    inline bool GetShareMemPoint(T& cargo,bool flag = true);          // 取出缓冲区数据,flag为true，则无限等待，false则有限时间内没取到数据返回空
    void* pShareMem;     //共享内存在本进程映射地址
    inline void setShareMemPoint(void* p);
*/

    inline unsigned int getCapacity() const {return Capacity;}       //返回总缓冲区大小，初始化时候决定
    inline unsigned int getWritePos() const {return pSC->WriteIndex;}    //获取写入位置
    inline unsigned int getReadPos() const {return pSC->ReadIndex;}      //获取读取位置
    inline bool IsFull() const 
    {
        //if(CurrCount >=Capacity)
        if(getUsedSpace()>=Capacity)
        { 
            return true; 
        }
        return false;
    }
    inline bool IsEmpty() const 
    {
        return pSC->WriteIndex==pSC->ReadIndex;  //faster
        //if(getUsedSpace()==0)
        //{ 
        //    return true; 
        //}
        //return false;
    }

        //返回缓冲区中货物数量
    unsigned int getUsedSpace() const
    {
        //return pSC->CurrCount;   //读可能写在不同线程，不能用一个计数器之间计数，

        if (pSC->WriteIndex>=pSC->ReadIndex)
        {
            return pSC->WriteIndex-pSC->ReadIndex;
        }
        else
        {
            return  (RingCountMax-pSC->ReadIndex)+pSC->WriteIndex;
        }
    }
    inline bool clear(bool flag = true);          // 清空缓冲区--flag为true表明存放的是new出来的指针,需要释放
    
    //特殊应用暂时如此
    bool ClearifIsFullWithLock();
    bool ClearifIsFull();

    bool ClearifIsFullByGetAll();
    void getAlltoClear();


    void printStatus() const
    {
        //printf("RingBuffer:%p Total:%u ReadPos:%u WritePos:%u UsedSpace:%u\n",
        //   this,getTotalSize(),getReadPos(),getWritePos(),getUsedSpace());
        printf("ReadPos:%u WritePos:%u Total:%u UsedSpace:%u semFull:%d semEmpty:%d\n",
            getReadPos(),getWritePos(),getCapacity(),getUsedSpace(),semFull.AvailableCount(),semEmpty.AvailableCount());
    }
private:
    //实际直接存取数据 私有
    void pushData(const T& cargo);
    void getData(T& cargo);
};




template <class T>
RingBufferProcess<T>::RingBufferProcess(const unsigned int bufferSize,const char* name): 
    semEmpty(bufferSize,bufferSize,std::string(std::string(name)+"_semEmpty").c_str())
    ,semFull(bufferSize,0,std::string(std::string(name)+"_semFull").c_str())
    ,writeMutex(std::string(std::string(name)+"_writeMutex").c_str())
    ,readMutex(std::string(std::string(name)+"_readMutex").c_str())
    ,initMutex(std::string(std::string(name)+"_initMutex").c_str())
    ,m_ShareMem(bufferSize*sizeof(T)+sizeof(ShareCount),std::string(std::string(name)+"_privateMem").c_str())
{
    Capacity=bufferSize;
    RingCountMax=Capacity+1;
    //RingCountMax=Capacity;
    //pData = new T[BufferSize];

    //用进程间共享内存构造对象
    void* pShare=m_ShareMem.getAddress();
    //pData=operator new(pShare) T[BufferSize];
    pData=(T*)pShare;

    //进程间共享计数器的初始化
    pSC=(ShareCount*)((char*)pShare+RingCountMax*sizeof(T));
    {
        //构造共享变量保护锁，避免极低概率，不同进程同时初始化
        LockerGuard<MutexProcess> lock(initMutex);

        for(unsigned int n = 0; n <RingCountMax; n++)
        {
            //pData[i].T::T();   //显示调用构造函数,不行
            new(&pData[n]) T;
        }

        if (pSC->initKey!=0X08250512)    //共享内存创建默认会清零,所以指定一个任意值或者bool也是可以的
        {
            pSC->WriteIndex=0;                // 当前写入位置
            pSC->ReadIndex=0;                // 当前读取位置
            pSC->CurrCount=0;	
            pSC->initKey=0X08250512;
        }
    }
}


template <class T>
RingBufferProcess<T>::~RingBufferProcess()  
{

    //共享内存不需要释放，只需要构造析构函数即可--一般也不需要，因为在只能在进程中传递基础类型，不能传递std::string之类的结构
    for(unsigned int n = 0; n <RingCountMax; n++) 
    {
          pData[n].~T();    //显示调用模板类型的析构函数
    }
}

template <class T>
void RingBufferProcess<T>::pushData(const T& cargo)
{
    pData[pSC->WriteIndex] = cargo;
    pSC->WriteIndex=getNextCycle(pSC->WriteIndex,RingCountMax);      //pSC->或BufferSize  Capacity
    //pSC->CurrCount++;  //有效货物计数器，暂不使用，会有线程竞争问题
}

template <class T>
void RingBufferProcess<T>::getData(T& cargo)
{
    cargo = pData[pSC->ReadIndex];
    pSC->ReadIndex=getNextCycle(pSC->ReadIndex,RingCountMax);  //pSC->或BufferSize  Capacity
    //pSC->CurrCount--;
}

template <class T>
bool RingBufferProcess<T>::pushOnlySingle(const T& cargo, bool flag)  // 往缓冲区放数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushOnlySingle(cargo,timeout);
}

template <class T>
bool RingBufferProcess<T>::getOnlySingle(T& cargo,bool flag)  // 取出缓冲区数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getOnlySingle(cargo,timeout);
}

template <class T>
bool RingBufferProcess<T>::pushOnlySingle(const T& cargo,unsigned int timeout)
{
    if(!semEmpty.Wait(timeout))
    {
        return false;
    }

    {
        //LockerGuard<MutexProcess> lock(writeMutex);
         pushData(cargo);
    }

    semFull.Signal();
    return true;
}

template <class T>
bool RingBufferProcess<T>::getOnlySingle(T& cargo,unsigned int timeout)
{
    if(!semFull.Wait(timeout))
    {
        return false;
    }

    {
        //LockerGuard<MutexProcess> lock(readMutex);
        getData(cargo);
    }

    semEmpty.Signal();
    return true;
}

template <class T>
bool RingBufferProcess<T>::pushMayMultiple(const T& cargo, bool flag)  // 往缓冲区放数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushMayMultiple(cargo,timeout);
}

template <class T>
bool RingBufferProcess<T>::getMayMultiple(T& cargo,bool flag)  // 取出缓冲区数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getMayMultiple(cargo,timeout);
}

template <class T>
bool RingBufferProcess<T>::pushMayMultiple(const T& cargo,unsigned int timeout)
{
    if(!semEmpty.Wait(timeout))
    {
        return false;
    }

    {
        LockerGuard<MutexProcess> lock(writeMutex);
        pushData(cargo);
    }

    semFull.Signal();
    return true;
}

template <class T>
bool RingBufferProcess<T>::getMayMultiple(T& cargo,unsigned int timeout)
{
    if(!semFull.Wait(timeout))
    {
        return false; 
    }

    {
        LockerGuard<MutexProcess> lock(readMutex);
        getData(cargo);
    }

    semEmpty.Signal();
    return true;
}

template <class T>
bool RingBufferProcess<T>::clear(bool flag)  // 清空缓冲区
{
    //可能设计不合理，待续
    AutoProcessLocker lockR(readMutex);
    AutoProcessLocker lockW(writeMutex);

    semEmpty.ReSet();
    semFull.ReSet();
    //读写位置复位
    pSC->WriteIndex = 0;
    pSC->ReadIndex = 0;
    //pSC->CurrCount = 0;
    return true;
}


template <class T>
bool RingBufferProcess<T>::ClearifIsFullWithLock()
{
    LockerGuard<MutexProcess> lockR(readMutex);
    LockerGuard<MutexProcess> lockW(writeMutex);
    return ClearifIsFull();
}

template <class T>
bool RingBufferProcess<T>::ClearifIsFull()
{
    if (IsFull())
    {
        //不能clear() 可能会死锁，暂时没有跨进程递归锁
        semEmpty.ReSet();
        semFull.ReSet();
        //读写位置复位
        pSC->WriteIndex = 0;
        pSC->ReadIndex = 0;
        //pSC->CurrCount = 0;
        return true;
    }
    return false;
}


template <class T>
bool RingBufferProcess<T>::ClearifIsFullByGetAll()
{
    //LockerGuard<MutexProcess> lockR(readMutex);
    //LockerGuard<MutexProcess> lockW(writeMutex);

    if (IsFull())
    {
        getAlltoClear();
        return true;
    }
    return false;
}

template <class T>
void RingBufferProcess<T>::getAlltoClear()
{
    T tmp;
    while (true)
    {   
       if (!this->getMayMultiple(tmp,0u))
       //if (!this->getOnlySingle(tmp,0u))     //若加锁版本只能用此,否则锁递归
       {
           return;
       } 
    }
}


//显示实例化
//template class RingBufferProcess<int>;
//template class RingBufferProcess<cardInput*>;


//template<class T> class B { ..... }; // 普通版本
//template<class T>class B<T*> { ..... }; //这个偏特化版本只接收指针类型的模板实参 
//template<class T>class B<T&> { ..... }; // 这个偏特化版本只接受引用类型的模板实参


//对指针使用共享内存方式的特化  T只能是
template <class T>
class RingbufferProcessWrap
{
public:
    RingbufferProcessWrap(const unsigned int bufferSize = 8,const char* name=""):
        ProcessShareRingbuffer(bufferSize,std::string(std::string(name)+"RingBuffer").c_str())
        ,ShareMemAudioBuf(sizeof(T)*bufferSize,std::string(std::string(name)+"shareMem").c_str())
    {
        pShareMemAddress=ShareMemAudioBuf.getAddress(); 
        IndexBuf=0;
        this->BufferSize=bufferSize;
    }
    ~RingbufferProcessWrap()
    {

    }
    T* getNextBufferFromShareMem()
    {
        IndexBuf=getNextCycle(IndexBuf,this->BufferSize);
        return pShareMemAddress+IndexBuf*sizeof(T);
    }
    inline bool push(const T*& cargo, bool flag)
    {
            unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
            return push(cargo,timeout);
    }
    inline bool get(T*& cargo,bool flag)
    {
            unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
            return get(cargo,timeout);
    }
   inline bool push(const T*& cargo,unsigned int timeout=Wait_Dafault)
   {
       T*  inPush=(T*)((char*)cargo-(char*)pShareMemAddress);
       return ProcessShareRingbuffer.push(cargo,timeout);
   }
   inline bool get(T*& cargo,unsigned int timeout=Wait_Dafault)
   {
        bool ret=ProcessShareRingbuffer.get(cargo,timeout);
        T* out=(T*)((char*)cargo+(uint64_t)(pShareMemAddress)); 
        cargo=out;
        return ret;
   }
private:
    RingBufferProcess<T*> ProcessShareRingbuffer;
    ShareMemory ShareMemAudioBuf;
    void* pShareMemAddress;
    unsigned int BufferSize;
    unsigned int IndexBuf;
};


 NAME_SPACE_MAIN_END
#endif // RINGBUFFERPROCESS_H_


