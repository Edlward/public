 
#ifndef NOISE_h_
#define NOISE_h_

//#ifdef _WIN32
//
//#ifdef FUNCLASS_API_EXPORT
//#define FUNCLASS_API __declspec(dllexport)
//#else
//#define FUNCLASS_API __declspec(dllimport)
//#endif
//#else
//#define FUNCLASS_API 
//#endif

#if defined(_WIN32)
	#if defined(FUNCLASS_API_EXPORT)
		#define FUNCLASS_API __declspec(dllexport)
	#elif defined(FUNCLASS_API_IMPORT)
		#define FUNCLASS_API __declspec(dllimport)
	#else
	    #define FUNCLASS_API 
	#endif
#else
	#define FUNCLASS_API 
#endif


typedef  float FloatType;   //���ݾ�����Ҫ����Ϊdouble����float

//�ۺ�����,������������,ȥ�����㷨
FUNCLASS_API void getNWhiteNoise(FloatType* pBuf,int len);   //ȡһ����N����������ɵİ�����  ��Χ-1.0��1.0
FUNCLASS_API void getNPinkNoise(FloatType* pBuf,int len);   //ȡһ����N����������ɵķۺ�����  ��Χ-1.0��1.0

FUNCLASS_API int  Test_Noise();


#endif


