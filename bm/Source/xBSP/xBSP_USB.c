
/**************************************************START OF FILE*****************************************************/


/*
    ----------------------------------------------------------------------------------------------------------------
    1.文件使用说明 






    ----------------------------------------------------------------------------------------------------------------
*/



/*  ------------------------------------------------------------------------------------------------------------------
包含的头文件
*/


#include "xBSP_USB.h"
#include "YJ_QUEUE.h"
#include "usb.h" 

extern void USB_DeviceApplicationInit(void);
extern usb_status_t HID_Send64Byte(unsigned char* buff);

static uint8_t USB_Tx_Buffer[64] = {0,1,2,3,4};



/*
*********************************************************************************************************************
@ Name   : void xBSP_USB_Init(void)

@ Brief  : USB设备初始化

@ Param  : NONE

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 08 - 28
*********************************************************************************************************************
*/
void xBSP_USB_Init(void)
{
	USB_DeviceApplicationInit();
}



/*
*********************************************************************************************************************
@ Name   : uint32_t xBSP_USB_SendData(uint8_t* DataBuffer , uint32_t len)

@ Brief  : USB发送数据

@ Param  : DataBuffer：数据缓冲区 len：数据长度，范围为1-63

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 08 - 28
*********************************************************************************************************************
*/
usb_status_t xBSP_USB_SendData(uint8_t* DataBuffer , uint32_t len)
{
    return HID_Send64Byte(DataBuffer);
}


/*
*********************************************************************************************************************
@ Name   : xBSP_USB_SendBuffer

@ Brief  : USB发送数据

@ Param  : uint8_t* buffer:数据指针
@          uint32_t len：数据长度

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 06 - 13
*********************************************************************************************************************
*/
uint8_t xBSP_USB_SendBuffer(uint8_t* buffer , uint32_t len)
{
	unsigned int FrameTime = len / 63;	
	unsigned int LastFrameLen = len % 63;
	unsigned char isHaveLastFrame = 0;
	unsigned int ReadAddr = 0;
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int timeout = 0;
	
	if (LastFrameLen != 0)
	{
		isHaveLastFrame = 1;
	}

	for (i = 0; i < FrameTime; i++)
	{
		USB_Tx_Buffer[0] = 63;
		for (j = 0; j < 63; j++)
		{
			USB_Tx_Buffer[j + 1] = buffer[ReadAddr + j];
		}
		
        timeout = 40000;
		while((xBSP_USB_SendData(USB_Tx_Buffer , 64) != kStatus_USB_Success)&&(timeout!=0))
		{
            timeout--;
		}
		ReadAddr += 63;
	}

	if (isHaveLastFrame == 1)
	{
		USB_Tx_Buffer[0] = LastFrameLen;

		for (j = 0; j < LastFrameLen; j++)
		{
			USB_Tx_Buffer[j + 1] = buffer[ReadAddr + j];
		}
        if(LastFrameLen<63)
        {
            memset(USB_Tx_Buffer+LastFrameLen+1, 0, 63-LastFrameLen); 
        }
        
        timeout = 14000;
		while((xBSP_USB_SendData(USB_Tx_Buffer , 64) != kStatus_USB_Success)&&(timeout!=0))
		{
			timeout--;
		}
	}
	return 1;      
}

/*
*********************************************************************************************************************
@ Name   : void xBSP_USB_ReceiveeData(uint8_t* DataBuffer , uint32_t len)

@ Brief  : USB接收数据中断（注意这个函数是在中断中调用的）

@ Param  : DataBuffer：接收到数据缓冲区  数据长度，范围为1-63

@ Return : NONE

@ Author : WZB

@  Date  : 2019 - 08 - 28
*********************************************************************************************************************
*/
void xBSP_USB_ReceiveDataCallBack(uint8_t* DataBuffer , uint32_t len)
{
    QUEUE_PutBuffer(CHANNEL_USB, DataBuffer, len);
}


/**************************************************END OF FILE**********************************************************/
