#ifndef ProtocalNetHead_H
#define ProtocalNetHead_H

//报文头,可能被网络框架和不同项目解决方案使用,提取出来

enum EType_Const
{   
    KL_HEAD=0x7585,             //数据报文头
    ZERO=0x0000,
}; 


#pragma pack(push,1)     //设置一个字节对齐  
struct KL
{
    unsigned short head;        //报文头恒为0x7585
    unsigned short key;          //报文类型  
    unsigned int len;              //报文长度不包括此KL头,为了传送大报文,用int32
    KL()
    {
        Init();
    }
    void Init()
    {
       head=KL_HEAD;
       len=0;
    }
    void toNetStream()
    {

    }
    void fromNetStream()
    {

    }
};

#pragma pack(pop)       //恢复原来对齐状态



//不同项目的网络数据报报文头可能不相同,为了网络架构的伸缩适配性,抽象处理, by lizulin 20191019
//不同项目注册不同报文头解析器

//
//根据一般的需求,构建如下


class DataHeadBase
{
public:
    DataHeadBase()
    {
    }
    ~DataHeadBase()
    {
    }
     //网络框架(IOCP/EPOLL等)应该与具体业务业务报文无关，进一步抽象分离,在具体业务报文处理器里面程序实现以下虚函数
    //不同项目不同报文头协议分别实现，KL头,KH头
    virtual int getHead(char* buf)                 //取报文头标识符常量
    {
        return 0;
    }
    virtual unsigned int getLen(char* buf)   //取报文长度-不包含报文头
    {
        return 0;
    }
    virtual int getSizeofHead()       //报文头长度
    {
        return 0;
    }
    virtual int getConstHeadKey()  //报文头常量值
    {
        return 0;
    }
    virtual int getMsgType()  //获取报文类型
    {
        return 0;
    }
};





//常用报文测试命令类型,暂时放此处
enum ETest
{
    CMD_TEXT_MESSAGE=0xFF00,
    CMD_IMAGE_FILE,
    CMD_BIG_FILE,
    CMD_RAND_DATA,      //随机数测试报文
};

#endif

