#ifndef __DRIVER_RELAY_H
#define __DRIVER_RELAY_H 

#include "User_header.h"

typedef enum {
	Relay_ON  = 1,
	Relay_OFF = 0
} Relay_State;

typedef enum {
	Relay_631HSLS   = 1,//G1 J4 ��0���� ��1����
	Relay_MESS_0    = 2,//G3 J2 ��ʱû�� һֱ��
	Relay_9851      = 3,//G5 J1 9851��˥�� ��ʱ���� ��
	Relay_LOAD      = 4,//G7 J3 4k���صĿ��� 0�ز��� 1��
	Relay_7K        = 5,//G9 J5 0���� 1��
	Relay_ALL       = 6
} Relay_Select;


void Relay_Init(void);
void Relay_Control(Relay_Select Relay_Num, Relay_State relay_state);


#endif

