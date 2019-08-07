/*
*********************************************************************************************************
*                                              _01_OS
* File			 : App_OSC.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : DS18B20���£�֧�ֶ�㣬���������һ����������룩
* warning 	 : ���ڶ�������ID��ַ�ǰ�ROM���Ⱥ�����ģ��������´�����ʵ�ְ��ֶ������˳������ͬ����ÿ�������������Ա���һ��������
*********************************************************************************************************
*/

#include "App_DS18B20.h"

void DS18B20_main(void)
{
	
   static short Temp; 
	 u8 i=0,j,k;			     
	 static u8 num,m=0,ID_Buff[MAXNUM][8]={0},TEMP_Buff[MAXNUM][8]={0}; 
	 
		while(1)
		{
			num = DS18B20_SearchROM(TEMP_Buff,MAXNUM);  
		  OS_Num_Show(100,100,24,1,num,"������ %.0f ��DS18B20"); 
		
			if(m<num)  //����
			{
				 for(i=0;i<=num;i++)
				 {
					  for(j=0;j<m;j++)
					     if(strcmp((char *)TEMP_Buff[i],(char *)ID_Buff[j])==0)
							    break;
						
						if(j==m)
						{
						   memcpy((char *)ID_Buff[m],(char *)TEMP_Buff[i],sizeof(TEMP_Buff[i]));
							 break;
						}
				 }
				 
         m=num;				 
			}
			else if(m>num) //�γ�
			{
				 for(i=0;i<m;i++)
				{
					   for(j=0;j<num;j++)
					     if(strcmp((char *)TEMP_Buff[j],(char *)ID_Buff[i])==0)
							    break;
							 
						if(j==num)
						{
						    for(k=i;k<m;k++)
							    memcpy((char *)ID_Buff[k],(char *)ID_Buff[k+1],sizeof(ID_Buff[k+1]));
							
							  m--;
								memset(ID_Buff[m],0,sizeof(ID_Buff[m]));
							  break;
						}  
				}			 
			}			
		
		 for(i=0;i<num;i++)
		 {
			   Temp = DS18B20_ReadDesignateTemper(ID_Buff[i]);			 
         OS_Num_Show(100,100+30*(i+1),24,1,Temp/100.0,"��  ��DS18B20�¶�Ϊ:%.2f");
         OS_Num_Show(130,100+30*(i+1),24,1,i+1,"%.0f");	
         delay_us(50);			 
		 }
		 	
		 memset(TEMP_Buff,0,sizeof(TEMP_Buff));
		 
			OSTimeDly(100);
		}
}


