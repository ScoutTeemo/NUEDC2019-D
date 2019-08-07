#ifndef __DRIVE_DMA_H
#define __DRIVE_DMA_H 

/* ͷ�ļ����� ----------------------------------------------------------------*/  
#include "User_header.h"

/* ȫ�ֺ궨�� ----------------------------------------------------------------*/ 
//#define 	ADC1_DMA2_LENTH 		4000
//#define 	DAC1_DMA1_LENTH			2000 
#define 	ADC1_DMA2_LENTH 		8//2048//1
#define 	ADC2_DMA2_LENTH 		8//2048//1
#define 	ADC3_DMA2_LENTH 		8
#define 	DAC1_DMA1_LENTH			1


/* �ṹ������ ----------------------------------------------------------------*/ 
																	
/* ȫ�ֱ������� --------------------------------------------------------------*/ 

extern u16 ADC1_DMA2_Buff[ADC1_DMA2_LENTH];
extern u16 ADC2_DMA2_Buff[ADC2_DMA2_LENTH];
extern u16 ADC3_DMA2_Buff[ADC3_DMA2_LENTH];
extern u16 DAC1_DMA1_Buff[DAC1_DMA1_LENTH];



/* ȫ�ֺ������� --------------------------------------------------------------*/    
void ADC1_DMA2_Reload(void); 
void ADC3_DMA2_Reload(u8 Speed);
void ADC3_DMA2_Init(void);	

void DAC1_DMA1_Init(void);
void DAC1_DMA1_Reload(u32 speed);

#endif



