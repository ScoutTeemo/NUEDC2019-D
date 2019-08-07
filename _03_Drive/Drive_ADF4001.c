/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_ADF4001.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ADF4001ģ������
*********************************************************************************************************
*/
#include "Drive_ADF4001.h"

/*
****************************************************************************************
*	������	: 	ADF4001_Init
*	��  ��	: 	void
*	��  ��	: 	void
*	��  ��	: 	ADF4001��ʼ��
*	˵  ��	:	  ADF4001_CLK = PF7	ADF4001_DAT = PF8	ADF4001_LE = PF9
****************************************************************************************
*/

void ADF4001_Init(void)
{
	
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	/* Configure PG0 pin as ouput */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

	AD4001_CLK_0;
	AD4001_DAT_0;
	AD4001_LE_0;
}

/*
****************************************************************************************
*	������	: 	ADF4001_InputData
*	��  ��	��	data:	ADF4001����������	
*	��  ��	:		void
*	��  ��	:		ģ��SPI���ߣ���ADF4001д������
****************************************************************************************
*/
static void ADF4001_InputData(unsigned long int data)
{ 
	int i;
	long int temp=0x800000;
	
	AD4001_CLK_0;
	AD4001_LE_0;

	for(i=0;i<24;i++)
	{
		if(data&temp)
			AD4001_DAT_1;
		else 
			AD4001_DAT_0;

		AD4001_CLK_1;
		Delay_ms(2);
		AD4001_CLK_0;
		Delay_ms(2);
		temp = temp>>1;
	}
	AD4001_LE_1;
	Delay_ms(10);
}

#ifndef R_COUNTER
/*
****************************************************************************************
*	������	: 	ADF4001_SetFreq
*	��  ��	��	RCounter:	R��������R COUNTER��ֵ
							NCounter:	N��������N COUNTER��ֵ
							Muxout:		MUXOUT�������״̬
*	��  ��	:		void
*	��  ��	:		����ADF4001���������Ƶ�ʼ�MUXOUT���ŵ����״̬
*	˵  ��	��	ADF4001���������Ƶ�� Fout=REF*NCounter/RCounter
****************************************************************************************
*/
void ADF4001_SetFreq(unsigned long int RCounter, unsigned long int NCounter,unsigned int Muxout)
{
	unsigned long int Rdata,Ndata,Idata;
	
	Rdata = (RCounter<<2) | 0x000000;
	Ndata = (NCounter<<8) | 0x000001;
	Idata = (Muxout<<4)   | 0x0D8883;

	ADF4001_InputData(Idata);//Init Latch															  
	ADF4001_InputData(Rdata);//R
	ADF4001_InputData(Ndata);//N	 	
}
#else
/*
****************************************************************************************
*	������	:		ADF4001_SetFreq
*	��  ��	��	freq:		ADF4001�������Ƶ��ֵ
							Muxout:		MUXOUT�������״̬
*	��  ��	:		void
*	��  ��	:		����ADF4001���������Ƶ�ʼ�MUXOUT���ŵ����״̬
*	˵  ��	��	ADF4001���������Ƶ�� Fout=REF*NCounter/RCounter
****************************************************************************************
*/
void ADF4001_SetFreq(unsigned long int freq,unsigned long int Muxout)
{
	unsigned long int step;
	unsigned long int Rdata,Ndata,Idata;	

	step  = 24000000/R_COUNTER;
	Rdata = (R_COUNTER<<2) | 0x000000;
	Ndata = ((freq/step)<<8) | 0x000001;
	Idata = (Muxout<<4) | 0x0D8883;
	
	ADF4001_InputData(Idata);//Init Latch											  
	ADF4001_InputData(Rdata);//R Latch
	ADF4001_InputData(Ndata);//N Latch	 	
}
#endif

