#ifndef __DRIVE_CDCE937_H
#define __DRIVE_CDCE937_H

#include "User_header.h"


//CDCE937��Ϊ���豸ʱ�ĵ�ַ
#define CDCE937_ADDRESS     	0XDA	


void CDCE937_Init(void);
void CDCE937_Start(void);
void CECE937_WriteRegister(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite);


#endif 

