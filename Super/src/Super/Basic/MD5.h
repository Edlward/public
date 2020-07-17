#ifndef __MD5_Heafer__
#define __MD5_Header__

#include "global.h" 

typedef struct
{
	unsigned int count[2];
	unsigned int state[4];
	unsigned char buffer[64];   
}MD5_CTX;


void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputlen);
void MD5Final(MD5_CTX *context, unsigned char digest[16]);
void MD5Transform(unsigned int state[4], unsigned char block[64]);
void MD5Encode(unsigned char *output, unsigned int *input, unsigned int len);
void MD5Decode(unsigned int *output, unsigned char *input, unsigned int len);


//二进制形式MD5为16字节，字符串形式32字节+'\0' 共33字节

//封装
void md5_to_string(char md5_value[16],char md5_str[33]);
void getMD5FromBuffer(const char *buf, unsigned int len,char md5_value[16]); //计算缓存的MD5值，结果保存以二进制保存
void getMD5StrFromBuffer(const char *buf, unsigned int len,char md5_str[33]); //计算缓存的MD5值，结果以字符串保存 MD5 字符串长度=32

bool getMD5FromFile(const char *file_path, char md5_value[16]); //计算文件的MD5值，结果保存以二进制保存 失败返回false 
bool getMD5StrFromFile(const char *file_path, char md5_str[33]); //计算文件的MD5值，结果以字符串保存 MD5 字符串长度=32 

#ifdef UNIT_TEST
BASIC_API int Test_MD5(void);//未完待续
#endif


#endif
