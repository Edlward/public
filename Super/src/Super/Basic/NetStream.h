#ifndef _NetStream_H
#define _NetStream_H

#include "global.h"

//各种数据类型到网络流,本地流之间的转换
//设计思想，从网络或本地还原数据，考虑大小端，是否分离读写流，未完待续

/*
字节序不是由操作系统决定的，而是由cpu架构决定的，比如 x86 的是 little endian，
而 PPC (PowerPC) 是big endian。 所以跑在 x86 上的 linux/windows 都是小端，
而跑在 PPC 上的linux则是大端。 
(PS: 实际上 PowerPC 是即支持大端也支持小端，但是由于历史原因默认用大端）
JAVA语言默认采用大端:
*/

#include <string>
#include <iostream>
#include <vector>
#include <memory.h>


#include "global.h"


 NAME_SPACE_MAIN_BEGIN
enum EByteOrder
{
    Endian_Little,   //小端字节序LSB
    Endian_Big,      //大端字节序MSB
};

//为了避免复杂包含，基础操作与字节序转换函数复制一份到此处 加NS_前缀

inline std::wstring NS_StringToWString(const std::string &str)
{
    std::wstring wstr(str.length(),L' ');
    std::copy(str.begin(), str.end(), wstr.begin());
    return wstr; 
}
//只拷贝低字节至string中
inline std::string NS_WStringToString(const std::wstring &wstr)
{
    std::string str(wstr.length(), ' ');
    std::copy(wstr.begin(), wstr.end(), str.begin());
    return str; 
}


inline bool NS_isBigEndian()
{
    int a = 0x12345678;  
    char * p = (char*)&a;  
    if (*p==0x12)  
    {  
        return true;
    }  
    else 
    {
        return false; 
    }
}
inline bool NS_isLittleEndian()
{
    return !NS_isBigEndian();
}


inline int NS_getByteOrder()
{
    if (NS_isBigEndian())
    {
         return Endian_Big;
    }
   return Endian_Little;
};

inline uint16_t NS_ByteOrderSwap16(uint16_t x)
{
    return (((uint16_t)(x) & 0x00ff) << 8)|\
        (((uint16_t)(x) & 0xff00) >> 8) ;
}

inline uint32_t NS_ByteOrderSwap32(uint32_t x)
{
    return (((uint32_t)(x) & 0xff000000)>>24)|\
        (((uint32_t)(x) & 0x00ff0000)>>8)|\
        (((uint32_t)(x) & 0x0000ff00)<<8)|\
        (((uint32_t)(x) & 0x000000ff)<<24);
}

inline uint64_t NS_ByteOrderSwap64(uint64_t x)
{
    return  (((uint64_t)x&0xff00000000000000LL)>>56|\
        ((uint64_t)x & 0x00ff000000000000LL)>>40| \
        ((uint64_t)x & 0x0000ff0000000000LL)>>24|\
        ((uint64_t)x & 0x000000ff00000000LL)>>8 |\
        ((uint64_t)x & 0x00000000ff000000LL)<<8 |\
        ((uint64_t)x & 0x0000000000ff0000LL)<<24|\
        ((uint64_t)x & 0x000000000000ff00LL)<<40|\
        ((uint64_t)x & 0x00000000000000ffLL)<<56);
}

//浮点数的字节序转换
union NS_float_conv
{
    //char c[4];
    float f;
    uint32_t n;
};

union NS_double_conv
{
    double f;
    uint64_t n;
};

inline float NS_ByteOrderSwapfloat(float x)
{
    //float_conv& tt=*((float_conv*)&x);  //方法1

    NS_float_conv tt; //方法二
    tt.f=x;
    tt.n=NS_ByteOrderSwap32(tt.n);
    return tt.f;
}

inline double NS_ByteOrderSwapdouble(double x)
{
    NS_double_conv tt;
    tt.f=x;
    tt.n=NS_ByteOrderSwap64(tt.n);
    return tt.f;
}

inline void NS_ByteOrderSwap24(char* p)
{
    char tmp=*p;
    *(p)=*(p+2);
    *(p+2)=tmp;
}

const unsigned int sizeHead=64; //待续     //sizeof(KL);            //预留报文头大小
const int dstByteOrder=Endian_Little;                //目标字节序,本系统大多数情况在小端机器运行，为效率，收发双方统一到小端
const int CurrByteOrder=NS_getByteOrder();     //当前机器的字节序

//字节序统一方法：
//1.大多数情况在小端机器运行，为效率，收发双方统一到小端
//2.或者为了减轻服务端的负载，一般统一到服务端的字节序，客户端登陆上，服务端首个报文立即把其字节序通知给客户端
//3.或者每个报文客户端自动根据报文头常量值判断服务器的字节序。如服务端发0x7585,  客户端可能收到0X7585或0x8575


//网络流中的数据长度类型,如字符串长度不能用size_t,必须是确定的,为了长远考虑固定用64位的uint64_t
typedef uint64_t NetStreamSizeType;

class InNetStream
{
private:
    char* bufRaw; 
    char* posDataBegin;
    char* pData;
    unsigned int maxLen;
    char* pos;
    size_t useLen;
    char* newBuf;
    bool useNewbuf;
    bool wasSetHead;

private:
    bool bSwapByteOrder;   //是否需要反转字节序
private:
    InNetStream(const InNetStream&);
    InNetStream& operator=(const InNetStream&);
public:
    InNetStream(char* buf,unsigned int maxLen)
    {
        bSwapByteOrder=(CurrByteOrder!=dstByteOrder);

        bufRaw=buf;
        this->maxLen=maxLen;

        pData=bufRaw+sizeHead;
        pos=pData;
        posDataBegin=pData;

        newBuf=NULL;
        useNewbuf=false;
        useLen=0;
        wasSetHead=false;
    }
    ~InNetStream(void)
    {
          releaseBuffer();
    }
    char* data()const {return pData;}
    size_t size() const {return useLen;}
    void clear()
    {
        releaseBuffer();
        useLen=0;
    }
    void resizeBuffer()
    {
        char* buffer=new char[maxLen*2];
        char* pbufferlast=posDataBegin;
        memcpy(buffer,posDataBegin,useLen);
        newBuf=buffer;
        posDataBegin=buffer;
        pData=posDataBegin;
        if (wasSetHead)
        {
            pData=posDataBegin+sizeHead;
        }
        pos=pData+useLen;
        if (useNewbuf)
        {
            delete[] pbufferlast;
        }
        useNewbuf=true;
    }
    void resizeIfNeed(size_t len)  //容量不够则扩容
    {
        if ((useLen+len)>maxLen)
        {
            //resizeBuffer();
        }
    }
    void releaseBuffer()
    {
        if (newBuf)
        {
            delete[] newBuf;
            newBuf=0;
        }
    }

    //设置报文头
#if 0
    void setHead(const KL* pkl)
    {
        setHead(*pkl);
    }
    void setHead(const KL& kl)
    {
        char* posold=pos;
        pos=bufRaw;
        this->operator<<(kl.head);
        this->operator<<(kl.key);
        this->operator<<(kl.len);
        pos=posold;
        posDataBegin=bufRaw;
    }
#endif

    template<typename T>
    InNetStream& operator<<(const T& type)
    {
        resizeIfNeed(sizeof(type));
        *((T*)pos)=type;
        pos+=sizeof(type);
        useLen+=sizeof(type);
        return *this;
    }

    //各种指针类型的是否需要特殊考虑--与数组是否冲突
    //template<typename T>
    //InNetStream& operator<<(const T*& type)
    //{
    //    resizeIfNeed(sizeof(type));
    //    *((T**)pos)=type;
    //    pos+=sizeof(type);
    //    useLen+=sizeof(type);
    //    return *this;
    //}

    //用put或者重载操作符都可以
    template<typename T>
    inline InNetStream& Put(const T& type)
    {
        return this->operator<<(type);
    }

    //template<typename T,unsigned int N>
    //inline InNetStream& Put(const T (&p)[N])  //不行？
    //{
    //    return this->operator<<((&p[N]));
    //}


    //某种类型数组，如Put(int a[20])
    template<typename T,unsigned int N>
    InNetStream& operator<<(const T (&p)[N])
    {
        for (unsigned int n=0;n<N;n++)
        {
            operator<<(p[n]);
        }
        return *this;
    }
    template<typename T,unsigned int N>
    InNetStream& Put(const T (&p)[N])
    {
        for (unsigned int n=0;n<N;n++)
        {
            operator<<(p[n]);
        }
        return *this;
    }
    //不需要考虑大小端的单字节类型数组，为了效率偏特化
    template<unsigned int N>
    InNetStream& operator<<(const bool (&p)[N])
    {
        resizeIfNeed(sizeof(p));
        memcpy(pos,p,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    template<unsigned int N>
    InNetStream& Put(const bool (&p)[N])
    {
        resizeIfNeed(sizeof(p));
        memcpy(pos,p,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    template<unsigned int N>
    InNetStream& operator<<(const char (&p)[N])
    {
        resizeIfNeed(sizeof(p));
        memcpy(pos,p,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    template<unsigned int N>
    InNetStream& Put(const char (&p)[N])
    {
        resizeIfNeed(sizeof(p));
        memcpy(pos,p,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    template<unsigned int N>
    InNetStream& operator<<(const unsigned char (&p)[N])
    {
        resizeIfNeed(sizeof(p));
        memcpy(pos,p,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    template<unsigned int N>
    InNetStream& Put(const unsigned char (&p)[N])
    {
        resizeIfNeed(sizeof(p));
        memcpy(pos,p,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    //template<typename T>
    //friend InNetStream& operator<<(InNetStream &sin,T type);

    //friend InNetStream& operator<<(InNetStream &sin,T type);
    
    //基础数据类型--单字节的不需要考虑大小端
    inline InNetStream& operator<<(const bool& n)
    {
        resizeIfNeed(sizeof(n));
        *((bool*)pos)=n;
        pos+=sizeof(n);
        useLen+=sizeof(n);
        return *this;
    }
    inline InNetStream& operator<<(const char& n)
    {
        resizeIfNeed(sizeof(n));
        *((char*)pos)=n;
        pos+=sizeof(n);
        useLen+=sizeof(n);
         return *this;
    }
    inline InNetStream& operator<<(const unsigned char& n)
    {
        resizeIfNeed(sizeof(n));
        *((unsigned char*)pos)=n;
        pos+=sizeof(n);
        useLen+=sizeof(n);
        return *this;
    }
    inline InNetStream& operator<<(const short& n)
    {
        resizeIfNeed(sizeof(n));
        short tmp; //n
        if (bSwapByteOrder)
            tmp=NS_ByteOrderSwap16(n);
        else
            tmp=n;
        *((short*)pos)=tmp;
        pos+=sizeof(tmp);
        useLen+=sizeof(tmp);
        return *this;
    }

    inline InNetStream& operator<<(const unsigned short& n)
    {
        resizeIfNeed(sizeof(n));
        unsigned short tmp; //n
        if (bSwapByteOrder)
            tmp=NS_ByteOrderSwap16(n);
        else
            tmp=n;
        *((unsigned short*)pos)=tmp;
        pos+=sizeof(tmp);
        useLen+=sizeof(tmp);
        return *this;
    }
    inline InNetStream& operator<<(const int& n)
    {
        resizeIfNeed(sizeof(n));
        int tmp;
        if (bSwapByteOrder)
            tmp=NS_ByteOrderSwap32(n);
        else
            tmp=n;
        *((int*)pos)=tmp;
        pos+=sizeof(tmp);
        useLen+=sizeof(tmp);
        return *this;
    }
    inline InNetStream& operator<<(const unsigned int& n)
    {
        resizeIfNeed(sizeof(n));
        unsigned int tmp;
        if (bSwapByteOrder)
            tmp=NS_ByteOrderSwap32(n);
        else
            tmp=n;
        *((unsigned int*)pos)=tmp;
        pos+=sizeof(tmp);
        useLen+=sizeof(tmp);
        return *this;
    }
    inline InNetStream& operator<<(const int64_t& n)
    {
        resizeIfNeed(sizeof(n));
        int64_t tmp;
        if (bSwapByteOrder)
            tmp=NS_ByteOrderSwap64(n);
        else
            tmp=n;
        *((int64_t*)pos)=tmp;
        pos+=sizeof(tmp);
        useLen+=sizeof(tmp);
        return *this;
    }
    inline InNetStream& operator<<(const uint64_t& n)
    {
        resizeIfNeed(sizeof(n));
        uint64_t tmp;
        if (bSwapByteOrder)
            tmp=NS_ByteOrderSwap64(n);
        else
            tmp=n;
        *((uint64_t*)pos)=tmp;
        pos+=sizeof(tmp);
        useLen+=sizeof(tmp);
        return *this;
    }
    inline InNetStream& operator<<(const float& n)
    {
        resizeIfNeed(sizeof(n));
        float tmp;
        if (bSwapByteOrder)
            tmp=NS_ByteOrderSwapfloat(n);
        else
            tmp=n;
        *((float*)pos)=tmp;
        pos+=sizeof(tmp);
        useLen+=sizeof(tmp);
        return *this;
    }
    inline InNetStream& operator<<(const double& n)
    {
        resizeIfNeed(sizeof(n));
        double tmp;
        if (bSwapByteOrder)
            tmp=NS_ByteOrderSwapdouble(n);
        else
            tmp=n;
        *((double*)pos)=tmp;
        pos+=sizeof(tmp);
        useLen+=sizeof(tmp);
        return *this;
    }

    //特殊类型单独实现，自定义结构体需要重载(如果需要考虑大小端的化，否则没有必要)
    //把string存放到内存流中，先存放长度再存内容。
    InNetStream& operator<<(const std::string& s)
    {   
        NetStreamSizeType sLen=s.length();
        resizeIfNeed(sLen+sizeof(sLen));
        Put(sLen);
        memcpy(pos,s.data(),sLen);
        pos+=sLen;
        useLen+=sLen;
        return *this;
    }

    //wstring在windows与linux内置wchar_t 长度不一致，为了跨平台一致性,转成string后再保存
    InNetStream& operator<<(const std::wstring& ws)
    {
        std::string s=NS_WStringToString(ws);
        Put(s);
        return *this;
    }

    //std::vector<std::string> 比较常用在此实现
    inline InNetStream& operator<<(const std::vector<std::string>& Vs)
    {
        NetStreamSizeType Vsize=Vs.size();
        Put(Vsize);
        for (unsigned int n=0;n<Vsize;n++)
        {
            Put(Vs.at(n));
        }
         return *this;
    }
};

class OutNetStream
{
private:
    OutNetStream(const OutNetStream&);
    OutNetStream& operator=(const OutNetStream&);
private:
    char* posBegin;
    size_t maxLen;
    char* pos;
    size_t useLen;
private:
    bool bSwapByteOrder;   //是否需要反转字节序
public:
    OutNetStream(char* buf,size_t maxLen)
    {
        bSwapByteOrder=(CurrByteOrder!=dstByteOrder);
        posBegin=buf;
        pos=posBegin;
        this->maxLen=maxLen;
        useLen=0;
    }
    ~OutNetStream(void)
    {
    }
    bool CheckIfNotOutofRange(size_t sizetype)
    {
        if (useLen+sizetype<=maxLen)
        {
            return true;
        }
        return false;
    }
    //template<typename T>  
    //OutNetStream& operator>>(T type)
    //{
    //    if (CheckIfNotOutofRange(sizeof(type)))
    //    {
    //        type=*((T*)pos);
    //        pos+=sizeof(type);
    //        useLen+=sizeof(type);
    //    }
    //    return *this;
    //}


    //用Get或者重载操作符都可以
    template<typename T>
    inline OutNetStream& Get(T& type)
    {
        return operator>>(type);
    }

    //template<typename T,unsigned int N>
    //
    //OutNetStream& Get(T (&p)[N])   //不行，针对数组的特化，可能写法不对
    //{
    //    return operator>>(((p)[N]));
    //}

    template<typename T,size_t  N>
    OutNetStream& operator>>(T (&p)[N])
    {
        for (size_t n=0;n<N;n++)
        {
            this->operator>>(p[n]);
        }
        return *this;
    }
    template<typename T,size_t  N>
    OutNetStream& Get(T (&p)[N])
    {
        for (unsigned int n=0;n<N;n++)
        {
            this->operator>>(p[n]);
        }
        return *this;
    }

    //不需要考虑大小端的单字节类型数组，为了效率偏特化
    template<size_t  N>
    OutNetStream& operator>>(bool (&p)[N])
    {
        memcpy(p,pos,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    template<size_t  N>
    OutNetStream& Get(bool (&p)[N])
    {
        memcpy(p,pos,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    template<size_t  N>
    OutNetStream& operator>>(char (&p)[N])
    {
        memcpy(p,pos,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    template<size_t  N>
    OutNetStream& Get(char (&p)[N])
    {
        memcpy(p,pos,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
    template<size_t  N>
    OutNetStream& operator>>(unsigned char (&p)[N])
    {
        memcpy(p,pos,N);
        pos+=N;
        useLen+=N;
        return *this;
    }

    template<size_t  N>
    OutNetStream& Get(unsigned char (&p)[N])
    {
        memcpy(p,pos,N);
        pos+=N;
        useLen+=N;
        return *this;
    }
   //template<typename T>
   //friend OutNetStream& operator>>(OutNetStream &sout,T type);

   //基础数据类型--单字节的不需要考虑大小端
   inline OutNetStream& operator>>(bool& n)
   {
       n=*((bool*)pos);
       pos+=sizeof(n);
       useLen+=sizeof(n);
      return *this;
   }
   inline OutNetStream& operator>>(char& n)
   {
       n=*((char*)pos);
       pos+=sizeof(n);
       useLen+=sizeof(n);
       return *this;
   }
   inline OutNetStream& operator>>(unsigned char& n)
   {
       n=*((unsigned char*)pos);
       pos+=sizeof(n);
       useLen+=sizeof(n);
       return *this;
   }
   inline OutNetStream& operator>>(short& n)
   {
       n=*((short*)pos);
       if (bSwapByteOrder)
       {
           n=NS_ByteOrderSwap16(n);
       }
       pos+=sizeof(n);
       useLen+=sizeof(n);
       return *this;
   }
   inline OutNetStream& operator>>(unsigned short& n)
   {
       n=*((unsigned short*)pos);
       if (bSwapByteOrder)
       {
           n=NS_ByteOrderSwap16(n);
       }
       pos+=sizeof(n);
       useLen+=sizeof(n);
        return *this;
   }
   inline OutNetStream& operator>>(int& n)
   {
       n=*((int*)pos);
       if (bSwapByteOrder)
       {
           n=NS_ByteOrderSwap32(n);
       }
       pos+=sizeof(n);
       useLen+=sizeof(n);
       return *this;
   }
   inline OutNetStream& operator>>(unsigned int& n)
   {
       n=*((unsigned int*)pos);
       if (bSwapByteOrder)
       {
           n=NS_ByteOrderSwap32(n);
       }
       pos+=sizeof(n);
       useLen+=sizeof(n);
       return *this;
   }
   inline OutNetStream& operator>>(int64_t& n)
   {
       n=*((int64_t*)pos);
       if (bSwapByteOrder)
       {
           n=NS_ByteOrderSwap64(n);
       }
       pos+=sizeof(n);
       useLen+=sizeof(n);
       return *this;
   }
   inline OutNetStream& operator>>(uint64_t& n)
   {
       n=*((uint64_t*)pos);
       if (bSwapByteOrder)
       {
           n=NS_ByteOrderSwap64(n);
       }
       pos+=sizeof(n);
       useLen+=sizeof(n);
       return *this;
   }
   inline OutNetStream& operator>>(float& n)
   {
       n=*((float*)pos);
       if (bSwapByteOrder)
       {
           n=NS_ByteOrderSwapfloat(n);
       }
       pos+=sizeof(n);
       useLen+=sizeof(n);
       return *this;
   }
   inline OutNetStream& operator>>(double& n)
   {
       n=*((double*)pos);
       if (bSwapByteOrder)
       {
           n=NS_ByteOrderSwapdouble(n);
       }
       pos+=sizeof(n);
       useLen+=sizeof(n);
       return *this;
   }

   //特殊类型单独实现，自定义结构体需要重载(如果需要考虑大小端的化，否则没有必要)
   OutNetStream& operator>>(std::string& s)//从内存流中解析string,先取长度再构造字符串
   {
       unsigned int sLen=0;
       //Get(sLen);
       this->operator>>(sLen);

       s=std::string(pos,sLen);
       pos+=sLen;
       useLen+=sLen;
       return *this;
   }
   //wstring在windows与linux内置wchar_t 长度不一致，为了跨平台一致性,转成string后再保存
   OutNetStream& operator>>(std::wstring& ws)
   {
       NetStreamSizeType sLen=0;
       Get(sLen);
       ws=NS_StringToWString(std::string(pos,sLen));
       pos+=sLen;
       useLen+=sLen;
       return *this;
   }
   OutNetStream& operator>>(std::vector<std::string>& Vs)
   {
       char* pBegin=pos;
       NetStreamSizeType Vsize=0;
       Vs.clear();
       Get(Vsize);
       for (size_t n=0;n<Vsize;n++)
       {
           std::string stmp;
           Get(stmp);
           Vs.push_back(stmp);
       }
       return *this;
   }
};

class NetStream:public InNetStream,OutNetStream
{
public:
    NetStream(char* buf,unsigned int maxLen):InNetStream(buf,maxLen),OutNetStream(buf,maxLen)
    {
    }
    ~NetStream(void)
    {
    }
};


class FileStream:public NetStream
{

};


//使用方法: for example
struct TestNetStruct 
{
    bool b;
    char c;
    int n;
    int64_t l;
    float f;
    double df;
    bool arrayb[100];
    char arrayc[256];
    int arrayn[256];
    float arrayf[256];
    double arraydf[256];
    std::string s;
    std::wstring ws;
    std::string arrays[10];
    std::wstring arrayws[25];
    void print()
    {
        printf("b:%d c:%c n:%d f:%f df:%f s:%s\n",b,c,n,f,df,s.c_str());
    }
};
//自定义结构体需要重载以下方法
InNetStream& operator<<(InNetStream& in,const TestNetStruct& t);

OutNetStream& operator>>(OutNetStream& out,TestNetStruct& t);


#ifdef UNIT_TEST
BASIC_API int Test_NetStream();
#endif


NAME_SPACE_MAIN_END
#endif








