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
////��ѹ��
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
////ѹ��
//For zipping, add "zip.cpp" to your project. (You can add just one of
//                                             zip/unzip, or both; they function independently and also co-exist.)
//#include "zip.h"
//                                             //
//                                             HZIP hz = CreateZip("c:\\simple1.zip",0);
//ZipAdd(hz,"znsimple.bmp", "c:\\simple.bmp");
//ZipAdd(hz,"znsimple.txt", "c:\\simple.txt");
//



//https://blog.csdn.net/HeroGuo_JP/article/details/87696560

//���ڴ洴��,����



#if 0
class ZipHelper
{
public:
    ZipHelper();
    ~ZipHelper();

private:
    HZIP m_hz;				 //Zip�ļ���� 
    ZRESULT m_zr;			 //��������ֵ 
    ZIPENTRY m_ze;			 //Zip�ļ����

    CString m_FolderPath;	 //folder·�� 
    CString  m_FolderName;   //folder��Ҫ��ѹ�����ļ�����

    int m_nZipCnt;
private:
    //ʵ�ֱ����ļ��� 
    void BrowseFile(CString &strFile);

    //��ȡ���·�� 
    void GetRelativePath(CString& pFullPath, CString& pSubString);

    //����·�� 
    BOOL CreatedMultipleDirectory(wchar_t* direct);

    //ѹ����ѹ���ӿ� 
    BOOL Zip_PackFiles(vector<CString> arr, CString mZipFileFullPath);

    BOOL Zip_UnPackFiles(CString mZipFileFullPath, CString mUnPackPath);

    //����ļ�(��)����
    BOOL IsFolderExist(CString& strPath);

    BOOL IsFileExist(CString& strPath);
public:
    void PressFolder(CString strTgt,CString strSrcFolder = "", vector<CString> arrSrcFloder = {});//ָ��ѹ����  ��һ·�� ·������

    void UnPressFolder(CString strTgt);//ָ����ѹĿ��
private:
    vector<CString>  m_arrFileFullName;
};
#endif
