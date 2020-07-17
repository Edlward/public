#pragma once

#ifndef _ANY_Header_
#define _ANY_Header_









//from  https://www.cnblogs.com/qicosmos/p/3420095.html

//any�����Ҫ��
//1.any�಻����һ��ģ��������int i;any<int>anyValue=i;�����壬������ֱ��дint anyValue =i;
//2.any�����ṩģ�幹�캯������ֵ����������ģ��ģ�������������²���:
//  int i; long j; struct X; X x;any anyValue(i); anyValue=j; anyValue =x;
//3.�����ṩĳЩ�й���������Ķ����ͱ����Ϣ��
//4.�������ܱ�����ֵ���ṩĳ�ַ��������������ֵ��ȡ��������
//5.���ݲ��ܷ���any������ʹany���Ϊģ���࣬������1��Ҫ��
//  ����Ӧ�ö�̬��ţ�����̬����һ�����ݵ�������������ݣ���any�����򱣴�ָ�����������ָ�롣
//  ��ȷ��˵����ָ����������Ļ����ָ�룬������Ϊ�����������Ϊģ�壬��any���е�ָ���Ա�ֱ��벻�Ƿ��͵ġ�
//  ��Ϊany�����Ƿ��͵ģ�����any���������ݳ�Ա�������Ƿ��͵ġ�
//  ���ԣ������ǣ�Ϊ����׼��һ���Ƿ��͵Ļ��࣬����ָ��ָ��û��ࡣ


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

    //ͨ�õ���ֵ����
    //��������ָ��ʱ������һ������ͣ�ͨ��std::decay���Ƴ����ú�cv�����Ӷ���ȡԭʼ����
    //VS2012��������������
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
    //AnyCast������˼������������Ա������Լ�����ʲô��Ҫ��Ȼ�͸������쳣���ơ�

    //AnyCastģ����boost�ṩ��any_cast<>ģ�巽������������Any�ĳ�Ա����
    //ȡ��ԭʼ���ͣ���Ҫ�ͻ��ṩ������Ϣ �� anyValue.AnyCast<int>
    //m_tpIndex�ﱣ���˹���ԭʼ���ݵ��ͱ���Ϣ��������boost::anyʵ��ʱ��ʧ�ͱ���Ϣ�����
    //����boost�����ṩ��any_cast�汾�����������ﲻ��Ҫ
    template<class U>
    U& Cast()
    {
        std::cout << "Any::AnyCast() template function called!" << std::endl;
        //�ж�������Ϣ�Ƿ�ƥ�䣬��ƥ�䱨��
        if (!Is<U>())
        {
            std::cout << "can not cast " << typeid(U).name() << " to " << m_tpIndex.name() << std::endl;
            throw std::logic_error{ "bad cast" };
        }
        //ƥ����,��ȡ��ԭʼ����
        auto derived = dynamic_cast<Derived<U>*> (m_ptr.get());
        return derived->m_value;
    }
    //��ֵ�������ز���ģ�庯����������ģ������ݹ鼴ѭ����ֵ����
    Any& operator=(const Any& a)
    {
        std::cout << "Any& operator= function called!" << std::endl;
        //��ֹ�Ը�ֵ
        if (m_ptr == a.m_ptr)
            return *this;
        //�������ݳ�Ա��ֵ
        m_ptr = a.Clone();
        m_tpIndex = a.m_tpIndex;
        return *this;
    }
private:
    struct Base;
    typedef std::unique_ptr<Base> BasePtr;   
    //������������Derived�ķǷ��ͻ���
    struct Base
    {
        virtual ~Base() {
            std::cout << " virtual ~Base() function called!" << std::endl;
        }  //������������Ϊ��֤������������û���ָ������
        virtual BasePtr Clone() const = 0;   //��������
    };

    //Derived�Ǹ�ģ���࣬�������Ա֮����Թ�����ģ���������Ϣ
    //���ԣ����Է������ԭ�������������и��ֲ���
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
            return BasePtr(new Derived<T>(m_value)); //��д�麯������������ĸ�����,��̬����
        }
        T m_value;    //ԭʼ���ݱ���ĵط�
    };
    //any��ת��Clone������BasePtr��Clone������ʵ�����ݸ���
    BasePtr Clone() const
    {
        std::cout << "Any::Clone() function called!" << std::endl;
        if (m_ptr != nullptr)
            return m_ptr->Clone();  
        return nullptr;
    }
    //ͨ������ָ�������������
    //any�ำֵʱ��Ҫ�������������������std::unique_ptr���������������������
    BasePtr m_ptr;          
    std::type_index m_tpIndex; //�ṩ�����ͱ����Ϣ
};





//
//C++17��any��Ա��
//    void reset() �� ���ã���գ�any����
//
//    bool has_value() const �� ֵ�жϣ���ֵ����true����ֵ����false��
//
//    const type_info& type() const �� ��ȡ���ͣ�����һ��type_info&��ĳ����ã����Ա�����͵Ĺ�ϣֵ���������ƣ�const char*��
//
//    void swap(any & other) ������һ��any���󽻻���ֵ����
//
//    ģ�庯�����򻯰棩��
//
//    template<typename Type>
//
//Type& emplace(Type&& Args) ���޸�any�����ֵ��
//
//    template<typename Type>
//
//Type* _Cast() ������any����ֵ�ĵ�ַ





//��ԭ��
#if 0
class Any {
public:
    template<typename T>
    Any(T t) :base(new Data<T>(t)) {}		//ģ�幹�캯��
    template<typename T>
    T _Cast() {
        return dynamic_cast<Data<T>*>(base.get())->value;		//ǿ��ת��
    }
private:
    class Base {
    public:
        virtual ~Base() {}		//ȷ��BaseΪ��̬����
    };
    template <typename T>
    class Data :public Base {
    public:
        Data(T t) :value(t) {}
        T value;
    };
    std::unique_ptr<Base> base;					//����ָ��
};

#endif



#ifdef UNIT_TEST
int Test_Any();
#endif


#endif

