 
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


typedef  float FloatType;   //根据精度需要定义为double或者float

//粉红噪声,白噪声发生器,去噪声算法
FUNCLASS_API void getNWhiteNoise(FloatType* pBuf,int len);   //取一个由N个采样点组成的白噪声  范围-1.0到1.0
FUNCLASS_API void getNPinkNoise(FloatType* pBuf,int len);   //取一个由N个采样点组成的粉红噪声  范围-1.0到1.0

FUNCLASS_API int  Test_Noise();


#endif


