 #ifndef SIMD_H
#define SIMD_H

#include "global.h"


//SSE指令首地址必须是16字节对齐的   具体方法在后面
//封装简单常用的浮点数指令加速
//单精度浮点数组求和_SSE版. 


//需要手动在项目属性的“C++\命令行”的附加选项中加上“/arch:AVX”—— 否则AVX反而可能比普通更慢

float SSE_floatSum(const float* pbuf,unsigned int len);   //一列浮点数求和
float SSE_floatMuligain(float* pbufresult,const float* pbuf1,const float gain,unsigned int len);   //一列浮点数乘以一个gain   buf1*gain

void SSE_floatAdd(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len);    //buf1+buf2
void SSE_floatSub(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len);    //buf1-buf2
void SSE_floatMuli(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len);   //buf1*buf2
void SSE_floatDiv(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len);     //buf1/buf2
void SSE_floatSqrt(float* pbufresult,const float* pbuf1,unsigned int len);  //sqrt(pbuf1)



double SSE_doubleSum(const double* pbuf,unsigned int cntbuf);
void SSE_doubleAdd(double* pbufresult,const double* pbuf1, const double* pbuf2,unsigned int len); 
void SSE_doubleMuli(double* pbufresult,const double* pbuf1, const double* pbuf2,unsigned int len); 
void SSE_doubleDiv(double* pbufresult,const double* pbuf1, const double* pbuf2,unsigned int len); 



float  AVX_floatSum(const float* pbuf,unsigned int len);
void  AVX_floatMuligain(float* pbufresult,const float* pbuf1,const float gain,unsigned int len);   //一列浮点数乘以一个gain   buf1*gain
void  AVX_floatAdd(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len); 
void  AVX_floatSub(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len); 
void  AVX_floatMuli(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len); 
void  AVX_floatDiv(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len); 
void  AVX_floatSqrt(float* pbufresult,const float* pbuf1,unsigned int len); 



/*怎么在vs2010中使用SSE指令集?
1、主要就是在工程中包含SSE头文件即可，具体如下：
mmintrin.h MMX
xmmintrin.h SSE
emmintrin.h SSE2
pmmintrin.h SSE3
tmmintrin.h SSSE3
intrin.h SSE4A
smmintrin.h SSE4.1
nmmintrin.h SSE4.2
2、然后vs2010工程的编译方式最好修改为x64的64位编译。
3、项目属性的“C/C++”->“语言”中，添加openmp支持。
4、使用鲁大师查看你的CPU特征，以本人i5 2410为例，特征为“MMX SSE SSE2 SSE3 SSE4.1 SSE4.2”，就说明该CPU支持SSE 4.2指令集。
5、在vs2010工程中添加相应指令集的头文件，既可以开始SSE编程之旅。
*/




/*
指令	Intrinsic	功能
addps/addss	_mm_add_ps/_mm_add_ss	加法
subps/subss	_mm_sub_ps/_mm_sub_ss	減法
mulps/mulss	_mm_mul_ps/_mm_mul_ss	乘法
divps/divss	_mm_div_ps/_mm_div_ss	除法
sqrtps/sqrtss	_mm_sqrt_ps/_mm_sqrt_ss	平方根
maxps/maxss	_mm_max_ps/_mm_max_ss	逐項取最大值
minps/minss	_mm_min_ps/_mm_min_ss	逐項取最小值
采用 _mm_load_ps() 指令运行上述算法首先要将所有数据加载到 SIMD 寄存器中。
这样做会获得作为一项参数的数据加载地址。所有拥有 ps 后缀的指令都是单精度
版本的指令。许多指令都拥有多个版本，如 _mm_load_pd() 指令还可以用于加载
两个双精度数字。同样至关重要的是，数据必须为 16 字节对齐数据。如果不是 
16 字节对齐数据，则必须采用一条不同的指令--_mm_loadu_ps() 来运行函数，
但这样做不会带来相同的性能提升优势。


//---------------------说明：_ps结尾的指令表示对4个单精度浮点数同时进行运算，
	//_ss结尾的指令表示仅对4个单精度浮点数最低位的浮点数进行运算---------------------
	//返回一个__m128的寄存器，仅将寄存器_A和寄存器_B最低对应位置的32bit单精度浮点数相加，
	//其余位置取寄存器_A中的数据,例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器为r=(_A0+_B0, _A1, _A2, _A3)
	extern __m128 _mm_add_ss(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，将寄存器_A和_B的对应位置的32bit单精度浮点数相加，
	//例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器r0=_A0+_B0, r1=_A1+_B1, r2=_A2+_B2, r3=_A3+_B3
	extern __m128 _mm_add_ps(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，仅将寄存器_A和寄存器_B最低对应位置的32bit单精度浮点数相减，
	//其余位置取寄存器_A中的数据,例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器为r=(_A0-_B0, _A1, _A2, _A3)
	extern __m128 _mm_sub_ss(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，将寄存器_A和_B的对应位置的32bit单精度浮点数相减，
	//例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器r0=_A0-_B0, r1=_A1-_B1, r2=_A2-_B2, r3=_A3-_B3
	extern __m128 _mm_sub_ps(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，仅将寄存器_A和寄存器_B最低对应位置的32bit单精度浮点数相乘，
	//其余位置取寄存器_A中的数据,例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器为r=(_A0*_B0, _A1, _A2, _A3)
	extern __m128 _mm_mul_ss(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，将寄存器_A和_B的对应位置的32bit单精度浮点数相乘，
	//例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器r0=_A0*_B0, r1=_A1*_B1, r2=_A2*_B2, r3=_A3*_B3
	extern __m128 _mm_mul_ps(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，仅将寄存器_A和寄存器_B最低对应位置的32bit单精度浮点数相除，
	//其余位置取寄存器_A中的数据,例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器为r=(_A0/_B0, _A1, _A2, _A3)
	extern __m128 _mm_div_ss(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，将寄存器_A和_B的对应位置的32bit单精度浮点数相除，
	//例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器r0=_A0/_B0, r1=_A1/_B1, r2=_A2/_B2, r3=_A3/_B3
	extern __m128 _mm_div_ps(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，仅将寄存器_A最低对应位置的32bit单精度浮点数开平方，
	//其余位置取寄存器_A中的数据,例如_A=(_A0,_A1,_A2,_A3)
	//则返回寄存器为r=(sqrt(_A0), _A1, _A2, _A3)
	extern __m128 _mm_sqrt_ss(__m128 _A);
	//返回一个__m128的寄存器，将寄存器_A中4个32bit单精度浮点数开平方，
	//例如_A=(_A0,_A1,_A2,_A3)，则返回寄存器为
	//r=(sqrt(_A0), sqrt(_A1), sqrt(_A2), sqrt(_A3))
	extern __m128 _mm_sqrt_ps(__m128 _A);
	//返回一个__m128的寄存器，仅将寄存器_A最低对应位置的32bit单精度浮点数取倒数，
	//其余位置取寄存器_A中的数据,例如_A=(_A0,_A1,_A2,_A3)
	//则返回寄存器为r=(recip(_A0), _A1, _A2, _A3)
	extern __m128 _mm_rcp_ss(__m128 _A);
	//返回一个__m128的寄存器，将寄存器_A中4个32bit单精度浮点数取倒数，
	//例如_A=(_A0,_A1,_A2,_A3)，则返回寄存器为
	//r=(recip(_A0), recip(_A1), recip(_A2), recip(_A3))
	extern __m128 _mm_rcp_ps(__m128 _A);
	//返回一个__m128的寄存器，仅将寄存器_A最低对应位置的32bit单精度浮点数取平方根的倒数，
	//其余位置取寄存器_A中的数据,例如_A=(_A0,_A1,_A2,_A3)
	//则返回寄存器为r=(recip(sqrt(_A0)), _A1, _A2, _A3)
	extern __m128 _mm_rsqrt_ss(__m128 _A);
	//返回一个__m128的寄存器，将寄存器_A中4个32bit单精度浮点数取平方根的倒数，
	//例如_A=(_A0,_A1,_A2,_A3)，则返回寄存器为
	//r=(recip(sqrt(_A0)), recip(sqrt(_A1)), recip(sqrt(_A2)), recip(sqrt(_A3)))
	extern __m128 _mm_rsqrt_ps(__m128 _A);
	//返回一个__m128的寄存器，仅将寄存器_A和寄存器_B最低对应位置的32bit单精度浮点数取最小值，
	//其余位置取寄存器_A中的数据,例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器为r=(min(_A0,_B0), _A1, _A2, _A3)
	extern __m128 _mm_min_ss(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，将寄存器_A和_B的对应位置的32bit单精度浮点数取最小值，
	//例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器r0=min(_A0,_B0), r1=min(_A1,_B1), r2=min(_A2,_B2), r3=min(_A3,_B3)
	extern __m128 _mm_min_ps(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，仅将寄存器_A和寄存器_B最低对应位置的32bit单精度浮点数取最大值，
	//其余位置取寄存器_A中的数据,例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器为r=(max(_A0,_B0), _A1, _A2, _A3)
	extern __m128 _mm_max_ss(__m128 _A, __m128 _B);
	//返回一个__m128的寄存器，将寄存器_A和_B的对应位置的32bit单精度浮点数取最大值，
	//例如_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//则返回寄存器r0=max(_A0,_B0), r1=max(_A1,_B1), r2=max(_A2,_B2), r3=max(_A3,_B3)
	extern __m128 _mm_max_ps(__m128 _A, __m128 _B);

*/

//SSE指令首地址必须是16字节对齐的   具体方法在后面
/*
是地址对齐还是大小对齐？
大小：((size + (16 - 1)) & ~(16 - 1))
地址比较麻烦，分配的时候多分配16字节，然后把地址按照上面的方式对齐。
我给你个正确可行以及确定的答案：

如果动态分配，用memalign分配
如果分在栈上，用__attritube__((aligned(16)))


既然你用了C++，你要求的这种对齐需要一些变通的办法才能比较优雅的实现：
__m128 *p = new __m128[count];//不管这个__m128是存放在哪里、用途是什么，好歹他对于编译器也只是各类型。这样做可以保证对齐。
如果实在受不了这种直接new，或者因为某些原因不能直接new，也可以用下面的办法：
float * buf = new alignas(16) float [100];
__m128 *p= (__m128*) buf;
鉴于一些编译器还不支持alignas关键字，
float * buf = (float *)new std::align_storage<sizeof(__m128), 16>::type [100 * sizeof(float) / sizeof(__m128)];//注意这一行需要根据使用场景调整，我不知道你具体的使用场景，所以暂时只能这样
__m128 *p= (__m128*) buf;
这样肯定是对齐的

介于曾经我也对这个问题头疼了半天,送楼主一个方法用吧:
 
template<typename T,int alignByte=随便啥字节对齐>
class AlignedAllocator
{
public:
    inline static T* Malloc(size_t size)
    {
        dbg_code(++counter);
        char *ptr,*ptr2,*aligned_ptr;
 
        ptr=new char[size*sizeof(T)+alignByte+sizeof(int)];
 
        ptr2 = ptr + sizeof(int);
        aligned_ptr = ptr2 + (alignByte - ((size_t)ptr2 & (alignByte-1) ));
 
        ptr2 = aligned_ptr - sizeof(int);
        *((int *)ptr2)=(int)(aligned_ptr - ptr);
 
        return(T*)(aligned_ptr);
    }
    inline static void Free(T* ptr)
    {
        int *ptr2=(int *)ptr - 1;
        char*const m=(char*)ptr - *ptr2;
        delete[] m;
    }
private:
    STACK_ONLY;
    ZTD_NO_COPYABLE(AlignedAllocator);
};

结贴，可以直接传递，

编译器会自动检查是否对齐，并调用相应的指令，对其和不对其有不同的执行，对齐的指令效率会高一些

开辟地址对齐的数组我现在使用的是 
float *buf1 = reinterpret_cast<float*>(_aligned_malloc(sizeof(float)*dataLen, 32));

32字节对齐

释放
_aligned_free(buf1);


*/





//
/*
immintrin.h为AVX头文件，此头文件里包含AES头文件，其中__m256、__m256d、__m256i的定义为：
typedef union __declspec(intrin_type) _CRT_ALIGN(32) __m256 { 
    float m256_f32[8];
} __m256;

typedef struct __declspec(intrin_type) _CRT_ALIGN(32) {
    double m256d_f64[4]; 
} __m256d;

typedef union  __declspec(intrin_type) _CRT_ALIGN(32) __m256i {
    __int8              m256i_i8[32];
    __int16             m256i_i16[16];
    __int32             m256i_i32[8];
    __int64             m256i_i64[4];
    unsigned __int8     m256i_u8[32];
    unsigned __int16    m256i_u16[16];
    unsigned __int32    m256i_u32[8];
    unsigned __int64    m256i_u64[4];
} __m256i;
*/





#ifdef UNIT_TEST
int Test_SIMD();
#endif

#endif

