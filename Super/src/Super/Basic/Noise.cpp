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



 
void getNWhiteNoise(FloatType* pBuf,int NLen)   //ȡһ����N����������ɵİ�����white_noise(FloatType *x, int n) 
{
	int i;
	for (i = 0; i < NLen; i++) 
	{
		pBuf[i] = 2.0f * rand() / RAND_MAX - 1.0f;   //ȡֵΪ-1.0��+1.0
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
	FloatType* x=new FloatType[NLen];     //����������
	//float* y=new float[NLen];     //�ۺ���������
	FloatType* y=pBuf;

	// 1. Generate white noise signal.
	getNWhiteNoise(x,NLen);       
	
	//printf("WhiteNoise:\n");
	//printFloat(x,NLen);

	// 2. Generate pink noise filter coefficients.
	const int Ncoeff=64;
	FloatType coeffs[Ncoeff];                  //�˲�������
	FloatType alpha = 1.0f;
	FloatType s[Ncoeff];

	int i;
	for (i = 0; i < Ncoeff; i++)
	{
		s[i] = 0.0f;
	}
	invfreq_coeffs(coeffs, alpha, Ncoeff);
	// 3. Get pink noise signal by filtering the white noise signal.      
	pinknoise(x, y, coeffs, s, NLen, Ncoeff);         //�������˲��ɷۺ�����
	//�ѷۺ��������浽Ŀ�ĵ�
	delete x;
}



#define N 100
#define P 64    //��ȷ��
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

�ۺ�����������
 
	2017-04-26 17:51 60���Ķ� ����(0) �ղ� �ٱ�
	���ࣺ �˲�����ƣ�3��  
	��Ȩ����������Ϊ����ԭ�����£�δ������������ת�ء�
 
Definition of pinknoise

	Pink noise or ?1?f noise is a signal or process with a frequency spectrum such that the power spectral density (energy or power per frequency interval) is inversely proportional to the frequency of the signal. In pink noise, each octave (halving/doubling in frequency) carries an equal amount of noise energy. The name arises from the pink appearance of visible light with this power spectrum1.

	Generate pink noise in MATLAB

	ʹ��MALTLAB�ṩ��dsp.ColoredNoise System object ������ϵ��alpha�ֱ�ȡ0/1/2/-1/-2���ɷֱ����ɰ���/����/����/����/���롣�������÷����ɲο�����dsp.ColoredNoise��
	Pinknosie generation algorithm��

	��һ��ΪMATLAB�����ĵ��������㷨�� 
	Algorithm of pinknoise 
	Ҳ����˵����һ�ΰ���������������ͨ��һ��63�׵�ȫ�����˲������Ϳ��Եõ����롣����Ϊʲô�������Եĵ����룬����������ר�ŵĽ��⡣������˵˼·�� 
	��ôdsp.ColoredNoise�������ʵ�ַ��뷢�����Ϳ��Էֳ����������� 
Step1: ����һ�ΰ����� 
	   x = wgn(0, 1, 100); 
Step2:��������ͨ��63��ȫ�����˲����˲� 
		  y = filter(1, a, x);

	  ��C�������棬�Ϳ����������˼·��дPinknoise��������
		  C implementation of Pinknoise

		  ������������ͷǳ��򵥣���c����дһ�����뷢������
#endif


