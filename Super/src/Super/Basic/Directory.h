#if 0
/******************************************************************************
*File   : Directory.h
*Author : Lan Hu
*Email	: Hi_Blue@126.com
*Notes	:
*Licence: 
*Created: 2009-09-09
*******************************************************************************/

#ifndef _MYLIB_DIRECTORY_H_INCLUDE_20090909
#define _MYLIB_DIRECTORY_H_INCLUDE_20090909

#include "public.h"
#include "File.h"
#include "LinkedList.h"


#if( M_OS_TYPE == M_WINDOWS_OS )

#include <direct.h>
#include <io.h>

typedef HANDLE	DIR_T;

#else

#include <dirent.h>
#include<unistd.h>

typedef DIR * DIR_T;

#endif


M_BEGIN_NAMESPACE(mylib)

typedef struct FileName 
{
	string	path;
	string	name;

}T_FILE_NAME;



class Directory
{
public:
	typedef DIR_T T_DIR;
	typedef File::T_ATTR T_ATTR;
	typedef T_FILE_NAME T_NAME;
/*	typedef T_ATTR::E_FILE_TYPE E_FILE_TYPE;*/
public:
	typedef LinkedList<ListType<T_ATTR>::T_SLIST> T_FILE_LIST;
	typedef LinkedList<ListType<T_NAME>::T_SLIST> T_NAME_LIST;
	enum {E_SLASH = M_SLASH,E_ANOTHER_SLASH = M_ANOTHER_SLASH };

private:
	string m_Root;
	DIR_T m_DirHandle;
	ThreadMutex * m_Mutex;

public:

	Directory()
	{
		m_Mutex = new ThreadMutex();
		if (m_Mutex == NULL)
		{
			M_ERROR("NO MORE MEMORY.");
		}
	}

	Directory(const string & path)
	{
		assert(path.length()>0);
		m_Mutex = new ThreadMutex();
		if (m_Mutex == NULL)
		{
			M_ERROR("NO MORE MEMORY.");
		}
		if(GetPath(m_Root,path) >0 )
		{
			m_Root = "";
			M_WARNING("Error Path:%s",path.c_str());
		}
	}

	Directory(char szPath[])
	{
		assert(szPath != NULL && szPath[0] != 0);
		m_Mutex = new ThreadMutex();
		if (m_Mutex == NULL)
		{
			M_ERROR("NO MORE MEMORY.");
		}
		if(GetPath(m_Root,szPath) >0 )
		{
			m_Root = "";
			M_WARNING("Error Path:%s",szPath);
		}
	}

	~Directory()
	{
		m_Mutex->lock();
		//...
		m_Mutex->unlock();
		delete m_Mutex;
	}

	void setPath(char szPath[])
	{
		if(GetPath(m_Root,szPath) >0)
		{
			m_Root = "";
			M_WARNING("Error Path:%s",szPath);
		}
	}

	void setPath(const string & path)
	{
		if(GetPath(m_Root,path) > 0)
		{
			m_Root = "";
			M_WARNING("Error Path:%s",path.c_str());
		}
	}

	//***************说明**************************
	//功能：获取统一格式的路径字符串
	//返回值：成功则返回目录字符串长度，否则返回 0
	//*********************************************
	static int GetPath(string & path_out,const string & path_in)
	{
		int iReturn = 0;
		path_out = path_in.c_str();
		string::size_type pos = path_out.find(M_ANOTHER_SLASH); 
		while(pos != string::npos)
		{   
			path_out[pos]= M_SLASH;
			pos = path_out.find_first_of(M_ANOTHER_SLASH,pos + 1);
		}

		if(path_out.length()>0)
		{
			iReturn = path_out.length();
		}
		else
		{
			path_out = "";
		}
		return iReturn;
	}
	static int GetPath(char szPath_out[],size_t iLength,char szPath_in[])
	{
		int iReturn = 0;
		string path_out = szPath_in;
		string::size_type pos = path_out.find(M_ANOTHER_SLASH); 
		while(pos != string::npos)
		{   
			path_out[pos] = M_SLASH;
			pos = path_out.find_first_of(M_ANOTHER_SLASH,pos + 1);
		}

		if(path_out.length() == 0 || path_out.length() > iLength )
		{
			memset(szPath_out,0,iLength);
		}
		else
		{
			path_out.copy(szPath_out,iLength);
			iReturn = path_out.length();
		}
		return iReturn;
	}
	static int GetPath(string & path_out,char szPath_in[])
	{
		int iReturn = 0;
		path_out = szPath_in;
		string::size_type pos = path_out.find(M_ANOTHER_SLASH); 
		while(pos != string::npos)
		{   
			path_out[pos] = M_SLASH;
			pos = path_out.find_first_of(M_ANOTHER_SLASH,pos + 1);
		}
		iReturn = path_out.length();
		if(iReturn == 0 )
		{
			path_out = "";
		}
		return iReturn;
	}
	char * getPath(char szPath[],int iLength)
	{
		assert(szPath);
		int length = m_Root.length();
		if (length<=iLength && length>0)
		{
			strncpy(szPath,m_Root.c_str(),iLength);
		}
		else
		{
			memset(szPath,0,iLength);
		}
		return szPath;
	}
	string & getPath(string & path)
	{
		path = m_Root;
		return path;
	}
	string getPath()
	{
		return m_Root;
	}

	//***************说明************************
	//功能：创建文件夹
	//返回值：成功则返回创建目录层数， 否则返回-1
	//*******************************************
	static int Create(char * szPath)
	{
		int iCount = 0;
		char *p = szPath;
		while (p = strchr(++p, M_SLASH)) 
		{
			*p = 0;
			if (m_access(szPath, 0/*0:是否存在 */) != -1) 
			{
				*p = M_SLASH;
				continue;
			}
			if(MakeDir(szPath) == 0)
			{
				++iCount;
			}
			else
			{
				++iCount;
				iCount = 0 - iCount;
				break;
			}

			*p = M_SLASH;
		}
		return iCount;
	}
	static int Create(string path)
	{
		int iCount = 0;
		string subDir;
		string::size_type pos=0 ;
		
		pos = path.find(M_SLASH,0);
		while (pos != string::npos) 
		{
			subDir = path.substr(0,pos) + M_SLASH;
			if (Access(subDir) <=0) 
			{
				if(MakeDir(subDir.c_str()) == 0)
				{
					++iCount;
				}
				else
				{
					++iCount;
					iCount = 0 - iCount;
					break;
				}
			}
			pos = path.find(M_SLASH,pos +1 );
		}
		return iCount;
	}
	int create()
	{
		int iReturn = -1;
		if(m_Root.length() > 0)
		{
			iReturn = Create(m_Root);
		}
		return iReturn;
	}
	//*******************说明**************************
	//功  能：测试给定路径所指定的文件或文件夹是否存在
	//返回值：路径正确存在则返回目录层数。
	//        路径中有目录不存在时返回当前层数相反数。
	//*************************************************
	static int Access(string path)
	{
		int iReturn =-1;
		int length = path.length();
		if(length >0)
		{
			iReturn = 1;
			string subDir;
			string::size_type pos = path.find(M_SLASH);
			if (string::npos == pos)
			{
				if (m_access(path.c_str(), 0/*0:是否存在 */) == -1)
				{
					//当前目录不存在。
					iReturn = -1;
				}
				else
				{
					iReturn = 1;
				}
			}
			else
			{
				while (pos != string::npos) 
				{
					subDir = path.substr(0,pos);
					if (pos != 0)
					{
						if (m_access(subDir.c_str(), 0/*0:是否存在 */) == -1) 
						{
							//当前目录不存在。
							//iReturn = -1;
							iReturn = 0 - iReturn;
							break;
						}
					}
					pos = path.find(M_SLASH,pos+1);
					++iReturn;
				}
			}

		}
		return iReturn;
	}
	int access()
	{
		int iReturn =-1;
		int length = m_Root.length();
		if(length >0)
		{
			iReturn = 1;
			string subDir;
			string path = m_Root;
			string::size_type begin_pos = 0 ,end_pos = path.find(M_SLASH);
			if (string::npos == end_pos)
			{
				if (m_access(path.c_str(), 0/*0:是否存在 */) == -1) 
				{
					//当前目录不存在。
					iReturn = -1;
				}
				else
				{
					iReturn = 1;
				}
			}
			else
			{
				while (end_pos != string::npos) 
				{
					subDir = path.substr(begin_pos,end_pos);
					if (m_access(subDir.c_str(), 0/*0:是否存在 */) == -1) 
					{
						//当前目录不存在。
						//iReturn = -1;
						iReturn = 0 - iReturn;
						break;
					}
					begin_pos = end_pos;
					end_pos = path.find(M_SLASH,begin_pos);
					++iReturn;
				}
			}
		}
		return iReturn;
	}

	//***************************说明*********************************
	//功    能：获得给定目录下第一个文件的名称
	//输入参数：目录名(szName_in)、目录名最大长度(iLength)
	//输出参数：目录内第一个文件名(szName_out)、文件类型(eType)[可选]
	//返 回 值：成功返回目录的句柄，失败返回 NULL
	//****************************************************************
	static T_DIR GetFirstFile(char szName_out[],size_t iLength,char szName_in[],int * eType = NULL)
	{
		string pattern = szName_in ;
		szName_out[0] = 0;
		T_DIR dirHandle = NULL;

		if (eType != NULL)
		{
			*eType = T_ATTR::E_NOT_EXIST;
		}

		if ( pattern[pattern.length()-1] != M_SLASH )
		{
			pattern += M_SLASH;
		}
#if (M_OS_TYPE == M_WINDOWS_OS)
		pattern +=  '*';
		WIN32_FIND_DATA data;
		dirHandle = FindFirstFile( pattern.c_str(), & data );
		if ( dirHandle != INVALID_HANDLE_VALUE )
		{
			size_t length = strlen(data.cFileName);
			if(length < iLength)
			{
				snprintf(szName_out,length,"%s",data.cFileName);
				if (eType != NULL)
				{
					if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
					{
						*eType = T_ATTR::E_TYPE_DIR;
					}
					else if(data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
					{
						*eType = T_ATTR::E_TYPE_FILE;
					}
					else
					{
						*eType = T_ATTR::E_TYPE_UNKNOWN;
					}
				}
			}
			else
			{
				dirHandle = NULL;
				M_WARNING("need more space to save the file name.");
			}
		}
#else
		dirHandle = opendir( pattern.c_str() );
		if ( dirHandle != NULL )
		{
			struct dirent * entry =  readdir(dirHandle);
			if ( entry != NULL )
			{
				size_t length = strlen( entry->d_name);
				if(length < iLength)
				{
					snprintf(szName_out,iLength,"%s", entry->d_name);

					if (eType != NULL)
					{
						if (entry->d_type == DT_DIR ) 
						{
							*eType = T_ATTR::E_TYPE_DIR;
						}
						else if (entry->d_type == DT_REG)
						{
							*eType = T_ATTR::E_TYPE_FILE;
						}
						else
						{
							*eType = T_ATTR::E_TYPE_UNKNOWN;
						}
					}
				}
				else
				{
					dirHandle = NULL;
					M_WARNING("need more space to save the file name.");
				}
			}
		}
#endif
		return dirHandle;
	}

	//***************************说明*********************************
	//功    能：获得给定目录下第一个文件的名称
	//输入参数：目录名(szName_in)
	//输出参数：目录内第一个文件名(name_out)、文件类型(eType)[可选]
	//返 回 值：成功返回目录的句柄，失败返回 NULL
	//****************************************************************
	static T_DIR GetFirstFile(string & name_out,char szPath_in[],int * eType = NULL)
	{
		T_DIR dirHandle = NULL;
		if (eType != NULL)
		{
			*eType = T_ATTR::E_NOT_EXIST;
		}

		string pattern = szPath_in;
		if ( pattern[pattern.length()-1] != M_SLASH )
		{
			pattern += M_SLASH;
		}

#if (M_OS_TYPE == M_WINDOWS_OS)

		pattern +=  '*';

		WIN32_FIND_DATA data;
		dirHandle = FindFirstFile( pattern.c_str(), & data );
		if ( dirHandle != INVALID_HANDLE_VALUE )
		{
			name_out = data.cFileName;
			if (eType != NULL)
			{
				if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					*eType = T_ATTR::E_TYPE_DIR;
				}
				else if(data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
				{
					*eType = T_ATTR::E_TYPE_FILE;
				}
				else
				{
					*eType = T_ATTR::E_TYPE_UNKNOWN;
				}
			}
		}
#else
		dirHandle = opendir( pattern.c_str());
		if ( dirHandle != NULL )
		{
			struct dirent* entry =  readdir(dirHandle);
			if ( entry != NULL )
			{
				name_out = entry->d_name ;
				if (eType != NULL)
				{
					if (entry->d_type == DT_DIR ) 
					{
						*eType = T_ATTR::E_TYPE_DIR;
					}
					else if (entry->d_type == DT_REG)
					{
						*eType = T_ATTR::E_TYPE_FILE;
					}
					else
					{
						*eType = T_ATTR::E_TYPE_UNKNOWN;
					}
				}
			}
		}
#endif
		return dirHandle;
	}

	//***************************说明*********************************
	//功    能：获得给定目录下第一个文件的名称
	//输入参数：目录名(name_in)、目录名最大长度(iLength)
	//输出参数：目录内第一个文件名(szName_out)、文件类型(eType)[可选]
	//返 回 值：成功返回目录的句柄，失败返回 NULL
	//****************************************************************
	static T_DIR GetFirstFile(char szName_out[],size_t iLength,const string & name_in,int * eType = NULL)
	{
		return Directory::GetFirstFile(szName_out,iLength,name_in.c_str(),eType);
	}

	//***************************说明*********************************
	//功    能：获得给定目录下第一个文件的名称
	//输入参数：目录名(name_in)
	//输出参数：目录内第一个文件名(name_out)、文件类型(eType)[可选]
	//返 回 值：成功返回目录的句柄，失败返回 NULL
	//****************************************************************
	static T_DIR GetFirstFile(string & name_out,const string & path_in,int * eType = NULL)
	{
		T_DIR dirHandle = NULL;
		if (eType != NULL)
		{
			*eType = T_ATTR::E_NOT_EXIST;
		}

		string pattern = path_in;
		if ( pattern[pattern.length()-1] != M_SLASH )
		{
			pattern += M_SLASH;
		}

#if (M_OS_TYPE == M_WINDOWS_OS)

		pattern +=  '*';

		WIN32_FIND_DATA data;
		dirHandle = FindFirstFile( pattern.c_str(), & data );
		if ( dirHandle != INVALID_HANDLE_VALUE )
		{
			name_out = data.cFileName;
			if (eType != NULL)
			{
				if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					*eType = T_ATTR::E_TYPE_DIR;
				}
				else if(data.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
				{
					*eType = T_ATTR::E_TYPE_FILE;
				}
				else
				{
					*eType = T_ATTR::E_TYPE_UNKNOWN;
				}
			}
		}
#else
		dirHandle = opendir( pattern.c_str());
		if ( dirHandle != NULL )
		{
			struct dirent* entry =  readdir(dirHandle);
			if ( entry != NULL )
			{
				name_out = entry->d_name ;
				if (eType != NULL)
				{
					if (entry->d_type == DT_DIR ) 
					{
						*eType = T_ATTR::E_TYPE_DIR;
					}
					else if (entry->d_type == DT_REG)
					{
						*eType = T_ATTR::E_TYPE_FILE;
					}
					else
					{
						*eType = T_ATTR::E_TYPE_UNKNOWN;
					}
				}
			}
		}
#endif
		return dirHandle;
		//return Directory::GetFirstFile(name_out,path_in.c_str(),eType);
	}

	//***************************说明*********************************
	//功    能：获得给定目录下第一个文件的名称
	//输入参数：目录名(path_in\szPath_in)
	//输出参数：目录内第一个文件的属性信息结构体(attr)
	//返 回 值：成功返回目录的句柄，失败返回 NULL
	//****************************************************************
	static T_DIR GetFirstFile(File::T_ATTR * attr,char szPath_in[])
	{
		assert(attr);
		T_DIR dirHandle = NULL;

		attr->type = T_ATTR::E_NOT_EXIST;
		attr->name = "";
		attr->length = 0;
		attr->lastAccess = 0;
		attr->lastModify = 0;

		string pattern = szPath_in;
		if ( pattern[pattern.length()-1] != M_SLASH )
		{
			pattern += M_SLASH;
		}

#if (M_OS_TYPE == M_WINDOWS_OS)

		pattern +=  '*';

		WIN32_FIND_DATA data;
		dirHandle = FindFirstFile( pattern.c_str(), & data );
		if ( dirHandle != INVALID_HANDLE_VALUE )
		{
			attr->name = data.cFileName;

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

		}
#else
		dirHandle = opendir( pattern.c_str());
		if ( dirHandle != NULL )
		{
			struct dirent* entry =  readdir(dirHandle);
			if ( entry != NULL )
			{
				attr->name = entry->d_name ;
				if (entry->d_type == DT_DIR ) 
				{
					attr->type = T_ATTR::E_TYPE_DIR;
				}
				else if (entry->d_type == DT_REG)
				{
					attr->type = T_ATTR::E_TYPE_FILE;
				}
				else
				{
					attr->type = T_ATTR::E_TYPE_UNKNOWN;
				}

				struct stat info;
				if (stat( entry->d_name, & info) == 0) 
				{
					attr->length = info.st_size;
					attr->lastModify = (uint64_t) info.st_mtime * (1000);
					attr->lastAccess = (uint64_t) info.st_atime * (1000);
				}
			}
		}
#endif
		return dirHandle;
	}

	//***************************说明*********************************
	//功    能：获得给定目录下第一个文件的名称
	//输入参数：目录名(path_in\szPath_in)
	//输出参数：目录内第一个文件的属性信息结构体(attr)
	//返 回 值：成功返回目录的句柄，失败返回 NULL
	//****************************************************************
	static T_DIR GetFirstFile(File::T_ATTR * attr,const string & path_in)
	{
		return Directory::GetFirstFile(attr,path_in.c_str());
	}

	//***************************说明*********************************
	//功    能：获得当前目录(m_Root)下第一个文件的名称
	//输入参数：目录名(szPath_in)，最大文件名长度(iLength)
	//输出参数：目录内第一个文件名(name_out),文件类型(eType)[可选]
	//返 回 值：成功返回 true 否则返回 false 
	//****************************************************************
	bool getFirstFile(char szName_out[],size_t iLength,char szName_in[],int * eType = NULL)
	{
		return Directory::GetFirstFile(szName_out,iLength,m_Root.c_str(),eType) != 0 ? false:true;
	}

	//***************************说明*********************************
	//功    能：获得当前目录(m_Root)下第一个文件的名称
	//输入参数：(空)
	//输出参数：目录内第一个文件名(name_out),文件类型(eType)[可选]
	//返 回 值：成功返回 true 否则返回 false 
	//****************************************************************
	bool getFirstFile(string & name_out,int * eType = NULL)
	{
		return Directory::GetFirstFile(name_out,m_Root,eType) != 0 ? false:true;
	}

	//***************************说明*********************************
	//功    能：获得当前目录句柄指向目录的下一个文件的名称
	//输入参数：目录句柄(dirHandle)
	//输出参数：文件名(name_out)，文件类型(eType)[可选]
	//返 回 值：成功返回文件类型 否则返回 -1 
	//****************************************************************
	static int Directory::GetNextFile(string & name_out,T_DIR dirHandle,int * eType = NULL)
	{
		int iReturn = T_ATTR::E_NOT_EXIST;
		name_out = "";
		if ( dirHandle != NULL )
		{
			if (eType != NULL)
			{
				*eType = T_ATTR::E_NOT_EXIST;
			}
#ifdef _WIN32
			WIN32_FIND_DATA data;

			if ( FindNextFile(dirHandle, &data) )
			{
				name_out = data.cFileName;
				if (eType != NULL)
				{
					if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
					{
						*eType = T_ATTR::E_TYPE_DIR;
					}
					else
					{
						*eType = T_ATTR::E_TYPE_FILE;
					}
					
					iReturn = *eType;
				}

			}
			else
			{
				Directory::Close(dirHandle);
			}
#else
			struct dirent* entry =  readdir(dirHandle);
			if ( entry != NULL )
			{
				name_out = entry->d_name;
				if (eType != NULL)
				{
					if (entry->d_type == DT_DIR ) 
						*eType = T_ATTR::E_TYPE_DIR;
					else if (entry->d_type == DT_REG)
						*eType = T_ATTR::E_TYPE_FILE;
					else
						*eType = T_ATTR::E_TYPE_UNKNOWN;
				}
				iReturn = *eType;

				/*************************
				* DT_UNKNOWN	0 
				* DT_FIFO		1 
				* DT_CHR		2 
				* DT_DIR		4 
				* DT_BLK		6 
				* DT_REG		8 
				* DT_LNK		10 
				* DT_SOCK		12 
				* DT_WHT		14
				*************************/
			}
			else
			{
				Directory::Close(dirHandle);
			}
#endif
		}
		return iReturn;
	}

	//***************************说明*********************************
	//功    能：获得当前目录句柄指向目录的下一个文件的名称
	//输入参数：目录句柄(dirHandle)，最大文件名长度(iLength)
	//输出参数：文件名(name_out)，文件类型(eType)[可选]
	//返 回 值：成功返回文件类型 否则返回 -1 
	//****************************************************************
	static int GetNextFile(char szName_out[],size_t  iLength,T_DIR dirHandle,int * eType = NULL)
	{
		string name;
		int iReturn = Directory::GetNextFile(name,dirHandle,eType);
		if (iReturn>0)
		{
			size_t length = name.length();
			if (iLength > length)
			{
				name.copy(szName_out,length);
			}
			else
			{
				szName_out[0] = 0;
			}
		}
		return iReturn;
	}

	//***************************说明*********************************
	//功    能：获得当前目录句柄指向目录的下一个文件的属性信息
	//输入参数：目录句柄(dirHandle)，最大文件名长度(iLength)
	//输出参数：文件的属性信息(attr)
	//返 回 值：成功返回文件类型 否则返回 -1 
	//****************************************************************
	static int GetNextFile(File::T_ATTR * attr,T_DIR dirHandle)
	{
		assert(attr != NULL);

		attr->type = T_ATTR::E_NOT_EXIST;
		attr->length = 0;
		attr->lastAccess = 0;
		attr->lastModify = 0;

		if ( dirHandle != NULL )
		{
#ifdef _WIN32
			WIN32_FIND_DATA data;

			if ( FindNextFile(dirHandle, &data) )
			{		
				if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					attr->type = T_ATTR::E_TYPE_DIR;
				}
				else
				{
					attr->type = T_ATTR::E_TYPE_FILE;
				}
				

				attr->name = data.cFileName;
				attr->length = data.nFileSizeLow;
				attr->lastModify = ((uint64_t)data.ftLastWriteTime.dwLowDateTime |
					((uint64_t)data.ftLastWriteTime.dwHighDateTime << 32)) / (10000) - (11644473600000);
				attr->lastAccess = ((uint64_t)data.ftLastAccessTime.dwLowDateTime |
					((uint64_t)data.ftLastAccessTime.dwHighDateTime << 32)) / (10000) - (11644473600000);
			}
			else
			{
				Directory::Close(dirHandle);
			}
#else
			struct dirent * entry =  readdir(dirHandle);
			if ( entry != NULL )
			{
				attr->name = entry->d_name ;
				if (entry->d_type == DT_DIR ) 
				{
					attr->type = T_ATTR::E_TYPE_DIR;
				}
				else if (entry->d_type == DT_REG)
				{
					attr->type = T_ATTR::E_TYPE_FILE;
				}
				else
				{
					attr->type = T_ATTR::E_TYPE_UNKNOWN;
				}

				struct stat info;
				if (stat( entry->d_name, & info) == 0) 
				{
					attr->length = info.st_size;
					attr->lastModify = (uint64_t) info.st_mtime * (1000);
					attr->lastAccess = (uint64_t) info.st_atime * (1000);
				}
			}
			else
			{
				Directory::Close(dirHandle);
			}
#endif
		}
		return (attr->type);
	}

	//***************************说明*********************************
	//功    能：获得当前目录句柄指向目录的下一个文件的名称
	//输入参数：最大文件名长度(iLength)
	//输出参数：文件名(szName_out)，文件类型(eType)[可选]
	//返 回 值：成功返回文件类型 否则返回 -1 
	//****************************************************************
	int getNextFile(char szName_out[],size_t iLength,int * eType = NULL)
	{
		return Directory::GetNextFile(szName_out,iLength,m_DirHandle,eType);
	}
	int getNextFile(string & path_out,int * eType = NULL)
	{
		return Directory::GetNextFile(path_out,m_DirHandle,eType);
	}

	//***************************说明*********************************
	//功    能：查找指定文件在指定目录中的路径
	//输入参数：目标文件夹(path_in)、目标文件名(name_in),文件类型(eType)
	//输出参数：目标文件的路径(path_out)
	//返 回 值：成功返回 true 否则返回 false 
	//****************************************************************
	bool static Find(string & path_out,const string & path_in,const string & name_in,int eType)
	{
		bool bReturn = false;
		int type;
		string name;
		string current_path = path_in;
		if (current_path[current_path.length()-1] != E_SLASH)
		{
			current_path += E_SLASH;
		}
		DIR_T dirHandle = Directory::GetFirstFile(name,current_path,& type);
		while (name.length()>0)
		{
			if (type == eType && name.compare(name_in) == 0 )
			{
				bReturn = true;
				path_out = current_path + name;
				break;
			}
			else if(T_ATTR::E_TYPE_DIR == type)
			{
				current_path += name;
				if(Directory::Find(name,current_path,name_in,eType))
				{
					bReturn = true;
					path_out = name;
					break;
				}
			}
			name = "";
			Directory::GetNextFile(name,dirHandle,& type);
		}
		return bReturn;
	}

	bool static Find(string & path_out,char szPath_in[],char szName_in[],int eType)
	{
		bool bReturn = false;
		int type;
		string name;
		string current_path = szPath_in;
		if (current_path[current_path.length()-1] != E_SLASH)
		{
			current_path += E_SLASH;
		}
		DIR_T dirHandle = Directory::GetFirstFile(name,current_path,& type);
		while (name.length()>0)
		{
			if (type == eType && name.compare(szName_in) == 0 )
			{
				bReturn = true;
				path_out = current_path + name;
				break;
			}
			else if(T_ATTR::E_TYPE_DIR == type)
			{
				current_path += name;
				if(Directory::Find(name,current_path,szName_in,eType))
				{
					bReturn = true;
					path_out = name;
					break;
				}
			}
			name = "";
			Directory::GetNextFile(name,dirHandle,& type);
		}
		return bReturn;
	}

	bool static Find(char szPath_out[],size_t  iLength,const string & path_in,const string & name_in,int eType)
	{
		bool bReturn = false;
		int type;
		string name;
		string current_path = path_in;
		if (current_path[current_path.length()-1] != E_SLASH)
		{
			current_path += E_SLASH;
		}
		DIR_T dirHandle = Directory::GetFirstFile(name,current_path,& type);
		while (name.length()>0)
		{
			if (type == eType && name.compare(name_in) == 0 )
			{
				name = current_path + name;
				if (name.length()<iLength)
				{
					bReturn = true;
					name.copy(szPath_out,name.length());
				}
				else
				{
					M_WARNING("need more space to save the file name.");
				}
				break;
			}
			else if(T_ATTR::E_TYPE_DIR == type)
			{
				current_path += name;
				if(Directory::Find(name,current_path,name_in,eType))
				{
					if (name.length()<iLength)
					{
						bReturn = true;
						name.copy(szPath_out,name.length());
					}
					else
					{
						M_WARNING("need more space to save the file name.");
					}
					break;
				}
			}
			name = "";
			Directory::GetNextFile(name,dirHandle,& type);
		}
		return bReturn;
	}

	bool static Find(char szPath_out[],size_t  iLength,char szPath_in[],char szName_in[],int eType)
	{
		bool bReturn = false;
		int type;
		string name;
		string current_path = szPath_in;
		if (current_path[current_path.length()-1] != E_SLASH)
		{
			current_path += E_SLASH;
		}
		DIR_T dirHandle = Directory::GetFirstFile(name,current_path,& type);
		while (name.length()>0)
		{
			if (type == eType && name.compare(szName_in) == 0 )
			{
				name = current_path + name;
				if (name.length()<iLength)
				{
					bReturn = true;
					name.copy(szPath_out,name.length());
				}
				else
				{
					M_WARNING("need more space to save the file name.");
				}
				break;
			}
			else if(T_ATTR::E_TYPE_DIR == type)
			{
				current_path += name;
				if(Directory::Find(name,current_path,szName_in,eType))
				{
					if (name.length()<iLength)
					{
						bReturn = true;
						name.copy(szPath_out,name.length());
					}
					else
					{
						M_WARNING("need more space to save the file name.");
					}
					break;
				}
			}
			name = "";
			Directory::GetNextFile(name,dirHandle,& type);
		}
		return bReturn;
	}

	bool find(string & path_out,const string & name_in,int eType)
	{
		return Directory::Find(path_out,m_Root,name_in,eType);
	}

	bool find(string & path_out,char szName_in[],int eType)
	{
		return Directory::Find(path_out,m_Root.c_str(),szName_in,eType);
	}

	bool find(char szPath_out[],size_t  iLength,char szName_in[],int eType)
	{
		return Directory::Find(szPath_out,iLength,m_Root.c_str(),szName_in,eType);
	}

	//***************************说明*********************************
	//功    能：查找指定文件在指定目录中的路径
	//输入参数：目标文件夹(path_in)、目标文件名(name_in)
	//输出参数：目标文件的路径(path_out)
	//返 回 值：成功返回 true 否则返回 false 
	//****************************************************************
	static bool FindFile(char szPath_out[],size_t  length ,char szPath_in[],char szName_in[])
	{
		return Directory::Find(szPath_out,length,szPath_in,szName_in,T_ATTR::E_TYPE_FILE);
	}
	static bool FindFile(string & path_out ,char szPath_in[],char szName_in[])
	{
		return Directory::Find(path_out,szPath_in,szName_in,T_ATTR::E_TYPE_FILE);
	}
	static bool FindFile(char szPath_out[],size_t  iLength ,const string & path_in,const string & name_in)
	{
		return Directory::Find(szPath_out,iLength,path_in.c_str(),name_in.c_str(),T_ATTR::E_TYPE_FILE);
	}
	static bool FindFile(string & path_out ,const string & path_in,const string & name_in)
	{
		return Directory::Find(path_out,path_in,name_in,T_ATTR::E_TYPE_FILE);
	}

	//***************************说明*********************************
	//功    能：根据文件名查找目标文件路径。
	//输入参数：目标文件夹(szName_in)、最大文件名长度(iLength)
	//输出参数：目标文件的路径(szPath_out)
	//返 回 值：成功返回 true 否则返回 false 
	//****************************************************************
	bool findFile(char szPath_out[],size_t  iLength ,char szName_in[])
	{
		return Directory::Find(szPath_out,iLength,m_Root.c_str(),szName_in,T_ATTR::E_TYPE_FILE);
	}
	bool findFile(string & path_out ,char szName_in[])
	{
		return Directory::Find(path_out,m_Root.c_str(),szName_in,T_ATTR::E_TYPE_FILE);
	}
	bool findFile(char szPath_out[],size_t  iLength ,const string & name_in)
	{
		return Directory::Find(szPath_out,iLength,m_Root,name_in,T_ATTR::E_TYPE_FILE);
	}
	bool findFile(string & path_out ,const string & name_in)
	{
		return Directory::Find(path_out,m_Root,name_in,T_ATTR::E_TYPE_FILE);
	}

	//***************************说明*********************************
	//功    能：查找指定文件夹在指定目录中的路径
	//输入参数：目标文件夹(path_in)、目标文件名(name_in)
	//输出参数：目标文件夹的路径(path_out)
	//返 回 值：成功返回 true 否则返回 false 
	//****************************************************************
	static bool FindDir(char szPath_out[],size_t  length ,char szPath_in[],char szName_in[])
	{
		return Directory::Find(szPath_out,length,szPath_in,szName_in,T_ATTR::E_TYPE_DIR);
	}
	static bool FindDir(string & path_out ,char szPath_in[],char szName_in[])
	{
		return Directory::Find(path_out,szPath_in,szName_in,T_ATTR::E_TYPE_DIR);
	}
	static bool Directory::FindDir(char szPath_out[],size_t  iLength ,const string & path_in,const string & name_in)
	{
		return Directory::Find(szPath_out,iLength,path_in,name_in,T_ATTR::E_TYPE_DIR);
	}
	static bool Directory::FindDir(string & path_out ,const string & path_in,const string & name_in)
	{
		return Directory::Find(path_out,path_in,name_in,T_ATTR::E_TYPE_DIR);
	}

	//***************************说明*********************************
	//功    能：根据文件名查找目标文件夹路径。
	//输入参数：目标文件夹(szName_in)、最大文件名长度(iLength)
	//输出参数：目标文件夹的路径(szPath_out)
	//返 回 值：成功返回 true 否则返回 false 
	//****************************************************************
	bool findDir(char szPath_out[],char szName_in[],size_t  length)
	{
		return Directory::Find(szPath_out,length,m_Root.c_str(),szName_in,T_ATTR::E_TYPE_DIR);
	}
	bool findDir(string & path_out ,char szName_in[])
	{
		return Directory::Find(path_out,m_Root.c_str(),szName_in,T_ATTR::E_TYPE_DIR);
	}
	bool findDir(char szPath_out[],size_t  iLength ,const string & name_in)
	{
		return Directory::Find(szPath_out,iLength,m_Root,name_in,T_ATTR::E_TYPE_DIR);
	}
	bool findDir(string & path_out ,const string & name_in)
	{
		return Directory::Find(path_out,m_Root,name_in,T_ATTR::E_TYPE_DIR);
	}

	//***************************说明*********************************
	//功    能：获得给定目录以及子目录下所有文件的路径
	//输入参数：目标文件夹(path_in)、文件类型(eType)
	//输出参数：目标文件路径列表(fileList)
	//返 回 值：成功返回文件数，失败返回 -1
	//****************************************************************
	static int GetAll(T_NAME_LIST * fileList,const string & path_in,int eType) 
	{   
		assert(fileList != NULL);
		int type;
		int iReturn = -1;
		T_NAME_LIST::T_DATA *pNode = NULL;
		string name,current_path = path_in;
		T_DIR dirHandle = Directory::GetFirstFile(name,current_path,& type);
		if (dirHandle != NULL)
		{
			if (current_path[current_path.length()-1] != E_SLASH)
			{
				current_path += E_SLASH;
			}
			iReturn = 0;
			if (eType == type && name.compare(".") != 0 && name.compare("..") != 0)
			{
				pNode = new T_NAME_LIST::T_DATA;
				if (pNode == NULL)
				{
					M_WARNING("NO MORE MEMORY.");
					return iReturn;
				}
				iReturn = 1;
				pNode->path = current_path;
				pNode->name = name;
				fileList->putOne(pNode);
			}

			while (Directory::GetNextFile(name,dirHandle,& type) > 0)
			{
				if (name.compare(".") ==0 || name.compare("..") == 0)
				{
					continue;
				}
				if (type == eType )
				{
					pNode = new T_NAME_LIST::T_DATA;
					if (pNode == NULL)
					{
						M_WARNING("NO MORE MEMORY.");
						return iReturn;
					}
					iReturn++;
					pNode->path = current_path;
					pNode->name = name;
					fileList->putOne(pNode);
				}
				if(T_ATTR::E_TYPE_DIR == type)
				{
					current_path += name;
					size_t nCount = GetAll(fileList,current_path,eType);
					if (nCount == -1)
					{
						while (fileList->getOne(&pNode))
						{
							delete pNode;
						}
						iReturn = -1;
						break;
					}
					else
					{
						iReturn += nCount;
					}
				}
			}
		}
		return iReturn;
	}

	static int GetAll(T_NAME_LIST * fileList,T_NAME_LIST * dirList,const string & path_in) 
	{   
		assert(fileList != NULL);
		int type;
		int iReturn = -1;
		T_NAME_LIST::T_DATA *pNode = NULL;
		string name,current_path = path_in;
		T_DIR dirHandle = Directory::GetFirstFile(name,current_path,&type);
		if (dirHandle != NULL)
		{
			if (current_path[current_path.length()-1] != E_SLASH)
			{
				current_path += E_SLASH;
			}
			
			iReturn = 0;
			if (name.compare(".") != 0 && name.compare("..") != 0)
			{
				pNode = new T_NAME_LIST::T_DATA;
				if (pNode == NULL)
				{
					M_WARNING("NO MORE MEMORY.");
					return iReturn;
				}
				iReturn = 1;
				pNode->path = current_path;
				pNode->name = name;
				if (T_ATTR::E_TYPE_DIR == type)
				{
					dirList->putOne(pNode);
				}
				else
				{
					fileList->putOne(pNode);
				}
			}

			while (Directory::GetNextFile(name,dirHandle,& type) > 0)
			{
				if (name.compare(".") ==0 || name.compare("..") == 0)
				{
					continue;
				}
				
				if (type == T_ATTR::E_TYPE_FILE)
				{
					pNode = new T_NAME_LIST::T_DATA;
					if (pNode == NULL)
					{
						M_WARNING("NO MORE MEMORY.");
						return iReturn;
					}
					iReturn++;
					pNode->path = current_path;
					pNode->name = name;
					fileList->putOne(pNode);
				}
				else if(type == T_ATTR::E_TYPE_DIR)
				{
					pNode = new T_NAME_LIST::T_DATA;
					if (pNode == NULL)
					{
						M_WARNING("NO MORE MEMORY.");
						return iReturn;
					}
					iReturn++;
					pNode->path = current_path;
					pNode->name = name;
					dirList->putOne(pNode);
				}
				else
				{
					cout<<"path:"<<pNode->path<<endl;
					cout<<"name:"<<pNode->name<<endl;
					cout<<"enum size:"<<sizeof(T_ATTR::E_TYPE_FILE)<<endl;
					cout<<"int size:"<<sizeof(type)<<endl;
					cout<<"E_TYPE_FILE :"<<T_ATTR::E_TYPE_FILE<<endl;
					cout<<"type:"<<type<<endl;
				}
				if(T_ATTR::E_TYPE_DIR == type)
				{
										
					size_t nCount = Directory::GetAll(fileList,dirList,current_path + name);
					if (nCount == -1)
					{
						M_WARNING("Can't find the path :%s",current_path.c_str());
						while (fileList->getOne(&pNode))
						{
							delete pNode;
						}
						while (dirList->getOne(&pNode))
						{
							delete pNode;
						}
						iReturn = -1;
						break;
					}
					else
					{
						iReturn += nCount;
					}
				}

			}
		}
		return iReturn;
	}

	static int GetAll(T_NAME_LIST * fileList,T_NAME_LIST * dirList,char szPath_in[]) 
	{   
		assert(fileList != NULL);
		int type;
		int iReturn = -1;
		T_NAME_LIST::T_DATA *pNode = NULL;
		string name,current_path = szPath_in;
		T_DIR dirHandle = Directory::GetFirstFile(name,current_path,&type);
		if (dirHandle != NULL)
		{
			if (current_path[current_path.length()-1] != E_SLASH)
			{
				current_path += E_SLASH;
			}

			iReturn = 0;
			if (name.compare(".") != 0 && name.compare("..") != 0)
			{
				pNode = new T_NAME_LIST::T_DATA;
				if (pNode == NULL)
				{
					M_WARNING("NO MORE MEMORY.");
					return iReturn;
				}
				iReturn = 1;
				pNode->path = current_path;
				pNode->name = name;
				if (T_ATTR::E_TYPE_DIR == type)
				{
					dirList->putOne(pNode);
				}
				else
				{
					fileList->putOne(pNode);
				}
			}

			while (Directory::GetNextFile(name,dirHandle,& type) > 0)
			{
				if (name.compare(".") ==0 || name.compare("..") == 0)
				{
					continue;
				}
				if (type == T_ATTR::E_TYPE_FILE)
				{
					pNode = new T_NAME_LIST::T_DATA;
					if (pNode == NULL)
					{
						M_WARNING("NO MORE MEMORY.");
						return iReturn;
					}
					iReturn++;
					pNode->path = current_path;
					pNode->name = name;
					fileList->putOne(pNode);
				}
				else if(type == T_ATTR::E_TYPE_DIR)
				{
					pNode = new T_NAME_LIST::T_DATA;
					if (pNode == NULL)
					{
						M_WARNING("NO MORE MEMORY.");
						return iReturn;
					}
					iReturn++;
					pNode->path = current_path;
					pNode->name = name;
					dirList->putOne(pNode);
				}
				if(T_ATTR::E_TYPE_DIR == type)
				{
					current_path += name;

					size_t nCount = Directory::GetAll(fileList,dirList,current_path);
					if (nCount == -1)
					{
						while (fileList->getOne(&pNode))
						{
							delete pNode;
						}
						while (dirList->getOne(&pNode))
						{
							delete pNode;
						}
						iReturn = -1;
						break;
					}
					else
					{
						iReturn += nCount;
					}
				}

			}
		}
		return iReturn;
	}

	static int GetAll(T_NAME_LIST * fileList,const string & path_in) 
	{   
		assert(fileList != NULL);
		int type;
		int iReturn = -1;
		T_NAME_LIST::T_DATA *pNode = NULL;
		string name,current_path = path_in;
		T_DIR dirHandle = Directory::GetFirstFile(name,current_path,& type);
		if (dirHandle != NULL )
		{
			if (current_path[current_path.length()-1] != E_SLASH)
			{
				current_path += E_SLASH;
			}
			
			iReturn = 0;
			if (name.compare(".") != 0 && name.compare("..") != 0)
			{
				pNode = new T_NAME_LIST::T_DATA;
				if (pNode == NULL)
				{
					M_WARNING("NO MORE MEMORY.");
					return iReturn;
				}
				iReturn = 1;
				pNode->path = current_path;
				pNode->name = name;
				fileList->putOne(pNode);
			}

			while (Directory::GetNextFile(name,dirHandle,& type) > 0)
			{
				if (name.compare(".") ==0 || name.compare("..") == 0)
				{
					continue;
				}
				pNode = new T_NAME_LIST::T_DATA;
				if (pNode == NULL)
				{
					M_WARNING("NO MORE MEMORY.");
					return iReturn;
				}
				iReturn++;
				pNode->path = current_path;
				pNode->name = name;
				fileList->putOne(pNode);
				if(T_ATTR::E_TYPE_DIR == type)
				{
					current_path += name;

					size_t nCount = Directory::GetAll(fileList,current_path);
					if (nCount == -1)
					{
						while (fileList->getOne(&pNode))
						{
							delete pNode;
						}
						iReturn = -1;
						break;
					}
					else
					{
						iReturn += nCount;
					}
				}
			}
		}
		return iReturn;
	}

	//---------------------------------------------------
	//功能：删除指定文件夹
	//返回值：成功返回true，否则返回 false 
	//---------------------------------------------------
	bool remove()
	{
		return Directory::Remove(m_Root);
	}

	static bool Remove(const string & name)
	{
		return Directory::Remove(name.c_str());
	}

	static bool Remove(char szName[])
	{
		// 这个只能做一般的情况，并没有考虑复杂的情况
		bool bReturn = true;
		int eType = File::GetFileType(szName) ;
		if ( File::T_ATTR::E_TYPE_FILE == eType)
		{
			if ( File::Remove(szName,eType) != 0)
			{
				bReturn = false;
			}
		}
		else if ( File::T_ATTR::E_TYPE_DIR == eType )
		{
			string dir_name;
			T_DIR dirHandle =  Directory::GetFirstFile(dir_name,szName);
			
			while( dir_name.length() > 0 )
			{
				if ( dir_name != "." && dir_name != ".." )
				{
					string subdir = szName;
					subdir += M_SLASH;
					subdir += dir_name;

					bReturn &= Directory::Remove( subdir.c_str());
				}
				Directory::GetNextFile(dir_name,dirHandle);
			}
			if ( File::Remove(dir_name,eType) != 0)
			{
				bReturn = false;
			}
		}
		else
		{
			bReturn = false;
		}
		
		return bReturn;
	}

private:
	static int MakeDir(const char * szPath)
	{
		assert(szPath);
		int iReturn =0;
#if( M_OS_TYPE == M_WINDOWS_OS )
		iReturn = _mkdir(szPath);
#else
		/***************************
		* sys/stat.h
		* S_IRUSR   用户-读   
		* S_IWUSR   用户-写   
		* S_IXUSR   用户-执行   
		* S_IRGRP   组-读   
		* S_IWGRP   组-写   
		* S_IXGRP   组-执行   
		* S_IROTH   其他-读   
		* S_IWOTH   其他-写   
		* S_IXOTH   其他-执行
		***************************/
		iReturn = mkdir(szPath,S_IRUSR|S_IWUSR|S_IXUSR);
#endif
		return iReturn ;
	}

	static int Close(T_DIR dirHandle)
	{
		long lReturn = 0;
		if ( dirHandle != NULL )
		{
#if (M_OS_TYPE == M_WARNING)
			lReturn = FindClose(dirHandle);
			if(lReturn != 0)
			{
				lReturn = GetLastError();
			}
#else
			lReturn = closedir(dirHandle);
#endif
			dirHandle = NULL;
		}
		return lReturn;
	}

	int close()
	{
		return Close(m_DirHandle);
	}
};

M_END_NAMESPACE(mylib)

#endif //_MYLIB_DIRECTORY_H_INCLUDE_20090909
#endif
