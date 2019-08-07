#ifndef __APP_TOUCH_H
#define __APP_TOUCH_H 

#include "User_header.h"

/* ȫ�ֺ궨�� ----------------------------------------------------------------*/
#define TOUCH_FREE 	    		0	
#define TOUCH_PRESS 			1	
 
#define TOUCH_INVALID  	    	0	
#define TOUCH_VALID				1
#define TOUCH_VALID_FULL		2



/* �ṹ������ ----------------------------------------------------------------*/ 
typedef struct __Touch
{
	u8 state;
	u16 NowX;
	u16 NowY;
	u16 StartX;
	u16 StartY;
	u16 EndX;
	u16 EndY;
	
}Type_Touch;

/* ȫ�ֱ������� --------------------------------------------------------------*/ 
extern Type_Touch TouchStruct;

/* ȫ�ֺ������� --------------------------------------------------------------*/  				
u8 Touch_Judge(u16 x1,u16 y1,u16 x2,u16 y2);
u8 TouchStart_Judge(u16 x1,u16 y1,u16 x2,u16 y2);
void Touch_Clear(void);
void Touch_main(void);
#endif

