#include "App_NIT.h"

u8 SetChoose=2;//����Ŀ��ѡ��
void DDSDataInit(void);
void NIT_main(void)
{
    u8 key;
    float result;

    DDSDataInit();
    while(1)
    {
        /*	����ɨ��		*/
        key=KEY_Scan(0);
        Key_Handle(key);

        /*	ɨ��С����		*/
        result=TouchKey_Scan();

        /*	����������		*/
        Touch_Handle(result);

        OSTimeDly(30);
    }
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
    dds.fre=1000;

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
    dds.output=0;

    Refresh(dds,SetChoose);
}


/*
 * Return:      void
 * Parameters:  data:С��������ֵ
 * Description: ����������
 */
void Touch_Handle(float data)
{
    if(data !=0 )//С���������룬�ı��Ӧ����ֵ
    {
        switch(SetChoose)
        {
        case 1:
            if(data <= 3.3f) dds.range=data;
            break;

        case 2:
            dds.fre=data;
            break;

        case 3:
            if(data < dds.fre_stop - dds.fre_start) dds.step=data;
            break;

        case 4:
            if(dds.step_time > 0) dds.step_time=data;
            break;

        case 5:
            if(data < dds.fre_stop)  dds.fre_start=data;
            break;

        case 6:
            if(data > dds.fre_start) dds.fre_stop=data;
            break;

        default :
            break;
        }

        sendData(dds);
        Refresh(dds,SetChoose);
    }

    if(Touch_Judge(130,25,250,70) == TOUCH_VALID_FULL)
    {
        dds.mode=NORMAL;
        sendData(dds);
    }
    else if(Touch_Judge(260,25,370,70) == TOUCH_VALID_FULL)
    {
        dds.mode=SWEEP;
        sendData(dds);
    }
    else if(Touch_Judge(575,375,720,440) == TOUCH_VALID_FULL)
    {
        dds.output^=1;
        sendData(dds);
    }
    else if(Touch_Judge(130,85,290,130) == TOUCH_VALID_FULL)
        SetChoose=1;
    else if(Touch_Judge(130,145,290,190) == TOUCH_VALID_FULL)
        SetChoose=2;
    else if(Touch_Judge(130,205,290,250) == TOUCH_VALID_FULL)
        SetChoose=3;
    else if(Touch_Judge(190,265,350,310) == TOUCH_VALID_FULL)
        SetChoose=4;
    else if(Touch_Judge(190,325,350,370) == TOUCH_VALID_FULL)
        SetChoose=5;
    else if(Touch_Judge(190,385,350,430) == TOUCH_VALID_FULL)
        SetChoose=6;
    else
        return ;

    Refresh(dds,SetChoose);
}

/*
 * Return:      void
 * Parameters:  key:��������ֵ
 * Description: ��������
 */
void Key_Handle(u8 key)
{

    if(key && (dds.mode == NORMAL))
    {
        switch(key)
        {
        case 2:
            if(dds.fre -dds.step >0)   dds.fre -= dds.step;
            break;

        case 4:
            dds.fre += dds.step;
            break;

        case 1:
            break;
        case 3:
            break;
        default:
            break;
        }

        sendData(dds);
        Refresh(dds,SetChoose);
    }

}




