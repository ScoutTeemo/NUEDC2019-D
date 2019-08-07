/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_OSC.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ���������͹ر�����
*********************************************************************************************************
*/
#include "App_Switch.h"


PID_TypeDef	Pid_data; 				   //��ѹ��PID����

void Switch_main(void)
{
    static float Last_Vol;

    while(1)
    {
        /***      PID����       ***/
        if(!ADF4351_Sweep)
        {

            Last_Vol=(float)ADC_Result_Get(1,6)*0.5f + Last_Vol*0.5f;

            Dac2_Set_Vol(pid_calc( &Pid_data,(float)Wave_Vol/3300.0f*4096.0f, Last_Vol/Divisor));

        }

        if(Key_Now_Get(KEY1,KEY_MODE_SHORT))
        {

            OSTaskRecovery(6);

            OSTaskSuspend(5);    //��������

            OSTaskSuspend(4);    //��������

            OS_LCD_Clear(WHITE);

        }
        else if(Key_Now_Get(KEY2,KEY_MODE_SHORT))
        {

            OSTaskRecovery(5);   //�ָ�����

            OSTaskRecovery(4);   //�ָ�����

            OSTaskSuspend(6);

            OS_LCD_Clear(WHITE);

            TouchKey_Draw();//��С����

            Start_Meum();

        }

        OSTimeDly(55);

    }

}

/**----------------------------------------------------------------------------
* @FunctionName  : PID_Init()
* @Description   : None
* @Data          : 2019/7/26
* @Explain       : PID��ʼ��
------------------------------------------------------------------------------*/
void PID_Init(void)
{
    pid_reset(& Pid_data);

    Pid_data.T			= (float)80.0;
    Pid_data.Kp			= 0.65;
    Pid_data.Ti			= 100.0;
    Pid_data.Td			= 0.02;
    Pid_data.OutMin	= 0.00f/3.3f * 4096;
    Pid_data.OutMax = 3.30f /3.3f* 4096;
    Pid_data.L_Output=100;

    pid_init (& Pid_data);
}
