/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_CAN.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : CAN通信驱动程序
*********************************************************************************************************
*/

#include "Drive_CAN.h"


/*
*************************************************************************
*	功能	：	CAN初始化
*	参数	：	1：跳跃宽度
						2：Tbs1
						3：Tbs2
						4：分频系数
						5：模式选择
**************************************************************************
*/
void CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode) 
{

  	GPIO_InitTypeDef       GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef       NVIC_InitStructure;
	
    //使能相关时钟
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTA时钟	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2|RCC_APB1Periph_CAN1, ENABLE);//使能CAN2时钟	  必须初始化CAN1时钟
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //GPIO_Mode_OUT;////复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;      //上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);            //初始化PA11,PA12
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2); //GPIOA11复用为CAN1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2); //GPIOA12复用为CAN1
	  
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//软件自动离线管理	  
  	CAN_InitStructure.CAN_AWUM=DISABLE; //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=ENABLE;	//禁止报文自动传送 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= mode;	  //模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;	    //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1;     //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;     //Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;//分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN2, &CAN_InitStructure); // 初始化CAN1 
    
		//配置过滤器
 	  CAN_FilterInitStructure.CAN_FilterNumber=14;	  //过滤器14
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		
	
	  CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

}   
 

//中断服务函数			    
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
*	功能	：	CAN数据发送(固定格式:ID为0X12,标准帧,数据帧)	
*	参数	：	1：数据指针,最大为8个字节.
						2：数据长度(最大为8)	
*	返回值：	0,成功;
						其他,失败;
**************************************************************************
*/
u8 CAN2_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
	
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;	 // 标准标识符为0
  TxMessage.IDE=0;		   // 使用扩展标识符
  TxMessage.RTR=0;		   // 消息类型为数据帧，一帧8位
  TxMessage.DLC=len;		 // 发送两帧信息
	
  for(i=0;i<len;i++)
     TxMessage.Data[i]=msg[i];				 // 第一帧信息   
	
  mbox= CAN_Transmit(CAN2, &TxMessage);   
	
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)!=CAN_TxStatus_Ok)&&(i<0XFFF))
		i++;	//等待发送结束
	
  if(i>=0XFFF)
		return 1;
	
  return 0;		

}















