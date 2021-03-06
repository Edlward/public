// memory standard header
#pragma once
//#ifndef _MEMORY_
//#define _MEMORY_

#ifndef _smart_ptr_
#define _smart_ptr_


//来自VS2012的memory  想办法改写成跨平台的智能指针
#ifndef RC_INVOKED
//#include <xmemory>
//#include <stdint.h>	/* for uintptr_t */


#ifndef _XSTD2
#define _XSTD2
#endif /* _XSTD2 */


//#include <exception>
#include <typeinfo>
#include <type_traits>


#pragma pack(push,_CRT_PACKING)
#pragma warning(push,3)
#pragma push_macro("new")
#undef new

#pragma warning(disable: 28197)


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//为了去除vs依赖，添加以下宏定义
#define _SUPER_BEGIN	namespace super {
#define _SUPER_END		}

//#include <atomic>
//typedef  std::atomic_size_t _Atomic_counter_t;    //暂时如此
using namespace std;
//using namespace super;
#define _HAS_CPP0X     1
#define _NOEXCEPT	throw ()
//#define _STD ::std::
#define _SP_STD ::std::
#define _XSTD	_STD /* LEAVE SPACE */
#define __CLR_OR_THIS_CALL    

#include <algorithm> //swap need
#include <exception>   //use at class bad_weak_ptr: public std::exception

#if _HAS_EXCEPTIONS
#define _TRY_BEGIN	try {
#define _CATCH(x)	} catch (x) {
#define _CATCH_ALL	} catch (...) {
#define _CATCH_END	}

#define _RAISE(x)	throw x
#define _RERAISE	throw

#ifndef _THROWS
#define _THROWS(x)	throw (x)
#endif /* _THROWS */

#define _THROW0()		throw ()
#define _THROW1(x)	throw (...)

#define _THROW(x, y)			 throw x(y)
#define _THROW_NCEE(x, y)	 throw x(y)

#else /* _HAS_EXCEPTIONS */
#define _TRY_BEGIN	{{
#define _CATCH(x)	} if (0) {
#define _CATCH_ALL	} if (0) {
#define _CATCH_END	}}

#if defined(_DEBUG)
#define _RAISE(x)	\
    _invoke_watson(__STR2WSTR(#x), __FUNCTIONW__, __FILEW__, __LINE__, 0)

#else /* defined(_DEBUG) */
#define _RAISE(x)	\
    _invoke_watson(0, 0, 0, 0, 0)
#endif /* defined(_DEBUG) */

#define _RERAISE

#ifndef _THROWS
#define _THROWS(x)
#endif /* _THROWS */

#define _THROW0()
#define _THROW1(x)
#define _THROW(x, y)		x(y)._Raise()
#define _THROW_NCEE(x, y)	x(y)._Raise()
#endif /* _HAS_EXCEPTIONS */


#define _VARIADIC_EXPAND_0X(FUNC, X1, X2, X3, X4)\
    _VARIADIC_EXPAND_0(FUNC, X1, X2, X3, X4)\
    _VARIADIC_EXPAND_1X(FUNC, X1, X2, X3, X4)

///////////////////////////////////////////////////////////////////////////////////////
//_STD_BEGIN
_SUPER_BEGIN

#undef _TYPEDEF_BOOL_TYPE    //add by lizulin
#undef _M_CEE                          //add by lizulin

#ifndef _TYPEDEF_BOOL_TYPE
template<class _Ty>
struct _Bool_struct
{	// define member just for its address
    int _Member;
};

#ifdef _M_CEE
template<class _Ty> inline
    void __CLRCALL_OR_CDECL _Bool_function(_Bool_struct<_Ty>&)
{	// do nothing
}

#define _TYPEDEF_BOOL_TYPE	\
    typedef void (__CLRCALL_OR_CDECL * _Bool_type)(_STD _Bool_struct<_Myt>&)
#define _CONVERTIBLE_TO_TRUE	(&_STD _Bool_function<_Myt>)
#define _OPERATOR_BOOL		operator _Bool_type

#else /* _M_CEE */
#define _TYPEDEF_BOOL_TYPE	\
    typedef int _STD _Bool_struct<_Myt>::* _Bool_type
#define _CONVERTIBLE_TO_TRUE	(&_STD _Bool_struct<_Myt>::_Member)
#define _OPERATOR_BOOL		operator _Bool_type
#endif /* _M_CEE */
#endif /* _TYPEDEF_BOOL_TYPE */


//class nullptr_t
//{
//public:
//    template<class T>
//    inline operator T*() const    //定义类型转换操作符，使nullptr_t 可转为任意非类成员指针类型
//    { return 0; }
//
//
//    //重载类型转换操作符，使 nullptr_t 可以转换为类 C 中任意的指针类型（数据成员指针/函数成员指针）	
//    //对类中数据成员的指针，T 将被推导为数据成员的类型 eg: int (X::*); 此时 T 为 int,C 为 X
//    //对类中函数成员的指针，T 将被推导为函数成员的类型 eg: int (X::*)(int); 此时T 等效于： typedef int (T)(int)
//    template<class C, class T>
//    inline operator T C::*() const  
//    { return 0; } 			
//private:				
//    void operator&() const;
//};
//const nullptr_t nullptr = {};
// 不支持C++11的编译器
//class nullptr_t
//{
//public:
//    template<class _Tx> operator _Tx*() const { return 0; }
//    template<class _Tx, class _Ty> operator _Ty _Tx::*() const{ return 0; }
//private:
//    void operator& () const; // 不允许取地址
//};
//
//const nullptr_t nullptr={};

//_STD_END
_SUPER_END
///////////////////////////////////////////////////////////////////////////////////////
//_STD move _STD forward std::依赖s 移植

//_STD_BEGIN
_SUPER_BEGIN

// TEMPLATE remove_reference
template<class _Ty>
struct remove_reference
{	// remove reference
    typedef _Ty type;
};

template<class _Ty>
struct remove_reference<_Ty&>
{	// remove reference
    typedef _Ty type;
};

template<class _Ty>
struct remove_reference<_Ty&&>
{	// remove rvalue reference
    typedef _Ty type;
};


// TEMPLATE FUNCTION forward
template<class _Ty> inline
_Ty&& forward(typename remove_reference<_Ty>::type& _Arg)
{	// forward an lvalue
    return (static_cast<_Ty&&>(_Arg));
}

template<class _Ty> inline
    _Ty&& forward(typename remove_reference<_Ty>::type&& _Arg) _NOEXCEPT
{	// forward anything
    static_assert(!is_lvalue_reference<_Ty>::value, "bad forward call");
    return (static_cast<_Ty&&>(_Arg));
}

// TEMPLATE FUNCTION move
template<class _Ty> inline
    typename remove_reference<_Ty>::type&&
    move(_Ty&& _Arg) _NOEXCEPT
{	// forward _Arg as movable
    return ((typename remove_reference<_Ty>::type&&)_Arg);
}

// TEMPLATE FUNCTION _Move
template<class _Ty> inline
    typename remove_reference<_Ty>::type&&
    _Move(_Ty&& _Arg) _NOEXCEPT
{	// forward _Arg as movable
    return ((typename remove_reference<_Ty>::type&&)_Arg);
}

#if 0
// TEMPLATE CLASS is_nothrow_move_constructible
template<class _Ty>
struct is_nothrow_move_constructible
    : _Cat_base<!is_array<_Ty>::value
    && is_nothrow_constructible<
    typename remove_volatile<_Ty>::type,
    typename add_rvalue_reference<
    typename remove_volatile<_Ty>::type>::type>::value>
{	// determine whether _Ty has a nothrow move constructor
};

// TEMPLATE CLASS has_nothrow_move_constructor -- retained
template<class _Ty>
struct has_nothrow_move_constructor
    : is_nothrow_move_constructible<_Ty>::type
{	// determine whether _Ty has a nothrow move constructor
};

// TEMPLATE FUNCTION move_if_noexcept
template<class _Ty> inline
    typename _If<!has_nothrow_move_constructor<_Ty>::value
    && has_copy_constructor<_Ty>::value,
    const _Ty&, _Ty&&>::type
    move_if_noexcept(_Ty& _Arg) _NOEXCEPT
{	// forward _Arg if moveable

    return (_STD move(_Arg));  //old
    //return (super::move(_Arg));  //new
}
#endif

// TEMPLATE FUNCTION _Decay_copy
#if 0
template<class _Ty> inline
    typename decay<_Ty>::type _Decay_copy(_Ty&& _Arg)
{	// forward _Arg as value of decayed type
    return (_STD forward<_Ty>(_Arg)); //old
    //return (super::forward<_Ty>(_Arg)); //new
}
#endif


//--------------------------------------------------------------------------------------
// TEMPLATE STRUCT binary_function
template<class _Arg1,
class _Arg2,
class _Result>
struct binary_function
{	// base class for binary functions
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};

// TEMPLATE STRUCT equal_to
template<class _Ty>
struct equal_to
    : public binary_function<_Ty, _Ty, bool>
{	// functor for operator==
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {	// apply operator== to operands
        return (_Left == _Right);
    }
};

//--------------------------------------------------------------------------------------
template<class _Ty>
struct _Always_false
{	// false value that probably won't be optimized away
    static const bool value = false;
};


//--------------------------------------------------------------------------------------
// TEMPLATE CLASS add_reference -- retained
template<class _Ty>
struct add_reference
{	// add reference
    typedef typename remove_reference<_Ty>::type& type;
};

template<>
struct add_reference<void>
{	// add reference
    typedef void type;
};

template<>
struct add_reference<const void>
{	// add reference
    typedef const void type;
};

template<>
struct add_reference<volatile void>
{	// add reference
    typedef volatile void type;
};

template<>
struct add_reference<const volatile void>
{	// add reference
    typedef const volatile void type;
};

//_STD_END
_SUPER_END

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//_STD swap 实现移到此处
//_STD_BEGIN
_SUPER_BEGIN

template<class _Ty> inline
    void swap(_Ty& _Left, _Ty& _Right)
{	// exchange values stored at _Left and _Right
    _Ty _Tmp = _Move(_Left);
    _Left = _Move(_Right);
    _Right = _Move(_Tmp);
}

template<class _Ty> inline
    void _Swap_adl(_Ty& _Left, _Ty& _Right)
{	// exchange values stored at _Left and _Right, using ADL
    swap(_Left, _Right);
}

//_STD_END
_SUPER_END


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//auto_ptr的实现从原<xmemory> 移到此处

//_STD_BEGIN
_SUPER_BEGIN

// TEMPLATE CLASS auto_ptr
template<class _Ty>
class auto_ptr;

template<class _Ty>
struct auto_ptr_ref
{	// proxy reference for auto_ptr copying
    explicit auto_ptr_ref(_Ty *_Right)
        : _Ref(_Right)
    {	// construct from generic pointer to auto_ptr ptr
    }

    _Ty *_Ref;	// generic pointer to auto_ptr ptr
};

template<class _Ty>
class auto_ptr
{	// wrap an object pointer to ensure destruction
public:
    typedef auto_ptr<_Ty> _Myt;
    typedef _Ty element_type;

    explicit auto_ptr(_Ty *_Ptr = 0) _THROW0()
        : _Myptr(_Ptr)
    {	// construct from object pointer
    }

    auto_ptr(_Myt& _Right) _THROW0()
        : _Myptr(_Right.release())
    {	// construct by assuming pointer from _Right auto_ptr
    }

    auto_ptr(auto_ptr_ref<_Ty> _Right) _THROW0()
    {	// construct by assuming pointer from _Right auto_ptr_ref
        _Ty *_Ptr = _Right._Ref;
        _Right._Ref = 0;	// release old
        _Myptr = _Ptr;	// reset this
    }

    template<class _Other>
    operator auto_ptr<_Other>() _THROW0()
    {	// convert to compatible auto_ptr
        return (auto_ptr<_Other>(*this));
    }

    template<class _Other>
    operator auto_ptr_ref<_Other>() _THROW0()
    {	// convert to compatible auto_ptr_ref
        _Other *_Cvtptr = _Myptr;	// test implicit conversion
        auto_ptr_ref<_Other> _Ans(_Cvtptr);
        _Myptr = 0;	// pass ownership to auto_ptr_ref
        return (_Ans);
    }

    template<class _Other>
    _Myt& operator=(auto_ptr<_Other>& _Right) _THROW0()
    {	// assign compatible _Right (assume pointer)
        reset(_Right.release());
        return (*this);
    }

    template<class _Other>
    auto_ptr(auto_ptr<_Other>& _Right) _THROW0()
        : _Myptr(_Right.release())
    {	// construct by assuming pointer from _Right
    }

    _Myt& operator=(_Myt& _Right) _THROW0()
    {	// assign compatible _Right (assume pointer)
        reset(_Right.release());
        return (*this);
    }

    _Myt& operator=(auto_ptr_ref<_Ty> _Right) _THROW0()
    {	// assign compatible _Right._Ref (assume pointer)
        _Ty *_Ptr = _Right._Ref;
        _Right._Ref = 0;	// release old
        reset(_Ptr);	// set new
        return (*this);
    }

    ~auto_ptr() _NOEXCEPT
    {	// destroy the object
        delete _Myptr;
    }

    _Ty& operator*() const _THROW0()
    {	// return designated value

//调试模式不需要--废弃--by lizulin
//#if _ITERATOR_DEBUG_LEVEL == 2
//        if (_Myptr == 0)
//            _DEBUG_ERROR("auto_ptr not dereferencable");
//#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

        return (*get());
    }

    _Ty *operator->() const _THROW0()
    {	// return pointer to class object

//调试模式不需要--废弃--by lizulin
//#if _ITERATOR_DEBUG_LEVEL == 2
//        if (_Myptr == 0)
//            _DEBUG_ERROR("auto_ptr not dereferencable");
//#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

        return (get());
    }

    _Ty *get() const _THROW0()
    {	// return wrapped pointer
        return (_Myptr);
    }

    _Ty *release() _THROW0()
    {	// return wrapped pointer and give up ownership
        _Ty *_Tmp = _Myptr;
        _Myptr = 0;
        return (_Tmp);
    }

    void reset(_Ty *_Ptr = 0)
    {	// destroy designated object and store new pointer
        if (_Ptr != _Myptr)
            delete _Myptr;
        _Myptr = _Ptr;
    }

private:
    _Ty *_Myptr;	// the wrapped object pointer
};
//_STD_END
 _SUPER_END
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////










//_STD_BEGIN
 _SUPER_BEGIN





 #if _HAS_CPP0X
template<class _Ty>
    struct default_delete;

template<class _Ty,
    class _Dx = default_delete<_Ty> >
    class unique_ptr;
 #endif /* _HAS_CPP0X */

    // CLASS bad_weak_ptr
class bad_weak_ptr
    //: public _XSTD exception
: public std::exception
    {	// exception type for invalid use of expired weak_ptr object
public:
    explicit bad_weak_ptr(const char * = 0) _NOEXCEPT
        {	// construct with ignored message
        }

    virtual const char *__CLR_OR_THIS_CALL what() const throw()
        {	// return pointer to message string
        return ("bad_weak_ptr");
        }
    };



//暂时不用原子变量的引用计数
#if 0  //old
    // CLASS _Ref_count_base
class _Ref_count_base
    {	// common code for reference counting
private:
    virtual void _Destroy() = 0;
    virtual void _Delete_this() = 0;

private:
    _Atomic_counter_t _Uses;
    _Atomic_counter_t _Weaks;

protected:
    _Ref_count_base()
        {	// construct
        _Init_atomic_counter(_Uses, 1);
        _Init_atomic_counter(_Weaks, 1);
        }

public:
    virtual ~_Ref_count_base() _NOEXCEPT
        {	// ensure that derived classes can be destroyed properly
        }

    bool _Incref_nz()
        {	// increment use count if not zero, return true if successful
        for (; ; )
            {	// loop until state is known
 #if defined(_M_IX86) || defined(_M_X64) || defined(_M_CEE_PURE)
            _Atomic_integral_t _Count =
                static_cast<volatile _Atomic_counter_t&>(_Uses);

            if (_Count == 0)
                return (false);

            if (static_cast<_Atomic_integral_t>(_InterlockedCompareExchange(
                    reinterpret_cast<volatile long *>(&_Uses),
                    _Count + 1, _Count)) == _Count)
                return (true);
 #else /* defined(_M_IX86) || defined(_M_X64) || defined(_M_CEE_PURE) */
            _Atomic_integral_t _Count =
                _Load_atomic_counter_explicit(_Uses, memory_order_relaxed);

            if (_Count == 0)
                return (false);

            if (_Compare_increment_atomic_counter_explicit(
                    _Uses, _Count, memory_order_relaxed))
                return (true);
 #endif /* defined(_M_IX86) || defined(_M_X64) || defined(_M_CEE_PURE) */
            }
        }

    unsigned int _Get_uses() const
        {	// return use count
        return (_Get_atomic_count(_Uses));
        }

    void _Incref()
        {	// increment use count
        _MT_INCR(_Ignored, _Uses);
        }

    void _Incwref()
        {	// increment weak reference count
        _MT_INCR(_Ignored, _Weaks);
        }

    void _Decref()
        {	// decrement use count
        if (_MT_DECR(_Ignored, _Uses) == 0)
            {	// destroy managed resource, decrement weak reference count
            _Destroy();
            _Decwref();
            }
        }

    void _Decwref()
        {	// decrement weak reference count
        if (_MT_DECR(_Ignored, _Weaks) == 0)
            _Delete_this();
        }

    long _Use_count() const
        {	// return use count
        return (_Get_uses());
        }

    bool _Expired() const
        {	// return true if _Uses == 0
        return (_Get_uses() == 0);
        }

    virtual void *_Get_deleter(const _XSTD2 type_info&) const
        {	// return address of deleter object
        return (0);
        }
    };
#else

typedef int _Atomic_counter_t;
typedef int _Atomic_integral_t;

// CLASS _Ref_count_base
class _Ref_count_base
{	// common code for reference counting
private:
    virtual void _Destroy() = 0;
    virtual void _Delete_this() = 0;

private:
    _Atomic_counter_t _Uses;
    _Atomic_counter_t _Weaks;

protected:
    _Ref_count_base()
    {	// construct
        //_Init_atomic_counter(_Uses, 1);
        //_Init_atomic_counter(_Weaks, 1);
       _Uses=1;
       _Weaks=1;
    }

public:
    virtual ~_Ref_count_base() _NOEXCEPT
    {	// ensure that derived classes can be destroyed properly
    }

    bool _Incref_nz()
    {	// increment use count if not zero, return true if successful
        for (; ; )
        {	// loop until state is known
          
            //add temp 
                return (false);

#if defined(_M_IX86) || defined(_M_X64) || defined(_M_CEE_PURE)


#if 0
            _Atomic_integral_t _Count =static_cast<volatile _Atomic_counter_t&>(_Uses);

            if (_Count == 0)
                return (false);

            if (static_cast<_Atomic_integral_t>(_InterlockedCompareExchange(
                reinterpret_cast<volatile long *>(&_Uses),
                _Count + 1, _Count)) == _Count)
                return (true);
#endif


#else /* defined(_M_IX86) || defined(_M_X64) || defined(_M_CEE_PURE) */


#if 0
            _Atomic_integral_t _Count =
                _Load_atomic_counter_explicit(_Uses, memory_order_relaxed);

            if (_Count == 0)
                return (false);

            if (_Compare_increment_atomic_counter_explicit(
                _Uses, _Count, memory_order_relaxed))
                return (true);
#endif


#endif /* defined(_M_IX86) || defined(_M_X64) || defined(_M_CEE_PURE) */
        }
    }

    unsigned int _Get_uses() const
    {	// return use count
        //return (_Get_atomic_count(_Uses)); //old
        return (_Uses);
    }

    void _Incref()
    {	// increment use count
       // _MT_INCR(_Ignored, _Uses); //old
        _Uses++;
    }

    void _Incwref()
    {	// increment weak reference count
        //_MT_INCR(_Ignored, _Weaks);
        _Weaks++;
    }

    void _Decref()
    {	// decrement use count
        //if (_MT_DECR(_Ignored, _Uses) == 0) //old
        if (--_Uses== 0)  //new
        {	// destroy managed resource, decrement weak reference count
            _Destroy();
            _Decwref();
        }
    }

    void _Decwref()
    {	// decrement weak reference count
        //if (_MT_DECR(_Ignored, _Weaks) == 0)//old
        if (--_Weaks == 0)//new
            _Delete_this();
    }

    long _Use_count() const
    {	// return use count
        return (_Get_uses());//
    }

    bool _Expired() const
    {	// return true if _Uses == 0
        return (_Get_uses() == 0);
    }

    virtual void *_Get_deleter(const _XSTD2 type_info&) const
    {	// return address of deleter object
        return (0);
    }
};
#endif




    // TEMPLATE CLASS _Ref_count
template<class _Ty>
    class _Ref_count
    : public _Ref_count_base
    {	// handle reference counting for object without deleter
public:
    _Ref_count(_Ty *_Px)
        : _Ref_count_base(), _Ptr(_Px)
        {	// construct
        }

private:
    virtual void _Destroy()
        {	// destroy managed resource
        delete _Ptr;
        }

    virtual void _Delete_this()
        {	// destroy self
        delete this;
        }

    _Ty * _Ptr;
    };

    // TEMPLATE CLASS _Ref_count_del
template<class _Ty,
    class _Dx>
    class _Ref_count_del
    : public _Ref_count_base
    {	// handle reference counting for object with deleter
public:
    _Ref_count_del(_Ty *_Px, _Dx _Dt)
        : _Ref_count_base(), _Ptr(_Px), _Dtor(_Dt)
        {	// construct
        }

    virtual void *_Get_deleter(const _XSTD2 type_info& _Typeid) const
        {	// return address of deleter object
        return ((void *)(_Typeid == typeid(_Dx) ? &_Dtor : 0));
        }

private:
    virtual void _Destroy()
        {	// destroy managed resource
        _Dtor(_Ptr);
        }

    virtual void _Delete_this()
        {	// destroy self
        delete this;
        }

    _Ty * _Ptr;
    _Dx _Dtor;	// the stored destructor for the controlled object
    };

    // TEMPLATE CLASS _Ref_count_del_alloc
template<class _Ty,
    class _Dx,
    class _Alloc>
    class _Ref_count_del_alloc
    : public _Ref_count_base
    {	// handle reference counting for object with deleter and allocator
public:
    typedef _Ref_count_del_alloc<_Ty, _Dx, _Alloc> _Myty;
    typedef typename _Alloc::template rebind<_Myty>::other _Myalty;

    _Ref_count_del_alloc(_Ty *_Px, _Dx _Dt, _Myalty _Al)
        : _Ref_count_base(), _Ptr(_Px), _Dtor(_Dt), _Myal(_Al)
        {	// construct
        }

    virtual void *_Get_deleter(const _XSTD2 type_info& _Typeid) const
        {	// return address of deleter object
        return ((void *)(_Typeid == typeid(_Dx) ? &_Dtor : 0));
        }

private:
    virtual void _Destroy()
        {	// destroy managed resource
        _Dtor(_Ptr);
        }

    virtual void _Delete_this()
        {	// destroy self
        _Myalty _Al = _Myal;
        _Al.destroy(this);
        _Al.deallocate(this, 1);
        }

    _Ty * _Ptr;
    _Dx _Dtor;	// the stored destructor for the controlled object
    _Myalty _Myal;	// the stored allocator for this
    };

    // DECLARATIONS
template<class _Ty>
    class weak_ptr;
template<class _Ty>
    class shared_ptr;
template<class _Ty>
    class enable_shared_from_this;
struct _Static_tag {};
struct _Const_tag {};
struct _Dynamic_tag {};
template<class _Ty1,
    class _Ty2>
    void _Do_enable(_Ty1 *, enable_shared_from_this<_Ty2> *,
        _Ref_count_base *);

template<class _Ty>
    inline void _Enable_shared(_Ty *_Ptr, _Ref_count_base *_Refptr,
        typename _Ty::_EStype * = 0)
    {	// reset internal weak pointer
    if (_Ptr)
        _Do_enable(_Ptr,
            (enable_shared_from_this<typename _Ty::_EStype>*)_Ptr, _Refptr);
    }

inline void _Enable_shared(const volatile void *, const volatile void *)
    {	// not derived from enable_shared_from_this; do nothing
    }

    // TEMPLATE CLASS _Ptr_base
template<class _Ty>
    class _Ptr_base
    {	// base class for shared_ptr and weak_ptr
public:
    typedef _Ptr_base<_Ty> _Myt;
    typedef _Ty _Elem;
    typedef _Elem element_type;

    _Ptr_base()
        : _Ptr(0), _Rep(0)
        {	// construct
        }

    _Ptr_base(_Myt&& _Right)
        : _Ptr(0), _Rep(0)
        {	// construct _Ptr_base object that takes resource from _Right
        _Assign_rv(_SP_STD forward<_Myt>(_Right));
        }

    template<class _Ty2>
        _Ptr_base(_Ptr_base<_Ty2>&& _Right)
        : _Ptr(_Right._Ptr), _Rep(_Right._Rep)
        {	// construct _Ptr_base object that takes resource from _Right
        _Right._Ptr = 0;
        _Right._Rep = 0;
        }

    _Myt& operator=(_Myt&& _Right)
        {	// construct _Ptr_base object that takes resource from _Right
        _Assign_rv(_SP_STD forward<_Myt>(_Right));
        return (*this);
        }

    void _Assign_rv(_Myt&& _Right)
        {	// assign by moving _Right
        if (this != &_Right)
            _Swap(_Right);
        }

    long use_count() const _NOEXCEPT
        {	// return use count
        return (_Rep ? _Rep->_Use_count() : 0);
        }

    void _Swap(_Ptr_base& _Right)
        {	// swap pointers

        //old
        //_STD swap(_Rep, _Right._Rep);
        //_STD swap(_Ptr, _Right._Ptr);
        //new
          super::swap(_Rep, _Right._Rep);
          super::swap(_Ptr, _Right._Ptr);
        }

    template<class _Ty2>
        bool owner_before(const _Ptr_base<_Ty2>& _Right) const
        {	// compare addresses of manager objects
        return (_Rep < _Right._Rep);
        }

    void *_Get_deleter(const _XSTD2 type_info& _Typeid) const
        {	// return pointer to deleter object if its type is _Typeid
        return (_Rep ? _Rep->_Get_deleter(_Typeid) : 0);
        }

    _Ty *_Get() const
        {	// return pointer to resource
        return (_Ptr);
        }

    bool _Expired() const
        {	// test if expired
        return (!_Rep || _Rep->_Expired());
        }

    void _Decref()
        {	// decrement reference count
        if (_Rep != 0)
            _Rep->_Decref();
        }

    void _Reset()
        {	// release resource
        _Reset(0, 0);
        }

    template<class _Ty2>
        void _Reset(const _Ptr_base<_Ty2>& _Other)
        {	// release resource and take ownership of _Other._Ptr
        _Reset(_Other._Ptr, _Other._Rep);
        }

    template<class _Ty2>
        void _Reset(const _Ptr_base<_Ty2>& _Other, bool _Throw)
        {	// release resource and take ownership from weak_ptr _Other._Ptr
        _Reset(_Other._Ptr, _Other._Rep, _Throw);
        }

    template<class _Ty2>
        void _Reset(const _Ptr_base<_Ty2>& _Other, const _Static_tag&)
        {	// release resource and take ownership of _Other._Ptr
        _Reset(static_cast<_Elem *>(_Other._Ptr), _Other._Rep);
        }

    template<class _Ty2>
        void _Reset(const _Ptr_base<_Ty2>& _Other, const _Const_tag&)
        {	// release resource and take ownership of _Other._Ptr
        _Reset(const_cast<_Elem *>(_Other._Ptr), _Other._Rep);
        }

    template<class _Ty2>
        void _Reset(const _Ptr_base<_Ty2>& _Other, const _Dynamic_tag&)
        {	// release resource and take ownership of _Other._Ptr
        _Elem *_Ptr = dynamic_cast<_Elem *>(_Other._Ptr);
        if (_Ptr)
            _Reset(_Ptr, _Other._Rep);
        else
            _Reset();
        }

    template<class _Ty2>
        void _Reset(auto_ptr<_Ty2>&& _Other)
        {	// release resource and take _Other.get()
        _Ty2 *_Px = _Other.get();
        _Reset0(_Px, new _Ref_count<_Elem>(_Px));
        _Other.release();
        _Enable_shared(_Px, _Rep);
        }

 #if _HAS_CPP0X
    template<class _Ty2>
        void _Reset(_Ty *_Ptr, const _Ptr_base<_Ty2>& _Other)
        {	// release resource and alias _Ptr with _Other_rep
        _Reset(_Ptr, _Other._Rep);
        }
 #endif /* _HAS_CPP0X */

    void _Reset(_Ty *_Other_ptr, _Ref_count_base *_Other_rep)
        {	// release resource and take _Other_ptr through _Other_rep
        if (_Other_rep)
            _Other_rep->_Incref();
        _Reset0(_Other_ptr, _Other_rep);
        }

    void _Reset(_Ty *_Other_ptr, _Ref_count_base *_Other_rep, bool _Throw)
        {	// take _Other_ptr through _Other_rep from weak_ptr if not expired
            // otherwise, leave in default state if !_Throw,
            // otherwise throw exception
        if (_Other_rep && _Other_rep->_Incref_nz())
            _Reset0(_Other_ptr, _Other_rep);
        else if (_Throw)
            _THROW_NCEE(bad_weak_ptr, 0);
        }

    void _Reset0(_Ty *_Other_ptr, _Ref_count_base *_Other_rep)
        {	// release resource and take new resource
        if (_Rep != 0)
            _Rep->_Decref();
        _Rep = _Other_rep;
        _Ptr = _Other_ptr;
        }

    void _Decwref()
        {	// decrement weak reference count
        if (_Rep != 0)
            _Rep->_Decwref();
        }

    void _Resetw()
        {	// release weak reference to resource
        _Resetw((_Elem *)0, 0);
        }

    template<class _Ty2>
        void _Resetw(const _Ptr_base<_Ty2>& _Other)
        {	// release weak reference to resource and take _Other._Ptr
        _Resetw(_Other._Ptr, _Other._Rep);
        }

    template<class _Ty2>
        void _Resetw(const _Ty2 *_Other_ptr, _Ref_count_base *_Other_rep)
        {	// point to _Other_ptr through _Other_rep
        _Resetw(const_cast<_Ty2*>(_Other_ptr), _Other_rep);
        }

    template<class _Ty2>
        void _Resetw(_Ty2 *_Other_ptr, _Ref_count_base *_Other_rep)
        {	// point to _Other_ptr through _Other_rep
        if (_Other_rep)
            _Other_rep->_Incwref();
        if (_Rep != 0)
            _Rep->_Decwref();
        _Rep = _Other_rep;
        _Ptr = _Other_ptr;
        }

private:
    _Ty *_Ptr;
    _Ref_count_base *_Rep;
    template<class _Ty0>
        friend class _Ptr_base;
    };

    // TEMPLATE CLASS shared_ptr
template<class _Ty>
    class shared_ptr
        : public _Ptr_base<_Ty>
    {	// class for reference counted resource management
public:
    typedef shared_ptr<_Ty> _Myt;
    typedef _Ptr_base<_Ty> _Mybase;

    shared_ptr() _NOEXCEPT
        {	// construct empty shared_ptr object
        }

    template<class _Ux>
        explicit shared_ptr(_Ux *_Px)
        {	// construct shared_ptr object that owns _Px
        _Resetp(_Px);
        }

    template<class _Ux,
        class _Dx>
        shared_ptr(_Ux *_Px, _Dx _Dt)
        {	// construct with _Px, deleter
        _Resetp(_Px, _Dt);
        }

//#if _HAS_CPP0X

    shared_ptr(nullptr_t)
        {	// construct with nullptr
        _Resetp((_Ty *)0);
        }

    template<class _Dx>
        shared_ptr(nullptr_t, _Dx _Dt)
        {	// construct with nullptr, deleter
        _Resetp((_Ty *)0, _Dt);
        }

    template<class _Dx,
        class _Alloc>
        shared_ptr(nullptr_t, _Dx _Dt, _Alloc _Ax)
        {	// construct with nullptr, deleter, allocator
        _Resetp((_Ty *)0, _Dt, _Ax);
        }

    template<class _Ux,
        class _Dx,
        class _Alloc>
        shared_ptr(_Ux *_Px, _Dx _Dt, _Alloc _Ax)
        {	// construct with _Px, deleter, allocator
        _Resetp(_Px, _Dt, _Ax);
        }
//#endif /* _HAS_CPP0X */

 #if _HAS_CPP0X
    template<class _Ty2>
        shared_ptr(const shared_ptr<_Ty2>& _Right, _Ty *_Px) _NOEXCEPT
        {	// construct shared_ptr object that aliases _Right
        this->_Reset(_Px, _Right);
        }
 #endif /* _HAS_CPP0X */

    shared_ptr(const _Myt& _Other) _NOEXCEPT
        {	// construct shared_ptr object that owns same resource as _Other
        this->_Reset(_Other);
        }

    template<class _Ty2>
        shared_ptr(const shared_ptr<_Ty2>& _Other,
            typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
                void>::type ** = 0) _NOEXCEPT
        {	// construct shared_ptr object that owns same resource as _Other
        this->_Reset(_Other);
        }

    template<class _Ty2>
        explicit shared_ptr(const weak_ptr<_Ty2>& _Other,
            bool _Throw = true)
        {	// construct shared_ptr object that owns resource *_Other
        this->_Reset(_Other, _Throw);
        }

    template<class _Ty2>
        shared_ptr(auto_ptr<_Ty2>&& _Other)
        {	// construct shared_ptr object that owns *_Other.get()
        this->_Reset(_SP_STD move(_Other));
        }

    template<class _Ty2>
        shared_ptr(const shared_ptr<_Ty2>& _Other, const _Static_tag& _Tag)
        {	// construct shared_ptr object for static_pointer_cast
        this->_Reset(_Other, _Tag);
        }

    template<class _Ty2>
        shared_ptr(const shared_ptr<_Ty2>& _Other, const _Const_tag& _Tag)
        {	// construct shared_ptr object for const_pointer_cast
        this->_Reset(_Other, _Tag);
        }

    template<class _Ty2>
        shared_ptr(const shared_ptr<_Ty2>& _Other, const _Dynamic_tag& _Tag)
        {	// construct shared_ptr object for dynamic_pointer_cast
        this->_Reset(_Other, _Tag);
        }

    shared_ptr(_Myt&& _Right) _NOEXCEPT
        : _Mybase(_SP_STD forward<_Myt>(_Right))
        {	// construct shared_ptr object that takes resource from _Right
        }

    template<class _Ty2>
        shared_ptr(shared_ptr<_Ty2>&& _Right,
            typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
                void>::type ** = 0) _NOEXCEPT
        : _Mybase(_SP_STD forward<shared_ptr<_Ty2> >(_Right))
        {	// construct shared_ptr object that takes resource from _Right
        }

 #if _HAS_CPP0X
    template<class _Ux,
        class _Dx>
        shared_ptr(unique_ptr<_Ux, _Dx>&& _Right)
        {	// construct from unique_ptr
        _Resetp(_Right.release(), _Right.get_deleter());
        }

    template<class _Ux,
        class _Dx>
        _Myt& operator=(unique_ptr<_Ux, _Dx>&& _Right)
        {	// move from unique_ptr
        shared_ptr(_SP_STD move(_Right)).swap(*this);
        return (*this);
        }
 #endif /* _HAS_CPP0X */

    _Myt& operator=(_Myt&& _Right) _NOEXCEPT
        {	// construct shared_ptr object that takes resource from _Right
        shared_ptr(_SP_STD move(_Right)).swap(*this);
        return (*this);
        }

    template<class _Ty2>
        _Myt& operator=(shared_ptr<_Ty2>&& _Right) _NOEXCEPT
        {	// construct shared_ptr object that takes resource from _Right
        shared_ptr(_SP_STD move(_Right)).swap(*this);
        return (*this);
        }

    ~shared_ptr() _NOEXCEPT
        {	// release resource
        this->_Decref();
        }

    _Myt& operator=(const _Myt& _Right) _NOEXCEPT
        {	// assign shared ownership of resource owned by _Right
        shared_ptr(_Right).swap(*this);
        return (*this);
        }

    template<class _Ty2>
        _Myt& operator=(const shared_ptr<_Ty2>& _Right) _NOEXCEPT
        {	// assign shared ownership of resource owned by _Right
        shared_ptr(_Right).swap(*this);
        return (*this);
        }

    template<class _Ty2>
        _Myt& operator=(auto_ptr<_Ty2>&& _Right)
        {	// assign ownership of resource pointed to by _Right
        shared_ptr(_SP_STD move(_Right)).swap(*this);
        return (*this);
        }

    void reset() _NOEXCEPT
        {	// release resource and convert to empty shared_ptr object
        shared_ptr().swap(*this);
        }

    template<class _Ux>
        void reset(_Ux *_Px)
        {	// release, take ownership of _Px
        shared_ptr(_Px).swap(*this);
        }

    template<class _Ux,
        class _Dx>
        void reset(_Ux *_Px, _Dx _Dt)
        {	// release, take ownership of _Px, with deleter _Dt
        shared_ptr(_Px, _Dt).swap(*this);
        }

//#if _HAS_CPP0X
    template<class _Ux,
        class _Dx,
        class _Alloc>
        void reset(_Ux *_Px, _Dx _Dt, _Alloc _Ax)
        {	// release, take ownership of _Px, with deleter _Dt, allocator _Ax
        shared_ptr(_Px, _Dt, _Ax).swap(*this);
        }
//#endif /* _HAS_CPP0X */

    void swap(_Myt& _Other) _NOEXCEPT
        {	// swap pointers
        this->_Swap(_Other);
        }

    _Ty *get() const _NOEXCEPT
        {	// return pointer to resource
        return (this->_Get());
        }

    typename add_reference<_Ty>::type operator*() const _NOEXCEPT
        {	// return reference to resource
        return (*this->_Get());
        }

    _Ty *operator->() const _NOEXCEPT
        {	// return pointer to resource
        return (this->_Get());
        }

    bool unique() const _NOEXCEPT
        {	// return true if no other shared_ptr object owns this resource
        return (this->use_count() == 1);
        }

    _TYPEDEF_BOOL_TYPE;

    _OPERATOR_BOOL() const _NOEXCEPT
        {	// test if shared_ptr object owns no resource
        return (this->_Get() != 0 ? _CONVERTIBLE_TO_TRUE : 0);
        }

private:
    template<class _Ux>
        void _Resetp(_Ux *_Px)
        {	// release, take ownership of _Px
        _TRY_BEGIN	// allocate control block and reset
        _Resetp0(_Px, new _Ref_count<_Ux>(_Px));
        _CATCH_ALL	// allocation failed, delete resource
        delete _Px;
        _RERAISE;
        _CATCH_END
        }

    template<class _Ux,
        class _Dx>
        void _Resetp(_Ux *_Px, _Dx _Dt)
        {	// release, take ownership of _Px, deleter _Dt
        _TRY_BEGIN	// allocate control block and reset
        _Resetp0(_Px, new _Ref_count_del<_Ux, _Dx>(_Px, _Dt));
        _CATCH_ALL	// allocation failed, delete resource
        _Dt(_Px);
        _RERAISE;
        _CATCH_END
        }

//#if _HAS_CPP0X
    template<class _Ux,
        class _Dx,
        class _Alloc>
        void _Resetp(_Ux *_Px, _Dx _Dt, _Alloc _Ax)
        {	// release, take ownership of _Px, deleter _Dt, allocator _Ax
        typedef _Ref_count_del_alloc<_Ux, _Dx, _Alloc> _Refd;
        typename _Alloc::template rebind<_Refd>::other _Al = _Ax;

        _TRY_BEGIN	// allocate control block and reset
        _Refd *_Ptr = _Al.allocate(1);
        ::new (_Ptr) _Refd(_Px, _Dt, _Al);
        _Resetp0(_Px, _Ptr);
        _CATCH_ALL	// allocation failed, delete resource
        _Dt(_Px);
        _RERAISE;
        _CATCH_END
        }
//#endif /* _HAS_CPP0X */

public:
    template<class _Ux>
        void _Resetp0(_Ux *_Px, _Ref_count_base *_Rx)
        {	// release resource and take ownership of _Px
        this->_Reset0(_Px, _Rx);
        _Enable_shared(_Px, _Rx);
        }
    };

template<class _Ty1,
    class _Ty2>
    bool operator==(const shared_ptr<_Ty1>& _Left,
        const shared_ptr<_Ty2>& _Right) _NOEXCEPT
    {	// test if shared_ptr == shared_ptr
    return (_Left.get() == _Right.get());
    }

template<class _Ty1,
    class _Ty2>
    bool operator!=(const shared_ptr<_Ty1>& _Left,
        const shared_ptr<_Ty2>& _Right) _NOEXCEPT
    {	// test if shared_ptr != shared_ptr
    return (!(_Left == _Right));
    }

template<class _Ty1,
    class _Ty2>
    bool operator<(const shared_ptr<_Ty1>& _Left,
        const shared_ptr<_Ty2>& _Right) _NOEXCEPT
    {	// test if shared_ptr < shared_ptr
    return (less<decltype(_Always_false<_Ty1>::value
        ? _Left.get() : _Right.get())>()(
            _Left.get(), _Right.get()));
    }

template<class _Ty1,
    class _Ty2>
    bool operator>=(const shared_ptr<_Ty1>& _Left,
        const shared_ptr<_Ty2>& _Right) _NOEXCEPT
    {	// shared_ptr >= shared_ptr
    return (!(_Left < _Right));
    }

template<class _Ty1,
    class _Ty2>
    bool operator>(const shared_ptr<_Ty1>& _Left,
        const shared_ptr<_Ty2>& _Right) _NOEXCEPT
    {	// test if shared_ptr > shared_ptr
    return (_Right < _Left);
    }

template<class _Ty1,
    class _Ty2>
    bool operator<=(const shared_ptr<_Ty1>& _Left,
        const shared_ptr<_Ty2>& _Right) _NOEXCEPT
    {	// test if shared_ptr <= shared_ptr
    return (!(_Right < _Left));
    }

template<class _Elem,
    class _Traits,
    class _Ty>
    basic_ostream<_Elem, _Traits>&
    operator<<(basic_ostream<_Elem, _Traits>& _Out,
        const shared_ptr<_Ty>& _Px)
    {	// write contained pointer to stream
    return (_Out << _Px.get());
    }

template<class _Ty>
    void swap(shared_ptr<_Ty>& _Left,
        shared_ptr<_Ty>& _Right) _NOEXCEPT
    {	// swap _Left and _Right shared_ptrs
    _Left.swap(_Right);
    }

template<class _Ty1,
    class _Ty2>
    shared_ptr<_Ty1>
        static_pointer_cast(const shared_ptr<_Ty2>& _Other) _NOEXCEPT
    {	// return shared_ptr object holding static_cast<_Ty1 *>(_Other.get())
    return (shared_ptr<_Ty1>(_Other, _Static_tag()));
    }

template<class _Ty1,
    class _Ty2>
    shared_ptr<_Ty1>
        const_pointer_cast(const shared_ptr<_Ty2>& _Other) _NOEXCEPT
    {	// return shared_ptr object holding const_cast<_Ty1 *>(_Other.get())
    return (shared_ptr<_Ty1>(_Other, _Const_tag()));
    }

template<class _Ty1,
    class _Ty2>
    shared_ptr<_Ty1>
        dynamic_pointer_cast(const shared_ptr<_Ty2>& _Other) _NOEXCEPT
    {	// return shared_ptr object holding dynamic_cast<_Ty1 *>(_Other.get())
    return (shared_ptr<_Ty1>(_Other, _Dynamic_tag()));
    }

template<class _Dx,
    class _Ty>
    _Dx *get_deleter(const shared_ptr<_Ty>& _Sx) _NOEXCEPT
    {	// return pointer to shared_ptr's deleter object if its type is _Ty
    return ((_Dx *)_Sx._Get_deleter(typeid(_Dx)));
    }

 #if _HAS_CPP0X
    // TEMPLATE CLASS _Ref_count_obj
template<class _Ty>
    class _Ref_count_obj
    : public _Ref_count_base
    {	// handle reference counting for object in control block, no allocator
public:


 //屏蔽
 #if 0


#define _REF_COUNT_OBJ_CONSTRUCT(\
    TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, X1, X2, X3, X4)\
    TEMPLATE_LIST(_CLASS_TYPE) \
        _Ref_count_obj(LIST(_TYPE_REFREF_ARG))\
        : _Ref_count_base()\
        {	/* construct from argument list */ \
        ::new ((void *)&_Storage) _Ty(LIST(_FORWARD_ARG));\
        }

_VARIADIC_EXPAND_0X(_REF_COUNT_OBJ_CONSTRUCT, , , , )
#undef _REF_COUNT_OBJ_CONSTRUCT

#endif


    _Ty *_Getptr() const
        {	// get pointer
        return ((_Ty *)&_Storage);
        }

private:
    virtual void _Destroy()
        {	// destroy managed resource
        _Getptr()->~_Ty();
        }

    virtual void _Delete_this()
        {	// destroy self
        delete this;
        }

    typename aligned_storage<sizeof (_Ty),
        alignment_of<_Ty>::value>::type _Storage;
    };

    // TEMPLATE CLASS _Ref_count_obj_alloc
template<class _Ty,
    class _Alloc>
    class _Ref_count_obj_alloc
    : public _Ref_count_base
    {	// handle reference counting for object in control block, allocator
public:
    typedef _Ref_count_obj_alloc<_Ty, _Alloc> _Myty;
    typedef typename _Alloc::template rebind<_Myty>::other _Myalty;


 //remove by lizulin
 #if 0
#define _REF_COUNT_OBJ_ALLOC_CONSTRUCT( \
    TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, X1, X2, X3, X4) \
    TEMPLATE_LIST(_CLASS_TYPE) \
        _Ref_count_obj_alloc(_Myalty _Al COMMA LIST(_TYPE_REFREF_ARG)) \
        : _Ref_count_base(), _Myal(_Al) \
        {	/* construct from argument list, allocator */ \
        ::new ((void *)&_Storage) _Ty(LIST(_FORWARD_ARG)); \
        }

_VARIADIC_EXPAND_0X(_REF_COUNT_OBJ_ALLOC_CONSTRUCT, , , , )
#undef _REF_COUNT_OBJ_ALLOC_CONSTRUCT
#endif


    _Ty *_Getptr() const
        {	// get pointer
        return ((_Ty *)&_Storage);
        }

private:
    virtual void _Destroy()
        {	// destroy managed resource
        _Getptr()->~_Ty();
        }

    virtual void _Delete_this()
        {	// destroy self
        _Myalty _Al = _Myal;
        _Al.destroy(this);
        _Al.deallocate(this, 1);
        }

    typename aligned_storage<sizeof (_Ty),
        alignment_of<_Ty>::value>::type _Storage;
    _Myalty _Myal;	// the stored allocator for this
    };

    // TEMPLATE FUNCTIONS make_shared and allocate_shared
#define _ALLOCATE_MAKE_SHARED( \
    TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, X1, X2, X3, X4) \
template<class _Ty COMMA LIST(_CLASS_TYPE)> inline \
    shared_ptr<_Ty> make_shared(LIST(_TYPE_REFREF_ARG)) \
    {	/* make a shared_ptr */ \
    _Ref_count_obj<_Ty> *_Rx = \
        new _Ref_count_obj<_Ty>(LIST(_FORWARD_ARG)); \
    shared_ptr<_Ty> _Ret; \
    _Ret._Resetp0(_Rx->_Getptr(), _Rx); \
    return (_Ret); \
    } \
template<class _Ty, \
    class _Alloc COMMA LIST(_CLASS_TYPE)> inline \
    shared_ptr<_Ty> allocate_shared( \
        const _Alloc& _Al_arg COMMA LIST(_TYPE_REFREF_ARG)) \
    {	/* make a shared_ptr */ \
    typedef _Ref_count_obj_alloc<_Ty, _Alloc> _Refoa; \
    typename _Alloc::template rebind<_Refoa>::other _Alref = _Al_arg; \
    _Refoa *_Rx = _Alref.allocate(1); \
    _TRY_BEGIN \
        ::new (_Rx) _Refoa(_Al_arg COMMA LIST(_FORWARD_ARG)); \
    _CATCH_ALL \
        _Alref.deallocate(_Rx, 1); \
    _RERAISE; \
    _CATCH_END \
    shared_ptr<_Ty> _Ret; \
    _Ret._Resetp0(_Rx->_Getptr(), _Rx); \
    return (_Ret); \
    }

 //remove by lizulin
#if 0

_VARIADIC_EXPAND_0X(_ALLOCATE_MAKE_SHARED, , , , )
#undef _ALLOCATE_MAKE_SHARED
 #endif /* _HAS_CPP0X */
#endif


    // TEMPLATE CLASS weak_ptr
template<class _Ty>
    class weak_ptr
        : public _Ptr_base<_Ty>
    {	// class for pointer to reference counted resource
    typedef typename _Ptr_base<_Ty>::_Elem _Elem;

public:
    weak_ptr() _NOEXCEPT
        {	// construct empty weak_ptr object
        }

    template<class _Ty2>
        weak_ptr(const shared_ptr<_Ty2>& _Other,
            typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
                void>::type ** = 0) _NOEXCEPT
        {	// construct weak_ptr object for resource owned by _Other
        this->_Resetw(_Other);
        }

    weak_ptr(const weak_ptr& _Other) _NOEXCEPT
        {	// construct weak_ptr object for resource pointed to by _Other
        this->_Resetw(_Other);
        }

    template<class _Ty2>
        weak_ptr(const weak_ptr<_Ty2>& _Other,
            typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
                void>::type ** = 0) _NOEXCEPT
        {	// construct weak_ptr object for resource pointed to by _Other
        this->_Resetw(_Other.lock());
        }

    ~weak_ptr() _NOEXCEPT
        {	// release resource
        this->_Decwref();
        }

    weak_ptr& operator=(const weak_ptr& _Right) _NOEXCEPT
        {	// assign from _Right
        this->_Resetw(_Right);
        return (*this);
        }

    template<class _Ty2>
        weak_ptr& operator=(const weak_ptr<_Ty2>& _Right) _NOEXCEPT
        {	// assign from _Right
        this->_Resetw(_Right.lock());
        return (*this);
        }

    template<class _Ty2>
        weak_ptr& operator=(const shared_ptr<_Ty2>& _Right) _NOEXCEPT
        {	// assign from _Right
        this->_Resetw(_Right);
        return (*this);
        }

    void reset() _NOEXCEPT
        {	// release resource, convert to null weak_ptr object
        this->_Resetw();
        }

    void swap(weak_ptr& _Other) _NOEXCEPT
        {	// swap pointers
        this->_Swap(_Other);
        }

    bool expired() const _NOEXCEPT
        {	// return true if resource no longer exists
        return (this->_Expired());
        }

    shared_ptr<_Ty> lock() const _NOEXCEPT
        {	// convert to shared_ptr
        return (shared_ptr<_Elem>(*this, false));
        }
    };

//template<class _Ty1,
//	class _Ty2>
//	bool operator<(const weak_ptr<_Ty1>& _W1,
//		const weak_ptr<_Ty2>& _W2)
//	{	// return true if _W1 precedes _W2 (order defined by control block)
//	return (_W1.owner_before(_W2));
//	}

template<class _Ty>
    void swap(weak_ptr<_Ty>& _W1, weak_ptr<_Ty>& _W2) _NOEXCEPT
    {	// swap contents of _W1 and _W2
    _W1.swap(_W2);
    }

    // TEMPLATE CLASS enable_shared_from_this
template<class _Ty>
    class enable_shared_from_this
    {	// provide member functions that create shared_ptr to this
public:
    typedef _Ty _EStype;

    shared_ptr<_Ty> shared_from_this()
        {	// return shared_ptr
        return (shared_ptr<_Ty>(_Wptr));
        }

    shared_ptr<const _Ty> shared_from_this() const
        {	// return shared_ptr
        return (shared_ptr<const _Ty>(_Wptr));
        }

protected:
    enable_shared_from_this() _NOEXCEPT
        {	// construct (do nothing)
        }

    enable_shared_from_this(const enable_shared_from_this&) _NOEXCEPT
        {	// construct (do nothing)
        }

    enable_shared_from_this&
        operator=(const enable_shared_from_this&) _NOEXCEPT
        {	// assign (do nothing)
        return (*this);
        }

    ~enable_shared_from_this() _NOEXCEPT
        {	// destroy (do nothing)
        }

private:
    template<class _Ty1,
        class _Ty2>
        friend void _Do_enable(
            _Ty1 *,
            enable_shared_from_this<_Ty2>*,
            _Ref_count_base *);

    mutable weak_ptr<_Ty> _Wptr;
    };

template<class _Ty1,
    class _Ty2>
    inline void _Do_enable(
        _Ty1 *_Ptr,
        enable_shared_from_this<_Ty2> *_Es,
        _Ref_count_base *_Refptr)
    {	// reset internal weak pointer
    _Es->_Wptr._Resetw(_Ptr, _Refptr);
    }



//_STD_END
_SUPER_END


 #if _HAS_CPP0X


//_STD_BEGIN
 _SUPER_BEGIN

    // TEMPLATE CLASS unique_pointer AND HELPERS

    // TEMPLATE CLASS default_delete
template<class _Ty>
    struct default_delete
    {	// default deleter for unique_ptr
    typedef default_delete<_Ty> _Myt;

    default_delete() _NOEXCEPT
        {	// default construct
        }

    template<class _Ty2>
        default_delete(const default_delete<_Ty2>&,
            typename enable_if<is_convertible<_Ty2 *, _Ty *>::value,
                void>::type ** = 0) _NOEXCEPT
        {	// construct from another default_delete
        }

    void operator()(_Ty *_Ptr) const _NOEXCEPT
        {	// delete a pointer
        static_assert(0 < sizeof (_Ty),
            "can't delete an incomplete type");
        delete _Ptr;
        }
    };

template<class _Ty>
    struct default_delete<_Ty[]>
    {	// default deleter for unique_ptr to array of unknown size
    typedef default_delete<_Ty> _Myt;

    default_delete() _NOEXCEPT
        {	// default construct
        }

    template<class _Other>
        void operator()(_Other *) const;	// not defined

    void operator()(_Ty *_Ptr) const _NOEXCEPT
        {	// delete a pointer
        static_assert(0 < sizeof (_Ty),
            "can't delete an incomplete type");
        delete[] _Ptr;
        }
    };

        // TEMPLATE STRUCT _Get_deleter_pointer_type
template<class _Val,
    class _Ty>
    struct _Get_deleter_pointer_type
    _GET_TYPE_OR_DEFAULT(pointer,
        _Val *);

    // TEMPLATE CLASS _Unique_ptr_base
template<class _Ty,
    class _Dx,
    bool _Empty_deleter>
    class _Unique_ptr_base
    {	// stores pointer and deleter
public:
    typedef typename remove_reference<_Dx>::type _Dx_noref;
    typedef typename _Get_deleter_pointer_type<_Ty, _Dx_noref>::type pointer;

    _Unique_ptr_base(pointer _Ptr, _Dx _Dt)
        : _Myptr(_Ptr), _Mydel(_Dt)
        {	// construct with pointer and deleter
        }

    _Unique_ptr_base(pointer _Ptr)
        : _Myptr(_Ptr)
        {	// construct with pointer and deleter
        }

    template<class _Ptr2,
        class _Dx2>
        _Unique_ptr_base(_Ptr2 _Ptr, _Dx2 _Dt)
        : _Myptr(_Ptr), _Mydel(_Dt)
        {	// construct with compatible pointer and deleter
        }

    template<class _Ptr2>
        _Unique_ptr_base(_Ptr2 _Ptr)
        : _Myptr(_Ptr)
        {	// construct with compatible pointer and deleter
        }

    _Dx_noref& get_deleter()
        {	// return reference to deleter
        return (_Mydel);
        }

    const _Dx_noref& get_deleter() const
        {	// return const reference to deleter
        return (_Mydel);
        }

    pointer _Myptr;	// the managed pointer
    _Dx _Mydel;		// the deleter
    };

template<class _Ty,
    class _Dx>
    class _Unique_ptr_base<_Ty, _Dx, true>
        : public _Dx
    {	// store pointer and empty deleter
public:
    typedef _Dx _Mybase;
    typedef typename remove_reference<_Dx>::type _Dx_noref;
    typedef typename _Get_deleter_pointer_type<_Ty, _Dx_noref>::type pointer;

    _Unique_ptr_base(pointer _Ptr, _Dx _Dt) _NOEXCEPT
        : _Myptr(_Ptr), _Mybase(_Dt)
        {	// construct with pointer and deleter
        }

    _Unique_ptr_base(pointer _Ptr) _NOEXCEPT
        : _Myptr(_Ptr)
        {	// construct with pointer and deleter
        }

    template<class _Ptr2,
        class _Dx2>
        _Unique_ptr_base(_Ptr2 _Ptr, _Dx2 _Dt) _NOEXCEPT
        : _Myptr(_Ptr), _Mybase(_Dt)
        {	// construct with compatible pointer and deleter
        }

    template<class _Ptr2>
        _Unique_ptr_base(_Ptr2 _Ptr) _NOEXCEPT
        : _Myptr(_Ptr)
        {	// construct with compatible pointer and deleter
        }

    _Dx_noref& get_deleter() _NOEXCEPT
        {	// return reference to deleter
        return (*this);
        }

    const _Dx_noref& get_deleter() const _NOEXCEPT
        {	// return const reference to deleter
        return (*this);
        }

    pointer _Myptr;	// the managed pointer
    };

    // TEMPLATE CLASS unique_ptr SCALAR
template<class _Ty,
    class _Dx>	// = default_delete<_Ty>
    class unique_ptr
        : private _Unique_ptr_base<_Ty, _Dx,
            is_empty<_Dx>::value
                || is_same<default_delete<_Ty>, _Dx>::value>
    {	// non-copyable pointer to an object
public:
    typedef unique_ptr<_Ty, _Dx> _Myt;
    typedef _Unique_ptr_base<_Ty, _Dx,
        is_empty<_Dx>::value
            || is_same<default_delete<_Ty>, _Dx>::value> _Mybase;
    typedef typename _Mybase::pointer pointer;
    typedef _Ty element_type;
    typedef _Dx deleter_type;

    using _Mybase::get_deleter;

    unique_ptr() _NOEXCEPT
        : _Mybase(pointer())
        {	// default construct
        static_assert(!is_pointer<_Dx>::value,
            "unique_ptr constructed with null deleter pointer");
        }

    unique_ptr(nullptr_t) _NOEXCEPT
        : _Mybase(pointer())
        {	// null pointer construct
        static_assert(!is_pointer<_Dx>::value,
            "unique_ptr constructed with null deleter pointer");
        }

    _Myt& operator=(nullptr_t) _NOEXCEPT
        {	// assign a null pointer
        reset();
        return (*this);
        }

    explicit unique_ptr(pointer _Ptr) _NOEXCEPT
        : _Mybase(_Ptr)
        {	// construct with pointer
        static_assert(!is_pointer<_Dx>::value,
            "unique_ptr constructed with null deleter pointer");
        }

    unique_ptr(pointer _Ptr,
        typename _If<is_reference<_Dx>::value, _Dx,
            const typename remove_reference<_Dx>::type&>::type _Dt) _NOEXCEPT
        : _Mybase(_Ptr, _Dt)
        {	// construct with pointer and (maybe const) deleter&
        }

    unique_ptr(pointer _Ptr,
        typename remove_reference<_Dx>::type&& _Dt) _NOEXCEPT
        : _Mybase(_Ptr, _SP_STD move(_Dt))
        {	// construct by moving deleter
//		static_assert(!is_reference<_Dx>::value,
//			"unique_ptr constructed with reference to rvalue deleter");
        }

    unique_ptr(unique_ptr&& _Right) _NOEXCEPT
        : _Mybase(_Right.release(),
            _SP_STD forward<_Dx>(_Right.get_deleter()))
        {	// construct by moving _Right
        }

    template<class _Ty2,
        class _Dx2>
        unique_ptr(unique_ptr<_Ty2, _Dx2>&& _Right,
            typename enable_if<!is_array<_Ty2>::value
                && is_convertible<typename unique_ptr<_Ty2, _Dx2>::pointer,
                    pointer>::value
                && (is_reference<_Dx>::value && is_same<_Dx, _Dx2>::value
                    || !is_reference<_Dx>::value
                        && is_convertible<_Dx2, _Dx>::value),
                void>::type ** = 0) _NOEXCEPT
            : _Mybase(_Right.release(),
                _SP_STD forward<_Dx2>(_Right.get_deleter()))
        {	// construct by moving _Right
        }

    template<class _Ty2>
        unique_ptr(auto_ptr<_Ty2>&& _Right,
            typename enable_if<is_convertible<_Ty2 *, _Ty *>::value
                && is_same<_Dx, default_delete<_Ty> >::value,
                void>::type ** = 0) _NOEXCEPT
            : _Mybase(_Right.release())
        {	// construct by moving _Right
        }

    template<class _Ty2,
        class _Dx2>
        typename enable_if<!is_array<_Ty2>::value
            && is_convertible<typename unique_ptr<_Ty2, _Dx2>::pointer,
                pointer>::value,
            _Myt&>::type
        operator=(unique_ptr<_Ty2, _Dx2>&& _Right) _NOEXCEPT
        {	// assign by moving _Right
        reset(_Right.release());
        this->get_deleter() = _SP_STD forward<_Dx2>(_Right.get_deleter());
        return (*this);
        }

    _Myt& operator=(_Myt&& _Right) _NOEXCEPT
        {	// assign by moving _Right
        if (this != &_Right)
            {	// different, do the move
            reset(_Right.release());
            this->get_deleter() = _SP_STD forward<_Dx>(_Right.get_deleter());
            }
        return (*this);
        }

    void swap(_Myt& _Right) _NOEXCEPT
        {	// swap elements
            _Swap_adl(this->_Myptr, _Right._Myptr);
            _Swap_adl(this->get_deleter(),
            _Right.get_deleter());
        }

    ~unique_ptr() _NOEXCEPT
        {	// destroy the object
        _Delete();
        }

    typename add_reference<_Ty>::type operator*() const
        {	// return reference to object
        return (*this->_Myptr);
        }

    pointer operator->() const _NOEXCEPT
        {	// return pointer to class object
        return (_SP_STD pointer_traits<pointer>::pointer_to(**this));
        }

    pointer get() const _NOEXCEPT
        {	// return pointer to object
        return (this->_Myptr);
        }

    _TYPEDEF_BOOL_TYPE;

    _OPERATOR_BOOL() const _NOEXCEPT
        {	// test for non-null pointer
        return (this->_Myptr != pointer() ? _CONVERTIBLE_TO_TRUE : 0);
        }

    pointer release() _NOEXCEPT
        {	// yield ownership of pointer
        pointer _Ans = this->_Myptr;
        this->_Myptr = pointer();
        return (_Ans);
        }

    void reset(pointer _Ptr = pointer()) _NOEXCEPT
        {	// establish new pointer
        if (_Ptr != this->_Myptr)
            {	// different pointer, delete old and reassign
            _Delete();
            this->_Myptr = _Ptr;
            }
        }

private:
    void _Delete()
        {	// delete the pointer
        if (this->_Myptr != pointer())
            this->get_deleter()(this->_Myptr);
        }

    unique_ptr(const _Myt&);	// not defined
    _Myt& operator=(const _Myt&);	// not defined
    };

    // TEMPLATE CLASS unique_ptr ARRAY
template<class _Ty,
    class _Dx>
    class unique_ptr<_Ty[], _Dx>
        : private _Unique_ptr_base<_Ty, _Dx,
            is_empty<_Dx>::value
                || is_same<default_delete<_Ty[]>, _Dx>::value>
    {	// non-copyable pointer to an array object
public:
    typedef unique_ptr<_Ty[], _Dx> _Myt;
    typedef _Unique_ptr_base<_Ty, _Dx,
        is_empty<_Dx>::value
            || is_same<default_delete<_Ty[]>, _Dx>::value> _Mybase;
    typedef typename _Mybase::pointer pointer;
    typedef _Ty element_type;
    typedef _Dx deleter_type;

    using _Mybase::get_deleter;

    unique_ptr() _NOEXCEPT
        : _Mybase(pointer())
        {	// default construct
        static_assert(!is_pointer<_Dx>::value,
            "unique_ptr constructed with null deleter pointer");
        }

    explicit unique_ptr(pointer _Ptr) _NOEXCEPT
        : _Mybase(_Ptr)
        {	// construct with pointer
        static_assert(!is_pointer<_Dx>::value,
            "unique_ptr constructed with null deleter pointer");
        }

    unique_ptr(pointer _Ptr,
        typename _If<is_reference<_Dx>::value, _Dx,
            const typename remove_reference<_Dx>::type&>::type _Dt) _NOEXCEPT
        : _Mybase(_Ptr, _Dt)
        {	// construct with pointer and (maybe const) deleter&
        }

public:
    unique_ptr(pointer _Ptr,
        typename remove_reference<_Dx>::type&& _Dt) _NOEXCEPT
        : _Mybase(_Ptr, _SP_STD move(_Dt))
        {	// construct by moving deleter
//		static_assert(!is_reference<_Dx>::value,
//			"unique_ptr constructed with reference to rvalue deleter");
        }

    unique_ptr(unique_ptr&& _Right) _NOEXCEPT
        : _Mybase(_Right.release(),
            _SP_STD forward<_Dx>(_Right.get_deleter()))
        {	// construct by moving _Right
        }

private:
    template<class _Ty2,
        class _Dx2>
        unique_ptr(unique_ptr<_Ty2, _Dx2>&& _Right);	// not defined

    template<class _Ty2,
        class _Dx2>
        _Myt& operator=(unique_ptr<_Ty2, _Dx2>&& _Right);	// not defined

public:
    _Myt& operator=(_Myt&& _Right) _NOEXCEPT
        {	// assign by moving _Right
        if (this != &_Right)
            {	// different, do the swap
            reset(_Right.release());
            this->get_deleter() = _SP_STD move(_Right.get_deleter());
            }
        return (*this);
        }

    unique_ptr(nullptr_t) _NOEXCEPT
        : _Mybase(pointer())
        {	// null pointer construct
        static_assert(!is_pointer<_Dx>::value,
            "unique_ptr constructed with null deleter pointer");
        }

    _Myt& operator=(nullptr_t) _NOEXCEPT
        {	// assign a null pointer
        reset();
        return (*this);
        }

    void reset(nullptr_t) _NOEXCEPT
        {	// establish new null pointer
        if (this->_Myptr != 0)
            {	// different pointer, delete old and reassign
            _Delete();
            this->_Myptr = 0;
            }
        }

    void swap(_Myt& _Right) _NOEXCEPT
        {	// swap elements
            _Swap_adl(this->_Myptr, _Right._Myptr);
            _Swap_adl(this->get_deleter(), _Right.get_deleter());
        }

    ~unique_ptr() _NOEXCEPT
        {	// destroy the object
        _Delete();
        }

    typename add_reference<_Ty>::type operator[](size_t _Idx) const
        {	// return reference to object
        return (this->_Myptr[_Idx]);
        }

    pointer get() const _NOEXCEPT
        {	// return pointer to object
        return (this->_Myptr);
        }

    _TYPEDEF_BOOL_TYPE;

    _OPERATOR_BOOL() const _NOEXCEPT
        {	// test for non-null pointer
        return (this->_Myptr != 0 ? _CONVERTIBLE_TO_TRUE : 0);
        }

    pointer release() _NOEXCEPT
        {	// yield ownership of pointer
        pointer _Ans = this->_Myptr;
        this->_Myptr = pointer();
        return (_Ans);
        }

    void reset(pointer _Ptr = pointer()) _NOEXCEPT
        {	// establish new pointer
        if (_Ptr != this->_Myptr)
            {	// different pointer, delete old and reassign
            _Delete();
            this->_Myptr = _Ptr;
            }
        }

private:
    template<class _Ptr2>
        explicit unique_ptr(_Ptr2);	// not defined

    template<class _Ptr2,
        class _Dx2>
        unique_ptr(_Ptr2, _Dx2);	// not defined

    unique_ptr(const _Myt&);	// not defined
    template<class _Ty2,
        class _Dx2>
        unique_ptr(const unique_ptr<_Ty2, _Dx2>&);	// not defined

    _Myt& operator=(const _Myt&);	// not defined
    template<class _Ty2,
        class _Dx2>
        _Myt& operator=(const unique_ptr<_Ty2, _Dx2>&);	// not defined

    template<class _Ptr2>
        void reset(_Ptr2);	// not defined

    void _Delete()
        {	// delete the pointer
        this->get_deleter()(this->_Myptr);
        }
    };

template<class _Ty,
    class _Dx>
    void swap(unique_ptr<_Ty, _Dx>& _Left,
        unique_ptr<_Ty, _Dx>& _Right) _NOEXCEPT
    {	// swap _Left with _Right
    _Left.swap(_Right);
    }

template<class _Ty1,
    class _Dx1,
    class _Ty2,
    class _Dx2>
    bool operator==(const unique_ptr<_Ty1, _Dx1>& _Left,
        const unique_ptr<_Ty2, _Dx2>& _Right)
    {	// test if unique_ptr _Left equals _Right
    return (_Left.get() == _Right.get());
    }

template<class _Ty1,
    class _Dx1,
    class _Ty2,
    class _Dx2>
    bool operator!=(const unique_ptr<_Ty1, _Dx1>& _Left,
        const unique_ptr<_Ty2, _Dx2>& _Right)
    {	// test if unique_ptr _Left doesn't equal _Right
    return (!(_Left == _Right));
    }

template<class _Ty1,
    class _Dx1,
    class _Ty2,
    class _Dx2>
    bool operator<(const unique_ptr<_Ty1, _Dx1>& _Left,
        const unique_ptr<_Ty2, _Dx2>& _Right)
    {	// test if unique_ptr _Left precedes _Right
    return (less<decltype(_Always_false<_Ty1>::value
        ? _Left.get() : _Right.get())>()(
            _Left.get(), _Right.get()));
    }

template<class _Ty1,
    class _Dx1,
    class _Ty2,
    class _Dx2>
    bool operator>=(const unique_ptr<_Ty1, _Dx1>& _Left,
        const unique_ptr<_Ty2, _Dx2>& _Right)
    {	// test if unique_ptr _Left doesn't precede _Right
    return (!(_Left < _Right));
    }

template<class _Ty1,
    class _Dx1,
    class _Ty2,
    class _Dx2>
    bool operator>(const unique_ptr<_Ty1, _Dx1>& _Left,
        const unique_ptr<_Ty2, _Dx2>& _Right)
    {	// test if unique_ptr _Right precedes _Left
    return (_Right < _Left);
    }

template<class _Ty1,
    class _Dx1,
    class _Ty2,
    class _Dx2>
    bool operator<=(const unique_ptr<_Ty1, _Dx1>& _Left,
        const unique_ptr<_Ty2, _Dx2>& _Right)
    {	// test if unique_ptr _Right doesn't precede _Left
    return (!(_Right < _Left));
    }

        // GARBAGE COLLECTION

    namespace pointer_safety {
enum pointer_safety {	// names for levels of pointer safety
    relaxed,
    preferred,
    strict
    };
    }	// namespace pointer_safety

typedef pointer_safety::pointer_safety _Pointer_safety;

inline void declare_reachable(void *)
    {	// increment pointer reachable count
    }

template<class _Ty> inline
    _Ty *undeclare_reachable(_Ty *_Ptr)
    {	// decrement pointer reachable count
    return (_Ptr);
    }

inline void declare_no_pointers(char *, size_t)
    {	// declare region to be pointer free
    }

inline void undeclare_no_pointers(char *, size_t)
    {	// undeclare region to be pointer free
    }

inline _Pointer_safety get_pointer_safety() _NOEXCEPT
    {	// get pointer safety status
    return (pointer_safety::relaxed);
    }



#if 0



namespace tr1 {	// TR1 additions
using _STD allocate_shared;
using _STD bad_weak_ptr;
using _STD const_pointer_cast;
using _STD dynamic_pointer_cast;
using _STD enable_shared_from_this;
using _STD get_deleter;
using _STD make_shared;
using _STD shared_ptr;
using _STD static_pointer_cast;
using _STD swap;
using _STD weak_ptr;
}	// namespace tr1

        // TEMPLATE STRUCT owner_less
template<class _Ty>
    struct owner_less;	// not defined

template<class _Ty>
    struct owner_less<shared_ptr<_Ty> >
        : public binary_function<shared_ptr<_Ty>, shared_ptr<_Ty>, bool>
    {	// functor for owner_before
    bool operator()(const shared_ptr<_Ty>& _Left,
        const shared_ptr<_Ty>& _Right) const
        {	// apply owner_before to operands
        return (_Left.owner_before(_Right));
        }

    bool operator()(const shared_ptr<_Ty>& _Left,
        const weak_ptr<_Ty>& _Right) const
        {	// apply owner_before to operands
        return (_Left.owner_before(_Right));
        }

    bool operator()(const weak_ptr<_Ty>& _Left,
        const shared_ptr<_Ty>& _Right) const
        {	// apply owner_before to operands
        return (_Left.owner_before(_Right));
        }
    };

template<class _Ty>
    struct owner_less<weak_ptr<_Ty> >
        : public binary_function<weak_ptr<_Ty>, weak_ptr<_Ty>, bool>
    {	// functor for owner_before
    bool operator()(const weak_ptr<_Ty>& _Left,
        const weak_ptr<_Ty>& _Right) const
        {	// apply owner_before to operands
        return (_Left.owner_before(_Right));
        }

    bool operator()(const weak_ptr<_Ty>& _Left,
        const shared_ptr<_Ty>& _Right) const
        {	// apply owner_before to operands
        return (_Left.owner_before(_Right));
        }

    bool operator()(const shared_ptr<_Ty>& _Left,
        const weak_ptr<_Ty>& _Right) const
        {	// apply owner_before to operands
        return (_Left.owner_before(_Right));
        }
    };

    // TEMPLATE STRUCT SPECIALIZATION hash
template<class _Ty,
    class _Dx>
    struct hash<unique_ptr<_Ty, _Dx> >
        : public unary_function<unique_ptr<_Ty, _Dx>, size_t>
    {	// hash functor
    typedef unique_ptr<_Ty, _Dx> _Kty;
    typedef typename _Kty::pointer _Ptrtype;

    size_t operator()(const _Kty& _Keyval) const
        {	// hash _Keyval to size_t value by pseudorandomizing transform
        return (hash<_Ptrtype>()(_Keyval.get()));
        }
    };

template<class _Ty>
    struct hash<shared_ptr<_Ty> >
        : public unary_function<shared_ptr<_Ty>, size_t>
    {	// hash functor
    typedef shared_ptr<_Ty> _Kty;
    typedef _Ty *_Ptrtype;

    size_t operator()(const _Kty& _Keyval) const
        {	// hash _Keyval to size_t value by pseudorandomizing transform
        return (hash<_Ptrtype>()(_Keyval.get()));
        }
    };

        // FUNCTION align
inline void *align(size_t _Bound, size_t _Size,
    void *& _Ptr, size_t& _Space) _NOEXCEPT
    {	// try to carve out _Size bytes on boundary _Bound
    size_t _Off = _Bound == 0 ? 0 : (size_t)((uintptr_t)_Ptr & (_Bound - 1));
    if (0 < _Off)
        _Off = _Bound - _Off;	// number of bytes to skip
    if (_Ptr == 0 || _Space < _Off || _Space - _Off < _Size)
        return (0);
    else
        {	// enough room, update
        char *_Ans = (char *)_Ptr + _Off;
        _Ptr = (void *)(_Ans + _Size);
        _Space -= _Off + _Size;
        return ((void *)_Ans);
        }
    }

        // WRAP SPIN-LOCK
struct _Shared_ptr_spin_lock
    {	// class to manage a spin lock for shared_ptr atomic operations
    _Shared_ptr_spin_lock()
        {	// lock the spin lock
        _Lock_shared_ptr_spin_lock();
        }

    ~_Shared_ptr_spin_lock() _NOEXCEPT
        {	// unlock the spin lock
        _Unlock_shared_ptr_spin_lock();
        }
    };

template <class _Ty> inline
    bool atomic_is_lock_free(const shared_ptr<_Ty> *)
    {	// return true if atomic operations on shared_ptr<_Ty> are lock-free
    return (false);
    }

template <class _Ty> inline
    shared_ptr<_Ty> atomic_load_explicit(const shared_ptr<_Ty> *_Ptr,
        memory_order _Order)
    {	// load *_Ptr atomically
    _Shared_ptr_spin_lock _Lock;
    shared_ptr<_Ty> _Result = *_Ptr;
    return (_Result);
    }

template <class _Ty> inline
    shared_ptr<_Ty> atomic_load(const shared_ptr<_Ty> *_Ptr)
    {	// load *_Ptr atomically
    return (atomic_load_explicit(_Ptr, memory_order_seq_cst));
    }

template <class _Ty> inline
    void atomic_store_explicit(shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> _Other,
        memory_order _Order)
    {	// store _Other to *_Ptr atomically
    _Shared_ptr_spin_lock _Lock;
    _Ptr->swap(_Other);
    }

template <class _Ty> inline
    void atomic_store(shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> _Other)
    {	// store _Other to *_Ptr atomically
    atomic_store_explicit(_Ptr, _Other, memory_order_seq_cst);
    }

template <class _Ty> inline
    shared_ptr<_Ty> atomic_exchange_explicit(
        shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> _Other,
        memory_order _Order)
    {	// copy _Other to *_Ptr and return previous value of *_Ptr atomically
    _Shared_ptr_spin_lock _Lock;
    shared_ptr<_Ty> _Result = *_Ptr;
    _Ptr->swap(_Other);
    return (_Result);
    }

template <class _Ty> inline
    shared_ptr<_Ty> atomic_exchange(
        shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> _Other)
    {	// copy _Other to *_Ptr and return previous value of *_Ptr atomically
    return (atomic_exchange_explicit(_Ptr, _Other, memory_order_seq_cst));
    }

template <class _Ty> inline
    bool atomic_compare_exchange_weak_explicit(
        shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> *_Exp, shared_ptr<_Ty> _Value,
        memory_order _Order1, memory_order _Order2)
    {	// atomically compare and exchange
    _Shared_ptr_spin_lock _Lock;
    bool _Success = _Ptr->get() == _Exp->get();
    if (_Success)
        *_Ptr = _Value;
    else
        {	// match failed
        *_Exp = *_Ptr;
        }
    return (_Success);
    }

template <class _Ty> inline
    bool atomic_compare_exchange_weak(
        shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> *_Exp,
            shared_ptr<_Ty> _Value)
    {	// atomically compare and exchange
    return (atomic_compare_exchange_weak_explicit(_Ptr, _Exp, _Value,
        memory_order_seq_cst, memory_order_seq_cst));
    }

template <class _Ty> inline
    bool atomic_compare_exchange_strong_explicit(
        shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> *_Exp, shared_ptr<_Ty> _Value,
        memory_order, memory_order)
    {	// atomically compare and exchange
    return (atomic_compare_exchange_weak_explicit(_Ptr, _Exp, _Value,
        memory_order_seq_cst, memory_order_seq_cst));
    }

template <class _Ty> inline
    bool atomic_compare_exchange_strong(
    shared_ptr<_Ty> *_Ptr, shared_ptr<_Ty> *_Exp,
        shared_ptr<_Ty> _Value)
    {	// atomically compare and exchange
    return (atomic_compare_exchange_strong_explicit(_Ptr, _Exp, _Value,
        memory_order_seq_cst, memory_order_seq_cst));
    }
#endif



//_STD_END
_SUPER_END


 #endif /* _HAS_CPP0X */

 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _MEMORY_ */

/*
 * Copyright (c) 1992-2012 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V6.00:0009 */
