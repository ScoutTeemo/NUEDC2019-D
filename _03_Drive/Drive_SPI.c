/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_SPI.c
* By  			 : 
* platform       : STM32F407ZG
* Data   		 : 2019/8/7
* function 	     : SPI���ó���
*********************************************************************************************************
*/
#include "Drive_SPI.h" 

/* ȫ�ֺ�����д --------------------------------------------------------------*/
/*******************************************************************************
* ������  : SPI_Configuration
* ����    : W5500 SPI��ʼ������(STM32 SPI3)
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void SPI_Configuration(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	SPI_InitTypeDef   	SPI_InitStructure;	   

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	/* ��ʼ��SCK��MISO��MOSI���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);

	/* ��ʼ��CS���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);

	/* ��ʼ������STM32 SPI3 */
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;	//SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;							//����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;						//SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;							//ʱ�����յ�
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;							//���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;								//NSS���ⲿ�ܽŹ���
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;	//������Ԥ��ƵֵΪ2
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;					//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial=7;								//CRC����ʽΪ7
	SPI_Init(SPI3,&SPI_InitStructure);									//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI3�Ĵ���

	SPI_Cmd(SPI3,ENABLE);	//STM32ʹ��SPI3
}

/*******************************************************************************
* ������  : SPI3_Send_Byte
* ����    : SPI3����1���ֽ�����
* ����    : dat:�����͵�����
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void SPI3_Send_Byte(unsigned char dat)
{
	SPI_I2S_SendData(SPI3,dat);//д1���ֽ�����
	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);//�ȴ����ݼĴ�����
}

/*******************************************************************************
* ������  : SPI3_Send_Short
* ����    : SPI3����2���ֽ�����(16λ)
* ����    : dat:�����͵�16λ����
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void SPI3_Send_Short(unsigned short dat)
{
	SPI3_Send_Byte(dat/256);//д���ݸ�λ
	SPI3_Send_Byte(dat);	//д���ݵ�λ
}






