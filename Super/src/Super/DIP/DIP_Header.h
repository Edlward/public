#ifndef DIP_Header_H
#define DIP_Header_H

//M DIP是Digital Image Process(数字图像处理)的缩写
//M DVP是Digital Video Process(数字视频处理)的缩写--前后帧相关图像处理算法


enum EIMG_Forfat
{
	Img_RGB,   //RGB图像
	Img_RGBA,  //RGBA图像,带透明通道
	Img_Gray,  //灰度图像
	Img_BIN,   //二值图像只有0/1
}




//图像头结构体
class SImage   //
{
public:
	int  width;         /* 图像宽像素数 */  
	int  height;        /* 图像高像素数*/   
	int  widthStep;     /* 排列的图像行大小，以字节为单位 */	
	char *imageData;    /* 指向排列的图像数据 */

	//int  ID;            /* 版本 (=0)*/  	
	//int  nSize;         /* IplImage大小 */  
	int  imageSize;     /* 图像数据大小(在交叉存取格式下imageSize=image->height*image->widthStep），单位字节*/  	
	int  nChannels;     /* 大多数OPENCV函数支持1,2,3 或4个通道*/  
	int  depth;         /* 像素的位深度: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U, 
						   IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F 可支持 */  
				   
	//char colorModel[4]; /* 被OpenCV忽略 */  
	//char channelSeq[4]; /* 同上 */  
	int  dataOrder;     /* 0 - 交叉存取颜色通道, 1 - 分开的颜色通道. 
						   cvCreateImage只能创建交叉存取图像 */  

	//int  align;         /* 图像行排列 (4 or 8). OpenCV 忽略它，使用 widthStep 代替 */  
	int  origin;        /* 0 - 顶—左结构, 
						   1 - 底—左结构 (Windows bitmaps 风格) */  
	//struct _IplROI *roi;/* 图像感兴趣区域. 当该值非空只对该区域进行处理 */  
	//struct _IplImage *maskROI; /* 在 OpenCV中必须置NULL */  


	//int  BorderMode[4]; /* 边际结束模式, 被OpenCV忽略 */  
	//int  BorderConst[4]; /* 同上 */  
	//char *imageDataOrigin; /* 指针指向一个不同的图像数据结构（不是必须排列的），是为了纠正图像内存分配准备的 */  
};



struct IplImage  
{  
	int  nSize;         /* IplImage大小 */  
	int  ID;            /* 版本 (=0)*/  
	int  nChannels;     /* 大多数OPENCV函数支持1,2,3 或 4 个通道 */  
	int  alphaChannel;  /* 被OpenCV忽略 */  
	int  depth;         /* 像素的位深度: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U, 
						   IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F 可支持 */  
	char colorModel[4]; /* 被OpenCV忽略 */  
	char channelSeq[4]; /* 同上 */  
	int  dataOrder;     /* 0 - 交叉存取颜色通道, 1 - 分开的颜色通道. 
						   cvCreateImage只能创建交叉存取图像 */  
	int  origin;        /* 0 - 顶—左结构, 
						   1 - 底—左结构 (Windows bitmaps 风格) */  
	int  align;         /* 图像行排列 (4 or 8). OpenCV 忽略它，使用 widthStep 代替 */  
	int  width;         /* 图像宽像素数 */  
	int  height;        /* 图像高像素数*/  
	struct _IplROI *roi;/* 图像感兴趣区域. 当该值非空只对该区域进行处理 */  
	struct _IplImage *maskROI; /* 在 OpenCV中必须置NULL */  
	void  *imageId;     /* 同上*/  
	struct _IplTileInfo *tileInfo; /*同上*/  
	int  imageSize;     /* 图像数据大小(在交叉存取格式下imageSize=image->height*image->widthStep），单位字节*/  
	char *imageData;  /* 指向排列的图像数据 */  
	int  widthStep;   /* 排列的图像行大小，以字节为单位 */  
	int  BorderMode[4]; /* 边际结束模式, 被OpenCV忽略 */  
	int  BorderConst[4]; /* 同上 */  
	char *imageDataOrigin; /* 指针指向一个不同的图像数据结构（不是必须排列的），是为了纠正图像内存分配准备的 */  
}  


#endif // MYIMAGEPROCESS_H
