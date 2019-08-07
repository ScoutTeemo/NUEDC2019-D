/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_CDCE937.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/20
* function 	 : CDCE937��������
*********************************************************************************************************
*/
#include "Drive_CDCE937.h"


/* ˽�ж����-------------------------------------------------------------------------------------*/
#define I2C_Speed              	50000	//I2Cͨ���ٶ�	
#define I2C_SLAVE_ADDRESS7     	0x3A	  //STM32��Ϊ�ӻ���ʱ��ĵ�ַ
/* ˽�б��� --------------------------------------------------------------------------------------*/
//CDCE937�Ĵ�������ֵ��Ӧ��TI ClockPro��������ɿ��ٵó�
const uint8_t  Generic_register[7]={0x81,0x1,0xb4,0x8,0x2,0x50,0x81};
const uint8_t  PLL1_register[16]={0x0,0x0,0x0,0x0,0x6d,0x2,0x8,0x8,0x6a,0x4a,0xa3,0x4c,0x0,0x40,0x2,0x8};
const uint8_t  PLL2_register[16]={0x0,0x0,0x0,0x0,0x6d,0x2,0x8,0x8,0x6a,0x4a,0xa3,0x4c,0x0,0x40,0x2,0x8};
const uint8_t  PLL3_register[16]={0x0,0x0,0x0,0x0,0x6d,0x2,0x8,0x8,0x6a,0x4a,0xa3,0x4c,0x0,0x40,0x2,0x8};
/* ˽�к���ģ�� ----------------------------------------------------------------------------------*/
static void GPIO_Configuration(void);
static void I2C1_Configuration(void);
/* ˽�к�-----------------------------------------------------------------------------------------*/
/***************************************************************************************************
  * ����   : CDCE937ͨ�ų�ʼ��
  * ����   : ��
  * ����   : ��
  * ˵��   :
  *************************************************************************************************/
void CDCE937_Init(void)
{
	GPIO_Configuration();
	I2C1_Configuration();
}
/***************************************************************************************************
  * ����   : �������ò�������CDCE937
  * ����   : ��
  * ����   : ��
  * ˵��   :  
  *************************************************************************************************/
void CDCE937_Start(void)
{
	CECE937_WriteRegister((uint8_t*)PLL1_register,0x10,16);
	CECE937_WriteRegister((uint8_t*)PLL2_register,0x20,16);
	CECE937_WriteRegister((uint8_t*)PLL3_register,0x30,16);
	CECE937_WriteRegister((uint8_t*)Generic_register,0x00,7);
}
/***************************************************************************************************
  * ����   : д�Ĵ���
  * ����   : pBuffer->д�ֽڻ�����  WriteAddr->д��ʼ��ַ NumByteToWrite->д�ֽڸ���  
  * ����   : ��
  * ˵��   : ��CDCE937�Ĵ���д������ 
  *************************************************************************************************/
void CECE937_WriteRegister(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
{ 
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));//�ȴ����߿���
	
	I2C_GenerateSTART(I2C1, ENABLE);//������ʼ����
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));//��ʼ�����ѷ��Ͳ�Ӧ��
	
	I2C_Send7bitAddress(I2C1, CDCE937_ADDRESS, I2C_Direction_Transmitter);//���͵�ַѡ����豸
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//���ͽ�����Ӧ��  
	
	I2C_SendData(I2C1, WriteAddr);//����д��ַ
	
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//��ַ�ѷ��Ͳ�Ӧ��
	
	I2C_SendData(I2C1, NumByteToWrite);//���Ϳ鴫�䳤�� 
	/* Test on EV8 and clear it */
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	while(NumByteToWrite--)  
	{
		I2C_SendData(I2C1, *pBuffer); 	
		pBuffer++; 
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	I2C_GenerateSTOP(I2C1, ENABLE);
}
/***************************************************************************************************
  * ����   : I2C1����
  * ����   : ��
  * ����   : ��
  * ˵��   :  
  *************************************************************************************************/
static void I2C1_Configuration(void)
{ 
	I2C_InitTypeDef  I2C_InitStructure; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
// 	/* Reset I2C1 IP */
//   RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
//   /* Release reset signal of I2C1 IP */
//   RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	//����I2C
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;//STM32��Ϊ���豸�ĵ�ַ
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
	
	I2C_Cmd(I2C1, ENABLE);				//ʹ��I2C
	I2C_Init(I2C1, &I2C_InitStructure);	//I2C��ʼ��
}
/***************************************************************************************************
  * ����   : I/0����
  * ����   : ��
  * ����   : ��
  * ˵��   : CDCE937ͨ�Žӿ�
  *************************************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	/*Configure I2C SCL SDA pin */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}

