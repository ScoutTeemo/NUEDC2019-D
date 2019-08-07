#include "App_Keybord.h"


//�ȿ�����ʾ�� "USB Connecting..." �ٽ��������
//��ֵ����ʾ�������� usbh_usr.c ��

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;
extern HID_Machine_TypeDef HID_Machine;	


//USB��Ϣ��ʾ
//msgx:0,USB������
//     1,USB����
//     2,USB���
//     3,��֧�ֵ�USB�豸
void USBH_Msg_Show(u8 msgx)
{
	
	LCD_SetTextColor(Red);
	
	switch(msgx)
	{
		
		case 0:	//USB������
			OS_String_Show(30,130,16,1,"USB Connecting...");	
			LCD_Appoint_Clear(0,150,800,480,White);
			break;
		
		case 1:	//USB����
			OS_String_Show(30,130,16,1,"USB Connected    ");	
			OS_String_Show(30,150,16,1,"USB KeyBoard");	 
			OS_String_Show(30,180,16,1,"KEYVAL:");	
			OS_String_Show(30,200,16,1,"INPUT STRING:");	
			break;
		
		case 2:	//USB���
			OS_String_Show(30,130,16,1,"USB Connected    ");	
			OS_String_Show(30,150,16,1,"USB Mouse");	 
			OS_String_Show(30,180,16,1,"BUTTON:");	
			OS_String_Show(30,200,16,1,"X POS:");	
			OS_String_Show(30,220,16,1,"Y POS:");	
			OS_String_Show(30,240,16,1,"Z POS:");	
			break; 		
		
		case 3:	//��֧�ֵ�USB�豸
			OS_String_Show(30,130,16,1,"USB Connected    ");	
			OS_String_Show(30,150,16,1,"Unknow Device");	 
			break; 	 
		
	} 
}   


//HID��������
void USBH_HID_Reconnect(void)
{
	
	//�ر�֮ǰ������
	USBH_DeInit(&USB_OTG_Core_dev,&USB_Host);	//��λUSB HOST
	USB_OTG_StopHost(&USB_OTG_Core_dev);		  //ֹͣUSBhost
	
	if(USB_Host.usr_cb->DeviceDisconnected)		//����,�Ž�ֹ
	{
		
			USB_Host.usr_cb->DeviceDisconnected(); 	//�ر�USB����
			USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);
			USB_Host.usr_cb->DeInit();
			USB_Host.class_cb->DeInit(&USB_OTG_Core_dev,&USB_Host.device_prop);
	}
	
	USB_OTG_DisableGlobalInt(&USB_OTG_Core_dev);//�ر������ж�
	
	//���¸�λUSB
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS ��λ
	delay_ms(5);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//��λ����  

	memset(&USB_OTG_Core_dev,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));
	
	//��������USB HID�豸
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks); 
	
}


void Keybord_main(void)
{
	
	OS_LCD_Clear(White);
	LCD_SetTextColor(Blue);
	
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200�����ڲ鿴����״̬
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks); //��ʼ��USB����
	OS_String_Show(30,130,16,1,"USB Connecting...");	
	
	while(1)
	{
	
		OS_SchedLock();    /*  ��Ҫ��  */
		
		USBH_Process(&USB_OTG_Core_dev, &USB_Host);
		
		if(bDeviceState==1)//���ӽ�����
		{ 
			if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))//���USB HIDͨ��,�Ƿ�����? 
			{ 	    
				USBH_HID_Reconnect();//����
			}				
			
		}
		else	//����δ������ʱ��,���
		{
			if(USBH_Check_EnumeDead(&USB_Host))	//���USB HOST ö���Ƿ�������?������,�����³�ʼ�� 
			{ 	    
				USBH_HID_Reconnect();//����
			}			
		}
		
		OS_SchedUnlock();  /*  ��Ҫ��  */

	}
	
}



