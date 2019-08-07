/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_TMP275.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : TMP275����ģ����������
*********************************************************************************************************
*/
#include "Drive_TMP275.h"


static void TMP275IOInit(void);

static unsigned char TMP275Write(unsigned char reg, unsigned int dt);
static unsigned char TMP275Read(unsigned char reg, unsigned int *p);

static void TMP275I2cStart(void);
static void TMP275I2cStop(void);
static unsigned char TMP275I2cWriteByte(unsigned char dt);
static unsigned char TMP275I2cReadByte(unsigned char isack);

static void TMP275PinSclk(unsigned char st);
static void TMP275PinSda(unsigned char st);
static unsigned char TMP275PinSdaGet(void);
static void TMP275PinSdaCfgRx(unsigned char st);


/*
*********************************************************************************
*
* ��������	: 
*	��    ��	:	 ��ʼ����
*
**********************************************************************************
*/
void TMP275Init(void)
{
	TMP275IOInit();
	
	TMP275Write(0x01, (0 >> 15) |		//OS
					 (3 >> 13) |		//R1 R0: 0-9bits  1-10bits  2-11bits  3-12bits
					 (1 >> 11) |		//F1 F0: �����ж���Ϊ2
					 (0 >> 10) |		//POL: ALERT�͵�ƽ��Ч
					 (1 >> 9) |			//TM: 1-�ж�ģʽ
					 (0 >> 8));			//SD: 0-�ض�ģʽ�ر�
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:  ��ȡ�¶�
*
**********************************************************************************
*/
unsigned char TMP275ReadTemprature(float *p)
{
	unsigned char err;
	unsigned int res;
	err = TMP275Read(0x00, (unsigned int *)&res);
	
	if (err != 0)
		return err;
    
    res >>= 4;

	if ((res & (1 << 11)) == 0)
	{
		*p = FTMP275_TEMP_AD_STEP * res;
	}
	else
	{
		*p = FTMP275_TEMP_AD_STEP * (0x1000 - res);
	}

//	*p =res;
	
	return 0;
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:  д��Ĵ�������
*
**********************************************************************************
*/
unsigned char TMP275Write(unsigned char reg, unsigned int dt)
{
	unsigned char err = 0;
	
	TMP275I2cStart();
	err += TMP275I2cWriteByte(TMP275_IIC_ADDR | 0x00);
	err += TMP275I2cWriteByte(reg);
	
	err += TMP275I2cWriteByte((dt >> 8) & 0xff);
	err += TMP275I2cWriteByte(dt & 0xff);
	
	TMP275I2cStop();
    
    return err;
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:	 ���Ĵ�������
*
**********************************************************************************
*/
unsigned char TMP275Read(unsigned char reg, unsigned int *p)
{
  unsigned char err = 0;
	unsigned int res;
	
	TMP275I2cStart();
	
	err += TMP275I2cWriteByte(TMP275_IIC_ADDR | 0x00);
	err += TMP275I2cWriteByte(reg);
	
	TMP275I2cStart();
	err += TMP275I2cWriteByte(TMP275_IIC_ADDR | 0x01);
	
	res = TMP275I2cReadByte(1);
	res <<= 8;
	res |= TMP275I2cReadByte(1);
	
	TMP275I2cStop();
	
    *p = res;
	return err;
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:  дһ���ֽ�

**********************************************************************************
*/
unsigned char TMP275I2cWriteByte(unsigned char dt) 
{
	unsigned char i;
	unsigned char err;
	
	//8 bits Data
	for(i=0; i<8; i++)
	{
	  	if(dt & 0x80)
	  		TMP275PinSda(1);     	//DAT=1;  
	  	else 
	  		TMP275PinSda(0);      	//DAT=0;  
	  	TMP275PinSclk(1);          	//scl=1; 			
	  	TMP275PinSclk(0);        	//scl=0;	
	  	dt = dt<<1;
			Delay_us(100);
	}
	
	//Test Ack
	TMP275PinSda(1);      	//DAT=1;  
	TMP275PinSdaCfgRx(1);   	//DAT in; 
	TMP275PinSclk(1);      	//scl=1;  

	
	for (i=0; i<100; i++)
	{
		err = TMP275PinSdaGet();	//read the DAT;	
		if (err == 0)
			break;
	}
	TMP275PinSclk(0);     	//scl=0;		
	TMP275PinSdaCfgRx(0);  	//DAT out; 
	
	return err;	
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:	 ��ȡһ���ֽ�
*
**********************************************************************************
*/	
unsigned char TMP275I2cReadByte(unsigned char isack) 
{
	unsigned char i;
	unsigned char res=0;
	
	TMP275PinSdaCfgRx(1);        //DAT datain;  
	
	for(i=0;i<8;i++) 
	{ 
		TMP275PinSclk(1);      	//scl=1; 

		 
		res = res<<1;
		if(TMP275PinSdaGet()) 	//read the DAT;
			res = res | 0x01;

		
		TMP275PinSclk(0);    	//scl=0;	
		Delay_us(100);				//////////////////////
	}
	
	TMP275PinSdaCfgRx(0);      //DAT dataout; 

	
	if (isack != 0)
	{
		TMP275PinSda(0);   	//DAT=0;		
		TMP275PinSclk(1);    	//scl=1;  	
		TMP275PinSclk(0);     	//scl=0;
		
	}
	else
	{
		TMP275PinSda(1);       //DAT=1;		
		TMP275PinSclk(1);    	//scl=1;		
		TMP275PinSda(0);    	//DAT=0;  		
		TMP275PinSclk(0);     	//scl=0;

	}
	 
	return res;
}

/*
*********************************************************************************
*
* ��������	:	 
* ��    ��	:	 ��ʼ�ź�
*
**********************************************************************************
*/
void TMP275I2cStart(void)
{
	TMP275PinSda(1);                //DAT=1;  
	
	TMP275PinSclk(1);                //scl=1;  
	
	TMP275PinSda(0);               //DAT=0;  
	
	TMP275PinSclk(0);               //scl=0;	
	
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:  ֹͣ�ź�
* 
**********************************************************************************
*/
void TMP275I2cStop(void)
{
	TMP275PinSda(0);               //DAT=0;  
	
	TMP275PinSclk(1);            	//scl=1;
	
	TMP275PinSda(1);            	//DAT=1; 
	
	TMP275PinSclk(0);               //scl=0;
	
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:  IO�ڳ�ʼ
*
**********************************************************************************
*/
void TMP275IOInit(void)
{
	GPIO_InitTypeDef TMP275_GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	TMP275_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	TMP275_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	TMP275_GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	TMP275_GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	// SDA Pin ��ʼ��
	TMP275_GPIO_InitStructure.GPIO_Pin = TMP275_SDA_Pin;
	GPIO_Init(TMP275_SDA_GPIO, &TMP275_GPIO_InitStructure);
	// SCL Pin ��ʼ��
	TMP275_GPIO_InitStructure.GPIO_Pin = TMP275_SCL_Pin;
	GPIO_Init(TMP275_SCL_GPIO, &TMP275_GPIO_InitStructure);
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:	 ��λ/��λCLK
*
**********************************************************************************
*/
void TMP275PinSclk(unsigned char st)
{
	GPIO_WriteBit(TMP275_SCL_GPIO, TMP275_SCL_Pin, (st == 0) ? Bit_RESET : Bit_SET);
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:	 ��λ/��λSDA
*
**********************************************************************************
*/

void TMP275PinSda(unsigned char st)
{
	GPIO_WriteBit(TMP275_SDA_GPIO, TMP275_SDA_Pin, (st == 0) ? Bit_RESET : Bit_SET);
}

/*
*********************************************************************************
*
* ��������	: 
* ��    ��	:  SDA���ŵ�ƽ��ȡ����
*
**********************************************************************************
*/
unsigned char TMP275PinSdaGet(void)
{
	return GPIO_ReadInputDataBit(TMP275_SDA_GPIO, TMP275_SDA_Pin);
}

/*
*********************************************************************************
*
* ��������	:  TMP275PinSdaCfgRx(unsigned char st);
* ��    ��	: 	SDA�շ� ��������
* �������	�� st: �շ���־ ��1�����գ�
*
**********************************************************************************
*/
static void TMP275PinSdaCfgRx(unsigned char st)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = TMP275_SDA_Pin; 		// TMP SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	if (st == 0)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT ; 
		
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	}
	
	GPIO_Init(TMP275_SDA_GPIO, &GPIO_InitStructure);
}

