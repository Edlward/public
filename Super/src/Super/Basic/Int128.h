#ifndef __INT128_H_
#define __INT128_H_

//Int128 Uint128待续
//https://www.cnblogs.com/tyche116/p/11690769.html
//完善与测试待续

#include <stdint.h>
#include <string>


class uint128
{
public:
    uint128() :hi(0), lo(0){}
    uint128(uint32_t l) :hi(0), lo(l){}
    uint128(int32_t l) :hi(-(l < 0)), lo(l){}
    uint128(int64_t l) :hi(-(l < 0)), lo(l){}
    uint128(uint64_t l) :hi(0), lo(l){}
    uint128(const std::string& s);
    uint128(uint64_t _h, uint64_t _l)
        :hi(_h), lo(_l){}

    bool     operator == (const uint128& o)const{ return hi == o.hi && lo == o.lo; }
    bool     operator != (const uint128& o)const{ return hi != o.hi || lo != o.lo; }
    bool     operator < (const uint128& o)const { return (hi == o.hi) ? lo < o.lo : hi < o.hi; }
    bool     operator < (const int64_t& o)const { return *this < uint128(o); }
    bool     operator !()const                    { return !(hi != 0 || lo != 0); }
    uint128  operator -()const                    { return ++uint128(~hi, ~lo); }
    uint128  operator ~()const                    { return uint128(~hi, ~lo); }

    uint128& operator++()    { hi += (++lo == 0); return *this; }
    uint128& operator--()    { hi -= (lo-- == 0); return *this; }
    uint128  operator++(int) { auto tmp = *this; ++(*this); return tmp; }
    uint128  operator--(int) { auto tmp = *this; --(*this); return tmp; }

    uint128& operator |= (const uint128& u) { hi |= u.hi; lo |= u.lo; return *this; }
    uint128& operator &= (const uint128& u) { hi &= u.hi; lo &= u.lo; return *this; }
    uint128& operator ^= (const uint128& u) { hi ^= u.hi; lo ^= u.lo; return *this; }
    uint128& operator <<= (const uint128& u);
    uint128& operator >>= (const uint128& u);

    uint128& operator += (const uint128& u) { const uint64_t old = lo; lo += u.lo;  hi += u.hi + (lo < old); return *this; }
    uint128& operator -= (const uint128& u) { return *this += -u; }
    uint128& operator *= (const uint128& u);


    friend uint128 operator + (const uint128& l, const uint128& r)   { return uint128(l) += r; }
    friend uint128 operator + (const uint128& l, const uint64_t& r)   { return uint128(l) += uint128(r); }
    friend uint128 operator + (const uint128& l, const uint32_t& r)   { return uint128(l) += uint128(r); }
    friend uint128 operator + (const uint128& l, const int32_t& r)   { return uint128(l) += uint128(r); }
    friend uint128 operator + (const uint64_t& l, const uint128& r)   { return uint128(l) += r; }
    friend uint128 operator - (const uint128& l, const uint128& r)   { return uint128(l) -= r; }
    friend uint128 operator * (const uint128& l, const uint128& r)   { return uint128(l) *= r; }
    friend uint128 operator * (const uint128& l, const uint64_t& r)   { return uint128(l) *= uint128(r); }
    friend uint128 operator * (const uint128& l, const uint32_t& r)   { return uint128(l) *= uint128(r); }
    friend uint128 operator | (const uint128& l, const uint128& r)   { return uint128(l) = (r); }
    friend uint128 operator & (const uint128& l, const uint128& r)   { return uint128(l) &= r; }
    friend uint128 operator & (const uint128& l, const uint64_t& r)   { return uint128(l) &= uint128(r); }
    friend uint128 operator ^ (const uint128& l, const uint128& r)   { return uint128(l) ^= r; }
    friend uint128 operator << (const uint128& l, const uint128& r)  { return uint128(l) <<= r; }
    friend uint128 operator >> (const uint128& l, const uint128& r)  { return uint128(l) >>= r; }
    friend uint128 operator >> (const uint128& l, const int32_t& r)  { return uint128(l) >>= uint128(r); }
    friend bool    operator >  (const uint128& l, const uint128& r)  { return r < l; }
    friend bool    operator >(const uint128& l, const int64_t& r)  { return uint128(r) < l; }
    friend bool    operator >  (const int64_t& l, const uint128& r)  { return r < uint128(l); }

    friend bool    operator >=  (const uint128& l, const uint128& r) { return l == r || l > r; }
    friend bool    operator >=  (const uint128& l, const int64_t& r) { return l >= uint128(r); }
    friend bool    operator >=  (const int64_t& l, const uint128& r) { return uint128(l) >= r; }
    friend bool    operator <=  (const uint128& l, const uint128& r) { return l == r || l < r; }
    friend bool    operator <=  (const uint128& l, const int64_t& r) { return l <= uint128(r); }
    friend bool    operator <=  (const int64_t& l, const uint128& r) { return uint128(l) <= r; }

    operator uint64_t() { return lo; }               //强制转换为uint64_t
    operator uint32_t() { return (uint32_t)lo; }     //强制转换为uint32_t
    operator int32_t() { return (int32_t)lo; }       //强制转换为int32_t


    uint32_t low_32_bits()const { return (uint32_t)lo; }
    uint64_t low_bits()const  { return lo; }
    uint64_t high_bits()const { return hi; }


    uint32_t to_integer()const { return (uint32_t)lo;}   //找不到定义自己添加的 by lizulin

    uint64_t hi;
    uint64_t lo;
};


inline uint128::uint128(const std::string &sz)
    :hi(0), lo(0)
{
    // do we have at least one character?
    if (!sz.empty()) {
        // make some reasonable assumptions
        int radix = 10;
        bool minus = false;

        std::string::const_iterator i = sz.begin();

        // check for minus sign, i suppose technically this should only apply
        // to base 10, but who says that -0x1 should be invalid?
        if (*i == '-') {
            ++i;
            minus = true;
        }

        // check if there is radix changing prefix (0 or 0x)
        if (i != sz.end()) {
            if (*i == '0') {
                radix = 8;
                ++i;
                if (i != sz.end()) {
                    if (*i == 'x') {
                        radix = 16;
                        ++i;
                    }
                }
            }

            while (i != sz.end()) {
                unsigned int n = 0;
                const char ch = *i;

                if (ch >= 'A' && ch <= 'Z') {
                    if (((ch - 'A') + 10) < radix) {
                        n = (ch - 'A') + 10;
                    }
                    else {
                        break;
                    }
                }
                else if (ch >= 'a' && ch <= 'z') {
                    if (((ch - 'a') + 10) < radix) {
                        n = (ch - 'a') + 10;
                    }
                    else {
                        break;
                    }
                }
                else if (ch >= '0' && ch <= '9') {
                    if ((ch - '0') < radix) {
                        n = (ch - '0');
                    }
                    else {
                        break;
                    }
                }
                else {
                    /* completely invalid character */
                    break;
                }

                (*this) *= radix;
                (*this) += n;

                ++i;
            }
        }

        if (minus) {
            *this = -*this;
        }
    }
}

inline uint128& uint128::operator<<=(const uint128& rhs)
{
    if (rhs >= 128)
    {
        hi = 0;
        lo = 0;
    }
    else
    {
        unsigned int n = rhs.to_integer();
        const unsigned int halfsize = 128 / 2;

        if (n >= halfsize){
            n -= halfsize;
            hi = lo;
            lo = 0;
        }

        if (n != 0) {
            // shift high half
            hi <<= n;

            const uint64_t mask(~(uint64_t(-1) >> n));

            // and add them to high half
            hi |= (lo & mask) >> (halfsize - n);

            // and finally shift also low half
            lo <<= n;
        }
    }

    return *this;
}

inline uint128 & uint128::operator>>=(const uint128& rhs)
{
    if (rhs >= 128)
    {
        hi = 0;
        lo = 0;
    }
    else
    {
        unsigned int n = rhs.to_integer();
        const unsigned int halfsize = 128 / 2;

        if (n >= halfsize) {
            n -= halfsize;
            lo = hi;
            hi = 0;
        }

        if (n != 0) {
            // shift low half
            lo >>= n;

            // get lower N bits of high half
            const uint64_t mask(~(uint64_t(-1) << n));

            // and add them to low qword
            lo |= (hi & mask) << (halfsize - n);

            // and finally shift also high half
            hi >>= n;
        }
    }
    return *this;
}

inline uint128& uint128::operator*=(const uint128 &b)
{
    uint64_t a0 = (uint32_t)(this->lo);
    uint64_t a1 = (uint32_t)(this->lo >> 0x20);
    uint64_t a2 = (uint32_t)(this->hi);
    uint64_t a3 = (uint32_t)(this->hi >> 0x20);

    uint64_t b0 = (uint32_t)(b.lo);
    uint64_t b1 = (uint32_t)(b.lo >> 0x20);
    uint64_t b2 = (uint32_t)(b.hi);
    uint64_t b3 = (uint32_t)(b.hi >> 0x20);


    this->hi = 0;
    this->lo = a3*b0;
    (*this) += a2*b1;
    (*this) += a1*b2;
    (*this) += a0*b3;
    (*this) <<= 0x20;
    (*this) += a2*b0;
    (*this) += a1*b1;
    (*this) += a0*b2;
    (*this) <<= 0x20;
    (*this) += a1*b0;
    (*this) += a0*b1;
    (*this) <<= 0x20;
    (*this) += a0*b0;

    return *this;
}


#endif
