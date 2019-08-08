/*
*********************************************************************************************************
*                                              _04_OS
* File			 : OS_UI.c
* By  			 : Wind
* platform   : STM32F407ZG
*	Data   		 : 2018/7/16
*********************************************************************************************************
*/

/* ͷ�ļ����� ----------------------------------------------------------------*/
#include "OS_UI.h"

/* ˽�к궨�� ----------------------------------------------------------------*/

/* ˽�У���̬���������� -------------------------------------------------------*/
void OS_Char_Show(u16 x, u16 y, u8 num, u8 size, u8 mode);

/* ȫ�ֱ������� --------------------------------------------------------------*/

/* ȫ�ֱ������� --------------------------------------------------------------*/

/* ȫ�ֺ�����д --------------------------------------------------------------*/

/*============================================= �ײ����� =========================================*/
/**----------------------------------------------------------------------------
* @FunctionName  : OS_LCD_Init()
* @Description   : ��ʼ������
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* None
------------------------------------------------------------------------------*/
int OS_LCD_Init(void)
{
    TFT_LCD_Init();
    LCD_Clear(WHITE);//����
    if(font_init())
    {
        OS_String_Show(400, 200, 32, 1, "Font Error!!!"); //����ֿ�
        return -1;
    }
    TextColor = BLACK;
    BackColor = WHITE;

    return 0;

}

/**----------------------------------------------------------------------------
* @FunctionName  : OSTextColor_Set()
* @Description   : None
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* None
------------------------------------------------------------------------------*/
unsigned int OS_TextColor_Set(u16 _color)
{
    uint16_t temp = TextColor;
    TextColor = _color;
    return temp;
}


/**----------------------------------------------------------------------------
* @FunctionName  : OS_BackColor_Set()
* @Description   : None
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* None
------------------------------------------------------------------------------*/
unsigned int OS_BackColor_Set(u16 _color)
{
    uint16_t temp = BackColor;
    BackColor = _color;
    return temp;
}

/**----------------------------------------------------------------------------
* @FunctionName  : OS_LCD_Clear()
* @Description   : None
* @Data          : 2016/7/12
* @Explain
-------------------------------------------------------------------------------
* None
------------------------------------------------------------------------------*/
int OS_LCD_Clear(u16 color)
{
    LCD_Clear(color);
    return 0;
}



/**----------------------------------------------------------------------------
* @FunctionName  : OS_Point_Draw()
* @Description   : ����
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* x,y:����
* POINT_COLOR:�˵����ɫ
------------------------------------------------------------------------------*/
int OS_Point_Draw(u16 x, u16 y, u16 color)
{
    LCD_SetCursor(x, y); //���ù��λ��
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(color);
    return 0;
}

/**----------------------------------------------------------------------------
* @FunctionName  : OS_Char_Show()
* @Description   : ��ӡ�ַ�
* @Data          : 2016/7/14
* @Explain
-------------------------------------------------------------------------------
��ָ��λ����ʾһ���ַ�
x,y:��ʼ����
num:Ҫ��ʾ���ַ�:" "--->"~"
size:�����С 16/24/32
mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
------------------------------------------------------------------------------*/
void OS_Char_Show(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
    u8 temp, t1, t;
    u16 y0 = y;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
    //���ô���
    num = num - ' '; //�õ�ƫ�ƺ��ֵ
    for(t = 0; t < csize; t++)
    {
        if(size == 16)temp = asc2_1608[num][t];	//����1608����
        else if(size == 24)temp = asc2_2412[num][t]; //����2412����
        else if(size == 32)temp = asc2_3216[num][t]; //����3216����
        else return;//û�е��ֿ�
        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80)OS_Point_Draw(x, y, TextColor);
            else if(mode == 1)OS_Point_Draw(x, y, BackColor);
            temp <<= 1;
            y++;
            if(y >= 480)return;		//��������
            if((y - y0) == size)
            {
                y = y0;
                x++;
                if(x >= 800)return;	//��������
                break;
            }
        }
    }
}

/**----------------------------------------------------------------------------
* @FunctionName  : OS_String_Show()
* @Description   : ��ӡ����
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* x,y ����    *code ���ָ��   size �����С
------------------------------------------------------------------------------*/
void OS_HzMat_Get(char *code, uint8_t *mat, u8 size)
{
    uint8_t qh, ql;
    uint8_t i;
    uint32_t foffset;

    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    qh = *code;
    ql = *(++code);
    if(qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) //�ǳ��ú���
    {
        for(i = 0; i < csize; i++)*mat++ = 0x00; //�������
        return; //��������
    }
    if(ql < 0x7f)ql -= 0x40; //ע��!
    else ql -= 0x41;
    qh -= 0x81;
    foffset = ((uint32_t)190 * qh + ql) * csize;	//�õ��ֿ��е��ֽ�ƫ����
    switch(size)
    {
    case 32:
        W25Q64_Read(mat, foffset + ftinfo.f12addr, csize);
        break;
    case 16:
        W25Q64_Read(mat, foffset + ftinfo.f16addr, csize);
        break;
    case 24:
        W25Q64_Read(mat, foffset + ftinfo.f24addr, csize);
        break;

    }
}


/**----------------------------------------------------------------------------
* @FunctionName  : OS_Font_Show()
* @Description   : ��ӡ����
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* x,y ����    *str ���ָ��   size �����С
------------------------------------------------------------------------------*/
void OS_Font_Show(u16 x, u16 y, char *font, u8 size, u8 mode)
{
    u8 temp, t, t1;
    u16 y0 = y;
    u8 dzk[144];
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size); //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    if(size != 32 && size != 16 && size != 24) return;	//��֧�ֵ�size
    OS_HzMat_Get(font, dzk, size);	//�õ���Ӧ��С�ĵ�������
    for(t = 0; t < csize; t++)
    {
        temp = dzk[t];			//�õ���������
        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80) OS_Point_Draw(x, y, TextColor);
            else if(mode == 1) OS_Point_Draw(x, y, BackColor);
            temp <<= 1;
            y++;
            if((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}



/*============================================= Ӧ�ó���� =========================================*/
/**----------------------------------------------------------------------------
* @FunctionName  : OS_Line_Draw()
* @Description   : ����
* @Data          : 2016/7/14
* @Explain
-------------------------------------------------------------------------------
x1,y1	:�������
x2,y2	:�յ�����
size	:�������
------------------------------------------------------------------------------*/
int OS_Line_Draw(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{

    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    if(x1 == x2 && y1 == y2) x1 = x2 + 1;

    delta_x = x2 - x1; //������������
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if(delta_x > 0)incx = 1; //���õ�������
    else if(delta_x == 0)incx = 0; //��ֱ��
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if(delta_y > 0)incy = 1;
    else if(delta_y == 0)incy = 0; //ˮƽ��
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if( delta_x > delta_y)distance = delta_x; //ѡȡ��������������
    else distance = delta_y;
    for(t = 0; t <= distance + 1; t++ ) //�������
    {
        OS_Point_Draw(uRow, uCol, color); //����
        xerr += delta_x ;
        yerr += delta_y ;
        if(xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }

    return 0;
}




/**----------------------------------------------------------------------------
* @FunctionName  : OS_Circle_Draw()
* @Description   : ��Բ
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
(x,y)		:���ĵ�
r			:Բ�İ뾶
size		:�����Ŀ��  	0,��ʾʵ��
------------------------------------------------------------------------------*/
int OS_Circle_Draw(u16 x, u16 y, u8 r, u8 size, u16 color)
{
    u16 temp = TextColor;

    TextColor = color;
    if(size == 0)
    {
        LCD_DrawCircleS(x, y, r, color);
    }
    else
    {
        while(size--)
            LCD_DrawCircle(x, y, r - size);
    }
    TextColor = temp;

    return 0;
}


/**----------------------------------------------------------------------------
* @FunctionName  : OS_Rect_Draw()
* @Description   : ������
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
x1,y1,x2,y2 ���������Խ�����
size		���������   0,��ʾʵ��
------------------------------------------------------------------------------*/
int OS_Rect_Draw(u16 x1, u16 y1, u16 x2, u16 y2, u8 size, u16 color)
{
    u16 temp = TextColor;

    TextColor = color;
    if(size == 0)
    {
        LCD_DrawRectS(x1, y1, x2, y2, color);

    }
    else
    {
        while(size --)
        {
            OS_Line_Draw(x1, y1 + size, x2, y1 + size, color);
            OS_Line_Draw(x1, y2 - size, x2, y2 - size, color);
            OS_Line_Draw(x2 - size, y1, x2 - size, y2, color);
            OS_Line_Draw(x1 + size, y1, x1 + size, y2, color);
        }
    }
    TextColor = temp;

    return 0;
}



/**----------------------------------------------------------------------------
* @FunctionName  : OS_Picture_Draw()
* @Description   : ��ʾͼƬ
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
* RGB565  16λ�����ʾ
* mode :  0 ���ɷ���      1 ������       2 �����ţ�����ʾ��ɫ
		  3 �����ţ�����ʾ��ɫ,�ұ��
* ��ͷ����
------------------------------------------------------------------------------*/
int OS_Picture_Draw(u16 x1, u16 y1, u16 x2, u16 y2, u8 mode, const u8 *pic)
{
    u16 temp = TextColor;
    static u8 UeseState = 0;

    if(UeseState) return -1;
    UeseState = 1;

    if(mode == 0)
    {
        u16 x, y, p_color;
        u16 len_x = (pic[2] << 8) | pic[3];
        u16 len_y = (pic[4] << 8) | pic[5];

        for(y = 0; y < len_y; y++)
            for(x = 0; x < len_x * 2; x += 2)
            {
                if((u16)(x / 2 + x1) <= x2)
                    if((u16)(y + y1) <= y2)
                    {
                        p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];

                        LCD_SetCursor(x / 2 + x1, y + y1); //���ù��λ��
                        LCD_WriteRAM_Prepare();
                        LCD_WriteRAM(p_color);
                    }
            }
    }
    else if(mode == 1)
    {
        u16 x, y, p_color;
        float density_x, density_y;
        u16 len_x = (pic[2] << 8) | pic[3];
        u16 len_y = (pic[4] << 8) | pic[5];

        density_x = (float)(x2 - x1) / len_x;
        density_y = (float)(y2 - y1) / len_y;

        for(y = 0; y < len_y; y++)
            for(x = 0; x < len_x * 2; x += 2)
            {
                if((u16)((x / 2)*density_x + x1) <= x2)
                    if((u16)(y * density_y + y1) <= y2)
                    {
                        p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];
                        LCD_SetCursor((x / 2)*density_x + x1, y * density_y + y1); //���ù��λ��
                        LCD_WriteRAM_Prepare();
                        LCD_WriteRAM(p_color);
                    }

            }

    }

    else if(mode == 2)
    {
        u16 x, y, p_color;
        float density_x, density_y;
        u16 len_x = (pic[2] << 8) | pic[3];
        u16 len_y = (pic[4] << 8) | pic[5];

        density_x = (float)(x2 - x1) / len_x;
        density_y = (float)(y2 - y1) / len_y;

        for(y = 0; y < len_y; y++)
            for(x = 0; x < len_x * 2; x += 2)
            {
                if((u16)((x / 2)*density_x + x1) <= x2)
                    if((u16)(y * density_y + y1) <= y2)
                    {
                        p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];
                        if(p_color > 1) //����ʾ��ɫ����
                        {
                            LCD_SetCursor((x / 2)*density_x + x1, y * density_y + y1); //���ù��λ��
                            LCD_WriteRAM_Prepare();
                            LCD_WriteRAM(p_color);
                        }
                    }

            }
    }

    else if(mode == 3)
    {
        u16 x, y, p_color, BitRed, BitGreen, BitBlue;
        float density_x, density_y;
        u16 len_x = (pic[2] << 8) | pic[3];
        u16 len_y = (pic[4] << 8) | pic[5];

        density_x = (float)(x2 - x1) / len_x;
        density_y = (float)(y2 - y1) / len_y;

        for(y = 0; y < len_y; y++)
            for(x = 0; x < len_x * 2; x += 2)
            {
                if((u16)((x / 2)*density_x + x1) <= x2)
                    if((u16)(y * density_y + y1) <= y2)
                    {
                        p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];
                        if(p_color > 1) //����ʾ��ɫ����
                        {
                            p_color = pic[x + 6 + y * len_x * 2] << 8 | pic[(x + 7) + y * len_x * 2];
                            BitRed   = (p_color & (0x1f << 11)) >> 11;
                            BitGreen = (p_color & (0x3f << 5 )) >> 5;
                            BitBlue = p_color & 0x1f;
                            p_color = ((BitRed / 3) << 11) | ((BitGreen / 3) << 5) | (BitBlue / 3);

                            LCD_SetCursor((x / 2)*density_x + x1, y * density_y + y1); //���ù��λ��
                            LCD_WriteRAM_Prepare();
                            LCD_WriteRAM(p_color);
                        }
                    }

            }

    }
    TextColor = temp;

    UeseState = 0;
    return 0;
}







/**----------------------------------------------------------------------------
* @FunctionName  : OS_String_Show()
* @Description   : ��ӡ����
* @Data          : 2016/3/22
* @Explain
-------------------------------------------------------------------------------
x,y 		����
str 		���ָ��
size 		�����С 16 24 32
mode 		0,�ޱ���ɫ��1������ɫ
return 		0,��ӡ���		-1,��ӡ��ռ��
------------------------------------------------------------------------------*/
int OS_String_Show(u16 x, u16 y, u8 size, u8 mode, char *str)
{
    uint16_t x0 = x;
    uint8_t bHz = 0;   //�ַ���������


    while(*str != 0)		//����δ����
    {

        if(!bHz)
        {
            if((signed char)*str < 0)bHz = 1; //����
            else              //�ַ�
            {
                if(*str == '\n') //���з���
                {
                    y += size;
                    x = x0;
                    str++;
                }
                OS_Char_Show(x, y, *str, size, mode);
                str++;
                x += size / 2; //�ַ�,Ϊȫ�ֵ�һ��
            }
        }
        else //����
        {
            bHz = 0; //�к��ֿ�
            OS_Font_Show(x, y, str, size, mode);
            str += 2;
            x += size; //��һ������ƫ��
        }
    }

    return 0;
}




/**----------------------------------------------------------------------------
* @FunctionName  : OS_Num_Show()
* @Description   : �๦�ܴ�ӡ��ֵ
* @Data          : 2017/7/12
* @Explain
-------------------------------------------------------------------------------
x,y 		����
num 		��ֵ
size 		�����С 12 16 24 32
mode 		0,�ޱ���ɫ��1������ɫ
format		--------------------------------
			%d ʮ�����з�����������BUG������%0.0f���
			%u ʮ�����޷�������
			%f ������	%lf ��������
			%s �ַ���
			%c �����ַ�
			%p ָ���ֵ
			%e ָ����ʽ�ĸ�����
			%x,%X �޷�����ʮ�����Ʊ�ʾ������
			%o �޷����԰˽��Ʊ�ʾ������
			%g,NULL �Զ�ѡ����ʵı�ʾ��
			%p �����ַ��
			--------------------------------
return 		0,��ӡ���		-1,��ӡ��ռ��
ע��format����������÷������磺"ʱ�䣺%0.0f ms"
------------------------------------------------------------------------------*/
int OS_Num_Show(u16 x, u16 y, u8 size, u8 mode, double num, char *format)
{
    char temp[100] = {0};

    if(format == NULL) sprintf(temp, "%g", num); //�Զ�ѡ���ʽ
    else sprintf(temp, format, num);

    OS_String_Show(x, y, size, mode, temp);

    return 0;
}

/**----------------------------------------------------------------------------
* @FunctionName  : OS_Wave_Draw()
* @Description   : ��������Ϻ���
* @Data          : 2017/7/13
* @Explain
-------------------------------------------------------------------------------
Struct_OSC��--------------------------------
			u16 x,y;//OSC���ڵ�λ��
			float ZoomWindows;//��ʾ����������
			u16 BackColor;//������ɫ

			float ShiftX; //����ƫ����	1.0�Ǳ�׼��
			float ShiftY; //����ƫ����
			float ZoomX;  //����������
			float ZoomY;  //����������
			u16 LineColor;//������ɫ
			u8 LineSize;//������ϸ�ĳߴ�
			u8 Mode;//	OSC_MODE_NORMAL:����ʾ��� 	OSC_MODE_FFT����ʾ�����FFTģʽ��
			u16 BuffLenth;//����
			u8 BuffFormat;//Buff�ĸ�ʽ
			u16 *Buff_u16;
			u8 *Buff_u8;
			float *Buff_float;
			double *Buff_double;
			--------------------------------
mode		--------------------------------
			OSC_MODE_WINDOWS_SHOW	//������
            OSC_MODE_LINE_SHOW		//����
			--------------------------------
return 		0,��ӡ���		-1,��ӡ��ռ��	-2,���δ���
ע�⣺double float �����ݵ�����������룻��u8 u16 �����ݵ�����ڵ׶�
------------------------------------------------------------------------------*/
int OS_Wave_Windows_Show(Type_OSC *Struct_OSC)
{
    u16 x1 = Struct_OSC->x;
    u16 y1 = Struct_OSC->y;
    u16 x2 = Struct_OSC->x + OS_LCD_WHITH * Struct_OSC->ZoomWindows;
    u16 y2 = Struct_OSC->y + OS_LCD_HEIGHT* Struct_OSC->ZoomWindows;
    u8 size = 10 * Struct_OSC->ZoomWindows;
    u16 i = 0;
    u16 cnt = 0;

    /* ���߿򼰱��� --------------------------------------*/
    if(size < 1) size = 1;
    OS_Rect_Draw(x1 + size, y1 + size, x2 - size - 1, y2 - size - 1, 0, Struct_OSC->BackColor); //����
    OS_Rect_Draw(x1, y1, x2 - 1, y2 - 1, size, Struct_OSC->WindowsColor); //����
    OS_Line_Draw(x1 + 1, y2, x2, y2 - 1, (Struct_OSC->WindowsColor / 2)); //��Ӱ
    OS_Line_Draw(x2, y1 + 1, x2, y2, (Struct_OSC->WindowsColor / 2)); //��Ӱ

    /* ��� ---------------------------------------------*/
    for(i = y1 + size - 1; i < y2 - size; i += size) //Y��
    {
        cnt = (cnt + 1) % 5;
        if(cnt == 0)
        {
            OS_Line_Draw(x1 + size, i, x1 + size * 3, i, Struct_OSC->WindowsColor);
            OS_Line_Draw(x2 - size * 3, i, x2 - size, i, Struct_OSC->WindowsColor);
        }
        else
        {
            OS_Line_Draw(x1 + size, i, x1 + size * 2, i, Struct_OSC->WindowsColor);
            OS_Line_Draw(x2 - size * 2, i, x2 - size, i, Struct_OSC->WindowsColor);
        }
    }

    for(i = x1 + size - 1; i < x2 - size; i += size) //X��
    {
        cnt = (cnt + 1) % 5;
        if(cnt == 0)
        {
            OS_Line_Draw(i, y1 + size, i, y1 + size * 3, Struct_OSC->WindowsColor);
            OS_Line_Draw(i, y2 - size * 3, i, y2 - size, Struct_OSC->WindowsColor);
        }
        else
        {
            OS_Line_Draw(i, y1 + size, i, y1 + size * 2, Struct_OSC->WindowsColor);
            OS_Line_Draw(i, y2 - size * 2, i, y2 - size, Struct_OSC->WindowsColor);
        }
    }

    return 0;

}





int OS_Wave_Line_Show(Type_OSC *Struct_OSC)
{

//	if(Struct_OSC->Buff_u16[0] >= 0x0FFF||Struct_OSC->Buff_u16[ADC1_DMA2_LENTH/2] >= 0x0FFF\
//		||Struct_OSC->Buff_u16[ADC1_DMA2_LENTH-5] >= 0x0FFF||Struct_OSC->Buff_u16[ADC1_DMA2_LENTH/3] >= 0x0FFF)
//		return -1;


    u8 size = 10 * Struct_OSC->ZoomWindows;
    u16 x1 = Struct_OSC->x + size * 3 + Struct_OSC->LineSize + 3;
    u16 y1 = Struct_OSC->y + size * 3 + Struct_OSC->LineSize + 3;
    u16 x2 = Struct_OSC->x + OS_LCD_WHITH * Struct_OSC->ZoomWindows - size * 3 - Struct_OSC->LineSize - 3;
    u16 y2 = Struct_OSC->y + OS_LCD_HEIGHT* Struct_OSC->ZoomWindows - size * 3 - Struct_OSC->LineSize - 3;
    u16 i = 0, j = 0;
    u16 mid_y = (y1 + y2) / 2;
    u16 WD_lenth = x2 - x1;
    u16 t_x1 = 0, t_y1 = 0, t_x2 = 0, t_y2 = 0;
    u16 ZeroLine = 0;

    for(i = 0; i < WD_lenth - 1; i ++)
    {
        /* �����һ�εĺۼ� ----------------------------------------*/
        if(Struct_OSC ->LastMode != OSC_MODE_ERROR)
        {
            if(Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] >= y1//��ֹԽ������
                    && Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1] >= y1
                    && Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] <= y2
                    && Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1] <= y2)
            {
                if(Struct_OSC ->LastMode == OSC_MODE_NORMAL)
                {
                    for(j = 0; j < Struct_OSC->LastLineSize; j++)
                    {
                        OS_Line_Draw(i + x1 - j, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1 + 1 - j, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1], Struct_OSC->BackColor);
                        OS_Line_Draw(i + x1 + j, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1 + 1 + j, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1], Struct_OSC->BackColor);
                        OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] + j, i + x1 + 1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1] + j, Struct_OSC->BackColor);
                        OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] - j, i + x1 + 1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1 + 1] - j, Struct_OSC->BackColor);
                    }
                }
                else if(Struct_OSC ->LastMode == OSC_MODE_FFT)
                {
                    if(Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] < ZeroLine)
                        OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1, Struct_OSC->LastZeroLine + 1, Struct_OSC->BackColor);
                    else if(Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1] > ZeroLine)
                        OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1, Struct_OSC->LastZeroLine + 3, Struct_OSC->BackColor);

                    OS_Line_Draw(i + x1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1], i + x1 + 1, Struct_OSC->LastBuff[Struct_OSC->LastBuff_Cnt][i + x1+1], Struct_OSC->BackColor);
                }

                if(Struct_OSC->LastZeroLine >= y1 && Struct_OSC->LastZeroLine <= y2)//��ֹԽ������
                {
                    OS_Point_Draw(i + x1 + 1, Struct_OSC->LastZeroLine + 1, Struct_OSC->BackColor);
                    OS_Point_Draw(i + x1 + 1, Struct_OSC->LastZeroLine + 2, Struct_OSC->BackColor);
                    OS_Point_Draw(i + x1 + 1, Struct_OSC->LastZeroLine + 3, Struct_OSC->BackColor);
                }
            }
        }

        /* ����ת�� ------------------------------------------*/
        t_x1 = x1 + i / Struct_OSC->ZoomX - Struct_OSC->ShiftX;
        t_x2 = x1 + (i + 1) / Struct_OSC->ZoomX - Struct_OSC->ShiftX;

        if((t_x1 < Struct_OSC->BuffLenth) && (t_x2 < Struct_OSC->BuffLenth))
        {
            if(Struct_OSC->BuffFormat == OSC_FORMAT_U8)
            {
                t_y1 = y2 - Struct_OSC->Buff_u8[t_x1] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                t_y2 = y2 - Struct_OSC->Buff_u8[t_x2] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                ZeroLine = y2 - Struct_OSC->ShiftY;
            }
            else if(Struct_OSC->BuffFormat == OSC_FORMAT_U16)
            {
                t_y1 = y2 - Struct_OSC->Buff_u16[t_x1] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                t_y2 = y2 - Struct_OSC->Buff_u16[t_x2] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                ZeroLine = y2 - Struct_OSC->ShiftY;
            }
            else if(Struct_OSC->BuffFormat == OSC_FORMAT_FLOAT)
            {
                t_y1 = mid_y - Struct_OSC->Buff_float[t_x1] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                t_y2 = mid_y - Struct_OSC->Buff_float[t_x2] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                ZeroLine = mid_y - Struct_OSC->ShiftY;
            }
            else if(Struct_OSC->BuffFormat == OSC_FORMAT_DOUBLE)
            {
                t_y1 = mid_y - Struct_OSC->Buff_double[t_x1] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                t_y2 = mid_y - Struct_OSC->Buff_double[t_x2] * Struct_OSC->ZoomY - Struct_OSC->ShiftY;
                ZeroLine = mid_y - Struct_OSC->ShiftY;
            }

            /* ��ֹ��ͼԽ�� ------------------------------------------*/
            if(t_y1 < y1) t_y1 = y1;
            else if(t_y1 > y2)  t_y1 = y2;

            if(t_y2 < y1) t_y2 = y1;
            else if(t_y2 > y2)  t_y2 = y2;

            /* �����εĲ��� ------------------------------------------*/
            if(t_x1 != 0 && t_x2 != 0)
            {
                if(Struct_OSC->Mode == OSC_MODE_NORMAL)//����
                {
                    for(j = 0; j < Struct_OSC->LineSize; j++)
                    {
                        OS_Line_Draw(i + x1, t_y1 + j, i + x1 + 1, t_y2 + j, Struct_OSC->LineColor);
                        OS_Line_Draw(i + x1, t_y1 - j, i + x1 + 1, t_y2 - j, Struct_OSC->LineColor);
                        OS_Line_Draw(i + x1 + j, t_y1 , i + x1 + 1 + j, t_y2 , Struct_OSC->LineColor);
                        OS_Line_Draw(i + x1 - j, t_y1 , i + x1 + 1 - j, t_y2 , Struct_OSC->LineColor);
                    }
                }
                if(Struct_OSC->Mode == OSC_MODE_FFT)//�����
                {
                    if(t_y1 < ZeroLine)
                        OS_Line_Draw(i + x1, t_y1, i + x1, ZeroLine + 1, Struct_OSC->LineColor);
                    else if(t_y1 > ZeroLine)
                        OS_Line_Draw(i + x1, t_y1, i + x1, ZeroLine + 3, Struct_OSC->LineColor);

                    OS_Line_Draw(i + x1, t_y1, i + x1 + 1, t_y2, ~Struct_OSC->LineColor);
                }
            }

            if((ZeroLine >= y1 && ZeroLine <= y2))
            {
                OS_Point_Draw(i + x1 + 1, ZeroLine + 1, ~BLACK);//�����
                OS_Point_Draw(i + x1 + 1, ZeroLine + 2, BLACK);
                OS_Point_Draw(i + x1 + 1, ZeroLine + 3, ~BLACK);
            }
            /* ��¼��β��ε�λ�� ---------------------------------*/
            if(i == 0)
            {
                Struct_OSC->LastBuff[(Struct_OSC->LastBuff_Cnt + 1) % 2][x1] = t_y1;
            }
            Struct_OSC->LastBuff[(Struct_OSC->LastBuff_Cnt + 1) % 2][i + x1 + 1] = t_y2;
            Struct_OSC->LastZeroLine = ZeroLine;
        }
        else
        {
            Struct_OSC ->LastMode = OSC_MODE_ERROR;
        }

    }
    /* ��¼������ò��� ---------------------------------*/
    Struct_OSC->LastBuff_Cnt = (Struct_OSC->LastBuff_Cnt + 1) % 2;
    Struct_OSC->LastLineSize = Struct_OSC->LineSize;
    Struct_OSC->LastMode = Struct_OSC->Mode;

    return 0;
}

int OS_Wave_Unite(Type_OSC *Struct_OSC_Source, Type_OSC *Struct_New)
{
    if(Struct_New != NULL && Struct_OSC_Source != NULL)
    {
        Struct_New->ZoomWindows = Struct_OSC_Source->ZoomWindows;
        Struct_New->BackColor = Struct_OSC_Source->BackColor;
        Struct_New->WindowsColor = Struct_OSC_Source->WindowsColor;
        Struct_New->x = Struct_OSC_Source->x;
        Struct_New->y = Struct_OSC_Source->y;
        return 0;
    }

    return -1;
}

int OS_Wave_Draw(Type_OSC *Struct_OSC, u8 mode)
{
    if(mode == OSC_MODE_WINDOWS_SHOW)
        return OS_Wave_Windows_Show(Struct_OSC);
    else if(mode == OSC_MODE_LINE_SHOW)
        return OS_Wave_Line_Show(Struct_OSC);
    else return -2;

}



/**
* @brief  ������Ϳ̶��� (���õĲ���������xcase��ycaseΪ������������)
* @param
* @retval none
*/
void Draw_Grid(GRAPH_Struct griddata)
{
    u16 i,j;
    u16 shift_x,shift_y;
    u16 x1=griddata.x1;
    u16 y1=griddata.y1;
    u16 x2=griddata.x2;
    u16 y2=griddata.y2;
    u16 color=griddata.griacolor;
    u16 xnumber=griddata.xnumber;
    u16 ynumber=griddata.ynumber;

    float xcase=(float)(x2-x1)/((xnumber-1)*5);//������һС��ĳ���   ÿ�����5С��
    float ycase=(float)(y2-y1)/((ynumber-1)*5);//������һС��ĳ���

    shift_x=xcase;
    shift_y=ycase;

    OS_Rect_Draw(x1-shift_x,y1-shift_y,x2+shift_x,y2+shift_y,1,color);//��������

    if(griddata.grid == 1)
    {
        for(i=y1; i<=y2; i+=ycase*5) //������
        {
            for(j=x1; j<=x2; j+=xcase)
                OS_Point_Draw(j,i,color);
        }

        for(i=x1; i<=x2; i+=xcase*5) //������
        {
            for(j=y1; j<y2; j+=ycase)
                OS_Point_Draw(i,j,color);
        }
    }

    for(i=x1; i<=x2; i+=xcase) //�������̶���
    {
        if((i-x1)%((u16)(xcase*5)) == 0)
            OS_Line_Draw(i,y1-shift_y,i,y1-shift_y+ycase,color);
        else
            OS_Line_Draw(i,y1-shift_y,i,y1-shift_y+ycase/2,color);
    }

    for(i=x1; i<=x2; i+=xcase) //���ײ��̶���
    {
        if((i-x1)%((u16)(xcase*5)) == 0)
            OS_Line_Draw(i,y2+shift_y,i,y2+shift_y-ycase,color);
        else
            OS_Line_Draw(i,y2+shift_y,i,y2+shift_y-ycase/2,color);
    }

    for(i=y1; i<=y2; i+=ycase) //����߿̶���
    {
        if((i-y1)%((u16)(ycase*5)) == 0)
            OS_Line_Draw(x1-shift_x,i,x1-shift_x+xcase,i,color);
        else
            OS_Line_Draw(x1-shift_x,i,x1-shift_x+xcase/2,i,color);
    }

    for(i=y1; i<=y2; i+=ycase) //���ұ߿̶���
    {
        if((i-y1)%((u16)(ycase*5)) == 0)
            OS_Line_Draw(x2+shift_x,i,x2+shift_x-xcase,i,color);
        else
            OS_Line_Draw(x2+shift_x,i,x2+shift_x-xcase/2,i,color);
    }
}

/**
* @brief  ��ʾ��ǩ
* @param  xnumber:��������
					ynumber:��������
* @retval none
*/
void Show_Label(GRAPH_Struct griddata,u8 yselect)
{
    u16 i;
    u16 x1=griddata.x1;
    u16 y1=griddata.y1;
    u16 x2=griddata.x2;
    u16 y2=griddata.y2;
    u16 xnumber=griddata.xnumber;
    u16 ynumber=griddata.ynumber;

    float step;
    float xcase=(float)(x2-x1)/(xnumber-1);//�������С��ĳ���
    float ycase=(float)(y2-y1)/(ynumber-1);//�������С��ĳ���


    OS_String_Show(x2-20,y2+25,16,0,"Fre/Hz");//���������ǩ

    step=(log10f(griddata.xmax)-log10f(griddata.xmin))/(xnumber-1);
	

    for(i=0; i<xnumber; i++) //�������ᵥλ���ȱ�ǩ
        OS_Num_Show(x1+i*xcase-20,y2+10,16,1, powf(10, ( log10f(griddata.xmin)+i*step )),"%.0f"); //griddata.xmin +

    if(yselect & LEFTY)
    {
        OS_String_Show(x1,y1-25,16,0,"Gain/dB");//������������ǩ

        OS_Line_Draw(x2+60,y1,x2+100,y1,griddata.left_ycolor);		//��ע��ͬ��ɫ���ߴ���ĺ���
        OS_String_Show(x2-60,y1-25,16,0,"Gain--Fre");

        step=(griddata.left_ymax-griddata.left_ymin)/(ynumber-1);
        for(i=0; i<ynumber; i++) //����������ᵥλ���ȱ�ǩ
            OS_Num_Show(x1-40,y1+i*ycase-7,16,1,griddata.left_ymax-i*step,"%.1f");
    }

    if(yselect & RIGHTY)
    {
        OS_String_Show(x2-50,y1-25,16,0,"Phase/��");//�ұ����������ǩ

        OS_Line_Draw(x2+60,y1+30,x2+100,y1+30,griddata.right_ycolor);
        OS_String_Show(x2+110,y1+20,16,0,"Phase--Fre");

        step=(griddata.right_ymax-griddata.right_ymin)/(ynumber-1);
        for(i=0; i<ynumber; i++) //�ұ��������ᵥλ���ȱ�ǩ
            OS_Num_Show(x2+15,y1+i*ycase-7,16,1,griddata.right_ymax-i*step,"%.0f");
    }

}

/**
* @brief  ������
* @param  yselect:  0---�� x-left_y 1---�� x-right_y
* @retval none
*/
void Draw_Graph(GRAPH_Struct *griddata,u8 yselect)
{
    u16 i,j;
    u16 x1=griddata->x1;
    u16 y1=griddata->y1;
    u16 x2=griddata->x2;
    u16 y2=griddata->y2;
    u16 high=griddata->y2-griddata->y1;
    u16 wide=griddata->x2-griddata->x1;
    u16 length=griddata->Buff_Lenth;
    u16 tempx1,tempx2,tempy1,tempy2;
    float vpp;
    float xstep=(float)wide/(length);//���������
    float xcase=(float)(x2-x1)/((griddata->xnumber-1)*5);//������һС��ĳ���   ÿ�����5С��
    float ycase=(float)(y2-y1)/((griddata->ynumber-1)*5);//������һС��ĳ���
    static u8 frist=1;

    //����ϴεĲ���
    if(yselect & LEFTY)
    {
				tempx1=x1;
				for(i=1;i<length;i++)
				{
						tempx2=x1+i*xstep;
						OS_Line_Draw(tempx1,griddata->lastbuff[0][i-1],tempx2,griddata->lastbuff[0][i],griddata->Backcolor);
				}
        if(!frist)
        {
            for(i=0; i<length; i++)
            {
                tempx1=x1+i*xstep;
                OS_Line_Draw(tempx1,y2,tempx1,griddata->lastbuff[0][i],griddata->Backcolor);
            }
        }

        frist=0;
    }

    if(yselect & RIGHTY)
    {
        tempx1=x1;
        for(i=1; i<length; i++)
        {
            tempx2=x1+i*xstep;
            OS_Line_Draw(tempx1,griddata->lastbuff[1][i-1],tempx2,griddata->lastbuff[1][i],griddata->Backcolor);
        }
    }

    //���»����񣬲�������
    if(griddata->grid == 1)
    {
        for(i=y1; i<=y2; i+=ycase*5) //������
        {
            for(j=x1; j<=x2; j+=xcase)
                OS_Point_Draw(j,i,griddata->griacolor);
        }

        for(i=x1; i<=x2; i+=xcase*5) //������
        {
            for(j=y1; j<y2; j+=ycase)
                OS_Point_Draw(i,j,griddata->griacolor);
        }
    }

    //������
    if(yselect & LEFTY)
    {
				vpp=griddata->left_ymax-griddata->left_ymin;

			  tempx1=x1;
			  tempy1=y2-(griddata->left_buff[0]-griddata->left_ymin)/vpp*high;

			  griddata->lastbuff[0][0]=tempy1;//���汾������

			  for(i=1;i<length;i++)
			  {
					tempx2=x1+i*xstep;

					tempy2=y2-(griddata->left_buff[i]-griddata->left_ymin)/vpp*high;

					OS_Line_Draw(tempx1,tempy1,tempx2,tempy2,griddata->left_ycolor);

					griddata->lastbuff[0][i]=tempy2;//���汾������

					tempx1=tempx2;
					tempy1=tempy2;
				}

//        vpp=griddata->left_ymax-griddata->left_ymin;

//        for(i=0; i<length; i++)
//        {
//            tempx1=x1+i*xstep;

//            tempy1=y2-(griddata->left_buff[i]-griddata->left_ymin)/vpp*high;

//            OS_Line_Draw(tempx1,y2,tempx1,tempy1,griddata->left_ycolor);

//            griddata->lastbuff[0][i]=tempy1;//���汾������
//        }
    }

    if(yselect & RIGHTY)
    {
        vpp=griddata->right_ymax-griddata->right_ymin;

        tempx1=x1;
        tempy1=y2-(griddata->right_buff[0]-griddata->right_ymin)/vpp*high;

        griddata->lastbuff[1][0]=tempy1;

        for(i=1; i<length; i++)
        {
            tempx2=x1+i*xstep;

            tempy2=y2-(griddata->right_buff[i]-griddata->right_ymin)/vpp*high;

            OS_Line_Draw(tempx1,tempy1,tempx2,tempy2,griddata->right_ycolor);

            griddata->lastbuff[1][i]=tempy2;

            tempx1=tempx2;
            tempy1=tempy2;
        }
    }

    //���¹����ʾ
    griddata->cursor_refrssh=1;
}


/**
* @brief  ��ʾ���λ������
* @param
* @retval none
*/
void Cursor_Data_Show(GRAPH_Struct *griddata,u8 yselect)
{
    u16 i;
    u16 num,temp;
    u16 x1=griddata->x1;
    u16 y1=griddata->y1;
    u16 x2=griddata->x2;
    u16 y2=griddata->y2;
    u16 wide=griddata->x2-griddata->x1;
    u16 length=griddata->Buff_Lenth-1;
    u8  shift;

    float vpp;
    float xstep=(float)wide/length;//���������
    float xcase=(float)(x2-x1)/((griddata->xnumber-1)*5);//������һС��ĳ���   ÿ�����5С��
    float ycase=(float)(y2-y1)/((griddata->ynumber-1)*5);//������һС��ĳ���

    if(griddata->cursor_x == 0)//��ʼλ��
    {
        griddata->cursor_x=x1;
    }

    if(x1-10 >= 0 && x2+10 <= 800)	shift=10;
    else	shift=0;//���ӷ�Χ�������ƶ����߽�

    if(TouchStruct.NowX >= x1-shift && TouchStruct.NowX <= x2+shift)
    {
        if(TouchStruct.NowX >= x2)
            griddata->cursor_x=x2;
        else if(TouchStruct.NowX <= x1)
            griddata->cursor_x=x1;
        else
        {
            num=(u16)(TouchStruct.NowX/xstep);

            temp=num*xstep;

            if(temp <= x1)
                griddata->cursor_x=x1;
            else
                griddata->cursor_x=temp;
        }
    }

    //�ı���λ�ã�������ʾ
    if(griddata->cursor_x != griddata->lastcursor_x || griddata->cursor_refrssh)
    {
        if(griddata->cursor_refrssh)//ǿ�и��¹����ʾ
        {
            if(griddata->lastcursor_x == 0)
                griddata->cursor_x=x1;
            else
                griddata->cursor_x=griddata->lastcursor_x;
        }
        else
        {
            //����ϴεĹ����
            OS_Line_Draw(griddata->lastcursor_x,y1,griddata->lastcursor_x,y2,griddata->Backcolor);

            //����һ�ι��λ�õ����ߵ�
            num=(u16)(griddata->lastcursor_x - x1)/xstep;

            if(num < length)
            {
                if(yselect & LEFTY && griddata->lastcursor_x != 0)
                    OS_Line_Draw(griddata->lastcursor_x,griddata->lastbuff[0][num],griddata->lastcursor_x+xstep,griddata->lastbuff[0][num+1],griddata->left_ycolor);

                if(yselect & RIGHTY && griddata->lastcursor_x != 0)
                    OS_Line_Draw(griddata->lastcursor_x,griddata->lastbuff[1][num],griddata->lastcursor_x+xstep,griddata->lastbuff[1][num+1],griddata->right_ycolor);
            }

            //��ȫ����
            if((griddata->lastcursor_x % (u16)xcase) == 0 && griddata->lastcursor_x != 0)
            {
                if((griddata->lastcursor_x % (u16)(xcase*5)) == 0)
                {
                    for(i=y1; i<y2; i+=ycase)
                        OS_Point_Draw(griddata->lastcursor_x,i,griddata->griacolor);
                }
                else
                {
                    for(i=y1; i<=y2; i+=ycase*5)
                        OS_Point_Draw(griddata->lastcursor_x,i,griddata->griacolor);
                }
            }
        }

        //�����
        OS_Line_Draw(griddata->cursor_x,y1,griddata->cursor_x,y2,griddata->cursor_color);

        //��ʾ�������
        vpp=griddata->xmax-griddata->xmin;
        OS_Num_Show(x2+60,y1+60,24,1,griddata->xmin+(griddata->cursor_x-x1)/xstep*vpp/length,"Fre:%.2fMHz  ");

        num=(u16)(griddata->cursor_x - x1)/xstep;
        OS_Num_Show(x2+60,y1+100,24,1,griddata->left_buff[num],"Av:%.2fdB  ");
        OS_Num_Show(x2+60,y1+140,24,1,griddata->right_buff[num],"Phase:%.2f��  ");

        griddata->cursor_refrssh=0;
        griddata->lastcursor_x=griddata->cursor_x;
    }
}




/*******************************(C) COPYRIGHT 2016 Wind��л���죩*********************************/



















