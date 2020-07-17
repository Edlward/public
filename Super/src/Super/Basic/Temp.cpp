#include "Temp.h"

//int 与float double 互转底层实现--待研究

//提高double /float 强转为int类型的速度   static_cast<int>(f) 
 // double到整数的转换超强的算法，无误差 (利用了IEEE浮点编码格式) ,比使用C语言 int a = (int)(浮点数)快很多
 static inline int DoubleToInt_IEEE(double value)
{ 
    static const double magic = 6755399441055744.0;  // (1<<51) | (1<<52)
    double tmp = value + magic;
    return *(int*)&tmp;
}

//float到整数， 允许存在误差情况使用
static inline int Fast_FloatToInt_IEEE(float value) 
{ 
    static const float magic_f = (3<<21);
    static const long magic_i = 0x4ac00000;
    float ftmp = value + magic_f;
    return  (*((int*)&ftmp)-magic_i) >> 1; 
}





