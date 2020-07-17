#ifndef DIP_H
#define DIP_H
//M DIP是Digital Image Process(数字图像处理)的缩写
//M DVP是Digital Video Process(数字视频处理)的缩写--前后帧相关图像处理算法

//M 直接包含所有图像处理头文件
/*M
#include "DIP_EdgesobelCanny.h"  //七个边缘检测相关函数-包括canny
#include "DIP_Filter.h"          //4个常用图像滤波函数
#include "DIP_HazeRemoval.cpp"   //暗原色去雾--未改造纯C++
#include "DIP_Hough.h"    //霍夫直线 霍夫概率直线 霍夫圆变换
#include "DIP_mangyuan.h" //制作盲元图头文件
#include "DIP_MirrorFilter.h"    //本文件主要是各种图像滤镜算法
#include "DIP_Morphologyex.h"  //图像形态学变换 膨胀 腐蚀 开运算 闭运算 黑帽 礼帽
#include "DIP_movetack_invadealarm.h" //运动跟踪与入侵检测合并类--提高运行效率

*/






//在分之头文件里面直接实现函数不方便调试(调试时修改-运行-并不体现)--需要重新构建编译
//函数后缀的_RGB,_Gray,_BIN 分别用来标记此函数专门用于 RGB彩色图像、灰度图、二进制图像的处理，以免混淆

#include <stdio.h>
#include <stdlib.h>
//图像处理类
#include <math.h>  //绝对值头文件
//#include "MyHeader.h"




//全局变量
// extern int imgwidth;   //图像宽度
// extern int imgheight;  //图像高度
//函数申明
//inline---有这句编译不通过
//extern “C”{#include “OOXX.h”}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//纯C就能搞定的图像处理算法
///////////////////////////////////////////////////////////////
//下一步  精选opencv的常用算法--改成纯C语言陆续添加进来
///////////////////////////////////////////////////////////////
//下一步  制作类似的算法dll包 加密的DES AES RSA
//压缩的LZ77 LZ78 LZW ，霍夫曼 游程 算数编码
//图像格式处理包  jpg jpg2000，bmp png 等
///////////////////////////////////////////////////////////////






//亮度调整
//photoshop的曲线功能

//手动线性对比度

//photoshop的曲线功能

//色阶调整--黑--灰--白调整   改变输出色阶
//离散傅里叶dft--float精度--显示--高通滤波--低通滤波
//滤波方框均值 --高斯正态滤波 --中值滤波  双边滤波
//形态学操作  开运算 闭运算 顶帽 黑帽 膨胀 腐蚀--移植MorphologyEx()函数
//canny边缘 sobel 拉普拉斯
//霍夫直线 霍夫圆检测--距离变换
//根据设定的阈值 查找绘制轮廓findContours
//harris角点  shi-tomsi角点--SIFT SURF ORB MSER--角点匹配













//图像极性翻转--白热--黑热转换   PolarityFlip
void PolarityFlip(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );

//图像水平翻转--左右镜像
void HorizontalFlip(unsigned char* pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );

//图像垂直翻转--上下镜像
void VerticalFlip(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );

//亮度对比度一起调整调整---为了提高速度可以改用查找表实现。亮度brightness对比度contrast
void AdjBrightnessContrast(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                           int BrightnessValue,\
                           int ContrastValue, \
                           bool EnableFlag\
                           );

//热成像灰度图像转伪彩色  //彩虹编码1  添加其他编码
void GrayToPseudocolor(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );

//彩色图像转黑白灰度图  彩色RGB转RGB相同的灰度图
void ColorToGray(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );



//图像增强--单通道--多次分别处理--基于直方图均衡化的图像处理
void EqualizeHist(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                   bool EnableFlag \
                   );

//图像滤波--陆续添加




//火灾报警算法
void FireAlarmImageProcess(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool TemperatureAlarmEnable,\
                  unsigned char EdgeGrayValue,\
                  int  *pHeighSumArea,\
                  bool MoveAlarmEnable,\
                  unsigned char* pMoverateFrontBackImg,\
                  int  *pMoverateReturnValue,\
                  bool EnableFlag \
                  );








//黑白图或者灰度图专用图像处理算法
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

//图像二值化  --根据传进来的参数对RGB二值化--
void ThresholdRGBToBin(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                     unsigned char  ThresholdValue, \
                     bool EnableFlag \
                  );

//图像二值化  --根据传进来的参数对Gray二值化为BIN--  单通道灰度图二值话
void ThresholdGrayToBin(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                     unsigned char  ThresholdValue, \
                     bool EnableFlag \
                  );



//16位报警测试暂不用
void FireAlarmBit16(unsigned  int *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool TemperatureAlarmEnable,\
                  unsigned  int EdgeGrayValue,\
                  unsigned  int* pHeighSumArea\
                  );







///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//需要opencv的图像处理算法
///////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


#if 1
#include <opencv.hpp>
#include <opencv_modules.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/video/background_segm.hpp"
//#include "opencv2/highgui/highgui.hpp"






//运动跟踪算法
/*
void MotiontTacking(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool MotiontTackingEnable \
                  );
*/


#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include "cvaux.h"
#include <QImage>


class CMotiontTacking   //运动跟踪类--函数功能直接在内部实现  可以封装成类--完成后总结
{
public:
   CMotiontTacking()// CMotiontTacking();  //居然会编译出错--忘记实现函数内容了
   {
     nFrmNum=0;     //用于第一次调用函数申请内存
   };
   ~CMotiontTacking(){};
////运动跟踪算法需要的外部变量--可以封装成类
IplImage *m_imgFrame;
//pFrame当前帧. 背景差分(pFrImg前景，pBkImg背景)，帧间差分(pBkImg前一帧，pFrImg差值)
IplImage* pFrame;
IplImage* pFrImg;
IplImage* pBkImg;
IplImage* pCamImg;  //Camshift跟踪图片
IplImage* pHsv;
IplImage* pHue;
IplImage* pMask;
IplImage* pBackproject;
IplImage* pHistImg;
CvHistogram * pHist;
 //pFrameMat当前帧. 背景差分(pFrMat前景，pBkMat背景)，帧间差分(pBkMat前一帧，pFrMat差值)
CvMat* pFrameMat;
CvMat* pFrMat;
CvMat* pBkMat;
CvMemStorage *stor;
CvSeq *cont;//, *result, *squares;

int nFrmNum;
bool flagStartBK;
bool flagStartCS;
bool flagTrack;
int histSizes;//划分直方图bins的个数，越多越精确
CvRect selection;
CvRect trackWin;//跟踪窗口
CvFont m_font;
//CvMemStorage *pStor;
CvSeq *pCont;
CvGaussBGModel *bg_model;

QImage *pQImage;
IplImage *piplImg;

//运动跟踪算法--前景--背景图层内存没有释放
void MotiontTacking(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool MotiontTackingEnable \
                  );
};

#endif





#if 1
#include"cv.h"
#include"highgui.h"
#include "cxcore.h"
#include "cvaux.h"

class CInvadeAlarm   //运动跟踪--改入侵检测类
{
public:
   CInvadeAlarm()// CMotiontTacking();  //居然会编译出错--忘记实现函数内容了
   {
     nFrmNum=0;     //用于第一次调用函数申请内存
   };
   ~CInvadeAlarm(){};
////运动跟踪算法需要的外部变量--可以封装成类
IplImage *m_imgFrame;
//pFrame当前帧. 背景差分(pFrImg前景，pBkImg背景)，帧间差分(pBkImg前一帧，pFrImg差值)
IplImage* pFrame;
IplImage* pFrImg;
IplImage* pBkImg;
IplImage* pCamImg;  //Camshift跟踪图片
IplImage* pHsv;
IplImage* pHue;
IplImage* pMask;
IplImage* pBackproject;
IplImage* pHistImg;
CvHistogram * pHist;
//pFrameMat当前帧. 背景差分(pFrMat前景，pBkMat背景)，帧间差分(pBkMat前一帧，pFrMat差值)
CvMat* pFrameMat;
CvMat* pFrMat;
CvMat* pBkMat;
CvMemStorage *stor;
CvSeq *cont;//, *result, *squares;

int nFrmNum;
bool flagStartBK;
bool flagStartCS;
bool flagTrack;
int histSizes;//划分直方图bins的个数，越多越精确
CvRect selection;
CvRect trackWin;//跟踪窗口
CvFont m_font;
CvMemStorage *pStor;
CvSeq *pCont;
CvGaussBGModel *bg_model;

QImage *pQImage;
IplImage *piplImg;

int pnpoly(int nPointvert, int *Pointvertx, int *Pointverty, int testx, int testy);


//运动跟踪算法--改入侵检测--前景--背景图层内存没有释放
void InvadeAlarm(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool InvadeAlarmEnable ,\
                  int nPointvert, int *Pointvertx, int *Pointverty\
                  );

};

#endif


#if 1
//全局变量

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//w是为了保留一部分的雾
class CRemoveFog   //
{
public:
    int imgwidth;   //图像宽度
    int imgheight;  //图像高度

   CRemoveFog()// CMotiontTacking();  //居然会编译出错--忘记实现函数内容了
   {
       block=5;
       w1=80;
       w=w1/100;
       src=NULL;
       dst=NULL;
       src=cvCreateImageHeader(cvSize(imgwidth,imgheight), 8,3);   //创建文件头

   };
   ~CRemoveFog(){};
int block;
int w1;
double w;
IplImage *src;
IplImage *dst;

//定义去雾函数如下---修改源目标地址--
//IplImage *quw(IplImage *src,int block,double w)//暂时不用传递参数
void RemoveFog(unsigned char *pImage);
};

#endif




class MyImageProcess
{
public:
    MyImageProcess();
};

#endif // MYIMAGEPROCESS_H
