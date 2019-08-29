/**************************************************START OF FILE*****************************************************/

#ifndef _INCLUDE_H_
#define _INCLUDE_H_

/*******************************************************************************************************************************
INCLUDE
*/
#include "MIMXRT1052.h"
#include "core_cm7.h"
#include "MIMXRT1052_features.h"


#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_cache.h"
#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_flexspi.h"
#include "fsl_debug_console.h"

#include "common.h"



/*******************************************************************************************************************************
基本定义
*/


#define DEBUG_WORK   1         //是否为调试模式


//中断相关配置  类似于stm32
#define NVIC_PriorityGroup_0   	((uint8_t)0x7)
#define NVIC_PriorityGroup_1	((uint8_t)0x6)
#define NVIC_PriorityGroup_2   	((uint8_t)0x5)
#define NVIC_PriorityGroup_3   	((uint8_t)0x4)
#define NVIC_PriorityGroup_4   	((uint8_t)0x3) //只有抢占优先级 范围0-15

/*定时器控制块变量 任务数量*/
#define TASK_NUM 5

/*  ------------------------------------------------------------------------------------------------------------------
系统消息列表定义
*/
#define message_TASK_LED 		   0x00
#define message_TASK_USBSEND   0x01
#define message_TASK_ADC 		   0x02



#endif


/**************************************************END OF FILE*****************************************************/
