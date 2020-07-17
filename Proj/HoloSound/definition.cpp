#include "definition.h"



#if 0   //以下函数没有被用到，且不是跨平台的屏蔽 remove by lizulin 

#include <Windows.h>
//using namespace std;
string toUTF8(const wchar_t* buffer, int len)  
{  
	int size = ::WideCharToMultiByte(CP_UTF8, 0, buffer, len, NULL, 0, NULL,  
		NULL);  
	if (size == 0)  
		return "";  

	std::string newbuffer;  
	newbuffer.resize(size);  
	::WideCharToMultiByte(CP_UTF8, 0, buffer, len,  
		const_cast<char*>(newbuffer.c_str()), size, NULL, NULL);  

	return newbuffer;  
}  

string ToUTF8(const std::wstring& str)  
{  
	return toUTF8(str.c_str(), (int) str.size());  
}  

wstring StringToWString(string s)
{   
	std::wstring wszStr; 
	int nLength = MultiByteToWideChar( CP_ACP, 0, s.c_str(), -1, NULL, NULL );
	wszStr.resize(nLength);
	LPWSTR lpwszStr = new wchar_t[nLength];
	MultiByteToWideChar( CP_ACP, 0, s.c_str(), -1, lpwszStr, nLength );
	wszStr = lpwszStr;
	delete [] lpwszStr;

	return wszStr;
}

bool ReadUtf8ToString(const string &utf8str, string &str)
{  
	char buff[400];  
	//取得所有UTF-8字节  
	//先把UTF-8转换为UNICODE  
	strcpy(buff, utf8str.data());
	int num = MultiByteToWideChar(CP_UTF8,0,buff,-1,NULL,0);  
	if(num <= 0){  
		cerr << "UTF-8 Converting to UNICODE error"<< endl;  
		return false;  
	}  
	wchar_t *buffw2=new wchar_t[num];  
	if(buffw2 == NULL){  
		cerr << "ReadUtf8ToString: Memory allocate error "<< endl;  
		return false;  
	}  
	MultiByteToWideChar(CP_UTF8,0,buff,-1,buffw2,num);  

	//再将Unicode转换为ANSI字符串  
	int len = WideCharToMultiByte(CP_ACP, 0, buffw2, num, NULL, 0, NULL, NULL);  
	if(len <= 0){  
		cerr << "UNICODE Converting to ANSIC error"<< endl;  
		return false;  
	}  
	LPSTR lpsz = new CHAR[len+1];  
	if(lpsz == NULL){  
		cerr << "ReadUtf8ToString: Memory allocate error"<< endl;  
		return false;  
	}  
	WideCharToMultiByte(CP_ACP, 0, buffw2, num, lpsz, len, NULL, NULL);  
	lpsz[len]='\0';  
	str += string(lpsz);  
	delete []buffw2;  
	delete []lpsz;  
	return true;  
}  
#endif




