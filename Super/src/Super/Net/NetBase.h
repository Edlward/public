#ifndef myNetBase_Header
#define myNetBase_Header

//һЩ������صĻ���������������

#include "../Basic/ThreadSync.h"   //for mutex or std::mutex

#include <atomic>

//ע���������Ŀ�ġ�ʹ�÷���
//�������������ݷ��ʽӿ�,һЩ�������Ҫ��������¼һЩͳ������,�Ա��ڲ�ͬ�ͻ��߳��з���
//��������Ϊ��������ԭ���ܷ��ڿͻ��߳�����,��Ҫ���ڷ�������,�ṩ�ӿڸ������ͻ��̷߳���
//���������ڵ��ڷ�������������

//ʹ�÷�����
//1).���干��ṹ�壬���䴴��/�ͷź���
//struct ShareDataBetweenDCAP
//{
//    int clientNum;
//    std::string clientVer;
//};
//inline void* CreateShareDataBetweenDCAP(){return (void*)new ShareDataBetweenDCAP;}
//inline void* DestoryShareDataBetweenDCAP(void*p){ delete (ShareDataBetweenDCAP*)p;}

////�ҵ����ÿͻ��̻߳������������
//2).
//getTcpServet->setShareDataCreateDestoryFun(CreateShareDataBetweenDCAP,DestoryShareDataBetweenDCAP);

//3).�����޸Ĺ�������
//Mutex& mtxShareData=getShareDataMutex();
//LockerGuard<Mutex> lock(mtxShareData);   //������
////
//ShareDataBetweenDCAP* pData=(ShareDataBetweenDCAP*)getShareData();



//�������ݽṹ��Ĵ���/�ͷź���ָ�� �ڷ������벻ͬ�ͻ��߳�֮��
typedef void* (*ShareDataCreateFun)(void);
typedef void (*ShareDataDestoryFun)(void* p);

//�����������ͻ��߳�֮�乲�����ݵķ��ʹ����� by lizulin
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

    //�������������ݷ��ʽӿ�,һЩ�������Ҫ��������¼һЩͳ������,�Ա��ڲ�ͬ�ͻ��߳��з���
    //��������Ϊ��������ԭ���ܷ��ڿͻ��߳�����
public:
    volatile void* getShareData()const {return pShareData;}       //��������ָ��
    //void* getShareData()const {return pShareData.load();}           //��������ָ��
    //Mutex& getShareDataMutex(){return m_mtxShareData;}     //�������ݷ��ʻ�����
    std::recursive_mutex& getShareDataMutex(){return m_mtxShareData;}     //�������ݷ��ʻ�����

    //�������ݴ���/���ٺ���ָ��,������ָ��ΪNULL,��������˳�Ҳ�������ٹ�������
    //������ָ�벻ΪNULL�������������,  ��bCover=true����Ҫ�����ϴε�ֵ,һ�㲻��Ҫ
    void setShareDataCreateDestoryFun(ShareDataCreateFun creatFun,ShareDataDestoryFun destoryFun=NULL,bool bCover=false);
protected:
    volatile void* pShareData;    //std::atomic<>?
    //std::atomic<void*> pShareData;
    //Mutex m_mtxShareData;   //Ϊ�˰�ȫ����õݹ���,������std::recursive_mutex���
    std::recursive_mutex m_mtxShareData;
    ShareDataCreateFun pShareDataCreateFun; 
    ShareDataDestoryFun pShareDataDestoryFun; 
    void createShareData();
    void destoryShareData();
};



#endif






