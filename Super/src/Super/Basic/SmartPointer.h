#ifndef  SMARTPOINT_H
#define SMARTPOINT_H

#include "global.h"

#ifdef UNIT_TEST
BASIC_API void Test_SmartPtr();
#endif



#include <stdlib.h>  //NULLͷ�ļ�





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
	//�ڲ����ü���
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













//ʵ��һ���򵥵�shared_ptr
//	������ݴ����������������˽�shared_ptr��ʵ���ˡ�
//	һ����˵������ָ���ʵ����Ҫ���²��裺
//	1.һ��ģ��ָ��T* ptr��ָ��ʵ�ʵĶ���
//	2.һ�����ô���(����new�����ģ���Ȼ����shared_ptr������в�ͬ�����ô��������¶��delete)��
//	3.����operator*��operator->��ʹ������ָ��һ��ʹ��shared_ptr��
//	4.����copy constructor��ʹ�����ô�����һ��
//	5.����operator=�����ԭ����shared_ptr�Ѿ��ж������������ô�����һ���ж������Ƿ�Ϊ��(�Ƿ����delete)��
//	��Ȼ���µĶ������ô�����һ��
//	 6.��������������ʹ���ô�����һ���ж������Ƿ�Ϊ��(�Ƿ����delete)��
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


 

//�ڵ�λд����û����c++11,Ϊ�˷���д���룬������ſ���ʱ��д�˸�unique_ptr�ļ�ʵ��

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

//���Դ���
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
//������ ==>>  
//	[NEWOBJ][Obj] this =00000000002C7D20
//	nMyMem= 100
//	[DELOBJ][~Obj] this =00000000002C7D20
//
//










/*************************************************************************
    > File Name: mySmartPtr.cpp
    > Author: ye_create
    > Mail: 
    > Created Time: 2015��05��28�� ������ 13ʱ35��00��
 ************************************************************************/

template <typename T>
class mySmartPtr
{
public:
    // ���캯�� Ĭ��Ϊ��
    mySmartPtr(): pointer(0), counter(0)
    {
    } 

    // �β�Ϊָ��Ĺ��캯��
    mySmartPtr(T* p): pointer(0), counter(0){
        *this = p;
    }

    // ���ƹ��캯��
    mySmartPtr(const mySmartPtr<T> &p): 
    pointer(p.pointer), counter(p.counter){
        Increase();
    }

    ~mySmartPtr(){
        Decrease();
    }

    // ָ��ĸ�ֵ�����������Ͳ�ͬ�������Ը�ֵ
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

    // ����ָ�븳ֵ������
    mySmartPtr operator=(mySmartPtr<T> &p){
        // �����Ը�ֵ
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

    // ������������
    T* operator*() const{
        return this;
    }

    // ->����������
    T* operator->() const{
        return pointer;
    }

    // ���صײ�ָ��
    int getPtrPointer() const{
        return *pointer;
    }

    // �������ü���
    int getPtrCounter() const{
        return *counter;
    }

    // ���������������� ptr<derived>���ܷ��� ptr<based>���ڲ�����
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

    // �����ڲ�ʹ�� dynamic_cast���жϵ�ת����ʧ�����ָ��
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
������˵��һ������ָ���ʵ��Ҫ������¹��ܣ�

	û�в��������ʱ�򣬳�ʼ��Ϊ�գ�����������ü���������ָ�붼Ϊ0

	ʹ��ָ��Ϊ��������ʱ��ӵ�д�ָ�룬��û������ָ��ָ����ʱ��������

	����ָ�븴��ʱ����������ָ�빲ͬӵ���ڲ�ָ�룬���ü���ͬʱ+1

	����ָ�����ʹ������ָ�����ָͨ�����¸�ֵ������=����������������ָ�븳ֵ����Ҫ�����Ƿ��Ը�ֵ���Ա��⽫���������˺������¸�ֵ������ָͨ�븳ֵ������ָ�룬����Ҫ�����Ը�ֵ����Ϊ���߱�������������

	��õײ�ָ��ķ��ʣ�����getPtrPointer()��getPtrCounter()���ֱ𷵻صײ�ָ������ü���������operator bool()����������ָ����ʽת��Ϊbool�����

	����->�͡������� ����ʵ������ָͨ����ͬ��ָ�����

	��Ҫ֧����ʽָ������ת����static_cast��֧�ֶ�dynamic_cast֧�ֵ�ת����ʹ��Cast<T2>()��Ա��������������Ƕ�����Ԫ�࣬�Է�ָֹ�������������ָ����Ȩ�޷��ʻ�����ڲ����󣻵�ת�Ͳ��ɹ�ʱ������Ϊ��

	���һ����ָ��ֱ������������������ָ��Ļ�������������ǵ���������ָ����������ʱ�򣬸�ָ��ᱻdelete���δӶ�����������shared_ptr���ص㣩

	������ѭ�����ã�Ҳ��shared_ptr���ص㣩������ͨ����weak_ptrЭ��������ѭ��

	��ʱ��ʵ��deleter���ƣ���ֻ�ܴ��ݸ�mySmartPtrһ������

	ʵ���ϣ���8/��9����������ʹ������ָ������������ĵط���ƽʱʹ�ù�����Ҫ������⡣
*/

//��������
/**********************************************
    > File Name: testmySmartPtr.cpp
    > Author: ye_create
    > Mail: 
    > Created Time: 2015��05��28�� ������ 13ʱ03��42��
 ***********************************************/
/*
#include <iostream>
#include <vector>
#include "mySmartPtr.h"
using namespace std;

class Base             // ����һ������
{
public:
    virtual ~Base(){
        cout << "class Base" << endl;
    }
};

class Derived1 : public Base   // ������1
{
public:
    ~Derived1(){
        cout << "class Derived1" << endl;
    }
};    

class Derived2 : public Base    // ������2
{
public:
    ~Derived2(){
        cout << "class Derived2" << endl;
    }
};


int main()
{   
    int *pi = new int(42);     
    mySmartPtr<int> *hpa(new mySmartPtr<int>(pi));        // ���캯��  
    mySmartPtr<int> *hpb = new mySmartPtr<int>(*hpa);     // �������캯��  
    mySmartPtr<int>  hpd = *hpa;                          // �������캯��  
    // �۲����ü����ı仯
    cout << hpa->getPtrCounter() << " " << hpb->getPtrCounter() << " "<< hpd.getPtrCounter() << endl;  
    delete hpa;  
    cout << hpd.getPtrCounter() << endl;  
    delete hpb;  
    cout << hpd.getPtrCounter() << endl;

    // �۲���������������ʽת��
    vector<mySmartPtr<Base> > obj;        // ����ָ��vector
    obj.push_back(new Derived1);          // ��������
    obj.push_back(new Derived2);

    vector<mySmartPtr<Base> > obj2;  
    obj2.push_back(obj[0]);

    if (obj2[0])
        cout << "Cast Derived1 to Base successd" << endl;
    else
        cout << "Cast Derived1 to Base failed" << endl;

    // �۲첻ͬ���͵���ʽת��
    mySmartPtr<Derived1> d1 = new Derived1;
    mySmartPtr<Base> b = d1;              
    mySmartPtr<Derived2> d2 = b.Cast<Derived2>();  
    // d2�ǿգ���Ϊbָ�����Derived1������Derived2
    if (d2)
        cout << "Cast Derived1 to Derived2 successd" << endl;
    else
        cout << "Cast Derived1 to Derived2 failed" << endl;

    return 0;
}
*/










#endif 



























#if 0

4����Ҫ����auto_ptr
	1��auto_ptr���ܹ�������Ȩ������Ҫ������auto_ptrָ��ͬһ������
	2��auto_ptr����ָ�����飬��Ϊauto_ptr��������ʱ��ֻ�ǵ���delete,������Ӧ��Ҫ����delete[]��
	3��auto_ptrֻ��һ�ּ򵥵�����ָ�룬��������������Ҫʹ����������ָ�룬����share_ptr��
	4��auto_ptr������Ϊ��������STL�����е�Ԫ�ؾ���Ҫ֧�ֿ�������ֵ�Ȳ��������������auto_ptr�ᴫ������Ȩ����ôsource��sinkԪ��֮��Ͳ��ȼ��ˡ�

#endif








//ʵ����STD��׼�⹦����ȫ��ͬ������ָ��
//���� AutoPtr SharedPtr WeakePtr UniquePtr ��Ӧ����



#endif


