#ifndef SINGLETON_H
#define SINGLETON_H



//单例类
//指在整个系统生命期中,一个类最多只能有一个实例(instance)存在,比如用在统计在线人数

//懒汉式 : 只有当需要时,才去创建实例
//饿汉式 : 程序开始实例已经存在,当时需要时直接去访问即可


//用法
//将构造函数的访问属性设置为private,
//提供一个GetInstance()静态成员函数,只能供用户访问唯一一个实例.
//定义一个静态成员指针,用来供用户获取
//重载 (=)赋值操作符以及拷贝构造函数,并设为private, 避免对象间拷贝,复制.

#include <mutex>  //线程安全问题需要加锁


//单例模式模板 
//CreateIfNeed默认false,初始化时创建，对应恶汉模式
//CreateIfNeed为true则在第一次访问时创建,对应懒汉模式
//creation and improve by lizulin 2019

//可变参数初始化待续
template<typename T,bool CreateIfNeed=false>
class GSingleton
{
public:

    static T* getInstance()
    {
        if (CreateIfNeed)   //饱汉/汉饥模式编译时决议,不影响性能
        {
            if (instance==NULL)
            {
                  std::lock_guard<std::mutex> gLock(m_Gsmutex);
                  if (instance==NULL)
                  {
                        //instance = new T();   //有问题见注释
                        T* tmp=new T();    //更严谨的应该分成两步进行,new 之后指针已经存在但是,构造函数可能未完成
                        instance =tmp;
                  }
            }
        }
       return instance;
    }
    static T* CreateInstanceIfNeed() 
    {
        if (!CreateIfNeed)
        {
            instance = new T();
        }
        else
        {
            instance =NULL;
        }
        return instance;
    }
    //主动析构单例对象（一般不需要除非特殊需求）
    static void DesInstance()
    {
        if (instance)
        {
            delete instance;
            instance=NULL;
        }
    }
protected:
    static T* instance;
    static std::mutex m_Gsmutex;
private:
    GSingleton(){};
    virtual ~GSingleton(){};
    GSingleton(const GSingleton&);
    GSingleton& operator=(const GSingleton&);
};

//template<typename T> T* GSingleton<T>::instance=NULL;

template<typename T,bool CreateIfNeed>
T* GSingleton<T,CreateIfNeed>::instance=GSingleton<T,CreateIfNeed>::CreateInstanceIfNeed();  //=NULL

template<typename T,bool CreateIfNeed> 
std::mutex GSingleton<T,CreateIfNeed>::m_Gsmutex;



//单例模式宏  
#define SINGLETON(ClassName) \
public:\
inline static ClassName& GetInstance()\
{                                       \
static ClassName s_instance;              \
return s_instance;                      \
}                                       \
private:                                  \
    ClassName(){};                          \
    ClassName(const ClassName&){}\
    ClassName& operator= (const& ClassName ){return *this;}\
    ~ClassName();                             \

//单例模式默认构造函数与析构函数（配合单例模式宏使用
#define SINGLETON_C_D(ClassName) \
           ClassName::ClassName(){}            \
          ClassName::~ClassName(){}           \


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//VS2012 不支持可变次数,待续

#if 0

//简单单例模式面临的问题
//    不能阻止析构、拷贝、赋值、隐式转换等问题。
//    多线程调用中的线程安全问题。
//    没有自动GC机制（垃圾回收）。
//    一个基于C++11的万用单例模板类
    //Magic_Singleton.h

    /************************************************************************/
    /* 名称：万能单例类                                                     */
    /* 说明：可把任何类包装成线程安全的全局单例类，出口默认智能指针			*/
    /* 作者：Song															*/
    /* Email：songshuda@vip.qq.com											*/	
    /************************************************************************/
//利用了C++11的智能指针和自解锁等，巧妙避免了所有问题，并实现自动GC。

#include <mutex>
#include <memory>

template<typename T>
class Magic_Singleton 
{
public:
    //获取全局单例对象
    template<typename ...Args>
    static std::shared_ptr<T> GetInstance(Args&&... args)
    {
        if (!m_pSington)
        {
            std::lock_guard<std::mutex> gLock(m_Mutex);
            if (nullptr == m_pSington) {
                m_pSington = std::make_shared<T>(std::forward<Args>(args)...);
            }
        }
        return m_pSington;
    }
    //主动析构单例对象（一般不需要主动析构，除非特殊需求）
    static void DesInstance()
    {
        if (m_pSington)
        {
            m_pSington.reset();
            m_pSington = nullptr;
        }
    }

private:
    explicit Magic_Singleton();
    Magic_Singleton(const Magic_Singleton&) = delete;
    Magic_Singleton& operator=(const Magic_Singleton&) = delete;
    ~Magic_Singleton();
private:
    static std::shared_ptr<T> m_pSington;
    static std::mutex m_Mutex;
};

template<typename T>
std::shared_ptr<T> Magic_Singleton<T>::m_pSington = nullptr;

template<typename T>
std::mutex Magic_Singleton<T>::m_Mutex;
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////










#ifdef UNIT_TEST
BASIC_API int Test_Singleton();
#endif



#endif //SINGLETON_DIFINE_H_  


