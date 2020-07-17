#ifndef BigInt_H_
#define BigInt_H_

//先引用第三方库，改进待续

//#include "int512.hpp"

//#include "BigInteger.hpp"




//综合参考,实现备忘
//https://blog.csdn.net/code4101/article/details/38705155
//https://blog.csdn.net/harryguo2012/article/details/42036829
//https://blog.csdn.net/zypang1/article/details/77651449
//https://blog.csdn.net/guofubin1/article/details/78611108


//BigFloat?

//class BigInt
//{
//    std::vector<int> s;
//    bool sign;
//}

#if 0

template <size_t Nbyte=16>
//class BigInt
class BigInt
{
    const static size_t NBit=Nbyte*8;
    int num[Nbyte];   //int char

    string toStr() const;//转化为字符串，主要是便于输出
    friend istream& operator>>(istream &,BigInt &);//重载输入流
    friend ostream& operator<<(ostream &,BigInt &);//重载输出流
    //重载复制
    BigInt operator=(const char*);
    BigInt operator=(int);
    BigInt operator=(const string);
    //重载各种比较
    bool operator>(const BigInt &) const;
    bool operator>=(const BigInt &) const;
    bool operator<(const BigInt &) const;
    bool operator<=(const BigInt &) const;
    bool operator==(const BigInt &) const;
    bool operator!=(const BigInt &) const;
    //重载四则运算
    BigInt operator+(const BigInt &) const;
    BigInt operator++();
    BigInt operator++(int);
    BigInt operator+=(const BigInt&);
    BigInt operator-(const BigInt &) const;
    BigInt operator--();
    BigInt operator--(int);
    BigInt operator-=(const BigInt&);
    BigInt operator*(const BigInt &)const;
    BigInt operator*(const int num)const;
    BigInt operator*=(const BigInt&);
    BigInt operator/(const BigInt&)const;
    BigInt operator/=(const BigInt&);


    //四则运算的衍生运算
    BigInt operator%(const BigInt&)const;//取模（余数）
    BigInt factorial()const;//阶乘
    BigInt Sqrt()const;//整数开根（向下取整）
    BigInt pow(const BigInt&)const;//次方
    //一些乱乱的函数
    void clean();
    ~BigInt();

    BigInt& operator += (const BigInt& b) {*this = *this + b; return *this;}
    BigInt& operator -= (const BigInt& b) {*this = *this - b; return *this;}
    BigInt& operator *= (const BigInt& b) {*this = *this * b; return *this;}
    BigInt& operator /= (const BigInt& b) {*this = *this / b; return *this;}
    BigInt& operator %= (const BigInt& b) {*this = *this % b; return *this;}

    bool operator < (const BigInt& b) const {
        if (s.size() != b.s.size()) return s.size() < b.s.size();
        for (int i = s.size()-1; i >= 0; i--)
            if (s[i] != b.s[i]) return s[i] < b.s[i];
        return false;
    }
    bool operator >(const BigInt& b) const{return b < *this;}
    bool operator<=(const BigInt& b) const{return !(b < *this);}
    bool operator>=(const BigInt& b) const{return !(*this < b);}
    bool operator!=(const BigInt& b) const{return b < *this || *this < b;}
};

#endif



#ifdef UNIT_TEST
int Test_BigInt();
#endif

#endif

