#ifndef  BASICOPERATION_Header
#define BASICOPERATION_Header


//项目具体业务无关的基础操作
//#include "Super/Basic/BaseOperation.h"
//自己的常用函数跨平台封装,尽量只依赖标准C,C++为了避免污染命名空间不用using namespace std

#include "global.h" 
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory.h>


BASIC_API inline void print_bin(const void* buf,unsigned int len){printBin(buf,len);}
BASIC_API inline void print_hex(const void* buf,unsigned int len,unsigned int perLineChs=16)
{
    printHex(buf,len,perLineChs);
}

//BASIC_API void printCallStack();                                        //打印调用堆栈--TO DO:


extern BASIC_API std::wstring BS_StringToWString(const std::string &str);
extern BASIC_API std::string BS_WStringToString(const std::wstring &wstr);



BASIC_API std::string IntToString(int val);                    
BASIC_API std::string IntToString(unsigned int val);
BASIC_API std::string IntToString(int64_t val);
BASIC_API std::string IntToString(uint64_t val);
BASIC_API std::string FloatToString(float val);              
BASIC_API std::string DoubleToString(double val);


//defaultValue失败的时候返回,isOk返回是否转换成功
BASIC_API int StringToInt(const std::string& str,const int defaultV=0,bool* isOk=0);            
BASIC_API float StringToFloat(const std::string& str,const float defaultV=0.0f,bool* isOk=0);
BASIC_API double StringToDouble(const std::string& str,const double defaultV=0.0,bool* isOk=0);

////自己写的转换函数，可能低效，但是易于理解  依赖上面两个string与wstring之间的转换函数

BASIC_API inline std::wstring IntToWString(int val){return BS_StringToWString(IntToString(val));}
BASIC_API inline std::wstring FloatToWString(float val){return BS_StringToWString(FloatToString(val));}
BASIC_API inline std::wstring DoubleToWString(double val){return BS_StringToWString(DoubleToString(val));}

BASIC_API inline int WStringToInt(const std::wstring& wstr,const int defaultV=0,bool* isOk=0)
{
    return StringToInt(BS_WStringToString(wstr),defaultV,isOk);
}
BASIC_API inline float WStringToFloat(const std::wstring& wstr,const float defaultV=0,bool* isOk=0)
{ 
    return StringToFloat(BS_WStringToString(wstr),defaultV,isOk);
}
BASIC_API inline double WStringToDouble(const std::wstring& wstr,const double defaultV=0,bool* isOk=0)
{
    return StringToDouble(BS_WStringToString(wstr),defaultV,isOk);
}


//待续base 表示进制 base<=36 to be tested
BASIC_API std::string NumToString(int32_t n,int base=10);                    
BASIC_API std::string NumToString(uint32_t n,int base=10);                  
BASIC_API std::string NumToString(int64_t n,int base=10);
BASIC_API std::string NumToString(uint64_t n,int base=10);

//控制浮点格式与 保留小数位数
BASIC_API std::string NumToString(float n,int dec=6);
BASIC_API std::string NumToString(double n,int dec=6);


BASIC_API inline bool isDigital(char n){return (n >='0'&&n<='9');}
BASIC_API double my_atofV2(const char *str);

//字符串解析数值,失败返回defVal默认值,pEnd,返回搜索到的字符串结尾位置
//isOK存放解析过程是否出错，成功为true
BASIC_API double stringToDouble(const char *ps,double defVal=0,bool* isOK=NULL,const char** pEnd=NULL);
BASIC_API int64_t stringToInt64(const char *ps,int64_t defVal=0,bool* isOK=NULL,const char** pEnd=NULL);


//在一个字符串中查找多个数字并返回, 考虑不完善 todo
BASIC_API std::vector<int> string_FindIntNum(const std::string& str);
BASIC_API std::vector<float> string_FindFloatNum(const std::string& str);


#if 0
//根据名称查询其值和单位如gain:20dB 单位保存在pUnit 失败返回默认值defVal 
BASIC_API double string_FindLabelValueUnitF(const char* pSrc,const char* labelName,
                                            char* pUnitRes=NULL,double defVal=0,bool* isOK=NULL);
#endif


//字符串里寻找Label 标签里的数置,找到则填充返回值和结束位置 若label为NULL或"" ,直接查找值 不管标签
//失败返回defVal默认值,pEnd,返回搜索到的字符串结尾位置 isOK存放解析过程是否出错，成功为true
BASIC_API double string_FindLabelValueF(const char* pSrc,const char* labelName,double defVal=0,bool* isOK=NULL,const char** pEnd=NULL);
BASIC_API double string_FindLabelValueF(const std::string& Src,const char* labelName,double defVal=0,bool* isOK=NULL,const char** pEnd=NULL);
BASIC_API int64_t string_FindLabelValue(const char* pSrc,const char* labelName,int64_t defVal=0,bool* isOK=NULL,const char** pEnd=NULL);
BASIC_API int64_t string_FindLabelValue(const std::string& Src,const char* labelName,int64_t defVal=0,bool* isOK=NULL,const char** pEnd=NULL);

//获取某个标签下的值,可以返回多个,例如在字符串NRP:100 NRP:200 NRP:90  里提取100 200 90
BASIC_API std::vector<double> string_FindAllLabelValueF(const std::string& str,const char* labelName);
BASIC_API std::vector<int64_t> string_FindAllLabelValue(const std::string& str,const char* labelName);


//文件操作
BASIC_API uint64_t getFileSize(const char* path);         //取文件大小,返回字节数
BASIC_API int64 getCreateTime(const char* path);        //取文件创建时间
BASIC_API uint64_t getFloderFileSize(const char* dir);       //取目录下文件总大小,返回字节数  todo:待续
BASIC_API bool isExistFile(const char *path);                 //判断文件是否存在
BASIC_API bool isDirectory(const char* path);               //判断文件夹是否存在 Absolute path绝对路径
BASIC_API int isFileOrDirectory(const char* path);         //判断是文件还是文件夹 //0:不存在，1为文件, 2为文件夹
BASIC_API bool isExist(const char *path);                      //判断是否存在,文件或者目录都算,
BASIC_API bool createDirectory(const char* path );       //根据路径创建目录
BASIC_API bool deleteDirectory(const char * path);       //删除目录
BASIC_API bool deleteFile(const char * path);                //删除文件,根据路径
BASIC_API bool deletePath(const char * path);              //删除路径，是文件删除文件,是目录删除整个目录
BASIC_API bool RenamePath(const char* snew,const char* sold);   //重命名文件或目录
//BASIC_API bool symlink(const char* dst, const char* lnk);   //todo

//调用系统命令实现各种文件/目录操作
BASIC_API bool moveFilebySysCall(const char *to,const char *from,bool bCover=true);        //移动文件 bCover控制是否强制覆盖
BASIC_API bool moveDirectorybySysCall(const char *to,const char *from,bool bCover=true);//移动文件夹bCover控制是否强制覆盖
BASIC_API bool copyFilebySysCall(const char* dst,const char* src,bool bCover=true);            //拷贝文件bCover控制是否强制覆盖
BASIC_API bool copyDirectorybySysCall(const char* dst,const char* src,bool bCover=true);   //拷贝文件夹bCover控制是否强制覆盖
BASIC_API bool copyFileToDirectory(const char* dst,const char* src,bool bCover=true);   //拷贝文件到文件夹bCover控制是否强制覆盖
BASIC_API bool deletePathBySysCall(const char* path,bool bForce=true);                              //删除路径,bForce控制是否强制

BASIC_API bool copyTimeAttributes(const char* dst,const char* src);   //拷贝原文件的时间属性覆盖到目标文件

BASIC_API uint64_t getFileCheckSum(const char* pathSrc);                          //获取文件的校验值,自定义快速算法
BASIC_API bool CompareFileCheck(const char* path1,const char* path2);   //比较两个文件是否相等,基于快速校验值

//C++ std::string version
BASIC_API inline uint64_t getFileSize(const std::string& path){return getFileSize(path.c_str());}
BASIC_API inline uint64_t getFloderFileSize(const std::string& path){return getFloderFileSize(path.c_str());}
BASIC_API inline bool isExistFile(const std::string& path){return isExistFile(path.c_str());}
BASIC_API inline bool isDirectory(const std::string& path){return isDirectory(path.c_str());}
BASIC_API inline int isFileOrDirectory(const std::string& path){return isFileOrDirectory(path.c_str());}
BASIC_API inline bool isExist(const std::string& path) {return isExist(path.c_str());}
BASIC_API inline bool createDirectory(const std::string& path){return createDirectory(path.c_str());}
BASIC_API inline bool deleteDirectory(const std::string& path){return deleteDirectory(path.c_str());}
BASIC_API inline bool deleteFile(const std::string& path){return deleteFile(path.c_str());}
BASIC_API inline bool deletePath(const std::string& path){return deletePath(path.c_str());}
BASIC_API inline bool moveFilebySysCall(const std::string& to,const std::string& from,bool bCover=true)
{
    return moveFilebySysCall(to.c_str(),from.c_str(),bCover);
}
BASIC_API inline bool moveDirectorybySysCall(const std::string& to,const std::string& from,bool bCover=true)
{
    return moveDirectorybySysCall(to.c_str(),from.c_str(),bCover);
}
BASIC_API inline bool copyFilebySysCall(const std::string& to,const std::string& from,bool bCover=true)
{
    return copyFilebySysCall(to.c_str(),from.c_str(),bCover);
}
BASIC_API inline bool copyDirectorybySysCall(const std::string& to,const std::string& from,bool bCover=true)
{
    return copyDirectorybySysCall(to.c_str(),from.c_str(),bCover);
}
BASIC_API inline bool copyFileToDirectory(const std::string& to,const std::string& from,bool bCover=true)
{
	return copyFileToDirectory(to.c_str(),from.c_str(),bCover);
}

BASIC_API inline bool copyTimeAttributes(const std::string& dst,const std::string& src,bool bCover=true)
{
    return copyTimeAttributes(dst.c_str(),src.c_str(),bCover);
}
BASIC_API inline uint64_t getFileCheckSum(const std::string& path){return getFileCheckSum(path.c_str());}
BASIC_API inline bool CompareFileCheck(const std::string& path1,const std::string& path2){CompareFileCheck(path1.c_str(),path2.c_str());}



////可查询进度的文件/文件夹拷贝TODO:
//BASIC_API bool copyFileByProgress(const char* path_dst,const char* path_src,uint64_t* totalSize=0,uint64_t* isCopyed=0);
//BASIC_API bool copyDirectoryByProgress(const char* path_dst,const char* path_src,uint64_t* totalSize=0,uint64_t* isCopyed=0);  


//跨平台getlasterror
#ifdef _WIN32
    #define GetErrorNo() GetLastError()  
#else  
    #define GetErrorNo() errno  
#endif  


//路径分割
BASIC_API void SplitPath(const char* fullpath,char* drive,char* dir,char* filename,char* ext);
BASIC_API void SplitPath(const std::string fullpath,std::string& sdrive,std::string& sdir,std::string& sfilename,std::string&  sext);




//windows io.h
//enum EFileAttributeWin
//{
//    _A_ARCH,       //档案。文件被BACKUP指令改变或清除时被设置。值：0x20。
//    _A_HIDDEN,   //隐藏。使用DIR指令一般看不到，除非使用/AH选项。值：0x02。
//    _A_NORMAL,  //普通。文件没有更多属性被设置，可以没有限制地被读或写。值：0x00。
//    _A_RDONLY,  //只读。不能以“写”为目的打开该文件，并且不能创建同名的文件。值：0x01。
//    _A_SUBDIR,    //子目录。值：0x10。
//    _A_SYSTEM   //系统文件。使用DIR指令一般看不见，除非使用/A或/A:S选项。值：0x04
//};
//文件属性，win/linux 不同，待完善


//待实现
struct FileInfo
{
    //_A_ARCH（存档）、_A_HIDDEN（隐藏）、_A_NORMAL（正常）、_A_RDONLY（只读）、_A_SUBDIR（文件夹）、_A_SYSTEM（系统）。
    enum FileAttrib
    {
        FileAttrib_NORMAL=0x00,    /* Normal file - No read/write restrictions */
        FileAttrib_RDONLY=0x01,     /* Read only file */
        FileAttrib_HIDDEN=0x02,     /* Hidden file */
        FileAttrib_SYSTEM=0x04,     /* System file */
        FileAttrib_SUBDIR=0x10,     /* Subdirectory */
        FileAttrib_ARCH=0x20,        /* Archive file */
    };
	bool isSuccessfull;
    unsigned int RelativeDepth;   //当前文件相对开始搜索时的路径深度
    //参照_finddata_t结构
    int attrib;                     //文件属性
    time_t time_create;    //文件创建时间（FAT文件系统为-1）以UTC格式，转换成当地时间，使用localtime_s()。
    time_t time_access;   //文件最后一次被访问的时间
    time_t time_write;     //文件最后一次被写入的时间
    size_t size;                 //文件的长度 字节为单位
    char name[1024];       //_MAX_PATH=260 文件名，不包含路径
    char ext[256];            //文件后缀
    //add by me
    char  fullPath[1024];  //全部路径，文件名
    FileInfo()
    {
        isSuccessfull=false;
        attrib=FileAttrib_NORMAL;
        time_create=0; 
        time_access=0;
        time_write=0;
        size=0;
        memset(name,0,sizeof(name));
        memset(fullPath,0,sizeof(fullPath));
    }

    void print() const
    {
        printf("name:%s \nsize:%llu\n",name,(uint64_t)size);
    }
};

//struct FilterFunResult
//{
//    bool isMatch;
//    bool bContinue;
//};


BASIC_API FileInfo getFileInfo(const char* pDir);                          //获取文件信息
BASIC_API std::vector<std::string> getAllPath(const char* pDir);  //获取某个目录下所有文件路径

//获取文件的过滤条件函数指针,返回true表示匹配,加入搜索列表,bBreak为true则中止搜索。
typedef bool (*FunFileCond)(FileInfo& fileInfo,bool& bBreak);      


//去某个路径下的文件信息，可以传参决定是否递归子目录,depth表示递归深度，0表示全部
BASIC_API void getFileInfo(std::vector<FileInfo>& VFileInfos,const char* dir,FunFileCond pFC=NULL,bool bSubDir=true,unsigned int MaxDepth=UINT32_MAX);

//根据后缀取文件路径 多种后缀名称用|符合分割  形如".xml|.txt|.doc" 等。后缀比较忽略大小写
BASIC_API std::vector<std::string> getFilePath(const char* dir,const char* SuffixNames="",bool bSubDir=true,unsigned int MaxDepth=UINT32_MAX);

//BASIC_API std::vector<std::string> getFilePathFtp(const char* dir,const char* SuffixNames="",bool bSubDir=true,unsigned int MaxDepth=UINT32_MAX);
////BASIC_API inline std::vector<std::string> getFilePathFtp(std::string dir,const char* SuffixNames="",bool bSubDir=true,unsigned int MaxDepth=UINT32_MAX)
////{
////    return getFilePathFtp(dir.c_str(),SuffixNames,bSubDir,MaxDepth);
////}

typedef bool (*FunDirCond)(const char* dir,bool& bBreak); 
//获取某个目录下子目录,根据条件
BASIC_API std::vector<std::string> getSubDirs(const char* pDir,FunDirCond pFC=NULL,unsigned int maxDepth=UINT32_MAX);


BASIC_API std::string getFileNameFromPath(const std::string& sPath);  //从文件路径提取文件名
BASIC_API std::string getDirFromFullPath(const std::string& sFilePath);      //从文件全路径提取上级目录

BASIC_API std::string getExtensionFromPath(const std::string& sPath); //从路径文件名取文件后缀,用file.txt 返回.txt
BASIC_API std::string getExtensionFromPathNotDot(const std::string& sPath); //从路径文件名取文件后缀,用file.txt 返回txt

BASIC_API std::string delDirTailSeparatorIfHas(const std::string& s);  //去掉结尾目录分隔符，如果有的话
BASIC_API std::string addDirSeparatorIfnot(const std::string& s);  //添加目录分隔符，如果没有的话 "/"



//另一种是遍历思路,迭代器思路--待续



//DFS BFS 深度优先、广度优先遍历
BASIC_API int traversalFolder(const char* pDir,void* pFun);   //遍历文件夹对文件做某种处理，传入函数指针
BASIC_API void getDisk(char* DiskName,uint64_t& totalsize,uint64_t& usedsize);   //获取驱动器总大小，已经使用大小


//根据路径读文件内容到内存,textMode文件打开方式,true为文本,false为二进制
BASIC_API bool readFiletoBuf(const char* path,char* buf,size_t size,bool textMode=false);
//根据路径写内存数据到文件,textMode文件打开方式,true为文本,false为二进制
BASIC_API bool writeBuftoFile(const char* path,const char* buf,size_t size,bool textMode=false);

//根据路径读文件内容到内存,textMode文件打开方式,true为文本,false为二进制
//读取小文件到内存，成功返回新内存地址和数据大小(需要手动释放)，失败返回NULL
BASIC_API char* readFiletoNewBuf(const char* path,size_t& size,bool textMode=false);


//主要用于一些小文件与字符串之间的互转,得到的字符串保存的是文件原始二进制串。使用时要注意
BASIC_API bool SaveStringToFile(const std::string sPath,const std::string& sTxt);
BASIC_API bool FileToString(const std::string sPath,const std::string& sTxt);
BASIC_API std::string FileToString(const std::string sPath,bool* isOK=0);     //




//纯C  获取UUID
//纯C  MD5

//IP字符串地址和int32_t  一一映射互转    QTtoIPv4Address();


//字符串分割函数  std::string 没有split函数
//pattern为用来分割原字符串的标志 如- ， 等等
BASIC_API std::vector<std::string> Split(const std::string& str,const std::string& pattern);

//在sBig中查找sSrc替换为sDst 
//BASIC_API void Replace(std::string &sBig, const std::string &sSrc, const std::string &sDst );

//查找sSrc替换为sDst ,并返回
BASIC_API std::string Replace(const std::string& sOld,const std::string& sSrc, const std::string& sDst);



//移除头尾不不可见字符串,包括空格换行
BASIC_API std::string Trim_head_tail_InvisibleChar(std::string& str);


//不区分大小写的字符串比较,相等返回true,否则false caseSensitive为true则区分大小写
BASIC_API bool Comp(const std::string& str1,const std::string& str2,bool caseSensitive=false);
BASIC_API bool Comp(const std::wstring& str1,const std::wstring& str2,bool caseSensitive=false);


//字符串查找,caseSensitive为true则区分大小写,找到返回子串位置,否则返回std::string::npos即-1
BASIC_API size_t Find(const std::string& str,const std::string& substr,bool caseSensitive=false);
BASIC_API size_t Find(const std::wstring& str,const std::wstring& substr,bool caseSensitive=false);

//字符串包含判断,caseSensitive为true则区分大小写, 包含子串返回true,否则false
BASIC_API bool Contains(const std::string& str,const std::string& substr,bool caseSensitive=false);
BASIC_API bool Contains(const std::wstring& str,const std::wstring& substr,bool caseSensitive=false);


//std::string helper 弥补标准string的不足--不断添加待续
BASIC_API std::string& ltrim(std::string& s);  //移除字符串左边空格
BASIC_API std::string& rtrim(std::string& s); //移除字符串左边空格
BASIC_API std::string& trim(std::string& s);  //移除字符串左右边空格

//是否以某子字符串开始/结尾 caseSensitive=false则不区分大小写
BASIC_API bool startsWith(const std::string& str, const std::string& prefix,bool caseSensitive=true);
BASIC_API bool endsWith(const std::string& str, const std::string& suffix,bool caseSensitive=true);
BASIC_API bool startsWith(const std::wstring& str, const std::wstring& prefix,bool caseSensitive=true);
BASIC_API bool endsWith(const std::wstring& str, const std::wstring& suffix,bool caseSensitive=true);


BASIC_API size_t indexOf(const std::string& str, const std::string& substr);
BASIC_API std::string toUpper(const std::string& str);  //转大写
BASIC_API std::string toLower(const std::string& str);  //转小写
BASIC_API std::string FormatString(const char *fmt, ...);         //格式化字符串



//系统操作--跨平台
BASIC_API void SystemRestart(bool bForce=false);          //重启系统 bForce为true强制关闭,应用程序可能丢失数据
BASIC_API void SystemShutdown(bool bForce=false);     //关闭系统 bForce为true强制关闭,应用程序可能丢失数据
BASIC_API void SystemLogout();                                     //注销系统
BASIC_API unsigned int getCpuCount();                          //获取CPU核心数量
BASIC_API void showCurrProcessMemoryInfo(void);        //显示当前进程的内存占用等信息



struct  SystemMemoryInfo
{
    uint64_t MemoryLoad;  //返回一个介于0～100之间的值，用来指示当前系统内存的使用率
    uint64_t TotalPhys;        //返回总的物理内存大小，以字节(byte)为单位
    uint64_t AvailPhys;        //返回可用的物理内存大小，以字节(byte)为单位
    uint64_t TotalPageFile;  //显示可以存在页面文件中的字节数。注意这个数值并不表示在页面文件在磁盘的真实物理大小
    uint64_t AvailPageFile;  //返回可用的页面文件大小，以字节(byte)为单位
    uint64_t TotalVirtual;    //返回调用进程的用户模式部分的全部可用虚拟地址空间，以字节(byte)为单位
    uint64_t AvailVirtual;     //返回调用进程的用户模式部分的实际自由可用的虚拟地址空间，以字节(byte)为单位
    double usedPercentPhys;               //已经使用的物理内存百分比
    SystemMemoryInfo(){
        Reset();
    }
    void Reset()
    {
        MemoryLoad=TotalPhys=AvailPhys=TotalPageFile=AvailPageFile=TotalVirtual=AvailVirtual=0;
        usedPercentPhys=0;
    }
};
BASIC_API bool getSysMemoryInfo(SystemMemoryInfo& memInfo);
BASIC_API void showSystemMemoryInfo();

BASIC_API double getCurretProcessUsage();                   //获取当前进程CPU占用率


class CPUInfoPrivate;
class CPUInfo
{
public:
    CPUInfo();
    CPUInfo(int ProcessID);
    ~CPUInfo();
    bool Initialize();
    //调用Initialize后要等待1左右秒再调用此函数
    int GetCPU_UseRate();   //获取CPU总体利用率
    float getProcessCpuUsage();   //获取进程CPU占用率，可以修改监控的进程
private:
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    double m_fOldCPUIdleTime;
    double m_fOldCPUKernelTime;
    double m_fOldCPUUserTime;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//获取进程CPU 占用率相关
    // 时间转换  
    //uint64_t file_time_2_utc(const FILETIME* ftime);
    // 获得CPU的核数  
    static int get_processor_number();

    //初始化
    void init();
    //关闭进程句柄
    void clear();
    //返回值为进程句柄，可判断OpenProcess是否成功
    //HANDLE setpid(DWORD ProcessID);
    
    void* setProcessID(int ProcessID);
    //-1 即为失败或进程已退出； 如果成功，首次调用会返回-2（中途用setpid更改了PID后首次调用也会返回-2）
private:
    CPUInfoPrivate* pimpl;
};

 

BASIC_API std::string getCurrentWorkingPath();                       //获取当前程序工作目录
BASIC_API bool setCurrentWorkingPath(std::string& sNew);     //设置当前程序工作目录--待续
BASIC_API std::string getCurrentAppPath();                               //获取当前程序所在目录
BASIC_API bool RelativePathToAbsolutePath(char path_abs[1024],char path_rela[1024]);   //相对路径转绝对路径
BASIC_API bool isAbsolutePath(const char *Filename);  //! Determine if the specified filename refers to an absolute path



//文件路径格式整理,把\\替换为/，去除重叠的目录分隔符等 strlen(path)<2000;
BASIC_API void FormatPath(char* dst,const char* src);
BASIC_API std::string FormatPath(const std::string& spath);


//取一个路径的上一级父目录,没有则返回本身
BASIC_API void getParentPath(char* parent,const char* path);  //char parent[1024] gcc link error
BASIC_API std::string getParentPath(const std::string& path);


BASIC_API std::vector<std::string> getDriverList();     //获取本机硬盘驱动器列表
BASIC_API bool isMovableDisk(const char* drive);        //根据盘符判断是否是移动硬盘



BASIC_API bool KillProcessByName(const char* name);     //通过进程名称杀进程
BASIC_API bool KillProcessByPath(const char* path);         //通过进程可执行程序路径杀进程
BASIC_API bool CreatProcessByPath(const char* path);     //通过可执行程序路径创建进程




struct SpaceInfo
{
    // all values are byte counts 以字节为单位
    uint64_t capacity;
    uint64_t  free;         //<=capacity
    uint64_t  available; //<= free
    uint64_t  used;       //<= capacity
    SpaceInfo(){capacity=free= available=used=0;}
    void print()const {
    printf("capacity:%llu free:%llu avail:%llu used:%llu\n",capacity,free,available,used);
    }
};
//获取磁盘容量capacity总容量  avail有效容量 free未使用容量,used已用容量 概念冗余暂留
BASIC_API SpaceInfo getDiskSpace(const char dir[1024],bool* ok=0); 
BASIC_API std::string SpaceToStringByUnit(uint64_t spaceSize);   //硬盘空间大小转成以GB,MB,KB结尾的字符串


//从内存地址获取某种类型,考虑ARM非对齐访问可能崩溃，抽象出来
//#ifndef OS_ARM_LINUX
#define IGNORE_NOT_ALIGN_ACCESS
//#endif

#ifdef IGNORE_NOT_ALIGN_ACCESS
template<typename T>
inline T LoadData(char* p)
{
    return *((T*)p);
}
template<typename T>
inline void LoadData(T& t,char* p)
{
    t=*((T*)p);
}
template<typename T>
inline void StoreData(char* p,const T& t)
{
    *((T*)p)=t;
}
#else

template<class T>
inline T LoadData(void* p)
{
    T res;
    char* pD=(char*)(&res);
    char* pS=(char*)p;
    for (int n = 0;n<sizeof(T);n++)
    {
        //*pD++=*pS++;
        pD[n]=pS[n];
    }
    return res;
}

template<class T>
inline void LoadData(T& res,void* p)
{
    char* pD=(char*)(&res);
    char* pS=(char*)p;
    for (int n = 0;n<sizeof(T);n++)
    {
        //*pD++=*pS++;
        pD[n]=pS[n];
    }
}
template<class T>
inline void StoreData(void* p,const T& t)
{
    char* pD=(char*)p;
    char* pS=(char*)(&t);
    for (int n = 0;n<sizeof(T);n++)
    {
        //*pD++=*pS++;
        pD[n]=pS[n];
    }
}

inline void LoadData(char& n,void* p)
{
    n=*((char*)p);
}
inline void StoreData(void* p,char n)
{
    *((char*)p)=n;
}
inline void LoadData(unsigned char& n,void* p)
{
    n=*((unsigned char*)p);
}
inline void StoreData(void* p,unsigned char n)
{
    *((unsigned char*)p)=n;
}

inline void LoadData(bool& n,void* p)
{
    n=*((bool*)p);
}
inline void StoreData(void* p,bool n)
{
    *((bool*)p)=n;
}
inline void LoadData(short& n,void* p)
{
    ((char*)(&n))[0]=((char*)p)[0];
    ((char*)(&n))[1]=((char*)p)[1];
}
inline void StoreData(void* p,const short n)
{
    ((char*)p)[0]=((char*)(&n))[0];
    ((char*)p)[1]=((char*)(&n))[1];
}
inline void LoadData(int& n,void* p)
{
    ((char*)(&n))[0]=((char*)p)[0];
    ((char*)(&n))[1]=((char*)p)[1];
    ((char*)(&n))[2]=((char*)p)[2];
    ((char*)(&n))[3]=((char*)p)[3];
}
inline void StoreData(void* p,const int n)
{
    ((char*)p)[0]=((char*)(&n))[0];
    ((char*)p)[1]=((char*)(&n))[1];
    ((char*)p)[2]=((char*)(&n))[2];
    ((char*)p)[3]=((char*)(&n))[3];
}
inline void LoadData(int64_t& n,void* p)
{
    ((char*)(&n))[0]=((char*)p)[0];
    ((char*)(&n))[1]=((char*)p)[1];
    ((char*)(&n))[2]=((char*)p)[2];
    ((char*)(&n))[3]=((char*)p)[3];
    ((char*)(&n))[4]=((char*)p)[4];
    ((char*)(&n))[5]=((char*)p)[5];
    ((char*)(&n))[6]=((char*)p)[6];
    ((char*)(&n))[7]=((char*)p)[7];
}
inline void StoreData(void* p,const int64_t n)
{
    ((char*)p)[0]=((char*)(&n))[0];
    ((char*)p)[1]=((char*)(&n))[1];
    ((char*)p)[2]=((char*)(&n))[2];
    ((char*)p)[3]=((char*)(&n))[3];
    ((char*)p)[4]=((char*)(&n))[4];
    ((char*)p)[5]=((char*)(&n))[5];
    ((char*)p)[6]=((char*)(&n))[6];
    ((char*)p)[7]=((char*)(&n))[7];
}

inline void LoadData(unsigned short& n,void* p)
{
    ((char*)(&n))[0]=((char*)p)[0];
    ((char*)(&n))[1]=((char*)p)[1];
}
inline void StoreData(void* p,const unsigned short t)
{
    StoreData(p,(short)t);
}
inline void LoadData(unsigned int& n,void* p)
{
    //LoadData(*(int*)&n,p);
    ((char*)(&n))[0]=((char*)p)[0];
    ((char*)(&n))[1]=((char*)p)[1];
    ((char*)(&n))[2]=((char*)p)[2];
    ((char*)(&n))[3]=((char*)p)[3];
}
inline void StoreData(void* p,const unsigned int t)
{
    StoreData(p,(int)t);
}
inline void LoadData(uint64_t& n,void* p)
{
    //LoadData(*(int64_t*)&r,p);
    ((char*)(&n))[0]=((char*)p)[0];
    ((char*)(&n))[1]=((char*)p)[1];
    ((char*)(&n))[2]=((char*)p)[2];
    ((char*)(&n))[3]=((char*)p)[3];
    ((char*)(&n))[4]=((char*)p)[4];
    ((char*)(&n))[5]=((char*)p)[5];
    ((char*)(&n))[6]=((char*)p)[6];
    ((char*)(&n))[7]=((char*)p)[7];
}
inline void StoreData(void* p,const uint64_t t)
{
    StoreData(p,(int64_t)t);
}
inline void LoadData(float& n,void* p)
{
    ((char*)(&n))[0]=((char*)p)[0];
    ((char*)(&n))[1]=((char*)p)[1];
    ((char*)(&n))[2]=((char*)p)[2];
    ((char*)(&n))[3]=((char*)p)[3];
}
inline void StoreData(void* p,const float n)
{
    ((char*)p)[0]=((char*)(&n))[0];
    ((char*)p)[1]=((char*)(&n))[1];
    ((char*)p)[2]=((char*)(&n))[2];
    ((char*)p)[3]=((char*)(&n))[3];
}
inline void LoadData(double& n,void* p)
{
    ((char*)(&n))[0]=((char*)p)[0];
    ((char*)(&n))[1]=((char*)p)[1];
    ((char*)(&n))[2]=((char*)p)[2];
    ((char*)(&n))[3]=((char*)p)[3];
    ((char*)(&n))[4]=((char*)p)[4];
    ((char*)(&n))[5]=((char*)p)[5];
    ((char*)(&n))[6]=((char*)p)[6];
    ((char*)(&n))[7]=((char*)p)[7];
}
inline void StoreData(void* p,const double n)
{
    ((char*)p)[0]=((char*)(&n))[0];
    ((char*)p)[1]=((char*)(&n))[1];
    ((char*)p)[2]=((char*)(&n))[2];
    ((char*)p)[3]=((char*)(&n))[3];
    ((char*)p)[4]=((char*)(&n))[4];
    ((char*)p)[5]=((char*)(&n))[5];
    ((char*)p)[6]=((char*)(&n))[6];
    ((char*)p)[7]=((char*)(&n))[7];
}
#endif



//流与各种数据类型的转换,使用之后地址与长度会累加

//网络流中的数据长度类型,如字符串长度不能用size_t,必须是确定的,为了长远考虑固定用64位的uint64_t
typedef uint64_t NetSizeType;


//基于模版实现
template<typename T>
inline size_t StreamPut(char*& pos,size_t& AccLen,const T& n)
{
    //char* pBegin=pos;
    //*((T*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}
template<typename T>
inline size_t StreamGet(char*& pos,size_t& AccLen,T& n)
{
    //char* pBegin=pos;
    //n=*((T*)pos);
    n=LoadData<T>(pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

//不读也不写,仅修改指针偏移,一些特殊情况下需要
inline size_t StreamSkip(char*& pos,size_t& AccLen,size_t skipLen)
{
    pos+=skipLen;
    AccLen+=skipLen;
    return skipLen;
}



//类型数组,模板自动推导长度
template<typename T,size_t N>
inline size_t StreamPut(char*& pos,size_t& AccLen,const T (&arry)[N])
{
    char* pBegin=pos;
     for (size_t n=0;n<N;n++)
     {
         StreamPut(pos,AccLen,arry[n]);
     }
    return pos-pBegin;
}

//类型数组,模板自动推导长度
template<typename T,size_t N>
inline size_t StreamGet(char*& pos,size_t& AccLen,T (&arry)[N])
{
    char* pBegin=pos;
    for (size_t n=0;n<N;n++)
    {
        StreamGet(pos,AccLen,arry[n]);
    }
    return pos-pBegin;
}

//类型数组,手动指定长度
template<typename T>
inline size_t StreamPut(char*& pos,size_t& AccLen,const T* arry,size_t N)
{
    char* pBegin=pos;
    NetSizeType Vsize=N;
    //StreamPut(pos,AccLen,Vsize);   
    for (size_t n=0;n<N;n++)
    {
        StreamPut(pos,AccLen,arry[n]);
    }
    return pos-pBegin;
}

//类型数组,手动指定长度
template<typename T>
inline size_t StreamGet(char*& pos,size_t& AccLen,T* arry,size_t N)
{
    char* pBegin=pos;
    NetSizeType Vsize=N;
    //StreamGet(pos,AccLen,Vsize);
    for (size_t n=0;n<N;n++)
    {
        StreamGet(pos,AccLen,arry[n]);
    }
    return pos-pBegin;
}


//单字节类型数组 为了效率特化版 char /unsigned char/ bool
template<size_t N>
inline size_t StreamPut(char*& pos,size_t& AccLen,const bool (&arry)[N])
{
    char* pBegin=pos;
    //for (size_t n=0;n<N;n++){}
    ::memcpy(pos,arry,N);
    pos+=N;
    AccLen+=N;
    return N;
    //return pos-pBegin;
}

template<size_t N>
inline size_t StreamGet(char*& buf,size_t& AccLen,bool (&arry)[N])
{
    char* pBegin=buf;
    //for (size_t n=0;n<N;n++){}
    ::memcpy(arry,buf,N);
    buf+=N;
    AccLen+=N;
    return N;
    //return pos-pBegin;
}

template<size_t N>
inline size_t StreamPut(char*& pos,size_t& AccLen,const char (&arry)[N])
{
    char* pBegin=pos;
    //for (size_t n=0;n<N;n++){}
    ::memcpy(pos,arry,N);
    pos+=N;
    AccLen+=N;
    return N;
    //return pos-pBegin;
}

template<size_t N>
inline size_t StreamGet(char*& pos,size_t& AccLen,char (&arry)[N])
{
    char* pBegin=pos;
    //for (size_t n=0;n<N;n++){}
    ::memcpy(arry,pos,N);
    pos+=N;
    AccLen+=N;
    return N;
    //return pos-pBegin;
}

template<size_t N>
inline size_t StreamPut(char*& pos,size_t& AccLen,const unsigned char (&arry)[N])
{
    char* pBegin=pos;
    //for (size_t n=0;n<N;n++){}
    ::memcpy(pos,arry,N);
    pos+=N;
    AccLen+=N;
    return N;
    //return pos-pBegin;
}

template<size_t N>
inline size_t StreamGet(char*& pos,size_t& AccLen,unsigned char (&arry)[N])
{
    char* pBegin=pos;
    //for (size_t n=0;n<N;n++){}
    ::memcpy(arry,pos,N);
    pos+=N;
    AccLen+=N;
    return N;
    //return pos-pBegin;
}


//变长字节流的实现
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const char* pData,size_t dataSize)
{
    //char* pBegin=pos;
    ::memcpy(pos,pData,dataSize);
    pos+=dataSize;
    AccLen+=dataSize;
    return dataSize;
    //return pos-pBegin;
}


BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,char* pData,size_t dataSize)
{
    //char* pBegin=pos;
    ::memcpy(pData,pos,dataSize);
    pos+=dataSize;
    AccLen+=dataSize;
    return dataSize;
    //return pos-pBegin;
}


//各种基础数据类型分别实现
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const char& n)
{
    //char* pBegin=pos;
    //*((char*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,char& n)
{
    //char* pBegin=pos;
    //n=*((char*)pos);
    //n=LoadData<char>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}


BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const unsigned char& n)
{
    //char* pBegin=pos;
    //*((unsigned char*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,unsigned char& n)
{
    //char* pBegin=pos;
    //n=*((unsigned char*)pos);
    //n=LoadData<unsigned char>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const short& n)
{
    //char* pBegin=pos;
    //*((short*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,short& n)
{
    //char* pBegin=pos;
    //n=*((short*)pos);
    //n=LoadData<short>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const unsigned short& n)
{
    //char* pBegin=pos;
    //*((unsigned short*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,unsigned short& n)
{
    //char* pBegin=pos;
    //n=*((unsigned short*)pos);
    //n=LoadData<unsigned short>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const int& n)
{
    //char* pBegin=pos;
    //*((int*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,int& n)
{
    //char* pBegin=pos;
    //n=*((int*)pos);
    //n=LoadData<int>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}


BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const unsigned int& n)
{
    //char* pBegin=pos;
    //*((unsigned int*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,unsigned int& n)
{
    //char* pBegin=pos;
    //n=*((unsigned int*)pos);
    //n=LoadData<unsigned int>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const int64_t& n)
{
    //char* pBegin=pos;
    //*((int64_t*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,int64_t& n)
{
    //char* pBegin=pos;
    //n=*((int64_t*)pos);
    //n=LoadData<int64_t>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const uint64_t& n)
{
    //char* pBegin=pos;
    //*((uint64_t*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,uint64_t& n)
{
    //char* pBegin=pos;
    //n=*((uint64_t*)pos);
    //n=LoadData<uint64_t>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const float& n)
{
    //char* pBegin=pos;
    //*((float*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,float& n)
{
    //char* pBegin=pos;
    //n=*((float*)pos);
    //n=LoadData<float>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const double& n)
{
    //char* pBegin=pos;
    //*((double*)pos)=n;
    StoreData(pos,n);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}

BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,double& n)
{
    //char* pBegin=pos;
    //n=*((double*)pos);
    //n=LoadData<double>(pos);
    LoadData(n,pos);
    const size_t typeSize=sizeof(n);
    pos+=typeSize;
    AccLen+=typeSize;
    return typeSize;
    //return pos-pBegin;
}


//把string存放到内存流中，先存放长度再存内容。
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::string& s)
{
    char* pBegin=pos;
    NetSizeType sLen=s.length();
    //*((NetSizeType*)pos)=sLen;
    StoreData(pos,sLen);
    pos+=sizeof(sLen);
    AccLen+=sizeof(sLen);
    memcpy(pos,s.data(),sLen);
    pos+=sLen;
    AccLen+=sLen;
    return pos-pBegin;
}

//从内存流中解析string,先取长度再构造字符串
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,std::string& s)
{
    char* pBegin=pos;
    //NetSizeType sLen=*((NetSizeType*)pos);
    //NetSizeType sLen=LoadData<NetSizeType>(pos);
    NetSizeType sLen;
    LoadData(sLen,pos);
    pos+=sizeof(sLen);
    AccLen+=sizeof(sLen);
    s=std::string(pos,sLen);
    pos+=sLen;
    AccLen+=sLen;
    return pos-pBegin;
}

//wstring在windows与linux内置wchar_t 长度不一致，为了跨平台一致性,转成string后再保存
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::wstring& ws)
{
    char* pBegin=pos;
    std::string s=BS_WStringToString(ws);
    NetSizeType sLen=s.length();
    //*((NetSizeType*)pos)=sLen;
     StoreData(pos,sLen);
    pos+=sizeof(sLen);
    AccLen+=sizeof(sLen);
    ::memcpy(pos,s.data(),sLen);
    pos+=sLen;
    AccLen+=sLen;
    return pos-pBegin;
}

BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,std::wstring& ws)
{
    char* pBegin=pos;
    //NetSizeType sLen=*((NetSizeType*)pos);
    //NetSizeType sLen=LoadData<NetSizeType>(pos);
    NetSizeType sLen;
    LoadData(sLen,pos);

    pos+=sizeof(sLen);
    AccLen+=sizeof(sLen);
    ws=BS_StringToWString(std::string(pos,sLen));
    pos+=sLen;
    AccLen+=sLen;
    return pos-pBegin;
}


//std::vector<std::string> 比较常用在此实现
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::vector<std::string>& Vs)
{
    char* pBegin=pos;
    NetSizeType Vsize=Vs.size();
    StreamPut(pos,AccLen,Vsize);
    for (NetSizeType n=0;n<Vsize;n++)
    {
         StreamPut(pos,AccLen,Vs[n]);  //Vs.at(n)
    }
    return pos-pBegin;
}

BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,std::vector<std::string>& Vs)
{
    char* pBegin=pos;
    NetSizeType Vsize=0;
    Vs.clear();
    StreamGet(pos,AccLen,Vsize);
    for (NetSizeType n=0;n<Vsize;n++)
    {
        std::string stmp;
        StreamGet(pos,AccLen,stmp);
        Vs.push_back(stmp);
    }
    return pos-pBegin;
}


//std::string 定长数组--模板自动推导版
template<size_t N>
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::string (&arry)[N])
{
    char* pBegin=pos;
    const NetSizeType Vsize=N;
    //StreamPut(pos,AccLen,Vsize);
    for (NetSizeType n=0;n<Vsize;n++)   
    {
        StreamPut(pos,AccLen,arry[n]);
    }
    return pos-pBegin;
}
//std::string 定长数组--模板自动推导版
template<size_t N>
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,std::string (&arry)[N])
{
    char* pBegin=pos;
    NetSizeType Vsize=N;                 //Vsize=N; Vsize=0
    //StreamGet(pos,AccLen,Vsize);   //Vsize从流读取更加准确
    for (NetSizeType n=0;n<Vsize;n++)
    {
        StreamGet(pos,AccLen,arry[n]);
    }
    return pos-pBegin;
}
//std::string 定长数组--手动传参版
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::string* pArray,unsigned int N)
{
    char* pBegin=pos;
    NetSizeType Vsize=N;
    StreamPut(pos,AccLen,Vsize);
    for (NetSizeType n=0;n<Vsize;n++)
    {
        StreamPut(pos,AccLen,pArray[n]);
    }
    return pos-pBegin;
}

BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,std::string* pArray,unsigned int N)
{
    char* pBegin=pos;
    NetSizeType Vsize;   //Vsize=N;
    StreamGet(pos,AccLen,Vsize);        //Vsize从流读取更加准确
    for (NetSizeType n=0;n<Vsize;n++)
    {
        StreamGet(pos,AccLen,pArray[n]);
    }
    return pos-pBegin;
}

//std::vector<T> 某种结构类型的vector,比较常用在此实现
template<typename T>
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::vector<T>& Vs)
{
    char* pBegin=pos;
    NetSizeType Vsize=Vs.size();
    StreamPut(pos,AccLen,Vsize);
    for (NetSizeType n=0;n<Vsize;n++)
    {
        StreamPut(pos,AccLen,Vs[n]);  //Vs.at(n)  const T& item=Vs.at(n);
    }
    return pos-pBegin;
}
//std::vector<T> 某种结构类型的vector,比较常用在此实现
template<typename T>
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,std::vector<T>& Vs)
{
    char* pBegin=pos;
    NetSizeType Vsize=0;
    Vs.clear();
    StreamGet(pos,AccLen,Vsize);
    Vs.reserve(Vsize);
    for (NetSizeType n=0;n<Vsize;n++)
    {
        T tmp;
        StreamGet(pos,AccLen,tmp);
        Vs.push_back(tmp);
    }
    return pos-pBegin;
}

//std::list<T> 某种结构类型的list,比较常用在此实现
template<typename T>
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::list<T>& LS)
{
    char* pBegin=pos;
    NetSizeType Vsize=LS.size();
    StreamPut(pos,AccLen,Vsize);
    //typename std::list<T>::const_iterator it;
    const auto it=LS.begin();
    for(it=LS.begin();it!=LS.end();it++)
    {
           const T& item=*it;
           StreamPut(pos,AccLen,item);
    }
    return pos-pBegin;
}
//std::list<T> 某种结构类型的list,比较常用在此实现
template<typename T>
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,std::list<T>& Vs)
{
    char* pBegin=pos;
    NetSizeType Vsize=0;
    Vs.clear();
    StreamGet(pos,AccLen,Vsize);
    for (NetSizeType n=0;n<Vsize;n++)
    {
        T item;
        StreamGet(pos,AccLen,item);
        Vs.push_back(item);
    }
    return pos-pBegin;
}
//其他std容器deque,set/map/multiset/multimap等,在有高频需求时再实现


//std::vector<char> //可以作为二进制字节流 比较常用在此实现
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::vector<char>& Vs)
{
    char* pBegin=pos;
    NetSizeType Vsize=Vs.size();
    StreamPut(pos,AccLen,Vsize);
    memcpy(pos,Vs.data(),Vsize);   //字节内容直接拷贝更高效
    pos+=Vsize;
    AccLen+=Vsize;
    return pos-pBegin;
}
//std::vector<char> //可以作为二进制字节流 比较常用在此实现
BASIC_API inline size_t StreamGet(char*& buf,size_t& AccLen,std::vector<char>& Vs)
{
    char* pBegin=buf;
    NetSizeType Vsize=0;
    Vs.clear();
    StreamGet(buf,AccLen,Vsize);
    Vs.resize(Vsize);
    ::memcpy(Vs.data(),buf,Vsize);  //直接拷贝更高效
    buf+=Vsize;
    AccLen+=Vsize;
    return buf-pBegin;
}

//stringPair //字符串对,比较常用在此实现
//typedef std::pair<std::string,std::string> stringPair;
template<typename T>
BASIC_API inline size_t StreamPut(char*& pos,size_t& AccLen,const std::pair<std::string,std::string>& sPair)
{
    char* pBegin=pos;
    StreamPut(pos,AccLen,sPair.first);
    StreamPut(pos,AccLen,sPair.second);
    return pos-pBegin;
}
//std::list<T> 某种结构类型的list,比较常用在此实现
template<typename T>
BASIC_API inline size_t StreamGet(char*& pos,size_t& AccLen,std::pair<std::string,std::string>& sPair)
{
    char* pBegin=pos;
    StreamGet(pos,AccLen,sPair.first);
    StreamGet(pos,AccLen,sPair.second);
    return pos-pBegin;
}



//计算某种数据类型转换成网络流需要的字节数量
template<typename T>
inline size_t NeedBytesToStream(const T& n)  
{
    return sizeof(n);
}

//类型数组
template<typename T,size_t N>
inline size_t NeedBytesToStream(const T (&arry)[N])  
{
    size_t nbytes=0;
    //nbytes+=sizeof(NetSizeType);   //先存放某种类型定长数组长度
    for (size_t n=0;n<N;n++)            //定长类型数组
    {
        nbytes+=NeedBytesToStream(arry[n]);
    }
    return nbytes;
}

//特殊类型分别实现
inline size_t NeedBytesToStream(const std::string& s)  
{
    return sizeof(NetSizeType)+s.length();
}
inline size_t NeedBytesToStream(const std::wstring& s)  
{
    return sizeof(NetSizeType)+s.length();   //wstring为了兼容性先转成string后才转成流，所以不需要乘以sizeof(wchar_t)
}

//字符串类型数组
template<size_t N>
inline size_t NeedBytesToStream(const std::string (&arry)[N])  
{
    size_t nbytes=0;
    //nbytes+=sizeof(NetSizeType);   //先存放某种类型定长数组长度
    for (size_t n=0;n<N;n++)            //定长类型数组
    {
        nbytes+=NeedBytesToStream(arry[n]);
    }
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const std::wstring (&arry)[N])  
{
    size_t nbytes=0;
    //nbytes+=sizeof(NetSizeType);   //先存放某种类型定长数组长度
    for (size_t n=0;n<N;n++)            //定长类型数组
    {
        nbytes+=NeedBytesToStream(arry[n]);
    }
    return nbytes;
}


template<typename T>
inline size_t NeedBytesToStream(const std::vector<T> Vs)  
{
    size_t nbytes=0;
    nbytes+=sizeof(NetSizeType);   //先存放某种类型定长数组长度
    for (size_t n=0;n<Vs.size();n++)
    {
        nbytes+=NeedBytesToStream(Vs[n]); //.at(n)
    }
    return nbytes;
}

template<typename T>
inline size_t NeedBytesToStream(const std::list<T> LS)  
{
    size_t nbytes=0;
    nbytes+=sizeof(NetSizeType);   //先存放某种类型个数
    //typename std::list<T>::const_iterator it;
    const auto it=LS.begin();
    for(it=LS.begin();it!=LS.end();it++)
    {
        const T& item=*it;
        nbytes+=NeedBytesToStream(item);
    }
    return nbytes;
}

inline size_t NeedBytesToStream(const std::pair<std::string,std::string>& sPair)  
{
    return NeedBytesToStream(sPair.first)+NeedBytesToStream(sPair.second);
}

//基础类型数组特化
template<size_t N>
inline size_t NeedBytesToStream(const char (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const unsigned char (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const bool (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}

template<size_t N>
inline size_t NeedBytesToStream(const short (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const unsigned short (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const int (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const unsigned int (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const int64_t (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const uint64_t (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const float (&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
template<size_t N>
inline size_t NeedBytesToStream(const double(&arry)[N])  
{
    const size_t nbytes=N*sizeof(arry[0]);
    return nbytes;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//网络字节序转换等基础操作
//BASIC_API uint16_t bswap_16(uint16_t x);     //反转字节顺序16bit   
//BASIC_API uint32_t bswap_32(uint32_t x);     //反转字节顺序32bit
//BASIC_API uint64_t bswap_64(uint64_t x);     //反转字节顺序64bit


//大小端判断的几种方法:记忆方法大端符合正常书写顺序,地址从左到有。
//int a = 0x12345678;  
//char * p = (char*)&a;  
//if (*p==0x12)  
//{  
//    return true;     //big-endian
//}  
//return false;       //little-endian

//Linux 的内核神级代码kernel/arch/arm/kernel/setup.c
/*
static union 
{ 
    char c[4]; 
    unsigned long mylong; 
}endian_test={{'l','?','?','b' } };

#define ENDIANNESS ((char)endian_test.mylong);
仅仅用一个union 变量和一个简单的宏定义就实现了一大段代码同样的功能！
(如果ENDIANNESS='l'表示系统为little endian,为'b'表示big endian )
*/

const int ByteOrderCheckValue=0X12345678;
const char* const pBOC=(const char*)&ByteOrderCheckValue;
const bool islittle_constValue=(pBOC[0]==0X78);
const bool isbig_constValue=(pBOC[0]==0X12);
//fastest create by lizulin 
BASIC_API inline bool isLittleEndian()
{
     return islittle_constValue;
     //return pBOC[0]==0X78;
}

BASIC_API inline bool isBigEndian() 
{
    return isbig_constValue;
     //return pBOC[0]==0X12;
}

BASIC_API inline bool isLittleEndianV1() //判断当前主机字节序是否小端
{
    //return !isBigEndian();
    //faster
    union StuTmp
    {
        int a;
        char b;
    };
    StuTmp tmp;
    tmp.a=0x12345678;
    return (tmp.b==0x78);
}

BASIC_API inline bool isBigEndianV1()  //判断当前主机字节序是否大端
{
    union StuTmp
    {
        int a;
        char b;
    };
    StuTmp tmp;
    tmp.a=0x12345678;
    return (tmp.b==0x12);
}  




//低效实现废弃
//BASIC_API void ReverseByteOrder(void* buf,unsigned int TypeSize);   //反转字节顺序 通用函数适用于16,32,64位
//BASIC_API void CastTypeHostToNet(void* buf,unsigned int TypeSize);//把某类型数据(根据类型长度)本机格式转成网络大端
//BASIC_API void CastTypeNetToHost(void* buf,unsigned int TypeSize);//把某类型数据(根据类型长度)从网络大端转成本机格式





////本机字节序与网络字节序之间转换， Net表示网络 Host表示主机
BASIC_API inline uint16_t ByteOrderSwap16(uint16_t x)
{
    //return (((uint16_t)(x)&0x00FF)<<8)|(((uint16_t)(x)&0xFF00)>>8);
    //return ((x<<8)|((x&0xFF00)>>8));  //faster
    return ((x<<8)|(x>>8));  //faster2
}

BASIC_API inline uint32_t ByteOrderSwap32(uint32_t x)
{
    return (((uint32_t)(x)&0xFF000000)>>24)|\
              (((uint32_t)(x)&0x00FF0000)>>8)|\
              (((uint32_t)(x)&0x0000FF00)<<8)|\
              (((uint32_t)(x)&0x000000FF)<<24);
}

BASIC_API inline uint64_t ByteOrderSwap64(uint64_t x)
{
    return  (((uint64_t)x&0xFF00000000000000LL)>>56|\
                ((uint64_t)x&0x00FF000000000000LL)>>40|\
               ((uint64_t)x&0x0000FF0000000000LL)>>24|\
               ((uint64_t)x&0x000000FF00000000LL)>>8|\
               ((uint64_t)x&0x00000000FF000000LL)<<8|\
              ((uint64_t)x&0x0000000000FF0000LL)<<24|\
              ((uint64_t)x&0x000000000000FF00LL)<<40|\
              ((uint64_t)x&0x00000000000000FFLL)<<56);
}

//高效实现 快15倍
//ByteOrderSwap有汇编指令
//OSSwapInt64 (value);            //MAC ||IOS 
//_byteswap_uint64 (value);     //MSVC
extern uint16_t ByteOrderSwap16Bit(uint16_t n);
extern uint32_t ByteOrderSwap32Bit(uint32_t n);
extern uint64_t ByteOrderSwap64Bit(uint16_t n);


//#if defined(_MSC_VER)||defined(OS_LINUX)
//#define  ByteOrderSwap16 ByteOrderSwap16Bit
//#define  ByteOrderSwap32 ByteOrderSwap32Bit
//#define  ByteOrderSwap64 ByteOrderSwap64Bit
//#endif



//浮点数的字节序转换

BASIC_API inline float ByteOrderSwapFloat(float x)
{
    //方法1
    //uint32_t n=ByteOrderSwap32(*((uint32_t*)&x));
    //return (*(float*)(&n));

    //方法二:same speed
    union float_conv
    {
        float f;
        uint32_t n;
    };
    float_conv tt; 
    tt.f=x;
    tt.n=ByteOrderSwap32(tt.n);
    return tt.f;
}

BASIC_API inline double ByteOrderSwapDouble(double x)
{
    //方法1
    //uint64_t n=ByteOrderSwap64(*((uint64_t*)&x));
    //return (*(double*)(&n));

    union double_conv
    {
        double f;
        uint64_t n;
    };
    double_conv tt;
    tt.f=x;
    tt.n=ByteOrderSwap64(tt.n);
    return tt.f;
}


//float BLEndianFloat(float value)   //直接转换是否更高效?  经测，反而更慢
//{
//        union float_conv
//        {
//            float f;
//            char c[4];
//        };
//
//    float_conv d1,d2;
//    d1.f = value;
//    d2.c[0] = d1.c[3];
//    d2.c[1] = d1.c[2];
//    d2.c[2] = d1.c[1];
//    d2.c[3] = d1.c[0];
//    return d2.f;
//}


//Int 24
BASIC_API inline void ByteOrderSwap24(char* p)
{
    char tmp=*p;
    *(p)=*(p+2);
    *(p+2)=tmp;
}
//BASIC_API inline void ByteOrderSwap24(Int24 p){}//待续


//
////Net到host 网络到本机的转换
//BASIC_API inline int16_t NettoHost_int16(int16_t n)
//{
//    if(isBigEndian())
//        return n;
//    return ByteOrderSwap16(n);
//}
//BASIC_API inline uint16_t NettoHost_uint16(uint16_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap16(n);
//}
//
//BASIC_API inline int32_t NettoHost_int32(int32_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap32(n);
//}
//BASIC_API inline uint32_t NettoHost_uint32(uint32_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap32(n);
//}
//BASIC_API inline int64_t NettoHost_int64(int64_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap64(n);
//}
//BASIC_API inline uint64_t NettoHost_uint64(uint64_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap64(n);
//}
//BASIC_API inline float NettoHost_float(float Val)
//{
//    if(isBigEndian()) 
//        return Val;
//    return ByteOrderSwapFloat(Val);
//}
//
//BASIC_API inline double NettoHost_double(double Val)
//{
//    if(isBigEndian()) 
//        return Val;
//    return ByteOrderSwapDouble(Val);
//}
//
////host到Net 本机到网络的转换
//BASIC_API inline int16_t HosttoNet_int16(int16_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap16(n);
//}
//BASIC_API inline uint16_t HosttoNet_uint16(uint16_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap16(n);
//}
//BASIC_API inline int32_t HosttoNet_int32(int32_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap32(n);
//}
//BASIC_API inline uint32_t HosttoNet_uint32(uint32_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap32(n);
//}
//BASIC_API inline int64_t HosttoNet_int64(int64_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap64(n);
//}
//BASIC_API inline uint64_t HosttoNet_uint64(uint64_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap64(n);
//}
//BASIC_API inline float HosttoNet_float(float Val)
//{
//    if(isBigEndian()) 
//        return Val;
//    return ByteOrderSwapFloat(Val);
//}
//
//BASIC_API inline double HosttoNet_double(double Val)
//{
//    if(isBigEndian()) 
//        return Val;
//    return ByteOrderSwapDouble(Val);
//}


//一些标准规范会有统一字节序到大端或小端。

//Big到host 大端到本机的转换
//因为网络字节序固定是大端的，直接复用
//
//BASIC_API inline int16_t BigtoHost_int16(int16_t n)
//{
//    return NettoHost_int16(n);
//}
//
//BASIC_API inline int32_t BigtoHost_int32(int32_t n)
//{
//    return NettoHost_int32(n);
//}
//
//BASIC_API inline int64_t BigtoHost_int64(int64_t n)
//{
//    return NettoHost_int64(n);
//}
//
//BASIC_API inline float BigtoHost_float(float Val)
//{
//    return NettoHost_float(Val);
//}
//
//BASIC_API inline double BigtoHost_double(double Val)
//{
//    return NettoHost_double(Val);
//}
//
////host到Big 本机到大端的转换
//BASIC_API inline int16_t HosttoBig_int16(int16_t n)
//{
//    return HosttoNet_int16(n);
//}
//BASIC_API inline uint16_t HosttoBig_uint16(uint16_t n)
//{
//    return HosttoNet_uint16(n);
//}
//BASIC_API inline int32_t HosttoBig_int32(int32_t n)
//{
//    return HosttoNet_int32(n);
//}
//
//BASIC_API inline int64_t HosttoBig_int64(int64_t n)
//{
//    return HosttoNet_int64(n);
//}
//
//BASIC_API inline float HosttoBig_float(float Val)
//{
//    return HosttoNet_float(Val);
//}
//
//BASIC_API inline double HosttoBig_double(double Val)
//{
//    return HosttoNet_double(Val);
//}
//
////Little到Host 小端到本机的转换
//BASIC_API inline int16_t LittletoHost_int16(int16_t n)
//{
//    if(isLittleEndian()) 
//        return n;
//    return ByteOrderSwap16(n);
//}
//
//BASIC_API inline int32_t LittletoHost_int32(int32_t n)
//{
//    if(isLittleEndian()) 
//        return n;
//    return ByteOrderSwap32(n);
//}
//
//BASIC_API inline int64_t LittletoHost_int64(int64_t n)
//{
//    if(isLittleEndian()) 
//        return n;
//    return ByteOrderSwap64(n);
//}
//
//BASIC_API inline float LittletoHost_float(float Val)
//{
//    if(isLittleEndian()) 
//        return Val;
//    return ByteOrderSwapFloat(Val);
//}
//
//BASIC_API inline double LittletoHost_double(double Val)
//{
//    if(isLittleEndian()) 
//        return Val;
//    return ByteOrderSwapDouble(Val);
//}
//
////Host到Little 本机到小端的转换
//BASIC_API inline int16_t HosttoLittle_int16(int16_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap16(n);
//}
//
//BASIC_API inline int32_t HosttoLittle_int32(int32_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap32(n);
//}
//
//BASIC_API inline int64_t HosttoLittle_int64(int64_t n)
//{
//    if(isBigEndian()) 
//        return n;
//    return ByteOrderSwap64(n);
//}
//
//BASIC_API inline float HosttoLittle_float(float Val)
//{
//    if(isBigEndian()) 
//        return Val;
//    return ByteOrderSwapFloat(Val);
//}
//
//BASIC_API inline double HosttoLittle_double(double Val)
//{
//    if(isBigEndian()) 
//        return Val;
//    return ByteOrderSwapDouble(Val);
//}



//C++ 同名重装:便于模板函数使用
BASIC_API inline uint16_t ByteOrderSwap(uint16_t x)
{
    return ByteOrderSwap16(x);
}
BASIC_API inline int16_t ByteOrderSwap(int16_t x)
{
    return ByteOrderSwap16(x);
}
BASIC_API inline uint32_t ByteOrderSwap(uint32_t x)
{
    return ByteOrderSwap32(x);
}
BASIC_API inline int32_t ByteOrderSwap(int32_t x)
{
    return ByteOrderSwap32(x);
}
BASIC_API inline uint64_t ByteOrderSwap(uint64_t x)
{
    return ByteOrderSwap64(x);
}

//gcc error
#ifndef _WIN32
BASIC_API inline uint64_t ByteOrderSwap(long long int x)
{
    return ByteOrderSwap64(x);
}

BASIC_API inline uint64_t ByteOrderSwap(long long unsigned int x)
{
    return ByteOrderSwap64(x);
}
#endif


BASIC_API inline int64_t ByteOrderSwap(int64_t x)
{
    return ByteOrderSwap64(x);
}
BASIC_API inline float ByteOrderSwap(float x)
{
    return ByteOrderSwapFloat(x);
}
BASIC_API inline double ByteOrderSwap(double x)
{
    return ByteOrderSwapDouble(x);
}



//Net到host 网络到本机的转换
template<typename T>
BASIC_API inline T NetToHost(T n)
{
    if(isBigEndian()) 
        return n;
    return ByteOrderSwap(n);
}

//host到Net 本机到网络的转换
template<typename T>
BASIC_API inline T HostToNet(T n)
{
    if(isBigEndian()) 
        return n;
    return ByteOrderSwap(n);
}

//一些标准规范会有统一字节序到大端或小端。
//Big-->Host
template<typename T>
BASIC_API inline T BigToHost(T n)
{
    if(isBigEndian()) 
        return n;
    return ByteOrderSwap(n);
}
//Host-->Big
template<typename T>
BASIC_API inline T HostToBig(T n)
{
    if(isBigEndian()) 
        return n;
    return ByteOrderSwap(n);
}

//Litttle-->Host
template<typename T>
BASIC_API inline T LitttleToHost(T n)
{
    if(isLittleEndian()) 
        return n;
    return ByteOrderSwap(n);
}
//Host-->Little
template<typename T>
BASIC_API inline T HostToLitttle(T n)
{
    if(isLittleEndian()) 
        return n;
    return ByteOrderSwap(n);
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//时码与帧号的互转,放到与项目无关基础操作，其他音视频相关项目会用到


//帧号转时间字符串
void TimeCodeNumtoString(unsigned int FrameNum,unsigned int FRate,char sTimeCode[64]);
//时间字符串转帧号--返回是否成功，字符串不符合格式返回false
bool TimeCodeStringtoNum(const char sTimeCode[64],unsigned int FRate,unsigned int& FrameNum);
//以毫秒为单位的时间转时码字符串
void TimeInMstoTimeCodeString(unsigned int tImeInms,unsigned int FRate,char sTimeCode[64]);
//为了便于使用再封装与string 互转的 
unsigned int TimeCodeStringtoNum(std::string strTimeCode,unsigned int FRate,bool* isOK=0);  //可能失败 字符串不符合格式 是否成功返回在isOK里面
std::string TimeCodeNumtoString(unsigned int FrameNum,unsigned int FRate);                           //肯定成功

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////










////////////////////////////////////////////////////////////////////////////////


#if 0
没有躲过的坑--wstring与string的转换
在实际工程中，我们往往需要把string转换为wstring，你可以会进行百度或是Google，
很轻松找到转换的方法。但是这里就隐含着巨大的坑儿。 
看看这个转换吧：
std::wstring WStringToWString(const std::string& str) 
{
        int size_str = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
        wchar_t *wide = new wchar_t[size_str];
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, size_str);
        std::wstring return_wstring(wide);
        delete[] wide;
        wide = NULL;
        return return_wstring;
}
也许上面的代码帮你解决了问题，但是暂时的。当str = “我是中文”的时候，你会发现，无法完成正确的转换，导致乱码。
因此，为了增加你程序的健壮性，还是寻找另一种方法吧，如下：
#endif




#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

// 格式串垫片函数模版
template <const int iBufLen> inline const char * FormatString( const char *szFormat, ... )
{
	static char szOutStr[iBufLen];
	szOutStr[0] = 0;
	va_list vl;

	va_start( vl, szFormat );
	vsprintf(szOutStr, szFormat, vl);
	va_end(vl);

	assert( strlen(szOutStr) < iBufLen && "Critical Damage" );

	return szOutStr;
}
// 给出一个常用的缓冲大小为1024字节的宏
#define FSTR FormatString<1024>
// 用法:
//  OutputDebugString( FSTR( "Format Out %s", "ok" ) );



//从日期宏定义取日期字符串作为程序版本号--几种实现方式
time_t getDateFromMacro(char const *date);
void getDateMacrotoString(char buf[16]);
std::string getDateMacrotoString();



#ifdef UNIT_TEST
BASIC_API  int Test_MyBasicOperation();
#endif



#endif

