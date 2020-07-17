#include "MyCLib.h"
#include<assert.h>
#include<stdlib.h>
//#include<iostream>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

#include "../Basic/SuperTime.h"

USING_NAME_SPACE_MAIN

//实现1
void* MyMemcpyV1(void *pDst,const void *pSrc,size_t len)
{
    assert((pDst!=NULL)&&(pSrc!=NULL));                //普通断言
    //assert（pDst>=pSrc+num||pSrc>pDst+num);    //考虑内存重叠的断言

    const char *pS  =(const char*)(pSrc);
    char *pD = (char*)(pDst);
    while(len-->0)
    {
        *pD++ = *pS++;
    }
    return pDst;
}


//缺点：没有考虑内存重叠的情况，可以加一个断言换为：assert（des>=src+num||src>dst+num）;
//实现2：考虑重叠，有重叠情况也复制
void * MyMemcpyV2(void *pDst, const void *pSrc, size_t len)
{
    assert((pDst!=NULL)&&(pSrc!=NULL));
    const char *pS  =(const char*)(pSrc);
    char *pD = (char*)(pDst);
     size_t  n = len;
    if (pD > pS && pD < pS+len)   //有内存重叠，从高地址开始复制
    {  
        for (size_t i=n-1; i != -1; --i)
        {
            pD[i] = pS[i];
        }
    }
    else
    {
        for (size_t i= 0; i < n; i++)
        {
            pD[i] = pS[i];
        }
    }
    return pDst;
}


//改进循环计数方式
void* MyMemcpy_FastV1(void *pDst,const void *pSrc,size_t len)  
{  
    assert((pDst!=NULL)&&(pSrc!=NULL));  //添加考虑内存重叠

    typedef size_t wordType;
    const wordType* pS = (const wordType*)pSrc;  
    wordType* pD = (wordType *)pDst; 

    size_t LoopNum = len/sizeof(wordType);
    for (size_t n=0;n<LoopNum;n++)
    {
        *pD++ = *pS++; 
    }

    LoopNum*=sizeof(wordType);
    for (size_t n=LoopNum;n<len;n++)
    {
        *((char *)pD++) =*((char *)pS++);  
    }
    return pDst;  
}



//
////结论:比逐字节拷贝快，但是比系统函数慢
//void* MyMemcpy_Fast(void *pDst,const void *pSrc,size_t len)  
//{  
//    //assert((pDst!=NULL)&&(pSrc!=NULL));  //添加考虑内存重叠
//    typedef int64_t wordType;     //typedef lnt64 AlignType;  //控制每次拷贝的字长   int64_t比int位快10倍
//    wordType * pS = (wordType *)pSrc;  
//    wordType* pD = (wordType *)pDst;  
//
//   #define ParallelN (1)    //while循环里的并行组数 一般1-4；//经测1最快
//    unsigned int wordnum = len/(sizeof(wordType)*ParallelN);  
//    unsigned int slice = len%(sizeof(wordType)*ParallelN);         //剩余的按字节拷贝  
//    while (wordnum)
//    {
//        //pD[0] = pS[0]; 
//        //pD[1] = pS[1]; 
//        //pD[2] = pS[2]; 
//        //pD[3] = pS[3]; 
//        //pD+=4;
//        //pS+=4;
//
//        *pD++ = *pS++; 
//#if ParallelN>1
//        *pD++ = *pS++;  
//#endif
//
//#if ParallelN>2
//        *pD++ = *pS++;  
//#endif
//
//#if ParallelN>3
//        *pD++ = *pS++;  
//#endif
//        wordnum--;
//    }
//
//    while (slice--)
//    {
//        *((char *)pD++) =*((char *)pS++);  
//    }
//    return pDst;  
//}




//具体参见 达夫设备（Duff's Device）
void* memcpy_duff( char* to,const char* from,size_t count ) 
{
    size_t n = (count+7)/8;
    switch( count%8 ) 
    {
    case 0: do{ *to++ = *from++;
    case 7:     *to++ = *from++;
    case 6:     *to++ = *from++;
    case 5:     *to++ = *from++;
    case 4:     *to++ = *from++;
    case 3:     *to++ = *from++;
    case 2:     *to++ = *from++;
    case 1:     *to++ = *from++;
            }while(--n>0);
    }
    return to;
}
//当count==0时，还会至少do一次，实际上是错误的操作。



//
void* MyMemsetV1(void *pSrc,int c,size_t len) 
{ 
    const char uc = c; 
    char *su; 
    for(su = (char*)pSrc;0 < len;++su,--len) 
    {
        *su = uc; 
    }
    return pSrc; 
}

void* MyMemsetV2(void *pSrc, int c, size_t len)
{
    assert(pSrc!=NULL);
    char *tmpsrc=(char*)pSrc;
    const char uc = c; 
    while(len--)
    {
        *tmpsrc++ =uc;
    }
    return pSrc;
}

//结论:比逐字节拷贝快，但是比系统函数慢
void* MyMemset_FastV1(void *pSrc,int c,size_t len) 
{ 
    typedef int64_t wordType;       //控制每次拷贝的字长   int64_t比int位快1倍
    wordType Value;                       //把char 型字符拼接成wordType,然后可以直接赋值
    char* p=(char*)&Value;
    char uc=c;
    for (int i=0;i<sizeof(wordType)/sizeof(uc);i++)
    {
        *p++=uc;
    }
    unsigned int wordnum = len/(sizeof(wordType));    //
    unsigned int slice = len%(sizeof(wordType));          //剩余的按字节拷贝  
    wordType * ptmpsrc = (wordType *)pSrc;  

    while (wordnum--)
    {
        *ptmpsrc++=Value;  
    }
    while (slice--)
    {
        *((char *)ptmpsrc++)=c;  
    }
    return pSrc;  
} 
//结论:比逐字节拷贝快，但是比系统函数慢
void* MyMemset_FastV2(void *pDst,int c,size_t len) 
{ 
    typedef int64_t wordType;       //控制每次拷贝的字长   int64_t比int位快1倍
    wordType Value;                       //把char 型字符拼接成wordType,然后可以直接赋值
    char* p=(char*)&Value;
    char uc=c;
    for (int i=0;i<sizeof(wordType)/sizeof(uc);i++)
    {
        *p++=uc;
    }

    char *pBegin;
    //走到内存地址对齐的位置---待续
    for (pBegin =(char*)pDst; ((wordType) pBegin & (sizeof(wordType) - 1)) != 0;++pBegin)
    {
        *pBegin=uc;
        len--;   //len<0 考虑 待续
    }

    wordType * ptmpsrc = (wordType *)pBegin;  
    size_t LoopNum = len/sizeof(wordType);
    for (size_t n=0;n<LoopNum;n++)
    {
        *ptmpsrc++ =Value; 
    }

    LoopNum*=sizeof(wordType);
    for (size_t n=LoopNum;n<len;n++)
    {
        *((char *)ptmpsrc++)=uc; 
    }
    return pDst;  
} 


//glibc2.7 
//考虑到效率的因素：
//1、len < 8 的时候，按每次一个字节来读写。
//2、len >= 8 的时候，32位机器按每次 4 个字节来读写，64位机器按每次 8 个字节来读写，
//连续做 8 次。剩下的接着按 每次 4 或 8 字节读写， 每次 1 字节读写。


//1：先把将要赋值的块的起始地址对齐。见1号while。
//2：有大于等于8个OPSIZ长度未写，一次循环写8个OPSIZ长。 见2号while。 
//3：还有小于8个OPSIZ长度未写，一次循环写1个OPSIZ长。 见3号while。
//4：还有小于OPSIZ长度未写，一次循环写1个字节。 见4号while。

void * MyMemset_Fast2(void *dstpp, int c, size_t len)
{

#define op_t unsigned long int
#define OPSIZ	(sizeof(op_t))
typedef unsigned char byte;

  long int dstp = (long int) dstpp;
  if (len >= 8)
    {
      size_t xlen;
      op_t cccc;

      cccc = (unsigned char) c;
      cccc |= cccc << 8;
      cccc |= cccc << 16;
      if (OPSIZ > 4)
    /* Do the shift in two steps to avoid warning if long has 32 bits.  */
    cccc |= (cccc << 16) << 16;

      /* There are at least some bytes to set.
     No need to test for LEN == 0 in this alignment loop.  */
      while (dstp % OPSIZ != 0)
    {
      ((byte *) dstp)[0] = c;
      dstp += 1;
      len -= 1;
    }

      /* Write 8 `op_t' per iteration until less than 8 `op_t' remain.  */
      xlen = len / (OPSIZ * 8);
      while (xlen > 0)
    {
      ((op_t *) dstp)[0] = cccc;
      ((op_t *) dstp)[1] = cccc;
      ((op_t *) dstp)[2] = cccc;
      ((op_t *) dstp)[3] = cccc;
      ((op_t *) dstp)[4] = cccc;
      ((op_t *) dstp)[5] = cccc;
      ((op_t *) dstp)[6] = cccc;
      ((op_t *) dstp)[7] = cccc;
      dstp += 8 * OPSIZ;
      xlen -= 1;
    }
      len %= OPSIZ * 8;

      /* Write 1 `op_t' per iteration until less than OPSIZ bytes remain.  */
      xlen = len / OPSIZ;
      while (xlen > 0)
    {
      ((op_t *) dstp)[0] = cccc;
      dstp += OPSIZ;
      xlen -= 1;
    }
      len %= OPSIZ;
    }

  /* Write the last few bytes.  */
  while (len > 0)
    {
      ((byte *) dstp)[0] = c;
      dstp += 1;
      len -= 1;
    }
  return dstpp;
}



void strcpy1(char * str1,char * str2)
{
    for(;*str1;*str2++=*str1++)
        ;
    *str2='\0';	
}

void MyStrcpy2(char * dest, char * src)
{
    while ('\0' != (*src))
    {
        //后置++优先级高于*，而后置++是先赋值再加1，
        //因此先赋值的是src原来的字符(*src)，一次赋值结束后
        //src+1, dest+1
        *dest++ = *src++;
    }
    //循环跳出来时*src=='\0'，将'\0'赋给*dest
    *dest = *src;
    return;
}

//不足：*src要分成两种情况('\0' != *src 和 '\0' == *src)去讨论。
//改进：优化while循环内部的语句作为循环条件。
//不足：假如把dest与src的顺序写反了，程序只会在运行中报错。
//改进：保证src这个源字符串内容不可修改(添加const)，这样当写反dest和src时，会在编译时报错。
//不足：上述代码不能作为参数传入函数内部。
//改进：修改函数返回类型为char *，以实现链式访问。
char * MyStrcpy3(char * dest, const char * src)
{
    char * cp = dest;
    assert((NULL != src) && (NULL != cp));
    while (*cp++ = *src++)
    {
        ;
    }
    return dest;
}


//字符串的比较:比较两个字符串,若str1<str2,返回-1,若str1>str2，返回1,若str1=str2,返回0
//规则:从两个字符串第一个位置开始比较对应位置上的字符的ASCII码值，直到分出大小
//或者达到某个字符串的结尾，先到达结尾的字符串小。） 

int MyStrcmp(const char * str1,const char * str2)
{
    int ret;
    if(str1==NULL || str2==NULL)
        return 0;

    //转化为unsigned char 是为了防止溢出
    //例如(char)0x7f-(char)0x80=255, (unsigned char)0x7f-(unsigned char)0x80=-1
    ret=0;
    while( ! (ret = *(unsigned char *)str1- *(unsigned char *)str2) && * str1 )
    {
        str1++;//++str1;
        str2++;//++str2;
    }

    if( ret<0)
        ret=-1;
    else if(ret>0)
        ret=1;
    return ret;
}

//字符串的连接:将str2连接到str1后面 
void MyStrcat(char *str1,char *str2)
{
    for(;*str1;str1++)
        ;
    for(;*str2;*str1++=*str2++)
        ;
    *str1='\0';
}


char* MyStrcat2(char * dest, const char * src)
{
    char * cp = dest;
    assert((NULL != src) && (NULL != cp));
    while ('\0' != *cp)
    {
        cp++;
    }
    while (*cp++ = *src++)
    {
        ;
    }
    return dest;
}



//求字符串长度(不包括'\0') 
//unsigned int strlen(char *str)
//{
//    int n=0;
//    for(;*str;str++)
//    {
//        n++;
//    }
//    return n;
//} 
int MyStrlen(const char * str)
{
    int len = 0;
    assert(NULL != str);
    while ('\0' != *str)
    {
        len++;
        str++;
    }
    return len;
}

int MyStrlen2(const char * start)
{
    const char * end = start;
    assert(NULL != start);
    while ('\0' != *end)
    {
        end++;
    }
    return (end - start);
}

//递归实现1
int MyStrlen3(const char * str)
{
    assert(NULL != str);
    if ('\0' == *str)
    {
        return 0;
    }
    else
    {
        return (1 +	MyStrlen(str+1));
    }
}
//递归实现2
int MyStrlen4(const char * str)
{
    return assert(NULL != str), '\0' == *str ? 0 : 1 + MyStrlen(str + 1);
}


//glibc -- strlen源码  具体原理参见网上博文
typedef unsigned long ulong;
size_t strlen_glibc(const char * str) 
{
    const char * char_ptr;
    const ulong * longword_ptr;
    register ulong longword, magic_bits;

    //数据对齐(data alignment)，是指所在的内存地址必须是该数据长度的整数倍，CPU的存取速度就会最快。
    //在32位计算机中，一个int型的数据为4个字节，则int型数据的起始地址能被4整除的时候CPU的存取效率比较高。
    //CPU的优化规则如下：对于n字节(n = 2, 4, 8...)的元素，它的首地址能被n整除才能获得最好的性能。
    //sizeof(ulong) - 1=3  =0x00000003,低2位为1  
    for (char_ptr = str; ((ulong) char_ptr & (sizeof(ulong) - 1)) != 0;
        ++char_ptr)
    {
            if (*char_ptr == '\0')         //在对齐之前就遇到"\0"则可以直接return char_ptr - str;
                return char_ptr - str;
    }
    longword_ptr = (ulong*) char_ptr;
    magic_bits = 0x7efefeffL;
    while (1) 
    {
        longword = *longword_ptr++;
        if ((((longword + magic_bits) ^ ~longword) & ~magic_bits) != 0)     //主要判断word的4个bit中任意一个字节为0
        {
            const char *cp = (const char*) (longword_ptr - 1 );
            if (cp[0] == 0)
                return cp - str;
            if (cp[1] == 0)
                return cp - str + 1;
            if (cp[2] == 0)
                return cp - str + 2;
            if (cp[3] == 0)
                return cp - str + 3;
        }
    }
}
/*
//0x7efefeffL=01111110-11111110-11111110-11111111
//为0位置被称为holes位，它们都有一个特征，就是在每个字节的左边，这有什么用呢？ 很明显，当右边有进位时，
//会修改到这个0，或者这几个0的位置与另外一个数相运算时会被改变。

//~magic_bits的二进制形式为： 10000001-00000001-00000001-00000000，为1的位置正是相应holes的位置，
//那么可以得出(((longword + magic_bits) ^ ~longword) & ~magic_bits) != 0的作用是在判断
//((longword + magic_bits) ^ ~longword)中的相应holes为是0还是1，由最后的!=0得知当
//((longword + magic_bits) ^ ~longword)中相应holes位置一个或多个为1时，进入if判断语句块。

//那么现在的问题是((longword + magic_bits) ^ ~longword)是什么意思呢？
//我们假设result = longword + magic_bits, 那么result中相应holes位置的值就会根据longword的实际值而设置：
//如果longword中某个字节是0，那么与magic_bits对应字节相加就不会产生进位，那么result中相应
//holes位置就不会设置为0； 当longword中某个字节是非零时，结果恰恰相反-- 与magic_bits对应字
//节相加会产生进位，则result中相应holes位置就会设置为1。

//最后一个问题是：result ^ ~longword是什么意思呢？
//我们假设a = result ^ ~longword。
//我们举个具体的例子吧，如下(注意：longword中的各个字节的最高位不可能为1，因为ascii码范围是[0-127])

//可以看出a中为1的位的意义就是result相对longword中的未改变的位。就是说result的某一位与
//longword中的相同位相同时，那么在a中的相对位就为1。
//那么if ((a & b) != 0)什么意思呢？检测a中的holes位是1还是0.
//现在终于知道只要longword中有一个或几个为0的字节，就可以进入if判断语句块中，这正是strlen所做的 
//-- strlen就是为了找到结束符，然后返回长度 ^_^。
*/

//glibc2。27的实现 考虑64位系统
size_t strlen_glibc64(const char *str)
{
  const char *char_ptr;
  const unsigned long int *longword_ptr;
  unsigned long int longword, himagic, lomagic;

  /* Handle the first few characters by reading one character at a time.
     Do this until CHAR_PTR is aligned on a longword boundary.  */
  for (char_ptr = str; ((unsigned long int) char_ptr
            & (sizeof (longword) - 1)) != 0;
       ++char_ptr)
    if (*char_ptr == '\0')
      return char_ptr - str;

  /* All these elucidatory comments refer to 4-byte longwords,
     but the theory applies equally well to 8-byte longwords.  */

  longword_ptr = (unsigned long int *) char_ptr;

  /* Bits 31, 24, 16, and 8 of this number are zero.  Call these bits
     the "holes."  Note that there is a hole just to the left of
     each byte, with an extra at the end:

     bits:  01111110 11111110 11111110 11111111
     bytes: AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD

     The 1-bits make sure that carries propagate to the next 0-bit.
     The 0-bits provide holes for carries to fall into.  */
  himagic = 0x80808080L;
  lomagic = 0x01010101L;
  if (sizeof (longword) > 4)
    {
      /* 64-bit version of the magic.  */
      /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
      himagic = ((himagic << 16) << 16) | himagic;
      lomagic = ((lomagic << 16) << 16) | lomagic;
    }
  if (sizeof (longword) > 8)
    abort ();

  /* Instead of the traditional loop which tests each character,
     we will test a longword at a time.  The tricky part is testing
     if *any of the four* bytes in the longword in question are zero.  */
  for (;;)
    {
      longword = *longword_ptr++;

      if (((longword - lomagic) & ~longword & himagic) != 0)
    {
      /* Which of the bytes was the zero?  If none of them were, it was
         a misfire; continue the search.  */

      const char *cp = (const char *) (longword_ptr - 1);

      if (cp[0] == 0)
        return cp - str;
      if (cp[1] == 0)
        return cp - str + 1;
      if (cp[2] == 0)
        return cp - str + 2;
      if (cp[3] == 0)
        return cp - str + 3;
      if (sizeof (longword) > 4)
        {
          if (cp[4] == 0)
        return cp - str + 4;
          if (cp[5] == 0)
        return cp - str + 5;
          if (cp[6] == 0)
        return cp - str + 6;
          if (cp[7] == 0)
        return cp - str + 7;
        }
    }
    }
}




/*
atoi函数的介绍，该函数用来解析字符串str，并将str指向的内容转换为int型整数返回。首先该函数可能会丢弃空格字符，直到找到第一个非空格字符为止，然后从该字符开始，取一个可选的正负号，最后转换为int型整数即可。需要注意的是，如果字符串str中包含其他字符，那么这些字符将会被忽略。

    算法分析：实现atoi的基本功能是比较简单的，但是当考虑很多特殊情况时，却不是一件容易的事情，比如考虑指针的有效性、空字符串、空白字符、是否是数字字符、是否会溢出、正数还是负数，这些情况都应该考虑到，其次，异常情况下返回0与正常情况下返回0是不一样的，这里用全局变量status来标记当前输入是合法还是非法(也就是正常或者异常)。
*/

int MyAtoi(const char * str)
{
    int sign = 1;
    int64_t ret = 0;
    //判断指针是否为空
    assert(NULL != str);
    //处理空字符串
    if ('\0' == *str)
    {
        return 0;
    }
    //处理空白字符
    while (isspace(*str))
    {
        str++;
        //出现连续多个空白字符是非法的
        if ('\0' == *str)
        {
            return 0;
        }
    }
    //处理正负号
    if ('+' == *str)
    {
        str++;
    }
    else if ('-' == *str)
    {
        sign = -1;
        str++;
    }
    while (*str)
    {
        //处理数字字符
        if (isdigit(*str))
        {
            ret = ret * 10 + sign * (*str - '0');

            if ((ret > INT_MAX) || (ret < INT_MIN))
            {
                return 0;
            }
            str++;
        }
        else
        {
            return (int)ret;
        }
    }	
    return (int)ret;
}


//vc6下的源码。 atoi 也是调用 atol
long __cdecl atol_vc6(const char *nptr)
{
    int c;              /* current char */
    long total;         /* current total */
    int sign;           /* if '-', then negative, otherwise positive */

    /* skip whitespace */
    while ( isspace((int)(unsigned char)*nptr) )
        ++nptr;

    c = (int)(unsigned char)*nptr++;
    sign = c;           /* save sign indication */
    if (c == '-' || c == '+')
        c = (int)(unsigned char)*nptr++;    /* skip sign */

    total = 0;

    while (isdigit(c)) {
        total = 10 * total + (c - '0');     /* accumulate digit */
        c = (int)(unsigned char)*nptr++;    /* get next char */
    }

    if (sign == '-')
        return -total;
    else
        return total;   /* return result, negated if necessary */
}





/*
算法分析
    首先需要判断字符串str1的首字符是否与str2中的首字符相同，如果不相同的话，str1指向的字符向后移动，不断地去比较str1的字符与str2中的首字符，如果一直都不相同的话，就表示str1中找不到包含str2的字符串，如果相同的话，那么str1和str2指向的字符都向后移动，再次判断str2中的字符串是否全都在str1中出现，如果是的话，那么返回str2在str1中的位置，如果不是全都出现的话，那么表示没有找到，返回NULL。需要注意的是，匹配到首个字符时，有可能是连续几个字符相同的情况，这时就需要判断str2中的字符匹配str1时，究竟是str1中的哪个字符才是需要匹配的首个字符。

    这里举例说明下，若str1内容为"abcccde"，str2内容为"cd"，那么指向str1中的第三个字符'c'时，恰巧str2中的首个字符为'c'，这时可以认为匹配到首个字符。但是当str1后移一个字符时，即就是'c'，而str2后移一个字符，即就是'd'，显然不匹配了，但是并不能认为str1中没有匹配str2的项，这就需要在匹配前保存遇到的str2中的首个字符在str1中的位置，方便下一次查找(ps:也就是不需要再次遍历了)。当指向str1中的第二个'c'时，匹配str2中的首个字符'c'，此时str1后移一位，即就是'c',而str2后移一个字符，即就是'd'，显然不匹配了，由于保存了str1中第二个'c'的位置，那么接下来就可以从str1中的第三个'c'开始匹配，str2中首个字符为'c'，str1后移一位，即就是'd'，而str2后移一位也是'd'，str1再次后移一位得到'e'，而str2后移一位得到'\0'，查找结束。
*/

char * MyStrstr(const char * src, const char * substr)
{
    const char * str1 = src;
    const char * str2 = substr;
    assert((NULL != src) && (NULL != substr));
    if ('\0' == *str2)
    {
        return (char *)src;
    }
    else
    {
        while ('\0' != *src)
        {
            str1 = src;
            str2 = substr;

            while (('\0' != *str1) && ('\0' != *str2) && (*str1 == *str2))
            {
                str1++;
                str2++;
            }

            if ('\0' == *str2)
            {
                return (char *)src;
            }
            else
            {
                src++;
            }
        }
    }
    return NULL;
}




char * MyStrchr2(const char * src, int ch)
{
    assert(NULL != src);
    while (('\0' != *src) && (((char)ch) != *src))
    {
        src++;
    }

    if (((char)ch) == *src)
    {
        return (char *)src;
    }
    else
    {
        return NULL;
    }
}










#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "BasicOperation.h"     //自定义的基础操作类
//#include "FastMemcpy_Avx.h"


#ifdef UNIT_TEST

int Test_MyClib()
{
    const int BUFFER_SIZE=100*1024+7;
   //SP_ALIGN(32) 
       char buf1[BUFFER_SIZE];  
   //SP_ALIGN(32) 
       char buf2[BUFFER_SIZE];


    //各种基本类型数据相乘效率相对值，越高越快 ret*=b;
    //float 53 double 53 int 236 unsigned int 151 uint64_t 132
    //除法效率ret/=b;
    //float 27 double 27 int 34 unsigned int 43 int64_t 9.2 uint64_t 9.1 //int64_t比浮点还慢？
    typedef unsigned int TypeTest;  
    const unsigned int loopNum=1000;
    TypeTest a=39.99;
    TypeTest b=20.88;

    SuperTime time("Test_MyClib");


    size_t ret=0;
    for (int K=0;K<1000*10;K++)
    {
        time.getBeginTime();
        for (int i=0;i<loopNum;i++)
        {
           ret+=(int)memcpy(buf1,buf2,sizeof(buf1));   //375xhz 系统函数
           ret+=(int)MyMemcpyV1(buf1,buf2,sizeof(buf1)); //505xHz   
           ret+=(int)MyMemcpyV2(buf1,buf2,sizeof(buf1)); //517xHz  
           ret+=(int)MyMemcpy_FastV1(buf1,buf2,sizeof(buf1));   //512xhz  
           ret+=(int)memcpy_duff(buf1,buf2,sizeof(buf1));   //33xhz  
           //ret+=(int)memcpy_fast(buf1,buf2,sizeof(buf1));   //479xhz    //基于AVX 并没有飞快


            //基本相近
            ret+=(int)memset(buf1,0X99,sizeof(buf1));
            ret+=(int)MyMemsetV1(buf1,0X99,sizeof(buf1));
            ret+=(int)MyMemsetV2(buf1,0X99,sizeof(buf1));
            ret+=(int)MyMemset_FastV1(buf1,0X99,sizeof(buf1));
            ret+=(int)MyMemset_FastV2(buf1,0X99,sizeof(buf1));
        }
        time.getEndPrint(100);
    }

    std::cout<<"result="<<ret<<std::endl;
    //print_hex(buf1,BUFFER_SIZE);

    //float fv=0.99;
    //memset(&fv,0,sizeof(fv));
    //print_hex(&fv,sizeof(fv));
    //printf("%lf",fv);

    return 0;
}

#endif
