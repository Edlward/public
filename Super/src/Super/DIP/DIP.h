#ifndef DIP_H
#define DIP_H
//M DIP��Digital Image Process(����ͼ����)����д
//M DVP��Digital Video Process(������Ƶ����)����д--ǰ��֡���ͼ�����㷨

//M ֱ�Ӱ�������ͼ����ͷ�ļ�
/*M
#include "DIP_EdgesobelCanny.h"  //�߸���Ե�����غ���-����canny
#include "DIP_Filter.h"          //4������ͼ���˲�����
#include "DIP_HazeRemoval.cpp"   //��ԭɫȥ��--δ���촿C++
#include "DIP_Hough.h"    //����ֱ�� �������ֱ�� ����Բ�任
#include "DIP_mangyuan.h" //����äԪͼͷ�ļ�
#include "DIP_MirrorFilter.h"    //���ļ���Ҫ�Ǹ���ͼ���˾��㷨
#include "DIP_Morphologyex.h"  //ͼ����̬ѧ�任 ���� ��ʴ ������ ������ ��ñ ��ñ
#include "DIP_movetack_invadealarm.h" //�˶����������ּ��ϲ���--�������Ч��

*/






//�ڷ�֮ͷ�ļ�����ֱ��ʵ�ֺ������������(����ʱ�޸�-����-��������)--��Ҫ���¹�������
//������׺��_RGB,_Gray,_BIN �ֱ�������Ǵ˺���ר������ RGB��ɫͼ�񡢻Ҷ�ͼ��������ͼ��Ĵ����������

#include <stdio.h>
#include <stdlib.h>
//ͼ������
#include <math.h>  //����ֵͷ�ļ�
//#include "MyHeader.h"




//ȫ�ֱ���
// extern int imgwidth;   //ͼ����
// extern int imgheight;  //ͼ��߶�
//��������
//inline---�������벻ͨ��
//extern ��C��{#include ��OOXX.h��}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//��C���ܸ㶨��ͼ�����㷨
///////////////////////////////////////////////////////////////
//��һ��  ��ѡopencv�ĳ����㷨--�ĳɴ�C����½����ӽ���
///////////////////////////////////////////////////////////////
//��һ��  �������Ƶ��㷨dll�� ���ܵ�DES AES RSA
//ѹ����LZ77 LZ78 LZW �������� �γ� ��������
//ͼ���ʽ�����  jpg jpg2000��bmp png ��
///////////////////////////////////////////////////////////////






//���ȵ���
//photoshop�����߹���

//�ֶ����ԶԱȶ�

//photoshop�����߹���

//ɫ�׵���--��--��--�׵���   �ı����ɫ��
//��ɢ����Ҷdft--float����--��ʾ--��ͨ�˲�--��ͨ�˲�
//�˲������ֵ --��˹��̬�˲� --��ֵ�˲�  ˫���˲�
//��̬ѧ����  ������ ������ ��ñ ��ñ ���� ��ʴ--��ֲMorphologyEx()����
//canny��Ե sobel ������˹
//����ֱ�� ����Բ���--����任
//�����趨����ֵ ���һ�������findContours
//harris�ǵ�  shi-tomsi�ǵ�--SIFT SURF ORB MSER--�ǵ�ƥ��













//ͼ���Է�ת--����--����ת��   PolarityFlip
void PolarityFlip(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );

//ͼ��ˮƽ��ת--���Ҿ���
void HorizontalFlip(unsigned char* pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );

//ͼ��ֱ��ת--���¾���
void VerticalFlip(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );

//���ȶԱȶ�һ���������---Ϊ������ٶȿ��Ը��ò��ұ�ʵ�֡�����brightness�Աȶ�contrast
void AdjBrightnessContrast(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                           int BrightnessValue,\
                           int ContrastValue, \
                           bool EnableFlag\
                           );

//�ȳ���Ҷ�ͼ��תα��ɫ  //�ʺ����1  �����������
void GrayToPseudocolor(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );

//��ɫͼ��ת�ڰ׻Ҷ�ͼ  ��ɫRGBתRGB��ͬ�ĻҶ�ͼ
void ColorToGray(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  );



//ͼ����ǿ--��ͨ��--��ηֱ���--����ֱ��ͼ���⻯��ͼ����
void EqualizeHist(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                   bool EnableFlag \
                   );

//ͼ���˲�--½�����




//���ֱ����㷨
void FireAlarmImageProcess(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool TemperatureAlarmEnable,\
                  unsigned char EdgeGrayValue,\
                  int  *pHeighSumArea,\
                  bool MoveAlarmEnable,\
                  unsigned char* pMoverateFrontBackImg,\
                  int  *pMoverateReturnValue,\
                  bool EnableFlag \
                  );








//�ڰ�ͼ���߻Ҷ�ͼר��ͼ�����㷨
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

//ͼ���ֵ��  --���ݴ������Ĳ�����RGB��ֵ��--
void ThresholdRGBToBin(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                     unsigned char  ThresholdValue, \
                     bool EnableFlag \
                  );

//ͼ���ֵ��  --���ݴ������Ĳ�����Gray��ֵ��ΪBIN--  ��ͨ���Ҷ�ͼ��ֵ��
void ThresholdGrayToBin(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                     unsigned char  ThresholdValue, \
                     bool EnableFlag \
                  );



//16λ���������ݲ���
void FireAlarmBit16(unsigned  int *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool TemperatureAlarmEnable,\
                  unsigned  int EdgeGrayValue,\
                  unsigned  int* pHeighSumArea\
                  );







///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//��Ҫopencv��ͼ�����㷨
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






//�˶������㷨
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


class CMotiontTacking   //�˶�������--��������ֱ�����ڲ�ʵ��  ���Է�װ����--��ɺ��ܽ�
{
public:
   CMotiontTacking()// CMotiontTacking();  //��Ȼ��������--����ʵ�ֺ���������
   {
     nFrmNum=0;     //���ڵ�һ�ε��ú��������ڴ�
   };
   ~CMotiontTacking(){};
////�˶������㷨��Ҫ���ⲿ����--���Է�װ����
IplImage *m_imgFrame;
//pFrame��ǰ֡. �������(pFrImgǰ����pBkImg����)��֡����(pBkImgǰһ֡��pFrImg��ֵ)
IplImage* pFrame;
IplImage* pFrImg;
IplImage* pBkImg;
IplImage* pCamImg;  //Camshift����ͼƬ
IplImage* pHsv;
IplImage* pHue;
IplImage* pMask;
IplImage* pBackproject;
IplImage* pHistImg;
CvHistogram * pHist;
 //pFrameMat��ǰ֡. �������(pFrMatǰ����pBkMat����)��֡����(pBkMatǰһ֡��pFrMat��ֵ)
CvMat* pFrameMat;
CvMat* pFrMat;
CvMat* pBkMat;
CvMemStorage *stor;
CvSeq *cont;//, *result, *squares;

int nFrmNum;
bool flagStartBK;
bool flagStartCS;
bool flagTrack;
int histSizes;//����ֱ��ͼbins�ĸ�����Խ��Խ��ȷ
CvRect selection;
CvRect trackWin;//���ٴ���
CvFont m_font;
//CvMemStorage *pStor;
CvSeq *pCont;
CvGaussBGModel *bg_model;

QImage *pQImage;
IplImage *piplImg;

//�˶������㷨--ǰ��--����ͼ���ڴ�û���ͷ�
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

class CInvadeAlarm   //�˶�����--�����ּ����
{
public:
   CInvadeAlarm()// CMotiontTacking();  //��Ȼ��������--����ʵ�ֺ���������
   {
     nFrmNum=0;     //���ڵ�һ�ε��ú��������ڴ�
   };
   ~CInvadeAlarm(){};
////�˶������㷨��Ҫ���ⲿ����--���Է�װ����
IplImage *m_imgFrame;
//pFrame��ǰ֡. �������(pFrImgǰ����pBkImg����)��֡����(pBkImgǰһ֡��pFrImg��ֵ)
IplImage* pFrame;
IplImage* pFrImg;
IplImage* pBkImg;
IplImage* pCamImg;  //Camshift����ͼƬ
IplImage* pHsv;
IplImage* pHue;
IplImage* pMask;
IplImage* pBackproject;
IplImage* pHistImg;
CvHistogram * pHist;
//pFrameMat��ǰ֡. �������(pFrMatǰ����pBkMat����)��֡����(pBkMatǰһ֡��pFrMat��ֵ)
CvMat* pFrameMat;
CvMat* pFrMat;
CvMat* pBkMat;
CvMemStorage *stor;
CvSeq *cont;//, *result, *squares;

int nFrmNum;
bool flagStartBK;
bool flagStartCS;
bool flagTrack;
int histSizes;//����ֱ��ͼbins�ĸ�����Խ��Խ��ȷ
CvRect selection;
CvRect trackWin;//���ٴ���
CvFont m_font;
CvMemStorage *pStor;
CvSeq *pCont;
CvGaussBGModel *bg_model;

QImage *pQImage;
IplImage *piplImg;

int pnpoly(int nPointvert, int *Pointvertx, int *Pointverty, int testx, int testy);


//�˶������㷨--�����ּ��--ǰ��--����ͼ���ڴ�û���ͷ�
void InvadeAlarm(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool InvadeAlarmEnable ,\
                  int nPointvert, int *Pointvertx, int *Pointverty\
                  );

};

#endif


#if 1
//ȫ�ֱ���

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//w��Ϊ�˱���һ���ֵ���
class CRemoveFog   //
{
public:
    int imgwidth;   //ͼ����
    int imgheight;  //ͼ��߶�

   CRemoveFog()// CMotiontTacking();  //��Ȼ��������--����ʵ�ֺ���������
   {
       block=5;
       w1=80;
       w=w1/100;
       src=NULL;
       dst=NULL;
       src=cvCreateImageHeader(cvSize(imgwidth,imgheight), 8,3);   //�����ļ�ͷ

   };
   ~CRemoveFog(){};
int block;
int w1;
double w;
IplImage *src;
IplImage *dst;

//����ȥ��������---�޸�ԴĿ���ַ--
//IplImage *quw(IplImage *src,int block,double w)//��ʱ���ô��ݲ���
void RemoveFog(unsigned char *pImage);
};

#endif




class MyImageProcess
{
public:
    MyImageProcess();
};

#endif // MYIMAGEPROCESS_H
