/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_GPIO.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : IO�����ó���
*********************************************************************************************************
*/
#include "Drive_GPIO.h"

void GPIO_ADF_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}

void LED_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	#if SYSTEM_VERSION
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_1;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	#else
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	#endif
}

/**----------------------------------------------------------------------------
* @FunctionName  : GPIO_Key_Init()     
* @Description   : �����ĳ�ʼ������ 
* @Data          : 2016/7/19
* @Explain       : None
------------------------------------------------------------------------------*/ 
void GPIO_Key_Init(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���� 
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOE0,2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	   
}

/**----------------------------------------------------------------------------
* @FunctionName  : GPIO_POW_Init()     
* @Description   : ��������Դ
* @Data          : 2016/7/19
* @Explain       : None
------------------------------------------------------------------------------*/ 
void GPIO_POW_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;    //оƬ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);    
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);  
	
}
/**----------------------------------------------------------------------------
* @FunctionName  : GPIO_Data_Init()     
* @Description   : ���ݷ���IO
* @Data          : 2016/7/19
* @Explain       : None
------------------------------------------------------------------------------*/ 

void GPIO_Data_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOG,&GPIO_InitStructure);    

}	

