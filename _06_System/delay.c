#include "delay.h"
#include "sys.h"
#include "User_header.h"

			 

//��ʱnus
//nus:Ҫ��ʱ��us��.	
//nus:0~204522252(���ֵ��2^32/fac_us@fac_us=21)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD��ֵ	 
	if(nus==0)return;			//nus=0,ֱ���˳�
	ticks=nus*fac_us; 			//��Ҫ�Ľ�����	  		 
	tcnt=0;
	OS_SchedLock();				//��ֹucos���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	OS_SchedUnlock();			//����ucos���� 									    
}  
//��ʱnms
//nms:Ҫ��ʱ��ms��
//nms:0~65535
void delay_ms(u16 nms)
{	

	if( OS_Running == 1)//���os�Ѿ�������
	{		 
		if(nms>=fac_ms)//��ʱ��ʱ�����ucos������ʱ������ 
		{ 
   			OSTimeDly(nms/fac_ms);	//ucos��ʱ
		}
		nms%=fac_ms;			//ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));	//��ͨ��ʽ��ʱ 
}


















