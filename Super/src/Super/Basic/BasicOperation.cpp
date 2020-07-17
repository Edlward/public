#include "BasicOperation.h"     //自定义的基础操作类

//常用函数封装,只依赖标准C,C++，若用到平台相关（Windows/linux）用宏开关控制

#include <math.h>  //for power

#include <iostream>
#include <algorithm>  
#include <functional>  
#include <map>  
#include <time.h>  
#include <sstream>  
#include <fstream>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
//#include <io.h>





#ifdef _WIN32  
    //方法1
    //#define _WINSOCKAPI_
    #include <Windows.h>
   //#include <Winsock2.h>
   //#define   WIN32_LEAN_AND_MEAN   
   //#include <windows.h>


    #define PID DWORD  
    #include <process.h>  
    #pragma comment(lib, "Ws2_32.lib")  
    #include <direct.h>
    #include <tlhelp32.h>
    #include <shellapi.h>

   #include <direct.h>
   #include <io.h>

#else   //for linux
    //#define PID p_id
    #include <unistd.h>  
    #include <dirent.h>  
    #include <stdlib.h>   
    #include <sys/procfs.h>   
    #include <unistd.h>   
    //#include <stropts.h>
    #include <fcntl.h>   
    #include <signal.h>  
    #include<sys/types.h>
    #include<dirent.h>
    #include<sys/stat.h>
    #include <sys/statvfs.h>  //for struct statvfs
    #include<unistd.h>
    #include <limits.h>
    #include <utime.h>
#endif  


#include "Tstring.h"   //String Wstring 与宽字符串转换与编码相关

//为了解决gcc 模板函数必须在.h实现，用extern 函数中转
 std::wstring BS_StringToWString(const std::string &str)
 {
     return StringToWString(str);
 }
std::string BS_WStringToString(const std::wstring &wstr)
{
     return WStringToString(wstr);
}




//跨平台大文件fseek
 #if (defined(__WIN32__)|| defined(_WIN32)) && !defined(__MINGW32__)
    #define fseek _fseeki64
#else
    //#define D_FILE_OFFSET_BITS 64   //在编译选项添加才有效
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//64-bit file-I/O

#ifdef _WIN32

// _WIN32 platform specific File I/O, GUID and time functions
//#include <windows.h>		//!< Required for system specifics (such as CoCreateGuid, GUID)
#include <fcntl.h>			//!< for _O_BINARY etc
#include <sys/stat.h>		//!< for _S_IREAD, _S_IWRITE
#include <sys/timeb.h>		//!< for _timeb

#define DIR_SEPARATOR		'\\'
#define PATH_SEPARATOR		';'
#ifndef DEFAULT_DICT_PATH
#define DEFAULT_DICT_PATH	".\\"
#endif //DEFAULT_DICT_PATH


/******** 64-bit file-I/O ********/
typedef int FileHandle;
inline int FileSeek(FileHandle file, uint64_t offset) { return _lseeki64(file, offset, SEEK_SET) == -1 ? -1 : 0; }
inline int FileSeekEnd(FileHandle file) { return _lseeki64(file, 0, SEEK_END) == -1 ? -1 : 0; }

// DRAGONS: MSVC can't read or write more than 4Gb in one go currently
inline uint64_t FileRead(FileHandle file, unsigned char *dest, uint64_t size) { return read(file, dest, (unsigned int)size); }
inline uint64_t FileWrite(FileHandle file, const unsigned char *source, uint64_t size) { return write(file, source, (unsigned int)size); }

inline int FileGetc(FileHandle file) { uint8_t c; return (FileRead(file, &c, 1) == 1) ? (int)c : EOF; }
inline FileHandle FileOpen(const char *filename) { return open(filename, _O_BINARY | _O_RDWR ); }
inline FileHandle FileOpenRead(const char *filename) { return open(filename, _O_BINARY | _O_RDONLY ); }
inline FileHandle FileOpenNew(const char *filename) { return open(filename, _O_BINARY | _O_RDWR | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE); }
inline bool FileValid(FileHandle file) { return (file >= 0); }
inline bool FileEof(FileHandle file) { return eof(file) ? true : false; }
inline uint64_t FileTell(FileHandle file) { return _telli64(file); }
inline void FileClose(FileHandle file) { close(file); }
inline bool FileExists(const char *filename) { struct stat buf; return stat(filename, &buf) == 0; }

#else

//#include <sys/time.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>


#define DIR_SEPARATOR		'/'
#define PATH_SEPARATOR		':'

/******** 64-bit file-I/O ********/

typedef FILE *FileHandle;
inline int FileSeek(FileHandle file,uint64_t offset) { return fseeko(file, offset, SEEK_SET); }
inline int FileSeekEnd(FileHandle file) { return fseeko(file, 0, SEEK_END); }
inline uint64_t FileRead(FileHandle file, unsigned char *dest,uint64_t size) { return fread(dest, 1, size, file); }
inline uint64_t FileWrite(FileHandle file, const unsigned char *source,uint64_t size) { return fwrite(source, 1, size, file); }
inline int FileGetc(FileHandle file) {uint8_t c; return (FileRead(file, &c, 1) == 1) ? (int)c : EOF; }
inline FileHandle FileOpen(const char *filename) { return fopen(filename, "r+b" ); }
inline FileHandle FileOpenRead(const char *filename) { return fopen(filename, "rb" ); }
inline FileHandle FileOpenNew(const char *filename) { return fopen(filename, "w+b"); }
inline bool FileValid(FileHandle file) { return (file != NULL); }
inline bool FileEof(FileHandle file) { return feof(file); }
inline uint64_t FileTell(FileHandle file) { return ftello(file); }
inline void FileClose(FileHandle file) { fclose(file); }
inline bool FileExists(const char *filename) { struct stat buf; return stat(filename, &buf) == 0; }

#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//from https://blog.csdn.net/u013074465/article/details/46499959
//simple v1.0
//char* itoa(int val, int base)
//{
//    static char buf[32] = {0};
//    int i = 30;
//    for(; val && i ; --i, val /= base)
//        buf[i] = "0123456789abcdef"[val % base];
//    return &buf[i+1];
//}
//improve use template by lizulin 20200326

static const char* pStr="zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
template <typename IntType>
char* intertoStr(IntType  value,char* result,int base=10)
{
    // check that the base if valid
    if (base<2|| base > 36) {*result='\0'; return result;}

    char* ptr = result, *ptr1 = result, tmp_char;
    IntType tmpV;
    do
    {
        tmpV = value;
        value /= base;
        *ptr++ =pStr[35 + (tmpV-value * base)];
    } while ( value );


    //or use std::is_unsigned<unsigned int>::value

    // Apply negative sign
    if (tmpV < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }


    return result;
};

template <typename UintType>
char* uintertoStr(UintType  value,char* result,int base=10)
{
    // check that the base if valid
    if (base<2|| base > 36) {*result='\0'; return result;}

    char* ptr = result, *ptr1 = result, tmp_char;
    UintType tmpV;
    do
    {
        tmpV = value;
        value /= base;
        *ptr++ =pStr[35 + (tmpV-value * base)];
    } while ( value );
    return result;
};



std::string NumToString(int32_t n,int base)
{
    char buf[64]={0};
    return std::string(intertoStr<int32_t>(n,buf,base));
}

std::string NumToString(uint32_t n,int base)
{
    char buf[64]={0};
    return std::string(uintertoStr<uint32_t>(n,buf,base));
}

std::string NumToString(int64_t n,int base)
{
    char buf[64]={0};
    return std::string(intertoStr<int64_t>(n,buf,base));
}

std::string NumToString(uint64_t n,int base)
{
    char buf[64]={0};
    return std::string(uintertoStr<int64_t>(n,buf,base));
}

std::string NumToString(float n,int dec)
{
    return NumToString(double(n),dec);
}

std::string NumToString(double n,int dec)
{
     //temp todo
    char buf[64];
    sprintf(buf,"%f",n);

    //查找小数点并截断，to be tested
    char* pos=buf;
    for (int n=0;n<64;n++)
    {
        char curr=*pos;
        if (curr='.')
        {
            char* pCut=pos+dec+1;
            if (pCut<buf+sizeof(buf))    //截断小数点位置没有越界
            {
                *pCut='\0';
            }
            break;
        }
        pos++;
    }

    std::string tmp(buf);
    return tmp;
}


std::string IntToString(int val)
{
    char buf[64];
    sprintf(buf,"%d",val);
    std::string tmp(buf);
    return tmp;
}

std::string IntToString(unsigned int val)
{
    char buf[64];
    sprintf(buf,"%u",val);
    std::string tmp(buf);
    return tmp;
}

std::string IntToString(int64_t val)
{
    char buf[64];
    sprintf(buf,"%lld",val);
    std::string tmp(buf);
    return tmp;
}

std::string IntToString(uint64_t val)
{
    char buf[64];
    sprintf(buf,"%llu",val);
    std::string tmp(buf);
    return tmp;
}

std::string FloatToString(float val)
{
    char buf[64];
    sprintf(buf,"%f",val);
    std::string tmp(buf);
    return tmp;
}

std::string DoubleToString(double val)
{
    char buf[64];
    sprintf(buf,"%f",val);
    std::string tmp(buf);
    return tmp;
}

int StringToInt(const std::string& str,const int defaultV,bool* isOk)
{
    //std::string tmp=str;
    int ret=defaultV;
    bool isSuccess;
    if (sscanf(str.c_str(),"%d",&ret)==1)
    {
        isSuccess=true;
    }
    else
    {
        isSuccess=false;
        ret=defaultV;
    }
    if (isOk!=NULL)
    {
        *isOk=isSuccess;
    }
    return ret;
}

float StringToFloat(const std::string& str,const float defaultV,bool* isOk)
{
    //std::string tmp=str;
    float ret=defaultV;
    bool isSuccess;
    if (sscanf(str.c_str(),"%f",&ret)==1)
    {
        isSuccess=true;
    }
    else
    {
        isSuccess=false;
        ret=defaultV;
    }
    if (isOk!=NULL)
    {
        *isOk=isSuccess;
    }
    return ret;
}

double StringToDouble(const std::string& str,const double defaultV,bool* isOk)
{
    //std::string tmp=str;
    double ret=defaultV;
    bool isSuccess;
    if (sscanf(str.c_str(),"%lf",&ret)==1)
    {
        isSuccess=true;
    }
    else
    {
        isSuccess=false;
        ret=defaultV;
    }
    if (isOk!=NULL)
    {
        *isOk=isSuccess;
    }
    return ret;
}





//返回值大于等于0,表明获取成功
uint64_t getFileSize(const char* path)
{
    if (isNullCstr(path))
    {
        return 0;
    }

    ////C++库方法跨平台,是否支持大于4G,支持虚拟FTP目录等,待测试确认。
    //std::ifstream in(path);
    //in.seekg(0,std::ios::end);
    //std::streampos ps = in.tellg();  //读取文件指针的位置
    //in.close();
    //return ps;

#ifdef _WIN32
    FILE *fp;
    fp=fopen(path,"rb");
    if(fp==NULL)
    {
        return 0;//-1;
    }
    //方法一:不支持大于2G的文件
    //fseek(fp,0,SEEK_END);
    //uint64_t Size=ftell(fp);          
    //return Size;
  
    //方法2:
    fpos_t pos;
    fseek(fp,0,SEEK_END);
    fgetpos(fp,&pos);
    fclose(fp);
    return pos;     //gcc linux 下pos,是一个结构体,此法不行
#else
    //C++库方法跨平台,是否支持大于4G,支持虚拟FTP目录等,待测试确认。
    std::ifstream in(path);
    in.seekg(0,std::ios::end);
    std::streampos ps = in.tellg();  //读取文件指针的位置
    in.close();
    return ps;

	//以下注释，原因是FTP目录调用失败
	//struct stat statbuf;
    //stat(path,&statbuf);
    //return statbuf.st_size;
#endif


////其他方法:  失败返回-1，还是0，  待续
//#if defined(_WIN32)
//    WIN32_FILE_ATTRIBUTE_DATA info;
//     Tstring spath=FromCstr(path);
//    BOOL r =::GetFileAttributesEx(spath.c_str(), GetFileExInfoStandard, &info);  
//    if (!r) return -1;
//    return ((int64) info.nFileSizeHigh<<32)| info.nFileSizeLow;
//#else  
//    struct stat attr;
//    if (::lstat(path, &attr) != 0) return -1;
//    return attr.st_size;
//#endif
}


uint64_t getFloderFileSize(const char* dir)
{
    //跨平台实现待续
    return 0;
}

int64 getCreateTime(const char* path)
{
    //to test
#ifdef _WIN32
    WIN32_FILE_ATTRIBUTE_DATA info;
    Tstring spath=FromCstr(path);
    BOOL r =::GetFileAttributesEx(spath.c_str(), GetFileExInfoStandard, &info);
    if (!r) return -1;
    FILETIME& wt = info.ftLastWriteTime;
    return ((int64) wt.dwHighDateTime << 32) | wt.dwLowDateTime;
#else
    struct stat attr;
    if (::lstat(path,&attr)!=0) return 0;
    return attr.st_mtime;
#endif
}




//C/C++判断一个文件是否存在  跨平台
bool isExistFile(const char *path)
{
    if (isNullCstr(path))
    {
        return false;
    }

    //不完全正确，可能存在不可读
    //FILE *fp = fopen(path, "rb");
    //if(fp == NULL)
    //    return false;
    //fclose(fp);
    //return true;
    
    //方法2   #include <fstream>
    std::fstream file;
    file.open(path,std::ios::in);
    if(!file)
        return false;
    return true;

    //方法3利用 c 语言的库的办法：##include <iostream>
    //if((_access(path,0))!=-1)
 //   {
    //		printf( "File crt_ACCESS.C exists\n" );
 //           return true;
 //   }
 //   else
 //   {
 //       return false;
 //   }


    //to test
#ifdef _WIN32
    Tstring spath=FromCstr(path);
    return ::GetFileAttributes(spath.c_str()) != INVALID_FILE_ATTRIBUTES;
#else
    struct stat attr;
    return ::lstat(path, &attr) == 0;
#endif
}




///目录是否存在的检查：
bool  isDirectory(const char* path)
{
    if (isNullCstr(path))
    {
        return false;
    }

    //方法2 无法区分文件还是目录
    //if(access(path,0) == 0)
    //{
    //    return true;
    //}
    //return false;

#if defined(_WIN32)
    Tstring sPath=FromCstr(path);
    DWORD dwAttr =::GetFileAttributes(sPath.c_str());  
    if (dwAttr&FILE_ATTRIBUTE_DIRECTORY&&dwAttr!=INVALID_FILE_ATTRIBUTES)  
    {
        return true;
    }
    return false;

#else
    DIR *dir=NULL;
    dir = opendir(path);
    if(NULL == dir)
    {
        return false;//cout<<"1:文件不存在"<<endl;
    }
    closedir(dir);
    return true;
    //方法3
    //struct stat st;
    //if(stat(path,&st) == 0)   cout<<"3:文件存在"<<endl;

#endif
}



//0:不存在，1:为文件夹, -1:为文件   
int isFileOrDirectory(const char* path)
{
    if (isNullCstr(path))
    {
        return 0;
    }

#ifdef _WIN32     

    Tstring spath=FromCstr(path);
    DWORD dwAttr =GetFileAttributes(spath.c_str());      
    if (dwAttr == 0xFFFFFFFF)       //Not exist   
    {     
        return 0;     
    }     
    else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)     //exist     dir
    {     
        return 1;     
    }     
    else    
    {     
        return -1;          //is File     
    }     
#else     
    if (0 == access(path, 0))     
    {  
		//测试是否是目录或文件
		bool bIsDir = true;
		DIR * pTempDir;
		if ((pTempDir=opendir(path)) == NULL) //说明不是目录是文件
		{
			bIsDir = false;
		}
		else
		{
			bIsDir = true;
			closedir(pTempDir); //判断文件还文件夹完毕
		}
		//返回1 或 -1
        if (bIsDir)     
        {     
            return 1;     
        }     
        else         
        {   
            return -1;     
        }     
    }     
    else    
    {     
        return 0;     
    }     
#endif     
}

bool isExist(const char *path)
{
    return isFileOrDirectory(path)!=0;
}





#ifdef _WIN32
    #define ACCESS _access
    #define MKDIR(a) _mkdir((a))
#else
    #define ACCESS access
    #define MKDIR(a) mkdir((a),0755)
#endif

//核查目录，若目录不存在，创建目录
//创建文件夹,可创建多级文件夹,比如:12/34(当前运行目录)  c:/ab/cd(绝对目录) /ab/cd(当前盘符的根路径)
//创建相对路径 .\\123\\456\\789\\ 绝对路径如E:\\123\\456\\789\\   后面没有\\ 则不会创建最后级
//测试确认待续
bool createDirectory( const char* path)
{
    if (isNullCstr(path))
    {
        return false;
    }

    //if(0== ACCESS(path,0))  //已经存在直接退出
    //{
    //	return true;
    //}

    if (isDirectory(path))
    {
        return true;
    }

    char pszDir[2048]={0};
    strcpy(pszDir,path);

    int i = 0;
    int iRet;
    size_t iLen = strlen(pszDir);

    //在末尾加/
    if (pszDir[iLen - 1] != '\\' && pszDir[iLen - 1] != '/')
	{
		pszDir[iLen] = '/';
		pszDir[iLen + 1] = '\0';
		iLen += 1;
	}

    // 创建目录
    for (i = 1;i < iLen; i++)
    {
        if (pszDir[i] == '\\' || pszDir[i] == '/')
        { 
            pszDir[i] = '\0';
            //如果不存在,创建
            iRet = ACCESS(pszDir,0);
            if (iRet != 0)
            {
                iRet = MKDIR(pszDir);
                if (iRet != 0)
                {
                  //return -1;
                    return false;
                } 
            }
            //支持linux,将所有\换成/
            pszDir[i] = '/';
        } 
    }
    //return 0;
    return true;

//
//#if defined(_WIN32)
//	//该函数只能创建一级目录，当根目录或者中间目录不存在时，该函数将不起作用。
//	//所以假如我们要创建二级以上目录时，应该分开来一步步创建
//		if (!::CreateDirectory(path,NULL))//判断路径文件夹是否存在,不存在创建    
//		{
//				printf("创建目录失败: %s\n",path);
//				return false;
//		}
//		return true;
//#else
//
//		return false;
//#endif
}










bool deleteDirectory(const char * path)
{
    //return deletePath(path);
    return deletePathBySysCall(path);   //调用系统命令实现
}

//bool deleteDirectory(const char * DirName)
//{
//    return true;

///*
////  CFileFind tempFind;     //声明一个CFileFind类变量，以用来搜索
//    char szCurPath[MAX_PATH];       //用于定义搜索格式
//    _snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName); //匹配格式为*.*,即该目录下的所有文件
//    WIN32_FIND_DATAA FindFileData;
//    ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
//    HANDLE hFile = FindFirstFile(szCurPath, &FindFileData);
//    BOOL IsFinded = TRUE;
//    while(IsFinded)
//    {
//        IsFinded = FindNextFile(hFile, &FindFileData); //递归搜索其他的文件
//        if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) //如果不是"." ".."目录
//        {
//            std::string strFileName = "";
//            //strFileName = strFileName+DirName + "//" + FindFileData.cFileName;
//            strFileName +=DirName;
//            strFileName +="//";
//            strFileName +=FindFileData.cFileName;

//            std::string strTemp;
//            strTemp = strFileName;
//            if(isDirectory(strFileName.c_str()) ) //如果是目录，则递归地调用
//            {
//                printf("目录为:%s/n", strFileName.c_str());
//                deleteDirectory(strTemp.c_str());
//            }
//            else
//            {
//                DeleteFile(strTemp.c_str());
//            }
//        }
//    }
//    FindClose(hFile);

//    //BOOL bRet = RemoveDirectoryA(DirName);
//    //if( bRet == 0 ) //删除目录
//    //{
//    //    printf("删除%s目录失败！/n", DirName);
//    //    return FALSE;
//    //}
//    return true;

//    */
//}

//void moveFile(char *from,char *to)
//{
//  //  MoveFile(from,to);
//}

#ifndef _WIN32

//复制普通文件
int leonis_cp(const char *dir1, const char *dir2)               
{
	int fd1;
	int fd2;
	int ret;
	int sts;
	char buf[1024];


	fd1=open(dir1, O_RDONLY);
	fd2=open(dir2, O_WRONLY|O_CREAT|O_TRUNC, 0666);

	while(1)
	{
		bzero(buf, sizeof(buf));
		ret= read(fd1, buf, sizeof(buf)-1 );
		if(ret<=0)
			break;
		sts=write(fd2, buf, ret);
		while(sts<ret)
		{
			sts+=write(fd2, buf+sts, ret-sts);
		}
	}

	close(fd1);
	close(fd2);

	return 0;
}

//复制目录
int leonis_cpdir(const char *name1, const char *name2)              
{
	int ret;
	DIR *dir1;
	DIR *dir2;
	struct dirent *drr;

	dir1=opendir(name1);
	if(dir1==NULL)
	{
		perror("opendir1");
		return 1;
	}
	dir2=opendir(name2);  //打开name2, 打开失败则创建
	if(dir2==NULL)
	{
		mkdir(name2, S_IFDIR);
		dir2=opendir(name2);
	}

	while(1)
	{
		char buf1[1024];
		char buf2[1024];
		bzero(buf1, sizeof(buf1));
		bzero(buf2, sizeof(buf2));
		drr=readdir(dir1);
		if(drr==NULL)break;
		if(*drr->d_name=='.')          //过滤掉 "." ".."
			continue;
		sprintf(buf1, "%s/%s", name1, drr->d_name);           //构造完整的相对路径
		sprintf(buf2, "%s/%s", name2, drr->d_name);

		
		//测试是否是目录或文件
		bool bIsDir = true;
		DIR * pTempDir;
		if ((pTempDir=opendir(buf1)) == NULL) //说明不是目录是文件
		{
			bIsDir = false;
		}
		else
		{
			bIsDir = true;
			closedir(pTempDir); //判断文件还文件夹完毕
		}

		if(bIsDir)           //如果是目录
		{
			leonis_cpdir(buf1 , buf2);
		}
		else //如果是文件
		{
			leonis_cp(buf1, buf2);
		}
	}

	closedir(dir1);
	closedir(dir2);
	return 0;
}

int leonis_cpFileToDir(const char *name1, const char *name2)   
{
	int ret;
	DIR *dir1;
	DIR *dir2;
	struct dirent *drr;

	//测试是否是目录或文件
	bool bIsDir = true;
	DIR * pTempDir;
	if ((pTempDir=opendir(name1)) == NULL) //说明不是目录是文件
	{
		bIsDir = false;
	}
	else
	{
		bIsDir = true;
		closedir(pTempDir); //判断文件还文件夹完毕
	}

	if(bIsDir)      //如果不是普通文件
	{
		perror("error: copy path is not a file ");
		return 1;
	}

	dir2=opendir(name2);  //打开name2, 打开失败则创建
	if(dir2==NULL)
	{
		mkdir(name2, S_IFDIR );
		dir2=opendir(name2);
	}
	char buf1[2048];
	bzero(buf1, sizeof(buf1));
	sprintf(buf1, "%s/%s", name2, basename(name1));           //构造完整的相对路径

	printf("=========================================\n");
	printf("strSrc:%s\n",name1);
	printf("strDest:%s\n",buf1);
	leonis_cp(name1, buf1);//复制文件

	closedir(dir2);
	return 0;
}


#endif

//todo:调用系统命令实现版windows 与linux
bool copyFilebySysCall(const char* path_dst,const char* path_src,bool bCover)
{
    if (isNullCstr(path_dst)||isNullCstr(path_src))
    {
        return false;
    }
    if (!isExist(path_src))
    {
        return false;
    }

#ifdef _WIN32
    char str_cmd[2048]= {0};
    strcat(str_cmd,"echo f | xcopy ");
    strcat(str_cmd,path_src); 
    strcat(str_cmd," "); 
    strcat(str_cmd,path_dst); 

    if (bCover)  //控制是否强制覆盖已有文件
    {
        strcat(str_cmd," /Y ");
    }
    PreDprintf("cmd:%s\n",str_cmd);
    system(str_cmd); 
#else

#if 0
    char str_cmd[2048]="cp -rf ";
    strcat(str_cmd, path_src); 
    strcat(str_cmd, " "); 
    strcat(str_cmd, path_dst);
    printf("copyFile() ::linux_system() cmd:%s\n",str_cmd);
    system(str_cmd);
#else
	leonis_cp(path_src, path_dst);
#endif

#endif
    return true;
}

bool copyDirectorybySysCall(const char* path_dst,const char* path_src,bool bCover)
{
    if (isNullCstr(path_dst)||isNullCstr(path_src))
    {
        return false;
    }
    if (!isExist(path_src))
    {
        return false;
    }
#ifdef _WIN32
    char str_cmd[2048]= {0};
	strcat(str_cmd,"\"");
    strcat(str_cmd, "echo d | xcopy ");
	strcat(str_cmd,"\"");
    strcat(str_cmd, path_src); 
	strcat(str_cmd,"\"");
    strcat(str_cmd, " "); 
	strcat(str_cmd,"\"");
    strcat(str_cmd, path_dst); 
	strcat(str_cmd,"\"");
     //strcat(str_cmd," /E /Y ");
     strcat(str_cmd," /E");
     if (bCover)
     {
        strcat(str_cmd," /Y ");
     }
	strcat(str_cmd,"\"");

    PreDprintf("cmd:%s\n",str_cmd);
    system(str_cmd); 
#else

	//系统命令实现
#if 0
	char str_cmd[2048]="cp -rf ";
	strcat(str_cmd, path_src); 
	strcat(str_cmd, " "); 
	strcat(str_cmd, path_dst);
	PreDprintf("cmd:%s\n",str_cmd);
	
	sighandler_t old_handler = signal(SIGCHLD, SIG_DFL);
	pid_t status = system(str_cmd);
	signal(SIGCHLD, old_handler);
	if (-1 == status)
	{
		printf(" system cmd error\n");
		return false;
	}
	else
	{
		printf("exit status value = [0x%x]\n", status); 
		if (WIFEXITED(status))
		{
			if (0 == WEXITSTATUS(status))
			{
				printf("run shell script successfully.\n");
			}
			else
			{
				printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
			}
		}
		else
		{
			printf("exit status = [%d]\n", WEXITSTATUS(status)); 
		}
	}
#else
	//C语言函数实现
	if ( 0 != leonis_cpdir(path_src, path_dst) )
	{
		return false;
	}
#endif
    
#endif
    return true;
}

bool copyFileToDirectory(const char* path_dst1,const char* path_src,bool bCover)
{
	if (isNullCstr(path_dst1)||isNullCstr(path_src))
	{
		return false;
	}
	if (!isExist(path_src))
	{
		return false;
	}
	char path_dst[2048]={0};
	strcpy(path_dst,path_dst1);
    size_t tailPos=strlen(path_dst)-1;
    if (path_dst[tailPos]=='/')   //去掉路径尾部的反斜杠,否则失败，有空待续 TODO:
    {
        path_dst[tailPos]='\0';
    }

#ifdef _WIN32
	char str_cmd[2048]= {0};
	strcat(str_cmd,"\"");
	strcat(str_cmd,"echo f | xcopy ");   //f 

	//strcat(str_cmd,"/E /Y ");  // /S 

	strcat(str_cmd,"\"");
	strcat(str_cmd, path_src); 
	strcat(str_cmd,"\"");
	strcat(str_cmd," ");
	strcat(str_cmd,"\"");
	strcat(str_cmd, path_dst); 

    //xcopy总是询问是文件名还是目录名 
    //XCOPY  D:dir F:dir\    /Y
    //在新的目版录加上 \  然后带/Y 这样在第一次多次的时候就不会提示了权
	strcat(str_cmd,"\\");


	strcat(str_cmd,"\"");

	strcat(str_cmd," /e /y ");  // /S 

	//strcat(str_cmd," /E");
	//if (bCover)
	//{
	//	strcat(str_cmd," /Y ");
	//}
	strcat(str_cmd,"\"");

	PreDprintf("cmd:%s\n",str_cmd);
	system(str_cmd); 
#else

	//系统命令实现
#if 0
	char str_cmd[2048]="cp -rf ";
	strcat(str_cmd, path_src); 
	strcat(str_cmd, " "); 
	strcat(str_cmd, path_dst);
	PreDprintf("cmd:%s\n",str_cmd);

	sighandler_t old_handler = signal(SIGCHLD, SIG_DFL);
	pid_t status = system(str_cmd);
	signal(SIGCHLD, old_handler);
	if (-1 == status)
	{
		printf(" system cmd error\n");
		return false;
	}
	else
	{
		printf("exit status value = [0x%x]\n", status); 
		if (WIFEXITED(status))
		{
			if (0 == WEXITSTATUS(status))
			{
				printf("run shell script successfully.\n");
			}
			else
			{
				printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
			}
		}
		else
		{
			printf("exit status = [%d]\n", WEXITSTATUS(status)); 
		}
	}
#else
	//C语言函数实现
	if ( 0 != leonis_cpFileToDir(path_src, path_dst) )
	{
		return false;
	}
#endif

#endif
	return true;
}
bool moveFilebySysCall(const char *to,const char *from,bool bCover)
{
    if (isNullCstr(to)||isNullCstr(from))
    {
        return false;
    }
    if (!isExist(from))
    {
        return false;
    }
#ifdef _WIN32
    char str_cmd[2048]= {0};
	strcat(str_cmd,"\"");
    //strcat(str_cmd, "MOVE /Y ");
    strcat(str_cmd, "MOVE");
    if (bCover)
    {
         strcat(str_cmd," /Y ");
    }
	strcat(str_cmd,"\"");
    strcat(str_cmd, from); 
	strcat(str_cmd,"\"");
    strcat(str_cmd, " "); 
	strcat(str_cmd,"\"");
    strcat(str_cmd, to); 
	strcat(str_cmd,"\"");
    PreDprintf("cmd:%s\n",str_cmd);
    system(str_cmd); 
#else
    char str_cmd[2048]={0};
    if (bCover)
    {
        strcat(str_cmd,"mv -f "); 
    }
    else
    {
        strcat(str_cmd," mv "); 
    }
    strcat(str_cmd, from); 
    strcat(str_cmd, " "); 
    strcat(str_cmd, to);
    PreDprintf("cmd:%s\n",str_cmd);
    system(str_cmd);
#endif
    return true;
}


#ifndef _WIN32
/**
* 递归删除目录(删除该目录以及该目录包含的文件和目录)
* @dir:要删除的目录绝对路径
*/
int remove_dir(const char *dir)
{
    DIR *dirp;
    struct dirent *dp;
    struct stat dir_stat;

    // 参数传递进来的目录不存在，直接返回
    if ( 0 != access(dir, F_OK) ) 
    {
        return 0;
    }

    // 获取目录属性失败，返回错误
    if ( 0 > stat(dir, &dir_stat) ) 
    {
        printf("remove_dir:: get directory stat error");
        return -1;
    }

    if ( S_ISREG(dir_stat.st_mode) ) // 普通文件直接删除
    {	
        remove(dir);
    } 
    else if ( S_ISDIR(dir_stat.st_mode) ) // 目录文件，递归删除目录中内容
    {	
        dirp = opendir(dir);
        while ( (dp=readdir(dirp)) != NULL ) 
        {
            // 忽略 . 和 ..
            if ( (0 == strcmp(".", dp->d_name)) || (0 == strcmp("..", dp->d_name)) ) 
            {
                continue;
            }
            char dir_name[2048] = {0};
            sprintf(dir_name, "%s/%s", dir, dp->d_name);
            remove_dir(dir_name);   // 递归调用
        }
        closedir(dirp);

        rmdir(dir);		// 删除空目录
    } 
    else 
    {
        printf("remove_dir:: unknow file type!");	
    }

    return 0;
}
#endif


bool moveDirectorybySysCall(const char *to,const char *from,bool bCover)
{
    if (isNullCstr(to)||isNullCstr(from))
    {
        return false;
    }
    if (!isExist(from))
    {
        return false;
    }
#ifdef _WIN32
    char str_cmd[2048]= {0};
    //strcat(str_cmd, "MOVE /Y ");
	strcat(str_cmd,"\"");
    strcat(str_cmd, "MOVE");
    if (bCover)
    {
        strcat(str_cmd," /Y ");
    }
	strcat(str_cmd,"\"");
    strcat(str_cmd, from); 
	strcat(str_cmd,"\"");

    strcat(str_cmd, " "); 

	strcat(str_cmd,"\"");
    strcat(str_cmd, to); 
	strcat(str_cmd,"\"");

	strcat(str_cmd,"\"");

    PreDprintf("cmd:%s\n",str_cmd);
    system(str_cmd); 
#else
    char str_cmd[2048]={0};
    if (bCover)
    {
        strcat(str_cmd,"mv -f "); 
    }
    else
    {
        strcat(str_cmd,"mv "); 
    }
    strcat(str_cmd, from); 
    strcat(str_cmd, " "); 
    strcat(str_cmd, to);
    PreDprintf("cmd:%s\n",str_cmd);
    system(str_cmd);
#endif
    return true;
}



bool deletePathBySysCall(const char* path,bool bForce)
{
    if (!isExist(path))  //不存在
    {
        return false;
    }
#ifdef _WIN32
    char str_cmd[2048]= {0}; //路径加双引号，以免路径中包含空格，识别错误
    if (isDirectory(path))
    {
        strcat(str_cmd,"rmdir /S/Q \"");
		strcat(str_cmd, path); 
		strcat(str_cmd,"\"");
    }
    else
    {
        strcat(str_cmd,"del  \"");
		strcat(str_cmd, path); 
		strcat(str_cmd,"\"");
    }
    PreDprintf(" cmd:%s\n",str_cmd);
    system(str_cmd); 
#else

#if 0  
	//系统命令实现
	char str_cmd[2048]={0};
	if (bForce)
	{
		strcat(str_cmd,"rm -rf ");
	}
	else
	{
		strcat(str_cmd,"rm ");
	}

	strcat(str_cmd, path);
	PreDprintf(" cmd:%s\n",str_cmd);

	sighandler_t old_handler = signal(SIGCHLD, SIG_DFL);
	pid_t status = system(str_cmd);
	signal(SIGCHLD, old_handler);
	if (-1 == status)
	{
		printf(" system cmd error\n");
		return false;
	}
	else
	{
		printf("exit status value = [0x%x]\n", status); 
		if (WIFEXITED(status))
		{
			if (0 == WEXITSTATUS(status))
			{
				printf("run shell script successfully.\n");
			}
			else
			{
				printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
			}
		}
		else
		{
			printf("exit status = [%d]\n", WEXITSTATUS(status)); 
		}
	}
	
#else  
	//C语言函数实现
	if(0 != remove_dir( path)) 
	{
		return false;
	}
#endif
    
#endif
    return true;
}



//拷贝原文件的时间属性覆盖到目标文件,包括创建时间,修改时间,访问时间等 add by lizulin 20190819
bool copyTimeAttributes(const char* pathDst,const char* pathSrc)
{
    if (isNullCstr(pathDst)||isNullCstr(pathSrc))
    {
        PRINT_FUN_FAILED
            return false;
    }

#ifdef _WIN32
    HANDLE hFileSrc;
    FILETIME ftCreate, ftLastAccess, ftLastWrite;
    Tstring sPathSrc=FromCstr(pathSrc);
    hFileSrc =::CreateFile(sPathSrc.c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (INVALID_HANDLE_VALUE == hFileSrc) 
    {
        ::printf("CreateFile error: %d", GetLastError());
        PRINT_FUN_FAILED
            return false;
    }
    if (!GetFileTime(hFileSrc, &ftCreate, &ftLastAccess, &ftLastWrite))
    {
        ::CloseHandle(hFileSrc);
        PRINT_FUN_FAILED
        return false;
    }

    if (!CloseHandle(hFileSrc))
    {
        PRINT_FUN_FAILED
         return false;
    }

    HANDLE hFileDst;
    Tstring sPathDst=FromCstr(pathDst);
    hFileDst=::CreateFile(sPathDst.c_str(), GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (INVALID_HANDLE_VALUE==hFileDst)
    {
        ::printf("CreateFile error: %d", GetLastError());
        PRINT_FUN_FAILED
            return false;
    }

    if (!SetFileTime(hFileDst, &ftCreate, &ftLastAccess, &ftLastWrite))
    {
        ::CloseHandle(hFileDst);
        PRINT_FUN_FAILED
            return false;
    }

    if (!::CloseHandle(hFileDst))
    {
        PRINT_FUN_FAILED
            return false;
    }

    return true;
#else

    //linux 
    struct stat st1;
    if (0 != stat(pathSrc,&st1))
    {
        return false;
    }
    struct stat st2;
    if (0 != stat(pathDst,&st2))
    {
        return false;
    }

	struct utimbuf myTime;
	myTime.actime = st1.st_atime;
	myTime.modtime = st1.st_mtime;

    //struct timeval times[2];
    //times[0].tv_sec = st1.st_atime;
    //times[1].tv_sec = st1.st_mtime;

    //int iRet = utimes(pathDst,times);
    //int iRet = utimensat(AT_FDCWD,pathDst, times,int flag);

	//执行成功则返回0，失败返回-1，错误代码存于errno。
	//EACCESS 存取文件时被拒绝, 权限不足。
	//ENOENT 指定的文件不存在。
	int iRet = utime(pathDst,&myTime); 
	if (0 == iRet)
	{
		return true;
	} 
	else
	{
		printf("errno: %s\n",strerror(errno));
		return false;
	}
    
#endif
}

uint64_t getFileCheckSum(const char* pathSrc)
{
    if(isNullCstr(pathSrc))
    {
        return 0;
    }
    FILE *FpSrc=FpSrc=::fopen(pathSrc,"rb");
    if(FpSrc==NULL)
    {
        PreDprintf("fopen() failed! Src path:%s\n",pathSrc);
        return 0;
    }
    ::fseek(FpSrc,0L,SEEK_END); 
    int file_entire_size=ftell(FpSrc);
    ::fseek(FpSrc,0L,SEEK_SET);

    typedef uint64_t OpType;
    const int typeSize=sizeof(OpType);

    OpType iobuf[1*1024];
    size_t wantRead=sizeof(iobuf);
    uint64_t checkSum=0;

    if (file_entire_size < 1024 * 1024) //如果小于1M,全部比较
    {
        while(1)
        {         
            size_t  nread=fread(iobuf,1,wantRead,FpSrc);
            size_t blockNum=nread/typeSize;
            size_t remainbytes=nread%typeSize;

            for (size_t n=0;n<blockNum;n++)
            {
                checkSum+=iobuf[n];
            }

            if (remainbytes>0)
            {
                OpType tailBlock=iobuf[blockNum];
                int tailBytes=typeSize-remainbytes;         //排除无效字节的影响
                 int tailBits=tailBytes*8;  //1Byte=8bit
                tailBlock>>tailBits;
                tailBlock<<tailBits;
                checkSum+=tailBlock;
            }
            if (nread<wantRead)
            {
                break;
            }
        }
    } 
    else //如果大于1M，取前、中、后各1024个节
    {
        unsigned char TempBuf[3*1024] = {0};
        fread(TempBuf,1,1024,FpSrc); //前1024
        fseek(FpSrc,file_entire_size/2,SEEK_SET);//把FpSrc指针移动到离文件开头中间处；
        fread(TempBuf+1024,1,1024,FpSrc); //中1024
        fseek(FpSrc,1024,SEEK_END);//把FpSrc指针退回到离文件结尾1024字节处。
        fread(TempBuf+2048,1,1024,FpSrc); //后1024

        size_t  nread=3*1024;
        size_t blockNum=nread/typeSize;
        size_t remainbytes=nread%typeSize;

        for (size_t n=0;n<blockNum;n++)
        {
            checkSum+=TempBuf[n];
        }

        if (remainbytes>0)
        {
            OpType tailBlock=TempBuf[blockNum];
            int tailBytes=typeSize-remainbytes;         //排除无效字节的影响
            int tailBits=tailBytes*8;
            tailBlock>>tailBits;
            tailBlock<<tailBits;
            checkSum+=tailBlock;
        }
    }

    fclose(FpSrc); 
    return checkSum;
}


//对拷贝前后文件做校验,避免拷贝错误的情况,为了效率可能会用自定义高效算法
bool CompareFileCheck(const char* path1,const char* path2)
{
    if (isNullCstr(path2)||isNullCstr(path1))
    {
        return false;
    }

    //文件大小不一样
    uint64_t sizeFile1=getFileSize(path1);
    uint64_t sizeFile2=getFileSize(path2);
    if (sizeFile2!=sizeFile1)
    {
        return false;
    }

    uint64_t checkSumFile1=getFileCheckSum(path2);
    uint64_t checkSumFile2=getFileCheckSum(path1);
    if (checkSumFile1!=checkSumFile2)
    {
        PreDprintf("failed File1:%s File2:%s \nCheckValue: File1:%llu File2:%llu\n",path1,path2,checkSumFile2,checkSumFile1);
        return false;
    }
    return true;
}

//
//bool copyFileByProgress(const char* path_dst,const char* path_src,uint64_t* totalSize,uint64_t* isCopyed)
//{  
//    FILE *fp_src, *fp_dst;  
//    if (isNullCstr(path_src)||isNullCstr(path_dst))
//    {
//        return false;
//    }
//
//
//    fp_src = fopen(path_src, "rb"); 
//    fp_dst = fopen(path_dst, "wb"); 
//    if(fp_src==NULL || fp_dst==NULL) 
//    {  
//        return false;
//    } 
//
//////低效，一次只能拷贝一个字节
////#if 0
////    unsigned char byte[16];
////    while(1)  
////    {  
////        fread(byte, sizeof(unsigned char), 1, fp_src);
////        if(feof(fp_src))
////        {  
////            break;  
////        }  
////        fwrite(byte, sizeof(unsigned char), 1, fp_dst);
////    }  
////#endif
//
//    size_t nread;
//    const size_t BUFSIZE=1*1024*1024;
//    char* iobuf=new char[BUFSIZE];
//    while((nread=fread(iobuf,1,BUFSIZE, fp_src))>0)
//    {
//        if(fwrite(iobuf,1,nread,fp_dst)!=nread)
//        {
//            printf("[cpfile] Target file write error!\n");
//            fclose(fp_dst);
//            fclose(fp_src);
//            return false;
//        }
//        else
//        {
//            //完成大小累加   待续
//            //(*totalSize)+=nread;
//            //if(*totalSize/(*totalSize) == 1)
//            //{
//            //	printf("complete_size= %lld \n",*totalSize);
//            //}
//        }
//    }
//
//    fflush(fp_dst);
//    fclose(fp_src);
//    fclose(fp_dst); 
//    delete[] iobuf;
//    return true;  
//}
//
//


bool deleteFile(const char * path)
{
     int ret=::remove(path);
     return ret==0;
}

bool deletePath(const char * path)
{
    int retType=isFileOrDirectory(path);
    if (retType==1)
    {
        return deleteFile(path);
    }
    else if (retType==2)
    {
        return deletePath(path);
    }
    return false;
}

bool RenamePath(const char* snew,const char* sold)
{
    if (isNullCstr(snew)||isNullCstr(sold))
    {
        return false;
    }

    //to test
#ifdef _WIN32
       Tstring spnew=FromCstr(snew);
       Tstring spold=FromCstr(snew);
        return ::MoveFile(spold.c_str(),spnew.c_str())==TRUE;
#else
        return ::rename(sold,snew) == 0;
#endif
}

//bool symlink(const char* dst, const char* lnk) 
//{
//    ////win32
//    //fs::remove(lnk);
//    //return CreateSymbolicLinkA(lnk, dst, fs::isdir(dst));
//
//    ////linux
//    //fs::remove(lnk);
//    //return ::symlink(dst, lnk) == 0;
//}



#if !defined(_WIN32)
//判断是否为目录
bool is_dir(const char *path)
{
	//测试是否是目录或文件
	bool bIsDir = true;
	DIR * pTempDir;
	if ((pTempDir=opendir(path)) == NULL) //说明不是目录是文件
	{
		bIsDir = false;
	}
	else
	{
		bIsDir = true;
		closedir(pTempDir); //判断文件还文件夹完毕
	}
    return bIsDir;
}

//判断是否为常规文件
bool is_file(const char *path)
{
	std::fstream file;
	file.open(path,std::ios::in);
	if(!file)
		return false;
	return true;
}

//判断是否是特殊目录
bool is_special_dir(const char *path)
{
    if(path==NULL)
    {
        return false;
    }
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

//生成完整的文件路径
void get_file_path(const char *path, const char *file_name,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, file_name);
}

//void deleteDirectory(const char *path)
//{
//    DIR *dir;
//    dirent *dir_info;
//    char file_path[PATH_MAX];
//    if(is_file(path))
//    {
//        remove(path);
//        return;
//    }
//    if(is_dir(path))
//    {
//        if((dir = opendir(path)) == NULL)
//            return;
//        while((dir_info = readdir(dir)) != NULL)
//        {
//            get_file_path(path, dir_info->d_name, file_path);
//            if(is_special_dir(dir_info->d_name))
//                continue;
//            deleteDirectory(file_path);
//            rmdir(file_path);
//        }
//    }
//}

void moveFilebySysCall(char *to,char *from)
{
    std::string cmd = std::string("mv ")+std::string(from)+std::string(" ")+std::string(to);
    system(cmd.c_str());
}
#endif










std::vector<std::string> getAllPath(const char* pDir)
{
    std::vector<std::string> Vresult;
    std::vector<FileInfo> VFileInfos;
    getFileInfo(VFileInfos,pDir);
    for (size_t n=0;n<VFileInfos.size();n++)
    {
         std::string tmp(VFileInfos.at(n).name);
         Vresult.push_back(tmp);
    }
    return Vresult;
}


//windows环境下需要加上dir+"\\*"
//去某个路径下的文件信息，可以传参决定是否递归子目录,depth表示递归深度，0表示全部


 void getFileInfoRecursive(std::vector<FileInfo>& VFileInfos,const char* dir,FunFileCond pFunCond,bool bSubDir,unsigned int maxDepth,unsigned int currDepth)
{
    if (isNullCstr(dir))
    {
        printf("getFileInfoRecursive() failed! dir isNull!\n");
        return;
    }

    //printf("getFileInfoRecursive() dir:%s \n",dir);

    if (currDepth>maxDepth)
    {
        //printf("getFileInfoRecursive() Out of depth range break! Depth:%u max:%u path:%s\n",currDepth,maxDepth,dir);
        return;
    }

#ifdef _WIN32
    char dirNew[1024];
    strcpy(dirNew,dir);
    strcat(dirNew,"\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

    _finddata_t finddata;
     intptr_t  handle;
    if ((handle=_findfirst(dirNew, &finddata)) == -1) 
    {
        printf("getSubFileInfoDirsRecursive() found not dir:%s\n",dirNew);
        return;
    }
    do

    {
        //是否是子目录 
        if (finddata.attrib&_A_SUBDIR)
        {
            // 忽略"."和".."两个结果 
            if (strcmp(finddata.name, ".") == 0 || strcmp(finddata.name, "..") == 0)
            {
                //printf("finddata.name :%s\n",finddata.name);
                continue;
            }

            //printf("-->is Dir:%s\n",finddata.name);

            // 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
            if (bSubDir)
            {
                char subDir[1024];
                strcpy(subDir,dir);
                strcat(subDir,"\\");
                strcat(subDir, finddata.name);
                getFileInfoRecursive(VFileInfos,subDir,pFunCond,bSubDir,maxDepth,currDepth+1);
            }
        }
        else  //是文件
        {
            //std::cout << finddata.name << "\t" << finddata.size << std::endl;
            FileInfo tmp;
            tmp.RelativeDepth=currDepth;
            strcpy(tmp.name,finddata.name);
            tmp.size=finddata.size;
            tmp.time_create=finddata.time_create;
            tmp.time_access=finddata.time_access;
            tmp.time_write=finddata.time_write;

            char currPath[1024];
            strcpy(currPath,dir);
            strcat(currPath,"\\");
            strcat(currPath, finddata.name);
            FormatPath(currPath,currPath);   //

            strcpy(tmp.fullPath,currPath);

            //printf("Find file path:%s\n",tmp.fullPath);

            //为了跨平台一致性,不直接使用windows下原始值
            int fileattrib=FileInfo::FileAttrib_NORMAL;
            switch (finddata.attrib)
            {
            case _A_NORMAL:      //0x00    /* Normal file - No read/write restrictions */
                fileattrib=fileattrib|FileInfo::FileAttrib_NORMAL;break;
            case _A_RDONLY:       //0x01    /* Read only file */
               fileattrib=fileattrib|FileInfo::FileAttrib_RDONLY;break;
            case _A_HIDDEN:       //0x02    /* Hidden file */
               fileattrib=fileattrib|FileInfo::FileAttrib_HIDDEN;break;
            case _A_SYSTEM:       //0x04    /* System file */
              fileattrib=fileattrib|FileInfo::FileAttrib_SYSTEM;break;
            case _A_SUBDIR:       //0x10    /* Subdirectory */
              fileattrib=fileattrib|FileInfo::FileAttrib_SUBDIR;break;
            case _A_ARCH:         //0x20    /* Archive file */
              fileattrib=fileattrib|FileInfo::FileAttrib_ARCH;break;
            }

            tmp.attrib=fileattrib;

            if (pFunCond!=NULL)     //过滤条件函数指针非空,则根据函数返回值决定是否加入查找结果。和是否中止搜索
            {
                bool bBreak=false;
                bool bMatch=pFunCond(tmp,bBreak);
                if (bMatch)
                {
                      VFileInfos.push_back(tmp);
                }
                if (bBreak)
                {
                    break;
                }
            }
            else
            {
                VFileInfos.push_back(tmp);
            }
        }
    } while (_findnext(handle, &finddata) == 0);    // 查找目录中的下一个文件
    _findclose(handle);


#else   //Linux下待完善，待测试



#if 0
    struct dirent
    {
          long d_ino; /* inode number 索引节点号 */
          off_t d_off; /* offset to this dirent 在目录文件中的偏移 */
          unsigned short d_reclen; /* length of this d_name 文件名长 */
          unsigned char d_type; /* the type of d_name 文件类型 */
          char d_name [NAME_MAX+1]; /* file name (null-terminated) 文件名，最长255字符 */
    }
    struct stat 
    {
        mode_t     st_mode;       //文件访问权限
        ino_t      st_ino;       //索引节点号
        dev_t      st_dev;        //文件使用的设备号
        dev_t      st_rdev;       //设备文件的设备号
        nlink_t    st_nlink;      //文件的硬连接数
        uid_t      st_uid;        //所有者用户识别号
        gid_t      st_gid;        //组识别号
        off_t      st_size;       //以字节为单位的文件容量
        time_t     st_atime;      //最后一次访问该文件的时间
        time_t     st_mtime;      //最后一次修改该文件的时间
        time_t     st_ctime;      //最后一次改变该文件状态的时间
        blksize_t st_blksize;    //包含该文件的磁盘块的大小
        blkcnt_t   st_blocks;     //该文件所占的磁盘块
    };
#endif


    char dirNew[1024];
    strcpy(dirNew,dir);
    DIR* pDir;
    struct dirent *ptr;
    struct stat statbuf;
    if ((pDir=opendir(dirNew)) == NULL)
     {
        perror("Open dir error...");
        printf("%s\n", dirNew);
        return;// files;  
        //exit(1);
    }
    while ((ptr=readdir(pDir))!= NULL)
    {
        DIR* pTempDir;
        char szTempDir[2048] = {0};
        strcpy(szTempDir,dir);
        strcat(szTempDir,"/");
        strcat(szTempDir,ptr->d_name);
        FormatPath(szTempDir,szTempDir);  //整理路径格式
        bool bIsDir = true;
        //测试是否是目录或文件
        if ((pTempDir=opendir(szTempDir))==NULL) //说明不是目录是文件
        {
            bIsDir = false;
        }
        else
        {
            bIsDir = true;
            closedir(pTempDir); //判断文件还文件夹完毕
        }

        //开始逻辑操作
        if (bIsDir) 
        {
            if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0 )  
            {
                continue;
            }
            //递归遍历子目录
            if (bSubDir)
            {
                getFileInfoRecursive(VFileInfos,szTempDir,pFunCond,bSubDir,maxDepth,currDepth+1);
            }
        }
        else    //否则我们认为它是一个文件
        {
            //std::string tmp = szTempDir;
            //VFileInfos.push_back(tmp);
            FileInfo tmp;
            strcpy(tmp.name,ptr->d_name);
            strcpy(tmp.fullPath,szTempDir);
            //strcat(szTempDir,"/");
            //strcat(szTempDir,ptr->d_name);


            int ret=lstat(ptr->d_name,&statbuf);
            if (ret<0)
            {
                VFileInfos.push_back(tmp);
            }
            else
            {
                if(ptr->d_type==DT_REG)    ///file 8
                {
                    //FileInfo tmp;
                    //Linux 文件信息待补全
                    strcpy(tmp.name,ptr->d_name);
                    tmp.size=statbuf.st_size;       //以字节为单位的文件容量
                    tmp.time_create=statbuf.st_ctime;      //最后一次改变该文件状态的时间  //创建时间对应不上？
                    tmp.time_access=statbuf.st_atime;      //最后一次访问该文件的时间
                    tmp.time_write=statbuf.st_mtime;       //最后一次修改该文件的时间
                    //tmp.attrib=finddata.attrib;

                    //tmp.attrib=statbuf.;  // TODO  文件属性待续

                    //拼接文件全路径
                    char currPath[1024];
                    strcpy(currPath,dir);
                    strcat(currPath,"/");
                    strcat(currPath,ptr->d_name);
                    FormatPath(currPath,currPath);  //整理路径格式
                    strcpy(tmp.fullPath,currPath);

                    if (pFunCond!=NULL)     //过滤条件函数指针非空,则根据函数返回值决定是否加入查找结果。和是否中止搜索
                    {
                        bool bBreak=false;
                        bool bMatch=pFunCond(tmp,bBreak);
                        if (bMatch)
                        {
                            VFileInfos.push_back(tmp);
                        }
                        if (bBreak)
                        {
                            break;
                        }
                    }
                    else
                    {
                        VFileInfos.push_back(tmp);
                    }
                }
            }
        }


        //lstat(ptr->d_name, &statbuf);
        //if (S_ISDIR(statbuf.st_mode)) 
        //{
        //    ///current dir OR parrent dir
        //    if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0 )  
        //        continue;
        //}

        //if(ptr->d_type ==DT_REG )    ///file 8
        //{
        //    //Linux 文件信息待补全
        //    FileInfo tmp;
        //    strcpy(tmp.name,ptr->d_name);
        //    tmp.size=statbuf.st_size;       //以字节为单位的文件容量
        //    tmp.time_create=statbuf.st_ctime;      //最后一次改变该文件状态的时间  //创建时间对应不上？
        //    tmp.time_access=statbuf.st_atime;      //最后一次访问该文件的时间
        //    tmp.time_write=statbuf.st_mtime;       //最后一次修改该文件的时间
        //    //tmp.attrib=finddata.attrib;

        //    //tmp.attrib=statbuf.;  // TODO  文件属性待续

        //    //拼接文件全路径
        //    char currPath[1024];
        //    strcpy(currPath,dir);
        //    strcat(currPath,"/");
        //    strcat(currPath,ptr->d_name);
        //    FormatPath(currPath,currPath);  //整理路径格式
        //    strcpy(tmp.fullPath,currPath);

        //    if (pFunCond!=NULL)     //过滤条件函数指针非空,则根据函数返回值决定是否加入查找结果。和是否中止搜索
        //    {
        //        bool bBreak=false;
        //        bool bMatch=pFunCond(tmp,bBreak);
        //        if (bMatch)
        //        {
        //            VFileInfos.push_back(tmp);
        //        }
        //        if (bBreak)
        //        {
        //            break;
        //        }
        //    }
        //    else
        //    {
        //        VFileInfos.push_back(tmp);
        //    }

        //}
        //else if(ptr->d_type == DT_LNK)    ///link file 10
        //{
        //    //printf("d_name:%s/%s\n",basePath,ptr->d_name);
        //    continue;
        //}
        //else if(ptr->d_type == DT_DIR)    ///dir 4
        //{ 
        //    //VFileInfos.push_back(ptr->d_name);
        //    
        //    //递归遍历子目录
        //    if (bSubDir)
        //    {
        //        char subDir[1024];
        //        memset(subDir,'\0',sizeof(subDir));
        //        strcpy(subDir,dir);
        //        strcat(subDir,"/");
        //        strcat(subDir,ptr->d_name);
        //        FormatPath(subDir,subDir);  //整理路径格式

        //        getFileInfoRecursive(VFileInfos,subDir,pFunCond,bSubDir,maxDepth,currDepth+1);
        //    }
        //}
    }
    closedir(pDir);
#endif
}
void getFileInfo(std::vector<FileInfo>& VFileInfos,const char* dir,FunFileCond pFC,bool bSubDir,unsigned int MaxDepth)
{
    getFileInfoRecursive(VFileInfos,dir,pFC,bSubDir,MaxDepth,0);
}


std::vector<std::string> getFilePath(const char* dir,const char* SuffixNames,bool bSubDir,unsigned int MaxDepth)
{
    if (isNullCstr(dir))
    {
        PreDprintf("failed! dir isNull!\n");
        return std::vector<std::string>();
    }

    std::vector<FileInfo> VFileInfos;
    getFileInfo(VFileInfos,dir,NULL,bSubDir,MaxDepth);
    std::vector<std::string> listResult;
    bool useSuffix=true;
    if (isNullCstr(SuffixNames))
    {
        useSuffix=false;
    }

    if (!useSuffix)
    {
        for (int n=0;n<VFileInfos.size();n++)
        {
            const FileInfo& fileInfo=VFileInfos[n];
            //printf("Find path allSuffix:%s\n",fileInfo.fullPath);
            listResult.push_back(fileInfo.fullPath);
        }
    }
    else
    {
        std::vector<std::string> ListSuffix=Split(SuffixNames,"|");
        printf("ListSufffix:size()=%d\n",ListSuffix.size());
        for (size_t n=0;n<ListSuffix.size();n++)
        {
            const std::string& sSuffix=ListSuffix.at(n);
            printf("Suffix[%d]:%s\n",(int)n,sSuffix.c_str());
        }

        if (ListSuffix.empty())
        {

        }
        else
        {
            for (int n=0;n<ListSuffix.size();n++)
            {
                const std::string& sSuffix=ListSuffix[n];
                for (int k=0;k<VFileInfos.size();k++)
                {
                    const FileInfo& fileInfo=VFileInfos[k];
                    //111.XML.TXT.JPG //更精确的排除文件名的包含--待续
                    //if (strstr(fileInfo.name,sSuffix.c_str())!=NULL)      //文件名包含特定后缀  
                    if (endsWith(fileInfo.name,sSuffix.c_str(),false))   //判断后缀并忽略大小写
                    {
                        //printf("Find suffix:%s path:%s\n",sSuffix.c_str(),fileInfo.fullPath);
                        listResult.push_back(fileInfo.fullPath);
                    }
                }
            }
        }
    }
    return listResult;
}


void getAllFileFullPathRecursive(std::vector<std::string>& VFileInfos,const char* dir,FunFileCond pFunCond,bool bSubDir,unsigned int maxDepth,unsigned int currDepth)
{
	if (isNullCstr(dir))
	{
		printf("getFileInfoRecursive() failed! dir isNull!\n");
		return;
	}
	if (currDepth>maxDepth) //超过深度
	{
		return;
	}

#ifdef _WIN32

    //TOBE TEST
    char dirNew[1024];
    strcpy(dirNew,dir);
    strcat(dirNew,"\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

    _finddata_t finddata;
    intptr_t  handle;
    if ((handle=_findfirst(dirNew, &finddata)) == -1) 
    {
        printf("getSubFileInfoDirsRecursive() found not dir:%s\n",dirNew);
        return;
    }
    do

    {
        //是否是子目录 
        if (finddata.attrib&_A_SUBDIR)
        {
            // 忽略"."和".."两个结果 
            if (strcmp(finddata.name, ".") == 0 || strcmp(finddata.name, "..") == 0)
            {
                //printf("finddata.name :%s\n",finddata.name);
                continue;
            }

            //printf("-->is Dir:%s\n",finddata.name);

            // 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
            if (bSubDir)
            {
                char subDir[1024];
                strcpy(subDir,dir);
                strcat(subDir,"\\");
                strcat(subDir, finddata.name);
                getAllFileFullPathRecursive(VFileInfos,subDir,pFunCond,bSubDir,maxDepth,currDepth+1);
            }
        }
        else  //是文件
        {
            //std::cout << finddata.name << "\t" << finddata.size << std::endl;
            FileInfo tmp;
            tmp.RelativeDepth=currDepth;
            strcpy(tmp.name,finddata.name);
            tmp.size=finddata.size;
            tmp.time_create=finddata.time_create;
            tmp.time_access=finddata.time_access;
            tmp.time_write=finddata.time_write;

            char currPath[1024];
            strcpy(currPath,dir);
            strcat(currPath,"\\");
            strcat(currPath, finddata.name);
            FormatPath(currPath,currPath);   //

            strcpy(tmp.fullPath,currPath);

            //printf("Find file path:%s\n",tmp.fullPath);

            //为了跨平台一致性,不直接使用windows下原始值
            int fileattrib=FileInfo::FileAttrib_NORMAL;
            switch (finddata.attrib)
            {
            case _A_NORMAL:      //0x00    /* Normal file - No read/write restrictions */
                fileattrib=fileattrib|FileInfo::FileAttrib_NORMAL;break;
            case _A_RDONLY:       //0x01    /* Read only file */
                fileattrib=fileattrib|FileInfo::FileAttrib_RDONLY;break;
            case _A_HIDDEN:       //0x02    /* Hidden file */
                fileattrib=fileattrib|FileInfo::FileAttrib_HIDDEN;break;
            case _A_SYSTEM:       //0x04    /* System file */
                fileattrib=fileattrib|FileInfo::FileAttrib_SYSTEM;break;
            case _A_SUBDIR:       //0x10    /* Subdirectory */
                fileattrib=fileattrib|FileInfo::FileAttrib_SUBDIR;break;
            case _A_ARCH:         //0x20    /* Archive file */
                fileattrib=fileattrib|FileInfo::FileAttrib_ARCH;break;
            }

            tmp.attrib=fileattrib;

            if (pFunCond!=NULL)     //过滤条件函数指针非空,则根据函数返回值决定是否加入查找结果。和是否中止搜索
            {
                bool bBreak=false;
                bool bMatch=pFunCond(tmp,bBreak);
                if (bMatch)
                {
                    //VFileInfos.push_back(tmp);
                    VFileInfos.push_back(tmp.fullPath);

                }

                if (bBreak)
                {
                    break;
                }
            }
            else
            {
                //VFileInfos.push_back(tmp);
                VFileInfos.push_back(tmp.fullPath);
            }
        }
    } while (_findnext(handle, &finddata) == 0);    // 查找目录中的下一个文件
    _findclose(handle);

#else  
	char dirNew[1024];
	strcpy(dirNew,dir);
	DIR* pDir;
	struct dirent *ptr;
	if ((pDir=opendir(dirNew)) == NULL)
	{
		perror("Open dir error...");
		printf("%s\n", dirNew);
		return;// files;  
		//exit(1);
	}
	while ((ptr=readdir(pDir)) != NULL)
	{
		DIR* pTempDir;
		char szTempDir[2048] = {0};
		strcpy(szTempDir,dir);
		strcat(szTempDir,"/");
		strcat(szTempDir,ptr->d_name);
		FormatPath(szTempDir,szTempDir);  //整理路径格式
		bool bIsDir = true;
		//测试是否是目录或文件
		if ((pTempDir=opendir(szTempDir)) == NULL) //说明不是目录是文件
		{
			bIsDir = false;
		}
		else
		{
			bIsDir = true;
			closedir(pTempDir); //判断文件还文件夹完毕
		}

		//开始逻辑操作
		if (bIsDir) 
		{
			if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0 )  
			{
				continue;
			}
			//递归遍历子目录
			if (bSubDir)
			{
                getAllFileFullPathRecursive(VFileInfos,szTempDir,pFunCond,bSubDir,maxDepth,currDepth+1);
			}
		}
		else    //否则我们认为它是一个文件
		{
			std::string tmp = szTempDir;
			VFileInfos.push_back(tmp);
		}
	}
	closedir(pDir);
#endif
}

//
//void getAllFileFullPath(std::vector<std::string>& VFileInfos,const char* dir,FunFileCond pFC,bool bSubDir,unsigned int MaxDepth)
//{
//	getAllFileFullPathRecursive(VFileInfos,dir,pFC,bSubDir,MaxDepth,0);
//}
//
//std::vector<std::string> getFilePathFtp(const char* dir,const char* SuffixNames,bool bSubDir,unsigned int MaxDepth)
//{
//	if (isNullCstr(dir))
//    {
//        PreDprintf("failed! dir isNull!\n");
//        return std::vector<std::string>();
//    }
//
//    std::vector<std::string> VFileInfos;
//    getAllFileFullPath(VFileInfos,dir,NULL,bSubDir,MaxDepth);
//    std::vector<std::string> listResult;
//    bool useSuffix=true;
//    if (isNullCstr(SuffixNames))
//    {
//        useSuffix=false;
//    }
//
//    if (!useSuffix)
//    {
//        for (int n=0;n<VFileInfos.size();n++)
//        {
//            listResult.push_back(VFileInfos[n]);
//        }
//    }
//    else
//    {
//        std::vector<std::string> ListSuffix=Split(SuffixNames,"|");
//        printf("ListSufffix:size()=%d\n",ListSuffix.size());
//        for (size_t n=0;n<ListSuffix.size();n++)
//        {
//            const std::string& sSuffix=ListSuffix.at(n);
//            printf("Suffix[%d]:%s\n",(int)n,sSuffix.c_str());
//        }
//
//        if (ListSuffix.empty())
//        {
//
//        }
//        else
//        {
//            for (int n=0;n<ListSuffix.size();n++)
//            {
//                const std::string& sSuffix=ListSuffix[n];
//                for (int k=0;k<VFileInfos.size();k++)
//                {
//                    if (endsWith(VFileInfos[k],sSuffix.c_str(),false))   //判断后缀并忽略大小写
//                    {
//                        //printf("Find suffix:%s path:%s\n",sSuffix.c_str(),fileInfo.fullPath);
//                        listResult.push_back(VFileInfos[k]);
//                    }
//                }
//            }
//        }
//    }
//    return listResult;
//}


bool getSubDirsRecursive(std::vector<std::string>& ListDirs,const char* sDir,FunDirCond pFC,unsigned int maxDepth,unsigned int currDepth)
{
    //printf("getSubDirsRecursive() sDir:%s\n",sDir);
    if (isNullCstr(sDir))
    {
        PreDprintf("failed! dir isNull!\n");
        return true;
    }
    if (currDepth>maxDepth)
    {
        //printf("getSubDirsRecursive() Out of depth range break! Depth:%u max:%u DirPath:%s\n",currDepth,maxDepth,sDir);
        return true;
    }

#ifdef _WIN32

    char dirNew[1024];
    strcpy(dirNew,sDir);
    strcat(dirNew,"\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

    _finddata_t finddata;
    intptr_t  handle;
    //const char* mode = "\\*.*";    
    //std::string sOpenPath=sDir+mode;     //_findfirst 必须加\\*.*搜索，否则失败

    if ((handle=_findfirst(dirNew, &finddata))==-1) 
    {
        PreDprintf("found not dir:%s\n",dirNew);
        return true;  //true 表示继续搜索
    }


    //printf("Dind Dir:%s\n",finddata.name);

    //如果搜索过滤条件函数指针非空,根据返回值决定是否加入搜索结果
    if (pFC!=NULL)
    {
        bool bBreak=false;
        bool bMatch=pFC(sDir,bBreak);
        if (bMatch)
        {
            ListDirs.push_back(std::string(sDir));
            //printf("Find Dir With Cond and push ListSize:%d path:%s\n",ListDirs.size(),sDir);
        }
        //中止搜索
        if (bBreak)
        {
             return false;
        }
    }
    else
    {
        ListDirs.push_back(std::string(sDir));
        //printf("Find Dir and push ListSize:%d path:%s\n",ListDirs.size(),sDir);
    }


    do
    {
        //是否是子目录 
        if (finddata.attrib&_A_SUBDIR)
        {
                // 忽略"."和".."两个结果 
                if (strcmp(finddata.name,".") == 0 || strcmp(finddata.name,"..") == 0)
                {
                    //printf("finddata.name :%s\n",finddata.name);
                    continue;
                }

                //在目录后面加上"\\"和搜索到的目录名进行下一次搜索
               char subDir[1024];
               strcpy(subDir,sDir);
               strcat(subDir,"\\");
               strcat(subDir,finddata.name);
               FormatPath(subDir,subDir);//

               bool bContinue=getSubDirsRecursive(ListDirs,subDir,pFC,maxDepth,currDepth+1);
               if (!bContinue)
               {
                   break;
               }
        }
        else  //是文件
        {
            
        }
    } while (_findnext(handle,&finddata)==0);    // 查找目录中的下一个文件
    _findclose(handle);
    return true;
#else
	char dirNew[2048];
	strcpy(dirNew,sDir);
	DIR* pDir;
	struct dirent *ptr;
	if ((pDir=opendir(dirNew)) == NULL)
	{
        perror("Open dir error...");
        printf("%s\n", dirNew);
        return true; //true 表示继续搜索
	}

	//如果搜索过滤条件函数指针非空,根据返回值决定是否加入搜索结果
	if (pFC!=NULL)
	{
		bool bBreak=false;
		bool bMatch=pFC(sDir,bBreak);
		if (bMatch)
		{
			ListDirs.push_back(std::string(sDir));
            printf("Find Dir With Cond and push ListSize:%d path:%s\n",(int)ListDirs.size(),sDir);
		}
		//中止搜索
		if (bBreak)
		{
			return false;
		}
	}
	else
	{
		ListDirs.push_back(std::string(sDir));
        printf("Find Dir and push ListSize:%d path:%s\n",(int)ListDirs.size(),sDir);
	}

	while ((ptr=readdir(pDir)) != NULL) //查找目录中的下一个子目录
	{
		//测试是否是目录或文件
		DIR* pTempDir;
		char szTempDir[2048] = {0};
		strcpy(szTempDir,sDir);
		strcat(szTempDir,"/");
		strcat(szTempDir,ptr->d_name);
		FormatPath(szTempDir,szTempDir);  //整理路径格式
		bool bIsDir = true;
		//测试是否是目录或文件
		if ((pTempDir=opendir(szTempDir)) == NULL) //说明不是目录是文件
		{
			bIsDir = false;
		}
		else
		{
			bIsDir = true;
			closedir(pTempDir); //判断文件还文件夹完毕
		}


		if (bIsDir) 
		{
			///current dir OR parrent dir
			if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0 )  
            {
				continue;
            }

			//递归遍历子目录
			bool bContinue = getSubDirsRecursive(ListDirs,szTempDir,pFC,maxDepth,currDepth+1);
			if (!bContinue)
			{
				break;
			}
        }
	}
	closedir(pDir);
    return true;
#endif
}

static std::string removeTail(const std::string& s,const std::string& sSuffix)
{
    std::string sRes;
    size_t pos=s.find_last_of(sSuffix);
    if (pos!=std::string::npos)
    {
        sRes=s.substr(0,pos);
        return sRes;
    }
    return s;
}


std::vector<std::string> getSubDirs(const char* pDir,FunDirCond pFC,unsigned int maxDepth)
{
    std::vector<std::string> ListDirs;
    //getSubDirsRecursive(ListDirs,pDir,pFC,maxDepth,0);

    std::string sdir(pDir);
    //sdir=removeTail(sdir,"\\");
    //sdir=removeTail(sdir,"/");
    sdir=delDirTailSeparatorIfHas(sdir);
    getSubDirsRecursive(ListDirs,sdir.c_str(),pFC,maxDepth,0);
    return ListDirs;
}


std::string getFileNameFromPath(const std::string& sPath)
{
#if 1
    std::string sTmp=FormatPath(sPath);   //先整理路径格式统一成/分割的格式
    std::string sFileName;
    size_t  pos;
    if ((pos=sTmp.find_last_of('/'))!=std::string::npos)
    {
        sFileName=sTmp.substr(pos+1);
    }
    return sFileName;
#else
    std::string s=sPath;
    s=delDirTailSeparatorIfHas(s);  //先移除目录分割符号

    std::string sFileName;
    size_t  pos;
    if ((pos=s.find_last_of('/'))!=std::string::npos)
    {
        sFileName=s.substr(pos+1);
    }
    else  if ((pos=s.find_last_of('\\'))!=std::string::npos)
    {
        sFileName=s.substr(pos+1);
    }
    return sFileName;
#endif
}

std::string getDirFromFullPath(const std::string& sPath)
{
    std::string sTmp=FormatPath(sPath);   //先整理路径格式统一成/分割的格式
    std::string sParentDir;
    size_t  pos;
    if ((pos=sTmp.find_last_of('/'))!=std::string::npos)
    {
        //sParentDir=sTmp.substr(pos+1);
        sParentDir=sTmp.substr(0,pos);
    }
    return sParentDir;
}

std::string getExtensionFromPath(const std::string& sPath)
{
    std::string sName;
    size_t  pos;
    if ((pos=sPath.find_last_of('.'))!=std::string::npos)
    {
        sName=sPath.substr(pos);
    }
    return sName;
}

std::string getExtensionFromPathNotDot(const std::string& sPath)
{
    std::string sName;
    size_t  pos;
    if ((pos=sPath.find_last_of('.'))!=std::string::npos)
    {
        size_t subpos=pos+1;
        if (subpos<sPath.size())
        {
            sName=sPath.substr(subpos);
        }
    }
    return sName;
}

std::string delDirTailSeparatorIfHas(const std::string& sPath)
{
    //if (endsWith(sPath,"/")||endsWith(sPath,"\\"))
    //{
    //    return sPath.substr(0,sPath.length()-1);
    //}
    //return sPath;
    std::string sTmp=FormatPath(sPath);
    if (endsWith(sTmp,"/"))
    {
        return sTmp.substr(0,sTmp.length()-1);
    }
    return sPath;
}

std::string addDirSeparatorIfnot(const std::string& sPath)
{
    //if (endsWith(sPath,"/")||endsWith(sPath,"\\"))
    //{
    //    return sPath;
    //}
    //return sPath+"/";
    std::string sTmp=FormatPath(sPath);
    if (endsWith(sTmp,"/"))
    {
        return sTmp;
    }
    return sTmp+"/";
}


//by lizulin
void SplitPath(const char* fullpath,char* drive,char* dir,char* filename,char* ext)
{
    if(fullpath==NULL)
    {
        drive[0]='\0'; 
        dir[0]='\0'; 
        filename[0]='\0'; 
        ext[0]='\0'; 
        return;
    }

    char drive_tmp[1024]={0};
    char dir_tmp[1024]={0};
    char filename_tmp[1024]={0};
    char ext_tmp[1024]={0};

     //char fullpathtmp[1024];
     size_t fullpathLen=strlen(fullpath);
     const char* pfullpathEnd=fullpath+fullpathLen;
     drive_tmp[0]='\0'; 
     dir_tmp[0]='\0'; 
     filename_tmp[0]='\0'; 
     ext_tmp[0]='\0'; 

     const char* pDrivetail=strchr(fullpath,'\\' );   //取最第一个下划线位置
     if (pDrivetail==NULL)  
     {
            pDrivetail=strchr(fullpath, '/' );        
     }
     if (pDrivetail!=NULL)                       //取驱动器名称
     {
         size_t len=pDrivetail-fullpath;
         memcpy(drive_tmp,fullpath,len);
         drive_tmp[len]='\0'; 
     }

     if ('/'==fullpath[fullpathLen]||'\\'==fullpath[fullpathLen])   //只有路径，没有文件名
     {
         filename_tmp[0]='\0'; 
     }

     const char* pDirtail=strrchr(fullpath,'\\' );   //取最后一个斜杠位置
     if (pDirtail==NULL)  
     {
          pDirtail=strrchr(fullpath, '/' );   //反斜杠
     }

     if (pDirtail!=NULL)                       //取得路径与文件名
     {
         size_t len=pDirtail-fullpath;
         memcpy(dir_tmp,fullpath,len);
         dir_tmp[len]='\0'; 

         len=pfullpathEnd-(pDirtail+1);
         memcpy(filename_tmp,pDirtail+1,len);
         filename_tmp[len]='\0';
     }
     else  //没有子路径
     {
         //memcpy(filename_tmp,fullpath,fullpathLen);
         //filename_tmp[fullpathLen]='\0'; 
         filename_tmp[0]='\0'; 
     }

     const char* pext_pot=strrchr(fullpath, '.' );   //取文件后缀分隔符. 的位置
     if (pext_pot!=NULL)  
     {
         if (pext_pot>pDirtail)    //   避免出现 C:\\111\\222.2\\test   最后一个点要出现在最后一个/之后
         {
             size_t len=pfullpathEnd-(pext_pot+1);
             memcpy(ext_tmp,pext_pot+1,len);
             ext_tmp[len]='\0'; 
         }
     }

     if (drive!=NULL)
     {
         strcpy(drive,drive_tmp);
     }
     if (dir!=NULL)
     {
         strcpy(dir,dir_tmp);
     }
     if (filename!=NULL)
     {
         strcpy(filename,filename_tmp);
     }
     if (ext!=NULL)
     {
         strcpy(ext,ext_tmp);
     }
     printf("SplitPath() \nfullpath:%s\ndrive:%s\ndir:%s\nname:%s\next:%s\n",fullpath,drive,dir,filename,ext);
}

void SplitPath(const std::string fullpath,std::string& sdrive,std::string& sdir,std::string& sfilename,std::string& sext)
{
    char drive[1024];
    char dir[1024];
    char filename[1024];
    char ext[1024];
    SplitPath(fullpath.c_str(),drive,dir,filename,ext);
    sdrive=std::string(drive);
    sdir=std::string(dir);
    sfilename=std::string(filename);
    sext=std::string(ext);
}


//返回0表示操作成功
bool readFiletoBuf(const char* path,char* buf,size_t size,bool textMode) //从某个文件读取size大小到buf内存
{
    FILE* fp;
    const char* sfMode=(textMode)?("r"):("rb");
    fp=fopen(path,sfMode);
    if(NULL==fp)
    {
        printf("readFiletoBuf(): open file error\n");
        return false;
    }
    size_t count=fread(buf,sizeof(char),size,fp);
    if(count!=size)
    {
        printf("readFiletoBuf(): read error\n");
        return false;
    }
    fclose(fp);
    return true;
}


//用内存buf内的数据大小为size,根据文件路径path,写入到本地
bool writeBuftoFile(const char* path,const char* buf,size_t size,bool textMode)
{
    FILE* fp;
    const char* sfMode=(textMode)?("w+"):("wb+");
    fp=fopen(path,sfMode);
    if(NULL==fp)
    {
        printf("fopen error in writeBuftoFile()\n");
        return false;
    }
    size_t count=fwrite(buf,sizeof(char),size,fp);
    fclose(fp);

    if(count!=size)
    {
        printf("fwrite error in writeBuftoFile()\n");
        return false;
    }
    return true;
}

BASIC_API char* readFiletoNewBuf(const char* path,size_t& size,bool textMode)
{
    size=getFileSize(path);
    if (size==0)
    {
        return NULL;
    }

    FILE* fp;
    const char* sfMode=(textMode)?("r"):("rb");
    fp=fopen(path,sfMode);
    if(NULL==fp)
    {
        printf("readFiletoBuf(): open file error\n");
        return NULL;
    }

    char* buf=new(std::nothrow) char[size];
    if (!buf)
    {
        return NULL;
    }
    size_t count=fread(buf,sizeof(char),size,fp);
    if(count!=size)
    {
        printf("readFiletoBuf(): read error\n");
        delete[] buf;
        return NULL;
    }
    fclose(fp);
    return buf;
}


bool SaveStringToFile(std::string sPath,const std::string& sTxt)
{
    FILE* fp=::fopen(sPath.c_str(),"wb");
    if (fp==NULL)
    {
        return false;
    }
    size_t wantWrite=sTxt.length();
    size_t nWrite=fwrite(sTxt.c_str(),1,wantWrite,fp);
    fclose(fp);
    return nWrite==wantWrite;
}

std::string FileToString(const std::string sPath,bool* isOK/*=NULL*/)
{
    if (isOK!=NULL)
    {
        *isOK=false;
    }
    FILE* fp=::fopen(sPath.c_str(),"rb");
    if (fp==NULL)
    {
        return "";
    }
    size_t fileSize=getFileSize(sPath.c_str());
    size_t wantRead=fileSize;
    char* pBufNew=new(std::nothrow) char[fileSize];
    if (pBufNew==NULL)
    {
        return "";
    }
    size_t nRead=fread(pBufNew,1,fileSize,fp);
    fclose(fp);
    std::string sTmp=std::string(pBufNew,fileSize);
    delete[] pBufNew;
    if (nRead==wantRead)
    {
        if (isOK!=NULL)
        {
            *isOK=true;
        }
    }
    return sTmp;
}

bool FileToString(const std::string sPath,std::string& sTxt)
{
    bool isOK;
    sTxt=FileToString(sPath,&isOK);
    return isOK;
}


//字符串分割函数  std::string 没有split函数
//pattern为用来分割原字符串的标志 如- ， 等等
std::vector<std::string> Split(const std::string& strIn,const std::string& pattern)
{
   std::string::size_type pos;
   std::vector<std::string> result;
   std::string str=strIn;
   str+=pattern;//扩展字符串以方便操作
   size_t size=str.size();
   for(size_t i=0; i<size; i++)
   {
       pos=str.find(pattern,i);
       if(pos<size)
       {
           std::string s=str.substr(i,pos-i);
           result.push_back(s);
           i=pos+pattern.size()-1;
       }
   }
   return result;
}


//std::string中替换子字符串的函数
//#include <string>
//void Replace( std::string &strBig, const std::string &sSrc, const std::string &sDst )
//{
//    std::string::size_type pos = 0;
//    std::string::size_type srclen = sSrc.size();
//    std::string::size_type dstlen = sDst.size();
//    while((pos=strBig.find(sSrc,pos))!=std::string::npos )
//    {
//        strBig.replace(pos,srclen,sDst);
//        pos += dstlen;
//    }
//} 

std::string Replace(const std::string&sOld,const std::string &sSrc, const std::string &sDst)
{
    std::string sNew=sOld;
    std::string::size_type pos = 0;
    std::string::size_type srclen = sSrc.size();
    std::string::size_type dstlen = sDst.size();
    while((pos=sNew.find(sSrc,pos))!=std::string::npos )
    {
        sNew.replace(pos,srclen,sDst);
        pos += dstlen;
    }
    return sNew;
}

std::string Trim_head_tail_InvisibleChar(std::string& str)
{
    const char* pBegin=str.c_str();
    const char* pEnd=str.c_str()+str.length();
    const char* pBeginNew=pBegin;
    const char* pEndNew=pEnd;
    const char* pos;
    //size_t oldlen=str.length();
    //寻找字符串第一个可见字符位置
    for (pos=pBegin;pos<pEnd;pos++)
    {
        char tmp=*pos;
        if (tmp==' '||tmp=='\r'||tmp=='\n')  //不可见字符
        {
            pBeginNew=pos+1;
        }
        else
        {
            pBeginNew=pos;
            break;
        }
    }
    //寻找字符串最后一个可见字符位置
    for (pos=pEnd;pos>=pBegin;pos--)
    {
        char tmp=*pos;
        if (tmp==' '||tmp=='\r'||tmp=='\n')  //不可见字符
        {
            pEndNew=pos;
        }
        else
        {
            pEndNew=pos+1;
            break;
        }
    }

    return std::string(pBeginNew,pEndNew-pBeginNew);
}


#include <ctype.h>   //for std::toupper

template<typename T>
bool upperEqual(T a,T b)
{
    return ((::islower(a)?a-32:a) == (::islower(b)?b-32:b))?true:false;
}

static bool comp_wchar_case_insensitive(wchar_t a, wchar_t b)
{
    return toupper(a)==toupper(b);
}

bool ComIgnoreCase(char a,char b)
{
    //return ((::islower(a)?a-32:a) == (::islower(b)?b-32:b) ) ? true : false;

    //faster ?
    if (::islower(a))
    {
        a=a-32;
    }
    if (::islower(b))
    {
        b=b-32;
    }
    return a==b;
}


bool Comp(const std::string& str1,const std::string& str2,bool caseSensitive)
{
     if (caseSensitive)
     {
         return str1==str2;
     }
        //To be tested
#ifdef _WIN32
        return _stricmp(str1.c_str(),str2.c_str())==0;
#else
        return strcasecmp(str1.c_str(),str2.c_str())==0;
#endif
}


BASIC_API bool Comp(const std::wstring& str1,const std::wstring& str2,bool caseSensitive/*=false*/)
{
    //大小不相等，没有必要下一步
    if (!str1.size()!=str2.size())
    {
        return false;
    }

    if (caseSensitive)   //考虑大小写
    {
        return str1==str2;
    }

    //        //To be tested
    //#ifdef _WIN32
    //        return _wcsnicmp(str1.c_str(),str2.c_str())==0;
    //#else
    //        return strcasecmp(str1.c_str(),str2.c_str())==0;
    //#endif

    //忽略大小写
    return  std::equal(str1.begin(),str1.end(),str2.begin(),comp_wchar_case_insensitive);
}



//查找子字符串（不区分大小写）
#include <locale>
#include <algorithm>


//templated version of my_equal so it could work with both char and wchar_t
template<typename charT>
struct my_equal 
{
    my_equal( const std::locale& loc ):m_loc(loc) {}
    bool operator()(charT ch1, charT ch2) {
        return std::toupper(ch1, m_loc) == std::toupper(ch2, m_loc);
    }
private:
    const std::locale& m_loc;
};

template< typename T> 
size_t find_substr(const T& str1,const T& str2,bool caseSensitive/*=false*/,const std::locale& loc=std::locale())
{
    typename T::const_iterator it;
    if (caseSensitive)
    {
        it=std::search(str1.begin(),str1.end(),str2.begin(),str2.end());
    }
    else
    {
        it=std::search(str1.begin(),str1.end(),str2.begin(),str2.end(),my_equal<typename T:: value_type>(loc));
    }

     if(it!=str1.end())
    {
        return it-str1.begin(); 
    }
    //return -1; // not found =string::npos
    return std::string::npos;
} 

size_t Find(const std::string& str,const std::string& substr,bool caseSensitive/*=false*/)
{
    return find_substr(str,substr,caseSensitive);


    //TODO:
    //return str.find(substr);
}

size_t Find(const std::wstring& str,const std::wstring& substr,bool caseSensitive/*=false*/)
{
    return find_substr(str,substr,caseSensitive);

    //TODO:
    //return str.find(substr);
}

bool Contains(const std::string& str,const std::string& substr,bool caseSensitive/*=false*/)
{
    return Find(str,substr,caseSensitive)!=std::string::npos;
}

bool Contains(const std::wstring& str,const std::wstring& substr,bool caseSensitive/*=false*/)
{
        return Find(str,substr,caseSensitive)!=std::wstring::npos;
}



#if 0   //vs2012ok  gcc4.8.5 error
std::string& ltrim(std::string& s)
{
    if (s.empty()) return s;
    std::string::const_iterator iter = s.begin();
    while (iter != s.end() && isspace(*iter++));
    s.erase(s.begin(), --iter);
    return s;
}

std::string& rtrim(std::string& s)
{
    if (s.empty()) return s;
    std::string::const_iterator iter = s.end();
    while (iter != s.begin() && isspace(*--iter));
    s.erase(++iter, s.end());
    return s;
}

std::string& trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
    return s;
}
#endif

/*
简单解释一下算法：
    isntspace 是自定义的函数，位于匿名名字空间中，只能在本地编译单元中被引用。该函数调用标准库函数 isspace。
    find_if 该算法遍历指定的区域，返回第一个符合条件的迭代器，如果找不到就返回区域结束迭代器。
    find_if(s.begin(), s.end(), isntspace);  该语句遍历 s.begin() 到 s.end()。通过函数 isntspace 检查每个遍历到的字符，如果发现该字符不是空格则返回该迭代器。
    find_if(s.rbegin(), s.rend(), isntspace);  该语句遍历 s.rbegin() 到 s.rend()，这是反向遍历，也就是从字符串尾开始遍历。通过函数 isntspace 检查每个遍历到的字符，如果发现该字符不是空格则返回该迭代器。
    base() 该函数返回反向迭代器对应的正向迭代器。
    string::const_iterator 是随机迭代器，所以可以通过 < 运算符 来比较顺序。
*/

#if 0

namespace 
{
 bool isntspace(const char& ch) 
    {
         return !isspace(ch);
     }
 } // end of namespace

std::string ltrim(const std::string& s)
 {
         std::string::const_iterator iter = find_if(s.begin(), s.end(), isntspace);
         return std::string(iter,s.end());
 }
std::string rtrim(const std::string& s)
{
         std::string::const_iterator iter = find_if(s.rbegin(), s.rend(), isntspace).base();
         return std::string(s.begin(),iter);
 }

std::string trim(const std::string& s)
{
      std::string::const_iterator iter1 = find_if(s.begin(),s.end(),isntspace);
      std::string::const_iterator iter2 = find_if(s.rbegin(),s.rend(),isntspace).base();
      return iter1<iter2?std::string(iter1,iter2):std::string("");
 }
#endif


bool startsWith(const std::string& str, const std::string& prefix,bool caseSensitive)
{
    if (prefix.size()>str.size())
    {
        return false;
    }

    if (caseSensitive)
    {
        return std::equal(prefix.cbegin(), prefix.cend(), str.cbegin());
    }
    else
    {
        return std::equal(prefix.cbegin(), prefix.cend(), str.cbegin(),upperEqual<std::string::value_type>);
    }
}

bool startsWith(const std::wstring& str, const std::wstring& prefix,bool caseSensitive/*=true*/)
{
    if (prefix.size()>str.size())
    {
        return false;
    }

    if (caseSensitive)
    {
        return std::equal(prefix.cbegin(), prefix.cend(), str.cbegin());
    }
    else
    {
        return std::equal(prefix.cbegin(), prefix.cend(), str.cbegin(),comp_wchar_case_insensitive);
    }
}

bool endsWith(const std::string& str, const std::string& suffix,bool caseSensitive)
{
    if (suffix.size()>str.size())
    {
        return false;
    }
    if (caseSensitive)
    {
        return std::equal(suffix.crbegin(), suffix.crend(), str.crbegin());
    }
    else
    {
        return std::equal(suffix.crbegin(), suffix.crend(), str.crbegin(),upperEqual<std::string::value_type>);
    }
}


bool endsWith(const std::wstring& str, const std::wstring& suffix,bool caseSensitive/*=true*/)
{
    if (suffix.size()>str.size())
    {
        return false;
    }
    if (caseSensitive)
    {
        return std::equal(suffix.crbegin(), suffix.crend(), str.crbegin());
    }
    else
    {
        return std::equal(suffix.crbegin(), suffix.crend(), str.crbegin(),comp_wchar_case_insensitive);
    }
}




//实现方式2：仅供参考 by lizulin 20200228
//bool startwith = s.compare(0, head.size(), head) == 0;
//bool endwith = s.compare(s.size() - tail.size(), tail.size(), tail) == 0;

#if 1
//str截取出前len个字符与start比较(len为start字符串的长度)
//strutils.cpp
bool startswith(const std::string& str, const std::string& start,bool caseSensitive=true)
{
    //int srclen = str.size();
    //int startlen = start.size();
    //if(srclen >= startlen)
    //{
    //    std::string temp = str.substr(0, startlen);
    //    if(temp == start)
    //        return true;
    //}
    //return false;


    size_t srclen = str.size();
    size_t startlen = start.size();
    if(startlen<srclen)
    {
        return false;
    }
    const char* pstr=str.c_str();
    const char* pstart=start.c_str();
    if (caseSensitive)
    {
        for (size_t n=0;n<startlen;n++)
        {
            if (pstart[n]!=pstr[n])
            {
                return false;
            }
        }
    }
    else
    {
        for (size_t n=0;n<startlen;n++)
        {
            if (!ComIgnoreCase(pstart[n],pstr[n]))
            {
                return false;
            }
        }
    }
    return true;
}
//str截取出最后len个字符与start比较(len为start字符串的长度)
bool endswith(const std::string& str, const std::string& end,bool caseSensitive=true)
{
    //int srclen = str.size();
    //int endlen = end.size();
    //if(srclen >= endlen)
    //{
    //    std::string temp = str.substr(srclen - endlen, endlen);
    //    if(temp == end)
    //        return true;
    //}
    //return false;

    size_t srclen = str.size();
    size_t endlen = end.size();
    if(endlen<srclen)
    {
        return false;
    }
    const char* pstr=str.c_str()+(srclen-endlen);
    const char* pend=end.c_str();
    if (caseSensitive)
    {
        for (size_t n=0;n<endlen;n++)
        {
            if (pend[n]!=pstr[n])
            {
                return false;
            }
        }
    }
    else
    {
        for (size_t n=0;n<endlen;n++)
        {
            if (!ComIgnoreCase(pend[n],pstr[n]))
            {
                return false;
            }
        }
    }
    return true;
}
#endif


size_t indexOf(const std::string &str, const std::string &substr)
{
    return str.find(substr);
}

std::string toUpper(const std::string &str)
{
    std::string upper(str.size(),'\0');
    std::transform(str.cbegin(),str.cend(),upper.begin(),::toupper);
    return upper;
}

std::string toLower(const std::string &str)
{
    std::string lower(str.size(),'\0');
    std::transform(str.cbegin(),str.cend(),lower.begin(),::tolower);
    return lower;
}

std::string FormatString(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int len = vsnprintf(nullptr,0,fmt,ap);
    va_end(ap);
    std::string sNew(len+1, '\0');
    va_start(ap, fmt);
    vsnprintf(&sNew[0], sNew.size(),fmt,ap);
    va_end(ap);
    sNew.pop_back();  //why? to be tested
    return sNew;
}


//
//C语言 atof()的实现  原文链接：https://blog.csdn.net/wutao1530663/article/details/54381316
//网上搜了几个关于atof()实现的博客,实现的都不是很全面,有的未考虑科学计数法,有的未考虑非法输入,所以打算自己实现atof()。
//函数实现要求：
//将字符串表示的浮点数转化为double类型；
//可以采用科学计数法,如1.34e3 代表 1340.0；
//忽略前置空格；
//能够考虑正负数值；
//能够识别非法输入；(截断至非法字符处,如atof(” 12.2c32”)输出为12.2)。
//版权声明：本文为CSDN博主「wutao02」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。

double my_atofV2(const char *str)
{
    const char *p = str;
    int sign = 1;
    while (*p == ' ')++p;//忽略前置空格
    if (*p == '-')//考虑是否有符号位
    {
        sign = -1;
        ++p;
    }
    else if (*p == '+')
    {
        ++p;
    }
    int hasDot = 0,hasE = 0;
    double integerPart = 0.0,decimalPart = 0.0;
    //遇到'e'或'.'字符则退出循环,设置hasE和hasDot。
    for (; *p; ++p)
    {
        if (isdigit(*p)) //若p指向的字符为数字则计算当前整数部分的值
        {
            integerPart = 10 * integerPart + *p - '0';
        }
        else if (*p == '.')
        {
            hasDot = 1;
            p++;
            break;
        }
        else if (*p == 'e' || *p == 'E')
        {
            hasE = 1;
            p++;
            break;
        }
        else  //如果遇到非法字符,则截取合法字符得到的数值,返回结果。
        {
            return integerPart;
        }
    }

    //上一部分循环中断有三种情况,一是遍历完成,这种情况下一部分的循环会自动跳过；其次便是是遇到'.'或'e',两种hasE和hasDot只可能一个为真,若hasDot为真则计算小数部分,若hasE为真则计算指数部分。
    int decimalDigits = 1;
    int exponential = 0;    
    for (; *p; p++)
    {
        if (hasDot && isdigit(*p))
        {
            decimalPart += (*p - '0') / ::pow(10, decimalDigits++);
        }
        else if (hasDot && (*p == 'e' || *p == 'E')) 
        {
            integerPart += decimalPart;
            decimalPart = 0.0;
            hasE = 1;
            ++p;
            break;
        }
        else if (hasE && isdigit(*p))
        {
            exponential = 10 * exponential + *p - '0';
        }
        else break;
    }
    //上一部分较难理解的就是else if (hasDot && (*p == 'e' || *p == 'E')) 这一特殊情况,对于合法的浮点数,出现'.'字符后,仍然有可能是科学计数法表示,但是出现'e'之后,指数部分不能为小数(这符合<string.h>对atof()的定义)。这种情况变量IntegerPart和decimalPart都是科学计数法的基数,因此有integerPart += decimalPart(这使得IntergerPart的命名可能欠妥,BasePart可能是一种好的选择)。
    //上一部分循环结束一般情况下就能返回结果了,除非遇到前文所述的特殊情况，对于特殊情况需要继续计算指数。
    if (hasE && hasDot)
    {
        for (; *p; p++)
        {
            if (isdigit(*p))
            {
                exponential = 10 * exponential + *p - '0';
            }
        }
    }
    return sign * (integerPart * pow(10, exponential) + decimalPart);
}




//https://blog.csdn.net/weixin_34101784/article/details/85898267  完善

//字符串解析数值,失败返回defVal默认值,pEnd,返回搜索到的字符串结尾位置
//isOK存放解析过程是否出错，成功为true
double stringToDouble(const char *ps,double defVal,bool* isOK,const char** pEnd)
{
    if (isNullCstr(ps))
    {
        if (isOK)
        {
            *isOK=false;
        }
        if(pEnd)
        {
            *pEnd=NULL;
        }
        return defVal;   //首次失败返回默认值
    }


    double res=0.0;       //结果
    double dec=10.0;    //小数
    int num=0;              
    bool signFlag=false;
    bool bFind=false;     //是否找到过数字,add by lizulin

    while(*ps==' ')
    {
        ps++;
    }
    if(*ps=='-')//记录数字正负
    {
        signFlag=true;
        ps++;
    }else if (*ps=='+')   //跳过正号
    {
        ps++;
    }


    if(!(*ps>='0'&&*ps<='9'))//如果一开始非数字则退出，返回0.0
    {
        //return res;

        if (isOK)
        {
            *isOK=false;
        }
        if(pEnd)
        {
            *pEnd=NULL;
        }

        return defVal;   //首次失败返回默认值
    }
    while(*ps>='0'&&*ps<='9'&&*ps!='.')//计算小数点前整数部分
    {
        res=res*10.0+*ps-'0';
        ps++;

        bFind=true;
    }

    //此前找不到整数部分,默认当0,完善待续

    if(*ps=='.')//以后为小树部分
    {
        ps++;
    }
    while(*ps>='0'&&*ps<='9')//计算小数部分
    {
        res=res+(*ps-'0')/dec;
        dec*=10.0;
        ps++;

        bFind=true;
    }

    if(*ps=='e'||*ps=='E')//考虑科学计数法
    {
        ps++;
        if(*ps=='+')
        {
            ps++;
            while(*ps>='0'&&*ps<='9')
            {
                num=num*10+*ps-'0';
                ps++;
            }
            while(num>0)
            {
                res*=10;
                num--;
            }
        }
        if(*ps=='-')
        {
            ps++;
            while(*ps>='0'&&*ps<='9')
            {
                num=num*10+*ps-'0';
                ps++;
            }
            while(num>0)
            {
                res/=10;
                num--;
            }
        }
    }

    if (bFind)   //是否找到过数值
    {
        if (isOK)
        {
            *isOK=true;
        }
        if(pEnd)
        {
            *pEnd=const_cast<char*>(ps);
        }
        return res*(signFlag?-1.0:1.0);
        //return (signFlag)?(-s):(s);
    }
    else
    {
        if (isOK)
        {
            *isOK=false;
        }
        if(pEnd)
        {
            *pEnd=NULL;
        }
        return defVal;
    }
}



//字符串解析数值,失败返回defVal默认值,pEnd,返回搜索到的字符串结尾位置
//isOK存放解析过程是否出错，成功为true
int64_t stringToInt64(const char *ps,int64_t defVal,bool* isOK,const char** pEnd)
{
    if (isNullCstr(ps))
    {
        if (isOK)
        {
            *isOK=false;
        }
        if(pEnd)
        {
            *pEnd=NULL;
        }
        return defVal;   //首次失败返回默认值
    }

    int64_t res=0;
    bool signFlag=false;
    bool bFind=false;     //是否找到过数字,add by lizulin

    while(*ps==' ')
    {
        ps++;
    }

    //if(*ps=='-'||*ps=='+')
    //{
    //    if(*ps=='-')
    //    {
    //        signFlag=true;
    //    }
    //    ps++;
    //}

    if(*ps=='-')//记录数字正负
    {
        signFlag=true;
        ps++;
    }else if (*ps=='+')   //跳过正号
    {
        ps++;
    }

    while(*ps>='0'&&*ps<='9')
    {
        res=res*10+*ps-'0';
        ps++;
        bFind=true;

        if(res<0)
        {
            //res=2147483647;
            res=UINT64_MAX;
            break;
        }
    }
    // return res*(signFlag?-1:1);

    if (bFind)   //是否找到过数值
    {
        if (isOK)
        {
            *isOK=true;
        }
        if(pEnd)
        {
            *pEnd=const_cast<char*>(ps);
        }
        return res*(signFlag?-1.0:1.0);
        //return (signFlag)?(-s):(s);
    }
    else
    {
        if (isOK)
        {
            *isOK=false;
        }
        if(pEnd)
        {
            *pEnd=NULL;
        }
        return defVal;
    }
}



//在一个字符串中查找多个数字并返回 
std::vector<int> string_FindIntNum(const std::string& str)
{
    std::vector<int> ListRes;

    const char *pSrc=str.c_str();
    const char* pB;     //每一段数字的开始/结束指针
    const char* pE;
    bool find=false;
    for (size_t k=0;k<str.size();k++)
    {
        char curr=pSrc[k];
        const char* pCurr=&pSrc[k];
        if (isDigital(curr))
        {
            if (!find)       //若此前未搜索到数字
            {
                pB=pCurr;
                pE=pCurr+1;
                find=true;
            }
            pE=pCurr+1;    //更新结束指针
        }
        else
        {
            if (find)   //若曾经找到过数字
            {
                char tmp[64]={0};
                size_t strNumLen=pE-pB;
                if (strNumLen==0)
                {
                    continue;
                }

                memcpy(tmp,pB,strNumLen);
                int resNum=0;
                sscanf(tmp,"%d",&resNum);
                ListRes.push_back(resNum);
                find=false;
            }
            else
            {
                //若一直未找到,开始结束指针设为相同
                pB=pCurr;
                pE=pCurr;
            }
        }
    }

    // printf("string_findDigital() src:%s\n",str.c_str());
    // for (size_t n=0;n<ListRes.size();n++)
    // {
    //     printf("res[%d] %d",n,ListRes.at(n));
    // }
    //printf("\n");

    return ListRes;
}



//在一个字符串中查找多个数字并返回, 对于小数点,需要考虑在首位,或者有多个小数点的情况,完善待续,
//中文字符串暂时不考虑 + -号的情况考虑 指数形式的考虑 有空待续
std::vector<float> string_FindFloatNum(const std::string& str)
{
    const std::string strTmp=str+"   ";   //为了便于判断结尾加空格结束符号
    std::vector<float> ListRes;

    const char *pSrc=strTmp.c_str();
    const char* pB;     //每一段数字的开始/结束指针
    const char* pE;
    bool find=false;
    for (size_t k=0;k<strTmp.size();k++)
    {
        char curr=pSrc[k];
        const char* pCurr=&pSrc[k];
        if (isDigital(curr)||curr=='.')
        {
            if (!find)       //若此前未搜索到数字
            {
                pB=pCurr;
                find=true;
            }
            pE=pCurr+1;    //更新结束指针
        }
        else
        {
            if (find)   //若曾经找到过数字
            {
                char tmp[64]={0};
                size_t strNumLen=pE-pB;
                if (strNumLen==0)
                {
                    continue;
                }

                memcpy(tmp,pB,strNumLen);
                float resNum=0;
                sscanf(tmp,"%f",&resNum);
                ListRes.push_back(resNum);
                find=false;
            }
            else
            {
                //若一直未找到,开始结束指针设为相同
                pB=pCurr;
                pE=pCurr;
            }
        }
    }

    //printf("string_findDigital() src:%s\n",str.c_str());
    //for (size_t n=0;n<ListRes.size();n++)
    //{
    //    printf("res[%d] %f",n,ListRes.at(n));
    //}
    //printf("\n");

    return ListRes;
}


#if 0
BASIC_API double string_FindLabelValueUnitF(const char* pSrc,const char* labelName,char* pUnitRes/*=NULL*/,double defVal/*=0*/,bool* isOK/*=NULL*/)
{
    if (isNullCstr(pSrc))
    {
        if (pUnitRes)
        {
             *pUnitRes='\0';
        }
        if (isOK)
        {
            *isOK=false;
        }
        return defVal;
    }

    const char* pBV=pSrc;                     //标签值的开始位置
    if (!isNullCstr(labelName))      //若标签有效
    {
        size_t labelLen=strlen(labelName);
        const char* pBLabel=strstr(pSrc,labelName);
        if (pBLabel!=NULL)  
        {
            pBV=pBLabel+labelLen;      //标签结束位置
        }
    }

    const char* pEnd;  
    double res=stringToDouble(pBV,defVal,isOK,&pEnd);

    //解析完值的后后续字符串结前获取单位字符串(仅仅支持英文单位)TODO: 测试完善待续
    if (pUnitRes)
    {
        //*pUnitRes='\0';
        if(pEnd!=NULL)
        {
            //单位字符串的开始与结束位置
            const char* posUnitBegin=pEnd;
            const char* posUnitEnd=pEnd;
            const char* pC=pEnd;
            bool bFindUnit=false;
            char tmp=*pC;
            while (tmp)
            {
                tmp=*(pC);
                bool b=isalpha(tmp);
                if (!bFindUnit)
                {
                    posUnitBegin=pC;   //单位字符串开始位置
                    posUnitEnd=pC;
                    bFindUnit=true;
                }

                //如果已经找到过单位,且当前字符非英文，说明单位结束
                if (bFindUnit)
                {
                    if(!b)
                    {
                        posUnitEnd=pC;
                        break;
                    }
                }
                pC++;
            }
        }

        //拷贝解析得到的单位到结果缓存中
        size_t unitLen=posUnitEnd-posUnitBegin;
        memcpy(pUnitRes,posUnitBegin,unitLen);
        pUnitRes[unitLen]='\0';
    }

    return res;
}
#endif


//字符串里寻找Label 标签里的数置,找到则填充返回值和结束位置 若label为NULL或"" ,直接查找值 不管标签


//字符串里面搜索某个标签的数值,失败返回defVal默认值,pEnd,返回搜索到的字符串结尾位置
//isOK存放解析过程是否出错，成功为true
double string_FindLabelValueF(const char* pSrc,const char* labelName,double defVal,bool* isOK,const char** pEnd)
{
    if (isNullCstr(pSrc))
    {
        if (pEnd)
        {
            *pEnd=nullptr;
        }
        if (isOK)
        {
            *isOK=false;
        }
        return defVal;
    }

    const char* pBV=pSrc;                     //标签值的开始位置
    if (!isNullCstr(labelName))      //若标签有效
    {
        size_t labelLen=strlen(labelName);
        const char* pBLabel=strstr(pSrc,labelName);
        if (pBLabel!=NULL)  
        {
            pBV=pBLabel+labelLen;      //标签结束位置
        }
    }
    return stringToDouble(pBV,defVal,isOK,pEnd);
}

double string_FindLabelValueF(const std::string& Src,const char* labelName,double defVal,bool* isOK,const char** pEnd)
{
    if (Src.empty())
    {
        string_FindLabelValueF(NULL,labelName,defVal,isOK,pEnd);
    }
    return string_FindLabelValueF(Src.c_str(),labelName,defVal,isOK,pEnd);
}

int64_t string_FindLabelValue(const char* pSrc,const char* labelName,int64_t defVal,bool* isOK,const char** pEnd)
{
    if (isNullCstr(pSrc))
    {
        if (pEnd)
        {
            *pEnd=nullptr;
        }
        if (isOK)
        {
            *isOK=false;
        }
        return defVal;
    }
    const char* pBV=pSrc;           //标签值的开始位置
    if (!isNullCstr(labelName))      //若标签有效
    {
        size_t labelLen=strlen(labelName);
        const char* pBLabel=strstr(pSrc,labelName);
        if (pBLabel!=NULL)  
        {
            pBV=pBLabel+labelLen;      //标签结束位置
        }
    }
    return stringToInt64(pBV,defVal,isOK,pEnd);
}

int64_t string_FindLabelValue(const std::string& Src,const char* labelName,int64_t defVal,bool* isOK,const char** pEnd)
{
    if (Src.empty())
    {
        string_FindLabelValue(NULL,labelName,defVal,isOK,pEnd);
    }
    return string_FindLabelValue(Src.c_str(),labelName,defVal,isOK,pEnd);
}



//获取某个标签下的值,可以返回多个,例如在字符串NRP:100.5 NRP:200.1 NRP:90.99 里提取100.5 200.1 90.99
std::vector<double> string_FindAllLabelValueF(const std::string& str,const char* labelName)
{
    std::vector<double> ListRes;
    if (str.empty())
    {
        return ListRes;
    }
    const char *pSrc=str.c_str();
    const char* pCurr=const_cast<char*>(pSrc);    
    while (1)
    {
        bool isOk=false;
        double resVal=string_FindLabelValueF(pCurr,labelName,0,&isOk,&pCurr);    //根据返回指针不断寻找
        if (isOk)
        {
            ListRes.push_back(resVal);
        }
        else
        {
            break;
        }
    }
    return ListRes;
}

//获取某个标签下的值,可以返回多个,例如在字符串NRP:100 NRP:200 NRP:90 里提取100 200 90
std::vector<int64_t> string_FindAllLabelValue(const std::string& str,const char* labelName)
{
    std::vector<int64_t> ListRes;
    if (str.empty())
    {
        return ListRes;
    }

    const char *pSrc=str.c_str();
    const char* pCurr=const_cast<char*>(pSrc);    
    while (1)
    {
        bool isOk=false;
        int64_t resVal=string_FindLabelValue(pCurr,labelName,0,&isOk,&pCurr);    //根据返回指针不断寻找
        if (isOk)
        {
            ListRes.push_back(resVal);
        }
        else
        {
            break;
        }
    }
    return ListRes;
}




//二、用STL进行字符串的分割

//涉及到string类的两个函数find和substr：
// 1、find函数
// 原型：size_t find ( const string& str, size_t pos = 0 ) const;
// 功能：查找子字符串第一次出现的位置。
// 参数说明：str为子字符串，pos为初始查找位置。
// 返回值：找到的话返回第一次出现的位置，否则返回string::npos

//2、substr函数
// 原型：string substr ( size_t pos = 0, size_t n = npos ) const;
// 功能：获得子字符串。
// 参数说明：pos为起始位置（默认为0），n为结束位置（默认为npos）
// 返回值：子字符串







#if defined(_WIN32)
//#pragma region 用来提升系统权限  
//这是一个通用的提升权限函数，如果需要提升其他权限  
//更改LookupPrivilegeValue的第二个参数SE_SHUTDOWN_NAME，即可  
BOOL EnableShutDownPriv()  
{  
    HANDLE hToken=NULL;  
    TOKEN_PRIVILEGES tkp={0};  
    //打开当前程序的权限令牌  
    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))  
    {  
        return FALSE;  
    }  
    //获得某一特定权限的权限标识LUID，保存在tkp中  
    if (!LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid))  
    {  
        CloseHandle(hToken);  
        return FALSE;  
    }  
    tkp.PrivilegeCount=1;  
    tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;  
    //调用AdjustTokenPrivileges来提升我们需要的系统权限  
    if(!AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(TOKEN_PRIVILEGES),NULL,NULL))  
    {  
        CloseHandle(hToken);  
        return FALSE;  
    }  
    return TRUE;  
}  

BOOL ReSetWindows(DWORD dwFlags,bool bForce)  
{  
    //检查参数是否正确  
    if(dwFlags!=EWX_LOGOFF&&dwFlags!=EWX_REBOOT&&dwFlags!=EWX_SHUTDOWN)  
        return FALSE;  

    //获得系统的版本信息，让我们后面确定是否需要提升系统权限  
    OSVERSIONINFO osvi={0};  
    //获得参数的大小，以提供给GetVersionEx来判断这是一个新版本的OSVERSIONINFO，还是旧版本的  
    //新版本是OSVERSIONINFOEX。扩充版本  
    osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);  
    if(!GetVersionEx(&osvi))  
    {  
        return FALSE;  
    }  
    //检查操作系统的版本，如果是NT类型的系统，需要提升系统权限  
    if(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)  
    {  
        EnableShutDownPriv();  
    }  
    //判断是否是强制关机，强制关闭系统进程。  
    //dwFlags|=(bForce!=FALSE)?EWX_FORCE:EWX_FORCEIFHUNG;  

    dwFlags|=(bForce)?EWX_FORCE:EWX_FORCEIFHUNG;  

    //调用API  
    return ExitWindowsEx(dwFlags,0);  
}  
#endif


#if 0
Linux中关机，重启，注销命令
    关机：
    shutdown -h now  //立刻关机重启，工作中常用
    shutdown -h +1    //1分钟后关机
    init 0
    halt                        //立即停止系统，需要人工关闭电源
    halt -p                    //
    poweroff                //立即停止系统，并且关闭电源
    重启：
    reboot                      //工作中常用
    shutdown -r now      //工作中常用
    shutdown -r +1         //一分钟后重启
    init 6
    注销：
    logout
    exit                //工作中常用
    ctrl+d             //工作中常用
#endif


void SystemRestart(bool bForce)   //跨平台重启系统
{  
#ifdef OS_WIN  
    ReSetWindows(EWX_REBOOT,bForce);//重启     
#else  
    if (bForce)
    {
          system("reboot -nf");  
    }
    else
    {
          system("reboot");  
    }
#endif  
}  

void SystemShutdown(bool bForce)     //跨平台关闭系统
{  
#ifdef OS_WIN 
    ReSetWindows(EWX_SHUTDOWN,bForce);//重启    
#else  
    system("halt");  
#endif  
}  

void SystemLogout()
{
#ifdef OS_WIN 
    ReSetWindows(EWX_LOGOFF,false);   //注销
#else  
    system("logout");  
#endif  
}




unsigned int getCpuCount()  
{  
#ifdef _WIN32  
    SYSTEM_INFO sysInfo;  
    GetSystemInfo(&sysInfo);  
    return sysInfo.dwNumberOfProcessors;  
#else  
//    long cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
//    if (cpu_num == -1)
//    {
//        assert(false);
//        return 0;
//    }
//    // 看两者是否相等
//    assert(cpu_num == sysconf(_SC_NPROCESSORS_CONF));
//    return cpu_num;
#endif  
}  

#ifdef _WIN32
#include <psapi.h>
#pragma comment(lib,"psapi.lib")

#else

#endif




////2G以上内存建议使用GlobalMemoryStatusEx函数代替（2~4G需要链接器/LARGEADDRESSAWARE选项，4G以上不支持）
//MEMORYSTATUSEX适合内存大于4G
//    MEMORYSTATUSEX较MEMORYSTATUS多一个ullAvailExtendedVirtual成员
//
//    如果仅仅是求物理内存使用率,两种结构体任意一种都可以,结果是一样的.
//
//    结构成员的含义：
//    dwLength 　　MEMORYSTATUS结构的大小，在调GlobalMemoryStatus函数前用sizeof()函数求得，用来供函数检测结构的版本。 　　
//    dwMemoryLoad 　 返回一个介于0～100之间的值，用来指示当前系统内存的使用率。 　　
//    dwTotalPhys 　　返回总的物理内存大小，以字节(byte)为单位。 　　
//    dwAvailPhys 　　返回可用的物理内存大小，以字节(byte)为单位。 　　
//    dwTotalPageFile  显示可以存在页面文件中的字节数。注意这个数值并不表示在页面文件在磁盘上的真实物理大小。 　　
//    dwAvailPageFile  返回可用的页面文件大小，以字节(byte)为单位。 　　
//    dwTotalVirtual 　返回调用进程的用户模式部分的全部可用虚拟地址空间，以字节(byte)为单位。 　　
//    dwAvailVirtual 　返回调用进程的用户模式部分的实际自由可用的虚拟地址空间，以字节(byte)为单位。



bool getSysMemoryInfo(SystemMemoryInfo& memInfo)
{
    memInfo.Reset();

#ifdef _WIN32
    const uint64_t MBbytes=1024*1024;
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    if (GlobalMemoryStatusEx(&statusex))
    {
        memInfo.MemoryLoad=statusex.dwMemoryLoad;
        memInfo.TotalPhys=statusex.ullTotalPhys;
        memInfo.AvailPhys=statusex.ullAvailPhys;
        memInfo.TotalVirtual=statusex.ullTotalVirtual;
        memInfo.AvailVirtual=statusex.ullAvailVirtual;
        memInfo.usedPercentPhys=1.0-(double)memInfo.AvailPhys/memInfo.TotalPhys;

        //::printf("SysMem:PhysTotal:%llu Avail:%llu MemoryLoad:%d\n"
        //    ,statusex.ullTotalPhys,statusex.ullAvailPhys,statusex.dwMemoryLoad);

        //::printf("SysMem:VirtualTotal:%llu Avail:%llu\n"
        //    ,statusex.ullTotalVirtual,statusex.ullAvailVirtual);
        return true;
    }

#else

#endif
    return false;
}



void showSystemMemoryInfo()
{
    const uint64_t MBbytes=1024*1024;
    SystemMemoryInfo SmInfo;
    bool ret=getSysMemoryInfo(SmInfo);
    if(ret)
    {
       printf("SysMem:Total:%llu MB,Avail:%llu MB usedPercent:%f\n"
           ,SmInfo.TotalPhys/MBbytes,
           SmInfo.AvailPhys/MBbytes,
           SmInfo.usedPercentPhys);
    }
    else
    {
        PreDprintf("Failed\n");
    }
}




void showCurrProcessMemoryInfo(void)
{
#ifdef _WIN32
    HANDLE handle=GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));

    const uint64_t MBbytes=1024*1024;
    uint64_t MemUsedCurr=pmc.WorkingSetSize/MBbytes;
    uint64_t MemUsedPeak=pmc.PeakWorkingSetSize/MBbytes;
    uint64_t PageMemUsedCurr=pmc.PagefileUsage/MBbytes;
    uint64_t PageMemUsedPeak=pmc.PeakPagefileUsage/MBbytes;

    ::printf("MemUsed:curr%llu MB peak:%llu MB PageMemUsed:%llu MB peak:%llu MB\n"
        ,MemUsedCurr,MemUsedPeak,PageMemUsedCurr,PageMemUsedPeak);

    //std::cout<<"内存使用:"<<pmc.WorkingSetSize/1000 <<"K/"<<pmc.PeakWorkingSetSize/1000<<"K + "<<pmc.PagefileUsage/1000 <<"K/"<<pmc.PeakPagefileUsage/1000 <<"K"<<std::endl;
#else

#endif
}


#ifdef _WIN32

__int64 CompareFileTime(FILETIME time1, FILETIME time2)
{
    __int64 a = (((int64_t)time1.dwHighDateTime)<<32)| time1.dwLowDateTime;
    __int64 b = (((int64_t)time2.dwHighDateTime)<<32)| time2.dwLowDateTime;

    return (b - a);
}

inline double FileTimeToDouble(FILETIME &filetime)
{
    return (double)(filetime.dwHighDateTime * 4.294967296E9) + (double)filetime.dwLowDateTime;
}
uint64_t file_time_2_utc(const FILETIME* ftime)
{
    LARGE_INTEGER li;
    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}

double getCurretProcessUsage()
{

#ifdef _WIN32
    //HANDLE hEvent;
    //BOOL res;
   
    //FILETIME preidleTime;
    //FILETIME prekernelTime;
    //FILETIME preuserTime;

    //FILETIME idleTime;
    //FILETIME kernelTime;
    //FILETIME userTime;

    //res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
    //preidleTime = idleTime;
    //prekernelTime = kernelTime;
    //preuserTime = userTime;
    //res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
    //__int64 idle = CompareFileTime(preidleTime, idleTime);
    //__int64 kernel = CompareFileTime(prekernelTime, kernelTime);
    //__int64 user = CompareFileTime(preuserTime, userTime);

    ////__int64 cpu = (kernel + user - idle) * 100 / (kernel + user);
    ////__int64 cpuidle = (idle)* 100 / (kernel + user);
    ////cout << "CPU利用率:" << cpu << "%" << " CPU空闲率:" << cpuidle << "%" << endl;

    //double cpu = (kernel + user - idle)/ (kernel + user);
    //double cpuidle = (idle)/ (kernel + user);

    //preidleTime = idleTime;
    //prekernelTime = kernelTime;
    //preuserTime = userTime;
    //return cpu;


    FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;

    BOOL res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
    double Idle = FileTimeToDouble(idleTime);
    double kernel = FileTimeToDouble(kernelTime);
    double user = FileTimeToDouble(userTime);

    double cpu= (kernel + user - Idle)/ (kernel + user);
    return cpu;
#else

#endif
}

class CPUInfoPrivate
{
public:
         HANDLE _hProcess;  
         int _processor;    //cpu数量  
         int64_t _last_time;         //上一次的时间  
         int64_t _last_system_time;  
};


CPUInfo::CPUInfo()
{
   pimpl=new CPUInfoPrivate;

    Initialize();

    init(); 
    pimpl->_processor = get_processor_number();

    //add 20190103
    int iPid = (int)getpid();   //获取当前进程ID，跨平台方法
    setProcessID(iPid);
}


CPUInfo::CPUInfo(int ProcessID)
{
    init(); 
    pimpl->_processor = get_processor_number();
    setProcessID(ProcessID);
}

CPUInfo::~CPUInfo()
{
        clear(); 
        delete pimpl;
}

bool CPUInfo::Initialize()
{
    FILETIME ftIdle, ftKernel, ftUser;
    BOOL flag = FALSE;
    if (flag = GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
    {
        m_fOldCPUIdleTime = FileTimeToDouble(ftIdle);
        m_fOldCPUKernelTime = FileTimeToDouble(ftKernel);
        m_fOldCPUUserTime = FileTimeToDouble(ftUser);
    }
    return flag==TRUE;
}

int CPUInfo::GetCPU_UseRate()
{
    int nCPUUseRate = -1;
    FILETIME ftIdle, ftKernel, ftUser;
    if (GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
    {
        double fCPUIdleTime = FileTimeToDouble(ftIdle);
        double fCPUKernelTime = FileTimeToDouble(ftKernel);
        double fCPUUserTime = FileTimeToDouble(ftUser);
        nCPUUseRate= (int)(100.0 - (fCPUIdleTime - m_fOldCPUIdleTime) 
            / (fCPUKernelTime - m_fOldCPUKernelTime + fCPUUserTime - m_fOldCPUUserTime) 
            *100.0);
        m_fOldCPUIdleTime = fCPUIdleTime;
        m_fOldCPUKernelTime = fCPUKernelTime;
        m_fOldCPUUserTime = fCPUUserTime;
    }
    return nCPUUseRate;
}


void CPUInfo::init()
{
    pimpl->_last_system_time = 0;
    pimpl->_last_time = 0;
    pimpl->_hProcess = 0;
}

void CPUInfo::clear()
{
    if (pimpl->_hProcess) 
    {
        CloseHandle(pimpl->_hProcess);
        pimpl->_hProcess = 0;
    }
}

void* CPUInfo::setProcessID(int ProcessID)
{
    printf("CPUInfo::ProcessID() %d\n",ProcessID);
    clear();    //如果之前监视过另一个进程，就先关闭它的句柄
    init(); 
    return pimpl->_hProcess= OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, ProcessID);
}


float CPUInfo::getProcessCpuUsage()
{

    FILETIME now;
    FILETIME creation_time;
    FILETIME exit_time;
    FILETIME kernel_time;
    FILETIME user_time;
    int64_t system_time;
    int64_t cTime;
    int64_t system_time_delta;
    int64_t time_delta;

    DWORD exitcode;

    float cpu = -1;

    if (!pimpl->_hProcess) return -1;

    GetSystemTimeAsFileTime(&now);

    //判断进程是否已经退出
    GetExitCodeProcess(pimpl->_hProcess, &exitcode);  
    if (exitcode != STILL_ACTIVE) {
        clear();
        return -1;
    }

    //计算占用CPU的百分比
    if (!GetProcessTimes(pimpl->_hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
    {
        clear();
        return -1;
    }
    system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time))
        / pimpl->_processor;
    cTime = file_time_2_utc(&now);

    //判断是否为首次计算
    if ((pimpl->_last_system_time == 0) || (pimpl->_last_time == 0))
    {
        pimpl->_last_system_time = system_time;
        pimpl->_last_time = cTime;
        return -2;
    }

    system_time_delta = system_time - pimpl->_last_system_time;
    time_delta = cTime - pimpl->_last_time;

    if (time_delta == 0) {
        return -1;
    }

    cpu = (float)system_time_delta * 100 / (float)time_delta;
    pimpl->_last_system_time =system_time;
    pimpl->_last_time = cTime;
    return cpu;
}



int CPUInfo::get_processor_number()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwNumberOfProcessors;
}
 
#endif





//获取当前程序工作目录 可能变化？
std::string getCurrentWorkingPath()
{
#ifdef _WIN32  
    //TCHAR workingPath[MAX_PATH];  
    //ZeroMemory(workingPath, MAX_PATH);  

    //char path[3000];  
    //memset(path, 0, 3000);  
    //if (GetModuleFileName(NULL, workingPath, MAX_PATH) > 0)  
    //{  
    //    unsigned int index;  
    //    for (unsigned int i = strlen(workingPath); i >= 0 ; --i)  
    //    {  
    //        if ((workingPath[i] == '//') || (workingPath[i] == ':'))  
    //        {  
    //            index = i;  
    //            break;  
    //        }  
    //    }  
    //    strncpy(path, workingPath, index + 1);     //wchar 与char 互转需要考虑，待续
    //}  
#else  

#endif  

    char buftmp[1000];
    memset(buftmp,0,sizeof(buftmp));
    if (getcwd(buftmp,sizeof(buftmp))==NULL)
    {
        return "";
    }
    return std::string(buftmp);
}

bool setCurrentWorkingPath(std::string& sNew)
{
#ifdef _WIN32
    Tstring sNewPath=FromTCstr(sNew.c_str());
    return ::SetCurrentDirectory(sNewPath.c_str())==TRUE;
#else
    ////int r;
    ////r = QT_CHDIR(path.nativeFilePath().constData());  //qt
    ////return r >= 0;
    //return chdir(getFullPathName().toUTF8()) == 0;  //juce

    return chdir(sNew.c_str())==0;
#endif
}

std::string getCurrentAppPath()
{
#ifdef _WIN32
    std::string sret;
    TCHAR path[MAX_PATH] = {0};
    if (GetModuleFileName(NULL,path,MAX_PATH) != 0)  
    {
        sret=::FromTCstr(path);
    }
    return sret;

#else  
    char path[256] = {0};  
    char filepath[256] = {0};  
    char cmd[256] = {0};  
    FILE* fp = NULL;  

    // 设置进程所在proc路径  
    sprintf(filepath, "/proc/%d", getpid());  
    // 将当前路径设为进程路径  
    if(chdir(filepath) != -1)  
    {  
        //指定待执行的shell 命令  
        snprintf(cmd, 256, "ls -l | grep exe | awk '{print $10}'");  
        if((fp = popen(cmd,"r")) == NULL)  
        {  
            return std::string();  
        }  
        //读取shell命令执行结果到字符串path中  
        if (fgets(path, sizeof(path)/sizeof(path[0]), fp) == NULL)  
        {  
            pclose(fp);  
            return std::string();  
        }  

        //popen开启的fd必须要pclose关闭  
        pclose(fp);  
        return std::string(path);  
    }  
    return std::string();
#endif
}


bool isAbsolutePath(const char *Filename)
{
#ifdef _WIN32
    if (isNullCstr(Filename))return false;
    if(*Filename == '\\') return true;
    if(*Filename == '/') return true;
    if(*Filename != '\0' && Filename[1] == ':') return true;
    return false;
#else
    if(*Filename == '/') return true;
    return false;
#endif
}


static bool isSlash(char c)
{
    return (c=='/'||c=='\\');
}

//各种奇怪格式文件路径整理成合法格式,反斜杠统一整理成斜杠
// C:\dir/test//\sub2\\sub3/\sub4  等
//const char* src1="C:\\dir/test//\\sub2\\sub3/\\sub4////SUB5\\SUB6";

void FormatPath(char* dst,const char* src)
{
    if (isNullCstr(src))
    {
        return;
    }
    unsigned int outIndex=0;
    bool isSlashLast=false;   //上一个是否斜线
    while (1)
    {
        char tmp=*src++;
        if (tmp=='\0')
        {
            break;
        }
        bool bSlash=isSlash(tmp);
        if (tmp=='\\')  //反斜杠统一成正斜杠
        {
            tmp='/';
        }
        bool conSlash=bSlash&&isSlashLast;   //连续斜线
        if (!conSlash)
        {
            dst[outIndex++]=tmp;
        }
        isSlashLast=bSlash;
    }
    dst[outIndex]='\0';
}

std::string FormatPath(const std::string& spath)
{
    //old slow
    //std::string sRes=Replace(spath,"\\","/");     //反斜杠替换为正斜杠
    //sRes=Replace(spath,"////","/");                   //重复斜杠
    //sRes=Replace(spath,"//","/");                      //重复斜杠
    //return sRes;

    //fast
    char tmp[2000];
    FormatPath(tmp,spath.c_str());
    return std::string(tmp);
}



void getParentPath(char* parent,const char* path)
{
    //assert(parent!=NULL);
    //assert(path!=NULL);
    if (isNullCstr(path)||parent==NULL)
    {
        return;
    }
    std::string sParent=getParentPath(path);
    strcpy(parent,sParent.c_str());
}

std::string getParentPath(const std::string& path)
{
    std::string sSrc=FormatPath(path);    //为了便于编程实现统一整理格式
    sSrc=delDirTailSeparatorIfHas(sSrc);   //若结尾有目录分割符号则去掉
    size_t npos=sSrc.find_last_of("/");
    if (npos!=std::string::npos)
    {
        return sSrc.substr(0,npos);
    }
    else
    {
        return sSrc;
    }
}


//https://blog.csdn.net/gaiazhang/article/details/65628940  windows的磁盘操作之九——区分本地磁盘与移动硬盘
//refactor by lizulin 20181205 通过驱动器名称判断是否是U盘

//返回值true/false 表明获取是否成功，获取结果存在pBusType里
#ifdef _WIN32
//#include <windows.h>
//bool GetDriveTypeByBus(const CHAR*drive,int& BusType)
bool GetDriveTypeByBus(const char*drive,int& BusType)
{
    //printf("GetDriveTypeByBus() drive:%s\n",drive);
    BusType=BusTypeUnknown;  //add 

    HANDLE hDevice;               // handle to the drive to be examined
    BOOL result;                         // results flag
    DWORD nBytesRead;                   // discard results

    STORAGE_DESCRIPTOR_HEADER devDescHeader;
    //STORAGE_DEVICE_DESCRIPTOR devDesc;
    STORAGE_DEVICE_DESCRIPTOR* pDevDesc;
    DWORD devDescLength;
    STORAGE_PROPERTY_QUERY query;

    Tstring tsDrive;
    tsDrive=FromCstr(drive);

    hDevice=CreateFile(
        tsDrive.c_str(),                                       // drive to open
        GENERIC_READ,                                     // access to the drive     GENERIC_READ| GENERIC_WRITE
        FILE_SHARE_READ | FILE_SHARE_WRITE, //share mode
        NULL,                                                    // default security attributes
        //0,
        OPEN_EXISTING,                                    // disposition
        0,                                                           // file attributes
        NULL                                                     // do not copy file attribute
        );
    if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
    {
        PreDprintf("CreateFile() Error: %d driver:%s\n",GetLastError(),drive);
        return false;
    }

    memset(&query,0,sizeof(query));   //add
    query.PropertyId = StorageDeviceProperty;
    query.QueryType = PropertyStandardQuery;

    result = DeviceIoControl(
        hDevice,                                                // device to be queried
        IOCTL_STORAGE_QUERY_PROPERTY,     // operation to perform
        &query,
        sizeof(query),                                               // no input buffer
        &devDescHeader,
        sizeof(devDescHeader),     // output buffer
        &nBytesRead,                 // # bytes returned
        NULL);                           // synchronous I/O
    if (!result)                           //fail
    {
        fprintf(stderr,"GetDriveTypeByBus() failed! drive:%s\n",drive);
        fprintf(stderr, "IOCTL_STORAGE_QUERY_PROPERTY Error: %d  nBytesRead:%d\n", GetLastError(),nBytesRead);
        (void)CloseHandle(hDevice);
        return false;
    }

    devDescLength = devDescHeader.Size;
    pDevDesc=(STORAGE_DEVICE_DESCRIPTOR*)::malloc(sizeof(STORAGE_DEVICE_DESCRIPTOR)+devDescLength);    //

    result = DeviceIoControl(
        hDevice,                                           // device to be queried
        IOCTL_STORAGE_QUERY_PROPERTY, // operation to perform
        &query,
        sizeof(query),                                    // no input buffer
        //&devDesc,
        pDevDesc,
        devDescLength,                               // output buffer
        &nBytesRead,                                 // # bytes returned
        NULL);                                             // synchronous I/O
    if (!result)                                            //fail
    {
        fprintf(stderr,"GetDriveTypeByBus() failed! drive:%s\n",drive);
        fprintf(stderr, "IOCTL_STORAGE_QUERY_PROPERTY2 Error: %d  nBytesRead:%d\n", GetLastError(),nBytesRead);
        (void)CloseHandle(hDevice);
        free(pDevDesc);
        return false;
    }

    //printf("Driver.BusType %d\n", devDesc.BusType);
    //BusType=(int)devDesc.BusType;
    BusType=(int)pDevDesc->BusType;

    free(pDevDesc);

    (void)CloseHandle(hDevice);
    return true;
}
#endif
 



std::vector<std::string> getDriverList()
{
    std::vector<std::string> listRes;
#ifdef _WIN32
    TCHAR tBuf[1024]={0};
    DWORD drivLen=0;
    drivLen= GetLogicalDriveStrings(0, NULL);
    GetLogicalDriveStrings(drivLen,tBuf);
    TCHAR* pos=tBuf;
    while (*pos)
    {
        std::string sDriverName=::FromTCstr(pos);
        listRes.push_back(sDriverName);
        pos+=::Tstrlen(pos) +1;  
    }
#else
     //linux系统下没有驱动盘，只有挂载目录
    // OS   /run/media
    std::string sDriverName= "/run/media/";
	if (access(sDriverName.c_str(),F_OK) ==0)
	{
		//存在，查看目下面的
		DIR *pDir = NULL;
		struct dirent    *ent;
		pDir=opendir(sDriverName.c_str());
		while((ent=readdir(pDir))!=NULL)
		{
			if(ent->d_type & 4) //DT_DIR 4表示为目录，8表示为文件
			{
				std::string sDiskName = ent->d_name;
//                const char *p=strstr(sDiskName.c_str(),"sdb");
                // ARM LINUX 下为sdX?，如sda1,sdb2,要支持多个U盘同时插入，去掉b  [5/25/2020 jaontolt]
                const char *p=strstr(sDiskName.c_str(),"sd");
				if(p!=NULL)
				{
					std::string fullPath = sDriverName + sDiskName + "/";
					listRes.push_back(fullPath);
				}
			}
		}
		closedir(pDir);
	}
	else
	{
		//不存在
		printf("can not access /run/media/ \n");
	}




#endif

    PRINT_FUN_NAME
        for (size_t n=0;n<listRes.size();n++)
        {
            printf("Drive[%d]:%s\n",n,listRes.at(n).c_str());
        }

        return listRes;
}



bool isMovableDisk(const char* drive)
{
    if (isNullCstr(drive))
    {
        return false;
    }

#ifdef _WIN32     

    std::string winDrv="\\\\.\\";   //加windows设备前缀
    winDrv+=std::string(drive);

    int busType;
    //winsows下EXT3磁盘可能会获取不成功，但是也扫描内容 ，暂不判断返回值
    bool retGet=GetDriveTypeByBus(winDrv.c_str(),busType);    //需要管理员权限，否则可能获取失败
    printf("DriveBusType:%d\n",busType);
    if (busType==BusTypeUsb||busType==BusTypeUnknown)   
    {
        return true;
    }

#else
    //根据目录不能判断是否是移动硬盘，通常我们的移动硬盘挂载到/run/media/下面
    //
    // ARM LINUX 下为sdX?，如sda1,sdb2,要支持多个U盘同时插入，去掉b  [5/25/2020 jaontolt]
//     const char *p=strstr(drive,"/run/media/sdb");
     const char *p=strstr(drive,"/run/media/sd");
    if(p!=NULL)
    {
        return true;
    }
#endif
    return false;
}


#ifdef _WIN32   
bool do_processExist(const char* processName,PID& pid)      //PID& pid
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);     
    if(hProcessSnap == INVALID_HANDLE_VALUE)     
    {     
        return false;     
    }     

    PROCESSENTRY32 pe32;     
    pe32.dwSize = sizeof(PROCESSENTRY32);     
    if(!Process32First(hProcessSnap, &pe32))     
    {     
        CloseHandle(hProcessSnap);      
        return false;     
    }     

    do
    {
        std::string sszExeFile=FromTCstr(pe32.szExeFile);
        if(0 == strcmp(sszExeFile.c_str(),processName))
        {
            pid = pe32.th32ProcessID;     
            CloseHandle(hProcessSnap);     
            return true;     
        }     
    }while (Process32Next(hProcessSnap,&pe32));     

    CloseHandle(hProcessSnap);     
    return false;     
}

#else

bool do_processExist(const char* processName,pid_t& pid)      //PID& pid
{
    //todo
    return true;
}
#endif


bool KillProcessByName(const char* processName)
{
    printf("KillProcessByName():%s\n",processName);

#ifdef _WIN32  
    DWORD pid;  
    while (do_processExist(processName,pid))  
    {  
        //PROCESS_ALL_ACCESS PROCESS_TERMINATE
        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,0,pid); // gets handle to process  
        if (hProcess)  
        {
            //TerminateProcess(hProcess, 0); 
            //注：不要用if(ret==TRUE)去检测函数是否调用成功，因为函数调用成功会返回一个非零值，但不一定是TRUE(1)
            BOOL ret = TerminateProcess(hProcess,-1); // Terminate process by handle  
            CloseHandle(hProcess); // close the handle  
            if (!ret)  
            {  
                ::printf("TerminateProcess() failed errorCode:%d\n",GetLastError());
                return false;  
            }  
        }  
        else  
        {  
            ::printf(" OpenProcess() failed errorCode:%d\n",GetLastError());
            return false;  
        }  
    }  
#else  
    pid_t pid;
    while (do_processExist(processName,pid))  
    {  
        int ret = kill(pid, SIGKILL);  

        if(-1 == ret)  
        {  
            return false;  
        }  
    }  
#endif  
    return true;  
}

bool KillProcessByPath(const char* path)
{
    printf("KillProcessByPath(): path:%s\n",path);

    std::string spath(path);
    std::string sdrive;
    std::string sdir;
    std::string sname;
    std::string sext;
    SplitPath(spath,sdrive,sdir,sname,sext);     //取路径里面的程序名称
    return KillProcessByName(sname.c_str()); //根据名称杀进程，可能会误杀，同名，不同路径进程，暂不考虑此情况
} 

//bool CreatProcessByPath(std::string processName, std::string processWorkingPath, std::string startupParam = "", bool showWindow = true)

bool CreatProcessByPath(const char* processPath,char* startupParam, bool showWindow)
{
        if (isNullCstr(processPath))
        {
            return false;
        }
        std::string  sfullPath(processPath);
        std::string  sdrive;
        std::string  sdir;
        std::string  sname;
        std::string  sext;
        SplitPath(sfullPath,sdrive,sdir,sname,sext);   //全路径，提取路径

#ifdef _WIN32  
        int showmode=SW_HIDE;
        if (showWindow)  
        {  
            showmode=SW_SHOW;
        }
         Tstring sProcessfullPath=FromCstr(processPath);
         Tstring sProcessworkPath=FromCstr(sdir.c_str());
         Tstring sstartupParam=FromCstr(startupParam);

        HINSTANCE hInstance = ShellExecute(NULL, NULL,sProcessfullPath.c_str(),sstartupParam.c_str(), sProcessworkPath.c_str(), showmode);  
        if (32 < (int)hInstance)  
        {  
            return true;  
        }  
        else  
        {  
            return false;  
        }  
#else  
        //未测试  待完善
        std::string scmd="chmod 777 ";
//        std::string  sfullPath(processPath);
//        std::string  sdrive;
//        std::string  sdir;
//        std::string  sname;
//        std::string  sext;
//        SplitPath(sfullPath,sdrive,sdir,sname,sext);   //全路径，提取路径
        std::string  processWorkingPath=sdir;

        scmd+=processWorkingPath;
        system(scmd.c_str());  
        std::string cwp =getCurrentWorkingPath();  
        setCurrentWorkingPath(processWorkingPath);
        int ret = system((sfullPath + " " + startupParam + " &").c_str());  
        setCurrentWorkingPath(cwp);
        if (-1 == ret)  
        {  
            return false;  
        }  
#endif  
        return true;  
}


bool CreatProcessByPath(const char* path)     //通过可执行程序路径创建进程
{
    printf("CreatProcessByPath(): path:%s\n",path);
    char* startupParam="";
    bool showWindow = true;
    return  CreatProcessByPath(path,startupParam,showWindow);
}



SpaceInfo getDiskSpace(const char* dir,bool* ok)
{
    SpaceInfo  info; //init zero
    bool bSuccess=false;
#ifdef _WIN32
    Tstring sDrive=FromTCstr(dir);
    ULARGE_INTEGER avail, total, free;
    if(::GetDiskFreeSpaceEx(sDrive.c_str(), &avail, &total, &free)==TRUE)
    {
        info.capacity= (static_cast<uint64_t>(total.HighPart)<<32)+total.LowPart;
        info.free= (static_cast<uint64_t>(free.HighPart)<< 32)+free.LowPart;
        info.available= (static_cast<uint64_t>(avail.HighPart)<<32)+avail.LowPart;
        info.used=info.capacity-info.available;    //已经使用=总容量-可用
        bSuccess=true;
    }
#else  //for linux
    struct statvfs vfs;  //todo:vxworks statfs 

    //把文件系统信息读入 struct statvfs 中
    int error = statvfs(dir,&vfs);
    if(error>=0)
    {
        uint64_t FSIZE=vfs.f_frsize;   //todo:vxworks vfs.f_bsize
        info.capacity = static_cast<uint64_t>(vfs.f_blocks)*FSIZE;
        info.free = static_cast<uint64_t>(vfs.f_bfree)*FSIZE;
        info.available= static_cast<uint64_t>(vfs.f_bavail)*FSIZE;
        info.used=info.capacity-info.available;    //已经使用=总容量-可用
        bSuccess=true;
    }
#endif

    if (ok)
    {
        *ok=bSuccess;
    }
    if (!bSuccess)
    {
        PreDprintf("failed\n");
    }
    return info;
}


std::string SpaceToStringByUnit(uint64_t spaceSize)
{
    std::string sUnit;
    double rValue;
    if (spaceSize>1024*1024*1024)
    {
        rValue=(double)spaceSize/(1024*1024*1024);
        sUnit=(" GB");
    }
    else if(spaceSize>1024*1024)
    {
        rValue=(double)spaceSize/(1024*1024);
        sUnit=(" MB");
    }
    else if(spaceSize>1024)
    {
        rValue=(double)spaceSize/(1024);
        sUnit=(" KB");
    }
    else 
    {
        rValue=(double)spaceSize;
        sUnit=(" B");
    }
    char tmp[64];
    sprintf(tmp,"%.2f",rValue);
    return std::string(tmp)+sUnit;
}


int ColorArray[][3] =              //定义一个常用颜色的数组//共82种
{	
    {176,224,230},	//浅灰蓝色
    {65 ,105,225},	//品蓝
    {106,90 ,205},	//石板蓝
    {35 ,206,235},	//天蓝	
    {0  ,255,255},	//青色
    {56 ,94 ,15 },	    //绿土
    {8  ,46 ,84 },	    //靛青
    {127,255,212},	//碧绿色
    {64 ,224,208},	//青绿色
    {0  ,255,0  },	    //绿色
    {127,255,0  },	//黄绿色
    {61 ,145,64 },	//钴绿色
    {0  ,201,87 },  	//翠绿色
    {34 ,139,34 },	//森林绿
    {124,252,0  },	//草地绿
    {50 ,205,50 },	//酸橙绿
    {189,252,201},	//薄荷色	
    {107,142,35 },	//草绿色
    {48 ,128,20 },	//暗绿色
    {46 ,139,87 },	//海绿色
    {0  ,255,127},	//嫩绿色
    {160,32 ,240},	//紫色
    {138,43 ,226},	//紫罗蓝色
    {153,51 ,250},	//湖紫色	
    {218,112,214},	//淡紫色	
    {221,160,221},	//梅红
    {255,255,0  },	//黄色	 	
    {227,207,87 },	//香蕉色
    {255,153,18 },	//镉黄
    {255,215,0  },	//金黄色	
    {218,165,105},	//黄花色
    {227,168,105},	//瓜色	
    {255,97 ,0  },  	//橙色	
    {255,97 ,3  }, 	//镉橙	
    {237,145,33 },	//胡萝卜色	
    {255,128,0  },	//桔黄	
    {245,222,179},	//淡黄色
    {128,42 ,42 },	//棕色	
    {163,148,128},	//米色	
    {138,54 ,15 },	//锻浓黄土色	
    {135,51 ,36 },	//锻棕土色	
    {210,105,30 },	//巧克力色	
    {255,125,64 },	//肉色	
    {240,230,140},	//黄褐色	
    {188,143,143},	//玫瑰红	
    {199,97 ,20 },	//肖贡土色	
    {115,74 ,18 },	//标土棕	
    {94 ,38 ,18 },	   //乌贼墨棕	
    {160,82 ,45 },	//赫色	
    {139,69 ,19 },	//马棕色	
    {244,164,96 },	//沙棕色	
    {210,180,140},	//棕褐色	
    {0  ,0  ,255},	    //蓝色	
    {61 ,89 ,171},	//钴色	
    {3  ,168,158},	//锰蓝	
    {25 ,25 ,112},	//深蓝色	
    {51 ,161,201},	//孔雀蓝	
    {0  ,199,140},	//土耳其玉色	
    {255,0  ,0  },	    //红色	
    {156,102,31 },	//砖红	
    {227,23 ,13 },	//镉红	
    {255,127,80 },	//珊瑚色	
    {178,34 ,34 },	//耐火砖红	
    {176,23 ,31 },	//印度红	
    {176,48 ,96 },	//栗色	
    {255,192,203},	//粉红	
    {135,38 ,87 },	//草莓色	
    {250,128,114},	//橙红色	
    {255,99 ,71 },	//蕃茄红	
    {255,69 ,0  },	    //桔红	
    {255,0  ,255},	//深红色		
    {41 ,36 ,33 },	   //象牙黑	
    {192,192,192},	//灰色	
    {128,138,135},	//冷灰	
    {112,128,105},	//石板灰	
    {128,128,105},	//暖灰色	
    {225,225,225},	//白色	
    {250,235,215},	//古董白	
    {240,255,255},	//天蓝色	
    {245,245,245},	//白烟	
    {255,235,205},	//白杏仁	
    {252,230,201},	//蛋壳色	
};



/*
(一）指针法
#include <stdio.h>
int main()
{
    int val = 1;
    char *ret = (char *)&val;
    if (*ret == 1)
    printf("小端存储");
    else
    printf("大端存储");
    return 0;
}

（二）联合法
//Linux 操作系统中相关的源代码大小端转换如下:
//static union {char c[4]; unsigned long mylong;} endian_test = {{ 'l', '?', '?', 'b' }};
//#define ENDIANNESS ((char)endian_test.mylong)

#include <stdio.h>
union UN  //在union中所有的数据成员共用一个空间，同一时间只能储存其中一个数据成员
{
    int a;
    char c;
};
int main()
{
        union UN un;
        un.a = 1;
        if (un.c == 1)
        printf("小端存储");
        else
        printf("大端存储");
        return 0;
}
*/
#if 0
//16位  
#define BSWAP_16(x)\
    (uint16_t)((((uint16_t)(x) & 0x00ff) << 8)|\
    (((uint16_t)(x) & 0xff00) >> 8)\
    )  

//32位                 
#define BSWAP_32(x)\
    (uint32_t)((((uint32_t)(x) & 0xff000000) >> 24)|\
    (((uint32_t)(x) & 0x00ff0000) >> 8) |\
    (((uint32_t)(x) & 0x0000ff00) << 8) |\
    (((uint32_t)(x) & 0x000000ff) << 24)\
    )    

#define BSWAP_64(x) \
    (((uint64_t)x&0xff00000000000000LL)>>56|\
    ((uint64_t)x & 0x00ff000000000000LL)>>40|\
    ((uint64_t)x & 0x0000ff0000000000LL)>>24|\
    ((uint64_t)x & 0x000000ff00000000LL)>>8 |\
    ((uint64_t)x & 0x00000000ff000000LL)<<8 |\
    ((uint64_t)x & 0x0000000000ff0000LL)<<24|\
    ((uint64_t)x & 0x000000000000ff00LL)<<40|\
    ((uint64_t)x & 0x00000000000000ffLL)<<56)
#endif


#ifdef _MSC_VER
#include <intrin.h>
uint16_t ByteOrderSwap16Bit(uint16_t n)
{
    return _byteswap_ushort(n);
}
uint32_t ByteOrderSwap32Bit(uint32_t n)
{
    return _byteswap_ulong(n);
}
uint64_t ByteOrderSwap64Bit(uint64_t n)
{
     return _byteswap_uint64(n);
}
#elif defined(OS_LINUX)   //TODO:GCC

uint16_t ByteOrderSwap16Bit(uint16_t n)
{
    return __builtin_bswap16(n);
}
uint32_t ByteOrderSwap32Bit(uint32_t n)
{
    return __builtin_bswap32(n);
}
uint64_t ByteOrderSwap64Bit(uint64_t n)
{
    return __builtin_bswap64(n);
}
#endif

bool TimeCodeStringtoNum(const char sTimeCode[64],unsigned int FRate,unsigned int& FrameNum)
{
    FrameNum=0;
    unsigned int hour=0,min=0,sec=0,frame=0;
    if (sscanf(sTimeCode,"%02d:%02d:%02d:%02d",&hour, &min,&sec,&frame)!=4)   //返回匹配个数
    {
        return false;
    } 
    //小时不能大于23，分秒不能大于59
    if (hour>23||min>59||sec>59||frame>(FRate-1))      //数据越界
    {
        return false;
    }

    FrameNum=(hour*3600+min*60+sec)*FRate+frame;
    return true;
}

void TimeCodeNumtoString(unsigned int FrameNum,unsigned int FRate,char sTimeCode[64])
{
    unsigned int hour=FrameNum/FRate/3600;
    unsigned int min=FrameNum/FRate%3600/60;
    unsigned int sec=FrameNum/FRate%60;
    unsigned int frame=FrameNum%FRate;
    sprintf(sTimeCode,"%02d:%02d:%02d:%02d",hour, min,sec,frame);
}


void TimeInMstoTimeCodeString(unsigned int tImeInms,unsigned int FRate,char sTimeCode[64])
{
    unsigned int sumsecond =tImeInms/1000;
    unsigned int hour=sumsecond/3600;
    unsigned int min=(sumsecond%3600)/60;
    unsigned int sec=sumsecond%60;
    unsigned int frame=(tImeInms%1000)*FRate/1000;    //帧号
    sprintf(sTimeCode,"%02d:%02d:%02d:%02d",hour,min,sec,frame);
}


unsigned int TimeCodeStringtoNum(std::string strTimeCode,unsigned int FRate,bool* isOK)  //可能失败 字符串不符合格式 是否成功返回在isOK里面
{
    char sTimeCode[64];
    unsigned int FrameNum=0;
    size_t len=strTimeCode.length();
    if (len>(sizeof(sTimeCode)-1))
    {
        len=sizeof(sTimeCode)-1;
    }
    sTimeCode[len]=0;
    memcpy(sTimeCode,strTimeCode.c_str(),len);
    bool ret=TimeCodeStringtoNum(sTimeCode,FRate,FrameNum);
    if (isOK!=NULL)
    {
        *isOK=ret;
    }
    return FrameNum;
}

std::string TimeCodeNumtoString(unsigned int FrameNum,unsigned int FRate)  //肯定成功
{
    char sTimeCode[64];
    TimeCodeNumtoString(FrameNum,FRate,sTimeCode);
    return std::string(sTimeCode);
}





#if 0
unsigned int StringToStream(char* buf,std::string& s)
{
    unsigned int len=0;
    char* pos=buf;
    unsigned int sLen=s.length();
    *((unsigned int*)pos)=sLen;
    pos+=sizeof(sLen);
    memcpy(pos,s.data(),sLen);
    pos+=sLen;
    return pos-buf;
}

unsigned int StringFromStream(char* buf,std::string& s)
{
    char* pos=buf;
    unsigned int sLen= *((unsigned int*)pos);
    pos+=sizeof(sLen);
    s=std::string(pos,sLen);
    pos+=sLen;
    return pos-buf;
}
#endif


#include <time.h>
//从日期宏定义取日期字符串作为程序版本号--几种实现方式
time_t getDateFromMacro(char const *date) 
{
    char s_month[5];
    int month, day, year;
    struct tm t = {0};
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    sscanf(date, "%s %d %d", s_month, &day, &year);
    month = (strstr(month_names, s_month)-month_names)/3;
    t.tm_mon = month;
    t.tm_mday = day;
    t.tm_year = year - 1900;
    t.tm_isdst = -1;
    return mktime(&t);
}

void getDateMacrotoString(char buf[16]) 
{
    const char* date=__DATE__;
    char s_month[5];
    int month, day, year;
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    sscanf(date, "%s %d %d", s_month, &day, &year);
    month=(int)(strstr(month_names,s_month)-month_names)/3;
    sprintf(buf,"%04d%02d%02d",year,month+1,day);   //月份从零 加1
}


std::string getDateMacrotoString() 
{
    char buf[16];
    getDateMacrotoString(buf);
    return std::string(buf);
}




#ifdef UNIT_TEST

int Test_MyBasicOperation()
{
#if 0
    //char* path="C:\\1111\\222\\test.mp3";
    char* path="C:/1111/222/test.mp3";
    char drive[1024];
    char dir[1024];
    char filename[1024];
    char ext[1024];
    SplitPath(path,drive,dir,filename,ext);
    //printf("path:%s\ndrive:%s\ndir:%s\nfilename:%s\next:%s\n",path,drive,dir,filename,ext);
#endif

    //CreatProcessByPath("H:/LEONIS_HoloSound_System/bin/Win32/Release/Core_PushAudioFrom_AES16.exe");


    return 0;
}





#include <stdio.h>
//#include <execinfo.h>   //linux
//void printCallStack()
//{
//
//#ifdef _WIN32
//    //unsigned int   i;  
//    //void         * stack[ 100 ];  
//    //unsigned short frames;  
//    //SYMBOL_INFO  * symbol;  
//    //HANDLE         process;  
//    //process = GetCurrentProcess();  
//    //SymInitialize( process, NULL, TRUE );  
//    //frames               = CaptureStackBackTrace( 0, 100, stack, NULL );  
//    //symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );  
//    //symbol->MaxNameLen   = 255;  
//    //symbol->SizeOfStruct = sizeof( SYMBOL_INFO );  
//    //for( i = 0; i < frames; i++ )  
//    //{  
//    //    SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );  
//    //    printf( "%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address );  
//    //}  
//    //free( symbol );  
//
//#else
//        //const int STACK_SIZE=32;
//        //void *trace[STACK_SIZE];
//        //size_t size = backtrace(trace, STACK_SIZE);
//        //char **symbols = (char **)backtrace_symbols(trace,size);
//        //size_t i = 0;
//        //for(; i<size; i++)
//        //{
//        //    printf("%d--->%s\n", i, symbols[i]);
//        //}
//        //return;
//#endif
//}




#endif



