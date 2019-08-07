/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_ADPCM.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : ����ѹ��IAP
*********************************************************************************************************
*/
#include "Drive_ADPCM.h"


/*ADPCM ���벽��������*/
const signed char index_adjust[8] =
				{-1,-1,-1,-1,2,4,6,8};

/**/
const u16 step_table[89] = 
{
 7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,34,37,41,45,
 50,55,60,66,73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,337,371,
 408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,1707,1878,2047,
    
 2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,5894,6484,7132,7845,8630,9493,
 10442,11487,12635,13899,15289,16818,18500,20350,22385,24623,27086,29794,32767
};


/*
*************************************************************************
*
*	����	��	ADPCM����
*	����	��	1������������
						2�����������
						3�����ݳ���
*
**************************************************************************
*/
void ADPCM_Encode(volatile u16 *Data_in,volatile u8 *Data_out,u16 size)
{
	static int16_t old_sample=0;
	static signed char index=0;
	
	int16_t sample,delta;
	
	u8 flag,code;
	
	u8 i;
	
	size/=2;
	
	while(size--)
	{
	
			for(i=0;i<2;i++)
			{
				sample=*Data_in++;				
				
				delta=sample-old_sample;		//��ֵ
				
				if(delta<0)
				{
					delta=-delta;
					flag=8;
				}
				else
					flag=0;
				
				code= 4 * delta / step_table[index];	//���õ�����
				
				if(code > 7)
				{
					code=7;
				}
				index +=index_adjust[code];	 // ��������ǿ�ȵ����´�steptable ����� ����Ӧ����
				
				if(index<0)
					 index=0;
				else if(index>59)
					 index=59;
				
				old_sample=sample;//��¼��ǰ����ֵ
				
				*Data_out<<=4;	
				
				*Data_out |= (code|flag);	//���뱣���ں���λ
		
			}
			
			Data_out++;
			
	}
	
	
	
}
/*
*************************************************************************
*
*	����	��	ADPCM����
*	����	��	1������������
						2�����������
						3�����ݳ���
*
**************************************************************************
*/
void ADPCM_Decode(volatile u8 *Data_in,volatile u16 *Data_out,u16 size)
{
		static signed char index=0;
	  static int16_t cur_sample=0;
	
		int16_t delta;
		signed char flag,tem;
		u8 code=0,i;
	
		size/=2;
	
		while(size--)
		{
				code=tem=*Data_in++;
				
				for(i=0;i<2;i++)
				{
					 code=(code & 0x0f);	// ȡ����������
					 
						if((code & 8) != 0)	//�ж����ݷ���
								flag=1;
						else 
								flag=0;
						
						code &= 7;				 // ȥ������λ
						
						delta = (step_table[index]*code)/4+step_table[index]/8;     // �����ѹ��ֵ
						
						if(flag == 1)
								delta=-delta;
						
						cur_sample+=delta;		//������ǰ��ѹֵ
						
						if(cur_sample > 2047)
								cur_sample=2047;
						else if(cur_sample < -2047)
								cur_sample=(-2047);
						
						*Data_out++=(cur_sample+2047)&0xfff;
						
						index += index_adjust[code];
						
						if(index < 0)
								index=0;
						else if(index > 59)
								index=59;
						
						code =(tem>>4);
			}
		}
	
}


