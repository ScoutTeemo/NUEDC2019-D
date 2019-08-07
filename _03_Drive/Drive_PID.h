#ifndef __DRIVE_PID_H
#define __DRIVE_PID_H 

#include "User_header.h"

typedef struct  _PID{ 
	
	float	T;  		//����:����
	float	Kp; 		//����:����
	float	Ti; 		//����:����
	float	Td; 		//����:΢��

	float	a0; 		//����ֵ:a0 = Kp(1 + T/Ti + Td/T)
	float	a1; 		//����ֵ:a1 = Kp(1 + 2Td/T)
	float	a2; 		//����ֵ:a2 = Kp*Td/T

	float	Ek_0;   //����ֵ:Error[k],��ǰ���
	float	Ek_1; 	//����ֵ:Error[k-1]
	float	Ek_2; 	//����ֵ:Error[k-2]

	float	Inc;				//����ֵ(���PIDֵ����һʱ�̵�PIDֵ֮��)
	float	N_Output;		//����ֵ:PID���ֵ
	float	L_Output;		//����ֵ:�ϴ����ֵ

	float	OutMax;			//����:PID������ֵ
	float	OutMin;			//����:PID�����Сֵ
	
}PID_TypeDef;

extern PID_TypeDef	adc1_PID,adc2_PID; 

void pid_reset( PID_TypeDef	*p);
void pid_init ( PID_TypeDef *p);
float pid_calc( PID_TypeDef *p, float ref, float fb);

//void ADC1_PID_Init(void);
//void ADC2_PID_Init(void);


#endif


