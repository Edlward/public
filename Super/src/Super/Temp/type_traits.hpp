#ifndef CXXOMFORT_UTIL_TYPETRAITS_HPP
#define CXXOMFORT_UTIL_TYPETRAITS_HPP
/**
 * @brief Implements some of the type_traits additions in C++03 for internal use.
 * @author Luis Machuca Bezzaza <luis [dot] machuca [at] gulix [dot] cl>
 *
 * This file implements some of the <type_traits> to remove dependencies
 * when using this library.
 *
 * Interfaces defined in this file
 * follow the proposal in n1424:
 * 
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2003/n1424.htm
 * 
 *
 * integral_constant, true_type, false_type,
 * is_void, is_integral, is_floating_point
 * is_array
 * is_pointer, is_reference
 * is_member_object_pointer, is_member_function_pointer
 * is_enum, is_union, is_class
 * is_function
 * 
 * is_arithmetic
 * is_fundamental
 * is_object
 * is_scalar
 * is_compound
 * is_member_pointer
 * 
 * is_const, is_volatile
 * is_pod
 * is_empty, is_polymorphic, is_abstract
 * is_signed, is_unsigned
 * 
 * 
 * add_const, remove_const, add_volatile, remove_volatile, add_cv, remove_cv,
 * add_pointer, remove_pointer, add_reference, remove_reference,
 * rank, extent, 
 * make_signed, make_unsigned
 * is_empty
 *
 * The following traits are handled via compiler intrinsics:
 * is_class, is_enum, is_pod, is_union, is_empty
 * (they are needed for stuff like scoped enum emulation and move emulation)
 *
 */

#include "../config.hpp"
#include "../base/nullptr.hpp"
//#include "meta.hpp"

namespace cxxomfort {
namespace traits {

namespace helper {

    template <bool Cond, typename T1, typename T2> struct conditional        { typedef T2 type; };
    template <typename T1, typename T2> struct conditional<true,T1,T2>    { typedef T1 type; };

} // helper

// this macro sets up a cv-specialization as true for the given type
#define TR1LITE_DEF_SPECIALIZE_TRUE(name,arg) template <> struct name < arg > : true_type {};
#define TR1LITE_DEF_SPECIALIZE_CV_TRUE(name,arg) \
template <> struct name < arg > : true_type {}; \
template <> struct name < arg const > : true_type {}; \
template <> struct name < arg volatile > : true_type {}; \
template <> struct name < arg const volatile > : true_type {}; \




/**
 * integral_constant
 */
template <typename Integral, Integral I> struct integral_constant {
    typedef Integral value_type;
    typedef integral_constant<Integral,I> type;
    static const Integral value= I;
    CXXOMFORT_CXX11_CODE(constexpr,) operator value_type () const { return value; }
    CXXOMFORT_CXX11_CODE(constexpr,) value_type operator() () const { return value; } // from C++14
};

//! true_type
struct false_type : integral_constant<bool,false> {};
//! false_type
struct true_type : integral_constant<bool,true> {};



//
// is_void
//
template <typename T> struct is_void : false_type {};
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_void,void);

//
// is_null_pointer (c++14)
//
template <typename T> struct is_null_pointer : false_type {};
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_null_pointer,std::nullptr_t);


//
// is_integral
//
template <typename T> struct is_integral : false_type {};
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,int);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,unsigned int);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,short int);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,unsigned short int);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,long int);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,unsigned long int);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,bool);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,char);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,wchar_t);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,unsigned char);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,signed char);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,long long int);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_integral,unsigned long long int);

//
// is_floating_point
//
template <typename T> struct is_floating_point : false_type {};
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_floating_point,float);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_floating_point,double);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_floating_point,long double);




//
// is_same
//
template <typename TA, typename TB> struct is_same : false_type {};
template <typename T> struct is_same<T,T> : true_type {};



//! remove_const
template <typename T> struct remove_const {
    typedef T type;
};
template <typename T> struct remove_const<T const> {
    typedef T type;
};

//! remove_volatile
template <typename T> struct remove_volatile {
    typedef T type;
};
template <typename T> struct remove_volatile<T volatile> {
    typedef T type;
};

//! remove_cv
template <typename T> struct remove_cv {
    typedef typename remove_const< typename remove_volatile<T>::type >::type type;
};

//
// is_const
//
template <typename T> struct is_const
: false_type {};
template <typename T> struct is_const<T const>
: true_type {};

//
// is_volatile
//
template <typename T> struct is_volatile
: false_type {};
template <typename T> struct is_volatile<T volatile>
: true_type {};

//
// add_const
//
template <typename T> struct add_const {
    typedef T const type;
};
template <typename T> struct add_const<T const> {
    typedef T const type;
};

//
// add_volatile
//
template <typename T> struct add_volatile {
    typedef T volatile type;
};
template <typename T> struct add_volatile<T volatile> {
    typedef T volatile type;
};

//
// add_cv
//
template <typename T> struct add_cv {
    typedef typename add_volatile< typename add_const<T>::type >::type type;
};


//
// is_array
//
template <typename T> struct is_array : false_type {};
template <typename T, size_t N> struct is_array <T[N]> : true_type {};
template <typename T, size_t N> struct is_array <T const [N]> : true_type {};
template <typename T, size_t N> struct is_array <T volatile [N]> : true_type {};
template <typename T, size_t N> struct is_array <T const volatile [N]> : true_type {};

//
// is_reference
//
template <typename T> struct is_reference : false_type {};
template <typename T> struct is_reference<T&> : true_type {};

//
// is_pointer
//
template <typename T> struct is_pointer : false_type {};
template <typename T> struct is_pointer<T*> : true_type {};


//
// is_signed
//
template <typename T> struct is_signed : false_type {};
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_signed,signed short);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_signed,signed int);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_signed,signed long);
TR1LITE_DEF_SPECIALIZE_CV_TRUE(is_signed,signed long long);

//
// is_unsigned
//
template <typename T> struct is_unsigned : false_type {};
TR1LITE_DEF_SPECIALIZE_TRUE(is_unsigned,unsigned short);
TR1LITE_DEF_SPECIALIZE_TRUE(is_unsigned,unsigned int);
TR1LITE_DEF_SPECIALIZE_TRUE(is_unsigned,unsigned long);
TR1LITE_DEF_SPECIALIZE_TRUE(is_unsigned,unsigned long long);

//
// is_arithmetic
//
template <typename T> struct is_arithmetic
: integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value >
{};

//
// is_fundamental
//
template <typename T> struct is_fundamental
: integral_constant<bool,
    is_arithmetic<T>::value || is_void<T>::value || is_null_pointer<T>::value
>
{};

//
// is_compound -- any type that is not fundamental
//
template <typename T> struct is_compound
: integral_constant<bool, !is_fundamental<T>::value> {};



//
// is_member_pointer and associates
//
template <typename T> struct is_member_object_pointer : false_type {};

template <typename T, typename Class>
struct is_member_object_pointer<T Class::*>
: true_type {};


// disable unneeded "no arguments to macro" warnings in MSVC
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable:4003 )
#endif

namespace detail_mfp {
// this gets a little wieldly, we handle up to 5 arguments (A0 to A4)

    template <typename T> struct helper : false_type {};

    #define _ty(A) typename A

    // no arguments
    #define HELPER_SPEC(cv)                                 \
    template <typename R, typename Class>  struct helper<R(Class::*)() cv> : true_type {};
    //template <typename R, typename Class>  struct helper<R(Class::*)() cv &> : true_type {}
    HELPER_SPEC();HELPER_SPEC(const);HELPER_SPEC(volatile);HELPER_SPEC(const volatile);
    #undef HELPER_SPEC

    // 1 argument
    #define HELPER_SPEC(cv)                                 \
    template <typename R, typename Class, _ty(A1)>  struct helper<R(Class::*)(A1) cv> : true_type {};
    //template <typename R, typename Class, _ty(A1)>  struct helper<R(Class::*)(A1) cv &> : true_type {}
    HELPER_SPEC();HELPER_SPEC(const);HELPER_SPEC(volatile);HELPER_SPEC(const volatile);
    #undef HELPER_SPEC

    // 2 arguments
    #define HELPER_SPEC(cv)                                 \
    template <typename R, typename Class, _ty(A1), _ty(A2)> struct helper<R(Class::*)(A1,A2) cv> : true_type {}
    HELPER_SPEC(); HELPER_SPEC(const); HELPER_SPEC(volatile); HELPER_SPEC(const volatile);
    #undef HELPER_SPEC

    // 3 arguments
    #define HELPER_SPEC(cv)                                 \
    template <typename R, typename Class, _ty(A1), _ty(A2), _ty(A3)> struct helper<R (Class::*)(A1,A2,A3) cv> : true_type {};
    HELPER_SPEC(); HELPER_SPEC(const); HELPER_SPEC(volatile); HELPER_SPEC(const volatile);
    #undef HELPER_SPEC

    // 4 arguments
    #define HELPER_SPEC(cv)                                 \
    template <typename R, typename Class, _ty(A1), _ty(A2), _ty(A3), _ty(A4)> struct helper<R (Class::*)(A1,A2,A3,A4) cv> : true_type {};
    HELPER_SPEC(); HELPER_SPEC(const); HELPER_SPEC(volatile); HELPER_SPEC(const volatile);
    #undef HELPER_SPEC

    template <typename R, typename Class, _ty(A1), _ty(A2), _ty(A3), _ty(A4), _ty(A5)>
    struct helper<R (Class::*)(A1,A2,A3,A4,A5)> : true_type {};

    template <typename R, typename Class, _ty(A1), _ty(A2), _ty(A3), _ty(A4), _ty(A5)>
    struct helper<R (Class::*)(A1,A2,A3,A4,A5,...)> : true_type {};

    #undef _ty

} // member function pointer helper

#ifdef _MSC_VER
#pragma warning( pop ) // MSVC "no arguments to macro" warning
#endif

// is_member_function_pointer
template <typename T> struct is_member_function_pointer
: detail_mfp::helper <
    typename remove_cv<T>::type
> {};

namespace detail_mfp {
    template< class T >
    struct is_member_pointer_helper         : false_type {};
    template< class T, class U >
    struct is_member_pointer_helper<T U::*> : true_type {};
} // detail_mfp::

template< class T >
struct is_member_pointer :
detail_mfp::is_member_pointer_helper<typename remove_cv<T>::type> {};


#undef TR1LITE_DEF_SPECIALIZE_TRUE

//
// is_function
//

template <typename T> struct is_function : false_type {};
// we handle up to 5 arguments (1 to 5) then ellipsis
template <typename R> struct is_function <R()>
: true_type {};
template <typename R, typename T1>  struct is_function<R(T1)>
: true_type {};
template <typename R, typename T1, typename T2> struct is_function<R(T1,T2)>
: true_type {};
template <typename R, typename T1, typename T2, typename T3> struct is_function<R(T1,T2,T3)>
: true_type {};
template <typename R, typename T1, typename T2, typename T3, typename T4> struct is_function<R(T1,T2,T3,T4)>
: true_type {};
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5> struct is_function<R(T1,T2,T3,T4,T5)>
: true_type {};
template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5> struct is_function<R(T1,T2,T3,T4,T5,...)>
: true_type {};


//
// rank = number of dimensions
//
template <typename T> struct rank : integral_constant< size_t, 0> {};
template <typename T> struct rank<T[]> : integral_constant< size_t, rank<T>::value + 1> {};
template <typename T, size_t N> struct rank<T[N]> : integral_constant< size_t, rank<T>::value + 1> {};

//
// extent = size of topmost dimension
//
template<typename T, unsigned N=0> struct extent 
: integral_constant<size_t, 0> {};
template<typename T>                    struct extent<T[],0> 
: integral_constant<size_t, 0> {};
template<typename T, unsigned N>        struct extent<T[],N> 
: integral_constant<size_t, extent<T,N-1>::value> {};
template<typename T, size_t N>          struct extent<T[N],0> 
: integral_constant<size_t, N> {};
template<typename T, size_t I, unsigned N> struct extent<T[I],N> 
: integral_constant<size_t, extent<T,N-1>::value> {};

//
// remove extent
//
template <typename T>
struct remove_extent { typedef T type; };

template <typename T>
struct remove_extent<T[]> { typedef T type; };

template <typename T, std::size_t N>
struct remove_extent<T[N]> { typedef T type;};

//
// remove_reference
//
template <typename T> struct remove_reference {
    typedef T type;
};
template <typename T> struct remove_reference<T&> {
    typedef T type;
};



//
// add_reference
template <typename T> struct add_reference {
    // careful with T == cv-qualified void
    typedef typename helper::conditional< 
        is_reference< T >::value, T, T&
    >::type type;
};
template <> struct add_reference<void> {
    typedef void type;
};
template <> struct add_reference<void const> {
    typedef void const type;
};
template <> struct add_reference<void volatile> {
    typedef void volatile type;
};
template <> struct add_reference<void const volatile> {
    typedef void const volatile type;
};



// add_pointer
template <typename T> struct add_pointer {
    typedef T* type;
};

// remove_pointer
template <typename T> struct remove_pointer {
    typedef T type;
};
template <typename T> struct remove_pointer<T*> {
    typedef T type;
};

//
// make_signed, make_unsigned
//

namespace detail {
template <typename T> struct make_signed_impl { typedef T type; };

template <typename T> struct make_unsigned_impl { typedef T type; };

#define SIGNED_HELPER(T1,S1) \
template<> struct make_signed_impl<T1> { typedef S1 type; }; \
template<> struct make_signed_impl<T1 const> { typedef S1 const type; }; \
template<> struct make_signed_impl<T1 volatile> { typedef S1 volatile type; }; \
template<> struct make_signed_impl<T1 const volatile> { typedef S1 const volatile type; }; \

#define UNSIGNED_HELPER(T1,U1) template<> struct make_unsigned_impl<T1> { typedef U1 type; }
SIGNED_HELPER(signed char,signed char);
SIGNED_HELPER(unsigned char,signed char);
SIGNED_HELPER(char,signed char);

UNSIGNED_HELPER(signed char,unsigned char);
UNSIGNED_HELPER(unsigned char,unsigned char);
UNSIGNED_HELPER(char,unsigned char);

SIGNED_HELPER(signed short,signed short);
SIGNED_HELPER(unsigned short,signed short);
UNSIGNED_HELPER(signed short,unsigned short);
UNSIGNED_HELPER(unsigned short,unsigned short);

SIGNED_HELPER(signed long,signed long);
SIGNED_HELPER(unsigned long,signed long);
UNSIGNED_HELPER(signed long,unsigned long);
UNSIGNED_HELPER(unsigned long,unsigned long);

SIGNED_HELPER(signed int,signed int);
SIGNED_HELPER(unsigned int,signed int);
UNSIGNED_HELPER(signed int,unsigned int);
UNSIGNED_HELPER(unsigned int,unsigned int);

#undef SIGNED_HELPER
#undef UNSIGNED_HELPER
}

template <typename T> struct make_unsigned 
: detail::make_unsigned_impl<T> {
};

template <typename T> struct make_signed 
: detail::make_signed_impl<T> {
};


/*
 * The traits starting at this point are intended 
 * to require compiler support, so we rely on intrinsics
 */

// is_class
template <typename T> struct is_class
: helper::conditional< __is_class(T), true_type, false_type>::type {};

// is_empty
template <typename T> struct is_empty 
: helper::conditional< __is_empty(T), true_type, false_type>::type {};

// is_enum
template <typename T> struct is_enum
: helper::conditional< __is_enum(T), true_type, false_type>::type {};

// is_union
template <typename T> struct is_union
: helper::conditional< __is_union(T), true_type, false_type>::type {};


// is_abstract
template <typename T> struct is_abstract
: helper::conditional< __is_abstract(T), true_type, false_type>::type {};

// is_polymorphic
template <typename T> struct is_polymorphic
: helper::conditional< __is_polymorphic(T), true_type, false_type>::type {};

// is_pod
template <typename T> struct is_pod
: helper::conditional< __is_pod(T), true_type, false_type>::type {};


/*
 * Advanced type properties
 */

// given the above, it should be possible to 
// implement is_scalar.
template<class T>
struct is_scalar 
: integral_constant < bool,
    is_arithmetic<T>::value || is_pointer<T>::value ||
    is_member_pointer<T>::value || is_enum<T>::value ||
    is_null_pointer<T>::value
> {};

// as well as is_object
template <class T> 
struct is_object 
: helper::conditional< 
    !(is_function<T>::value 
     || is_reference<T>::value 
     || is_void<T>::value)
, true_type, false_type>::type     {};

} // traits::
} // cxxomfort::


#endif // type traits
// see http://stackoverflow.com/questions/9343329/how-to-know-underlying-type-of-class-enum/10956467#10956467
