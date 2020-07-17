#include "hash.h"
#include <stdint.h>


#if 0
几种经典的hash算法
    计算理论中，没有Hash函数的说法，只有单向函数的说法。
    如果某个函数在给定输入的时候，很容易计算出其结果来；
    而当给定结果的时候，很难计算出输入来，这就是单项函数。
    各种加密函 数都可以被认为是单向函数的逼近。Hash函数（或者成为散列函数）也可以看成是单向函数的一个逼近。即它接近于满足单向函数的定义。

    Hash函数还有另外的含义。实际中的Hash函数是指把一个大范围映射到一个小范围。把大范围映射到一个小范围的目的往往是为了节省空间，使得数据容易保存。除此以外，Hash函数往往应用于查找上。所以，在考虑使用Hash函数之前，需要明白它的几个限制：

    1. Hash的主要原理就是把大范围映射到小范围；所以，你输入的实际值的个数必须和小范围相当或者比它更小。不然冲突就会很多。
    2. 由于Hash逼近单向函数；所以，你可以用它来对数据进行加密。
    3. 不同的应用对Hash函数有着不同的要求；比如，用于加密的Hash函数主要考虑它和单项函数的差距，而用于查找的Hash函数主要考虑它映射到小范围的冲突率。

    应用于加密的Hash函数已经探讨过太多了，在作者的博客里面有更详细的介绍。所以，本文只探讨用于查找的Hash函数。

    Hash函数应用的主要对象是数组（比如，字符串），而其目标一般是一个int类型。以下我们都按照这种方式来说明。

    一般的说，Hash函数可以简单的划分为如下几类：
    1. 加法Hash；
    2. 位运算Hash；
    3. 乘法Hash；
    4. 除法Hash；
    5. 查表Hash；
    6. 混合Hash；
    下面详细的介绍以上各种方式在实际中的运用。

    一 加法Hash
    所谓的加法Hash就是把输入元素一个一个的加起来构成最后的结果。标准的加法Hash的构造如下：

    static int additiveHash(String key, int prime)
{
    int hash, i;
    for (hash = key.length(), i = 0; i < key.length(); i++)
        hash += key.charAt(i);
    return (hash % prime);
}
这里的prime是任意的质数，看得出，结果的值域为[0,prime-1]。

    二 位运算Hash
    这类型Hash函数通过利用各种位运算（常见的是移位和异或）来充分的混合输入元素。比如，标准的旋转Hash的构造如下：

    static int rotatingHash(String key, int prime)
{
    int hash, i;
    for (hash=key.length(), i=0; i<key.length(); ++i)
        hash = (hash<<4)^(hash>>28)^key.charAt(i);
    return (hash % prime);
}

先移位，然后再进行各种位运算是这种类型Hash函数的主要特点。比如，以上的那段计算hash的代码还可以有如下几种变形：
1.     hash = (hash<<5)^(hash>>27)^key.charAt(i);
2.     hash += key.charAt(i);
hash += (hash << 10);
hash ^= (hash >> 6);
3.     if((i&1) == 0)
{
    hash ^= (hash<<7) ^ key.charAt(i) ^ (hash>>3);
}
else
{
    hash ^= ~((hash<<11) ^ key.charAt(i) ^ (hash >>5));
}
4.     hash += (hash<<5) + key.charAt(i);
5.     hash = key.charAt(i) + (hash<<6) + (hash>>16) – hash;
6.     hash ^= ((hash<<5) + key.charAt(i) + (hash>>2));

三 乘法Hash
    这种类型的Hash函数利用了乘法的不相关性（乘法的这种性质，最有名的莫过于平方取头尾的随机数生成算法，虽然这种算法效果并不好）。比如，

static int bernstein(String key)
{
    int hash = 0;
    int i;
    for (i=0; i<key.length(); ++i) hash = 33*hash + key.charAt(i);
    return hash;
}

jdk5.0里面的String类的hashCode()方法也使用乘法Hash。不过，它使用的乘数是31。推荐的乘数还有：131, 1313, 13131, 131313等等。

使用这种方式的著名Hash函数还有：
//  32位FNV算法
int M_SHIFT = 0;
public int FNVHash(byte[] data)
{
    int hash = (int)2166136261L;
    for(byte b : data)
        hash = (hash * 16777619) ^ b;
    if (M_SHIFT == 0)
        return hash;
    return (hash ^ (hash >> M_SHIFT)) & M_MASK;
}

以及改进的FNV算法：
public static int FNVHash1(String data)
{
    final int p = 16777619;
    int hash = (int)2166136261L;
    for(int i=0;i<data.length();i++)
        hash = (hash ^ data.charAt(i)) * p;
    hash += hash << 13;
    hash ^= hash >> 7;
    hash += hash << 3;
    hash ^= hash >> 17;
    hash += hash << 5;
    return hash;
}

除了乘以一个固定的数，常见的还有乘以一个不断改变的数，比如：
static int RSHash(String str)
{
    int b    = 378551;
    int a    = 63689;
    int hash = 0;

    for(int i = 0; i < str.length(); i++)
    {
        hash = hash * a + str.charAt(i);
        a    = a * b;
    }
    return (hash & 0x7FFFFFFF);
}

虽然Adler32算法的应用没有CRC32广泛，不过，它可能是乘法Hash里面最有名的一个了。关于它的介绍，大家可以去看RFC 1950规范。

    四 除法Hash
    除法和乘法一样，同样具有表面上看起来的不相关性。不过，因为除法太慢，这种方式几乎找不到真正的应用。需要注意的是，我们在前面看到的hash的 结果除以一个prime的目的只是为了保证结果的范围。如果你不需要它限制一个范围的话，可以使用如下的代码替代”hash%prime”： hash = hash ^ (hash>>10) ^ (hash>>20)。

    五 查表Hash
    查表Hash最有名的例子莫过于CRC系列算法。虽然CRC系列算法本身并不是查表，但是，查表是它的一种最快的实现方式。查表Hash中有名的例子有：Universal Hashing和Zobrist Hashing。他们的表格都是随机生成的。

    六 混合Hash
    混合Hash算法利用了以上各种方式。各种常见的Hash算法，比如MD5、Tiger都属于这个范围。它们一般很少在面向查找的Hash函数里面使用。

    七 对Hash算法的评价
http://www.burtleburtle.net/bob/hash/doobs.html 这个页面提供了对几种流行Hash算法的评价。我们对Hash函数的建议如下：

1. 字符串的Hash。最简单可以使用基本的乘法Hash，当乘数为33时，对于英文单词有很好的散列效果（小于6个的小写形式可以保证没有冲突）。复杂一点可以使用FNV算法（及其改进形式），它对于比较长的字符串，在速度和效果上都不错。

2. 长数组的Hash。可以使用http://burtleburtle.net/bob/c/lookup3.c这种算法，它一次运算多个字节，速度还算不错。

八 后记
    本文简略的介绍了一番实际应用中的用于查找的Hash算法。Hash算法除了应用于这个方面以外，另外一个著名的应用是巨型字符串匹配（这时的 Hash算法叫做：rolling hash，因为它必须可以滚动的计算）。设计一个真正好的Hash算法并不是一件容易的事情。做为应用来说，选择一个适合的算法是最重要的。

九 数组hash
inline int hashcode(const int *v)
{
    int s = 0;
    for(int i=0; i<k; i++)
        s=((s<<2)+(v[i]>>4))^(v[i]<<10);
    s = s % M;
    s = s < 0 ? s + M : s;
    return s;
}

注：虽说以上的hash能极大程度地避免冲突，但是冲突是在所难免的。所以无论用哪种hash函数，都要加上处理冲突的方法。
#endif

//typedef unsigned int uint;
//typedef unsigned char uchar;


//加法Hash 就是把输入元素一个一个的加起来构成最后的结果。 //prime是任意的质数，得出结果的值域为[0,prime-1]。
unsigned int additiveHash(char* buf, unsigned int len,int prime)  
{
    unsigned int hash = len;
    unsigned int i;
    for ( i = 0; i < len; i++)
    {
        hash += buf[i];
    }
    return (hash%prime);
}

//位运算Hash 利用各种位运算（常见的是移位和异或）来充分的混合输入元素。比如，标准的旋转Hash如下：
unsigned int rotatingHash(char* buf, unsigned int len, int prime)
{
    unsigned int hash = len;
    unsigned int i;
    for ( i = 0; i < len; i++)
    {
        hash +=(hash<<4)^(hash>>28)^buf[i];
    }
    return (hash%prime);
}

//以上的那段计算hash的代码还可以有如下几种变形：
//1. hash = (hash<<5)^(hash>>27)^key.charAt(i);
//2. hash += key.charAt(i);
    //hash += (hash << 10);
    //hash ^= (hash >> 6);
//4.     hash += (hash<<5) + key.charAt(i);
//5.     hash = key.charAt(i) + (hash<<6) + (hash>>16) – hash;
//6.     hash ^= ((hash<<5) + key.charAt(i) + (hash>>2));
//3.  if((i&1) == 0)
    //hash ^= (hash<<7) ^ key.charAt(i) ^ (hash>>3);
    //else
    //hash ^= ~((hash<<11) ^ key.charAt(i) ^ (hash >>5));


//乘法Hash,利用了乘法的不相关性（乘法的这种性质，最有名的莫过于平方取头尾的随机数生成算法，虽然这种算法效果并不好）。
unsigned int bernstein(char* buf, unsigned int len)  
{
    unsigned int hash =0;
    unsigned int i;
    for ( i = 0; i < len; i++)
    {
        hash = 33*hash+buf[i];
    }
    return hash;
}

//jdk5.0里面的String类的hashCode()方法也使用乘法Hash。不过，它使用的乘数是31。
//推荐的乘数还有：131, 1313, 13131, 131313等等。

//除了乘以一个固定的数，常见的还有乘以一个不断改变的数，比如： RSHash

//九 数组hash
unsigned int hashcode(const int *v,int len)
{
    int s = 0;
    for(int i=0; i<len; i++)
    {
        s=((s<<2)+(v[i]>>4))^(v[i]<<10);
    }
    //s = s % M;
    //s = s < 0 ? s + M : s;
    return s;
}

//注：虽说以上的hash能极大程度地避免冲突，但是冲突是在所难免的。所以无论用哪种hash函数，都要加上处理冲突的方法。



//PHP中出现的字符串Hash函数
unsigned long hashpjw(char *arKey, unsigned int nKeyLength)
{
    unsigned long h = 0, g;
    char *arEnd=arKey+nKeyLength;
    while (arKey < arEnd)
    {
        h=(h<<4)+*arKey++;
        if ((g = (h & 0XF0000000)))
        {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
    }
    return h;
}


//OpenSSL 項目中lhash.c 文件提供兩種HashCode 的計算方法

unsigned long lh_strhash(char *str)
{
     int i,l;
    unsigned long ret=0;
    unsigned short *s;
    if (str == NULL) return(0);
    l=(strlen(str)+1)/2;
    s=(unsigned short *)str;
    for (i=0; i<l; i++)
      ret^=(s[i]<<(i&0x0f));
     return(ret);
}


/* The following hash seems to work very well on normal text strings
 * no collisions on /usr/dict/words and it distributes on %2^n quite
 * well, not as good as MD5, but still good.
 */
unsigned long lh_strhash(const char *c)
    {
    unsigned long ret=0;
    long n;
    unsigned long v;
    int r;
    if ((c == NULL) || (*c == '/0'))
           return(ret);
        /*
        unsigned char b[16];
        MD5(c,strlen(c),b);
        return(b[0]|(b[1]<<8)|(b[2]<<16)|(b[3]<<24));
        */

        n=0x100;
        while (*c)
        {
        v=n|(*c);
        n+=0x100;
        r= (int)((v>>2)^v)&0x0f;
        ret=(ret<<r)|(ret>>(32-r));
        ret&=0xFFFFFFFFL;
        ret^=v*v;
        c++;
        }
        return((ret>>16)^ret);
}
//MySql中出现的字符串Hash函数

#ifndef NEW_HASH_FUNCTION 
/* Calc hashvalue for a key (区分大小写)*/
static uint32_t calc_hashnr(const char *key,uint32_t  length)
{
    register uint32_t  nr=1, nr2=4; 
    while (length--)
    {
        nr^= (((nr & 63)+nr2)*((uint32_t ) (unsigned char) *key++))+ (nr << 8);
        nr2+=3;
    } 
    return((uint32_t ) nr);
} 



int my_tolower(int c)
{
    if ((c >= 'A') && (c <= 'Z'))
        return c + ('a' - 'A');
    return c;
}

int my_toupper(int c)
{
    if ((c >= 'a') && (c <= 'z'))
        return c + ('A' - 'a');
    return c;
}

/* Calc hashvalue for a key, case indepenently （不区分大小写） */
uint32_t  calc_hashnr_caseup(const char *key,uint32_t  length)
{
    register uint32_t  nr=1, nr2=4; 
    while (length--)
    {
        nr^= (((nr & 63)+nr2)*((uint32_t ) (unsigned char) my_toupper(*key++)))+ (nr << 8);
        nr2+=3;
    } 
    return((uint32_t ) nr);
}

#else
    /* 
    * Fowler/Noll/Vo hash 
    * 
    * The basis of the hash algorithm was taken from an idea sent by email to the 
    * IEEE Posix P1003.2 mailing list from Phong Vo (kpv@research.att.com) and 
    * Glenn Fowler (gsf@research.att.com). Landon Curt Noll (chongo@toad.com) 
    * later improved on their algorithm. 
    * 
    * The magic is in the interesting relationship between the special prime 
    * 16777619 (2^24 + 403) and 2^32 and 2^8. 
    * 
    * This hash produces the fewest collisions of any function that we’ve seen so 
    * far, and works well on both numbers and strings. 
    */

    //（区分大小写）
uint calc_hashnr(const byte *key, uint len)
{
    const byte *end=key+len;
    uint hash; 

    for (hash = 0; key < end; key++)
    {
        hash *= 16777619;
        hash ^= (uint) *(uchar*) key;
    } 

    return (hash);
} 

//（不区分大小写）
uint calc_hashnr_caseup(const byte *key, uint len)
{
    const byte *end=key+len;
    uint hash; 

    for (hash = 0; key < end; key++)
    {
        hash *= 16777619;
        hash ^= (uint) (uchar) toupper(*key);
    } 

    return (hash);
}
#endif


//经典times33算法如下：对于字符串
uint32_t  hash_times33(const char* key)
{
    uint32_t  nHash = 0;
    while (*key)
    {
        nHash=(nHash<<5)+nHash+*key++;
    }
    return nHash;
}
uint32_t hash_times33(const char* buf,size_t len)
{
    unsigned int nHash=0;
    const char* pos=buf;
    for (size_t n=0;n<len;n++)
    {
        nHash=(nHash<<5)+nHash+(*pos++);
    }
    return nHash;
}

//times33哈希算法(Perl、Berkeley DB、Apache中使用的hash算法)

//Time33是字符串哈希函数，现在几乎所有流行的HashMap都采用了DJB Hash Function，俗称“Times33”算法。
//Times33的算法很简单，就是不断的乘33。


unsigned int time33(char *str)
{
    unsigned int hash = 5381;  //为什么初始值是5381？5381（001 010 100 000 101），据说hash后的分布更好一些。
    while(*str)
    {
        hash += (hash << 5 ) + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}



//time33 哈希函数，又叫 DJBX33A，Bernstein's hash
//PHP, apache, perl, bsddb都使用time33哈希.
//为什么是33 其它倍数？
//Ngix使用的是 time31
//Tokyo Cabinet使用的是 time37
//Bob在他的文章说，小写英文词汇适合33, 大小写混合使用65。time33比较适合的是英文词汇的hash.

//原型:   hash（i） = hash（i-1）*33 + str[i] ;
//最简单的版本  这个版本最可以体现time33的算法思路，够简单。
unsigned long hash_time33_1(char const *str, int len)
{
    unsigned long  hash = 0;
    for (int i = 0; i < len; i++)
    {
        hash = hash *33 + (unsigned long) str[i];
    }
    return hash;
}

//把乘法操作换成位操作
unsigned long hash_time33_2(char const *str, int len)
{
    unsigned long  hash = 0;
    for (int i = 0; i < len; i++) {
        hash = ((hash <<5) + hash) + (unsigned long) str[i];
    }
    return hash;
}
//59个字符1000 0000次运行（gcc没有开启优化，因为开了优化后两个函数的实际代码会一样)
//php版本
unsigned hash_time33_php(char const*str, int len)
{
    unsigned long hash = 5381;
    //php版本的hash初始值是5381, 这个
    //Magic Constant 5381:

    /* variant with the hash unrolled eight times */
    for (; len >= 8; len -= 8) {
        hash = ((hash << 5) + hash) + *str++;
        hash = ((hash << 5) + hash) + *str++;
        hash = ((hash << 5) + hash) + *str++;
        hash = ((hash << 5) + hash) + *str++;
        hash = ((hash << 5) + hash) + *str++;
        hash = ((hash << 5) + hash) + *str++;
        hash = ((hash << 5) + hash) + *str++;
        hash = ((hash << 5) + hash) + *str++;
    }

    switch (len) {
    case 7: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
    case 6: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
    case 5: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
    case 4: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
    case 3: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
    case 2: hash = ((hash << 5) + hash) + *str++; /* fallthrough... */
    case 1: hash = ((hash << 5) + hash) + *str++; break;
    case 0: break;
    }

    return hash;
}
//59个字符,1000 0000次
//real    0m1.088s
//user    0m1.068s
//sys     0m0.000s

//Apache版本
unsigned long hash_time33_apache(char const  *str, int *len)
{
    unsigned long hash = 0;
    const char *p=str;
    if (*len<=0) {
        for(p = str; *p; p++) {
            hash = hash * 33 + *p;
        }
        *len = p - str;
    }
    else {
        int i = *len;
        for (p = str;i; i--, p++) {
            hash = hash * 33 + *p;
        }
    }
    return hash;
}
//测试结果
//real    0m1.418s
//user    0m1.412s
//sys     0m0.004s




//编程珠玑中的一个hash函数

//用跟元素个数最接近的质数作为散列表的大小
#define NHASH 29989
#define MULT 31
unsigned int hash(char *p)
{
    unsigned int h = 0;
    for (; *p; p++)
        h = MULT *h + *p;
    return h % NHASH;
}


unsigned int SDBMHash(char *str)
{
    unsigned int hash = 0;

    while (*str)
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}


//常用hash算法及评测
//RS hash 算法
unsigned int RSHash(char* str, unsigned int len)
{
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = hash * a + (*str);
        a    = a * b;
    }
    return hash;
}
/* End Of RS Hash Function */

//JS hash 算法
unsigned int JSHash(char* str, unsigned int len)
{
    unsigned int hash = 1315423911;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }
    return hash;
}
/* End Of JS Hash Function */

//PJW hash 算法
unsigned int PJWHash(char* str, unsigned int len)
{
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt * 3) / 4);
    const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash              = 0;
    unsigned int test              = 0;
    unsigned int i                 = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = (hash << OneEighth) + (*str);
        if((test = hash & HighBits) != 0)
        {
            hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }
    return hash;
}
/* End Of P. J. Weinberger Hash Function */

//ELF hash 算法
unsigned int ELFHash(char* str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = (hash << 4) + (*str);
        if((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}
/* End Of ELF Hash Function */

//BKDR hash 算法
unsigned int BKDRHash(char* str, unsigned int len)
{
    unsigned int seed = 131;
    /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = (hash * seed) + (*str);
    }
    return hash;
}
/* End Of BKDR Hash Function */

//SDBM hash 算法
unsigned int SDBMHash(char* str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}
/* End Of SDBM Hash Function */

//DJB hash 算法 (times33)-这个用得非常多，很多库都用它。
unsigned int DJBHash(char* str, unsigned int len)
{
    unsigned int hash = 5381;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = ((hash << 5) + hash) + (*str);
    }
    return hash;
}
/* End Of DJB Hash Function */

//DEK hash 算法
unsigned int DEKHash(char* str, unsigned int len)
{
    unsigned int hash = len;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }
    return hash;
}
/* End Of DEK Hash Function */

//BP hash 算法
unsigned int BPHash(char* str, unsigned int len)
{
    unsigned int hash = 0;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash = hash << 7 ^ (*str);
    }
    return hash;
}
/* End Of BP Hash Function */

//FNV hash 算法
unsigned int FNVHash(char* str, unsigned int len)
{
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash      = 0;
    unsigned int i         = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash *= fnv_prime;
        hash ^= (*str);
    }
    return hash;
}
/* End Of FNV Hash Function */

//AP hash 算法
unsigned int APHash(char* str, unsigned int len)
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i    = 0;
    for(i = 0; i < len; str++, i++)
    {
        hash ^= ((i & 1) == 0) ? ( (hash << 7) ^ (*str) * (hash >> 3)) :
            (~((hash << 11) + (*str) ^ (hash >> 5)));
    }
    return hash;
}
/* End Of AP Hash Function */

#if 0
各种算法评测
    Hash函数	数据1	数据2	数据3	数据4	数据1得分	数据2得分	数据3得分	数据4得分	平均分
    BKDRHash	2	0	    4774	481	    96.55	100	   90.95	82.05	92.64
    APHash	2	3	4754	493	    96.55	88.46	100	   51.28	86.28
    DJBHash	2	2	4975	474	    96.55	92.31	0	   100	    83.43
    JSHash	1	4	4761	506	    100	    84.62	96.83  17.95	81.94
    RSHash	1	0	4861	505	    100	    100	    51.58  20.51	75.96
    SDBMHash	3	2	    4849	504	    93.1	92.31  57.01	23.08	72.41
    PJWHash	30	26	4878	513	0	0	    43.89	0	   21.95
    ELFHash	30	26	4878	513	0	0	    43.89	0	   21.95

    其中
    数据1为100000个字母和数字组成的随机串哈希冲突个数。
    数据2为100000个有意义的英文句子哈希冲突个数。
    数据3为数据1的哈希值与1000003(大素数)求模后存储到线性表中冲突的个数。
    数据4为数据1的哈希值与10000019(更大素数)求模后存储到线性表中冲突的个数。
    经过比较，得出以上平均得分。平均数为平方平均数。
    可以发现，
    BKDRHash无论是在实际效果还是编码实现中，效果都是最突出的。
    APHash也是较为优秀的算法。DJBHash,JSHash,RSHash与SDBMHash各有千秋。
    PJWHash与ELFHash效果最差，但得分相似，其算法本质是相似的。
#endif


    //MurmurHash - 非常新的一个哈希算法，应该是目前效率最高的一个哈希算法，使用率很高。
    //MurmurHash3待搜索
unsigned int murMurHash2(const void *key, int len)
{
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    const int seed = 97;
    unsigned int h = seed ^ len;
    // Mix 4 bytes at a time into the hash
    const unsigned char *data = (const unsigned char *)key;
    while(len >= 4)
    {
        unsigned int k = *(unsigned int *)data;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }
    // Handle the last few bytes of the input array
    switch(len)
    {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0];
        h *= m;
    };
    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}

//测试类
 

#include<iostream>
#include<string>
class Employee
{
public:
    Employee(){name=""; salary=0.0; seniority=0;};
    Employee(const std::string & n, double sal, int sen):name(n), salary(sal), seniority(sen)
    {

    }
    //获得该类的name成员
    std::string getName() const
    { 
        return name; 
    }

    //二进制“<<”: 没有找到接受“std::string”类型的右操作数的运算符  解决办法: 用#include<string>里面不加.h
    //重载==运算符 
    //bool operator== (const Employee& rhs) const
    //{ 
    //    return this->getName()==rhs.getName(); 
    //}

    friend bool operator== (const Employee& lhs,const Employee& rhs);

    //重载!=运算符 
    bool operator!=(const Employee & rhs) const
    { 
        return !(*this == rhs); 
    }

    //friend std::ostream & operator<<(const std::ostream & os, const Employee & e)
    friend std::ostream & operator<<(std::ostream & os, const Employee & e)
    {
        std::cout << "name: " << e.name << ",\tsalary: " << e.salary << ",\tseniority: " << e.seniority << std::endl;
        return os;
    }
private:
    std::string name;
    double salary;
    int seniority;
};

bool operator== (const Employee& lhs,const Employee& rhs)
{ 
    return lhs.getName()==rhs.getName(); 
}

/****************************************************************
*   函数名称：hash(const HashedObj & key)
*   功能描述: 根据键值求个hash值,这个函数是根据一个特定的数学公式 
*   参数列表: key 数据项的键值 
*   返回结果：返回一个通过散列函数求得的值
*****************************************************************/
unsigned int hash(const std::string & key)
{
    unsigned int hashVal = 0;

    //用散列函数的那个公式求和
    for(int i = 0; i < key.length(); ++i)
    {
        hashVal = 37*hashVal + key[i];
    }
    return hashVal;
}

/****************************************************************
*   函数名称：hash(const HashedObj & key)
*   功能描述: 根据键值求个hash值,这个函数是根据一个特定的数学公式 
*   参数列表: key 数据项的键值 
*   返回结果：返回一个通过散列函数求得的值
*****************************************************************/
unsigned int hash(const Employee & item)
{
    return hash(item.getName());
}


#ifdef UNIT_TEST

int Test_Hash()
{

#if 0
    HashTableByList<int> hi;  
    for(int j = 0;j <= 100;j++)
    {  
        hi.insert(j*j);
    }  
    hi.print();  
    if(hi.contain(400))
    {
        std::cout<<"有"<<std::endl;  
    }
    hi.remove(400);  
    if(hi.contain(400))
    {
        std::cout<<"有"<<std::endl; 
    }
    else
    {
        std::cout<<"无"<<std::endl;  
    }
    std::cout<<std::endl;  
    hi.print();  
    hi.rehash();  
    std::cout<<"再散列"<<std::endl;  
    hi.print();  
    return 0;  
#endif


#if 1
    Employee e1("linux", 101.00, 1);
    Employee e2("ever", 102.00, 2);
    Employee e3("peter", 103.00, 3);
    Employee e4("may", 104.00, 4);
    Employee e5("usa", 105.00, 5);
    std::vector<Employee> v;
    v.push_back(e1);
    v.push_back(e2);
    v.push_back(e3);
    v.push_back(e4);
    v.push_back(e5);

    std::cout << "v: " << std::endl;
    for(unsigned i = 0; i < v.size(); ++i)
    {
        std::cout << v[i];
    }
    std::cout << std::endl;

    //HashTableByList<Employee> hashTable;
    HashTableByOpenAaddressing<Employee> hashTable;


    for(unsigned i = 0; i < v.size(); ++i)
    {
        hashTable.insert(v[i]);
    }
    hashTable.print();

    std::cout << std::endl;
    //测试查找函数findElement
    std::cout << "测试查找函数findElement: " << std::endl;
    Employee e11 = hashTable.findElement(e3);
    std::cout << "e3 = " << e11;
    Employee e13 = hashTable.findElement(e4);
    std::cout << "e4 = " << e13;
    std::cout << std::endl;

    std::cout << "测试包含函数containes: " << std::endl; 
    if(hashTable.contain(e2))
        std::cout << "containe e2" << std::endl;
    else 
        std::cout << "not containe e2" << std::endl;

    if(hashTable.contain(e4))
        std::cout << "containe e4" << std::endl;
    else 
        std::cout << "not containe e4" << std::endl;

    std::cout << "测试remove()：" << std::endl;
    hashTable.remove(e1);
    if(hashTable.contain(e1))
        std::cout << "containe e1" << std::endl;
    else 
        std::cout << "not containe e1" << std::endl;
    std::cout << std::endl;

    std::cout << "测试isEmpty(): " << std::endl;
    if(hashTable.isEmpty())
        std::cout << "hashTable is Empty " << std::endl;
    else
        std::cout << "hashTable is not Empty " << std::endl;
    std::cout << std::endl;
    std::cout << "测试makeEmpty(): " << std::endl;
    hashTable.makeEmpty();
    if(hashTable.isEmpty())
        std::cout << "hashTable is Empty " << std::endl << std::endl;
    else
        std::cout << "hashTable is not Empty " << std::endl;
    std::cout << std::endl;
    return 0;
 
#endif

}

#endif



//cityhash和murmurhash相比，速度略快
//使用复杂度：都非常简单
//语言支持：murmurhash主流语言基本都支持；cityhash 2010才发布算法，2011年发布实现，目前只有c++的支持。
//hash值位数：murmurhash支持32、64、128bit， cityhash支持64、128、256bit


