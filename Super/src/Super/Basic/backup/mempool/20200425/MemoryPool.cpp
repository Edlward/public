#include "MemoryPool.h"
#include "Thread.h"   //SLEEP
#include <stdlib.h>    //for memset
#include <memory.h>

NAME_SPACE_MAIN_BEGIN


//static MemPool<DevideType5> gMemPool(64*1024);
//MemPool<DevideType5>* GetMemPool()
//    {
//    return &gMemPool;
//}

//static MemPool<> gMemPool(512*1024);
MemPool<>* GetMemPool()
{
    static MemPool<> gMemPool(512*1024);	
    return &gMemPool;
}

#if 0
class MemoryChunk;
struct BlockHeader
{
    MemoryChunk* pChunk;
    size_t len;
};

struct MemoryBlock;
struct BlockData
{
    union
    {
        MemoryBlock* pNext;
        char pBuffer;
    };
};

struct MemoryBlock
{
    BlockHeader header;
    BlockData data;
};


class MemoryChunk
{
public:
    MemoryChunk(size_t size, int count)
    {
        //INITMUTEX(hMutex);
        this->pFreeList=NULL;
        this->size=size;
        this->count=0;
        MemoryBlock* pBlock;
        while(count--)
        {
            pBlock=CreateBlock();
            if(!pBlock)break;
            pBlock->data.pNext=pFreeList;
            pFreeList=pBlock;
        }
    }
    ~MemoryChunk()
    {
        int tempcount=0;
        MemoryBlock* pBlock;
        while(pBlock=pFreeList)
        {
            pFreeList=pBlock->data.pNext;
            DeleteBlock(pBlock);
            ++tempcount;
        }
        assert(tempcount==count);//!确保释放完全
        //DELMUTEX(hMutex);
    }
    void* malloc()
    {
        MemoryBlock* pBlock;
        //LOCK(hMutex);
        if(pFreeList)
        {
            pBlock=pFreeList;
            pFreeList=pBlock->data.pNext;
        }
        else
        {
            if(!(pBlock=CreateBlock()))
            {
                //UNLOCK(hMutex);
                return NULL;
            }
        }
        //UNLOCK(hMutex);
        return &pBlock->data.pBuffer;
    }
    static void free(void* pMem)
    {
        MemoryBlock* pBlock=(MemoryBlock*)((char*)pMem-sizeof(BlockHeader));
        pBlock->header.pChunk->free(pBlock);
    }
    void free(MemoryBlock* pBlock)
    {
        //LOCK(hMutex);
        pBlock->data.pNext=pFreeList;
        pFreeList=pBlock;
        //UNLOCK(hMutex);
    }
    
    MemoryChunk* Next(){return pNext;}
 
protected:
    MemoryBlock* CreateBlock()
    {
        MemoryBlock* pBlock=(MemoryBlock*)::malloc(sizeof(BlockHeader)+size);
        if(pBlock)
        {
            pBlock->header.pChunk=this;
            pBlock->header.len=size;
            ++count;
        }
        return pBlock;
    }
    void DeleteBlock(MemoryBlock* pBlock)
    {
        ::free(pBlock);
    }
private:
    MemoryBlock* pFreeList;
    size_t size;//!Block大小
    int count;//!Block数目
    MemoryChunk* pNext;
    //MUTEXTYPE hMutex;
};




// StaticMemory.h
 //* 定义实现内存池
 //* 采用固定大小策略进行内存管理与分配
 //* 减少因大量小内存分配导致的内存碎片增加


struct HeapHeader
{
    size_t size;
};

struct MemoryHeap
{
    HeapHeader header;
    char pBuffer;
};
 
class StaticMemory
{
public:
    typedef enum{MAX_SIZE=1024,MIN_SIZE=sizeof(MemoryChunk*)};
    StaticMemory()
    {
        chunkcount=0;
        for(size_t size=MIN_SIZE; size<=MAX_SIZE; size*=2)
        {
            ++chunkcount;
        }

        //pChunkList=(MemoryChunk**)malloc(sizeof(MemoryChunk*)*chunkcount);
        pChunkList=new MemoryChunk*[chunkcount];
        int index=0;
        for(size_t size=MIN_SIZE; size<=MAX_SIZE; size*=2)
        {
            pChunkList[index++]=new MemoryChunk(size,1000);
        }
    }
    ~StaticMemory()
    {
        for(int index=0; index<chunkcount; ++index)
        {
            delete pChunkList[index];
        }
        //free(pChunkList);
        delete[] pChunkList;
    }
    void* Malloc(size_t size)
    {
        if(size>MAX_SIZE)
        {
            return malloc(size);
        }
        int index=0;
        for(size_t tsize=MIN_SIZE; tsize<=MAX_SIZE; tsize*=2)
        {
            if(tsize>=size)break;
            ++index;
        }
        return pChunkList[index]->malloc();
    }
    void Free(void* pMem)
    {
        if(!free(pMem))MemoryChunk::free(pMem);
    }
protected:
    void* malloc(size_t size)
    {
        MemoryHeap* pHeap=(MemoryHeap*)::malloc(sizeof(HeapHeader)+size);
        if(pHeap)
        {
            pHeap->header.size=size;
            return &pHeap->pBuffer;
        }
        return NULL;
    }
    bool free(void* pMem)
    {
        //-sizeof(BlockHeader) 应该改为+吧 ？？
        MemoryHeap* pHeap=(MemoryHeap*)((char*)pMem-sizeof(HeapHeader));
        if(pHeap->header.size>MAX_SIZE)
        {
            ::free(pHeap);
            return true;
        }
        return false;
    }
private:
    MemoryChunk** pChunkList;
    int chunkcount;
};



/** @class ObjectManager
 * 实现利用内存池创建对象
 * 要求对象具有缺省构造函数
 */
template<typename T>
class ObjectManager
{
public:
    typedef T ObjectType;
 
    static ObjectType* Create(StaticMemory* pool)
    {
        void* pobject=pool->Malloc(sizeof(T));
        new(pobject) ObjectType();
        return static_cast<ObjectType*>(pobject);
    }
    static void Delete(StaticMemory* pool, ObjectType* pobject)
    {
        pobject->~ObjectType();
        pool->Free(pobject);
    }
};
#endif





#ifdef UNIT_TEST
#include "SuperTime.h"

USING_NAME_SPACE_MAIN

int Test_MemoryPool()
{

    {
        SuperTime tm("MemPool()");
        for (unsigned int T=0;T<10;T++)
        {
            tm.getBeginTime();
            unsigned int res=0;
            for (unsigned int K=0;K<100*1024*1024;K++)
            {
                res+=getHightBit1Pos(K);
            }
            tm.getEndPrint(1);
            PRINT_FUN_NAME_GOON
                printf("RES:%u\n",res);
        }
    }
    return 0;


    //INT32 与INT64区别
    unsigned int res=0;
    for (unsigned int K=0;K<100*1024*1024;K++)
    {
         res+=MemPool<>::SizeToBlockNumStrategy1(K);
         res+=MemPool<>::SizeToBlockNumStrategy2(K);
         res+=MemPool<>::SizeToBlockNumStrategy3(K);
         res+=MemPool<>::SizeToBlockNumStrategy4(K);
         res+=MemPool<>::SizeToBlockNumStrategy5(K);
    }
    PRINT_FUN_NAME_GOON
    printf("RES:%d\n",res);
    return 0;






    const size_t PoolMgrSize=512*1024;          //内存池管理的最大大小
   // const size_t PoolMgrSize=0xFFFFFFFFFFFFFFFF;  

    MemPool<DevideType5> memPool(PoolMgrSize);
    memPool.TestBlockDevideStrategy();
    memPool.printStatus();

    int* pint=memPool.New<int>();
    int* pintarray=memPool.NewArrayNoLock<int>(500);
    pintarray[499]=0;


    std::vector<void*> Vused;
    SuperTime tm("MemPool()");
    //return 0;

    void* pData;
    for (unsigned int K=0;K<10000;K++)
    {
            Vused.clear();
            unsigned sumMem=0;
            for (unsigned int n=0;n<10;n++)
            {

                unsigned int mallocSize=MyRand()%(PoolMgrSize);  //rand()

                //unsigned int randValue=rand();  //0-32767
                //unsigned int mallocSize=(randValue*100)%(PoolMgrSize);

                sumMem+=mallocSize;
                //printf("n:%u mallocSize:%u\n",n,mallocSize);

                tm.getBeginTime();
                for (unsigned int K=0;K<100;K++)
                {
                   pData=memPool.Malloc(mallocSize);
                   memset(pData,53,mallocSize);
                   Vused.push_back(pData);
                }
                tm.getEndPrint(1000);


                //void* pData=malloc(mallocSize);   //与系统函数对比 大约快30倍
                //Vused.push_back(pData);
                //memset(pData,53,mallocSize);

                //memPool.Free(pData);   //不能重复释放

            }

            //memPool.printStatus();

            for (size_t n=0;n<Vused.size();n++)
            {
                void* p=Vused.at(n);
                memPool.Free(p);

                //::free(p);
            }
            //memPool.FreeSpace();
    }


     PRINT_FUN_END

    //printf("MemBlock ObjCnt:%d\n",MemBlock::ObjCnt);

    return 0;
}



#endif

NAME_SPACE_MAIN_END

