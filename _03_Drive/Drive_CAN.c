/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_CAN.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : CANͨ����������
*********************************************************************************************************
*/

#include "Drive_CAN.h"


/*
*************************************************************************
*	����	��	CAN��ʼ��
*	����	��	1����Ծ���
						2��Tbs1
						3��Tbs2
						4����Ƶϵ��
						5��ģʽѡ��
**************************************************************************
*/
void CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode) 
{

  	GPIO_InitTypeDef       GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef       NVIC_InitStructure;
	
    //ʹ�����ʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTAʱ��	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2|RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN2ʱ��	  �����ʼ��CAN1ʱ��
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //GPIO_Mode_OUT;////���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //����
    GPIO_Init(GPIOB, &GPIO_InitStructure);            //��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2); //GPIOA12����ΪCAN1
	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE; //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= mode;	  //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=tsjw;	    //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1;     //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;     //Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;//��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN2, &CAN_InitStructure); // ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=14;	  //������14
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		
	
	  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

}   
 

//�жϷ�����			    
void CAN2_RX0_IRQHandler(void)
{
	  u8 i;
  	CanRxMsg RxMessage;
	  
    CAN_Receive(CAN2, 0, &RxMessage);
	
	  for(i=0;i<8;i++)
	    OS_Num_Show(100+50*i,160,24,1,RxMessage.Data[i],"%0.0f ");
	
}

/*
*************************************************************************
*	����	��	CAN���ݷ���(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
*	����	��	1������ָ��,���Ϊ8���ֽ�.
						2�����ݳ���(���Ϊ8)	
*	����ֵ��	0,�ɹ�;
						����,ʧ��;
**************************************************************************
*/
u8 CAN2_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
	
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;	 // ��׼��ʶ��Ϊ0
  TxMessage.IDE=0;		   // ʹ����չ��ʶ��
  TxMessage.RTR=0;		   // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=len;		 // ������֡��Ϣ
	
  for(i=0;i<len;i++)
     TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ   
	
  mbox= CAN_Transmit(CAN2, &TxMessage);   
	
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)!=CAN_TxStatus_Ok)&&(i<0XFFF))
		i++;	//�ȴ����ͽ���
	
  if(i>=0XFFF)
		return 1;
	
  return 0;		

}















