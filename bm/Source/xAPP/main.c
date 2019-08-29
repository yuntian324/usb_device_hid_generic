/*                                                     RT1052 实时操作系统
**********************************************************************************************************************************************
*平    台   ：RT1052  Cortex_M7  512 KB on-chip RAM(具体参考 1052数据手册).
*
*
*创建时间   ：
*
*
*作者：
**********************************************************************************************************************************************
*/
/*  ------------------------------------------------------------------------------------------------------------------
包含的头文件
*/
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>
#include "pin_mux.h"
#include "Bsp_Init.h"
#include "xBSP_USB.h"

extern unsigned char USB2PC(uint8_t *str,uint16_t date_leng);

//变量定义
uint8_t testArry[64] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 
60, 61, 62, 63 };

uint8_t testArry2[64] = {
5, 4, 3, 2, 1, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 
60, 61, 62, 63 };


//函数
void delay(uint32_t count)
{
	volatile uint32_t i = 0;
	for (i = 0; i < count; ++i)
	{
		__asm("NOP"); /* delay */
	}
}


int main(void)
{
    BOARD_ConfigMPU();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
	Bsp_Init();

    while (1U)
    {		
		xBSP_USB_SendBuffer(testArry, 64);
		delay(100000000);
		xBSP_USB_SendBuffer(testArry2, 64);
		delay(100000000);
    }
}
