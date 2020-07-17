#ifndef _INT24_Header_
#define _INT24_Header_

#include <stdint.h>  //for int64_t
#include <fstream>
//同时考虑大小端并静态决议待续  


const int INT24_MAX = 8388607;
const int INT24_MIN = -8388608;

const float FINT24_MAX =INT24_MAX;
const float FINT24_MIN =INT24_MIN;

const double Recip_INT24_MAX =1.0/INT24_MAX;  //倒数 reciprocal 
const double Recip_INT24_MIN =1.0/INT24_MIN;

//抹平越界
inline int FlattnOutOfRange(int tmp)
{
    if(tmp <= INT24_MIN)
    {
       return INT24_MIN;
    }
    else if(tmp > INT24_MAX)
    {
        return INT24_MAX;
    }
    return tmp;
}


inline float FlattnOutOfRange(float tmp)
{
    if(tmp <= FINT24_MIN)
    {
       return  FINT24_MIN;
    }
    else if(tmp > FINT24_MAX)
    {
       return  FINT24_MAX;
    }
    return tmp;
}


#pragma pack(push, 1)   //为了作为音频cardinput帧保存到PM，设置为1字节对齐,但是可能影响效率


//仅考虑小端机器,大端补全待续
class Int24
{
    ////fromhttps://www.cnblogs.com/sdragonx/p/10933096.html
    static inline int32_t f64toint32(double x)
    {
        x += 6755399441055744.0;
        return *(int32_t*)&x;
    }
    //四舍五入，处理的数据范围是-2^22 ~ 2^22-1, -4194304.0 ~ 4194303.0
    static inline int32_t f32toint32(float x)
    {
        //取得符号位，设置掩码
        uint32_t n = ((*(uint32_t*)&x)&0x80000000)?0xFFC00000 : 0;//一个三元操作符，直接储存掩码
        x += 12582912.0f;//魔法数字加法
        return ((*(uint32_t*)&x)&0x3FFFFF)|n;//直接or运算
    }

private:
public:     //为了便于操作改公有
    unsigned char m_bytes[3];    
public:
    Int24()
    {
		m_bytes[0] = m_bytes[1] = m_bytes[2] = 0;     
        //没有必要设默认值，在作为局部数组频繁使用时浪费性能，在需要时清零即可   modify by lizulin
    }

    Int24( const int val )
    {
        *this = val;
    }

    Int24( const Int24& val )
    {
        *this = val;
    }

    //bool isZero()   //是否等于0，快速方法 add by lizulin
    //{
    //   if((m_bytes[2]==0)&(m_bytes[1]==0)&(m_bytes[0]==0))
    //        return true;
    //    return false;
    //}

    bool LessThenZero() const            //是否小于0，只判断符号位，快速方法 add by lizulin
    {
        if (m_bytes[2]&0x80) // Is this a negative?  Then we need to siingn extend.
        {
            return true;
        }
        return false;
    }

    bool greatherThenZero() const       //是否大于0，add by lizulin
    {
        if (m_bytes[2]&0x80 ) // Is this a negative?  Then we need to siingn extend.
        {
            return false;
        }
        return true;
    }

	operator short() const
	{
		return (m_bytes[2] << 8) | (m_bytes[1] << 0);
	}

    operator int() const
    {
        //仅考虑小端机器，
        //方法1：
        //if (m_bytes[2]&0x80) // Is this a negative?  Then we need to siingn extend.
        //{
        //    return (0xFF<<24)|(m_bytes[2]<<16)|(m_bytes[1]<<8)|(m_bytes[0]<<0);   
        //}
        //else
        //{
        //    return (m_bytes[2]<<16)|(m_bytes[1]<<8)|(m_bytes[0]<<0);
        //}

        //方法2:与方法1一样快
        int res = (m_bytes[2]<<16)|(m_bytes[1]<<8)|m_bytes[0];
        if (m_bytes[2]&0x80)     //此句用 (res&0x00800000) 反而慢两倍
        {
            res|=(~0xffffff); //integer|=0xFF000000;  //一样快
        }
        return res;
    }
    operator long long() const
    {
        return (long long)this->operator int();
	}
    operator float() const
    {
        return (float)this->operator int();
    }
	operator double() const
    {
        return (double)this->operator int();
    }

    //POD类型用系统默认实现也许更高效? arm下内存对不齐情况可能崩溃，还是要实现
    Int24& operator =( const Int24& input )
    {
        m_bytes[0]=input.m_bytes[0];
        m_bytes[1]=input.m_bytes[1];
        m_bytes[2]=input.m_bytes[2];
        return *this;
    }

    Int24& operator =(int input)   //(const int input)
    {
        //为了安全性暂时放此处。为了通用性应该移除，并在外部判断，待续
         int tmp=FlattnOutOfRange(input);   //越界与负数考虑 add by lizulin   
         //int tmp=input;

        m_bytes[0]=((unsigned char*)&tmp)[0];
        m_bytes[1]=((unsigned char*)&tmp)[1];
        m_bytes[2]=((unsigned char*)&tmp)[2];

        //处理帧率125hz--最快，但是不易于理解
        //m_bytes[0] =((unsigned char*)&tmp)[0];
        //const char* ps=(char*)&tmp;
        //*((short*)&m_bytes[1])=*((short*)&ps[1]);

        ////处理帧率115hz
         //m_bytes[0] = (tmp&0x000000ff);
         //m_bytes[1] = (tmp&0x0000ff00)>>8;
         //m_bytes[2] = (tmp&0x00ff0000)>>16;

         //m_bytes[0]=tmp;
         //m_bytes[1]=tmp>>8;
         //m_bytes[2]=tmp>>16;

        //old 
        //m_bytes[0]   = ((unsigned char*)&input)[0];
        //m_bytes[1]   = ((unsigned char*)&input)[1];
        //m_bytes[2]   = ((unsigned char*)&input)[2];
        return *this;
    }

    //高效写法？待测--不行反而更慢39hz
    //Int24& operator =( const int input )
    //{
    //    int* p=(int*)m_bytes;
    //    *p=(input|((*p)&0xFF000000));
    //    return *this;
    //}

	Int24& operator=(const short input )
	{
        m_bytes[0]=0;
        m_bytes[1]=((unsigned char*)&input)[0];
        m_bytes[2]=((unsigned char*)&input)[1];
        return *this;
	}
    Int24& operator =( const long long input )
    {
        return operator=((int)input);   //operator =(int)内会做溢出判断，没有必要重复
        //return *this;
    }

    Int24& operator =( const float input)
    {
        return operator=((int)input);   //operator =(int)内会做溢出判断，没有必要重复
        //return operator=(f32toint32(input));
    }
	Int24& operator =( const double input )
    {
        return operator=((int)input); //operator =(int)内会做溢出判断，没有必要重复
        //return *this;
    }

    /***********************************************/

    Int24 operator+(const Int24& val) const
    {
        return Int24((int)*this+(int)val);
    }

    Int24 operator-(const Int24& val) const
    {
        return Int24((int)*this-(int)val);
    }

    Int24 operator*(const Int24& val) const
    {
        return Int24((int)*this*(int)val);
    }

    Int24 operator/(const Int24& val) const
    {
        return Int24((int)*this/(int)val);
    }

    /***********************************************/

    int operator+(const int val) const
    {
        return ((int)*this+val);
    }

    int operator-(const int val) const
    {
        return ((int)*this-val );
    }

    int operator*(const int val) const
    {
        return ((int)*this*val);
    }

    int operator/(const int val) const
    {
        return ((int)*this/val);
    }

    /***********************************************/

    float operator+(const float val) const
    {
        return ((float)*this+val);
    }

    float operator-(const float val) const
    {
        return ((float)*this-val);
    }

    float operator*(const float val) const
    {
        return ((float)*this*val);
    }

    float operator/(const float val) const
    {
        return ((float)*this/val);
    }

    /***********************************************/

    double operator+(const double val) const
    {
        return ((double)*this+val);
    }

    double operator-(const double val) const
    {
        return ((double)*this-val);
    }

    double operator *(const double val ) const
    {
        return ((double)(*this)*val);
    }

    double operator /(const double val) const
    {
        return ((double)*this/val);
    }

    /***********************************************/

	friend int operator +(const int val1, const Int24 val2);
	
	friend int operator -(const int val1, const Int24 val2);
	
	friend int operator *(const int val1, const Int24 val2);

	friend int operator /(const int val1, const Int24 val2);
    /***********************************************/

	friend float operator +(const float val1, const Int24 val2);
	
	friend float operator -(const float val1, const Int24 val2);
	
	friend float operator *(const float val1, const Int24 val2);

	friend float operator /(const float val1, const Int24 val2);
    /***********************************************/

	friend double operator +(const double val1, const Int24 val2);
	
	friend double operator -(const double val1, const Int24 val2);
	
	friend double operator *(const double val1, const Int24 val2);

	friend double operator /(const double val1, const Int24 val2);
    /***********************************************/

    Int24& operator+=(const Int24& val)
    {
        *this=(*this)+val;
        return *this;
    }

    Int24& operator-=(const Int24& val)
    {
        *this=(*this)-val;
        return *this;
    }

    Int24& operator*=(const Int24& val)
    {
        *this=(*this)*val;
        return *this;
    }

    Int24& operator/=(const Int24& val)
    {
        *this=(*this)/val;
        return *this;
    }

    Int24& operator+=(const int val)
    {
        *this=(*this)+val;
        return *this;
    }
    Int24& operator+=(const long long val)
    {
        return operator+=((int)val);
        return *this;
    }
    Int24& operator-=(const int val)
    {
        *this=(*this)-val;
        return *this;
    }

    Int24& operator *=(const int val)
    {
        *this=(*this)*val;
        return *this;
    }

    Int24& operator/=(const int val )
    {
        *this   = *this / val;
        return *this;
    }

    /***********************************************/
	
    Int24& operator+=(const float val)
    {
        *this=*this+val;
        return *this;
    }

    Int24& operator-=(const float val)
    {
        *this=*this-val;
        return *this;
    }

    Int24& operator*=(const float val)
    {
        *this=(*this)*val;
        return *this;
    }

    Int24& operator /=(const float val)
    {
        *this=(*this)/val;
        return *this;
    }

	//add by lizulin 音频处理大多是float乘法，需要Int24转int在转float,运算完成后又float->Int->Int24,且浮点乘法效率低。
	//可以转成long long然后乘以整数gain 在转回int24即可
	//static long long toInt64Gain(float gain)
	//{
	//	return gain*(65536);
	//}
	//Int24 MulIntGain(long long gain)
	//{
	//	(long long )this*gain>>16;
	//}

    /***********************************************/
	
    Int24& operator+=(const double val)
    {
        *this=(*this)+ val;
        return *this;
    }

    Int24& operator -=(const double val)
    {
        *this=(*this)-val;
        return *this;
    }

    Int24& operator *=(const double val)
    {
        *this=(*this)*val;
        return *this;
    }

    Int24& operator /=( const double val )
    {
        *this   = *this / val;
        return *this;
    }

    /***********************************************/
    /***********************************************/

    Int24 operator >>(const int val) const
    {
        return Int24((int)*this>>val);
    }

    Int24 operator <<(const int val) const
    {
        return Int24((int)*this<<val);
    }

	Int24 operator ~( )const
	{
		return Int24(~(int)*this);
	}

    /***********************************************/

    Int24& operator >>=(const int val)
    {
        *this=(*this)>>val;
        return *this;
    }

    Int24& operator <<=(const int val)
    {
        *this=(*this)<<val;
        return *this;
    }

    /***********************************************/
    /***********************************************/

    operator bool() const
    {
        return (int)*this!=0;
    }

    bool operator !() const
    {
        return !((int)*this);
    }

    Int24 operator -()
    {
        return Int24(-(int)*this );
    }

    /***********************************************/
    /***********************************************/

    bool operator ==( const Int24& val ) const
    {
        return (int)*this==(int)val;
    }

    bool operator !=( const Int24& val ) const
    {
        return (int)*this!=(int)val;
    }

    bool operator >=(const Int24& val) const
    {
        return (int)*this >= (int)val;
    }

    bool operator <=(const Int24& val) const
    {
        return (int)*this<=(int)val;
    }

    bool operator >(const Int24& val) const
    {
        return (int)*this >(int)val;
    }

    bool operator <(const Int24& val) const
    {
        return (int)*this < (int)val;
    }

    /***********************************************/

    bool operator ==( const int val ) const
    {
        return (int)*this == val;
    }

    bool operator !=( const int val ) const
    {
        return (int)*this != val;
    }

    bool operator >=( const int val ) const
    {
        return (int)*this >= val;
    }

    bool operator <=( const int val ) const
    {
        return (int)*this <= val;
    }

    bool operator >( const int val ) const
    {
        return ((int)*this) > val;
    }

    bool operator <( const int val ) const
    {
        return (int)*this < val;
    }

    /***********************************************/

	friend std::ostream & operator<<(std::ostream &os, const Int24 &obj);
	friend std::istream & operator>>(std::istream &is, Int24 &obj);
    /***********************************************/
};
#pragma pack(pop)


//基础类型前置操作符重载
inline int operator+(const int val1,const Int24 val2)  //old const Int24 val2
{
    return (val1+(int)val2);
}

inline int operator-(const int val1,const Int24 val2)
{
    return (val1 - (int)val2);
}

inline int operator *(const int val1,const Int24 val2)
{
    return (val1 * (int)val2);
}

inline int operator/(const int val1,const Int24 val2)
{
    return (val1 / (int)val2);
}

inline float operator+(const float val1,const Int24 val2)
{
    return (val1 + (float)val2);
}

inline float operator-(const float val1,const Int24 val2)
{
    return (val1 - (float)val2);
}

inline float operator*(const float val1, const Int24 val2)
{
    return (val1 * (float)val2);
}

inline float operator/(const float val1, const Int24 val2)
{
    return (val1 / (float)val2);
}

inline double operator+(const double val1, const Int24 val2)
{
    return (val1 + (double)val2);
}

inline double operator-(const double val1, const Int24 val2)
{
    return (val1 - (double)val2);
}

inline double operator*(const double val1, const Int24 val2)
{
    return (val1 * (double)val2);
}

inline double operator/(const double val1, const Int24 val2)
{
    return (val1/(double)val2);
}

inline std::ostream& operator<<(std::ostream &os,const Int24 &obj)
{
    os<<(int)obj;
    return os;
}

inline std::istream&operator>>(std::istream &is,Int24 &obj)
{
    int num;
    is>>num;
    obj = Int24(num);
    return is;
}



#if 0
// Most architectures do not include a 24 bit integer
#ifndef INT24
#define INT24
#pragma pack(push, 1) // This will align all of the data nicely

// Most architectures do not support a 24 bit integer
// so a way to convert 24 bit integer data had to be
// defined. This structure is used for both its
// pointers as well as its native integer conversions. 
struct __int24 {

    Byte bytes[3];

#if LITTLE_ENDIAN

    __int24& operator = (const __int32& integer) {
        bytes[0] = (integer & 0x000000ff);
        bytes[1] = (integer & 0x0000ff00) >> 8;
        bytes[2] = (integer & 0x00ff0000) >> 16;
        return *this;
    }

    operator __int32 () {
        __int32 integer = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16);
        if (integer & 0x800000) 
            integer |= ~0xffffff;
        return integer;
    }

#else
#error endianness not supported
#endif // Endianness
};

#pragma pack(pop) // Restore old alignment
#endif // INT24 
#endif

#endif

