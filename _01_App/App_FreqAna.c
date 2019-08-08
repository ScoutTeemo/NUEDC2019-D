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

char Fault_Type_str[][10]=
{
    "����",
    "δ֪�쳣",
    "R1��·",
    "R1��·",
    "R2��·",
    "R2��·",
    "R3��·",
    "R3��·",
    "R4��·",
    "R4��·",
    "C1��·",
    "C2��·",
    "C3��·",
    "C1����",
    "C2����",
    "C3����"
};


#define UNKOWN_VAL0 1.2f
#define UNKOWN_VAL1 1.3f

float AD_ACNormal    = 1.13f;	//��������
float AD_AC50k_C1C2D = 1.004f;	//50k 100mv C1C2���������
float AD_AC50k_C3O   = 1.096f;	//50k 100mv C3��·�����
float AD_AC50k_C3D   = 0.0f;    	//50k 100mv C3���������
float AD_AC15_C1D    = UNKOWN_VAL0;			//15hz 1V C1��������� Ŀǰ��û��ֵ
float AD_AC15_C2D    = UNKOWN_VAL1;			//15hz 1V C2��������� Ŀǰ��û��ֵ
float AD_AC_C2O      = 0.053f;				// C2��·�����
float AD_AC_C1O_R    = 0.0f;    			//ʣ�µ���� C1��· ������� 1k0.1v����Ϊ0
float AD_DC_C1O      = 7.6f/4.0f;  			//C1��·�����������
float AD_AC_C1O      = 0.03536f; 		//C1��·�����
float AD_DC_R_FULL   = 11.98f/4.0f;    	//���������ֱ��������� ����R1�� R2�� R3�� R4��
float AD_RS_R1O      = 0.084f;    		//R1��
float AD_RS_R4O      = 0.074f;    		//R4��
float AD_RS_R3S      = 0.030f;    		//R3��
float AD_RS_R2S      = 0.000f;    		//R2��
float AD_DC_R1S      = 11.23f/4.0f;    	//R1�� RSӦΪ0
float AD_DC_R2O      = 4.19f/4.0f;    	//R2�� RSӦΪ0
float AD_DC_R3O      = 0.221f/4.0f;    	//R3�� RSӦΪ5mv
float AD_DC_R4S      = 0.135f/4.0f;    	//R4�� RSӦΪ0


//#define MEASURE_LENGTH	200 	//��Ƭ����ʾ��������
//#define Get_Length      201    //�ܲ����ص��� ((10^2-10^6)��������)
#define R_Real    5000.0f       //�̶������С
#define ADS9851_V   0.01f       //9851�������

GRAPH_Struct 	GridData;		//����ṹ�嶨��
const int log_table_length = sizeof(log_table)/sizeof(float);//101
float SignalData[log_table_length]= {0};   //�ɼ���ԭʼ����
float AvData[log_table_length]= {0};			//ת���ɶ���

float VMax_Fre,Rin,Rout,All_Gain;
int last_fault = Fault_Type_Normal;
u8 Fault_Change_Flag = 1;//�ϵ���һ��
u8 UpdateGragh = 0;

void DDSDataInit(void);
void task_1_3(void);
Fault_Type Fault_Detect(void);
float ADS1256_Measure(float fre, float range, u32 delay);

void FreqAna_main()
{
    GridData_Init();

    DDSDataInit();
    //sendData(dds);

    Draw_Grid(GridData);
    Draw_Graph(&GridData,LEFTY);

    while(1)
    {

//        if(Fault_Change_Flag)
//        {
            task_1_3();
            OS_Num_Show(10,390     ,16,1,Rin,"Rin:%0.3f   ");
            OS_Num_Show(10,390+16  ,16,1,Rout,"Rout:%0.3f   ");
            OS_Num_Show(10,390+16*2,16,1,All_Gain,"Gain:%0.3f   ");
//            Fault_Change_Flag = 0;
//        }

//			LED1 = 0;
//			Fault_Detect();
//			LED1 = 1;


//        Draw_Grid(GridData);
//        Show_Label(GridData,LEFTY);


//        if(UpdateGragh)
//        {
//            Draw_Graph(&GridData,LEFTY);
//            UpdateGragh = 0;
//        }




//        if(Key_Now_Get(KEY3,KEY_MODE_SHORT))
//        {
//            OS_LCD_Clear(WHITE);
//
//            Draw_Grid(GridData);
//
//            Draw_Graph(&GridData,LEFTY);
//
//        }



//        AD9851_Sweep();

        OSTimeDly(111);//����ɾ��������ִ��ʱ����ʱ

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
    GridData.Buff_Lenth = log_table_length;
    GridData.left_buff = AvData;

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
void AD9851_Sweep(void)
{
    u32 i;
    Fault_Type fault_Type;
    LED1 = 0;
    //������ʱ1ms��101�㣬һ��ѭ����ʱ600ms
    for(i=0; i<101; i++)
    {
        dds.fre= log_table[i];
        dds.range = ADS9851_V;
        sendData(dds);
        delay_ms(1);
        SignalData[i] = ADS1256ReadData(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM);
        AvData[i] = 20 * log10(SignalData[i] / ADS9851_V);

        if(i % 33)  //һ��ѭ��3��
        {
            fault_Type = Fault_Detect();
            if(last_fault != fault_Type)//���ϴ�״̬��һ�������²��������¹���������ʾ
            {
                Fault_Change_Flag = 1;
                last_fault = fault_Type;
                OS_String_Show(10,390+16*3,16,1,Fault_Type_str[fault_Type]);
                return ;//ʣ�µ�Ƶ����ʱ��ɨ�裬���Ȳ�����ʾ����
            }
            last_fault = fault_Type;

        }

        if(i==100)
            UpdateGragh = 1;

    }

    LED1 = 1;
}

#define RANGEIN(Vol,val,range)   (((Vol)>=((val) - (range))) && ((Vol)<=((val) + (range))))


//������������Ľ�����ѹ
__inline float ADS1256_Measure(float fre, float range, u32 delay)
{
    dds.fre = fre;
    dds.range = range;
    sendData(dds);
    delay_ms(delay);
    return Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM));
}


Fault_Type Fault_Detect(void)
{
    //��ǰһ�ֲ���ʱ��21ms ���Ӧ����80ms
    float Vol,VolDC,VolAC,VolRS;

    //RS�Ǳ�����������ǰ�ĵ��裬VolRS�ǵ����ĵ�ѹ

    Relay_Control(0,0);	//J3�̵����л��͵�ƽ �������������
    Relay_Control(1,0);	//J2�̵����л��͵�ƽ �޸���

    dds.fre = 1000;
    dds.range = 0.1;
    sendData(dds);
    delay_ms(1);
    VolRS = Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN3 | ADS1256_MUXN_AINCOM));

    //����AC
    Relay_Control(0,1);	//J3�̵����л��ߵ�ƽ ����������� AC
    Relay_Control(1,0);	//J2�̵����л��͵�ƽ �޸���
    VolAC =  ADS1256_Measure(1000, 0.1, 1);

    if(RANGEIN(VolAC,AD_ACNormal,0.05f)) //��������
    {
        Vol =  ADS1256_Measure(50000, 0.1, 1);

        if(RANGEIN(Vol,AD_AC50k_C1C2D,0.05f)) //��������C1���� C2����
        {
            Vol =  ADS1256_Measure(15, 1, 70);

            if(RANGEIN(Vol,AD_AC15_C1D,0.05f))//C1����
            {
                return Fault_Type_C1Double;
            }
            else if(RANGEIN(Vol,AD_AC15_C2D,0.05f))//C2����
            {
                return Fault_Type_C2Double;
            }
            else //����
            {
                return Fault_Type_Normal;
            }

        }
        else if(RANGEIN(Vol,AD_AC50k_C3O,0.05f))//C3��·
        {
            return Fault_Type_C3Open;
        }
        else if(RANGEIN(Vol,AD_AC50k_C3D,0.05f))//C3����
        {
            return Fault_Type_C3Double;
        }
        else
        {
            return Fault_Type_Error;
        }

    }
    else if(RANGEIN(VolAC,AD_AC_C2O,0.02f))//C2��·
    {
        return Fault_Type_C2Open;
    }
    else if(RANGEIN(VolAC,AD_AC_C1O_R,0.02f))//����Ϊ0
    {

        dds.output = 0;
        sendData(dds);
        dds.output = 1; 			 //9851�´ο���DDSʱ���ز�����Ч
        delay_ms(10);
        //�ر�ADC ����ֱ��
        VolDC = Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN0 | ADS1256_MUXN_AINCOM));

        if(RANGEIN(VolDC,AD_DC_C1O,0.02f)) //ֱ����7.6V
        {
            if(RANGEIN(VolAC,AD_AC_C1O,0.02f))
            {
                return Fault_Type_C1Open;
            }
            else
            {
                return Fault_Type_Error;
            }

        }
        else //�ۺϷ����������
        {
            //�����Ѿ���������1k100mv��ֱ���ͽ���ֵ
            if(RANGEIN(VolDC,AD_DC_R_FULL,0.02f))//ֱ����11.9V����
            {
                //ͨ���ж� VolRS
                if(RANGEIN(VolRS,AD_RS_R1O,0.004f))
                {
                    return Fault_Type_R1Open;
                }
                else if(RANGEIN(VolRS,AD_RS_R4O,0.004f))
                {
                    return Fault_Type_R1Short;
                }
                else if(RANGEIN(VolRS,AD_RS_R3S,0.004f))
                {
                    return Fault_Type_R3Short;
                }
                else if(RANGEIN(VolRS,AD_RS_R2S,0.002f))
                {
                    return Fault_Type_R2Short;
                }
                else //�ֵֹ�ֵ
                {
                    return Fault_Type_Error;
                }

            }
            else if(RANGEIN(VolDC,AD_DC_R1S,0.05f)) //����˵RS֮��ĵ�ѹΪ0
            {
                return Fault_Type_R1Short;
            }
            else if(RANGEIN(VolDC,AD_DC_R2O,0.05f)) //����˵RS֮��ĵ�ѹΪ0
            {
                return Fault_Type_R2Open;
            }
            else if(RANGEIN(VolDC,AD_DC_R3O,0.05f)) //����˵RS֮��ĵ�ѹΪ5mv
            {
                return Fault_Type_R3Open;
            }
            else if(RANGEIN(VolDC,AD_DC_R4S,0.05f)) //����˵RS֮��ĵ�ѹΪ0
            {
                return Fault_Type_R4Short;
            }
            else
            {
                return Fault_Type_Error;
            }

        } //�ۺϷ����������


    }//����㲢��if

    return Fault_Type_Error;

}


void AD9851_Sweep_once(int control, u32 index)
{
    static u32 i;

    if(control)
        i = index;

    dds.fre= log_table[i];
    sendData(dds);
    delay_ms(1);
    SignalData[i] = ADS1256ReadData(ADS1256_MUXP_AIN1 | ADS1256_MUXN_AINCOM);

}


/*
 * Return:      void
 * Parameters:  void
 * Description: DDS���ݳ�ʼ��
 */
__inline void DDSDataInit(void)
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
 * Description: У׼�����ܲ���
 */
void Calib_Audion()
{

}
/*
 * Return:      void
 * Parameters:  void
 * Description: �������롢������裬����
 */
void task_1_3(void)
{
    float Vol0,Vol1,Vol2;

    LED1 = 0;
    //��ǰ����һ��44ms
    dds.fre=1000;

    dds.range=0.01;

    sendData(dds);


    Relay_Control(1,0);	//J3�̵����л��ߵ�ƽ
    delay_ms(100);
    Vol0=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //������׼��������˵�ѹ
    delay_ms(100);
    Vol0=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //������׼��������˵�ѹ
    OS_Num_Show(180,370     ,16,1,Vol0,"Vol0:%0.3f   ");

//314mv
//1.68V

    Relay_Control(1,1);	//J3�̵����л��͵�ƽ
    Relay_Control(4,0);	//J2�̵����л��͵�ƽ  �Ȳ��޸���
    delay_ms(100);
    Vol1=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //�����Ŵ��·����˵�ѹ
    delay_ms(100);
    Vol1=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //�����Ŵ��·����˵�ѹ
    OS_Num_Show(180,370+16  ,16,1,Vol1,"Vol1:%0.3f   ");
    //���ù�J4
    //Relay_Control(4,1);	//J2�̵����л��ߵ�ƽ �����4k����



    Relay_Control(3,0);	//J2�̵����л��ߵ�ƽ �����4k����
    delay_ms(100);
    Vol2=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //�����Ŵ��·����˵�ѹ
    delay_ms(100);
    Vol2=Get_Val(ADS1256ReadData(ADS1256_MUXP_AIN1|ADS1256_MUXN_AINCOM));  //�����Ŵ��·����˵�ѹ
    OS_Num_Show(180,370+16*2,16,1,Vol2,"Vol2:%0.3f   ");



    All_Gain=Vol1/Vol0;     //����

    Rin=(R_Real * Vol0 ) / (ADS9851_V - Vol0 );  //�������

    Rout=(Vol1 / Vol2 - 1.0f )* 4000 ;   //�������




    GPIO_SetBits(GPIOG, GPIO_Pin_1 | GPIO_Pin_3| GPIO_Pin_5| GPIO_Pin_7);
    delay_ms(10);
    LED1 = 1;

}

void SelfCalibration(void)
{

}


