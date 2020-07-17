#include "Temp.h"

//int ��float double ��ת�ײ�ʵ��--���о�

//���double /float ǿתΪint���͵��ٶ�   static_cast<int>(f) 
 // double��������ת����ǿ���㷨������� (������IEEE��������ʽ) ,��ʹ��C���� int a = (int)(������)��ܶ�
 static inline int DoubleToInt_IEEE(double value)
{ 
    static const double magic = 6755399441055744.0;  // (1<<51) | (1<<52)
    double tmp = value + magic;
    return *(int*)&tmp;
}

//float�������� �������������ʹ��
static inline int Fast_FloatToInt_IEEE(float value) 
{ 
    static const float magic_f = (3<<21);
    static const long magic_i = 0x4ac00000;
    float ftmp = value + magic_f;
    return  (*((int*)&ftmp)-magic_i) >> 1; 
}





