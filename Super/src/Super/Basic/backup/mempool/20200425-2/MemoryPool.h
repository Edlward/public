#ifndef MemoryPool_H
#define MemoryPool_H

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <stdint.h>

#include <mutex> 
#include "global.h"
#include "Math.h"

//#include "Thread.h"              //test sleep()
#include "ThreadSync.h"        //for mutex

//create by lizulin 20190521
//我的实现思想备忘:
//地址校验功能，如果用非本内存池申请的指针去释放要判断
//进阶:基于模板实现，自动调用构造函数与析构函数
//每个类内置内存池，？
//简单定长,定个数内存池
//自动增长，动态申请内存池
//不能用系统malloc申请然后用内存池释放，可能越界崩溃,应该由程序员保证。
//释放内存归还需要遍历容器，（除非chunk里面保存有其在已使用列表的位置，然后直接找到位置,标记废弃不删除）
//或用记住循环链表，前后级指针，删除时候不需要遍历列表
//一种可以快速新增删除指针的容器--待续
//重复释放的考虑，增加字段判断，没有必要，应该由使用者保证
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//不再管理已经分配的MemChunk,因为其push操作内部调用了new node,失去内存池O1申请的意义了
////释放已经使用
//DoubleListNode<MemChunk*>* pNode=ListUsed.getBegin();
//while (1)
//{
//    pNode=ListUsed.hasNext(pNode);
//    if (pNode==NULL)
//    {
//        break;
//    }
//    MemChunk* pChunk=ListUsed.getData(pNode);
//    delete pChunk;
//}

//不再管理已经分配的MemChunk,因为其push操作内部调用了new node,失去内存池O1申请的意义了
//用chunk在listUsed中地址去快速删除,可以做到O1,否则只能遍历，那么此内存池意义就不大了
//ListUsed.removeNodeByItsAddrress(pParentChunk->pNodeAtList);     

//按照原先设计,用另一个容器保存已经被内存池分配出去的MemChunk指针，以便于最后清理释放，
//但是考虑到如果被本内存池分配出去而没有归还，应该不再管理，
//再者DoubleListFastDel容器可以做到快速O1删除节点，但是push内部还是调用new node操作，失去了内存池的意义。
//std::vector<MemChunk*> ListUsed;  //已经分配出去在使用的 直接使用std::vector 没有办法快速删除某个节点。
//DoubleList<MemChunk*> ListUsed; //已经分配出去在使用的，使用自定义可以快速根据指针删除节点的容器






NAME_SPACE_MAIN_BEGIN

//为了不损失性能,由模板参数决定行为
class DevideType;
class DevideType1;
class DevideType2;
class DevideType3;
class DevideType4;
class DevideType5;


template<class DevideType> class  MemBlock;
template<class DevideType> class  MemPool;
template<class DevideType> class  MemChunk;

template<class DevideType>
struct MemHeader
{
    size_t memSize; 
    MemBlock<DevideType>* pBlock;   //记住所在Block指针,便于快速释放,而不是根据大小寻找
    MemChunk<DevideType>* pChunk;  //保存自己所在Chunk的地址

    ////////////////////////////////////////////////////////////////////////////////
    //完善备忘:要释放的地址是否是通过本内存池申请的判断,理论上存在外部地址p之前的内存刚好
    //等于校验值,而错误进入释放流程导致崩溃,但是概率已经很低,不用考虑.正确使用不会出现问题
    void* pRawUser;        //校验用
};


template<class DevideType>
class  MemChunk
{
public:
    MemChunk(void* pBufRaw,size_t nMemSize)
    {
        this->pData=pBufRaw;
        this->m_Size=nMemSize;  //仅保存，暂时无用
        //pData=(void*)::malloc(sizeof(MemHeader)+nMemSize);  //移动到block中申请

        MemHeader<DevideType>* pHead=(MemHeader<DevideType>*)pData;
        //给用户使用的地址，预留头部偏移,直接计算并保存,避免每次获取时再计算
        this->pUser=(char*)pData+sizeof(MemHeader<DevideType>);    
        pHead->pRawUser=pUser;
        pHead->memSize=nMemSize;   //+sizeof(MemHeader);
    }
    ~MemChunk()
    {
        if (this->pData!=NULL)
        {
            ::free(this->pData);
            this->pData=NULL;
        }
    }
    MemHeader<DevideType>* getHead()
    {
        return (MemHeader<DevideType>*)(this->pData);
    }
    void* getAddress()  //取用户可以使用的地址，预留头部偏移
    {
        //return (char*)pData+sizeof(MemHeader);
        return this->pUser; //faster
    }
public:
    //void* pNodeAtList;   //保存自己在双链表中的位置，方便快速删除，可以做到O1--废弃
private:
    void* pData;
    void* pUser;
    size_t m_Size;
    //TraceClass<MemChunk> test;   //某个类对象个数跟踪辅助类
};

template<class DevideType>
class  MemBlock
{
public:
    //默认不创建，需要时会自动扩容
    MemBlock(MemPool<DevideType>* pMemPool,size_t nChunkSize,size_t nChunkNumber=0)  
        :pMemPool(pMemPool),m_SumChunkNum(0)
    {
        this->m_ChunkNumBase=nChunkNumber;
        this->m_ChunkSize=nChunkSize;
        AppendChunkSpace(nChunkNumber);     //首次扩展容量
    }
    ~MemBlock()
    {
        ClearMemSpace();
    }

    void* Malloc()
    {
        if (ListFree.empty())
        {
            if (!AutoResize())   //扩容失败
            {
                return NULL;
            }       
        }
        //从空闲列表取一个元素
        MemChunk<DevideType>* pChunk=ListFree.back();        //取最后一个元素，
        ListFree.pop_back();                          //删除最后一个元素

        //不再管理已经分配的MemChunk,因为其push操作内部调用了new node,失去内存池O1申请的意义了
        //void* pNode=ListUsed.pushBack(p);   //放入已经使用列表  并记住其在列表中位置，方便删除
        //p->pNodeAtList=pNode;

        return pChunk->getAddress();
    }
    void Free(MemChunk<DevideType>* pChunk)   //fast
    {
        //释放的还原到空闲列表
        ListFree.push_back(pChunk);
    }
    void FreeOld(void* p)
    {
        //注意理解此处
        MemHeader<DevideType>* pHead=(MemHeader<DevideType>*)((char*)p-sizeof(MemHeader<DevideType>));
        MemChunk<DevideType>* pParentChunk=pHead->pChunk; //取得所在Chunk地址
        //释放的还原到空闲列表
        ListFree.push_back(pParentChunk);
    }

    //在不扩展分块的情况下,没有可用空闲分块
    bool HasNoAvailableChunk()const {return ListFree.empty();}

    //当内存不够用时,上级memPool会调用某个block请求释放一部分空间
    bool RemoveFreeOneChunk()
    {
        if (ListFree.empty())
        {
            MemChunk<DevideType>* p=ListFree.back();        //取最后一个元素，
            ListFree.pop_back();                                               //删除最后一个元素
            delete p;
            m_SumChunkNum--;        //统计本block管理的总Chunk数量
            this->pMemPool->TotalMemUsedSub(ChunkMemUsed());  //统计内存池消耗总内存大小
            
            return true;
        }
        return false;
    }
private:
    bool AppendChunkSpace(size_t nChunkNumber);    //依赖mempool在其后实现
    bool AutoResize()
    {    
        //容量不足情况扩展策略,每次增加一倍,或者0.5倍,或者由外部传参决定，待续
        m_ChunkNumBase=m_ChunkNumBase*1+1;   //+1 是为了考虑ChunkNumber为0的特殊情况

#if 1  //考虑内存池总内存消耗限制的情况,否则直接扩张--若扩展之后,超出限制,则尽可能少的扩展
        size_t memPoolUsed=pMemPool->getTotalMemUsed();
        size_t poolTotalSizeMax=pMemPool->getTotalSizeMax();
        if ((memPoolUsed+m_ChunkNumBase*ChunkMemUsed())>poolTotalSizeMax)
        {
            m_ChunkNumBase=(poolTotalSizeMax-memPoolUsed)/ChunkMemUsed();

            printf("MemPool will full Used:%llu TotalSizeMax:%llu AppendNum:%llu\n"
                ,memPoolUsed,poolTotalSizeMax,m_ChunkNumBase);

        }
#endif
        return AppendChunkSpace(m_ChunkNumBase);
    }
    //释放本block所有空间
    void ClearMemSpace()
    {
        FreeUnusedSpace();
        FreeUsedSpace();
    }
    //释放本block已经申请未分配出去给用户的空间
   void FreeUnusedSpace()
   {
       for (size_t n=0;n<ListFree.size();n++)
       {
           MemChunk<DevideType>* pChunk=ListFree.at(n);
           delete pChunk;
           this->pMemPool->TotalMemUsedSub(ChunkMemUsed());   //统计内存池消耗总内存大小
       }
       ListFree.clear();
       m_SumChunkNum=0;        //统计本block管理的总Chunk数量
   }
    //释放本block已分配出去给用户的空间--不再管理已经分配的MemChunk,不安全
   void FreeUsedSpace()
   {
      
   }
public:
    //释放已经从系统获取，但是未分配出去的内存
    void FreeSpace()
    {
        FreeUnusedSpace();
        m_ChunkNumBase=2;    //最好复位扩展系数基数为一个比较小的数,否则可能下次立即申请很大的空间
    }

    //每个chunk消耗内存,暂不统计MemChunk内部变量的
    size_t ChunkMemUsed()const {return m_ChunkSize+sizeof(MemHeader<DevideType>);}  
    size_t UsedChunkNum()const {return m_SumChunkNum-ListFree.size();}    //已经分配出去给用户使用的分块数
    size_t UnUsedChunkNum()const {return ListFree.size();}                              //已经向系统申请到未分配给用户的分块数
    size_t UsedTotalMemSize()const {return UsedChunkNum()*ChunkMemUsed();}         //已经分配出去的内存总大小
    size_t UnusedTotalMemSize()const {return UnUsedChunkNum()*ChunkMemUsed();}  //未分配给用户的内存总大小
    size_t TotalMemUsed()const{return m_SumChunkNum*ChunkMemUsed();}               //本Block消耗总内存大小
    void printStatus()
    {
        printf("MemBlock:chunkSize:%llu ChunkNum:Sum:%llu Used:%llu Unused:%llu \n"
            ,m_ChunkSize,m_SumChunkNum,UsedChunkNum(),UnUsedChunkNum());
    }
private:
    size_t m_ChunkSize;            //本block管理的最大内存大小。
    size_t m_ChunkNumBase;   //block数量基数
    size_t m_SumChunkNum;   //本block管理的chunk总数量,包括已经分配给用户使用的

    std::vector<MemChunk<DevideType>*> ListFree;   //空闲分块列表
     //记住内存池指针,以便申请失败时，可以令mempool释放其他block一部分空间
    MemPool<DevideType>* pMemPool;                
};


template<class DevideType=DevideType5>
class MemPool
{
public:
    //nChunkSizeMax本内存池管理的最大单个内存块,大于此值向系统申请,实际管理为此值向上取最接近的二次幂-1
    //TotalSizeMax当内存池消耗系统内存容量大于此值,除非池内部刚好有可用，否则无论大小,直接向系统申请
    //std::string sName内存池名称,一些情况用于打印区分
    MemPool(size_t nChunkSizeMax=1024,size_t nTotalSizeMax=0XFFFFFFFF,std::string sName="MyMemPool")
        :sMemPoolName(sName),m_TotalSizeMax(nTotalSizeMax),poolTotalMemUsed(0)
    {
        this->m_ChunkSizeMax=nChunkSizeMax;
        InitBlock();
    }
    ~MemPool()
    {
        printStatus();     //析构前打印,各个分块峰值占用
        destoryBlock();
    }
    void printStatus()
    {
        printf("MemPool Status:%s\n",sMemPoolName.c_str());
        size_t sumUsed=0;
        for(size_t n=0;n<ListMemBlock.size();n++)
        {
            printf("BlockNum:%llu  ",n);
            ListMemBlock.at(n)->printStatus();
            sumUsed+=ListMemBlock.at(n)->TotalMemUsed();
        }

        //内存消耗,两种统计方式对比
        printf("MemPool MemUsed: BlockAdd:%llu this:%llu\n",sumUsed,this->getTotalMemUsed());  
    }

    void InitBlock()
    {
        //初始化固定大小依次翻倍的内部块//1,2,4,8,16,32,64,128,256,512,1024 
        //每一个索引管理<=2^n-1的内存；0，1，3，7，15，31.
         PoolMgrChunkSizeMax=next_pow_of_2((uint64_t)m_ChunkSizeMax)-1;

         printf("MemPool::InitBlock() ChunkSizeMax:%llu MgrChunkSizeMax:%llu\n",m_ChunkSizeMax,PoolMgrChunkSizeMax);
        

         //size_t 最大64位,最大扩展策略是16倍。
         for (unsigned int nBlock=0;nBlock<64*16;nBlock++)
         {
             size_t blockMgrSize=BlockNumToSize(nBlock);  //old BlockNumToSizeStrategy1(n);


             if (blockMgrSize>PoolMgrChunkSizeMax)
             {
                 break;
             }
             //if (blockMgrSize<16)  //为了解决在低容量大小时BlockNumToSize与SizeToBlockNum不对称的 已改进--废弃
             //{
             //    blockMgrSize=16;
             //}

             //内存块管理器，默认不申请实际内存，需要时再自动扩容
             MemBlock<DevideType>* p=new MemBlock<DevideType>(this,blockMgrSize,0);   
             ListMemBlock.push_back(p);

             //::printf("Block Index:%u ChunkMgrSize:%llu MgrMaxSize:%llu\n",nBlock,blockMgrSize,PoolMgrChunkSizeMax);

             size_t tBlock=SizeToBlockNum(blockMgrSize);

             //printf("SizeToBlockNum:%llu\n",tBlock);
             if (tBlock!=nBlock)
             {
                 //PRINT_FUN_NAME_GOON
                  //::printf("May Error at:BlockNumToSize<->SizeToBlockNum\n");   //可能大小与分块号的分配策略错误
             }
         }
    }
    void* doMalloc(size_t nMemSize)
    {
       //if (nMemSize>blockSizeMax)  //超出内存池管理的最大大小,直接向系统申请
        if (nMemSize>PoolMgrChunkSizeMax) 
        {
            return doMallocFromSystem(nMemSize);
        }

        //根据大小取得Block分块号,去内存池内部申请
         size_t BlockIndex=SizeToBlockNum(nMemSize);  //old SizeToBlockNumStrategy1(size); getBlockNum(size); 
         //主要是为了内存池总消耗容量限制功能,若不需要屏蔽此段,提升效率
#if 1
         //如果本次再申请可能超出设定的最大内存池大小,除非内部刚好有空闲Chunk,不需要扩展容量,否则不论多大调用系统申请,
         if ((poolTotalMemUsed+nMemSize)>m_TotalSizeMax)  //或者用nActuAlallocSize
         {
             printf("MemPool will full Used:%llu mSize:%llu TotalSizeMax:%llu\n"
                 ,poolTotalMemUsed,nMemSize,m_TotalSizeMax);
             printStatus();

             if (ListMemBlock[BlockIndex]->HasNoAvailableChunk())
             {
                 return doMallocFromSystem(nMemSize);
             }
         }
#endif

        return  ListMemBlock[BlockIndex]->Malloc();        //到具体的分块去申请
    }
    void* doMallocFromSystem(size_t nMemSize)
        {
            //多申请一部分保存头信息，否则释放是无法区分是向系统直接申请的还是由内存池管理的
            size_t nActuAlallocSize=sizeof(MemHeader<DevideType>)+nMemSize;
            void* p=(void*)::malloc(nActuAlallocSize);   
            if (p==NULL)
            {
                //极限情况的考虑，若malloc失败则释放内存池未分配出去的空余节点到系统，再重新申请
                while (p==NULL)
                {
                        bool retF=this->FreeOneChunk();
                        if (!retF)
                        {
                            return NULL;
                        }
                        p=(void*)::malloc(nActuAlallocSize);
                        if (p!=NULL)   //申请成功,跳出循环,进入下一步填充信息头
                        {
                            break;       //return p;
                        }
                }
                //return NULL;
            }

        printf("Sys malloc:%p size:%llu memPoolMgrSizeMax:%llu RealSize:%llu\n"
        ,p,nMemSize,m_ChunkSizeMax,PoolMgrChunkSizeMax);

        MemHeader<DevideType>* pHead=(MemHeader<DevideType>*)p;
        pHead->memSize=nMemSize;
        void* pUser=((char*)p+sizeof(MemHeader<DevideType>));        //返回给用户时候去掉减去头部大小
        pHead->pRawUser=pUser;

        pHead->pBlock=NULL;      //没有必要
        //pHead->pChunk=NULL;  //没有必要
        return pUser;
    }
    void doFree(void* p)
    {
        if (p==NULL)   //C语言标准规定为free(NULL)什么都不做
        {
            return;
        }

        MemHeader<DevideType>* pHead=(MemHeader<DevideType>*)((char*)p-sizeof(MemHeader<DevideType>));
        if (p!=pHead->pRawUser)       //校验一下此指针到底是不是通过本内存池提供的接口申请的
        {
            printf("doFree() addr check error! pCurr:%p pCheck:%p Size:%llu\n",p,pHead->pRawUser,pHead->memSize);
            return;
        }
        size_t nRawSize=pHead->memSize;      //取得指针申请时候的大小
        //if (nRawSize>blockSizeMax)               //超出内存池管理的最大大小，调用系统释放
        if (nRawSize>PoolMgrChunkSizeMax) 
        {
            printf("Sys free():%p size:%llu memPoolMgrSizeMax:%llu RealMgr:%llu\n"
                ,p,nRawSize,m_ChunkSizeMax,PoolMgrChunkSizeMax);

            //p减去头部大小才得到原始指针,交给系统释放
            ::free(pHead);
            //::free(p);
            return;
        }


#if 1 //主要是为了内存池总消耗容量限制功能,若不需要屏蔽此段,提升效率
        //虽然内存块较小,由于快溢出最终还是调用系统申请的部分,交给系统释放
        if (pHead->pBlock==NULL)
        {
            ::free(pHead);
            return;
        }
#endif


        //到内存池释放
        //方式1:通过保存的block/chunk指针直接释放,每一个内存头消耗一个指针空间,更快
        MemBlock<DevideType>* pBlock=pHead->pBlock;
        pBlock->Free(pHead->pChunk); //faster    //FreeOld(p);

        //方式2：
        //根据大小取得分块号,到具体的分块去释放
        //size_t BlockIndex=SizeToBlockNum(mSize);
        //ListMemBlock[BlockIndex]->Free(p);   
    }

    //单线程版本,为了效率不加访问互斥锁
    void* MallocNoLock(size_t nSize)
    {
        return doMalloc(nSize);
    }
    void FreeNoLock(void* p)
    {
        return doFree(p);
    }

    //多线程安全版本
    void* Malloc(size_t nSize)
    {
        LockerGuard<Mutex> lock(m_mutex);                    //加锁版本大约慢百分之20
        //std::lock_guard<std::mutex> lock(m_stdmutex);   //std库的锁更慢

        return doMalloc(nSize);
    }
    void Free(void* p)
    {
        LockerGuard<Mutex> lock(m_mutex);
        //std::lock_guard<std::mutex> lock(m_stdmutex);

        return doFree(p);
    }

    //调用构造/析构函数版本 多线程安全版本
    template<class T>
    T* New()
    {
        void* pObj=this->Malloc(sizeof(T));
        if (pObj==NULL)
        {
            return NULL;
        }
        new(pObj) T();
        return static_cast<T*>(pObj);
    }
    template<class T>
    void Delete(T* pObj)
    {
        if (pObj==NULL)
        {
            return;
        }
        pObj->~T();
        this->Free(pObj);
    }

    //数组版本待续对应new[] delete[]
    template<class T>
    T* NewArray(size_t N)
    {
        size_t memSize=sizeof(N)+sizeof(T)*N;
        void* pArray=this->Malloc(memSize);
        if (pArray==NULL)
        {
            return NULL;
        }

        *((size_t*)pArray)=N;
        T* pObjBegin=(T*)((char*)pArray+sizeof(N));
        T* pObjtmp=pObjBegin;
        for (size_t n=0;n<N;n++)
        {
               new(pObjtmp) T();
               pObjtmp++;
        }
        return pObjBegin;
    }
    template<class T>
    void DeleteArray(T* pObj)
    {
        if (pObj==NULL)
        {
            return;
        }
        void* pRaw=(char*)pObj-sizeof(size_t);
        size_t ArrayNum=*((size_t*)(pRaw));
        T* pObjtmp=pObj;
        for (size_t n=0;n<ArrayNum;n++)
        {
            pObjtmp->~T();
            pObjtmp++;
        }
        this->Free(pRaw);
    }

    //单线程无锁版本
    template<class T>
    T* NewNoLock()
    {
        void* pObj=this->MallocNoLock(sizeof(T));
        if (pObj==NULL)
        {
            return NULL;
        }
        new(pObj) T();
        return static_cast<T*>(pObj);
    }
    template<class T>
    void DeleteNoLock(T* pObj)
    {
        if (pObj==NULL)
        {
            return;
        }
        pObj->~T();
        this->FreeNoLock(pObj);
    }
    //数组版本待续对应new[] delete[]
    template<class T>
    T* NewArrayNoLock(size_t N)
    {
        size_t memSize=sizeof(N)+sizeof(T)*N;
        void* pArray=this->MallocNoLock(memSize);
        if (pArray==NULL)
        {
            return NULL;
        }
        *((size_t*)pArray)=N;
        T* pObjBegin=(T*)((char*)pArray+sizeof(N));
        T* pObjtmp=pObjBegin;
        for (size_t n=0;n<N;n++)
        {
            new(pObjtmp) T();
            pObjtmp++;
        }
        return pObjBegin;
    }
    template<class T>
    void DeleteArrayNoLock(T* pObj)
    {
        if (pObj==NULL)
        {
            return;
        }
        void* pRaw=(char*)pObj-sizeof(size_t);
        size_t ArrayNum=*((size_t*)(pRaw));
        T* pObjtmp=pObj;
        for (size_t n=0;n<ArrayNum;n++)
        {
            pObjtmp->~T();
            pObjtmp++;
        }
        this->FreeNoLock(pRaw);
    }
private:
    Mutex m_mutex;
    //std::mutex m_stdmutex;
public:
    void destoryBlock()
    {
        for (size_t n=0;n<ListMemBlock.size();n++)
        {
            MemBlock<DevideType>* p=ListMemBlock.at(n);
            delete p;
        }
        ListMemBlock.clear();
    }
public:
    //内存池随着使用时间的增加，内存池内部管理并拥有的系统内存块可能越来越多。
    //提供给外部一个接口，释放无用没有被分配出去的内存块，降低系统内存占用，某些特殊需求会用到
    void FreeSpace()
    {
        LockerGuard<Mutex> lock(m_mutex);   //加锁

        for (size_t n=0;n<ListMemBlock.size();n++)
        {
            MemBlock<DevideType>* p=ListMemBlock.at(n);
            p->FreeSpace();
        }
    }
    //空间不够释放部分未使用的,释放成功一个则返回true.没有可以释放的返回false
    bool FreeOneChunk()
    {
        //注意锁递归
        for (size_t n=0;n<ListMemBlock.size();n++)
        {
            MemBlock<DevideType>* p=ListMemBlock[n];
            bool retFree=p->RemoveFreeOneChunk();
            if (retFree)
            {
                return true;
            }
        }
        return false;
    }

    //预先分配容量,内存池容量默认不预先申请的,一些特殊要求内存池一开始就能高效工作。
    //可以手动在使用前预先分配
    bool PreMalloc(size_t nMemSize,size_t nCount)
    {
        if (nMemSize>PoolMgrChunkSizeMax)
        {
            return false;
        }
        bool ret=true;
        std::vector<void*> listMem;
        for (size_t n=0;n<nCount;n++)
        {
              void* pAddr=this->Malloc(nMemSize);
              if (pAddr==NULL)
              {
                  ret=false;
                  break;
              }
              listMem.push_back(pAddr);
        }
        for (size_t n=0;n<listMem.size();n++)
        {
            void* pAddr=listMem.at(n);
            this->Free(pAddr);
        }
        return ret;
    }
protected:
private:
    std::vector<MemBlock<DevideType>*> ListMemBlock;    //各个分块容器
    size_t m_ChunkSizeMax;            //外部传入的内存大小,实际提升到最接近的2次幂后管理的大小
    size_t PoolMgrChunkSizeMax;   //本内存池实际管理的内存最大值,高于此值,实际直接调用系统的malloc/free
    std::string sMemPoolName;      //一些情况用于区分不同内存池
public:
    //统计本内存池消耗内存大小，管理的内存总大小(包括给系统调用的),备忘
    //统计实际使用大小与,总消耗系统内存大小--等待,待续

    size_t getTotalMemUsed()const {return poolTotalMemUsed;}               //本内存池消耗内存空间
    void TotalMemUsedAdd(size_t nSize){poolTotalMemUsed+=nSize;}      //MemBlock统计改变的接口
    void TotalMemUsedSub(size_t nSize){poolTotalMemUsed-=nSize;}
    size_t getTotalSizeMax()const {return m_TotalSizeMax;}                        //取内存池最大容量限制
private:
    size_t m_TotalSizeMax;
    size_t poolTotalMemUsed;       //本内存池所有block消耗内存空间(暂不统计内部容器,变量等消耗）
private:
public:
    //容量与所在分块来回互转，基础操作

    //根据内存块大小，取得所在的分块号，
    //1,2,4,8,16,32,64,128,256,512,1024    内存大小
    //0,1,3,7,15,31,63127,255,512,1023
    //0 1 2 3  4   5  6    7    8     9    10      索引号
    //低效--废弃
    static unsigned int getBlockNum(unsigned int block_size)
    {
        unsigned int  sizeTmp=1;
        unsigned int n;
        for(n=0;n<31;n++)
        {   
            if(sizeTmp>=block_size)
            {
                return n;
            }
            sizeTmp*=2;
            sizeTmp+=1;
        }
        return 31U;
    }
    static unsigned int getBlockNum(int64_t block_size)
    {
        int64_t  sizeTmp=1;
        unsigned int n;
        for(n=0;n<63;n++)
        {   
            if(sizeTmp>=block_size)
            {
                return n;
            }
            sizeTmp*=2;
            sizeTmp+=1;
        }
        return 63U;
    }

    //improve by lizulin 2018-2020
    //获一个数某bitpos位置取后N个bit的值,不包含bitpos本身
    template <const unsigned int N>  //N=1,2,3,4,5
    static unsigned int getRightNBit(uint32_t x,unsigned int bitPos)  //bitPos>N+1
    {
        x=x>>(bitPos-N);
        const uint32_t Mask=((1<<N)-1);    //TODO:极大值越界？
        x&=Mask;   
        return x;
    };
    //获一个数某bitpos位置取后N个bit的值,不包含bitpos本身
    template <const unsigned int N>  //N=1,2,3,4,5
    static uint64_t getRightNBit(uint64_t x,unsigned int bitPos)  //bitPos>N+1
    {
        x=x>>(bitPos-N);
        const uint64_t Mask=((1ULL<<N)-1);  //TODO:极大值越界？
        x&=Mask;   
        return x;
    };

    ////获一个数某bitpos位置取后N个bit的值,连同bitpos本身
    //template <const unsigned int N>  //N=1,2,3,4,5
    //inline unsigned int getRightNBit(uint32_t x,unsigned int bitPos)  //bitPos>N+1
    //{
    //    x=x>>(bitPos+1-N);
    //    const uint32_t Mask=((1<<(N+1))-1);   //TODO:极大值越界？
    //    x&=Mask;   
    //    return x;
    //};
    ////获一个数某bitpos位置取后N个bit的值,连同bitpos本身
    //template <const unsigned int N>  //N=1,2,3,4,5
    //inline uint64_t getRightNBit(uint64_t x,unsigned int bitPos)  //bitPos>N+1
    //{
    //    x=x>>(bitPos+1-N);
    //    const uint64_t Mask=((1ULL<<(N+1))-1);  //TODO:极大值越界？
    //    x&=Mask;   
    //    return x;
    //};


    //分块号与其管理的大小来回转换策略--高效实现
    //根据数据大小,快速找到分块号,快速方法
    //1-2-4-8-16-32-64-128-256-512-1024.....        //直接2次幂倍增
    //1-2-3-4--6-8-12-16-24-32-48-64-96-128      //2次幂之间插入2个中间值的递增
    //1-2-3-4-5-6-7-8-10-12-14-16-20-24-28-32-40-48-56-64-96-128     //2次幂之间插入4个中间值的递增

    //template <const size_t N>  //N=1,2,3,4,5
    //inline uint32_t  SizeToBlockNumStrategy(uint32_t  nSize)
    //{
    //    unsigned int h1pos=getHightBit1Pos(nSize);
    //    if (N==1)
    //    {
    //        return h1pos;
    //    }
    //    uint32_t subIndex=getRightNBit<N>(nSize,h1pos);
    //    return (h1pos<<N)+subIndex;
    //}
    template <const size_t N>  //N=1,2,3,4,5
    static uint32_t  SizeToBlockNumStrategy(uint64_t  nSize)
    {
        unsigned int h1pos=getHightBit1Pos(nSize);
        if (N==1)
        {
            return h1pos;
        }
        uint64_t subIndex=getRightNBit<N>(nSize,h1pos);
        uint64_t index=(h1pos<<N)+subIndex;   
        return (uint32_t)index;                          //blocknum不会太大
    }

    template <const size_t N>  //N=1,2,3,4,5
    static uint64_t BlockNumToSizeStrategy(uint32_t nBlock)
    {
        const uint64_t BlockL=1<<(N-1);      //比较低的block
        if(nBlock<BlockL)
        {
            //const uint32_t BlockLV=1<<(N-1);  
            //return BlockLV;
            if (N==1)
            {
                return 1;
            }
            if (N==2)
            {
                return 4;
            }
            return 16;   //N=3,4,5 
        }

        const uint64_t BMax=64*(1<<(N-1))-1;
        if (nBlock>=BMax)   //极限情况考虑,以下移位运算会溢出
        {
            return UINT64_MAX;   //UINT32_MAX or UINT64_MAX
        }

        if (N==1)
        {
            uint64_t nSize=(1ull<<(nBlock+1))-1;
            return nSize;
        }
        const uint64_t P=1<<(N-1);
        nBlock+=1;
        uint64_t oBlock=nBlock/P;
        uint64_t oldSubBlock=(nBlock%P);
        uint64_t oSize=(1ull<<(oBlock));
        uint64_t mSize=oSize-1;
        uint64_t nSize=mSize+(oldSubBlock)*(oSize/P);
        return nSize;
    }

    //////内存分块策略根据模板传参决定
    //static size_t SizeToBlockNum(size_t nSize)
    //{
    //    return SizeToBlockNum<DevideType>(nSize);
    //}
    //template<class DevideType1>
    //static size_t SizeToBlockNum(size_t nSize)
    //{
    //    return SizeToBlockNumStrategy1(nSize);
    //}
    //template<>
    //static size_t SizeToBlockNum<DevideType2>(size_t nSize)
    //{
    //     return SizeToBlockNumStrategy2(nSize);
    //}
    //template<>
    //static size_t SizeToBlockNum<DevideType3>(size_t nSize)
    //{
    //    return SizeToBlockNumStrategy3(nSize);
    //}
    //template<>
    //static size_t SizeToBlockNum<DevideType4>(size_t nSize)
    //{
    //    return SizeToBlockNumStrategy4(nSize);
    //}
    //template<>
    //static size_t SizeToBlockNum<DevideType5>(size_t nSize)
    //{
    //    return SizeToBlockNumStrategy5(nSize);
    //}

    ////分块号到其管理的最大内存大小
    //static size_t BlockNumToSize(size_t nBlock)
    //{
    //    return BlockNumToSize<DevideType>(nBlock);
    //}
    //template<class DevideType1>
    //static size_t BlockNumToSize(size_t nBlock)
    //{
    //    return BlockNumToSizeStrategy1(nBlock);
    //}
    //template<>
    //static size_t BlockNumToSize<DevideType2>(size_t nBlock)
    //{
    //    return BlockNumToSizeStrategy2(nBlock);
    //}
    //template<>
    //static size_t BlockNumToSize<DevideType3>(size_t nBlock)
    //{
    //    return BlockNumToSizeStrategy3(nBlock);
    //}
    //template<>
    //static size_t BlockNumToSize<DevideType4>(size_t nBlock)
    //{
    //    return BlockNumToSizeStrategy4(nBlock);
    //}
    //template<>
    //static size_t BlockNumToSize<DevideType5>(size_t nBlock)
    //{
    //    return BlockNumToSizeStrategy5(nBlock);
    //}


     //C++这样的类成员函数模板特化,以上写法只能在VS编译通过,不符合C++标准,gcc编译错误
    //方法一:改用函数参数重载实现,略有性能损失

    //方法二:如下
    //虽然看起来像是运行时决议影响性能,但是if内表达式是编译器确定的,无用代码会被优化掉
    static size_t SizeToBlockNum(size_t nSize)
    {
        //if (nSize==0)  return 0;   //size=0 特殊情况,放到使用处考虑

        if (std::is_same<DevideType,DevideType1>::value)
        {
            return SizeToBlockNumStrategy<1>(nSize);
        }
        if (std::is_same<DevideType,DevideType2>::value)
        {
            return SizeToBlockNumStrategy<2>(nSize);
        }
        if (std::is_same<DevideType,DevideType3>::value)
        {
            return SizeToBlockNumStrategy<3>(nSize);
        }
        if (std::is_same<DevideType,DevideType4>::value)
        {
            return SizeToBlockNumStrategy<4>(nSize);
        }
        if (std::is_same<DevideType,DevideType5>::value)
        {
            return SizeToBlockNumStrategy<5>(nSize);
        }
    }

    static size_t BlockNumToSize(uint32_t nBlock)
    {
        if (std::is_same<DevideType,DevideType1>::value)
        {
            return BlockNumToSizeStrategy<1>(nBlock);
        }
        if (std::is_same<DevideType,DevideType2>::value)
        {
            return BlockNumToSizeStrategy<2>(nBlock);
        }
        if (std::is_same<DevideType,DevideType3>::value)
        {
            return BlockNumToSizeStrategy<3>(nBlock);
        }
        if (std::is_same<DevideType,DevideType4>::value)
        {
            return BlockNumToSizeStrategy<4>(nBlock);
        }
        if (std::is_same<DevideType,DevideType5>::value)
        {
            return BlockNumToSizeStrategy<5>(nBlock);
        }
    }
public:
    static void TestMemSize(size_t nSize)
    {
        size_t RES=0;
        uint32_t blockNum;
        size_t BlockMgrMaxSize;
        blockNum=SizeToBlockNumStrategy<1>(nSize);
        BlockMgrMaxSize=BlockNumToSizeStrategy<1>(blockNum);
        //printf("SizeToBlockNum S1:Size%llu BlockNum:%02u BlockMgrMaxSize:%u\n",nSize,blockNum,BlockMgrMaxSize);
        RES+=blockNum;
        if (nSize>BlockMgrMaxSize)
        {
            printf("Maybe Error! S1:Size:%llu blockNum:%llu blockMgrMaxSize:%llu\n",nSize,blockNum,BlockMgrMaxSize);
            //Thread::sleep_for(3000);
        }
        blockNum=SizeToBlockNumStrategy<2>(nSize);
        BlockMgrMaxSize=BlockNumToSizeStrategy<2>(blockNum);
        //printf("SizeToBlockNum S2:Size%llu BlockNum:%02u BlockMgrMaxSize:%llu\n",nSize,blockNum,BlockMgrMaxSize);
        RES+=blockNum;
        if (nSize>BlockMgrMaxSize)
        {
            printf("Maybe Error! S2:Size:%llu blockNum:%u blockMgrMaxSize:%llu\n",nSize,blockNum,BlockMgrMaxSize);
            //Thread::sleep_for(3000);
        }
        blockNum=SizeToBlockNumStrategy<3>(nSize);
        BlockMgrMaxSize=BlockNumToSizeStrategy<3>(blockNum);
        //printf("SizeToBlockNum S3:Size%llu BlockNum:%02u BlockMgrMaxSize:%llu\n",nSize,blockNum,BlockMgrMaxSize);
        RES+=blockNum;
        if (nSize>BlockMgrMaxSize)
        {
            printf("Maybe Error! S3:Size:%llu blockNum:%u blockMgrMaxSize:%llu\n",nSize,blockNum,BlockMgrMaxSize);
            //Thread::sleep_for(3000);
        }
        blockNum=SizeToBlockNumStrategy<4>(nSize);
        BlockMgrMaxSize=BlockNumToSizeStrategy<4>(blockNum);
        //printf("SizeToBlockNum S4:Size%llu BlockNum:%02u BlockMgrMaxSize:%llu\n",nSize,blockNum,BlockMgrMaxSize);
        RES+=blockNum;
        if (nSize>BlockMgrMaxSize)
        {
            printf("Maybe Error! S4:Size:%llu blockNum:%u blockMgrMaxSize:%llu\n",nSize,blockNum,BlockMgrMaxSize);
            //Thread::sleep_for(3000);
        }
        blockNum=SizeToBlockNumStrategy<5>(nSize);
        BlockMgrMaxSize=BlockNumToSizeStrategy<5>(blockNum);
        //printf("SizeToBlockNum S5:Size%llu BlockNum:%02u BlockMgrMaxSize:%llu\n",nSize,blockNum,BlockMgrMaxSize);
        RES+=blockNum;
        if (nSize>BlockMgrMaxSize)
        {
            printf("Maybe Error! S5:Size:%llu blockNum:%u blockMgrMaxSize:%llu\n",nSize,blockNum,BlockMgrMaxSize);
            //Thread::sleep_for(3000);
        }
    }

    static void TestBlockDevideStrategy()
    {
        //内存大小到分块号，分块号到最大管理内存大小,分配策略正确性测试

        //小数测试
        for (size_t n=0;n<1000;n++)
        {
            TestMemSize(n);
        }

        //随机数数测试
        //for (unsigned int n=0;n<65536;n++)
        for (size_t n=0;n<(1ULL<<25);n++)
            //for (int n=0;n<64;n++)
        {
            unsigned int nSize=MyRand();
            TestMemSize(nSize);

            nSize=nSize%(1<<28);   //
            TestMemSize(nSize);

            nSize=nSize%(1<<24);   //
            TestMemSize(nSize);

            nSize=nSize%(1<<16);   //小数
            TestMemSize(nSize);

            nSize=nSize%(1<<8);   //小数
            TestMemSize(nSize);

            //大于32位数测试
            size_t nSizeBig=MyRand();
            nSizeBig*=nSizeBig;
            TestMemSize(nSizeBig);
        }
    }

private:
    //禁止拷贝
    MemPool(const  MemPool&){};
    MemPool& operator=(const  MemPool&){};
};


//内存池全局单例,可以直接使用,也可以自己创建
//MemPool<DevideType5>* GetMemPool();

MemPool<>* GetMemPool();



template<class DevideType>
inline bool MemBlock<DevideType>::AppendChunkSpace(size_t nChunkNumber)
{

    for (size_t n=0;n<nChunkNumber;n++)
    {
        //MemChunk* pChunk=new MemChunk(m_ChunkSize);

        size_t newChunkmemSize=sizeof(MemHeader<DevideType>)+m_ChunkSize;
        void* pBufRaw=(void*)::malloc(newChunkmemSize);

        //pData=(void*)::malloc(sizeof(MemHeader)+nMemSize);
        //if(pData==NULL)
        //{
        //    printf("MemChunk() malloc Error\n");
        //    std::exit(-1);
        //    return;
        //}

        //系统内存不够，调用内存池释放一些block未使用的空间,然后继续申请
        while(pBufRaw==NULL)
        {
            bool retFree=this->pMemPool->FreeOneChunk();
            if (!retFree)
            {
                printf("Mempool MemChunk() malloc Error\n");
                //std::exit(-1);
                return false;
            }
            pBufRaw=(void*)::malloc(newChunkmemSize);

            //if (pBufRaw==NULL)
            //{
            //    return false;
            //}
        }

        MemChunk<DevideType>* pChunk=new MemChunk<DevideType>(pBufRaw,m_ChunkSize);
        MemHeader<DevideType>* pHead=pChunk->getHead();

        pHead->pChunk=pChunk;   //记住自己所在chunk指针,便于快速释放
        pHead->pBlock=this;           //记住自己所在block指针,便于快速释放
        ListFree.push_back(pChunk);

        this->pMemPool->TotalMemUsedAdd(newChunkmemSize);      //统计内存池消耗总内存大小
        m_SumChunkNum++;         //统计本block管理的总Chunk数量
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//一个分配固定大小块的内存池（memory pool的例子
//rafactor by lizulin 
class  MemPoolFixedSize 
{
public :
    MemPoolFixedSize(size_t  blockSize, size_t preAlloc):
        m_blockSize(blockSize),
        m_allocatedNum(preAlloc)
    {
        //保留r空间 
        ListFree.reserve(preAlloc);
        for  (size_t  n = 0 ; n < preAlloc; ++n)
        {
            char* p=new  char [m_blockSize];
            ListFree.push_back(p);
        }
    }

    ~MemPoolFixedSize()
    {
        std::vector<char*>::const_iterator it;
        for  (it = ListFree.begin(); it != ListFree.end(); ++it)
        {
            delete[] *it;
        }
        ListFree.clear();  //add 
    }
    void * get()
    {
        if  (ListFree.empty())
        {
            size_t appendSize=ListFree.size()+1;  //*2+1 
            for (size_t n=0;n<appendSize;n++)
            {
            char* p=new  char [m_blockSize];
            ListFree.push_back(p);
             ++m_allocatedNum;
        }
        }

        char * ptr = ListFree.back();
        ListFree.pop_back();
        return  ptr;
    }
    void  release(void * ptr)
    {
        //将get的内存还回vec中去
        ListFree.push_back(reinterpret_cast <char *>(ptr));
    }
    size_t blockSize() const {return m_blockSize;}
    size_t allocated() const {return  m_allocatedNum;}       //返回总的分配的数目 
    size_t available() const { return ListFree.size();}             //凡存在blocks中的都是未交付使用的（但已分配)
private:
    MemPoolFixedSize(const  MemPoolFixedSize &);
    MemPoolFixedSize & operator  = (const  MemPoolFixedSize &);
    size_t  m_blockSize;
    size_t  m_allocatedNum;
    //思路2,不用vector保存备用的指针，直接在node加一个指向下一级的指针,待实现
    std::vector<char *>  ListFree;
};

template<class T>
class ObjectPool
{
public:
    ObjectPool():pool(sizeof(T),0)
    {
    }
    T* New()
    {
        char* pObj=pool.get();
        return new(pObj) T();
    }
    void  Delete(T* ptr)
    {
        ptr->~T();
        pool.release(ptr);
    }
private:
    MemPoolFixedSize pool;
    ObjectPool(const  ObjectPool &);
    ObjectPool & operator  = (const  ObjectPool &);
};


#ifdef UNIT_TEST
BASIC_API int Test_MemoryPool();
#endif

NAME_SPACE_MAIN_END

#endif







