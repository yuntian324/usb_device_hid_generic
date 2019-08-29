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
��������
*/


#define DEBUG_WORK   1         //�Ƿ�Ϊ����ģʽ


//�ж��������  ������stm32
#define NVIC_PriorityGroup_0   	((uint8_t)0x7)
#define NVIC_PriorityGroup_1	((uint8_t)0x6)
#define NVIC_PriorityGroup_2   	((uint8_t)0x5)
#define NVIC_PriorityGroup_3   	((uint8_t)0x4)
#define NVIC_PriorityGroup_4   	((uint8_t)0x3) //ֻ����ռ���ȼ� ��Χ0-15

/*��ʱ�����ƿ���� ��������*/
#define TASK_NUM 5

/*  ------------------------------------------------------------------------------------------------------------------
ϵͳ��Ϣ�б���
*/
#define message_TASK_LED 		   0x00
#define message_TASK_USBSEND   0x01
#define message_TASK_ADC 		   0x02



#endif


/**************************************************END OF FILE*****************************************************/
