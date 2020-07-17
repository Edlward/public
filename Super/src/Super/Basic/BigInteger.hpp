#pragma once
//#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>



class BigInteger
{
public:
	static void setMax(__int32 len);
	static const __int32 getMax();
	BigInteger();
	BigInteger(__int16 value);
	BigInteger(__int32 value);
	BigInteger(__int64 value);
	BigInteger(unsigned __int16 value);
	BigInteger(unsigned __int32 value);
	BigInteger(unsigned __int64 value);
	BigInteger(const BigInteger& bi);
	BigInteger(std::string value, __int32 radix);
	const BigInteger	operator +( const BigInteger& T)const;
	const BigInteger	operator -( const BigInteger& T)const;
	const BigInteger	operator *( const BigInteger& T)const;
	const BigInteger	operator /( const BigInteger& T)const;
	const BigInteger	operator %( const BigInteger& T)const;
	const BigInteger	operator &( const BigInteger& T)const;
	const BigInteger	operator |( const BigInteger& T)const;
	const BigInteger	operator ^( const BigInteger& T)const;
	const BigInteger	operator >>(const __int32 shiftVal)const;
	const BigInteger	operator <<(const __int32 shiftVal)const;
	const bool			operator !=(const BigInteger& T)const;
	const bool			operator ==(const BigInteger& T)const;
	const bool			operator > (const BigInteger& T)const;
	const bool			operator < (const BigInteger& T)const;
	const bool			operator >=(const BigInteger& T)const;
	const bool			operator <=(const BigInteger& T)const;
	const BigInteger&	operator +=(const BigInteger& T);
	const BigInteger&	operator -=(const BigInteger& T);
	const BigInteger&	operator *=(const BigInteger& T);
	const BigInteger&	operator /=(const BigInteger& T);
	const BigInteger&	operator %=(const BigInteger& T);
	const BigInteger&	operator >>=(const __int32 shiftVal);
	const BigInteger&	operator <<=(const __int32 shiftVal);
	const BigInteger	operator ~()const ;
	const BigInteger	operator -()const ;
	const BigInteger	operator ++(__int32);
	const BigInteger&	operator ++();
	const BigInteger	operator --(__int32);
	const BigInteger&	operator --();
	const BigInteger	operator =(__int16 value);
	const BigInteger	operator =(__int32 value);
	const BigInteger	operator =(__int64 value);
	const BigInteger	operator =(unsigned __int16 value);
	const BigInteger	operator =(unsigned __int32 value);
	const BigInteger	operator =(unsigned __int64 value);
	const std::string ToString(__int32 radix = 10)const ;
	const BigInteger abs();
	const BigInteger sqrt();
	const BigInteger gcd(const BigInteger& T)const ;
	const __int32 bitCount();
	~BigInteger();
private:
	static __int32 maxLength;
	__int32 dataLength;
	unsigned __int32 *data;
	const bool Equals(const BigInteger& T)const;
	BigInteger(unsigned __int32* inData , __int32 dataLength );
	void multiByteDivide(BigInteger& bi1, BigInteger& bi2, BigInteger& outQuotient, BigInteger& outRemainder)const;
	void singleByteDivide(BigInteger& bi1, BigInteger& bi2, BigInteger& outQuotient, BigInteger& outRemainder)const;
	const __int32 shiftLeft(unsigned __int32* buffer, __int32 bufferLength, __int32 shiftVal)const;
	const __int32 shiftRight(unsigned __int32* buffer ,__int32 bufferLength, __int32 shiftVal)const;
};



__int32 BigInteger::maxLength=70;
void BigInteger::setMax(__int32 len)
{
	maxLength = len;
}
const __int32 BigInteger::getMax()
{
	return maxLength;
}
BigInteger::BigInteger()
{
    data = new unsigned __int32[maxLength];
	for(__int32 i = 0 ;i != maxLength ; i++)
		data[i] = 0;
    dataLength = 1;
}
BigInteger::BigInteger(__int16 value)
{
    data = new unsigned __int32[maxLength];
	for(__int32 i = 0 ;i != maxLength ; i++)
		data[i] = 0;
    __int64 tempVal = (__int64)value;
    dataLength = 0;
    while(value != 0 && dataLength < maxLength)
    {
        data[dataLength] = (unsigned __int32)(value & 0xFFFFFFFF);
        value = 0;
        dataLength++;
    }
    if(tempVal > 0)
    {
        if(value != 0 || (data[maxLength-1] & 0x80000000) != 0)
			throw(new std::overflow_error("Positive overflow in constructor."));
    }
    else if(tempVal < 0)
    {
        if(value != -1 || (data[dataLength-1] & 0x80000000) == 0)
            throw(new std::overflow_error("Negative underflow in constructor."));
    }
    if(dataLength == 0)
        dataLength = 1;
}
BigInteger::BigInteger(unsigned __int16 value)
{
    data = new unsigned __int32[maxLength];
	for(__int32 i = 0 ;i != maxLength ; i++)
		data[i] = 0;
    dataLength = 0;
    while(value != 0 && dataLength < maxLength)
    {
        data[dataLength] = (unsigned __int32)(value & 0xFFFFFFFF);
        value = 0;
        dataLength++;
    }
    if(value != 0 || (data[maxLength-1] & 0x80000000) != 0)
        throw(new std::overflow_error("Positive overflow in constructor."));
    if(dataLength == 0)
        dataLength = 1;
}
BigInteger::BigInteger(__int32 value)
{
    data = new unsigned __int32[maxLength];
	for(__int32 i = 0 ;i != maxLength ; i++)
		data[i] = 0;
    __int64 tempVal = (__int64)value;
    dataLength = 0;
    while(value != 0 && dataLength < maxLength)
    {
        data[dataLength] = (unsigned __int32)(value & 0xFFFFFFFF);
		value = 0;
        dataLength++;
    }
    if(tempVal > 0)
    {
        if(value != 0 || (data[maxLength-1] & 0x80000000) != 0)
			throw(new std::overflow_error("Positive overflow in constructor."));
    }
    else if(tempVal < 0)
    {
        if(value != -1 || (data[dataLength-1] & 0x80000000) == 0)
            throw(new std::overflow_error("Negative underflow in constructor."));
    }
    if(dataLength == 0)
        dataLength = 1;
}
BigInteger::BigInteger(unsigned __int32 value)
{
    data = new unsigned __int32[maxLength];
	for(__int32 i = 0 ;i != maxLength ; i++)
		data[i] = 0;
    dataLength = 0;
    while(value != 0 && dataLength < maxLength)
    {
        data[dataLength] = (unsigned __int32)(value & 0xFFFFFFFF);
        value = 0;
        dataLength++;
    }
    if(value != 0 || (data[maxLength-1] & 0x80000000) != 0)
        throw(new std::overflow_error("Positive overflow in constructor."));
    if(dataLength == 0)
        dataLength = 1;
}
BigInteger::BigInteger(__int64 value)
{
    data = new unsigned __int32[maxLength];
	for(__int32 i = 0 ;i != maxLength ; i++)
		data[i] = 0;
    __int64 tempVal = value;
    dataLength = 0;
    while(value != 0 && dataLength < maxLength)
    {
        data[dataLength] = (unsigned __int32)(value & 0xFFFFFFFF);
        value >>= 32;
        dataLength++;
    }
    if(tempVal > 0)
    {
        if(value != 0 || (data[maxLength-1] & 0x80000000) != 0)
			throw(new std::overflow_error("Positive overflow in constructor."));
    }
    else if(tempVal < 0)
    {
        if(value != -1 || (data[dataLength-1] & 0x80000000) == 0)
            throw(new std::overflow_error("Negative underflow in constructor."));
    }
    if(dataLength == 0)
        dataLength = 1;
}
BigInteger::BigInteger(unsigned __int64 value)
{
    data = new unsigned __int32[maxLength];
	for(__int32 i = 0 ;i != maxLength ; i++)
		data[i] = 0;
    dataLength = 0;
    while(value != 0 && dataLength < maxLength)
    {
        data[dataLength] = (unsigned __int32)(value & 0xFFFFFFFF);
        value >>= 32;
        dataLength++;
    }
    if(value != 0 || (data[maxLength-1] & 0x80000000) != 0)
        throw(new std::overflow_error("Positive overflow in constructor."));
    if(dataLength == 0)
        dataLength = 1;
}
BigInteger::BigInteger(unsigned __int32* inData , __int32 dataLength)
{
    dataLength = dataLength;
    if(dataLength > maxLength)
		throw(new std::overflow_error("Byte overflow in constructor."));
    data = new unsigned __int32[maxLength];
	for(__int32 i =0 ; i != maxLength ; i++)
		data[i] = 0;
    for(__int32 i = dataLength - 1, j = 0; i >= 0; i--, j++)
        data[j] = inData[i];
    while(dataLength > 1 && data[dataLength-1] == 0)
        dataLength--;
}
BigInteger::BigInteger(const BigInteger& bi)
{
    data = new unsigned __int32[maxLength];
	dataLength = bi.dataLength;
    for(__int32 i = 0; i < maxLength ; i++)
        data[i] = bi.data[i];
}
BigInteger::BigInteger(std::string value, __int32 radix)
{
	data = new unsigned __int32[maxLength];
    BigInteger multiplier = BigInteger(1);
    BigInteger result = BigInteger();
    transform(value.begin(), value.end(), value.begin(), (__int32 (*)(__int32))toupper);
    __int32 limit = 0;
	if(value.c_str()[0] == '-')
        limit = 1;
	for(__int32 i = value.size() - 1; i >= limit ; i--)
    {
		__int32 posVal = (__int32)(value.c_str()[i]);
        if(posVal >= '0' && posVal <= '9')
            posVal -= '0';
        else if(posVal >= 'A' && posVal <= 'Z')
            posVal = (posVal - 'A') + 10;
        else
            posVal = 9999999;
        if(posVal >= radix)
            throw(new std::overflow_error("Invalid string in constructor."));
        else
        {
			if(value.c_str()[0] == '-')
                posVal = -posVal;
			result = result + (multiplier * BigInteger(posVal));
            if((i - 1) >= limit)
                multiplier = multiplier * BigInteger(radix);
        }
    }
	if(value.c_str()[0] == '-')
    {
        if((result.data[maxLength-1] & 0x80000000) == 0)
            throw(new std::overflow_error("Negative underflow in constructor."));
    }
    else
    {
        if((result.data[maxLength-1] & 0x80000000) != 0)
            throw(new std::overflow_error("Positive overflow in constructor."));
    }
	for(__int32 i = 0 ;i != maxLength ; i++)
        data[i] = result.data[i];
    dataLength = result.dataLength;
}
const BigInteger BigInteger::operator +(const BigInteger& T)const
{
	BigInteger bi1 = BigInteger( *this );
	BigInteger bi2 = BigInteger( T );
    BigInteger result = BigInteger();
    result.dataLength = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
    __int64 carry = 0;
    for(__int32 i = 0; i < result.dataLength; i++)
    {
        __int64 sum = (__int64)bi1.data[i] + (__int64)bi2.data[i] + carry;
        carry  = sum >> 32;
        result.data[i] = (__int32)(sum & 0xFFFFFFFF);
    }
    if(carry != 0 && result.dataLength < maxLength)
    {
        result.data[result.dataLength] = (unsigned __int32)(carry);
        result.dataLength++;
    }
    while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
        result.dataLength--;
    __int32 lastPos = maxLength - 1;
    if((bi1.data[lastPos] & 0x80000000) == (bi2.data[lastPos] & 0x80000000) &&
        (result.data[lastPos] & 0x80000000) != (bi1.data[lastPos] & 0x80000000))
    {
		throw (new std::overflow_error(""));
    }
    return result;
}
const BigInteger BigInteger::operator -(const BigInteger& T)const
{
	BigInteger bi1 = BigInteger( *this );
	BigInteger bi2 = BigInteger( T );
    BigInteger result = BigInteger();
    result.dataLength = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
    __int64 carryIn = 0;
    for(__int32 i = 0; i < result.dataLength; i++)
    {
        __int64 diff;
        diff = (__int64)bi1.data[i] - (__int64)bi2.data[i] - carryIn;
        result.data[i] = (unsigned __int32)(diff & 0xFFFFFFFF);
        if(diff < 0)
            carryIn = 1;
        else
            carryIn = 0;
    }
    if(carryIn != 0)
    {
        for(__int32 i = result.dataLength; i < maxLength; i++)
            result.data[i] = 0xFFFFFFFF;
        result.dataLength = maxLength;
    }
    while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
        result.dataLength--;
    __int32 lastPos = maxLength - 1;
    if((bi1.data[lastPos] & 0x80000000) != (bi2.data[lastPos] & 0x80000000) &&
        (result.data[lastPos] & 0x80000000) != (bi1.data[lastPos] & 0x80000000))
    {
		throw (new std::overflow_error(""));
    }
    return result;
}
const BigInteger BigInteger::operator *(const BigInteger& T)const
{
        __int32 lastPos = maxLength-1;
        bool bi1Neg = false, bi2Neg = false;
		BigInteger bi1 = BigInteger( *this );
		BigInteger bi2 = BigInteger( T );
        try
        {
            if((bi1.data[lastPos] & 0x80000000) != 0)
            {
                    bi1Neg = true; bi1 = -bi1;
            }
            if((bi2.data[lastPos] & 0x80000000) != 0)
            {
                    bi2Neg = true; bi2 = -bi2;
            }
        }
		catch(std::overflow_error) {}
        BigInteger result = BigInteger();
        try
        {
            for(int i = 0; i < bi1.dataLength; i++)
            {
                if(bi1.data[i] == 0)    
					continue;
                unsigned __int64 mcarry = 0;
                for(int j = 0, k = i; j < bi2.dataLength; j++, k++)
                {
                    unsigned __int64 val = ((unsigned __int64)bi1.data[i] * (unsigned __int64)bi2.data[j]) + (unsigned __int64)result.data[k] + mcarry;
                    result.data[k] = (unsigned __int32)(val & 0xFFFFFFFF);
					mcarry = (val >> 32);
                }
                if(mcarry != 0)
                    result.data[i+bi2.dataLength] = (unsigned __int32)mcarry;
            }
        }
		catch(std::overflow_error)
        {
            throw(new std::overflow_error("Multiplication overflow."));
        }
        result.dataLength = bi1.dataLength + bi2.dataLength;
        if(result.dataLength > maxLength)
            result.dataLength = maxLength;
        while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
            result.dataLength--;
        if((result.data[lastPos] & 0x80000000) != 0)
        {
            if(bi1Neg != bi2Neg && result.data[lastPos] == 0x80000000)
            {
                if(result.dataLength == 1)
                    return result;
                else
                {
                    bool isMaxNeg = true;
                    for(int i = 0; i < result.dataLength - 1 && isMaxNeg; i++)
                    {
                        if(result.data[i] != 0)
                            isMaxNeg = false;
                    }
                    if(isMaxNeg)
                        return result;
                }
            }
			throw(new std::overflow_error("Multiplication overflow."));
        }
        if(bi1Neg != bi2Neg)
            return -result;
        return result;
}
const BigInteger BigInteger::operator /(const BigInteger& T)const
{
    BigInteger quotient = BigInteger();
    BigInteger remainder = BigInteger();
	BigInteger bi1 = BigInteger( *this );
	BigInteger bi2 = BigInteger( T );
    __int32 lastPos = maxLength-1;
    bool divisorNeg = false , dividendNeg = false;
    if((bi1.data[lastPos] & 0x80000000) != 0)
    {
        bi1 = -bi1;
        dividendNeg = true;
    }
    if((bi2.data[lastPos] & 0x80000000) != 0)
    {
        bi2 = -bi2;
        divisorNeg = true;
    }
    if(bi1 < bi2)
    {
        return quotient;
    }
    else
    {
        if(bi2.dataLength == 1)
            singleByteDivide(bi1, bi2, quotient, remainder);
        else
            multiByteDivide(bi1, bi2, quotient, remainder);
        if(dividendNeg != divisorNeg)
            return -quotient;
        return quotient;
    }
}
const BigInteger BigInteger::operator %(const BigInteger& T)const
{
	BigInteger bi1 = BigInteger(*this);
	BigInteger bi2 = BigInteger(T);
    BigInteger quotient = BigInteger();
    BigInteger remainder = BigInteger(*this);
    __int32 lastPos = maxLength-1;
    bool dividendNeg = false;
    if((bi1.data[lastPos] & 0x80000000) != 0)
    {
        bi1 = -bi1;
        dividendNeg = true;
    }
    if((bi2.data[lastPos] & 0x80000000) != 0)
        bi2 = -bi2;
    if(bi1 < bi2)
    {
        return remainder;
    }
    else
    {
        if(bi2.dataLength == 1)
            singleByteDivide(bi1, bi2, quotient, remainder);
        else
            multiByteDivide(bi1, bi2, quotient, remainder);
        if(dividendNeg)
            return -remainder;
		return remainder;
    }
}
const BigInteger& BigInteger::operator +=(const BigInteger& T){
	(*this) = (*this) + T;
	return *this;
}
const BigInteger& BigInteger::operator -=(const BigInteger& T){
	(*this) = (*this) - T;
	return *this;
}
const BigInteger& BigInteger::operator *=(const BigInteger& T){
	(*this) = (*this) * T;
	return *this;
}
const BigInteger& BigInteger::operator /=(const BigInteger& T){
	(*this) = (*this) / T;
	return *this;
}
const BigInteger& BigInteger::operator %=(const BigInteger& T)
{
	(*this) = (*this) % T;
	return *this;
}
const bool BigInteger::operator >(const BigInteger& T)const
{
	BigInteger bi1 = BigInteger( *this );
	BigInteger bi2 = BigInteger( T );
    int pos = maxLength - 1;
    if((bi1.data[pos] & 0x80000000) != 0 && (bi2.data[pos] & 0x80000000) == 0)
        return false;
    else if((bi1.data[pos] & 0x80000000) == 0 && (bi2.data[pos] & 0x80000000) != 0)
        return true;
    __int32 len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
	for(pos = len - 1; pos >= 0 && bi1.data[pos] == bi2.data[pos]; pos--);
	if(pos >= 0)
	{
		if(bi1.data[pos] > bi2.data[pos])
			return true;
		return false;
	}
	return false;
}
const bool BigInteger::operator <(const BigInteger& T)const
{
	BigInteger bi1 = BigInteger( *this );
	BigInteger bi2 = BigInteger( T );
    __int32 pos = maxLength - 1;
    if((bi1.data[pos] & 0x80000000) != 0 && (bi2.data[pos] & 0x80000000) == 0)
        return true;
    else if((bi1.data[pos] & 0x80000000) == 0 && (bi2.data[pos] & 0x80000000) != 0)
        return false;
    __int32 len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
	for(pos = len - 1; pos >= 0 && bi1.data[pos] == bi2.data[pos]; pos--);
	if(pos >= 0)
	{
		if(bi1.data[pos] < bi2.data[pos])
			return true;
		return false;
	}
	return false;
}
const bool BigInteger::operator ==(const BigInteger& T)const
{
    return this->Equals(T);
}
const bool BigInteger::operator !=(const BigInteger& T)const
{
    return !(this->Equals(T));
}
const bool BigInteger::operator >=(const BigInteger& T)const
{
    return (*this == T || *this > T);
}
const bool BigInteger::operator <=(const BigInteger& T)const
{
    return (*this == T || *this < T);
}
const BigInteger BigInteger::operator &(const BigInteger& T)const
{
	BigInteger bi1 = BigInteger(*this);
	BigInteger bi2 = BigInteger(T);
    BigInteger result = BigInteger();
    __int32 len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
    for(__int32 i = 0; i < len; i++)
    {
        unsigned __int32 sum = (unsigned __int32)(bi1.data[i] & bi2.data[i]);
        result.data[i] = sum;
    }
    result.dataLength = maxLength;
    while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
        result.dataLength--;
    return result;
}
const BigInteger BigInteger::operator |(const BigInteger& T)const
{
	BigInteger bi1 = BigInteger(*this);
	BigInteger bi2 = BigInteger(T);
    BigInteger result = BigInteger();
    __int32 len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
    for(__int32 i = 0; i < len; i++)
    {
        unsigned __int32 sum = (unsigned __int32)(bi1.data[i] | bi2.data[i]);
        result.data[i] = sum;
    }
    result.dataLength = maxLength;
    while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
        result.dataLength--;
    return result;
}
const BigInteger BigInteger::operator ^(const BigInteger& T)const
{
	BigInteger bi1 = BigInteger(*this);
	BigInteger bi2 = BigInteger(T);
    BigInteger result = BigInteger();
    __int32 len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
    for(__int32 i = 0; i < len; i++)
    {
        unsigned __int32 sum = (unsigned __int32)(bi1.data[i] ^ bi2.data[i]);
        result.data[i] = sum;
    }
    result.dataLength = maxLength;
    while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
        result.dataLength--;
    return result;
}
const bool BigInteger::Equals(const BigInteger& T)const
{
    if(this->dataLength != T.dataLength)
        return false;
    for(__int32 i = 0; i < this->dataLength; i++)
    {
        if(this->data[i] != T.data[i])
            return false;
    }
    return true;
}
const BigInteger BigInteger::operator ~()const 
{
    BigInteger result = BigInteger(*this);
    for(__int32 i = 0; i < maxLength; i++)
        result.data[i] = (unsigned __int32)(~data[i]);
    result.dataLength = maxLength;
    while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
        result.dataLength--;
    return result;
}
const BigInteger BigInteger::operator -()const 
{
    if(dataLength == 1 && data[0] == 0)
        return BigInteger();
    BigInteger result = BigInteger(*this);
    for(__int32 i = 0; i < maxLength; i++)
        result.data[i] = (unsigned __int32)(~data[i]);
    __int64 val, carry = 1;
    int index = 0;
    while(carry != 0 && index < maxLength)
    {
        val = (__int64)(result.data[index]);
        val++;
        result.data[index] = (unsigned __int32)(val & 0xFFFFFFFF);
        carry = val >> 32;
        index++;
    }
    if((data[maxLength-1] & 0x80000000) == (result.data[maxLength-1] & 0x80000000))
		throw (new std::overflow_error("Overflow in negation.\n"));
    result.dataLength = maxLength;
    while(result.dataLength > 1 && result.data[result.dataLength-1] == 0)
        result.dataLength--;
    return result;
}
const BigInteger	BigInteger::operator ++(__int32)
{
	*this = (*this + BigInteger(1));
	return (*this - BigInteger(1));
}
const BigInteger&	BigInteger::operator ++()
{
	*this = *this + BigInteger(1);
	return *this;
}
const BigInteger	BigInteger::operator --(__int32)
{
	*this = (*this - BigInteger(1));
	return (*this + BigInteger(1));
}
const BigInteger&	BigInteger::operator --()
{
	*this = *this - BigInteger(1);
	return *this;
}
const BigInteger BigInteger::operator =(__int16 value)
{
	return BigInteger(value);
}
const BigInteger BigInteger::operator =(unsigned __int16 value)
{
	return BigInteger(value);
}
const BigInteger BigInteger::operator =(__int32 value)
{
	return BigInteger(value);
}
const BigInteger BigInteger::operator =(unsigned __int32 value)
{
	return BigInteger(value);
}
const BigInteger BigInteger::operator =(__int64 value)
{
	return BigInteger(value);
}
const BigInteger BigInteger::operator =(unsigned __int64 value)
{
	return BigInteger(value);
}
const std::string BigInteger::ToString(__int32 radix )const
{
    if(radix < 2 || radix > 36)
		throw (new std::overflow_error("Radix must be >= 2 and <= 36"));
    std::string charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string NumSet = "0123456789";
    std::string result = "";
    BigInteger a = *this;
    bool negative = false;
    if((a.data[maxLength-1] & 0x80000000) != 0)
    {
        negative = true;
        try
        {
			a = -a;
        }
		catch(std::overflow_error) {}
    }
    BigInteger quotient = BigInteger();
    BigInteger remainder = BigInteger();
    BigInteger biRadix = BigInteger(radix);
    if(a.dataLength == 1 && a.data[0] == 0)
        result = "0";
    else
    {
        while(a.dataLength > 1 || (a.dataLength == 1 && a.data[0] != 0))
        {
            singleByteDivide(a, biRadix, quotient, remainder);
            if(remainder.data[0] < 10)
				result = NumSet.c_str()[(__int32)remainder.data[0]] + result;
			else
				result = charSet.c_str()[(__int32)remainder.data[0] - 10] + result;
            a = quotient;
        }
        if(negative)
            result = "-" + result;
    }
    return result;
}
const BigInteger BigInteger::abs()
{
	if((this->data[maxLength - 1] & 0x80000000) != 0)
		return ( - (*this) );
	else
		return BigInteger(*this);
}
const BigInteger BigInteger::sqrt()
{
    unsigned __int32 numBits = (unsigned __int32)this->bitCount();
    if((numBits & 0x1) != 0)
        numBits = (numBits >> 1) + 1;
    else
        numBits = (numBits >> 1);
    unsigned __int32 bytePos = numBits >> 5;
    unsigned __int8 bitPos = (unsigned __int8)(numBits & 0x1F);
    unsigned __int32 mask;
    BigInteger result = BigInteger();
    if(bitPos == 0)
        mask = 0x80000000;
    else
    {
        mask = (unsigned __int32)1 << bitPos;
        bytePos++;
    }
    result.dataLength = (__int32)bytePos;
    for(__int32 i = (__int32)bytePos - 1; i >= 0; i--)
    {
        while(mask != 0)
        {
            result.data[i] ^= mask;
            if((result * result) > *this)
                result.data[i] ^= mask;
            mask >>= 1;
        }
        mask = 0x80000000;
    }
    return result;
}
const BigInteger BigInteger::gcd(const BigInteger& T)const
{
        BigInteger x;
        BigInteger y;
        if((data[maxLength-1] & 0x80000000) != 0)
            x = -(*this);
        else
            x = (*this);
        if((T.data[maxLength-1] & 0x80000000) != 0)
            y = -T;
        else
            y = T;
	BigInteger g = y;
	while(x.dataLength > 1 || (x.dataLength == 1 && x.data[0] != 0))
	{
		g = x;
		x = y % x;
		y = g;
    }
	return g;
}
const __int32 BigInteger::bitCount()
{
    while(dataLength > 1 && data[dataLength-1] == 0)
        dataLength--;
    unsigned __int32 value = data[dataLength - 1];
    unsigned __int32 mask = 0x80000000;
    __int32 bits = 32;
    while(bits > 0 && (value & mask) == 0)
    {
        bits--;
        mask >>= 1;
    }
    bits += ((dataLength - 1) << 5);
    return bits;
}
void BigInteger::multiByteDivide(BigInteger& bi1, BigInteger& bi2, BigInteger& outQuotient, BigInteger& outRemainder)const
{
	unsigned __int32* result = new unsigned __int32[maxLength];
	for(__int32 i = 0 ; i != maxLength ;i++)
		result[i] = 0;
	__int32 remainderLen = bi1.dataLength + 1;
	unsigned __int32* remainder = new unsigned __int32[remainderLen];
	for(__int32 i = 0 ; i != remainderLen ;i++)
		remainder[i] = 0;
	unsigned __int32 mask = 0x80000000;
	unsigned __int32 val = bi2.data[bi2.dataLength - 1];
	__int32 shift = 0, resultPos = 0;
	while(mask != 0 && (val & mask) == 0)
	{
		shift++; mask >>= 1;
	}
	for(__int32 i = 0; i < bi1.dataLength; i++)
		remainder[i] = bi1.data[i];
	shiftLeft(remainder,remainderLen ,shift);
	bi2 = bi2 << shift;__int32 j = remainderLen - bi2.dataLength;
	__int32 pos = remainderLen - 1;
	unsigned __int64 firstDivisorByte = bi2.data[bi2.dataLength-1];
	unsigned __int64 secondDivisorByte = bi2.data[bi2.dataLength-2];
	__int32 divisorLen = bi2.dataLength + 1;
	unsigned __int32* dividendPart = new unsigned __int32[divisorLen];
	for(__int32 i = 0 ; i != divisorLen ;i++)
		dividendPart[i] = 0;
	while(j > 0)
	{
		unsigned __int64 dividend = ((unsigned __int64)remainder[pos] << 32) + (unsigned __int64)remainder[pos-1];
		unsigned __int64 q_hat = dividend / firstDivisorByte;
		unsigned __int64 r_hat = dividend % firstDivisorByte;
		bool done = false;
		while(!done)
		{
			done = true;
			if(q_hat == 0x100000000 ||
				(q_hat * secondDivisorByte) > ((r_hat << 32) + remainder[pos-2]))
			{
				q_hat--;
				r_hat += firstDivisorByte;
				if(r_hat < 0x100000000)
						done = false;
			}
		}
		for(__int32 h = 0; h < divisorLen; h++)
			dividendPart[h] = remainder[pos-h];
		BigInteger kk = BigInteger(dividendPart , divisorLen);
		BigInteger ss = bi2 * BigInteger((__int64)q_hat);
		while(ss > kk)
		{
			q_hat--;
			ss -= bi2;
		}
		BigInteger yy = kk - ss;
		for(__int32 h = 0; h < divisorLen; h++)
				remainder[pos-h] = yy.data[bi2.dataLength-h];
		result[resultPos++] = (unsigned __int32)q_hat;
		pos--;
		j--;
	}
	outQuotient.dataLength = resultPos;
	__int32 y = 0;
	for(__int32 x = outQuotient.dataLength - 1; x >= 0; x--, y++)
		outQuotient.data[y] = result[x];
	for(; y < maxLength; y++)
		outQuotient.data[y] = 0;
	while(outQuotient.dataLength > 1 && outQuotient.data[outQuotient.dataLength-1] == 0)
		outQuotient.dataLength--;
	if(outQuotient.dataLength == 0)
		outQuotient.dataLength = 1;
	outRemainder.dataLength = shiftRight(remainder, remainderLen, shift);
	for(y = 0; y < outRemainder.dataLength; y++)
		outRemainder.data[y] = remainder[y];
	for(; y < maxLength; y++)
		outRemainder.data[y] = 0;
}
const BigInteger BigInteger::operator <<(const __int32 shiftVal)const
{
	BigInteger bi1 = *this;
    BigInteger result = BigInteger(bi1);
    result.dataLength = shiftLeft(result.data , result.maxLength , shiftVal);
    return result;
}
const BigInteger BigInteger::operator >>(const __int32 shiftVal)const
{
	BigInteger bi1 = BigInteger(*this);
    BigInteger result = BigInteger(*this);
	result.dataLength = shiftRight(result.data,  result.maxLength ,shiftVal);
    if((bi1.data[maxLength-1] & 0x80000000) != 0)
    {
        for(__int32 i = maxLength - 1; i >= result.dataLength; i--)
            result.data[i] = 0xFFFFFFFF;
        unsigned __int32 mask = 0x80000000;
        for(__int32 i = 0; i < 32; i++)
        {
            if((result.data[result.dataLength-1] & mask) != 0)
                break;
            result.data[result.dataLength-1] |= mask;
            mask >>= 1;
        }
        result.dataLength = maxLength;
    }
    return result;
}
const BigInteger&	BigInteger::operator >>=(const __int32 shiftVal)
{
	(*this) = (*this)>>shiftVal;
	return *this;
}
const BigInteger&	BigInteger::operator <<=(const __int32 shiftVal)
{
	(*this) = (*this)<<shiftVal;
	return *this;
}
void BigInteger::singleByteDivide(BigInteger& bi1, BigInteger& bi2, BigInteger& outQuotient, BigInteger& outRemainder)const
{
    unsigned __int32* result = new unsigned __int32[maxLength];
	for(__int32 i = 0 ; i != maxLength ;i++)
		result[i] = 0;
    __int32 resultPos = 0;
    for(int i = 0; i < maxLength; i++)
        outRemainder.data[i] = bi1.data[i];
    outRemainder.dataLength = bi1.dataLength;
    while(outRemainder.dataLength > 1 && outRemainder.data[outRemainder.dataLength-1] == 0)
        outRemainder.dataLength--;
    unsigned __int64 divisor = (unsigned __int64)bi2.data[0];
    __int32 pos = outRemainder.dataLength - 1;
    unsigned __int64 dividend = (unsigned __int64)outRemainder.data[pos];
    if(dividend >= divisor)
    {
        unsigned __int64 quotient = dividend / divisor;
        result[resultPos++] = (unsigned __int32)quotient;
        outRemainder.data[pos] = (unsigned __int32)(dividend % divisor);
    }
    pos--;
    while(pos >= 0)
    {
        dividend = ((unsigned __int64)outRemainder.data[pos+1] << 32) + (unsigned __int64)outRemainder.data[pos];
        unsigned __int64 quotient = dividend / divisor;
        result[resultPos++] = (unsigned __int32)quotient;
        outRemainder.data[pos+1] = 0;
        outRemainder.data[pos--] = (unsigned __int32)(dividend % divisor);
    }
    outQuotient.dataLength = resultPos;
    __int32 j = 0;
    for(__int32 i = outQuotient.dataLength - 1; i >= 0; i--, j++)
        outQuotient.data[j] = result[i];
    for(; j < maxLength; j++)
        outQuotient.data[j] = 0;
    while(outQuotient.dataLength > 1 && outQuotient.data[outQuotient.dataLength-1] == 0)
        outQuotient.dataLength--;
    if(outQuotient.dataLength == 0)
        outQuotient.dataLength = 1;
    while(outRemainder.dataLength > 1 && outRemainder.data[outRemainder.dataLength-1] == 0)
        outRemainder.dataLength--;
}
const __int32 BigInteger::shiftLeft(unsigned __int32* buffer, __int32 bufferLength, __int32 shiftVal)const
{
    __int32 shiftAmount = 32;
    __int32 bufLen = bufferLength;
    while(bufLen > 1 && buffer[bufLen-1] == 0)
        bufLen--;
    for(__int32 count = shiftVal; count > 0;)
    {
        if(count < shiftAmount)
            shiftAmount = count;
        unsigned __int64 carry = 0;
        for(int i = 0; i < bufLen; i++)
        {
            unsigned __int64 val = ((unsigned __int64)buffer[i]) << shiftAmount;
            val |= carry;
            buffer[i] = (unsigned __int32)(val & 0xFFFFFFFF);
            carry = val >> 32;
        }
        if(carry != 0)
        {
            if(bufLen + 1 <= bufferLength)
            {
                buffer[bufLen] = (unsigned __int32)carry;
                bufLen++;
            }
        }
        count -= shiftAmount;
    }
    return bufLen;
}
const __int32 BigInteger::shiftRight(unsigned __int32* buffer ,__int32 bufferLength, __int32 shiftVal)const
{
    int shiftAmount = 32;
    int invShift = 0;
    int bufLen = bufferLength;
    while(bufLen > 1 && buffer[bufLen-1] == 0)
        bufLen--;
    for(int count = shiftVal; count > 0;)
    {
        if(count < shiftAmount)
        {
            shiftAmount = count;
            invShift = 32 - shiftAmount;
        }
        unsigned __int64 carry = 0;
        for(int i = bufLen - 1; i >= 0; i--)
        {
            unsigned __int64 val = ((unsigned __int64)buffer[i]) >> shiftAmount;
            val |= carry;
            carry = ((unsigned __int64)buffer[i]) << invShift;
            buffer[i] = (unsigned __int32)(val);
        }
        count -= shiftAmount;
    }
    while(bufLen > 1 && buffer[bufLen-1] == 0)
        bufLen--;
    return bufLen;
}
BigInteger::~BigInteger()
{
}
