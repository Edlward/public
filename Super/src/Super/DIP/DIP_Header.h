#ifndef DIP_Header_H
#define DIP_Header_H

//M DIP��Digital Image Process(����ͼ����)����д
//M DVP��Digital Video Process(������Ƶ����)����д--ǰ��֡���ͼ�����㷨


enum EIMG_Forfat
{
	Img_RGB,   //RGBͼ��
	Img_RGBA,  //RGBAͼ��,��͸��ͨ��
	Img_Gray,  //�Ҷ�ͼ��
	Img_BIN,   //��ֵͼ��ֻ��0/1
}




//ͼ��ͷ�ṹ��
class SImage   //
{
public:
	int  width;         /* ͼ��������� */  
	int  height;        /* ͼ���������*/   
	int  widthStep;     /* ���е�ͼ���д�С�����ֽ�Ϊ��λ */	
	char *imageData;    /* ָ�����е�ͼ������ */

	//int  ID;            /* �汾 (=0)*/  	
	//int  nSize;         /* IplImage��С */  
	int  imageSize;     /* ͼ�����ݴ�С(�ڽ����ȡ��ʽ��imageSize=image->height*image->widthStep������λ�ֽ�*/  	
	int  nChannels;     /* �����OPENCV����֧��1,2,3 ��4��ͨ��*/  
	int  depth;         /* ���ص�λ���: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U, 
						   IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F ��֧�� */  
				   
	//char colorModel[4]; /* ��OpenCV���� */  
	//char channelSeq[4]; /* ͬ�� */  
	int  dataOrder;     /* 0 - �����ȡ��ɫͨ��, 1 - �ֿ�����ɫͨ��. 
						   cvCreateImageֻ�ܴ��������ȡͼ�� */  

	//int  align;         /* ͼ�������� (4 or 8). OpenCV ��������ʹ�� widthStep ���� */  
	int  origin;        /* 0 - ������ṹ, 
						   1 - �ס���ṹ (Windows bitmaps ���) */  
	//struct _IplROI *roi;/* ͼ�����Ȥ����. ����ֵ�ǿ�ֻ�Ը�������д��� */  
	//struct _IplImage *maskROI; /* �� OpenCV�б�����NULL */  


	//int  BorderMode[4]; /* �߼ʽ���ģʽ, ��OpenCV���� */  
	//int  BorderConst[4]; /* ͬ�� */  
	//char *imageDataOrigin; /* ָ��ָ��һ����ͬ��ͼ�����ݽṹ�����Ǳ������еģ�����Ϊ�˾���ͼ���ڴ����׼���� */  
};



struct IplImage  
{  
	int  nSize;         /* IplImage��С */  
	int  ID;            /* �汾 (=0)*/  
	int  nChannels;     /* �����OPENCV����֧��1,2,3 �� 4 ��ͨ�� */  
	int  alphaChannel;  /* ��OpenCV���� */  
	int  depth;         /* ���ص�λ���: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U, 
						   IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F ��֧�� */  
	char colorModel[4]; /* ��OpenCV���� */  
	char channelSeq[4]; /* ͬ�� */  
	int  dataOrder;     /* 0 - �����ȡ��ɫͨ��, 1 - �ֿ�����ɫͨ��. 
						   cvCreateImageֻ�ܴ��������ȡͼ�� */  
	int  origin;        /* 0 - ������ṹ, 
						   1 - �ס���ṹ (Windows bitmaps ���) */  
	int  align;         /* ͼ�������� (4 or 8). OpenCV ��������ʹ�� widthStep ���� */  
	int  width;         /* ͼ��������� */  
	int  height;        /* ͼ���������*/  
	struct _IplROI *roi;/* ͼ�����Ȥ����. ����ֵ�ǿ�ֻ�Ը�������д��� */  
	struct _IplImage *maskROI; /* �� OpenCV�б�����NULL */  
	void  *imageId;     /* ͬ��*/  
	struct _IplTileInfo *tileInfo; /*ͬ��*/  
	int  imageSize;     /* ͼ�����ݴ�С(�ڽ����ȡ��ʽ��imageSize=image->height*image->widthStep������λ�ֽ�*/  
	char *imageData;  /* ָ�����е�ͼ������ */  
	int  widthStep;   /* ���е�ͼ���д�С�����ֽ�Ϊ��λ */  
	int  BorderMode[4]; /* �߼ʽ���ģʽ, ��OpenCV���� */  
	int  BorderConst[4]; /* ͬ�� */  
	char *imageDataOrigin; /* ָ��ָ��һ����ͬ��ͼ�����ݽṹ�����Ǳ������еģ�����Ϊ�˾���ͼ���ڴ����׼���� */  
}  


#endif // MYIMAGEPROCESS_H
