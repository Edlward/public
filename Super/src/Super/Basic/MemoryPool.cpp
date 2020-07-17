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
MemPool<>& GetMemPool()
{
    static MemPool<> gMemPool(512*1024);	
    return gMemPool;
}



//historty 
#if 0

//返回一个数最高1bit的位置后几位的值,不足则为0,H1pos为最高为1的位置
template <const size_t N>  //N=1,2,3,4,5
unsigned int getNearHightNBit(size_t x,size_t H1pos)
{
    x=x>>(H1pos-N);
    const size_t Mask=((1<<N)-1); 
    x&=Mask;
    return x;
};

#if 0
static size_t getNearHight1Bit(size_t x,size_t H1pos)//次高1位
{
    x=x>>(H1pos-1);
    x&=0X00000001;
    return x;
}

static size_t getNearHight2Bit(size_t x,size_t H1pos) //次高2位
{
    x=x>>(H1pos-2);
    x&=0X00000003;
    return x;
}

static size_t getNearHight3Bit(size_t  x,size_t H1pos)//次高3位
{
    x=(x>>(H1pos-3));
    x&=0X00000007;
    return x;
}

static size_t getNearHight4Bit(size_t x,size_t H1pos) //次高4位
{
    x=(x>>(H1pos-4));
    x&=0X0000000F; 
    return x;
}

//1-2-4-8-16-32-64-128-256-512-1024.....        //直接2次幂倍增
//1-2-3-4--6-8-12-16-24-32-48-64-96-128      //2次幂之间插入2个中间值的递增
//1-2-3-4-5-6-7-8-10-12-14-16-20-24-28-32-40-48-56-64-96-128     //2次幂之间插入4个中间值的递增
//1-2-4-8-12-16-24-32-48-64-96-128              //2次幂之间插入8个中间值的递增
//1-2-4-8-12-16-24-32-48-64-96-128              //2次幂之间插入16个中间值的递增

//根据数据大小,快速找到分块号,快速方法
static size_t SizeToBlockNumStrategy1(size_t nSize)
{
    size_t  h1pos=getHightBit1PosFast(nSize);
    return h1pos;
}
static size_t  SizeToBlockNumStrategy2(size_t  nSize)
{
    unsigned int h1pos=getHightBit1PosFast(nSize);
    unsigned int tmp=getNearHight1Bit(nSize,h1pos);
    size_t  Index=(h1pos<<1)+tmp;
    return Index;
}
static size_t  SizeToBlockNumStrategy3(size_t  nSize)
{
    unsigned int h1pos=getHightBit1PosFast(nSize);
    unsigned int tmp=getNearHight2Bit(nSize,h1pos);
    size_t  Index=(h1pos<<2)+tmp;
    return Index;
}
static size_t  SizeToBlockNumStrategy4(size_t  nSize)
{
    unsigned int h1pos=getHightBit1PosFast(nSize);
    unsigned int tmp=getNearHight3Bit(nSize,h1pos);
    size_t  Index=(h1pos<<3)+tmp;
    return Index;
}
static size_t  SizeToBlockNumStrategy5(size_t  nSize)
{
    unsigned int h1pos=getHightBit1PosFast(nSize);
    unsigned int tmp=getNearHight4Bit(nSize,h1pos);
    size_t  Index=(h1pos<<4)+tmp;
    return Index;
}

static size_t BlockNumToSizeStrategy1(size_t nBlock)
{
    if(nBlock<1)
    {
        return 1;
    }
    if (nBlock>=63)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    size_t nSize=(1ULL<<(nBlock+1))-1;
    return nSize;
}
static size_t BlockNumToSizeStrategy2(size_t nBlock)
{
    if(nBlock<4)
    {
        return 4;
    }
    if (nBlock>=127)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    nBlock+=1;
    size_t oBlock=nBlock/2;
    size_t oldSubBlock=(nBlock%2);
    size_t oSize=(1ULL<<(oBlock));
    size_t mSize=oSize-1;
    size_t nSize=mSize+(oldSubBlock)*(oSize/2);
    return nSize;
}
static size_t BlockNumToSizeStrategy3(size_t nBlock)
{
    if(nBlock<16)
    {
        return 16;
    }
    if (nBlock>=255)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    nBlock+=1;
    size_t oBlock=nBlock/4;
    size_t oldSubBlock=(nBlock%4);
    size_t oSize=(1ULL<<(oBlock));
    size_t mSize=oSize-1;
    size_t nSize=mSize+(oldSubBlock)*(oSize/4);
    return nSize;
}
static size_t BlockNumToSizeStrategy4(size_t nBlock)
{
    if(nBlock<32)
    {
        return 16;
    }
    if (nBlock>=511)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    nBlock+=1;
    size_t oBlock=nBlock/8;
    size_t oldSubBlock=(nBlock%8);
    size_t oSize=(1ULL<<(oBlock));
    size_t mSize=oSize-1;
    size_t nSize=mSize+(oldSubBlock)*(oSize/8);
    return nSize;
}
static size_t BlockNumToSizeStrategy5(size_t nBlock)
{
    if(nBlock<64)
    {
        return 16;
    }
    if (nBlock>=1023)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    nBlock+=1;
    size_t oBlock=nBlock/16;
    size_t oldSubBlock=(nBlock%16);
    size_t oSize=(1ULL<<(oBlock));
    size_t mSize=oSize-1;
    size_t nSize=mSize+(oldSubBlock)*(oSize/16);
    return nSize;
}
#endif

static uint32_t getNearHight1Bit(uint32_t x,uint32_t H1pos)//次高1位
{
    x=x>>(H1pos-1);
    x&=0X00000001;
    return x;
}
static uint64_t getNearHight1Bit(uint64_t x,uint32_t H1pos)//次高1位
{
    x=x>>(H1pos-1);
    x&=0X00000001ULL;
    return x;
}
static uint32_t getNearHight2Bit(uint32_t x,uint32_t H1pos) //次高2位
{
    x=x>>(H1pos-2);
    x&=0X00000003;
    return x;
}
static uint64_t getNearHight2Bit(uint64_t x,uint32_t H1pos) //次高2位
{
    x=x>>(H1pos-2);
    x&=0X00000003ULL;
    return x;
}
static uint32_t getNearHight3Bit(uint32_t  x,uint32_t H1pos)//次高3位
{
    x=(x>>(H1pos-3));
    x&=0X00000007;
    return x;
}
static uint64_t getNearHight3Bit(uint64_t  x,uint32_t H1pos)//次高3位
{
    x=(x>>(H1pos-3));
    x&=0X00000007ULL;
    return x;
}
static uint32_t getNearHight4Bit(uint32_t x,uint32_t H1pos) //次高4位
{
    x=(x>>(H1pos-4));
    x&=0X0000000F; 
    return x;
}
static uint64_t getNearHight4Bit(uint64_t x,uint32_t H1pos) //次高4位
{
    x=(x>>(H1pos-4));
    x&=0X0000000FULL; 
    return x;
}

//1-2-4-8-16-32-64-128-256-512-1024.....        //直接2次幂倍增
//1-2-3-4--6-8-12-16-24-32-48-64-96-128      //2次幂之间插入2个中间值的递增
//1-2-3-4-5-6-7-8-10-12-14-16-20-24-28-32-40-48-56-64-96-128     //2次幂之间插入4个中间值的递增
//1-2-4-8-12-16-24-32-48-64-96-128              //2次幂之间插入8个中间值的递增
//1-2-4-8-12-16-24-32-48-64-96-128              //2次幂之间插入16个中间值的递增

//根据数据大小,快速找到分块号,快速方法
static uint32_t SizeToBlockNumStrategy1(uint32_t nSize)
{
    uint32_t  h1pos=getHightBit1Pos(nSize);
    return h1pos;
}
static uint64_t SizeToBlockNumStrategy1(uint64_t nSize)
{
    uint32_t  h1pos=getHightBit1Pos(nSize);
    return h1pos;
}
static uint32_t  SizeToBlockNumStrategy2(uint32_t  nSize)
{
    unsigned int h1pos=getHightBit1Pos(nSize);
    unsigned int tmp=getNearHight1Bit(nSize,h1pos);
    uint32_t  Index=(h1pos<<1)+tmp;
    return Index;
}
static uint64_t  SizeToBlockNumStrategy2(uint64_t  nSize)
{
    unsigned int h1pos=getHightBit1Pos(nSize);
    uint64_t tmp=getNearHight1Bit(nSize,h1pos);
    uint64_t  Index=(h1pos<<1)+tmp;
    return Index;
}
static uint32_t  SizeToBlockNumStrategy3(uint32_t  nSize)
{
    unsigned int h1pos=getHightBit1Pos(nSize);
    unsigned int tmp=getNearHight2Bit(nSize,h1pos);
    uint32_t  Index=(h1pos<<2)+tmp;
    return Index;
}
static uint64_t  SizeToBlockNumStrategy3(uint64_t  nSize)
{
    unsigned int h1pos=getHightBit1Pos(nSize);
    uint64_t tmp=getNearHight2Bit(nSize,h1pos);
    uint64_t  Index=(h1pos<<2)+tmp;
    return Index;
}
static uint32_t  SizeToBlockNumStrategy4(uint32_t  nSize)
{
    unsigned int h1pos=getHightBit1Pos(nSize);
    unsigned int tmp=getNearHight3Bit(nSize,h1pos);
    uint32_t  Index=(h1pos<<3)+tmp;
    return Index;
}
static uint64_t  SizeToBlockNumStrategy4(uint64_t  nSize)
{
    unsigned int h1pos=getHightBit1Pos(nSize);
    uint64_t tmp=getNearHight3Bit(nSize,h1pos);
    uint64_t  Index=(h1pos<<3)+tmp;
    return Index;
}
static uint32_t  SizeToBlockNumStrategy5(uint32_t  nSize)
{
    unsigned int h1pos=getHightBit1Pos(nSize);
    unsigned int tmp=getNearHight4Bit(nSize,h1pos);
    uint32_t  Index=(h1pos<<4)+tmp;
    return Index;
}
static uint64_t  SizeToBlockNumStrategy5(uint64_t  nSize)
{
    unsigned int h1pos=getHightBit1Pos(nSize);
    uint64_t tmp=getNearHight4Bit(nSize,h1pos);
    uint64_t  Index=(h1pos<<4)+tmp;
    return Index;
}

static uint32_t BlockNumToSizeStrategy1(uint32_t nBlock)
{
    if(nBlock<1)
    {
        return 1;
    }
    if (nBlock>=63)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT32_MAX;
    }

    uint32_t nSize=(1ULL<<(nBlock+1))-1;
    return nSize;
}
static uint32_t BlockNumToSizeStrategy2(uint32_t nBlock)
{
    if(nBlock<4)
    {
        return 4;
    }
    if (nBlock>=127)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT32_MAX;
    }

    nBlock+=1;
    uint32_t oBlock=nBlock/2;
    uint32_t oldSubBlock=(nBlock%2);
    uint32_t oSize=(1<<(oBlock));
    uint32_t mSize=oSize-1;
    uint32_t nSize=mSize+(oldSubBlock)*(oSize/2);
    return nSize;
}
static uint32_t BlockNumToSizeStrategy3(uint32_t nBlock)
{
    if(nBlock<16)
    {
        return 16;
    }
    if (nBlock>=255)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT32_MAX;
    }

    nBlock+=1;
    uint32_t oBlock=nBlock/4;
    uint32_t oldSubBlock=(nBlock%4);
    uint32_t oSize=(1ULL<<(oBlock));
    uint32_t mSize=oSize-1;
    uint32_t nSize=mSize+(oldSubBlock)*(oSize/4);
    return nSize;
}
static uint32_t BlockNumToSizeStrategy4(uint32_t nBlock)
{
    if(nBlock<32)
    {
        return 16;
    }
    if (nBlock>=511)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT32_MAX;
    }

    nBlock+=1;
    uint32_t oBlock=nBlock/8;
    uint32_t oldSubBlock=(nBlock%8);
    uint32_t oSize=(1ULL<<(oBlock));
    uint32_t mSize=oSize-1;
    uint32_t nSize=mSize+(oldSubBlock)*(oSize/8);
    return nSize;
}
static uint32_t BlockNumToSizeStrategy5(uint32_t nBlock)
{
    if(nBlock<64)
    {
        return 16;
    }
    if (nBlock>=1023)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT32_MAX;
    }

    nBlock+=1;
    uint32_t oBlock=nBlock/16;
    uint32_t oldSubBlock=(nBlock%16);
    uint32_t oSize=(1ULL<<(oBlock));
    uint32_t mSize=oSize-1;
    uint32_t nSize=mSize+(oldSubBlock)*(oSize/16);
    return nSize;
}

static uint64_t BlockNumToSizeStrategy1(uint64_t nBlock)
{
    if(nBlock<1)
    {
        return 1;
    }
    if (nBlock>=63)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    uint64_t nSize=(1ULL<<(nBlock+1))-1;
    return nSize;
}
static uint64_t BlockNumToSizeStrategy2(uint64_t nBlock)
{
    if(nBlock<4)
    {
        return 4;
    }
    if (nBlock>=127)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    nBlock+=1;
    uint64_t oBlock=nBlock/2;
    uint64_t oldSubBlock=(nBlock%2);
    uint64_t oSize=(1ULL<<(oBlock));
    uint64_t mSize=oSize-1;
    uint64_t nSize=mSize+(oldSubBlock)*(oSize/2);
    return nSize;
}
static uint64_t BlockNumToSizeStrategy3(uint64_t nBlock)
{
    if(nBlock<16)
    {
        return 16;
    }
    if (nBlock>=255)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    nBlock+=1;
    uint64_t oBlock=nBlock/4;
    uint64_t oldSubBlock=(nBlock%4);
    uint64_t oSize=(1ULL<<(oBlock));
    uint64_t mSize=oSize-1;
    uint64_t nSize=mSize+(oldSubBlock)*(oSize/4);
    return nSize;
}
static uint64_t BlockNumToSizeStrategy4(uint64_t nBlock)
{
    if(nBlock<32)
    {
        return 16;
    }
    if (nBlock>=511)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    nBlock+=1;
    uint64_t oBlock=nBlock/8;
    uint64_t oldSubBlock=(nBlock%8);
    uint64_t oSize=(1ULL<<(oBlock));
    uint64_t mSize=oSize-1;
    uint64_t nSize=mSize+(oldSubBlock)*(oSize/8);
    return nSize;
}
static uint64_t BlockNumToSizeStrategy5(uint64_t nBlock)
{
    if(nBlock<64)
    {
        return 16;
    }
    if (nBlock>=1023)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;
    }

    nBlock+=1;
    uint64_t oBlock=nBlock/16;
    uint64_t oldSubBlock=(nBlock%16);
    uint64_t oSize=(1ULL<<(oBlock));
    uint64_t mSize=oSize-1;
    uint64_t nSize=mSize+(oldSubBlock)*(oSize/16);
    return nSize;
}
#endif



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
    PRINT_FUN_NAME
    MemPool<>::TestBlockDevideStrategy();

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


