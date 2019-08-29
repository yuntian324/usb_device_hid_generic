#ifndef HH__COMMON__HH
#define HH__COMMON__HH

#include "stdbool.h"
#include <stdint.h>
#include "string.h"

#if 1
#define  	BOOL 		unsigned char
	
#define  	TRUE		0X01
#define  	FALSE 	  	0X00

#define 	ON 			0x01
#define 	OFF			0x00

/*  ------------------------------------------------------------------------------------------------------------------
常用函数定义
*/
//#define  MEM_BYTE(x)      (*(unsigned char*)(x))
//#define  MEM_HWORD(x)     (*(UINT16*)(x))    
//#define  MEM_WORD(x)      (*(vu32*)(x)) 
//#define  MAX(x,y)         ((x)>(y)?(x):(y))


/*  ------------------------------------------------------------------------------------------------------------------
宏定义 - 打开关闭总中断
*/

#if 1

/*打开总中断*/
#define OPEN_INT()  __enable_irq() 
/*关闭总中断*/
#define CLOSE_INT()	__disable_irq()

#endif


/*  ------------------------------------------------------------------------------------------------------------------
联合体 - 定义位操作
*/
#if 0
typedef	union
{
	u32	DW;
	u16	W[2];
	u8	B[4];
	struct 
	{
		u32 b0:1; u32 b1:1; u32 b2:1; u32 b3:1; u32 b4:1; u32 b5:1; u32 b6:1; u32 b7:1; 
		u32 b8:1; u32 b9:1; u32 b10:1;u32 b11:1;u32 b12:1;u32 b13:1;u32 b14:1;u32 b15:1;
		u32 b16:1;u32 b17:1;u32 b18:1;u32 b19:1;u32 b20:1;u32 b21:1;u32 b22:1;u32 b23:1;
		u32 b24:1;u32 b25:1;u32 b26:1;u32 b27:1;u32 b28:1;u32 b29:1;u32 b30:1;u32 b31:1;
	}data;
}Dtype;		//sizeof(Dtype)	 4

#endif
#endif

#endif


