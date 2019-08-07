/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Os_cpu.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
*********************************************************************************************************
*/
#ifndef __OS_CPU_H
#define __OS_CPU_H 	 

#include "sys.h"

#ifdef   OS_SET
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

/*
*****************************************************************************
*                 	  ϵͳ�����궨��
*****************************************************************************
*/

#define OS_EXCEPT_STK_SIZE 		128u					//��ջ��С
#define OS_MAX_Task 			32u					//���������(���ȼ���)
#define IDLE_STK_SIZE 			64u					//���������ջ��С
#define OS_MAX_Event 			32u					//����¼���
#define System_Ticks  			1000u					//ÿ1000/System_Ticks ms����һ���ж�



#define OS_Resume			 1u						//����Ϊ����״̬
#define OS_Suspend     2u						//����Ϊ����״̬



#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}

/*
*****************************************************************************
*                   	ϵͳ����ȫ�ֶ���
*****************************************************************************
*/

typedef struct os_tcb
{
		unsigned int *StkPtr;//����ջ��
		unsigned int DLy;//������ʱʱ��
		unsigned char OSTCBStatPend;//����״̬
	
}TCB;


OS_EXT unsigned int  CPU_ExceptStk[OS_EXCEPT_STK_SIZE];//�������ջ
OS_EXT unsigned int * CPU_ExceptStkBase;//ָ������������һ��Ԫ��
OS_EXT unsigned int IDLE_STK[IDLE_STK_SIZE];//���������ջ
OS_EXT TCB *p_TCB_Cur;//ָ��ǰ�����tcb
OS_EXT TCB *p_TCBHightRdy;//ָ����߼������tcb


OS_EXT TCB TCB_Task[OS_MAX_Task];//������ƿ�
OS_EXT volatile unsigned char OS_PrioCur;//��¼��ǰ���е��������ȼ�
OS_EXT volatile unsigned char OS_PrioHighRdy;
OS_EXT volatile unsigned int OSRdyTbl;//���������
//OS_EXT unsigned int OS_Tisks;
OS_EXT unsigned int fac_ms;//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
OS_EXT unsigned int fac_us;//us��ʱ������	
OS_EXT unsigned char Sched_flag;//��������־λ
OS_EXT unsigned char OS_Running;//�������б�־λ

extern void OSCtxSw(void);
extern void OSStartHighRdy(void);
extern unsigned int OS_CPU_SR_Save(void);
extern void OS_CPU_SR_Restore(unsigned int cpu_sr);


/*
*****************************************************************************
*                   	ϵͳ��������
*****************************************************************************
*/

void System_init(void);
void Task_End(void);//�������
void Task_Create(void (*task)(void),unsigned int *stk,unsigned char prio);//��������
void OS_Start(void); //������������
void OS_Sched(void);//�������
void OS_SchedLock(void);//����������
void OS_SchedUnlock(void);//����������
void OSSetPrioRdy(unsigned char prio);//�������ȼ�
void OSDelPrioRdy(unsigned char prio);//ɾ�����ȼ�
void OSTimeDly(unsigned int ticks);//ϵͳ��ʱ����
int OSTaskSuspend(unsigned char prio);//��������
int OSTaskRecovery(unsigned char prio);//����ָ�

#endif

/************************************************* (C) COPYLEFT 2018 Muhe  *****************************************************/
