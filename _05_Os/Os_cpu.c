/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Os_cpu.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
*********************************************************************************************************
*/

#define OS_SET

#include "Os_cpu.h"
#include "Os_malloc.h"

unsigned char Sched_flag = 1;//��������־λ

void OSGetHighRdy(void);
/*
*************************************************************************
*
*	����  ��	�������ȼ�
*	����  ��	1��������
*
**************************************************************************
*/
__inline void OSSetPrioRdy(unsigned char prio)
{
	OSRdyTbl|=0x01<<prio;
}

/*
*************************************************************************
*
*	����	��	ɾ�����ȼ�
*	����	��	1��������
*
**************************************************************************
*/
__inline void OSDelPrioRdy(unsigned char prio)
{
	OSRdyTbl&=~(0x01<<prio);
}

/*
*************************************************************************
*
*	����	��	��ȡ���ȼ�
*	����	��
*
**************************************************************************
*/
__inline void OSGetHighRdy(void)				
{									
	unsigned char	OS_NEXT_Prio;	
	for(OS_NEXT_Prio=0;(OS_NEXT_Prio<OS_MAX_Task)&&(!(OSRdyTbl&(0x01<<OS_NEXT_Prio)));OS_NEXT_Prio++);
	OS_PrioHighRdy=OS_NEXT_Prio;	
}

/*
*************************************************************************
*
*	����	��	������ȣ��������ȼ�������񲢵��ȣ�
*	����	��
*
**************************************************************************
*/
void OS_Sched(void)
{
	unsigned int cpu_sr;
	OS_ENTER_CRITICAL();                                  //�����ٽ��� ���������ж� NMI����
	
	if(Sched_flag)
	{
			OSGetHighRdy();    							//�ҳ���������������ȼ���ߵ�����
			if(OS_PrioHighRdy!=OS_PrioCur)              //������ǵ�ǰ�������񣬽����������
			{
				p_TCBHightRdy=&TCB_Task[OS_PrioHighRdy];
				//p_TCB_Cur=&TCB_Task[OS_PrioCur];
				OS_PrioCur= OS_PrioHighRdy;//����OS_PrioCur
				OSCtxSw();//��������,�ڻ��������
			}
	}
	
	OS_EXIT_CRITICAL();                                 //�˳��ٽ���
}
/*
*************************************************************************
*
*	����	��	����������
*	����	��
*
**************************************************************************
*/
void OS_SchedLock(void)
{
	unsigned int cpu_sr;
	OS_ENTER_CRITICAL();                                  //�����ٽ���

	Sched_flag = 0;
	
	OS_EXIT_CRITICAL();                                 //�˳��ٽ���
}

/*
*************************************************************************
*
*	����	��	����������
*	����	��
*
**************************************************************************
*/
void OS_SchedUnlock(void)
{
	unsigned int cpu_sr;
	OS_ENTER_CRITICAL();                                  //�����ٽ���

	Sched_flag = 1;
	
	OS_EXIT_CRITICAL();                                 //�˳��ٽ���
}

/*
*************************************************************************
*
*	����	��	��ʼ���δ�ʱ��
*	����	��
*
**************************************************************************
*/
void System_init(void)
{
	u32 reload;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8=168/8=21M 
	fac_us=SystemCoreClock/8000000;							//Ϊϵͳʱ�ӵ� 1/8
	
	reload=SystemCoreClock/8000000;							//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/System_Ticks;		//����System_Ticks�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.7989s����	
	fac_ms=1000/System_Ticks;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 		//ÿ1/System_Ticks���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    

}

/*
*************************************************************************
*
*	����	��	�δ�ʱ���ж�
*	����	��	ÿ1/System_Ticks �ж�һ�� ���ݶ�10ms��
*
**************************************************************************
*/
void SysTick_Handler(void)
{
	unsigned int cpu_sr;
	unsigned char i=0;

	
	//OS_Tisks++;
	
	if(OS_Running == 1 &&Sched_flag == 1)//ϵͳ���к��ٿ�ʼ
	{
			for(;i<OS_MAX_Task;i++)
		 {
				
				OS_ENTER_CRITICAL();
				if(TCB_Task[i].DLy)
				{
					TCB_Task[i].DLy-=1000/System_Ticks;
					if(TCB_Task[i].DLy==0&&TCB_Task[i].OSTCBStatPend == OS_Resume)			//��ʱʱ�ӵ���������״̬Ϊ����״̬
					{
						OSSetPrioRdy(i);            //�������¾���
					}
				}
					
				OS_EXIT_CRITICAL();
		 }
		 
		OS_Sched();//������pendsv�жϽ��е���
	 }
}

/*
*************************************************************************
*
*	����	��	����ɾ������
*	P S 	��	����һ�㲻��Ҫɾ��
*
**************************************************************************
*/
void Task_End()
{
	while(1)
	{
		;
	}
}

/*
*************************************************************************
*
*	����	��	���ڴ�������
*	����	��	1��	������ָ��
*						2��	�����ջָ��
*						3��	���ȼ�
*
**************************************************************************
*/
void Task_Create(void (*task)(void),unsigned int *stk,unsigned char prio)
{
	unsigned int * p_stk;
	p_stk=stk;
	p_stk=(unsigned int *) ((unsigned int)(p_stk)&0xFFFFFFF8u);
	
#if (__FPU_PRESENT==1)&&(__FPU_USED==1)	
	*(--p_stk) = (unsigned int)0x00000000L; //No Name Register  
	*(--p_stk) = (unsigned int)0x00001000L; //FPSCR
	*(--p_stk) = (unsigned int)0x00000015L; //s15
	*(--p_stk) = (unsigned int)0x00000014L; //s14
	*(--p_stk) = (unsigned int)0x00000013L; //s13
	*(--p_stk) = (unsigned int)0x00000012L; //s12
	*(--p_stk) = (unsigned int)0x00000011L; //s11
	*(--p_stk) = (unsigned int)0x00000010L; //s10
	*(--p_stk) = (unsigned int)0x00000009L; //s9
	*(--p_stk) = (unsigned int)0x00000008L; //s8
	*(--p_stk) = (unsigned int)0x00000007L; //s7
	*(--p_stk) = (unsigned int)0x00000006L; //s6
	*(--p_stk) = (unsigned int)0x00000005L; //s5
	*(--p_stk) = (unsigned int)0x00000004L; //s4
	*(--p_stk) = (unsigned int)0x00000003L; //s3
	*(--p_stk) = (unsigned int)0x00000002L; //s2
	*(--p_stk) = (unsigned int)0x00000001L; //s1
	*(--p_stk) = (unsigned int)0x00000000L; //s0
#endif
	//���¼Ĵ���˳���PendSV�˳�ʱ�Ĵ����ָ�˳��һ��
	*(--p_stk)=(unsigned int)0x01000000uL;//xPSR״̬�Ĵ�������24λTHUMBģʽ������λһ 
	*(--p_stk)=(unsigned int)task;//entry point//�������
	*(--p_stk)=(unsigned int)Task_End ;//R14(LR);
	*(--p_stk)=(unsigned int)0x12121212uL;//R12
	*(--p_stk)=(unsigned int)0x03030303uL;//R3
	*(--p_stk)=(unsigned int)0x02020202uL;//R2
	*(--p_stk)=(unsigned int)0x01010101uL;//R1
	*(--p_stk)=(unsigned int)0x00000000uL;//R0

	
#if (__FPU_PRESENT==1)&&(__FPU_USED==1)	
	*(--p_stk) = (unsigned int)0x00000031L; //s31
	*(--p_stk) = (unsigned int)0x00000030L; //s30
	*(--p_stk) = (unsigned int)0x00000029L; //s29
	*(--p_stk) = (unsigned int)0x00000028L; //s28
	*(--p_stk) = (unsigned int)0x00000027L; //s27
	*(--p_stk) = (unsigned int)0x00000026L; //s26	
	*(--p_stk) = (unsigned int)0x00000025L; //s25
	*(--p_stk) = (unsigned int)0x00000024L; //s24
	*(--p_stk) = (unsigned int)0x00000023L; //s23
	*(--p_stk) = (unsigned int)0x00000022L; //s22
	*(--p_stk) = (unsigned int)0x00000021L; //s21
	*(--p_stk) = (unsigned int)0x00000020L; //s20
	*(--p_stk) = (unsigned int)0x00000019L; //s19
	*(--p_stk) = (unsigned int)0x00000018L; //s18
	*(--p_stk) = (unsigned int)0x00000017L; //s17
	*(--p_stk) = (unsigned int)0x00000016L; //s16
#endif
	//PendSV����ʱδ�Զ�������ں˼Ĵ�����R4~R11
	*(--p_stk)=(unsigned int)0x11111111uL;//R11
	*(--p_stk)=(unsigned int)0x10101010uL;//R10
	*(--p_stk)=(unsigned int)0x09090909uL;//R9
	*(--p_stk)=(unsigned int)0x08080808uL;//R8
	*(--p_stk)=(unsigned int)0x07070707uL;//R7
	*(--p_stk)=(unsigned int)0x06060606uL;//R6
	*(--p_stk)=(unsigned int)0x05050505uL;//R5
	*(--p_stk)=(unsigned int)0x04040404uL;//R4
	
	TCB_Task[prio].StkPtr =p_stk;//����������ƿ���Ӧ��ָ��ջ����ָ�룬ָ���˸��������ջ��
	TCB_Task[prio].DLy =0;
	TCB_Task[prio].OSTCBStatPend = 1;
	OSSetPrioRdy(prio);
}
/*
*************************************************************************
*
*	����	��	��������
*	P S 	��	��ֹCPU�����������ϵͳ����
*
**************************************************************************
*/
void OS_IDLE_Task()
{
		unsigned int count=0;
	
		while(1)
		{
			count++;
		
		}
	
}

/*
*************************************************************************
*
*	����	��	ϵͳ����
*	����	��
*
**************************************************************************
*/
void OS_Start(void)
{
	//System_init();
	if(OS_Running == 0)
	{
		
		CPU_ExceptStkBase=CPU_ExceptStk+OS_EXCEPT_STK_SIZE-1;//Cortex-M3ջ��������
		Task_Create(OS_IDLE_Task,&IDLE_STK[IDLE_STK_SIZE-1],OS_MAX_Task-1);
		OSGetHighRdy();//�����߼��ľ�������
		OS_PrioCur= OS_PrioHighRdy;
		p_TCBHightRdy=&TCB_Task[OS_PrioHighRdy];
		OS_Running = 1 ;
		OSStartHighRdy();
	
	}

}

/*
*************************************************************************
*
*	����	��	ϵͳ��ʱ����
*	����	��	1��ʱ��  ms
*
**************************************************************************
*/
void OSTimeDly(unsigned int ticks)
{
	if(ticks> 0)
	{
		unsigned int cpu_sr;
		OS_ENTER_CRITICAL();                                  //�����ٽ���
		OSDelPrioRdy(OS_PrioCur);                             //���������
		TCB_Task[OS_PrioCur].DLy= ticks;                      //����TCB��������ʱ������
		OS_EXIT_CRITICAL();                                   //�˳��ٽ���
		OS_Sched();                                           //�������
	}
}
/*
*************************************************************************
*
*	����	��	��������
*	����	:		1: ��������������ȼ�
*
*************************************************************************
*/
int OSTaskSuspend(unsigned char prio)
{
		unsigned char flag=1,nprio;
		unsigned int  cpu_sr;
	
		OS_ENTER_CRITICAL();                                  //�����ٽ���
	
		if(prio==OS_PrioCur)//�ж��Ƿ��ǹ���ǰ����
		{
			flag = 0;
		}
		
		nprio = 0x01 && ( OSRdyTbl >> prio );//�жϹ��������Ƿ����
		
		if(nprio==0)		//��������񲻴���
		{
			OS_EXIT_CRITICAL();  														  //�˳��ٽ���
			
			return nprio;
		}
		else
		{
			TCB_Task[prio].OSTCBStatPend = OS_Suspend; //�ı�����״̬Ϊ����״̬
			OSDelPrioRdy(prio);			//ɾ���������ȼ�
		}
		
		OS_EXIT_CRITICAL();                                   //�˳��ٽ���
		
		if(flag==0)
		{
				OS_Sched();                                           //�������
		}
		
		return 1;
}

/*
*************************************************************************
*
*	����	��	�ָ�����
*	����	��	1�����ָ���������ȼ�
*
**************************************************************************
*/
int OSTaskRecovery(unsigned char prio)
{
		unsigned char nprio;
		unsigned int  cpu_sr;
	
		OS_ENTER_CRITICAL();                                  //�����ٽ���
		nprio = 0x01 & ( OSRdyTbl >> prio );//�жϹ��������Ƿ����
		if(nprio == 1)	//�ָ����������ȼ��Ѿ�����
		{
				OS_EXIT_CRITICAL();                                   //�˳��ٽ���
				return 0;
		}
		if(TCB_Task[prio].OSTCBStatPend == OS_Suspend)//����Ϊ����״̬ 
		{
			
				OSSetPrioRdy(prio); //�ָ��������ȼ�
			
				TCB_Task[prio].OSTCBStatPend=1; //�޸�����״̬Ϊ����״̬
			
				if(TCB_Task[prio].DLy == 0)//��������ʱ
				{
						OS_Sched();                                           //�������
				}
		}
		else 
		{
				OS_EXIT_CRITICAL();                                   //�˳��ٽ���
				return 0;
		}
	
	
		OS_EXIT_CRITICAL();                                   //�˳��ٽ���
		
		return 1;
	
	
}



