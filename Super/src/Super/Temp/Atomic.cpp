#include "Atomic.h"


#ifdef _WIN32

#ifdef _MSC_VER
#pragma warning (disable:4800)  // disable boring char-to-bool warnings
#endif

#include <intrin.h>

#ifndef _WIN64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//extern "C" {
//__int64 _InterlockedCompareExchange64(__int64 volatile* Destination, __int64 Exchange, __int64 Comperand);
//#pragma intrinsic(_InterlockedCompareExchange64)
//}

#ifdef InterlockedIncrement64
#define _InterlockedIncrement64    InterlockedIncrement64 
#endif
#ifdef InterlockedDecrement64
#define _InterlockedDecrement64    InterlockedDecrement64
#endif
#ifdef InterlockedExchangeAdd64
#define _InterlockedExchangeAdd64  InterlockedExchangeAdd64
#endif
#ifdef InterlockedOr64
#define _InterlockedOr64           InterlockedOr64
#endif
#ifdef InterlockedAnd64
#define _InterlockedAnd64          InterlockedAnd64
#endif
#ifdef InterlockedXor64
#define _InterlockedXor64          InterlockedXor64
#endif
#ifdef InterlockedExchange64
#define _InterlockedExchange64     InterlockedExchange64
#endif
#endif


extern char myWrap_InterlockedExchangeAdd8(char volatile * _Addend, char _Value)
{
    return _InterlockedExchangeAdd8( _Addend,_Value);
}
extern short myWrap_InterlockedExchangeAdd16(short volatile * _Addend, short _Value)
{
    return _InterlockedExchangeAdd16( _Addend,_Value);
}
extern long myWrap_InterlockedExchangeAdd(long volatile * _Addend, long _Value)
{
    return _InterlockedExchangeAdd( _Addend,_Value);
}
extern __int64 myWrap_InterlockedExchangeAdd64(__int64 volatile * _Addend, __int64 _Value)
{
        return _InterlockedExchangeAdd64( _Addend,_Value);
}

extern short myWrap_InterlockedIncrement16(short volatile * _Addend)
{
    return _InterlockedIncrement16(_Addend);
}
extern long  myWrap_InterlockedIncrement(long volatile * _Addend)
{
    return _InterlockedIncrement(_Addend);
}
extern __int64 myWrap_InterlockedIncrement64(__int64 volatile * _Addend)
{
    return _InterlockedIncrement64(_Addend);
}

extern short myWrap_InterlockedDecrement16(short volatile * _Addend)
{
    return _InterlockedDecrement16(_Addend);
}
extern long __cdecl myWrap_InterlockedDecrement(long volatile * _Addend)
{
    return _InterlockedDecrement(_Addend);
}
extern __int64 myWrap_InterlockedDecrement64(__int64 volatile * _Addend)
{
    return _InterlockedDecrement64(_Addend);
}

extern char myWrap_InterlockedOr8(char volatile * _Value, char _Mask)
{
    return _InterlockedOr8(_Value,_Mask);
}
extern short myWrap_InterlockedOr16(short volatile * _Value, short _Mask)
{
    return _InterlockedOr16(_Value,_Mask);
}
extern long myWrap_InterlockedOr(long volatile * _Value, long _Mask)
{
    return _InterlockedOr(_Value,_Mask);
}
extern __int64 myWrap_InterlockedOr64(__int64 volatile * _Value, __int64 _Mask)
{
    return _InterlockedOr64(_Value,_Mask);
}

extern char myWrap_InterlockedAnd8(char volatile * _Value, char _Mask)
{
    return _InterlockedAnd8(_Value,_Mask);
}
extern short myWrap_InterlockedAnd16(short volatile * _Value, short _Mask)
{
    return _InterlockedAnd16(_Value,_Mask);
}
extern long myWrap_InterlockedAnd(long volatile * _Value, long _Mask)
{
    return _InterlockedAnd(_Value,_Mask);
}
extern __int64 myWrap_InterlockedAnd64(__int64 volatile * _Value, __int64 _Mask)
{
    return _InterlockedAnd64(_Value,_Mask);
}

extern char myWrap_InterlockedXor8(char volatile * _Value, char _Mask)
{
    return _InterlockedXor8(_Value,_Mask);
}
extern short myWrap_InterlockedXor16(short volatile * _Value, short _Mask)
{
    return _InterlockedXor16(_Value,_Mask);
}
extern long myWrap_InterlockedXor(long volatile * _Value, long _Mask)
{
    return _InterlockedXor(_Value,_Mask);
}
extern __int64 myWrap_InterlockedXor64(__int64 volatile * _Value, __int64 _Mask)
{
    return _InterlockedXor64(_Value,_Mask);
}

extern char myWrap_InterlockedCompareExchange8(char volatile * _Destination, char _Exchange, char _Comparand)
{
    return _InterlockedCompareExchange8(_Destination,_Exchange,_Comparand);
}
extern short myWrap_InterlockedCompareExchange16(short volatile * _Destination, short _Exchange, short _Comparand)
{
    return _InterlockedCompareExchange16(_Destination,_Exchange,_Comparand);
}
extern long  myWrap_InterlockedCompareExchange(long volatile * _Destination, long _Exchange, long _Comparand)
{
    return _InterlockedCompareExchange(_Destination,_Exchange,_Comparand);
}
extern __int64 myWrap_InterlockedCompareExchange64(__int64 volatile * _Destination, __int64 _Exchange, __int64 _Comparand)
{
    return _InterlockedCompareExchange64(_Destination,_Exchange,_Comparand);
}


extern char myWrap_InterlockedExchange8(char volatile * _Target, char _Value)
{
    return _InterlockedExchange8(_Target,_Value);
}
extern short myWrap_InterlockedExchange16(short volatile * _Target, short _Value)
{
    return _InterlockedExchange16(_Target,_Value);
}
extern long  myWrap_InterlockedExchange(long volatile * _Target, long _Value)
{
    return _InterlockedExchange(_Target,_Value);
}
extern __int64 myWrap_InterlockedExchange64(__int64 volatile * _Target, __int64 _Value)
{
    return _InterlockedExchange64(_Target,_Value);
}


#else


#endif



#ifdef UNIT_TEST


//#include "unitest.h"
//#include "base/def.h"
//#include "base/atomic.h"
//
//namespace test {
//
//    DEF_test(atomic) {
//        bool b = false;
//        int8 i8 = 0;
//        int16 i16 = 0;
//        int32 i32 = 0;
//        int64 i64 = 0;
//
//        DEF_case(inc) {
//            EXPECT_EQ(atomic_inc(&i8), 1);
//            EXPECT_EQ(atomic_inc(&i16), 1);
//            EXPECT_EQ(atomic_inc(&i32), 1);
//            EXPECT_EQ(atomic_inc(&i64), 1);
//        }
//
//        DEF_case(dec) {
//            EXPECT_EQ(atomic_dec(&i8), 0);
//            EXPECT_EQ(atomic_dec(&i16), 0);
//            EXPECT_EQ(atomic_dec(&i32), 0);
//            EXPECT_EQ(atomic_dec(&i64), 0);
//        }
//
//        DEF_case(add) {
//            EXPECT_EQ(atomic_add(&i8, 1), 1);
//            EXPECT_EQ(atomic_add(&i16, 1), 1);
//            EXPECT_EQ(atomic_add(&i32, 1), 1);
//            EXPECT_EQ(atomic_add(&i64, 1), 1);
//        }
//
//        DEF_case(sub) {
//            EXPECT_EQ(atomic_sub(&i8, 1), 0);
//            EXPECT_EQ(atomic_sub(&i16, 1), 0);
//            EXPECT_EQ(atomic_sub(&i32, 1), 0);
//            EXPECT_EQ(atomic_sub(&i64, 1), 0);
//        }
//
//        DEF_case(fetch_inc) {
//            EXPECT_EQ(atomic_fetch_inc(&i8), 0);
//            EXPECT_EQ(atomic_fetch_inc(&i16), 0);
//            EXPECT_EQ(atomic_fetch_inc(&i32), 0);
//            EXPECT_EQ(atomic_fetch_inc(&i64), 0);
//        }
//
//        DEF_case(fetch_dec) {
//            EXPECT_EQ(atomic_fetch_dec(&i8), 1);
//            EXPECT_EQ(atomic_fetch_dec(&i16), 1);
//            EXPECT_EQ(atomic_fetch_dec(&i32), 1);
//            EXPECT_EQ(atomic_fetch_dec(&i64), 1);
//        }
//
//        DEF_case(fetch_add) {
//            EXPECT_EQ(atomic_fetch_add(&i8, 1), 0);
//            EXPECT_EQ(atomic_fetch_add(&i16, 1), 0);
//            EXPECT_EQ(atomic_fetch_add(&i32, 1), 0);
//            EXPECT_EQ(atomic_fetch_add(&i64, 1), 0);
//        }
//
//        DEF_case(fetch_sub) {
//            EXPECT_EQ(atomic_fetch_sub(&i8, 1), 1);
//            EXPECT_EQ(atomic_fetch_sub(&i16, 1), 1);
//            EXPECT_EQ(atomic_fetch_sub(&i32, 1), 1);
//            EXPECT_EQ(atomic_fetch_sub(&i64, 1), 1);
//        }
//
//        DEF_case(or) {
//            EXPECT_EQ(atomic_or(&i8, 1), 1);
//            EXPECT_EQ(atomic_or(&i16, 1), 1);
//            EXPECT_EQ(atomic_or(&i32, 1), 1);
//            EXPECT_EQ(atomic_or(&i64, 1), 1);
//        }
//
//        DEF_case(xor) {
//            EXPECT_EQ(atomic_xor(&i8, 3), 2);
//            EXPECT_EQ(atomic_xor(&i16, 3), 2);
//            EXPECT_EQ(atomic_xor(&i32, 3), 2);
//            EXPECT_EQ(atomic_xor(&i64, 3), 2);
//        }
//
//        DEF_case(and) {
//            EXPECT_EQ(atomic_and(&i8, 1), 0);
//            EXPECT_EQ(atomic_and(&i16, 1), 0);
//            EXPECT_EQ(atomic_and(&i32, 1), 0);
//            EXPECT_EQ(atomic_and(&i64, 1), 0);
//        }
//
//        DEF_case(fetch_or) {
//            EXPECT_EQ(atomic_fetch_or(&i8, 1), 0);
//            EXPECT_EQ(atomic_fetch_or(&i16, 1), 0);
//            EXPECT_EQ(atomic_fetch_or(&i32, 1), 0);
//            EXPECT_EQ(atomic_fetch_or(&i64, 1), 0);
//        }
//
//        DEF_case(fetch_xor) {
//            EXPECT_EQ(atomic_fetch_xor(&i8, 3), 1);
//            EXPECT_EQ(atomic_fetch_xor(&i16, 3), 1);
//            EXPECT_EQ(atomic_fetch_xor(&i32, 3), 1);
//            EXPECT_EQ(atomic_fetch_xor(&i64, 3), 1);
//        }
//
//        DEF_case(fetch_and) {
//            EXPECT_EQ(atomic_fetch_and(&i8, 1), 2);
//            EXPECT_EQ(atomic_fetch_and(&i16, 1), 2);
//            EXPECT_EQ(atomic_fetch_and(&i32, 1), 2);
//            EXPECT_EQ(atomic_fetch_and(&i64, 1), 2);
//        }
//
//        DEF_case(swap) {
//            EXPECT_EQ(atomic_swap(&b, true), false);
//            EXPECT_EQ(atomic_swap(&i8, 1), 0);
//            EXPECT_EQ(atomic_swap(&i16, 1), 0);
//            EXPECT_EQ(atomic_swap(&i32, 1), 0);
//            EXPECT_EQ(atomic_swap(&i64, 1), 0);
//        }
//
//        DEF_case(compare_swap) {
//            EXPECT_EQ(atomic_compare_swap(&b, true, false), true);
//            EXPECT_EQ(atomic_compare_swap(&i8, 1, 0), 1);
//            EXPECT_EQ(atomic_compare_swap(&i16, 1, 0), 1);
//            EXPECT_EQ(atomic_compare_swap(&i32, 1, 0), 1);
//            EXPECT_EQ(atomic_compare_swap(&i64, 1, 0), 1);
//        }
//
//        DEF_case(get) {
//            EXPECT_EQ(atomic_get(&b), false);
//            EXPECT_EQ(atomic_get(&i8), 0);
//            EXPECT_EQ(atomic_get(&i16), 0);
//            EXPECT_EQ(atomic_get(&i32), 0);
//            EXPECT_EQ(atomic_get(&i64), 0);
//        }
//
//        DEF_case(set) {
//            atomic_set(&b, true);
//            atomic_set(&i8, 1);
//            atomic_set(&i16, 1);
//            atomic_set(&i32, 1);
//            atomic_set(&i64, 1);
//            EXPECT_EQ(atomic_get(&b), true);
//            EXPECT_EQ(atomic_get(&i8), 1);
//            EXPECT_EQ(atomic_get(&i16), 1);
//            EXPECT_EQ(atomic_get(&i32), 1);
//            EXPECT_EQ(atomic_get(&i64), 1);
//        }
//
//        DEF_case(reset) {
//            atomic_reset(&b);
//            atomic_reset(&i8);
//            atomic_reset(&i16);
//            atomic_reset(&i32);
//            atomic_reset(&i64);
//            EXPECT_EQ(atomic_get(&b), false);
//            EXPECT_EQ(atomic_get(&i8), 0);
//            EXPECT_EQ(atomic_get(&i16), 0);
//            EXPECT_EQ(atomic_get(&i32), 0);
//            EXPECT_EQ(atomic_get(&i64), 0);
//        }
//
//        DEF_case(pointer) {
//            typedef void (*func_t)();
//            func_t f = (func_t) 8;
//            void* p = (void*) 0;
//
//            atomic_swap(&p, 8);
//            EXPECT_EQ(p, (void*)8);
//
//            atomic_compare_swap(&p, 8, 0);
//            EXPECT_EQ(p, (void*)0);
//
//            atomic_set(&f, 0);
//            EXPECT_EQ(f, (func_t)0);
//        }
//    }
//
//} // namespace test


int Test_Atomic()
{

}

#endif
