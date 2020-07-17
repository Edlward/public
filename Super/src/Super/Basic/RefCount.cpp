#include "RefCount.h"

//C++ 引用计数器
//    2016年12月14日 17:16:56 飞碟说 阅读数：531 标签： c语言 计数器
//    版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/I_can_do_1098/article/details/53642298
//今天讲一个实用的东西，C++类的引用计数，不废话直接上代码；
//


#ifdef _WIN32
    #include <Windows.h>
#else
    #include <atomic> 
#endif
using namespace std;

class CRefCounter
{
public:
    CRefCounter();
    ~CRefCounter(){};
    long AddRef();  //增加引用计数;
    void Release(); //减少引用计数，为0的时候自动释放;
private:
#ifdef _WIN32
    //加这个volatile是表示这个变量是易变的，不需要编译器去优化，一切读取都是从内存操作
    volatile long refercounter;
#else
   // atomic_t refercounter;
    atomic<int> refercounter;
#endif
};

CRefCounter::CRefCounter()
{
#ifdef _WIN32
    this->refercounter = 0;
#else
    //atomic_set(&this->refercounter, 0);
    refercounter=0;
#endif
}

long  CRefCounter::AddRef()
{
#ifdef _WIN32
    InterlockedIncrement(&this->refercounter);
    return this->refercounter;
#else
    //atomic_add(1, &this->refercounter);
    //return atomic_read(&this->refercounter);
        refercounter=refercounter+1;
#endif
}

void CRefCounter::Release()
{
#ifdef _WIN32
    InterlockedDecrement(&this->refercounter);
    if (this->refercounter == 0)
    {
        delete(this);
    }
#else
//    atomic_dec(&this->refercounter);
//    if (atomic_read(&this->refercounter) == 0)
      refercounter=refercounter-1;
    if(refercounter==0)
    {
        delete(this);
    }
#endif
    return;
};
 
    //代码很简单，而且大家不要担心有坑，这段代码是经过无数次锤炼的。 
    //说白了就是原子操作，用的时候怎么用？直接继承就可以了。 
    //例如：

class A:public CRefCounter
{
public:
    A(){};
    ~A(){};
};


void funB()
{
    A* pA =new A();

    pA->AddRef();
    //你的代码
    pA->Release();
}

 
    //释放类的时候不要用delete，改成调用Release(）函数， 
    //好处：避免别的多线程的时候，X线程还在使用，结果被其它线程给释放掉了。 
    //坏处：原子操作是很耗时的，会降低运行的效率，尤其是多线程的时候。 
