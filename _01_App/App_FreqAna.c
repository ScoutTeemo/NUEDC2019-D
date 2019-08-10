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
//#include "log_table.inc"
#include "log_table_201.inc"

char Fault_Type_str[][20]=
{
    "����      ",
    "----------",
    "R1��·    ",
    "R1��·    ",
    "R2��·    ",
    "R2��·    ",
    "R3��·    ",
    "R3��·    ",
    "R4��·    ",
    "R4��·    ",
    "C1��·    ",
    "C2��·    ",
    "C3��·    ",
    "C1����    ",
    "C2����    ",
    "C3����    ",
    "++++++++++",
    "=========="
};

#define KEY_TEST  1 //��������

#define CircuitGain 12.98f// 4.0f
#define INCircuitGain 11.24f//12.4552f//11.24f // 4.0f

#define ADS1256_MUX_IN   (ADS1256_MUXP_AIN5 | ADS1256_MUXN_AINCOM)
#define ADS1256_MUX_DC   (ADS1256_MUXP_AIN6 | ADS1256_MUXN_AINCOM)
#define ADS1256_MUX_OUT  (ADS1256_MUXP_AIN7 | ADS1256_MUXN_AINCOM)

#define ShowX1 10
#define ShowX2 180
#define ShowX3 360
#define ShowX4 540

//#define MEASURE_LENGTH	200 	//��Ƭ����ʾ��������
//#define Get_Length      201    //�ܲ����ص��� ((10^2-10^6)��������)
#define R_Real    6800.f//6788.0f       //�̶������С
#define R_OUT    3895.0f//7892.0f//3895.0f
#define ADS9851_V_BIG        0.210;//0.571f       //9851������� ����������õĴ��ѹ
#define ADS9851_V_BIG_REAL   0.500f       //9851������� ����������õĴ��ѹ
#define ADS9851_V_10MV  0.0115f       //9851������� ʵ�����10mv�ĵ�ѹ
#define ADS9851_V_SWEEP  0.020f       //9851������� ɨƵʱ�õķ���

GRAPH_Struct 	GridData;		//����ṹ�嶨��
const int log_table_length = sizeof(log_table)/sizeof(float);//101
float SignalData[log_table_length]= {0};   //�ɼ���ԭʼ����
float AvData[log_table_length]= {0};			//ת���ɶ���
float UpFreq;

float VMax_Fre, Rin, Rout, All_Gain, Vol_IN_Std, Vol_Out50k_Std;
int last_fault = Fault_Type_Normal;
u8 Fault_Change_Flag = 1;//�ϵ���һ��
u8 UpdateGragh = 0;

void DDSDataInit2(void);
void task_1_3(void);
Fault_Type Fault_Detect(void);
void mission0(void);
Fault_Type fault_Type;
u8 Interface_Num = 0;

void FreqAna_main()
{
    GridData_Init();

    DDSDataInit2();
    //sendData(dds);

    Draw_Grid(GridData);
    Show_Label(GridData,LEFTY);
    //Draw_Graph(&GridData,LEFTY);

    while(1)
    {

		if(Interface_Num == 0)
		{
			mission0();
			if(UpdateGragh)
			{
				Draw_Graph(&GridData,LEFTY);
				UpdateGragh = 0;
			}
		}
		else
		{
			LED1 = 0;
			fault_Type = Fault_Detect();
			LED1 = 1;
			OS_String_Show(ShowX3,390+16*3,16,1,Fault_Type_str[fault_Type]);
		}
		
        if(Key_Now_Get(KEY2,KEY_MODE_SHORT))
        {
            Interface_Num = !Interface_Num;
			if(Interface_Num==1)
			{
				LCD_Clear(BLACK);//����
			}
			else
			{
				Draw_Grid(GridData);
				Show_Label(GridData,LEFTY);
			}
        }


        OSTimeDly(53);//����ɾ��������ִ��ʱ����ʱ

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
    GridData.griacolor=White;			//����Ϊ��ɫ
    GridData.Backcolor=Black;			//����Ϊ��ɫ
    GridData.left_ycolor=Red;			//��������Ӧ������Ϊ��ɫ

    /*	���ߡ���������		*/
    GridData.xnumber=11;
    GridData.ynumber=11;

    /*	�������ݳ��Ⱥͱ�������Y�����ݵ�ָ��		*/
    GridData.Buff_Lenth = log_table_length;
    GridData.left_buff = AvData;

    /*	��������ַ�Χ		*/
    GridData.xmax=1000000;
    GridData.xmin=100;

    /*	�����������ַ�Χ		*/
    GridData.left_ymax=50;
    GridData.left_ymin=-5;
}



/**
* @brief  AD9851ɨƵ����һ��
* @param
* @retval none
*/
void AD9851_Sweep(void)
{
    u32 i;
    int j;
	int limint_j;
	float min,min_val=999999;
	
	float MaxFreqAv = 0;
    LED1 = 0;
    //������ʱ1ms��log_table_length��
    for(i=0; i<log_table_length; i++)
    {
		
        dds.fre= log_table[i];
        dds.range = ADS9851_V_SWEEP;
        sendData(dds);
		
        if(i<20)
		{
			//Relay_Control(Relay_631HSLS,Relay_OFF);
            delay_ms(100);
		}
        else
		{
			//Relay_Control(Relay_631HSLS,Relay_ON);
            delay_ms(40);
		}

		
		//��ҪJ5���� ����7k����
		Relay_Control(Relay_7K,Relay_OFF);
        SignalData[i] = Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));
		delay_ms(1);
        SignalData[i] += Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));
		SignalData[i] /= 2;
		
        AvData[i] = 20 * log10f(SignalData[i] / ((ADS9851_V_SWEEP-0.01f)/2.828f)) - 6.0f;//ƥ��˥��6db
		
		if(AvData[i] > MaxFreqAv)
		{
			MaxFreqAv = AvData[i];
		}
		//All_Gain = Vol_Out / ( Rin/(R_Real+Rin) * 0.01f / 2 / 1.414);   //����

        OS_Num_Show(ShowX3,390     ,16,1,SignalData[i],"***%0.3f   ");
        OS_Num_Show(ShowX3,390+16  ,16,1,AvData[i]    ,"###%0.3f   ");
        OS_Num_Show(ShowX3,390+16*2,16,1,i/2.0f       ,"+++%0.1f%% ");
        //OS_Num_Show(ShowX3,390+16*4,16,1,Vol_IN_Std   ,"INStd%0.3f   ");
		if(Key_Now_Get(KEY1,KEY_MODE_SHORT))
		{
			Interface_Num = 1;
			Relay_Control(Relay_7K,Relay_ON);
			LCD_Clear(BLACK);//����
			return ;
		}

    }
	for(j=10; j>=0; j--)
	{
		AvData[j] = (AvData[j] + AvData[j+1] + AvData[j+2] + AvData[j+3] + AvData[j+4] + AvData[j+5] + AvData[j+6]) / 7;
	}
	
	
	//UpFreq
	MaxFreqAv = AvData[26] -3;//AvData[26] = 1000
	for(j = 50; j < log_table_length; j++)
	{

		min = fabsf(AvData[j] - MaxFreqAv);
		if(min < min_val)
		{
			min_val = min;
			limint_j = j;
		}
		
	}
	UpFreq = log_table[limint_j];
	
	UpdateGragh = 1;
	Relay_Control(Relay_7K,Relay_ON);

    LED1 = 1;
}

#define RANGEIN(Vol,val,range)   (((Vol)>=((val) - (range))) && ((Vol)<=((val) + (range))))


float AD_ACNormal    = 0.162f;	//�����������
float AD_AC50k_C1C2D = 1.004f;	//50k 10mv C1C2���������
float AD_AC50k_C3O   = 0.155f;	//50k 10mv C3��·�����
float AD_AC50k_C3D   = 0.123f;    	//50k 100mv C3���������

//TODO: ��ʹ��15Hz�����жϣ����޸�
float AD_AC15_C1D    = 1.304f;			//15hz 1V C1���������
float AD_AC15_C2D    = 1.286f;			//15hz 1V C2���������

float AD_DC_C1C2O    = 7.6f/4.0f;  			//�Ƿ���C1 C2��·���

float AD_IN_C1O      = 2.000f; 		//���C1���뽻������ 
float AD_AC_C1O      = 0.0f; 		    //���C1����������� 

float AD_IN_C2O      = 0.053f;				// C2 ��·����� ���뽻��

float AD_AC_C2O      = 0.053f;		  // C2 ��·����� �������

float AD_AC_R4O    =  0.0f;           //R4 �Ͽ�����

float AD_DC_R2OU   = 6.0f/4.0f;    	//R2�� RSӦΪ0
float AD_DC_R2OD   = 3.0f/4.0f;    		//R2�� RSӦΪ0

float AD_DC_R1S     = 10.96f/4.0f;    	//R1�� RSӦΪ0
float AD_DC_R2S     = 11.48f/4.0f;    	//R2�� RSӦΪ0
float AD_DC_R4S     = 0.016f/4.0f;    	//R4�� RSӦΪ0
float AD_DC_R3O     = 0.2f/4.0f;    	//R3�� RSӦΪ0
float AD_DC_R3S     = 11.74f/4.0f;    //R3��

float AD_IN_R4O     = 1.213f;//0.025;
float AD_IN_R1O     = 1.365f;//0.028;//1.347f;
#define MeasureDelay 300

Fault_Type Fault_Detect(void)
{
    //��ǰһ�ֲ���ʱ��21ms ���Ӧ����80ms
    float Vol,VolDC,VolAC,VolIN,VolIN_BIG,Vol_Out50k;
	

    /***���1k 10MV�ź� **/
    dds.fre = 1000;
    dds.range = ADS9851_V_10MV;
    sendData(dds);
	
    delay_ms(MeasureDelay);
    //�������������ѹ
    VolAC =  Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));
    VolAC =  Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));

    //�������뽻����ѹ
	
    VolIN =  Get_Val(ADS1256ReadData(ADS1256_MUX_IN));
    VolIN =  Get_Val(ADS1256ReadData(ADS1256_MUX_IN));


	OS_Num_Show(ShowX4,390     ,16,1,VolAC,"VolAC:%0.3f   ");
	OS_Num_Show(ShowX4,390+16  ,16,1,VolIN,"VolIN:%0.3f   ");
	OS_Num_Show(ShowX4,390+16*2,16,1,AD_ACNormal,"AD_ACNormal:%0.3f   ");
	
	
    if(RANGEIN(VolAC,AD_ACNormal,0.05f)) //��������
    {
		
		dds.fre = 200000;
		dds.range = ADS9851_V_10MV;
		sendData(dds);
		delay_ms(MeasureDelay*4);
		Vol_Out50k =  Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));
		Vol_Out50k =  Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));
		OS_Num_Show(ShowX4,390+16*4,16,1,Vol_Out50k,"Vol_Out50k:%0.3f   ");
		
		
		//AD_AC50k_C1C2D = Vol_Out50k_Std;
        if(RANGEIN(Vol_Out50k,Vol_Out50k_Std,0.0080f)) //��������C1���� C2���� ����
        {

			dds.fre = 15;
			dds.range = 1.0f;
			sendData(dds);
			delay_ms(MeasureDelay);
			Vol = Get_Val(ADS1256ReadData(ADS1256_MUX_IN));
			Vol = Get_Val(ADS1256ReadData(ADS1256_MUX_IN));
			
			OS_Num_Show(ShowX4,390+16*3,16,1,Vol,"Vol15hz:%0.3f   ");
			
            if(RANGEIN(Vol,AD_AC15_C1D,0.005f))//C1����
            {
                return Fault_Type_C1Double;
            }
            else if(RANGEIN(Vol,AD_AC15_C2D,0.005f))//C2����
            {
                return Fault_Type_C2Double;
            }
            else //����
            {
                return Fault_Type_Normal;
            }

        }
        else
        {
		
            if(Vol_Out50k > Vol_Out50k_Std)//C3��·// (RANGEIN(Vol,AD_AC50k_C3O,0.008f))//C3��·
            {
                return Fault_Type_C3Open;
            }
			else if(Vol_Out50k < Vol_Out50k_Std)//(RANGEIN(Vol,AD_AC50k_C3D,0.008f))
			{
				return Fault_Type_C3Double;
			}
			else
			{
				return Fault_Type_Error1;
			}
			
        }

    }
    else    //����������
    {
        //�رս������ ���ֱ��
        dds.output=0;
        sendData(dds);
		dds.output=1;
		delay_ms(MeasureDelay);
        VolDC =  Get_Val(ADS1256ReadData(ADS1256_MUX_DC));
        VolDC =  Get_Val(ADS1256ReadData(ADS1256_MUX_DC));
		OS_Num_Show(ShowX4,390+16*4,16,1,VolDC,"VolDC:%0.3f   ");

        if(RANGEIN(VolDC,AD_DC_C1C2O,0.5f))//ֱ��ֵ�� ��� C1 C2 ��· ���н������
        {
			
			if(RANGEIN(VolIN,(ADS9851_V_10MV /2.828f *INCircuitGain),0.02f))
			{
				return Fault_Type_C1Open;
            }
			else// if(RANGEIN(Vol,AD_IN_C2O,0.005f))  //�Ƿ���C2��·
            {
				return Fault_Type_C2Open;
            }
		
        }
        else   //���е����ж�
        {
				dds.fre = 1000;
				dds.range = ADS9851_V_BIG;
				sendData(dds);
				delay_ms(MeasureDelay*2);
				VolIN_BIG =  Get_Val(ADS1256ReadData(ADS1256_MUX_IN));
				VolIN_BIG =  Get_Val(ADS1256ReadData(ADS1256_MUX_IN));
			
				OS_Num_Show(ShowX4,390+16*4,16,1,Vol,"Vol1V:%0.3f   ");
			
			
			if(VolIN_BIG > Vol_IN_Std)//���뽻�����ڱ�׼ֵ
            {

				
				//�����迹 �迹ֵֻ�����뽻�����
                if(RANGEIN(VolIN_BIG,AD_IN_R4O,0.05f))
                {
                    return Fault_Type_R4Open;
                }
                else if(RANGEIN(VolIN_BIG,AD_IN_R1O,0.05f))
                {
					return Fault_Type_R1Open;
                }
				else
				{
					return Fault_Type_Error2;
				}

            }
			else//С�ڱ�׼���� // if(RANGEIN(VolIN,0,0.003f))//���뽻������Ϊ0
			{
				//�������ֱ�� Ҳ��֮ǰ������
				if(VolDC > AD_DC_R2OD && VolDC < AD_DC_R2OU)
				{
					return Fault_Type_R2Open;
				}
				else if(RANGEIN(VolDC,AD_DC_R1S,0.03f))
				{
					return Fault_Type_R1Short;
				}
				else if(RANGEIN(VolDC,AD_DC_R2S,0.03f))
				{
					return Fault_Type_R2Short;
				}
				else if(RANGEIN(VolDC,AD_DC_R4S,0.03f))
				{
					return Fault_Type_R4Short;
				}
				else if(RANGEIN(VolDC,AD_DC_R3O,0.03f))
				{
					return Fault_Type_R3Open;
				}
				else if(RANGEIN(VolDC,AD_DC_R3S,0.03f))
                {
                    return Fault_Type_R3Short;
                }
				else
				{
					return Fault_Type_Error;
				}
            }
			
			

        }//���ֱ��

    }//����� �����������жϽ���

}


/*
 * Return:      void
 * Parameters:  void
 * Description: DDS���ݳ�ʼ��
 */
__inline void DDSDataInit2(void)
{
    /*	������� 15mv	*/
    dds.range=0.015;

    /*	���Ƶ��	100Hz	*/
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

    /*	Ĭ�ϴ����		*/
    dds.output=1;

}

/*
 * Return:      void
 * Parameters:  void
 * Description: �������롢������裬����
 */

u8 Save_Flag0=0,Save_Flag1=0;

float GetAve(u8 ADS1256_MUX_TYPE)
{
	float ave = 0;

	ave = Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  

	delay_ms(1);
	ave = Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	delay_ms(1);
	ave += Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	delay_ms(1);
	ave += Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	delay_ms(1);
	ave += Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	delay_ms(1);
	ave += Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	delay_ms(1);
	ave += Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	delay_ms(1);
	ave += Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	delay_ms(1);
	ave += Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	delay_ms(1);
	ave += Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	delay_ms(1);
	ave += Get_Val(ADS1256ReadData(ADS1256_MUX_TYPE));  
	
	return ave/10.0f;
	
}



void task_1_3(void)
{
    float Vol_in,Vol_in_7k,Vol_Out,Vol_Out_Load;

    LED1 = 0;
	//Relay_Control(Relay_631HSLS, Relay_ON);

#if KEY_TEST == 1
    OS_String_Show(ShowX2-16,390+16*2,16,1,"  ");
    OS_String_Show(ShowX2-16,390     ,16,1,"->");
    while(1)
    {
#endif
		Relay_Control(Relay_7K,Relay_OFF);//����7k�������迹
        dds.fre=1000;
        dds.range=ADS9851_V_BIG;		//���ѹ������
        sendData(dds);
        delay_ms(100);
		Vol_in = GetAve(ADS1256_MUX_IN);

        OS_Num_Show(180,390     ,16,1,Vol_in,"Vol_in:%0.3f   ");
	
	    if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
            break;
    }
	
    OS_String_Show(ShowX2-16,390     ,16,1,"  ");
    OS_String_Show(ShowX3-16,390     ,16,1,"->");
	 while(1)
    {
		Relay_Control(Relay_7K,Relay_ON);//����7k
        delay_ms(100);
		Vol_in_7k = GetAve(ADS1256_MUX_IN);
		
        OS_Num_Show(ShowX3,390     ,16,1,Vol_in_7k,"Vol_in_7K:%0.3f   ");
		
		if(Save_Flag0 == 0)
		{
			Vol_IN_Std = Vol_in_7k;
			Save_Flag0 = 1;
		}
		
		
		
#if KEY_TEST == 1
        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
            break;
    }
#endif


		if(Save_Flag1 == 0)
		{
			Save_Flag0 = 1;
			
			dds.fre = 200000;
			dds.range=ADS9851_V_10MV;//0.010VУ׼ֵ
			sendData(dds);
			delay_ms(MeasureDelay*4);
			Vol_Out50k_Std=Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));  //�����Ŵ��·����˵�ѹ
			Vol_Out50k_Std=Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));  //�����Ŵ��·����˵�ѹ
			
//			Vol_Out50k_Std = GetAve(ADS1256_MUX_OUT);
			
			OS_Num_Show(180,390+16*4,16,1,Vol_Out50k_Std,"Vol_Out50k_Std:%0.3f   ");

		}
		
#if KEY_TEST == 1
    OS_String_Show(ShowX3-16,390     ,16,1,"  ");
    //OS_String_Show(ShowX2-16,390     ,16,1,"  ");
    OS_String_Show(ShowX2-16,390+16  ,16,1,"->");
//    while(1)
//    {
#endif

		//1k С�ź� �����
		dds.fre = 1000;
		dds.range = ADS9851_V_10MV;//0.010mvУ׼ֵ
		dds.output = 1;
		sendData(dds);
		delay_ms(MeasureDelay);
//        Vol_Out=Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));  //�����Ŵ��·����˵�ѹ
//        Vol_Out=Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));  //�����Ŵ��·����˵�ѹ
		
		Vol_Out = GetAve(ADS1256_MUX_OUT);
		
		AD_ACNormal = Vol_Out;//0.164;
        OS_Num_Show(180,390+16  ,16,1,Vol_Out,"Vol_Out:%0.3f   ");
		
//		#if KEY_TEST == 1
//		while(1)
//		{
//		#endif
			//50k С�ź� ����� ��׼ֵ
			
			
//			
//		#if KEY_TEST == 1	
//        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
//            break;
//		}
//#endif



#if KEY_TEST == 1
//        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
//            break;
//    }
#endif

#if KEY_TEST == 1
    OS_String_Show(ShowX2-16,390+16  ,16,1,"  ");
    OS_String_Show(ShowX2-16,390+16*2,16,1,"->");
//    while(1)
//    {
#endif
		//1k С�ź� ���Ӹ���
		dds.fre = 1000;
		dds.range=ADS9851_V_10MV;
		dds.output=1;
		sendData(dds);
		
        Relay_Control(Relay_LOAD,Relay_ON);	//���Ӹ���
        delay_ms(MeasureDelay*2);
		
//		dds.output=1;
//		sendData(dds);
		
        //delay_ms(MeasureDelay*6);
        //delay_ms(MeasureDelay*6);
		
//        Vol_Out_Load=Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));  //�����Ŵ��·����˵�ѹ ��4k����
//        Vol_Out_Load=Get_Val(ADS1256ReadData(ADS1256_MUX_OUT));  //�����Ŵ��·����˵�ѹ ��4k����
		
		Vol_Out_Load=GetAve(ADS1256_MUX_OUT);
		
        OS_Num_Show(180,390+16*2,16,1,Vol_Out_Load,"Vol_Out_Load:%0.3f   ");
		Relay_Control(Relay_LOAD,Relay_OFF);	//�Ͽ�����

#if KEY_TEST == 1
//        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
//            break;
//    }
#endif

//	Relay_Control(Relay_631HSLS, Relay_OFF);

    //Rin=(R_Real * Vol_in/INCircuitGain ) / (ADS9851_V_BIG/2.828f - Vol_in/INCircuitGain );  //�������
	
	Rin=(R_Real * Vol_in_7k ) / (Vol_in - Vol_in_7k );  //�������
	

    Rout=(Vol_Out / Vol_Out_Load - 1.0f ) * R_OUT;   //�������4500.0f;//

    //Rin = 3500;
    All_Gain = Vol_Out / ( Rin/(R_Real+Rin) * 0.01f / 2.0f / 1.414f);   //����
	
    delay_ms(10);
    LED1 = 1;

}



void mission0(void)
{
	
	task_1_3();
	OS_Num_Show(ShowX1,390     ,16,1,Rin , "�������:%0.1f   ");
	OS_Num_Show(ShowX1,390+16  ,16,1,Rout, "�������:%0.1f   ");
	OS_Num_Show(ShowX1,390+16*2,16,1,All_Gain,"����:%0.1f    ");
	
//	AD9851_Sweep();
//	if(Interface_Num==0)
//		OS_Num_Show(ShowX1,390+16*3,16,1,UpFreq/1000.0f,"�Ͻ�ֹƵ��:%0.3fkhz   ");
}
