#ifndef __DRIVE_EXTI_H
#define __DRIVE_EXTI_H 

/* ͷ�ļ����� ----------------------------------------------------------------*/  
#include "User_header.h" 

/* ȫ�ֺ궨�� ----------------------------------------------------------------*/

/* �ṹ������ ----------------------------------------------------------------*/ 

/* ȫ�ֱ������� --------------------------------------------------------------*/ 

/* ȫ�ֺ������� --------------------------------------------------------------*/  
void Exti0_Init(void);
void Exti1_Init(void);
void Exti2_Init(void);
void Exti3_Init(void);
void Exti4_Init(void);
void Exti5_9_Init(void);
void Exti10_15_Init(void);

void Exti_Enable(u8 Exti_Num);	
void Exti_Disable(u8 Exti_Num);

#endif



