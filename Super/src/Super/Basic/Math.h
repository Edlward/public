#ifndef MATH_Header
#define MATH_Header

#include "../Basic/global.h"

#include <cmath>
#include <stdint.h>
#include <type_traits>


//TODO:more math const value
#ifndef M_2PI
#define M_2PI 6.28318530717958647692528676655900576
#endif

#ifndef M_E
#define M_E (2.7182818284590452354)
#endif

#ifndef M_LOG2E
#define M_LOG2E (1.4426950408889634074)
#endif

#ifndef M_LOG10E
#define M_LOG10E (0.43429448190325182765)
#endif

#ifndef M_LN2
#define M_LN2 (0.69314718055994530942)
#endif

#ifndef M_LN10
#define M_LN10 (2.30258509299404568402)
#endif

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#ifndef M_PI_2
#define M_PI_2 (1.57079632679489661923)
#endif

#ifndef M_PI_4
#define M_PI_4 (0.78539816339744830962)
#endif

#ifndef M_1_PI
#define M_1_PI (0.31830988618379067154)
#endif

#ifndef M_2_PI
#define M_2_PI (0.63661977236758134308)
#endif

#ifndef M_2_SQRTPI
#define M_2_SQRTPI (1.12837916709551257390)
#endif

#ifndef M_SQRT2
#define M_SQRT2 (1.41421356237309504880)
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2 (0.70710678118654752440)
#endif


//c语言实现sin,cos,sqrt,pow函数
float MySin(float x);
float MyCos(float x);
float MyPow(float a,int b);


typedef double  qreal;

#define SINE_TABLE_SIZE 256

extern  const double table_sin256[SINE_TABLE_SIZE];

inline int qCeil(qreal v)
{
    using std::ceil;
    return int(ceil(v));
}

inline int qFloor(qreal v)
{
    using std::floor;
    return int(floor(v));
}

inline qreal qFabs(qreal v)
{
    using std::fabs;
    return fabs(v);
}

inline qreal qSin(qreal v)
{
    using std::sin;
    return sin(v);
}

inline qreal qCos(qreal v)
{
    using std::cos;
    return cos(v);
}

inline qreal qTan(qreal v)
{
    using std::tan;
    return tan(v);
}

inline qreal qAcos(qreal v)
{
    using std::acos;
    return acos(v);
}

inline qreal qAsin(qreal v)
{
    using std::asin;
    return asin(v);
}

inline qreal qAtan(qreal v)
{
    using std::atan;
    return atan(v);
}

inline qreal qAtan2(qreal y, qreal x)
{
    using std::atan2;
    return atan2(y, x);
}

inline qreal qSqrt(qreal v)
{
    using std::sqrt;
    return sqrt(v);
}

inline qreal qLn(qreal v)
{
    using std::log;
    return log(v);
}

inline qreal qExp(qreal v)
{
    using std::exp;
    return exp(v);
}

inline qreal qPow(qreal x, qreal y)
{
    using std::pow;
    return pow(x, y);
}


//
//inline bool qt_is_inf(double d)
//{
//    uchar *ch = (uchar *)&d;
//    if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
//        return (ch[0] & 0x7f) == 0x7f && ch[1] == 0xf0;
//    } else {
//        return (ch[7] & 0x7f) == 0x7f && ch[6] == 0xf0;
//    }
//}
//
//inline bool qt_is_nan(double d)
//{
//    uchar *ch = (uchar *)&d;
//    if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
//        return (ch[0] & 0x7f) == 0x7f && ch[1] > 0xf0;
//    } else {
//        return (ch[7] & 0x7f) == 0x7f && ch[6] > 0xf0;
//    }
//}
//
//inline bool qt_is_finite(double d)
//{
//    uchar *ch = (uchar *)&d;
//    if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
//        return (ch[0] & 0x7f) != 0x7f || (ch[1] & 0xf0) != 0xf0;
//    } else {
//        return (ch[7] & 0x7f) != 0x7f || (ch[6] & 0xf0) != 0xf0;
//    }
//}
//
//inline bool qt_is_inf(float d)
//{
//    uchar *ch = (uchar *)&d;
//    if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
//        return (ch[0] & 0x7f) == 0x7f && ch[1] == 0x80;
//    } else {
//        return (ch[3] & 0x7f) == 0x7f && ch[2] == 0x80;
//    }
//}
//
//inline bool qt_is_nan(float d)
//{
//    uchar *ch = (uchar *)&d;
//    if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
//        return (ch[0] & 0x7f) == 0x7f && ch[1] > 0x80;
//    } else {
//        return (ch[3] & 0x7f) == 0x7f && ch[2] > 0x80;
//    }
//}

//inline bool qt_is_finite(float d)
//{
//    uchar *ch = (uchar *)&d;
//    if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
//        return (ch[0] & 0x7f) != 0x7f || (ch[1] & 0x80) != 0x80;
//    } else {
//        return (ch[3] & 0x7f) != 0x7f || (ch[2] & 0x80) != 0x80;
//    }
//}

inline bool qt_is_finite(float d)
{
    uchar *ch = (uchar *)&d;
    //if (QSysInfo::ByteOrder == QSysInfo::BigEndian) 
    if(0)
    {
        return (ch[0] & 0x7f) != 0x7f || (ch[1] & 0x80) != 0x80;
    } else {
        return (ch[3] & 0x7f) != 0x7f || (ch[2] & 0x80) != 0x80;
    }
}


inline uint32_t f2i(float f)
{
    uint32_t i;
    //memcpy(&i, &f, sizeof(f));
    i=*((uint32_t*)&f);
    return i;
}

inline uint64_t d2i(double d)
{
    uint64_t i;
    //memcpy(&i, &d, sizeof(d));
    i=*((uint64_t*)&d);
    return i;
}

inline uint32 qFloatDistance(float a, float b)
{
    static const uint32 smallestPositiveFloatAsBits = 0x00000001;  // denormalized, (SMALLEST), (1.4E-45)
    /* Assumes:
    * IEE754 format.
    * Integers and floats have the same endian
    */

    //暂时屏蔽
    //Q_STATIC_ASSERT(sizeof(quint32) == sizeof(float));
    //S_ASSERT(qIsFinite(a) && qIsFinite(b));


    if (a == b)
        return 0;
    if ((a < 0) != (b < 0)) {
        // if they have different signs
        if (a < 0)
            a = -a;
        else /*if (b < 0)*/
            b = -b;
        return qFloatDistance(0.0F, a) + qFloatDistance(0.0F, b);
    }
    if (a < 0) {
        a = -a;
        b = -b;
    }
    // at this point a and b should not be negative

    // 0 is special
    if (!a)
        return f2i(b) - smallestPositiveFloatAsBits + 1;
    if (!b)
        return f2i(a) - smallestPositiveFloatAsBits + 1;

    // finally do the common integer subtraction
    return a > b ? f2i(a) - f2i(b) : f2i(b) - f2i(a);
}




inline uint64 qFloatDistance(double a, double b)
{
    static const uint64 smallestPositiveFloatAsBits = 0x1;  // denormalized, (SMALLEST)
    /* Assumes:
    * IEE754 format double precision
    * Integers and floats have the same endian
    */


    //暂时屏蔽
    //Q_STATIC_ASSERT(sizeof(uint64) == sizeof(double));
    //S_ASSERT(qIsFinite(a) && qIsFinite(b));

    if (a == b)
        return 0;
    if ((a < 0) != (b < 0)) {
        // if they have different signs
        if (a < 0)
            a = -a;
        else /*if (b < 0)*/
            b = -b;
        return qFloatDistance(0.0, a) + qFloatDistance(0.0, b);
    }
    if (a < 0) {
        a = -a;
        b = -b;
    }
    // at this point a and b should not be negative

    // 0 is special
    if (!a)
        return d2i(b) - smallestPositiveFloatAsBits + 1;
    if (!b)
        return d2i(a) - smallestPositiveFloatAsBits + 1;

    // finally do the common integer subtraction
    return a > b ? d2i(a) - d2i(b) : d2i(b) - d2i(a);
}





inline double FastSin(double x)
{
    int si = int(x * (0.5 * SINE_TABLE_SIZE / M_PI)); // Would be more accurate with qRound, but slower.
    double d = x - si * (2.0 * M_PI / SINE_TABLE_SIZE);
    int ci = si + SINE_TABLE_SIZE / 4;
    si &= SINE_TABLE_SIZE - 1;
    ci &= SINE_TABLE_SIZE - 1;
    return table_sin256[si] + (table_sin256[ci] - 0.5 * table_sin256[si] * d) * d;
}

inline double FastCos(double x)
{
    int ci = int(x * (0.5 * SINE_TABLE_SIZE / M_PI)); // Would be more accurate with qRound, but slower.
    double d = x - ci * (2.0 * M_PI / SINE_TABLE_SIZE);
    int si = ci + SINE_TABLE_SIZE / 4;
    si &= SINE_TABLE_SIZE - 1;
    ci &= SINE_TABLE_SIZE - 1;
    return table_sin256[si] - (table_sin256[ci] + 0.5 * table_sin256[si] * d) * d;
}

inline float DegreesToRadians(float degrees)
{
    return degrees * float(M_PI/180);
}

inline double DegreesToRadians(double degrees)
{
    return degrees * (M_PI / 180);
}

inline float RadiansToDegrees(float radians)
{
    return radians * float(180/M_PI);
}

inline double RadiansToDegrees(double radians)
{
    return radians * (180 / M_PI);
}











inline uint32_t NextPowerOfTwo(uint32_t v)
{
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return v;
}

inline uint64_t NextPowerOfTwo(uint64_t v)
{
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    ++v;
    return v;
}

inline uint32_t qNextPowerOfTwo(int32_t v)
{
    return NextPowerOfTwo(uint32_t(v));
}

inline uint64_t qNextPowerOfTwo(int64_t v)
{
    return NextPowerOfTwo(uint64_t(v));
}


/** Returns true if the specified integer is a power-of-two. */
template <typename IntegerType>
bool isPowerOfTwo (IntegerType value)
{
    static_assert(std::is_integral<IntegerType>::value,"not IntegerType");
    return (value&(value-1)) == 0;
}

//是否二次幂
inline bool is_pow_of_2(unsigned int x) 
{
    return (x&(x-1))==0;
}

inline bool is_pow_of_2(uint64_t x) 
{
    return (x&(x-1))==0;
}

//上一个接近二次幂的数  
//greatest power of 2 less than or equal to 
inline unsigned int prev_pow_of_2(unsigned int n)
{
    n|=(n>>1);
    n|=(n>>2);
    n|=(n>>4);
    n|=(n>>8);
    n|=(n>>16);
    return n-(n>>1);
}

//greatest power of 2 less than or equal to 
inline uint64_t prev_pow_of_2(uint64_t n)
{
    n|=(n>>1);
    n|=(n>>2);
    n|=(n>>4);
    n|=(n>>8);
    n|=(n>>16);
    n|=(n>>32);
    return n-(n>>1);
}


//下一个接近二次幂的数  或者使用汇编位扫描指令BSF 和 BSR可以一次得到
//least power of 2 greater than or equal to 
inline unsigned int next_pow_of_2(unsigned int v) 
{
    //if (is_pow_of_2(v))
    //{
    //    return v;
    //}
    --v;
    v|=v>>1;
    v|=v>>2;
    v|=v>>4;
    v|=v>>8;
    v|=v>>16;
    return v+1;
}

inline uint64_t next_pow_of_2(uint64_t v)
{
    //if (is_pow_of_2(v))
    //{
    //    return v;
    //}
    --v;
    v |= v>>1;
    v |= v>>2;
    v |= v>>4;
    v |= v>>8;
    v |= v>>16;
    v |= v>>32;
    //x |= x>>64;
    return v+1;
}


inline  unsigned nextPowerOf2(unsigned int n)
{
    // decrement n (to handle the case when n itself 
    // is a power of 2)
    n = n - 1;
    // do till only one bit is left
    while (n&(n-1))
    {
        n=n&(n-1);	// unset rightmost bit
    }
    // n is now a power of two (less than n)
    // return next power of 2
    return n << 1;
}

// compute power of two greater than or equal to n
inline  unsigned nextPowerOf2_222(unsigned int n)
{
    // decrement n (to handle the case when n itself 
    // is a power of 2)
    n = n - 1;
    // initialize result by 2
    int k = 2;
    // double k and divide n in half till it becomes 0
    while (n >>=1)
    {
        k = k << 1; // double k
    }
    return k;
}


//求一个整数数的以2为低的对数 自定义 by 
//inline unsigned int FastLog2(unsigned int x)
//{
//     for (unsigned int n=0;n<7;n++)
//     {
//
//     }
//}



////32bit版本，思路是二分法
//unsigned int Hight(unsigned x)
//{
//    int n=1;
//    if(x==0) return -1;
//    if ((x>>16) == 0){n=n+16;x = x<<16;}
//    if ((x>>24) == 0){n=n+8;x = x<<8;}
//    if ((x>>28) == 0){n=n+4; x = x<<4;}
//    if ((x>>30) == 0){n=n+2; x = x<<2;}
//    n = n-(x>>31);
//    return 31-n;    
//}
//看了好一会才看明白，容我做个注解给后来人。。。如有不对还请大大们纠正
//    if ((x>>16) == 0) {n = n+16; x = x<<16;} cout<<n<<", "; printf("%x\n", x);
//if ((x>>24) == 0) {n = n+8; x = x<<8;}  cout<<n<<", "; printf("%x\n", x);
//if ((x>>28) == 0) {n = n+4; x = x<<4;}  cout<<n<<", "; printf("%x\n", x);
//if ((x>>30) == 0) {n = n+2; x = x<<2;}  cout<<n<<", "; printf("%x\n", x);
//这几行是二分，依次判断最高位是在高16位还是低16位，是在16位中的高8位还是低8位...
//    n = n-(x>>31);
//这行是针对当x高位为31时的情况，此时n原本应该是0， 这样最后31-n=31，但如果n赋初值0，位移就没法做了，所以此处做一判断修正n的值。如果写的更直白一点可以直接在一开始就判断：
//    if(x>>31) return 31;

//纠正一下，n = n-(x>>31); 是二分的最后一步用来判断最高位1是在高1位还是低1位。所以不能简单的用
//if(x>>31) return 31;来代替。


////获得32位数二进制位的第一个1的位置
inline unsigned int getHightBit1PosTest1(unsigned int x)
{
    //unsigned int tmp=ByteOrderSwap32(x);
    //print_bin(&tmp,sizeof(tmp));
    //print_bin(&x,sizeof(x));
    //print_bin(x);

    int n=1;
    //printf("n:%02u x:%08x Bin:",n,x);
    //print_bin(x);

    //if(x==0) return 1;
    if(x==0) return 0;

    if ((x>>16) == 0)
    {
        n=n+16;x = x<<16;
        //printf("n:%02u x:%08x Bin:",n,x);
        //print_bin(x);
    }
    if ((x>>24) == 0)
    {
        n=n+8;x = x<<8;
        //printf("n:%02u x:%08x Bin:",n,x);
        //print_bin(x);
    }
    if ((x>>28) == 0)
    {
        n=n+4; x = x<<4;
        //printf("n:%02u x:%08x Bin:",n,x);
        //print_bin(x);
    }
    if ((x>>30) == 0)
    {
        n=n+2; x = x<<2;
        //printf("n:%02u x:%08x Bin:",n,x);
        //print_bin(x);
    }
    n = n-(x>>31);

    //printf("n:%02u x:%08x Bin:",n,x);
    //print_bin(x);

    return 31-n;    
}

//int GetLeftFirstone(unsigned int v)
//{
//    v ^= v - 1;
//    v = (v & 0x55555555) + ((v >> 1) & 0x55555555);
//    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
//    v = (v + (v >> 4)) & 0x0F0F0F0F;
//    v += v >> 8;
//    v += v >> 16;
//    return (v & 0x3F) - 1;
//}
//
//int GetLeftFirstOne(unsigned int v)
//{
//    int pos=0;
//    if (!(v & 0xFFFF))
//    { 
//        v >>= 16;
//        pos += 16;
//    }
//    if (!(v & 0xFF))
//    {
//        v >>= 8;
//        pos += 8;
//    }
//    if (!(v & 0xF))
//    {
//        v >>= 4; 
//        pos += 4;
//    }
//    if (!(v & 0x3))
//    {
//        v >>= 2; 
//        pos += 2;
//    }
//    if (!(v & 0x1))
//    {
//        pos += 1;
//    }
//
//    return pos;
//}
//
//
//
//int GetRightFirstOne(unsigned int v)
//{
//    int pos=0;
//
//    if (!(v & 0xFFFF0000))
//    { 
//        v <<= 16;
//        pos += 16;
//    }
//    if (!(v & 0xFF000000))
//    {
//        v <<= 8;
//        pos += 8;
//    }
//    if (!(v & 0xF0000000))
//    {
//        v <<= 4; 
//        pos += 4;
//    }
//    if (!(v & 0xC0000000))
//    {
//        v <<= 2; 
//        pos += 2;
//    }
//    if (!(v & 0x80000000))
//    {
//        pos += 1;
//    }
//    return pos;
//}
//
//int f1(unsigned x)
//{
//    int n=1;
//    if(x==0) return -1;
//    if ((x>>16) == 0) {n = n+16; x = x<<16;}
//    if ((x>>24) == 0) {n = n+8; x = x<<8;}
//    if ((x>>28) == 0) {n = n+4; x = x<<4;}
//    if ((x>>30) == 0) {n = n+2; x = x<<2;}
//    n = n-(x>>31);
//    return 31-n;    
//}


#ifdef _MSC_VER
#include <intrin.h>

//逆向扫描指令BSR（Bit Scan Reverse），从左向右扫描，即从高位向低位扫描。//即第一个出现1的位置。
//Search the mask data from most significant bit (MSB) to least significant bit (LSB) for a set bit (1).
//原型：unsigned char _BitScanReverse(unsigned long * Index, unsigned long Mask);
//返回值：Nonzero if Index was set,or 0 if no set bits were found.


//msc编译时使用内建函数(Compiler Intrinsics)
inline unsigned long BSR_uint32(unsigned __int32 n)
{
    unsigned long index;
    _BitScanReverse(&index,n);
    return index;
    //return _BitScanReverse(&index,n)?index:-1;  //old
}
inline  unsigned long BSR_uint64(unsigned __int64 num)
{
    unsigned long index;
    _BitScanReverse64(&index,num);
    return index;
    //return _BitScanReverse64(&index,num)?index:-1;  //old
}
inline unsigned long BSF_uint32(unsigned __int32 n)
{
    unsigned long index;
    _BitScanForward(&index,n);
    return index;
}
inline  unsigned long BSF_uint64(unsigned __int64 num)
{
    unsigned long index;
    _BitScanForward64(&index,num);
    return index;
}

#elif __GNUC__
//gcc 编译时使用内建函数(Built-in Functions)
//高效位运算 __builtin_系列函数
//•int __builtin_ffs (unsigned int x)返回x的最后一位1的是从后向前第几位，比如7368（1110011001000）返回4。
//•int __builtin_clz (unsigned int x) 返回前导的0的个数。 如果x=0 的话，结果未知
//•int __builtin_ctz (unsigned int x)返回后面的0个个数，和__builtin_clz相对。
//•int __builtin_popcount (unsigned int x)返回二进制表示中1的个数。
//•int __builtin_parity (unsigned int x)返回x的奇偶校验位，也就是x的1的个数模2的结果。
//这些函数都有相应的usigned long和usigned long long版本，只需要在函数名后面加上l或ll就可以了

//— Built-in Function: int __builtin_ffs (unsigned int x)
//    Returns one plus the index of the least significant 1-bit of x, or if x is zero, returns zero.
//
//    — Built-in Function: int __builtin_clz (unsigned int x)
//    Returns the number of leading 0-bits in x, starting at the most significant bit position. If x is 0, the result is undefined.
//
//    — Built-in Function: int __builtin_ctz (unsigned int x)
//    Returns the number of trailing 0-bits in x, starting at the least significant bit position. If x is 0, the result is undefined.
//
//    — Built-in Function: int __builtin_popcount (unsigned int x)
//    Returns the number of 1-bits in x.
//
//    — Built-in Function: int __builtin_parity (unsigned int x)
//    Returns the parity of x, i.e. the number of 1-bits in x modulo 2.


inline uint8_t BSR_uint32(uint32_t n)  //n!=0
{
    if (n==0) return 0;
    return 31-__builtin_clz(n);  //
    //return num==0?-1:(sizeof(num)<<3)-1-__builtin_clz(num);
    //clz返回32位数前导为0的个数，比如2返回30(前面有30个0)
}
inline uint8_t BSR_uint64(uint64_t n) //n!=0
{
    if (n==0) return 0;
    return 63-__builtin_clzll(n);
    //return num==0?-1:(sizeof(num)<<3)-1-__builtin_clzll(num);
    //clzll返回64位数前导为0的个数，比如1返回63(前面有63个0)
}
inline uint8_t BSF_uint32(uint32_t n)  //n!=0
{
    if (n==0) return 0;
    return __builtin_ctz(n);  //
}
inline uint8_t BSF_uint64(uint64_t n) //n!=0
{
    if (n==0) return 0;
    return __builtin_ctzll(n);
}
#endif


///* We use the De-Bruijn method outlined in: http://supertech.csail.mit.edu/papers/debruijn.pdf. 
//两个最快版本居然比_BitScanReverse 快,什么原理
//static 
const unsigned char MultiplyDeBruijnBitPosition32[32]={0,9,1,10,13,21,2,29,11,14,16,18,22,25,3,30,
8,12,20,28,15,17,24,7,19,27,23,6,26,5,4,31};

inline uint32_t MSB_DeBruijn32(uint32_t v)
{
    v|=v>>1;
    v|=v>>2;
    v|=v>>4;
    v|=v>>8;
    v|=v>>16;
    return MultiplyDeBruijnBitPosition32[(uint32_t)(v*0x07C4ACDDU)>>27];
}
const unsigned char MultiplyDeBruijnBitPosition64[64]={
    0, 47,  1, 56, 48, 27,  2, 60, 57, 49, 41, 37, 28, 16,  3, 61,
        54, 58, 35, 52, 50, 42, 21, 44, 38, 32, 29, 23, 17, 11,  4, 62,
        46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30,  9, 24, 13, 18,  8, 12,  7,  6,  5, 63,
};

inline uint32_t MSB_DeBruijn64(uint64_t v )
{
    v|=(v>>1);
    v|=(v>>2);
    v|=(v>>4);
    v|=(v>>8);
    v|=(v>>16);
    v|=(v>>32);
    return MultiplyDeBruijnBitPosition64[(uint64_t)(v*0x03F79D71B4CB0A89ULL)>>58];
}


const unsigned char debruijn_ctz32[32] = {
    0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};
inline uint32_t ff_ctz_c(int32_t v)  //uint32_t warning
{
    return debruijn_ctz32[(uint32_t)((v&-v)*0x077CB531U)>>27];
}

//static 
const unsigned char debruijn_ctz64[64]={
    0,1,2,53,3,7,54,27,4,38,41,8,34,55,48,28,
    62,5,39,46,44,42,22,9,24,35,59,56,49,18,29,11,
    63,52,6,26,37,40,33,47,61,45,43,21,23,58,17,10,
    51,25,36,32,60,20,57,16,50,31,19,15,30,14,13,12
};
inline uint32_t  ff_ctzll_c(int64_t v)  //uint warning
{
    return debruijn_ctz64[(uint64_t)((v&-v)*0x022FDD63CC95386DU)>>58];
}


//from https://blog.csdn.net/xueyan0096/article/details/83785384
inline uint32_t lsbDeBruijn32(uint32_t n )  //to test
{
    n &= (~n + 1);
    return debruijn_ctz32[(n*0x077CB531U)>>27];
}

const unsigned char DeBruijnLSB64Lookup[64] =
{
    0,1,56,2,57,49,28,3,61,58,42,50,38,29,17,4,
    62,47,59,36,45,43,51,22,53,39,33,30,24,18,12,5,
    63,55,48,27,60,41,37,16,46,35,44,21,52,32,23,11,
    54,26,40,15,34,20,31,10,25,14,19,9,13,8,7,6
};
inline uint32_t lsbDeBruijn64(uint64_t n)
{
    n &= (~n + 1);
    return DeBruijnLSB64Lookup[(n*0x03F79D71B4CA8B09ULL)>>58];
}


inline unsigned int msb32_v11(unsigned int x)
{
    if(x==0)  return 0;
    static const unsigned int bval[]={0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4};
    int r =-1;  //old 0
    if(x&0xFFFF0000){r+=16;x>>=16;}
    if(x&0x0000FF00){r+=8;x>>=8;}
    if(x&0x000000F0){r+=4;x>>=4;}
    return r + bval[x];   //old
    //return r + bval[x]-1;
}

inline unsigned int msb32_v22(unsigned int x)   //linux fls
{
    int r=31;   //old 32
    if(!x)  return 0;
    if(!(x&0xffff0000u)){
        x<<=16;
        r-=16;
    }
    if(!(x&0xff000000u)){
        x<<=8;
        r-=8;
    }
    if(!(x&0xf0000000UL)){
        x<<=4;
        r-=4;
    }
    if(!(x&0xc0000000UL)){
        x<<=2;
        r-=2;
    }
    if(!(x&0x80000000UL)){
        x<<=1;
        r-=1;
    }
    return r;
}


//求一个二进制数最高位1的索引位置——最高效的算法 汇编指令BSF找值为1的最专低bit //BSR找值为1的最高bit 
//二进制数最高位为1的索引位置查找表,从0开始，若为0,虽然没有为1的bit值也返回0。
inline unsigned int getHightBit1PosV0(unsigned int n)  //正确低效的方法
{
    if (n==0)  return 0;
    int c=-1;
    while(n)
    {
        n>>=1;
        c++;
    }
    return c;
}


//基于查找表方法的各种版本
//各种不同大小的表根据需要选择
const unsigned char table_Hight1BitPos16[16]={0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3};
extern const unsigned char table_Hight1BitPos256[256];
extern const unsigned char table_Hight1BitPos64k[65536];
//极限优化，避免一次加法实现更多情况的表，暂无必要
//extern const unsigned char table_Hight1BitPos256add8[256];  //

inline unsigned int getHightBit1PosFastV1(unsigned int n)  //fast 4200kXhz
{
    unsigned short hTmp16=(unsigned short)(n>>16);    //比用unsigned int 更快
    if (hTmp16==0)  //高16位为零
    {
             return table_Hight1BitPos64k[n];
    }
    else
    {
            return table_Hight1BitPos64k[hTmp16]+16;
    }
}

inline unsigned int getHightBit1PosFastV2(unsigned int n)  //fast  3000kXhz
{
    unsigned short hTmp16=(unsigned short)(n>>16);    //比unsigned int 更快
    unsigned char htmp8;
    if (hTmp16!=0)  //高16位非零
    {
        htmp8=(unsigned char)(hTmp16>>8);   //
        if (htmp8)    //高16位中的高8位非零
        {
            return table_Hight1BitPos256[htmp8]+24;
        }
        else
        {
            return table_Hight1BitPos256[hTmp16]+16;
        }
    }
    else
    {
        htmp8=(unsigned char)(n>>8);
        if (htmp8)
        {
            return table_Hight1BitPos256[htmp8]+8;
        }
        else
        {
            return table_Hight1BitPos256[n];
        }
    }
}

inline unsigned int getHightBit1PosFastV3(unsigned int n)  //fast 2500kXhz
{
    unsigned short hTmp16=(unsigned short)(n>>16);    //比unsigned int 更快
    unsigned char htmp8;
    unsigned char htmp4;
    if (hTmp16!=0)  //高16位非零
    {
        htmp8=(unsigned char)(hTmp16>>8);   //
        htmp4=(htmp8>>4);
        if (htmp8) 
        {
            if (htmp4)
            {
                return table_Hight1BitPos16[htmp4]+28;
            }
            else
            {
                return table_Hight1BitPos16[htmp8]+24;
            }
        }
        else
        {
            if (htmp4)
            {
                return table_Hight1BitPos16[htmp4]+20;
            }
            else
            {
                return table_Hight1BitPos16[htmp8]+16;
            }
        }
    }
    else
    {
        htmp8=(unsigned char)(n>>8);
        htmp4=(htmp8>>4);
        if (htmp8)
        {
            if (htmp4)
            {
                return table_Hight1BitPos16[htmp4]+12;
            }
            else
            {
                return table_Hight1BitPos16[htmp8]+8;
            }
        }
        else
        {
            if (htmp4)
            {
                return table_Hight1BitPos16[htmp4]+4;
            }
            else
            {
                return table_Hight1BitPos16[htmp8];
            }
        }
    }
}
inline unsigned int getHightBit1PosFastV1(uint64_t n)  //fast
{
    unsigned int h32bit=(unsigned int)(n>>32);
    if (h32bit!=0)
    {
        return getHightBit1PosFastV1(h32bit)+32;
    }
    else
    {
        return getHightBit1PosFastV1((uint32_t)n);
    }
}



//二进制数最高位为1的索引位置,从0开始,若为0,虽然没有为1的bit值也返回0。经测试,用汇编实现是最快的版本,
inline unsigned int getHightBit1Pos(uint32_t n)
{
    //if(n==0)  return 0;          //=0 特殊情况,放到使用处考虑   TODO:
    return BSR_uint32(n);     //fastest 8300kXhz
    //return getHightBit1PosFastV1(n);
}
inline unsigned int getHightBit1Pos(uint64_t n)
{
    //if(n==0)  return 0;         //=0 特殊情况,放到使用处考虑   TODO:
    return BSR_uint64(n);
    //return getHightBit1PosFastV1(n);
}

//二进制数从右侧最低为到左侧最高位，第一个出现bit1的位置，若为0,虽然没有为1的bit值也返回0
inline unsigned int getLowBit1Pos(uint32_t n)
{
    return BSF_uint32(n);
}
inline unsigned int getLowBit1Pos(uint64_t n)
{
    return BSF_uint64(n);
}



extern const unsigned int tablepow2_uint32[32];
inline unsigned int get_NextLogof2(unsigned int block_size)
{
    //uint64_t sizeTmp=1;
    //for(unsigned int n=0;n<31;n++)
    //{
    //    if(sizeTmp>=block_size)
    //    {
    //        return n;
    //    }
    //    sizeTmp*=2;
    //    sizeTmp+=1;
    //}
    //return 31;

    for(int n=0;n<31;n++)
    {
        if(tablepow2_uint32[n]>=block_size)
        {
            return n;
        }
    }
    return 31;
}

extern const uint64_t tablepow2_uint64[64];
inline unsigned int get_NextLogof2(uint64_t block_size)
{
#if 1
    uint64_t sizeTmp=1;
    for(unsigned int n=0;n<63;n++)
    {
        if(sizeTmp>=block_size)
        {
            return n;
        }
        sizeTmp*=2;
        sizeTmp+=1;
    }
    return 63;

    //1<<0, 从2开始
    //for(int n=0;n<63;n++)
    //{
    //    if(tablepow2_uint64[n]>=block_size)
    //    {
    //            return n;
    //    }
    //}
    //return 63;
#else


    //
    ////平均二分法
    //if (block_size<tablepow2_uint64[32])
    //{
    //    if (block_size<tablepow2_uint64[16])
    //    {
    //        if (block_size<tablepow2_uint64[8])
    //        {
    //            for(int n=0;n<8;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //        else
    //        {
    //            for(int n=8;n<16;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //    }
    //    else
    //    {
    //        if (block_size<tablepow2_uint64[24])
    //        {
    //            for(int n=16;n<24;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //        else
    //        {
    //            for(int n=24;n<32;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //    }
    //}
    //else
    //{
    //    if (block_size<tablepow2_uint64[48])
    //    {
    //        if (block_size<tablepow2_uint64[40])
    //        {
    //            for(int n=32;n<40;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //        else
    //        {
    //            for(int n=40;n<48;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //    }
    //    else
    //    {
    //        if (block_size<tablepow2_uint64[48])
    //        {
    //            for(int n=48;n<56;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //        else
    //        {
    //            for(int n=56;n<63;n++)  //for(int n=56;n<64;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //    }
    //}
    //return 63;

    //
    // //小数优先二分法
    //if (block_size<tablepow2_uint64[16])
    //{
    //    if (block_size<tablepow2_uint64[8])
    //    {
    //        if (block_size<tablepow2_uint64[4])
    //        {
    //            for(int n=0;n<4;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //        else
    //        {
    //            for(int n=4;n<8;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //    }
    //    else
    //    {
    //        if (block_size<tablepow2_uint64[12])
    //        {
    //            for(int n=8;n<12;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //        else
    //        {
    //            for(int n=12;n<16;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //    }
    //}
    //else
    //{
    //    if (block_size<tablepow2_uint64[32])
    //    {
    //        if (block_size<tablepow2_uint64[24])
    //        {
    //            for(int n=16;n<24;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //        else
    //        {
    //            for(int n=24;n<32;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //    }
    //    else
    //    {
    //        if (block_size<tablepow2_uint64[48])
    //        {
    //            for(int n=48;n<56;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //        else
    //        {
    //            for(int n=56;n<63;n++)  //for(int n=56;n<64;n++)
    //            {
    //                if(tablepow2_uint64[n]>=block_size)
    //                {
    //                    return n;
    //                }
    //            }
    //        }
    //    }
    //}
    //return 63;

#endif
}


//improve by lizulin 2018-2020
//获一个数某bitpos位置取后N个bit的值,不包含bitpos本身
template <const unsigned int N>  //N=1,2,3,4,5
inline unsigned int getRightNBit(uint32_t x,unsigned int bitPos)  //bitPos>N+1
{
    static_assert(N<sizeof(x)*8,"out of range");
    x=x>>(bitPos-N);
    const uint32_t Mask=((1<<N)-1);    //TODO:极大值越界？
    x&=Mask;   
    return x;
};
//获一个数某bitpos位置取后N个bit的值,不包含bitpos本身
template <const unsigned int N>  //N=1,2,3,4,5
inline uint64_t getRightNBit(uint64_t x,unsigned int bitPos)  //bitPos>N+1
{
    static_assert(N<sizeof(x)*8,"out of range");
    x=x>>(bitPos-N);
    const uint64_t Mask=((1ULL<<N)-1);  //TODO:极大值越界？
    x&=Mask;   
    return x;
};

////获一个数某bitpos位置取后N个bit的值,连同bitpos本身
//template <const unsigned int N>  //N=1,2,3,4,5
//inline unsigned int getRightNBit(uint32_t x,unsigned int bitPos)  //bitPos>N+1
//{
//    x=x>>(bitPos+1-N);
//    const uint32_t Mask=((1<<(N+1))-1);   //TODO:极大值越界？
//    x&=Mask;   
//    return x;
//};
////获一个数某bitpos位置取后N个bit的值,连同bitpos本身
//template <const unsigned int N>  //N=1,2,3,4,5
//inline uint64_t getRightNBit(uint64_t x,unsigned int bitPos)  //bitPos>N+1
//{
//    x=x>>(bitPos+1-N);
//    const uint64_t Mask=((1ULL<<(N+1))-1);  //TODO:极大值越界？
//    x&=Mask;   
//    return x;
//};


//分块号与其管理的大小来回转换策略--高效实现
//根据数据大小,快速找到分块号,快速方法
//1-2-4-8-16-32-64-128-256-512-1024.....        //直接2次幂倍增
//1-2-3-4--6-8-12-16-24-32-48-64-96-128      //2次幂之间插入2个中间值的递增
//1-2-3-4-5-6-7-8-10-12-14-16-20-24-28-32-40-48-56-64-96-128     //2次幂之间插入4个中间值的递增

//template <const size_t N>  //N=1,2,3,4,5
//inline uint32_t  SizeToBlockNumStrategy(uint32_t  nSize)
//{
//    unsigned int h1pos=getHightBit1Pos(nSize);
//    if (N==1)
//    {
//        return h1pos;
//    }
//    uint32_t subIndex=getRightNBit<N>(nSize,h1pos);
//    return (h1pos<<N)+subIndex;
//}
template <const size_t N>  //N=1,2,3,4,5
inline uint32_t  SizeToBlockNumStrategy(uint64_t  nSize)
{
    unsigned int h1pos=getHightBit1Pos(nSize);
    if (N==1)
    {
        return h1pos;
    }
    uint64_t subIndex=getRightNBit<N-1>(nSize,h1pos);
    uint64_t index=(h1pos<<(N-1))+subIndex;   
    return (uint32_t)index;                          //blocknum不会太大
}

template <const size_t N>  //N=1,2,3,4,5
inline uint64_t BlockNumToSizeStrategy(uint32_t nBlock)
{
    const uint64_t BlockL=1<<(N+1);
    if(nBlock<BlockL)             //分块号比较小时 特别处理,策略完善与描述待续
    {
        //return  BlockL<<N-1;
        if(N<=5)
        {
            return 16;     //16  //N=3,4,5 
        }
        return BlockL*2;   //to be confirn
    }

    const uint64_t BMax=64*(1<<(N-1))-1;
    if (nBlock>=BMax)   //极限情况考虑,以下移位运算会溢出
    {
        return UINT64_MAX;   //UINT32_MAX or UINT64_MAX
    }

    if (N==1)
    {
        uint64_t nSize=(1ull<<(nBlock+1))-1;
        return nSize;
    }
    const uint64_t P=1<<(N-1);
    nBlock+=1;
    uint64_t oBlock=nBlock/P;
    uint64_t oldSubBlock=(nBlock%P);
    uint64_t oSize=(1ull<<(oBlock));
    uint64_t mSize=oSize-1;
    uint64_t nSize=mSize+(oldSubBlock)*(oSize/P);
    return nSize;
}




//C语言快速取以2为底的对数的方法
inline int FastLog2(int x)
{
    float fx;
    unsigned long ix, exp;
    fx = (float)x;
    ix = *(unsigned long*)&fx;
    exp = (ix>>23)&0xFF;
    return exp-127;
}

//C语言，快速求以2为底的整数对数(int_log2函数)
//在做AI版2048时，没必要用c库求对数的函数（浮点数），整数对数的算法速度要快很多。
//static int int_log2 (unsigned int x)
//{
//    static const unsigned char log_2[256] = {
//        0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
//        6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
//        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
//        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
//        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
//        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
//        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
//        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
//    };
//    int l = -1;
//    while (x >= 256) { l += 8; x >>= 8; }
//    return l + log_2[x];
//}


//正整数的 二进制表示 中 从低位向高位 第1个 0 出现位置 的计算方法
inline int find_first_0_occur_binary_expression(int n)
{
    int zeroPosition = 0;
    while ((n&1)!=0)
    {
        zeroPosition++;
        n = n >> 1;
    }
    return zeroPosition;
}

//正整数的 二进制表示 中 从低位向高位 第1个 1 出现位置 的计算方法
inline int find_first_1_occur_binary_expression(int n)
{
    int first_one_Position = 0;
    while ((n&1)!=1)
    {
        first_one_Position++;
        n = n >> 1;
    }
    return first_one_Position;
}




//_ftol 是什么？ 当你写 C 程序的时候，(int)float_v 就会被编译器产生一个对 _ftol 这个 CRT 函数的调用。
inline int my_ftol(float f)
{     
    int a         = *(int*)(&f);    
    int sign      = (a>>31);     
    int mantissa  = (a&((1<<23)-1))|(1<<23);   
    int exponent  = ((a&0x7fffffff)>>23)-127;    
    int r         = ((unsigned int)(mantissa)<<8)>>(31-exponent);    
    return ((r ^ (sign)) - sign ) &~ (exponent>>31);       
}



//位运算--统计一个数的二进制序列中1的个数
inline int count_one(int num)
{
    int count = 0;
    int i = 0;
    for (i = 0;i < 32;i++)
    {
        if (((num>>i)&1)==1)
        {
            count++;
        }
    }
    return count;
}

// https://blog.csdn.net/peiyao456/article/details/51724099
inline int count_onesV2(int num)
{
    int m_2 = 0x55555555;
    int m_4 = 0x33333333;
    int m_8 = 0x0f0f0f0f;
    int m_16 = 0x00ff00ff;
    int b = (m_2&num) + ((num >> 1)&m_2);
    int c = (m_4&b) + ((b >> 2)&m_4);
    int d = (m_8&c) + ((c >> 4)&m_8);
    int g = (m_16&d) + ((d >> 8)&m_16);
    return g;
}


////////////////////////////////////////////////////////////////////////////////

/** Returns the number of bits in a 32-bit integer. */
inline int countNumberOfBits (uint32 n)
{
    n -= ((n >> 1) & 0x55555555);
    n =  (((n >> 2) & 0x33333333) + (n & 0x33333333));
    n =  (((n >> 4) + n) & 0x0f0f0f0f);
    n += (n >> 8);
    n += (n >> 16);
    return (int) (n & 0x3f);
}

/** Returns the number of bits in a 64-bit integer. */
inline int countNumberOfBits (uint64 n)
{
    return countNumberOfBits ((uint32) n) + countNumberOfBits ((uint32) (n >> 32));
}


//查表法，分段查表然后累加，待续
//inline  int count_onesV3(int num)
//{
//
//}


/** Scans an array of values, returning the minimum value that it contains. */
template <typename T>
inline T findMinimum (const T* data,size_t numValues)
{
    if (numValues==0) return;
    T result = *data++;
    while (--numValues > 0) 
    {
        T v = *data++;
        if (v < result)
        {
            result = v;
        }
    }
    return result;
}

/** Scans an array of values, returning the maximum value that it contains. */
template <typename T>
inline T findMaximum (const T* values,size_t numValues)
{
    if (numValues==0) return;
    T result = *values++;
    while (--numValues > 0) // (> 0 rather than >= 0 because we've already taken the first sample)
    {
        T v = *values++;
        if (result < v)
        {
            result = v;
        }
    }
    return result;
}

/** Scans an array of values, returning the minimum and maximum values that it contains. */
template <typename T>
inline void findMinAndMax (const T* values,size_t numValues, T& lowest, T& highest)
{
    if (numValues==0) return;
    T minV = *values++;
    T maxV = minV;
    while (--numValues > 0)
    {
            auto v = *values++;
            if (maxV < v)  maxV = v;
            if (v < minV)  minV = v;
    }
    lowest = minV;
    highest = maxV;
}


////是否素数
////math.h required
//bool is_prime(int x) 
//{
//    int val=sqrt(x);
//    for (int k = 1; k <= val; k++)
//    {
//        if (!(x%k))
//        {
//            return false;
//        }
//    }
//    return true;
//}
//

////打印小于N的素数
//void getPrimeTable(int x,int* result,int& cnt) 
//{  
//    cnt=0;
//    for (int i = 2; i <= x; i++)
//    {
//        if (is_prime(i))
//        {
//            result[cnt++] = i;
//         }
//    }
//}


//求最大公约数（GCD）和最小公倍数(LCM)
//最小公倍数与最大公约数的关系：
//假设存在两个数A和B，那他们的最大公倍数就是A和B的积除以的A和B最大公约数即A*B/gcd(A,B)

//GCD欧几里德辗转相除法 递归实现 
//https://blog.csdn.net/zxh1592000/java/article/details/78846229

inline uint64_t getGCD(uint64_t a,uint64_t b)
{
    if(b == 0)
        return a;
    else
        return getGCD(b,a%b);
}

inline uint64_t getLCM(uint64_t a,uint64_t b)
{
    return a*b/getGCD(a,b);
}


template<typename T>
inline const T& Max4(const T& v1,const T& v2,const T& v3,const T& v4)
{
    const T& max=Max3(v1,v2,v3);
    if(v4>max) 
    {
        return v4;
    }
    return max;
}

template<typename T>
inline const T& Max5(const T& v1,const T& v2,const T& v3,const T& v4,const T& v5)
{
    const T& max=Max4(v1,v2,v3,v4);
    if(v5>max) 
    {
        return v5;
    }
    return max;
}

//可变参数支持待续C++11



//template<typename T,size_t N>
//inline size_t ArrayMin(T (&arry)[N])
//{
//
//}
//template<typename T,size_t N>
//inline size_t ArrayMax(T (&arry)[N])
//{
//
//}


template<typename T>
inline const T& Min4(const T& v1,const T& v2,const T& v3,const T& v4)
{
    const T& min=Min3(v1,v2,v3);
    if(v4<min) 
    {
        return v4;
    }
    return min;
}

template<typename T>
inline const T& Min5(const T& v1,const T& v2,const T& v3,const T& v4,const T& v5)
{
    const T& min=Min4(v1,v2,v3,v4);
    if(v5<min) 
    {
        return v5;
    }
    return min;
}


template <typename T>
T limitRange(const T& min,const T& max,const T& value)
{
    //assert (max<= min); // if these are in the wrong order, results are unpredictable..
    return value<min?min:(max<value?max:value);
}



#ifdef UNIT_TEST
int Test_Math();
#endif



#endif 

