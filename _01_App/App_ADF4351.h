#ifndef __APP_ADF4351_H
#define __APP_ADF4351_H 

#include "User_header.h"

extern u8 ADF4351_Sweep;
extern double Start_Freq,Stop_Freq;
extern u16 Sweep_Time;
extern double Step_Freq;   			//����Ƶ��
extern u16 Wave_Vol;     					  	//�������
extern double Out_Freq;    				 //���Ƶ��
extern double Divisor;
void ADF4351_main(void);
void Touch_Handle(double data); 	 //��������
void Start_Meum(void);					  //��ʼ�˵�
void PID_Init(void);							//PID��ʼ��

#endif 
