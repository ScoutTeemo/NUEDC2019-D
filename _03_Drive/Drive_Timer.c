/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_Timer.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ��ʱ�����ó���
*********************************************************************************************************
*/
#include "Drive_Timer.h"



/**----------------------------------------------------------------------------
* @FunctionName  : TIM1_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ� 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/

void TIM1_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ�� 
	TIM_TimeBaseStructure.TIM_Period =5000-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =8400-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);//����						    		
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 

	NVIC_InitStructure.NVIC_IRQChannel= TIM1_UP_TIM10_IRQn; //��ʱ�� x �ж�							
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2 ; //��ռ���ȼ� 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC

	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
}   


/**----------------------------------------------------------------------------
* @FunctionName  : TIM2_Init()     
* @Description   : None 
* @Data          : 2016/3/19
* @Explain       : �����ж�ADF4351ɨƵ״̬
* @PS						 : 
------------------------------------------------------------------------------*/
void TIM2_Init(u32 pre,u32 period)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); ///ʹ�� TIMxʱ��				   

//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	TIM_TimeBaseStructure.TIM_Prescaler = pre-1; //Ԥ��Ƶ 
  TIM_TimeBaseStructure.TIM_Period = period-1;//��װ��ֵ 32λ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);//����						    		//@

	
	TIM_ITConfig(TIM2,TIM_IT_CC3,ENABLE);//����ʱ�� �����ж�					
//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn; //��ʱ�� x �ж�						
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2 ; //��ռ���ȼ� 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC
	
 	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ�� x											

}   


/**----------------------------------------------------------------------------
* @FunctionName  : TIM3_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ� 
* @Data          : 2018/7/16
* @Explain       : ADC�����ʵ���
------------------------------------------------------------------------------*/
void TIM3_Init(u32 pre,u32 period)
{

 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);          

 
    TIM_Cmd(TIM3, DISABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    
   
    TIM_TimeBaseStructure.TIM_Prescaler = pre-1; 
    TIM_TimeBaseStructure.TIM_Period = period-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ; 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
    

    TIM_ARRPreloadConfig(TIM3, ENABLE); 
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);  


    TIM_Cmd(TIM3, ENABLE); 

}   


	
/**----------------------------------------------------------------------------
* @FunctionName  : TIM4_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ� 
* @Data          : 2018/7/16
* @Explain       : ͨ�ŷ������ʿ��ƣ�8K��
------------------------------------------------------------------------------*/
void TIM4_Init(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =2-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =2625-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM4_IRQn; //��ʱ�� x �ж�							//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ; //��ռ���ȼ� 4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC
 	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ�� x				4								//@
/*---------------------------------------------------------------------------------------------*/
}			

/**----------------------------------------------------------------------------
* @FunctionName  : TIM5_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ�  
* @Data          : 2018/7/16
* @Explain       : ͨ�Ž������ʿ���
------------------------------------------------------------------------------*/
void TIM5_Init(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); ///ʹ�� TIMxʱ��				   

	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5); //GPIOB10����Ϊ��ʱ��2	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	
	TIM_TimeBaseStructure.TIM_Period = 1000000-1;//��װ��ֵ 32λ
	TIM_TimeBaseStructure.TIM_Prescaler =84-1;//Ԥ��Ƶ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);//����						    		//@

	
/*---------------------------------------------------------------------------------------------*/ 
	TIM_ICStructure.TIM_Channel=TIM_Channel_3;	//ͨ��2
	TIM_ICStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//������
	TIM_ICStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//����ѡ��
	TIM_ICStructure.TIM_ICFilter=0;														//�����˲�
	TIM_ICStructure.TIM_ICPrescaler=TIM_ICPSC_DIV8;					//�����Ƶ
	TIM_ICInit(TIM5, &TIM_ICStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_CC3|TIM_IT_Update,ENABLE);//����ʱ�� �����ж�					
//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM5_IRQn; //��ʱ�� x �ж�						
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2 ; //��ռ���ȼ� 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;	
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC
	
 	TIM_Cmd(TIM5,ENABLE); //ʹ�ܶ�ʱ�� x			
}
		
/**----------------------------------------------------------------------------
* @FunctionName  : TIM6_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ�  
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/
void TIM6_Init(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =1000000-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =84-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM6_DAC_IRQn; //��ʱ�� x �ж�							//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ; //��ռ���ȼ� 4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC
 	TIM_Cmd(TIM6,ENABLE); //ʹ�ܶ�ʱ�� x				5								//@
/*---------------------------------------------------------------------------------------------*/
}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM7_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ�  
* @Data          : 2018/7/16
* @Explain       :
------------------------------------------------------------------------------*/
void TIM7_Init(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =5000-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =8400-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM7_IRQn; //��ʱ�� x �ж�							//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ; //��ռ���ȼ� 4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC
 	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ�� x				5								//@
/*---------------------------------------------------------------------------------------------*/
}

/**----------------------------------------------------------------------------
* @FunctionName  : TIM8_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ�  
* @Data          : 2018/7/16
* @Explain       : ���������
------------------------------------------------------------------------------*/
void TIM8_Init(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 ;      //PC34
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOB , &GPIO_InitStructure); 
	GPIO_ResetBits(GPIOB ,GPIO_Pin_1);  
	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;    //оƬ���
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
////	GPIO_InitStructure.GPIO_Pin=GPIO_PuPd_UP;
//	GPIO_Init(GPIOD,&GPIO_InitStructure);    
//	GPIO_ResetBits(GPIOD,GPIO_Pin_11);    
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =200-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =168-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM8_UP_TIM13_IRQn; //��ʱ�� x �ж�							//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3 ; //��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC
 	TIM_Cmd(TIM8,DISABLE); //ʹ�ܶ�ʱ�� x				4								//@ 



}


/**----------------------------------------------------------------------------
* @FunctionName  : TIM9_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ� 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/
void TIM9_Init(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =5000-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =8400-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM1_BRK_TIM9_IRQn; //��ʱ�� x �ж�							//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ; //��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC
 	TIM_Cmd(TIM9,ENABLE); //ʹ�ܶ�ʱ�� x				4								//@ 
/*---------------------------------------------------------------------------------------------*/
}	

/**----------------------------------------------------------------------------
* @FunctionName  : TIM10_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ�  
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/
void TIM10_Init(void)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =5000-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =8400-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM1_UP_TIM10_IRQn; //��ʱ�� x �ж�				//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1 ; //��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC
 	TIM_Cmd(TIM10,ENABLE); //ʹ�ܶ�ʱ�� x				4								//@
/*---------------------------------------------------------------------------------------------*/
}	
		
/**----------------------------------------------------------------------------
* @FunctionName  : TIM11_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ� 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/
void TIM11_Init(void)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =5000-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =8400-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM11, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM11,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM1_TRG_COM_TIM11_IRQn; //��ʱ�� x �ж�			//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 5 ; //��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC_
 	TIM_Cmd(TIM11,ENABLE); //ʹ�ܶ�ʱ�� x				4								//@
/*---------------------------------------------------------------------------------------------*/
}	

/**----------------------------------------------------------------------------
* @FunctionName  : TIM12_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ� 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/
void TIM12_Init(void)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =5000-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =8400-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM8_BRK_TIM12_IRQn; //��ʱ�� x �ж�			//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2; //��ռ���ȼ� 4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC_
 	TIM_Cmd(TIM12,ENABLE); //ʹ�ܶ�ʱ�� x				4								//@ 
/*---------------------------------------------------------------------------------------------*/
}	

/**----------------------------------------------------------------------------
* @FunctionName  : TIM13_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ� 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/
void TIM13_Init(void)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =5000-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =8400-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM8_UP_TIM13_IRQn; //��ʱ�� x �ж�			//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 5 ; //��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC_
 	TIM_Cmd(TIM13,ENABLE); //ʹ�ܶ�ʱ�� x				4								//@
/*---------------------------------------------------------------------------------------------*/
}	


/**----------------------------------------------------------------------------
* @FunctionName  : TIM14_Init()     
* @Description   : TIM_Period��TIM_Period������Ҫ�Ķ� 
* @Data          : 2018/7/16
* @Explain       : 
------------------------------------------------------------------------------*/
void TIM14_Init(void)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
//����ʱ�Ӽ���ʼ��Ԥװֵ
/*---------------------------------------------------------------------------------------------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE); ///ʹ�� TIMxʱ��				   //@
	TIM_TimeBaseStructure.TIM_Period =5000-1;//��װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =8400-1;//Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����ۼ�ģʽ������0��ʼ�ӵ����ֵ
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);//����						    		//@
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);//����ʱ�� �����ж�							//@ 
/*---------------------------------------------------------------------------------------------*/ 

//�����жϲ�
/*---------------------------------------------------------------------------------------------*/	
	NVIC_InitStructure.NVIC_IRQChannel= TIM8_TRG_COM_TIM14_IRQn; //��ʱ�� x �ж�			//@
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 5 ; //��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0 ; //��Ӧ���ȼ�  
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);// ��ʼ�� NVIC_
 	TIM_Cmd(TIM14,ENABLE); //ʹ�ܶ�ʱ�� x				4								//@
/*---------------------------------------------------------------------------------------------*/
}








