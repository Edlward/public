#ifndef MemPoolFast_H
#define MemPoolFast_H

#include <vector>
//#include <stdint.h>
//#include <list>
#include <cstdio>
#include <cassert>
#include <cstdlib>

#include <mutex>
#include "global.h"
#include "Math.h"
//#include "Thread.h"         //test sleep
//#include "ThreadSync.h"   //mutex



class  MemPoolByFixedSize 
{
public :
    MemPoolByFixedSize(size_t  blockSize, size_t preAlloc=1024):
        m_blockSize(blockSize),
        m_allocatedNum(preAlloc)
    {
        //保留r空间 
        ListFree.reserve(preAlloc);
        for  (size_t  n = 0 ; n < preAlloc; ++n)
        {
            char* p=(char*)::malloc(m_blockSize);   //new  char [m_blockSize];
            ListFree.push_back(p);
        }
    }
    ~MemPoolByFixedSize()
    {
        //std::vector<char*>::iterator it;
        //for  (it = ListFree.begin(); it != ListFree.end(); ++it)   //delete[] *it;
        for  (size_t n=0;n<ListFree.size();n++)
        {
            char* p=ListFree[n];
            ::free(p);
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
                //char* p=new  char [m_blockSize];
                char* p=(char*)::malloc(m_blockSize);
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
    MemPoolByFixedSize(const  MemPoolByFixedSize &);
    MemPoolByFixedSize & operator  = (const  MemPoolByFixedSize &);
    size_t  m_blockSize;
    size_t  m_allocatedNum;
    //思路2,不用vector保存备用的指针，直接在node加一个指向下一级的指针,待实现
    std::vector<char*>  ListFree;
};

template<class T>
class MyObjectPool
{
public:
    MyObjectPool():pool(sizeof(T),0)
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
    MemPoolByFixedSize pool;
    MyObjectPool(const  MyObjectPool &);
    MyObjectPool & operator  = (const  MyObjectPool &);
};



template<typename T> 
class MySingleList;

template<typename T> 
class MySingleListNode
{
public:
    MySingleListNode():pNext(NULL){}
    //MySingleListNode(const T& item,MySingleListNode<T> *next=NULL):m_data(item),pNext(next)
    MySingleListNode(const T& item):m_data(item),pNext(NULL)
    {

    }
    ~MySingleListNode()
    {
        pNext=NULL;
    }
public:
    T GetData() const {return this->m_data;}
    T& GetDataRef(){return this->m_data;}
    friend class MySingleList<T >;
private:
public:
    T m_data;
    MySingleListNode* pNext;
};

template<typename T> 
class MySingleList
{
public:
    MySingleList()://head(new MySingleListNode<T>())
        pHead(NULL),pTail(NULL)
        ,m_length(0)
    {
        //pHead=new MySingleListNode<T>();
        pHead=CreateHeadNode();
        pHead->pNext=NULL;
        pTail=pHead;
    }
    ~MySingleList()
    {
        ClearData();
        //delete pHead;
        if (pHead!=NULL)
        {
            DestoryNode(pHead);
            pHead=NULL;
            pTail=NULL;
        }
    }
private:
    static MySingleListNode<T>* CreateHeadNode()   //has not valid data 
    {
        return new MySingleListNode<T>();
        //void* p=::malloc(sizeof(MySingleListNode<T>));
        //new(p) MySingleListNode<T>();
        //return (MySingleListNode<T>*)p;
    }
    static MySingleListNode<T>* CreateNode(const T& item)
    {
        return new MySingleListNode<T>(item);
        //void* p=::malloc(sizeof(MySingleListNode<T>));
        //new(p) MySingleListNode<T>(item);
        //return (MySingleListNode<T>*)p;
    }
    static void DestoryNode(MySingleListNode<T>* pNode)
    {
        delete pNode;
        //pNode->MySingleListNode<T>::~MySingleListNode();
        //::free(pNode);
    }
public:
    //size_t Length()const;                            //get the length
    size_t Length()const {return m_length;}     //get the length
    bool isEmpty()const {return Length()==0;}
    void Print();                                           //print the list
    void ClearData()
    {
        MySingleListNode<T> *pDelete;
        while(pHead->pNext!=NULL)
        {
            pDelete=pHead->pNext;
            pHead->pNext=pDelete->pNext;
            DestoryNode(pDelete);   // delete pDelete;
        }
        //pHead->pNext=NULL;
        pTail=pHead;
        m_length=0;
    }
    void pushBack(const T& item)
    {
         MySingleListNode<T>* pNodeNew=CreateNode(item);
         pTail->pNext=pNodeNew;
         pTail=pNodeNew;
         pNodeNew->pNext=NULL;  //repeat 
         ++m_length;
    }
    void pushBack(const T& item,MySingleListNode<T>** pPrev,MySingleListNode<T>** pCurr)
    {
        MySingleListNode<T>* pNodeNew=CreateNode(item);
        pTail->pNext=pNodeNew;
        pNodeNew->pNext=NULL;   //repeat
         *pPrev=pTail;
        *pCurr=pNodeNew;
         pTail=pNodeNew;
        ++m_length;

    }
    bool popFont(T& item)   //改 font 和popfont  get and pop
    {
         if (isEmpty())
         {
             return false;
         }
         MySingleListNode<T>* pCurr=pHead->pNext;
         if(pCurr==NULL)   //unnecessary 
         {
                return false;
         }

         item=pCurr->m_data;
         pHead->pNext=pCurr->pNext;    //pTail
         DestoryNode(pCurr);
         --m_length;
         return true;
    }

    //从head开始取下一个，有空改迭代器版本
    bool GetCurrNodeData(T& item,MySingleListNode<T>* pCurr)const    
    {
        if(pCurr==NULL)   //unnecessary 
        {
            return false;
        }
        item=pCurr->m_data;
        return true;
    }

    //delee node by it's prev Node point in list
    void deleteNode(MySingleListNode<T>* pPrev)
    {
        MySingleListNode<T>* pCurr=pPrev->pNext;
        MySingleListNode<T>* pNext=pCurr->pNext;
        pPrev->pNext=pNext;
        DestoryNode(pCurr);
        m_length--;
    }
    MySingleListNode<T>* getHead()const {return pHead;}
    MySingleListNode<T>* hasNext(MySingleListNode<T>* pNode)const   //判断是否存在下一个并返回，失败返回NULL
    {
        if (!pNode)
        {
            return NULL;
        }
        return pNode->pNext;
    }

private:
    MySingleListNode<T> *pHead;
    MySingleListNode<T> *pTail;
    int m_length;

public:
    MySingleList(const  MySingleList& R)
    {
        copyFrom(R);
    }
    MySingleList & operator  = (const  MySingleList& R)
    {
         copyFrom(R);
    }
    void copyFrom(const  MySingleList& R)
    {
        if (this!=&R)
        {
            this->ClearData();
            T tmp;
            MySingleListNode<T>* pNode=R.getHead();

            while (pNode=R.hasNext(pNode))
            {
                if (R.GetCurrNodeData(tmp,pNode))
                {
                      this->pushBack(tmp);
                }
            }
        }
    }

    //赋值swap暂不实现
};

//
////一种高性O1计算机系统数据资源(内存/硬盘)管理策略
////可以用在内存管理,硬盘/SSD/Flash数据写入管理。
////完全O1
////在硬盘上还可以实现碎片管理,搬迁合并,可以完全利用空间。
//
////逻辑segment block chunk 
////part 部分, 部, 部件, 份, 局部, 分
////piece 片, 一块, 块, 一片, 部分, 个
////section 部分, 部, 截面, 段, 章节, 剖面
// 
////MHeadInfo可以存储在实际使用的区域的头部，也可以用专门的地址存储。
////为了避免被破坏。尤其是用在SSD的情况
////用UI实时显示分配情况。QT写出来
//
////遇到问题，随着不断的创建归还，最后小碎片会越来越多？。分裂时若右边太小可以不合并
//
//
//struct MHeadInfo
//{  
//   char* posBegin;
//   size_t nRawMemSize;
//   bool bFree;    //表示此块是否空闲，决定是否可以合并分块，高效实现可以合并到void的某个bit。或者Size的最后1bit
//   MySingleList<MHeadInfo>* pParentList;    //也可仅仅保存parentIndex
//   MySingleListNode<MHeadInfo>* pPrevNodeInList;
//   MHeadInfo()
//   {
//       posBegin=NULL;
//       nRawMemSize=0;
//       bFree=false; 
//       pParentList=NULL;    //也可仅仅保存parentIndex
//       pPrevNodeInList=NULL;
//   }
//};
//
//struct EndMask
//{
//    size_t mSize;
//};
// 
//
////#define FAST_VERSION    //optimization version
// 
//
//class MemPoolFast
//{
//public:
//    MemPoolFast()
//    {
//        //test
//        //const int partSize=100*1024*1024;
//        //char* pNewPart=(char*)::malloc(partSize);
//        //appendNewPartMemSpace(pNewPart,partSize);
//        initBlock();
//    }
//    ~MemPoolFast()
//    {
//
//    }
//    void initBlock()
//    {
//        for(size_t n=0;n<64;n++)
//        {
//            ListChunk NullChunk;
//            ListMemBlock.push_back(NullChunk);
//        }
//
//    }
//    void appendNewPartMemSpace(char* pAddr,size_t memSize)
//    {
//        initPartMem(pAddr,memSize);
//        appendMemChunk(pAddr+sizeof(size_t),memSize-sizeof(size_t));
//    }
//
//    void initPartMem(char* pAddr,size_t memSize)
//    {
//        //不同内存片段之间头尾用0填充，防止，向前向后合并时超出界限。
//        //之前，之后没有可用的chunk。
//        size_t* pSizeHead=(size_t*)pAddr;
//        size_t* pSizeTail=(size_t*)(pAddr+memSize-sizeof(size_t));
//        *pSizeHead=0;
//        *pSizeTail=0;
//    }
//    void FillMemInfo(char* pAddr,size_t memSize)
//    {
//        MHeadInfo* pMemInfo=(MHeadInfo*)pAddr;
//        pMemInfo->posBegin=pAddr;
//        pMemInfo->nRawMemSize=memSize;
//        pMemInfo->bFree=true;
//    }
//    void appendMemChunk(char* pAddr,size_t memSize)
//    {
//         FillMemInfo(pAddr,memSize);
//         appendMemChunk((MHeadInfo*)pAddr);
//    }
//    void appendMemChunk(MHeadInfo* pMemInfo)
//    {
//        size_t blockNum=SizeToBlockNumStrategy1(pMemInfo->nRawMemSize);  //-  待续
//        MySingleListNode<MHeadInfo>* pPrev;
//        MySingleListNode<MHeadInfo>* pCurr;
//        pMemInfo->pParentList=&ListMemBlock[blockNum];
//
//        ListMemBlock[blockNum].pushBack(*pMemInfo,&pPrev,&pCurr);
//
//#ifdef FAST_VERSION
//        updateBlockNum(blockNum,1);
//#endif
//
//        //MHeadInfo& MHeadInfoInList=pCurr->GetDataRef();
//        //MHeadInfoInList.pPrevNodeInList=pPrev;
//        pCurr->m_data.pPrevNodeInList=pPrev;
//    }
//    //重新划分Chunk,返回刚好合适的大小,把多余部分归还到池中
//    void RepartitionChunk(MHeadInfo& memInfo,size_t oldSize)
//    {
//        if ((memInfo.nRawMemSize-oldSize)>(sizeof(MHeadInfo)+sizeof(EndMask)+8))
//        {
//            //memInfo.nRawMemSize=oldSize;
//            //size_t* pEnd=(size_t*)(memInfo.posBegin+sizeof(MHeadInfo)+oldSize);
//            //*pEnd=oldSize;
//            FillMemInfo(memInfo.posBegin,oldSize);
//            char* pRemainAddr=memInfo.posBegin+oldSize;
//            size_t RemainSize=memInfo.nRawMemSize-oldSize;
//            FillMemInfo(pRemainAddr, RemainSize);
//            appendMemChunk((MHeadInfo*)pRemainAddr);
//        }
//    }
//    void giveBackChunk(MHeadInfo* pMemInfo)
//    {
//        mergeWithNeighborChunkifNeed(pMemInfo);
//        //removeChunk(pMemInfo);
//        appendMemChunk(pMemInfo);
//    }
//
//    //遇到问题,即将被合并的chunk,如何从自身list里面快速删除。只能用List.可以快速删除自身的List
//    //基于定长内存池做一个可以快速添加/删除节点的List
//    void mergeWithNeighborChunkifNeed(MHeadInfo* pMemInfo)
//    {
//        char* pAddr=pMemInfo->posBegin;
//        size_t memSize=pMemInfo->nRawMemSize;
//        char* pAddrNew=pAddr;
//        size_t newSize=memSize;
//        size_t prevSize=*(pAddr-sizeof(size_t));                     //为0,表明没有前一个chunk
//        size_t nextSize=*(pAddr+memSize+sizeof(size_t));   //为0,表明没有后一个chunk
//        if(prevSize!=0)
//        {
//            MHeadInfo* prevMemChunk=(MHeadInfo*)(pAddr-prevSize);
//            if(prevMemChunk->bFree)      //若空闲则合并前一块      
//            {
//                pAddrNew-=prevSize;
//                newSize+=prevSize;
//                
//                removeChunk(prevMemChunk);
//                //prevMemChunk->pParentList->deleteNode(prevMemChunk->pPrevNodeInList);
//            }
//        }
//        if(nextSize!=0)
//        {
//            MHeadInfo* nextMemChunk=(MHeadInfo*)(pAddr+memSize+nextSize);
//            if(nextMemChunk->bFree)      //若空闲则合并前一块      
//            {
//                //pAddrNew-=prevSize;
//                newSize+=prevSize;
//
//               removeChunk(nextMemChunk);
//                //nextMemChunk->pParentList->deleteNode(nextMemChunk->pPrevNodeInList);
//            }
//        }
//        appendMemChunk(pAddrNew,newSize);
//    }
//    void removeChunk(MHeadInfo* pMemInfo)
//    {
//        pMemInfo->bFree=true;
//        pMemInfo->pParentList->deleteNode(pMemInfo->pPrevNodeInList);
//
//
//#ifdef FAST_VERSION
//        //updateBlockNum(blockNum,1);   //待续用blockNum
//#endif
//
//    }
//    void* Malloc(size_t mSize)
//    {
//#ifdef FAST_VERSION
//          size_t blockNum=SizeToBlockNumStrategy1(mSize);
//          size_t  tmpblockNumValidMask=blockNumValidMask;
//          tmpblockNumValidMask>>=blockNum;
//          tmpblockNumValidMask<<=blockNum;
//          //==0的情况考虑  
//          if (tmpblockNumValidMask==0)
//          {
//              return NULL;
//          }
//          size_t newblockNum=getHightBit1Pos(tmpblockNumValidMask);
//          MHeadInfo res;
//          ListMemBlock[newblockNum].popFont(res);  //肯定存在
//          RepartitionChunk(res,mSize);
//          //到实际地址去刷新使用标记
//          MHeadInfo* pHeadInfo=(MHeadInfo*)res.posBegin;
//          pHeadInfo->bFree=false;//分配给用户使用
//          return res.posBegin+sizeof(MHeadInfo);
//
//#else
//          size_t blockNum=SizeToBlockNumStrategy1(mSize);
//          MHeadInfo res;
//          for (size_t n=blockNum;n<64;n++)        //faster   use bit oparator find hight bit one
//          {
//               if (ListMemBlock[n].popFont(res))
//               {
//                   //return res.posBegin+sizeof(MHeadInfo);
//                   RepartitionChunk(res,mSize);
//                   //removeChunk(pHead);
//
//                   //到实际地址去刷新使用标记
//                   MHeadInfo* pHeadInfo=(MHeadInfo*)res.posBegin;
//                   pHeadInfo->bFree=false;//分配给用户使用
//                   return res.posBegin+sizeof(MHeadInfo);
//               }
//          }
//#endif
//          return NULL;
//    }
//    void Free(void* p)
//    {
//        if (p==NULL)
//        {
//            return;
//        }
//        MHeadInfo* pHead=(MHeadInfo*)((char*)p-sizeof(MHeadInfo));
//        //removeChunk(pHead);
//        pHead->bFree=true;    //归还
//        //appendMemChunk(pHead);
//        giveBackChunk(pHead);
//    }
//private:
//    //typedef std::vector<MHeadInfo> ListChunk;
//    typedef MySingleList<MHeadInfo> ListChunk;
//    std::vector<ListChunk> ListMemBlock;
//
//    //根据数据大小,快速找到分块号,快速方法
//    static size_t SizeToBlockNumStrategy1(size_t nSize)
//    {
//        size_t  h1pos=getHightBit1Pos(nSize);
//        return h1pos;
//    }
//
//
//#ifdef FAST_VERSION
//private:
//    void updateBlockNum(unsigned int blockNum,size_t num)
//    {
//        if (num==0)
//        {
//            size_t newMask=1ULL<<blockNum;
//            blockNumValidMask&=(~newMask);
//        }
//        else
//        {
//            size_t newMask=1ULL<<blockNum;
//            blockNumValidMask|=newMask;
//        }
//    }
//    size_t blockNumValidMask;
//#endif
//};



#ifdef UNIT_TEST
BASIC_API int Test_MemPoolFast();
#endif

NAME_SPACE_MAIN_END

#endif







