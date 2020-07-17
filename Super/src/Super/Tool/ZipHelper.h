#pragma once
 

//ZipHelper.h


#pragma once
#include <iostream>
#include<vector>
using namespace std;


//#include "./zlib/zip.h"
//#include "./zlib/unzip.h"

//#include <windows.h>
//#include <tchar.h>
//
//#include "zip.h"
//#include "unzip.h"




//
//
//
////解压缩
//For unzipping, add "unzip.cpp" to your project. Then, for example,
//#include "unzip.h"
//    //
//    HZIP hz = OpenZip("c:\\stuff.zip",0);
//ZIPENTRY ze; GetZipItem(hz,-1,&ze); int numitems=ze.index;
//for (int i=0; i<numitems; i++)
//{ GetZipItem(hz,i,&ze);
//UnzipItem(hz,i,ze.name);
//}
//CloseZip(hz);
//
////压缩
//For zipping, add "zip.cpp" to your project. (You can add just one of
//                                             zip/unzip, or both; they function independently and also co-exist.)
//#include "zip.h"
//                                             //
//                                             HZIP hz = CreateZip("c:\\simple1.zip",0);
//ZipAdd(hz,"znsimple.bmp", "c:\\simple.bmp");
//ZipAdd(hz,"znsimple.txt", "c:\\simple.txt");
//



//https://blog.csdn.net/HeroGuo_JP/article/details/87696560

//从内存创建,待续



#if 0
class ZipHelper
{
public:
    ZipHelper();
    ~ZipHelper();

private:
    HZIP m_hz;				 //Zip文件句柄 
    ZRESULT m_zr;			 //操作返回值 
    ZIPENTRY m_ze;			 //Zip文件入口

    CString m_FolderPath;	 //folder路径 
    CString  m_FolderName;   //folder将要被压缩的文件夹名

    int m_nZipCnt;
private:
    //实现遍历文件夹 
    void BrowseFile(CString &strFile);

    //获取相对路径 
    void GetRelativePath(CString& pFullPath, CString& pSubString);

    //创建路径 
    BOOL CreatedMultipleDirectory(wchar_t* direct);

    //压缩解压缩接口 
    BOOL Zip_PackFiles(vector<CString> arr, CString mZipFileFullPath);

    BOOL Zip_UnPackFiles(CString mZipFileFullPath, CString mUnPackPath);

    //检查文件(夹)存在
    BOOL IsFolderExist(CString& strPath);

    BOOL IsFileExist(CString& strPath);
public:
    void PressFolder(CString strTgt,CString strSrcFolder = "", vector<CString> arrSrcFloder = {});//指定压缩包  单一路径 路径集合

    void UnPressFolder(CString strTgt);//指定解压目标
private:
    vector<CString>  m_arrFileFullName;
};
#endif
