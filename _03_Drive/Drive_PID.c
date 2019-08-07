/*
*********************************************************************************************************
*                                              _03_OS
* File			 : PID.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ����PID�����ڵ�Դ����
*********************************************************************************************************
*/

#include "Drive_PID.h"


/**
 **********************************************************
 * ������void PID_DeInit( PID_TypeDef	*p)
 * ������PID���ݸ�λ
 * ���룺*p�������PID����
 * ���أ��� 
 **********************************************************
 */
void pid_reset( PID_TypeDef	*p)
{
	p->a0			= 0;
	p->a1			= 0;
	p->a2			= 0;

	p->Ek_0 	= 0;     //  Error[k]
	p->Ek_1		= 0;     //  Error[k-1]
	p->Ek_2		= 0;     //  Error[k-2]

	p->T			= 0;
	p->Kp			= 0;
	p->Ti			= 0;
	p->Td			= 0;
		
	p->L_Output	= 0;
	p->N_Output	= 0;

	p->Inc			= 0;

	p->OutMax		= 0;
	p->OutMin		= 0; 
}

/**
 **********************************************************
 * ������void PID_init ( PID_TypeDef *p)
 * ������PID��������
 * ���룺*p�������PID����
 * ���أ��� 
 **********************************************************
 */
void pid_init ( PID_TypeDef *p)
{   
	
	if(p->Ti == 0)
	
	p->Ti		= p->Ti == 0 ? 0xFFFFFFFF: p->Ti;
	p->a0		= p->Kp * (1 + 1.0f * p->T / p->Ti + 1.0f * p->Td / p->T);
	p->a1		= p->Kp * (1 + 2.0f * p->Td / p->T );
	p->a2		= 1.0f * p->Kp * p->Td / p->T;
}

/**
 **********************************************************
 * ������float PID_Calc( PID_TypeDef *p)
 * ������PID����ʽ����	 
 * ���룺*p��PID�ṹ��
 * ���أ�PID������	
 **********************************************************
 */
float pid_calc( PID_TypeDef *p, float ref, float fb)
{
	p->Ek_0		= ref - fb;	//�������

	p->Inc		= (  p->a0 * p->Ek_0	- p->a1 * p->Ek_1  + p->a2 * p->Ek_2 	);				//PID����

 	p->N_Output = p->L_Output + p->Inc; 

	if(p->N_Output > p->OutMax)p->N_Output   =	p->OutMax;
	if(p->N_Output < p->OutMin)p->N_Output   =	p->OutMin;
	
	p->Ek_2        = p->Ek_1;
	p->Ek_1        = p->Ek_0;		
	p->L_Output    = p->N_Output;
	
	return p->N_Output;

}


/**
 **********************************************************
 * ������void PID_Clean( PID_TypeDef	*p)
 * ���������PID�����еĻ���
 * ���룺*p�������PID����
 * ���أ��� 
 **********************************************************
 */
void pid_clc( PID_TypeDef	*p)
{
	p->Ek_0 			= 0;     //  Error[k]
	p->Ek_1				= 0;     //  Error[k-1]
	p->Ek_2				= 0;     //  Error[k-2]
	p->Inc				= 0;
	p->L_Output 	= 0;
	p->N_Output 	= 0;
}





