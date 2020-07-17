#ifndef PID_H_
#define  PID_H_

#include "global.h"

 NAME_SPACE_MAIN_BEGIN

//�Ա���������
typedef struct PID{
	double SetPoint;            //�趨ֵ
	double Kp;                  //����ϵ��
	double Ki;                  //����ϵ��
	double Kd;                  //΢��ϵ��
	double LastError;           //���һ�������Er[-1]
	double PrevError;           //���ڶ��������er[-2]
	double SumError;            //������  
}PID;


double PIDCalc(PID *pp, double NextPoint);
void PIDInit (PID *pp);
double sensor(void);
void actuator(double rDelta);

#ifdef UNIT_TEST
BASIC_API int Test_PID();
#endif


 NAME_SPACE_MAIN_END
#endif

