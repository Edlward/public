#include "PerformanceTest.h"
#include "SuperTime.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "BasicOperation.h"     //自定义的基础操作类

#include <memory.h>
#include <vector>



NAME_SPACE_MAIN_BEGIN

#ifdef UNIT_TEST

 int Test_PerformanceTest()
 {
	 /*
	 int count = 0;
	 std::vector<void*> Vpoint;
	 while(1)   
	 {
		 void* p=malloc(1<<20);   //1<<20=1M
         if(p==NULL)
		 {
			 break;
		 }
		 Vpoint.push_back(p);
		 count++;
	 }

	 for (size_t i=0;i<Vpoint.size();i++)
	 {
		 free(Vpoint.at(i));
	 }

	 printf("当前最大可用内存%dMB\n",count);
	 */
	 //return 0;

	 //们常用的pc机都是x86系列的，32、64位都是小端的，intel、amd的pc芯片，都一样。目前我们用的win系统桌面基本都是这种x86架构，暂时还没有其它的大端的。
	 int EndianTest = 1;                          //大小端是CPU相关还是操作系统相关, CPU相关         
	 char *p = (char *)&EndianTest;       //arm的cpu是可以设置大小端，由操作系统自己选
	 if(*p == 1)                                      //英特尔普遍是小端0x12345678在内存中放的顺序是0x78,0x56,0x34,0x12
	 {                 
		 printf("当前系统为小端字节序\n");
	 }
	 else
	 {
		 printf("当前系统为大端字节序\n");
	 }
	 print_hex(&EndianTest,sizeof(EndianTest));
	 //return 0;

	 const int BUFFERBIG_SIZE=1024*1024*100;
	 char* bufbig1=new char[BUFFERBIG_SIZE];
	 char* bufbig2=new char[BUFFERBIG_SIZE];

	 //const int BUFFER_SIZE=1024*10;
	 //char buf1[BUFFER_SIZE];
	 //char buf2[BUFFER_SIZE];
	 typedef unsigned int TypeTest;  
	 const unsigned int loopNum=1024*1024*10;
	 TypeTest a=39.99;
	 TypeTest b=20.88;
	 TypeTest ret=2199;
	 //各种基本类型数据相乘效率相对值，越高越快 ret*=b;
	 //float 53 double 53 int 236 unsigned int 151 uint64_t 132
	 //除法效率ret/=b;
	 //float 27 double 27 int 34 unsigned int 43 int64_t 9.2 uint64_t 9.1 //int64_t比浮点还慢？

	 SuperTime time("CpuTest()");
	 for (int K=0;K<32;K++)
	 {
		 time.getBeginTime();
		 memcpy(bufbig1,bufbig2,100*1024*1024);
		 time.getEndPrint(10);
	 }
	 printf("memcpy 100MB avrgTime=%lf ms avrgFrameRate=%lf Hz\n",time.getAvrgTime()/1000,time.getAvrgRate());

	 time.reSet();

	 std::cout<<" result= "<<ret<<std::endl;
	 //print_hex(buf1,BUFFER_SIZE);
	 print_hex(bufbig1,10);

	 //float fv=0.99;
	 //memset(&fv,0,sizeof(fv));
	 //print_hex(&fv,sizeof(fv));
	 //printf("%lf",fv);


	 //float到int比float到float慢3.5倍  double转int比float转int快约1倍
	 double buff[2000];
	 int bufi[2000];
	  time.getBeginTime();
	 for (int K=0;K<32;K++)
	 {
		 time.getBeginTime();
		 for (int K=0;K<2000;K++)
	     {
		      bufi[K]=buff[K];
		 }
		 time.getEndPrint(4);
	 }

	 time.reSet();


	 delete[] bufbig1;
	 delete[] bufbig2;

	 return 0;
 }

#endif



NAME_SPACE_MAIN_END
