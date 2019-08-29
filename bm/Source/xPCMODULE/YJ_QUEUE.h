
/**************************************************START OF FILE*****************************************************/



#ifndef HH__USART__HH
#define HH__USART__HH



/*  ------------------------------------------------------------------------------------------------------------------
包含头文件
*/
#include "stdint.h"

/*  ------------------------------------------------------------------------------------------------------------------
宏定义
*/
//定义队列的缓冲区大小
#define CHANNELS_NUM            2//定义缓冲区队列数量
#define QUEUE_BUFFER_SIZE_USB       1024
#define QUEUE_BUFFER_SIZE_DSP1      1024
#define CHANNEL_USB             0
#define CHANNEL_DSP1            1


typedef struct
{
    uint8_t *prx_buffer;//缓存队列指针
    uint32_t rx_buffer_count;            //有效数据数量记录
    uint32_t rx_buffer_writep;           //写指针位置
    uint32_t rx_buffer_readp;            //读指针
}QueueBuffTypeDef;
extern QueueBuffTypeDef QueueBuff[CHANNELS_NUM];


/*  ------------------------------------------------------------------------------------------------------------------
函数定义
*/


extern void QUEUE_ClearBuffer(uint8_t channel);
extern uint8_t QUEUE_PutBuffer(uint8_t channel, uint8_t *buffer, uint32_t count);
extern uint8_t QUEUE_GetBuffer(uint8_t channel, uint32_t count, uint32_t* offset);



#endif


/**************************************************END OF FILE**********************************************************/


