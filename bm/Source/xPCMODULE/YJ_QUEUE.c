
/**************************************************START OF FILE*****************************************************/








/*  ------------------------------------------------------------------------------------------------------------------
包含头文件
*/
#include "YJ_QUEUE.h"

#define INTX_ENABLE()   __enable_irq();
#define INTX_DISABLE()  __disable_irq();

/*  ------------------------------------------------------------------------------------------------------------------
定义变量
*/
//数据缓冲区数组
uint8_t rx_buffer_USB[QUEUE_BUFFER_SIZE_USB];
uint8_t rx_buffer_DSP1[QUEUE_BUFFER_SIZE_DSP1];

//数据缓冲区结构体，必须初始化缓存数组指针
QueueBuffTypeDef QueueBuff[CHANNELS_NUM] = 
{
	{
		.prx_buffer = rx_buffer_USB,
	},
	{
		.prx_buffer = rx_buffer_DSP1,
	}
};

const uint32_t point_max[CHANNELS_NUM] = {QUEUE_BUFFER_SIZE_USB-1, QUEUE_BUFFER_SIZE_DSP1-1};
const uint32_t QueueBuffSize[CHANNELS_NUM] = {QUEUE_BUFFER_SIZE_USB, QUEUE_BUFFER_SIZE_DSP1};


/*
***********************************************************************************************************************
@函数名称 	：void QUEUE_ClearBuffer(void)

@功    能 	：清除缓冲区

@参    数 	：channel：缓冲区通道号

@返 回 值	：NONE

@作    者 	：YWJ
************************************************************************************************************************
*/
void QUEUE_ClearBuffer(uint8_t channel)
{
	INTX_DISABLE();
	QueueBuff[channel].rx_buffer_count = 0;
	QueueBuff[channel].rx_buffer_writep = 0;
	QueueBuff[channel].rx_buffer_readp = 0;
	INTX_ENABLE();
}



/*
***************************************************************************************************************
@ Name   : QUEUE_PutBuffer

@ Brief  : 向队列中写入数据

@ Param  : uint8_t channel：缓冲区通道号
@          uint8_t *buffer :写入数据指针 
@          uint32_t count ：写入数据数量

@ Return : 0x00 : 读取数据失败  0x01 读取数据成功

@ Author : YWJ WZB

@  Date  : 
***************************************************************************************************************
*/
uint8_t QUEUE_PutBuffer(uint8_t channel, uint8_t *buffer, uint32_t count)
{
	uint32_t index = 0;
	if(QueueBuffSize[channel] - QueueBuff[channel].rx_buffer_count < count)	 
	{
		return 0;
	}
	else							
	{
		for(index = 0; index < count; index++)
		{
			QueueBuff[channel].prx_buffer[QueueBuff[channel].rx_buffer_writep]  = buffer[index];
			QueueBuff[channel].rx_buffer_writep = (point_max[channel] & (QueueBuff[channel].rx_buffer_writep + 1));	//指针增加
		}

		QueueBuff[channel].rx_buffer_count+=count;	//数据个数增加
	}
	return 1;
}


/*
***************************************************************************************************************
@ Name   : QUEUE_GetBuffer

@ Brief  : 读取队列中数据

@ Param  : uint8_t channel：缓冲区通道号
@          uint32_t count ：读取数据数量  
@          uint32_t* offset：数据在缓存区内的偏移地址

@ Return : 0x00 : 读取数据失败  0x01 读取数据成功

@ Author : YWJ WZB

@  Date  : 2019 - 04 - 23
***************************************************************************************************************
*/
uint8_t QUEUE_GetBuffer(uint8_t channel, uint32_t count, uint32_t* offset)
{	
	INTX_DISABLE();
	if(QueueBuff[channel].rx_buffer_count < count )	//读取的个数大于缓冲区内的数据个数
	{
		INTX_ENABLE();
		return 0;//读取失败
	}
	else							
	{
		*offset = QueueBuff[channel].rx_buffer_readp;//获取地址偏移量
		QueueBuff[channel].rx_buffer_readp = (point_max[channel] & (QueueBuff[channel].rx_buffer_readp + count));	//指针增加

		INTX_DISABLE();
		QueueBuff[channel].rx_buffer_count-=count;
		INTX_ENABLE();
		
		return 1;
	}
}


/**************************************************END OF FILE**********************************************************/


