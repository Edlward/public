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
	 //SSE普通版
	 
	 float s = 0;    // 求和变量.  
     size_t i;  
     size_t nBlockWidth = 4; // 块宽. SSE寄存器能一次处理4个float.  
     size_t cntBlock = cntbuf / nBlockWidth;    // 块数.  
     size_t cntRem = cntbuf % nBlockWidth;   // 剩余数量.  
    __m128 xfsSum = _mm_setzero_ps();        // 求和变量。[SSE] 赋初值0  
    __m128 xfsLoad; // 加载.  
    const float* p = pbuf;            // SSE批量处理时所用的指针.  
    const float* q;                      // 将SSE变量上的多个数值合并时所用指针.  
  
    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
		//__m128 可以直接赋值吗？还是必须用_mm_load_ps函数？ 既然是一个数据结构，当然可以直接存取，只是_mm_load_ps更快一些。
		//要求p的地址是16字节对齐，否则读取的结果会出错
        xfsLoad=_mm_load_ps(p);   // [SSE] 加载  
        xfsSum=_mm_add_ps(xfsSum, xfsLoad);   // [SSE] 单精浮点紧缩加法  
        p += nBlockWidth;
    }  
    // 合并.  
    q = (const float*)&xfsSum;  
    s = q[0] + q[1] + q[2] + q[3];  
  
    // 处理剩下的.  
    for(i=0; i<cntRem; ++i)  
    {  
        s += p[i];  
    }  
    return s;  
	

	 /*
	//SSE四路循环展开版
	float s = 0;    // 返回值.  
    size_t i;  
    size_t nBlockWidth = 4*4;   // 块宽. SSE寄存器能一次处理4个float，然后循环展开4次.  
    size_t cntBlock = cntbuf / nBlockWidth; // 块数.  
    size_t cntRem = cntbuf % nBlockWidth;   // 剩余数量.  
    __m128 xfsSum = _mm_setzero_ps();   // 求和变量。[SSE] 赋初值0  
    __m128 xfsSum1 = _mm_setzero_ps();  
    __m128 xfsSum2 = _mm_setzero_ps();  
    __m128 xfsSum3 = _mm_setzero_ps();  
    __m128 xfsLoad; // 加载.  
    __m128 xfsLoad1;  
    __m128 xfsLoad2;  
    __m128 xfsLoad3;  
    const float* p = pbuf;  // SSE批量处理时所用的指针.  
    const float* q; // 将SSE变量上的多个数值合并时所用指针.  
  
    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  

        xfsLoad = _mm_load_ps(p);   // [SSE] 加载.  
        xfsLoad1 = _mm_load_ps(p+4);  
        xfsLoad2 = _mm_load_ps(p+8);  
        xfsLoad3 = _mm_load_ps(p+12);  
        xfsSum = _mm_add_ps(xfsSum, xfsLoad);   // [SSE] 单精浮点紧缩加法  
        xfsSum1 = _mm_add_ps(xfsSum1, xfsLoad1);  
        xfsSum2 = _mm_add_ps(xfsSum2, xfsLoad2);  
        xfsSum3 = _mm_add_ps(xfsSum3, xfsLoad3);  
        p += nBlockWidth;  
    }  
    // 合并.  
    xfsSum = _mm_add_ps(xfsSum, xfsSum1);       // 两两合并(0~1).  
    xfsSum2 = _mm_add_ps(xfsSum2, xfsSum3);   // 两两合并(2~3).  
    xfsSum = _mm_add_ps(xfsSum, xfsSum2);       // 两两合并(0~3).  
    q = (const float*)&xfsSum;  
    s = q[0] + q[1] + q[2] + q[3];  
  
    // 处理剩下的.  
    for(i=0; i<cntRem; ++i)  
    {  
        s += p[i];  
    }  
  
    return s;  
	*/
#else
	float s = 0;    // 求和变量.  
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
     const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
     size_t cntBlock =len/nBlockWidth;    // 块数.  
     size_t cntRem = len%nBlockWidth;   // 剩余数量.  
     const __m128* p1 = (__m128*)pbuf1;
     const __m128* p2 = (__m128*)pbuf2;
     __m128* pRet=(__m128*)pbufresult;
     // SSE批量处理.  
     for(i=0; i<cntBlock; ++i)  
     {  
         *pRet++=_mm_add_ps(*p1++,*p2++);
     }  
     // 处理剩下的. 
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
    const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    size_t cntBlock =len/nBlockWidth;    // 块数.  
    size_t cntRem = len%nBlockWidth;   // 剩余数量.  
    const __m128* p1 = (__m128*)pbuf1;
    const __m128* p2 = (__m128*)pbuf2;
    __m128* pRet=(__m128*)pbufresult;
    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm_sub_ps(*p1++,*p2++);
    }  
    // 处理剩下的. 
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
    //SSE普通版
     size_t i;  
     const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
     size_t cntBlock =len/nBlockWidth;    // 块数.  
     size_t cntRem = len%nBlockWidth;   // 剩余数量.  
    __m128 Ret;// = _mm_setzero_ps();        // 求和变量。[SSE] 赋初值0  
    __m128 LoadX; // 加载. 
    __m128 LoadY; // 加载.  
    //const float* p1 = pbuf1;            // SSE批量处理时所用的指针.  
    //const float* p2 = pbuf2;            // SSE批量处理时所用的指针.  
    //float* pRet=pbufresult;             // 将SSE变量上的多个数值合并时所用指针.  

    const __m128* p1 = (__m128*)pbuf1;
    const __m128* p2 = (__m128*)pbuf2;
     __m128* pRet=(__m128*)pbufresult;

    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
  //      LoadX = _mm_load_ps(p1);   // [SSE] 加载  
  //      LoadY = _mm_load_ps(p2);   // [SSE] 加载  
  //      Ret = _mm_mul_ps(LoadX,LoadY);   // [SSE] 单精浮点紧缩加法  
  //      _mm_store_ps(pRet,Ret);   
  //      p1+= nBlockWidth;  
  //      p2+= nBlockWidth;  
  //      pRet+= nBlockWidth; 
      
      *pRet++=_mm_mul_ps(*p1++,*p2++);  //速度没有提高

    }  

    // 处理剩下的.  
    // 处理剩下的. 
    const float* Remp1 = (float*)p1;            // 
    const float* Remp2 = (float*)p2;            //
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]*Remp2[i];
    }  


#endif

    ////SSE--4路并行加速版--经测试--更慢 没有意义
    //const size_t N=2;   //2 路 4路并行
    //size_t i;  
    //const size_t nBlockWidth =4;// sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    //size_t cntBlock =len/(nBlockWidth*N);    // 块数.  
    //size_t cntRem = len%(nBlockWidth*N);   // 剩余数量.  
    //__m128 xfsRet[N];// = _mm_setzero_ps();        // 求和变量。[SSE] 赋初值0  
    //__m128 xfsLoadX[N]; // 加载. 
    //__m128 xfsLoadY[N]; // 加载.  

    //const float* pX = pbuf1;            // SSE批量处理时所用的指针.  
    //const float* pY = pbuf2;            // SSE批量处理时所用的指针.  
    //float* pRet=pbufresult;             // 将SSE变量上的多个数值合并时所用指针.  

    //// SSE批量处理.  
    //for(i=0; i<cntBlock; ++i)  
    //{  
    //    xfsLoadX[0] = _mm_load_ps(pX);   // [SSE] 加载 
    //    xfsLoadX[1] = _mm_load_ps(pX+1*nBlockWidth);   // [SSE] 加载 
    //    //xfsLoadX[2] = _mm_load_ps(pX+2*nBlockWidth);   // [SSE] 加载 
    //    //xfsLoadX[3] = _mm_load_ps(pX+3*nBlockWidth);   // [SSE] 加载 

    //    xfsLoadY[0] = _mm_load_ps(pY);   // [SSE] 加载  
    //    xfsLoadY[1] = _mm_load_ps(pY+1*nBlockWidth);   // [SSE] 加载  
    //    //xfsLoadY[2] = _mm_load_ps(pY+2*nBlockWidth);   // [SSE] 加载  
    //    //xfsLoadY[3] = _mm_load_ps(pY+3*nBlockWidth);   // [SSE] 加载  

    //    xfsRet[0] = _mm_mul_ps(xfsLoadX[0],xfsLoadY[0]);   // [SSE] 单精浮点紧缩加法  
    //    xfsRet[1] = _mm_mul_ps(xfsLoadX[1],xfsLoadY[1]);   // [SSE] 单精浮点紧缩加法  
    //    //xfsRet[2] = _mm_mul_ps(xfsLoadX[2],xfsLoadY[2]);   // [SSE] 单精浮点紧缩加法  
    //    //xfsRet[3] = _mm_mul_ps(xfsLoadX[3],xfsLoadY[3]);   // [SSE] 单精浮点紧缩加法  

    //    _mm_store_ps(&pRet[0],xfsRet[0]);
    //    _mm_store_ps(&pRet[1*nBlockWidth],xfsRet[1]);
    //    //_mm_store_ps(&pRet[2*nBlockWidth],xfsRet[2]);
    //    //_mm_store_ps(&pRet[3*nBlockWidth],xfsRet[3]);

    //    pX+= N*nBlockWidth;  
    //    pY+= N*nBlockWidth;  
    //    pRet+= N*nBlockWidth; 
    //}  

    //// 处理剩下的.  
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
    const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    size_t cntBlock =len/nBlockWidth;    // 块数.  
    size_t cntRem = len%nBlockWidth;   // 剩余数量.  
    const __m128* p1 = (__m128*)pbuf1;
    const __m128* p2 = (__m128*)pbuf2;
    __m128* pRet=(__m128*)pbufresult;
    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm_div_ps(*p1++,*p2++);
    }  
    // 处理剩下的. 
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
    const size_t nBlockWidth =4;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    size_t cntBlock =len/nBlockWidth;    // 块数.  
    size_t cntRem = len%nBlockWidth;   // 剩余数量.  
    const __m128* p1 = (__m128*)pbuf1;
    __m128* pRet=(__m128*)pbufresult;
    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm_sqrt_ps(*p1++);
    }  
    // 处理剩下的. 
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
    float s = 0;    // 求和变量.  
    size_t i;  
    const size_t nBlockWidth = 4; // 块宽. SSE寄存器能一次处理4个float.  
    size_t cntBlock =len/ nBlockWidth;    // 块数.  
    size_t cntRem = len% nBlockWidth;   // 剩余数量.  
    __m128 xfsSum = _mm_setzero_ps();        // 求和变量。[SSE] 赋初值0  
    __m128 xfsLoad; 
    const float* p = pbuf;            // SSE批量处理时所用的指针.  
    const float* q;                      // 将SSE变量上的多个数值合并时所用指针.  

    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
        //__m128 可以直接赋值吗？还是必须用_mm_load_ps函数？ 既然是一个数据结构，当然可以直接存取，只是_mm_load_ps更快一些。
        //要求p的地址是16字节对齐，否则读取的结果会出错
        xfsLoad=_mm_load_ps(p);   // [SSE] 加载  
        xfsSum=_mm_add_ps(xfsSum, xfsLoad);   // [SSE] 单精浮点紧缩加法  
        p += nBlockWidth;
    }  
    // 合并.  
    q = (const float*)&xfsSum;  
    s = q[0] + q[1] + q[2] + q[3];  

    // 处理剩下的.  
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
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    size_t cntBlock=len/nBlockWidth;   //块数.  
    size_t cntRem=len%nBlockWidth;   // 剩余数量.  
    const __m256* p1 = (__m256*)pbuf1;            // 
    __m256* pRet=(__m256*)pbufresult;             //

    __m256 g;
    for(i=0; i<8; ++i)  
    {  
       g.m256_f32[i]=gain;
    }  

    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm256_add_ps(*p1++,g);  
    }  

    // 处理剩下的. 
    const float* Remp1 = (float*)p1;            // 
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]*gain;
    }  
}

void AVX_floatAdd(float* pbufresult,const float* pbuf1, const float* pbuf2,unsigned int len)
{
    //AVX普通版
    size_t i;  
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    size_t cntBlock=len/nBlockWidth;   //块数.  
    size_t cntRem=len%nBlockWidth;   // 剩余数量.  

    const __m256* p1 = (__m256*)pbuf1;            // 
    const __m256* p2 = (__m256*)pbuf2;            //
    __m256* pRet=(__m256*)pbufresult;             //

    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm256_add_ps(*p1++,*p2++);  
    }  

    // 处理剩下的. 
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
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    size_t cntBlock=len/nBlockWidth;   //块数.  
    size_t cntRem=len%nBlockWidth;   // 剩余数量.  

    const __m256* p1 = (__m256*)pbuf1;            // 
    const __m256* p2 = (__m256*)pbuf2;            //
    __m256* pRet=(__m256*)pbufresult;             //

    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm256_sub_ps(*p1++,*p2++);  
    }  

    // 处理剩下的. 
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
    //AVX普通版
    size_t i;  
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    size_t cntBlock=len/nBlockWidth;    // 块数.  
    size_t cntRem=len%nBlockWidth;   // 剩余数量.  
  
    //const float* p1 = pbuf1;            // 
    //const float* p2 = pbuf2;            //
    //float* pRet=pbufresult;             //

    const __m256* p1 = (__m256*)pbuf1;            // 
    const __m256* p2 = (__m256*)pbuf2;            //
    __m256* pRet=(__m256*)pbufresult;             //

    __m256 Ret;// = _mm_setzero_ps();//
    __m256 LoadX;
    __m256 LoadY;


    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
        //LoadX=_mm256_load_ps(p1);    //     主要瓶颈一部分在load 和store
        //LoadY = _mm256_load_ps(p2);  //
        ////__m256& Ret=*(__m256*)pRet;   //可以达到_mm256_store_ps一样的效果，但是速度没有增加     
        //Ret = _mm256_mul_ps(LoadX,LoadY);     
        //_mm256_store_ps(pRet,Ret);         //store 回写内存更耗时
        //p1+=nBlockWidth;  
        //p2+=nBlockWidth;  
        //pRet+= nBlockWidth;


        ////_mm256_store_ps(pRet,_mm256_mul_ps(_mm256_load_ps(p1),_mm256_load_ps(p2)));  //速度没有提高
        *pRet++=_mm256_mul_ps(*p1++,*p2++);  //速度没有提高
    }  

    // 处理剩下的. 
    const float* Remp1 = (float*)p1;            // 
    const float* Remp2 = (float*)p2;            //
    float* RempRet= (float*)pRet;                //
    for(i=0; i<cntRem; ++i)  
    {  
        RempRet[i]=Remp1[i]*Remp2[i];
    }  
#endif


    //AVX 双路并行效果更慢
    //size_t i;  
    //const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    //const size_t block=nBlockWidth*2;
    //
    //size_t cntBlock=len/(block);    // 块数.  
    //size_t cntRem=len%(block);   // 剩余数量.  

    //const float* p1 = pbuf1;            // 
    //const float* p2 = pbuf2;            //
    //float* pRet=pbufresult;             //

    //__m256 Ret[2];// = _mm_setzero_ps();//
    //__m256 LoadX[2];
    //__m256 LoadY[2];

    //// SSE批量处理.  
    //for(i=0; i<cntBlock; ++i)  
    //{  
    //    LoadX[0]=_mm256_load_ps(p1);    //     主要瓶颈一部分在load 和store
    //    LoadX[1]=_mm256_load_ps(p1+nBlockWidth);    //     主要瓶颈一部分在load 和store    
    //    
    //    LoadY[0] = _mm256_load_ps(p2);  //
    //    LoadY[1] = _mm256_load_ps(p2+nBlockWidth);  //

    //    //__m256& Ret=*(__m256*)pRet;   //可以达到_mm256_store_ps一样的效果，但是速度没有增加
    //           
    //    Ret[0] = _mm256_mul_ps(LoadX[0],LoadY[0]);   
    //    Ret[1] = _mm256_mul_ps(LoadX[1],LoadY[1]);   

    // 
    //    _mm256_store_ps(pRet,Ret[0]);         //store 回写内存更耗时
    //    _mm256_store_ps(pRet+nBlockWidth,Ret[1]);         //store 回写内存更耗时

    //    p1+=block;  
    //    p2+=block;  
    //    pRet+=block; 
    //}  

    //// 处理剩下的.  
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
    const size_t nBlockWidth =8;//sizeof(__m128)/sizeof(float);//4; // 块宽. SSE寄存器能一次处理4个float.  
    size_t cntBlock=len/nBlockWidth;   //块数.  
    size_t cntRem=len%nBlockWidth;   // 剩余数量.  

    const __m256* p1 = (__m256*)pbuf1;            // 
    const __m256* p2 = (__m256*)pbuf2;            //
    __m256* pRet=(__m256*)pbufresult;             //

    // SSE批量处理.  
    for(i=0; i<cntBlock; ++i)  
    {  
        *pRet++=_mm256_div_ps(*p1++,*p2++);  
    }  

    // 处理剩下的. 
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




