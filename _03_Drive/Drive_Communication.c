#include "Drive_Communication.h"


/*ͨ�����ݽṹ��*/
communicationDataStruct communicationData = {{0},{0}};

//dds�ṹ�������ʼ��
DDSDataStruct dds;

/**
* @brief  ��ȡCRCУ����
* @param  none
* @retval CRCУ����
*/
static u16 crc_16(u8 *Datas, u16 Length)
{
    u8 j;
    u16 temp = 0xFFFF, i;

    for(i=0; i<Length; ++i)
    {
        temp ^= Datas[i];

        for(j=0; j<8; ++j)
        {
            if(temp & 1)
            {
                temp >>= 1;
                temp ^= 0xA001;
            }
            else
            {
                temp >>= 1;
            }
        }
    }

    return temp;
}

/**
 * @brief  UART4��������
 * @param  *sendData : �����͵�����
           length    : ���ݳ���
 * @retval ��
 */
static void usartSendData(u8 *sendData,u16 length)
{
    u16 i;

    for(i=0 ; i<length ; i++)
    {
        USART_SendData(UART4,sendData[i]);

        while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
    }
}

/**
* @brief  ���Ͳ�������
* @param  data :DDS�ṹ�����
* @retval none
*/
void sendData(DDSDataStruct data)
{

    u16 crcValue;
    u32 range=(u32)(data.range*1000);

    communicationData.txBuffer[0]  = 0xAA;												//֡ͷ
    communicationData.txBuffer[1]  = 0xA5;

    communicationData.txBuffer[2]  = (u8) data.mode >>8;     		 	//ģʽ
    communicationData.txBuffer[3]  = (u8)(data.mode &0xff);

    communicationData.txBuffer[4]	 = (u8)(data.fre>>24);       		//Ƶ��
    communicationData.txBuffer[5]  = (u8)((data.fre>>16) & 0xff);
    communicationData.txBuffer[6]  = (u8)((data.fre>>8) & 0xff);
    communicationData.txBuffer[7]  = (u8)(data.fre & 0xff);

    communicationData.txBuffer[8]  = (u8)(range>>24);  						//����
    communicationData.txBuffer[9]  = (u8)((range>>16) & 0xff);
    communicationData.txBuffer[10] = (u8)((range>>8) & 0xff);
    communicationData.txBuffer[11] = (u8)(range & 0xff);

    communicationData.txBuffer[12] = data.output;  								//�������

    communicationData.txBuffer[13] = (u8)(data.step>>24); 	 	 						//����
    communicationData.txBuffer[14] = (u8)((data.step>>16) & 0xff);
    communicationData.txBuffer[15] = (u8)((data.step>>8) & 0xff);
    communicationData.txBuffer[16] = (u8)(data.step & 0xff);

    communicationData.txBuffer[17] = (u8)(data.step_time>>24);						//����ʱ��
    communicationData.txBuffer[18] = (u8)((data.step_time>>16) & 0xff);
    communicationData.txBuffer[19] = (u8)((data.step_time>>8) & 0xff);
    communicationData.txBuffer[20] = (u8)(data.step_time & 0xff);

    communicationData.txBuffer[21]  = (u8)(data.fre_start>>24); 						//��ʼƵ��
    communicationData.txBuffer[22]  = (u8)((data.fre_start>>16) & 0xff);
    communicationData.txBuffer[23]  = (u8)((data.fre_start>>8) & 0xff);
    communicationData.txBuffer[24]  = (u8)(data.fre_start & 0xff);

    communicationData.txBuffer[25]  = (u8)(data.fre_stop>>24);  						//��ֹƵ��
    communicationData.txBuffer[26]  = (u8)((data.fre_stop>>16) & 0xff);
    communicationData.txBuffer[27] = (u8)((data.fre_stop>>8) & 0xff);
    communicationData.txBuffer[28] = (u8)(data.fre_stop & 0xff);

    /*����CRCУ����*/
    crcValue = crc_16(&communicationData.txBuffer[0],TX_LENGTH - 3);

    communicationData.txBuffer[TX_LENGTH-3] = crcValue>>8;
    communicationData.txBuffer[TX_LENGTH-2] = crcValue & 0xff;

    communicationData.txBuffer[TX_LENGTH-1] = 0x55;							//֡β

    usartSendData(communicationData.txBuffer,TX_LENGTH);

}


///**
//* @brief  �������ݴ���
//* @param  none
//* @retval none
//*/
//void rxDataprocess(void)
//{
//
//}


/**
* @brief  ˢ�½���
* @param  data :DDS�ṹ�����
					choose :ѡ���
* @retval none
*/
void Refresh(DDSDataStruct data,u8 choose)
{

    LCD_Clear(White);
    OS_String_Show(30,30,32,0,"Mode:  Normal  Sweep");
    OS_Rect_Draw(130,25,250,70,1,Black);
    OS_Rect_Draw(260,25,370,70,1,Black);

    if(data.mode == NORMAL)
        OS_Rect_Draw(130,25,250,70,1,Red);
    else if(data.mode == SWEEP)
        OS_Rect_Draw(260,25,370,70,1,Red);

    OS_Num_Show(30,90,32,1,data.range,"Val:   %.3fV");
    OS_Rect_Draw(130,85,290,130,1,Black);

    OS_Num_Show(30,150,32,1,data.fre,"Fre:   %.0f");
    OS_Rect_Draw(130,145,290,190,1,Black);

    OS_Num_Show(30,210,32,1,data.step,"Step:  %.0f");
    OS_Rect_Draw(130,205,290,250,1,Black);

    OS_Num_Show(30,270,32,1,data.step_time,"Step_time: %.0fus");
    OS_Rect_Draw(190,265,350,310,1,Black);

    OS_Num_Show(30,330,32,1,data.fre_start,"Start_Fre: %.0f");
    OS_Rect_Draw(190,325,350,370,1,Black);

    OS_Num_Show(30,390,32,1,data.fre_stop,"Stop_Fre:  %.0f");
    OS_Rect_Draw(190,385,350,430,1,Black);

    OS_String_Show(600,390,32,0,"OutPut");
    if(data.output)
        OS_Rect_Draw(575,375,720,440,1,Red);
    else
        OS_Rect_Draw(575,375,720,440,1,Black);

    switch(choose)
    {
    case 1:
        OS_Rect_Draw(130,85,290,130,1,Red);
        break;
    case 2:
        OS_Rect_Draw(130,145,290,190,1,Red);
        break;
    case 3:
        OS_Rect_Draw(130,205,290,250,1,Red);
        break;
    case 4:
        OS_Rect_Draw(190,265,350,310,1,Red);
        break;
    case 5:
        OS_Rect_Draw(190,325,350,370,1,Red);
        break;
    case 6:
        OS_Rect_Draw(190,385,350,430,1,Red);
        break;
    default :
        break;
    }

    TouchKey_Draw();//��С����
}



