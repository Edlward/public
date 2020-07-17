#include "CopyFileByThread.h"

#include "../Basic/BasicOperation.h"
//#include "./Basic/Log.h"

#include <vector>
#include <stdio.h>



long long scan_dir(std::vector<std::string>& ListDirFile,const std::string& dir,FilterFun pFilterFun=NULL)   //
{
    long long totalDirFilesize=0;
    ListDirFile.push_back(dir);
    std::vector<std::string> ListFile=getFilePath(dir.c_str());
    for (size_t k=0;k<ListFile.size();k++)
    {
        const std::string& sCurrPath=ListFile.at(k);
        if(isDirectory(sCurrPath))
        {
            totalDirFilesize += scan_dir(ListDirFile,sCurrPath,pFilterFun);
        }
        else
        {
            //用条件过滤,rafactor by lizulin 20190220
            if (pFilterFun)     //过滤条件函数非空
            {
                if (pFilterFun(sCurrPath.c_str()))
                {
                    ListDirFile.push_back(sCurrPath);
                    totalDirFilesize +=getFileSize(sCurrPath);
                }
            }
            else
            {
                ListDirFile.push_back(sCurrPath);
                totalDirFilesize +=getFileSize(sCurrPath);
            }
        }
    }
   return  totalDirFilesize;
}  


static uint32_t hash_times33(const char* buf,size_t len)
{
    unsigned int nHash=0;
    const char* pos=buf;
    for (size_t n=0;n<len;n++)
    {
        nHash=(nHash<<5)+nHash+(*pos++);
    }
    return nHash;
}

bool cpfile(const char *path_dst,const char *path_src,CopyHelp& copyHelp)
{
    if (!copyHelp.CreateIOBuffer())
    {
        PRINT_ERROR_LINE
        return false;
    }
    char* pBufIO=copyHelp.pBufIO;

    FILE* FpSrc=fopen(path_src,"rb");
    if(FpSrc==NULL)
    {
        printf("cpfile(): fopen() failed! Src path:%s\n",path_src);
        return false;
    }
    FILE* FpDst=fopen(path_dst,"wb");
    if(FpDst==NULL)
    {
        printf("cpfile(): fopen() failed! Dst path:%s\n",path_dst);
        fclose(FpSrc);
        return false;
    }
    copyHelp.currFileSize=getFileSize(path_src);
    if (copyHelp.currFileSize==0)
    {
        return true;
    }
    copyHelp.currFileCopied=0;

    std::string sFileName=getFileNameFromPath(path_src);
    memcpy(copyHelp.sCurrCopyingFileName,sFileName.c_str(),sFileName.size()+1);  //'\0'
    //copyHelp.FileNameLen=sFileName.size()+1;
    copyHelp.FileNameLen=sFileName.size();
    //copyHelp.FileNameID=(int)FpSrc;  //用字符串校验或文件指针或其他方法唯一标识文件名
    copyHelp.FileNameID=hash_times33(sFileName.c_str(),sFileName.size());

    size_t nread;
    while((nread=fread(pBufIO,1,CopyHelp::BUFFER_SIZE,FpSrc))>0)
    {
        if(fwrite(pBufIO,1,nread,FpDst)!=nread)
        {
            PreDprintf("failed! fwrite() error! path:%s\n",path_dst);
            fclose(FpSrc);
            fclose(FpDst);

            //操作失败,函数指针非空则调用
            if (copyHelp.pFunCallAtCopying!=NULL)
            {
                copyHelp.pFunCallAtCopying(copyHelp.parameterofCopyingFun);
            }

           //为了避免文件残缺不完整导致各种解析崩溃,操作失败或中途退出,删除正在写入未完成的文件
           bool retDel=deleteFile(path_dst);

            return false;
        }
        else
        {
            //当前拷贝文件百分比
            copyHelp.currFileCopied+=nread;
            if (copyHelp.currFileSize!=0)
            {
                  copyHelp.currFileCopyPercent=100.0*copyHelp.currFileCopied/copyHelp.currFileSize;
            }

            copyHelp.totalCopied+=nread;
            if(copyHelp.totalCopied>=copyHelp.totalAllFileSize)   // ==1
            {
                printf("complete_size= %lld \n",copyHelp.totalCopied);       
                printf("totalsize= %lld\n",copyHelp.totalAllFileSize);                       //
                printf("Copy file to %s ......\n",path_dst);
            }
            if(copyHelp.totalAllFileSize!=0)
            {
                 copyHelp.AllCopyPercent=100.0*copyHelp.totalCopied/copyHelp.totalAllFileSize;
            }


            //拷贝过程被外部控制停止
             if (*copyHelp.bContinue==false)
             {
                 fclose(FpSrc);
                 fclose(FpDst);
                 if (copyHelp.currFileCopied==copyHelp.currFileSize)   //中止时已经拷贝完成则返回成功
                 {
                     return true;
                 }
                 else
                 {
                     //为了避免文件残缺不完整导致各种解析崩溃,操作失败或中途退出,删除正在写入未完成的文件
                     bool retDel=deleteFile(path_dst);

                     return false;
                 }
             }

             //拷贝过程被外部控制暂停
             while(*copyHelp.bPause)
             {
                 Thread::sleep_for(500);
             }

             //拷贝进度发生变化再调用
#if 1
             //if (copyHelp.lastAllCopyPercent!=copyHelp.AllCopyPercent)
             if (copyHelp.lastcurrFileCopyPercent!=copyHelp.currFileCopyPercent)
             {
                 //函数指针非空则调用
                 if (copyHelp.pFunCallAtCopying!=NULL)
                 {
                     copyHelp.pFunCallAtCopying(copyHelp.parameterofCopyingFun);
                 }
                 //copyHelp.lastAllCopyPercent=copyHelp.AllCopyPercent;
                 copyHelp.lastcurrFileCopyPercent=copyHelp.currFileCopyPercent;
             }
#endif

        }
    }
    fclose(FpSrc);
    fclose(FpDst);


    //拷贝完成,函数指针非空则调用
    if (copyHelp.pFunCallAtCopying!=NULL)
    {
        copyHelp.pFunCallAtCopying(copyHelp.parameterofCopyingFun);
    }

    return true;
}

//target_dir目的路径,ListSrcFile源路径文件列表,src_root原路径的共同根目录
bool cpdir(const char *target_dir,std::vector<std::string> &ListSrcFile,const char* src_root,CopyHelp& copyHelp)
{

    if (isNullCstr(target_dir))
    {
        PreDprintf("failed target_dir is null!\n");
        return false;
    }
    PreDprintf("dst path:%s src_root:%s\n",target_dir,src_root);


    for(size_t n=0;n<ListSrcFile.size();n++)
    {
        const std::string& pathSrc=ListSrcFile[n];
        std::string sSubPath;
        size_t pos;
        if((pos=pathSrc.find(src_root))!=std::string::npos)
        {
            sSubPath=pathSrc.substr(pos+strlen(src_root));
        }
        std::string pathDst=target_dir;
        pathDst+="/";
        pathDst+=sSubPath;
        pathDst=FormatPath(pathDst);
        //创建源路径目录结构
        if(isDirectory(pathSrc))
        {
            //continue;
            //保持原路径目录结构
            std::string sPathDstParentDir=getDirFromFullPath(pathDst);
            if (!createDirectory(sPathDstParentDir))
            {
                continue;
            }
            continue;
        }

        printf("PathPair[%llu]:\n",n);
        printf("Dst path:%s\n",pathDst.c_str());
        printf("Src path:%s\n",pathSrc.c_str());

        bool ret=cpfile(pathDst.c_str(),pathSrc.c_str(),copyHelp);
        if (!ret)
        {
            //copyHelp.Status=CopyStatus_CopyFailed;
            return false;
        }
        //为了不破坏文件原始属性,保持原文件的创建日期,修改日期不变
        bool retSet=copyTimeAttributes(pathDst.c_str(),pathSrc.c_str());
        if (!retSet)
        {
            PRINT_MAY_ERROR_LINE
        }
        //校验文件是否拷贝错误--非必须   //add by lizulin 20190819
#if 1
        if (!CompareFileCheck(pathDst.c_str(),pathSrc.c_str()))
        {
            PRINT_MAY_ERROR_LINE
            copyHelp.Status=CopyStatus_CopyFailed;

            //为了避免文件残缺不完整导致各种解析崩溃,文件拷贝前后校验不一致,则删除
            bool retDel=deleteFile(pathDst);

            return false;
        }
#endif
    }
    return true;


//#if 0
//
//    std::string pathDst=target_dir;
//    for(size_t n = 0; n < ListSrcFile.size(); n++)
//    {
//        const std::string& pathSrc=ListSrcFile[n];
//        if(isDirectory(pathSrc))
//        {
//#ifdef _WIN32
//            pathDst+="\\";
//#else
//            pathDst+="/";
//#endif
//
//            pathDst+=getFileNameFromPath(pathSrc);
//            pathDst=FormatPath(pathDst);
//
//            if (!createDirectory(pathDst))
//            {
//                ::printf("fs::create_directory failed path:%s\n",pathDst.c_str());
//                PRINT_FUN_FAILED
//                    return false;
//            }
//        }
//        else
//        {
//#ifdef _WIN32
//           pathDst+="\\";
//#else
//           pathDst+="/";
//#endif // _WIN32
//           pathDst+=getFileNameFromPath(pathSrc);
//           pathDst=FormatPath(pathDst);
//
//            bool ret=cpfile(pathDst.c_str(),pathSrc.c_str(),copyHelp);
//            if (!ret)
//            {
//                //copyHelp.Status=CopyStatus_CopyFailed;
//                return false;
//            }
//            //为了不破坏文件原始属性,保持原文件的创建日期,修改日期不变
//            bool retSet=copyTimeAttributes(pathDst.c_str(),pathSrc.c_str());
//            if (!retSet)
//            {
//                PRINT_MAY_ERROR_LINE
//            }
//            //校验文件是否拷贝错误--非必须   //add by lizulin 20190819
//#if 1
//            if (!CompareFileCheck(pathDst.c_str(),pathSrc.c_str()))
//            {
//                PRINT_MAY_ERROR_LINE
//                    copyHelp.Status=CopyStatus_CopyFailed;
//                return false;
//            }
//#endif
//        }
//    }
//    return true;
//#endif
}


void CopyFileByThread::run()
{
         //便于线程控制可以随时结束拷贝
         volatile bool& bThreadContinue=Thread::getContinueRunRef();
         this->copyHelp.bContinue=&bThreadContinue;
         this->copyHelp.bPause=&bPause;

         PRINT_FUN_TRACE
         bool retCio=this->copyHelp.CreateIOBuffer();
         if (!retCio)
         {
             PRINT_ERROR_LINE
             return;
         }

         CallBeforeCopy();
         //copy_dir(this->sDst.c_str(),this->sSrc.c_str(),this->copyHelp);
         std::vector<pathPair> ListCopyPathUse=this->ListCopyPath;
         for (size_t n=0;n<ListCopyPathUse.size();n++)
         { 
             const pathPair& tmp=ListCopyPathUse.at(n);
              copy_dir(tmp.sDst.c_str(),tmp.sSrc.c_str(),this->copyHelp);
         }
         CallAfterCopyFinish();
         this->copyHelp.ReleaseIOBuffer();
}


void CopyFileByThread::copy_dir(const char *dir_dst,const char *dir_src,CopyHelp& copyHelp)
{
    copyHelp.Status=CopyStatus_isCopying;
    copyHelp.totalAllFileSize=0;
    copyHelp.totalCopied=0;
    printf("CopyThread::copy_dir() Path dst:%s src:%s\n",dir_dst,dir_src);  
    std::vector<std::string> ListSrcFile;

    copyHelp.totalAllFileSize= scan_dir(ListSrcFile,dir_src,this->dirDcpCopyFilter);

    printf("CopyThread::copy_dir(): toCopying Dir totalsize= %lld\n",copyHelp.totalAllFileSize);  
    if(copyHelp.totalAllFileSize==0)  
    {
        copyHelp.Status=CopyStatus_CopyFinish;
        return;
    }
    printf("CopyThread::copy_dir(): Begin copy Dir ......\n");
    bool ret=cpdir(dir_dst,ListSrcFile,dir_src,copyHelp);
    if (!ret)
    {
            printf("CopyThread::copy_dir(): failed!\n");
            copyHelp.Status=CopyStatus_CopyFailed;
            return;
    }

    printf("CopyThread::copy_dir():Copy end!\n");
    copyHelp.Status=CopyStatus_CopyFinish;
    return;
}
