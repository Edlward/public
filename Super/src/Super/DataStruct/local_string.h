
/*
本文实现了c++ STL中的basic_string模板类，当然。通过typedef也就实现了string类和wstring类。
限于篇幅，实现代码中用到了标准库的char_traits模板类，本人自己也实现了

char_traits模板类，为此还写了一篇博客，在我的博客里能够找到，那里的代码全然能够用在这里的
basic_string类的实现中，当然，详细加到代码中要稍加修改，以和与basic_string类相关的全局模板
函数兼容，如operator >> 模板函数的重载。

经过測试。这里的是实如今效率上不输标准库（与vs2012比对），代码凝视还算清晰，并且具有
良好的扩充性。能够以此量身打造自己的精简版本号string。

好了。废话不多说了！看代码！
还请高手们指教！！！

此文件我建议命名为local_string.h。还是我一惯的习惯，封装于名字空间mystd中。

假设要測试代码，直接复制以下的代码就能够了，都在一个文件里。

标准库有的功能。这里基本上都有！



*看完《STL源代码剖析》后就有重写STL的冲动，这个basic_string的实现还算完整。
标准库有的功能，这里基本都有，假设有须要，能够在此基础上扩展功能。！
！
大家如有发现bug，请与我联系，以使代码不断地完好！！。
*大家交流交流，谢谢!
* 邮箱：sunkang2101024@foxmail.com
*/


// 建议此文件命名为 "local_string.h"



// vs2012 调试通过
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
//随机訪问迭代器
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
		 // 此构造函数主要为了进行转换之用,
		 //reverse_iterator转换为 const_reverse_iterator 
	 }
	 self& operator = (const self& tmp)  // 能够不定义。直接使用默认的函数
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
 class random_access_iterator{  // 反向随机訪问迭代器
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
		 // 此构造函数主要为了进行转换之用,
		 //iterator转换为 const_iterator 
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
	 reference operator * () const // 足够的自由度
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
 //限于篇幅，这里的char_traits模板类用的是标准库的，
 //我的博客里有一篇文章是关于char_traits模板类的实现的，全然能够用在这里。


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
	pointer start,finish,end_of_storage; //数据成员
	typedef basic_string self;  //私有类型
	allocator_type alloc;  // 分配器
private: // 内部使用函数
	size_type alloc_capacity(size_type old_size) const  // 容量分配函数
	{ //这个函数主要是为了方便改动分配方案，
	//能够依照须要改动。可是要确保返回值大于old_size，以此保证capacity()非空（看实现可知）
	// 提示：与c_str()的实现有关。
		if(old_size < 10)
			return 10;  //wstring 
		else if(old_size < 100)
			return old_size * 2;
		else if(old_size < 1024)
			return old_size + old_size / 10;
		else
			return old_size + rand() % 1024 + 1;
		//返回值得大于old_size
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
	{ //用于insert相关函数
		assert(pos <= size());
		if(str.empty())
			return iterator(start+pos);
		if(size()+str.size() > capacity()) // 无论this是否等于&str
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
	{ //用于erase函数
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
	{  //用于初始化
		size_type capacity_size = alloc_capacity(1);
		start = alloc.allocate(capacity_size); //分配一定的空间
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
	{ // 后面的查找功能的函数会用到，如find_first_of()函数
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
	//这个构造函数不可用explicitkeyword。后面大量用到此构造函数做隐式转换
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
	  //标准库没有对长度进行调整(測试vs2012的执行结果可知),本人认为应该加上。这样有更好的容错性
	  //当然。在str非常大的时候，效率上会有点折扣（计算长度的时间花销上）
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
		while(new_first != last) //用后自增測试时会出现一点问题
			++len, ++new_first;
		size_type capacity_size = alloc_capacity(len);
		start = alloc.allocate(capacity_size);
		finish = start + len;
		/*while(first != last)  
			*start++ = *first++;*/  
		while(last != first)  //用后自增測试时会出现一点问题
			*--finish = *--last;
		finish = start + len; // 复位
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
		if(this == &tmp) // 防止自身复制
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
			alloc.deallocate(start,capacity()); // 释放原有空间
			start = alloc.allocate(capacity_size);
			traits_type::copy(start,tmp.start,tmp.size());
			finish = start + tmp.size();
			end_of_storage = start + capacity_size;
		}
		return *this;
	}
	self& operator = (value_type val)
	{
		clear(); // 不释放内存,看代码可知
		insert_aux(0,self(1,val));
		return *this;
	}
	~basic_string()
	{
		//alloc.deallocate(start,capacity());
		//由于capacity()实现上的缘故，这里用end_of_storage - start（看capacity代码可知）
		//当然，第二个參数其实没实用。随意写一个数字都可
		alloc.deallocate(start,end_of_storage - start);
	}
public:
	size_type size() const throw() 
	{ //依照标准文档的要求，不抛出异常,下面同理可得,不再赘述！
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
	{ //这里的计算本来不应该减 1 的，有意这样做是为了确保留有一个字符的空间(包括宽字符）,方便做
		 // C风格字符串的转换,写入空字符，用于成员函数c_str()
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
		finish = start; //并不释放内存
	}
	self substr(size_type pos = 0,size_type len = npos) const
	{
		assert(pos <= size());
		return self(*this,pos,len); // 显式调用构造函数
	}
    self& insert(size_type pos,const self& str)
    {
		assert(pos <= size());
		insert_aux(pos,str);
		return *this;
    }
	const_pointer c_str() const  throw()
	{ //构造函数保证capacity() != 0,或者说start,finish,end_of_storage不为空指针
		//当然，这和详细的实现有关，这是我的一个实现
		*finish = 0;  // 转换成C风格的字符串
		return start;
	}
	const_pointer data() const throw()
	{ 
	    *finish = 0; 
		// 也能够直接返回start,标准文档没有强制要求为C风格字符串
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
	   resize(n,0); //转调
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
	   // 后面的replace重载函数就不用写异常了，由于都会直接或间接地调用到这个函数
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
	   else if(this != &str)  //假设不是自身的替换
	   {
		   if(str.size() <= len)
		   {
			   traits_type::copy(start+pos,str.start,str.size());
			   erase(pos+str.size(),len-str.size()); // 长度len - str.size()
			   // finish 已经更新
		   }
		   else // size() + str.size() - len <= capacity() 
		   {
			   traits_type::move(start+pos+str.size(),start+pos+len,size()-pos-len);
			   traits_type::copy(start+pos,str.start,str.size());
			   finish += str.size() - len;
		   }
	   }
	   else
		   return replace(pos,len,self(str)); //递归调用，这次不会是自身了。
	  return *this;
   }
 
   self& replace(iterator itr_1,iterator itr_2,const self& str)
   {
	   assert(itr_1 >= begin() && itr_2 <= end());
	   return replace(itr_1-begin(),itr_2-itr_1,str); // 转调
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
	   return replace(itr_1,itr_2,self(str)); //转调
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
  { //后面的compare重载函数都间接调用此函数
	  if(this == &str)
		  return 0;
	  size_type num = size() >= str.size() ? size() : str.size();
	  return traits_type::compare(c_str(),str.c_str(),num); //须要转换为C风格字符串
  }
  int compare(size_type pos,size_type len,const self& str) const
  {
	  assert(pos <= size());
	  if(pos > size()) // 后面的重载函数不用写异常了。这里写了
		  throw std::out_of_range("invalid string position");
	  //size_type num = len >= str.size() ? len : str.size();
	  //return traits_type::compare(start+pos,str.start,num);
	  return substr(pos,len).compare(str);  //转调
  }
  int compare(size_type pos,size_type len,const self& str,
	  size_type subpos,size_type sublen) const
  {
	  assert(pos <= size());
	  return compare(pos,len,self(str,subpos,sublen)); //转调
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
		  len = size() - pos; // 调整一下长度
	  traits_type::copy(str,start+pos,len);
	  return len;
  }
  size_type find(const self& str,size_type pos = 0) const throw()
  { //pos为搜索開始处，用数学区间表示为[pos,size() )
	  assert(pos <= size());
	  if(empty())
		  return npos;
	  size_type len_str = str.size();
	  size_type len = size() - pos; 
	  if(len < len_str)
		  return npos;
	  size_type end_pos = size() - len_str; // 调整了一下有效长度，避免做无用功
	  while(pos <= end_pos)
	  {
		  if(compare(pos,len_str,str) == 0) // 转调重载的成员函数
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
	   return find(self(str),pos); // 转调重载的成员函数
   }

   size_type find(const_pointer str,size_type pos,size_type n) const throw()
   { //str中的n个字符,注意 n 的详细所指
	   assert(pos <= size() && str != 0);
	   if(empty())
		  return npos;
	   return find(self(str,n),pos);// 转调重载的成员函数
   }

   size_type find(value_type val,size_type pos = 0) const throw()
   {
	   assert(pos <= size());
	   if(empty())
		  return npos;
	   return find(self(1,val),pos); // 转调重载的成员函数
   }

   size_type rfind(const self& str,size_type pos = npos) const throw()
   {  // pos为搜索開始处，用数学区间表示为[0,pos]
	   	if(empty())
		   return npos;
	   if(pos == npos || pos >= size()) //加上pos == npos的推断并不是多此一举
		   pos = size() - 1;
	   if(size() < str.size())
		   return npos;
	   size_type len_str = str.size();
	   if(pos > size() - len_str)
		   pos = size() - len_str;  // 调整位置，避免做没用的搜索
	   while(pos >= 0 && pos != size_type(-1))
	   {
		   if(compare(pos,len_str,str) == 0) // 转调成员函数
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
	   if(pos == npos || pos >= size()) //加上pos == npos的推断并不是多此一举
	   pos = size() - 1;
	   return rfind(self(str),pos); // 转调重载的成员函数
   }

   size_type rfind(const_pointer str,size_type pos,size_type n) const throw()
   { // n为str须要搜索字符的个数,由于构造函数实现上的差异，我的n值在调用构造函数时会有调整，
	 // 这样是为了增强容错性，可是測试vs2012可知此实现上没有此功能，
	 //因此。用标准库时，依赖于用户使用正确的n值，否则会得不到预期的结果。
	 // 用我的这个实现，当n值大于str的长度时。会匹配整个str字符串
	   //相似情况同理可知，不再赘述。
	   assert(str != 0);
	   if(empty())
		   return npos;
	   if(pos >= size())
	   pos = size() - 1;
	   return rfind(self(str,n),pos); // 转调重载的成员函数
   }

   size_type rfind(value_type val,size_type pos = npos) const throw()
   { 
	   if(empty())
		   return npos;
	   if(pos == npos || pos >= size())
	   pos = size() - 1;
	   return rfind(self(1,val),pos); // 转调重载的成员函数
   }
   size_type find_first_of(const self& str,size_type pos = 0) const throw()
   { //pos为搜索開始处。用数学区间表示为[pos,size() )
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
   {  //pos为搜索開始处，范围为[pos,size() )
	   assert(pos <= size() && str != 0);
	   if(empty())
		   return npos;
	   return find_first_of(self(str),pos); // 转调重载的成员函数
   }

   size_type find_first_of(const_pointer str,size_type pos, size_type n) const throw()
   { //pos为搜索開始处，范围为[pos,size() )
	   assert(pos <= size() && str != 0);
	   if(empty())
		   return npos;
	   return find_first_of(self(str,n),pos); // // 转调重载的成员函数
   }

   size_type find_first_of(value_type val,size_type pos = 0) const throw()
   { //pos为搜索開始处，范围为[pos,size() )
	   assert(pos <= size());
	   if(empty())
		   return npos;
	   return find_first_of(self(1,val),pos); // 转调重载的成员函数
   }
   size_type find_last_of(const self& str,size_type pos = npos) const throw()
   {  //pos 搜索结束处。范围[0,pos],包括pos
	   if(empty())
		   return npos;
	   if(pos == npos || pos >= size()) //加上pos == npos的推断并不是多此一举
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
   {  //pos 搜索结束处。范围[0,pos],包括pos
	   if(empty())
		   return npos;
	   assert(str != 0);
	   if(pos == npos || pos >= size())
		   pos = size() - 1;
	   return find_last_of(self(str),pos); // 转调重载的成员函数
   }

   size_type find_last_of(const_pointer str,size_type pos,size_type n) const throw()
   {  //pos 搜索结束处，范围[0,pos],包括pos
	   assert(str != 0);
	   if(empty())
		   return npos;
	   if(pos >= size())
		   pos = size() - 1;
	   return find_last_of(self(str,n),pos); // 转调重载的成员函数
   }

   size_type find_last_of(value_type val,size_type pos = npos) const throw()
   { //pos 搜索结束处，范围[0,pos],包括pos
	   if(empty())
		   return npos;
	   if(pos == npos || pos >= size())
		   pos = size() - 1;
	   return find_last_of(self(1,val),pos); // 转调重载的成员函数
   }
   size_type find_first_not_of(const self& str,size_type pos = 0) const throw()
   {    //pos 为搜索開始处，范围[pos,size() )
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
   {  //pos 为搜索開始处，范围[pos,size() ) 
	   assert(pos <= size() && str != 0);
	   if(empty())
		   return npos;
	   return find_first_not_of(self(str),pos);
   }

   size_type find_first_not_of(const_pointer str,size_type pos,size_type n) const throw()
   { //pos 为搜索開始处，范围[pos,size() )
	   assert(pos <= size() && str != 0);
	   if(empty())
		   return npos;
	   return find_first_not_of(self(str,n),pos); // 转调重载的成员函数
   }

   size_type find_first_not_of(value_type val,size_type pos = 0) const throw()
   { //pos 为搜索開始处，范围[pos,size() )
	   assert(pos <= size());
	   if(empty())
		   return npos;
	   return find_first_not_of(self(1,val),pos);
   }
   size_type find_last_not_of(const self& str,size_type pos = npos) const throw()
  {  //pos 搜索结束处，范围[0,pos]
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
  {  //pos 搜索结束处，范围[0,pos]
	  assert(str != 0);
	  if(empty())
		  return npos;
	  if(pos == npos || pos >= size()) //加上pos == npos的推断并不是多此一举
		  pos = size() - 1;
	   return find_last_not_of(self(str),pos); // 转调重载的成员函数
   }

   size_type find_last_not_of (const_pointer str,size_type pos,size_type n) const throw()
  {  //pos 搜索结束处，范围[0,pos]
	   assert(str != 0);
	   if(empty())
		   return npos;
	   if(pos >= size())
		   pos = size() - 1;
	   return find_last_not_of(self(str,n),pos); // 转调重载的成员函数
   }

   size_type find_last_not_of (value_type val,size_type pos = npos) const throw()
   { //pos 搜索结束处，范围[0,pos]
	   if(empty())
		   return npos;
	   if(pos == npos || pos >= size())
		   pos = size() - 1;
	   return find_last_not_of(self(1,val),pos); // 转调重载的成员函数
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
	str.clear();  //先清空str
	int chr = 0;  //对wchar_t类型字符也适用
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
	str.clear(); //先清空str
	while((chr = is.get()) && chr != delim && chr != '\n')
		str.push_back(chr);
	return is;
}

template <class charT, class traits, class Alloc>
std::basic_istream<charT,traits>& getline(std::basic_istream<charT,traits>& is,
                                 mystd::basic_string<charT,traits,Alloc>& str)
{
	charT chr = 0;
	str.clear(); //先清空str
	while((chr = is.get()) && chr != '\n')
		str.push_back(chr);
	return is;
}


typedef mystd::basic_string<char,std::char_traits<char>,std::allocator<char> > string;
typedef mystd::basic_string<wchar_t,std::char_traits<wchar_t>,std::allocator<wchar_t> > wstring;


MYSTD_END // end of namespace mystd 
#endif // __cplusplus 
#endif // MYSTD_STRING_H_EX

