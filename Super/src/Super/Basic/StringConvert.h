/******************************************************************************
*File   : String.h
*Author : Lan Hu
*Email	: Hi_Blue@126.com
*Notes	:
*Licence: 
*Created: 2009-09-15
*******************************************************************************/

#ifndef _MYLIB_STRING_H__INLCLUED_20100201_
#define _MYLIB_STRING_H__INLCLUED_20100201_


////////////////////////////////////////////////////////////////////////////////
//
//  ConvertUTF
//
//  Description - Convert between UTF-8 & UTF-16, and convert \uhhhh to UTF-16
//  Author      - Li Ming Jie (Martin)
// Email - limingjie0719@gmail.com
//  Created     - 2008.03
//  Licence     - Free
//  Copyright   - Free
//  Version     - 
//  Changes     - 
//    2008.07.17 Add function convert_useq_to_UTF16 for converting \uhhhh
//               sequence to UTF-16
//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <wchar.h>

unsigned long FirstByteMasks[4] = {0x00UL, 0xC0UL, 0xE0UL, 0xF0UL};

size_t Convert_UTF8_To_UTF16(const char *src, wchar_t *dest);
size_t Convert_UTF16_To_UTF8(const wchar_t *src, char *dest);
size_t Convert_useq_To_UTF16(const char *src, wchar_t *dest);



size_t Convert_UTF8_To_UTF16(const char *src, wchar_t *dest)
{    
	unsigned long c;    
	size_t extra_bytes;    
	size_t len = 0;    

	while(*src)    
	{        
		c = (unsigned long)*src++ & 0xFFUL;                
		if((c & 0x80UL) == 0UL) //1 octet       
		{            
			*dest++ = (wchar_t)c;            
			len++;            
			continue;        
		}        
		else if((c & 0xE0UL) == 0xC0UL) //2 octet        
		{            
			c -= 0xC0UL;            
			extra_bytes = 1;        
		}        
		else if((c & 0xF0UL) == 0xE0UL) // 3 octet        
		{            
			c -= 0xE0UL;            
			extra_bytes = 2;        
		}        
		else if((c & 0xF8UL) == 0xF0UL) // 4 octet        
		{            
			c -= 0xF0UL;            
			extra_bytes = 3;        
		}        
		else        
		{            //5 or 6 octets cannot be converted to UTF-16            
			return 0;        
		}                
		while(extra_bytes)        
		{            
			if(*src == 0) 
				return 0; //unexpected end of string            
			if((*src & 0xC0UL) != 0x80UL) 
				return 0; //illegal trailing byte
			c <<= 6;            
			c += (unsigned long)*src++ & 0x3FUL;                        
			extra_bytes--;        
		}                
		if(c < 0x10000UL)        
		{            
			//value between 0xD800 and 0xDFFF are preserved for UTF-16 pairs            
			if(c >= 0xD800UL && c <= 0xDFFFUL) 
				return 0;                        
			*dest++ = (wchar_t)c;            
			len++;        
		}        
		else        
		{            
			c -= 0x10000UL;                        
			//value greater than 0x10FFFF, illegal UTF-16 value;            
			if(c >= 0x100000UL) return 0;                         
			*dest++ = (wchar_t)(0xD800UL + (c >> 10));            
			*dest++ = (wchar_t)(0xDC00UL + (c & 0x3FFUL));                        
			len += 2;         
		}    
	}        
	*dest = (wchar_t)'\0';        
	return len;
}

size_t Convert_UTF16_To_UTF8(const wchar_t *src, char *dest)
{    
	unsigned long c;    
	size_t extra_bytes;
	size_t len = 0;        
	while(*src)    
	{        
		c = (unsigned long)*src++ & 0xFFFFUL;               
		//convert UTF-16 literal to UCS-4        
		if(c >= 0xD800UL && c <= 0xDBFFUL)        
		{            
			c &= 0x03FFUL;            
			c <<= 10;                        
			unsigned long c1 = (unsigned long)*src++ & 0xFFFFUL;                        
			if(c1 >= 0xDC00UL && c1 <= 0xDFFFUL)            
			{                
				c += c1 & 0x03FFUL;            
			}            
			else            
			{                
				return 0;            
			}        
		}        
		else if(c >= 0xDC00UL && c <= 0xDFFFUL)        
		{            
			return 0;        
		}                
		//convert UCS-4 literal to UTF-8        
		if(c < 0x80UL) 
			extra_bytes = 0;        
		else if(c < 0x800UL) extra_bytes = 1;        
		else if(c < 0x10000UL) extra_bytes = 2;        
		else extra_bytes = 3;                
		dest += extra_bytes;
		switch(extra_bytes)        
		{        
		case 3 : *dest-- = (char)((c | 0x80UL) & 0xBFUL); c >>= 6;        
		case 2 : *dest-- = (char)((c | 0x80UL) & 0xBFUL); c >>= 6;        
		case 1 : *dest-- = (char)((c | 0x80UL) & 0xBFUL); c >>= 6;        
		case 0 : *dest = (char)(c | FirstByteMasks[extra_bytes]);        
		}                
		extra_bytes++;        
		dest += extra_bytes;        
		len += extra_bytes;    
	}        
	*dest = '\0';    
	return len;
}

size_t Convert_useq_To_UTF16(const char *src, wchar_t *dest)
{    
	int i = 0, j;    
	size_t len = strlen(src);        
	unsigned int hex;    
	size_t size = 0;        
	while(1)    
	{        
		//Test if sequence start with '\'       
		//Also test if sequence ends        
		if(src[i++] != '\\') break;                //Test if enough bytes exist        
		if(len - i < 5) break;        //Test sequence type, 'u'        
		if(src[i++] != 'u') break;                //process hhhh        
		dest[size] = 0;        
		for(j = 0; j < 4; j++)        
		{            
			if(src[i] >= '0' && src[i] <= '9')            
			{                
				hex = src[i] - '0';            
			}            
			else if(src[i] >= 'a' && src[i] <= 'f')            
			{                
				hex = src[i] - 'a' + 10;            
			}            
			else if(src[i] >= 'A' && src[i] <= 'F')            
			{                
				hex = src[i] - 'A' + 10;            
			}            
			else            
			{                
				break;            
			}                        
			dest[size] <<= 4;            
			dest[size] += hex;            
			i++;        
		}                
		size++;    
	}        
	dest[size] = '\0';    
	return size;
}



#include <wchar.h>
#include <string>
#include "public.h"

using std::string;
using std::wstring;


M_BEGIN_NAMESPACE(mylib)


#define M_DEFAULT_STRING_LEN 1024
#define M_DEFAULT_STRBUF_LEN 1024

typedef char T_ASCII;
typedef wchar_t T_UNICODE;


template <typename T_CODE = T_ASCII,size_t iMAX_SIZE = M_DEFAULT_STRING_LEN>
class String
{
public:
	enum{E_MAX_SIZE = iMAX_SIZE};
	enum{E_EOS = 0 , E_EOL = '\n'};
	enum{E_UTF8 = 1 , E_UTF16 = 2};
	typedef T_CODE T_CHAR;

public:

	String(void)
	{
		init();

	}

	String(String<T_CODE> & str)
	{
		init();
		m_Length = str.length();
		for (int i=0;i<= m_Length;i++)
		{
			m_Array[i] = str[i];
		}
		
	}

	String(string & str)
	{
		init();
		if (E_UTF16 == m_Code)
		{
			int i=0;
			m_Length = 0;
			while (str[i++] != E_EOS)
			{
				if (str[i] >0)
				{
					m_Array[m_Length] = str[i];
				}
				else
				{
					m_Array[m_Length] = (*(wchar_t*)&str[i++]);
				}
				++m_Length;

			}
		}
		else
		{
			m_Length = str.length();
			memcpy(m_Array,str.c_str(),m_Length);
			m_Array[m_Length] = E_EOS;
		}
		
		
	}

	String(wstring & str)
	{
		init();

		assert(E_UTF16 == m_Code);

		m_Length = str.length();

		for (int i=0;i<= m_Length;i++)
		{
			m_Array[i] = str[i];
		}
	}

	String(char * str)
	{
		assert(str);

		init();

		if (E_UTF16 == m_Code)
		{
			int i=0;
			m_Length = Convert_UTF8_To_UTF16(str,m_Array);
// 			while (str[i] != E_EOS)
// 			{
// 				if (str[i] >0)
// 				{
// 					m_Array[m_Length] = str[i];
// 				}
// 				else
// 				{
// 					
// 					UTF8_TO_UCS2(&str[i++],&m_Array[m_Length]);
// 				}
// 				++i;
// 				++m_Length;
// 			}
		}
		else
		{
			m_Length = strlen(str);
			memcpy(m_Array,str,m_Length);
			m_Array[m_Length] = E_EOS;
		}
				
	}

	String(wchar_t * str)
	{
		init();

		if(E_UTF16 == m_Code)
		{
			m_Length = Length(str);

			memcpy(m_Array,str,m_Length * m_Code);
			m_Array[m_Length] = E_EOS;
		}
		else
		{
			int i=0;
			m_Length = 0;
			while (str[i] != E_EOS)
			{
				if (str[i] >0x0080)
				{
					m_Array[m_Length] = str[i];
					++m_Length;
				}
				else
				{
					m_Length += UCS2_TO_UTF8(str[i],&m_Array[m_Length]);
				}
				++i;
				
			}
		}
	}

	virtual ~String(void)
	{
	}

	void init()
	{
		m_Code = sizeof(T_CHAR);
		memset(m_Array,0,E_MAX_SIZE * m_Code);
		m_Length = 0;
		
	}

	const T_CHAR * getArray()
	{
		return m_Array;
	}

	template <class T> static size_t Length( T * str )
	{
		size_t nLength = 0;
		while (*str++)
		{
			nLength++;
		}
		return nLength;
	}
	
	size_t length()
	{
		return Length(m_Array);
	}

	bool compare(T_CHAR * str)
	{
		return Compare(m_Array,str);
	}

	bool compare(String<T_CODE> & str)
	{
		return Compare(m_Array,str.getArray());
	}

	bool operator == (T_CHAR * str)
	{
		return Compare(m_Array,str);
	}
	
	bool operator == (String<T_CODE> & str)
	{
		return Compare(m_Array,str.getArray());
	}
	
	static bool Compare(T_CHAR * strA,T_CHAR * strB)
	{
		while (*strA)
		{
			if (*strA != *strB)
			{
				break;
			}
			else
			{
				strA++;
				strB++;
			}
		}
		return !(*strA||*strB);
	}

	static int Concat(T_CHAR * strA,T_CHAR * strB)
	{
		int pos = 0;
		while (strA[pos++]){}
		while (*strB)
		{
			strA[pos] = *strB++;
		}
		strA[pos] = E_EOS;
		return pos;
	}

	int concat(T_CHAR * str)
	{
		return Concat(m_Array,str);
	}

	int concat(String<T_CODE> & str)
	{
		return Concat(m_Array,str.getArray());
	}

	int operator + (T_CHAR * str)
	{
		return Concat(m_Array,str);
	}

	int operator + (String<T_CODE> & str)
	{
		return Concat(m_Array,str.getArray());
	}

	static bool Insert(T_CHAR * strA,size_t pos,T_CHAR * strB)
	{
		int lenA=0,lenB=0;
		T_CHAR c = 0;

		while(strA[lenA++]){}

		if (lenA<pos) return false;

		while(strA[lenB++]){}

		for(int i=0;i<lenB;i++)
		{
			c = strA[pos+i];
			strA[pos+i] = strB[i];
			strA[pos+lenB+i] = c;
		}
		strA[lenA+lenB] = E_EOS;
		return true;
	}

	bool insert(size_t pos,T_CHAR * str)
	{
		return Insert(m_Array,pos,str);
	}

	bool insert(size_t pos,String<T_CODE> & str )
	{
		return Insert(m_Array,pos,str.getArray());
	}

	T_CHAR operator [] (size_t i)
	{
		if (i>=iMAX_SIZE)
		{
			return E_EOS;
		}
		return m_Array[i];
	}


	static bool UTF8_TO_UCS2(const char* utf8_code, wchar_t* ucs2_code)
	{
		wchar_t temp1, temp2;
		bool is_unrecognized = false ;
		const wchar_t * in = (const wchar_t *)utf8_code;
		if(!utf8_code || !ucs2_code)
		{
			return is_unrecognized;
		}
		if(0x00 == (*in & 0x80))
		{
			/* 1 byte UTF-8 Charater.*/
			*ucs2_code= (wchar_t)*in;
			is_unrecognized = true;
		}
		else if(0xc0 == (*in & 0xe0) &&0x80 == (*(in + 1) & 0xc0))
		{
			/* 2 bytes UTF-8 Charater.*/ 
			temp1 = (wchar_t)(*in & 0x1f);
			temp1 <<= 6;
			temp1 |= (wchar_t)(*(in + 1) & 0x3f);
			*ucs2_code = temp1;
			is_unrecognized = true;
		}
		else if( 0xe0 == (*in & 0xf0) && 0x80 == (*(in +1) & 0xc0) && 0x80 == (*(in + 2) & 0xc0))
		{
			/* 3bytes UTF-8 Charater.*/
			temp1 = (wchar_t)(*in &0x0f);
			temp1 <<= 12;
			temp2 = (wchar_t)(*(in+1) & 0x3F);
			temp2 <<= 6;
			temp1 = temp1 | temp2 | (wchar_t)(*(in+2) & 0x3F);
			*ucs2_code = temp1;
			is_unrecognized = true;
		}
		else
		{
			/* unrecognize byte. */ 
			*ucs2_code = 0x22e0;
			is_unrecognized = false;
		}
		return is_unrecognized;
	}

	//　2,把一个UCS-2字符转换成UTF-8字符。函数返回转换成UTF-8的长度(字节1 -- 3)，如果目标指针为空，返回0。
	static byte UCS2_TO_UTF8(wchar_t ucs2_code, char* utf8_code)
	{
		int length = 0;
		char* out = utf8_code;
		if(!utf8_code)
		{
			return length;
		}
		if(0x0080 > ucs2_code)
		{
			/* 1 byte UTF-8 Character.*/
			*out = (char)ucs2_code;
			length++; 
		}
		else if(0x0800 > ucs2_code)
		{
			/*2 bytes UTF-8 Character.*/
			*out = ((char)(ucs2_code >> 6)) | 0xc0;
			*(out+1) = ((char)(ucs2_code & 0x003F)) | 0x80;
			length += 2;
		}
		else
		{
			/* 3 bytes UTF-8 Character .*/
			*out = ((char)(ucs2_code >> 12)) | 0xE0;
			*(out+1) = ((char)((ucs2_code & 0x0FC0)>> 6)) | 0x80;
			*(out+2) = ((char)(ucs2_code & 0x003F)) | 0x80;
			length += 3; 
		}
		return length;
	}

private:

	T_CHAR m_Array[iMAX_SIZE];
	size_t m_Length;
	char m_Code;
};



M_END_NAMESPACE(mylib)




#endif //_MYLIB_STRING_H__INLCLUED_20100201_

