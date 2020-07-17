#if 0
/******************************************************************************
*File	: File.h
*Author	: Lan Hu
*Email	: Hi_Blue@126.com
*Notes	:
*Licence: 
*Created: 2009-08-05
*******************************************************************************/

#ifndef _MYLIB_FILE_H__INLCLUED_20090805_
#define _MYLIB_FILE_H__INLCLUED_20090805_

#include <string>
#include <stdarg.h>

#include "public.h"
#include "Thread.h"

M_BEGIN_NAMESPACE(mylib)

#define M_MAX_FILE_NAME_LEN	128
#define M_DEFAULT_BUFF_LEN	512
#define M_MAX_BUFFER_SIZE	10240


#define M_SZ_READ_T_MODE	"rt"
#define M_SZ_WRITE_T_MODE	"wt"
#define M_SZ_APPEND_T_MODE	"at"
#define M_SZ_RW_T_MODE		"wt+"
#define M_SZ_RA_T_MODE		"at+"
#define M_SZ_READ_B_MODE	"rb"
#define M_SZ_WRITE_B_MODE	"wb"
#define M_SZ_APPEND_B_MODE	"ab"
#define M_SZ_RW_B_MODE		"wb+"
#define M_SZ_RA_B_MODE		"ab+"


typedef struct FileAttribute
{
	enum E_FILE_TYPE{E_NOT_EXIST = -1,E_TYPE_UNKNOWN = 0, E_TYPE_FILE = 1 ,E_TYPE_DIR = 2 };
	byte	type;
	size_t	length;
	uint64_t  lastModify;
	uint64_t  lastAccess;
	string	name;
}T_FILE_ATTR;

class File
{
	
public:
	typedef FILE T_FILE;
	typedef T_FILE_ATTR T_ATTR;

	enum FILE_STATE{E_UNINITIALIZED,E_INITIALIZED,E_FILE_ERROR,E_FILE_OPENED};
	
	enum OPEN_FLAGS
	{
		E_MODE_READ		=	(uint32_t)0x00000001,
		E_MODE_APPEND	=	(uint32_t)0x00000010,
		E_MODE_WRITE	=	(uint32_t)0x00000100,
		E_TYPE_TEXT		=	(uint32_t)0x00001000,
		E_TYPE_BINARY	=	(uint32_t)0x00010000
	};

	enum WHENCE_START
	{
		E_BEGIN_FILE	= SEEK_SET,
		E_CURR_POS		= SEEK_CUR,
		E_END_FILE		= SEEK_END
	};

	enum {E_BUFFER_SIZE = 1024};
	enum {E_END_OF_FILE = EOF };
	
private:
	string m_Name;
// 	char * m_Buffer;
// 	int m_BufferSzie;
	bool m_Exist;
	char m_State;
	int m_Mode;
	int m_ErrCode;
	ThreadMutex * m_Mutex;
	T_FILE * m_File;
	T_ATTR * m_Attr;
	
public:
	File(char szName[]):m_File(NULL),m_Attr(NULL)
	{
		assert(szName != NULL);
		m_Name = szName;
		m_Mutex = new ThreadMutex;
		assert(m_Mutex);
// 		m_BufferSzie = DEFAULT_BUFF_LEN;
// 		m_Buffer= new char[m_BufferSzie];
//		assert(m_Buffer);

		m_State = E_INITIALIZED;
	}

	File(const string & name):m_File(NULL),m_Attr(NULL)
	{
		assert(name.length()>0);
		m_Name = name;
		m_Mutex = new ThreadMutex;
		assert(m_Mutex);
// 		m_BufferSzie = DEFAULT_BUFF_LEN;
// 		m_Buffer= new char[m_BufferSzie];
//		assert(m_Buffer);
		m_State = E_INITIALIZED;
	}

	File():m_File(NULL),m_Attr(NULL)
	{
		m_Mutex = new ThreadMutex;
		assert(m_Mutex);
		m_State = E_INITIALIZED;
	}

	~File()
	{
		m_Mutex->lock();
		if (E_FILE_OPENED == m_State)
		{
			M_WARNING("FILE[%s] NOT CLOSED.",m_Name.c_str());
			//::fclose(m_File);
		}
		if (m_Attr != NULL)
		{
			delete m_Attr;
			m_Attr = NULL;
		}
		
/*		delete m_Buffer;*/
		m_Mutex->unlock();
		delete m_Mutex;
		
	}

	bool getAttribute(T_ATTR * attr)
	{
		assert(attr != NULL);
		m_Mutex->lock();
		bool bReturn  = false;
		if (m_Attr != NULL)
		{
			bReturn = true;
			attr->lastAccess = m_Attr->lastAccess;
			attr->lastModify = m_Attr->lastModify;
			attr->length = m_Attr->length;
			attr->name = m_Attr->name;
			attr->type = m_Attr->type;
		}
		else
		{
			m_Attr = new T_ATTR;
			if (m_Attr)
			{
				bReturn = GetAttribute(m_Name,m_Attr);
				if (bReturn)
				{
					attr->lastAccess = m_Attr->lastAccess;
					attr->lastModify = m_Attr->lastModify;
					attr->length = m_Attr->length;
					attr->name = m_Attr->name;
					attr->type = m_Attr->type;
				}
			}
			else
			{
				M_WARNING("NO MORE MEMORY.");
			}
		}
		m_Mutex->unlock();
		return bReturn;
	}

	static bool GetAttribute(char szName[],T_ATTR * attr)
	{
		assert(attr != NULL);
		assert(szName != NULL);
		bool bReturn = false;

#if(M_OS_TYPE == M_WINDOWS_OS)
		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile( szName, &data);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			bReturn = true;
			attr->name = szName;
			if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
			{
				attr->type = T_ATTR::E_TYPE_DIR;
			}
			else if(data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
			{
				attr->type = T_ATTR::E_TYPE_FILE;
			}
			else
			{
				attr->type = T_ATTR::E_TYPE_UNKNOWN;
			}
			attr->length = data.nFileSizeLow;
			attr->lastModify = ((uint64_t)data.ftLastWriteTime.dwLowDateTime |
				((uint64_t)data.ftLastWriteTime.dwHighDateTime << 32)) / (10000) - (11644473600000);
			attr->lastAccess = ((uint64_t)data.ftLastAccessTime.dwLowDateTime |
				((uint64_t)data.ftLastAccessTime.dwHighDateTime << 32)) / (10000) - (11644473600000);
			//exist = true;
			FindClose(hFind);
		}
#else
		struct stat info;
		if (stat( szName, &info) == 0) 
		{
			bReturn = true;
			attr->name = szName;
			attr->length = info.st_size;
			attr->lastModify = (uint64_t) info.st_mtime * (1000);
			attr->lastAccess = (uint64_t) info.st_atime * (1000);
			if ( S_ISREG(info.st_mode) ) 
			{
				attr->type = T_ATTR::E_TYPE_FILE;
			}
			else if ( S_ISDIR(info.st_mode) )
			{
				attr->type = T_ATTR::E_TYPE_DIR;
			}
			else
			{
				attr->type = T_ATTR::E_TYPE_UNKNOWN;
			}
		}
#endif
		return bReturn;
	}

	static bool GetAttribute(const string & name,T_ATTR * attr)
	{
		assert(attr != NULL);
		return File::GetAttribute(name.c_str(),attr);
	}

	static int GetFileType(char szName[])
	{
		int eType = T_ATTR::E_TYPE_UNKNOWN;
#if(M_OS_TYPE == M_WINDOWS_OS)
		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile( szName, &data);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
			{
				eType = T_ATTR::E_TYPE_DIR;
			}
			else if(data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
			{
				eType = T_ATTR::E_TYPE_FILE;
			}
			else
			{
				eType = T_ATTR::E_TYPE_UNKNOWN;
			}
			FindClose(hFind);
		}
#else
		struct stat info;
		if (stat( szName, &info) == 0) 
		{
			if ( S_ISREG(info.st_mode) ) 
			{
				eType = T_ATTR::E_TYPE_FILE;
			}
			else if ( S_ISDIR(info.st_mode) )
			{
				eType = T_ATTR::E_TYPE_DIR;
			}
			else
			{
				eType = T_ATTR::E_TYPE_UNKNOWN;
			}
		}
#endif
		return eType;
	}

	static int GetFileType(const string & name)
	{
		return File::GetFileType(name.c_str());
	}

	int getFileType()
	{
		return GetFileType(m_Name.c_str());
	}

	void setName(const string & name)
	{
		m_Name = name;
	}
	void setName(char szName[])
	{
		m_Name = szName;
	}

	char * getName(char szName[],uint32_t length)
	{
		assert(length >= m_Name.length());
		m_Name.copy(szName,m_Name.length(),0);
		return szName;
	}
	string getName()
	{
		return m_Name;
	}


	static T_FILE * Open(char szName[],const char * szMode = "a+")
	{
		assert(szName);
		return ::fopen(szName,szMode);
	}
	static T_FILE * Open(string & name,const char * szMode = "a+")
	{
		assert(name.length() > 0);
		return ::fopen(name.c_str(),szMode);
	}

	static T_FILE * Open(char szName[],int iMode)
	{
		assert(szName);
		//char szFileName[128]={0};
		//Directory::GetPath(szName,szFileName,strlen(szName));
		return ::fopen(szName,Mode(iMode));
	}
	static T_FILE * Open(string name,int iMode)
	{
		assert(name.length()>0);
		//char szFileName[128]={0};
		//Directory::GetPath(szName,szFileName,strlen(szName));
		return ::fopen(name.c_str(),Mode(iMode));
	}

	bool open(const char * szMode = "a+")
	{
		m_Mutex->lock();
		assert(szMode);
		assert(m_State != E_FILE_OPENED);
		bool bReturn = false;
		m_Mode = Mode(szMode);
		m_File = ::fopen(m_Name.c_str(),szMode);
		if(m_File != NULL)
		{
			m_State = E_FILE_OPENED;
			bReturn = true;
		}
		else
		{
			m_State = E_FILE_ERROR;
			m_ErrCode = errno;
		}
		m_Mutex->unlock();
		return bReturn;		
	}

	bool open(int iMode)
	{
		m_Mutex->lock();
		assert(m_State != E_FILE_OPENED);
		bool bReturn = false;
		m_Mode = iMode;
		m_File = ::fopen(m_Name.c_str(),Mode(iMode));
		if(m_File != NULL)
		{
			m_State = E_FILE_OPENED;
			bReturn = true;
		}
		else
		{
			m_State = E_INITIALIZED;
			m_ErrCode = errno;
		}
		m_Mutex->unlock();
		return bReturn;
	}

	bool create()
	{
		m_Mutex->lock();
		assert(m_Name.length()>0);
		assert(m_State != E_FILE_OPENED);
		bool bReturn = false;
		m_File = ::fopen(m_Name.c_str(),"a+");
		if(m_File != NULL)
		{
			m_State = E_INITIALIZED;
			bReturn = true;
			::fclose(m_File);
		}
		else
		{
			m_State = E_INITIALIZED;
			m_ErrCode = errno;
		}
		m_Mutex->unlock();
		return bReturn;
	}

	static T_FILE * Create(char szName[])
	{
		assert(szName);
		//char szFileName[128]={0};
		//Directory::GetPath(szName,szFileName,strlen(szName));
		File::T_FILE * pFile = ::fopen(szName,"a+");
		if(pFile != NULL)
		{
			::fclose(pFile);
		}
		return pFile;
	}

	static T_FILE * Create(string name)
	{
		assert(name.length()>0);
		//char szFileName[128]={0};
		//Directory::GetPath(szName,szFileName,strlen(szName));
		File::T_FILE * pFile = NULL;
		pFile = ::fopen(name.c_str(),"a+");
		if(pFile != NULL)
		{
			::fclose(pFile);
		}
		return pFile;
	}

	static T_FILE * Create(char szName[],T_FILE ** pFile)
	{
		assert(szName);
		//char szFileName[128]={0};
		//Directory::GetPath(szName,szFileName,strlen(szName));
		*pFile = ::fopen(szName,"a+");
		if(pFile != NULL)
		{
			::fclose(*pFile);
		}
		return *pFile;
	}

	static int Close(T_FILE * pFile)
	{
		assert(pFile);
		return fclose(pFile);
	}

	int close()
	{
		m_Mutex->lock();
		assert(m_File);
		assert(m_State == E_FILE_OPENED);
		bool bReturn = false;
		if(M_CALL_FUNC_SUCCES == ::fclose(m_File))
		{
			m_State = E_INITIALIZED;
			bReturn = true;
		}
		else
		{
			M_WARNING("CLOSE FILE[%s] FAILED.",m_Name.c_str());
			m_State = E_FILE_ERROR;
			m_ErrCode = errno;
		}
		m_Mutex->unlock();	
		return bReturn;	
	}

	static int Remove(const string & name)
	{
		assert(name.length()>0);
		return File::Remove(name.c_str());
	}

	static int Remove(char szName[])
	{
		assert(szName);
		return File::Remove(szName,File::GetFileType(szName));
	}

	static int Remove(const string & name ,int eType)
	{
		return File::Remove(name.c_str(),eType);
	}

	static int Remove(char szName[],int eType)
	{
		assert(szName);
		int iReturn = 0;
#if (M_OS_TYPE == M_WINDOWS_OS)
		if (T_ATTR::E_TYPE_DIR == eType )
		{
			iReturn = ::RemoveDirectory(szName);
			if (iReturn != 0)
			{
				iReturn = GetLastError();
			}
		}
		else if (T_ATTR::E_TYPE_FILE == eType )
		{
			iReturn = ::remove(szName);
		}
		else
		{
			iReturn = -1;
		}
#else
		if ( T_ATTR::E_TYPE_DIR == eType)
		{
			iReturn = ::rmdir(szName);
		}
		else if (T_ATTR::E_TYPE_FILE == eType )
		{
			iReturn = ::remove(szName);
		}
		else
		{
			iReturn = -1;
		}
#endif
		return iReturn;
	}

	int remove()
	{
		m_Mutex->lock();
		bool bReturn = false;
		
		if(E_FILE_OPENED == m_State)
		{
			this->close();
		}
		assert(E_INITIALIZED == m_State);
		int iReturn = File::Remove(m_Name.c_str());
		if (iReturn != 0)
		{
			m_State = E_FILE_ERROR;
			m_ErrCode = iReturn;
		}
		else
		{
			bReturn = true;
		}
		m_Mutex->unlock();
		return bReturn;
	}

	static bool CopyTo(const string & name_src,const string & name_dst)
	{
		bool bReturn = false;
		int iSize = 0;
		T_FILE * pSrcFile = NULL;
		T_FILE * pDstFile = NULL;
		char szBuffer[E_BUFFER_SIZE] = {0};
		//char szDstFileName[M_MAX_FILE_NAME_LEN+1]={0};
		pSrcFile = Open(name_src,E_TYPE_BINARY|E_MODE_READ);
		if (pSrcFile!=NULL)
		{
			//Directory::GetPath(name_dst,szDstFileName,M_MAX_FILE_NAME_LEN);
			//Directory::Create(name_dst);
			pDstFile = Open(name_dst,E_MODE_WRITE|E_TYPE_BINARY);
			if (pDstFile)
			{
				iSize = Read(szBuffer,1,E_BUFFER_SIZE,pSrcFile);
				while (iSize>0)
				{
					Write(szBuffer,1,iSize,pDstFile);
					iSize = Read(szBuffer,1,E_BUFFER_SIZE,pSrcFile);
				}
				Close(pDstFile);
				bReturn = true;
			}
			Close(pSrcFile);
		}
		return bReturn;
	}

	static bool CopyTo(char szName_src[],char szName_dst[])
	{
		assert(szName_src != NULL);
		assert(szName_dst != NULL);

		bool bReturn = false;
		int iSize = 0;
		T_FILE * pSrcFile = NULL;
		T_FILE * pDstFile = NULL;
		char szBuffer[E_BUFFER_SIZE] = {0};
		string name_src = szName_src;
		string name_dst = szName_dst;
		//char szDstFileName[M_MAX_FILE_NAME_LEN+1]={0};
		pSrcFile = Open(name_src,E_TYPE_BINARY|E_MODE_READ);
		if (pSrcFile!=NULL)
		{
			//Directory::GetPath(name_dst,szDstFileName,M_MAX_FILE_NAME_LEN);
			//Directory::Create(szDstFileName);
			pDstFile = Open(name_dst,E_MODE_WRITE|E_TYPE_BINARY);
			if (pDstFile)
			{
				iSize = Read(szBuffer,1,E_BUFFER_SIZE,pSrcFile);
				while (iSize>0)
				{
					Write(szBuffer,1,iSize,pDstFile);
					iSize = Read(szBuffer,1,E_BUFFER_SIZE,pSrcFile);
				}
				Close(pDstFile);
				bReturn = true;
			}
			Close(pSrcFile);
		}
		return bReturn;
	}

	bool copyTo(char szPath[])
	{
		assert(szPath);
		return File::CopyTo(m_Name.c_str(),szPath);
	}

	bool copyTo(const string & path)
	{
		return File::CopyTo(m_Name,path);
	}

	/******************************************************************************
	 ptr:指向保存读取的数据内存,
	 size:每块的字节数,	 
	 nCount:要读取的块数,
	 return :如果成功，返回实际读取的块数(不是字节数)
	 本函数一般用于二进制模式打开的文件中。
	******************************************************************************/
	static size_t Read(void * ptr, size_t size, size_t nCount, T_FILE * pFile)
	{
		assert(ptr);
		assert(pFile);
		assert(nCount > 0);
		return ::fread(ptr,size,nCount,pFile);
	}

	size_t read(void * ptr, size_t size, size_t nCount)
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		size_t iReturn = Read(ptr,size,nCount,m_File);
		m_Mutex->unlock();
		return iReturn;
	}

	/******************************************************************************
	ptr:指向待写入数据,
	size:每块的字节数,	 
	nCount:要写的块数,
	return :如果成功，返回实际写入的块数(不是字节数)
	本函数一般用于二进制模式打开的文件中。
	******************************************************************************/
	static size_t Write(const void * ptr, size_t size, size_t nCount, T_FILE * pFile)
	{
		assert(ptr);
		assert(pFile);
		assert(nCount > 0);
		return ::fwrite(ptr,size,nCount,pFile);
	}

	size_t write(const void * ptr, size_t size, size_t nCount)
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		size_t iReturn = Write(ptr,size,nCount,m_File);
		m_Mutex->unlock();
		return iReturn;
	}

	static bool PutChar(int c, T_FILE * pFile)
	{
		assert(pFile);
		return ::fputc(c,pFile) == c ? true:false;
	}

	bool putChar(int c)
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		bool bReturn = PutChar(c,m_File);
		if (bReturn!=true)
		{
			m_ErrCode = this->error();
			m_State = E_FILE_ERROR;
		}
		m_Mutex->unlock();
		return bReturn;
	}

	static int GetChar(T_FILE * pFile)
	{
		assert(pFile);
		return ::fgetc(pFile); 
	}

	int getChar()
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		int iReturn = GetChar(m_File); 
		m_Mutex->unlock();
		return iReturn;
	}

	static int PutString(const char * str, T_FILE * pFile)
	{
		assert(pFile);
		assert(str);
		return ::fputs(str,pFile);
	}

	int putString(const char * str)
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		int iReturn = PutString(str,m_File);
		if (iReturn < 0)
		{
			m_ErrCode = this->error();
		}
		
		m_Mutex->unlock();
		return iReturn;
	}

	bool Flush(T_FILE * pFile)
	{
		assert(pFile);
		return fflush(pFile) != EOF ? true:false;
	}

	bool flush()
	{	
		assert(E_FILE_OPENED == m_State);
		m_Mutex->lock();
		bool bReturn = Flush(m_File);
		if (bReturn != true)
		{
			m_ErrCode = this->error();
		}
		m_Mutex->unlock();
		return bReturn;
	}

	static char * GetString(char str[], int nMaxLength, T_FILE * pFile)
	{
		assert(str);
		assert(pFile);
		return fgets(str,nMaxLength,pFile); 
	}

	char * getString(char str[], int nMaxLength)
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		char* szReturn = GetString(str,nMaxLength,m_File); 
		m_Mutex->unlock();
		return szReturn;
	}

	static bool SetFilePtr(T_FILE * pFile, long offset, int whence = File::E_BEGIN_FILE)
	{
		assert(pFile);
		return ::fseek(pFile,offset,whence) == 0 ? true : false;
	}

	bool setFilePtr(long offset, int whence = File::E_BEGIN_FILE)
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		bool bReturn = SetFilePtr(m_File,offset,whence);
		if (bReturn != true)
		{
			m_State = E_FILE_ERROR;
			m_ErrCode = this->error();
		}
		m_Mutex->unlock();
		return bReturn;
	}

	static bool End(T_FILE *pFile)
	{
		//feof(fp)有两个返回值:
		//如果遇到文件结束，函数feof（fp）的值为1，否则为0。
		assert(pFile);
		return feof(pFile)==1;
	}

	bool end()
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		bool bReturn = End(m_File);
		m_Mutex->unlock();
		return bReturn;
	}

	static void Rewind(T_FILE * pFile)
	{
		assert(pFile);
		::rewind(pFile);
	}

	void rewind()
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		Rewind(m_File);
		m_Mutex->unlock();
	}

	/********************************************************************************/
	/* 生成一个临时文件，以"w+b"的模式打开，并返回这个临时流的指针，如果失败返回NULL。
	/* 在程序结束时，这个文件会被自动删除。                                         */
	/********************************************************************************/
	static T_FILE * Create()
	{
		return ::tmpfile();
	}

	/************************************************************************/
	/*  生成一个唯一的文件名，并返回这个指针，如果失败，返回NULL。          */
	/************************************************************************/
	static char * CreateName(char szName[])
	{
		assert(szName);
		return ::tmpnam(szName);
	}
	
	string CreateName()
	{
		char szName[E_BUFFER_SIZE]={0};
		::tmpnam(szName);
		string name(szName);
		return name;
	}

	static int Error(FILE *pFile)
	{
		assert(pFile);
		return ferror(pFile);
	}

	int error()
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		int iReturn = Error(m_File);
		m_Mutex->unlock();
		return iReturn;
	}

	static void ClearError(FILE * pFile)
	{
		assert(pFile);
		clearerr(pFile);
	}

	void clearError()
	{
		m_Mutex->lock();
		assert( E_FILE_ERROR == m_State);
		m_State = E_FILE_OPENED;
		ClearError(m_File);
		m_Mutex->unlock();
	}

// 	static int WirteFormat(FILE *pFile, const char *format,...)
// 	{
// 		assert(pFile);
// 		assert(format);
// 		char buf[DEFAULT_BUFF_LEN],*pBuffer,*oldBuffer=NULL;
// 		int iBufferSize = DEFAULT_BUFF_LEN;
// 		pBuffer = buf;
// 		va_list args;
// 		va_start(args,format);
// 		int iReturn = vsnprintf(pBuffer,DEFAULT_BUFF_LEN,format,args);
// 		if ( iReturn < 0 || iReturn >= DEFAULT_BUFF_LEN-1)
// 		{
// 			iBufferSize *= 2;
// 			pBuffer = new char[iBufferSize];
// 			assert(pBuffer);
// 			iReturn = vsnprintf(pBuffer,iBufferSize,format,args);
// 			while( iReturn < 0 || iReturn >= iBufferSize -1)
// 			{
// 				if (iBufferSize*2 >= MAX_BUFFER_SIZE)
// 				{
// 					M_TRACE("THIS STRING IS TOO BIG");
// 					M_TRACE("CFILE BUFFER SIZE:%d",iBufferSize*2);
// 					break;
// 				}
// 				iBufferSize *= 2;
// 				oldBuffer = pBuffer;
// 				pBuffer = new char[iBufferSize];
// 				if (NULL == pBuffer)
// 				{
// 					pBuffer = oldBuffer;
// 					M_TRACE("ERROR:NO MEMORY!");
// 					break;
// 				}
// 				else
// 				{
// 					delete oldBuffer;
// 				}
// 				iReturn = vsnprintf(pBuffer,iBufferSize,format,args);
// 			}
// 			iReturn =  ::fprintf(pFile,"%s",pBuffer);
// 			delete pBuffer;
// 			
// 		}
// 		else	//传入字符串大小在默认大小以内
// 		{
// 			iReturn =  ::fprintf(pFile,"%s",pBuffer);
// 		}
// 		va_end(args);
// 		return iReturn;
// 	}
// 
// 	int writeFormat(const char *format,...)
// 	{
// 		m_Mutex->lock();
// 		assert(FILE_OPENED == m_State);
// 		assert(m_File);
// 		va_list args;
// 		va_start(args,format);
// 		int iReturn = vsnprintf(m_Buffer,m_BufferSzie,format,args);
// 		if(iReturn < 0||iReturn >= m_BufferSzie-1)
// 		{
// 			int iMultiple = 1;
// 			do{
// 				if (m_BufferSzie >= MAX_BUFFER_SIZE)
// 				{
// 					M_TRACE("THIS STRING IS TOO BIG");
// 					M_TRACE("CFILE BUFFER SIZE:%d",m_BufferSzie);
// 					break;
// 				}
// 				this->_setBuffer(m_BufferSzie + DEFAULT_BUFF_LEN * iMultiple++);
// 				iReturn = vsnprintf(m_Buffer,m_BufferSzie,format,args);
// 			}
// 			while(iReturn < 0 || iReturn >= m_BufferSzie-1);
// 		}
// 		va_end(args);
// 		iReturn = ::fprintf(m_File,"%s",m_Buffer);
// 		m_Mutex->unlock();
// 		return iReturn;
// 	}

	int writeFormat(const char *format,...)
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		assert(m_File);
		va_list args;
		va_start(args,format);
		int iReturn = vfprintf(m_File,format,args);
		va_end(args);
		m_Mutex->unlock();
		return iReturn;
	}

	static int WriteFormat(T_FILE * pFile,const char *format,...)
	{
		assert(pFile);
		va_list args;
		va_start(args,format);
		int iReturn = ::vfprintf(pFile,format,args);
		va_end(args);
		return iReturn;
	}

	int writeFormatA(char * format,...)
	{
		assert(E_FILE_OPENED == m_State);
		assert(m_File);
		va_list ap; 
		char *p, *sval,cval;
		int ival,iReturn = 0;
		double dval;

		va_start(ap, format); 
		for (p = format; *p; p++)
		{
			if (*p != '%')	continue;
			switch (*++p)
			{
			case 'c':
				cval = va_arg(ap,char);
				iReturn += fprintf(m_File,"%c",cval);
				break;
			case 'd':
				ival = va_arg(ap, int);
				iReturn += fprintf(m_File,"%d", ival);
				break;
			case 'f':
				dval = va_arg(ap, double);
				iReturn += fprintf(m_File,"%f", dval);
				break;
			case 's':
				sval = va_arg(ap, char*);
				iReturn += fprintf(m_File,"%s", sval);
				break;
			default:
				iReturn += fprintf(m_File,"%c",*p);
				break;
			}
		}
		va_end(ap); /* clean up when done */
		return iReturn;
	}

#if( M_OS_TYPE == M_LINUX_OS )
	//linux
	static int ReadFormat(FILE *pFile, const char *format,...)
	{
		assert(pFile);
		assert(format);
		va_list args;
		va_start(args,format);
		//args = (char*)&format+sizeof(format);//取不定参数首地址
		return ::vfscanf(pFile,format,args);
	}

	int readFormat(const char *format,...)
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		va_list args;
		va_start(args,format);
		//args = (char*)&format+sizeof(format);//取不定参数首地址
		int iReturn = ::vfscanf(m_File,format,args);
		m_Mutex->unlock();
		return iReturn;
	}

#else	//WIN32
	static int ReadFormat(T_FILE *pFile,char *format, ...)
	{
		assert(pFile);
		int iReturn=0;
		va_list ap;
		char defaultChar=0;
		char tempChar=0;
		char *pChar,*pFormat;
		int *pInt;
		unsigned int * pUint;
		double *pDouble;
		va_start(ap,format);

		for (pFormat = format; *pFormat; pFormat++)
		{
			switch(*pFormat)
			{
			case 'c':
				{
					pChar = va_arg(ap,char*);
					iReturn += ::fscanf(pFile,"%c",pChar);
					break;
				}
			case 'd':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(pFile,"%d",pInt);
					break;
				}
			case 'e':
				{
					pDouble = va_arg(ap,double*);
					iReturn += ::fscanf(pFile,"%e",pDouble);
					break;
				}
			case 'f':
				{
					pDouble = va_arg(ap,double*);
					iReturn += ::fscanf(pFile,"%f",pDouble);
					break;
				}
			case 'i':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(pFile,"%i",pInt);
					break;
				}
			case 'o':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(pFile,"%o",pInt);
					break;
				}
			case 'x':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(pFile,"%x",pInt);
					break;
				}
			case 'u':
				{
					pUint = va_arg(ap,unsigned int*);
					iReturn += ::fscanf(pFile,"%u",pUint);
					break;
				}
			case 'n':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(pFile,"%n",pInt);
					break;
				}
			case 's':
				{
					pChar = va_arg(ap,char*);
					iReturn += ::fscanf(pFile,"%s",pChar);
					break;
				}

			default:
				defaultChar = *pFormat;
				::fscanf(pFile,&defaultChar,&tempChar);
				break;
			}
		}
		va_end(ap);
		return iReturn;
	}

	int readFormat(char *format, ...)
	{
		m_Mutex->lock();
		assert(E_FILE_OPENED == m_State);
		assert(m_File);
		int iReturn=0;
		va_list ap;
		char defaultChar=0;
		char tempChar=0;
		char *pChar,*pFormat;
		int *pInt;
		unsigned int * pUint;
		double *pDouble;
		va_start(ap,format);

		for (pFormat = format; *pFormat; pFormat++)
		{
			if ( '%' == *pFormat)	
			{
				continue;
			}
			switch(*pFormat)
			{
			case 'c':
				{
					pChar = va_arg(ap,char*);
					iReturn += ::fscanf(m_File,"%c",pChar);
					break;
				}
			case 'd':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(m_File,"%d",pInt);
					break;
				}
			case 'e':
				{
					pDouble = va_arg(ap,double*);
					iReturn += ::fscanf(m_File,"%e",pDouble);
					break;
				}
			case 'f':
				{
					pDouble = va_arg(ap,double*);
					iReturn += ::fscanf(m_File,"%f",pDouble);
					break;
				}
			case 'i':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(m_File,"%i",pInt);
					break;
				}
			case 'o':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(m_File,"%o",pInt);
					break;
				}
			case 'x':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(m_File,"%x",pInt);
					break;
				}
			case 'u':
				{
					pUint = va_arg(ap,unsigned int*);
					iReturn += ::fscanf(m_File,"%u",pUint);
					break;
				}
			case 'n':
				{
					pInt = va_arg(ap,int*);
					iReturn += ::fscanf(m_File,"%n",pInt);
					break;
				}
			case 's':
				{
					pChar = va_arg(ap,char*);
					iReturn += ::fscanf(m_File,"%s",pChar);
					break;
				}

			default:
				defaultChar = *pFormat;
				::fscanf(m_File,&defaultChar,&tempChar);
				break;
			}
		}
		va_end(ap);
		m_Mutex->unlock();
		return iReturn;
	}
#endif


// 	bool setBuffer(int lenght)
// 	{
// 		m_Mutex->lock();
// 		bool bReturn = _setBuffer(lenght);
// 		m_Mutex->unlock();
// 		return bReturn;
// 	}

private:

// 	bool _setBuffer(int lenght)
// 	{
// 		assert(m_Buffer);
// 		delete m_Buffer;
// 		m_BufferSzie = lenght;
// 		m_Buffer = new char[m_BufferSzie];
// 		bool bReturn = (m_Buffer != NULL);
// 		return bReturn;
// 	}

	static char * Mode(int iMode)
	{
		switch(iMode)
		{
		case E_MODE_READ:								return M_SZ_READ_T_MODE;
		case E_MODE_READ		|	E_TYPE_TEXT:		return M_SZ_READ_T_MODE;
		case E_MODE_READ		|	E_TYPE_BINARY:		return M_SZ_READ_B_MODE;

		case E_MODE_APPEND:								return M_SZ_APPEND_T_MODE;
		case E_MODE_APPEND		|	E_TYPE_TEXT:		return M_SZ_APPEND_T_MODE;
		case E_MODE_APPEND		|	E_TYPE_BINARY:		return M_SZ_APPEND_B_MODE;

		case E_MODE_WRITE:								return M_SZ_WRITE_T_MODE;
		case E_MODE_WRITE		|	E_TYPE_TEXT:		return M_SZ_WRITE_T_MODE;
		case E_MODE_WRITE		|	E_TYPE_BINARY:		return M_SZ_WRITE_B_MODE;

		case E_MODE_READ		|	E_MODE_WRITE:							return M_SZ_RW_T_MODE;
		case E_MODE_READ		|	E_MODE_WRITE	|	E_TYPE_TEXT:		return M_SZ_RW_T_MODE;
		case E_MODE_READ		|	E_MODE_WRITE	|	E_TYPE_BINARY:		return M_SZ_RW_B_MODE;

		case E_MODE_READ		|	E_MODE_APPEND:							return M_SZ_RA_T_MODE;
		case E_MODE_READ		|	E_MODE_APPEND	|	E_TYPE_TEXT:		return M_SZ_RA_T_MODE;
		case E_MODE_READ		|	E_MODE_APPEND	|	E_TYPE_BINARY:		return M_SZ_RA_B_MODE;

		default:	M_ERROR("FILE OPEN MODE!");
		}
		return M_SZ_RA_T_MODE;
	}

	static int Mode(const char* szMode)
	{
		int iMode=0,len = strlen(szMode);
		for(int i=0;i<len;i++)
		{
			switch(szMode[i])
			{
			case 'r':iMode |= E_MODE_READ;	break;
			case 'w':iMode |= E_MODE_WRITE;	break;
			case 'a':iMode |= E_MODE_APPEND;break;
			case 't':iMode |= E_TYPE_TEXT;	break;
			case 'b':iMode |= E_TYPE_BINARY;break;
			case '\n':break;
			case '\0':break;
			case '+' :break;
			default:	M_ERROR("FILE OPEN MODE!");
			}
		}
		//printf("%X",iMode);
		return iMode;
	}
};


M_END_NAMESPACE(mylib)

#endif	//_MYLIB_FILE_H__INLCLUED_20090805_

// 
// 
// #include <stdio.h>
// #include <stdarg.h>
// 
// /* minprintf: minimal printf with variable argument list */
// 
// void minprintf(char *fmt, ...)
// {
// 	va_list ap; /* points to each unnamed arg in turn */
// 	char *p, *sval;
// 	int ival;
// 	double dval;
// 
// 	va_start(ap, fmt); /* make ap point to 1st unnamed arg */
// 
// 	for (p = fmt; *p; p++)
// 	{
// 		if (*p != '%')
// 		{
// 			putchar(*p);
// 			continue;
// 		}
// 		switch (*++p)
// 		{
// 		case 'c':
// 			ival = va_arg(ap,char);
// 			printf("%c",ival);
// 			break;
// 		case 'd':
// 			ival = va_arg(ap, int);
// 			printf("%d", ival);
// 			break;
// 		case 'f':
// 			dval = va_arg(ap, double);
// 			printf("%f", dval);
// 			break;
// 		case 's':
// 			for (sval = va_arg(ap, char *); *sval; sval++)
// 				putchar(*sval);
// 			break;
// 		default:
// 			putchar(*p);
// 			break;
// 		}
// 	}
// 	va_end(ap); /* clean up when done */
// }
// 
// 
// int printf_test()
// {
// 	int x = 10, y = 20;
// 	char s[] = "test my minprintf";
// 	minprintf("%s: x = %d, y = %d", s, x, y);
// 	return 0;
// }
// 
#endif
