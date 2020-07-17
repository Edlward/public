#ifndef MessageRegistrar_H
#define MessageRegistrar_H

//报文传递辅组类,网络事件注册器及其辅助使用类


#include <stdio.h>
#include <memory.h>

 
#include "Super/Basic/global.h"
#include "Super/Basic/Tools.h" //for traceclass

//报文信息传递结构,替代QByteArray避免频繁内存申请释放,长时间运行可能存在内存碎片的问题
//思路:网络消息小于此大小用本结构内部内存传递,大于则申请堆内存传递,(改使用自定义内存池)使用结束后手动释放
class StuMessage
{
public:
    enum 
    {
        //不可太大否则可能栈溢出,信号槽触发太多时? 1-10k
        MsgBufferSize=1*1024      
    };
    char bufSelf[MsgBufferSize];
    char* pData;
    unsigned int dataLen;            //改进 根据长度调用拷贝构造函数，避免无用拷贝
    bool useNewBuf;
    //int type;
    unsigned int copyCnt;         //拷贝次数计数,调试用
    typedef StuMessage self;
    StuMessage()
    {
        //type=0;
        init();
    }
    StuMessage(const char* buf,unsigned int len)
    {
        init();
        setData(buf,len);
    }
    void init()
    {
        //printf("StuMessage::init()\n");
        dataLen=0;
        pData=bufSelf;
        useNewBuf=false;
        copyCnt=0;
    }
    //根据长度调用拷贝构造函数，避免无用拷贝，只拷贝有效数据
    self& operator=(const self& R)   //赋值构造
    {
        //printf("赋值构造\n");
        copyFrom(R);
        return *this;
    }
    StuMessage(const self& R)       //拷贝构造
    {
        //printf("拷贝构造\n");
        copyFrom(R);
    }
    void copyFrom(const self& R);
    void setData(const char* pBuf,unsigned int len);
    //不再继续使用时手动调用此函数释放数据
    void DestoryNoLongerUse();
    
    //TraceClass<StuMessage> traceObj;
};

//使用注意:








////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//完善备忘:考虑一种特殊情况,对于弹出对话框窗体各种消息的注册与反注册，
//若此时刚好网络数据到来并调用相应函数,可能会发生同时操作同一数据结构尤其是内部用map结构管理的情况
//是否所有操作加内部锁，但是又会严重影响效率。


//此类作用是根据报文关键字key，来自动调用回调函数。便于不同模块解耦合
#include <map>
#include <utility>    //包含pair,用std::pair   vector iostream 等头文件都包含了utility头文件
#include <vector>
#include <list>
#include <iostream>


////定义指向模板函数的函数指针类型
//template <typename T>
//struct FunObjWrapper
//{
//    //typedef int (* CallbackFunPtrType) (const PktType& CurPkt);
//    typedef void (*T::FilterProcess)(char* buf,int len);
//    //void WinEqualization::FilterProcess(char* buf, int len)
//    typedef void (T::*pProc)(char*,int);
//};



//考虑是否有必要改std::function实现，有空待续 20191021 
//类成员函数结构体
//要访问任意类成员函数，只需要知道对象地址和函数地址即可
struct CFunObj
{
    CFunObj(void* pObject=NULL,void* pFun=NULL)
    {
       setValue(pObject,pFun);
    }
    void setValue(void* pObject,void* pFun)
    {
        this->pObject=pObject;
        this->pFun=pFun;
    }
    //template<typename T>
    //void setValue(void* pObject,FunObjWrapper<T>.FilterProcess);

    bool operator==(const CFunObj& R) const
    {
        return (this->pObject==R.pObject)&&(this->pFun==R.pFun);
    }
    bool operator!=(const CFunObj& R) const
    {
        return !operator==(R);
    }
//private:
    void* pObject;           //对象地址
    void* pFun;               //回调函数地址
};

//template<typename T>
//void CFunObj::setValue(void* pObject,FunObjWrapper<T>::FilterProcess pFuncall)
//{
//    void* ptmp=*((void**)&pFuncall);
//    this->setValue(pObject,ptmp)
//}



//空类 --无用，仅仅是为了引入类函数指针
class  CNoUsetoCAST
{
public:
	void Process(char* buf,int len){};                
	void fun(){};                
};
typedef void (CNoUsetoCAST::*pClassFun)(char*,int);
typedef void (CNoUsetoCAST::*pFunVoid)();


//一般消息报文回调函数注册管理
//注意:一般情况始终存在的widget不需要UnRegister反注册，Dialog可能需要，因为对话框关闭后对象不存在，可能会非法访问

class MessageRegistrar
{
public:
    MessageRegistrar();
    ~MessageRegistrar();
private:
    MessageRegistrar(const MessageRegistrar& r){};                       //声明私有，禁止外部 拷贝构造
    MessageRegistrar& operator=(const MessageRegistrar& r){return *this;}  //声明私有，禁止外部 赋值构造
public:
    void UnRegisterAll();  //取消所有回调函数的注册，包括消息、网络通/断

	void RegisterEvenvCallofMsgReceived(int msgType,const CFunObj& funObj);
	void UnRegisterEvenvCallofMsgReceived(int msgType,const CFunObj& funObj);//删除某个key的某个value
	void UnRegisterEvenvCallofMsgReceived(int msgType);                   //删除某个key所有value

	void doEvenvCallofMsgReceived(int msgType,char* buf,int len);     //自动根据注册的报文类型与对应处理函数地址，回调相应函数

private:
    void doCall(const CFunObj& obj,char* buf,int len);
    void printKeyFun(int msgType,const CFunObj& funObj);
    void printAll();
private:
    //map改multimap支持同一命令对应多个处理函数
	typedef std::pair<int,CFunObj>  pairMsgFun;
	typedef std::multimap<int,CFunObj> :: iterator MapMsgFun_it; 
	std::multimap<int,CFunObj> MapMsgFun;

private:
    //报文回调内部进一步优化，小于65536的报文用查找表实现，不用map,一般业务逻辑key的范围不会太大
    //可以保证小报文回调是O1操作
    void RegisterEvenvCallofMsgReceivedByTable(unsigned short msgType,const CFunObj& funObj);
    void UnRegisterEvenvCallofMsgReceivedByTable(unsigned short msgType,const CFunObj& funObj);
    void UnRegisterEvenvCallofMsgReceivedByTable(unsigned short msgType); 
    void doEvenvCallofMsgReceivedByTable(unsigned short msgType,char* buf,int len);
    void printByTable();
    std::vector<CFunObj>* pSerchTable[65536];
public:
    //网络连接成功与网络断开被调用的函数注册与反注册
    void RegisterEventCallofNetConnected(const CFunObj& funObj);
    void UnRegisterEventCallofNetConnected(const CFunObj& funObj);
    void doEventCallofNetConnected();
    void RegisterEventCallofNetDisConnected(const CFunObj& funObj);
    void UnRegisterEventCallofNetDisConnected(const CFunObj& funObj);
    void doEventCallofNetDisConnected();
private:
     //std::list  一般数量比较少 用std::vector
    std::vector<CFunObj> ListConnectedFun;        
    std::vector<CFunObj> ListDisConnectedFun;
};


//报文注册辅助类，作为使用类内部常用，会自动注销报文，避免作用域已经结束，报文回调仍然被调用导致崩溃的情况
class MessageRegisterHelper
{
    struct MsgFunObjPkg
    {
        MsgFunObjPkg(MessageRegistrar* pMsm,int MsgType,const CFunObj& obj)
            :pMsm(pMsm),MsgType(MsgType),obj(obj)
        {}
        bool operator==(const MsgFunObjPkg& r) const
        {
            return (this->pMsm==r.pMsm)&&(this->MsgType==r.MsgType)&&(this->obj==r.obj);
        }
        MessageRegistrar* pMsm;
        int MsgType;   //
        CFunObj obj;
    };
    struct FunObjPkg
    {
        FunObjPkg(MessageRegistrar* pMsm,const CFunObj& obj)
            :pMsm(pMsm),obj(obj)
        {}
        bool operator==(const FunObjPkg& r) const
        {
            return (this->pMsm==r.pMsm)&&(this->obj==r.obj);
        }
        MessageRegistrar* pMsm;
        CFunObj obj;
    };
public:
    MessageRegisterHelper()
    {
    }
    ~MessageRegisterHelper()
    {
        UnRegisterAll();   //自动注销所有报文
    }
    void RegisterEvenvCallofMsgReceived(MessageRegistrar& msgR,int MsgType,const CFunObj& obj)
    {
        MsgFunObjPkg tmp(&msgR,MsgType,obj);
        for (size_t n=0;n<ListRegisterEvenvCallofMsgReceived.size();n++)
        {
            const MsgFunObjPkg& Curr=ListRegisterEvenvCallofMsgReceived[n];
            if (tmp==Curr)
            {
                return;      //重复事件排除机制
            }
        }
        msgR.RegisterEvenvCallofMsgReceived(MsgType,obj);
        ListRegisterEvenvCallofMsgReceived.push_back(tmp);
    }
    void RegisterEventCallofNetConnected(MessageRegistrar& msgR,const CFunObj& obj)
    {
       FunObjPkg tmp(&msgR,obj);
       for (size_t n=0;n<ListRegisterEventCallofNetConnected.size();n++)
       {
           const FunObjPkg& Curr=ListRegisterEventCallofNetConnected[n];
           if (tmp==Curr)
           {
               return;      //重复事件排除机制
           }
       }
       msgR.RegisterEventCallofNetConnected(obj);
       ListRegisterEventCallofNetConnected.push_back(tmp);
    }
    void RegisterEventCallofNetDisConnected(MessageRegistrar& msgR,const CFunObj& obj)
    {
       FunObjPkg tmp(&msgR,obj);
       for (size_t n=0;n<ListRegisterEventCallofNetDisConnected.size();n++)
       {
           const FunObjPkg& Curr=ListRegisterEventCallofNetDisConnected[n];
           if (tmp==Curr)
           {
               return;      //重复事件排除机制
           }
       }
       msgR.RegisterEventCallofNetDisConnected(obj);
       ListRegisterEventCallofNetDisConnected.push_back(tmp);
    }
private:
    void UnRegisterAll()
    {
        for (size_t n=0;n<ListRegisterEvenvCallofMsgReceived.size();n++)
        {
             MsgFunObjPkg& tmp=ListRegisterEvenvCallofMsgReceived[n];
             tmp.pMsm->UnRegisterEvenvCallofMsgReceived(tmp.MsgType,tmp.obj);
        }
        ListRegisterEvenvCallofMsgReceived.clear();
        for (size_t n=0;n<ListRegisterEventCallofNetConnected.size();n++)
        {
            FunObjPkg& tmp=ListRegisterEventCallofNetConnected[n];
            tmp.pMsm->UnRegisterEventCallofNetConnected(tmp.obj);
        }
        ListRegisterEventCallofNetConnected.clear();
        for (size_t n=0;n<ListRegisterEventCallofNetDisConnected.size();n++)
        {
            FunObjPkg& tmp=ListRegisterEventCallofNetDisConnected[n];
            tmp.pMsm->UnRegisterEventCallofNetDisConnected(tmp.obj);
        }
        ListRegisterEventCallofNetDisConnected.clear();
    }
private:
    std::vector<MsgFunObjPkg>  ListRegisterEvenvCallofMsgReceived;
    std::vector<FunObjPkg>  ListRegisterEventCallofNetConnected;
    std::vector<FunObjPkg>  ListRegisterEventCallofNetDisConnected;
};



//使用方法说明--待更新
#if 0
class MainWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots: //也可以是普通函数
	void slots_DataProcess(char* buf,int len){printf("DataProcess()")}; 
}

typedef void (MainWindow::*pProc)(char*,int);                //简单命名发送给UI处理,方便改变UI状态
MainWindow::MainWindow()
{
    MessageRegistrar& Msm=GetNetModule(Srv_Core)->getMessageRegistar();
    CFunObj funObj;
    funObj.pObject=(void*)this;
    typedef void (MainWindow::*pProcVoid)();   //定义一个函数指针
    pProcVoid pFunVoid=&MainWindow::CallAfterNetConnected;
    funObj.pFun=*((void**)&pFunVoid);      //方式1:强转  
    MsgRegisterHelper.RegisterEventCallofNetConnected(Msm,funObj);   //注册网络连接成功会被调用的函数

    funObj.pObject=(void*)this;
    pFunVoid=&MainWindow::CallAfterNetDisConnected;
    funObj.pFun=*((void**)&pFunVoid);      //方式1:强转  
    MsgRegisterHelper.RegisterEventCallofNetDisConnected(Msm,funObj);    //注册网络连接断开会被调用的函数

    //CFunObj funObj;
    funObj.pObject=(void*)this;
    typedef void (MainWindow::*pProc)(char*,int);             //定义一个函数指针
    pProc pFun=&MainWindow::FilterProcess;
    funObj.pFun=*((void**)&pFun);                          //方式1:强转  

    //注册 某个报文对应回调函数
    //MessageRegistrar& Msm=GetNetModule(Srv_Core)->getMessageRegistar();
    MsgRegisterHelper.RegisterEvenvCallofMsgReceived(Msm,CMD_RAND_DATA_TEST_RESP,funObj);
    MsgRegisterHelper.RegisterEvenvCallofMsgReceived(Msm,CMD_HEARTBEAT_UPLOAD_RESP,funObj);
	 Msm.printMap();	

	char buf[1024];
	Msm.doAutoCallback(1,buf,sizeof(buf));   //回调模拟
	//会自动调用slots_DataProcess()函数          //测试成功
}
#endif

#endif // MESSAGEREGISTRAR_H



