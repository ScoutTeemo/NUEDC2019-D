#ifndef __DRIVE_ADF4001_H
#define __DRIVE_ADF4001_H 

#include "User_header.h"




#define AD4001_CLK_PIN													GPIO_Pin_11
#define AD4001_CLK_GPIO_PORT										GPIOG
#define AD4001_CLK_GPIO_CLK											RCC_AHB1Periph_GPIOG 

#define AD4001_DAT_PIN													GPIO_Pin_12
#define AD4001_DAT_GPIO_PORT										GPIOG
#define AD4001_DAT_GPIO_CLK											RCC_AHB1Periph_GPIOG 

#define AD4001_LE_PIN														GPIO_Pin_13
#define AD4001_LE_GPIO_PORT											GPIOG
#define AD4001_LE_GPIO_CLK											RCC_AHB1Periph_GPIOG 


#define AD4001_CLK_1			AD4001_CLK_GPIO_PORT->BSRRL = AD4001_CLK_PIN
#define AD4001_CLK_0  		AD4001_CLK_GPIO_PORT->BSRRH = AD4001_CLK_PIN

#define AD4001_DAT_1	    AD4001_DAT_GPIO_PORT->BSRRL = AD4001_DAT_PIN
#define AD4001_DAT_0	    AD4001_DAT_GPIO_PORT->BSRRH = AD4001_DAT_PIN

#define AD4001_LE_1				AD4001_LE_GPIO_PORT->BSRRL = AD4001_LE_PIN	
#define AD4001_LE_0				AD4001_LE_GPIO_PORT->BSRRH = AD4001_LE_PIN	


//MUXOUT�������״̬
typedef enum
{
	THREE_STATE_OUTPUT=0,		//��̬���
	DIGITAL_LOCK_DETECT,		//�����������
	N_DIVIDER_OUTPUT,				//N��Ƶ���
	AVDD_OUTPUT,						//ģ���Դ���
	R_DIVIDER_OUTPUT,				//R��Ƶ���
	OPENDRAIN_LOCK_DETECT,	//��©�������
	SERIAL_DATA_OUTPUT,			//�����������
	DGND_OUTPUT							//���ֵ����	
}MUXOUT_STATE;

void ADF4001_Init(void);

//�����������Ƶ�ʺ����ӿڣ������ַ�ʽ��
//���������R_COUNTER��ͨ������RCounter,NCounter��ֻȡ��Ƶ��
//���������R_COUNTER�����ֱ������Ƶ�ʼ���
 //#define R_COUNTER	120	
#ifndef R_COUNTER

void ADF4001_SetFreq(unsigned long int RCounter, unsigned long int NCounter,unsigned int Muxout);
#else
void ADF4001_SetFreq(unsigned long int freq,unsigned long int Muxout); 
#endif

#endif

