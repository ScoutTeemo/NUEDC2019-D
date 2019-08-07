#ifndef DRIVE_COMMUNICATION_H
#define DRIVE_COMMUNICATION_H

#include "stdint.h"
#include "stm32f4xx.h" 	
#include "Drive_ADC.h"  
#include "Drive_Timer.h"
//#include "App_NIT.h"

#ifndef True
#define True 1
#endif

#ifndef False
#define False  0
#endif

/* ͨ��Э��ָ�� */
#define    NORMAL         0x0001 //��ͨģʽ
#define    SWEEP          0x0002 //ɨƵģʽ
#define		 MODULATION			0x0004 //����ģʽ
#define		 INPHASE				0x0008 //ͬ�����ģʽ

#define    OSCDATA                0x0001
#define    FREQUENCYDATA          0x0002
#define    OPENOSC                0x0004
#define    CLOSEOSC               0x0008
#define    SAMPINGRATE            0x0010
#define    DDSDATA                0x0020

#define    RX_LENGTH      48
#define    TX_LENGTH      2048

typedef struct communicationDataStruct
{
    u8 rxBuffer[RX_LENGTH];
    u8 txBuffer[TX_LENGTH];
    u8 flag;					  //������λ
}communicationDataStruct;



/** 
  * @brief  Modulation Type Configuration
  */
typedef enum
{
    AD9958_DISABLE = 0,
    AD9958_AMPLITUDE,
    AD9958_FREQUENCY,
    AD9958_PHASE,
}AFP_TypeDef;

/** 
  * @brief  Modulation Level Selection
  */
typedef enum
{
    TWO_LEVEL_MODULATION = 0x00,
    FOUR_LEVEL_MODULATION,
    EIGHT_LEVEL_MODULATION,
    SIXTEEN_LEVEL_MODULATION,
}ModulationLevel_TypeDef;

typedef struct DDSDataStruct
{
	u8  mode;      //���ģʽ
	u8  output;    //�������

	float range;   //����
	float phase;   //��λ 	
	u32 fre;       //��ͨģʽ��Ƶ��

	u32 fre_start; //ɨƵģʽ����ʼƵ��
	u32 fre_stop;	 //ɨƵģʽ�Ľ�ֹƵ��
	u32 step;      //����
	u32 step_time; //����ʱ��(us)

	u8 modulation_mode;       /* ���ƻ�ɨƵģʽ */
	u8 modulation_levels;			/* ���ֵ��ƽ��� */
	float ask[16]; 						/* ���ֵ���ģʽ��ASK��ϵ��ֵ */
	float fsk[16];  					/* ���ֵ���ģʽ��FSK��Ƶ��ֵ */
	float psk[16];  					/* ���ֵ���ģʽ��PSK����λֵ */
	
}DDSDataStruct; 


void sendData(u8 buff[],u16 length,u8 num);
void Refresh(DDSDataStruct data,u8 choose,u8 ch,u8 inphase);
void DDSDataInit(void);
void Signal_Source_Show(DDSDataStruct data,u8 choose);
void ESP8266_Client_Connect_Server(void);
void ESP8266_Server_Init(void);


extern DDSDataStruct dds[2];
extern communicationDataStruct communicationData; 

#endif



/******************************************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/











