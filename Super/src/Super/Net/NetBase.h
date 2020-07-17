#ifndef myNetBase_Header
#define myNetBase_Header

//一些网络相关的基础操作，辅组类

#include "../Basic/ThreadSync.h"   //for mutex or std::mutex

#include <atomic>

//注意理解此设计目的、使用方法
//服务器共享数据访问接口,一些情况下需要服务器记录一些统计数据,以便在不同客户线程中访问
//此数据因为生命周期原因不能放在客户线程类中,需要放在服务器中,提供接口给各个客户线程访问
//其生命周期等于服务器生命周期

//使用方法：
//1).定义共享结构体，及其创建/释放函数
//struct ShareDataBetweenDCAP
//{
//    int clientNum;
//    std::string clientVer;
//};
//inline void* CreateShareDataBetweenDCAP(){return (void*)new ShareDataBetweenDCAP;}
//inline void* DestoryShareDataBetweenDCAP(void*p){ delete (ShareDataBetweenDCAP*)p;}

////找到调用客户线程或服务器调用其
//2).
//getTcpServet->setShareDataCreateDestoryFun(CreateShareDataBetweenDCAP,DestoryShareDataBetweenDCAP);

//3).访问修改共享数据
//Mutex& mtxShareData=getShareDataMutex();
//LockerGuard<Mutex> lock(mtxShareData);   //互斥锁
////
//ShareDataBetweenDCAP* pData=(ShareDataBetweenDCAP*)getShareData();



//共享数据结构体的创建/释放函数指针 在服务器与不同客户线程之间
typedef void* (*ShareDataCreateFun)(void);
typedef void (*ShareDataDestoryFun)(void* p);

//网络服务器与客户线程之间共享数据的访问管理器 by lizulin
class SharaDataMgr
{
public:
    SharaDataMgr():pShareData(NULL)
        ,pShareDataCreateFun(NULL)
        ,pShareDataDestoryFun(NULL)
    {

    }
    ~SharaDataMgr()
    {
        destoryShareData();
    }

    //服务器共享数据访问接口,一些情况下需要服务器记录一些统计数据,以便在不同客户线程中访问
    //此数据因为生命周期原因不能放在客户线程类中
public:
    volatile void* getShareData()const {return pShareData;}       //共享数据指针
    //void* getShareData()const {return pShareData.load();}           //共享数据指针
    //Mutex& getShareDataMutex(){return m_mtxShareData;}     //共享数据访问互斥锁
    std::recursive_mutex& getShareDataMutex(){return m_mtxShareData;}     //共享数据访问互斥锁

    //共享数据创建/销毁函数指针,若销毁指针为NULL,则服务器退出也不会销毁共享数据
    //若创建指针不为NULL，则会立即创建,  若bCover=true表明要覆盖上次的值,一般不需要
    void setShareDataCreateDestoryFun(ShareDataCreateFun creatFun,ShareDataDestoryFun destoryFun=NULL,bool bCover=false);
protected:
    volatile void* pShareData;    //std::atomic<>?
    //std::atomic<void*> pShareData;
    //Mutex m_mtxShareData;   //为了安全起见用递归锁,可以用std::recursive_mutex替代
    std::recursive_mutex m_mtxShareData;
    ShareDataCreateFun pShareDataCreateFun; 
    ShareDataDestoryFun pShareDataDestoryFun; 
    void createShareData();
    void destoryShareData();
};



#endif






