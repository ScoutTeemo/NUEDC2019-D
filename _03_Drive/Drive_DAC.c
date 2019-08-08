/*
*********************************************************************************************************
*                                              _04_OS
* File			 : Drive_DAC.c
* By  			 :
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
* function 	 : DAC���ó���
*********************************************************************************************************
*/
#include "Drive_DAC.h"


/* ˽�к궨�� ----------------------------------------------------------------*/
#define DAC_ADDRESS 0x40007414
/* ˽�У���̬���������� ------------------------------------------------------*/
const uint16_t aSine12bit[4] = {2772,3796,1323,299};
/* ȫ�ֱ������� --------------------------------------------------------------*/
ddsStruct ddsStructData;
/* ȫ�ֺ�����д --------------------------------------------------------------*/

/**
 * @brief  ����sin��
 * @param  ��
 * @retval ��
 */
void Dac1_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

    DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
    DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
    DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1

    DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DACͨ��1

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}

void Dac1_Set_Vol(u16 vol)
{
    double temp=vol;
//	Dac1_Init();		 		//DACͨ��1��ʼ��
    temp/=1000;
    temp=temp*4096/3.3;
    DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}

void Dac2_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

    DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
    DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
    DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��1

    DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DACͨ��1

    DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}

void Dac2_Set_Vol(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
    DAC_SetChannel2Data(DAC_Align_12b_R,vol);//12λ�Ҷ������ݸ�ʽ����DACֵ
}

static double ddsSinWave(uint32_t t, uint32_t T,double cycle)
{

    t %= T;

    return sin(2 * 3.14159265358979 * t / T);
}

static double ddsTriangleWave(uint32_t t,uint32_t T,double cycle)
{
    t %= T;

    if(t < (T/4) )
    {
        return 4.0f * t /T ;
    }
    else if(t < (T*3/4))
    {
        return 2.0f - 4.0f * t /T;
    }
    else
    {
        return 4.0f * t/T - 4.0f;
    }
}

static double ddsSawtoothWave(uint32_t t,uint32_t T,double cycle)
{
    t %= T;

    if(t <(T /2))
    {
        return 2.0 * t/T;
    }
    else
    {
        return 2.0f * t /T - 2.0f;
    }
}

static double ddsSquareWave(uint32_t t,uint32_t T,double cycle)
{

    t %= T;

    if(t < (T*(cycle/100.0f)) )
    {
        return 1.0f;
    }
    else
    {
        return -1.0f;
    }

}

/**
 * @brief  dds�ṹ���ʼ��
 * @param  ��
 * @retval ��
 */
static void ddsDataInit(void)
{
    uint16_t i;


    for(i=0 ; i<1000 ; i++)
    {
        ddsStructData.data[i] = 0;
    }

    ddsStructData.wave = SINWAVE;
    ddsStructData.hz = 1000;
    ddsStructData.vpp = 2.0;
    ddsStructData.dutycycle = 50;
    ddsStructData.createWaveData = ddsSinWave;

    // setDDS(2,1000,50,SINWAVE);
}

/**
 * @brief   dacDMA ��ʼ��
 * @param  ��
 * @retval ��
 */
void dacInit(void)
{
    static uint8_t firstInitFlag = False;

    GPIO_InitTypeDef GPIO_InitStruct;
    DMA_InitTypeDef  DMA_InitStruct;
    DAC_InitTypeDef  DAC_InitStruct;

    //ʹ����Ӧʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

    //����IO��
    GPIO_InitStruct.GPIO_Mode   =  GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd   =  GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Pin    =  GPIO_Pin_4|GPIO_Pin_5;
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    //����DAC1
    DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude   = DAC_LFSRUnmask_Bit0;
    DAC_InitStruct.DAC_OutputBuffer   = DAC_OutputBuffer_Disable;
    DAC_InitStruct.DAC_Trigger     =  DAC_Trigger_T6_TRGO;
    DAC_InitStruct.DAC_WaveGeneration    = DAC_WaveGeneration_None;
    DAC_Init(DAC_Channel_1,&DAC_InitStruct);


    DMA_DeInit(DMA1_Stream5);

    DMA_InitStruct.DMA_Mode                  = DMA_Mode_Circular;   //ѭ��ģʽ
    DMA_InitStruct.DMA_Channel               = DMA_Channel_7;
    DMA_InitStruct.DMA_BufferSize            = ddsStructData.length;
    DMA_InitStruct.DMA_DIR                   = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct.DMA_FIFOMode              = DMA_FIFOMode_Disable;   //��ֹFIFO
    DMA_InitStruct.DMA_FIFOThreshold         = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStruct.DMA_PeripheralBaseAddr    = (u32)&(DAC->DHR12R1);
    DMA_InitStruct.DMA_Memory0BaseAddr       = (u32)ddsStructData.data;
    DMA_InitStruct.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;
    DMA_InitStruct.DMA_MemoryBurst           = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_Priority              = DMA_Priority_High;
    DMA_Init(DMA1_Stream5,&DMA_InitStruct);

    DMA_Cmd(DMA1_Stream5,ENABLE);

    DAC_DMACmd(DAC_Channel_1,ENABLE);

    DAC_Cmd(DAC_Channel_1,ENABLE);

    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);

    if(firstInitFlag == False)
    {
        ddsDataInit();
        firstInitFlag = True;
    }

}


/**
 * @brief   �ر�dac���
 * @param  ��
 * @retval ��
 */
void dacClose(void)
{
    DAC_Cmd(DAC_Channel_1,DISABLE);
    TIM_Cmd(TIM6,DISABLE);
}


/**
 * @brief   �ر�dac���
 * @param  ��
 * @retval ��
 */
void dacOpen(void)
{
    DAC_Cmd(DAC_Channel_1,ENABLE);
    TIM_Cmd(TIM6,ENABLE);
}


/**
 * @brief  �ı�dds���
 * @param  ��
 * @retval ��
 */
void setDDS(double vpp,uint32_t fre,double dutyCycle,waveEnum wave)
{

    uint16_t i;
    float temp;

    static uint16_t preLength = 0;
    TIM_Cmd(TIM6,DISABLE);
    for(i=0 ; i<1000 ; i++)
    {
        ddsStructData.data[i] = 0;
    }

    if(fre < 500)
    {
        ddsStructData.length = 512;
    }
    else if(fre < 5000)
    {
        ddsStructData.length = 128;
    }
    else if(fre < 50000)
    {
        ddsStructData.length = 64;
    }
    else if(fre < 100000)
    {
        ddsStructData.length  = 32;
    }
    else if(fre < 200000)
    {
        ddsStructData.length  = 32;
    }
    else if(fre >= 200000)
    {
        ddsStructData.length  = 32;
    }
    else
    {
        ddsStructData.length  = 512;
    }

    if(preLength != ddsStructData.length)
    {
        preLength = ddsStructData.length;
        dacInit();
    }



    if(wave == SINWAVE)
    {
        ddsStructData.createWaveData = ddsSinWave;
    }
    else if(wave == TRIANGLEWAE)
    {
        ddsStructData.createWaveData = ddsTriangleWave;
    }
    else if(wave == SAWTOOTHWAVE)
    {
        ddsStructData.createWaveData = ddsSawtoothWave;
    }
    else if(wave == SQUAREWAVE)
    {
        ddsStructData.createWaveData = ddsSquareWave;
    }


    ddsStructData.wave = wave;

    ddsStructData.vpp = vpp;

    ddsStructData.hz = fre /= 2.0;

    ddsStructData.dutycycle = dutyCycle;

    for(i=0 ; i<ddsStructData.length ; i++)
    {
        temp = ((ddsStructData.createWaveData(i,ddsStructData.length,ddsStructData.dutycycle) + 1.0f)/2.0f);
        ddsStructData.data[i] =  (uint16_t)(temp * (ddsStructData.vpp/3.3f) * 4095.0f);
    }

    timer6Init(ddsStructData.hz * ddsStructData.length);

    ddsStructData.hz *= 2;

    TIM_Cmd(TIM6,ENABLE);

}
/**
 * @brief  ���ڴ���DAC���
 * @param  ��
 * @retval ��
 */
void timer6Init(uint32_t hz)
{

    uint32_t period = 84000000/hz;


    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

    TIM_TimeBaseInitStruct.TIM_Period = period + 1;

    TIM_TimeBaseInitStruct.TIM_Prescaler = 0;

    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);

    TIM_SelectOutputTrigger(TIM6,TIM_TRGOSource_Update);

    TIM_Cmd(TIM6,ENABLE);

}


/**----------------------------------------------------------------------------
* @FunctionName  : DAC1_Init()
* @Description   : None
* @Data          : 2016/7/11
* @Explain       : None
------------------------------------------------------------------------------*/
void DAC1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//��ʹ�� PA ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//��ʹ�� DAC ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//�ٳ�ʼ�� GPIO
    DAC_InitType.DAC_Trigger=DAC_Trigger_None; //��ʹ�ô������� TEN1=0
    DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0; //���Ρ���ֵ����
    DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;//�������ر�
    DAC_Init(DAC_Channel_1,&DAC_InitType); //�۳�ʼ�� DAC ͨ�� 1
    DAC_Cmd(DAC_Channel_1, ENABLE); //��ʹ�� DAC ͨ�� 1
    DAC_SetChannel1Data(DAC_Align_12b_R , 0); //��12 λ�Ҷ������ݸ�ʽ

}


/**----------------------------------------------------------------------------
* @FunctionName  : Get_ADC_Average()
* @Description   : None
* @Data          : 2016/7/11
* @Explain       :
-------------------------------------------------------------------------------
����ͨ�� 1(PA4) �����ѹ
vol 0~3.3V
------------------------------------------------------------------------------*/
void DAC1_Vol_Set(u16 vol)
{
//	double temp = vol*4096/3.3;
    DAC_SetChannel1Data(DAC_Align_12b_R,vol);//12 λ�Ҷ������ݸ�ʽ
}



/*******************************(C) COPYRIGHT 2016 Wind��л���죩*********************************/





