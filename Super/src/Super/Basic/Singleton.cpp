#include "Singleton.h"
#include "global.h"

//关于单利模式的理解参见以下博文

//版权声明：本文为CSDN博主「X-Programer」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/q5707802/java/article/details/79251491

//上篇文章提及到单例的懒汉模式是线程不安全的，会产生race condition从而产生多次初始化的情况。
//要想在多线程下工作，最容易想到的就是用锁来保护共享变量了。下面是伪代码：


#if 0
template<typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        {
            MutexGuard guard(mutex_) // RAII
                if (!value_)
                {
                    value_ = new T();
                }
        }
        return *value_;
    }
private:
    Singleton();
    ~Singleton();
    static T* value_;
    static Mutex mutex_;
};
template<typename T>
T* Singleton<T>::value_ = NULL;
template<typename T>
Mutex Singleton<T>::mutex_;
#endif

//这样在多线程下就能正常工作了。但是多个线程调用getInstance的时候，线程A第一次进入临界区，发现value_为NULL值就创建对象实例返回，接着线程B进入临界区获得锁，同时线程C也调用getInstance，因为临界区被线程B占有，所以线程C被阻塞等待。但是线程B和线程C只是读操作，读操作是不需要加锁，所以对读操作加锁是浪费的，加锁的代码过高，特别是在高并发的情景下，这个锁的代价是非常高的。这个时候，为了解决这个问题，DCL写法就被聪明的先驱者发明了。
//DCL即double-checked locking。在普通加锁的写法中，每次调用getInstance都会进入临界区，这样在heavy contention的情况下该函数就会成为系统性能的瓶颈，这个时候就有先驱者们想到了DCL写法，也就是进行两次check，当第一次check为空时，才加锁，再进行第二次check，再为空才创建。


#if 0
template<typename T>
  class Singleton
  {
  public:
      static T& getInstance()
      {
          if(!value_)
          {
              MutexGuard guard(mutex_);
              if (!value_)
              {
                  value_ = new T();
              }
          }
          return *value_;
      }
  private:
      Singleton();
      ~Singleton();
      static T* value_;
      static Mutex mutex_;
  };

  template<typename T>
  T* Singleton<T>::value_ = NULL;
  template<typename T>
  Mutex Singleton<T>::mutex_;
#endif

#if 0
有人会问为什么还要第二次check呢？假如不要第二次check，线程A和线程B都进行了第一次check,
线程A先获得锁就创建对象实例，之后线程B已经check为空,那么线程B也创建了对象实例，这样就有两个对象实例，破坏了唯一性。
在相当长的一段时间内，大家都以为这段代码很完美，直到1999年某专家发现这代码有漏洞，由于内存读写reorder不安全，会导致双检查锁失效。问题出在value_ = new T();这行代码。每行代码都它的指令序列，通常大家以为指令序列会按照想像的那样执行，
但实际上代码到汇编层次指令序列有可能和我们的假设的不一样，还有线程之间的切换是在指令层次进行的。
我们先看看第12行value_ = new T这一句发生了什么：
1.分配了一个T类型对象所需要的内存。
2.在分配的内存处构造T类型的对象。
3.把分配的内存的地址赋给指针value_
主观上，我们会觉得计算机在会按照1、2、3的步骤来执行代码，但是问题就出在这。实际上只能确定步骤1最先执行，
经过reorder后可能是先执行步骤3最后才步骤2。假如某一个线程A在调用getInstance的时候第12行的语句按照1、3、2的步骤执行，
那么当刚刚执行完步骤3的时候发生线程切换，计算机开始执行另外一个线程B。因为第一次check没有上锁保护，那么在线程B中调用getInstance的时候，不会在第一次check上等待，而是执行这一句，那么此时value_已经被赋值了，就会直接返回*value_然后执行后面使用T类型对象的语句，但是在A线程中步骤3还没有执行！也就是说在B线程中通过getInstance返回的对象还没有被构造就被拿去使用了！这样就会发生一些难以debug的灾难问题。

C#使用volatile关键字来解决这个问题，告诉编译器不要reorder这行代码，严格按照1、2、3的步骤来执行代码。
关于DCL问题的详细讨论分析，可以参考Scott Meyer的paper：《C++ and the Perils of Double-Checked Locking》
:http://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf
不过在新的C++11中，这个问题得到了解决。因为新的C++11规定了新的内存模型，保证了执行上述3个步骤的时候不会发生线程切换，相当这个初始化过程是“原子性”的的操作，DCL又可以正确使用了，不过在C++11下却有更简洁的多线程Singleton写法了，这个留在下一篇再介绍。

在C++11之前的版本下，要使其正确执行的话，就得在步骤2、3直接加上一道memory barrier(内存屏障)，强迫CPU执行的时候按照1、2、3的步骤来运行，还需要用RCU技法，即read-copy-update(读拷贝更新机制)。
关于内存屏障的详细概念参见?:
http://blog.csdn.net/world_hello_100/article/details/50131497
https://www.zhihu.com/question/20228202
还有read-copy-update(读拷贝更新机制)的概念参见:
http://blog.csdn.net/bluenet13/article/details/39378713
以下是使用内存屏障技术来实现DCLP的代码:
#endif

#if 0
static T& getInstance()
{
    if(!value_)
    {
        MutexGuard guard(mutex_);
        if (!value_)
        {
            T* p = static_cast<T*>(operator new(sizeof(T)));
            new (p) T();
            // insert some memory barier
            value_ = p; // RCU method
        }
    }
    return *value_;
}
#endif


#if 0
也许有人会说，你这已经把先前的value_ = new T()这一句拆成了下面这样的两条语句, 
为什么还要在后面插入some memory barrier？
T* p = static_cast<T*>(operator new(sizeof(T)));
new (p) T();

原因是现代处理器都是以Out-of-order execution（乱序执行）的方式来执行指令的。现代CPU基本都是多核心的，
一个核包含多个执行单元。例如，一个现代的Intel CPU 包含6个执行单元，可以做一组数学，条件逻辑和内存操作的组合。每个执行单元可以做这些任务的组合。这些执行单元并行地操作，允许指令并行地执行。如果从其它 CPU 来观察，这引入了程序顺序的另一层不确定性。
如果站在单个CPU核心的角度上讲，它（一个CPU核心）看到的程序代码都是单线程的，所以它在内部以自己的“优化方式”乱序、并行的执行代码，然后保证最终的结果和按代码逻辑顺序执行的结果一致。但是如果我们编写的代码是多线程的，当不同线程访问、操作共享内存区域的时候，就会出现CPU实际执行的结果和代码逻辑所期望的结果不一致的情况。这是因为以单个CPU核心的视角来看代码是“单线程”的。
所以为了解决这个问题，就需要memory barrier了，利用它来强迫CPU按代码的逻辑顺序执行。例如上面改动版本的getInstance代码中，因为第10行有memory barrier，所以CPU执行第9、10、11按“顺序”执行的。即使在CPU核心内是并行执行指令（比如一个单元执行第9行、一个单元执行第11行）的，但是他们在退役单元（retirement unit）更新执行结果到通用寄存器或者内存中时也是按照9、10、11顺序更新的。例如一个单元A先执行完了第11行，CPU让单元A等待直到执行第9行的单元B执行完成并在退役单元更新完结果以后再在退役单元更新A的结果。

参考文章:http://www.domaigne.com/blog/computing/mutex-and-memory-visibility/
memory barreir是一种特殊的处理器指令，他指挥处理器做下面三件事
*刷新store buffer。
*等待直到memory barreir之前的操作已经完成。
*不将memory barreir之后的操作移到memory barreir之前执行。
通过使用memory barreir，可以确保之前的乱序执行已经全部完成，并且未完成的写操作已全部刷新到主存。因此，数据一致性又重新回到其他线程的身边，从而保证正确内存的可见性。实际上，原子操作以及通过原子操作实现的模型（例如一些锁之类的），都是通过在底层加入memory barrier来实现的。

至于如何加入memory barrier，在unix上可以通过内核提供的barrier()宏来实现。或者直接嵌入ASM汇编指令mfence也可以，
barrier宏也是通过该指令实现的。
关于memory fence，不同的CPU，不同的编译器有不同的实现方式，要是直接使用还真是麻烦，不过，c++11中对这一概念进行了抽象，提供了方便的使用方式 。
#endif








//用C++实现单例模式——C++11实现

//1.static local 
//Scott Meyer在《Effective C++》中提出了一种简洁的singleton写法。

#if 0
template<typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T value;
        return value;
    }
private:
    Singleton();
    ~Singleton();
};
#endif

//先说结论：
//*单线程下，正确。
//*C++11及以后的版本（如C++14）的多线程下，正确。
//*C++11之前的多线程下，不一定正确。
//原因在于在C++11之前的标准中并没有规定local static变量的内存模型，所以很多编译器在实现local static变量
//的时候仅仅是进行了一次check（参考《深入探索C++对象模型》），于是getInstance函数被编译器改写成这样了：

#if 0
bool initialized = false;
char value[sizeof(T)];
T& getInstance()
{
        if (!initialized)
        {
            initialized = true;
            new (value) T();
        }
        return *(reinterpret_cast<T*>(value));
}
#endif

//于是乎它就是不是线程安全的了。
//但是在C++11却是线程安全的，这是因为新的C++标准规定了当一个线程正在初始化一个变量的时候，
//其他线程必须得等到该初始化完成以后才能访问它.
//在C++11 standard中的§6.7 [stmt.dcl] p4：
//If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.


//2.atomic_thread_fence
//关于memory fence，不同的CPU，不同的编译器有不同的实现方式，要是直接使用还真是麻烦，不过，
//c++11中对这一概念进行了抽象，提供了方便的使用方式
//在c++11中，可以获取(acquire/consume)和释放(release)内存栅栏来实现上述功能。使用c++11中的atomic类型来包装
//m_instance指针，这使得对m_instance的操作是一个原子操作。下面的代码演示了如何使用内存栅栏：

//std::atomic<Singleton*> Singleton::m_instance;
//std::mutex Singleton::m_mutex;
//Singleton* Singleton::getInstance()
//{
//             Singleton* tmp = m_instance.load(std::memory_order_relaxed);
//             std::atomic_thread_fence(std::memory_order_acquire);  
//             if (tmp == nullptr) {
//                 std::lock_guard<std::mutex> lock(m_mutex);
//                 tmp = m_instance.load(std::memory_order_relaxed);
//                 if (tmp == nullptr) {
//                     tmp = new Singleton;
//                     std::atomic_thread_fence(std::memory_order_release); 
//                     m_instance.store(tmp, std::memory_order_relaxed);
//                 }
//             }
//             return tmp;
//}


//上面的代码中atomic_thread_fence在创建对象线程和使用对象线程之间建立了一种“同步-与”的关系（synchronizes-with）。
//中文大意是创建一个多线程栅栏,调用该函数的位置成为一个(acquire或release或两者)的同步点, 
//在release线程中此同步点之前的数据更新都将同步于acquire 线程的同步点之前，这就实现线程可见性一致


//3.atomic
//上节的代码使用内存栅栏锁定技术可以很方便地实现双重检查锁定。但是看着实在有点麻烦，在C++11中更好的实现方式是直接使用原子操作.

#if 0
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_mutex;
Singleton* Singleton::getInstance()
{
        Singleton* tmp = m_instance.load(std::memory_order_acquire);
        if (tmp == nullptr) {
            std::lock_guard<std::mutex> lock(m_mutex);
            tmp = m_instance.load(std::memory_order_relaxed);
            if (tmp == nullptr) {
                tmp = new Singleton;
                m_instance.store(tmp, std::memory_order_release);
            }
        }
        return tmp;
}
#endif

//如果你对memory_order的概念还是不太清楚，那么就使用C++顺序一致的原子操作，所有std::atomic的操作如果不带参数默认都是std::memory_order_seq_cst,即顺序的原子操作（sequentially consistent）,简称SC,使用（SC）原子操作库，
//整个函数执行指令都将保证顺序执行,这是一种最保守的内存模型策略。
//下面的代码就是使用SC原子操作实现双重检查锁定。


#if 0
std::atomic<Singleton*> Singleton::m_instance;
std::mutex Singleton::m_mutex;
Singleton* Singleton::getInstance() 
{
            Singleton* tmp = m_instance.load();
            if (tmp == nullptr) {
                std::lock_guard<std::mutex> lock(m_mutex);
                tmp = m_instance.load();
                if (tmp == nullptr) {
                    tmp = new Singleton;
                    m_instance.store(tmp);
                }
            }
            return tmp;
}
#endif

//4.call_once(最简单的实现)
//在多线程编程中，有一个常见的情景是某个任务只需要执行一次。在C++11中提供了很方便的辅助类once_flag，call_once。
//以下是对std::call_once的原文说明：
//大意就是:
//call_one保证函数fn只被执行一次，如果有多个线程同时执行函数fn调用，则只有一个活动线程(active call)会执行函数，
//其他的线程在这个线程执行返回之前会处于”passive execution”(被动执行状态)—不会直接返回，
//直到活动线程对fn调用结束才返回。对于所有调用函数fn的并发线程的数据可见性都是同步的(一致的)。 
//如果活动线程在执行fn时抛出异常，则会从处于”passive execution”状态的线程中挑一个线程成为活动线程继续执行fn,依此类推。 
// 一旦活动线程返回,所有”passive execution”状态的线程也返回,不会成为活动线程。
//由上面的说明，我们可以确信call_once完全满足对多线程状态下对数据可见性的要求。
//所以利用call_once再结合lambda表达式,前面几节那么多复杂代码，在这里千言万语凝聚为一句话：

#if 0
Singleton* Singleton::m_instance;
Singleton* Singleton::getInstance()
{
    static std::once_flag oc;   //用于call_once的局部静态变量
    std::call_once(oc,[&]{m_instance = new Singleton();});
    return m_instance;
}
#endif

//总结
//本文中提到的几种方法都是安全可用的方案,具体用哪种,我个人觉得还是call_once最简单，我肯定选call_one。
//但不代表前面的那么多都白写了，其实学习每种方法过程中让我对c++11内存模型有了更深入的理解，这才是最大的收获。


class MyClass
{
public:
    MyClass()
    {
        PRINT_FUN_NAME
    }
    ~MyClass()
    {
        PRINT_FUN_NAME
    }
    void testFun()
    {
        PRINT_FUN_NAME
    }
private:

};

#ifdef UNIT_TEST
int Test_Singleton()
{
    GSingleton<MyClass>::getInstance()->testFun();
    return 0;
}
#endif



#if 0

//调用示例
//test.cpp
#include <iostream>


using namespace std;
class MyClass2 {
public:
    MyClass2(const string& strData) : m_strData(strData)
    {
        cout << m_strData.data() << endl;
    };
    ~MyClass2()
    {
        cout << "destory" << endl;
    };

private:
    string m_strData;
};

//auto main()->int   //等同于入口函数int main()
int testMagic_Singleton()
{
    auto pClass = Magic_Singleton<MyClass2>::GetInstance("create");
    Magic_Singleton<MyClass>::DesInstance();
    return 1;
}

#endif
