#include "PID.h"
 NAME_SPACE_MAIN_BEGIN

/****************************��νPID��ΪP��������+I�����֣�+D��΢��**************************/

#if 0
**(1)P���ƣ��������ƣ���    e(t) = SP - y(t);        SP--�趨ֵ   e(t)--���ֵ���趨��ֵ�͵�ǰ���� y(t)--����ֵ����ǰֵ��
	**�������ͺ��Բ����ϵͳ   u(t) = e(t) * P;        u(t)--���ֵ����Ҫ����ģ� P--����ϵ��
	**
	**(2)PI���ƣ�����+���֣���  u(t) = Kp*e(t) + Ki��e(t) + u0
	**��ʹϵͳ������̬����̬���u(t)--��� Kp--�����Ŵ�ϵ�� ki--���ַŴ�ϵ�� e(t)--��� u0--��������׼ֵ������ƫ�
	**
	**(3)PD����(΢�ֿ���)��     
	**΢��������Ӧ�ٶ�����   ���������ȶ���
	**(4)PID���ƣ���������΢�ֿ��ƣ���
	**΢��������Ӧ�ٶ�����   u(t) = Kp*e(t) + Ki��e(t)+ Kd[e(t) �C e(t-1)] + u0            
	**�������ڣ�������ֵ�Ĳ������ڣ�
	**�������ڣ�����ÿ���೤ʱ�����һ��PID���㣬������������
	****************************С�᣺P��������--��ʱ�� I(����)--��Ӧ�ٶ� D��΢�֣�--�ȶ���************************************
*/
#endif

#include<stdio.h>
#include<string.h>

//#include<stdib.h>



/*
**PID���㲿��
*/
double PIDCalc(PID *pp, double NextPoint)   
{
	double dError,                              //��ǰ΢��
		Error;                               //ƫ��
	Error = pp->SetPoint - NextPoint;           //ƫ��ֵ=�趨ֵ-����ֵ����ǰֵ��
	pp->SumError += Error;                      //����=����+ƫ��  --ƫ����ۼ�
	dError = pp->LastError - pp->PrevError;     //��ǰ΢�� = ������ - ֮ǰ���
	pp->PrevError = pp->LastError;              //���¡�֮ǰ��
	pp->LastError = Error;                      //���¡������
	return (pp->Kp * Error                      //������ = �������� * ƫ��
		+   pp->Ki *  pp->SumError              //������ = ���ֳ��� * ������
		+   pp->Kd * dError                     //΢���� = ΢�ֳ��� * ��ǰ΢��
		);
}
//ΪPID���������ڴ棬��Χָ��pp��ָ��
void PIDInit (PID *pp)   
{
	//memset�Ǽ������C / C++���Ժ�������s��ָ���ĳһ���ڴ��е�ǰn��
	memset(pp, 0, sizeof(PID)); //�ֽڵ�����ȫ������Ϊchָ����ASCIIֵ����Ĵ�С�ɵ���������ָ����
}                               //�������ͨ��Ϊ��������ڴ�����ʼ�������� �䷵��ֵΪָ��s��ָ�롣

double sensor(void)                 //����
{   
	double shuru;
	scanf("%lf", &shuru);
	return shuru;
}

void actuator(double rDelta)       //���
{
	printf("%lf\n", rDelta);
}


#ifdef UNIT_TEST

int Test_PID()
{
	PID sPID;                       //PID�ṹ��
	double rOut;                    //�������
	double rIn;                     //�������
	PIDInit(&sPID);                 //��ʼ���ṹ��
	sPID.Kp = 0.5;
	sPID.Ki = 0.5;                  
	sPID.Kd = 0.5;
	sPID.SetPoint = 100.0;

	for (;;)
	{
		rIn = sensor();             //ȡ����ֵ
		rOut = PIDCalc(&sPID, rIn); //PID����
		actuator(rOut);             //���
	}
}

#endif


 NAME_SPACE_MAIN_END