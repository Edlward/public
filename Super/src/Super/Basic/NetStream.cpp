#include "NetStream.h"


 NAME_SPACE_MAIN_BEGIN


//模板不支持在cpp实现

//void InNetStream::resizeBuffer()
//{
//
//}
//
//InNetStream& InNetStream::operator<<(const std::string& s)
//{
//
//}
//
//InNetStream& InNetStream::operator<<(const std::wstring& ws)
//{
//
//}
//
//OutNetStream& OutNetStream::operator<<(std::string& s) //从内存流中解析string,先取长度再构造字符串
//{
//
//}
//
//OutNetStream& OutNetStream::operator<<(const std::wstring& ws)
//{
//
//}
//
//OutNetStream& OutNetStream::operator<<(std::vector<std::string>& Vs)
//{
//
//}

InNetStream& operator<<(InNetStream& in,const TestNetStruct& t)
{
    return in.Put(t.b).Put(t.c).Put(t.n).Put(t.l).Put(t.f).Put(t.df).Put(t.arrayb).Put(t.arrayc).Put(t.arrayn).Put(t.arraydf).Put(t.s).Put(t.ws).Put(t.arrays).Put(t.arrayws);
    //or
    //return in<<(t.b)<<(t.c)<<(t.n)<<(t.l)<<(t.f)<<(t.df)<<(t.arrayb)<<(t.arrayc)<<(t.arrayn)<<(t.arraydf)<<(t.s)<<(t.ws)<<(t.arrays)<<(t.arrayws);
}

OutNetStream& operator>>(OutNetStream& out,TestNetStruct& t)
{
    return out.Get(t.b).Get(t.c).Get(t.n).Get(t.l).Get(t.f).Get(t.df).Get(t.arrayb).Get(t.arrayc).Get(t.arrayn).Get(t.arraydf).Get(t.s).Get(t.ws).Get(t.arrays).Get(t.arrayws);
    //or
    //return out>>(t.b)>>(t.c)>>(t.n)>>(t.l)>>(t.f)>>(t.df)>>(t.arrayb)>>(t.arrayc)>>(t.arrayn)>>(t.arraydf)>>(t.s)>>(t.ws)>>(t.arrays)>>(t.arrayws);
}

#include "SuperTime.h"


#ifdef UNIT_TEST

int Test_NetStream()
{

#if 0   //网络数据流封包解包测试
    //sender 发送方 数据转网络流
    char buf[10*1024];
    InNetStream INet(buf,sizeof(buf));
    int testn=99;
    float testf=33.3146;
    char arrayc[256];
    std::string tests="abcdef123456789";
    TestNetStruct tstu;
    tstu.f=99.456;
    tstu.df=12.978;
    tstu.s="test123456789";

    INet<<testn;
    INet<<testf;
    INet<<arrayc;
    INet<<tests;
    INet<<tstu;
    //SendData(INet.data(),INet.size());

    //receiver 接收方 还原数据
    {
        char buf[10*1024];
        memcpy(buf,INet.data(),INet.size());
        OutNetStream ONet(buf,sizeof(buf));
        int testn;
        float testf;
        char arrayc[256];
        std::string tests;
        TestNetStruct tstu;
        ONet>>testn;
        ONet>>testf;
        ONet>>arrayc;
        ONet>>tests;
        ONet>>tstu;
        printf("n:%d f:%f s:%s\n",testn,testf,tests.c_str());
        tstu.print();
    }
#endif



#if 0
    SuperTime tm("Test:");
    float f=12.9877755;
    float df=12.9877755;
    tm.getBeginTime();
    for (int n=0;n<100000;n++)
    {
        float sf=NS_ByteOrderSwapfloat(f);
        f=NS_ByteOrderSwapfloat(sf);

        //double sdf=bswap_float(df);
        //df=bswap_float(sdf);
    }
    tm.getEndPrint();
    printf("f:%f\n",f);
#endif

    return 0;
}
#endif



NAME_SPACE_MAIN_END
