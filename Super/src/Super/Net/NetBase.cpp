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
        //���ͷž������ٴ����µ�
        destoryShareData();
        this->pShareDataCreateFun=creatFun;
        this->pShareDataDestoryFun=destoryFun;
        createShareData();
    }
}

void SharaDataMgr::createShareData()
{
    PRINT_FUN_TRACE
    //LockerGuard<Mutex> lock(m_mtxShareData);   //������
    std::lock_guard<std::recursive_mutex> lock(m_mtxShareData);   //������
    if (pShareData==NULL&&this->pShareDataCreateFun!=NULL)
    {
        pShareData=this->pShareDataCreateFun();
    }
}

void SharaDataMgr::destoryShareData()
{
    PRINT_FUN_TRACE

    //LockerGuard<Mutex> lock(m_mtxShareData);   //������
    std::lock_guard<std::recursive_mutex> lock(m_mtxShareData);   //������

    //�����������ָ�������ٺ���ָ�붼��ΪNULL��������֮
    if (pShareData!=NULL&&pShareDataDestoryFun!=NULL)
    {
        pShareDataDestoryFun((void*)pShareData);
        pShareData=NULL;  //�Ѿ��ͷű��ΪNULL
    }
}
