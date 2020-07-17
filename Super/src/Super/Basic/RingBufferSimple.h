#ifndef RINGBUFFERSIMPLE_H_
#define RINGBUFFERSIMPLE_H_

//#include "RingBufferSimple.h"   //简单版的环形缓冲区
//为了声明与实现分离，不用模版实现，可以避开很多编译错误
//解决了线程同步基础件必须包含.h文件实现后，已经没有必要，暂留不再维护 2020/03/20

#include "global.h" 

NAME_SPACE_MAIN_BEGIN

class BASIC_API RingBufferSimple
{
private://disable copy
    RingBufferSimple(const RingBufferSimple&);
    RingBufferSimple& operator=(const RingBufferSimple&);
private:
    char* pData;                                     // 存放数据的指针
    unsigned int DataSize;                      // 数据类型结构大小
    unsigned int Capacity;                      // 缓冲区大小
    unsigned int RingCountMax;            //为了区分能区分WriteIndex=ReadIndex 是满还是空的情况，多开一个空间
    unsigned int WriteIndex;                   // 当前写入位置
    unsigned int ReadIndex;                   // 当前读取位置
    int Count;                                         // A count for judge buffer empty or not
    class Impl;
    Impl* pImpl;
    char bufimpl[128];
public:
    RingBufferSimple(const unsigned int DataSize,const unsigned int bufferSize = 8);
    ~RingBufferSimple();                                             

    // 往缓冲区读写数据,否则当flag为true，则无限等待，false则默认时间等待
    bool pushOnlySingle(const void* cargo, bool flag);   //为了效率用引用
    //T get(bool flag = true); //用返回NULL表明取数据失败不科学，如果是整数缓冲区null为0是有意义的, refactor by lizulin
    //改如下
    bool getOnlySingle(void* cargo,bool flag);      

    //往缓冲区读写数据 以timeout作为延时,time=0XFFFFFFFF则无限等待
	bool pushOnlySingle(const void* cargo,unsigned int timeout=Wait_Dafault);      //不能设定默认参数，否则调用不明确
	bool getOnlySingle(void* cargo,unsigned int timeout=Wait_Dafault);     

    //多生产者或多消费者版本
    bool pushByMultiple(const void* cargo, bool flag);
    bool getByMultiple(void* cargo,bool flag);
    bool pushByMultiple(const void* cargo,unsigned int timeout=Wait_Dafault);
    bool getByMultiple(void* cargo,unsigned int timeout=Wait_Dafault);     

    unsigned int getCapacity() const {return Capacity;}   // 返回总缓冲区大小，初始化时候决定
    unsigned int getWritePos() const {return WriteIndex;}  // 获取写入位置
    unsigned int getReadPos() const {return ReadIndex;}   // 获取读取位置
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
    bool clear(bool flag = true);                             // 清空缓冲区--flag为true表明存放的是new出来的指针,需要释放

private:
    //实际直接存取数据 私有
    void pushData(const void* cargo);
    void getData(void* cargo);
};


//改通过命名信号量，实现跨进程的ringbufer，一个进程里面push，另一个进程里面get，
//通过共享内存实现数据交互
class BASIC_API ringbufferProcessSimple
{
private://disable copy
    ringbufferProcessSimple(const ringbufferProcessSimple&);
    ringbufferProcessSimple& operator=(const ringbufferProcessSimple&);
private:
    char* pData;   // 存放数据的指针
    unsigned int DataSize;                        
    unsigned int Capacity;                     // 缓冲区大小
    unsigned int RingCountMax;            //为了区分能区分WriteIndex=ReadIndex 是满还是空的情况，多开一个空间
    //读写可能在不同线程，需要把一些计数器变量在进程间共享，考虑避免多次初始化问题
    struct ShareCount
    {
        int initKey;                                        //创建者标记，决定是否初始化以下变量
        unsigned int Capacity;                    //缓冲区大小
        unsigned int WriteIndex;                  //当前写入位置
        unsigned int ReadIndex;                   //当前读取位置
        //int Count;                                         //记录有效货物数量 A count for judge buffer empty or not
    };
    ShareCount* pSC;
    class Impl;
    Impl* pImpl;
public:
     ringbufferProcessSimple(unsigned int DataSize,unsigned int bufferSize = 8,const char* name="");
     ~ringbufferProcessSimple();   

    // 往缓冲区写数据,flag为true，则无限等待，false则有限时间内没有空位则丢弃数据
    bool pushOnlySingle(const void* cargo, bool flag);
    bool getOnlySingle(void* cargo,bool flag);           

    //往缓冲区读写数据 以timeout作为延时,time=0XFFFFFFFF则无限等待
	bool pushOnlySingle(const void* cargo,unsigned int timeout=Wait_Dafault);      //不能设定默认参数，否则调用不明确
	bool getOnlySingle(void* cargo,unsigned int timeout=Wait_Dafault);     

    //多生产者或多消费者版本
    bool pushByMultiple(const void* cargo, bool flag);
    bool getByMultiple(void* cargo,bool flag);
    bool pushByMultiple(const void* cargo,unsigned int timeout=Wait_Dafault);
    bool getByMultiple(void* cargo,unsigned int timeout=Wait_Dafault);     

    unsigned int getCapacity() const {return Capacity;}                                 // 返回总缓冲区大小，初始化时候决定
    unsigned int getWritePos() const {return pSC->WriteIndex;}                       // 获取写入位置
    unsigned int getReadPos() const {return pSC->ReadIndex;}                        // 获取读取位置
    inline bool IsFull() const 
    {
        return pSC->WriteIndex==pSC->ReadIndex;  //faster
        //if(getUsedSpace()>=Capacity)
        //{ 
        //    return true; 
        //}
        //return false;
    }
    inline bool IsEmpty() const 
    {
        if(getUsedSpace()==0)
        { 
            return true; 
        }
        return false;
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
    bool clear(bool flag = true);                        // 清空缓冲区--flag为true表明存放的是new出来的指针,需要释放

private:
    //实际直接存取数据 私有
    void pushData(const void* cargo);
    void getData(void* cargo);
};


#ifdef UNIT_TEST
BASIC_API int Test_ringbufferSimple();
#endif


 NAME_SPACE_MAIN_END
#endif



