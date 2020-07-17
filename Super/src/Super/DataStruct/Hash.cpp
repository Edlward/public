#include "hash.h"
#include <stdint.h>


#if 0
���־����hash�㷨
    ���������У�û��Hash������˵����ֻ�е�������˵����
    ���ĳ�������ڸ��������ʱ�򣬺����׼������������
    �������������ʱ�򣬺��Ѽ����������������ǵ������
    ���ּ��ܺ� �������Ա���Ϊ�ǵ������ıƽ���Hash���������߳�Ϊɢ�к�����Ҳ���Կ����ǵ�������һ���ƽ��������ӽ������㵥�����Ķ��塣

    Hash������������ĺ��塣ʵ���е�Hash������ָ��һ����Χӳ�䵽һ��С��Χ���Ѵ�Χӳ�䵽һ��С��Χ��Ŀ��������Ϊ�˽�ʡ�ռ䣬ʹ���������ױ��档�������⣬Hash��������Ӧ���ڲ����ϡ����ԣ��ڿ���ʹ��Hash����֮ǰ����Ҫ�������ļ������ƣ�

    1. Hash����Ҫԭ����ǰѴ�Χӳ�䵽С��Χ�����ԣ��������ʵ��ֵ�ĸ��������С��Χ�൱���߱�����С����Ȼ��ͻ�ͻ�ܶࡣ
    2. ����Hash�ƽ������������ԣ�����������������ݽ��м��ܡ�
    3. ��ͬ��Ӧ�ö�Hash�������Ų�ͬ��Ҫ�󣻱��磬���ڼ��ܵ�Hash������Ҫ�������͵�����Ĳ�࣬�����ڲ��ҵ�Hash������Ҫ������ӳ�䵽С��Χ�ĳ�ͻ�ʡ�

    Ӧ���ڼ��ܵ�Hash�����Ѿ�̽�ֹ�̫���ˣ������ߵĲ��������и���ϸ�Ľ��ܡ����ԣ�����ֻ̽�����ڲ��ҵ�Hash������

    Hash����Ӧ�õ���Ҫ���������飨���磬�ַ�����������Ŀ��һ����һ��int���͡��������Ƕ��������ַ�ʽ��˵����

    һ���˵��Hash�������Լ򵥵Ļ���Ϊ���¼��ࣺ
    1. �ӷ�Hash��
    2. λ����Hash��
    3. �˷�Hash��
    4. ����Hash��
    5. ���Hash��
    6. ���Hash��
    ������ϸ�Ľ������ϸ��ַ�ʽ��ʵ���е����á�

    һ �ӷ�Hash
    ��ν�ļӷ�Hash���ǰ�����Ԫ��һ��һ���ļ������������Ľ������׼�ļӷ�Hash�Ĺ������£�

    static int additiveHash(String key, int prime)
{
    int hash, i;
    for (hash = key.length(), i = 0; i < key.length(); i++)
        hash += key.charAt(i);
    return (hash % prime);
}
�����prime����������������ó��������ֵ��Ϊ[0,prime-1]��

    �� λ����Hash
    ������Hash����ͨ�����ø���λ���㣨����������λ���������ֵĻ������Ԫ�ء����磬��׼����תHash�Ĺ������£�

    static int rotatingHash(String key, int prime)
{
    int hash, i;
    for (hash=key.length(), i=0; i<key.length(); ++i)
        hash = (hash<<4)^(hash>>28)^key.charAt(i);
    return (hash % prime);
}

����λ��Ȼ���ٽ��и���λ��������������Hash��������Ҫ�ص㡣���磬���ϵ��Ƕμ���hash�Ĵ��뻹���������¼��ֱ��Σ�
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
5.     hash = key.charAt(i) + (hash<<6) + (hash>>16) �C hash;
6.     hash ^= ((hash<<5) + key.charAt(i) + (hash>>2));

�� �˷�Hash
    �������͵�Hash���������˳˷��Ĳ�����ԣ��˷����������ʣ���������Ī����ƽ��ȡͷβ������������㷨����Ȼ�����㷨Ч�������ã������磬

static int bernstein(String key)
{
    int hash = 0;
    int i;
    for (i=0; i<key.length(); ++i) hash = 33*hash + key.charAt(i);
    return hash;
}

jdk5.0�����String���hashCode()����Ҳʹ�ó˷�Hash����������ʹ�õĳ�����31���Ƽ��ĳ������У�131, 1313, 13131, 131313�ȵȡ�

ʹ�����ַ�ʽ������Hash�������У�
//  32λFNV�㷨
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

�Լ��Ľ���FNV�㷨��
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

���˳���һ���̶������������Ļ��г���һ�����ϸı���������磺
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

��ȻAdler32�㷨��Ӧ��û��CRC32�㷺���������������ǳ˷�Hash������������һ���ˡ��������Ľ��ܣ���ҿ���ȥ��RFC 1950�淶��

    �� ����Hash
    �����ͳ˷�һ����ͬ�����б����Ͽ������Ĳ�����ԡ���������Ϊ����̫�������ַ�ʽ�����Ҳ���������Ӧ�á���Ҫע����ǣ�������ǰ�濴����hash�� �������һ��prime��Ŀ��ֻ��Ϊ�˱�֤����ķ�Χ������㲻��Ҫ������һ����Χ�Ļ�������ʹ�����µĴ��������hash%prime���� hash = hash ^ (hash>>10) ^ (hash>>20)��

    �� ���Hash
    ���Hash������������Ī����CRCϵ���㷨����ȻCRCϵ���㷨�������ǲ�����ǣ����������һ������ʵ�ַ�ʽ�����Hash�������������У�Universal Hashing��Zobrist Hashing�����ǵı����������ɵġ�

    �� ���Hash
    ���Hash�㷨���������ϸ��ַ�ʽ�����ֳ�����Hash�㷨������MD5��Tiger�����������Χ������һ�������������ҵ�Hash��������ʹ�á�

    �� ��Hash�㷨������
http://www.burtleburtle.net/bob/hash/doobs.html ���ҳ���ṩ�˶Լ�������Hash�㷨�����ۡ����Ƕ�Hash�����Ľ������£�

1. �ַ�����Hash����򵥿���ʹ�û����ĳ˷�Hash��������Ϊ33ʱ������Ӣ�ĵ����кܺõ�ɢ��Ч����С��6����Сд��ʽ���Ա�֤û�г�ͻ��������һ�����ʹ��FNV�㷨������Ľ���ʽ���������ڱȽϳ����ַ��������ٶȺ�Ч���϶�����

2. �������Hash������ʹ��http://burtleburtle.net/bob/c/lookup3.c�����㷨����һ���������ֽڣ��ٶȻ��㲻��

�� ���
    ���ļ��ԵĽ�����һ��ʵ��Ӧ���е����ڲ��ҵ�Hash�㷨��Hash�㷨����Ӧ��������������⣬����һ��������Ӧ���Ǿ����ַ���ƥ�䣨��ʱ�� Hash�㷨������rolling hash����Ϊ��������Թ����ļ��㣩�����һ�������õ�Hash�㷨������һ�����׵����顣��ΪӦ����˵��ѡ��һ���ʺϵ��㷨������Ҫ�ġ�

�� ����hash
inline int hashcode(const int *v)
{
    int s = 0;
    for(int i=0; i<k; i++)
        s=((s<<2)+(v[i]>>4))^(v[i]<<10);
    s = s % M;
    s = s < 0 ? s + M : s;
    return s;
}

ע����˵���ϵ�hash�ܼ���̶ȵر����ͻ�����ǳ�ͻ����������ġ���������������hash��������Ҫ���ϴ����ͻ�ķ�����
#endif

//typedef unsigned int uint;
//typedef unsigned char uchar;


//�ӷ�Hash ���ǰ�����Ԫ��һ��һ���ļ������������Ľ���� //prime��������������ó������ֵ��Ϊ[0,prime-1]��
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

//λ����Hash ���ø���λ���㣨����������λ���������ֵĻ������Ԫ�ء����磬��׼����תHash���£�
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

//���ϵ��Ƕμ���hash�Ĵ��뻹���������¼��ֱ��Σ�
//1. hash = (hash<<5)^(hash>>27)^key.charAt(i);
//2. hash += key.charAt(i);
    //hash += (hash << 10);
    //hash ^= (hash >> 6);
//4.     hash += (hash<<5) + key.charAt(i);
//5.     hash = key.charAt(i) + (hash<<6) + (hash>>16) �C hash;
//6.     hash ^= ((hash<<5) + key.charAt(i) + (hash>>2));
//3.  if((i&1) == 0)
    //hash ^= (hash<<7) ^ key.charAt(i) ^ (hash>>3);
    //else
    //hash ^= ~((hash<<11) ^ key.charAt(i) ^ (hash >>5));


//�˷�Hash,�����˳˷��Ĳ�����ԣ��˷����������ʣ���������Ī����ƽ��ȡͷβ������������㷨����Ȼ�����㷨Ч�������ã���
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

//jdk5.0�����String���hashCode()����Ҳʹ�ó˷�Hash����������ʹ�õĳ�����31��
//�Ƽ��ĳ������У�131, 1313, 13131, 131313�ȵȡ�

//���˳���һ���̶������������Ļ��г���һ�����ϸı���������磺 RSHash

//�� ����hash
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

//ע����˵���ϵ�hash�ܼ���̶ȵر����ͻ�����ǳ�ͻ����������ġ���������������hash��������Ҫ���ϴ����ͻ�ķ�����



//PHP�г��ֵ��ַ���Hash����
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


//OpenSSL �Ŀ��lhash.c �ļ��ṩ�ɷNHashCode ��Ӌ�㷽��

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
//MySql�г��ֵ��ַ���Hash����

#ifndef NEW_HASH_FUNCTION 
/* Calc hashvalue for a key (���ִ�Сд)*/
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

/* Calc hashvalue for a key, case indepenently �������ִ�Сд�� */
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
    * This hash produces the fewest collisions of any function that we��ve seen so 
    * far, and works well on both numbers and strings. 
    */

    //�����ִ�Сд��
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

//�������ִ�Сд��
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


//����times33�㷨���£������ַ���
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

//times33��ϣ�㷨(Perl��Berkeley DB��Apache��ʹ�õ�hash�㷨)

//Time33���ַ�����ϣ���������ڼ����������е�HashMap��������DJB Hash Function���׳ơ�Times33���㷨��
//Times33���㷨�ܼ򵥣����ǲ��ϵĳ�33��


unsigned int time33(char *str)
{
    unsigned int hash = 5381;  //Ϊʲô��ʼֵ��5381��5381��001 010 100 000 101������˵hash��ķֲ�����һЩ��
    while(*str)
    {
        hash += (hash << 5 ) + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}



//time33 ��ϣ�������ֽ� DJBX33A��Bernstein's hash
//PHP, apache, perl, bsddb��ʹ��time33��ϣ.
//Ϊʲô��33 ����������
//Ngixʹ�õ��� time31
//Tokyo Cabinetʹ�õ��� time37
//Bob����������˵��СдӢ�Ĵʻ��ʺ�33, ��Сд���ʹ��65��time33�Ƚ��ʺϵ���Ӣ�Ĵʻ��hash.

//ԭ��:   hash��i�� = hash��i-1��*33 + str[i] ;
//��򵥵İ汾  ����汾���������time33���㷨˼·�����򵥡�
unsigned long hash_time33_1(char const *str, int len)
{
    unsigned long  hash = 0;
    for (int i = 0; i < len; i++)
    {
        hash = hash *33 + (unsigned long) str[i];
    }
    return hash;
}

//�ѳ˷���������λ����
unsigned long hash_time33_2(char const *str, int len)
{
    unsigned long  hash = 0;
    for (int i = 0; i < len; i++) {
        hash = ((hash <<5) + hash) + (unsigned long) str[i];
    }
    return hash;
}
//59���ַ�1000 0000�����У�gccû�п����Ż�����Ϊ�����Ż�������������ʵ�ʴ����һ��)
//php�汾
unsigned hash_time33_php(char const*str, int len)
{
    unsigned long hash = 5381;
    //php�汾��hash��ʼֵ��5381, ���
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
//59���ַ�,1000 0000��
//real    0m1.088s
//user    0m1.068s
//sys     0m0.000s

//Apache�汾
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
//���Խ��
//real    0m1.418s
//user    0m1.412s
//sys     0m0.004s




//��������е�һ��hash����

//�ø�Ԫ�ظ�����ӽ���������Ϊɢ�б�Ĵ�С
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


//����hash�㷨������
//RS hash �㷨
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

//JS hash �㷨
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

//PJW hash �㷨
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

//ELF hash �㷨
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

//BKDR hash �㷨
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

//SDBM hash �㷨
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

//DJB hash �㷨 (times33)-����õ÷ǳ��࣬�ܶ�ⶼ������
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

//DEK hash �㷨
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

//BP hash �㷨
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

//FNV hash �㷨
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

//AP hash �㷨
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
�����㷨����
    Hash����	����1	����2	����3	����4	����1�÷�	����2�÷�	����3�÷�	����4�÷�	ƽ����
    BKDRHash	2	0	    4774	481	    96.55	100	   90.95	82.05	92.64
    APHash	2	3	4754	493	    96.55	88.46	100	   51.28	86.28
    DJBHash	2	2	4975	474	    96.55	92.31	0	   100	    83.43
    JSHash	1	4	4761	506	    100	    84.62	96.83  17.95	81.94
    RSHash	1	0	4861	505	    100	    100	    51.58  20.51	75.96
    SDBMHash	3	2	    4849	504	    93.1	92.31  57.01	23.08	72.41
    PJWHash	30	26	4878	513	0	0	    43.89	0	   21.95
    ELFHash	30	26	4878	513	0	0	    43.89	0	   21.95

    ����
    ����1Ϊ100000����ĸ��������ɵ��������ϣ��ͻ������
    ����2Ϊ100000���������Ӣ�ľ��ӹ�ϣ��ͻ������
    ����3Ϊ����1�Ĺ�ϣֵ��1000003(������)��ģ��洢�����Ա��г�ͻ�ĸ�����
    ����4Ϊ����1�Ĺ�ϣֵ��10000019(��������)��ģ��洢�����Ա��г�ͻ�ĸ�����
    �����Ƚϣ��ó�����ƽ���÷֡�ƽ����Ϊƽ��ƽ������
    ���Է��֣�
    BKDRHash��������ʵ��Ч�����Ǳ���ʵ���У�Ч��������ͻ���ġ�
    APHashҲ�ǽ�Ϊ������㷨��DJBHash,JSHash,RSHash��SDBMHash����ǧ�
    PJWHash��ELFHashЧ�������÷����ƣ����㷨���������Ƶġ�
#endif


    //MurmurHash - �ǳ��µ�һ����ϣ�㷨��Ӧ����ĿǰЧ����ߵ�һ����ϣ�㷨��ʹ���ʺܸߡ�
    //MurmurHash3������
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

//������
 

#include<iostream>
#include<string>
class Employee
{
public:
    Employee(){name=""; salary=0.0; seniority=0;};
    Employee(const std::string & n, double sal, int sen):name(n), salary(sal), seniority(sen)
    {

    }
    //��ø����name��Ա
    std::string getName() const
    { 
        return name; 
    }

    //�����ơ�<<��: û���ҵ����ܡ�std::string�����͵��Ҳ������������  ����취: ��#include<string>���治��.h
    //����==����� 
    //bool operator== (const Employee& rhs) const
    //{ 
    //    return this->getName()==rhs.getName(); 
    //}

    friend bool operator== (const Employee& lhs,const Employee& rhs);

    //����!=����� 
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
*   �������ƣ�hash(const HashedObj & key)
*   ��������: ���ݼ�ֵ���hashֵ,��������Ǹ���һ���ض�����ѧ��ʽ 
*   �����б�: key ������ļ�ֵ 
*   ���ؽ��������һ��ͨ��ɢ�к�����õ�ֵ
*****************************************************************/
unsigned int hash(const std::string & key)
{
    unsigned int hashVal = 0;

    //��ɢ�к������Ǹ���ʽ���
    for(int i = 0; i < key.length(); ++i)
    {
        hashVal = 37*hashVal + key[i];
    }
    return hashVal;
}

/****************************************************************
*   �������ƣ�hash(const HashedObj & key)
*   ��������: ���ݼ�ֵ���hashֵ,��������Ǹ���һ���ض�����ѧ��ʽ 
*   �����б�: key ������ļ�ֵ 
*   ���ؽ��������һ��ͨ��ɢ�к�����õ�ֵ
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
        std::cout<<"��"<<std::endl;  
    }
    hi.remove(400);  
    if(hi.contain(400))
    {
        std::cout<<"��"<<std::endl; 
    }
    else
    {
        std::cout<<"��"<<std::endl;  
    }
    std::cout<<std::endl;  
    hi.print();  
    hi.rehash();  
    std::cout<<"��ɢ��"<<std::endl;  
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
    //���Բ��Һ���findElement
    std::cout << "���Բ��Һ���findElement: " << std::endl;
    Employee e11 = hashTable.findElement(e3);
    std::cout << "e3 = " << e11;
    Employee e13 = hashTable.findElement(e4);
    std::cout << "e4 = " << e13;
    std::cout << std::endl;

    std::cout << "���԰�������containes: " << std::endl; 
    if(hashTable.contain(e2))
        std::cout << "containe e2" << std::endl;
    else 
        std::cout << "not containe e2" << std::endl;

    if(hashTable.contain(e4))
        std::cout << "containe e4" << std::endl;
    else 
        std::cout << "not containe e4" << std::endl;

    std::cout << "����remove()��" << std::endl;
    hashTable.remove(e1);
    if(hashTable.contain(e1))
        std::cout << "containe e1" << std::endl;
    else 
        std::cout << "not containe e1" << std::endl;
    std::cout << std::endl;

    std::cout << "����isEmpty(): " << std::endl;
    if(hashTable.isEmpty())
        std::cout << "hashTable is Empty " << std::endl;
    else
        std::cout << "hashTable is not Empty " << std::endl;
    std::cout << std::endl;
    std::cout << "����makeEmpty(): " << std::endl;
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



//cityhash��murmurhash��ȣ��ٶ��Կ�
//ʹ�ø��Ӷȣ����ǳ���
//����֧�֣�murmurhash�������Ի�����֧�֣�cityhash 2010�ŷ����㷨��2011�귢��ʵ�֣�Ŀǰֻ��c++��֧�֡�
//hashֵλ����murmurhash֧��32��64��128bit�� cityhash֧��64��128��256bit


