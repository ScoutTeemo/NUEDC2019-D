#include "Drive_ADS1256.h"


//***************************
//		Pin assign	   	
//		STM32						ADS1256

//		GPIOG_Pin_0 		---> SCLK
//		GPIOG_Pin_2			<--- DIN
//		GPIOG_Pin_4  		---> DOUT
//		GPIOG_Pin_6 		<--- DRDY
//		GPIOG_Pin_8 		---> CS
//		GPIOG_Pin_10  	<--- PDWN

//***************************	

/*�˿ڶ���*/ 
#define SCLK		 	 GPIO_Pin_0
#define DIN	 			 GPIO_Pin_4
#define DOUT			 GPIO_Pin_2
#define DRDY			 GPIO_Pin_6
#define CS				 GPIO_Pin_8
#define PDWN		   GPIO_Pin_10

#define ADS1256_CS_0()   		GPIO_ResetBits(GPIOG,CS)
#define ADS1256_CS_1()   		GPIO_SetBits(GPIOG,CS)

#define ADS1256_SCLK_0()  	GPIO_ResetBits(GPIOG,SCLK)
#define ADS1256_SCLK_1()  	GPIO_SetBits(GPIOG,SCLK)

#define ADS1256_DIN_0()   	GPIO_ResetBits(GPIOG,DOUT) //ע�������Ƭ����DOUT����ADS1256��DIN
#define ADS1256_DIN_1()   	GPIO_SetBits(GPIOG,DOUT)

#define ADS1256_DOUT			 (GPIOG->IDR & DIN)          //
#define ADS1256_DRDY			 (GPIOG->IDR & DRDY)
#define ADS1256_PDWN		   (GPIOG->IDR & PDWN)



/*
*********************************************************************************************************
*	�� �� ��: Init_ADS1256_GPIO
*	����˵��: ��ʼ��ADS1256 GPIO
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Init_ADS1256_GPIO(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

		GPIO_InitStructure.GPIO_Pin = SCLK | DOUT | CS ;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN; 
		GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	  ADS1256_CS_1();
	
	  GPIO_InitStructure.GPIO_Pin = DIN | DRDY | PDWN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOG,&GPIO_InitStructure);
}


/*
*********************************************************************************************************
*	�� �� ��: SPI_WriteByte
*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
*	��    ��: _data : ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SPI_WriteByte(u8 TxData)
{
	u8 i;
	
	/* �������Ͷ���ֽ�ʱ����Ҫ�ӳ�һ�� */
	delay_us(10);
	
	for(i = 0; i < 8; i++)
	{
		ADS1256_SCLK_1();
		
		if (TxData & 0x80)
			ADS1256_DIN_1();  
		else
			ADS1256_DIN_0();
		
		TxData <<= 1;
		
		delay_us(5);
		ADS1256_SCLK_0();	
		delay_us(5);
	}
	
} 

/*
*********************************************************************************************************
*	�� �� ��: SPI_ReadByte
*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8 SPI_ReadByte(void)
{
	u8 i;
	u8 read = 0;
	
	for (i = 0; i < 8; i++)
	{
		ADS1256_SCLK_1();
		read = read<<1;
		delay_us(5);
		ADS1256_SCLK_0();
		
		if(ADS1256_DOUT)
				read ++;
		
		delay_us(5);
	}
	
	return read;
}


/*
*********************************************************************************************************
*	�� �� ��: ADS1256WREG
*	����˵��: ADS1256 д����
*	��    ��: regaddr:�Ĵ�����ַ
						databyte:��д������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ADS1256WREG(u8 regaddr,u8 databyte)
{
	
	ADS1256_CS_0();
	while(ADS1256_DRDY);//��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	
	//��Ĵ���д�����ݵ�ַ
	SPI_WriteByte(ADS1256_CMD_WREG | (regaddr & 0x0F));
	
	//д�����ݵĸ���n-1
	SPI_WriteByte(0x00);
	
	delay_us(10);
	
	//��regaddr��ַָ��ļĴ���д������databyte
	SPI_WriteByte(databyte);
	
	ADS1256_CS_1();
}

/*
*********************************************************************************************************
*	�� �� ��: ADS1256RREG
*	����˵��: ADS1256 ������
*	��    ��: regaddr:�Ĵ�����ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8 ADS1256RREG(u8 regaddr)
{
	u8 databyte;
	
	ADS1256_CS_0();
	while(ADS1256_DRDY);//��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	
	//��Ĵ���д�����ݵ�ַ
	SPI_WriteByte(ADS1256_CMD_RREG | (regaddr & 0x0F));
	
	//д�����ݵĸ���n-1
	SPI_WriteByte(0x00);
	
	delay_us(10);
	
	//��regaddr��ַָ��ļĴ���д������databyte
	databyte=SPI_ReadByte();
	
	ADS1256_CS_1();

  return databyte;
}


/*
*********************************************************************************************************
*	�� �� ��: ADS1256_Init
*	����˵��: ��ʼ��ADS1256
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ADS1256_Init(void)
{
	Init_ADS1256_GPIO(); 																							  //ADS1256_GPIO��ʼ��
	delay_ms(10);
	
	ADS1256_CS_0();
	delay_us(100);
	while(ADS1256_DRDY);
	SPI_WriteByte(ADS1256_CMD_REST);																		//��λ
	delay_ms(10);
	
	while(ADS1256_DRDY);
	SPI_WriteByte(ADS1256_CMD_SELFCAL);																	 //�Զ�У׼
	
	while(ADS1256_DRDY);                             		 								//��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	SPI_WriteByte(ADS1256_CMD_SYNC);                 										//ͬ��
	SPI_WriteByte(ADS1256_CMD_WAKEUP);              										//ͬ������
	
	while(ADS1256_DRDY);
	ADS1256WREG(ADS1256_STATUS,0x04);               										// ��λ��ǰ����ʹ�û���
	ADS1256WREG(ADS1256_MUX,ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM);		//����ͨ��
	ADS1256WREG(ADS1256_ADCON,ADS1256_GAIN_1);      										// �Ŵ���1
	ADS1256WREG(ADS1256_DRATE,ADS1256_DRATE_30000SPS); 									// ����1000sps,��ͬ����ע�����ʱ����
	ADS1256WREG(ADS1256_IO,0x00);  																			//��ʹ������IO��
	
	while(ADS1256_DRDY);
	SPI_WriteByte(ADS1256_CMD_SELFCAL);																	 //�Զ�У׼
	ADS1256_CS_1();

}             



/*
*********************************************************************************************************
*	�� �� ��: ADS1256ReadData
*	����˵��: ��ȡ24λADֵ
*	��    ��: channel:ͨ��ѡ��
*	�� �� ֵ: ADֵ
*********************************************************************************************************
*/
u32 ADS1256ReadData(u8 channel)  
{
	u32 sum=0;

	while(ADS1256_DRDY);								//��ADS1256_DRDYΪ��ʱ����д�Ĵ���
	ADS1256WREG(ADS1256_MUX,channel);		//����ͨ��
	
  ADS1256_CS_0();
	delay_us(10);
	
	SPI_WriteByte(ADS1256_CMD_SYNC);		//ͬ��
	SPI_WriteByte(ADS1256_CMD_WAKEUP);	//ͬ������               
	SPI_WriteByte(ADS1256_CMD_RDATA);   //��ȡ����

	sum = (SPI_ReadByte() << 16);
	sum |= (SPI_ReadByte() << 8);
	sum |= SPI_ReadByte();

	ADS1256_CS_1();
	
	while(ADS1256_DRDY);
	
	return sum;
}

/*
*********************************************************************************************************
*	�� �� ��: Get_Val
*	����˵��: ����ȡ��24λADֵת���ɵ�ѹֵ
*	��    ��: addata:ADֵ
*	�� �� ֵ: ��ѹֵ
*********************************************************************************************************
*/
float Get_Val(u32 addata)
{
	 /*		PGA��ADS1256��ʼ�����ã�VREFΪ��·�Ļ�׼Դ������ʵ������޸�*/
	 u8 PGA=1;
	 float VREF=2.4922f;
	
	 float val;
	
	 /*  ����ѹ��ת��  */
	 if(addata > 0x800000)
	 {
			addata=0xFFFFFF-addata;
		  val= -2*VREF/(PGA*8388607)*addata;
	 }
   else
			val=2*VREF/(PGA*8388607)*addata;

	 return val;
}



/*
*********************************************************************************************************
*	�� �� ��: Middle_Filter
*	����˵��: ����ƽ���˲�
*	��    ��: channel:ͨ��ѡ��
						times:���ݸ���
*	�� �� ֵ: �˲����ADֵ
*********************************************************************************************************
*/
u32 Moving_Average_Filter(u8 channel,u16 times) 
{

	 u16 i,counts=0;
	 u32 sum=0,data[100];
	 u32 value;
	 u32 max,min;


	 //��ȡtimes������
	 while(counts < times)
	 {
			data[counts++]=ADS1256ReadData(channel);
	 }

	 max=min=data[0];
	 
	 for(i=0;i<times;i++)
	 {
		  if(data[i] > max)
				max=data[i];
			
			if(data[i] < min)
				min=data[i];
			
			sum+=data[i];
	 }

	 value=(sum - max - min)/(times - 2);
	 
	 return value;
}






