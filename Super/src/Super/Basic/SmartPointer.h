#ifndef  SMARTPOINT_H
#define SMARTPOINT_H

#include "global.h"

#ifdef UNIT_TEST
BASIC_API void Test_SmartPtr();
#endif



#include <stdlib.h>  //NULL头文件





#if 0


// CMyAutoPtr (auto-pointer) class
// Created by liang 

#if 0
#include <vector>
#include <list>
#include <assert.h>
using namespace std;
#endif

template<class T>
class CMyAutoPtr{
public:
	// local class CMyRefCount ->
	template<class T1>
	//内部引用计数
	class CMyRefCount{
	public:
		CMyRefCount():_ptr(0),_count(0){}
		CMyRefCount(T1* p):_ptr(p),_count(1){
		}
		virtual ~CMyRefCount(){
			delete _ptr;
		}
		int Release(){
			if(_ptr)
			{
				//assert(_count>0);   //modify by lizulin 20170607
				_count--;
				if(_count==0){
					delete _ptr; _ptr = 0;
				}
			}
			return _count;
		}
		void AddRef(unsigned int cnt=1){
			_count += cnt;
		}
		T1* GetPtr(){return _ptr;}
		//bool IsEmpty(){return _count==0;};
		int GetRefCount(){return _count;};
	private:
		T1*  _ptr;
		int _count;
	};
	// local class CMyRefCount <-

public:
	CMyAutoPtr():_count_ptr(0){}
	CMyAutoPtr(T* p){
		_count_ptr = new CMyRefCount<T>(p);
	}
	CMyAutoPtr(const CMyAutoPtr& v){
		*this = v;
	}
	virtual ~CMyAutoPtr(){
		Release();
	}
	CMyAutoPtr& operator =(const CMyAutoPtr& a){
		if(this!=&a)
		{
			_count_ptr = a._count_ptr;
			if(_count_ptr)
				_count_ptr->AddRef();
		}
		return *this;
	}
	CMyAutoPtr& operator =(T* p){
		CMyRefCount<T>* save = _count_ptr;
		_count_ptr = p ? new CMyRefCount<T>(p) : 0;
		if(save && save->Release()==0)
		{
			delete save;
		}
		return *this;
	}
	T* operator *()
	{
		return _count_ptr==0?0:_count_ptr->GetPtr();
	}
	T* operator ->()
	{
		//assert(GetPtr()!=0);
		return GetPtr();
	}
	operator T*()
	{
		//assert(GetPtr()!=0);
		return GetPtr();
	}
	T* GetPtr()
	{
		return _count_ptr==0?0:_count_ptr->GetPtr();
	}
	void Release()
	{
		if(_count_ptr && _count_ptr->Release()==0)
		{
			delete _count_ptr;
			_count_ptr = 0;
		}
	}
	void AddRef(unsigned int cnt=1)
	{
		if(_count_ptr)
			_count_ptr->AddRef(cnt);
	}
	int GetRefCount()
	{
		return _count_ptr?_count_ptr->GetRefCount():0;
	}
	bool IsEmpty(){return _count_ptr==0;};
private:
	CMyRefCount<T>*  _count_ptr;
};










/////////////////////////////////////////////////////// 
// 
//  Author Sandu Turcan 
//  idlsoft@hotmail.com 
// 


template <class T> class SmartPtr; 
///////////////////////////////////////// 
// IRefCount 
// is an interface for reference counting 
// Classes can implement it themselves, 
// or SmartPtr will provide its internal implementation of IRefCount 
template <class T> 
class IRefCount { 
	friend class SmartPtr<T>; 
protected: 
	virtual void __IncRefCount() = 0; 
	virtual void __DecRefCount() = 0; 
	virtual T * GetPtr() const = 0; 
}; 
//============================================================ 
///////////////////////////////////////// 
// IRefCountImpl 
// is a standart implementation of IRefCount 
// To use it just derive your class from it: 
// class CMyObject : public IRefCountImpl<CMyObject> { ... }; 
// Reminder: implementing IRefCount is optional but it would reduce 
// memory fragmentation.  
template <class T> 
class IRefCountImpl : public IRefCount<T> 
{ 
private: 
	int __m_counter; 
protected: 
	virtual void __IncRefCount()  
	{ 
		__m_counter++;  
	} 
	virtual void __DecRefCount() 
	{ 
		__m_counter--; 
		if(__m_counter<=0) 
		{ 
			__DestroyRef(); 
		} 
	} 
	virtual T * GetPtr() const 
	{ 
		return ((T *)this); 
	} 
	virtual void __DestroyRef()  
	{  
		if(GetPtr()!=NULL) 
			delete GetPtr(); 
	} 
protected: 
	IRefCountImpl() 
	{ 
		__m_counter = 0; 
	} 
}; 

//============================================================ 

///////////////////////////////////////// 
// SmartPtr 
// Usage: 
// ---------------------- 
// 1. In a program block 
// ---------------------- 
// SmartPtr<CMyObject> ptr1(new CMyObject); // creates object 1 
// SmartPtr<CMyObject> ptr2(new CMyObject); // creates object 2 
// ptr1 = ptr2;             // destroys object 1 
// ptr2 = NULL; 
// ptr1 = new CMyObject;    // creates object 3, destroys object 2 
// ptr1->methodcall(...); 
// CMyObject o1; 
// // ptr1 = &o1;  // DON'T ! only memory allocated by new operator should be used 
// CMyObject *o2 = new CMyObject; 
// ptr1 = o2; 
// //ptr2 = o2;  // DON'T ! unless CMyObject implements IRefCount 
//               // try to use ptr1 = ptr2 instead, it's always safe; 
// ---------------------- 
// 2. in a function call 
// ---------------------- 
// void f(CMyObject *o) {...} 
// ... 
// SmartPtr<CMyObject> ptr(new CMyObject) 
// f(ptr); 
// ---------------------- 
// 3. As a return value 
// ---------------------- 
// SmartPtr<CMyObject> f() 
// { 
//      SmartPtr<CMyObject> ptr(new CMyObject); 
//      return ptr; 
// } 
template <class T> 
class SmartPtr 
{ 
private: 
	IRefCount<T> *__m_refcount; 
	///////////////////////////////////////// 
	// __RefCounter 
	// An internal implementation of IRefCount 
	// for classes that don't implement it 
	// SmartPtr will automatically choose between its internal and  
	// class' implementation of IRefCount 
	class __RefCounter : public IRefCountImpl<T> { 
	private: 
		T *__m_ptr; 
	protected: 
		virtual T * GetPtr() const 
		{ 
			return __m_ptr; 
		} 
		virtual void __DestroyRef() {delete this;} 
	public: 
		__RefCounter(T *ptr) 
		{ 
			__m_ptr = ptr; 
		} 
		virtual ~__RefCounter() 
		{ 
			IRefCountImpl<T>::__DestroyRef(); 
		} 
	}; 
	// this method is called if T does not implement IRefCount 
	void __Assign(void *ptr) 
	{ 
		if(ptr==NULL) 
			__Assign((IRefCount<T> *)NULL); 
		else 
		{ 
			__Assign(new __RefCounter(static_cast<T *>(ptr))); 
		} 
	} 
	// this method is picked over __Assign(void *ptr) 
	// if T implements IRefCount. 
	// This allows some memory usage optimization 
	void __Assign(IRefCount<T> *refcount) 
	{ 
		if(refcount!=NULL) refcount->__IncRefCount(); 
		IRefCount<T> *oldref = __m_refcount; 
		__m_refcount = refcount; 
		if(oldref!=NULL) oldref->__DecRefCount(); 
	} 
public: 
	SmartPtr() 
	{ 
		__m_refcount = NULL; 
	} 
	SmartPtr(T * ptr) 
	{ 
		__m_refcount = NULL; 
		__Assign(ptr); 
	} 
	SmartPtr(const SmartPtr &sp) 
	{ 
		__m_refcount = NULL; 
		__Assign(sp.__m_refcount); 
	} 
	virtual ~SmartPtr() 
	{ 
		__Assign((IRefCount<T> *)NULL); 
	} 

	// get the contained pointer, not really needed but... 
	T *GetPtr() const 
	{ 
		if(__m_refcount==NULL) return NULL; 
		return __m_refcount->GetPtr(); 
	} 

	// assign another smart pointer 
	SmartPtr & operator = (const SmartPtr &sp) {__Assign(sp.__m_refcount); return *this;} 
	// assign pointer or NULL 
	SmartPtr & operator = (T * ptr) {__Assign(ptr); return *this;} 
	// to access members of T 
	T * operator ->() 
	{ 
#ifdef _ASSERT 
		_ASSERT(GetPtr()!=NULL); 
#endif 
		return GetPtr(); 
	} 
	// conversion to T* (for function calls) 
	operator T* () const 
	{ 
		return GetPtr(); 
	} 

	// utilities 
	bool operator !() 
	{ 
		return GetPtr()==NULL; 
	} 
	bool operator ==(const SmartPtr &sp) 
	{ 
		return GetPtr()==sp.GetPtr(); 
	} 
	bool operator !=(const SmartPtr &sp) 
	{ 
		return GetPtr()!=sp.GetPtr(); 
	} 
}; 













//实现一个简单的shared_ptr
//	觉得这份代码足以用来初步了解shared_ptr的实现了。
//	一般来说，智能指针的实现需要以下步骤：
//	1.一个模板指针T* ptr，指向实际的对象。
//	2.一个引用次数(必须new出来的，不然会多个shared_ptr里面会有不同的引用次数而导致多次delete)。
//	3.重载operator*和operator->，使得能像指针一样使用shared_ptr。
//	4.重载copy constructor，使其引用次数加一。
//	5.重载operator=，如果原来的shared_ptr已经有对象，则让其引用次数减一并判断引用是否为零(是否调用delete)。
//	　然后将新的对象引用次数加一。
//	 6.重载析构函数，使引用次数减一并判断引用是否为零(是否调用delete)。
template <typename T>
class shared_ptr 
{
public:
	shared_ptr(T* p) : count(new int(1)), _ptr(p){}
	shared_ptr(shared_ptr<T>& other) : count(&(++*other.count)), _ptr(other._ptr) {}
	shared_ptr<T>& operator=(shared_ptr<T>& other)
	{
		++*other.count;
		if (this->_ptr && 0 == --*this->count)
		{
			delete count;
			delete _ptr;
		}
		this->_ptr = other._ptr;
		this->count = other.count;
		return *this;
	}
	~shared_ptr()
	{
		if (--*count == 0)
		{
			delete count;
			delete _ptr;
		}
	}
	T* operator->() { return _ptr; }
	T& operator*() { return *_ptr; }
	int getRef() { return *count; }
private:
	int* count;
	T* _ptr;
};


 

//在单位写代码没法用c++11,为了方便写代码，今天趁着空闲时间写了个unique_ptr的简单实现

//default deleter for unique_ptr
template<typename T>
struct DefaultDeleter
{
	void operator()(T *p)
	{
		if (p)
		{
			delete p;
			p = NULL;
		}
	}
};

template<typename T, typename Deleter = DefaultDeleter<T>>
class unique_ptr
{
public:
	//construct 
	unique_ptr(T *pT = NULL);
	//destroy
	~unique_ptr();
	//not allow copyable
private:
	unique_ptr(const unique_ptr &);
	unique_ptr&operator=(const unique_ptr &);
public:
	//reset 
	void reset(T *p);
	//release the own of the pointer
	T *release();
	//get the pointer
	T *get();

public:
	//convert unique_ptr to bool
	operator bool() const;
	//overload for operator *
	T &operator*();
	//overload for operator ->
	T *operator->();

private:
	//pointer
	T *m_pT;
	//deleter
	Deleter m_deleter;
	//call deleter
	void del();
};

//construct 
template<typename T, typename Deleter>
unique_ptr<T, Deleter>::unique_ptr(T *pT) :m_pT(pT)
{

}

//destroy
template<typename T, typename Deleter>
unique_ptr<T, Deleter>::~unique_ptr()
{
	del();
}

//call deleter
template<typename T, typename Deleter>
void unique_ptr<T, Deleter>::del()
{
	if (*this)
	{
		m_deleter(m_pT);
		m_pT = NULL;
	}
}

//get the pointer
template<typename T, typename Deleter>
T *unique_ptr<T, Deleter>::get()
{
	return m_pT;
}

//reset 
template<typename T, typename Deleter>
void unique_ptr<T, Deleter>::reset(T *p)
{
	del();
	m_pT = p;
}

//release the own of the pointer
template<typename T, typename Deleter>
T *unique_ptr<T, Deleter>::release()
{
	T *p = m_pT;
	m_pT = NULL;
	return p;
}

//convert unique_ptr to bool
template<typename T, typename Deleter>
unique_ptr<T, Deleter>::operator bool()const
{
	return NULL != m_pT;
}

//overload for operator *
template<typename T, typename Deleter>
T &unique_ptr<T, Deleter>::operator*()
{
	assert(*this);
	return *m_pT;
}

//overload for operator ->
template<typename T, typename Deleter>
T *unique_ptr<T, Deleter>::operator->()
{
	return &*(*this);
}

//测试代码
//#include <iostream>
//#include "unique_ptr.h"
//
//struct Obj
//{
//	int m_nMyMem;
//	Obj()
//	{
//		std::cout << "[NEWOBJ][Obj] this =" << this << std::endl;
//	}
//	~Obj()
//	{
//		std::cout << "[DELOBJ][~Obj] this =" << this << std::endl;
//	}
//};
//
//int main(int argc,char *argv[])
//{
//	unique_ptr<Obj> uptr(new Obj);
//	assert(uptr);
//	uptr->m_nMyMem = 100;
//	std::cout << "nMyMem= " << (*uptr).m_nMyMem << std::endl;
//	auto p = uptr.release();
//	if (p)
//	{
//		delete p;
//		p = NULL;
//	}
//	return 0;
//}
//
//输出结果 ==>>  
//	[NEWOBJ][Obj] this =00000000002C7D20
//	nMyMem= 100
//	[DELOBJ][~Obj] this =00000000002C7D20
//
//










/*************************************************************************
    > File Name: mySmartPtr.cpp
    > Author: ye_create
    > Mail: 
    > Created Time: 2015年05月28日 星期四 13时35分00秒
 ************************************************************************/

template <typename T>
class mySmartPtr
{
public:
    // 构造函数 默认为空
    mySmartPtr(): pointer(0), counter(0)
    {
    } 

    // 形参为指针的构造函数
    mySmartPtr(T* p): pointer(0), counter(0){
        *this = p;
    }

    // 复制构造函数
    mySmartPtr(const mySmartPtr<T> &p): 
    pointer(p.pointer), counter(p.counter){
        Increase();
    }

    ~mySmartPtr(){
        Decrease();
    }

    // 指针的赋值操作符，类型不同，不是自赋值
    mySmartPtr operator=(T* p){
        Decrease();
        if (p){
            pointer = p;
            counter = new int(1);
        }
        else {
            pointer = 0;
            counter = 0;
        }
        return *this;
    }

    // 智能指针赋值操作符
    mySmartPtr operator=(mySmartPtr<T> &p){
        // 处理自赋值
        if (this != &p){
            Decrease();
            pointer = p.pointer;
            counter = p.counter;
            Increase();
        }
        return *this;
    }

    operator bool() const{
        return counter != 0;
    }

    // ×操作符重载
    T* operator*() const{
        return this;
    }

    // ->操作符重载
    T* operator->() const{
        return pointer;
    }

    // 返回底层指针
    int getPtrPointer() const{
        return *pointer;
    }

    // 返回引用计数
    int getPtrCounter() const{
        return *counter;
    }

    // 处理父类子类的情况， ptr<derived>不能访问 ptr<based>的内部对象
    template<typename C> friend class mySmartPtr;

    template<typename C> 
    mySmartPtr(const mySmartPtr<C> &p): 
        pointer(p.pointer), counter(p.counter){
            Increase();
        }

    template<typename C>
    mySmartPtr<T> & operator=(const mySmartPtr<C> &p){
        Decrease();
        pointer = p.pointer;
        counter = p.counter;
        Increase();
        return *this;
    }

    // 处理内部使用 dynamic_cast做判断的转换，失败则空指针
    template<typename C>
    mySmartPtr<C> Cast() const{
        C* converted = dynamic_cast<C*>(pointer);
        mySmartPtr<C> result;
        if (converted){
            result.pointer = converted;
            result.counter = counter;
            result.Increase();
        }
        return result;
    }

private:
    T*      pointer;
    int*    counter;

    void Increase(){
        if (counter) 
            ++*counter;
    }

    void Decrease(){
        if (counter && --*counter == 0){
            delete pointer;
            delete counter;
            counter = 0;
            pointer = 0;
        }
    }

};
/*
具体来说，一个智能指针的实现要完成以下功能：

	没有参数构造的时候，初始化为空，即对象和引用计数的两个指针都为0

	使用指针为参数构造时，拥有此指针，在没有智能指针指向它时进行析构

	智能指针复制时，两个智能指针共同拥有内部指针，引用计数同时+1

	智能指针可以使用智能指针或普通指针重新赋值。重载=操作符，对于智能指针赋值，需要考虑是否自赋值，以避免将自身析构了后再重新赋值，而普通指针赋值给智能指针，则不需要考虑自赋值，因为两者本身是两个类型

	获得底层指针的访问，定义getPtrPointer()和getPtrCounter()来分别返回底层指针和引用计数，定义operator bool()来处理智能指针隐式转换为bool的情况

	重载->和×操作符 ，来实现与普通指针相同的指针访问

	需要支持隐式指针类型转换，static_cast不支持而dynamic_cast支持的转换则使用Cast<T2>()成员函数来解决。考虑定义友元类，以防止指向派生类的智能指针有权限访问基类的内部对象；当转型不成功时，返回为空

	如果一个裸指针直接用来创建两个智能指针的话，期望的情况是当两个智能指针析构掉的时候，该指针会被delete两次从而崩溃（这是shared_ptr的特点）

	不处理循环引用（也是shared_ptr的特点），可以通过与weak_ptr协作来打破循环

	暂时不实现deleter机制，即只能传递给mySmartPtr一个参数

	实际上，第8/第9两点往往是使用智能指针出现问题最多的地方，平时使用过程中要多加留意。
*/

//测试用例
/**********************************************
    > File Name: testmySmartPtr.cpp
    > Author: ye_create
    > Mail: 
    > Created Time: 2015年05月28日 星期四 13时03分42秒
 ***********************************************/
/*
#include <iostream>
#include <vector>
#include "mySmartPtr.h"
using namespace std;

class Base             // 定义一个基类
{
public:
    virtual ~Base(){
        cout << "class Base" << endl;
    }
};

class Derived1 : public Base   // 派生类1
{
public:
    ~Derived1(){
        cout << "class Derived1" << endl;
    }
};    

class Derived2 : public Base    // 派生类2
{
public:
    ~Derived2(){
        cout << "class Derived2" << endl;
    }
};


int main()
{   
    int *pi = new int(42);     
    mySmartPtr<int> *hpa(new mySmartPtr<int>(pi));        // 构造函数  
    mySmartPtr<int> *hpb = new mySmartPtr<int>(*hpa);     // 拷贝构造函数  
    mySmartPtr<int>  hpd = *hpa;                          // 拷贝构造函数  
    // 观察引用计数的变化
    cout << hpa->getPtrCounter() << " " << hpb->getPtrCounter() << " "<< hpd.getPtrCounter() << endl;  
    delete hpa;  
    cout << hpd.getPtrCounter() << endl;  
    delete hpb;  
    cout << hpd.getPtrCounter() << endl;

    // 观察派生类向基类的隐式转换
    vector<mySmartPtr<Base> > obj;        // 父类指针vector
    obj.push_back(new Derived1);          // 存入子类
    obj.push_back(new Derived2);

    vector<mySmartPtr<Base> > obj2;  
    obj2.push_back(obj[0]);

    if (obj2[0])
        cout << "Cast Derived1 to Base successd" << endl;
    else
        cout << "Cast Derived1 to Base failed" << endl;

    // 观察不同类型的显式转换
    mySmartPtr<Derived1> d1 = new Derived1;
    mySmartPtr<Base> b = d1;              
    mySmartPtr<Derived2> d2 = b.Cast<Derived2>();  
    // d2是空，因为b指向的是Derived1而不是Derived2
    if (d2)
        cout << "Cast Derived1 to Derived2 successd" << endl;
    else
        cout << "Cast Derived1 to Derived2 failed" << endl;

    return 0;
}
*/










#endif 



























#if 0

4，不要误用auto_ptr
	1）auto_ptr不能共享所有权，即不要让两个auto_ptr指向同一个对象。
	2）auto_ptr不能指向数组，因为auto_ptr在析构的时候只是调用delete,而数组应该要调用delete[]。
	3）auto_ptr只是一种简单的智能指针，如有特殊需求，需要使用其他智能指针，比如share_ptr。
	4）auto_ptr不能作为容器对象，STL容器中的元素经常要支持拷贝，赋值等操作，在这过程中auto_ptr会传递所有权，那么source与sink元素之间就不等价了。

#endif








//实现与STD标准库功能完全相同的智能指针
//包括 AutoPtr SharedPtr WeakePtr UniquePtr 对应数组



#endif


