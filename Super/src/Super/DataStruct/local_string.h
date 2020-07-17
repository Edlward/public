
/*
����ʵ����c++ STL�е�basic_stringģ���࣬��Ȼ��ͨ��typedefҲ��ʵ����string���wstring�ࡣ
����ƪ����ʵ�ִ������õ��˱�׼���char_traitsģ���࣬�����Լ�Ҳʵ����

char_traitsģ���࣬Ϊ�˻�д��һƪ���ͣ����ҵĲ������ܹ��ҵ�������Ĵ���ȫȻ�ܹ����������
basic_string���ʵ���У���Ȼ����ϸ�ӵ�������Ҫ�Լ��޸ģ��Ժ���basic_string����ص�ȫ��ģ��
�������ݣ���operator >> ģ�庯�������ء�

�����y�ԡ��������ʵ���Ч���ϲ����׼�⣨��vs2012�ȶԣ����������ӻ������������Ҿ���
���õ������ԡ��ܹ��Դ���������Լ��ľ���汾��string��

���ˡ��ϻ�����˵�ˣ������룡
���������ָ�̣�����

���ļ��ҽ�������Ϊlocal_string.h��������һ�ߵ�ϰ�ߣ���װ�����ֿռ�mystd�С�

����Ҫ�y�Դ��룬ֱ�Ӹ������µĴ�����ܹ��ˣ�����һ���ļ��

��׼���еĹ��ܡ���������϶��У�



*���꡶STLԴ�����������������дSTL�ĳ嶯�����basic_string��ʵ�ֻ���������
��׼���еĹ��ܣ�����������У���������Ҫ���ܹ��ڴ˻�������չ���ܡ���
��
������з���bug����������ϵ����ʹ���벻�ϵ���ã�����
*��ҽ���������лл!
* ���䣺sunkang2101024@foxmail.com
*/


// ������ļ�����Ϊ "local_string.h"



// vs2012 ����ͨ��
#pragma once 
#ifndef MYSTD_STRING_H_EX
#define MYSTD_STRING_H_EX

#include<cassert>       // assert
#include<cstddef>      // std::size_t,std::ptrdiff_t
#include<memory>     //std::allocator 
#include<cstdlib>       //rand()
#include<exception>  //std::out_of_range 
#include<iostream>    //basic_istream, basic_ostream 



#define MYSTD_BEGIN namespace mystd {
#define MYSTD_END   }
  

// mystd 
#ifdef __cplusplus
MYSTD_BEGIN 
/////////////
//����L�ʵ�����
template<class random_access_iterator, class Value_type, class Reference,class Difference_type>
 class reverse_random_access_iterator{
 public:
	 typedef Value_type value_type;
	 typedef value_type* pointer;
	 typedef Reference reference;
	 typedef Difference_type difference_type;
	 typedef std::size_t size_type;
	 typedef std::random_access_iterator_tag reverse_iterator_category;
 public:
	 random_access_iterator iterator;
	 typedef reverse_random_access_iterator self;
 public:
	 reverse_random_access_iterator()
	 {
		 //default constructor 
	 }
	 explicit reverse_random_access_iterator(const random_access_iterator& tmp):iterator(tmp)
	 {
		 //constructor  
	 }
	 reverse_random_access_iterator(const self& tmp):iterator(tmp.iterator)
	 {
		 // copy constructor 
	 }
	 template<class random_access_iterator, class Value_type, class Reference,class Difference_type>
	 reverse_random_access_iterator(const reverse_random_access_iterator<random_access_iterator,Value_type,Reference,Difference_type>& obj)
		 :iterator(obj.iterator)
	 {
		 // �˹��캯����ҪΪ�˽���ת��֮��,
		 //reverse_iteratorת��Ϊ const_reverse_iterator 
	 }
	 self& operator = (const self& tmp)  // �ܹ������塣ֱ��ʹ��Ĭ�ϵĺ���
	 {
		 iterator = tmp.iterator;
		 return *this;
	 }
	 ~reverse_random_access_iterator()
	 {
		 // do nothing 
	 }

	 self& operator ++ () 
	 {
		 --iterator;
		 return *this;
	 }
	 self operator ++ (int)
	 {
		 self tmp(*this);
		 ++*this;
		 return tmp;
	 }
	 self& operator -- ()
	 {
		 ++iterator;
		 return *this;
	 }
	 self operator -- (int)
	 {
		 self tmp(*this);
		 --*this;
		 return tmp;
	 }

	 self operator + (difference_type n) const 
	 {
		 return self(iterator - n);
	 }
	 self operator - (difference_type n) const 
	 {
		 return *this + (-n);
	 }
	 self& operator += (difference_type n) 
	 {
		 iterator -= n;
		 return *this;
	 }
	 self& operator -= (difference_type n) 
	 {
		 iterator += n;
		 return *this;
	 }
	 reference operator * () const 
	 {
		 return *(iterator - 1);
	 }
	 bool operator == (const self& tmp) const 
	 {
		 return iterator == tmp.iterator;
	 }
	 bool operator != (const self& tmp) const 
	 {
		 return !(*this == tmp);
	 }
	 bool operator < (const self& tmp) const 
	 {
		 return tmp.iterator < iterator;
	 }
	 bool operator <= (const self& tmp) const 
	 {
		  return tmp.iterator <= iterator;
	 }
	 bool operator > (const self& tmp) const 
	 {
		 return iterator < tmp.iterator;
	 }
	 bool operator >= (const self& tmp) const 
	 {
		  return iterator <= tmp.iterator;
	 }
	 random_access_iterator base() const 
	 {
		 return iterator;
	 }
	 friend difference_type operator - (const self& tmp_1,const self& tmp_2)
	 {
		 return tmp_2.iterator - tmp_1.iterator;
	 }
 };

 template<class T, class Reference,class Difference_type>
 class random_access_iterator{  // ��������L�ʵ�����
 public:
	 typedef T value_type;
	 typedef value_type *pointer;
	 typedef Reference reference;
	 typedef Difference_type difference_type;
	 typedef std::size_t size_type;
	 typedef std::random_access_iterator_tag iterator_category;
 public:
	 pointer ptr;
	 typedef random_access_iterator self;
 public:
	 random_access_iterator()
	 {
	 }
	 explicit random_access_iterator(pointer tmp):ptr(tmp)
	 { 
	 }
	 random_access_iterator(const self& tmp):ptr(tmp.ptr)
	 {
	 }
	 template<class T, class Reference,class Difference_type>
	 random_access_iterator(const random_access_iterator<T,Reference,Difference_type>& obj)
		 :ptr(obj.ptr)
	 {
		 // �˹��캯����ҪΪ�˽���ת��֮��,
		 //iteratorת��Ϊ const_iterator 
	 }
	 self& operator = (const self& tmp)
	 {
		 ptr = tmp.ptr;
		 return *this;
	 }
	 ~random_access_iterator()
	 {
	 }

	 self& operator ++ ()
	 {
		 ++ptr;
		 return *this;
	 }
	 self operator ++ (int)
	 {
		 self tmp(*this);
		 ++*this;
		 return tmp;
	 }
	 self& operator -- ()
	 {
		 --ptr;
		 return *this;
	 }
	 self operator -- (int)
	 {
		 self tmp(*this);
		 --*this;
		 return tmp;
	 }
	 reference operator * () const // �㹻�����ɶ�
	 {
		 return *ptr;
	 }
	 bool operator == (const self& tmp) const 
	 {
		 return ptr == tmp.ptr;
	 }
	 bool operator != (const self& tmp) const 
	 {
		 return !(*this == tmp);
	 }

	 bool operator < (const self& tmp) const 
	 {
		 return ptr < tmp.ptr;
	 }
	 bool operator <= (const self& tmp) const 
	 {
		 return ptr <= tmp.ptr;
	 }
	 bool operator > (const self& tmp) const 
	 {
		 return tmp.ptr < ptr;
	 }
	 bool operator >= (const self& tmp) const 
	 {
		 return tmp.ptr <= ptr;
	 }
	 self operator + (difference_type n) const 
	 {
		 return self(ptr + n);
	 }
	 self operator - (difference_type n) const 
	 {
		 return *this + (-n);
	 }
	 self& operator += (difference_type n)
	 {
		 ptr += n;
		 return *this;
	 }
	 self& operator -= (difference_type n)
	 {
		 ptr -= n;
		 return *this;
	 }
	 friend difference_type operator - (const self& tmp_1,const self& tmp_2)
 	 {
		 return tmp_1.ptr - tmp_2.ptr;
	 }
 };

////////////////////////
 //����ƪ���������char_traitsģ�����õ��Ǳ�׼��ģ�
 //�ҵĲ�������һƪ�����ǹ���char_traitsģ�����ʵ�ֵģ�ȫȻ�ܹ��������


 //
template<class charT,class traits_type = std::char_traits<charT>,class Alloc = std::allocator<charT> >
class basic_string
{
public:
	typedef mystd::random_access_iterator<charT,charT&,std::ptrdiff_t> iterator;
	typedef mystd::random_access_iterator<const charT,const charT&,std::ptrdiff_t> const_iterator;
	typedef mystd::reverse_random_access_iterator<iterator,charT,charT&,std::ptrdiff_t> reverse_iterator;
	typedef mystd::reverse_random_access_iterator<const_iterator,const charT,const charT&,std::ptrdiff_t> const_reverse_iterator;
	typedef typename iterator::value_type value_type;
	typedef typename iterator::reference reference;
	typedef typename iterator::pointer pointer;
	typedef typename iterator::size_type size_type;
	typedef typename iterator::difference_type difference_type;
	typedef typename iterator::iterator_category iterator_category;
	typedef typename const_iterator::reference const_reference;
	typedef typename const_iterator::pointer const_pointer;
	typedef Alloc allocator_type;
public: 
	static const size_type npos = size_type(-1);
private:  
	pointer start,finish,end_of_storage; //���ݳ�Ա
	typedef basic_string self;  //˽������
	allocator_type alloc;  // ������
private: // �ڲ�ʹ�ú���
	size_type alloc_capacity(size_type old_size) const  // �������亯��
	{ //���������Ҫ��Ϊ�˷���Ķ����䷽����
	//�ܹ�������Ҫ�Ķ�������Ҫȷ������ֵ����old_size���Դ˱�֤capacity()�ǿգ���ʵ�ֿ�֪��
	// ��ʾ����c_str()��ʵ���йء�
		if(old_size < 10)
			return 10;  //wstring 
		else if(old_size < 100)
			return old_size * 2;
		else if(old_size < 1024)
			return old_size + old_size / 10;
		else
			return old_size + rand() % 1024 + 1;
		//����ֵ�ô���old_size
	}
	void fill_n(pointer first,size_type n,value_type val)
	{
		assert(first != 0);
		while(n--)
			*first++ = val;
	}
	void fill(pointer first,pointer last,value_type val)
	{
		assert(first != 0 && last != 0);
		while(first != last)
			*first++ = val;
	}
	
	iterator insert_aux(size_type pos,const self& str)
	{ //����insert��غ���
		assert(pos <= size());
		if(str.empty())
			return iterator(start+pos);
		if(size()+str.size() > capacity()) // ����this�Ƿ����&str
		{
			size_type str_size = str.size(); // 
			size_type old_size = size();
			size_type capacity_size = alloc_capacity(size()+str_size);
			pointer new_start = alloc.allocate(capacity_size);
			traits_type::copy(new_start,start,pos);
			traits_type::copy(new_start+pos,str.start,str_size);
			traits_type::copy(new_start+pos+str_size,start+pos,old_size-pos);
			alloc.deallocate(start,capacity());
			start = new_start;
			finish = start + old_size+ + str_size; // 
			end_of_storage = start + capacity_size;
			return iterator(start+pos);
		}
		else if(this != &str)
		{
			traits_type::move(start+pos+str.size(),start+pos,size()-pos);
			traits_type::copy(start+pos,str.start,str.size());
			finish += str.size();
			return iterator(start+pos);
		}
		else
		{
           size_type len_left = pos;
		   size_type len_right = size() - pos;
		   traits_type::copy(start+len_left*2+len_right,start+len_left,len_right);
		   traits_type::copy(start+len_left,start,len_left);
		   traits_type::copy(start+len_left*2,start+len_left*2+len_right,len_right);
		   finish += str.size();
		   return iterator(start+len_left);
		}
	}
	iterator erase_aux(size_type first,size_type last)
	{ //����erase����
		assert(first <= last  && last <= size());
		if(first > size())
			throw std::out_of_range("invalid string position");
		if(first == last)
			return iterator(start+first);
		traits_type::copy(start+first,start+last,size()-last);
		finish -= last - first;
		return iterator(start+first);
	}
	void empty_init()
	{  //���ڳ�ʼ��
		size_type capacity_size = alloc_capacity(1);
		start = alloc.allocate(capacity_size); //����һ���Ŀռ�
		finish = start;
		end_of_storage = start + capacity_size;
	}
	void swap(pointer& x,pointer& y)
	{
		assert(x != 0 && y != 0);
		pointer temp = x;
		x = y;
		y = temp;
	}
	bool is_inside(const self& str,value_type val) const 
	{ // ����Ĳ��ҹ��ܵĺ������õ�����find_first_of()����
		size_type n = str.size();
		for(size_type i = 0; i < n; ++i)
			if(traits_type::eq(str[i],val))
				return true;
		return false;
	}
public:
	basic_string():start(0),finish(0),end_of_storage(0)
	{
		assert(sizeof(value_type) <= sizeof(wchar_t));
		empty_init();
	}
	basic_string(size_type n,value_type val):start(0),finish(0),end_of_storage(0)
	{
		assert(sizeof(value_type) <= sizeof(wchar_t));
		size_type capacity_size = alloc_capacity(n);
		start = alloc.allocate(capacity_size);
		finish = start + n;
		end_of_storage = start + capacity_size;
		fill_n(start,n,val);
	}
	basic_string(const self& str,size_type pos,size_type len = npos)
	{
		assert(sizeof(value_type) <= sizeof(wchar_t));
		assert(pos <= str.size());
		if(str.empty())
			empty_init();
		else
		{
		if(len == npos || len > str.size()-pos)
			len = str.size() - pos;
		size_type capacity_size = alloc_capacity(len);
		start = alloc.allocate(capacity_size);
		traits_type::copy(start,str.start+pos,len);
		finish = start + len;
		end_of_storage = start + capacity_size;
		}
	}
	//������캯��������explicitkeyword����������õ��˹��캯������ʽת��
     basic_string(const_pointer str):start(0),finish(0),end_of_storage(0)
	{
		assert(sizeof(value_type) <= sizeof(wchar_t) && str != 0);
		size_type str_size = traits_type::length(str);
		size_type capacity_size = alloc_capacity(str_size);
		start = alloc.allocate(capacity_size);
		traits_type::copy(start,str,str_size);
		finish = start + str_size;
		end_of_storage = start + capacity_size;
	}
	basic_string(const_pointer str,size_type n):start(0),finish(0),end_of_storage(0)
	{
      assert(sizeof(value_type) <= sizeof(wchar_t)  && str != 0);
	  size_type len = traits_type::length(str);
	  //��׼��û�жԳ��Ƚ��е���(�y��vs2012��ִ�н����֪),������ΪӦ�ü��ϡ������и��õ��ݴ���
	  //��Ȼ����str�ǳ����ʱ��Ч���ϻ��е��ۿۣ����㳤�ȵ�ʱ�仨���ϣ�
	  if(n > len)  
		  n = len;
	  size_type capacity_size = alloc_capacity(n);
	  start = alloc.allocate(capacity_size);
	  traits_type::copy(start,str,n);
	  finish = start + n;
	  end_of_storage = start + capacity_size;
	}
	template <class InputIterator>
	basic_string(InputIterator first,InputIterator last):start(0),finish(0),end_of_storage(0)
	{
		assert(sizeof(value_type) <= sizeof(wchar_t));
		size_type len = 0;
		InputIterator new_first = first;
		while(new_first != last) //�ú������y��ʱ�����һ������
			++len, ++new_first;
		size_type capacity_size = alloc_capacity(len);
		start = alloc.allocate(capacity_size);
		finish = start + len;
		/*while(first != last)  
			*start++ = *first++;*/  
		while(last != first)  //�ú������y��ʱ�����һ������
			*--finish = *--last;
		finish = start + len; // ��λ
		end_of_storage = start + capacity_size;
	}
	basic_string(const self& tmp):start(0),finish(0),end_of_storage(0)
	{
		assert(sizeof(value_type) <= sizeof(wchar_t));
		start = alloc.allocate(tmp.capacity());
		finish = start + tmp.size();
		end_of_storage = start + tmp.capacity();
		traits_type::copy(start,tmp.start,tmp.size());
	}

	self& operator = (const self& tmp)
	{
		if(this == &tmp) // ��ֹ������
			return *this;
		clear();
		if(tmp.empty())
			return *this;
		if(capacity() >= tmp.capacity())
		{
			traits_type::copy(start,tmp.start,tmp.size());
			finish = start + tmp.size();
		}
		else
		{
			size_type capacity_size = alloc_capacity(tmp.size());
			alloc.deallocate(start,capacity()); // �ͷ�ԭ�пռ�
			start = alloc.allocate(capacity_size);
			traits_type::copy(start,tmp.start,tmp.size());
			finish = start + tmp.size();
			end_of_storage = start + capacity_size;
		}
		return *this;
	}
	self& operator = (value_type val)
	{
		clear(); // ���ͷ��ڴ�,�������֪
		insert_aux(0,self(1,val));
		return *this;
	}
	~basic_string()
	{
		//alloc.deallocate(start,capacity());
		//����capacity()ʵ���ϵ�Ե�ʣ�������end_of_storage - start����capacity�����֪��
		//��Ȼ���ڶ���������ʵûʵ�á�����дһ�����ֶ���
		alloc.deallocate(start,end_of_storage - start);
	}
public:
	size_type size() const throw() 
	{ //���ձ�׼�ĵ���Ҫ�󣬲��׳��쳣,����ͬ��ɵ�,����׸����
		return finish - start;
	}
	size_type length() const throw()
	{
		return size();
	}
	size_type max_size() const  throw()
	{
		return size_type(-1) / sizeof(value_type) - 1;
	}
	size_type capacity() const throw()
	{ //����ļ��㱾����Ӧ�ü� 1 �ģ�������������Ϊ��ȷ������һ���ַ��Ŀռ�(�������ַ���,������
		 // C����ַ�����ת��,д����ַ������ڳ�Ա����c_str()
		return end_of_storage - start - 1;
	}
	bool empty() const throw()
	{
		return start == finish;
	}
	iterator begin() throw()
	{
		return iterator(start);
	}
	const_iterator begin() const  throw()
	{
		return const_iterator(start);
	}
	iterator end()  throw()
	{
		return iterator(finish);
	}
	const_iterator end() const throw()
	{
		return const_iterator(finish);
	}
	reverse_iterator rbegin()  throw()
	{
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const throw()
	{
		return const_reverse_iterator(end());
	}
	reverse_iterator rend()  throw()
	{
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const  throw()
	{
		return const_reverse_iterator(begin());
	}
	reference operator [] (size_type n)  throw()
	{
		assert(n < size());
		return *(start + n);
	}
	const_reference operator [] (size_type n) const throw()
	{
		assert(n < size());
		return *(start + n);
	}
	reference at(size_type n) 
	{
		if(n < size())
			return *(start + n);
		else
			throw std::out_of_range("invalid string position");
	}
	const_reference at(size_type n) const 
	{
		if(n < size())
			return *(start + n);
		else
			throw std::out_of_range("invalid string position");
	}
	allocator_type get_allocator() const throw()
	{
		return allocator_type();
	}
	void reserve(size_type n = 0)
	{
		if(n <= capacity())
			;
		else
		{
			size_type capacity_size = alloc_capacity(n);
			size_type old_size = size();
			pointer new_start = alloc.allocate(capacity_size);
			traits_type::copy(new_start,start,size());
			alloc.deallocate(start,capacity());
			start = new_start;
			finish = start + old_size;
			end_of_storage = start + capacity_size;
		}
	}
	void clear() 
	{
		finish = start; //�����ͷ��ڴ�
	}
	self substr(size_type pos = 0,size_type len = npos) const
	{
		assert(pos <= size());
		return self(*this,pos,len); // ��ʽ���ù��캯��
	}
    self& insert(size_type pos,const self& str)
    {
		assert(pos <= size());
		insert_aux(pos,str);
		return *this;
    }
	const_pointer c_str() const  throw()
	{ //���캯����֤capacity() != 0,����˵start,finish,end_of_storage��Ϊ��ָ��
		//��Ȼ�������ϸ��ʵ���йأ������ҵ�һ��ʵ��
		*finish = 0;  // ת����C�����ַ���
		return start;
	}
	const_pointer data() const throw()
	{ 
	    *finish = 0; 
		// Ҳ�ܹ�ֱ�ӷ���start,��׼�ĵ�û��ǿ��Ҫ��ΪC����ַ���
		return start;
	}
    self& insert(size_type pos,const self& str,size_type subpos,size_type sublen)
	{
		assert(pos <= size());
		insert_aux(pos,str.substr(subpos,sublen));
		return *this;
	}
    self& insert(size_type pos,const_pointer str)
	{
		assert(pos <= size() && str != 0);
		insert_aux(pos,self(str));
		return *this;
	}
    self& insert(size_type pos,const_pointer str,size_type n)
	{
		assert(pos <= size() && str != 0);
		insert_aux(pos,self(str,n));
		return *this;
	}

    self& insert(size_type pos,size_type n,value_type val)
	{
		assert(pos <= size());
		insert_aux(pos,self(n,val));
		return *this;
	}
    void insert(iterator itr,size_type n,value_type val)
	{
		assert(itr >= begin() && itr <= end());
		insert_aux(itr-begin(),self(n,val));
	}
    iterator insert(iterator itr,value_type val)
	{
		assert(itr >= begin() && itr <= end());
		return insert_aux(itr-begin(),self(1,val));
	}
    template <class InputIterator>
    void insert(iterator itr,InputIterator first,InputIterator last)
	{
		assert(itr >= begin() && itr <= end());
		insert_aux(itr-begin(),self(first,last));
	}
	self& erase(size_type pos = 0,size_type len = npos)
	{
		assert(pos <= size());
		if(pos > size())
			throw std::out_of_range("invalid string position");
		if(len == npos || len > size()-pos)
			len = size() - pos;
		erase_aux(pos,pos+len);
		return *this;
	}

    iterator erase(iterator itr)
	{
		assert(itr >= begin() && itr < end());
		return erase_aux(itr-begin(),itr-begin()+1);
	}
    iterator erase(iterator first,iterator last)
	{
		assert(first >= begin() && first <= last && last <= end());
		return erase_aux(first-begin(),last-begin());
	}
	void push_back(value_type val)
	{
		insert_aux(size(),self(1,val));
	}
	self& append(const self& str)
	{
		return *this += str;
	}

    self& append(const self& str,size_type subpos,size_type sublen)
	{
		return *this += str.substr(subpos,sublen);
	}

    self& append(const_pointer str)
	{
		assert(str != 0);
		return *this += str;
	}

    self& append(const_pointer str,size_type n)
	{
		assert(str != 0);
		return *this += self(str,n);
	}

    self& append(size_type n,value_type val)
	{
		return *this += self(n,val);
	}

   template <class InputIterator>
   self& append(InputIterator first,InputIterator last)
   {
	   return *this += self(first,last);
   }
   self& assign(const self& str)
   {
	   return *this = str;
   }

   self& assign(const self& str,size_type subpos,size_type sublen)
   {
	  return *this = self(str,subpos,sublen);
   }

   self& assign(const_pointer str)
   {
	   assert(str != 0);
	   return *this = self(str);
   }

   self& assign(const_pointer str,size_type n)
   {
	   assert(str != 0);
	   return *this = self(str,n);
   }
   self& assign(size_type n,value_type val)
   {
	   return *this = self(n,val);
   }

   template <class InputIterator>
   self& assign(InputIterator first,InputIterator last)
   {
	  return *this = self(first,last);
   }
   void resize(size_type n)
   {
	   resize(n,0); //ת��
   }
   void resize(size_type n,value_type val)
   {
	   if(n <= size())
		   finish -= size() - n;
	   else if(n <= capacity())
	   {
		   fill(finish,finish+n,val);
		   finish += n - size();
	   }
 	   else
	   {
		   size_type capacity_size = alloc_capacity(n);
		   pointer new_start = alloc.allocate(capacity_size);
		   traits_type::copy(new_start,start,size());
		   fill(new_start+size(),new_start+n,val);
		   alloc.deallocate(start,capacity());
		   start = new_start;
		   finish = start + n;
		   end_of_storage = start + capacity_size;
	   }
   }
   void swap(self& str)
   {
	   if(this == &str)
		   return ;
	   swap(start,str.start);
	   swap(finish,str.finish);
	   swap(end_of_storage,str.end_of_storage);
   }

   self& replace(size_type pos,size_type len,const self& str)
   {
	   assert(pos <= size());
	   if(pos > size())
		   throw std::out_of_range("invalid string position"); 
	   // �����replace���غ����Ͳ���д�쳣�ˣ����ڶ���ֱ�ӻ��ӵص��õ��������
	   if(len > size() - pos)
		   len = size() - pos;
	   if(size() + str.size() - len > capacity())
	   {    
		   size_type new_size = size()+str.size()-len;
		   size_type capacity_size = alloc_capacity(new_size);
		   pointer new_start = alloc.allocate(capacity_size);
		   traits_type::copy(new_start,start,pos);
		   traits_type::copy(new_start+pos,str.start,str.size());
		   traits_type::copy(new_start+pos+str.size(),start+pos+len,size()-pos-len);
		   alloc.deallocate(start,capacity());
		   start = new_start;
		   finish = start + new_size;
		   end_of_storage = start + capacity_size;
	   }
	   else if(this != &str)  //���費��������滻
	   {
		   if(str.size() <= len)
		   {
			   traits_type::copy(start+pos,str.start,str.size());
			   erase(pos+str.size(),len-str.size()); // ����len - str.size()
			   // finish �Ѿ�����
		   }
		   else // size() + str.size() - len <= capacity() 
		   {
			   traits_type::move(start+pos+str.size(),start+pos+len,size()-pos-len);
			   traits_type::copy(start+pos,str.start,str.size());
			   finish += str.size() - len;
		   }
	   }
	   else
		   return replace(pos,len,self(str)); //�ݹ���ã���β����������ˡ�
	  return *this;
   }
 
   self& replace(iterator itr_1,iterator itr_2,const self& str)
   {
	   assert(itr_1 >= begin() && itr_2 <= end());
	   return replace(itr_1-begin(),itr_2-itr_1,str); // ת��
   }
   self& replace(size_type pos,size_type len,const self& str,
                       size_type subpos,size_type sublen)
   {
	   assert(pos <= size());
	   return replace(pos,len,str.substr(subpos,sublen));
   }

   self& replace(size_type pos,size_type len,const_pointer str)
   {
	   assert(pos <= size() && str != 0);
	   return replace(pos,len,self(str));
   }
   self& replace(iterator itr_1,iterator itr_2,const_pointer str)
   {
	   assert(itr_1 >= begin() && itr_2 <= end());
	   return replace(itr_1,itr_2,self(str)); //ת��
   }
   self& replace(size_type pos,size_type len,const_pointer str,size_type n)
   {
	   assert(pos <= size() && str != 0);
	   return replace(pos,len,self(str,n));
   }
   self& replace(iterator itr_1,iterator itr_2,const_pointer str,size_type n)
   {
	   assert(itr_1 >= begin() && itr_2 <= end() && str != 0);
	   return replace(itr_1,itr_2,self(str,n));
   }

   self& replace(size_type pos,size_type len,size_type n,value_type val)
   {
	   assert(pos <= size());
	   return replace(pos,len,self(n,val));
   }
   self& replace(iterator itr_1,iterator itr_2,size_type n,value_type val)
   {
	   assert(itr_1 >= begin() && itr_2 <= end());
	   return replace(itr_1,itr_2,self(n,val));
   }

  template <class InputIterator>
  self& replace(iterator itr_1,iterator itr_2,InputIterator first,InputIterator last)
  {
	  assert(itr_1 >= begin() && itr_2 <= end());
	  return replace(itr_1,itr_2,self(first,last));
  }
  int compare(const self& str) const
  { //�����compare���غ�������ӵ��ô˺���
	  if(this == &str)
		  return 0;
	  size_type num = size() >= str.size() ? size() : str.size();
	  return traits_type::compare(c_str(),str.c_str(),num); //��Ҫת��ΪC����ַ���
  }
  int compare(size_type pos,size_type len,const self& str) const
  {
	  assert(pos <= size());
	  if(pos > size()) // ��������غ�������д�쳣�ˡ�����д��
		  throw std::out_of_range("invalid string position");
	  //size_type num = len >= str.size() ? len : str.size();
	  //return traits_type::compare(start+pos,str.start,num);
	  return substr(pos,len).compare(str);  //ת��
  }
  int compare(size_type pos,size_type len,const self& str,
	  size_type subpos,size_type sublen) const
  {
	  assert(pos <= size());
	  return compare(pos,len,self(str,subpos,sublen)); //ת��
  }

  int compare(const_pointer str) const
  {
	  assert(str != 0);
	  return compare(self(str));
  }
  int compare(size_type pos,size_type len,const_pointer str) const
  {
	  assert(pos <= size() && str != 0);
	  return compare(pos,len,self(str));
  }

  int compare(size_type pos,size_type len,const_pointer str,size_type n) const
  {
	  assert(pos <= size() && str != 0);
	  return compare(pos,len,self(str,n));
  }
  size_type copy(pointer str,size_type len,size_type pos = 0) const
  {
	  assert(pos <= size() && str != 0);
	  if(len + pos > size())
		  len = size() - pos; // ����һ�³���
	  traits_type::copy(str,start+pos,len);
	  return len;
  }
  size_type find(const self& str,size_type pos = 0) const throw()
  { //posΪ�����_ʼ��������ѧ�����ʾΪ[pos,size() )
	  assert(pos <= size());
	  if(empty())
		  return npos;
	  size_type len_str = str.size();
	  size_type len = size() - pos; 
	  if(len < len_str)
		  return npos;
	  size_type end_pos = size() - len_str; // ������һ����Ч���ȣ����������ù�
	  while(pos <= end_pos)
	  {
		  if(compare(pos,len_str,str) == 0) // ת�����صĳ�Ա����
			  return pos;
		  else	
			  ++pos;
	  }
	  return npos;
  }

   size_type find(const_pointer str,size_type pos = 0) const throw()
   {
	   assert(pos <= size() && str != 0);
	   if(empty())
		  return npos;
	   return find(self(str),pos); // ת�����صĳ�Ա����
   }

   size_type find(const_pointer str,size_type pos,size_type n) const throw()
   { //str�е�n���ַ�,ע�� n ����ϸ��ָ
	   assert(pos <= size() && str != 0);
	   if(empty())
		  return npos;
	   return find(self(str,n),pos);// ת�����صĳ�Ա����
   }

   size_type find(value_type val,size_type pos = 0) const throw()
   {
	   assert(pos <= size());
	   if(empty())
		  return npos;
	   return find(self(1,val),pos); // ת�����صĳ�Ա����
   }

   size_type rfind(const self& str,size_type pos = npos) const throw()
   {  // posΪ�����_ʼ��������ѧ�����ʾΪ[0,pos]
	   	if(empty())
		   return npos;
	   if(pos == npos || pos >= size()) //����pos == npos���ƶϲ����Ƕ��һ��
		   pos = size() - 1;
	   if(size() < str.size())
		   return npos;
	   size_type len_str = str.size();
	   if(pos > size() - len_str)
		   pos = size() - len_str;  // ����λ�ã�������û�õ�����
	   while(pos >= 0 && pos != size_type(-1))
	   {
		   if(compare(pos,len_str,str) == 0) // ת����Ա����
			   return pos;
		   else
		   --pos;
	   }
	   return npos;
   }

   size_type rfind(const_pointer str,size_type pos = npos) const throw()
   {
	   assert(str != 0);
	   if(empty())
		   return npos;
	   if(pos == npos || pos >= size()) //����pos == npos���ƶϲ����Ƕ��һ��
	   pos = size() - 1;
	   return rfind(self(str),pos); // ת�����صĳ�Ա����
   }

   size_type rfind(const_pointer str,size_type pos,size_type n) const throw()
   { // nΪstr��Ҫ�����ַ��ĸ���,���ڹ��캯��ʵ���ϵĲ��죬�ҵ�nֵ�ڵ��ù��캯��ʱ���е�����
	 // ������Ϊ����ǿ�ݴ��ԣ����ǜy��vs2012��֪��ʵ����û�д˹��ܣ�
	 //��ˡ��ñ�׼��ʱ���������û�ʹ����ȷ��nֵ�������ò���Ԥ�ڵĽ����
	 // ���ҵ����ʵ�֣���nֵ����str�ĳ���ʱ����ƥ������str�ַ���
	   //�������ͬ���֪������׸����
	   assert(str != 0);
	   if(empty())
		   return npos;
	   if(pos >= size())
	   pos = size() - 1;
	   return rfind(self(str,n),pos); // ת�����صĳ�Ա����
   }

   size_type rfind(value_type val,size_type pos = npos) const throw()
   { 
	   if(empty())
		   return npos;
	   if(pos == npos || pos >= size())
	   pos = size() - 1;
	   return rfind(self(1,val),pos); // ת�����صĳ�Ա����
   }
   size_type find_first_of(const self& str,size_type pos = 0) const throw()
   { //posΪ�����_ʼ��������ѧ�����ʾΪ[pos,size() )
	   assert(pos <= size());
	   if(empty())
		   return npos;
	   size_type end_pos = size();
	   while(pos != end_pos && !is_inside(str,*(start+pos)))
		   ++pos;
	   if(pos == end_pos)
		   return npos;
	   else
		   return pos;
   }

   size_type find_first_of(const_pointer str,size_type pos = 0) const throw()
   {  //posΪ�����_ʼ������ΧΪ[pos,size() )
	   assert(pos <= size() && str != 0);
	   if(empty())
		   return npos;
	   return find_first_of(self(str),pos); // ת�����صĳ�Ա����
   }

   size_type find_first_of(const_pointer str,size_type pos, size_type n) const throw()
   { //posΪ�����_ʼ������ΧΪ[pos,size() )
	   assert(pos <= size() && str != 0);
	   if(empty())
		   return npos;
	   return find_first_of(self(str,n),pos); // // ת�����صĳ�Ա����
   }

   size_type find_first_of(value_type val,size_type pos = 0) const throw()
   { //posΪ�����_ʼ������ΧΪ[pos,size() )
	   assert(pos <= size());
	   if(empty())
		   return npos;
	   return find_first_of(self(1,val),pos); // ת�����صĳ�Ա����
   }
   size_type find_last_of(const self& str,size_type pos = npos) const throw()
   {  //pos ��������������Χ[0,pos],����pos
	   if(empty())
		   return npos;
	   if(pos == npos || pos >= size()) //����pos == npos���ƶϲ����Ƕ��һ��
		   pos = size() - 1;
	   while(pos >= 0 && pos != size_type(-1) && 
		   !is_inside(str,*(start+pos)))
		   --pos;
	   if(pos != size_type(-1))
		   return pos;
	   else
		   return npos;
   }

   size_type find_last_of(const_pointer str,size_type pos = npos) const throw()
   {  //pos ��������������Χ[0,pos],����pos
	   if(empty())
		   return npos;
	   assert(str != 0);
	   if(pos == npos || pos >= size())
		   pos = size() - 1;
	   return find_last_of(self(str),pos); // ת�����صĳ�Ա����
   }

   size_type find_last_of(const_pointer str,size_type pos,size_type n) const throw()
   {  //pos ��������������Χ[0,pos],����pos
	   assert(str != 0);
	   if(empty())
		   return npos;
	   if(pos >= size())
		   pos = size() - 1;
	   return find_last_of(self(str,n),pos); // ת�����صĳ�Ա����
   }

   size_type find_last_of(value_type val,size_type pos = npos) const throw()
   { //pos ��������������Χ[0,pos],����pos
	   if(empty())
		   return npos;
	   if(pos == npos || pos >= size())
		   pos = size() - 1;
	   return find_last_of(self(1,val),pos); // ת�����صĳ�Ա����
   }
   size_type find_first_not_of(const self& str,size_type pos = 0) const throw()
   {    //pos Ϊ�����_ʼ������Χ[pos,size() )
	   assert(pos <= size());
	   if(empty())
		   return npos;
	   size_type end_pos = size();
	   while(pos != end_pos && is_inside(str,*(start+pos)))
		   ++pos;
	   if(pos == end_pos)
		   return npos;
	   else
		   return pos;
   }
   size_type find_first_not_of(const_pointer str,size_type pos = 0) const throw()
   {  //pos Ϊ�����_ʼ������Χ[pos,size() ) 
	   assert(pos <= size() && str != 0);
	   if(empty())
		   return npos;
	   return find_first_not_of(self(str),pos);
   }

   size_type find_first_not_of(const_pointer str,size_type pos,size_type n) const throw()
   { //pos Ϊ�����_ʼ������Χ[pos,size() )
	   assert(pos <= size() && str != 0);
	   if(empty())
		   return npos;
	   return find_first_not_of(self(str,n),pos); // ת�����صĳ�Ա����
   }

   size_type find_first_not_of(value_type val,size_type pos = 0) const throw()
   { //pos Ϊ�����_ʼ������Χ[pos,size() )
	   assert(pos <= size());
	   if(empty())
		   return npos;
	   return find_first_not_of(self(1,val),pos);
   }
   size_type find_last_not_of(const self& str,size_type pos = npos) const throw()
  {  //pos ��������������Χ[0,pos]
	  if(empty())
		  return npos;
	   if(pos == npos || pos >= size())
		   pos = size() - 1;
	   while(pos >= 0 &&  pos != size_type(-1) && 
		   is_inside(str,*(start+pos)))
		   --pos;
	   if(pos != size_type(-1))
		   return pos;
	   else
		   return npos;
   }
   size_type find_last_not_of (const_pointer str,size_type pos = npos) const throw()
  {  //pos ��������������Χ[0,pos]
	  assert(str != 0);
	  if(empty())
		  return npos;
	  if(pos == npos || pos >= size()) //����pos == npos���ƶϲ����Ƕ��һ��
		  pos = size() - 1;
	   return find_last_not_of(self(str),pos); // ת�����صĳ�Ա����
   }

   size_type find_last_not_of (const_pointer str,size_type pos,size_type n) const throw()
  {  //pos ��������������Χ[0,pos]
	   assert(str != 0);
	   if(empty())
		   return npos;
	   if(pos >= size())
		   pos = size() - 1;
	   return find_last_not_of(self(str,n),pos); // ת�����صĳ�Ա����
   }

   size_type find_last_not_of (value_type val,size_type pos = npos) const throw()
   { //pos ��������������Χ[0,pos]
	   if(empty())
		   return npos;
	   if(pos == npos || pos >= size())
		   pos = size() - 1;
	   return find_last_not_of(self(1,val),pos); // ת�����صĳ�Ա����
   }
   self& operator += (const self& str)
   {
		insert_aux(size(),str);
		return *this;
   }
	self& operator += (value_type val)  
	{
		insert_aux(size(),self(1,val));
		return *this;
	}

	friend self operator + (const self& str1,const self& str2)
	{
		return self(str1) += str2;
	}
	friend self operator + (const self& str,value_type val)
	{
		return self(str) += self(1,val);
	}
	friend self operator + (value_type val,const self& str)
	{
		return self(1,val) += str;
	}
    friend void swap(self& str1,self& str2)
	{
		if(&str1 == &str2)
			return ;
		str1.swap(str2);
	}
	friend bool operator == (const self& str1,const self& str2)
	{
		return str1.compare(str2) == 0;
	}
	friend bool operator != (const self& str1,const self& str2)
	{
		return str1.compare(str2) != 0;
	}
	friend bool operator < (const self& str1,const self& str2)
	{
		return str1.compare(str2) < 0;
	}
	friend bool operator <= (const self& str1,const self& str2)
	{
		return str1.compare(str2) <= 0;
	}
	friend bool operator > (const self& str1,const self& str2)
	{
		return str1.compare(str2) > 0;
	}
	friend bool operator >= (const self& str1,const self& str2)
	{
		return str1.compare(str2) >= 0;
	}

};


template <class charT, class traits, class Alloc>
std::basic_istream<charT,traits>& operator >> (std::basic_istream<charT,traits>& is,
                                           mystd::basic_string<charT,traits,Alloc>& str)
{
	str.clear();  //�����str
	int chr = 0;  //��wchar_t�����ַ�Ҳ����
	while((chr = is.get()) && chr != ' ' && chr != '\n')
		str.push_back(chr);
	return is;
}


template <class charT, class traits, class Alloc>
 std::basic_ostream<charT,traits>& operator<< (std::basic_ostream<charT,traits>& os,
                                           const mystd::basic_string<charT,traits,Alloc>& str)
{
	typedef typename mystd::basic_string<charT,traits,Alloc>::size_type size_type;
	size_type size = str.size();
	for(size_type i = 0; i < size; ++i)
		os<<str[i];
	return os;

}

template <class charT, class traits, class Alloc>
std::basic_istream<charT,traits>& getline(std::basic_istream<charT,traits>& is,
                                 mystd::basic_string<charT,traits,Alloc>& str, charT delim)
{
	charT chr = 0;
	str.clear(); //�����str
	while((chr = is.get()) && chr != delim && chr != '\n')
		str.push_back(chr);
	return is;
}

template <class charT, class traits, class Alloc>
std::basic_istream<charT,traits>& getline(std::basic_istream<charT,traits>& is,
                                 mystd::basic_string<charT,traits,Alloc>& str)
{
	charT chr = 0;
	str.clear(); //�����str
	while((chr = is.get()) && chr != '\n')
		str.push_back(chr);
	return is;
}


typedef mystd::basic_string<char,std::char_traits<char>,std::allocator<char> > string;
typedef mystd::basic_string<wchar_t,std::char_traits<wchar_t>,std::allocator<wchar_t> > wstring;


MYSTD_END // end of namespace mystd 
#endif // __cplusplus 
#endif // MYSTD_STRING_H_EX

