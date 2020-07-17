#pragma once

#ifndef _ANY_Header_
#define _ANY_Header_









//from  https://www.cnblogs.com/qicosmos/p/3420095.html

//any类设计要点
//1.any类不能是一个模板类形如int i;any<int>anyValue=i;无意义，还不如直接写int anyValue =i;
//2.any必须提供模板构造函数（赋值操作不必是模板的），才能完成如下操作:
//  int i; long j; struct X; X x;any anyValue(i); anyValue=j; anyValue =x;
//3.必须提供某些有关它所保存的对象型别的信息。
//4.它必须能保存数值且提供某种方法将它保存的数值“取出来”。
//5.数据不能放在any类里，这会使any类成为模板类，不符合1的要求。
//  数据应该动态存放，即动态分配一个数据的容器来存放数据，而any类中则保存指向这个容器的指针。
//  明确地说，是指向这个容器的基类的指针，这是因为容器本身必须为模板，而any类中的指针成员又必须不是泛型的。
//  因为any不能是泛型的，所以any中所有数据成员都不能是泛型的。
//  所以，结论是：为容器准备一个非泛型的基类，而让指针指向该基类。


#include <memory>
#include <typeindex>
#include <exception>
#include <iostream>

struct Any
{
    Any(void) : m_tpIndex(std::type_index(typeid(void))) 
    { 
        std::cout << "Any(void) function called!" << std::endl; 
    }
    Any(const Any& that) : m_ptr(that.Clone()), m_tpIndex(that.m_tpIndex) 
    { 
        std::cout << "Any(const Any& that) called!" << std::endl; 
    }
    Any(Any && that) : m_ptr(std::move(that.m_ptr)), m_tpIndex(std::move(that.m_tpIndex)) 
    { 
        std::cout << " Any(Any && that) function called!" << std::endl; 
    }

    //通用的右值构造
    //创建智能指针时，对于一般的类型，通过std::decay来移除引用和cv符，从而获取原始类型
    //VS2012编译问题解决待续
#if 0
    template<typename U, class= typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value,U>::type>
    Any(U && value) : m_ptr(new Derived < typename std::decay<U>::type>(std::forward<U>(value))),
        m_tpIndex(std::type_index(typeid(typename std::decay<U>::type)))
    {
            std::cout << "Any(U && value) template function called!" << std::endl;
    } 
#endif


    template<class U> bool Is() const
    {
        std::cout << "Any::Is() template function called!" << std::endl;
        return m_tpIndex == std::type_index(typeid(U));
    }
    bool IsNull() const 
    {
        std::cout << "Any::IsNull() function called!" << std::endl;
        return !bool(m_ptr);
    }
    //AnyCast方法的思想是期望程序员们清楚自己在做什么，要不然就给他个异常瞧瞧。

    //AnyCast模仿了boost提供的any_cast<>模板方法，在这里是Any的成员函数
    //取出原始类型，需要客户提供类型信息 如 anyValue.AnyCast<int>
    //m_tpIndex里保存了关于原始数据的型别信息，不存在boost::any实现时丢失型别信息的情况
    //所以boost单独提供了any_cast版本出来，而这里不需要
    template<class U>
    U& Cast()
    {
        std::cout << "Any::AnyCast() template function called!" << std::endl;
        //判断类型信息是否匹配，不匹配报错
        if (!Is<U>())
        {
            std::cout << "can not cast " << typeid(U).name() << " to " << m_tpIndex.name() << std::endl;
            throw std::logic_error{ "bad cast" };
        }
        //匹配了,则取出原始数据
        auto derived = dynamic_cast<Derived<U>*> (m_ptr.get());
        return derived->m_value;
    }
    //赋值操作重载不是模板函数，不存在模板无穷递归即循环赋值问题
    Any& operator=(const Any& a)
    {
        std::cout << "Any& operator= function called!" << std::endl;
        //防止自赋值
        if (m_ptr == a.m_ptr)
            return *this;
        //两个数据成员赋值
        m_ptr = a.Clone();
        m_tpIndex = a.m_tpIndex;
        return *this;
    }
private:
    struct Base;
    typedef std::unique_ptr<Base> BasePtr;   
    //泛型数据容器Derived的非泛型基类
    struct Base
    {
        virtual ~Base() {
            std::cout << " virtual ~Base() function called!" << std::endl;
        }  //虚析构函数，为保证派生类对象能用基类指针析构
        virtual BasePtr Clone() const = 0;   //复制容器
    };

    //Derived是个模板类，各个类成员之间可以共享类模板参数的信息
    //所以，可以方便地用原数据类型来进行各种操作
    template<typename T>
    struct Derived :public Base
    {
        template<typename U>
        Derived(U && value) : m_value(std::forward<U>(value)) { 
            std::cout << "Derived(U && value) function called!" << std::endl;
        }

        //template<typename...Args>
        //Derived(Args&&...args) : m_value(std::forward<Args>(args)...)
        //{
        //}

        BasePtr Clone() const
        {
            std::cout << "Derived::Clone() function called!" << std::endl;
            return BasePtr(new Derived<T>(m_value)); //改写虚函数，返回自身的复制体,动态构造
        }
        T m_value;    //原始数据保存的地方
    };
    //any类转发Clone操作给BasePtr的Clone方法，实现数据复制
    BasePtr Clone() const
    {
        std::cout << "Any::Clone() function called!" << std::endl;
        if (m_ptr != nullptr)
            return m_ptr->Clone();  
        return nullptr;
    }
    //通过基类指针擦除具体类型
    //any类赋值时需要创建派生类对象，这里用std::unique_ptr管理派生类对象生命周期
    BasePtr m_ptr;          
    std::type_index m_tpIndex; //提供关于型别的信息
};





//
//C++17的any成员：
//    void reset() ： 重置（清空）any对象。
//
//    bool has_value() const ： 值判断，有值返回true，无值返回false。
//
//    const type_info& type() const ： 获取类型，返回一个type_info&类的常引用，其成员有类型的哈希值及类型名称（const char*）
//
//    void swap(any & other) ：与另一个any对象交换“值”。
//
//    模板函数（简化版）：
//
//    template<typename Type>
//
//Type& emplace(Type&& Args) ：修改any对象的值。
//
//    template<typename Type>
//
//Type* _Cast() ：返回any对象值的地址





//简化原理
#if 0
class Any {
public:
    template<typename T>
    Any(T t) :base(new Data<T>(t)) {}		//模板构造函数
    template<typename T>
    T _Cast() {
        return dynamic_cast<Data<T>*>(base.get())->value;		//强制转换
    }
private:
    class Base {
    public:
        virtual ~Base() {}		//确定Base为多态类型
    };
    template <typename T>
    class Data :public Base {
    public:
        Data(T t) :value(t) {}
        T value;
    };
    std::unique_ptr<Base> base;					//基类指针
};

#endif



#ifdef UNIT_TEST
int Test_Any();
#endif


#endif

