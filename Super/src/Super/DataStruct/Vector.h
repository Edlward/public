#ifndef _Vector_H_
#define _Vector_H_


#include <exception>
#include <iterator>
//�����Ҫ�����Լ������ĵ���������ͨ������������STL���㷨�����Ļ������ȼ̳�iteroter��Ȼ��ʵ�ֱ�Ҫ�Ĳ��������ɡ�


//using namespace std; 
//#include <vector>
//
//

//template<class T>
//class VectorIterator : public iterator<forward_iterator_tag, T>{
//public:
//        VectorIterator(T* p)
//    {
//            m_p = p;
//    }
//    VectorIterator& operator = (const VectorIterator &iter)
//    {
//        m_p = iter.m_p;
//    }
//    bool operator != (const VectorIterator &iter)
//    {
//        return m_p!= iter.m_p;
//    }
//    bool operator == (const VectorIterator &iter)
//    {
//        return m_p == iter.m_p;
//    }
//    VectorIterator& operator ++ ()
//    {
//        m_p++;
//        return *this;
//    }
//    VectorIterator operator ++ (int)
//    {
//        VectorIterator tmp= *this;
//        m_p++;
//        return tmp;
//    }
//    //ȡֵ
//    T& operator * ()
//    {
//        return *m_p;
//    }
//    private:
//    T* m_p;//ʵ�ʵ�����ָ�룬ͨ����ָ�����������
//};
//
//
//
////Vector ����
//template<typename T> 
//class Vector
//{
//    typedef T value_type;
//    typedef value_type* pointer;
//    typedef const value_type* const_pointer;
//    typedef value_type& reference;
//    typedef const value_type& const_reference;
//    typedef size_t size_type;
//    typedef ptrdiff_t difference_type;
//
//   //typedef value_type* iterator;
//   //typedef const value_type* const_iterator;
//
//    //��iteratorģ��ʵ��
//    typedef Vector self;
//    typedef VectorIterator<T> iterator;     //�������͵�������ͬһ�����֣�����д����ͨ�õĴ���
//    typedef const VectorIterator<T> const_iterator; 
//public:
//    Vector(size_t capacity)
//        :m_Capacity(capacity)
//    {
//        pArray=new T[m_Capacity];
//    }
//    ~Vector()
//    {
//        delete[] pArray;
//    }
//
//    iterator begin()const {return iterator(pArray);}
//    iterator end()const {return iterator(pArray+length);}
//    const_iterator begin()const {return const_iterator(pArray);}
//    const_iterator end()const {return const_iterator(pArray+length);}
//
//    //�������������ȫ
//    //reverse_iterator rbegin()
//    //{ return reverse_iterator(end()); }
//    //const_reverse_iterator rbegin() const
//    //{ return const_reverse_iterator(end()); }
//    //reverse_iterator rend()
//    //{ return reverse_iterator(begin()); }
//    //const_reverse_iterator rend() const
//    //{ return const_reverse_iterator(begin()); }
//
//   // size_type size() const
//   // { return size_type(end() - begin()); }
//    //bool empty() const{ return begin() == end(); }
//
//    size_t max_size() const{ return size_t(-1) / sizeof(T); }
//    //size_t capacity() const{ return size_type(_M_end_of_storage - begin()); }
//
//    reference operator[](size_t __n) { return *(begin() + __n); }
//    const_reference operator[](size_t __n) const { return *(begin() + __n); }
//
//
//    size_t size() const { return length();}
//    size_t length() const { return m_length;}
//
//private:
//    T *pArray;
//    const size_t m_Capacity;
//    size_t m_length;
//};
//
//





#define _SCL_SECURE_NO_WARNINGS //Ϊ�˷�ֹ��VS2013�б���  
#include<cstddef> //ptrdiff_t  
#include<memory>  

template<typename T>
void destroy(T* ptr)
{
    ptr->~T();  
}  

template<typename ForwardIterator> 
void destroy(ForwardIterator first, ForwardIterator last)
{  
    for (;first != last; ++first)  
    {
        destroy(&*first);  
    }
}  


//��STLԴ����������ʼǡ��Լ�ʵ��vector֮MyVector https://blog.csdn.net/ruan875417/article/details/46123455
template<typename T>  
class Vector
{  
public:  
    //Ƕ���ͱ���  
    typedef T value_type;  
    typedef T* pointer;  
    typedef const T* const_pointer;  
    typedef T* iterator;  
    typedef const T* const_iterator;  
    typedef T& reference;  
    typedef const T& const_reference;  
    typedef size_t size_type;  
    typedef ptrdiff_t difference_type;  
protected:  
    iterator start;                  //Ŀǰʹ�ÿռ��ͷ  
    iterator finish;                //Ŀǰʹ�ÿռ��β  
    iterator end_of_storage; //Ŀǰ���ÿռ��β  
    std::allocator<T> alloc;  //�ռ������  
public:  
    Vector(); 
    Vector(size_type n, const T& value);
    Vector(iterator first, iterator last);
    ~Vector();
    Vector(Vector& vec);                     //�������캯��  
    Vector& operator=(Vector& vec); //������ֵ�����  
public:  
    iterator begin(){ return start; }  
    const_iterator begin() const { return start; }  
    iterator end(){ return finish; }  
    const_iterator end() const { return finish; }  

    size_type size() const { return (size_type)(finish - start); }  
    size_type capacity() const { return (size_type)(end_of_storage - start); }  
    bool empty() const { return start == finish; }  

    reference operator[](size_type n){ return *(begin() + n); }  
    const_reference operator[](size_type n) const { return *(begin() + n); }  
    reference front() { return *begin(); }  
    const_reference front() const { return *begin(); }  
    reference back(){ return *(end() - 1); }  
    const_reference back() const { return *(end() - 1); }  

    void push_back(const T& value);  
    void pop_back();  
    void insert(iterator position,size_type n,const T& value);  
    iterator erase(iterator position);  
    iterator erase(iterator first,iterator last);  
    void resize(size_type new_size);  
    void resize(size_type new_size, const T& value);  
    void reserve(size_type new_size);  
    void clear();  
private:  
    void free();  
    void reallocate();  
};  

template<typename T>  
Vector<T>::Vector():start(nullptr),finish(nullptr),end_of_storage(nullptr)
{

}  

template<typename T>  
Vector<T>::Vector(size_type n, const T& value)
{  
    start=alloc.allocate(n);  
    finish = end_of_storage = start + n;  
    for (iterator it = start; it != finish; ++start)  
    {
        alloc.construct(it, value);  
    }
}  

template<typename T>  
Vector<T>::Vector(iterator first, iterator last)
{  
    size_type n = (size_type)(last - first);  
    start = alloc.allocate(n);  
    finish = end_of_storage = start + n;  
    std::uninitialized_copy(first, last, start);  
}  
  
template<typename T>  
Vector<T>::~Vector()
{  
    free();  
}  

template<typename T>  
void Vector<T>::free()
{  
    destroy(start, finish);  
    alloc.deallocate(start, capacity());  
}  


template<typename T>  
Vector<T>::Vector(Vector& vec)
{  
    start = alloc.allocate(vec.size());  
    finish = end_of_storage = start + vec.size();  
    std::uninitialized_copy(vec.begin(), vec.end(), start);  
}  

//������ֵ�����  
template<typename T>  
Vector<T>& Vector<T>::operator=(Vector& vec)
{  
    if (vec != *this)
    {  
        iterator new_start = alloc.allocate(vec.size());  
        std::uninitialized_copy(vec.begin(), vec.end(), new_start);  
        size_type old_size = size();  
        size_type old_capacity = capacity();  
        free();  
        start = new_start;  
        finish = start + old_size;  
        end_of_storage = start + old_capacity;  
    }  
    return *this;  
}  
//reallocate()  
template<typename T>  
void Vector<T>::reallocate()
{  
    size_type new_size = size() ? size() * 2 : 1;  
    iterator new_start = alloc.allocate(new_size);  
    iterator new_finish=std::uninitialized_copy(start, finish, new_start);  
    free();  
    start = new_start;  
    finish = new_finish;  
    end_of_storage = start + new_size;  
}

//push_back(const T& value)  
template<typename T>  
void Vector<T>::push_back(const T& value)
{  
    if (size() == capacity())  
    {
        reallocate();  
    }
    alloc.construct(finish++, value);  
}


//pop_back()  
template<typename T>  
void Vector<T>::pop_back()
{  
    alloc.destroy(--finish);  
}

//insert(iterator position, size_type n, const T& value)  
template<typename T>  
void Vector<T>::insert(iterator position, size_type n, const T& value)
{  
    if (n <= (size_type)(capacity() - size()))
    {  
        if (n>(size_type)(finish-position))
        {  
            std::uninitialized_fill(finish, finish + n - (size_type)(finish - position),value);  
            std::uninitialized_copy(position, finish, position + n);  
            std::uninitialized_fill(position, finish, value);  
        }  
        else
        {  
            std::copy_backward(position, finish, finish + n);  
            std::uninitialized_fill_n(position, n, value);  
        }  
        finish += n;  
    }  
    else
    {  
        size_type new_size = size() + (size() > n ? size() : n);  
        iterator new_start = alloc.allocate(new_size);  
        iterator new_finish = std::uninitialized_copy(start, position, new_start);  
        new_finish=std::uninitialized_fill_n(new_finish, n, value);  
        new_finish = std::uninitialized_copy(position, finish, new_finish);  
        free();  
        start = new_start;  
        finish = new_finish;  
        end_of_storage = start + new_size;  
    }  
}  

//erase(iterator position)  
template<typename T>  
typename Vector<T>::iterator Vector<T>::erase(iterator position)
{  
    if (position+1!=finish)  
    {
        std::uninitialized_copy(position + 1, finish, position);  
    }
    destroy(--finish);  
    return position;  
}  

//erase(iterator first, iterator last)  
template<typename T>  
typename Vector<T>::iterator Vector<T>::erase(iterator first, iterator last)
{  
    size_type n = (size_type)(last - first);  
    std::uninitialized_copy(last, finish, first);  
    destroy(finish - n, finish);  
    finish = finish - n;  
    return first;  
}  
//resize(size_type new_size)  
template<typename T>  
void Vector<T>::resize(size_type new_size)
{  
    resize(new_size, T());  
}  

//resize(size_type new_size, const T& value)  
template<typename T>  
void Vector<T>::resize(size_type new_size, const T& value)
{  
    if (new_size > size())  
    {
        insert(finish, new_size - size(), value);  
    }
    else  
    {
        erase(begin() + new_size, finish);  
    }
}  

//reserve(size_type new_size)  
template<typename T>  
void Vector<T>::reserve(size_type new_size)
{  
    if (new_size > capacity())
    {  
        iterator new_start = alloc.allocate(new_size);  
        size_t old_size = size();  
        std::uninitialized_copy(start, finish, new_start);  
        free();  
        start = new_start;  
        finish = start + old_size;  
        end_of_storage = start + new_size;  
    }  
}  

//clear()  
template<typename T>  
void Vector<T>::clear()
{  
    erase(start, finish);  
}  


#ifdef UNIT_TEST
int Test_Vector();
#endif


#endif  


