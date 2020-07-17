#include "MemPoolFast.h"
#include "Thread.h"   //SLEEP

NAME_SPACE_MAIN_BEGIN
 
//一种高性O1计算机系统数据资源(内存/硬盘)管理策略
//可以用在内存管理,硬盘/SSD/Flash数据写入管理。
//完全O1
//在硬盘上还可以实现碎片管理,搬迁合并,可以完全利用空间。

//逻辑segment block chunk 
//part 部分, 部, 部件, 份, 局部, 分
//piece 片, 一块, 块, 一片, 部分, 个
//section 部分, 部, 截面, 段, 章节, 剖面
 
//MHeadInfo可以存储在实际使用的区域的头部，也可以用专门的地址存储。
//为了避免被破坏。尤其是用在SSD的情况
//用UI实时显示分配情况。QT写出来

//遇到问题，随着不断的创建归还，最后小碎片会越来越多？。分裂时若右边太小可以不合并


//在x86_64下面，其实虚拟地址只使用了48位。所以C程序里，打印的地址都是只有12位16进制。
//48位地址长度也就是对应了256TB的地址空间。对于内存也可能40位就够用了


//为了保证本结构不大于16字节,用位域
struct SP_ALIGN(1) MemHeadInfo
{  
    //高级优化,用位域  //表示此块是否空闲，决定是否可以合并分块，高效实现可以合并到void的某个bit。或者Size的最后1bit
   uint64_t posBegin:48;             //real type is void*
   uint64_t BlockIndex:12;          //2^12=4096
   uint64_t bFree:1;                    //bool type
   uint64_t resv:3;                      //reserved
   uint64_t nRawMemSize:40;    //uint64:40  根据实际情况最大管理的chunk不可能超过2^40,1TB
   uint64_t subPos:24;               //
   MemHeadInfo()
   {
       static_assert(sizeof(MemHeadInfo)==16,"too large");
       posBegin=NULL;
       BlockIndex=0;
       subPos=0;
       nRawMemSize=0;
       bFree=false;
       resv=0;
   }
};

struct MemEndMask
{
    size_t mSize;
};
 

//#define FAST_VERSION    //optimization version
 

//为了不损失性能,由模板参数决定行为

//不同的内存大小块划分策略
class DevideType;             //1-2-4-8-16-32-64-128-256-512-1024 //二次幂划分 powerof2  
class DevideType1;           //二次幂内部之间再划分2块
class DevideType2;           //二次幂内部之间再划分4块
class DevideType3;           //二次幂内部之间再划分8块
class DevideType4;           //二次幂内部之间再划分16块
class DevideType5;           //二次幂内部之间再划分32块
class DevideType6;           //二次幂内部之间再划分64块

template<class DevideType=DevideType5>
class MemPoolF
{
    static_assert(
        std::is_same<DevideType,DevideType1>::value
        ||std::is_same<DevideType,DevideType2>::value
        ||std::is_same<DevideType,DevideType3>::value
        ||std::is_same<DevideType,DevideType4>::value
        ||std::is_same<DevideType,DevideType5>::value
        ||std::is_same<DevideType,DevideType6>::value,"MemPool unknow DevideType");   //todo

public:
    MemPoolF()
    {
        //test
        //const int partSize=100*1024*1024;
        //char* pNewPart=(char*)::malloc(partSize);
        //appendNewPartMemSpace(pNewPart,partSize);
        initBlock();
    }
    ~MemPoolF()
    {

    }
    void initBlock()
    {
        //for(size_t n=0;n<64;n++)
        //{
        //    ListChunk NullChunk;
        //    ListMemBlock.push_back(NullChunk);
        //}

        ListMemBlock.resize(1024);  //64
        for(size_t n=0;n<32;n++)
        {
             hasFreeMask[n]=0;
        }
    }
    void appendNewPartMemSpace(char* pAddr,size_t memSize)
    {
        initPartMem(pAddr,memSize);

        //添加可以实际使用的空间
        appendMemChunk(pAddr+sizeof(size_t),memSize-sizeof(size_t));
    }

    void initPartMem(char* pAddr,size_t memSize)
    {
        //不同内存片段之间头尾用0填充，防止，向前向后合并时超出界限。
        //表示之前，之后没有可用的chunk。
        size_t* pSizeHead=(size_t*)pAddr;
        size_t* pSizeTail=(size_t*)(pAddr+memSize-sizeof(size_t));
        *pSizeHead=0;
        *pSizeTail=0;
    }
    void FillMemInfo(char* pAddr,size_t memSize)
    {
        MemHeadInfo* pMemInfo=(MemHeadInfo*)pAddr;
        pMemInfo->posBegin=(uint64_t)pAddr;
        pMemInfo->nRawMemSize=memSize;
        pMemInfo->bFree=true;
        //pMemInfo->BlockIndex=0;
        //pMemInfo->subPos=0;
    }
    void appendMemChunk(char* pAddr,size_t memSize)
    {
         FillMemInfo(pAddr,memSize);
         appendMemChunk((MemHeadInfo*)pAddr);
    }
    void appendMemChunk(MemHeadInfo* pMemInfo)
    {
        size_t blockNum=SizeToBlockNum(pMemInfo->nRawMemSize);  //-  待续

      std::vector<MemHeadInfo>& CurrBlock=ListMemBlock[blockNum];

        //本内存头信息即将保存的分块号,以及在分块号中的位置
        pMemInfo->BlockIndex=blockNum;
        pMemInfo->subPos=CurrBlock.size();
        CurrBlock.push_back(*pMemInfo);

       updateBlockFreeMask(blockNum,CurrBlock.size()); 
    }
    //统计递归次数,一般不超过4-5次
    void RepartitionChunkRecursive(MemHeadInfo* pMemInfo,size_t oldSize)  
    {
        const int sizeMemInfo=sizeof(MemHeadInfo)+sizeof(MemEndMask);
        size_t partsize=oldSize-sizeMemInfo;
        partsize=downSize(partsize);  //TODO
        if (partsize<=16)
        {
            return;
        }
        FillMemInfo((char*)(pMemInfo->posBegin),partsize);
        appendMemChunk((MemHeadInfo*)(pMemInfo->posBegin));

        size_t remainSize=oldSize-partsize;
        RepartitionChunkRecursive(pMemInfo+partsize,remainSize);
    }
    //重新划分Chunk,返回刚好合适的大小,把多余部分归还到池中
    void RepartitionChunk(MemHeadInfo* pMemInfo,size_t oldSize,size_t needSize)
    {
        const int sizeMemInfo=sizeof(MemHeadInfo)+sizeof(MemEndMask);
        size_t partSize=needSize+sizeMemInfo;
        //partSize=upToProperSize();  //扩展到合适大小TODO:
        FillMemInfo((char*)(pMemInfo->posBegin),partSize);

        appendMemChunk((MemHeadInfo*)(pMemInfo->posBegin));
        char* pRemainPart=(char*)(pMemInfo->posBegin)+partSize;
        size_t remainSize=oldSize-partSize;
        RepartitionChunkRecursive((MemHeadInfo*)pRemainPart,remainSize);
    }
    void giveBackChunk(MemHeadInfo* pMemInfo)
    {
         //pMemInfo->bFree=true;    //归还

        mergeWithNeighborChunkifNeed(pMemInfo);
        //removeChunk(pMemInfo);
        appendMemChunk(pMemInfo);
    }

    //遇到问题,即将被合并的chunk,如何从自身list里面快速删除。只能用List.可以快速删除自身的List
    //基于定长内存池做一个可以快速添加/删除节点的List--已经解决
    //实现思路:用vector保存:记住所在block位置以及在vector里的位置，以便快速删除
    void mergeWithNeighborChunkifNeed(MemHeadInfo* pMemInfo)
    {
        char* pAddr=(char*)(pMemInfo->posBegin);
        size_t memSize=pMemInfo->nRawMemSize;
        char* pAddrNew=pAddr;
        size_t newSize=memSize;
        size_t prevSize=*(pAddr-sizeof(size_t));                     //为0,表明没有前一个chunk
        size_t nextSize=*(pAddr+memSize+sizeof(size_t));   //为0,表明没有后一个chunk
        if(prevSize!=0)
        {
            MemHeadInfo* prevMemChunk=(MemHeadInfo*)(pAddr-prevSize);
            if(prevMemChunk->bFree)      //若空闲则合并前一块      
            {
                pAddrNew-=prevSize;
                newSize+=prevSize;    
                removeChunk(prevMemChunk);
            }
        }
        if(nextSize!=0)
        {
            MemHeadInfo* nextMemChunk=(MemHeadInfo*)(pAddr+memSize+nextSize);
            if(nextMemChunk->bFree)      //若空闲则合并后一块      
            {
                //pAddrNew-=prevSize;
                newSize+=prevSize;
               removeChunk(nextMemChunk);
            }
        }
        appendMemChunk(pAddrNew,newSize);
    }
    void removeChunk(MemHeadInfo* pMemInfo)
    {
        //pMemInfo->bFree=true;
        //pMemInfo->pParentList->deleteNode(pMemInfo->pPrevNodeInList);

        //新思路
        uint32_t blockNum=pMemInfo->BlockIndex;
        uint32_t subPos=pMemInfo->subPos;
        std::vector<MemHeadInfo>& CurrBlock=ListMemBlock[blockNum];
        MemHeadInfo& TailElement=CurrBlock[CurrBlock.size()-1];   //若运行到此size不可能为0，不需要判断溢出
        
        //去真实的内存地址修改,此处逻辑比较绕,添加注释待续
        MemHeadInfo* pRealAddress=(MemHeadInfo*)TailElement.posBegin;
        pRealAddress->subPos=subPos;
        TailElement.subPos=subPos;

        //把尾部block移到当前,
        CurrBlock[subPos]=TailElement;
        CurrBlock.pop_back();
        updateBlockFreeMask(blockNum,CurrBlock.size());   //待续用blockNum
    }
    void* Malloc(size_t mSize)
    {
          uint32_t blockNum=SizeToBlockNum(mSize);
          uint32_t blockNumFree=getFreeBlock(blockNum);
          //==0的情况考虑  
          if (blockNumFree>=FreeMaskBitPos_MAX)
          {
              return NULL;
          }
          
          //ListMemBlock[blockNumFree].popFont(res);  //肯定存在
          //取尾部，并移除
          std::vector<MemHeadInfo>& CurrBlock=ListMemBlock[blockNumFree];
          MemHeadInfo res=CurrBlock.back();
          CurrBlock.pop_back();
          updateBlockFreeMask(blockNumFree,CurrBlock.size()); 
          RepartitionChunk(&res,mSize,mSize);
          //到实际地址去刷新使用标记
          MemHeadInfo* pHeadInfo=(MemHeadInfo*)res.posBegin;
          pHeadInfo->bFree=false;//分配给用户使用
          return (char*)res.posBegin+sizeof(MemHeadInfo);
    }
    void Free(void* p)
    {
        if (p==NULL)
        {
            return;
        }
        MemHeadInfo* pHead=(MemHeadInfo*)((char*)p-sizeof(MemHeadInfo));
        pHead->bFree=true;        //归还
        giveBackChunk(pHead);
    }
private:
    typedef std::vector<MemHeadInfo> ListChunk;   ///优化待续,实际上保存指针即可
    std::vector<ListChunk> ListMemBlock;



private:
//C语言bit位操作宏,封装成模板待续
#define GetBit(num,pos) ((num)>>(pos)&1))           //获取某位bit值,放到最低位
#define SetBit1(num,pos) ((num)|=1<<(pos))          //把某bit位置1
#define SetBit0(num,pos) ((num)&=~(1<<(pos)))    //把某bit位置0
#define BitReverse(num,pos) ((num)^=1<<(pos))   //把bit位取反
//#define getbit(num,pos)   (num&=(1<<pos))          //取某bit位,保留原址

    uint32_t getFreeBlock(uint32_t blockNumbegin)
    {
        uint32_t pos=blockNumbegin/64;             //sizeof(uint64_t)*8bit=64
        unsigned int posbit=blockNumbegin%64;
        uint64_t curr=hasFreeMask[pos];
        curr>>=posbit;
        curr<<=posbit;
        if (curr!=0)  //has 1bit
        {
            return getLowBit1Pos(curr)+pos*64;
        }
        for (int n=pos+1;n<32;n++)
        {
             uint64_t tmp=hasFreeMask[pos];
             if (curr!=0)  //has 1bit
             {
                 return getLowBit1Pos(curr)+n*64;
             }
        }
        
        //throw("over flow");   //error todo:
        return FreeMaskBitPos_MAX;  //max bit pos
    }
    void updateBlockFreeMask(unsigned int blockNum,size_t num)
    {
        if (num==0)
        {
            unsigned int  pos=blockNum/64;
            unsigned int posbit=blockNum%64;
            SetBit0(hasFreeMask[pos],posbit);
        }
        else
        {
            unsigned int  pos=blockNum/64;
            unsigned int posbit=blockNum%64;
            SetBit1(hasFreeMask[pos],posbit);
        }
    }

    uint64_t hasFreeMask[32];
    enum{
        FreeMaskBitPos_MAX=32*64,   //sizeof(uint64_t)*8bit=64
    };
private:
    //improve by lizulin 2018-2020
    //获一个数某bitpos位置取后N个bit的值,不包含bitpos本身
    template <const unsigned int N>  //N=1,2,3,4,5
    static unsigned int getRightNBit(uint32_t x,unsigned int bitPos)  //bitPos>N+1
    {
        static_assert(N<sizeof(x)*8,"N out of range");
        x=x>>(bitPos-N);
        const uint32_t Mask=((1u<<N)-1);    //TODO:极大值越界？
        x&=Mask;   
        return x;
    };
    //获一个数某bitpos位置取后N个bit的值,不包含bitpos本身
    template <const unsigned int N>  //N=1,2,3,4,5
    static uint64_t getRightNBit(uint64_t x,unsigned int bitPos)  //bitPos>N+1
    {
        static_assert(N<sizeof(x)*8,"N out of range");
        x=x>>(bitPos-N);
        const uint64_t Mask=((1ULL<<N)-1);  //TODO:极大值越界？
        x&=Mask;   
        return x;
    };
    template <const size_t N>  //N=1,2,3,4,5
    static uint32_t  SizeToBlockNumStrategy(uint64_t  nSize)
    {
        unsigned int h1pos=getHightBit1Pos(nSize);
        if (N==1)
        {
            return h1pos;
        }
        uint64_t subIndex=getRightNBit<N-1>(nSize,h1pos);
        uint64_t index=(h1pos<<(N-1))+subIndex;   
        return (uint32_t)index;                          //blocknum不会太大
    }

    template <const size_t N>  //N=1,2,3,4,5
    static uint64_t BlockNumToSizeStrategy(uint32_t nBlock)
    {
        const uint64_t BlockL=1<<(N+1);
        if(nBlock<BlockL)             //分块号比较小时 特别处理,策略完善与描述待续
        {
            //return  BlockL<<N-1;
            if(N<=5)
            {
                return 16;     //16  //N=3,4,5 
            }
            return BlockL*2;   //to be confirn
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
        if (std::is_same<DevideType,DevideType6>::value)
        {
            return SizeToBlockNumStrategy<6>(nSize);
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
        if (std::is_same<DevideType,DevideType6>::value)
        {
            return BlockNumToSizeStrategy<6>(nBlock);
        }
    }

    //阶梯策略 根据大小 完善与测试,未完待续
    static size_t SizeToBlockNumLadder(size_t nSize)
    {
        const int MB=1024*1024;
        if (nSize<1*MB)
        {
            return SizeToBlockNumStrategy<4>();
        }
        if (nSize<128*MB)
        {
            return SizeToBlockNumStrategy<5>();
        }
        if (nSize<1024*MB)
        {
            return SizeToBlockNumStrategy<6>();
        }
        if (nSize<4096*MB)
        {
            return SizeToBlockNumStrategy<7>();
        }
    }
    //
    static size_t BlockNumToSizeLadder(uint32_t nBlock)
    {
        if (nBlock<128)
        {
            return BlockNumToSizeStrategy<4>(nBlock);
        }
        if (nBlock<512)
        {
            return BlockNumToSizeStrategy<5>(nBlock);
        }
        if (nBlock<1024)
        {
            return BlockNumToSizeStrategy<6>(nBlock);
        }
    }

    ////根据数据大小,快速找到分块号,快速方法
    //static size_t SizeToBlockNumStrategy1(size_t nSize)
    //{
    //    size_t  h1pos=getHightBit1Pos(nSize);
    //    return h1pos;
    //}
    static size_t downSize(size_t nSize)
    {
        uint32_t currBlock=SizeToBlockNum(nSize);
        uint64_t dSize=BlockNumToSize(currBlock-1);
        return dSize;
    }

};



#ifdef UNIT_TEST
#include "SuperTime.h"

USING_NAME_SPACE_MAIN

int Test_MemPoolFast()
{
#if 0
    std::vector<void*> ListMem;
    const int memChunkSize=8*1024-16;
    for (int n=0;n<1024*100;n++)
    {
        void* p=::malloc(memChunkSize);
        //::printf("p:%p =%llu\n",p,p);
        ListMem.push_back(p);
    }
    for (size_t n=0;(n+1)<ListMem.size();n++)
    {
        void* p=ListMem[n];
        void* pNext=ListMem[n+1];
        size_t offset;
            
            if(pNext>p)
            {
                 offset=(size_t)pNext-(size_t)p;
            }
            else
            {
                offset=(size_t)p-(size_t)pNext;
            }
        ::printf("p:%p pNext:%p chunkSize:%llu offset:%llu\n",p,pNext,memChunkSize,offset);
        free(p);
    }
#endif


    MemPoolF<> memPool;
    const int partSize=100*1024*1024;
    char* pNewPart=(char*)::malloc(partSize);
    memPool.appendNewPartMemSpace(pNewPart,partSize);


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

            unsigned int mallocSize=MyRand()%(partSize/10000);  //rand()
            //unsigned int randValue=rand();  //0-32767
            //unsigned int mallocSize=(randValue*100)%(PoolMgrSize);

            sumMem+=mallocSize;
            //printf("n:%u mallocSize:%u\n",n,mallocSize);

            tm.getBeginTime();
            for (unsigned int K=0;K<100;K++)
            {
                pData=memPool.Malloc(mallocSize);
                //memset(pData,53,mallocSize);
               printf("Index:%u mallocSize:%u p:%p\n",n*K,mallocSize,pData);

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


    return 0;
}



#endif

NAME_SPACE_MAIN_END

