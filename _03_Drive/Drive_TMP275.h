#ifndef __DRIVE_TMP275_H
#define __DRIVE_TMP275_H

#include "User_header.h"


//---------- TPM 275 ��ض��� ----------------------------------------	 
#define TMP275_IIC_ADDR	0x90		//7λ��ַ + 0(read)
// �¶���ADCת��ֵ�ù�ϵ
#define FTMP275_TEMP_AD_STEP		0.0625
	 
//----------- GPIO ���� ----------------------------------------------
#define TMP275_SDA_CLK     RCC_AHBPeriph_GPIOB      
#define TMP275_SDA_Pin     GPIO_Pin_11
#define TMP275_SDA_GPIO    GPIOB
	 
#define TMP275_SCL_CLK     RCC_AHBPeriph_GPIOB       
#define TMP275_SCL_Pin     GPIO_Pin_12
#define TMP275_SCL_GPIO	   GPIOB
	 
	 
void TMP275Init(void);

unsigned char TMP275ReadTemprature(float *p);



#endif


