/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_PWM.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : PWM���ó���
*********************************************************************************************************
*/
#include "Drive_PWM.h"

/* ˽�к궨�� ----------------------------------------------------------------*/
#define PWM1_FREQUENCY 	10000//PWM1�����Ƶ�ʣ���λHz
#define PWM1_PERIOD 	(105000.0f/PWM1_FREQUENCY)

/* ˽�У���̬���������� ------------------------------------------------------*/

/* ȫ�ֱ������� --------------------------------------------------------------*/
 
/* ȫ�ֺ�����д --------------------------------------------------------------*/
/**----------------------------------------------------------------------------
* @FunctionName  : PWM1_Init()     
* @Description   : None 
* @Data          : 2016/7/11
* @Explain       : ���������
------------------------------------------------------------------------------*/  
void PWM1_Init(u16 arr,u16 psc)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//PWM�˿ڸ���   

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); 
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);//���ö�ʱ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;        
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //����
		GPIO_Init(GPIOA,&GPIO_InitStructure);    
		GPIO_SetBits(GPIOA,GPIO_Pin_8); 
		GPIO_SetBits(GPIOA,GPIO_Pin_9); 
		
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������á�
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����������
		TIM_TimeBaseStructure.TIM_Prescaler =0;   //Ԥ��ƵTimer clock = sysclock /(TIM_Prescaler+1) = 168M
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseStructure.TIM_Period = arr;    //ת��ֵPeriod = (TIM counter clock / TIM output clock) - 1 = 20K
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);

	
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;     //BUCK  OC1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;                   //�Ƚ�ֵһ��ҪΪ��
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;     //BOOST  OC2
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);
		
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
		
		TIM_ARRPreloadConfig(TIM1,ENABLE);
	
		
		TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
		
		TIM_CtrlPWMOutputs(TIM1,ENABLE);  
}
	
/**----------------------------------------------------------------------------
* @FunctionName  : PWM1_CCR_Set()     
* @Description   : ����PWM1ռ�ձ�	
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/  	 
void PWM1_CCR_Set(double xccr1)
{	
	xccr1 = xccr1*PWM1_PERIOD;	 			
	assert_param(IS_TIM_LIST1_PERIPH(TIM14));		
	TIM14->CCR1 = (u16)xccr1; 		
}	
	
	
	
	
/**----------------------------------------------------------------------------
* @FunctionName  : PWM2_Init()     
* @Description   : PWMͨ��2
* @Data          : 2016/7/11
* @Explain       : PWM_Cap2����     PA1����λ��ʱ��5
------------------------------------------------------------------------------*/  	 
void PWM2_Init(void)
{
	
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA1

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //PA1����λ��ʱ��5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=168-1;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=1000000-1;  //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC2ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5

 
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}




