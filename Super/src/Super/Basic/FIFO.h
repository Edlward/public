#ifndef _FIFO_H
#define _FIFO_H

#include "global.h" 

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <algorithm>


NAME_SPACE_MAIN_BEGIN


//传入参数
class IsPOD;        //POD类型内部调用memcpy拷贝数据，其他类型用for循环依次赋值
class NotPOD;
class UnknowPOD;


//使用说明:
//StackBufSize表示内部栈空间个数,若数据容量小于此为了效率值将使用内部栈


//FIFO内部计数
//实现方式1:
//用读写索引来控制容量与内部计数w_index与r_index
//缺点是需要多开一个空间,在管理比较大内部结构会成为瓶颈
//相同表示有效数据为0


//实现方式2，不需要多开辟空间,但是内部需要一个辅助计数器count
//用来区分w_index==r_index是满还是空



template<typename T,const size_t InBufNum=0,class dataType=IsPOD>
class CFIFO
{
public:
    CFIFO(size_t capacity)
    {
        InitMember(capacity);
    }
    //不带默认参数版,需要外部调用Reinitmember,否则根据模板传参使用内部空间
    //为了避免作为数组成员使用无法批量初始化的问题
    CFIFO()    
    {
        InitMember(InBufNum);
    }
    ~CFIFO()
    {
        DestoryWorkSpace();
        DestoryWorkSpaceTmp(); 
    }
public:
    //外部调用,重新初始化,内部空间可能使用过,需要释放
    void ReInitMember(size_t nCapacity)
    {
         //释放旧的工作内存空间
         DestoryWorkSpace();
         DestoryWorkSpaceTmp(); 
         InitMember(nCapacity);
    }
    void Resize(size_t newSize)    //仅扩展容量,会释放旧数据，FIFO
    {
        ReInitMember(newSize);
    }
    void Clear()
    {
        ReInitMember(0);
    }
    //扩展容量并拷贝旧数据,待续。
private:
    void InitMember(size_t nCapacity)
    {
        pDataNew=NULL;
        pData=NULL;
        pDataTmp=NULL;
        m_Capacity=nCapacity;
        m_RingCount=m_Capacity+1;     //必须多开一个，否则windex/rindex无法表示刚好等于满容量的情况
        m_IndexWrite=0;
        m_IndexRead=0;  
        CreateWorkSpace();
    }
    void CreateWorkSpace()
    {
        size_t needBufferSize=m_RingCount*sizeof(T);

        //小容量直接用内部空间
        //if (m_Capacity<InBufSize)
        if (needBufferSize<=inArraySize)
        {
            //::printf("Use internal Mem Space:size=%u B=%u\n",sizeof(mData));
            //pData=mData;
            pData=(T*)m_buffer;
        }
        else     //大容量new堆空间 
        {
            //::printf("Use new Mem Space:size=%u Bytes\n",m_RingCount*sizeof(T));
            pDataNew=new T[m_RingCount];
            pData=pDataNew;
        }
    }
    void DestoryWorkSpace()
    {
        if (pDataNew!=NULL)
        {
            delete[] pDataNew;
            pDataNew=NULL;
        }
    }
    void DestoryWorkSpaceTmp()
    {
        if (pDataTmp!=NULL)
        {
            delete[] pDataTmp;
            pDataTmp=NULL;
        }
    }
public:
    //如果剩余空间足够才推送,否则一个也不存,返回失败
    bool pushDataIfPossible(const T* Array,size_t N)
    {   
        if(!isWriteSpaceEnough(N))
        {
            return false;
        }
        doPushN(Array,N);
        return true;
    }
    //如果有足够数据则取出，否则一个也不读,返回失败
    bool getDataIfPossible(T* Array,size_t N)
    {
        if(!isReadSpaceEnough(N))
        {
            return false;
        }
        doGetN(Array,N);
        return true;
    }
    //推送，返回已经推送个数,空间不够则有多少推送多少
    size_t push(const T* Array,size_t N)
    {
        size_t wSize=std::min<size_t>(getUnUseSpace(),N);
        return doPushN(Array,wSize);
    } 
    //取数据，如果有效数据不足，有多少取多少
    size_t get(T* Array,size_t N)
    {
        size_t rSize=std::min<size_t>(getUsedSpace(),N);
        return doGetN(Array,rSize);
    }
    //推送一个 
    bool push(const T& tmp)
    {
        //if(!isWriteSpaceEnough(1))
        size_t NextIndexWrite=getNextCycle(m_IndexWrite,m_RingCount);
        if(NextIndexWrite==m_IndexRead)   //if((m_IndexWrite+1)%m_RingCount==m_IndexRead)
        {
            return false;
        }
        pData[m_IndexWrite]=tmp;
        //m_IndexWrite=(m_IndexWrite+1)%m_RingCount;
        m_IndexWrite=NextIndexWrite;
        return true;
    }
    //获取一个
    bool get(T& tmp)
    {
        //if(!isReadSpaceEnough(1))
        //if(m_IndexRead==m_IndexWrite)
        if(IsEmpty())
        {
            return false;
        }
        tmp=pData[m_IndexRead];
        //m_IndexRead=(m_IndexRead+1)%m_RingCount;
        m_IndexRead=getNextCycle(m_IndexRead,m_RingCount);
        return true;
    } 

    //获取一个,直接返回数据指针使用,一些高效扩展场景会用到
    bool getDataRef(T*& pDataOut)
    {
        //if(!isReadSpaceEnough(1))
        //if(m_IndexRead==m_IndexWrite)
        if (IsEmpty())
        {
            return false;
        }
        //pDataOut=pData[m_IndexRead];
        pDataOut=&pData[m_IndexRead];
        //m_IndexRead=(m_IndexRead+1)%m_RingCount;
        m_IndexRead=getNextCycle(m_IndexRead,m_RingCount);
        return true;
    } 

    //快速获取数据的方法,一些高效应用场景,获取数据的优化
    //试图以直接使用内部指针的形式访问数据,前提是数据首尾地址在内部是线性的，没有分段的情况
    bool getDataRefIfPossible(T*& pDataOut,size_t N)
    {
        if (!isReadSpaceEnough(N))   //首先有足够数据
        {
            return false;
        }
        if (!isDataAddressLiner(N))     //数据没有被分段
        {
            return false;
        }
        pDataOut=&pData[m_IndexRead];
        return true;
    }
    //试图以直接使用内部指针的形式访问数据,如果数据被分段,则移动内部数据再访问
    bool getDataRefTryBest(T*& pDataOut,size_t N)
    {
        if (!isReadSpaceEnough(N))   //没有足够数据可读,返回失败
        {
            return false;
        }
        if (!isDataAddressLiner(N))   //如果数据被分段,则移动数据,使有效数据地址是线性递增的
        {
            ShiftDataAddressToLinner();
            //return false;
        }
        pDataOut=&pData[m_IndexRead];
        return true;
    }

    //以数组的形式访问数据--待完善
    size_t size()const {return getUsedSpace();}

    //以下用法需要判断index<size() 拿到的数据才是有效的
    T& operator [](size_t nIndex)
    {
        return this->at(nIndex);
    };
    const T& operator [](size_t nIndex)const 
    {
        return this->at(nIndex);
    }
    T& at(size_t nIndex)
    {
        size_t IndexR=getNextNCycle(m_IndexRead,nIndex,m_RingCount);
        return pData[IndexR];
    }
private:
    bool isDataAddressLiner(size_t N)
    {
        //其次数据没有被分段,当前索引+要获取的大小没有超过尾部
        //if ((m_IndexRead+N)>=(m_RingCount-1))    //ok
        if ((m_IndexRead+N)>=(m_Capacity))           //fast way  m_RingCount=m_Capacity+1;
        {
            return false;
        }
        return true;
    }
    void ShiftDataAddressToLinner()
    {
        //取出全部数据再重新插入，复位内部读写计数,数据地址就会变成线性递增的
        size_t availableSize=getUsedSpace();
        //T* pDataTmp=new T[availableSize];

        if (pDataTmp==NULL)
        {
            pDataTmp=new T[m_RingCount];
        }
        this->get(pDataTmp,availableSize);
        m_IndexRead=0;
        m_IndexWrite=0;
        this->push(pDataTmp,availableSize);

        //delete[] pDataTmp;

#if 0
        T tmpNnit;
        size_t posWbegin=m_IndexWrite;
        size_t posRbegin=m_IndexRead;
        size_t nCurr;
        size_t posNewW=0;
        for (nCurr=m_IndexWrite;nCurr!=m_IndexRead;nCurr=getNextCycleIndex(nCurr,m_RingCount))
        {
            tmpNnit=pData[posNewW];
            pData[posNewW]=pData[nCurr];
            posNewW++;
        }
        //读写位置重新设置
        m_IndexRead=0;
        m_IndexWrite=posNewW;
#endif
    }
private:
    //实际推送数据,wSizex<=剩余空间
    size_t doPushN(const T* Array,size_t wSize)
    {
        if((m_IndexWrite+wSize)<=m_RingCount)
        {
            CopyData(&pData[m_IndexWrite],Array,wSize);
        }
        else
        {
            size_t tailSize=m_RingCount-m_IndexWrite;
            CopyData(&pData[m_IndexWrite],Array,tailSize);
            size_t remainSize=wSize-tailSize;
            CopyData(&pData[0],Array+tailSize,remainSize);
        }
        //m_IndexWrite=(m_IndexWrite+wSize)%m_RingCount;
        m_IndexWrite=getNextNCycle(m_IndexWrite,wSize,m_RingCount);
        return wSize;
    } 
    //实际推送数据,rSizex<=有效空间
    size_t doGetN(T* Array,size_t rSize)
    {
        if((m_IndexRead+rSize)<=m_RingCount)
        {
            CopyData(Array,&pData[m_IndexRead],rSize);
        }
        else
        {
            size_t tailSize=m_RingCount-m_IndexRead;
            CopyData(Array,&pData[m_IndexRead],tailSize);
            size_t remainSize=rSize-tailSize;
            CopyData(Array+tailSize,&pData[0],remainSize);
        }
        //m_IndexRead=(m_IndexRead+rSize)%m_RingCount;
         m_IndexRead=getNextNCycle(m_IndexRead,rSize,m_RingCount);
        return rSize;
    }
public:

    //数据满时循环覆盖的推送方式,某些特殊应用会用到,
    //推送，返回已经推送个数
    size_t pushCover(const T* Array,size_t N)
    {
        size_t Vsize;   //=1+N%m_Capacity;   //数据量大于容器容量情况的考虑
        if (N>m_Capacity)
        {
            Vsize=m_Capacity;   //N%m_Capacity  m_Capacity%N; 
        }
        else
        {
            Vsize=N;
        }
        const T* VArray=Array+N-Vsize;
        size_t ValidSpace=getUnUseSpace();
        if (Vsize>ValidSpace)    //空间不够了,
        {
            size_t getNum=Vsize-ValidSpace;
            //::printf("clear old memory num:%u\n",(unsigned int)getNum);
            getNullData(getNum);    //取数据,仅改变内部计数,为了效率实际不取,为了清空空间以备推送数据。
        }
        return this->push(VArray,Vsize);
    } 
    //以覆盖的形式推送一个,剩余空间不够则腾出再推送
    bool pushCover(const T& tmp)
    {
#if 1   //faster
        size_t NextIndexWrite=getNextCycle(m_IndexWrite,m_RingCount);
        if(NextIndexWrite==m_IndexRead)
        {
            m_IndexRead=getNextCycle(m_IndexRead,m_RingCount);
        }
        pData[m_IndexWrite]=tmp;
        m_IndexWrite=NextIndexWrite;
        return true;
#else
        //if(!isWriteSpaceEnough(1))
        if((m_IndexWrite+1)%m_RingCount==m_IndexRead)
        {
            //return false;
            //getNullData(1);
            m_IndexRead=(m_IndexRead+1)%m_RingCount;
        }
        pData[m_IndexWrite]=tmp;
        m_IndexWrite=(m_IndexWrite+1)%m_RingCount;
        return true;
#endif
    }


    //仅取数据,不改变内部计数,如用在搜索,打印容器内部数据,但是不想取出,不想改变FIFO容器状态
    size_t getonlyData(T* Array,size_t N)
    {
        size_t rSize=std::min<size_t>(getUsedSpace(),N);
        if((m_IndexRead+rSize)<=m_RingCount)
        {
            CopyData(Array,&pData[m_IndexRead],rSize);
        }
        else
        {
            size_t tailSize=m_RingCount-m_IndexRead;
            CopyData(Array,&pData[m_IndexRead],tailSize);
            size_t remainSize=rSize-tailSize;
            CopyData(Array+tailSize,&pData[0],remainSize);
        }
        //m_IndexRead=(m_IndexRead+rSize)%m_RingCount;
        return rSize;
    } 
    //仅仅改变内部计数,为了效率，实际不取数据，
    size_t getNullData(size_t N)
    {
        size_t rSize=std::min<size_t>(getUsedSpace(),N);
        if((m_IndexRead+rSize)<=m_RingCount)
        {
        }
        else
        {
        }
        //m_IndexRead=(m_IndexRead+rSize)%m_RingCount;
        m_IndexRead=getNextNCycle(m_IndexRead,rSize,m_RingCount);
        return rSize;
    } 

    //推送空数据,为了效率实际不拷贝数据，仅仅改变FIFO容器内部计数，某些特殊应用占位用
    size_t pushNullData(size_t N)
    {
        size_t wSize=std::min<size_t>(getUnUseSpace(),N);
        if((m_IndexWrite+wSize)<=m_RingCount)
        {
        }
        else
        {
        }
        //m_IndexWrite=(m_IndexWrite+wSize)%m_RingCount;
        m_IndexWrite=getNextNCycle(m_IndexWrite,wSize,m_RingCount);
        return wSize;
    }

    //特殊需要应用,FIFO一般是先进先出,从头部取数据,从尾部插入数据
    //从头部插入数据
    bool pushFront(const T& tmp)
    {
        if (IsFull())
        {
            return false;
        }
        m_IndexWrite=getPrevCycle(m_IndexWrite,m_RingCount);
        m_IndexWrite=tmp;
        return true;
    }

    //从尾部取数据
    bool getBack(T& tmp)
    {
        if (IsEmpty())
        {
            return false;
        }
        m_IndexRead=getPrevCycle(m_IndexRead,m_RingCount);
        tmp=pData[m_IndexRead];
        return true;
    }
    //从头部添加数据N个,正序,倒叙考虑，有必要在补全
    bool pushFront(const T& Array,size_t N)
    {
        if (!isWriteSpaceEnough(N))
        {
            return false;
        }
        size_t nIndex=0;
        for (;m_IndexWrite!=m_IndexRead;)   //while(!isFull())
        {
                m_IndexWrite=Array[nIndex++];
                m_IndexWrite=getPrevCycle(m_IndexWrite,m_RingCount);
        }
        return true;
    }
    //从尾部获取数据N个,正序,倒叙考虑，有必要在补全
    bool getBack(T& Arry,size_t N)
    {
        if (!isReadSpaceEnough(N))
        {
            return false;
        }
        size_t nIndex=0;
        for (;m_IndexRead!=m_IndexWrite;)   //while(!isEmpty())
        {
            m_IndexRead=getPrevCycle(m_IndexRead,m_RingCount);
			Arry[nIndex++]=pData[m_IndexRead];
        }
        return true;
    }

    //取FIFO容器容量
    size_t getCapacity()
    {
        return m_Capacity;
    }
    bool  IsFull()
    {
        return getUsedSpace()==getCapacity();
    }
    bool IsEmpty()
    {
        //return getUsedSpace()==0;
        return m_IndexRead==m_IndexWrite;
    }
    //取已经使用空间大小，已使用空间
    size_t getUsedSpace()
    {
        if(m_IndexWrite>=m_IndexRead)
        {
            return m_IndexWrite-m_IndexRead;
        }
        //return m_IndexWrite+(m_Capacity-m_IndexRead);   
        return m_IndexWrite+(m_RingCount-m_IndexRead);   
    }
    //取剩余未使用空间大小
    size_t getUnUseSpace()
    {
        return m_Capacity-getUsedSpace();
       //return m_RingCount-getUsedSpace();
    }
    //是否有足够剩余空间来写入
    bool isWriteSpaceEnough(size_t wSize)
    {
        return wSize<=getUnUseSpace();
    }
    //是否有足够数据可读取
    bool isReadSpaceEnough(size_t rSize)
    {
        return rSize<=getUsedSpace();
    }  
    //调试用,打印内部数据
    void PrintData()
    {
        //printf("PrintData() Not implemented\n");
        size_t availableSize=getUsedSpace();
        int* pDataTmp=new int[availableSize];
        getonlyData(pDataTmp,availableSize);
        ::printf("Currdata: availableSize:%u\n",(unsigned int)availableSize);
        for (size_t n=0;n<availableSize;++n)
        {
            ::printf("%03d ",pDataTmp[n]);
            if (n%16==15)
            {
                ::printf("\n");
            }
        }
        delete[] pDataTmp;
    };
    void PrintStatus()
    {
        printf("PosR:%u posW:%u rCount:%u\n",m_IndexRead,m_IndexWrite,m_RingCount);
    }


    //不同数据类型的打印特化待研究
#if 0
template<const size_t StackBufSize,class dataType>
void PrintData(){}
template<typename T>
void PrintData()<int>
{
        //printf("PrintData() Not implemented\n");
        size_t nValid=getUsedSpace();
        int* pDataTmp=new int[nValid];
        getonlyData(pDataTmp,nValid);
        ::printf("Currdata:\n");
        for (size_t n=0;n<nValid;++n)
        {
            ::printf("%d ",pDataTmp[n]);
            if (n%16==15)
            {
                ::printf("\n");
            }
        }
        delete[] pDataTmp;
}
#endif

    size_t memUsed()const //本类所占用内存空间预估,包括所指向的堆内存,辅助调试用
    {
          size_t  memUsedSize=sizeof(*this);
          if (pDataNew)
          {
              memUsedSize+=m_RingCount*sizeof(T);
          }
          if (pDataTmp)
          {
              memUsedSize+=m_RingCount*sizeof(T);
          }
          return memUsedSize;
    }
    size_t memUsedSize;
private:
    //void CopyData(T* dst,const T* src,size_t N)
    //{
    //    doCopyData<dataType>(dst,src,N);
    //}
    //template<class IsPOD>
    //void doCopyData(T* dst,const T* src,size_t N)
    //{
    //    //::printf("copy as POD\n");
    //    memcpy(dst,src,N*sizeof(T));
    //}
    //template<>
    //void doCopyData<NotPOD>(T* dst,const T* src,size_t N)
    //{
    //    //::printf("copy as NotPOD\n");
    //    for (size_t n=0;n<N;++n)
    //    {
    //        dst[n]=src[n];
    //    }
    //}
    //template<>
    //void doCopyData<UnknowPOD>(T* dst,const T* src,size_t N)
    //{
    //    //::printf("copy as UnknowPOD\n");
    //    for (size_t n=0;n<N;++n)
    //    {
    //        dst[n]=src[n];
    //    }
    //}
    //未指定的,为了安全按照NotPOD处理
    //template<>
    //void doCopyData(T* dst,const T* src,size_t N)
    //{
    //   doCopyData(dst,src,N)<NotPOD>;
    //}

    //C++类成员函数模板特化,以上写法只能在VS编译通过,不符合C++标准,gcc编译错误

    void CopyData(T* dst,const T* src,size_t N)
    {
        if (std::is_same<dataType,IsPOD>::value)
        {
               ::memcpy(dst,src,N*sizeof(T));
        }
        else
        {
            T* dstTmp=dst;
            const T* srcTmp=src;
            for (size_t n=0;n<N;++n)
            {
                *(dstTmp++)=*(srcTmp++);  //faster than dst[n]=src[n];
            }
        }
    }

private:
    enum 
    {
        inArraySize=(InBufNum==0)?1:((InBufNum+1)*sizeof(T)),   //usesize=buffersize+1
    };
    //避免浪费或0字节数组
    char m_buffer[inArraySize];

    //T mData[InBufSize+1];    //内部空间,为了效率,容量小时用
    T* pDataNew;                       //new的堆空间
    T* pData;                              //正在使用的地址,根据传参决定
    T* pDataTmp;                       //临时数据指针,在移动数据时候会用到

    size_t m_Capacity;
    size_t m_RingCount;
    size_t m_IndexWrite;    //w_index与r_index相同表示有效数据为0，满如何表示。r_index=0,w_index=m_size-1;?
    size_t m_IndexRead;
};



//Linux内核kfifoC++封装 
//C++ 环形缓冲区的实现
//本文参考linux系统中 kfifo缓冲区实现.由于没有涉及到锁,在多线程环境下,只适用于 单生产者 + 单消费者 模型.
//////////////////////////////////////////////////////////////////////////
// when out < in
// |                                        |
// |----------------------------------------|
// 0       out|~~~~~~~~~~|in                size
//////////////////////////////////////////////////////////////////////////
// when out > in
// |                                        |
// |----------------------------------------|
// 0~~~~~~~|in        out|~~~~~~~~~~~~~~~~~~size
//////////////////////////////////////////////////////////////////////////

//判断一个整数是否为2的幂 
//方法一：
//将2的幂次方写成二进制形式后，很容易就会发现有一个特点：二进制中只有一个1，并且1后面跟了n个0； 因此问题可以转化为判断1后面是否跟了n个0就可以了。
//如果将这个数减去1后会发现，仅有的那个1会变为0，而原来的那n个0会变为1；因此将原来的数与去减去1后的数字进行与运算后会发现为零。

//inline bool isPowofTwo(size_t a)
//{
//    return  (a&(a-1));
//}


//方法二：
//拿这个数来除以2,得到商和余数,再用商除以2,又得到商和余数,重复上面的操作,直到商为0,当商为0,
//余数也为0时,这个数就是2的整数次幂当商为0,余数不为0时,这个数就不是2的整数次幂。

//inline bool isPowofTwo2(int Num)
//{
//    if(Num==1)
//    {
//        return true;
//    }
//    else
//    {
//        do
//        {
//            if(Num%2==0)
//            {
//                Num/=2;
//            }
//            else
//            {
//                 return false;
//            }
//        }while(Num!=1);
//        return true;
//    }
//    return false;
//}
//
//
//
//
////一个整数以向2的幂扩展
//inline size_t roundup_pow_of_two(const size_t x)
//{
//    if (x == 0){ return 0;}
//    if (x == 1){ return 2;}
//    size_t ret = 1;
//    while (ret < x)
//    {
//        ret = ret << 1;
//    }
//    return ret;
//}
//
//inline int log2(int value) //递归判断一个数是2的多少次方  
//{
//    if(value==1)
//        return 0;
//    else
//        return  1+log2(value>>1);
//}
//
//inline int log2NonRecursive(int value)//非递归判断一个数是2的多少次方
//{
//    int x=0;
//    while(value>1)
//    {
//        value>>=1;
//        x++;
//    }
//    return x;
//}

#if 0
#include <stdint.h>
class KFIFO
{
public:
public:
    size_t Capacity(){ return m_Capacity;}
    const char *Buffer(){ return buffer; }
private:
   size_t  Min(size_t  left, size_t   right){ return left > right ? right : left; }
private:
    size_t   m_Capacity;
    size_t   m_RingCount;
    volatile  size_t   in;
    volatile  size_t  out;
    char *buffer;
    KFIFO::KFIFO(const  size_t   Capacity) :in(0), out(0)
    {
        this->m_Capacity =Capacity;
        //m_RingCount=m_Capacity+1;
        m_RingCount=m_Capacity;
        buffer = new char[m_RingCount];  // (std::nothrow)   if (!buffer){ return; }
        memset(buffer,0, m_RingCount);
    }
    KFIFO::~KFIFO()
    {
        if (buffer)
        {
            delete[] buffer;
        }
    }
    size_t   KFIFO::Put(const char *buf, const  size_t  bufLen)
    {
        size_t lengthToPut = Min(bufLen, UnUsedSize());
        /* first put the data starting from fifo->in to buffer end */
        size_t len = Min(lengthToPut, m_Capacity - (in % m_Capacity));
        memcpy(buffer + (in % m_Capacity), buf, len);
        /* then put the rest (if any) at the beginning of the buffer */
        memcpy(buffer, buf + len, lengthToPut - len);
        in += lengthToPut;
        return lengthToPut;
    }

    size_t KFIFO::Get(char buf[], const size_t maxBufLen)
    {
        size_t lengthToGet = Min(maxBufLen, UsedSize());
        /* first get the data from fifo->out until the end of the buffer */
        size_t len = Min(lengthToGet, out % m_Capacity);
        memcpy(buf, buffer + (out % m_Capacity), len);
        /* then get the rest (if any) from the beginning of the buffer */
        memcpy(buf + len, buffer, lengthToGet - len);
        out += lengthToGet;
        return lengthToGet;
    }

    size_t KFIFO::UnUsedSize()
    {
        return m_Capacity-UsedSize();     //in + out;
    }

    size_t KFIFO::UsedSize()
    {
        return in-out;
    }
};


#endif



#ifdef UNIT_TEST
int Test_FIFO();
#endif

NAME_SPACE_MAIN_END

#endif



