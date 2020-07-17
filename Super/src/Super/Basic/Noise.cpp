#include "Noise.h"
#include <iostream>




#include <stdio.h>
#include <stdlib.h>
 
void printFloat(FloatType* p,int n)
{
	for (int i=0;i<n;i++)
	{
		printf("%f ",p[i]);
		//std::cout<<" "<<(float)p[i];
		if (i%8==7)
		{
			//std::cout<<std::endl;
			printf("\n");
		}
	}
}



 
void getNWhiteNoise(FloatType* pBuf,int NLen)   //取一个由N个采样点组成的白噪声white_noise(FloatType *x, int n) 
{
	int i;
	for (i = 0; i < NLen; i++) 
	{
		pBuf[i] = 2.0f * rand() / RAND_MAX - 1.0f;   //取值为-1.0到+1.0
		//pBuf[i] = 1.0*rand() /RAND_MAX;
	}
}


void invfreq_coeffs(FloatType* coeffs, float alpha, int n)
{    
	float a = -alpha / 2.0f;
	coeffs[0] = a;
	int i;
	for (i = 1; i < n; i++)
	{
		coeffs[i] = coeffs[i-1] / (i+1) * (i+a);
	}
}

FloatType allpole_filter(FloatType  x,        // Input signal
					 const FloatType *a,             // Feedback coefficients
					 FloatType *s,                      // Delayline
					 int  p)                                 // Number of coefficients
{
	FloatType y = x;
	int i;
	for (i = 0; i < p; i++)
		y -= a[i] * s[i];
	for (i = p - 1; i > 0; i--)
		s[i] = s[i-1];
	s[0] = y;
	return y;
}

void pinknoise(const FloatType *x,  // Input signal  
			   FloatType *y,  // Output signal
			   const FloatType *a,  // Feedback coefficients
			   FloatType *s,  // Delayline
			   int  n,  // Sample points
			   int  p)  // Number of coefficients
{
	int i;
	for(i = 0; i < n; i++)
	{
		y[i] = allpole_filter(x[i], a, s, p);
	}
}               



void getNPinkNoise(FloatType* pBuf,int NLen)
{
	FloatType* x=new FloatType[NLen];     //白噪声缓存
	//float* y=new float[NLen];     //粉红噪声缓存
	FloatType* y=pBuf;

	// 1. Generate white noise signal.
	getNWhiteNoise(x,NLen);       
	
	//printf("WhiteNoise:\n");
	//printFloat(x,NLen);

	// 2. Generate pink noise filter coefficients.
	const int Ncoeff=64;
	FloatType coeffs[Ncoeff];                  //滤波参数？
	FloatType alpha = 1.0f;
	FloatType s[Ncoeff];

	int i;
	for (i = 0; i < Ncoeff; i++)
	{
		s[i] = 0.0f;
	}
	invfreq_coeffs(coeffs, alpha, Ncoeff);
	// 3. Get pink noise signal by filtering the white noise signal.      
	pinknoise(x, y, coeffs, s, NLen, Ncoeff);         //白噪声滤波成粉红噪声
	//把粉红噪声保存到目的地
	delete x;
}



#define N 100
#define P 64    //不确定
FloatType x[N];
FloatType y[N];


void print_array_to_file(FILE *fp, const FloatType *x, int n)
{
	int i;

	for (i = 0; i < n; i++)    
		fprintf(fp, "%.15f\n", x[i]);
}
int GenaratorNoise()
{
	// 1. Generate white noise signal.
	getNWhiteNoise(x, N);

	// 2. Generate pink noise filter coefficients.
	FloatType coeffs[P];

	FloatType alpha = 1.0f;

	FloatType s[P];

	int i;
	for (i = 0; i < P; i++)
		s[i] = 0.0f;

	invfreq_coeffs(coeffs, alpha, P);

	// 3. Get pink noise signal by filtering the white noise signal.      
	pinknoise(x, y, coeffs, s, N, P);

	// 4. Save pink noise signal to file and verify it in MATLAB.
	FILE *fpx = fopen("whitenosie.txt", "w");

	FILE *fpy = fopen("pinknosie.txt", "w");

	FILE *fpa = fopen("coeffs.txt", "w");

	FILE *fps = fopen("delayline.txt", "w"); 

	print_array_to_file(fpx, x, N);

	print_array_to_file(fpy, y, N);

	print_array_to_file(fpa, coeffs, P);

	print_array_to_file(fps, s, P);

	fclose(fpx);

	fclose(fpy);

	fclose(fpa);

	fclose(fps);

	return 0;
}



int Test_Noise()
{

	const int NS=8*5;
	FloatType wNoise[NS]={0};
	FloatType pNoise[NS]={0};


	//printFloat(wNoise,NS);
	//printFloat(pNoise,NS);

	//getNWhiteNoise(wNoise,NS);
	getNPinkNoise(pNoise,NS);

	//printf("WhiteNoise:\n");
   // printFloat(wNoise,NS);
	printf("PinkNoise:\n");
    printFloat(pNoise,NS);
	return 0;
}








#if 0

粉红噪声发生器
 
	2017-04-26 17:51 60人阅读 评论(0) 收藏 举报
	分类： 滤波器设计（3）  
	版权声明：本文为博主原创文章，未经博主允许不得转载。
 
Definition of pinknoise

	Pink noise or ?1?f noise is a signal or process with a frequency spectrum such that the power spectral density (energy or power per frequency interval) is inversely proportional to the frequency of the signal. In pink noise, each octave (halving/doubling in frequency) carries an equal amount of noise energy. The name arises from the pink appearance of visible light with this power spectrum1.

	Generate pink noise in MATLAB

	使用MALTLAB提供的dsp.ColoredNoise System object 函数，系数alpha分别取0/1/2/-1/-2，可分别生成白噪/粉噪/棕噪/蓝噪/紫噪。函数调用方法可参考函数dsp.ColoredNoise。
	Pinknosie generation algorithm：

	这一段为MATLAB帮助文档给出的算法： 
	Algorithm of pinknoise 
	也就是说，给一段白噪声，将白噪声通过一个63阶的全极点滤波器，就可以得到粉噪。至于为什么这样可以的到粉噪，接下来会有专门的讲解。这里先说思路。 
	那么dsp.ColoredNoise这个函数实现粉噪发生器就可以分成这样两步： 
Step1: 生成一段白噪声 
	   x = wgn(0, 1, 100); 
Step2:将白噪声通过63阶全极点滤波器滤波 
		  y = filter(1, a, x);

	  在C语言里面，就可以利用这个思路来写Pinknoise发生器。
		  C implementation of Pinknoise

		  接下来的事情就非常简单，用c语言写一个粉噪发生器：
#endif


