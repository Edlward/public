#ifndef _A_NewClassTemplate_Header_
#define _A_NewClassTemplate_Header_

#include "global.h" 

NAME_SPACE_MAIN_BEGIN

    //MyBigFloat.h

#include <deque>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>


//    ����C++STL���������߾��ȸ�������������
//    ��������ǽ���������ߵ�BigInteger���㷽����Ȼ���Լ����иĶ���ʹ���ܹ����㸡������+-*/�������㣬����֧��int����Ϊ��������һ���������ĳ˷����ӿڶ������ص��������ͨ������.h�ļ�������BigFloat���󣬾Ϳ��Խ��м��㣬����ʹ�õ���vector���д洢�ͼ��㣬���Լ���ľ��Ⱥܸߡ�
//    �Ƕ��帡���������ļ��㾫�ȣ������˵���������ͨ�����������ֵ�����Խ�һ�����Ӽ��㾫�ȡ�
//    �����Ѿ�ͨ�������Ĳ��ԣ����г����������벻�ߴͽ̣�ת����ע��������
//�ο���http://blog.csdn.net/touzani/article/details/1749256 
//
//�������壬�����ﶨ����BigFloat���Լ��쳣�࣬�ڴ˻����ϣ������˼�����Ҫ�ļ����������������ʹ������������ͨ����������ļ���ɴ������㡣



//using namespace std;
#define ACCURACY 100


class DividedByZeroException{
public:
    void error(){std::cout<<"Divided By Zero Exception!"<<std::endl;}
};


class BigFloat{
private:
    std::vector<char> integer;
    std::vector<char> decimal;
    void trim();
    bool tag;
public:

    BigFloat(int);    // construct with a int integer
    BigFloat(std::string&) ;
    BigFloat();
    BigFloat(const BigFloat&);
    BigFloat operator=(const BigFloat& op);

    BigFloat      abs() const;
    BigFloat      pow(int a);

    //binary operators

    friend BigFloat operator+(const BigFloat&,const BigFloat&);
    friend BigFloat operator-(const BigFloat&,const BigFloat&);
    friend BigFloat operator*(const BigFloat&,const BigFloat&);
    friend BigFloat operator/(const BigFloat&,const BigFloat&) throw(DividedByZeroException); 

    friend BigFloat operator-(const BigFloat&);   //negative

    friend bool operator==(const BigFloat&,const BigFloat&);
    friend bool operator==(const BigFloat&,const BigFloat&);
    friend bool operator<(const BigFloat&,const BigFloat&);
    friend bool operator<=(const BigFloat&,const BigFloat&);
    friend bool operator>(const BigFloat&,const BigFloat&);
    friend bool operator>=(const BigFloat&,const BigFloat&);

    friend BigFloat operator+=(BigFloat&,const BigFloat&);
    friend BigFloat operator-=(BigFloat&,const BigFloat&);
    friend BigFloat operator*=(BigFloat&,const BigFloat&);
    friend BigFloat operator/=(BigFloat&,const BigFloat&) throw(DividedByZeroException);

    friend std::ostream& operator<<(std::ostream&,const BigFloat&);    //print the BigInteger
    friend std::istream& operator>>(std::istream&, BigFloat&);         // input the BigInteger

public:
    static const BigFloat ZERO;
    static const BigFloat ONE;
    static const BigFloat TEN;
};
inline BigFloat operator-(const BigFloat& op)
{
    BigFloat temp(op);
    temp.tag = !temp.tag;
    return temp;
}
inline void BigFloat::trim() {
    std::vector<char>::reverse_iterator iter = integer.rbegin();
    while(!integer.empty() && (*iter) == 0){
        integer.pop_back();
        iter=integer.rbegin();
    }
    if( integer.size()==0 ) {
        tag = true;
        integer.push_back(0);
    }
    std::vector<char>::const_iterator it = decimal.begin();
    while(!decimal.empty() && (*it) == 0){
        it = decimal.erase(it);
    }
    if( decimal.size()==0 ) {
        tag = true;
        decimal.push_back(0);
    }
}
//delete those 0 in the front
const BigFloat BigFloat::ZERO=BigFloat(0);
const BigFloat BigFloat::ONE =BigFloat(1);
const BigFloat BigFloat::TEN =BigFloat(10);

inline BigFloat::BigFloat(int val){
    if (val >= 0)
        tag = true;
    else{
        tag = false;
        val *= (-1);
    }
    do{
        integer.push_back( (char)(val%10) );
        val /= 10;
    } while ( val != 0 );
    decimal.push_back(0);
}

inline BigFloat::BigFloat( ){
    tag = true;
    integer.push_back(0);
    decimal.push_back(0);
}


inline BigFloat::BigFloat(std::string& def){
    bool type = true;
    for(std::string::reverse_iterator iter = def.rbegin() ; iter < def.rend();  iter++)
    {
        char ch = (*iter);
        if(ch == '.'){
            type = false;
            iter++;
        }
        if (iter == def.rend()-1){
            if ( ch == '+' )
                break;
            if(ch == '-' ){
                tag = false;
                break;
            }
        }
        //�ڴ˴�ʹ��string����������������������ݴӺ���ǰ���д���
        if(type)decimal.push_back( (char)((*iter) - '0' ) );
        else integer.push_back( (char)((*iter) - '0' ) );
    }
}

inline BigFloat::BigFloat(const BigFloat &op)
{
    integer = op.integer;
    decimal = op.decimal;
    tag = op.tag;
}

inline BigFloat BigFloat::operator=(const BigFloat& op)
{
    integer = op.integer;
    decimal = op.decimal;
    tag = op.tag;
    return (*this);
}

inline BigFloat BigFloat::abs() const  {
    if(tag)  return (*this);
    else      return -(*this);
}
//
inline BigFloat BigFloat::pow(int a) 
{
    BigFloat res(1);
    for(int i=0; i<a; i++)
        res*=(*this);
    return res;
}

inline std::ostream& operator<<(std::ostream& stream,const BigFloat& val){    //print the BigInteger
    if (!val.tag)
        stream << "-";    
    for ( std::vector<char>::const_reverse_iterator iter = val.integer.rbegin(); iter != val.integer.rend() ; iter++)
        stream << (char)((*iter) + '0');  
    std::cout<<'.';
    for ( std::vector<char>::const_reverse_iterator iter = val.decimal.rbegin(); iter != val.decimal.rend() ; iter++)
        stream << (char)((*iter) + '0');
    return stream;
}

inline std::istream& operator>>(std::istream& stream, BigFloat& val){    //Input the BigInteger
    std::string str;
    stream >> str;
    val=BigFloat(str);
    return stream;
}

inline BigFloat operator+=(BigFloat& op1,const BigFloat& op2){
    if( op1.tag == op2.tag ) {     //ֻ������ͬ�ķ��ŵ��������ŵ������-����
        std::vector<char>::iterator iter1;
        std::vector<char>::const_iterator iter2,it;
        //����С������
        int a = op1.decimal.size();
        int b = op2.decimal.size();
        char to_add = 0;     //��λ
        if(a<b){
            iter1 = op1.decimal.begin();
            iter2 = op2.decimal.begin();
            iter2 = iter2 - (a-b);

            while ( iter1 != op1.decimal.end() && iter2 != op2.decimal.end()){
                (*iter1) = (*iter1) + (*iter2) + to_add;
                to_add = ((*iter1) > 9);    // ����9��һλ
                (*iter1) = (*iter1) % 10;
                iter1++; iter2++;
            }
            it = op2.decimal.begin();
            iter2 = op2.decimal.end();
            iter2 = iter2 - a -1;
            while(iter2 != it){
                op1.decimal.insert(op1.decimal.begin(),*iter2);
                iter2--;
            }//�˴�Ӧ���÷��������
            op1.decimal.insert(op1.decimal.begin(),*iter2);
            iter1 = op1.decimal.begin();
        }
        else if(a>b){
            iter1 = op1.decimal.begin();
            iter1 = iter1+(a-b);
            iter2 = op2.decimal.begin();
            while ( iter1 != op1.decimal.end() && iter2 != op2.decimal.end()){
                (*iter1) = (*iter1) + (*iter2) + to_add;
                to_add = ((*iter1) > 9);    // ����9��һλ
                (*iter1) = (*iter1) % 10;
                iter1++; iter2++;
            }
        }
        else {
            iter1 = op1.decimal.begin();
            iter2 = op2.decimal.begin();
            while ( iter1 != op1.decimal.end() && iter2 != op2.decimal.end()){
                (*iter1) = (*iter1) + (*iter2) + to_add;
                to_add = ((*iter1) > 9);    // ����9��һλ
                (*iter1) = (*iter1) % 10;
                iter1++; iter2++;
            }
        }

        //������������
        iter1 = op1.integer.begin();
        iter2 = op2.integer.begin();
        //��λ
        while ( iter1 != op1.integer.end() && iter2 != op2.integer.end()){
            (*iter1) = (*iter1) + (*iter2) + to_add;
            to_add = ((*iter1) > 9);    // ����9��һλ
            (*iter1) = (*iter1) % 10;
            iter1++; iter2++;
        }
        while ( iter1 != op1.integer.end() ){   // 
            (*iter1) = (*iter1) + to_add;
            to_add = ( (*iter1) > 9 );
            (*iter1) %= 10;
            iter1++;
        }
        while ( iter2 != op2.integer.end() ){
            char val = (*iter2) + to_add;
            to_add = (val > 9) ;
            val %= 10;
            op1.integer.push_back(val);
            iter2++;
        }
        if( to_add != 0 )
            op1.integer.push_back(to_add);
        return op1;
    }
    else{
        if (op1.tag)
            return op1 -= (-op2);
        else
            return op1= op2 - (-op1);
    }
}
////
inline BigFloat operator-=(BigFloat& op1,const BigFloat& op2){
    if( op1.tag == op2.tag ) {     //ֻ������ͬ�ķ��ŵ������ͬ�ŵ������+����
        if(op1.tag)  { 
            if(op1 < op2)  // 2 - 3
            {
                BigFloat op(op2 - op1);
                op1 = -op;
                return op1;//
            }
        } 
        else {
            if(-op1 > -op2)  // (-3)-(-2) = -(3 - 2)
                return op1=-((-op1)-(-op2));
            else             // (-2)-(-3) = 3 - 2 
                return op1= (-op2) - (-op1);
        }
        //С������ 
        char to_substract = 0;  //��λ
        int a = op1.decimal.size();
        int b = op2.decimal.size();
        std::vector<char>::iterator it1 = op1.decimal.begin();
        std::vector<char>::const_iterator it2 = op2.decimal.begin();
        if(a>b){
            a -= b;
            it1 = it1 + a;
        }
        else{
            int number = b-a;
            while(number!=0)
            {
                op1.decimal.insert(op1.decimal.begin(),'0');//should be insert into the head of the vector
                number--;
            }
        }
        while ( it1 != op1.decimal.end() && it2 != op2.decimal.end()) {
            (*it1) = (*it1) - (*it2) - to_substract;
            to_substract = 0;
            if( (*it1) < 0 ) {
                to_substract=1;
                (*it1) += 10;
            }
            it1++;
            it2++;
        }
        //��������
        std::vector<char>::iterator iter1;
        std::vector<char>::const_iterator iter2;
        iter1 = op1.integer.begin();
        iter2 = op2.integer.begin();

        while ( iter1 != op1.integer.end() && iter2 != op2.integer.end()) {
            (*iter1) = (*iter1) - (*iter2) - to_substract;
            to_substract = 0;
            if( (*iter1) < 0 ) {
                to_substract=1;
                (*iter1) += 10;
            }
            iter1++;
            iter2++;
        }
        while ( iter1 != op1.integer.end() ) {
            (*iter1) = (*iter1) - to_substract;
            to_substract = 0;
            if( (*iter1) < 0 ) {
                to_substract=1;
                (*iter1) += 10;
            }
            else break;
            iter1++;
        }
        op1.trim();
        return op1;
    }//������ͬ
    else {
        if (op1 > BigFloat::ZERO)
            return op1 += (-op2);
        else
            return op1 = -(op2 + (-op1));
    }
}
inline BigFloat operator*=(BigFloat& op1,const BigFloat& op2) {
    BigFloat result(0);
    if (op1 == BigFloat::ZERO || op2==BigFloat::ZERO)
        result = BigFloat::ZERO;
    else{
        int size = 0;
        std::vector<char> op_temp1(op1.integer.begin(), op1.integer.end());
        if(op1.decimal.size()!=1||(op1.decimal.size()==1&&(*op1.decimal.begin())!=0)){
            op_temp1.insert(op_temp1.begin(),op1.decimal.begin(), op1.decimal.end());
            size += op1.decimal.size();
        }
        std::vector<char> op_temp2(op2.integer.begin(), op2.integer.end());
        if(op2.decimal.size()!=1||(op2.decimal.size()==1&&(*op2.decimal.begin())!=0)){
            op_temp2.insert(op_temp2.begin(),op2.decimal.begin(), op2.decimal.end());
            size += op2.decimal.size();
        }
        std::vector<char>::const_iterator iter2 = op_temp2.begin();
        while( iter2 != op_temp2.end() ){
            if(*iter2 != 0){
                std::deque<char> temp(op_temp1.begin() , op_temp1.end());
                char to_add = 0;
                std::deque<char>::iterator iter1 = temp.begin();
                while( iter1 != temp.end() ){
                    (*iter1) *= (*iter2);
                    (*iter1) += to_add;
                    to_add = (*iter1) / 10;
                    (*iter1) %= 10;
                    iter1++;
                }//while
                if( to_add != 0)
                    temp.push_back( to_add );
                int num_of_zeros = iter2 - op_temp2.begin();
                while(  num_of_zeros--)
                    temp.push_front(0);
                BigFloat temp2;
                temp2.integer.clear();
                temp2.integer.insert( temp2.integer.end() , temp.begin() , temp.end() );
                temp2.trim();
                result = result + temp2;
                //
            }//if
            iter2++;
        }//while
        result.tag = ( (op1.tag && op2.tag) || (!op1.tag && !op2.tag) );
        if(size!=0){
            result.decimal.clear();
            result.decimal.insert(result.decimal.begin(), result.integer.begin(), result.integer.begin()+size);
            result.integer.erase(result.integer.begin(), result.integer.begin()+size);
        }
    }//else
    op1 = result;
    return op1;
}
inline BigFloat operator/=(BigFloat& op1,const BigFloat& op2){
    if(op2 == BigFloat::ZERO)
        throw DividedByZeroException();
    if(op1 == BigFloat::ZERO)
        return op1;
    BigFloat op_temp2 = op2;
    BigFloat op_temp1 = op1;
    int Integer_Size = 0;
    if((op_temp2.decimal.size() == 1)&&(*(op_temp2.decimal.begin()) == 0)){    }
    else{
        //Integer_Size -= op_temp2.decimal.size();
        int t = op_temp2.decimal.size();
        while(t--){
            op_temp1 = op_temp1*BigFloat::TEN;
            op_temp2 = op_temp2*BigFloat::TEN;
        }
    }
    if(op_temp1<op_temp2){
        while(op_temp1>op_temp2){
            op_temp1 *= BigFloat::TEN;
            Integer_Size--;
        }
    }
    else{
        while(op_temp1>op_temp2){
            op_temp1.decimal.push_back(*op_temp1.integer.begin());
            op_temp1.integer.erase(op_temp1.integer.begin());
            Integer_Size++;
        }
    }
    int k = ACCURACY;
    BigFloat re(0); 
    while(k--){
        if(op_temp1<op_temp2){
            op_temp1 = op_temp1*BigFloat::TEN;
            re = re*BigFloat::TEN;
        }
        else{
            int i;
            BigFloat compare;
            for(i=2;i<10;i++){
                BigFloat BF(i);
                compare = op_temp2*BF;
                if(compare>op_temp1)
                    break;
            }//for
            compare -= op_temp2;
            op_temp1 -= compare;
            BigFloat index(i-1);
            re = re + index;
        }//else
    }//while    
    if(re.integer.size()>Integer_Size){
        std::vector<char> temp(re.integer.begin(), re.integer.end());
        re.integer.assign(temp.end() - Integer_Size, temp.end());
        re.decimal.insert(re.decimal.begin(), temp.begin(),temp.end() - Integer_Size);
    }
    else{
        int t = Integer_Size - re.integer.size();
        while(t--){
            re = re*BigFloat::TEN;
        }
    }

    op1 = re;
    op1.trim();
    op1.tag = ( (op1.tag && op2.tag) || (!op1.tag && !op2.tag) );
    return op1;
}
inline BigFloat operator+(const BigFloat& op1,const BigFloat& op2){
    BigFloat temp(op1);
    temp += op2;
    return temp;
}
inline BigFloat operator-(const BigFloat& op1,const BigFloat& op2){
    BigFloat temp(op1);
    temp -= op2;
    return temp;
}
inline BigFloat operator*(const BigFloat& op1,const BigFloat& op2){
    BigFloat temp(op1);
    temp *= op2;
    return temp;
}
inline BigFloat operator/(const BigFloat& op1,const BigFloat& op2){
    BigFloat temp(op1);
    temp /= op2;
    return temp;
}
inline bool operator<(const BigFloat& op1,const BigFloat& op2){
    bool sign;
    if( op1.tag != op2.tag ){
        sign =  !op1.tag;    
        return sign;
    }
    else{
        if(op1.integer.size() != op2.integer.size()){
            if(op1.tag){
                sign = op1.integer.size()<op2.integer.size();            
                return sign;
            }
            else {
                sign = op1.integer.size()>op2.integer.size();
                return sign;
            }
        } 
        std::vector<char>::const_reverse_iterator iter1,iter2;
        iter1 = op1.integer.rbegin();
        iter2 = op2.integer.rbegin();
        while( iter1 != op1.integer.rend() ){
            if(  op1.tag &&  *iter1 < *iter2 ) return true;
            if(  op1.tag &&  *iter1 > *iter2 ) return false;
            if( !op1.tag &&  *iter1 > *iter2 ) return true;
            if( !op1.tag &&  *iter1 < *iter2 ) return false;
            iter1++;
            iter2++;
        }
        std::vector<char>::const_reverse_iterator it1,it2;
        it1 = op1.decimal.rbegin();
        it2 = op2.decimal.rbegin();
        while( it1 != op1.decimal.rend() && it2 != op2.decimal.rend()){
            //cout<<((*it1)+'0')<<((*it1)+'0')<<endl;
            if(  op1.tag &&  *it1 < *it2 ) return true;
            if(  op1.tag &&  *it1 > *it2 ) return false;
            if( !op1.tag &&  *it1 > *it2 ) return true;
            if( !op1.tag &&  *it1 < *it2 ) return false;
            it1++;
            it2++;
        }
        return (op1.tag && it2!=op2.decimal.rend())
            || (!op1.tag && it1!=op1.decimal.rend());
    }
}
inline bool operator>(const BigFloat& op1,const BigFloat& op2){
    bool tag = !(op1<=op2);
    return tag;
}
inline bool operator==(const BigFloat& op1,const BigFloat& op2){
    if(op1.tag == (!op2.tag)){
        return false;
    }
    //����ʮ����֣���Ϊ������Ƕ���true��ʱ��t�õ��Ľ����false��Ȼ��if�жϵ�ʱ��ȷ����Ϊtrue
    if(op1.integer.size() != op2.integer.size() ){
        return false;
    }
    if( op1.decimal.size() != op2.decimal.size() ){
        return false;
    }
    std::vector<char>::const_iterator iter1,iter2;
    iter1 = op1.decimal.begin();
    iter2 = op2.decimal.begin();
    while( iter1!= op1.decimal.end() ){
        if( *iter1 != *iter2 )  return false;
        iter1++;
        iter2++;
    }
    iter1 = op1.integer.begin();
    iter2 = op2.integer.begin();
    while( iter1!= op1.integer.end() ){
        if( *iter1 != *iter2 )  return false;
        iter1++;
        iter2++;
    }
    return true;
}

inline bool operator!=(const BigFloat& op1,const BigFloat& op2)
{
    return !(op1==op2);
}
inline bool operator>=(const BigFloat& op1,const BigFloat& op2)
{
    bool tag = (op1>op2) || (op1==op2);   
    return tag;
}

inline bool operator<=(const BigFloat& op1,const BigFloat& op2)
{
    bool tag = (op1<op2) || (op1==op2);
    return tag;
}


NAME_SPACE_MAIN_END


#ifdef UNIT_TEST
BASIC_API int Test_A_NewClassTemplate();
#endif

#endif


