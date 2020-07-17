#include "NetBase.h"


void SharaDataMgr::setShareDataCreateDestoryFun(ShareDataCreateFun creatFun,ShareDataDestoryFun destoryFun/*=NULL*/,bool bCover/*=false*/)
{
    PRINT_FUN_TRACE
    if (!bCover)
    {
        this->pShareDataCreateFun=creatFun;
        this->pShareDataDestoryFun=destoryFun;
        createShareData();
    }
    else
    {
        //先释放旧数据再创建新的
        destoryShareData();
        this->pShareDataCreateFun=creatFun;
        this->pShareDataDestoryFun=destoryFun;
        createShareData();
    }
}

void SharaDataMgr::createShareData()
{
    PRINT_FUN_TRACE
    //LockerGuard<Mutex> lock(m_mtxShareData);   //互斥锁
    std::lock_guard<std::recursive_mutex> lock(m_mtxShareData);   //互斥锁
    if (pShareData==NULL&&this->pShareDataCreateFun!=NULL)
    {
        pShareData=this->pShareDataCreateFun();
    }
}

void SharaDataMgr::destoryShareData()
{
    PRINT_FUN_TRACE

    //LockerGuard<Mutex> lock(m_mtxShareData);   //互斥锁
    std::lock_guard<std::recursive_mutex> lock(m_mtxShareData);   //互斥锁

    //如果共享数据指针与销毁函数指针都不为NULL，则销毁之
    if (pShareData!=NULL&&pShareDataDestoryFun!=NULL)
    {
        pShareDataDestoryFun((void*)pShareData);
        pShareData=NULL;  //已经释放标记为NULL
    }
}
