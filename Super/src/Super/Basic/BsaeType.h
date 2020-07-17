#ifndef BSAETYPE_H
#define BSAETYPE_H

#include "global.h" 

//正在调试中的函数，类
#include <iostream>


NAME_SPACE_MAIN_BEGIN

//设计一种半精度浮点数，可以通过移位与float互转，不需要乘除提高效率，2，3字节浮点数
//目的在于表示像-1.0000-1.0000高精度小范围浮点数字
//未完待续--先实现简单乘除法的用-32718-32767表示-1.000-1.000
class HalfFloat
{
public: 
	//char mByte[2];
	short int mData;
	operator float()
	{
		float tmp=0.0F;
		//memcpy(&tmp,mByte,sizeof(mByte));
		tmp=mData/32768.0f;
		return tmp;
	}
	HalfFloat& operator =(const float F)
	{
		//memcpy(mByte,&F,sizeof(mByte));
		mData=F*32768.0f;
		return *this;
	}
	void printInterValue()
	{
		std::cout<<"HalfFloat: Inter="<<this->mData<<std::endl;
	}
	void printValue()
	{
		std::cout<<"HalfFloat: Float="<<this->operator float()<<std::endl;
	}
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//from opencv core/cvdef.h refactor by lizulin 
class float16_t
{
   union Cv16suf
    {
        short i;
        ushort u;
    };
    union Cv32suf
    {
        int i;
        unsigned u;
        float f;
    };
public:
    float16_t() : w(0) {}
    explicit float16_t(float x)
    {
        Cv32suf in;
        in.f = x;
        unsigned sign = in.u & 0x80000000;
        in.u ^= sign;

        if( in.u >= 0x47800000 )
            w = (ushort)(in.u > 0x7f800000 ? 0x7e00 : 0x7c00);
        else
        {
            if (in.u < 0x38800000)
            {
                in.f += 0.5f;
                w = (ushort)(in.u - 0x3f000000);
            }
            else
            {
                unsigned t = in.u + 0xc8000fff;
                w = (ushort)((t + ((in.u >> 13) & 1)) >> 13);
            }
        }

        w = (ushort)(w | (sign >> 16));
    }

    operator float() const
    {
        Cv32suf out;
        unsigned t = ((w & 0x7fff) << 13) + 0x38000000;
        unsigned sign = (w & 0x8000) << 16;
        unsigned e = w & 0x7c00;

        out.u = t + (1 << 23);
        out.u = (e >= 0x7c00 ? t + 0x38000000 :
            e == 0 ? (static_cast<void>(out.f -= 6.103515625e-05f), out.u) : t) | sign;
        return out.f;
    }

    static float16_t fromBits(ushort b)
    {
        float16_t result;
        result.w = b;
        return result;
    }
    static float16_t zero()
    {
        float16_t result;
        result.w = (ushort)0;
        return result;
    }
    ushort bits() const { return w; }
protected:
    ushort w;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//from opencv interface.h
typedef signed char schar;
#  define CV_BIG_INT(n)   n##LL
#  define CV_BIG_UINT(n)  n##ULL

#define CV_CN_MAX     512
#define CV_CN_SHIFT   3
#define CV_DEPTH_MAX  (1 << CV_CN_SHIFT)

#define CV_8U   0
#define CV_8S   1
#define CV_16U  2
#define CV_16S  3
#define CV_32S  4
#define CV_32F  5
#define CV_64F  6
#define CV_16F  7

#define CV_MAT_DEPTH_MASK       (CV_DEPTH_MAX - 1)
#define CV_MAT_DEPTH(flags)     ((flags) & CV_MAT_DEPTH_MASK)

#define CV_MAKETYPE(depth,cn) (CV_MAT_DEPTH(depth) + (((cn)-1) << CV_CN_SHIFT))
#define CV_MAKE_TYPE CV_MAKETYPE

#define CV_8UC1 CV_MAKETYPE(CV_8U,1)
#define CV_8UC2 CV_MAKETYPE(CV_8U,2)
#define CV_8UC3 CV_MAKETYPE(CV_8U,3)
#define CV_8UC4 CV_MAKETYPE(CV_8U,4)
#define CV_8UC(n) CV_MAKETYPE(CV_8U,(n))

#define CV_8SC1 CV_MAKETYPE(CV_8S,1)
#define CV_8SC2 CV_MAKETYPE(CV_8S,2)
#define CV_8SC3 CV_MAKETYPE(CV_8S,3)
#define CV_8SC4 CV_MAKETYPE(CV_8S,4)
#define CV_8SC(n) CV_MAKETYPE(CV_8S,(n))

#define CV_16UC1 CV_MAKETYPE(CV_16U,1)
#define CV_16UC2 CV_MAKETYPE(CV_16U,2)
#define CV_16UC3 CV_MAKETYPE(CV_16U,3)
#define CV_16UC4 CV_MAKETYPE(CV_16U,4)
#define CV_16UC(n) CV_MAKETYPE(CV_16U,(n))

#define CV_16SC1 CV_MAKETYPE(CV_16S,1)
#define CV_16SC2 CV_MAKETYPE(CV_16S,2)
#define CV_16SC3 CV_MAKETYPE(CV_16S,3)
#define CV_16SC4 CV_MAKETYPE(CV_16S,4)
#define CV_16SC(n) CV_MAKETYPE(CV_16S,(n))

#define CV_32SC1 CV_MAKETYPE(CV_32S,1)
#define CV_32SC2 CV_MAKETYPE(CV_32S,2)
#define CV_32SC3 CV_MAKETYPE(CV_32S,3)
#define CV_32SC4 CV_MAKETYPE(CV_32S,4)
#define CV_32SC(n) CV_MAKETYPE(CV_32S,(n))

#define CV_32FC1 CV_MAKETYPE(CV_32F,1)
#define CV_32FC2 CV_MAKETYPE(CV_32F,2)
#define CV_32FC3 CV_MAKETYPE(CV_32F,3)
#define CV_32FC4 CV_MAKETYPE(CV_32F,4)
#define CV_32FC(n) CV_MAKETYPE(CV_32F,(n))

#define CV_64FC1 CV_MAKETYPE(CV_64F,1)
#define CV_64FC2 CV_MAKETYPE(CV_64F,2)
#define CV_64FC3 CV_MAKETYPE(CV_64F,3)
#define CV_64FC4 CV_MAKETYPE(CV_64F,4)
#define CV_64FC(n) CV_MAKETYPE(CV_64F,(n))

#define CV_16FC1 CV_MAKETYPE(CV_16F,1)
#define CV_16FC2 CV_MAKETYPE(CV_16F,2)
#define CV_16FC3 CV_MAKETYPE(CV_16F,3)
#define CV_16FC4 CV_MAKETYPE(CV_16F,4)
#define CV_16FC(n) CV_MAKETYPE(CV_16F,(n))








////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//trains.hpp
template<typename _Tp> class DataType
{
public:
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
    typedef _Tp         value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 1,
           depth        = -1,
           channels     = 1,
           fmt          = 0,
           type = CV_MAKETYPE(depth, channels)
         };
#endif
};

template<> class DataType<bool>
{
public:
    typedef bool        value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_8U,
           channels     = 1,
           fmt          = (int)'u',
           type         = CV_MAKETYPE(depth, channels)
         };
};

template<> class DataType<uchar>
{
public:
    typedef uchar       value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_8U,
           channels     = 1,
           fmt          = (int)'u',
           type         = CV_MAKETYPE(depth, channels)
         };
};

template<> class DataType<schar>
{
public:
    typedef schar       value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_8S,
           channels     = 1,
           fmt          = (int)'c',
           type         = CV_MAKETYPE(depth, channels)
         };
};

template<> class DataType<char>
{
public:
    typedef schar       value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_8S,
           channels     = 1,
           fmt          = (int)'c',
           type         = CV_MAKETYPE(depth, channels)
         };
};

template<> class DataType<ushort>
{
public:
    typedef ushort      value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_16U,
           channels     = 1,
           fmt          = (int)'w',
           type         = CV_MAKETYPE(depth, channels)
         };
};

template<> class DataType<short>
{
public:
    typedef short       value_type;
    typedef int         work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_16S,
           channels     = 1,
           fmt          = (int)'s',
           type         = CV_MAKETYPE(depth, channels)
         };
};

template<> class DataType<int>
{
public:
    typedef int         value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_32S,
           channels     = 1,
           fmt          = (int)'i',
           type         = CV_MAKETYPE(depth, channels)
         };
};

template<> class DataType<float>
{
public:
    typedef float       value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_32F,
           channels     = 1,
           fmt          = (int)'f',
           type         = CV_MAKETYPE(depth, channels)
         };
};

template<> class DataType<double>
{
public:
    typedef double      value_type;
    typedef value_type  work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_64F,
           channels     = 1,
           fmt          = (int)'d',
           type         = CV_MAKETYPE(depth, channels)
         };
};

template<> class DataType<float16_t>
{
public:
    typedef float16_t   value_type;
    typedef float       work_type;
    typedef value_type  channel_type;
    typedef value_type  vec_type;
    enum { generic_type = 0,
           depth        = CV_16F,
           channels     = 1,
           fmt          = (int)'h',
           type         = CV_MAKETYPE(depth, channels)
         };
};

/** @brief A helper class for cv::DataType

The class is specialized for each fundamental numerical data type supported by OpenCV. It provides
DataDepth<T>::value constant.
*/
template<typename _Tp> class DataDepth
{
public:
    enum
    {
        value = DataType<_Tp>::depth,
        fmt   = DataType<_Tp>::fmt
    };
};


#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED

template<int _depth> class TypeDepth
{
#ifdef OPENCV_TRAITS_ENABLE_LEGACY_DEFAULTS
    enum { depth = CV_USRTYPE1 };
    typedef void value_type;
#endif
};

template<> class TypeDepth<CV_8U>
{
    enum { depth = CV_8U };
    typedef uchar value_type;
};

template<> class TypeDepth<CV_8S>
{
    enum { depth = CV_8S };
    typedef schar value_type;
};

template<> class TypeDepth<CV_16U>
{
    enum { depth = CV_16U };
    typedef ushort value_type;
};

template<> class TypeDepth<CV_16S>
{
    enum { depth = CV_16S };
    typedef short value_type;
};

template<> class TypeDepth<CV_32S>
{
    enum { depth = CV_32S };
    typedef int value_type;
};

template<> class TypeDepth<CV_32F>
{
    enum { depth = CV_32F };
    typedef float value_type;
};

template<> class TypeDepth<CV_64F>
{
    enum { depth = CV_64F };
    typedef double value_type;
};

template<> class TypeDepth<CV_16F>
{
    enum { depth = CV_16F };
    typedef float16_t value_type;
};

#endif

//! @}

namespace traits {

namespace internal {
#define CV_CREATE_MEMBER_CHECK(X) \
template<typename T> class CheckMember_##X { \
    struct Fallback { int X; }; \
    struct Derived : T, Fallback { }; \
    template<typename U, U> struct Check; \
    typedef char CV_NO[1]; \
    typedef char CV_YES[2]; \
    template<typename U> static CV_NO & func(Check<int Fallback::*, &U::X> *); \
    template<typename U> static CV_YES & func(...); \
public: \
    typedef CheckMember_##X type; \
    enum { value = sizeof(func<Derived>(0)) == sizeof(CV_YES) }; \
};

CV_CREATE_MEMBER_CHECK(fmt)
CV_CREATE_MEMBER_CHECK(type)

} // namespace internal


template<typename T>
struct Depth
{ enum { value = DataType<T>::depth }; };

template<typename T>
struct Type
{ enum { value = DataType<T>::type }; };

/** Similar to traits::Type<T> but has value = -1 in case of unknown type (instead of compiler error) */
template<typename T, bool available = internal::CheckMember_type< DataType<T> >::value >
struct SafeType {};

template<typename T>
struct SafeType<T, false>
{ enum { value = -1 }; };

template<typename T>
struct SafeType<T, true>
{ enum { value = Type<T>::value }; };


template<typename T, bool available = internal::CheckMember_fmt< DataType<T> >::value >
struct SafeFmt {};

template<typename T>
struct SafeFmt<T, false>
{ enum { fmt = 0 }; };

template<typename T>
struct SafeFmt<T, true>
{ enum { fmt = DataType<T>::fmt }; };

} // namespace



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//base.hpp
//#include <initializer>  

template<typename _Tp, int cn> class Vec;
template<typename _Tp, int m, int n> class Matx;

template<typename _Tp> class Complex;
template<typename _Tp> class Point_;
template<typename _Tp> class Point3_;
template<typename _Tp> class Size_;
template<typename _Tp> class Rect_;
template<typename _Tp> class Scalar_;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//saturate.hpp

//! @addtogroup core_utils
//! @{

/////////////// saturate_cast (used in image & signal processing) ///////////////////

/** @brief Template function for accurate conversion from one primitive type to another.

 The function saturate_cast resembles the standard C++ cast operations, such as static_cast\<T\>()
 and others. It perform an efficient and accurate conversion from one primitive type to another
 (see the introduction chapter). saturate in the name means that when the input value v is out of the
 range of the target type, the result is not formed just by taking low bits of the input, but instead
 the value is clipped. For example:
 @code
 uchar a = saturate_cast<uchar>(-100); // a = 0 (UCHAR_MIN)
 short b = saturate_cast<short>(33333.33333); // b = 32767 (SHRT_MAX)
 @endcode
 Such clipping is done when the target type is unsigned char , signed char , unsigned short or
 signed short . For 32-bit integers, no clipping is done.

 When the parameter is a floating-point value and the target type is an integer (8-, 16- or 32-bit),
 the floating-point value is first rounded to the nearest integer and then clipped if needed (when
 the target type is 8- or 16-bit).

 This operation is used in the simplest or most complex image processing functions in OpenCV.

 @param v Function parameter.
 @sa add, subtract, multiply, divide, Mat::convertTo
 */
template<typename _Tp> static inline _Tp saturate_cast(uchar v)    { return _Tp(v); }
/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(schar v)    { return _Tp(v); }
/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(ushort v)   { return _Tp(v); }
/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(short v)    { return _Tp(v); }
/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(unsigned v) { return _Tp(v); }
/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(int v)      { return _Tp(v); }
/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(float v)    { return _Tp(v); }
/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(double v)   { return _Tp(v); }
/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(int64 v)    { return _Tp(v); }
/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(uint64 v)   { return _Tp(v); }

template<> inline uchar saturate_cast<uchar>(schar v)        { return (uchar)std::max((int)v, 0); }
template<> inline uchar saturate_cast<uchar>(ushort v)       { return (uchar)std::min((unsigned)v, (unsigned)UCHAR_MAX); }
template<> inline uchar saturate_cast<uchar>(int v)          { return (uchar)((unsigned)v <= UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
template<> inline uchar saturate_cast<uchar>(short v)        { return saturate_cast<uchar>((int)v); }
template<> inline uchar saturate_cast<uchar>(unsigned v)     { return (uchar)std::min(v, (unsigned)UCHAR_MAX); }
template<> inline uchar saturate_cast<uchar>(float v)        { int iv = Round(v); return saturate_cast<uchar>(iv); }
template<> inline uchar saturate_cast<uchar>(double v)       { int iv = Round(v); return saturate_cast<uchar>(iv); }
template<> inline uchar saturate_cast<uchar>(int64 v)        { return (uchar)((uint64)v <= (uint64)UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
template<> inline uchar saturate_cast<uchar>(uint64 v)       { return (uchar)std::min(v, (uint64)UCHAR_MAX); }

template<> inline schar saturate_cast<schar>(uchar v)        { return (schar)std::min((int)v, SCHAR_MAX); }
template<> inline schar saturate_cast<schar>(ushort v)       { return (schar)std::min((unsigned)v, (unsigned)SCHAR_MAX); }
template<> inline schar saturate_cast<schar>(int v)          { return (schar)((unsigned)(v-SCHAR_MIN) <= (unsigned)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN); }
template<> inline schar saturate_cast<schar>(short v)        { return saturate_cast<schar>((int)v); }
template<> inline schar saturate_cast<schar>(unsigned v)     { return (schar)std::min(v, (unsigned)SCHAR_MAX); }
template<> inline schar saturate_cast<schar>(float v)        { int iv = Round(v); return saturate_cast<schar>(iv); }
template<> inline schar saturate_cast<schar>(double v)       { int iv = Round(v); return saturate_cast<schar>(iv); }
template<> inline schar saturate_cast<schar>(int64 v)        { return (schar)((uint64)((int64)v-SCHAR_MIN) <= (uint64)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN); }
template<> inline schar saturate_cast<schar>(uint64 v)       { return (schar)std::min(v, (uint64)SCHAR_MAX); }

template<> inline ushort saturate_cast<ushort>(schar v)      { return (ushort)std::max((int)v, 0); }
template<> inline ushort saturate_cast<ushort>(short v)      { return (ushort)std::max((int)v, 0); }
template<> inline ushort saturate_cast<ushort>(int v)        { return (ushort)((unsigned)v <= (unsigned)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
template<> inline ushort saturate_cast<ushort>(unsigned v)   { return (ushort)std::min(v, (unsigned)USHRT_MAX); }
template<> inline ushort saturate_cast<ushort>(float v)      { int iv = Round(v); return saturate_cast<ushort>(iv); }
template<> inline ushort saturate_cast<ushort>(double v)     { int iv = Round(v); return saturate_cast<ushort>(iv); }
template<> inline ushort saturate_cast<ushort>(int64 v)      { return (ushort)((uint64)v <= (uint64)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
template<> inline ushort saturate_cast<ushort>(uint64 v)     { return (ushort)std::min(v, (uint64)USHRT_MAX); }

template<> inline short saturate_cast<short>(ushort v)       { return (short)std::min((int)v, SHRT_MAX); }
template<> inline short saturate_cast<short>(int v)          { return (short)((unsigned)(v - SHRT_MIN) <= (unsigned)USHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN); }
template<> inline short saturate_cast<short>(unsigned v)     { return (short)std::min(v, (unsigned)SHRT_MAX); }
template<> inline short saturate_cast<short>(float v)        { int iv = Round(v); return saturate_cast<short>(iv); }
template<> inline short saturate_cast<short>(double v)       { int iv = Round(v); return saturate_cast<short>(iv); }
template<> inline short saturate_cast<short>(int64 v)        { return (short)((uint64)((int64)v - SHRT_MIN) <= (uint64)USHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN); }
template<> inline short saturate_cast<short>(uint64 v)       { return (short)std::min(v, (uint64)SHRT_MAX); }

template<> inline int saturate_cast<int>(unsigned v)         { return (int)std::min(v, (unsigned)INT_MAX); }
template<> inline int saturate_cast<int>(int64 v)            { return (int)((uint64)(v - INT_MIN) <= (uint64)UINT_MAX ? v : v > 0 ? INT_MAX : INT_MIN); }
template<> inline int saturate_cast<int>(uint64 v)           { return (int)std::min(v, (uint64)INT_MAX); }
template<> inline int saturate_cast<int>(float v)            { return Round(v); }
template<> inline int saturate_cast<int>(double v)           { return Round(v); }

template<> inline unsigned saturate_cast<unsigned>(schar v)  { return (unsigned)std::max(v, (schar)0); }
template<> inline unsigned saturate_cast<unsigned>(short v)  { return (unsigned)std::max(v, (short)0); }
template<> inline unsigned saturate_cast<unsigned>(int v)    { return (unsigned)std::max(v, (int)0); }
template<> inline unsigned saturate_cast<unsigned>(int64 v)  { return (unsigned)((uint64)v <= (uint64)UINT_MAX ? v : v > 0 ? UINT_MAX : 0); }
template<> inline unsigned saturate_cast<unsigned>(uint64 v) { return (unsigned)std::min(v, (uint64)UINT_MAX); }
// we intentionally do not clip negative numbers, to make -1 become 0xffffffff etc.
template<> inline unsigned saturate_cast<unsigned>(float v)  { return static_cast<unsigned>(Round(v)); }
template<> inline unsigned saturate_cast<unsigned>(double v) { return static_cast<unsigned>(Round(v)); }

template<> inline uint64 saturate_cast<uint64>(schar v)      { return (uint64)std::max(v, (schar)0); }
template<> inline uint64 saturate_cast<uint64>(short v)      { return (uint64)std::max(v, (short)0); }
template<> inline uint64 saturate_cast<uint64>(int v)        { return (uint64)std::max(v, (int)0); }
template<> inline uint64 saturate_cast<uint64>(int64 v)      { return (uint64)std::max(v, (int64)0); }

template<> inline int64 saturate_cast<int64>(uint64 v)       { return (int64)std::min(v, (uint64)LLONG_MAX); }

/** @overload */
template<typename _Tp> static inline _Tp saturate_cast(float16_t v) { return saturate_cast<_Tp>((float)v); }

// in theory, we could use a LUT for 8u/8s->16f conversion,
// but with hardware support for FP32->FP16 conversion the current approach is preferable
template<> inline float16_t saturate_cast<float16_t>(uchar v)   { return float16_t((float)v); }
template<> inline float16_t saturate_cast<float16_t>(schar v)   { return float16_t((float)v); }
template<> inline float16_t saturate_cast<float16_t>(ushort v)  { return float16_t((float)v); }
template<> inline float16_t saturate_cast<float16_t>(short v)   { return float16_t((float)v); }
template<> inline float16_t saturate_cast<float16_t>(unsigned v){ return float16_t((float)v); }
template<> inline float16_t saturate_cast<float16_t>(int v)     { return float16_t((float)v); }
template<> inline float16_t saturate_cast<float16_t>(uint64 v)  { return float16_t((float)v); }
template<> inline float16_t saturate_cast<float16_t>(int64 v)   { return float16_t((float)v); }
template<> inline float16_t saturate_cast<float16_t>(float v)   { return float16_t(v); }
template<> inline float16_t saturate_cast<float16_t>(double v)  { return float16_t((float)v); }

//! @}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//softfloat.hpp //cpp

 

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//matx.hpp


//! @addtogroup core_basic
//! @{

////////////////////////////// Small Matrix ///////////////////////////

//! @cond IGNORED
// FIXIT Remove this (especially CV_EXPORTS modifier)
struct  Matx_AddOp { Matx_AddOp() {} Matx_AddOp(const Matx_AddOp&) {} };
struct  Matx_SubOp { Matx_SubOp() {} Matx_SubOp(const Matx_SubOp&) {} };
struct  Matx_ScaleOp { Matx_ScaleOp() {} Matx_ScaleOp(const Matx_ScaleOp&) {} };
struct  Matx_MulOp { Matx_MulOp() {} Matx_MulOp(const Matx_MulOp&) {} };
struct  Matx_DivOp { Matx_DivOp() {} Matx_DivOp(const Matx_DivOp&) {} };
struct  Matx_MatMulOp { Matx_MatMulOp() {} Matx_MatMulOp(const Matx_MatMulOp&) {} };
struct  Matx_TOp { Matx_TOp() {} Matx_TOp(const Matx_TOp&) {} };
//! @endcond

/** @brief Template class for small matrices whose type and size are known at compilation time

If you need a more flexible type, use Mat . The elements of the matrix M are accessible using the
M(i,j) notation. Most of the common matrix operations (see also @ref MatrixExpressions ) are
available. To do an operation on Matx that is not implemented, you can easily convert the matrix to
Mat and backwards:
@code{.cpp}
    Matx33f m(1, 2, 3,
              4, 5, 6,
              7, 8, 9);
    cout << sum(Mat(m*m.t())) << endl;
@endcode
Except of the plain constructor which takes a list of elements, Matx can be initialized from a C-array:
@code{.cpp}
    float values[] = { 1, 2, 3};
    Matx31f m(values);
@endcode
In case if C++11 features are available, std::initializer_list can be also used to initialize Matx:
@code{.cpp}
    Matx31f m = { 1, 2, 3};
@endcode
 */
template<typename _Tp, int m, int n> class Matx
{
public:
    enum {
           rows     = m,
           cols     = n,
           channels = rows*cols,
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           depth    = traits::Type<_Tp>::value,
           type     = CV_MAKETYPE(depth, channels),
#endif
           shortdim = (m < n ? m : n)
         };

    typedef _Tp                           value_type;
    typedef Matx<_Tp, m, n>               mat_type;
    typedef Matx<_Tp, shortdim, 1> diag_type;

    //! default constructor
    Matx();

    explicit Matx(_Tp v0); //!< 1x1 matrix
    Matx(_Tp v0, _Tp v1); //!< 1x2 or 2x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2); //!< 1x3 or 3x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3); //!< 1x4, 2x2 or 4x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4); //!< 1x5 or 5x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5); //!< 1x6, 2x3, 3x2 or 6x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6); //!< 1x7 or 7x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7); //!< 1x8, 2x4, 4x2 or 8x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8); //!< 1x9, 3x3 or 9x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9); //!< 1x10, 2x5 or 5x2 or 10x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
         _Tp v4, _Tp v5, _Tp v6, _Tp v7,
         _Tp v8, _Tp v9, _Tp v10, _Tp v11); //!< 1x12, 2x6, 3x4, 4x3, 6x2 or 12x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
         _Tp v4, _Tp v5, _Tp v6, _Tp v7,
         _Tp v8, _Tp v9, _Tp v10, _Tp v11,
         _Tp v12, _Tp v13); //!< 1x14, 2x7, 7x2 or 14x1 matrix
    Matx(_Tp v0, _Tp v1, _Tp v2, _Tp v3,
         _Tp v4, _Tp v5, _Tp v6, _Tp v7,
         _Tp v8, _Tp v9, _Tp v10, _Tp v11,
         _Tp v12, _Tp v13, _Tp v14, _Tp v15); //!< 1x16, 4x4 or 16x1 matrix
    explicit Matx(const _Tp* vals); //!< initialize from a plain array

    //remove by lizulin
    //Matx(std::initializer_list<_Tp>); //!< initialize from an initializer list

    static Matx all(_Tp alpha);
    static Matx zeros();
    static Matx ones();
    static Matx eye();
    static Matx diag(const diag_type& d);
    static Matx randu(_Tp a, _Tp b);
    static Matx randn(_Tp a, _Tp b);

    //! dot product computed with the default precision
    _Tp dot(const Matx<_Tp, m, n>& v) const;

    //! dot product computed in double-precision arithmetics
    double ddot(const Matx<_Tp, m, n>& v) const;

    //! conversion to another data type
    template<typename T2> operator Matx<T2, m, n>() const;

    //! change the matrix shape
    template<int m1, int n1> Matx<_Tp, m1, n1> reshape() const;

    //! extract part of the matrix
    template<int m1, int n1> Matx<_Tp, m1, n1> get_minor(int base_row, int base_col) const;

    //! extract the matrix row
    Matx<_Tp, 1, n> row(int i) const;

    //! extract the matrix column
    Matx<_Tp, m, 1> col(int i) const;

    //! extract the matrix diagonal
    diag_type diag() const;

    //! transpose the matrix
    Matx<_Tp, n, m> t() const;

    //! invert the matrix
    Matx<_Tp, n, m> inv(int method=DECOMP_LU, bool *p_is_ok = NULL) const;

    //! solve linear system
    template<int l> Matx<_Tp, n, l> solve(const Matx<_Tp, m, l>& rhs, int flags=0) const;
    Vec<_Tp, n> solve(const Vec<_Tp, m>& rhs, int method) const;

    //! multiply two matrices element-wise
    Matx<_Tp, m, n> mul(const Matx<_Tp, m, n>& a) const;

    //! divide two matrices element-wise
    Matx<_Tp, m, n> div(const Matx<_Tp, m, n>& a) const;

    //! element access
    const _Tp& operator ()(int row, int col) const;
    _Tp& operator ()(int row, int col);

    //! 1D element access
    const _Tp& operator ()(int i) const;
    _Tp& operator ()(int i);

    Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_AddOp);
    Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_SubOp);
    template<typename _T2> Matx(const Matx<_Tp, m, n>& a, _T2 alpha, Matx_ScaleOp);
    Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_MulOp);
    Matx(const Matx<_Tp, m, n>& a, const Matx<_Tp, m, n>& b, Matx_DivOp);
    template<int l> Matx(const Matx<_Tp, m, l>& a, const Matx<_Tp, l, n>& b, Matx_MatMulOp);
    Matx(const Matx<_Tp, n, m>& a, Matx_TOp);

    _Tp val[m*n]; //< matrix elements
};

typedef Matx<float, 1, 2> Matx12f;
typedef Matx<double, 1, 2> Matx12d;
typedef Matx<float, 1, 3> Matx13f;
typedef Matx<double, 1, 3> Matx13d;
typedef Matx<float, 1, 4> Matx14f;
typedef Matx<double, 1, 4> Matx14d;
typedef Matx<float, 1, 6> Matx16f;
typedef Matx<double, 1, 6> Matx16d;

typedef Matx<float, 2, 1> Matx21f;
typedef Matx<double, 2, 1> Matx21d;
typedef Matx<float, 3, 1> Matx31f;
typedef Matx<double, 3, 1> Matx31d;
typedef Matx<float, 4, 1> Matx41f;
typedef Matx<double, 4, 1> Matx41d;
typedef Matx<float, 6, 1> Matx61f;
typedef Matx<double, 6, 1> Matx61d;

typedef Matx<float, 2, 2> Matx22f;
typedef Matx<double, 2, 2> Matx22d;
typedef Matx<float, 2, 3> Matx23f;
typedef Matx<double, 2, 3> Matx23d;
typedef Matx<float, 3, 2> Matx32f;
typedef Matx<double, 3, 2> Matx32d;

typedef Matx<float, 3, 3> Matx33f;
typedef Matx<double, 3, 3> Matx33d;

typedef Matx<float, 3, 4> Matx34f;
typedef Matx<double, 3, 4> Matx34d;
typedef Matx<float, 4, 3> Matx43f;
typedef Matx<double, 4, 3> Matx43d;

typedef Matx<float, 4, 4> Matx44f;
typedef Matx<double, 4, 4> Matx44d;
typedef Matx<float, 6, 6> Matx66f;
typedef Matx<double, 6, 6> Matx66d;

/*!
  traits
*/
template<typename _Tp, int m, int n> class DataType< Matx<_Tp, m, n> >
{
public:
    typedef Matx<_Tp, m, n>                               value_type;
    typedef Matx<typename DataType<_Tp>::work_type, m, n> work_type;
    typedef _Tp                                           channel_type;
    typedef value_type                                    vec_type;

    enum { generic_type = 0,
           channels     = m * n,
           fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };
};

namespace traits {
template<typename _Tp, int m, int n>
struct Depth< Matx<_Tp, m, n> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp, int m, int n>
struct Type< Matx<_Tp, m, n> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, n*m) }; };
} // namespace


/** @brief  Comma-separated Matrix Initializer
*/
template<typename _Tp, int m, int n> class MatxCommaInitializer
{
public:
    MatxCommaInitializer(Matx<_Tp, m, n>* _mtx);
    template<typename T2> MatxCommaInitializer<_Tp, m, n>& operator , (T2 val);
    Matx<_Tp, m, n> operator *() const;

    Matx<_Tp, m, n>* dst;
    int idx;
};

/*
 Utility methods
*/
template<typename _Tp, int m> static double determinant(const Matx<_Tp, m, m>& a);
template<typename _Tp, int m, int n> static double trace(const Matx<_Tp, m, n>& a);
template<typename _Tp, int m, int n> static double norm(const Matx<_Tp, m, n>& M);
template<typename _Tp, int m, int n> static double norm(const Matx<_Tp, m, n>& M, int normType);



/////////////////////// Vec (used as element of multi-channel images /////////////////////

/** @brief Template class for short numerical vectors, a partial case of Matx

This template class represents short numerical vectors (of 1, 2, 3, 4 ... elements) on which you
can perform basic arithmetical operations, access individual elements using [] operator etc. The
vectors are allocated on stack, as opposite to std::valarray, std::vector, cv::Mat etc., which
elements are dynamically allocated in the heap.

The template takes 2 parameters:
@tparam _Tp element type
@tparam cn the number of elements

In addition to the universal notation like Vec<float, 3>, you can use shorter aliases
for the most popular specialized variants of Vec, e.g. Vec3f ~ Vec<float, 3>.

It is possible to convert Vec\<T,2\> to/from Point_, Vec\<T,3\> to/from Point3_ , and Vec\<T,4\>
to CvScalar or Scalar_. Use operator[] to access the elements of Vec.

All the expected vector operations are also implemented:
-   v1 = v2 + v3
-   v1 = v2 - v3
-   v1 = v2 \* scale
-   v1 = scale \* v2
-   v1 = -v2
-   v1 += v2 and other augmenting operations
-   v1 == v2, v1 != v2
-   norm(v1) (euclidean norm)
The Vec class is commonly used to describe pixel types of multi-channel arrays. See Mat for details.
*/
template<typename _Tp, int cn> class Vec : public Matx<_Tp, cn, 1>
{
public:
    typedef _Tp value_type;
    enum {
           channels = cn,
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           depth    = Matx<_Tp, cn, 1>::depth,
           type     = CV_MAKETYPE(depth, channels),
#endif
           _dummy_enum_finalizer = 0
         };

    //! default constructor
    Vec();

    Vec(_Tp v0); //!< 1-element vector constructor
    Vec(_Tp v0, _Tp v1); //!< 2-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2); //!< 3-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3); //!< 4-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4); //!< 5-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5); //!< 6-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6); //!< 7-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7); //!< 8-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8); //!< 9-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9); //!< 10-element vector constructor
    Vec(_Tp v0, _Tp v1, _Tp v2, _Tp v3, _Tp v4, _Tp v5, _Tp v6, _Tp v7, _Tp v8, _Tp v9, _Tp v10, _Tp v11, _Tp v12, _Tp v13); //!< 14-element vector constructor
    explicit Vec(const _Tp* values);

    //remove by lizulin
    //Vec(std::initializer_list<_Tp>);

    Vec(const Vec<_Tp, cn>& v);

    static Vec all(_Tp alpha);

    //! per-element multiplication
    Vec mul(const Vec<_Tp, cn>& v) const;

    //! conjugation (makes sense for complex numbers and quaternions)
    Vec conj() const;

    /*!
      cross product of the two 3D vectors.

      For other dimensionalities the exception is raised
    */
    Vec cross(const Vec& v) const;
    //! conversion to another data type
    template<typename T2> operator Vec<T2, cn>() const;

    /*! element access */
    const _Tp& operator [](int i) const;
    _Tp& operator[](int i);
    const _Tp& operator ()(int i) const;
    _Tp& operator ()(int i);

#ifdef CV_CXX11
    Vec<_Tp, cn>& operator=(const Vec<_Tp, cn>& rhs) = default;
#endif

    Vec(const Matx<_Tp, cn, 1>& a, const Matx<_Tp, cn, 1>& b, Matx_AddOp);
    Vec(const Matx<_Tp, cn, 1>& a, const Matx<_Tp, cn, 1>& b, Matx_SubOp);
    template<typename _T2> Vec(const Matx<_Tp, cn, 1>& a, _T2 alpha, Matx_ScaleOp);
};

/** @name Shorter aliases for the most popular specializations of Vec<T,n>
  @{
*/
typedef Vec<uchar, 2> Vec2b;
typedef Vec<uchar, 3> Vec3b;
typedef Vec<uchar, 4> Vec4b;

typedef Vec<short, 2> Vec2s;
typedef Vec<short, 3> Vec3s;
typedef Vec<short, 4> Vec4s;

typedef Vec<ushort, 2> Vec2w;
typedef Vec<ushort, 3> Vec3w;
typedef Vec<ushort, 4> Vec4w;

typedef Vec<int, 2> Vec2i;
typedef Vec<int, 3> Vec3i;
typedef Vec<int, 4> Vec4i;
typedef Vec<int, 6> Vec6i;
typedef Vec<int, 8> Vec8i;

typedef Vec<float, 2> Vec2f;
typedef Vec<float, 3> Vec3f;
typedef Vec<float, 4> Vec4f;
typedef Vec<float, 6> Vec6f;

typedef Vec<double, 2> Vec2d;
typedef Vec<double, 3> Vec3d;
typedef Vec<double, 4> Vec4d;
typedef Vec<double, 6> Vec6d;
/** @} */

/*!
  traits
*/
template<typename _Tp, int cn> class DataType< Vec<_Tp, cn> >
{
public:
    typedef Vec<_Tp, cn>                               value_type;
    typedef Vec<typename DataType<_Tp>::work_type, cn> work_type;
    typedef _Tp                                        channel_type;
    typedef value_type                                 vec_type;

    enum { generic_type = 0,
           channels     = cn,
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8),
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           depth        = DataType<channel_type>::depth,
           type         = CV_MAKETYPE(depth, channels),
#endif
           _dummy_enum_finalizer = 0
         };
};

namespace traits {
template<typename _Tp, int cn>
struct Depth< Vec<_Tp, cn> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp, int cn>
struct Type< Vec<_Tp, cn> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, cn) }; };
} // namespace


/** @brief  Comma-separated Vec Initializer
*/
template<typename _Tp, int m> class VecCommaInitializer : public MatxCommaInitializer<_Tp, m, 1>
{
public:
    VecCommaInitializer(Vec<_Tp, m>* _vec);
    template<typename T2> VecCommaInitializer<_Tp, m>& operator , (T2 val);
    Vec<_Tp, m> operator *() const;
};

template<typename _Tp, int cn> static Vec<_Tp, cn> normalize(const Vec<_Tp, cn>& v);

//! @} core_basic

//! @cond IGNORED

///////////////////////////////////// helper classes /////////////////////////////////////
namespace internal
{

template<typename _Tp, int m> struct Matx_DetOp
{
    double operator ()(const Matx<_Tp, m, m>& a) const
    {
        Matx<_Tp, m, m> temp = a;
        double p = LU(temp.val, m*sizeof(_Tp), m, 0, 0, 0);
        if( p == 0 )
            return p;
        for( int i = 0; i < m; i++ )
            p *= temp(i, i);
        return p;
    }
};

template<typename _Tp> struct Matx_DetOp<_Tp, 1>
{
    double operator ()(const Matx<_Tp, 1, 1>& a) const
    {
        return a(0,0);
    }
};

template<typename _Tp> struct Matx_DetOp<_Tp, 2>
{
    double operator ()(const Matx<_Tp, 2, 2>& a) const
    {
        return a(0,0)*a(1,1) - a(0,1)*a(1,0);
    }
};

template<typename _Tp> struct Matx_DetOp<_Tp, 3>
{
    double operator ()(const Matx<_Tp, 3, 3>& a) const
    {
        return a(0,0)*(a(1,1)*a(2,2) - a(2,1)*a(1,2)) -
            a(0,1)*(a(1,0)*a(2,2) - a(2,0)*a(1,2)) +
            a(0,2)*(a(1,0)*a(2,1) - a(2,0)*a(1,1));
    }
};

template<typename _Tp> Vec<_Tp, 2> inline conjugate(const Vec<_Tp, 2>& v)
{
    return Vec<_Tp, 2>(v[0], -v[1]);
}

template<typename _Tp> Vec<_Tp, 4> inline conjugate(const Vec<_Tp, 4>& v)
{
    return Vec<_Tp, 4>(v[0], -v[1], -v[2], -v[3]);
}

} // internal


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//types.hpp
#include <vector>



//! @addtogroup core_basic
//! @{

//////////////////////////////// Complex //////////////////////////////

/** @brief  A complex number class.

  The template class is similar and compatible with std::complex, however it provides slightly
  more convenient access to the real and imaginary parts using through the simple field access, as opposite
  to std::complex::real() and std::complex::imag().
*/
template<typename _Tp> class Complex
{
public:

    //! default constructor
    Complex();
    Complex( _Tp _re, _Tp _im = 0 );

    //! conversion to another data type
    template<typename T2> operator Complex<T2>() const;
    //! conjugation
    Complex conj() const;

    _Tp re, im; //< the real and the imaginary parts
};

typedef Complex<float> Complexf;
typedef Complex<double> Complexd;

template<typename _Tp> class DataType< Complex<_Tp> >
{
public:
    typedef Complex<_Tp> value_type;
    typedef value_type   work_type;
    typedef _Tp          channel_type;

    enum { generic_type = 0,
           channels     = 2,
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
    };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Complex<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Complex<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 2) }; };
} // namespace


//////////////////////////////// Point_ ////////////////////////////////

/** @brief Template class for 2D points specified by its coordinates `x` and `y`.

An instance of the class is interchangeable with C structures, CvPoint and CvPoint2D32f . There is
also a cast operator to convert point coordinates to the specified type. The conversion from
floating-point coordinates to integer coordinates is done by rounding. Commonly, the conversion
uses this operation for each of the coordinates. Besides the class members listed in the
declaration above, the following operations on points are implemented:
@code
    pt1 = pt2 + pt3;
    pt1 = pt2 - pt3;
    pt1 = pt2 * a;
    pt1 = a * pt2;
    pt1 = pt2 / a;
    pt1 += pt2;
    pt1 -= pt2;
    pt1 *= a;
    pt1 /= a;
    double value = norm(pt); // L2 norm
    pt1 == pt2;
    pt1 != pt2;
@endcode
For your convenience, the following type aliases are defined:
@code
    typedef Point_<int> Point2i;
    typedef Point2i Point;
    typedef Point_<float> Point2f;
    typedef Point_<double> Point2d;
@endcode
Example:
@code
    Point2f a(0.3f, 0.f), b(0.f, 0.4f);
    Point pt = (a + b)*10.f;
    cout << pt.x << ", " << pt.y << endl;
@endcode
*/
template<typename _Tp> class Point_
{
public:
    typedef _Tp value_type;

    //! default constructor
    Point_();
    Point_(_Tp _x, _Tp _y);
    Point_(const Point_& pt);
    Point_(Point_&& pt) ;
    Point_(const Size_<_Tp>& sz);
    Point_(const Vec<_Tp, 2>& v);

    Point_& operator = (const Point_& pt);
    Point_& operator = (Point_&& pt) ;
    //! conversion to another data type
    template<typename _Tp2> operator Point_<_Tp2>() const;

    //! conversion to the old-style C structures
    operator Vec<_Tp, 2>() const;

    //! dot product
    _Tp dot(const Point_& pt) const;
    //! dot product computed in double-precision arithmetics
    double ddot(const Point_& pt) const;
    //! cross-product
    double cross(const Point_& pt) const;
    //! checks whether the point is inside the specified rectangle
    bool inside(const Rect_<_Tp>& r) const;
    _Tp x; //!< x coordinate of the point
    _Tp y; //!< y coordinate of the point
};

typedef Point_<int> Point2i;
typedef Point_<int64> Point2l;
typedef Point_<float> Point2f;
typedef Point_<double> Point2d;
typedef Point2i Point;

template<typename _Tp> class DataType< Point_<_Tp> >
{
public:
    typedef Point_<_Tp>                               value_type;
    typedef Point_<typename DataType<_Tp>::work_type> work_type;
    typedef _Tp                                       channel_type;

    enum { generic_type = 0,
           channels     = 2,
           fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Point_<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Point_<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 2) }; };
} // namespace


//////////////////////////////// Point3_ ////////////////////////////////

/** @brief Template class for 3D points specified by its coordinates `x`, `y` and `z`.

An instance of the class is interchangeable with the C structure CvPoint2D32f . Similarly to
Point_ , the coordinates of 3D points can be converted to another type. The vector arithmetic and
comparison operations are also supported.

The following Point3_\<\> aliases are available:
@code
    typedef Point3_<int> Point3i;
    typedef Point3_<float> Point3f;
    typedef Point3_<double> Point3d;
@endcode
@see cv::Point3i, cv::Point3f and cv::Point3d
*/
template<typename _Tp> class Point3_
{
public:
    typedef _Tp value_type;

    //! default constructor
    Point3_();
    Point3_(_Tp _x, _Tp _y, _Tp _z);
    Point3_(const Point3_& pt);
    Point3_(Point3_&& pt) ;
    explicit Point3_(const Point_<_Tp>& pt);
    Point3_(const Vec<_Tp, 3>& v);

    Point3_& operator = (const Point3_& pt);
    Point3_& operator = (Point3_&& pt) ;
    //! conversion to another data type
    template<typename _Tp2> operator Point3_<_Tp2>() const;
    //! conversion to cv::Vec<>
    operator Vec<_Tp, 3>() const;

    //! dot product
    _Tp dot(const Point3_& pt) const;
    //! dot product computed in double-precision arithmetics
    double ddot(const Point3_& pt) const;
    //! cross product of the 2 3D points
    Point3_ cross(const Point3_& pt) const;
    _Tp x; //!< x coordinate of the 3D point
    _Tp y; //!< y coordinate of the 3D point
    _Tp z; //!< z coordinate of the 3D point
};

typedef Point3_<int> Point3i;
typedef Point3_<float> Point3f;
typedef Point3_<double> Point3d;

template<typename _Tp> class DataType< Point3_<_Tp> >
{
public:
    typedef Point3_<_Tp>                               value_type;
    typedef Point3_<typename DataType<_Tp>::work_type> work_type;
    typedef _Tp                                        channel_type;

    enum { generic_type = 0,
           channels     = 3,
           fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Point3_<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Point3_<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 3) }; };
} // namespace

//////////////////////////////// Size_ ////////////////////////////////

/** @brief Template class for specifying the size of an image or rectangle.

The class includes two members called width and height. The structure can be converted to and from
the old OpenCV structures CvSize and CvSize2D32f . The same set of arithmetic and comparison
operations as for Point_ is available.

OpenCV defines the following Size_\<\> aliases:
@code
    typedef Size_<int> Size2i;
    typedef Size2i Size;
    typedef Size_<float> Size2f;
@endcode
*/
template<typename _Tp> class Size_
{
public:
    typedef _Tp value_type;

    //! default constructor
    Size_();
    Size_(_Tp _width, _Tp _height);
    Size_(const Size_& sz);
    Size_(Size_&& sz) ;
    Size_(const Point_<_Tp>& pt);

    Size_& operator = (const Size_& sz);
    Size_& operator = (Size_&& sz) ;
    //! the area (width*height)
    _Tp area() const;
    //! aspect ratio (width/height)
    double aspectRatio() const;
    //! true if empty
    bool empty() const;

    //! conversion of another data type.
    template<typename _Tp2> operator Size_<_Tp2>() const;

    _Tp width; //!< the width
    _Tp height; //!< the height
};

typedef Size_<int> Size2i;
typedef Size_<int64> Size2l;
typedef Size_<float> Size2f;
typedef Size_<double> Size2d;
typedef Size2i Size;

template<typename _Tp> class DataType< Size_<_Tp> >
{
public:
    typedef Size_<_Tp>                               value_type;
    typedef Size_<typename DataType<_Tp>::work_type> work_type;
    typedef _Tp                                      channel_type;

    enum { generic_type = 0,
           channels     = 2,
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Size_<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Size_<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 2) }; };
} // namespace

//////////////////////////////// Rect_ ////////////////////////////////

/** @brief Template class for 2D rectangles

described by the following parameters:
-   Coordinates of the top-left corner. This is a default interpretation of Rect_::x and Rect_::y
    in OpenCV. Though, in your algorithms you may count x and y from the bottom-left corner.
-   Rectangle width and height.

OpenCV typically assumes that the top and left boundary of the rectangle are inclusive, while the
right and bottom boundaries are not. For example, the method Rect_::contains returns true if

\f[x  \leq pt.x < x+width,
      y  \leq pt.y < y+height\f]

Virtually every loop over an image ROI in OpenCV (where ROI is specified by Rect_\<int\> ) is
implemented as:
@code
    for(int y = roi.y; y < roi.y + roi.height; y++)
        for(int x = roi.x; x < roi.x + roi.width; x++)
        {
            // ...
        }
@endcode
In addition to the class members, the following operations on rectangles are implemented:
-   \f$\texttt{rect} = \texttt{rect} \pm \texttt{point}\f$ (shifting a rectangle by a certain offset)
-   \f$\texttt{rect} = \texttt{rect} \pm \texttt{size}\f$ (expanding or shrinking a rectangle by a
    certain amount)
-   rect += point, rect -= point, rect += size, rect -= size (augmenting operations)
-   rect = rect1 & rect2 (rectangle intersection)
-   rect = rect1 | rect2 (minimum area rectangle containing rect1 and rect2 )
-   rect &= rect1, rect |= rect1 (and the corresponding augmenting operations)
-   rect == rect1, rect != rect1 (rectangle comparison)

This is an example how the partial ordering on rectangles can be established (rect1 \f$\subseteq\f$
rect2):
@code
    template<typename _Tp> inline bool
    operator <= (const Rect_<_Tp>& r1, const Rect_<_Tp>& r2)
    {
        return (r1 & r2) == r1;
    }
@endcode
For your convenience, the Rect_\<\> alias is available: cv::Rect
*/
template<typename _Tp> class Rect_
{
public:
    typedef _Tp value_type;

    //! default constructor
    Rect_();
    Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
    Rect_(const Rect_& r);
    Rect_(Rect_&& r) ;
    Rect_(const Point_<_Tp>& org, const Size_<_Tp>& sz);
    Rect_(const Point_<_Tp>& pt1, const Point_<_Tp>& pt2);

    Rect_& operator = ( const Rect_& r );
    Rect_& operator = ( Rect_&& r ) ;
    //! the top-left corner
    Point_<_Tp> tl() const;
    //! the bottom-right corner
    Point_<_Tp> br() const;

    //! size (width, height) of the rectangle
    Size_<_Tp> size() const;
    //! area (width*height) of the rectangle
    _Tp area() const;
    //! true if empty
    bool empty() const;

    //! conversion to another data type
    template<typename _Tp2> operator Rect_<_Tp2>() const;

    //! checks whether the rectangle contains the point
    bool contains(const Point_<_Tp>& pt) const;

    _Tp x; //!< x coordinate of the top-left corner
    _Tp y; //!< y coordinate of the top-left corner
    _Tp width; //!< width of the rectangle
    _Tp height; //!< height of the rectangle
};

typedef Rect_<int> Rect2i;
typedef Rect_<float> Rect2f;
typedef Rect_<double> Rect2d;
typedef Rect2i Rect;

template<typename _Tp> class DataType< Rect_<_Tp> >
{
public:
    typedef Rect_<_Tp>                               value_type;
    typedef Rect_<typename DataType<_Tp>::work_type> work_type;
    typedef _Tp                                      channel_type;

    enum { generic_type = 0,
           channels     = 4,
           fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Rect_<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Rect_<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 4) }; };
} // namespace

///////////////////////////// RotatedRect /////////////////////////////

/** @brief The class represents rotated (i.e. not up-right) rectangles on a plane.

Each rectangle is specified by the center point (mass center), length of each side (represented by
#Size2f structure) and the rotation angle in degrees.

The sample below demonstrates how to use RotatedRect:
@snippet snippets/core_various.cpp RotatedRect_demo
![image](pics/rotatedrect.png)

@sa CamShift, fitEllipse, minAreaRect, CvBox2D
*/
class  RotatedRect
{
public:
    //! default constructor
    RotatedRect();
    /** full constructor
    @param center The rectangle mass center.
    @param size Width and height of the rectangle.
    @param angle The rotation angle in a clockwise direction. When the angle is 0, 90, 180, 270 etc.,
    the rectangle becomes an up-right rectangle.
    */
    RotatedRect(const Point2f& center, const Size2f& size, float angle);
    /**
    Any 3 end points of the RotatedRect. They must be given in order (either clockwise or
    anticlockwise).
     */
    RotatedRect(const Point2f& point1, const Point2f& point2, const Point2f& point3);

    /** returns 4 vertices of the rectangle
    @param pts The points array for storing rectangle vertices. The order is bottomLeft, topLeft, topRight, bottomRight.
    */
    void points(Point2f pts[]) const;
    //! returns the minimal up-right integer rectangle containing the rotated rectangle
    Rect boundingRect() const;
    //! returns the minimal (exact) floating point rectangle containing the rotated rectangle, not intended for use with images
    Rect_<float> boundingRect2f() const;
    //! returns the rectangle mass center
    Point2f center;
    //! returns width and height of the rectangle
    Size2f size;
    //! returns the rotation angle. When the angle is 0, 90, 180, 270 etc., the rectangle becomes an up-right rectangle.
    float angle;
};

template<> class DataType< RotatedRect >
{
public:
    typedef RotatedRect  value_type;
    typedef value_type   work_type;
    typedef float        channel_type;

    enum { generic_type = 0,
           channels     = (int)sizeof(value_type)/sizeof(channel_type), // 5
           fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<>
struct Depth< RotatedRect > { enum { value = Depth<float>::value }; };
template<>
struct Type< RotatedRect > { enum { value = CV_MAKETYPE(Depth<float>::value, (int)sizeof(RotatedRect)/sizeof(float)) }; };
} // namespace


//////////////////////////////// Range /////////////////////////////////

/** @brief Template class specifying a continuous subsequence (slice) of a sequence.

The class is used to specify a row or a column span in a matrix ( Mat ) and for many other purposes.
Range(a,b) is basically the same as a:b in Matlab or a..b in Python. As in Python, start is an
inclusive left boundary of the range and end is an exclusive right boundary of the range. Such a
half-opened interval is usually denoted as \f$[start,end)\f$ .

The static method Range::all() returns a special variable that means "the whole sequence" or "the
whole range", just like " : " in Matlab or " ... " in Python. All the methods and functions in
OpenCV that take Range support this special Range::all() value. But, of course, in case of your own
custom processing, you will probably have to check and handle it explicitly:
@code
    void my_function(..., const Range& r, ....)
    {
        if(r == Range::all()) {
            // process all the data
        }
        else {
            // process [r.start, r.end)
        }
    }
@endcode
*/
class  Range
{
public:
    Range();
    Range(int _start, int _end);
    int size() const;
    bool empty() const;
    static Range all();

    int start, end;
};

template<> class DataType<Range>
{
public:
    typedef Range      value_type;
    typedef value_type work_type;
    typedef int        channel_type;

    enum { generic_type = 0,
           channels     = 2,
           fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<>
struct Depth< Range > { enum { value = Depth<int>::value }; };
template<>
struct Type< Range > { enum { value = CV_MAKETYPE(Depth<int>::value, 2) }; };
} // namespace


//////////////////////////////// Scalar_ ///////////////////////////////

/** @brief Template class for a 4-element vector derived from Vec.

Being derived from Vec\<_Tp, 4\> , Scalar\_ and Scalar can be used just as typical 4-element
vectors. In addition, they can be converted to/from CvScalar . The type Scalar is widely used in
OpenCV to pass pixel values.
*/
template<typename _Tp> class Scalar_ : public Vec<_Tp, 4>
{
public:
    //! default constructor
    Scalar_();
    Scalar_(_Tp v0, _Tp v1, _Tp v2=0, _Tp v3=0);
    Scalar_(_Tp v0);

    Scalar_(const Scalar_& s);
    Scalar_(Scalar_&& s) ;

    Scalar_& operator=(const Scalar_& s);
    Scalar_& operator=(Scalar_&& s) ;

    template<typename _Tp2, int cn>
    Scalar_(const Vec<_Tp2, cn>& v);

    //! returns a scalar with all elements set to v0
    static Scalar_<_Tp> all(_Tp v0);

    //! conversion to another data type
    template<typename T2> operator Scalar_<T2>() const;

    //! per-element product
    Scalar_<_Tp> mul(const Scalar_<_Tp>& a, double scale=1 ) const;

    //! returns (v0, -v1, -v2, -v3)
    Scalar_<_Tp> conj() const;

    //! returns true iff v1 == v2 == v3 == 0
    bool isReal() const;
};

typedef Scalar_<double> Scalar;

template<typename _Tp> class DataType< Scalar_<_Tp> >
{
public:
    typedef Scalar_<_Tp>                               value_type;
    typedef Scalar_<typename DataType<_Tp>::work_type> work_type;
    typedef _Tp                                        channel_type;

    enum { generic_type = 0,
           channels     = 4,
           fmt          = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<typename _Tp>
struct Depth< Scalar_<_Tp> > { enum { value = Depth<_Tp>::value }; };
template<typename _Tp>
struct Type< Scalar_<_Tp> > { enum { value = CV_MAKETYPE(Depth<_Tp>::value, 4) }; };
} // namespace


/////////////////////////////// KeyPoint ////////////////////////////////

/** @brief Data structure for salient point detectors.

The class instance stores a keypoint, i.e. a point feature found by one of many available keypoint
detectors, such as Harris corner detector, #FAST, %StarDetector, %SURF, %SIFT etc.

The keypoint is characterized by the 2D position, scale (proportional to the diameter of the
neighborhood that needs to be taken into account), orientation and some other parameters. The
keypoint neighborhood is then analyzed by another algorithm that builds a descriptor (usually
represented as a feature vector). The keypoints representing the same object in different images
can then be matched using %KDTree or another method.
*/
class  KeyPoint
{
public:
    //! the default constructor
     KeyPoint();
    /**
    @param _pt x & y coordinates of the keypoint
    @param _size keypoint diameter
    @param _angle keypoint orientation
    @param _response keypoint detector response on the keypoint (that is, strength of the keypoint)
    @param _octave pyramid octave in which the keypoint has been detected
    @param _class_id object id
     */
    KeyPoint(Point2f _pt, float _size, float _angle=-1, float _response=0, int _octave=0, int _class_id=-1);
    /**
    @param x x-coordinate of the keypoint
    @param y y-coordinate of the keypoint
    @param _size keypoint diameter
    @param _angle keypoint orientation
    @param _response keypoint detector response on the keypoint (that is, strength of the keypoint)
    @param _octave pyramid octave in which the keypoint has been detected
    @param _class_id object id
     */
     KeyPoint(float x, float y, float _size, float _angle=-1, float _response=0, int _octave=0, int _class_id=-1);

    size_t hash() const;

    /**
    This method converts vector of keypoints to vector of points or the reverse, where each keypoint is
    assigned the same size and the same orientation.

    @param keypoints Keypoints obtained from any feature detection algorithm like SIFT/SURF/ORB
    @param points2f Array of (x,y) coordinates of each keypoint
    @param keypointIndexes Array of indexes of keypoints to be converted to points. (Acts like a mask to
    convert only specified keypoints)
    */
     static void convert(const std::vector<KeyPoint>& keypoints,
                                 std::vector<Point2f>& points2f,
                                const std::vector<int>& keypointIndexes=std::vector<int>());
    /** @overload
    @param points2f Array of (x,y) coordinates of each keypoint
    @param keypoints Keypoints obtained from any feature detection algorithm like SIFT/SURF/ORB
    @param size keypoint diameter
    @param response keypoint detector response on the keypoint (that is, strength of the keypoint)
    @param octave pyramid octave in which the keypoint has been detected
    @param class_id object id
    */
     static void convert(const std::vector<Point2f>& points2f,
                                 std::vector<KeyPoint>& keypoints,
                                float size=1, float response=1, int octave=0, int class_id=-1);

    /**
    This method computes overlap for pair of keypoints. Overlap is the ratio between area of keypoint
    regions' intersection and area of keypoint regions' union (considering keypoint region as circle).
    If they don't overlap, we get zero. If they coincide at same location with same size, we get 1.
    @param kp1 First keypoint
    @param kp2 Second keypoint
    */
     static float overlap(const KeyPoint& kp1, const KeyPoint& kp2);

     Point2f pt; //!< coordinates of the keypoints
     float size; //!< diameter of the meaningful keypoint neighborhood
     float angle; //!< computed orientation of the keypoint (-1 if not applicable);
                            //!< it's in [0,360) degrees and measured relative to
                            //!< image coordinate system, ie in clockwise.
     float response; //!< the response by which the most strong keypoints have been selected. Can be used for the further sorting or subsampling
     int octave; //!< octave (pyramid layer) from which the keypoint has been extracted
     int class_id; //!< object class (if the keypoints need to be clustered by an object they belong to)
};

#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
template<> class DataType<KeyPoint>
{
public:
    typedef KeyPoint      value_type;
    typedef float         work_type;
    typedef float         channel_type;

    enum { generic_type = 0,
           depth        = DataType<channel_type>::depth,
           channels     = (int)(sizeof(value_type)/sizeof(channel_type)), // 7
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8),
           type         = CV_MAKETYPE(depth, channels)
         };

    typedef Vec<channel_type, channels> vec_type;
};
#endif


//////////////////////////////// DMatch /////////////////////////////////

/** @brief Class for matching keypoint descriptors

query descriptor index, train descriptor index, train image index, and distance between
descriptors.
*/
class  DMatch
{
public:
     DMatch();
     DMatch(int _queryIdx, int _trainIdx, float _distance);
     DMatch(int _queryIdx, int _trainIdx, int _imgIdx, float _distance);

     int queryIdx; //!< query descriptor index
     int trainIdx; //!< train descriptor index
     int imgIdx;   //!< train image index

     float distance;

    // less is better
    bool operator<(const DMatch &m) const;
};

#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
template<> class DataType<DMatch>
{
public:
    typedef DMatch      value_type;
    typedef int         work_type;
    typedef int         channel_type;

    enum { generic_type = 0,
           depth        = DataType<channel_type>::depth,
           channels     = (int)(sizeof(value_type)/sizeof(channel_type)), // 4
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8),
           type         = CV_MAKETYPE(depth, channels)
         };

    typedef Vec<channel_type, channels> vec_type;
};
#endif


///////////////////////////// TermCriteria //////////////////////////////

/** @brief The class defining termination criteria for iterative algorithms.

You can initialize it by default constructor and then override any parameters, or the structure may
be fully initialized using the advanced variant of the constructor.
*/
class  TermCriteria
{
public:
    /**
      Criteria type, can be one of: COUNT, EPS or COUNT + EPS
    */
    enum Type
    {
        COUNT=1, //!< the maximum number of iterations or elements to compute
        MAX_ITER=COUNT, //!< ditto
        EPS=2 //!< the desired accuracy or change in parameters at which the iterative algorithm stops
    };

    //! default constructor
    TermCriteria();
    /**
    @param type The type of termination criteria, one of TermCriteria::Type
    @param maxCount The maximum number of iterations or elements to compute.
    @param epsilon The desired accuracy or change in parameters at which the iterative algorithm stops.
    */
    TermCriteria(int type, int maxCount, double epsilon);

    inline bool isValid() const
    {
        const bool isCount = (type & COUNT) && maxCount > 0;
        const bool isEps = (type & EPS) && !IsNaN(epsilon);
        return isCount || isEps;
    }

    int type; //!< the type of termination criteria: COUNT, EPS or COUNT + EPS
    int maxCount; //!< the maximum number of iterations/elements
    double epsilon; //!< the desired accuracy
};


//! @} core_basic

///////////////////////// raster image moments //////////////////////////

//! @addtogroup imgproc_shape
//! @{

/** @brief struct returned by cv::moments

The spatial moments \f$\texttt{Moments::m}_{ji}\f$ are computed as:

\f[\texttt{m} _{ji}= \sum _{x,y}  \left ( \texttt{array} (x,y)  \cdot x^j  \cdot y^i \right )\f]

The central moments \f$\texttt{Moments::mu}_{ji}\f$ are computed as:

\f[\texttt{mu} _{ji}= \sum _{x,y}  \left ( \texttt{array} (x,y)  \cdot (x -  \bar{x} )^j  \cdot (y -  \bar{y} )^i \right )\f]

where \f$(\bar{x}, \bar{y})\f$ is the mass center:

\f[\bar{x} = \frac{\texttt{m}_{10}}{\texttt{m}_{00}} , \; \bar{y} = \frac{\texttt{m}_{01}}{\texttt{m}_{00}}\f]

The normalized central moments \f$\texttt{Moments::nu}_{ij}\f$ are computed as:

\f[\texttt{nu} _{ji}= \frac{\texttt{mu}_{ji}}{\texttt{m}_{00}^{(i+j)/2+1}} .\f]

@note
\f$\texttt{mu}_{00}=\texttt{m}_{00}\f$, \f$\texttt{nu}_{00}=1\f$
\f$\texttt{nu}_{10}=\texttt{mu}_{10}=\texttt{mu}_{01}=\texttt{mu}_{10}=0\f$ , hence the values are not
stored.

The moments of a contour are defined in the same way but computed using the Green's formula (see
<http://en.wikipedia.org/wiki/Green_theorem>). So, due to a limited raster resolution, the moments
computed for a contour are slightly different from the moments computed for the same rasterized
contour.

@note
Since the contour moments are computed using Green formula, you may get seemingly odd results for
contours with self-intersections, e.g. a zero area (m00) for butterfly-shaped contours.
 */
class  Moments
{
public:
    //! the default constructor
    Moments();
    //! the full constructor
    Moments(double m00, double m10, double m01, double m20, double m11,
            double m02, double m30, double m21, double m12, double m03 );
    ////! the conversion from CvMoments
    //Moments( const CvMoments& moments );
    ////! the conversion to CvMoments
    //operator CvMoments() const;

    //! @name spatial moments
    //! @{
     double  m00, m10, m01, m20, m11, m02, m30, m21, m12, m03;
    //! @}

    //! @name central moments
    //! @{
     double  mu20, mu11, mu02, mu30, mu21, mu12, mu03;
    //! @}

    //! @name central normalized moments
    //! @{
     double  nu20, nu11, nu02, nu30, nu21, nu12, nu03;
    //! @}
};

template<> class DataType<Moments>
{
public:
    typedef Moments     value_type;
    typedef double      work_type;
    typedef double      channel_type;

    enum { generic_type = 0,
           channels     = (int)(sizeof(value_type)/sizeof(channel_type)), // 24
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           ,depth        = DataType<channel_type>::depth
           ,type         = CV_MAKETYPE(depth, channels)
#endif
         };

    typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<>
struct Depth< Moments > { enum { value = Depth<double>::value }; };
template<>
struct Type< Moments > { enum { value = CV_MAKETYPE(Depth<double>::value, (int)(sizeof(Moments)/sizeof(double))) }; };
} // namespace

//! @} imgproc_shape

//! @cond IGNORED

/////////////////////////////////////////////////////////////////////////
///////////////////////////// Implementation ////////////////////////////
/////////////////////////////////////////////////////////////////////////

//////////////////////////////// Complex ////////////////////////////////

template<typename _Tp> inline
Complex<_Tp>::Complex()
    : re(0), im(0) {}

template<typename _Tp> inline
Complex<_Tp>::Complex( _Tp _re, _Tp _im )
    : re(_re), im(_im) {}

template<typename _Tp> template<typename T2> inline
Complex<_Tp>::operator Complex<T2>() const
{
    return Complex<T2>(saturate_cast<T2>(re), saturate_cast<T2>(im));
}

template<typename _Tp> inline
Complex<_Tp> Complex<_Tp>::conj() const
{
    return Complex<_Tp>(re, -im);
}


template<typename _Tp> static inline
bool operator == (const Complex<_Tp>& a, const Complex<_Tp>& b)
{
    return a.re == b.re && a.im == b.im;
}

template<typename _Tp> static inline
bool operator != (const Complex<_Tp>& a, const Complex<_Tp>& b)
{
    return a.re != b.re || a.im != b.im;
}

template<typename _Tp> static inline
Complex<_Tp> operator + (const Complex<_Tp>& a, const Complex<_Tp>& b)
{
    return Complex<_Tp>( a.re + b.re, a.im + b.im );
}

template<typename _Tp> static inline
Complex<_Tp>& operator += (Complex<_Tp>& a, const Complex<_Tp>& b)
{
    a.re += b.re; a.im += b.im;
    return a;
}

template<typename _Tp> static inline
Complex<_Tp> operator - (const Complex<_Tp>& a, const Complex<_Tp>& b)
{
    return Complex<_Tp>( a.re - b.re, a.im - b.im );
}

template<typename _Tp> static inline
Complex<_Tp>& operator -= (Complex<_Tp>& a, const Complex<_Tp>& b)
{
    a.re -= b.re; a.im -= b.im;
    return a;
}

template<typename _Tp> static inline
Complex<_Tp> operator - (const Complex<_Tp>& a)
{
    return Complex<_Tp>(-a.re, -a.im);
}

template<typename _Tp> static inline
Complex<_Tp> operator * (const Complex<_Tp>& a, const Complex<_Tp>& b)
{
    return Complex<_Tp>( a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re );
}

template<typename _Tp> static inline
Complex<_Tp> operator * (const Complex<_Tp>& a, _Tp b)
{
    return Complex<_Tp>( a.re*b, a.im*b );
}

template<typename _Tp> static inline
Complex<_Tp> operator * (_Tp b, const Complex<_Tp>& a)
{
    return Complex<_Tp>( a.re*b, a.im*b );
}

template<typename _Tp> static inline
Complex<_Tp> operator + (const Complex<_Tp>& a, _Tp b)
{
    return Complex<_Tp>( a.re + b, a.im );
}

template<typename _Tp> static inline
Complex<_Tp> operator - (const Complex<_Tp>& a, _Tp b)
{ return Complex<_Tp>( a.re - b, a.im ); }

template<typename _Tp> static inline
Complex<_Tp> operator + (_Tp b, const Complex<_Tp>& a)
{
    return Complex<_Tp>( a.re + b, a.im );
}

template<typename _Tp> static inline
Complex<_Tp> operator - (_Tp b, const Complex<_Tp>& a)
{
    return Complex<_Tp>( b - a.re, -a.im );
}

template<typename _Tp> static inline
Complex<_Tp>& operator += (Complex<_Tp>& a, _Tp b)
{
    a.re += b; return a;
}

template<typename _Tp> static inline
Complex<_Tp>& operator -= (Complex<_Tp>& a, _Tp b)
{
    a.re -= b; return a;
}

template<typename _Tp> static inline
Complex<_Tp>& operator *= (Complex<_Tp>& a, _Tp b)
{
    a.re *= b; a.im *= b; return a;
}

template<typename _Tp> static inline
double abs(const Complex<_Tp>& a)
{
    return std::sqrt( (double)a.re*a.re + (double)a.im*a.im);
}

template<typename _Tp> static inline
Complex<_Tp> operator / (const Complex<_Tp>& a, const Complex<_Tp>& b)
{
    double t = 1./((double)b.re*b.re + (double)b.im*b.im);
    return Complex<_Tp>( (_Tp)((a.re*b.re + a.im*b.im)*t),
                        (_Tp)((-a.re*b.im + a.im*b.re)*t) );
}

template<typename _Tp> static inline
Complex<_Tp>& operator /= (Complex<_Tp>& a, const Complex<_Tp>& b)
{
    a = a / b;
    return a;
}

template<typename _Tp> static inline
Complex<_Tp> operator / (const Complex<_Tp>& a, _Tp b)
{
    _Tp t = (_Tp)1/b;
    return Complex<_Tp>( a.re*t, a.im*t );
}

template<typename _Tp> static inline
Complex<_Tp> operator / (_Tp b, const Complex<_Tp>& a)
{
    return Complex<_Tp>(b)/a;
}

template<typename _Tp> static inline
Complex<_Tp> operator /= (const Complex<_Tp>& a, _Tp b)
{
    _Tp t = (_Tp)1/b;
    a.re *= t; a.im *= t; return a;
}



//////////////////////////////// 2D Point ///////////////////////////////

template<typename _Tp> inline
Point_<_Tp>::Point_()
    : x(0), y(0) {}

template<typename _Tp> inline
Point_<_Tp>::Point_(_Tp _x, _Tp _y)
    : x(_x), y(_y) {}

template<typename _Tp> inline
Point_<_Tp>::Point_(const Point_& pt)
    : x(pt.x), y(pt.y) {}

template<typename _Tp> inline
Point_<_Tp>::Point_(Point_&& pt) 
    : x(std::move(pt.x)), y(std::move(pt.y)) {}

template<typename _Tp> inline
Point_<_Tp>::Point_(const Size_<_Tp>& sz)
    : x(sz.width), y(sz.height) {}

template<typename _Tp> inline
Point_<_Tp>::Point_(const Vec<_Tp,2>& v)
    : x(v[0]), y(v[1]) {}

template<typename _Tp> inline
Point_<_Tp>& Point_<_Tp>::operator = (const Point_& pt)
{
    x = pt.x; y = pt.y;
    return *this;
}

template<typename _Tp> inline
Point_<_Tp>& Point_<_Tp>::operator = (Point_&& pt) 
{
    x = std::move(pt.x); y = std::move(pt.y);
    return *this;
}

template<typename _Tp> template<typename _Tp2> inline
Point_<_Tp>::operator Point_<_Tp2>() const
{
    return Point_<_Tp2>(saturate_cast<_Tp2>(x), saturate_cast<_Tp2>(y));
}

template<typename _Tp> inline
Point_<_Tp>::operator Vec<_Tp, 2>() const
{
    return Vec<_Tp, 2>(x, y);
}


//remove by lizulin
template<typename _Tp> inline
_Tp Point_<_Tp>::dot(const Point_& pt) const
{
    return saturate_cast<_Tp>(x*pt.x + y*pt.y);
}

template<typename _Tp> inline
double Point_<_Tp>::ddot(const Point_& pt) const
{
    return (double)x*pt.x + (double)y*pt.y;
}

template<typename _Tp> inline
double Point_<_Tp>::cross(const Point_& pt) const
{
    return (double)x*pt.y - (double)y*pt.x;
}

template<typename _Tp> inline bool
Point_<_Tp>::inside( const Rect_<_Tp>& r ) const
{
    return r.contains(*this);
}


template<typename _Tp> static inline
Point_<_Tp>& operator += (Point_<_Tp>& a, const Point_<_Tp>& b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

template<typename _Tp> static inline
Point_<_Tp>& operator -= (Point_<_Tp>& a, const Point_<_Tp>& b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

template<typename _Tp> static inline
Point_<_Tp>& operator *= (Point_<_Tp>& a, int b)
{
    a.x = saturate_cast<_Tp>(a.x * b);
    a.y = saturate_cast<_Tp>(a.y * b);
    return a;
}

template<typename _Tp> static inline
Point_<_Tp>& operator *= (Point_<_Tp>& a, float b)
{
    a.x = saturate_cast<_Tp>(a.x * b);
    a.y = saturate_cast<_Tp>(a.y * b);
    return a;
}

template<typename _Tp> static inline
Point_<_Tp>& operator *= (Point_<_Tp>& a, double b)
{
    a.x = saturate_cast<_Tp>(a.x * b);
    a.y = saturate_cast<_Tp>(a.y * b);
    return a;
}

template<typename _Tp> static inline
Point_<_Tp>& operator /= (Point_<_Tp>& a, int b)
{
    a.x = saturate_cast<_Tp>(a.x / b);
    a.y = saturate_cast<_Tp>(a.y / b);
    return a;
}

template<typename _Tp> static inline
Point_<_Tp>& operator /= (Point_<_Tp>& a, float b)
{
    a.x = saturate_cast<_Tp>(a.x / b);
    a.y = saturate_cast<_Tp>(a.y / b);
    return a;
}

template<typename _Tp> static inline
Point_<_Tp>& operator /= (Point_<_Tp>& a, double b)
{
    a.x = saturate_cast<_Tp>(a.x / b);
    a.y = saturate_cast<_Tp>(a.y / b);
    return a;
}

template<typename _Tp> static inline
double norm(const Point_<_Tp>& pt)
{
    return std::sqrt((double)pt.x*pt.x + (double)pt.y*pt.y);
}

template<typename _Tp> static inline
bool operator == (const Point_<_Tp>& a, const Point_<_Tp>& b)
{
    return a.x == b.x && a.y == b.y;
}

template<typename _Tp> static inline
bool operator != (const Point_<_Tp>& a, const Point_<_Tp>& b)
{
    return a.x != b.x || a.y != b.y;
}

template<typename _Tp> static inline
Point_<_Tp> operator + (const Point_<_Tp>& a, const Point_<_Tp>& b)
{
    return Point_<_Tp>( saturate_cast<_Tp>(a.x + b.x), saturate_cast<_Tp>(a.y + b.y) );
}

template<typename _Tp> static inline
Point_<_Tp> operator - (const Point_<_Tp>& a, const Point_<_Tp>& b)
{
    return Point_<_Tp>( saturate_cast<_Tp>(a.x - b.x), saturate_cast<_Tp>(a.y - b.y) );
}

template<typename _Tp> static inline
Point_<_Tp> operator - (const Point_<_Tp>& a)
{
    return Point_<_Tp>( saturate_cast<_Tp>(-a.x), saturate_cast<_Tp>(-a.y) );
}

template<typename _Tp> static inline
Point_<_Tp> operator * (const Point_<_Tp>& a, int b)
{
    return Point_<_Tp>( saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b) );
}

template<typename _Tp> static inline
Point_<_Tp> operator * (int a, const Point_<_Tp>& b)
{
    return Point_<_Tp>( saturate_cast<_Tp>(b.x*a), saturate_cast<_Tp>(b.y*a) );
}

template<typename _Tp> static inline
Point_<_Tp> operator * (const Point_<_Tp>& a, float b)
{
    return Point_<_Tp>( saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b) );
}

template<typename _Tp> static inline
Point_<_Tp> operator * (float a, const Point_<_Tp>& b)
{
    return Point_<_Tp>( saturate_cast<_Tp>(b.x*a), saturate_cast<_Tp>(b.y*a) );
}

template<typename _Tp> static inline
Point_<_Tp> operator * (const Point_<_Tp>& a, double b)
{
    return Point_<_Tp>( saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b) );
}

template<typename _Tp> static inline
Point_<_Tp> operator * (double a, const Point_<_Tp>& b)
{
    return Point_<_Tp>( saturate_cast<_Tp>(b.x*a), saturate_cast<_Tp>(b.y*a) );
}

template<typename _Tp> static inline
Point_<_Tp> operator * (const Matx<_Tp, 2, 2>& a, const Point_<_Tp>& b)
{
    Matx<_Tp, 2, 1> tmp = a * Vec<_Tp,2>(b.x, b.y);
    return Point_<_Tp>(tmp.val[0], tmp.val[1]);
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (const Matx<_Tp, 3, 3>& a, const Point_<_Tp>& b)
{
    Matx<_Tp, 3, 1> tmp = a * Vec<_Tp,3>(b.x, b.y, 1);
    return Point3_<_Tp>(tmp.val[0], tmp.val[1], tmp.val[2]);
}

template<typename _Tp> static inline
Point_<_Tp> operator / (const Point_<_Tp>& a, int b)
{
    Point_<_Tp> tmp(a);
    tmp /= b;
    return tmp;
}

template<typename _Tp> static inline
Point_<_Tp> operator / (const Point_<_Tp>& a, float b)
{
    Point_<_Tp> tmp(a);
    tmp /= b;
    return tmp;
}

template<typename _Tp> static inline
Point_<_Tp> operator / (const Point_<_Tp>& a, double b)
{
    Point_<_Tp> tmp(a);
    tmp /= b;
    return tmp;
}


template<typename _AccTp> static inline _AccTp normL2Sqr(const Point_<int>& pt);
template<typename _AccTp> static inline _AccTp normL2Sqr(const Point_<int64>& pt);
template<typename _AccTp> static inline _AccTp normL2Sqr(const Point_<float>& pt);
template<typename _AccTp> static inline _AccTp normL2Sqr(const Point_<double>& pt);

template<> inline int normL2Sqr<int>(const Point_<int>& pt) { return pt.dot(pt); }
template<> inline int64 normL2Sqr<int64>(const Point_<int64>& pt) { return pt.dot(pt); }
template<> inline float normL2Sqr<float>(const Point_<float>& pt) { return pt.dot(pt); }
template<> inline double normL2Sqr<double>(const Point_<int>& pt) { return pt.dot(pt); }

template<> inline double normL2Sqr<double>(const Point_<float>& pt) { return pt.ddot(pt); }
template<> inline double normL2Sqr<double>(const Point_<double>& pt) { return pt.ddot(pt); }



//////////////////////////////// 3D Point ///////////////////////////////

template<typename _Tp> inline
Point3_<_Tp>::Point3_()
    : x(0), y(0), z(0) {}

template<typename _Tp> inline
Point3_<_Tp>::Point3_(_Tp _x, _Tp _y, _Tp _z)
    : x(_x), y(_y), z(_z) {}

template<typename _Tp> inline
Point3_<_Tp>::Point3_(const Point3_& pt)
    : x(pt.x), y(pt.y), z(pt.z) {}

template<typename _Tp> inline
Point3_<_Tp>::Point3_(Point3_&& pt) 
    : x(std::move(pt.x)), y(std::move(pt.y)), z(std::move(pt.z)) {}

template<typename _Tp> inline
Point3_<_Tp>::Point3_(const Point_<_Tp>& pt)
    : x(pt.x), y(pt.y), z(_Tp()) {}

template<typename _Tp> inline
Point3_<_Tp>::Point3_(const Vec<_Tp, 3>& v)
    : x(v[0]), y(v[1]), z(v[2]) {}

template<typename _Tp> template<typename _Tp2> inline
Point3_<_Tp>::operator Point3_<_Tp2>() const
{
    return Point3_<_Tp2>(saturate_cast<_Tp2>(x), saturate_cast<_Tp2>(y), saturate_cast<_Tp2>(z));
}

template<typename _Tp> inline
Point3_<_Tp>::operator Vec<_Tp, 3>() const
{
    return Vec<_Tp, 3>(x, y, z);
}

template<typename _Tp> inline
Point3_<_Tp>& Point3_<_Tp>::operator = (const Point3_& pt)
{
    x = pt.x; y = pt.y; z = pt.z;
    return *this;
}

template<typename _Tp> inline
Point3_<_Tp>& Point3_<_Tp>::operator = (Point3_&& pt) 
{
    x = std::move(pt.x); y = std::move(pt.y); z = std::move(pt.z);
    return *this;
}

template<typename _Tp> inline
_Tp Point3_<_Tp>::dot(const Point3_& pt) const
{
    return saturate_cast<_Tp>(x*pt.x + y*pt.y + z*pt.z);
}

template<typename _Tp> inline
double Point3_<_Tp>::ddot(const Point3_& pt) const
{
    return (double)x*pt.x + (double)y*pt.y + (double)z*pt.z;
}

template<typename _Tp> inline
Point3_<_Tp> Point3_<_Tp>::cross(const Point3_<_Tp>& pt) const
{
    return Point3_<_Tp>(y*pt.z - z*pt.y, z*pt.x - x*pt.z, x*pt.y - y*pt.x);
}


template<typename _Tp> static inline
Point3_<_Tp>& operator += (Point3_<_Tp>& a, const Point3_<_Tp>& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}

template<typename _Tp> static inline
Point3_<_Tp>& operator -= (Point3_<_Tp>& a, const Point3_<_Tp>& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}

template<typename _Tp> static inline
Point3_<_Tp>& operator *= (Point3_<_Tp>& a, int b)
{
    a.x = saturate_cast<_Tp>(a.x * b);
    a.y = saturate_cast<_Tp>(a.y * b);
    a.z = saturate_cast<_Tp>(a.z * b);
    return a;
}

template<typename _Tp> static inline
Point3_<_Tp>& operator *= (Point3_<_Tp>& a, float b)
{
    a.x = saturate_cast<_Tp>(a.x * b);
    a.y = saturate_cast<_Tp>(a.y * b);
    a.z = saturate_cast<_Tp>(a.z * b);
    return a;
}

template<typename _Tp> static inline
Point3_<_Tp>& operator *= (Point3_<_Tp>& a, double b)
{
    a.x = saturate_cast<_Tp>(a.x * b);
    a.y = saturate_cast<_Tp>(a.y * b);
    a.z = saturate_cast<_Tp>(a.z * b);
    return a;
}

template<typename _Tp> static inline
Point3_<_Tp>& operator /= (Point3_<_Tp>& a, int b)
{
    a.x = saturate_cast<_Tp>(a.x / b);
    a.y = saturate_cast<_Tp>(a.y / b);
    a.z = saturate_cast<_Tp>(a.z / b);
    return a;
}

template<typename _Tp> static inline
Point3_<_Tp>& operator /= (Point3_<_Tp>& a, float b)
{
    a.x = saturate_cast<_Tp>(a.x / b);
    a.y = saturate_cast<_Tp>(a.y / b);
    a.z = saturate_cast<_Tp>(a.z / b);
    return a;
}

template<typename _Tp> static inline
Point3_<_Tp>& operator /= (Point3_<_Tp>& a, double b)
{
    a.x = saturate_cast<_Tp>(a.x / b);
    a.y = saturate_cast<_Tp>(a.y / b);
    a.z = saturate_cast<_Tp>(a.z / b);
    return a;
}

template<typename _Tp> static inline
double norm(const Point3_<_Tp>& pt)
{
    return std::sqrt((double)pt.x*pt.x + (double)pt.y*pt.y + (double)pt.z*pt.z);
}

template<typename _Tp> static inline
bool operator == (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

template<typename _Tp> static inline
bool operator != (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

template<typename _Tp> static inline
Point3_<_Tp> operator + (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
{
    return Point3_<_Tp>( saturate_cast<_Tp>(a.x + b.x), saturate_cast<_Tp>(a.y + b.y), saturate_cast<_Tp>(a.z + b.z));
}

template<typename _Tp> static inline
Point3_<_Tp> operator - (const Point3_<_Tp>& a, const Point3_<_Tp>& b)
{
    return Point3_<_Tp>( saturate_cast<_Tp>(a.x - b.x), saturate_cast<_Tp>(a.y - b.y), saturate_cast<_Tp>(a.z - b.z));
}

template<typename _Tp> static inline
Point3_<_Tp> operator - (const Point3_<_Tp>& a)
{
    return Point3_<_Tp>( saturate_cast<_Tp>(-a.x), saturate_cast<_Tp>(-a.y), saturate_cast<_Tp>(-a.z) );
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (const Point3_<_Tp>& a, int b)
{
    return Point3_<_Tp>( saturate_cast<_Tp>(a.x*b), saturate_cast<_Tp>(a.y*b), saturate_cast<_Tp>(a.z*b) );
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (int a, const Point3_<_Tp>& b)
{
    return Point3_<_Tp>( saturate_cast<_Tp>(b.x * a), saturate_cast<_Tp>(b.y * a), saturate_cast<_Tp>(b.z * a) );
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (const Point3_<_Tp>& a, float b)
{
    return Point3_<_Tp>( saturate_cast<_Tp>(a.x * b), saturate_cast<_Tp>(a.y * b), saturate_cast<_Tp>(a.z * b) );
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (float a, const Point3_<_Tp>& b)
{
    return Point3_<_Tp>( saturate_cast<_Tp>(b.x * a), saturate_cast<_Tp>(b.y * a), saturate_cast<_Tp>(b.z * a) );
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (const Point3_<_Tp>& a, double b)
{
    return Point3_<_Tp>( saturate_cast<_Tp>(a.x * b), saturate_cast<_Tp>(a.y * b), saturate_cast<_Tp>(a.z * b) );
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (double a, const Point3_<_Tp>& b)
{
    return Point3_<_Tp>( saturate_cast<_Tp>(b.x * a), saturate_cast<_Tp>(b.y * a), saturate_cast<_Tp>(b.z * a) );
}

template<typename _Tp> static inline
Point3_<_Tp> operator * (const Matx<_Tp, 3, 3>& a, const Point3_<_Tp>& b)
{
    Matx<_Tp, 3, 1> tmp = a * Vec<_Tp,3>(b.x, b.y, b.z);
    return Point3_<_Tp>(tmp.val[0], tmp.val[1], tmp.val[2]);
}

template<typename _Tp> static inline
Matx<_Tp, 4, 1> operator * (const Matx<_Tp, 4, 4>& a, const Point3_<_Tp>& b)
{
    return a * Matx<_Tp, 4, 1>(b.x, b.y, b.z, 1);
}

template<typename _Tp> static inline
Point3_<_Tp> operator / (const Point3_<_Tp>& a, int b)
{
    Point3_<_Tp> tmp(a);
    tmp /= b;
    return tmp;
}

template<typename _Tp> static inline
Point3_<_Tp> operator / (const Point3_<_Tp>& a, float b)
{
    Point3_<_Tp> tmp(a);
    tmp /= b;
    return tmp;
}

template<typename _Tp> static inline
Point3_<_Tp> operator / (const Point3_<_Tp>& a, double b)
{
    Point3_<_Tp> tmp(a);
    tmp /= b;
    return tmp;
}



////////////////////////////////// Size /////////////////////////////////

template<typename _Tp> inline
Size_<_Tp>::Size_()
    : width(0), height(0) {}

template<typename _Tp> inline
Size_<_Tp>::Size_(_Tp _width, _Tp _height)
    : width(_width), height(_height) {}

template<typename _Tp> inline
Size_<_Tp>::Size_(const Size_& sz)
    : width(sz.width), height(sz.height) {}

template<typename _Tp> inline
Size_<_Tp>::Size_(Size_&& sz) 
    : width(std::move(sz.width)), height(std::move(sz.height)) {}

template<typename _Tp> inline
Size_<_Tp>::Size_(const Point_<_Tp>& pt)
    : width(pt.x), height(pt.y) {}

template<typename _Tp> template<typename _Tp2> inline
Size_<_Tp>::operator Size_<_Tp2>() const
{
    return Size_<_Tp2>(saturate_cast<_Tp2>(width), saturate_cast<_Tp2>(height));
}

template<typename _Tp> inline
Size_<_Tp>& Size_<_Tp>::operator = (const Size_<_Tp>& sz)
{
    width = sz.width; height = sz.height;
    return *this;
}

template<typename _Tp> inline
Size_<_Tp>& Size_<_Tp>::operator = (Size_<_Tp>&& sz) 
{
    width = std::move(sz.width); height = std::move(sz.height);
    return *this;
}

template<typename _Tp> inline
_Tp Size_<_Tp>::area() const
{
    const _Tp result = width * height;
    CV_DbgAssert(!std::numeric_limits<_Tp>::is_integer
        || width == 0 || result / width == height); // make sure the result fits in the return value
    return result;
}

template<typename _Tp> inline
double Size_<_Tp>::aspectRatio() const
{
    return width / static_cast<double>(height);
}

template<typename _Tp> inline
bool Size_<_Tp>::empty() const
{
    return width <= 0 || height <= 0;
}


template<typename _Tp> static inline
Size_<_Tp>& operator *= (Size_<_Tp>& a, _Tp b)
{
    a.width *= b;
    a.height *= b;
    return a;
}

template<typename _Tp> static inline
Size_<_Tp> operator * (const Size_<_Tp>& a, _Tp b)
{
    Size_<_Tp> tmp(a);
    tmp *= b;
    return tmp;
}

template<typename _Tp> static inline
Size_<_Tp>& operator /= (Size_<_Tp>& a, _Tp b)
{
    a.width /= b;
    a.height /= b;
    return a;
}

template<typename _Tp> static inline
Size_<_Tp> operator / (const Size_<_Tp>& a, _Tp b)
{
    Size_<_Tp> tmp(a);
    tmp /= b;
    return tmp;
}

template<typename _Tp> static inline
Size_<_Tp>& operator += (Size_<_Tp>& a, const Size_<_Tp>& b)
{
    a.width += b.width;
    a.height += b.height;
    return a;
}

template<typename _Tp> static inline
Size_<_Tp> operator + (const Size_<_Tp>& a, const Size_<_Tp>& b)
{
    Size_<_Tp> tmp(a);
    tmp += b;
    return tmp;
}

template<typename _Tp> static inline
Size_<_Tp>& operator -= (Size_<_Tp>& a, const Size_<_Tp>& b)
{
    a.width -= b.width;
    a.height -= b.height;
    return a;
}

template<typename _Tp> static inline
Size_<_Tp> operator - (const Size_<_Tp>& a, const Size_<_Tp>& b)
{
    Size_<_Tp> tmp(a);
    tmp -= b;
    return tmp;
}

template<typename _Tp> static inline
bool operator == (const Size_<_Tp>& a, const Size_<_Tp>& b)
{
    return a.width == b.width && a.height == b.height;
}

template<typename _Tp> static inline
bool operator != (const Size_<_Tp>& a, const Size_<_Tp>& b)
{
    return !(a == b);
}



////////////////////////////////// Rect /////////////////////////////////

template<typename _Tp> inline
Rect_<_Tp>::Rect_()
    : x(0), y(0), width(0), height(0) {}

template<typename _Tp> inline
Rect_<_Tp>::Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height)
    : x(_x), y(_y), width(_width), height(_height) {}

template<typename _Tp> inline
Rect_<_Tp>::Rect_(const Rect_<_Tp>& r)
    : x(r.x), y(r.y), width(r.width), height(r.height) {}

template<typename _Tp> inline
Rect_<_Tp>::Rect_(Rect_<_Tp>&& r) 
    : x(std::move(r.x)), y(std::move(r.y)), width(std::move(r.width)), height(std::move(r.height)) {}

template<typename _Tp> inline
Rect_<_Tp>::Rect_(const Point_<_Tp>& org, const Size_<_Tp>& sz)
    : x(org.x), y(org.y), width(sz.width), height(sz.height) {}

template<typename _Tp> inline
Rect_<_Tp>::Rect_(const Point_<_Tp>& pt1, const Point_<_Tp>& pt2)
{
    x = std::min(pt1.x, pt2.x);
    y = std::min(pt1.y, pt2.y);
    width = std::max(pt1.x, pt2.x) - x;
    height = std::max(pt1.y, pt2.y) - y;
}

template<typename _Tp> inline
Rect_<_Tp>& Rect_<_Tp>::operator = ( const Rect_<_Tp>& r )
{
    x = r.x;
    y = r.y;
    width = r.width;
    height = r.height;
    return *this;
}

template<typename _Tp> inline
Rect_<_Tp>& Rect_<_Tp>::operator = ( Rect_<_Tp>&& r ) 
{
    x = std::move(r.x);
    y = std::move(r.y);
    width = std::move(r.width);
    height = std::move(r.height);
    return *this;
}

template<typename _Tp> inline
Point_<_Tp> Rect_<_Tp>::tl() const
{
    return Point_<_Tp>(x,y);
}

template<typename _Tp> inline
Point_<_Tp> Rect_<_Tp>::br() const
{
    return Point_<_Tp>(x + width, y + height);
}

template<typename _Tp> inline
Size_<_Tp> Rect_<_Tp>::size() const
{
    return Size_<_Tp>(width, height);
}

template<typename _Tp> inline
_Tp Rect_<_Tp>::area() const
{
    const _Tp result = width * height;
    CV_DbgAssert(!std::numeric_limits<_Tp>::is_integer
        || width == 0 || result / width == height); // make sure the result fits in the return value
    return result;
}

template<typename _Tp> inline
bool Rect_<_Tp>::empty() const
{
    return width <= 0 || height <= 0;
}

template<typename _Tp> template<typename _Tp2> inline
Rect_<_Tp>::operator Rect_<_Tp2>() const
{
    return Rect_<_Tp2>(saturate_cast<_Tp2>(x), saturate_cast<_Tp2>(y), saturate_cast<_Tp2>(width), saturate_cast<_Tp2>(height));
}

template<typename _Tp> inline
bool Rect_<_Tp>::contains(const Point_<_Tp>& pt) const
{
    return x <= pt.x && pt.x < x + width && y <= pt.y && pt.y < y + height;
}


template<typename _Tp> static inline
Rect_<_Tp>& operator += ( Rect_<_Tp>& a, const Point_<_Tp>& b )
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

template<typename _Tp> static inline
Rect_<_Tp>& operator -= ( Rect_<_Tp>& a, const Point_<_Tp>& b )
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

template<typename _Tp> static inline
Rect_<_Tp>& operator += ( Rect_<_Tp>& a, const Size_<_Tp>& b )
{
    a.width += b.width;
    a.height += b.height;
    return a;
}

template<typename _Tp> static inline
Rect_<_Tp>& operator -= ( Rect_<_Tp>& a, const Size_<_Tp>& b )
{
    const _Tp width = a.width - b.width;
    const _Tp height = a.height - b.height;
    CV_DbgAssert(width >= 0 && height >= 0);
    a.width = width;
    a.height = height;
    return a;
}

template<typename _Tp> static inline
Rect_<_Tp>& operator &= ( Rect_<_Tp>& a, const Rect_<_Tp>& b )
{
    _Tp x1 = std::max(a.x, b.x);
    _Tp y1 = std::max(a.y, b.y);
    a.width = std::min(a.x + a.width, b.x + b.width) - x1;
    a.height = std::min(a.y + a.height, b.y + b.height) - y1;
    a.x = x1;
    a.y = y1;
    if( a.width <= 0 || a.height <= 0 )
        a = Rect();
    return a;
}

template<typename _Tp> static inline
Rect_<_Tp>& operator |= ( Rect_<_Tp>& a, const Rect_<_Tp>& b )
{
    if (a.empty()) {
        a = b;
    }
    else if (!b.empty()) {
        _Tp x1 = std::min(a.x, b.x);
        _Tp y1 = std::min(a.y, b.y);
        a.width = std::max(a.x + a.width, b.x + b.width) - x1;
        a.height = std::max(a.y + a.height, b.y + b.height) - y1;
        a.x = x1;
        a.y = y1;
    }
    return a;
}

template<typename _Tp> static inline
bool operator == (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
{
    return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
}

template<typename _Tp> static inline
bool operator != (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
{
    return a.x != b.x || a.y != b.y || a.width != b.width || a.height != b.height;
}

template<typename _Tp> static inline
Rect_<_Tp> operator + (const Rect_<_Tp>& a, const Point_<_Tp>& b)
{
    return Rect_<_Tp>( a.x + b.x, a.y + b.y, a.width, a.height );
}

template<typename _Tp> static inline
Rect_<_Tp> operator - (const Rect_<_Tp>& a, const Point_<_Tp>& b)
{
    return Rect_<_Tp>( a.x - b.x, a.y - b.y, a.width, a.height );
}

template<typename _Tp> static inline
Rect_<_Tp> operator + (const Rect_<_Tp>& a, const Size_<_Tp>& b)
{
    return Rect_<_Tp>( a.x, a.y, a.width + b.width, a.height + b.height );
}

template<typename _Tp> static inline
Rect_<_Tp> operator - (const Rect_<_Tp>& a, const Size_<_Tp>& b)
{
    const _Tp width = a.width - b.width;
    const _Tp height = a.height - b.height;
    CV_DbgAssert(width >= 0 && height >= 0);
    return Rect_<_Tp>( a.x, a.y, width, height );
}

template<typename _Tp> static inline
Rect_<_Tp> operator & (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
{
    Rect_<_Tp> c = a;
    return c &= b;
}

template<typename _Tp> static inline
Rect_<_Tp> operator | (const Rect_<_Tp>& a, const Rect_<_Tp>& b)
{
    Rect_<_Tp> c = a;
    return c |= b;
}

/**
 * @brief measure dissimilarity between two sample sets
 *
 * computes the complement of the Jaccard Index as described in <https://en.wikipedia.org/wiki/Jaccard_index>.
 * For rectangles this reduces to computing the intersection over the union.
 */
template<typename _Tp> static inline
double jaccardDistance(const Rect_<_Tp>& a, const Rect_<_Tp>& b) {
    _Tp Aa = a.area();
    _Tp Ab = b.area();

    if ((Aa + Ab) <= std::numeric_limits<_Tp>::epsilon()) {
        // jaccard_index = 1 -> distance = 0
        return 0.0;
    }

    double Aab = (a & b).area();
    // distance = 1 - jaccard_index
    return 1.0 - Aab / (Aa + Ab - Aab);
}

////////////////////////////// RotatedRect //////////////////////////////

inline
RotatedRect::RotatedRect()
    : center(), size(), angle(0) {}

inline
RotatedRect::RotatedRect(const Point2f& _center, const Size2f& _size, float _angle)
    : center(_center), size(_size), angle(_angle) {}

///////////////////////////////// Range /////////////////////////////////

inline
Range::Range()
    : start(0), end(0) {}

inline
Range::Range(int _start, int _end)
    : start(_start), end(_end) {}

inline
int Range::size() const
{
    return end - start;
}

inline
bool Range::empty() const
{
    return start == end;
}

inline
Range Range::all()
{
    return Range(INT_MIN, INT_MAX);
}


static inline
bool operator == (const Range& r1, const Range& r2)
{
    return r1.start == r2.start && r1.end == r2.end;
}

static inline
bool operator != (const Range& r1, const Range& r2)
{
    return !(r1 == r2);
}

static inline
bool operator !(const Range& r)
{
    return r.start == r.end;
}

static inline
Range operator & (const Range& r1, const Range& r2)
{
    Range r(std::max(r1.start, r2.start), std::min(r1.end, r2.end));
    r.end = std::max(r.end, r.start);
    return r;
}

static inline
Range& operator &= (Range& r1, const Range& r2)
{
    r1 = r1 & r2;
    return r1;
}

static inline
Range operator + (const Range& r1, int delta)
{
    return Range(r1.start + delta, r1.end + delta);
}

static inline
Range operator + (int delta, const Range& r1)
{
    return Range(r1.start + delta, r1.end + delta);
}

static inline
Range operator - (const Range& r1, int delta)
{
    return r1 + (-delta);
}



///////////////////////////////// Scalar ////////////////////////////////

template<typename _Tp> inline
Scalar_<_Tp>::Scalar_()
{
    this->val[0] = this->val[1] = this->val[2] = this->val[3] = 0;
}

template<typename _Tp> inline
Scalar_<_Tp>::Scalar_(_Tp v0, _Tp v1, _Tp v2, _Tp v3)
{
    this->val[0] = v0;
    this->val[1] = v1;
    this->val[2] = v2;
    this->val[3] = v3;
}

template<typename _Tp> inline
Scalar_<_Tp>::Scalar_(const Scalar_<_Tp>& s) : Vec<_Tp, 4>(s) {
}

template<typename _Tp> inline
Scalar_<_Tp>::Scalar_(Scalar_<_Tp>&& s)  {
    this->val[0] = std::move(s.val[0]);
    this->val[1] = std::move(s.val[1]);
    this->val[2] = std::move(s.val[2]);
    this->val[3] = std::move(s.val[3]);
}

template<typename _Tp> inline
Scalar_<_Tp>& Scalar_<_Tp>::operator=(const Scalar_<_Tp>& s) {
    this->val[0] = s.val[0];
    this->val[1] = s.val[1];
    this->val[2] = s.val[2];
    this->val[3] = s.val[3];
    return *this;
}

template<typename _Tp> inline
Scalar_<_Tp>& Scalar_<_Tp>::operator=(Scalar_<_Tp>&& s)  {
    this->val[0] = std::move(s.val[0]);
    this->val[1] = std::move(s.val[1]);
    this->val[2] = std::move(s.val[2]);
    this->val[3] = std::move(s.val[3]);
    return *this;
}

template<typename _Tp> template<typename _Tp2, int cn> inline
Scalar_<_Tp>::Scalar_(const Vec<_Tp2, cn>& v)
{
    int i;
    for( i = 0; i < (cn < 4 ? cn : 4); i++ )
        this->val[i] = cv::saturate_cast<_Tp>(v.val[i]);
    for( ; i < 4; i++ )
        this->val[i] = 0;
}

template<typename _Tp> inline
Scalar_<_Tp>::Scalar_(_Tp v0)
{
    this->val[0] = v0;
    this->val[1] = this->val[2] = this->val[3] = 0;
}

template<typename _Tp> inline
Scalar_<_Tp> Scalar_<_Tp>::all(_Tp v0)
{
    return Scalar_<_Tp>(v0, v0, v0, v0);
}


template<typename _Tp> inline
Scalar_<_Tp> Scalar_<_Tp>::mul(const Scalar_<_Tp>& a, double scale ) const
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(this->val[0] * a.val[0] * scale),
                        saturate_cast<_Tp>(this->val[1] * a.val[1] * scale),
                        saturate_cast<_Tp>(this->val[2] * a.val[2] * scale),
                        saturate_cast<_Tp>(this->val[3] * a.val[3] * scale));
}

template<typename _Tp> inline
Scalar_<_Tp> Scalar_<_Tp>::conj() const
{
    return Scalar_<_Tp>(saturate_cast<_Tp>( this->val[0]),
                        saturate_cast<_Tp>(-this->val[1]),
                        saturate_cast<_Tp>(-this->val[2]),
                        saturate_cast<_Tp>(-this->val[3]));
}

template<typename _Tp> inline
bool Scalar_<_Tp>::isReal() const
{
    return this->val[1] == 0 && this->val[2] == 0 && this->val[3] == 0;
}


template<typename _Tp> template<typename T2> inline
Scalar_<_Tp>::operator Scalar_<T2>() const
{
    return Scalar_<T2>(saturate_cast<T2>(this->val[0]),
                       saturate_cast<T2>(this->val[1]),
                       saturate_cast<T2>(this->val[2]),
                       saturate_cast<T2>(this->val[3]));
}


template<typename _Tp> static inline
Scalar_<_Tp>& operator += (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    a.val[0] += b.val[0];
    a.val[1] += b.val[1];
    a.val[2] += b.val[2];
    a.val[3] += b.val[3];
    return a;
}

template<typename _Tp> static inline
Scalar_<_Tp>& operator -= (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    a.val[0] -= b.val[0];
    a.val[1] -= b.val[1];
    a.val[2] -= b.val[2];
    a.val[3] -= b.val[3];
    return a;
}

template<typename _Tp> static inline
Scalar_<_Tp>& operator *= ( Scalar_<_Tp>& a, _Tp v )
{
    a.val[0] *= v;
    a.val[1] *= v;
    a.val[2] *= v;
    a.val[3] *= v;
    return a;
}

template<typename _Tp> static inline
bool operator == ( const Scalar_<_Tp>& a, const Scalar_<_Tp>& b )
{
    return a.val[0] == b.val[0] && a.val[1] == b.val[1] &&
           a.val[2] == b.val[2] && a.val[3] == b.val[3];
}

template<typename _Tp> static inline
bool operator != ( const Scalar_<_Tp>& a, const Scalar_<_Tp>& b )
{
    return a.val[0] != b.val[0] || a.val[1] != b.val[1] ||
           a.val[2] != b.val[2] || a.val[3] != b.val[3];
}

template<typename _Tp> static inline
Scalar_<_Tp> operator + (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    return Scalar_<_Tp>(a.val[0] + b.val[0],
                        a.val[1] + b.val[1],
                        a.val[2] + b.val[2],
                        a.val[3] + b.val[3]);
}

template<typename _Tp> static inline
Scalar_<_Tp> operator - (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(a.val[0] - b.val[0]),
                        saturate_cast<_Tp>(a.val[1] - b.val[1]),
                        saturate_cast<_Tp>(a.val[2] - b.val[2]),
                        saturate_cast<_Tp>(a.val[3] - b.val[3]));
}

template<typename _Tp> static inline
Scalar_<_Tp> operator * (const Scalar_<_Tp>& a, _Tp alpha)
{
    return Scalar_<_Tp>(a.val[0] * alpha,
                        a.val[1] * alpha,
                        a.val[2] * alpha,
                        a.val[3] * alpha);
}

template<typename _Tp> static inline
Scalar_<_Tp> operator * (_Tp alpha, const Scalar_<_Tp>& a)
{
    return a*alpha;
}

template<typename _Tp> static inline
Scalar_<_Tp> operator - (const Scalar_<_Tp>& a)
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(-a.val[0]),
                        saturate_cast<_Tp>(-a.val[1]),
                        saturate_cast<_Tp>(-a.val[2]),
                        saturate_cast<_Tp>(-a.val[3]));
}


template<typename _Tp> static inline
Scalar_<_Tp> operator * (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    return Scalar_<_Tp>(saturate_cast<_Tp>(a[0]*b[0] - a[1]*b[1] - a[2]*b[2] - a[3]*b[3]),
                        saturate_cast<_Tp>(a[0]*b[1] + a[1]*b[0] + a[2]*b[3] - a[3]*b[2]),
                        saturate_cast<_Tp>(a[0]*b[2] - a[1]*b[3] + a[2]*b[0] + a[3]*b[1]),
                        saturate_cast<_Tp>(a[0]*b[3] + a[1]*b[2] - a[2]*b[1] + a[3]*b[0]));
}

template<typename _Tp> static inline
Scalar_<_Tp>& operator *= (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    a = a * b;
    return a;
}

template<typename _Tp> static inline
Scalar_<_Tp> operator / (const Scalar_<_Tp>& a, _Tp alpha)
{
    return Scalar_<_Tp>(a.val[0] / alpha,
                        a.val[1] / alpha,
                        a.val[2] / alpha,
                        a.val[3] / alpha);
}

template<typename _Tp> static inline
Scalar_<float> operator / (const Scalar_<float>& a, float alpha)
{
    float s = 1 / alpha;
    return Scalar_<float>(a.val[0] * s, a.val[1] * s, a.val[2] * s, a.val[3] * s);
}

template<typename _Tp> static inline
Scalar_<double> operator / (const Scalar_<double>& a, double alpha)
{
    double s = 1 / alpha;
    return Scalar_<double>(a.val[0] * s, a.val[1] * s, a.val[2] * s, a.val[3] * s);
}

template<typename _Tp> static inline
Scalar_<_Tp>& operator /= (Scalar_<_Tp>& a, _Tp alpha)
{
    a = a / alpha;
    return a;
}

template<typename _Tp> static inline
Scalar_<_Tp> operator / (_Tp a, const Scalar_<_Tp>& b)
{
    _Tp s = a / (b[0]*b[0] + b[1]*b[1] + b[2]*b[2] + b[3]*b[3]);
    return b.conj() * s;
}

template<typename _Tp> static inline
Scalar_<_Tp> operator / (const Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    return a * ((_Tp)1 / b);
}

template<typename _Tp> static inline
Scalar_<_Tp>& operator /= (Scalar_<_Tp>& a, const Scalar_<_Tp>& b)
{
    a = a / b;
    return a;
}

//template<typename _Tp> static inline
//Scalar operator * (const Matx<_Tp, 4, 4>& a, const Scalar& b)
//{
//    Matx<double, 4, 1> c((Matx<double, 4, 4>)a, b, Matx_MatMulOp());
//    return reinterpret_cast<const Scalar&>(c);
//}
//
//template<> inline
//Scalar operator * (const Matx<double, 4, 4>& a, const Scalar& b)
//{
//    Matx<double, 4, 1> c(a, b, Matx_MatMulOp());
//    return reinterpret_cast<const Scalar&>(c);
//}



//////////////////////////////// KeyPoint ///////////////////////////////

inline
KeyPoint::KeyPoint()
    : pt(0,0), size(0), angle(-1), response(0), octave(0), class_id(-1) {}

inline
KeyPoint::KeyPoint(Point2f _pt, float _size, float _angle, float _response, int _octave, int _class_id)
    : pt(_pt), size(_size), angle(_angle), response(_response), octave(_octave), class_id(_class_id) {}

inline
KeyPoint::KeyPoint(float x, float y, float _size, float _angle, float _response, int _octave, int _class_id)
    : pt(x, y), size(_size), angle(_angle), response(_response), octave(_octave), class_id(_class_id) {}



///////////////////////////////// DMatch ////////////////////////////////

inline
DMatch::DMatch()
    : queryIdx(-1), trainIdx(-1), imgIdx(-1), distance(FLT_MAX) {}

inline
DMatch::DMatch(int _queryIdx, int _trainIdx, float _distance)
    : queryIdx(_queryIdx), trainIdx(_trainIdx), imgIdx(-1), distance(_distance) {}

inline
DMatch::DMatch(int _queryIdx, int _trainIdx, int _imgIdx, float _distance)
    : queryIdx(_queryIdx), trainIdx(_trainIdx), imgIdx(_imgIdx), distance(_distance) {}

inline
bool DMatch::operator < (const DMatch &m) const
{
    return distance < m.distance;
}



////////////////////////////// TermCriteria /////////////////////////////

inline
TermCriteria::TermCriteria()
    : type(0), maxCount(0), epsilon(0) {}

inline
TermCriteria::TermCriteria(int _type, int _maxCount, double _epsilon)
    : type(_type), maxCount(_maxCount), epsilon(_epsilon) {}

//! @endcond











#ifdef UNIT_TEST
BASIC_API int Test_BaseType()
{
	HalfFloat F;
	F=0.52555F;
	F.printInterValue();
	F.printValue();
	return 0;
}
#endif



NAME_SPACE_MAIN_END


#endif
