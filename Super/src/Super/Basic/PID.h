#ifndef PID_H_
#define  PID_H_

#include "global.h"

 NAME_SPACE_MAIN_BEGIN

//对变量的声明
typedef struct PID{
	double SetPoint;            //设定值
	double Kp;                  //比例系数
	double Ki;                  //积分系数
	double Kd;                  //微分系数
	double LastError;           //最后一次误差数Er[-1]
	double PrevError;           //最后第二次误差数er[-2]
	double SumError;            //误差积分  
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

