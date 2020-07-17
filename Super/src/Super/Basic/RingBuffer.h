#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <iostream>
#include "ThreadSync.h"

//RingBuffer类模板 20150812 王领 雏形原版本见历史备份
//refactor by lizulin 2017-2020 解决bug与改进


//重构改封装跨平台的信号量，互斥锁等
//重构增加跨平台支持，增加考虑多个生产者/多消费者的情况


//注意理解使用:by lizulin 20180907
//实现生产者消费者模型不同线程生产/消费,除了用信号量控制货物个数外，
//还需要考虑不同线程同时生产(push);消费(get)问题
//关于互斥锁的使用,如果只有一个生产者线程，一个消费者线程，则不需要互斥锁
//如果有多个生产者则需要在推送函数内部用互斥锁保护WriteIndex
//如果有多个消费者者则需要在获取函数内部用互斥锁保护ReadIndex

//使用方法:
//一般只有一个生产者一个消费者的情况，如串联模式那么一个前一级push，后级get即可 
//用pushOnlySingle/getOnlySingle

//可能有多个生产者的情况，每个生产者必须有用pushMayMultipy推送数据。
//可能有多个消费者的情况，每个消费者必须用getMayMultipy获取数据。

//对于消息队列，一般可能存在多个线程同时push给一个线程，用pushMayMultipy
//消息队列内部线程一般只在其内部处理消息循环，用get即可




/*
◇判断“空”和“满”
上述的操作并不复杂，不过有一个小小的麻烦：空环和满环的时候，R和W都指向同一个位置！这样就无法判断到底是“空”还是“满”。大体上有两种方法可以解决该问题。
办法1：始终保持一个元素不用
当空环的时候，R和W重叠。当W比R跑得快，追到距离R还有一个元素间隔的时候，就认为环已经满。当环内元素占用的存储空间较大的时候，这种办法显得浪费空间。适用于多线程的情况。读写线程不会操作同一个变量。


办法2：维护额外变量
如果不喜欢上述办法，还可以采用额外的变量来解决。比如可以用一个整数记录当前环中已经保存的元素个数（该整数>=0）。当R和W重叠的时候，通过该变量就可以知道是“空”还是“满”。多线程需要加锁，否则不准确。
*/



template <class T,size_t inSize=1024/sizeof(T)>//默认不大于1k使用内部空间避免new =
class RingBuffer
{
    //T m_data[inSize+2];
    enum 
    {
        inArraySize=(inSize==0)?1:((inSize+1)*sizeof(T)),   //usesize=buffersize+1
    };
    //避免浪费或0字节数组
    char m_data[inArraySize];
private://disable copy
    RingBuffer(const RingBuffer&);
    RingBuffer& operator=(const RingBuffer&);
private:
    T *pData;                                         //指向存放数据的指针
    unsigned int Capacity;                     //缓冲区容量大小
    unsigned int RingCountMax;            //为了区分能区分WriteIndex=ReadIndex 是满还是空的情况，多开一个空间
    volatile unsigned int WriteIndex;      //当前写入位置
    volatile unsigned int ReadIndex;       //当前读取位置
    //int CurrCount;                                //当前货物数量 

    //原设计错误，读写过程不能共用一个互斥锁，否则用信号量就没有意义了。
    //递归锁/非递归锁 TODO：
    Mutex writeMutex;   
    Mutex readMutex;
    Semaphore semEmpty;
    Semaphore semFull;

    //SemaphoreCpp11 semEmpty;
    //SemaphoreCpp11 semFull;

    //std::mutex mtx;
    //std::condition_variable cv;
    //MyProcessMutex ProcessMutex;    //跨进程使用进程间互斥锁，当本类作为跨进程使用时候

    //一些特殊需求外部控制需要 为外部提供访问内部互斥锁与信号量的访问接口
    Mutex& getReadMutex(){return readMutex;}
    Mutex& getWriteMutex(){return writeMutex;}

    Semaphore& getEmptySemaphore(){return semEmpty;}
    Semaphore& getFullSemaphore(){return semFull;}
public:
    RingBuffer(const unsigned int bufferSize = 8);
    ~RingBuffer();

////////////////////////////////////////////////////////////////////////////////

    //默认版本--内部调用多生产者消费者版本,若不理解以下版本设计意图,可能效率稍低,但是肯定不会出错
    bool push(const T& cargo, bool flag){return pushMayMultiple(cargo,flag);}
    bool get(T& cargo,bool flag){return getMayMultiple(cargo,flag);}
    bool push(const T& cargo,unsigned int timeout=Wait_Dafault){return pushMayMultiple(cargo,timeout);}
    bool get(T& cargo,unsigned int timeout=Wait_Dafault){return getMayMultiple(cargo,timeout);}


    //推送/获取一个数据--确定只存在单生产者/单消费者线程的版本
    // 往缓冲区读写数据,否则当flag为true，则无限等待，false则默认时间等待
    bool pushOnlySingle(const T& cargo, bool flag);
    bool getOnlySingle(T& cargo,bool flag);  //T get();原设计用返回NULL表明取失败不科学,如果是整数缓冲区NULL为0是有意义的
    //往缓冲区读写数据 以timeout作为延时,time=0XFFFFFFFF则无限等待
    bool pushOnlySingle(const T& cargo,unsigned int timeout=Wait_Dafault);      //不能设定默认参数，否则调用不明确
    bool getOnlySingle(T& cargo,unsigned int timeout=Wait_Dafault);     

    //多生产者或多消费者版本
    bool pushMayMultiple(const T& cargo, bool flag);
    bool getMayMultiple(T& cargo,bool flag);
    bool pushMayMultiple(const T& cargo,unsigned int timeout=Wait_Dafault);
    bool getMayMultiple(T& cargo,unsigned int timeout=Wait_Dafault);     



////////////////////////////////////////////////////////////////////////////////
//一次推送/获取多个数据版  用windows信号量WaitForMultipleObjects最大只支持64，暂时屏蔽 用C++11信号量应该可以
#if 0
    //单生产者消费者版本
    bool pushN(const T* pCargo,unsigned int N,unsigned int timeout=Wait_Dafault);
    bool getN(T* pCargo,unsigned int N,unsigned int timeout=Wait_Dafault);
    //多生产者消费者版本
    bool pushNByMultiple(const T* pCargo,unsigned int N,unsigned int timeout=Wait_Dafault);
    bool getNByMultiple(T* pCargo,unsigned int N,unsigned int timeout=Wait_Dafault);

    //默认时间版本
    bool pushN(const T* pCargo,unsigned int N,bool flag);
    bool getN(T* pCargo,unsigned int N,bool flag);
    bool pushNByMultiple(const T* pCargo,unsigned int N,bool flag);
    bool getNByMultiple(T* pCargo,unsigned int N,bool flag);
#endif

    //
    //以覆盖的形式推送数据,若缓冲区已经满,取出一部分再推送。
    bool pushOnlySingleCover(const T& cargo, bool flag);
    bool pushOnlySingleCover(const T& cargo,unsigned int timeout=Wait_Dafault);
    bool getNullDataOnlySingle(unsigned int timeout=Wait_Dafault);    //释放一个数据,不取出

    //高效扩展:注意理解并使用
    //高效推送数据,对于比较大的结构T,可以直接获取即将推送数据地址,填充数据后，释放对应信号量，
    //相当于把原先一个push过程拆分成三个步骤实现,避免大结构填充拷贝
    bool SearchPush(T*& pCargo,bool flag);     
    bool SearchPush(T*& pCargo,unsigned int timeout=Wait_Dafault);     
    void storeData(const T& cargo);
    void storeDataByLock(const T& cargo);
    void ReleasePush();


    //高效扩展:注意理解并使用
    //有些时候仅仅是想搜索RingBuffer内部结构是否符合条件。符合再取出,不符合就搜索下一个
    //对于比较大的结构T，每次都获取会严重影响效率。
    //改进设计:当数据有效先返回指针使用,判断是否符合条件,符合再加载数据本身,否则释放信号量,继续下一个
    //相当于把原先一个get过程拆分成三个步骤实现（为了搜索效率）

    bool SearchGet(T*& pCargo,bool flag);     
    bool SearchGet(T*& pCargo,unsigned int timeout=Wait_Dafault);     
    void loadData(T& cargo);
    void loadDataByLock(T& cargo);   //多线程加锁
    void ReleaseGet();


    //待废弃
    ////仅查询，不影响内部计数器，既不push也不get,查询队列头/尾内容
    //bool CheckHead(T& cargo);
    //bool CheckTail(T& cargo);
    //bool Search(T& cargo,unsigned int& CurrIndex);



    //高效扩展2:先搜索可用区域返回其引用，与内部索引，若符合条件则取数据，并清理此前的
    bool SearchNextN(T*& pCargo,unsigned int& nextIndexNum)
    {
        if (nextIndexNum>=semFull.AvailableCount())
        {
            //printf("nextIndexNum>=semFull.AvailableCount():%u\n",semFull.AvailableCount());
            return false;
        }
        unsigned int willSearchIndex=getNextNCycle(ReadIndex,nextIndexNum,RingCountMax);  //Capacity
        pCargo=&pData[willSearchIndex];
        nextIndexNum=getNextCycle(nextIndexNum,RingCountMax); //注意语义Capacity or RingCountMax
        return true;
    }
    void ClearBeforeN(unsigned int IndexNum)
    {
        for (unsigned int n=0;n<IndexNum;n++)
        {
            semFull.Wait(1);       //配合SearchNext使用,肯定是有信号量的，不需要判断
            //移动数据指针,为了效率不取数据
            ReadIndex=getNextCycle(ReadIndex,RingCountMax);   //Capacity
            semEmpty.Signal();
        }
    }


    unsigned int getCapacity() const {return Capacity;}        // 返回总缓冲区大小，初始化时候决定
    unsigned int getWritePos() const {return WriteIndex;}      // 获取写入位置
    unsigned int getReadPos() const {return ReadIndex;}       // 获取读取位置
    bool IsFull()  const                                                           // 改缓冲区是否满标记 lizulin
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
        return WriteIndex==ReadIndex;  //faster
        //if(getUsedSpace()==0)
        //{ 
        //    return true; 
        //}
        //return false;
    }

    //返回缓冲区中货物数量 //读写可能在不同线程，不能用一个计数器直接计数，加锁又影响效率，此返回值不精确
    unsigned int getUsedSpace() const 
    {
        if (WriteIndex>=ReadIndex)
        {
            return  WriteIndex-ReadIndex;
        }
        else
        {
            //realCnt=(Capacity-ReadIndex)+WriteIndex;
            return  (RingCountMax-ReadIndex)+WriteIndex;
        }
    }

    unsigned int getUnUsedSpace() const 
    {
        return Capacity-getUsedSpace();
    }

    bool clear(bool flag = true);                        // 清空缓冲区--flag为true表明存放的是new出来的指针,需要释放

    //回收ringbufer里面保存的内容，如果是new出来的指针，让外部可以有机会释放，否则不处理返回再即可
    //std::vector<T> clear()
    //{
    //    std::vector<T> VT;
    //    while (1)
    //    {
    //        T tmp;
    //        if (!get(tmp,0))   //立即获取，不等待，失败说明队列为空，退出
    //        {
    //            break;
    //        }
    //        VT.push_back(tmp);
    //    }
    //    return VT;
    //}
    void printStatus() const
    {
        //printf("RingBuffer:%p Total:%u ReadPos:%u WritePos:%u UsedSpace:%u\n",
         //   this,getTotalSize(),getReadPos(),getWritePos(),getUsedSpace());
        printf("ReadPos:%u WritePos:%u Total:%u UsedSpace:%u\n",
            getReadPos(),getWritePos(),getCapacity(),getUsedSpace());
    }

private:
    //实际直接存取数据 私有
    void pushData(const T& cargo);
    void getData(T& cargo);
};



template <class T,size_t inSize>
RingBuffer<T,inSize>::RingBuffer(const unsigned int bufferSize):
    semEmpty(bufferSize,bufferSize)
    ,semFull(bufferSize,0)
{
    //PRINT_FUN_TRACE

    Capacity = bufferSize;
    RingCountMax=Capacity+1;  //多开一个，否则windex/rindex无法表示刚好等于满容量的情况
    WriteIndex = 0;
    ReadIndex = 0;
    //CurrCount = 0;

    //当数据容量较小为了效率使用内部空间
    size_t needBufferSize=RingCountMax*sizeof(T);
    if (needBufferSize<=inArraySize)    //
    {
          pData=(T*)m_data;
          PreDprintf("use internal mem space\n");
    }
    else
    {
        //pData=new T[RingCountMax];
        pData=new(std::nothrow) T[RingCountMax];
        if (pData==NULL)
        {
            PreDprintf("new failed  num:%d size:%d\n",RingCountMax,needBufferSize);
        }
        else
        {
            PreDprintf("new success  num:%d size:%d\n",RingCountMax,RingCountMax*sizeof(T));
        }
    }
}


template <class T,size_t inSize>
RingBuffer<T,inSize>::~RingBuffer()
{
    if (pData!=(T*)m_data)  //若使用的是new空间则释放
    {
         delete[] pData;
         pData=(T*)m_data;
    }
}


template <class T,size_t inSize>
void RingBuffer<T,inSize>::pushData(const T& cargo)
{
    pData[WriteIndex] = cargo;
    WriteIndex=getNextCycle(WriteIndex,RingCountMax);  //Capacity
    //CurrCount++;   //有效货物计数器，暂不使用，会有线程竞争问题
}

template <class T,size_t inSize>
void RingBuffer<T,inSize>::getData(T& cargo)
{
    cargo = pData[ReadIndex];
    ReadIndex=getNextCycle(ReadIndex,RingCountMax);   //Capacity
    //CurrCount--;
}


template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushOnlySingle(const T& cargo, bool flag)  // 往缓冲区放数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;  //三目运算符与if else效率差异待确认
    return pushOnlySingle(cargo,timeout);
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::getOnlySingle(T& cargo,bool flag)  // 取出缓冲区数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getOnlySingle(cargo,timeout);
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushOnlySingle(const T& cargo,unsigned int timeout)
{
    if(!semEmpty.Wait(timeout))
    {
        return false;
    }

    {
         //LockerGuard<Mutex> lock(writeMutex);
         pushData(cargo);
    }

    semFull.Signal();
    return true;
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::getOnlySingle(T& cargo,unsigned int timeout)
{
    if(!semFull.Wait(timeout))
    {
        return false; 
    }
 
    {
        //LockerGuard<Mutex> lock(readMutex);
        getData(cargo);
    }

    semEmpty.Signal();
    return true;
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushMayMultiple(const T& cargo, bool flag)  // 往缓冲区放数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushMayMultiple(cargo,timeout);
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::getMayMultiple(T& cargo,bool flag)  // 取出缓冲区数据
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getMayMultiple(cargo,timeout);
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushMayMultiple(const T& cargo,unsigned int timeout)
{
    if(!semEmpty.Wait(timeout))
    {
        return false;
    }

    {
        LockerGuard<Mutex> lock(writeMutex);
        pushData(cargo);
    }

    semFull.Signal();
    return true;
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::getMayMultiple(T& cargo,unsigned int timeout)
{
    if(!semFull.Wait(timeout))
    {
        return false; 
    }

    {
        LockerGuard<Mutex> lock(readMutex);
        getData(cargo);
    }

    semEmpty.Signal();
    return true;
}


#if 0
template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushN(const T* pCargo,unsigned int N,unsigned int timeout/*=Wait_Dafault*/)
{
    //N=N%BufferSize;	//大于容量则取余,或者外部控制N不大于总容量
    if(!semEmpty.Wait(N,timeout))
    {
        return false;
    }

        for(unsigned int n=0;n<N;n++)
        {
            pushData(pCargo[n]);
        }

    semFull.Signal(N);
    return true;
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::getN(T* pCargo,unsigned int N,unsigned int timeout=Wait_Dafault)
{
    if(!semFull.Wait(N,timeout))
    {
        return false; 
    }		

    for(unsigned int n=0;n<N;n++)
    {
        getData(pCargo[n]);
    }

    semEmpty.Signal(N);
    return true;
}

//多生产者消费者版本
template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushNByMultiple(const T* pCargo,unsigned int N,unsigned int timeout=Wait_Dafault)
{
    if(!semEmpty.Wait(N,timeout))
    {
        return false;
    }
    {
        MyAutoLocker lock(writeMutex);
        for(unsigned int n=0;n<N;n++)
        {
            pushData(pCargo[n]);
        }
    }
    semFull.Signal(N);
    return true;
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::getNByMultiple(T* pCargo,unsigned int N,unsigned int timeout=Wait_Dafault)
{
    if(!semFull.Wait(N,timeout))
    {
        return false; 
    }

    {
        MyAutoLocker lock(readMutex);
        for(unsigned int n=0;n<N;n++)
        {
            getData(pCargo[n]);
        }
    }

    semEmpty.Signal(N);
    return true;
}

//默认时间版本
template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushN(const T* pCargo,unsigned int N,bool flag)
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushN(cargo,N,timeout);
}
template <class T,size_t inSize>
bool RingBuffer<T,inSize>::getN(T* pCargo,unsigned int N,bool flag)
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getN(cargo,N,timeout);
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushNByMultiple(const T* pCargo,unsigned int N,bool flag)
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushNByMultiple(cargo,N,timeout);
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::getNByMultiple(T* pCargo,unsigned int N,bool flag)
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return getNByMultiple(cargo,N,timeout);
}
#endif


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushOnlySingleCover(const T& cargo, bool flag)
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return pushOnlySingleCover(cargo,timeout);
}
template <class T,size_t inSize>
bool RingBuffer<T,inSize>::pushOnlySingleCover(const T& cargo,unsigned int timeout)
{
    bool retPush=pushOnlySingle(cargo,timeout);
    if (!retPush)
    {
        //推送数据失败,取出一个分数据再推送
        getNullDataOnlySingle(timeout);
    }
    //再次推送
    retPush=pushOnlySingle(cargo,timeout);
    return retPush;
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::getNullDataOnlySingle(unsigned int timeout/*=Wait_Dafault*/)
{
    if(!semFull.Wait(timeout))
    {
        return false; 
    }

    {
        //MyAutoLocker lock(readMutex);
        //getData(cargo);

      //移动数据指针,为了效率不取数据
       ReadIndex=getNextCycle(ReadIndex,RingCountMax);   //Capacity
    }

    semEmpty.Signal();
    return true;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template <class T,size_t inSize>
bool RingBuffer<T,inSize>::SearchPush(T*& pCargo,bool flag)
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return SearchPush(pCargo,timeout);
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::SearchPush(T*& pCargo,unsigned int timeout)
{
    if(!semEmpty.Wait(timeout))
    {
        return false; 
    }

    //取数据
    //cargo = pData[ReadIndex];
    //ReadIndex=getNextCycleIndex(ReadIndex,RingCountMax);   //Capacity

    //暂不取数据本身,返回数据指针给调用者使用。
    pCargo=&pData[WriteIndex];

    //getData();
    //semEmpty.Signal();
    return true;
}

template <class T,size_t inSize>
void RingBuffer<T,inSize>::storeData(const T& cargo)
{
    //取数据本身
     pData[WriteIndex]=cargo;
}
template <class T,size_t inSize>
void RingBuffer<T,inSize>::storeDataByLock(const T& cargo)
{
    LockerGuard<Mutex> lock(writeMutex);
    return storeData(cargo);
}
template <class T,size_t inSize>
void RingBuffer<T,inSize>::ReleasePush()
{
    //释放信号量并移动读指针
    WriteIndex=getNextCycle(WriteIndex,RingCountMax);   //Capacity
    semFull.Signal();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::SearchGet(T*& pCargo,bool flag)
{
    unsigned int timeout=(flag)?Wait_INFINITE:Wait_Dafault;
    return SearchGet(pCargo,timeout);
}

template <class T,size_t inSize>
bool RingBuffer<T,inSize>::SearchGet(T*& pCargo,unsigned int timeout)
{
    if(!semFull.Wait(timeout))
    {
        return false; 
    }

    //取数据
    //cargo = pData[ReadIndex];
    //ReadIndex=getNextCycleIndex(ReadIndex,RingCountMax);   //Capacity

    //暂不取数据本身,返回数据指针给调用者使用。
    pCargo=&pData[ReadIndex];

    //getData();
    //semEmpty.Signal();
    return true;
}

template <class T,size_t inSize>
void RingBuffer<T,inSize>::loadData(T& cargo)
{
    //取数据本身
    cargo = pData[ReadIndex];
}

template <class T,size_t inSize>
void RingBuffer<T,inSize>::loadDataByLock(T& cargo)
{
    LockerGuard<Mutex> lock(readMutex);
    return loadData(cargo);
}

template <class T,size_t inSize>
void RingBuffer<T,inSize>::ReleaseGet()
{
    //释放信号量并移动读指针
    ReadIndex=getNextCycle(ReadIndex,RingCountMax);   //Capacity
    semEmpty.Signal();
}

//
//template <class T,size_t inSize>
//bool RingBuffer<T,inSize>::CheckHead(T& cargo)
//{
//    AutoLocker lock(readMutex);
//    if (CurrCount<=0)    //有货物
//    {
//        return false;
//    }
//    //unsigned int RIndex=getPrevCycleIndex(WriteIndex,BufferSize);  //取即将写的上一个
//    //cargo = pData[RIndex];   //队列头部
//
//    cargo = pData[ReadIndex];
//    return true;
//}
//
//template <class T,size_t inSize>
//bool RingBuffer<T,inSize>::CheckTail(T& cargo)
//{
//    AutoLocker lock(readMutex);
//
//    if (CurrCount<=0)
//    {
//        return false;
//    }
//    //unsigned int RIndex=getPrevCycleIndex(ReadIndex,BufferSize);   //取即将读的上一个
//    //cargo = pData[RIndex];  //队列尾部，准备写入位置
//    cargo = pData[ReadIndex];
//    return true;
//}
//
//
//template <class T,size_t inSize>
//bool RingBuffer<T,inSize>::Search(T& cargo,unsigned int& CurrIndex)
//{
//    //WriteIndex/ReadIndex分别用读/写 锁保护 应该同时互斥才正确
//    LockerGuard<Mutex> lockr(readMutex);
//    LockerGuard<Mutex> lockw(writeMutex);   
//
//    //if (CurrIndex>=WriteIndex||CurrIndex<ReadIndex)   //无效区间
//    if (WriteIndex>=ReadIndex)
//    {
//        if (CurrIndex>=WriteIndex||CurrIndex<ReadIndex)
//        {
//            printf("WriteIndex:%u ReadIndex:%u CurrIndex:%u\n",WriteIndex,ReadIndex,CurrIndex);
//
//            return false;
//        }
//    }
//    else
//    {
//        if (CurrIndex>=WriteIndex&&CurrIndex<ReadIndex)
//        {
//            printf("WriteIndex:%u ReadIndex:%u CurrIndex:%u\n",WriteIndex,ReadIndex,CurrIndex);
//
//            return false;
//        }
//    }
//
//    cargo = pData[CurrIndex];
//    CurrIndex=getNextCycle(CurrIndex,RingCountMax);   //取即将读的下一个  Capacity
//    return true;
//}


template <class T,size_t inSize>
bool RingBuffer<T,inSize>::clear(bool flag )  // 清空缓冲区
{
    //可能设计不合理，待续

    //WriteIndex/ReadIndex分别用读/写 锁保护 应该同时互斥才正确
    LockerGuard<Mutex> lockR(readMutex);
    LockerGuard<Mutex> lockW(writeMutex);   

    semEmpty.ReSet();
    semFull.ReSet();


    //读写位置复位
    WriteIndex = 0;
    ReadIndex = 0;
    //CurrCount = 0;

    return true;
}

NAME_SPACE_MAIN_END

#endif // RINGBUFFER_H_

