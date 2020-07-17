#if 1
#include "DIP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //memset头文件


//调用方法 //函数调用
//M ImageProcess(qImage->bits(),frame->width,frame->height,3,false,true,200);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//图像极性翻转--白热--黑热转换   PolarityFlip
void PolarityFlip(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
if(EnableFlag)
{
    //极性翻转--黑热白热
    unsigned char *ptmpImg=pImgStart;     //指针复位
    for(int i=0;i<imgwidth*imgheight*3;i++)
    {
        *ptmpImg=(255-*ptmpImg);    //极性翻转 0-255一共256
        ptmpImg++;
    }
}
}




//图像水平翻转--左右镜像
void HorizontalFlip(unsigned char* pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
if(EnableFlag)
{
    //水平翻转--用swap交换法，耗时大，但节约内存
    unsigned char* ptmpImg=pImgStart;       //指针复位
    unsigned char* ptmpImgLine=pImgStart;   //新行头地址
    unsigned char temp;     //存放临时交换数据
    for(int j=0;j<imgheight;j++)
    {
    //M for(int i=1;i<imgwidth;i++)   //如果这样怎么也翻转不了，因为图像左/右半边一共翻转两回，相当于没有翻转
       for(int i=1;i<imgwidth/2;i++)
       {
        //M 调试容易出错--总是成对称图形,原因是在原图操作--应该在临时图像操作，再覆盖回原图，或者设置临时变量
        //先用单点交换法，与拷贝副本图像然后交换，，，比较效率差别
        temp= *ptmpImg;
        *ptmpImg=*(ptmpImgLine+3*(imgwidth-i));      //R
        *(ptmpImgLine+3*(imgwidth-i))=temp;

         temp= *(ptmpImg+1);
        *(ptmpImg+1)=*(ptmpImgLine+3*(imgwidth-i)+1);  //G
        *(ptmpImgLine+3*(imgwidth-i)+1)=temp;

         temp= *(ptmpImg+2);
        *(ptmpImg+2)=*(ptmpImgLine+3*(imgwidth-i)+2);  //B
        *(ptmpImgLine+3*(imgwidth-i)+2)=temp;

         ptmpImg=ptmpImg+3;   //下一个像素值
       }
        ptmpImgLine=ptmpImgLine+3*imgwidth;   //新行头地址递增
        ptmpImg=ptmpImgLine;
     //   ptmpImg++;
    }
}
}


//图像垂直翻转--上下镜像
void VerticalFlip(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
if(EnableFlag)
{
    //垂直翻转
    unsigned char* ptmpImg=pImgStart;     //指针复位
    unsigned char* ptmpImgLine=pImgStart;   //新行头地址
    unsigned char* ptmpImgLineLast=ptmpImgLine+imgwidth*3*(imgheight-1);   //最后一行地址
    unsigned char temp2;     //存放临时交换数据
    for(int j=0;j<imgheight/2;j++)
    {
       for(int i=0;i<imgwidth*3;i++)
       {
         //先用单点交换法，与拷贝副本图像然后交换，，，比较效率差别
         temp2= *ptmpImg;
         *ptmpImg=*(ptmpImgLineLast-j*imgwidth*3+i);
         *(ptmpImgLineLast-j*imgwidth*3+i)=temp2;
         ptmpImg++;
       }
       ptmpImgLine=ptmpImgLine+3*imgwidth;   //新行头地址递增
       ptmpImg=ptmpImgLine;
    }
}
}


//亮度对比度一起调整调整---为了提高速度可以改用查找表实现。亮度brightness对比度contrast
void AdjBrightnessContrast(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                           int BrightnessValue,\
                           int ContrastValue,\
                           bool EnableFlag\
                           )
{
if(EnableFlag)
{
if(ContrastValue==10&&BrightnessValue==0)   { return; } //如果传参亮度对比度变化为0，直接退出，节省时间

//alpha value [1.0-3.0]: 2.2   对比度范围
//the beta value [0-100]: 50   亮度范围

float alpha=(float)ContrastValue/10.0;
unsigned char* ptmpImg=pImgStart;     //指针复位
for(int i=0;i<imgwidth*imgheight*3;i++)
{
   float val=alpha*(*ptmpImg)+BrightnessValue;
   if(val>255)
       val=255;
   if(val<0)
       val=0;
   *ptmpImg=(unsigned char)val;    //极性翻转 0-255一共256
   ptmpImg++;
}

}
}





//热成像灰度图像转伪彩色--//彩虹编码1  添加其他编码
void GrayToPseudocolor(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
//bool EnableFlag=false;   //伪彩色使能控制
if(EnableFlag)
{
   unsigned char* pTempBit=(unsigned char*)pImgStart;
   for(int i=0;i<imgwidth*imgheight;i++)
   {
       //彩虹编码1
       unsigned char color=(*pTempBit);//
       if((*pTempBit)<= 32 )
       {
           *(pTempBit+0)=0;      //G绿
           *(pTempBit+1)=0;      //G绿
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



//彩色图像转黑白灰度图  彩色RGB转RGB相同的灰度图
void ColorToGray(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool EnableFlag \
                  )
{
if(EnableFlag)
{
    //彩色转灰度
    unsigned char *ptmpImg=pImgStart;     //指针复位
    for(int i=0;i<imgwidth*imgheight;i++)
    {
        unsigned char tmpGray=((*ptmpImg)*28+(*(ptmpImg+1))*151+(*(ptmpImg+2))*77)>>8;
        *ptmpImg=tmpGray;
        *(ptmpImg+1)=tmpGray;
        *(ptmpImg+2)=tmpGray;

        ptmpImg=ptmpImg+3;
    }

/*M
彩色图转灰度图：
1.浮点算法：Gray=R*0.3+G*0.59+B*0.11
2.整数方法：Gray=(R*30+G*59+B*11)/100
3.移位方法：Gray =(R*28+G*151+B*77)>>8;
4.平均值法：Gray=(R+G+B)/3;
5.仅取绿色：Gray=G；
*/
}
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//图像增强--单通道--多次分别处理--基于直方图均衡化的图像处理Histogram equalization
void EqualizeHist(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                   bool EnableFlag \
                   )
{
if(EnableFlag)
  {
    //M 有RGB三个通道,黑白图像,但是
    //M 彩色图片直方图均衡化一般不能直接对R、G、B三个分量操作，而要将RGB转换成HSV来对V分量进行直方图均衡化。
   for(unsigned char nchannel=0;nchannel<=2;nchannel++)  //三个通道
   {


         unsigned char *ptemp=pImgStart+nchannel;  //各个通道起始地址
         int hist[256];           //M 灰度级表
         float  fpHist[256];      //M 各个灰度值概率表
         float eqHistTemp[256];
         int eqHist[256];         //M 重新映射后灰度级表,
         int size = imgwidth *imgheight;    //图像大小,单通道

          memset(&hist,0x00,sizeof(int)*256);          //M  没有必要初始化
//        memset(&fpHist,0x00,sizeof(float)*256);      //M  没有必要初始化
//        memset(&eqHistTemp,0x00,sizeof(float)*256);  //M  没有必要初始化


         for (int i = 0;i < size; i++) //计算差分矩阵直方图
         {

           unsigned char GrayIndex = *ptemp;
           hist[GrayIndex] ++ ;
           ptemp=ptemp+3;
         }
         ptemp=pImgStart+nchannel;//M 地址复位


         for (int i = 0; i< 256; i++)   // 计算灰度分布密度
         {
          fpHist[i] = (float)hist[i] / (float)size;
         }


         for (int i = 1; i< 256; i++)   // 计算累计直方图分布
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


         //M 累计分布取整，保存计算出来的灰度映射关系
         for (int i = 0; i< 256; i++)
         {
          eqHist[i] = (int)(255.0 * eqHistTemp[i] + 0.5);
         }


        for (int i = 0;i < size; i++)  //进行灰度映射 均衡化
         {
           unsigned char GrayIndex = *ptemp;
           *ptemp = eqHist[GrayIndex];
            ptemp=ptemp+3;
         }

    }

  }
}


//黑白图像或者灰度图像处理算法:
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//图像二值化  --根据传进来的参数对RGB二值化--
void ThresholdRGBToBin(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                     unsigned char  ThresholdValue, \
                     bool EnableFlag \
                  )
{
if(EnableFlag)
{
    unsigned char *ptmpImg=pImgStart;     //指针复位
    for(int i=0;i<imgwidth*imgheight*3;i++)
    {
        unsigned char tmpGray=((*ptmpImg)>ThresholdValue)?255:0;
        *ptmpImg=tmpGray;
        ptmpImg++;
    }
}
}



//图像二值化  --根据传进来的参数对Gray二值化为BIN--  单通道灰度图二值话
void ThresholdGrayToBin(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                     unsigned char  ThresholdValue, \
                     bool EnableFlag \
                  )
{
if(EnableFlag)
{
    unsigned char *ptmpImg=pImgStart;     //指针复位
    for(int i=0;i<imgwidth*imgheight;i++)
    {
        unsigned char tmpGray=((*ptmpImg)>ThresholdValue)?255:0;
        *ptmpImg=tmpGray;
        ptmpImg++;
    }
}
}




























//火灾报警算法
//M 火灾报警算法--内部new无法判断delete,用全局mem--返回运动变化率换思路直接通过地址存为数组
//M 不同地方调用--前景图不同,需要每次初始化
//M 上一帧作为背景层(0-2...,偶数地址)  当前帧作为前景层(1-3-5,奇数地址),
//火灾报警算法
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
    //M 闭合的高温区边缘 调试  //if（上下都大于阈值||左右都大于阈值）--->那么这个点就是边缘点
    //bool HotEdgeEnable=true;  //控制高温边缘

    if(TemperatureAlarmEnable)
    {
      int SumHeigArea=0;     //高于阈值的像素
      int TempSummoverate=0; //变化的像素
     // unsigned char high=230;//高温阈值
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

      pTempBit=(unsigned char*)(pImgStart+3*imgwidth+3);//起始地址
      //边缘暂不判断
      unsigned char* pTempMoverateFrontBackImg=pMoverateFrontBackImg;  //保存为临时指针
      for(int col=0;col<imgheight-2;col++) //列--高
      {
          for(int row=0;row<imgwidth-2;row++) //行--宽
          {
           if(MoveAlarmEnable)  //运动报警使能打开
           {    //高温点面积统计
              if(*(pTempBit)>EdgeGrayValue)
              {
                    SumHeigArea=SumHeigArea+1;
                    *(pTempMoverateFrontBackImg+1)=255;   //二值化高于阈值--当前帧填充前景图层
              }else { *(pTempMoverateFrontBackImg+1)=0;}; //记得复位否则多次之后全是255

           //M   像素遍历完成后--情景图与背景图不相同,即为变化的部分
           //  printf("pTempMoverateFrontBackImg=%d\n",*pTempMoverateFrontBackImg);
            if(*pTempMoverateFrontBackImg!=*(pTempMoverateFrontBackImg+1))//如果前后两个像素值不相等
             {
                TempSummoverate=TempSummoverate+1;  //变化像素统计
                *pTempMoverateFrontBackImg=*(pTempMoverateFrontBackImg+1);  //把前景图层存为背景,供下一帧处理调用
             }
           pTempMoverateFrontBackImg=pTempMoverateFrontBackImg+2;  //前景图与背景图交错存储,   //注意要跨个像素,否则没有意义,然后总遍历数/2,一

           }//否则只是单纯统计高温面积
           else{ if(*(pTempBit)>EdgeGrayValue) { SumHeigArea=SumHeigArea+1; }};

          left=(*(pTempBit-3));
          right=(*(pTempBit+3));
          //   up=(*(pTempBit-3*imgwidth));
          //   down=(*(pTempBit+3*imgwidth));
       //   if((left>high&&right>high)||(up>high&&down>high))  //为什么不行
          if((left<EdgeGrayValue&&right>EdgeGrayValue)||(left>EdgeGrayValue&&right<EdgeGrayValue))\
       //     ||(up>EdgeGrayValue&&down<EdgeGrayValue)||(up<EdgeGrayValue&&down>EdgeGrayValue))
           {
             // *(pTempBit)=250;
              *(pTempBit+1)=10;      //高温边界点//G绿
              *(pTempBit+2)=10;
           }



          //M 闭合边缘--判断上下像素--默认可以不用--浪费时间
          up=(*(pTempBit-3*imgwidth));
          down=(*(pTempBit+3*imgwidth));
          if((up>EdgeGrayValue&&down<EdgeGrayValue)||(up<EdgeGrayValue&&down>EdgeGrayValue))
          {
           //  *(pTempBit)=250;
             *(pTempBit+1)=10;      //高温边界点//G绿
             *(pTempBit+2)=10;
          }


          //下一个像素点
            pTempBit=pTempBit+3;
          }
       //换行
       //M pTempBit=pTempBit+3*imgwidth;---换行思路错误
       pTempBit=pTempBit+6;  //下一行只间隔边缘两个点,六个像素
     }

      //把高温面积存进指针序列
     // *pHeighSumArea=SumHeigArea;  //把高温面积赋给地址，外部可以读取
      for(int i=1;i<=49;i++)       //返回值逐步存入Buff
      {
        *(pHeighSumArea+i-1)=*(pHeighSumArea+i);
      }
      *(pHeighSumArea+49)=SumHeigArea;



      //printf("%d:TempSummoverate=%d\n",numprint++,TempSummoverate);
      //把得到的变化像素数量,移位存入数组指针
       for(int i=1;i<=49;i++)       //返回值逐步存入Buff
       {
         *(pMoverateReturnValue+i-1)=*(pMoverateReturnValue+i);
       }
       *(pMoverateReturnValue+49)=TempSummoverate;
       //返回运动变化率换思路直接通过地址存为数组


     }
}
}


//16位的火灾报警算法测试
void FireAlarmBit16(unsigned  int *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool TemperatureAlarmEnable,\
                  unsigned  int EdgeGrayValue,\
                  unsigned  int* pHeighSumArea\
                  )
{

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//M 闭合的高温区边缘 调试  //if（上下都大于阈值||左右都大于阈值）--->那么这个点就是边缘点
//bool HotEdgeEnable=true;  //控制高温边缘

if(TemperatureAlarmEnable)
{
  int SumHeigArea=0;     //高于阈值的像素
  int TempSummoverate=0; //变化的像素
 // unsigned char high=230;//高温阈值
  unsigned  int left,right,up,down;
  unsigned  int* pTempBit;
  unsigned  int* pstart;
  unsigned  int* pend;

  pstart=(unsigned int*)pImgStart;
//    printf(" pstart=%d ",pstart);
  pend=(unsigned int*)(pImgStart+imgheight*imgwidth*3);

  pTempBit=(unsigned int*)(pImgStart+3*imgwidth+3);//起始地址
  //边缘暂不判断

  for(int col=0;col<imgheight-2;col++) //列--高
  {
      for(int row=0;row<imgwidth-2;row++) //行--宽
      {
       //if(MoveAlarmEnable)  //运动报警使能打开
        if(0)  //运动报警使能打开
       {    //高温点面积统计
          if(*(pTempBit)>EdgeGrayValue)
          {
                SumHeigArea=SumHeigArea+1;

          }
       }//否则只是单纯统计高温面积
       else{ if(*(pTempBit)>EdgeGrayValue) { SumHeigArea=SumHeigArea+1; }};

      left=(*(pTempBit-3));
      right=(*(pTempBit+3));
     if((left<EdgeGrayValue&&right>EdgeGrayValue)||(left>EdgeGrayValue&&right<EdgeGrayValue))\
     {
         // *(pTempBit)=250;
          *(pTempBit+2)=16383;   //设置为一个特殊的值最小或最大   //高温边界点//G绿  2^14 = 16 384
         // *(pTempBit+2)=10;
       }


      //M 闭合边缘--判断上下像素--默认可以不用--浪费时间
      up=(*(pTempBit-3*imgwidth));
      down=(*(pTempBit+3*imgwidth));
      if((up>EdgeGrayValue&&down<EdgeGrayValue)||(up<EdgeGrayValue&&down>EdgeGrayValue))
      {
       //  *(pTempBit)=250;
       //  *(pTempBit+1)=10;      //高温边界点//G绿
         *(pTempBit+2)=16383;
      }



      //下一个像素点
        pTempBit=pTempBit+3;
      }
   //换行
   //M pTempBit=pTempBit+3*imgwidth;---换行思路错误
   pTempBit=pTempBit+6;  //下一行只间隔边缘两个点,六个像素
 }

  //把高温面积存进指针序列
 // *pHeighSumArea=SumHeigArea;  //把高温面积赋给地址，外部可以读取
  for(int i=1;i<=49;i++)       //返回值逐步存入Buff
  {
    *(pHeighSumArea+i-1)=*(pHeighSumArea+i);
  }
    *(pHeighSumArea+49)=SumHeigArea;
 }
}














#if 1



//class CMotiontTacking   //运动跟踪类--函数功能直接在内部实现  可以封装成类--完成后总结
//M 运动跟踪算法--前景--背景图层内存没有释放
void CMotiontTacking::MotiontTacking(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool MotiontTackingEnable \
                  )
{
if(true==MotiontTackingEnable)
   {

    nFrmNum++;
   //如果是第一帧，需要申请内存，并初始化
    if(nFrmNum == 1)
    {
      pQImage = new QImage(QSize(imgwidth,imgheight), QImage::Format_RGB888);
        //创建IplImage图像头，制定图像的尺寸，深度和通道数；然后由
      piplImg = cvCreateImageHeader(cvSize(imgwidth,imgheight), 8,3);
      piplImg->imageData = (char*)pQImage->bits();//把qImage地址映射成iplImg的地址
      piplImg->imageData = (char*)pImgStart;


      //拷贝外部传入数据到本函数内部
       memcpy(pQImage->bits(),pImgStart,imgwidth*imgheight*3);

      pFrame=piplImg;

       cvSmooth(pFrame, pFrame, CV_GAUSSIAN, 3, 0, 0);
       pBkImg = cvCreateImage(cvSize(imgwidth,imgheight),  IPL_DEPTH_8U,1);  //创建背景图
       pFrImg = cvCreateImage(cvSize(imgwidth,imgheight),  IPL_DEPTH_8U,1);  //创建前景图

       pBkMat = cvCreateMat(imgheight, imgwidth, CV_32FC1);  //创建mat背景图
       pFrMat = cvCreateMat(imgheight, imgwidth, CV_32FC1);  //创建mat前景图
       pFrameMat = cvCreateMat(imgheight, imgwidth, CV_32FC1); //创建mat全图

        //转化成灰度图处理
        cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
        cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);

        cvConvert(pFrImg, pFrameMat);
        cvConvert(pFrImg, pFrMat);
        cvConvert(pFrImg, pBkMat);
     }
     else
     {
      //  unsigned char *pQImage =pQImage->bits();  本函数创建的图片数据内存地址
      //拷贝外部传入数据到本函数内部
       //memcpy(pQImage->bits(),pImgStart,imgwidth*imgheight*3);



        cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
        cvConvert(pFrImg, pFrameMat);
                  //高斯滤波先，以平滑图像
        cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

                  //当前帧跟背景图相减
        cvAbsDiff(pFrameMat, pBkMat, pFrMat);

                  //二值化前景图
        //cvThreshold(pFrMat, pFrImg, 35, 255.0, CV_THRESH_BINARY);
        //新修改
        cvThreshold(pFrMat, pFrImg, 55, 255.0, CV_THRESH_BINARY);

                  //M 进行形态学滤波，去掉噪音

                  //原来已经注释--
                  cvErode(pFrImg, pFrImg, 0, 1);
                  cvDilate(pFrImg, pFrImg, 0, 1);

                  //更新背景
        cvRunningAvg(pFrameMat, pBkMat, 0.003, 0);

        //M 将背景转化为图像格式，用以显示
        //二次注释cvConvert(pBkMat, pBkImg);  //不用显示先注释

      }
     //保持原图像的旋转方向
       // pBkImg->origin = pFrImg->origin = pFrame->origin;

      //  QImage showImage_1((const uchar*)pFrImg->imageData, pFrImg->width, pFrImg->height,
                   //      QImage::Format_RGB888);
        //showImage_1 = showImage_1.rgbSwapped();
     //   ui->label_process->setPixmap(QPixmap::fromImage(showImage_1));




       //第二次屏蔽 cvShowImage("foreground", pFrImg);



      /*M  CRect r1;
        r1.left = 10;
        r1.top = 10;
        r1.right = r1.left + 300;
        r1.bottom = r1.top + 250;
        m_imgFrame.DrawToHDC(pDC->GetSafeHdc(), r1);

        m_imgFr.CopyOf(pFrImg, 3);//IplImage转换为CvvImage
        CRect r2;
        r2.left = 320;
        r2.top = 10;
        r2.right = r2.left + 300;
        r2.bottom = r2.top + 250;
        m_imgFr.DrawToHDC(pDC->GetSafeHdc(), r2);

        m_imgBk.CopyOf(pBkImg, 3);//IplImage转换为CvvImage
        CRect r3;
        r3.left = 630;
        r3.top = 10;
        r3.right = r3.left + 300;
        r3.bottom = r3.top + 250;
        m_imgBk.DrawToHDC(pDC->GetSafeHdc(), r3);*/
        stor = cvCreateMemStorage(0);
        cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);

          // 找到所有轮廓
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

          // 直接使用CONTOUR中的矩形来画轮廓
          for(;cont;cont = cont->h_next)
          {
            CvRect r = ((CvContour*)cont)->rect;//子类转换为父类例子
         //  if(r.height * r.width > 900) // 面积小的方形抛弃掉
          if((r.height * r.width > 900)&&(r.height * r.width < 40000)) // 面积小的方形抛弃掉
            {
                cvRectangle( pFrame, cvPoint(r.x,r.y),
                        cvPoint(r.x + r.width, r.y + r.height),
                        CV_RGB(0,0,255), 1, CV_AA,0);   //4画笔粗细
            }
          }
          /*cvDrawContours(pFrame, cont,
                           CV_RGB(0,0,255), CV_RGB(255, 0, 0),
                           2, 2, 8, cvPoint(0,0));*/
          // free memory



         //第二次屏蔽 cvShowImage("video", pFrame);
         //第二次屏蔽 cvShowImage("background", pBkImg);



          cvReleaseMemStorage(&stor);
          //cvReleaseImage( &pyr );
       //free(buf);*/




     //处理完成把图像有回拷贝给传进来的地址
      memcpy(pImgStart,pQImage->bits(),imgwidth*imgheight*3);



   }
};

#endif











#if 1
//class CInvadeAlarm   //运动跟踪--改入侵检测类

int CInvadeAlarm::pnpoly(int nPointvert, int *Pointvertx, int *Pointverty, int testx, int testy)
{
  int i, j, c = 0;
  for (i = 0, j = nPointvert-1; i < nPointvert; j = i++)
  {
    if ( ((Pointverty[i]>testy) != (Pointverty[j]>testy)) &&
         (testx <(int)( (float)(Pointvertx[j]-Pointvertx[i]) * (float)(testy-Pointverty[i])/(float)(Pointverty[j]-Pointverty[i])+Pointvertx[i])) )
       c = !c;   //0取反=？
  }
  return c;
}


//运动跟踪算法--改入侵检测--前景--背景图层内存没有释放
void CInvadeAlarm::InvadeAlarm(unsigned char *pImgStart,int imgwidth,int imgheight,int ImgDepth,\
                  bool InvadeAlarmEnable ,\
                  int nPointvert, int *Pointvertx, int *Pointverty\
                  )
{
if(InvadeAlarmEnable)
   {

    nFrmNum++;
   //如果是第一帧，需要申请内存，并初始化
    if(nFrmNum == 1)
    {
      pQImage = new QImage(QSize(imgwidth,imgheight), QImage::Format_RGB888);
        //创建IplImage图像头，制定图像的尺寸，深度和通道数；然后由
      piplImg = cvCreateImageHeader(cvSize(imgwidth,imgheight), 8,3);
      piplImg->imageData = (char*)pQImage->bits();//把qImage地址映射成iplImg的地址

      //M 拷贝外部传入数据到本函数内部
       memcpy(pQImage->bits(),pImgStart,imgwidth*imgheight*3);

      pFrame=piplImg;

       cvSmooth(pFrame, pFrame, CV_GAUSSIAN, 3, 0, 0);
       pBkImg = cvCreateImage(cvSize(imgwidth,imgheight),  IPL_DEPTH_8U,1);  //创建背景图
       pFrImg = cvCreateImage(cvSize(imgwidth,imgheight),  IPL_DEPTH_8U,1);  //创建前景图

       pBkMat = cvCreateMat(imgheight, imgwidth, CV_32FC1);  //创建mat背景图
       pFrMat = cvCreateMat(imgheight, imgwidth, CV_32FC1);  //创建mat前景图
       pFrameMat = cvCreateMat(imgheight, imgwidth, CV_32FC1); //创建mat全图

        //转化成灰度图处理
        cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
        cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);

        cvConvert(pFrImg, pFrameMat);
        cvConvert(pFrImg, pFrMat);
        cvConvert(pFrImg, pBkMat);
     }
     else
     {
      //  unsigned char *pQImage =pQImage->bits();  本函数创建的图片数据内存地址
      //M 拷贝外部传入数据到本函数内部
       memcpy(pQImage->bits(),pImgStart,imgwidth*imgheight*3);



        cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
        cvConvert(pFrImg, pFrameMat);
                  //高斯滤波先，以平滑图像
        cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

                  //当前帧跟背景图相减
        cvAbsDiff(pFrameMat, pBkMat, pFrMat);

                  //二值化前景图
        //M cvThreshold(pFrMat, pFrImg, 35, 255.0, CV_THRESH_BINARY);
        //M 新修改
        cvThreshold(pFrMat, pFrImg, 55, 255.0, CV_THRESH_BINARY);

                  //进行形态学滤波，去掉噪音

                  //原来已经注释--
                  cvErode(pFrImg, pFrImg, 0, 1);
                  cvDilate(pFrImg, pFrImg, 0, 1);

                  //更新背景
        cvRunningAvg(pFrameMat, pBkMat, 0.003, 0);

        //将背景转化为图像格式，用以显示
        //二次注释cvConvert(pBkMat, pBkImg);  //不用显示先注释

      }
     //M 保持原图像的旋转方向
       //M pBkImg->origin = pFrImg->origin = pFrame->origin;

      //M  QImage showImage_1((const uchar*)pFrImg->imageData, pFrImg->width, pFrImg->height,
                   //      QImage::Format_RGB888);
        //showImage_1 = showImage_1.rgbSwapped();
     //   ui->label_process->setPixmap(QPixmap::fromImage(showImage_1));




       //第二次屏蔽 cvShowImage("foreground", pFrImg);



      /*M  CRect r1;
        r1.left = 10;
        r1.top = 10;
        r1.right = r1.left + 300;
        r1.bottom = r1.top + 250;
        m_imgFrame.DrawToHDC(pDC->GetSafeHdc(), r1);

        m_imgFr.CopyOf(pFrImg, 3);//IplImage转换为CvvImage
        CRect r2;
        r2.left = 320;
        r2.top = 10;
        r2.right = r2.left + 300;
        r2.bottom = r2.top + 250;
        m_imgFr.DrawToHDC(pDC->GetSafeHdc(), r2);

        m_imgBk.CopyOf(pBkImg, 3);//IplImage转换为CvvImage
        CRect r3;
        r3.left = 630;
        r3.top = 10;
        r3.right = r3.left + 300;
        r3.bottom = r3.top + 250;
        m_imgBk.DrawToHDC(pDC->GetSafeHdc(), r3);*/
        stor = cvCreateMemStorage(0);
        cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);

          // 找到所有轮廓
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

          // 直接使用CONTOUR中的矩形来画轮廓
          for(;cont;cont = cont->h_next)
          {
            CvRect r = ((CvContour*)cont)->rect;//子类转换为父类例子
         //  if(r.height * r.width > 900) // 面积小的方形抛弃掉
          if((r.height * r.width > 900)&&(r.height * r.width < 40000)) // 面积小的方形抛弃掉
            {
          //判断矩形中心是否在布防边界内
          int AlarmPointX=(r.x + r.x+r.width)/2; //矩形中点坐标
          int AlarmPointY=(r.y + r.y+r.height)/2;
          int ret=pnpoly(nPointvert,Pointvertx,Pointverty,AlarmPointX,AlarmPointY);

                  if(ret==1) //如果是则绘制提示框矩形
                     {
                        cvRectangle( pFrame, cvPoint(r.x,r.y),
                                cvPoint(r.x + r.width, r.y + r.height),
                                CV_RGB(0,0,255), 2, CV_AA,0);  //4画笔粗细
                     }

            }
          }
          /*M
           cvDrawContours(pFrame, cont,
                           CV_RGB(0,0,255), CV_RGB(255, 0, 0),
                           2, 2, 8, cvPoint(0,0));
            */
          // free memory



         //第二次屏蔽 cvShowImage("video", pFrame);
         //第二次屏蔽 cvShowImage("background", pBkImg);



          cvReleaseMemStorage(&stor);
          //cvReleaseImage( &pyr );
       //free(buf);*/




     //处理完成把图像有回拷贝给传进来的地址
      memcpy(pImgStart,pQImage->bits(),imgwidth*imgheight*3);



   }
};
#endif





















#if 1
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//w是为了保留一部分的雾
//class CRemoveFog   //运动跟踪--改入侵检测类
void CRemoveFog::RemoveFog(unsigned char *pImage)
{
//图像分别有三个颜色通道
//    IplImage *src=cvCreateImageHeader(cvSize(imgwidth,imgheight), 8,3);   //创建文件头
    src->imageData = (char*)pImage;  //把任一一段内存空间地址映射成iplImg的图像数据地址

         IplImage *dst1=NULL;
         IplImage *dst2=NULL;
         IplImage *dst3=NULL;
         IplImage *imgroi1;
         //dst1的ROI
         IplImage *imgroi2;
         //dst2的ROI
         IplImage *imgroi3;
         //dst3的ROI
         IplImage *roidark;
         //dark channel的ROI
         IplImage *dark_channel=NULL;
         //暗原色先验的指针
         IplImage *toushelv=NULL;
         //透射率

//去雾算法运算后的三个通道
         IplImage *j1=NULL;
         IplImage *j2=NULL;
         IplImage *j3=NULL;
//去雾后的图像,三通道合并成
         IplImage *dst=NULL;
//源图像ROI位置以及大小
         CvRect ROI_rect;

//分离的三个通道
         dst1=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
         dst2=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
         dst3=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);

//为各个ROI分配内存
         imgroi1=cvCreateImage(cvSize(block,block),IPL_DEPTH_8U,1);
         imgroi2=cvCreateImage(cvSize(block,block),IPL_DEPTH_8U,1);
         imgroi3=cvCreateImage(cvSize(block,block),IPL_DEPTH_8U,1);
         roidark=cvCreateImage(cvSize(block,block),IPL_DEPTH_8U,1);

//为j1 j2 j3分配大小
         j1=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
         j2=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
         j3=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);

//为暗原色先验指针分配大小
         dark_channel=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
//为透射率指针分配大小
         toushelv=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
//dst分配大小
         dst=cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,3);
//将原彩色图像分离成三通道
         cvSplit(src,dst1,dst2,dst3,NULL);
//求暗原色
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
                            //分别计算三个通道内ROI的最小值
                            cvSetImageROI(dst1,ROI_rect);
                            cvCopy(dst1,imgroi1,NULL);
                            cvMinMaxLoc(imgroi1,&min1,&max1,NULL,NULL);
                            cvSetImageROI(dst2,ROI_rect);
                            cvCopy(dst2,imgroi2,NULL);
                            cvMinMaxLoc(imgroi2,&min2,&max2,NULL,NULL);
                            cvSetImageROI(dst3,ROI_rect);
                            cvCopy(dst3,imgroi3,NULL);
                            cvMinMaxLoc(imgroi3,&min3,&max3,NULL,NULL);
                            //求三个通道内最小值的最小值
                            if(min1<min2)
                                     min=min1;
                            else
                                     min=min2;
                            if(min>min3)
                                     min=min3;//min为这个ROI中暗原色
                            value=cvScalar(min,min,min,min);//min放在value中
                            //min赋予dark_channel中相应的ROI
                            cvSetImageROI(dark_channel,ROI_rect);
                            cvSet(roidark,value,NULL);
                            cvCopy(roidark,dark_channel,NULL);
                            //释放各个ROI
                            cvResetImageROI(dst1);
                            cvResetImageROI(dst2);
                            cvResetImageROI(dst3);
                            cvResetImageROI(dark_channel);
                            //转入下一个ROI
                            ROI_rect.x=block*i;
                            ROI_rect.y=block*j;
                   }
         }
         //M 保存暗原色先验的图像
//        cvSaveImage("e:/dark_channel_prior.jpg",dark_channel);
//M 利用得到的暗原色先验dark_channel_prior.jpg求大气光强
         double min_dark;
         double max_dark;
         CvPoint min_loc;
         CvPoint max_loc;//max_loc是暗原色先验最亮一小块的原坐标
         cvMinMaxLoc(dark_channel,&min_dark,&max_dark,&min_loc,&max_loc,NULL);
  //       cout<<max_loc.x<<" "<<max_loc.y<<endl;
         ROI_rect.x=max_loc.x;
         ROI_rect.y=max_loc.y;
         double A_dst1;//定义大气光成分的估计值
         double dst1_min;
         double A_dst2;
         double dst2_min;
         double A_dst3;
         double dst3_min;
         cvSetImageROI(dst1,ROI_rect);
//按照论文方法求大气光强估计值
         cvCopy(dst1,imgroi1,NULL);
         cvMinMaxLoc(imgroi1,&dst1_min,&A_dst1,NULL,NULL);
         cvSetImageROI(dst2,ROI_rect);
         cvCopy(dst2,imgroi2,NULL);
         cvMinMaxLoc(imgroi2,&dst2_min,&A_dst2,NULL,NULL);
         cvSetImageROI(dst3,ROI_rect);
         cvCopy(dst3,imgroi3,NULL);
         cvMinMaxLoc(imgroi3,&dst3_min,&A_dst3,NULL,NULL);
  //       cout<<A_dst1<<" "<<A_dst2<<" "<<A_dst3<<endl;//这三值为大气光强度估计值
//求透射率
         int k;
         int l;
         CvScalar m;
         CvScalar n;//暗原色先验各元素值

         for(k=0;k<src->height;k++)
         {
                   for(l=0;l<src->width;l++)
                   {
                            m=cvGet2D(dark_channel,k,l);
                            n=cvScalar(255-w*m.val[0]);
                            //w目的是保留一部分的雾,使图像看起来真实些
                            cvSet2D(toushelv,k,l,n);
                   }
         }
      //   cvSaveImage("e:/toushelv.jpg",toushelv);

//求无雾图像
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
                            jj1=(ix.val[0]-A_dst1)/tx+A_dst1;//根据雾产生模型运算,还原出无雾图像
                            jj2=(ix.val[1]-A_dst2)/tx+A_dst2;
                            jj3=(ix.val[2]-A_dst3)/tx+A_dst3;
                            jx=cvScalar(jj1,jj2,jj3,0.0);
                            cvSet2D(dst,p,q,jx);
                   }
         }
      //   cvSaveImage("e:/removed_haze.jpg",dst);

//释放指针
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


   //M 处理完成有拷贝回原地址
   memcpy(src->imageData,dst->imageData,(src->width)*(src->height)*3*sizeof(char));


      //  src->imageData=NULL;  //把地址赋值空--再释放--否则会把传递进来的内存也释放掉
      //  cvReleaseImage(&src);
          cvReleaseImage(&dst);
      //  return dst;
};
#endif



/*M
Threshold

对数组元素进行固定阈值操作

void cvThreshold( const CvArr* src, CvArr* dst, double threshold, double max_value, int threshold_type );

   src   原始数组 (单通道 , 8-bit of 32-bit 浮点数).
   dst   输出数组，必须与 src 的类型一致，或者为 8-bit.
   threshold  阈值
   max_value  使用 CV_THRESH_BINARY 和 CV_THRESH_BINARY_INV 的最大值.
   threshold_type  阈值类型：
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
