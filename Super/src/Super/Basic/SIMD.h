 #ifndef SIMD_H
#define SIMD_H

#include "global.h"


//SSEָ���׵�ַ������16�ֽڶ����   ���巽���ں���
//��װ�򵥳��õĸ�����ָ�����
//�����ȸ����������_SSE��. 


//��Ҫ�ֶ�����Ŀ���Եġ�C++\�����С��ĸ���ѡ���м��ϡ�/arch:AVX������ ����AVX�������ܱ���ͨ����

float SSE_floatSum(const float* pbuf,unsigned int len);   //һ�и��������
float SSE_floatMuligain(float* pbufresult,const float* pbuf1,const float gain,unsigned int len);   //һ�и���������һ��gain   buf1*gain

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
void  AVX_floatMuligain(float* pbufresult,const float* pbuf1,const float gain,unsigned int len);   //һ�и���������һ��gain   buf1*gain
void  AVX_floatAdd(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len); 
void  AVX_floatSub(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len); 
void  AVX_floatMuli(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len); 
void  AVX_floatDiv(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len); 
void  AVX_floatSqrt(float* pbufresult,const float* pbuf1,unsigned int len); 



/*��ô��vs2010��ʹ��SSEָ�?
1����Ҫ�����ڹ����а���SSEͷ�ļ����ɣ��������£�
mmintrin.h MMX
xmmintrin.h SSE
emmintrin.h SSE2
pmmintrin.h SSE3
tmmintrin.h SSSE3
intrin.h SSE4A
smmintrin.h SSE4.1
nmmintrin.h SSE4.2
2��Ȼ��vs2010���̵ı��뷽ʽ����޸�Ϊx64��64λ���롣
3����Ŀ���Եġ�C/C++��->�����ԡ��У����openmp֧�֡�
4��ʹ��³��ʦ�鿴���CPU�������Ա���i5 2410Ϊ��������Ϊ��MMX SSE SSE2 SSE3 SSE4.1 SSE4.2������˵����CPU֧��SSE 4.2ָ���
5����vs2010�����������Ӧָ���ͷ�ļ����ȿ��Կ�ʼSSE���֮�á�
*/




/*
ָ��	Intrinsic	����
addps/addss	_mm_add_ps/_mm_add_ss	�ӷ�
subps/subss	_mm_sub_ps/_mm_sub_ss	�p��
mulps/mulss	_mm_mul_ps/_mm_mul_ss	�˷�
divps/divss	_mm_div_ps/_mm_div_ss	����
sqrtps/sqrtss	_mm_sqrt_ps/_mm_sqrt_ss	ƽ����
maxps/maxss	_mm_max_ps/_mm_max_ss	���ȡ���ֵ
minps/minss	_mm_min_ps/_mm_min_ss	���ȡ��Сֵ
���� _mm_load_ps() ָ�����������㷨����Ҫ���������ݼ��ص� SIMD �Ĵ����С�
������������Ϊһ����������ݼ��ص�ַ������ӵ�� ps ��׺��ָ��ǵ�����
�汾��ָ����ָ�ӵ�ж���汾���� _mm_load_pd() ָ��������ڼ���
����˫�������֡�ͬ��������Ҫ���ǣ����ݱ���Ϊ 16 �ֽڶ������ݡ�������� 
16 �ֽڶ������ݣ���������һ����ͬ��ָ��--_mm_loadu_ps() �����к�����
�����������������ͬ�������������ơ�


//---------------------˵����_ps��β��ָ���ʾ��4�������ȸ�����ͬʱ�������㣬
	//_ss��β��ָ���ʾ����4�������ȸ��������λ�ĸ�������������---------------------
	//����һ��__m128�ļĴ����������Ĵ���_A�ͼĴ���_B��Ͷ�Ӧλ�õ�32bit�����ȸ�������ӣ�
	//����λ��ȡ�Ĵ���_A�е�����,����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���Ϊr=(_A0+_B0, _A1, _A2, _A3)
	extern __m128 _mm_add_ss(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ��������Ĵ���_A��_B�Ķ�Ӧλ�õ�32bit�����ȸ�������ӣ�
	//����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���r0=_A0+_B0, r1=_A1+_B1, r2=_A2+_B2, r3=_A3+_B3
	extern __m128 _mm_add_ps(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ����������Ĵ���_A�ͼĴ���_B��Ͷ�Ӧλ�õ�32bit�����ȸ����������
	//����λ��ȡ�Ĵ���_A�е�����,����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���Ϊr=(_A0-_B0, _A1, _A2, _A3)
	extern __m128 _mm_sub_ss(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ��������Ĵ���_A��_B�Ķ�Ӧλ�õ�32bit�����ȸ����������
	//����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���r0=_A0-_B0, r1=_A1-_B1, r2=_A2-_B2, r3=_A3-_B3
	extern __m128 _mm_sub_ps(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ����������Ĵ���_A�ͼĴ���_B��Ͷ�Ӧλ�õ�32bit�����ȸ�������ˣ�
	//����λ��ȡ�Ĵ���_A�е�����,����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���Ϊr=(_A0*_B0, _A1, _A2, _A3)
	extern __m128 _mm_mul_ss(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ��������Ĵ���_A��_B�Ķ�Ӧλ�õ�32bit�����ȸ�������ˣ�
	//����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���r0=_A0*_B0, r1=_A1*_B1, r2=_A2*_B2, r3=_A3*_B3
	extern __m128 _mm_mul_ps(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ����������Ĵ���_A�ͼĴ���_B��Ͷ�Ӧλ�õ�32bit�����ȸ����������
	//����λ��ȡ�Ĵ���_A�е�����,����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���Ϊr=(_A0/_B0, _A1, _A2, _A3)
	extern __m128 _mm_div_ss(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ��������Ĵ���_A��_B�Ķ�Ӧλ�õ�32bit�����ȸ����������
	//����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���r0=_A0/_B0, r1=_A1/_B1, r2=_A2/_B2, r3=_A3/_B3
	extern __m128 _mm_div_ps(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ����������Ĵ���_A��Ͷ�Ӧλ�õ�32bit�����ȸ�������ƽ����
	//����λ��ȡ�Ĵ���_A�е�����,����_A=(_A0,_A1,_A2,_A3)
	//�򷵻ؼĴ���Ϊr=(sqrt(_A0), _A1, _A2, _A3)
	extern __m128 _mm_sqrt_ss(__m128 _A);
	//����һ��__m128�ļĴ��������Ĵ���_A��4��32bit�����ȸ�������ƽ����
	//����_A=(_A0,_A1,_A2,_A3)���򷵻ؼĴ���Ϊ
	//r=(sqrt(_A0), sqrt(_A1), sqrt(_A2), sqrt(_A3))
	extern __m128 _mm_sqrt_ps(__m128 _A);
	//����һ��__m128�ļĴ����������Ĵ���_A��Ͷ�Ӧλ�õ�32bit�����ȸ�����ȡ������
	//����λ��ȡ�Ĵ���_A�е�����,����_A=(_A0,_A1,_A2,_A3)
	//�򷵻ؼĴ���Ϊr=(recip(_A0), _A1, _A2, _A3)
	extern __m128 _mm_rcp_ss(__m128 _A);
	//����һ��__m128�ļĴ��������Ĵ���_A��4��32bit�����ȸ�����ȡ������
	//����_A=(_A0,_A1,_A2,_A3)���򷵻ؼĴ���Ϊ
	//r=(recip(_A0), recip(_A1), recip(_A2), recip(_A3))
	extern __m128 _mm_rcp_ps(__m128 _A);
	//����һ��__m128�ļĴ����������Ĵ���_A��Ͷ�Ӧλ�õ�32bit�����ȸ�����ȡƽ�����ĵ�����
	//����λ��ȡ�Ĵ���_A�е�����,����_A=(_A0,_A1,_A2,_A3)
	//�򷵻ؼĴ���Ϊr=(recip(sqrt(_A0)), _A1, _A2, _A3)
	extern __m128 _mm_rsqrt_ss(__m128 _A);
	//����һ��__m128�ļĴ��������Ĵ���_A��4��32bit�����ȸ�����ȡƽ�����ĵ�����
	//����_A=(_A0,_A1,_A2,_A3)���򷵻ؼĴ���Ϊ
	//r=(recip(sqrt(_A0)), recip(sqrt(_A1)), recip(sqrt(_A2)), recip(sqrt(_A3)))
	extern __m128 _mm_rsqrt_ps(__m128 _A);
	//����һ��__m128�ļĴ����������Ĵ���_A�ͼĴ���_B��Ͷ�Ӧλ�õ�32bit�����ȸ�����ȡ��Сֵ��
	//����λ��ȡ�Ĵ���_A�е�����,����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���Ϊr=(min(_A0,_B0), _A1, _A2, _A3)
	extern __m128 _mm_min_ss(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ��������Ĵ���_A��_B�Ķ�Ӧλ�õ�32bit�����ȸ�����ȡ��Сֵ��
	//����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���r0=min(_A0,_B0), r1=min(_A1,_B1), r2=min(_A2,_B2), r3=min(_A3,_B3)
	extern __m128 _mm_min_ps(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ����������Ĵ���_A�ͼĴ���_B��Ͷ�Ӧλ�õ�32bit�����ȸ�����ȡ���ֵ��
	//����λ��ȡ�Ĵ���_A�е�����,����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���Ϊr=(max(_A0,_B0), _A1, _A2, _A3)
	extern __m128 _mm_max_ss(__m128 _A, __m128 _B);
	//����һ��__m128�ļĴ��������Ĵ���_A��_B�Ķ�Ӧλ�õ�32bit�����ȸ�����ȡ���ֵ��
	//����_A=(_A0,_A1,_A2,_A3), _B=(_B0,_B1,_B2,_B3),
	//�򷵻ؼĴ���r0=max(_A0,_B0), r1=max(_A1,_B1), r2=max(_A2,_B2), r3=max(_A3,_B3)
	extern __m128 _mm_max_ps(__m128 _A, __m128 _B);

*/

//SSEָ���׵�ַ������16�ֽڶ����   ���巽���ں���
/*
�ǵ�ַ���뻹�Ǵ�С���룿
��С��((size + (16 - 1)) & ~(16 - 1))
��ַ�Ƚ��鷳�������ʱ������16�ֽڣ�Ȼ��ѵ�ַ��������ķ�ʽ���롣
�Ҹ������ȷ�����Լ�ȷ���Ĵ𰸣�

�����̬���䣬��memalign����
�������ջ�ϣ���__attritube__((aligned(16)))


��Ȼ������C++����Ҫ������ֶ�����ҪһЩ��ͨ�İ취���ܱȽ����ŵ�ʵ�֣�
__m128 *p = new __m128[count];//�������__m128�Ǵ���������;��ʲô���ô������ڱ�����Ҳֻ�Ǹ����͡����������Ա�֤���롣
���ʵ���ܲ�������ֱ��new��������ΪĳЩԭ����ֱ��new��Ҳ����������İ취��
float * buf = new alignas(16) float [100];
__m128 *p= (__m128*) buf;
����һЩ����������֧��alignas�ؼ��֣�
float * buf = (float *)new std::align_storage<sizeof(__m128), 16>::type [100 * sizeof(float) / sizeof(__m128)];//ע����һ����Ҫ����ʹ�ó����������Ҳ�֪��������ʹ�ó�����������ʱֻ������
__m128 *p= (__m128*) buf;
�����϶��Ƕ����

����������Ҳ���������ͷ���˰���,��¥��һ�������ð�:
 
template<typename T,int alignByte=���ɶ�ֽڶ���>
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

����������ֱ�Ӵ��ݣ�

���������Զ�����Ƿ���룬��������Ӧ��ָ�����Ͳ������в�ͬ��ִ�У������ָ��Ч�ʻ��һЩ

���ٵ�ַ���������������ʹ�õ��� 
float *buf1 = reinterpret_cast<float*>(_aligned_malloc(sizeof(float)*dataLen, 32));

32�ֽڶ���

�ͷ�
_aligned_free(buf1);


*/





//
/*
immintrin.hΪAVXͷ�ļ�����ͷ�ļ������AESͷ�ļ�������__m256��__m256d��__m256i�Ķ���Ϊ��
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

