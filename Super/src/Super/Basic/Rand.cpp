#include "Rand.h"

//平均分布随机数，某种概率分布随机数，待续


//
//产生一定范围随机数的通用表示公式
//要取得[a,b)的随机整数，使用(rand() % (b-a))+ a; 
//要取得[a,b]的随机整数，使用(rand() % (b-a+1))+ a; 
//要取得(a,b]的随机整数，使用(rand() % (b-a))+ a + 1; 
//通用公式:a + rand() % n；其中的a是起始值，n是整数的范围。 
//要取得a到b之间的随机整数，另一种表示：a + (int)b * rand() / (RAND_MAX + 1)。 
//要取得0～1之间的浮点数，可以使用rand() / double(RAND_MAX)。




//梅森旋转算法随机数C 语言实现


#include<cstdio>                                /*Periodparameters*/ 
#define N 624 
#define M 397 
#define MATRIX_A 0x9908b0dfUL       /*constantvectora*/ 
#define UPPER_MASK 0x80000000UL  /*mostsignificantw-rbits*/ 
#define LOWER_MASK 0x7fffffffUL      /*leastsignificantrbits*/ 

static unsigned long mt[N];                 /*thearrayforthestatevector*/ 
static int mti=N+1;                              /*mti==N+1meansmt[N]isnotinitialized*/ /*initializesmt[N]withaseed*/ 
void init_genrand(unsigned long s) 
{ 
    mt[0]=s & 0xffffffffUL; 
    for(mti=1;mti<N;mti++) 
    { 
        mt[mti]= 
            (1812433253UL*(mt[mti-1]^(mt[mti-1]>>30))+mti); /*SeeKnuthTAOCPVol2.3rdEd.P.106formultiplier.*//*Inthepreviousversions,MSBsoftheseedaffect*//*onlyMSBsofthearraymt[].*//*2002/01/09modifiedbyMakotoMatsumoto*/ 
        mt[mti]&=0xffffffffUL;/*for>32bitmachines*/ 
    } 
} 
/*initializebyanarraywitharray-length*//*init_keyisthearrayforinitializingkeys*//*key_lengthisitslength*/ /*slightchangeforC++,2004/2/26*/ 

void init_by_array(unsigned long init_key[],int key_length) 
{ 
    int i,j,k; init_genrand(19650218UL); 
    i=1;j=0; 
    k=(N>key_length?N:key_length); 
    for(;k;k--) 
    { 
        mt[i]=(mt[i]^((mt[i-1]^(mt[i-1]>>30))*1664525UL)) 
            +init_key[j]+j;/*nonlinear*/ 
        mt[i]&=0xffffffffUL;/*forWORDSIZE>32machines*/ 
        i++;j++; if(i>=N){mt[0]=mt[N-1];i=1;}if(j>=key_length)j=0; 
    } 
    for(k=N-1;k;k--) 
    { mt[i]=(mt[i]^((mt[i-1]^(mt[i-1]>>30))*1566083941UL)) -i;/*nonlinear*/ 
    mt[i]&=0xffffffffUL;/*forWORDSIZE>32machines*/i++; 
    if(i>=N){mt[0]=mt[N-1];i=1;} } 
    mt[0]=0x80000000UL; 
    /*MSBis1;assuringnon-zeroinitialarray*/ } /*generatesarandomnumberon[0,0xffffffff]-interval*/ 


static unsigned long mag01[2]={0x0UL,MATRIX_A};/*mag01[x]=x*MATRIX_Aforx=0,1*/ 
unsigned long genrand_int32(void) 
{ 
    unsigned long y; 
    if(mti>=N)
    {
        /*generateNwordsatonetime*/ 
        int kk; 
    if(mti==N+1)/*ifinit_genrand()hasnotbeencalled,*/ 
    {
        init_genrand(5489UL);/*adefaultinitialseedisused*/ 
    }
    for(kk=0;kk<N-M;kk++) 
    { 
        y=(mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);mt[kk]=mt[kk+M]^(y>>1)^mag01[y&0x1UL]; 
    }
    for(;kk<N-1;kk++) 
    { 
        y=(mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK); 
        mt[kk]=mt[kk+(M-N)]^(y>>1)^mag01[y&0x1UL]; 
    } 
    y=(mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK); 
    mt[N-1]=mt[M-1]^(y>>1)^mag01[y&0x1UL];mti=0; } 
    y=mt[mti++]; /*Tempering*/ 
    y^=(y>>11); y^=(y<<7)&0x9d2c5680UL;y^=(y<<15)&0xefc60000UL;y^=(y>>18); 
    return y; 
} 
/*generatesarandomnumberon[0,0x7fffffff]-interval*/ 
long genrand_int31(void) 
{ 
    return(long)(genrand_int32()>>1); 
}
/*generatesarandomnumberon[0,1]-real-interval*/ 
double genrand_real1(void) 
{ 
    return genrand_int32()*(1.0/4294967295.0); 
} 
double genrand_real2(void) 
{
    return genrand_int32()*(1.0/4294967296.0);/*dividedby2^32*/ 
} 
/*generatesarandomnumberon(0,1)-real-interval*/ 
double genrand_real3(void) 
{ 
    return(((double)genrand_int32())+0.5)*(1.0/4294967296.0);/*dividedby2^32*/ 
} 
/*generatesarandomnumberon[0,1)with53-bitresolution*/ 
double genrand_res53(void) 
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6; 
return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/*TheserealversionsareduetoIsakuWada,2002/01/09added*/ 


int Test_Rand(void)
{ 
    int i; 
    unsigned long init[4]={0x123,0x234,0x345,0x456};
    unsigned long length=4;
    init_by_array(init,length); 
    printf("1000outputsofgenrand_int32()\n"); 
    for(i=0;i<1000;i++) 
    { 
        printf("%10lu",genrand_int32()); 
        if(i%5==4)
            printf("\n"); 
    } 
    printf("\n1000outputsofgenrand_real2()\n"); 
    for(i=0;i<1000;i++)
    { 
        printf("%10.8f",genrand_real2()); 
        if(i%5==4)
            printf("\n"); 
    } 
    return 0; 
} 









//
////性能较好的随机数发生器
////产生随机数的最简单的方法是线性同余数发生器，它于1951年由Lehmer首先提出，数x1,x2,... 的生成满足：
////xi+1 = Axi mod M
////为了开始这个序列，必须给出x0的某个值，这个值叫做种子(seed)。
//
//static const int A = 48271;
//static const int M = 2147483647;
//static const int Q = M / A;
//static const int R = M % A;
//
//class Random
//{
//    public:
//        explicit Random( int initialValue = 1 );
//        int randomInt();
//        double random0_1();
//        int randomInt( int low, int high);
//    private:
//       int state;
//};
//
///**
// * Construct with initialValue for the state.
// */
//Random::Random( int initialValue)
//{
//      if( initialValue < 0)
//          initialValue += M;
//      state = initialValue;
//      if( state == 0)
//          state = 1;
//}
//
///**
// *Return a pseudorandom int, and change the internal state.
// */
//int Random::randomInt()
//{
//    int tmpState = A * ( state % Q ) - R * (state / Q );
//
//    if( tmpState >= 0)
//        state = tmpState;
//    else
//        state = tmpState + M;
//    return state;
//}
//
///**
// * Return a pseudorandom double in the open range 0..1
// * and change the internal state.
// */
//double Random::random0_1( )
//{
//    return (double) randomInt( ) / M;
//}
//
///**
// * Return an int in the closed range [low,high], and
// * change the internal state.
// */
//int Random::randomInt( int low, int high )
//{
//    double partitionSize = (double) M / ( high - low + 1 );
//
//    return (int) ( randomInt( ) / partitionSize ) + low;
//}




//
//数学原理可以参考下面的文章：
//一种快速的随机数生成算法
//The GLIBC random number generator
//无重复生成 1 ~ 2^31-2 之间的整数，速度极快，均匀性好，非常实用。
#if 0

class Random 
{
public:
    explicit Random(unsigned int seed = 1) : _seed(seed & 0x7fffffffu) {
        if (_seed == 0 || _seed == 2147483647L) {
            _seed = 1;
        }
    }

    unsigned int next() {
        static const unsigned int M = 2147483647L;  // 2^31-1
        static const uint64_t A = 16385;  // 2^14+1

        uint64_t product = _seed * A;

        _seed = static_cast<unsigned int>((product >> 31) + (product & M));
        if (_seed > M) _seed -= M;

        return _seed;
    }

private:
    unsigned int _seed;
};
#endif







#if 0
//stdlib.c 源代码 rand
//    原来在做单板机，而随机出来的是有规律可循的随机数，于是乎。。。。。。。
//
//    想知道rand()这个函数是咋实现的，就在网上搜索了一下，选择了三个实现版本，摘要其关键代码如下
//    GNU的实现完全不懂    ─_─ #
//    ==========================================================
//    一、sanos
//http://www.jbox.dk/sanos/source/lib/stdlib.c.html
//
//static long holdrand = 1L;
//
//void srand(unsigned int seed)
//{
//    holdrand = (long) seed;
//}
//
//int rand()
//{
//    return (((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
//}
//
//
//==========================================================
//    二、FreeBSD
//http://www.freebsd.org/cgi/cvsweb.cgi/src/lib/libc/stdlib/
//
//static int
//    do_rand(unsigned long *ctx)
//{
//#ifdef USE_WEAK_SEEDING
//    /*
//    * Historic implementation compatibility.
//    * The random sequences do not vary much with the seed,
//    * even with overflowing.
//    */
//    return ((*ctx = *ctx * 1103515245 + 12345) % ((u_long)RAND_MAX + 1));
//#else    /* !USE_WEAK_SEEDING */
//    /*
//    * Compute x = (7^5 * x) mod (2^31 - 1)
//    * wihout overflowing 31 bits:
//    *       (2^31 - 1) = 127773 * (7^5) + 2836
//    * From "Random number generators: good ones are hard to find",
//    * Park and Miller, Communications of the ACM, vol. 31, no. 10,
//    * October 1988, p. 1195.
//    */
//    long hi, lo, x;
//
//    /* Can't be initialized with 0, so use another value. */
//    if (*ctx == 0)
//        *ctx = 123459876;
//    hi = *ctx / 127773;
//    lo = *ctx % 127773;
//    x = 16807 * lo - 2836 * hi;
//    if (x < 0)
//        x += 0x7fffffff;
//    return ((*ctx = x) % ((u_long)RAND_MAX + 1));
//#endif /* !USE_WEAK_SEEDING */
//}
//
//
//int
//    rand_r(unsigned int *ctx)
//{
//    u_long val = (u_long) *ctx;
//    int r = do_rand(&val);
//
//    *ctx = (unsigned int) val;
//    return (r);
//}
//
//
//static u_long next = 1;
//
//int
//    rand()
//{
//    return (do_rand(&next));
//}
//
//void
//    srand(seed)
//    u_int seed;
//{
//    next = seed;
//}
//
//=============================================================
//
//    三、GNU libc
//http://ftp.gnu.org/gnu/glibc/
//
//
//int
//    __random_r (buf, result)
//struct random_data *buf;
//int32_t *result;
//{
//    int32_t *state;
//
//    if (buf == NULL || result == NULL)
//        goto fail;
//
//    state = buf->state;
//
//    if (buf->rand_type == TYPE_0)
//    {
//        int32_t val = state[0];
//        val = ((state[0] * 1103515245) + 12345) & 0x7fffffff;
//        state[0] = val;
//        *result = val;
//    }
//    else
//    {
//        int32_t *fptr = buf->fptr;
//        int32_t *rptr = buf->rptr;
//        int32_t *end_ptr = buf->end_ptr;
//        int32_t val;
//
//        val = *fptr += *rptr;
//        /* Chucking least random bit. */
//        *result = (val >> 1) & 0x7fffffff;
//        ++fptr;
//        if (fptr >= end_ptr)
//        {
//            fptr = state;
//            ++rptr;
//        }
//        else
//        {
//            ++rptr;
//            if (rptr >= end_ptr)
//                rptr = state;
//        }
//        buf->fptr = fptr;
//        buf->rptr = rptr;
//    }
//    return 0;
//
//fail:
//    __set_errno (EINVAL);
//    return -1;
//}
#endif