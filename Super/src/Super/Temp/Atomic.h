#ifndef _Atomic_Header_
#define _Atomic_Header_

#include "../Basic/global.h" 
NAME_SPACE_MAIN_BEGIN

#ifdef _WIN32

//避免系统头文件包含，用extern函数转发，完善待续 by lizulin 
extern char myWrap_InterlockedExchangeAdd8(char volatile * _Addend, char _Value);
extern short myWrap_InterlockedExchangeAdd16(short volatile * _Addend, short _Value);
extern long myWrap_InterlockedExchangeAdd(long volatile * _Addend, long _Value);
extern __int64 myWrap_InterlockedExchangeAdd64(__int64 volatile * _Addend, __int64 _Value);

extern short myWrap_InterlockedIncrement16(short volatile * _Addend);
extern long  myWrap_InterlockedIncrement(long volatile * _Addend);
extern __int64 myWrap_InterlockedIncrement64(__int64 volatile * _Addend);

extern short myWrap_InterlockedDecrement16(short volatile * _Addend);
extern long __cdecl myWrap_InterlockedDecrement(long volatile * _Addend);
extern __int64 myWrap_InterlockedDecrement64(__int64 volatile * _Addend);

extern char myWrap_InterlockedOr8(char volatile * _Value, char _Mask);
extern short myWrap_InterlockedOr16(short volatile * _Value, short _Mask);
extern long myWrap_InterlockedOr(long volatile * _Value, long _Mask);
extern __int64 myWrap_InterlockedOr64(__int64 volatile * _Value, __int64 _Mask);

extern char myWrap_InterlockedAnd8(char volatile * _Value, char _Mask);
extern short myWrap_InterlockedAnd16(short volatile * _Value, short _Mask);
extern long myWrap_InterlockedAnd(long volatile * _Value, long _Mask);
extern __int64 myWrap_InterlockedAnd64(__int64 volatile * _Value, __int64 _Mask);

extern char myWrap_InterlockedXor8(char volatile * _Value, char _Mask);
extern short myWrap_InterlockedXor16(short volatile * _Value, short _Mask);
extern long myWrap_InterlockedXor(long volatile * _Value, long _Mask);
extern __int64 myWrap_InterlockedXor64(__int64 volatile * _Value, __int64 _Mask);

extern char myWrap_InterlockedCompareExchange8(char volatile * _Destination, char _Exchange, char _Comparand);
extern short myWrap_InterlockedCompareExchange16(short volatile * _Destination, short _Exchange, short _Comparand);
extern long  myWrap_InterlockedCompareExchange(long volatile * _Destination, long _Exchange, long _Comparand);
extern __int64 myWrap_InterlockedCompareExchange64(__int64 volatile * _Destination, __int64 _Exchange, __int64 _Comparand);


extern char myWrap_InterlockedExchange8(char volatile * _Target, char _Value);
extern short myWrap_InterlockedExchange16(short volatile * _Target, short _Value);
extern long  myWrap_InterlockedExchange(long volatile * _Target, long _Value);
extern __int64 myWrap_InterlockedExchange64(__int64 volatile * _Target, __int64 _Value);


namespace xx
{
        template<int> struct I {};

        template<> struct I<1> {
            typedef char type;
        };

        template<> struct I<2> {
            typedef short type;
        };

        template<> struct I<4> {
            typedef long type;
        };

        template<> struct I<8> {
            typedef long long type;
        };

        inline char atomic_inc(void* p, I<1>) {
            return myWrap_InterlockedExchangeAdd8((char*)p, 1) + 1;
        }

        inline short atomic_inc(void* p, I<2>) {
            return myWrap_InterlockedIncrement16((short*)p);
        }

        inline long atomic_inc(void* p, I<4>) {
            return myWrap_InterlockedIncrement((long*)p);
        }

        inline long long atomic_inc(void* p, I<8>) {
            return myWrap_InterlockedIncrement64((long long*)p);
        }

        inline char atomic_dec(void* p, I<1>) {
            return myWrap_InterlockedExchangeAdd8((char*)p, -1) - 1;
        }

        inline short atomic_dec(void* p, I<2>) {
            return myWrap_InterlockedDecrement16((short*)p);
        }

        inline long atomic_dec(void* p, I<4>) {
            return myWrap_InterlockedDecrement((long*)p);
        }

        inline long long atomic_dec(void* p, I<8>) {
            return myWrap_InterlockedDecrement64((long long*)p);
        }

        inline char atomic_add(void* p, char v, I<1>) {
            return myWrap_InterlockedExchangeAdd8((char*)p, v) + v;
        }

        inline short atomic_add(void* p, short v, I<2>) {
            return myWrap_InterlockedExchangeAdd16((short*)p, v) + v;
        }

        inline long atomic_add(void* p, long v, I<4>) {
            return myWrap_InterlockedExchangeAdd((long*)p, v) + v;
        }

        inline long long atomic_add(void* p, long long v, I<8>) {
            return myWrap_InterlockedExchangeAdd64((long long*)p, v) + v;
        }

        inline char atomic_sub(void* p, char v, I<1>) {
            return myWrap_InterlockedExchangeAdd8((char*)p, -v) - v;
        }

        inline short atomic_sub(void* p, short v, I<2>) {
            return myWrap_InterlockedExchangeAdd16((short*)p, -v) - v;
        }

        inline long atomic_sub(void* p, long v, I<4>) {
            return myWrap_InterlockedExchangeAdd((long*)p, -v) - v;
        }

        inline long long atomic_sub(void* p, long long v, I<8>) {
            return myWrap_InterlockedExchangeAdd64((long long*)p, -v) - v;
        }

        inline char atomic_fetch_inc(void* p, I<1>) {
            return myWrap_InterlockedExchangeAdd8((char*)p, 1);
        }

        inline short atomic_fetch_inc(void* p, I<2>) {
            return myWrap_InterlockedExchangeAdd16((short*)p, 1);
        }

        inline long atomic_fetch_inc(void* p, I<4>) {
            return myWrap_InterlockedExchangeAdd((long*)p, 1);
        }

        inline long long atomic_fetch_inc(void* p, I<8>) {
            return myWrap_InterlockedExchangeAdd64((long long*)p, 1);
        }

        inline char atomic_fetch_dec(void* p, I<1>) {
            return myWrap_InterlockedExchangeAdd8((char*)(p), -1);
        }

        inline short atomic_fetch_dec(void* p, I<2>) {
            return myWrap_InterlockedExchangeAdd16((short*)p, -1);
        }

        inline long atomic_fetch_dec(void* p, I<4>) {
            return myWrap_InterlockedExchangeAdd((long*)(p), -1);
        }

        inline long long atomic_fetch_dec(void* p, I<8>) {
            return myWrap_InterlockedExchangeAdd64((long long*)p, -1);
        }

        inline char atomic_fetch_add(void* p, char v, I<1>) {
            return myWrap_InterlockedExchangeAdd8((char*)p, v);
        }

        inline short atomic_fetch_add(void* p, short v, I<2>) {
            return myWrap_InterlockedExchangeAdd16((short*)p, v);
        }

        inline long atomic_fetch_add(void* p, long v, I<4>) {
            return myWrap_InterlockedExchangeAdd((long*)p, v);
        }

        inline long long atomic_fetch_add(void* p, long long v, I<8>) {
            return myWrap_InterlockedExchangeAdd64((long long*)p, v);
        }

        inline char atomic_fetch_sub(void* p, char v, I<1>) {
            return myWrap_InterlockedExchangeAdd8((char*)p, -v);
        }

        inline short atomic_fetch_sub(void* p, short v, I<2>) {
            return myWrap_InterlockedExchangeAdd16((short*)p, -v);
        }

        inline long atomic_fetch_sub(void* p, long v, I<4>) {
            return myWrap_InterlockedExchangeAdd((long*)p, -v);
        }

        inline long long atomic_fetch_sub(void* p, long long v, I<8>) {
            return myWrap_InterlockedExchangeAdd64((long long*)p, -v);
        }

        inline char atomic_or(void* p, char v, I<1>) {
            return myWrap_InterlockedOr8((char*)p, v) | v;
        }

        inline short atomic_or(void* p, short v, I<2>) {
            return myWrap_InterlockedOr16((short*)p, v) | v;
        }

        inline long atomic_or(void* p, long v, I<4>) {
            return myWrap_InterlockedOr((long*)p, v) | v;
        }

        inline long long atomic_or(void* p, long long v, I<8>) {
            return myWrap_InterlockedOr64((long long*)p, v) | v;
        }

        inline char atomic_and(void* p, char v, I<1>) {
            return myWrap_InterlockedAnd8((char*)p, v) & v;
        }

        inline short atomic_and(void* p, short v, I<2>) {
            return myWrap_InterlockedAnd16((short*)p, v) & v;
        }

        inline long atomic_and(void* p, long v, I<4>) {
            return myWrap_InterlockedAnd((long*)p, v) & v;
        }

        inline long long atomic_and(void* p, long long v, I<8>) {
            return myWrap_InterlockedAnd64((long long*)p, v) & v;
        }

        inline char atomic_xor(void* p, char v, I<1>) {
            return myWrap_InterlockedXor8((char*)p, v) ^ v;
        }

        inline short atomic_xor(void* p, short v, I<2>) {
            return myWrap_InterlockedXor16((short*)p, v) ^ v;
        }

        inline long atomic_xor(void* p, long v, I<4>) {
            return myWrap_InterlockedXor((long*)p, v) ^ v;
        }

        inline long long atomic_xor(void* p, long long v, I<8>) {
            return myWrap_InterlockedXor64((long long*)p, v) ^ v;
        }

        inline char atomic_fetch_or(void* p, char v, I<1>) {
            return myWrap_InterlockedOr8((char*)p, v);
        }

        inline short atomic_fetch_or(void* p, short v, I<2>) {
            return myWrap_InterlockedOr16((short*)p, v);
        }

        inline long atomic_fetch_or(void* p, long v, I<4>) {
            return myWrap_InterlockedOr((long*)p, v);
        }

        inline long long atomic_fetch_or(void* p, long long v, I<8>) {
            return myWrap_InterlockedOr64((long long*)p, v);
        }

        inline char atomic_fetch_and(void* p, char v, I<1>) {
            return myWrap_InterlockedAnd8((char*)p, v);
        }

        inline short atomic_fetch_and(void* p, short v, I<2>) {
            return myWrap_InterlockedAnd16((short*)p, v);
        }

        inline long atomic_fetch_and(void* p, long v, I<4>) {
            return myWrap_InterlockedAnd((long*)p, v);
        }

        inline long long atomic_fetch_and(void* p, long long v, I<8>) {
            return myWrap_InterlockedAnd64((long long*)p, v);
        }

        inline char atomic_fetch_xor(void* p, char v, I<1>) {
            return myWrap_InterlockedXor8((char*)p, v);
        }

        inline short atomic_fetch_xor(void* p, short v, I<2>) {
            return myWrap_InterlockedXor16((short*)p, v);
        }

        inline long atomic_fetch_xor(void* p, long v, I<4>) {
            return myWrap_InterlockedXor((long*)p, v);
        }

        inline long long atomic_fetch_xor(void* p, long long v, I<8>) {
            return myWrap_InterlockedXor64((long long*)p, v);
        }

        inline char atomic_swap(void* p, char v, I<1>) {
            return myWrap_InterlockedExchange8((char*)p, v);
        }

        inline short atomic_swap(void* p, short v, I<2>) {
            return myWrap_InterlockedExchange16((short*)p, v);
        }

        inline long atomic_swap(void* p, long v, I<4>) {
            return myWrap_InterlockedExchange((long*)p, v);
        }

        inline long long atomic_swap(void* p, long long v, I<8>) {
            return myWrap_InterlockedExchange64((long long*)p, v);
        }

        inline char atomic_compare_swap(void* p, char o, char v, I<1>) {
            return myWrap_InterlockedCompareExchange8((char*)p, v, o);
        }

        inline short atomic_compare_swap(void* p, short o, short v, I<2>) {
            return myWrap_InterlockedCompareExchange16((short*)p, v, o);
        }

        inline long atomic_compare_swap(void* p, long o, long v, I<4>) {
            return myWrap_InterlockedCompareExchange((long*)p, v, o);
        }

        inline long long atomic_compare_swap(void* p, long long o, long long v, I<8>) {
            return myWrap_InterlockedCompareExchange64((long long*)p, v, o);
        }

} // namespace xx

template<typename T>
inline T atomic_inc(T* p) {
    typedef xx::I<sizeof(T)> it;
    return (T) xx::atomic_inc(p, it());
}

template<typename T>
inline T atomic_dec(T* p) {
    typedef xx::I<sizeof(T)> it;
    return (T) xx::atomic_dec(p, it());
}

template<typename T, typename V>
inline T atomic_add(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_add(p, (type)v, it());
}

template<typename T, typename V>
inline T atomic_sub(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_sub(p, (type)v, it());
}

template<typename T>
inline T atomic_fetch_inc(T* p) {
    typedef xx::I<sizeof(T)> it;
    return (T) xx::atomic_fetch_inc(p, it());
}

template<typename T>
inline T atomic_fetch_dec(T* p) {
    typedef xx::I<sizeof(T)> it;
    return (T) xx::atomic_fetch_dec(p, it());
}

template<typename T, typename V>
inline T atomic_fetch_add(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_fetch_add(p, (type)v, it());
}

template<typename T, typename V>
inline T atomic_fetch_sub(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_fetch_sub(p, (type)v, it());
}

template<typename T, typename V>
inline T atomic_or(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_or(p, (type)v, it());
}

template<typename T, typename V>
inline T atomic_and(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_and(p, (type)v, it());
}

template<typename T, typename V>
inline T atomic_xor(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_xor(p, (type)v, it());
}

template<typename T, typename V>
inline T atomic_fetch_or(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_fetch_or(p, (type)v, it());
}

template<typename T, typename V>
inline T atomic_fetch_and(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_fetch_and(p, (type)v, it());
}

template<typename T, typename V>
inline T atomic_fetch_xor(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_fetch_xor(p, (type)v, it());
}

template<typename T, typename V>
inline T atomic_swap(T* p, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_swap(p, (type)v, it());
}

template<typename T, typename O, typename V>
inline T atomic_compare_swap(T* p, O o, V v) {
    typedef xx::I<sizeof(T)> it;
    typedef typename it::type type;
    return (T) xx::atomic_compare_swap(p, (type)o, (type)v, it());
}

template<typename T>
inline T atomic_get(T* p) {
    return atomic_fetch_xor(p, 0);
}

template<typename T, typename V>
inline void atomic_set(T* p, V v) {
    (void) atomic_swap(p, v);
}

template<typename T>
inline void atomic_reset(T* p) {
    (void) atomic_swap(p, 0);
}

 



#else  //for unix










#pragma once

#ifndef _WIN32

// clang or gcc 4.7+
#if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)))

template <typename T>
inline T atomic_inc(T* p) {
    return __atomic_add_fetch(p, 1, __ATOMIC_SEQ_CST);
}

template <typename T>
inline T atomic_dec(T* p) {
    return __atomic_sub_fetch(p, 1, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_add(T* p, V v) {
    return __atomic_add_fetch(p, v, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_sub(T* p, V v) {
    return __atomic_sub_fetch(p, v, __ATOMIC_SEQ_CST);
}

template <typename T>
inline T atomic_fetch_inc(T* p) {
    return __atomic_fetch_add(p, 1, __ATOMIC_SEQ_CST);
}

template <typename T>
inline T atomic_fetch_dec(T* p) {
    return __atomic_fetch_sub(p, 1, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_fetch_add(T* p, V v) {
    return __atomic_fetch_add(p, v, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_fetch_sub(T* p, V v) {
    return __atomic_fetch_sub(p, v, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_or(T* p, V v) {
    return __atomic_or_fetch (p, v, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_and(T* p, V v) {
    return __atomic_and_fetch(p, v, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_xor(T* p, V v) {
    return __atomic_xor_fetch(p, v, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_fetch_or(T* p, V v) {
    return __atomic_fetch_or(p, v, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_fetch_and(T* p, V v) {
    return __atomic_fetch_and(p, v, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_fetch_xor(T* p, V v) {
    return __atomic_fetch_xor(p, v, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline T atomic_swap(T* p, V v) {
    return __atomic_exchange_n(p, (T)v, __ATOMIC_SEQ_CST);
}

template <typename T, typename O, typename V>
inline T atomic_compare_swap(T* p, O o, V v) {
    T x = (T) o;
    __atomic_compare_exchange_n(p, &x, (T)v, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    return x;
}

template <typename T>
inline T atomic_get(T* p) {
    return __atomic_load_n(p, __ATOMIC_SEQ_CST);
}

template <typename T, typename V>
inline void atomic_set(T* p, V v) {
    __atomic_store_n(p, (T)v, __ATOMIC_SEQ_CST);
}

template <typename T>
inline void atomic_reset(T* p) {
    __atomic_store_n(p, (T)0, __ATOMIC_SEQ_CST);
}

// gcc 4.1+
#elif defined(__GNUC__) && (__GNUC__ == 4 && __GNUC_MINOR__ >= 1)

template <typename T>
inline T atomic_inc(T* p) {
    return __sync_add_and_fetch(p, 1);
}

template <typename T>
inline T atomic_dec(T* p) {
    return __sync_sub_and_fetch(p, 1);
}

template <typename T, typename V>
inline T atomic_add(T* p, V v) {
    return __sync_add_and_fetch(p, v);
}

template <typename T, typename V>
inline T atomic_sub(T* p, V v) {
    return __sync_sub_and_fetch(p, v);
}

template <typename T>
inline T atomic_fetch_inc(T* p) {
    return __sync_fetch_and_add(p, 1);
}

template <typename T>
inline T atomic_fetch_dec(T* p) {
    return __sync_fetch_and_sub(p, 1);
}

template <typename T, typename V>
inline T atomic_fetch_add(T* p, V v) {
    return __sync_fetch_and_add(p, v);
}

template <typename T, typename V>
inline T atomic_fetch_sub(T* p, V v) {
    return __sync_fetch_and_sub(p, v);
}

template <typename T, typename V>
inline T atomic_or(T* p, V v) {
    return __sync_or_and_fetch(p, v);
}

template <typename T, typename V>
inline T atomic_and(T* p, V v) {
    return __sync_and_and_fetch(p, v);
}

template <typename T, typename V>
inline T atomic_xor(T* p, V v) {
    return __sync_xor_and_fetch(p, v);
}

template <typename T, typename V>
inline T atomic_fetch_or(T* p, V v) {
    return __sync_fetch_and_or(p, v);
}

template <typename T, typename V>
inline T atomic_fetch_and(T* p, V v) {
    return __sync_fetch_and_and(p, v);
}

template <typename T, typename V>
inline T atomic_fetch_xor(T* p, V v) {
    return __sync_fetch_and_xor(p, v);
}

template <typename T, typename V>
inline T atomic_swap(T* p, V v) {
    return __sync_lock_test_and_set(p, (T)v);  // acquire barrier
}

template <typename T, typename O, typename V>
inline T atomic_compare_swap(T* p, O o, V v) {
    return __sync_val_compare_and_swap(p, (T)o, (T)v);
}

template <typename T>
inline T atomic_get(T* p) {
    return atomic_fetch_or(p, 0);
}

//     |
//     v   ^
//---------|-----  release barrier
//
//---------|-----  acquire barrier
//     ^   v
//     |

template <typename T, typename V>
inline void atomic_set(T* p, V v) {
    __sync_lock_test_and_set(p, v); // acquire barrier
}

template <typename T>
inline void atomic_reset(T* p) {
    __sync_lock_release(p);         // release barrier
}

#else
#error "clang or gcc 4.1+ required"
#endif

#endif






#endif


NAME_SPACE_MAIN_END


#ifdef UNIT_TEST
BASIC_API int Test_A_NewClassTemplate();
#endif

#endif


