#include "User_header.h"

#ifndef DS18B20_H_  
#define DS18B20_H_

/* ͷ�ļ����� ----------------------------------------------------------------*/  
#include "User_header.h"

/* ȫ�ֺ궨�� ----------------------------------------------------------------*/

#define MAXNUM 4    //���������ϵ�DS18B20��������MAXNUM

/* �ṹ������ ----------------------------------------------------------------*/

/* ȫ�ֱ������� --------------------------------------------------------------*/ 
 
/* ȫ�ֺ������� --------------------------------------------------------------*/ 

//DS18B20ָ��
typedef enum   
{  
    SEARCH_ROM          =   0xf0,   //����ROMָ��  
    READ_ROM            =   0x33,   //��ȡROMָ��
    MATH_ROM            =   0x55,   //ƥ��ROMָ��
    SKIP_ROM            =   0xcc,   //����ROMָ��
    ALARM_SEARCH        =   0xec,   //��������ָ�� 
    CONVERT_T           =   0x44,   //�¶�ת��ָ��
    WRITE_SCRATCHPAD    =   0x4e,   //д�ݴ���ָ��
    READ_SCRATCHPAD     =   0xbe,   //��ȡת����ָ��
    COPY_SCRATCHPAD     =   0x48,   //�����ݴ���ָ��  
    RECALL_E2           =   0xb8,   //�ٻ�EEPROMָ��
    READ_POWER_SUPPLY   =   0xb4,   //��ȡ��Դģʽָ��  
	
} DS18B20_CMD;  
  
  
//DS18B20 ROM���� 
typedef struct  
{  
    u8  DS18B20_CODE;   //DS18B20�����߱���:0x19  
    u8  SN_1;           //���кŵ�1�ֽ�  
    u8  SN_2;           //���кŵ�2�ֽ�
    u8  SN_3;           //���кŵ�3�ֽ�
    u8  SN_4;           //���кŵ�4�ֽ�  
    u8  SN_5;           //���кŵ�5�ֽ� 
    u8  SN_6;           //���кŵ�6�ֽ�
    u8  crc8;           //CRC8У��    
	
} DS18B20_ROM_CODE;  


  
#define TRUE 0xff
#define FALSE 0X00

//IO��������
#define Set18b20IOin()  {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=0<<9*2;}
#define Set18b20IOout() {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=1<<9*2;}

////IO��������											   
#define	DS18B20_OUT PGout(9) //���ݶ˿�	PG9
#define	DS18B20_IN  PGin(9)  //���ݶ˿�	PG9

//����ʹ������

int DS18B20_Reset(void);
u8 DS18B20_Init(void);
u8 DS18B20_ReadBit(void);
u8 DS18B20_ReadData(void) ;
void DS18B20_WriteBit(u8 bit);
void DS18B20_WriteData(u8 data);
s16 DS18B20_ReadTemper(void);
u8 DS18B20_Read2Bit(void);
u8 DS18B20_SearchROM(u8 (*pID)[8],u8 Num);
s16 DS18B20_ReadDesignateTemper(u8 pID[8]);

#endif 

   	
















