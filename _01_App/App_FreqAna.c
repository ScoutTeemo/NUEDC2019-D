/*
*********************************************************************************************************
*                                              _04_OS
* File			 : App_FreqAna.c
* By  			 : Muhe
* platform   : STM32F407ZG
*	Data   		 : 2019/7/26
* function 	 : Ƶ�ʷ���
*********************************************************************************************************
*/
#include "App_FreqAna.h"

#include "log_table.inc"

#define MEASURE_LENGTH	200 			 //��Ƭ����ʾ��������
#define Get_Length      4650     	 //�ܲ����ص��� ((35M-500M)/0.1MHZ) 

GRAPH_Struct 	GridData;							//����ṹ�嶨��

float SignalData[Get_Length]= {0};   //�ɼ�������

float VMax_Fre;

void DDSDataInit(void);
u8 flag_relay = 0;
void FreqAna_main()
{
    GridData_Init();
	DDSDataInit();
    Draw_Grid(GridData);
    Draw_Graph(&GridData,LEFTY);

    while(1)
    {
        Draw_Grid(GridData);

        Show_Label(GridData,LEFTY);

        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
        {
            OS_LCD_Clear(WHITE);
            Draw_Grid(GridData);
            Draw_Graph(&GridData,LEFTY);
        }
		
		
		
		//AD9851_Sweep();
		
        OSTimeDly(111);
    }

}
/**
* @brief  ����ṹ�����ݳ�ʼ��
* @param
* @retval none
*/
void GridData_Init(void)
{

    /*	��������С		*/
    GridData.x1=40;
    GridData.y1=40;
    GridData.x2=760;
    GridData.y2=360;

    /*	��ʾ����		*/
    GridData.grid=1;

    /*	���ö�Ӧ������ɫ		*/
    GridData.griacolor=Black;			//����Ϊ��ɫ
    GridData.Backcolor=White;			//����Ϊ��ɫ
    GridData.left_ycolor=Red;			//��������Ӧ������Ϊ��ɫ

    /*	���ߡ���������		*/
    GridData.xnumber=11;
    GridData.ynumber=11;

    /*	�������ݳ��Ⱥͱ�������Y�����ݵ�ָ��		*/
    GridData.Buff_Lenth=MEASURE_LENGTH;
    GridData.left_buff=SignalData;

    /*	��������ַ�Χ		*/
    GridData.xmax=1000000;
    GridData.xmin=100;

    /*	�����������ַ�Χ		*/
    GridData.left_ymax=44;
    GridData.left_ymin=0;
}



/**
* @brief  AD9851ɨƵ����һ��
* @param
* @retval none
*/
int log_table_length = 101;// sizeof(log_table);
void AD9851_Sweep(void)
{
	static int count = 0;

	count++;
	if(count == log_table_length)
		count = 0;
	
	dds.fre = log_table[count];
	sendData(dds);
}


/*
 * Return:      void
 * Parameters:  void
 * Description: DDS���ݳ�ʼ��
 */
__inline void DDSDataInit(void)
{
    /*	������� 2v	*/
    dds.range=0.005;

    /*	���Ƶ��	100000Hz	*/
    dds.fre=100;

    /*	ɨƵ����	1000Hz	*/
    dds.step=1000;

    /*	ɨƵʱ��	1000us	*/
    dds.step_time=1000;

    /*	ɨƵ��ʼƵ��1000Hz		*/
    dds.fre_start=1000;

    /*	ɨƵ��ֹ200000	Hz	*/
    dds.fre_stop=200000;

    /*	Ĭ��Ϊ��ͨ���ģʽ		*/
    dds.mode=NORMAL;

    /*	Ĭ�ϲ������		*/
    dds.output=1;

}

void task_1_3(void)
{
	dds.fre=1000;
	sendData(dds);
	
	
	
	
}


