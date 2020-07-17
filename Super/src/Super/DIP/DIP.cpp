#if 1
#include "DIP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //memsetͷ�ļ�


//���÷��� //��������
//M ImageProcess(qImage->bits(),frame->width,frame->height,3,false,true,200);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//ͼ���Է�ת--����--����ת��   PolarityFlip
void PolarityFlip(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
if(EnableFlag)
{
    //���Է�ת--���Ȱ���
    unsigned char *ptmpImg=pImgStart;     //ָ�븴λ
    for(int i=0;i<imgwidth*imgheight*3;i++)
    {
        *ptmpImg=(255-*ptmpImg);    //���Է�ת 0-255һ��256
        ptmpImg++;
    }
}
}




//ͼ��ˮƽ��ת--���Ҿ���
void HorizontalFlip(unsigned char* pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
if(EnableFlag)
{
    //ˮƽ��ת--��swap����������ʱ�󣬵���Լ�ڴ�
    unsigned char* ptmpImg=pImgStart;       //ָ�븴λ
    unsigned char* ptmpImgLine=pImgStart;   //����ͷ��ַ
    unsigned char temp;     //�����ʱ��������
    for(int j=0;j<imgheight;j++)
    {
    //M for(int i=1;i<imgwidth;i++)   //���������ôҲ��ת���ˣ���Ϊͼ����/�Ұ��һ����ת���أ��൱��û�з�ת
       for(int i=1;i<imgwidth/2;i++)
       {
        //M �������׳���--���ǳɶԳ�ͼ��,ԭ������ԭͼ����--Ӧ������ʱͼ��������ٸ��ǻ�ԭͼ������������ʱ����
        //���õ��㽻�������뿽������ͼ��Ȼ�󽻻��������Ƚ�Ч�ʲ��
        temp= *ptmpImg;
        *ptmpImg=*(ptmpImgLine+3*(imgwidth-i));      //R
        *(ptmpImgLine+3*(imgwidth-i))=temp;

         temp= *(ptmpImg+1);
        *(ptmpImg+1)=*(ptmpImgLine+3*(imgwidth-i)+1);  //G
        *(ptmpImgLine+3*(imgwidth-i)+1)=temp;

         temp= *(ptmpImg+2);
        *(ptmpImg+2)=*(ptmpImgLine+3*(imgwidth-i)+2);  //B
        *(ptmpImgLine+3*(imgwidth-i)+2)=temp;

         ptmpImg=ptmpImg+3;   //��һ������ֵ
       }
        ptmpImgLine=ptmpImgLine+3*imgwidth;   //����ͷ��ַ����
        ptmpImg=ptmpImgLine;
     //   ptmpImg++;
    }
}
}


//ͼ��ֱ��ת--���¾���
void VerticalFlip(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
if(EnableFlag)
{
    //��ֱ��ת
    unsigned char* ptmpImg=pImgStart;     //ָ�븴λ
    unsigned char* ptmpImgLine=pImgStart;   //����ͷ��ַ
    unsigned char* ptmpImgLineLast=ptmpImgLine+imgwidth*3*(imgheight-1);   //���һ�е�ַ
    unsigned char temp2;     //�����ʱ��������
    for(int j=0;j<imgheight/2;j++)
    {
       for(int i=0;i<imgwidth*3;i++)
       {
         //���õ��㽻�������뿽������ͼ��Ȼ�󽻻��������Ƚ�Ч�ʲ��
         temp2= *ptmpImg;
         *ptmpImg=*(ptmpImgLineLast-j*imgwidth*3+i);
         *(ptmpImgLineLast-j*imgwidth*3+i)=temp2;
         ptmpImg++;
       }
       ptmpImgLine=ptmpImgLine+3*imgwidth;   //����ͷ��ַ����
       ptmpImg=ptmpImgLine;
    }
}
}


//���ȶԱȶ�һ���������---Ϊ������ٶȿ��Ը��ò��ұ�ʵ�֡�����brightness�Աȶ�contrast
void AdjBrightnessContrast(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                           int BrightnessValue,\
                           int ContrastValue,\
                           bool EnableFlag\
                           )
{
if(EnableFlag)
{
if(ContrastValue==10&&BrightnessValue==0)   { return; } //����������ȶԱȶȱ仯Ϊ0��ֱ���˳�����ʡʱ��

//alpha value [1.0-3.0]: 2.2   �Աȶȷ�Χ
//the beta value [0-100]: 50   ���ȷ�Χ

float alpha=(float)ContrastValue/10.0;
unsigned char* ptmpImg=pImgStart;     //ָ�븴λ
for(int i=0;i<imgwidth*imgheight*3;i++)
{
   float val=alpha*(*ptmpImg)+BrightnessValue;
   if(val>255)
       val=255;
   if(val<0)
       val=0;
   *ptmpImg=(unsigned char)val;    //���Է�ת 0-255һ��256
   ptmpImg++;
}

}
}





//�ȳ���Ҷ�ͼ��תα��ɫ--//�ʺ����1  �����������
void GrayToPseudocolor(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
//bool EnableFlag=false;   //α��ɫʹ�ܿ���
if(EnableFlag)
{
   unsigned char* pTempBit=(unsigned char*)pImgStart;
   for(int i=0;i<imgwidth*imgheight;i++)
   {
       //�ʺ����1
       unsigned char color=(*pTempBit);//
       if((*pTempBit)<= 32 )
       {
           *(pTempBit+0)=0;      //G��
           *(pTempBit+1)=0;      //G��
           *(pTempBit+2)=8*color-1;
       }
      else if(( color > 32)&&(color<= 64))
          {
             *(pTempBit+0)= 0;
             *(pTempBit+1)= 8*(color-32)-1;
             *(pTempBit+2) =255;
          }
      else if((color > 64)&&(color <= 96))
          {
              *(pTempBit+0)= 0;
              *(pTempBit+1)= 255;
           *(pTempBit+2) = 256-8*(color-64);
          }
      else if((color > 96)&&(color <= 128))
          {
           *(pTempBit+0) = 8*(color-96)-1;
              *(pTempBit+1)= 255;
              *(pTempBit+2)= 0;
          }
      else if((color > 128)&&(color <= 192))
       {
           *(pTempBit+0) = 255;
           *(pTempBit+1)= 256-4*(color-128);
           *(pTempBit+2)= 0;
       }
     else if((color > 192))
      {
           *(pTempBit+0) = 255;
           *(pTempBit+1)= 4*(color-192) - 1;
           *(pTempBit+2)= 4*(color-192) - 1;
      }
       pTempBit=pTempBit+3;
   }


}//
}



//��ɫͼ��ת�ڰ׻Ҷ�ͼ  ��ɫRGBתRGB��ͬ�ĻҶ�ͼ
void ColorToGray(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
if(EnableFlag)
{
    //��ɫת�Ҷ�
    unsigned char *ptmpImg=pImgStart;     //ָ�븴λ
    for(int i=0;i<imgwidth*imgheight;i++)
    {
        unsigned char tmpGray=((*ptmpImg)*28+(*(ptmpImg+1))*151+(*(ptmpImg+2))*77)>>8;
        *ptmpImg=tmpGray;
        *(ptmpImg+1)=tmpGray;
        *(ptmpImg+2)=tmpGray;

        ptmpImg=ptmpImg+3;
    }

/*M
��ɫͼת�Ҷ�ͼ��
1.�����㷨��Gray=R*0.3+G*0.59+B*0.11
2.����������Gray=(R*30+G*59+B*11)/100
3.��λ������Gray =(R*28+G*151+B*77)>>8;
4.ƽ��ֵ����Gray=(R+G+B)/3;
5.��ȡ��ɫ��Gray=G��
*/
}
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//ͼ����ǿ--��ͨ��--��ηֱ���--����ֱ��ͼ���⻯��ͼ����Histogram equalization
void EqualizeHist(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                   bool EnableFlag \
                   )
{
if(EnableFlag)
  {
    //M ��RGB����ͨ��,�ڰ�ͼ��,����
    //M ��ɫͼƬֱ��ͼ���⻯һ�㲻��ֱ�Ӷ�R��G��B����������������Ҫ��RGBת����HSV����V��������ֱ��ͼ���⻯��
   for(unsigned char nchannel=0;nchannel<=2;nchannel++)  //����ͨ��
   {


         unsigned char *ptemp=pImgStart+nchannel;  //����ͨ����ʼ��ַ
         int hist[256];           //M �Ҷȼ���
         float  fpHist[256];      //M �����Ҷ�ֵ���ʱ�
         float eqHistTemp[256];
         int eqHist[256];         //M ����ӳ���Ҷȼ���,
         int size = imgwidth *imgheight;    //ͼ���С,��ͨ��

          memset(&hist,0x00,sizeof(int)*256);          //M  û�б�Ҫ��ʼ��
//        memset(&fpHist,0x00,sizeof(float)*256);      //M  û�б�Ҫ��ʼ��
//        memset(&eqHistTemp,0x00,sizeof(float)*256);  //M  û�б�Ҫ��ʼ��


         for (int i = 0;i < size; i++) //�����־���ֱ��ͼ
         {

           unsigned char GrayIndex = *ptemp;
           hist[GrayIndex] ++ ;
           ptemp=ptemp+3;
         }
         ptemp=pImgStart+nchannel;//M ��ַ��λ


         for (int i = 0; i< 256; i++)   // ����Ҷȷֲ��ܶ�
         {
          fpHist[i] = (float)hist[i] / (float)size;
         }


         for (int i = 1; i< 256; i++)   // �����ۼ�ֱ��ͼ�ֲ�
         {
          if (i == 0)
          {
           eqHistTemp[i] = fpHist[i];
          }
          else
          {
           eqHistTemp[i] = eqHistTemp[i-1] + fpHist[i];
          }
         }


         //M �ۼƷֲ�ȡ���������������ĻҶ�ӳ���ϵ
         for (int i = 0; i< 256; i++)
         {
          eqHist[i] = (int)(255.0 * eqHistTemp[i] + 0.5);
         }


        for (int i = 0;i < size; i++)  //���лҶ�ӳ�� ���⻯
         {
           unsigned char GrayIndex = *ptemp;
           *ptemp = eqHist[GrayIndex];
            ptemp=ptemp+3;
         }

    }

  }
}


//�ڰ�ͼ����߻Ҷ�ͼ�����㷨:
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//ͼ���ֵ��  --���ݴ������Ĳ�����RGB��ֵ��--
void ThresholdRGBToBin(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                     unsigned char  ThresholdValue, \
                     bool EnableFlag \
                  )
{
if(EnableFlag)
{
    unsigned char *ptmpImg=pImgStart;     //ָ�븴λ
    for(int i=0;i<imgwidth*imgheight*3;i++)
    {
        unsigned char tmpGray=((*ptmpImg)>ThresholdValue)?255:0;
        *ptmpImg=tmpGray;
        ptmpImg++;
    }
}
}



//ͼ���ֵ��  --���ݴ������Ĳ�����Gray��ֵ��ΪBIN--  ��ͨ���Ҷ�ͼ��ֵ��
void ThresholdGrayToBin(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                     unsigned char  ThresholdValue, \
                     bool EnableFlag \
                  )
{
if(EnableFlag)
{
    unsigned char *ptmpImg=pImgStart;     //ָ�븴λ
    for(int i=0;i<imgwidth*imgheight;i++)
    {
        unsigned char tmpGray=((*ptmpImg)>ThresholdValue)?255:0;
        *ptmpImg=tmpGray;
        ptmpImg++;
    }
}
}




























//���ֱ����㷨
//M ���ֱ����㷨--�ڲ�new�޷��ж�delete,��ȫ��mem--�����˶��仯�ʻ�˼·ֱ��ͨ����ַ��Ϊ����
//M ��ͬ�ط�����--ǰ��ͼ��ͬ,��Ҫÿ�γ�ʼ��
//M ��һ֡��Ϊ������(0-2...,ż����ַ)  ��ǰ֡��Ϊǰ����(1-3-5,������ַ),
//���ֱ����㷨
void FireAlarmImageProcess(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool TemperatureAlarmEnable,\
                  unsigned char EdgeGrayValue,\
                  int  *pHeighSumArea,\
                  bool MoveAlarmEnable,\
                  unsigned char* pMoverateFrontBackImg,\
                  int  *pMoverateReturnValue,\
                  bool EnableFlag \
                  )
{
if(EnableFlag)
{
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    //M �պϵĸ�������Ե ����  //if�����¶�������ֵ||���Ҷ�������ֵ��--->��ô�������Ǳ�Ե��
    //bool HotEdgeEnable=true;  //���Ƹ��±�Ե

    if(TemperatureAlarmEnable)
    {
      int SumHeigArea=0;     //������ֵ������
      int TempSummoverate=0; //�仯������
     // unsigned char high=230;//������ֵ
      unsigned char left,right,up,down;
      unsigned char* pTempBit;
      unsigned char* pstart;
      unsigned char* pend;

      pstart=(unsigned char*)pImgStart;
    //    printf(" pstart=%d ",pstart);
      pend=(unsigned char*)(pImgStart+3*imgheight*imgwidth);
    //    printf(" pend=%d ",pend);
    //    printf(" height=%d ",imgheight);
    //    printf(" width=%d ",imgwidth);
    //    printf("\n");

      pTempBit=(unsigned char*)(pImgStart+3*imgwidth+3);//��ʼ��ַ
      //��Ե�ݲ��ж�
      unsigned char* pTempMoverateFrontBackImg=pMoverateFrontBackImg;  //����Ϊ��ʱָ��
      for(int col=0;col<imgheight-2;col++) //��--��
      {
          for(int row=0;row<imgwidth-2;row++) //��--��
          {
           if(MoveAlarmEnable)  //�˶�����ʹ�ܴ�
           {    //���µ����ͳ��
              if(*(pTempBit)>EdgeGrayValue)
              {
                    SumHeigArea=SumHeigArea+1;
                    *(pTempMoverateFrontBackImg+1)=255;   //��ֵ��������ֵ--��ǰ֡���ǰ��ͼ��
              }else { *(pTempMoverateFrontBackImg+1)=0;}; //�ǵø�λ������֮��ȫ��255

           //M   ���ر�����ɺ�--�龰ͼ�뱳��ͼ����ͬ,��Ϊ�仯�Ĳ���
           //  printf("pTempMoverateFrontBackImg=%d\n",*pTempMoverateFrontBackImg);
            if(*pTempMoverateFrontBackImg!=*(pTempMoverateFrontBackImg+1))//���ǰ����������ֵ�����
             {
                TempSummoverate=TempSummoverate+1;  //�仯����ͳ��
                *pTempMoverateFrontBackImg=*(pTempMoverateFrontBackImg+1);  //��ǰ��ͼ���Ϊ����,����һ֡�������
             }
           pTempMoverateFrontBackImg=pTempMoverateFrontBackImg+2;  //ǰ��ͼ�뱳��ͼ����洢,   //ע��Ҫ�������,����û������,Ȼ���ܱ�����/2,һ

           }//����ֻ�ǵ���ͳ�Ƹ������
           else{ if(*(pTempBit)>EdgeGrayValue) { SumHeigArea=SumHeigArea+1; }};

          left=(*(pTempBit-3));
          right=(*(pTempBit+3));
          //   up=(*(pTempBit-3*imgwidth));
          //   down=(*(pTempBit+3*imgwidth));
       //   if((left>high&&right>high)||(up>high&&down>high))  //Ϊʲô����
          if((left<EdgeGrayValue&&right>EdgeGrayValue)||(left>EdgeGrayValue&&right<EdgeGrayValue))\
       //     ||(up>EdgeGrayValue&&down<EdgeGrayValue)||(up<EdgeGrayValue&&down>EdgeGrayValue))
           {
             // *(pTempBit)=250;
              *(pTempBit+1)=10;      //���±߽��//G��
              *(pTempBit+2)=10;
           }



          //M �պϱ�Ե--�ж���������--Ĭ�Ͽ��Բ���--�˷�ʱ��
          up=(*(pTempBit-3*imgwidth));
          down=(*(pTempBit+3*imgwidth));
          if((up>EdgeGrayValue&&down<EdgeGrayValue)||(up<EdgeGrayValue&&down>EdgeGrayValue))
          {
           //  *(pTempBit)=250;
             *(pTempBit+1)=10;      //���±߽��//G��
             *(pTempBit+2)=10;
          }


          //��һ�����ص�
            pTempBit=pTempBit+3;
          }
       //����
       //M pTempBit=pTempBit+3*imgwidth;---����˼·����
       pTempBit=pTempBit+6;  //��һ��ֻ�����Ե������,��������
     }

      //�Ѹ���������ָ������
     // *pHeighSumArea=SumHeigArea;  //�Ѹ������������ַ���ⲿ���Զ�ȡ
      for(int i=1;i<=49;i++)       //����ֵ�𲽴���Buff
      {
        *(pHeighSumArea+i-1)=*(pHeighSumArea+i);
      }
      *(pHeighSumArea+49)=SumHeigArea;



      //printf("%d:TempSummoverate=%d\n",numprint++,TempSummoverate);
      //�ѵõ��ı仯��������,��λ��������ָ��
       for(int i=1;i<=49;i++)       //����ֵ�𲽴���Buff
       {
         *(pMoverateReturnValue+i-1)=*(pMoverateReturnValue+i);
       }
       *(pMoverateReturnValue+49)=TempSummoverate;
       //�����˶��仯�ʻ�˼·ֱ��ͨ����ַ��Ϊ����


     }
}
}


//16λ�Ļ��ֱ����㷨����
void FireAlarmBit16(unsigned  int *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool TemperatureAlarmEnable,\
                  unsigned  int EdgeGrayValue,\
                  unsigned  int* pHeighSumArea\
                  )
{

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//M �պϵĸ�������Ե ����  //if�����¶�������ֵ||���Ҷ�������ֵ��--->��ô�������Ǳ�Ե��
//bool HotEdgeEnable=true;  //���Ƹ��±�Ե

if(TemperatureAlarmEnable)
{
  int SumHeigArea=0;     //������ֵ������
  int TempSummoverate=0; //�仯������
 // unsigned char high=230;//������ֵ
  unsigned  int left,right,up,down;
  unsigned  int* pTempBit;
  unsigned  int* pstart;
  unsigned  int* pend;

  pstart=(unsigned int*)pImgStart;
//    printf(" pstart=%d ",pstart);
  pend=(unsigned int*)(pImgStart+imgheight*imgwidth*3);

  pTempBit=(unsigned int*)(pImgStart+3*imgwidth+3);//��ʼ��ַ
  //��Ե�ݲ��ж�

  for(int col=0;col<imgheight-2;col++) //��--��
  {
      for(int row=0;row<imgwidth-2;row++) //��--��
      {
       //if(MoveAlarmEnable)  //�˶�����ʹ�ܴ�
        if(0)  //�˶�����ʹ�ܴ�
       {    //���µ����ͳ��
          if(*(pTempBit)>EdgeGrayValue)
          {
                SumHeigArea=SumHeigArea+1;

          }
       }//����ֻ�ǵ���ͳ�Ƹ������
       else{ if(*(pTempBit)>EdgeGrayValue) { SumHeigArea=SumHeigArea+1; }};

      left=(*(pTempBit-3));
      right=(*(pTempBit+3));
     if((left<EdgeGrayValue&&right>EdgeGrayValue)||(left>EdgeGrayValue&&right<EdgeGrayValue))\
     {
         // *(pTempBit)=250;
          *(pTempBit+2)=16383;   //����Ϊһ�������ֵ��С�����   //���±߽��//G��  2^14 = 16 384
         // *(pTempBit+2)=10;
       }


      //M �պϱ�Ե--�ж���������--Ĭ�Ͽ��Բ���--�˷�ʱ��
      up=(*(pTempBit-3*imgwidth));
      down=(*(pTempBit+3*imgwidth));
      if((up>EdgeGrayValue&&down<EdgeGrayValue)||(up<EdgeGrayValue&&down>EdgeGrayValue))
      {
       //  *(pTempBit)=250;
       //  *(pTempBit+1)=10;      //���±߽��//G��
         *(pTempBit+2)=16383;
      }



      //��һ�����ص�
        pTempBit=pTempBit+3;
      }
   //����
   //M pTempBit=pTempBit+3*imgwidth;---����˼·����
   pTempBit=pTempBit+6;  //��һ��ֻ�����Ե������,��������
 }

  //�Ѹ���������ָ������
 // *pHeighSumArea=SumHeigArea;  //�Ѹ������������ַ���ⲿ���Զ�ȡ
  for(int i=1;i<=49;i++)       //����ֵ�𲽴���Buff
  {
    *(pHeighSumArea+i-1)=*(pHeighSumArea+i);
  }
    *(pHeighSumArea+49)=SumHeigArea;
 }
}














#if 1



//class CMotiontTacking   //�˶�������--��������ֱ�����ڲ�ʵ��  ���Է�װ����--��ɺ��ܽ�
//M �˶������㷨--ǰ��--����ͼ���ڴ�û���ͷ�
void CMotiontTacking::MotiontTacking(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool MotiontTackingEnable \
                  )
{
if(true==MotiontTackingEnable)
   {

    nFrmNum++;
   //����ǵ�һ֡����Ҫ�����ڴ棬����ʼ��
    if(nFrmNum == 1)
    {
      pQImage = new QImage(QSize(imgwidth,imgheight), QImage::Format_RGB888);
        //����IplImageͼ��ͷ���ƶ�ͼ��ĳߴ磬��Ⱥ�ͨ������Ȼ����
      piplImg = cvCreateImageHeader(cvSize(imgwidth,imgheight), 8,3);
      piplImg->imageData = (char*)pQImage->bits();//��qImage��ַӳ���iplImg�ĵ�ַ
      piplImg->imageData = (char*)pImgStart;


      //�����ⲿ�������ݵ��������ڲ�
       memcpy(pQImage->bits(),pImgStart,imgwidth*imgheight*3);

      pFrame=piplImg;

       cvSmooth(pFrame, pFrame, CV_GAUSSIAN, 3, 0, 0);
       pBkImg = cvCreateImage(cvSize(imgwidth,imgheight),  IPL_DEPTH_8U,1);  //��������ͼ
       pFrImg = cvCreateImage(cvSize(imgwidth,imgheight),  IPL_DEPTH_8U,1);  //����ǰ��ͼ

       pBkMat = cvCreateMat(imgheight, imgwidth, CV_32FC1);  //����mat����ͼ
       pFrMat = cvCreateMat(imgheight, imgwidth, CV_32FC1);  //����matǰ��ͼ
       pFrameMat = cvCreateMat(imgheight, imgwidth, CV_32FC1); //����matȫͼ

        //ת���ɻҶ�ͼ����
        cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
        cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);

        cvConvert(pFrImg, pFrameMat);
        cvConvert(pFrImg, pFrMat);
        cvConvert(pFrImg, pBkMat);
     }
     else
     {
      //  unsigned char *pQImage =pQImage->bits();  ������������ͼƬ�����ڴ��ַ
      //�����ⲿ�������ݵ��������ڲ�
       //memcpy(pQImage->bits(),pImgStart,imgwidth*imgheight*3);



        cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
        cvConvert(pFrImg, pFrameMat);
                  //��˹�˲��ȣ���ƽ��ͼ��
        cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

                  //��ǰ֡������ͼ���
        cvAbsDiff(pFrameMat, pBkMat, pFrMat);

                  //��ֵ��ǰ��ͼ
        //cvThreshold(pFrMat, pFrImg, 35, 255.0, CV_THRESH_BINARY);
        //���޸�
        cvThreshold(pFrMat, pFrImg, 55, 255.0, CV_THRESH_BINARY);

                  //M ������̬ѧ�˲���ȥ������

                  //ԭ���Ѿ�ע��--
                  cvErode(pFrImg, pFrImg, 0, 1);
                  cvDilate(pFrImg, pFrImg, 0, 1);

                  //���±���
        cvRunningAvg(pFrameMat, pBkMat, 0.003, 0);

        //M ������ת��Ϊͼ���ʽ��������ʾ
        //����ע��cvConvert(pBkMat, pBkImg);  //������ʾ��ע��

      }
     //����ԭͼ�����ת����
       // pBkImg->origin = pFrImg->origin = pFrame->origin;

      //  QImage showImage_1((const uchar*)pFrImg->imageData, pFrImg->width, pFrImg->height,
                   //      QImage::Format_RGB888);
        //showImage_1 = showImage_1.rgbSwapped();
     //   ui->label_process->setPixmap(QPixmap::fromImage(showImage_1));




       //�ڶ������� cvShowImage("foreground", pFrImg);



      /*M  CRect r1;
        r1.left = 10;
        r1.top = 10;
        r1.right = r1.left + 300;
        r1.bottom = r1.top + 250;
        m_imgFrame.DrawToHDC(pDC->GetSafeHdc(), r1);

        m_imgFr.CopyOf(pFrImg, 3);//IplImageת��ΪCvvImage
        CRect r2;
        r2.left = 320;
        r2.top = 10;
        r2.right = r2.left + 300;
        r2.bottom = r2.top + 250;
        m_imgFr.DrawToHDC(pDC->GetSafeHdc(), r2);

        m_imgBk.CopyOf(pBkImg, 3);//IplImageת��ΪCvvImage
        CRect r3;
        r3.left = 630;
        r3.top = 10;
        r3.right = r3.left + 300;
        r3.bottom = r3.top + 250;
        m_imgBk.DrawToHDC(pDC->GetSafeHdc(), r3);*/
        stor = cvCreateMemStorage(0);
        cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);

          // �ҵ���������
         cvFindContours( pFrImg, stor, &cont, sizeof(CvContour),
                          CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

        /*M  for(;cont;cont = cont->h_next)
          {
              // Number point must be more than or equal to 6 (for cvFitEllipse_32f).
              if( cont->total < 6 )
                  continue;
              // Draw current contour.
              cvDrawContours(img,cont,CV_RGB(255,0,0),CV_RGB(255,0,0),0,1, 8, cvPoint(0,0));
          }  // end of for-loop: "cont"*/

          // ֱ��ʹ��CONTOUR�еľ�����������
          for(;cont;cont = cont->h_next)
          {
            CvRect r = ((CvContour*)cont)->rect;//����ת��Ϊ��������
         //  if(r.height * r.width > 900) // ���С�ķ���������
          if((r.height * r.width > 900)&&(r.height * r.width < 40000)) // ���С�ķ���������
            {
                cvRectangle( pFrame, cvPoint(r.x,r.y),
                        cvPoint(r.x + r.width, r.y + r.height),
                        CV_RGB(0,0,255), 1, CV_AA,0);   //4���ʴ�ϸ
            }
          }
          /*cvDrawContours(pFrame, cont,
                           CV_RGB(0,0,255), CV_RGB(255, 0, 0),
                           2, 2, 8, cvPoint(0,0));*/
          // free memory



         //�ڶ������� cvShowImage("video", pFrame);
         //�ڶ������� cvShowImage("background", pBkImg);



          cvReleaseMemStorage(&stor);
          //cvReleaseImage( &pyr );
       //free(buf);*/




     //������ɰ�ͼ���лؿ������������ĵ�ַ
      memcpy(pImgStart,pQImage->bits(),imgwidth*imgheight*3);



   }
};

#endif











#if 1
//class CInvadeAlarm   //�˶�����--�����ּ����

int CInvadeAlarm::pnpoly(int nPointvert, int *Pointvertx, int *Pointverty, int testx, int testy)
{
  int i, j, c = 0;
  for (i = 0, j = nPointvert-1; i < nPointvert; j = i++)
  {
    if ( ((Pointverty[i]>testy) != (Pointverty[j]>testy)) &&
         (testx <(int)( (float)(Pointvertx[j]-Pointvertx[i]) * (float)(testy-Pointverty[i])/(float)(Pointverty[j]-Pointverty[i])+Pointvertx[i])) )
       c = !c;   //0ȡ��=��
  }
  return c;
}


//�˶������㷨--�����ּ��--ǰ��--����ͼ���ڴ�û���ͷ�
void CInvadeAlarm::InvadeAlarm(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool InvadeAlarmEnable ,\
                  int nPointvert, int *Pointvertx, int *Pointverty\
                  )
{
if(InvadeAlarmEnable)
   {

    nFrmNum++;
   //����ǵ�һ֡����Ҫ�����ڴ棬����ʼ��
    if(nFrmNum == 1)
    {
      pQImage = new QImage(QSize(imgwidth,imgheight), QImage::Format_RGB888);
        //����IplImageͼ��ͷ���ƶ�ͼ��ĳߴ磬��Ⱥ�ͨ������Ȼ����
      piplImg = cvCreateImageHeader(cvSize(imgwidth,imgheight), 8,3);
      piplImg->imageData = (char*)pQImage->bits();//��qImage��ַӳ���iplImg�ĵ�ַ

      //M �����ⲿ�������ݵ��������ڲ�
       memcpy(pQImage->bits(),pImgStart,imgwidth*imgheight*3);

      pFrame=piplImg;

       cvSmooth(pFrame, pFrame, CV_GAUSSIAN, 3, 0, 0);
       pBkImg = cvCreateImage(cvSize(imgwidth,imgheight),  IPL_DEPTH_8U,1);  //��������ͼ
       pFrImg = cvCreateImage(cvSize(imgwidth,imgheight),  IPL_DEPTH_8U,1);  //����ǰ��ͼ

       pBkMat = cvCreateMat(imgheight, imgwidth, CV_32FC1);  //����mat����ͼ
       pFrMat = cvCreateMat(imgheight, imgwidth, CV_32FC1);  //����matǰ��ͼ
       pFrameMat = cvCreateMat(imgheight, imgwidth, CV_32FC1); //����matȫͼ

        //ת���ɻҶ�ͼ����
        cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
        cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);

        cvConvert(pFrImg, pFrameMat);
        cvConvert(pFrImg, pFrMat);
        cvConvert(pFrImg, pBkMat);
     }
     else
     {
      //  unsigned char *pQImage =pQImage->bits();  ������������ͼƬ�����ڴ��ַ
      //M �����ⲿ�������ݵ��������ڲ�
       memcpy(pQImage->bits(),pImgStart,imgwidth*imgheight*3);



        cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
        cvConvert(pFrImg, pFrameMat);
                  //��˹�˲��ȣ���ƽ��ͼ��
        cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

                  //��ǰ֡������ͼ���
        cvAbsDiff(pFrameMat, pBkMat, pFrMat);

                  //��ֵ��ǰ��ͼ
        //M cvThreshold(pFrMat, pFrImg, 35, 255.0, CV_THRESH_BINARY);
        //M ���޸�
        cvThreshold(pFrMat, pFrImg, 55, 255.0, CV_THRESH_BINARY);

                  //������̬ѧ�˲���ȥ������

                  //ԭ���Ѿ�ע��--
                  cvErode(pFrImg, pFrImg, 0, 1);
                  cvDilate(pFrImg, pFrImg, 0, 1);

                  //���±���
        cvRunningAvg(pFrameMat, pBkMat, 0.003, 0);

        //������ת��Ϊͼ���ʽ��������ʾ
        //����ע��cvConvert(pBkMat, pBkImg);  //������ʾ��ע��

      }
     //M ����ԭͼ�����ת����
       //M pBkImg->origin = pFrImg->origin = pFrame->origin;

      //M  QImage showImage_1((const uchar*)pFrImg->imageData, pFrImg->width, pFrImg->height,
                   //      QImage::Format_RGB888);
        //showImage_1 = showImage_1.rgbSwapped();
     //   ui->label_process->setPixmap(QPixmap::fromImage(showImage_1));




       //�ڶ������� cvShowImage("foreground", pFrImg);



      /*M  CRect r1;
        r1.left = 10;
        r1.top = 10;
        r1.right = r1.left + 300;
        r1.bottom = r1.top + 250;
        m_imgFrame.DrawToHDC(pDC->GetSafeHdc(), r1);

        m_imgFr.CopyOf(pFrImg, 3);//IplImageת��ΪCvvImage
        CRect r2;
        r2.left = 320;
        r2.top = 10;
        r2.right = r2.left + 300;
        r2.bottom = r2.top + 250;
        m_imgFr.DrawToHDC(pDC->GetSafeHdc(), r2);

        m_imgBk.CopyOf(pBkImg, 3);//IplImageת��ΪCvvImage
        CRect r3;
        r3.left = 630;
        r3.top = 10;
        r3.right = r3.left + 300;
        r3.bottom = r3.top + 250;
        m_imgBk.DrawToHDC(pDC->GetSafeHdc(), r3);*/
        stor = cvCreateMemStorage(0);
        cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);

          // �ҵ���������
         cvFindContours( pFrImg, stor, &cont, sizeof(CvContour),
                          CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

        /*M  for(;cont;cont = cont->h_next)
          {
              // Number point must be more than or equal to 6 (for cvFitEllipse_32f).
              if( cont->total < 6 )
                  continue;
              // Draw current contour.
              cvDrawContours(img,cont,CV_RGB(255,0,0),CV_RGB(255,0,0),0,1, 8, cvPoint(0,0));
          }  // end of for-loop: "cont"*/

          // ֱ��ʹ��CONTOUR�еľ�����������
          for(;cont;cont = cont->h_next)
          {
            CvRect r = ((CvContour*)cont)->rect;//����ת��Ϊ��������
         //  if(r.height * r.width > 900) // ���С�ķ���������
          if((r.height * r.width > 900)&&(r.height * r.width < 40000)) // ���С�ķ���������
            {
          //�жϾ��������Ƿ��ڲ����߽���
          int AlarmPointX=(r.x + r.x+r.width)/2; //�����е�����
          int AlarmPointY=(r.y + r.y+r.height)/2;
          int ret=pnpoly(nPointvert,Pointvertx,Pointverty,AlarmPointX,AlarmPointY);

                  if(ret==1) //������������ʾ�����
                     {
                        cvRectangle( pFrame, cvPoint(r.x,r.y),
                                cvPoint(r.x + r.width, r.y + r.height),
                                CV_RGB(0,0,255), 2, CV_AA,0);  //4���ʴ�ϸ
                     }

            }
          }
          /*M
           cvDrawContours(pFrame, cont,
                           CV_RGB(0,0,255), CV_RGB(255, 0, 0),
                           2, 2, 8, cvPoint(0,0));
            */
          // free memory



         //�ڶ������� cvShowImage("video", pFrame);
         //�ڶ������� cvShowImage("background", pBkImg);



          cvReleaseMemStorage(&stor);
          //cvReleaseImage( &pyr );
       //free(buf);*/




     //������ɰ�ͼ���лؿ������������ĵ�ַ
      memcpy(pImgStart,pQImage->bits(),imgwidth*imgheight*3);



   }
};
#endif





















#if 1
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//w��Ϊ�˱���һ���ֵ���
//class CRemoveFog   //�˶�����--�����ּ����
void CRemoveFog::RemoveFog(unsigned char *pImage)
{
//ͼ��ֱ���������ɫͨ��
//    IplImage *src=cvCreateImageHeader(cvSize(imgwidth,imgheight), 8,3);   //�����ļ�ͷ
    src->imageData = (char*)pImage;  //����һһ���ڴ�ռ��ַӳ���iplImg��ͼ�����ݵ�ַ

         IplImage *dst1=NULL;
         IplImage *dst2=NULL;
         IplImage *dst3=NULL;
         IplImage *imgroi1;
         //dst1��ROI
         IplImage *imgroi2;
         //dst2��ROI
         IplImage *imgroi3;
         //dst3��ROI
         IplImage *roidark;
         //dark channel��ROI
         IplImage *dark_channel=NULL;
         //��ԭɫ�����ָ��
         IplImage *toushelv=NULL;
         //͸����

//ȥ���㷨����������ͨ��
         IplImage *j1=NULL;
         IplImage *j2=NULL;
         IplImage *j3=NULL;
//ȥ����ͼ��,��ͨ���ϲ���
         IplImage *dst=NULL;
//Դͼ��ROIλ���Լ���С
         CvRect ROI_rect;

//���������ͨ��
         dst1=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
         dst2=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
         dst3=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);

//Ϊ����ROI�����ڴ�
         imgroi1=cvCreateImage(cvSize(block,block),IPL_DEPTH_8U,1);
         imgroi2=cvCreateImage(cvSize(block,block),IPL_DEPTH_8U,1);
         imgroi3=cvCreateImage(cvSize(block,block),IPL_DEPTH_8U,1);
         roidark=cvCreateImage(cvSize(block,block),IPL_DEPTH_8U,1);

//Ϊj1 j2 j3�����С
         j1=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
         j2=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
         j3=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);

//Ϊ��ԭɫ����ָ������С
         dark_channel=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
//Ϊ͸����ָ������С
         toushelv=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
//dst�����С
         dst=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,3);
//��ԭ��ɫͼ��������ͨ��
         cvSplit(src,dst1,dst2,dst3,NULL);
//��ԭɫ
         ROI_rect.width=block;
         ROI_rect.height=block;
         ROI_rect.x=0;
         ROI_rect.y=0;


         int i;
         int j;
         double min1=0;
         double max1=0;
         double min2=0;
         double max2=0;
         double min3=0;
         double max3=0;
         double min=0;
         CvScalar value;
         for(i=0;i<src->width/block;i++)
         {        for(j=0;j<src->height/block;j++)
                   {
                            //�ֱ��������ͨ����ROI����Сֵ
                            cvSetImageROI(dst1,ROI_rect);
                            cvCopy(dst1,imgroi1,NULL);
                            cvMinMaxLoc(imgroi1,&min1,&max1,NULL,NULL);
                            cvSetImageROI(dst2,ROI_rect);
                            cvCopy(dst2,imgroi2,NULL);
                            cvMinMaxLoc(imgroi2,&min2,&max2,NULL,NULL);
                            cvSetImageROI(dst3,ROI_rect);
                            cvCopy(dst3,imgroi3,NULL);
                            cvMinMaxLoc(imgroi3,&min3,&max3,NULL,NULL);
                            //������ͨ������Сֵ����Сֵ
                            if(min1<min2)
                                     min=min1;
                            else
                                     min=min2;
                            if(min>min3)
                                     min=min3;//minΪ���ROI�а�ԭɫ
                            value=cvScalar(min,min,min,min);//min����value��
                            //min����dark_channel����Ӧ��ROI
                            cvSetImageROI(dark_channel,ROI_rect);
                            cvSet(roidark,value,NULL);
                            cvCopy(roidark,dark_channel,NULL);
                            //�ͷŸ���ROI
                            cvResetImageROI(dst1);
                            cvResetImageROI(dst2);
                            cvResetImageROI(dst3);
                            cvResetImageROI(dark_channel);
                            //ת����һ��ROI
                            ROI_rect.x=block*i;
                            ROI_rect.y=block*j;
                   }
         }
         //M ���氵ԭɫ�����ͼ��
//        cvSaveImage("e:/dark_channel_prior.jpg",dark_channel);
//M ���õõ��İ�ԭɫ����dark_channel_prior.jpg�������ǿ
         double min_dark;
         double max_dark;
         CvPoint min_loc;
         CvPoint max_loc;//max_loc�ǰ�ԭɫ��������һС���ԭ����
         cvMinMaxLoc(dark_channel,&min_dark,&max_dark,&min_loc,&max_loc,NULL);
  //       cout<<max_loc.x<<" "<<max_loc.y<<endl;
         ROI_rect.x=max_loc.x;
         ROI_rect.y=max_loc.y;
         double A_dst1;//���������ɷֵĹ���ֵ
         double dst1_min;
         double A_dst2;
         double dst2_min;
         double A_dst3;
         double dst3_min;
         cvSetImageROI(dst1,ROI_rect);
//�������ķ����������ǿ����ֵ
         cvCopy(dst1,imgroi1,NULL);
         cvMinMaxLoc(imgroi1,&dst1_min,&A_dst1,NULL,NULL);
         cvSetImageROI(dst2,ROI_rect);
         cvCopy(dst2,imgroi2,NULL);
         cvMinMaxLoc(imgroi2,&dst2_min,&A_dst2,NULL,NULL);
         cvSetImageROI(dst3,ROI_rect);
         cvCopy(dst3,imgroi3,NULL);
         cvMinMaxLoc(imgroi3,&dst3_min,&A_dst3,NULL,NULL);
  //       cout<<A_dst1<<" "<<A_dst2<<" "<<A_dst3<<endl;//����ֵΪ������ǿ�ȹ���ֵ
//��͸����
         int k;
         int l;
         CvScalar m;
         CvScalar n;//��ԭɫ�����Ԫ��ֵ

         for(k=0;k<src->height;k++)
         {
                   for(l=0;l<src->width;l++)
                   {
                            m=cvGet2D(dark_channel,k,l);
                            n=cvScalar(255-w*m.val[0]);
                            //wĿ���Ǳ���һ���ֵ���,ʹͼ��������ʵЩ
                            cvSet2D(toushelv,k,l,n);
                   }
         }
      //   cvSaveImage("e:/toushelv.jpg",toushelv);

//������ͼ��
         int p,q;
         double tx;
         double jj1,jj2,jj3;
         CvScalar ix,jx;
         for(p=0;p<src->height;p++)
         {
                   for(q=0;q<src->width;q++)
                   {
                            tx=cvGetReal2D(toushelv,p,q);
                            tx=tx/255;
                            if(tx<0.1)
                                     tx=0.1;
                            ix=cvGet2D(src,p,q);
                            jj1=(ix.val[0]-A_dst1)/tx+A_dst1;//���������ģ������,��ԭ������ͼ��
                            jj2=(ix.val[1]-A_dst2)/tx+A_dst2;
                            jj3=(ix.val[2]-A_dst3)/tx+A_dst3;
                            jx=cvScalar(jj1,jj2,jj3,0.0);
                            cvSet2D(dst,p,q,jx);
                   }
         }
      //   cvSaveImage("e:/removed_haze.jpg",dst);

//�ͷ�ָ��
         cvReleaseImage(&dst1);
         cvReleaseImage(&dst2);
         cvReleaseImage(&dst3);
         cvReleaseImage(&imgroi1);
         cvReleaseImage(&imgroi2);
         cvReleaseImage(&imgroi3);
         cvReleaseImage(&roidark);
         cvReleaseImage(&dark_channel);
         cvReleaseImage(&toushelv);
         cvReleaseImage(&j1);
         cvReleaseImage(&j2);
         cvReleaseImage(&j3);


   //M ��������п�����ԭ��ַ
   memcpy(src->imageData,dst->imageData,(src->width)*(src->height)*3*sizeof(char));


      //  src->imageData=NULL;  //�ѵ�ַ��ֵ��--���ͷ�--�����Ѵ��ݽ������ڴ�Ҳ�ͷŵ�
      //  cvReleaseImage(&src);
          cvReleaseImage(&dst);
      //  return dst;
};
#endif



/*M
Threshold

������Ԫ�ؽ��й̶���ֵ����

void cvThreshold( const CvArr* src, CvArr* dst, double threshold, double max_value, int threshold_type );

   src   ԭʼ���� (��ͨ�� , 8-bit of 32-bit ������).
   dst   ������飬������ src ������һ�£�����Ϊ 8-bit.
   threshold  ��ֵ
   max_value  ʹ�� CV_THRESH_BINARY �� CV_THRESH_BINARY_INV �����ֵ.
   threshold_type  ��ֵ���ͣ�
                        CV_THRESH_BINARY   dst(x,y) = max_value, if src(x,y)>threshold 0, otherwise.
                        CV_THRESH_BINARY_INV   dst(x,y) = 0, if src(x,y)>threshold; dst(x,y) = max_value, otherwise.
                        CV_THRESH_TRUNC   dst(x,y) = threshold, if src(x,y)>threshold;   dst(x,y) = src(x,y), otherwise.
                        CV_THRESH_TOZERO   dst(x,y) = src(x,y), if (x,y)>threshold ;  dst(x,y) = 0, otherwise.
                        CV_THRESH_OTSU
                        CV_THRESH_TOZERO_INV dst(x,y) = 0, if src(x,y)>threshold ;  dst(x,y) = src(x,y), otherwise
*/

























MyImageProcess::MyImageProcess()
{
}


#endif
