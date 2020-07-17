#include "SIMD.h"
#include<stdio.h>
#include<iostream>
#include "SuperTime.h"

#define ENABLE_SIMD


#ifdef ENABLE_SIMD
	#ifdef _WIN32
		//Enable SSE with Visual C++
		#include <intrin.h>  //intrin.h  xmmintrin.h
	#else
    #endif
#endif

//#pragma pack(push, 16)

 float SSE_floatSum(const float* pbuf,unsigned int cntbuf)
 {
#ifdef ENABLE_SIMD
	 //SSE��ͨ��
	 
	 float s = 0;    // ��ͱ���.  
     size_t i;  
     size_t nBlockWidth = 4; // ���. SSE�Ĵ�����һ�δ���4��float.  
     size_t cntBlock = cntbuf / nBlockWidth;    // ����.  
     size_t cntRem = cntbuf % nBlockWidth;   // ʣ������.  
    __m128 xfsSum = _mm_setzero_ps();        // ��ͱ�����[SSE] ����ֵ0  
    __m128 xfsLoad; // ����.  
    const float* p = pbuf;            // SSE��������ʱ���õ�ָ��.  
    const float* q;                      // ��SSE�����ϵĶ����ֵ�ϲ�ʱ����ָ��.  
  
    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
		//__m128 ����ֱ�Ӹ�ֵ�𣿻��Ǳ�����_mm_load_ps������ ��Ȼ��һ�����ݽṹ����Ȼ����ֱ�Ӵ�ȡ��ֻ��_mm_load_ps����һЩ��
		//Ҫ��p�ĵ�ַ��16�ֽڶ��룬�����ȡ�Ľ�������
        xfsLoad=_mm_load_ps(p);   // [SSE] ����  
        xfsSum=_mm_add_ps(xfsSum, xfsLoad);   // [SSE] ������������ӷ�  
        p += nBlockWidth;
    }  
    // �ϲ�.  
    q = (const float*)&xfsSum;  
    s = q[0] + q[1] + q[2] + q[3];  
  
    // ����ʣ�µ�.  
    for(i=0; i<cntRem; ++i)  
    {  
        s += p[i];  
    }  
    return s;  
	

	 /*
	//SSE��·ѭ��չ����
	float s = 0;    // ����ֵ.  
    size_t i;  
    size_t nBlockWidth = 4*4;   // ���. SSE�Ĵ�����һ�δ���4��float��Ȼ��ѭ��չ��4��.  
    size_t cntBlock = cntbuf / nBlockWidth; // ����.  
    size_t cntRem = cntbuf % nBlockWidth;   // ʣ������.  
    __m128 xfsSum = _mm_setzero_ps();   // ��ͱ�����[SSE] ����ֵ0  
    __m128 xfsSum1 = _mm_setzero_ps();  
    __m128 xfsSum2 = _mm_setzero_ps();  
    __m128 xfsSum3 = _mm_setzero_ps();  
    __m128 xfsLoad; // ����.  
    __m128 xfsLoad1;  
    __m128 xfsLoad2;  
    __m128 xfsLoad3;  
    const float* p = pbuf;  // SSE��������ʱ���õ�ָ��.  
    const float* q; // ��SSE�����ϵĶ����ֵ�ϲ�ʱ����ָ��.  
  
    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  

        xfsLoad = _mm_load_ps(p);   // [SSE] ����.  
        xfsLoad1 = _mm_load_ps(p+4);  
        xfsLoad2 = _mm_load_ps(p+8);  
        xfsLoad3 = _mm_load_ps(p+12);  
        xfsSum = _mm_add_ps(xfsSum, xfsLoad);   // [SSE] ������������ӷ�  
        xfsSum1 = _mm_add_ps(xfsSum1, xfsLoad1);  
        xfsSum2 = _mm_add_ps(xfsSum2, xfsLoad2);  
        xfsSum3 = _mm_add_ps(xfsSum3, xfsLoad3);  
        p += nBlockWidth;  
    }  
    // �ϲ�.  
    xfsSum = _mm_add_ps(xfsSum, xfsSum1);       // �����ϲ�(0~1).  
    xfsSum2 = _mm_add_ps(xfsSum2, xfsSum3);   // �����ϲ�(2~3).  
    xfsSum = _mm_add_ps(xfsSum, xfsSum2);       // �����ϲ�(0~3).  
    q = (const float*)&xfsSum;  
    s = q[0] + q[1] + q[2] + q[3];  
  
    // ����ʣ�µ�.  
    for(i=0; i<cntRem; ++i)  
    {  
        s += p[i];  
    }  
  
    return s;  
	*/
#else
	float s = 0;    // ��ͱ���.  
    size_t i;  
    for(i=0; i<cntbuf; ++i)  
    {  
		//std::cout<<"s="<<s<<std::endl;
        s+= pbuf[i];  
    }  
    return s;  
#endif
	return 0;
 }

 void SSE_floatAdd(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len)
 {
     size_t i;  
     const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
     size_t cntBlock =len/nBlockWidth;    // ����.  
     size_t cntRem = len%nBlockWidth;   // ʣ������.  
     const __m128* p1 = (__m128*)pbuf1;
     const __m128* p2 = (__m128*)pbuf2;
     __m128* pRet=(__m128*)pbufresult;
     // SSE��������.  
     for(i=0; i<cntBlock; ++i)  
     {  
         *pRet++=_mm_add_ps(*p1++,*p2++);
     }  
     // ����ʣ�µ�. 
     const float* Remp1 = (float*)p1;            // 
     const float* Remp2 = (float*)p2;            //
     float* RempRet= (float*)pRet;                //
     for(i=0; i<cntRem; ++i)  
     {  
         RempRet[i]=Remp1[i]+Remp2[i];
     }  
 }

void SSE_floatSub(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len)
{
    size_t i;  
    const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    size_t cntBlock =len/nBlockWidth;    // ����.  
    size_t cntRem = len%nBlockWidth;   // ʣ������.  
    const __m128* p1 = (__m128*)pbuf1;
    const __m128* p2 = (__m128*)pbuf2;
    __m128* pRet=(__m128*)pbufresult;
    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm_sub_ps(*p1++,*p2++);
    }  
    // ����ʣ�µ�. 
    const float* Remp1 = (float*)p1;            // 
    const float* Remp2 = (float*)p2;            //
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]-Remp2[i];
    }  
}


void SSE_floatMuli(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len)
{
#ifdef ENABLE_SIMD
#if 1
    //SSE��ͨ��
     size_t i;  
     const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
     size_t cntBlock =len/nBlockWidth;    // ����.  
     size_t cntRem = len%nBlockWidth;   // ʣ������.  
    __m128 Ret;// = _mm_setzero_ps();        // ��ͱ�����[SSE] ����ֵ0  
    __m128 LoadX; // ����. 
    __m128 LoadY; // ����.  
    //const float* p1 = pbuf1;            // SSE��������ʱ���õ�ָ��.  
    //const float* p2 = pbuf2;            // SSE��������ʱ���õ�ָ��.  
    //float* pRet=pbufresult;             // ��SSE�����ϵĶ����ֵ�ϲ�ʱ����ָ��.  

    const __m128* p1 = (__m128*)pbuf1;
    const __m128* p2 = (__m128*)pbuf2;
     __m128* pRet=(__m128*)pbufresult;

    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
  //      LoadX = _mm_load_ps(p1);   // [SSE] ����  
  //      LoadY = _mm_load_ps(p2);   // [SSE] ����  
  //      Ret = _mm_mul_ps(LoadX,LoadY);   // [SSE] ������������ӷ�  
  //      _mm_store_ps(pRet,Ret);   
  //      p1+= nBlockWidth;  
  //      p2+= nBlockWidth;  
  //      pRet+= nBlockWidth; 
      
      *pRet++=_mm_mul_ps(*p1++,*p2++);  //�ٶ�û�����

    }  

    // ����ʣ�µ�.  
    // ����ʣ�µ�. 
    const float* Remp1 = (float*)p1;            // 
    const float* Remp2 = (float*)p2;            //
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]*Remp2[i];
    }  


#endif

    ////SSE--4·���м��ٰ�--������--���� û������
    //const size_t N=2;   //2 · 4·����
    //size_t i;  
    //const size_t nBlockWidth =4;// sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    //size_t cntBlock =len/(nBlockWidth*N);    // ����.  
    //size_t cntRem = len%(nBlockWidth*N);   // ʣ������.  
    //__m128 xfsRet[N];// = _mm_setzero_ps();        // ��ͱ�����[SSE] ����ֵ0  
    //__m128 xfsLoadX[N]; // ����. 
    //__m128 xfsLoadY[N]; // ����.  

    //const float* pX = pbuf1;            // SSE��������ʱ���õ�ָ��.  
    //const float* pY = pbuf2;            // SSE��������ʱ���õ�ָ��.  
    //float* pRet=pbufresult;             // ��SSE�����ϵĶ����ֵ�ϲ�ʱ����ָ��.  

    //// SSE��������.  
    //for(i=0; i<cntBlock; ++i)  
    //{  
    //    xfsLoadX[0] = _mm_load_ps(pX);   // [SSE] ���� 
    //    xfsLoadX[1] = _mm_load_ps(pX+1*nBlockWidth);   // [SSE] ���� 
    //    //xfsLoadX[2] = _mm_load_ps(pX+2*nBlockWidth);   // [SSE] ���� 
    //    //xfsLoadX[3] = _mm_load_ps(pX+3*nBlockWidth);   // [SSE] ���� 

    //    xfsLoadY[0] = _mm_load_ps(pY);   // [SSE] ����  
    //    xfsLoadY[1] = _mm_load_ps(pY+1*nBlockWidth);   // [SSE] ����  
    //    //xfsLoadY[2] = _mm_load_ps(pY+2*nBlockWidth);   // [SSE] ����  
    //    //xfsLoadY[3] = _mm_load_ps(pY+3*nBlockWidth);   // [SSE] ����  

    //    xfsRet[0] = _mm_mul_ps(xfsLoadX[0],xfsLoadY[0]);   // [SSE] ������������ӷ�  
    //    xfsRet[1] = _mm_mul_ps(xfsLoadX[1],xfsLoadY[1]);   // [SSE] ������������ӷ�  
    //    //xfsRet[2] = _mm_mul_ps(xfsLoadX[2],xfsLoadY[2]);   // [SSE] ������������ӷ�  
    //    //xfsRet[3] = _mm_mul_ps(xfsLoadX[3],xfsLoadY[3]);   // [SSE] ������������ӷ�  

    //    _mm_store_ps(&pRet[0],xfsRet[0]);
    //    _mm_store_ps(&pRet[1*nBlockWidth],xfsRet[1]);
    //    //_mm_store_ps(&pRet[2*nBlockWidth],xfsRet[2]);
    //    //_mm_store_ps(&pRet[3*nBlockWidth],xfsRet[3]);

    //    pX+= N*nBlockWidth;  
    //    pY+= N*nBlockWidth;  
    //    pRet+= N*nBlockWidth; 
    //}  

    //// ����ʣ�µ�.  
    //for(i=0; i<cntRem; ++i)  
    //{  
    //    pRet[i]=pX[i]*pY[i];
    //}  


#else
	size_t i;  
    for(i=0; i<len; ++i)  
    {  
        pbufresult[i]= pbuf1[i]*pbuf2[i];  
    }  
#endif
}

void SSE_floatDiv(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len)
{
    size_t i;  
    const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    size_t cntBlock =len/nBlockWidth;    // ����.  
    size_t cntRem = len%nBlockWidth;   // ʣ������.  
    const __m128* p1 = (__m128*)pbuf1;
    const __m128* p2 = (__m128*)pbuf2;
    __m128* pRet=(__m128*)pbufresult;
    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm_div_ps(*p1++,*p2++);
    }  
    // ����ʣ�µ�. 
    const float* Remp1 = (float*)p1;            // 
    const float* Remp2 = (float*)p2;            //
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]/Remp2[i];
    }  
}

void SSE_floatSqrt(float* pbufresult,const float* pbuf1,unsigned int len)
{
    size_t i;  
    const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    size_t cntBlock =len/nBlockWidth;    // ����.  
    size_t cntRem = len%nBlockWidth;   // ʣ������.  
    const __m128* p1 = (__m128*)pbuf1;
    __m128* pRet=(__m128*)pbufresult;
    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm_sqrt_ps(*p1++);
    }  
    // ����ʣ�µ�. 
    const float* Remp1 = (float*)p1;            // 
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=sqrt(Remp1[i]);
    }  
}




float AVX_floatSum(const float* pbuf,unsigned int len)
{
#ifdef ENABLE_SIMD
    float s = 0;    // ��ͱ���.  
    size_t i;  
    const size_t nBlockWidth = 4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    size_t cntBlock =len/ nBlockWidth;    // ����.  
    size_t cntRem = len% nBlockWidth;   // ʣ������.  
    __m128 xfsSum = _mm_setzero_ps();        // ��ͱ�����[SSE] ����ֵ0  
    __m128 xfsLoad; 
    const float* p = pbuf;            // SSE��������ʱ���õ�ָ��.  
    const float* q;                      // ��SSE�����ϵĶ����ֵ�ϲ�ʱ����ָ��.  

    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
        //__m128 ����ֱ�Ӹ�ֵ�𣿻��Ǳ�����_mm_load_ps������ ��Ȼ��һ�����ݽṹ����Ȼ����ֱ�Ӵ�ȡ��ֻ��_mm_load_ps����һЩ��
        //Ҫ��p�ĵ�ַ��16�ֽڶ��룬�����ȡ�Ľ�������
        xfsLoad=_mm_load_ps(p);   // [SSE] ����  
        xfsSum=_mm_add_ps(xfsSum, xfsLoad);   // [SSE] ������������ӷ�  
        p += nBlockWidth;
    }  
    // �ϲ�.  
    q = (const float*)&xfsSum;  
    s = q[0] + q[1] + q[2] + q[3];  

    // ����ʣ�µ�.  
    for(i=0; i<cntRem; ++i)  
    {  
        s += p[i];  
    }  
    return s;  
#endif
}

void AVX_floatMuligain(float* pbufresult,const float* pbuf1,const float gain,unsigned int len)
{
    size_t i;  
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    size_t cntBlock=len/nBlockWidth;   //����.  
    size_t cntRem=len%nBlockWidth;   // ʣ������.  
    const __m256* p1 = (__m256*)pbuf1;            // 
    __m256* pRet=(__m256*)pbufresult;             //

    __m256 g;
    for(i=0; i<8; ++i)  
    {  
       g.m256_f32[i]=gain;
    }  

    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm256_add_ps(*p1++,g);  
    }  

    // ����ʣ�µ�. 
    const float* Remp1 = (float*)p1;            // 
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]*gain;
    }  
}

void AVX_floatAdd(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len)
{
    //AVX��ͨ��
    size_t i;  
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    size_t cntBlock=len/nBlockWidth;   //����.  
    size_t cntRem=len%nBlockWidth;   // ʣ������.  

    const __m256* p1 = (__m256*)pbuf1;            // 
    const __m256* p2 = (__m256*)pbuf2;            //
    __m256* pRet=(__m256*)pbufresult;             //

    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm256_add_ps(*p1++,*p2++);  
    }  

    // ����ʣ�µ�. 
    const float* Remp1 = (float*)p1;            // 
    const float* Remp2 = (float*)p2;            //
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]+Remp2[i];
    }  
}
void AVX_floatSub(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len)
{
    size_t i;  
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    size_t cntBlock=len/nBlockWidth;   //����.  
    size_t cntRem=len%nBlockWidth;   // ʣ������.  

    const __m256* p1 = (__m256*)pbuf1;            // 
    const __m256* p2 = (__m256*)pbuf2;            //
    __m256* pRet=(__m256*)pbufresult;             //

    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm256_sub_ps(*p1++,*p2++);  
    }  

    // ����ʣ�µ�. 
    const float* Remp1 = (float*)p1;            // 
    const float* Remp2 = (float*)p2;            //
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]-Remp2[i];
    }  
}
void AVX_floatMuli(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len)
{
    //__m256 p1,p2,ret;
    //for(int i=0;i<1000000;i++)
    //{
    //    //sum+=SSE_floatSum(s.buff1,BUFFER_SIZE);
    //    //AVX_floatMuli(s.buffresult,s.buff1,s.buff2,BUFFER_SIZE-3);
    //    p1=_mm256_load_ps(&s.buff1[0]);
    //    p2=_mm256_load_ps(&s.buff2[0]);
    //    ret=_mm256_mul_ps(p1,p2);
    //    _mm256_store_ps(&s.buffresult[0],ret);
    //}


#ifdef ENABLE_SIMD

   // const int BUFFER_SIZE=3200;
   // __m256 Ret[BUFFER_SIZE];           //=_mm_setzero_ps();//
   // __m256 LoadX[BUFFER_SIZE];
   // __m256 LoadY[BUFFER_SIZE];

   // //memcpy(&LoadX[0],pbuf1,len);
   // //memcpy(&LoadY[0],pbuf2,len);

   // unsigned int i;
   // for(i=0; i<BUFFER_SIZE; ++i)  
   // {  
   //     Ret[i] = _mm256_mul_ps(LoadX[i],LoadY[i]);     
   // }  

   // //*pbufresult=*(float*)&Ret[i-1];
   //// memcpy(pbufresult,&Ret[0],len);


#if 1
    //AVX��ͨ��
    size_t i;  
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    size_t cntBlock=len/nBlockWidth;    // ����.  
    size_t cntRem=len%nBlockWidth;   // ʣ������.  
  
    //const float* p1 = pbuf1;            // 
    //const float* p2 = pbuf2;            //
    //float* pRet=pbufresult;             //

    const __m256* p1 = (__m256*)pbuf1;            // 
    const __m256* p2 = (__m256*)pbuf2;            //
    __m256* pRet=(__m256*)pbufresult;             //

    __m256 Ret;// = _mm_setzero_ps();//
    __m256 LoadX;
    __m256 LoadY;


    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
        //LoadX=_mm256_load_ps(p1);    //     ��Ҫƿ��һ������load ��store
        //LoadY = _mm256_load_ps(p2);  //
        ////__m256& Ret=*(__m256*)pRet;   //���Դﵽ_mm256_store_psһ����Ч���������ٶ�û������     
        //Ret = _mm256_mul_ps(LoadX,LoadY);     
        //_mm256_store_ps(pRet,Ret);         //store ��д�ڴ����ʱ
        //p1+=nBlockWidth;  
        //p2+=nBlockWidth;  
        //pRet+= nBlockWidth;


        ////_mm256_store_ps(pRet,_mm256_mul_ps(_mm256_load_ps(p1),_mm256_load_ps(p2)));  //�ٶ�û�����
        *pRet++=_mm256_mul_ps(*p1++,*p2++);  //�ٶ�û�����
    }  

    // ����ʣ�µ�. 
    const float* Remp1 = (float*)p1;            // 
    const float* Remp2 = (float*)p2;            //
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]*Remp2[i];
    }  
#endif


    //AVX ˫·����Ч������
    //size_t i;  
    //const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    //const size_t block=nBlockWidth*2;
    //
    //size_t cntBlock=len/(block);    // ����.  
    //size_t cntRem=len%(block);   // ʣ������.  

    //const float* p1 = pbuf1;            // 
    //const float* p2 = pbuf2;            //
    //float* pRet=pbufresult;             //

    //__m256 Ret[2];// = _mm_setzero_ps();//
    //__m256 LoadX[2];
    //__m256 LoadY[2];

    //// SSE��������.  
    //for(i=0; i<cntBlock; ++i)  
    //{  
    //    LoadX[0]=_mm256_load_ps(p1);    //     ��Ҫƿ��һ������load ��store
    //    LoadX[1]=_mm256_load_ps(p1+nBlockWidth);    //     ��Ҫƿ��һ������load ��store    
    //    
    //    LoadY[0] = _mm256_load_ps(p2);  //
    //    LoadY[1] = _mm256_load_ps(p2+nBlockWidth);  //

    //    //__m256& Ret=*(__m256*)pRet;   //���Դﵽ_mm256_store_psһ����Ч���������ٶ�û������
    //           
    //    Ret[0] = _mm256_mul_ps(LoadX[0],LoadY[0]);   
    //    Ret[1] = _mm256_mul_ps(LoadX[1],LoadY[1]);   

    // 
    //    _mm256_store_ps(pRet,Ret[0]);         //store ��д�ڴ����ʱ
    //    _mm256_store_ps(pRet+nBlockWidth,Ret[1]);         //store ��д�ڴ����ʱ

    //    p1+=block;  
    //    p2+=block;  
    //    pRet+=block; 
    //}  

    //// ����ʣ�µ�.  
    //for(i=0; i<cntRem; ++i)  
    //{  
    //    pRet[i]=p1[i]*p2[i];
    //}  

#else
size_t i;  
for(i=0; i<len; ++i)  
{  
    pbufresult[i]= pbuf1[i]*pbuf2[i];  
}  
#endif
}

void AVX_floatDiv(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len)
{
    size_t i;  
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // ���. SSE�Ĵ�����һ�δ���4��float.  
    size_t cntBlock=len/nBlockWidth;   //����.  
    size_t cntRem=len%nBlockWidth;   // ʣ������.  

    const __m256* p1 = (__m256*)pbuf1;            // 
    const __m256* p2 = (__m256*)pbuf2;            //
    __m256* pRet=(__m256*)pbufresult;             //

    // SSE��������.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm256_div_ps(*p1++,*p2++);  
    }  

    // ����ʣ�µ�. 
    const float* Remp1 = (float*)p1;            // 
    const float* Remp2 = (float*)p2;            //
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]/Remp2[i];
    } 
}



void AVX_floatSqrt(float* pbufresult,const float* pbuf1,unsigned int len)
{

}



#ifdef UNIT_TEST

USING_NAME_SPACE_MAIN
 int Test_SIMD()
 {
     std::cout<<"Test_SIMD()"<<std::endl;

//#pragma pack(push, 16)
//	 const int BUFFER_SIZE=100*16;
//	 float buffresult[BUFFER_SIZE];
//	 float buff1[BUFFER_SIZE];
//	 //float buff1= new alignas(16) float [BUFFER_SIZE];
//	 float buff2[BUFFER_SIZE];
//  #pragma pack(pop)
	  const int BUFFER_SIZE=2000;
	  typedef  __declspec(align(32)) struct
		{
			 float buffresult[BUFFER_SIZE];
			 float buff1[BUFFER_SIZE];
			 float buff2[BUFFER_SIZE];
		}S;
	    S s;

	 for(int i=0;i<BUFFER_SIZE;i++)
	 {
	     s.buff1[i]=5;
	     s.buff2[i]=3;
         s.buffresult[i]=0;
	 }
	 SuperTime time("SSE_floatSum");
	 float sum;


	 time.getBeginTime();

     __m256 p1,p2,ret;
	for(int i=0;i<100000;i++)
	 {
		//sum+=SSE_floatSum(s.buff1,BUFFER_SIZE);
		SSE_floatDiv(s.buffresult,s.buff1,s.buff2,BUFFER_SIZE-3);
	 }


	 time.getEndPrint();

	 //printf("%f ",sum);
	 //std::cout<<"sum="<<sum<<std::endl;

	 for(int i=0;i<BUFFER_SIZE;++i)
	 {
		 printf("%.6f ",s.buffresult[i]);
	 }
	 return 0;
 }

#endif




