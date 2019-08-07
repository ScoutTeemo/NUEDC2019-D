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

#define    RX_LENGTH              48
#define    TX_LENGTH              48

typedef struct communicationDataStruct
{
    u8 rxBuffer[RX_LENGTH];
    u8 txBuffer[TX_LENGTH];
    u8 flag;					  //������λ
} communicationDataStruct;

typedef struct DDSDataStruct
{
    u8  mode;      //���ģʽ
    u8  output;    //�������

    float range;   //����
    u32 fre;       //��ͨģʽ��Ƶ��

    u32 step;      //����
    u32 step_time; //����ʱ��(us)
    u32 fre_start; //ɨƵģʽ����ʼƵ��
    u32 fre_stop;	 //ɨƵģʽ�Ľ�ֹƵ��

} DDSDataStruct;


void sendData(DDSDataStruct data);
void Refresh(DDSDataStruct data,u8 choose);

extern DDSDataStruct dds;
extern communicationDataStruct communicationData;

#endif



/******************************************(C)  COPYRIGHT 2017 ��Զ�� *********************************************************/











