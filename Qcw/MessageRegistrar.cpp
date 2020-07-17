#include "MessageRegistrar.h"        //报文派发管理器

#include "Super/Basic/MemoryPool.h"      //内存池

//static MemPool<> memPool(500*1024);
//static MemPool<>& getMemPool()
//{
//    static MemPool<> memPool(500*1024);
//    return memPool;
//}

static void* doMalloc(size_t nSize)
{
    //return ::malloc(nSize);
    //return memPool.Malloc(nSize);
    return GetMemPool().Malloc(nSize);
}
static void doFree(void* ptr)
{
    //return ::free(ptr);
    //return memPool.Free(ptr);
    return GetMemPool().Free(ptr);
}

void StuMessage::copyFrom(const self& R)
{
    this->dataLen=R.dataLen;
    this->useNewBuf=R.useNewBuf;
    this->copyCnt=R.copyCnt+1;
    if (R.useNewBuf)                    //如果没有使用外部堆内存才需要拷贝
    {
        this->pData=R.pData;
    }
    else
    {
        memcpy(this->bufSelf,R.bufSelf,R.dataLen);
        this->pData=this->bufSelf;
    }
}

void StuMessage::setData(const char* pBuf,unsigned int len)
{
    if (len>MsgBufferSize)
    {
        pData=(char*)doMalloc(len);
        useNewBuf=true;
    }
    else
    {
        pData=this->bufSelf;
        useNewBuf=false;
    }
    memcpy(pData,pBuf,len);
    dataLen=len;
}

void StuMessage::DestoryNoLongerUse()
{
    //printf("DestoryNoLonger() copyCnt:%u\n",copyCnt);
    if (useNewBuf)
    {
        doFree(pData);
    }
    init();
}








MessageRegistrar::MessageRegistrar()
{
    //MapMsgFun.clear();
    for (unsigned n=0;n<65536;n++)
    {
            pSerchTable[n]=NULL;
    }
}

MessageRegistrar::~MessageRegistrar()
{
    //MapMsgFun.clear();

    for (unsigned n=0;n<65536;n++)
    {
        if (pSerchTable[n]!=NULL)
        {
            delete pSerchTable[n];
            pSerchTable[n]=NULL;
        }
    }
}

void MessageRegistrar::RegisterEvenvCallofMsgReceived(int msgType,const CFunObj& oFunObj)
{
    //优化，key值小与65536报文用查找表实现。
    if ((unsigned int)msgType<65536)
    {
        return RegisterEvenvCallofMsgReceivedByTable(msgType,oFunObj);
    }

    //完全一样的key与值，避免重复插入 multimap允许重复，以下低效写法待改进
    bool isExist=false;
    std::multimap<int,CFunObj>& m=MapMsgFun;
    std::multimap<int,CFunObj>::iterator it;
    for(it=m.begin();it!=m.end();++it)
    {
        if (it->first ==msgType&&(it->second==oFunObj))
        {
            isExist=true;
            break;
        }
    }

    if (!isExist)
    {
         MapMsgFun.insert(pairMsgFun(msgType,oFunObj));
    }
}

void MessageRegistrar::printAll()
{
    printByTable();  //打印查找表实现的部分

    std::multimap<int,CFunObj>& m=MapMsgFun;
    //typename std::multimap<int,CFunObj>::iterator it;
    auto it=m.begin();
    for(it=m.begin();it!=m.end();it++)
    {
       // pairMsgFun& p1=(pairMsgFun)(*it);
       // printKeyFun(p1.first,p1.second);    //TODO
    }
}


void MessageRegistrar::printKeyFun(int msgType,const CFunObj& funObj)
{
    std::cout<<"key::"<<msgType<<" ";//<<std::endl;
    //std::cout<<std::fixed<<std::cout.precision(2)<<"second:"<<p1.second<<" "<<std::endl;
    void* pObj=funObj.pObject;
    void* pFun=funObj.pFun;
    std::cout<<" Value:  pObj: "<<pObj<<" pFun: "<<pFun<<std::endl;
    //std::cout<<std::fixed<<std::cout.precision(2)<<"second:"<<p1.second<<" "<<std::endl;
}


void MessageRegistrar::doEvenvCallofMsgReceived(int msgType,char* buf,int len)
{
    if ((unsigned int)msgType<65536)
    {
        return doEvenvCallofMsgReceivedByTable(msgType,buf,len);
    }

    MapMsgFun_it begin=MapMsgFun.lower_bound(msgType);
    MapMsgFun_it end=MapMsgFun.upper_bound(msgType); 
    while(begin!=end)
    { 
        //std::cout<<"Key="<<begin->first<<" Value="<<begin->second<< std::endl; 
        //void* pFun=begin->second;      //得到对应报文的处理函数地址，这是一个类的成员函数
        CFunObj& funObj=begin->second;
        doCall(funObj,buf,len);
        ++begin;
    } 
}



void MessageRegistrar::doCall(const CFunObj& funObj,char* buf,int len)
{
    void* pObj=funObj.pObject;
    void* pFun=funObj.pFun;
    //std::cout<<"Match  pObj: "<<pObj<<" pFun: "<<pFun<<std::endl;
    //方法1：失败
    //pClassFun pCallfun=(pClassFun)pFun;                      //强转成类的函数指针--VS编译不通过 mingw可以
    //方法2：暴力填充 成功
    //pClassFun pCallfun=NULL;
    //memcpy(&pCallfun,&pFun,sizeof(void*));
    //方法3:不用memcpy 成功

    //method1:
    //pClassFun pCallfun=*((pClassFun*)(&pFun));   //VS WINDOWS OK  gcc LINUX ERROR

    //method2
    pClassFun pCallfun;
   *((void** )&pCallfun)=pFun;    //gcc LINUX OK


    //CNoUsetoCAST oNoUse;
    //pClassFun pCallfun=&CNoUsetoCAST::Process;         //在VS下Process必须声明为public
    //((CNoUsetoCAST*)pObj->*pCallfun)(buf,len);            //写法1，正确
    //写法2；易于理解
    CNoUsetoCAST* pObject=(CNoUsetoCAST*)pObj;   //强转成类的对象指针
    (pObject->*pCallfun)(buf,len);                                  //类的对象首地址+函数指针方式调用
}


void MessageRegistrar::UnRegisterEvenvCallofMsgReceived(int msgType)
{
    if ((unsigned int)msgType<65536)
    {
        return UnRegisterEvenvCallofMsgReceivedByTable(msgType);
    }

    //以下低效写法待改进
    //MapMsgFun.erase(MsgType);
    std::multimap<int,CFunObj>& m=MapMsgFun;
    //typename std::multimap<int,CFunObj>::iterator it;
    auto it=m.begin();
    for(it=m.begin();it!=m.end();)
    {
        if (it->first ==msgType)
        {
            //std::cout <<"UnRegister: key: "<< it->first << "\t value:" << it->second.pObject <<std::endl;
            it= m.erase(it);
        }
        else
        {
            //std::cout <<"find key: "<< it->first << "\t value:" << it->second.pObject <<std::endl;
            it++;
        }
    }
}

void MessageRegistrar::UnRegisterEvenvCallofMsgReceived(int msgType,const CFunObj& funObj)
{
    if ((unsigned int)msgType<65536)
    {
        return UnRegisterEvenvCallofMsgReceivedByTable(msgType,funObj);
    }

    //以下低效写法待改进
    std::multimap<int,CFunObj>& m=MapMsgFun;
    //typename std::multimap<int,CFunObj>::iterator it;
    auto it=m.begin();
    for(it=m.begin();it!=m.end();)
    {
        if (it->first ==msgType&&(it->second==funObj))
        {
            //std::cout <<"UnRegister: key: "<< it->first <<"\t value:" <<it->second.pObject<<std::endl;
            it= m.erase(it);
        }
        else
        {
            //std::cout <<"find key: "<< it->first <<"\t value:" <<it->second.pObject<<std::endl;
            it++;
        }
    }
}

void MessageRegistrar::RegisterEvenvCallofMsgReceivedByTable(unsigned short msgType,const CFunObj& funObj)
{
    //为了节约资源需要时才创建
    if (pSerchTable[msgType]==NULL)
    {
        pSerchTable[msgType]=new std::vector<CFunObj>;
    }
    //相同的msgType可以有多个funObj(例如MainWindows和WinChannels的FilterProcess)
    pSerchTable[msgType]->push_back(funObj);
}

void MessageRegistrar::UnRegisterEvenvCallofMsgReceivedByTable(unsigned short msgType,const CFunObj& funObj)
{
    if (pSerchTable[msgType]==NULL)
    {
        return;
    }
    std::vector<CFunObj>::iterator it;
    for (it=pSerchTable[msgType]->begin();it!=pSerchTable[msgType]->end();)
    {
        if (funObj==*it)
        {
            it=pSerchTable[msgType]->erase(it);
        }
        else
        {
            it++;
        }
    }
}

void MessageRegistrar::UnRegisterEvenvCallofMsgReceivedByTable(unsigned short msgType)
{
    if (pSerchTable[msgType]==NULL)
    {
        return;
    }
    //delete pSerchTable[MsgType];
    //pSerchTable[MsgType]=NULL;
    pSerchTable[msgType]->clear();  //清除或删除都可以
}

void MessageRegistrar::doEvenvCallofMsgReceivedByTable(unsigned short msgType,char* buf,int len)
{
    if (pSerchTable[msgType]==NULL)
    {
        return;
    }
    for (size_t n=0;n<pSerchTable[msgType]->size();n++)
    {
        CFunObj& funObj=pSerchTable[msgType]->at(n);
        doCall(funObj,buf,len);
    }
}

void MessageRegistrar::printByTable()
{
    for (unsigned n=0;n<65536;n++)
    {
        if (pSerchTable[n]!=NULL)
        {
            for (size_t n=0;n<pSerchTable[n]->size();n++)
            {
                CFunObj& obj=pSerchTable[n]->at(n);
                printKeyFun(n,obj);
            }
        }
    }
}


void MessageRegistrar::RegisterEventCallofNetConnected(const CFunObj& funObj)
{
    std::vector<CFunObj>::iterator it;
    bool isExist=false;
    for(it=ListConnectedFun.begin();it!=ListConnectedFun.end();++it)  
    {  
        if (*it==funObj)
        {
            isExist=true;
            break;
        }
    }
    //避免添加重复的回调
    if (!isExist)
    {
         ListConnectedFun.push_back(funObj);
    }
}

void MessageRegistrar::UnRegisterEventCallofNetConnected(const CFunObj& funObj)
{
    std::vector<CFunObj>::iterator it;

    //std::vector删除比较低效,优化待续
    for(it=ListConnectedFun.begin();it!=ListConnectedFun.end();)  
    {  
        if (*it==funObj)
        {
            it=ListConnectedFun.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void MessageRegistrar::doEventCallofNetConnected()
{
    std::vector<CFunObj>::iterator it;  
    for(it=ListConnectedFun.begin();it!=ListConnectedFun.end();++it)  
    {  
        CFunObj& funObj=*it;
        void* pObj=funObj.pObject;
        void* pFun=funObj.pFun;

        //method1:
        //pFunVoid pCallfun=*((pFunVoid*)(&pFun));   //VS WINDOWS OK  gcc LINUX ERROR

        //method2
        pFunVoid pCallfun;
       *((void** )&pCallfun)=pFun;    //gcc LINUX OK

        //写法2；易于理解
        CNoUsetoCAST* pObject=(CNoUsetoCAST*)pObj;   //强转成类的对象指针
        (pObject->*pCallfun)();   //vs gcc ok

      //  ((*pObject).*pCallfun)();

    }
}

void MessageRegistrar::RegisterEventCallofNetDisConnected(const CFunObj& funObj)
{
    std::vector<CFunObj>::iterator it;
    bool isExist=false;
    for(it=ListDisConnectedFun.begin();it!=ListDisConnectedFun.end();++it)  
    {  
        if (*it==funObj)
        {
            isExist=true;
            break;
        }
    }
    //避免添加重复的回调
    if (!isExist)
    {
        ListDisConnectedFun.push_back(funObj);
    }
}

void MessageRegistrar::UnRegisterEventCallofNetDisConnected(const CFunObj& funObj)
{
    std::vector<CFunObj>::iterator it;  
    for(it=ListDisConnectedFun.begin();it!=ListDisConnectedFun.end();)  
    {  
        if (*it==funObj)
        {
            it=ListDisConnectedFun.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void MessageRegistrar::doEventCallofNetDisConnected()
{
    std::vector<CFunObj>::iterator it;  
    for(it=ListDisConnectedFun.begin();it!=ListDisConnectedFun.end();++it)  
    {  
        CFunObj& funObj=*it;
        void* pObj=funObj.pObject;
        void* pFun=funObj.pFun;

        //method1:
        //pFunVoid pCallfun=*((pFunVoid*)(&pFun));   //VS WINDOWS OK  gcc LINUX ERROR

        //method2
        pFunVoid pCallfun;
       *((void** )&pCallfun)=pFun;    //gcc LINUX OK


        //写法2；易于理解
        CNoUsetoCAST* pObject=(CNoUsetoCAST*)pObj;   //强转成类的对象指针
        (pObject->*pCallfun)();
    }
}

void MessageRegistrar::UnRegisterAll()
{
    MapMsgFun.clear();
    ListConnectedFun.clear();
    ListDisConnectedFun.clear();
}
