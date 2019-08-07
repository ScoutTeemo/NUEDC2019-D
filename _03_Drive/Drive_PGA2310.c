/*
*********************************************************************************************************
*                                              _03_OS
* File			 : Drive_PGA2310.c
* By  			 : 
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : PGA2310�̿طŴ�ģ������
*********************************************************************************************************
*/

#include "Drive_PGA2310.h"
#include <math.h>




void GPIO_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_5|GPIO_Pin_3|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

}

void PGA2310_Mute( FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        PGA2310_MUTE=0;
    }
    else
    {
        PGA2310_MUTE=1;
    }
    delay_ms(1);
}


void PGA2310_Init()
{
    GPIO_Config();

    /**********оƬ��ʼ��***************/
    PGA2310_CS=1;
    PGA2310_SDI=1;
    PGA2310_CLK=1;
    PGA2310_MUTE=1;



}


static void Send_onebit(uint8_t Right_N,uint8_t Left_N)        //����һλ����
{
    u16 data=0;
    int i=0;
    u16 temp;

    data=Right_N;
    data=data<<8;
    data+=Left_N;

    PGA2310_CS=0;
    PGA2310_CLK=0;
    delay_us(80);


    for(i=0; i<16; i++)
    {
        temp=data&0x8000;
        if(temp>0)
        {
            PGA2310_SDI=1;
        }
        else
        {
            PGA2310_SDI=0;
        }
        delay_us(80);
        PGA2310_CLK=1;
        data=data<<1;
        delay_us(80);
        PGA2310_CLK=0;
        delay_us(80);
    }

    PGA2310_CS=1;
}






void DB_AMP(float L_InuptV,float L_OutV,float R_InuptV,float R_OutV)
{
    double L_AMP=0.0,R_AMP=0.0;          //���ҵ�ѹ�Ŵ���
    float  R_dB=0.0,L_dB=0.0;        // �Ŵ�����
    uint8_t  Right_N,Left_N;    //����Nֵ

    L_AMP=L_OutV/L_InuptV;
    R_AMP=R_OutV/R_InuptV;

    R_dB=(float)(20.0*log10(R_AMP));   //����Ŵ�db
    L_dB=(float)(20.0*log10(L_AMP));

    Right_N=(uint8_t)(192.0f+R_dB*2.0);
    Left_N= (uint8_t)(192.0f+L_dB*2.0);
 
    Send_onebit(Right_N,Left_N);


}

