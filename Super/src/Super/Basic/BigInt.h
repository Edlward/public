#ifndef BigInt_H_
#define BigInt_H_

//�����õ������⣬�Ľ�����

//#include "int512.hpp"

//#include "BigInteger.hpp"




//�ۺϲο�,ʵ�ֱ���
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

    string toStr() const;//ת��Ϊ�ַ�������Ҫ�Ǳ������
    friend istream& operator>>(istream &,BigInt &);//����������
    friend ostream& operator<<(ostream &,BigInt &);//���������
    //���ظ���
    BigInt operator=(const char*);
    BigInt operator=(int);
    BigInt operator=(const string);
    //���ظ��ֱȽ�
    bool operator>(const BigInt &) const;
    bool operator>=(const BigInt &) const;
    bool operator<(const BigInt &) const;
    bool operator<=(const BigInt &) const;
    bool operator==(const BigInt &) const;
    bool operator!=(const BigInt &) const;
    //������������
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


    //�����������������
    BigInt operator%(const BigInt&)const;//ȡģ��������
    BigInt factorial()const;//�׳�
    BigInt Sqrt()const;//��������������ȡ����
    BigInt pow(const BigInt&)const;//�η�
    //һЩ���ҵĺ���
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

