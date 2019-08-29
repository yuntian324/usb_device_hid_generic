
/**************************************************START OF FILE*****************************************************/


#ifndef HH__XBSP_USB__HH
#define HH__XBSP_USB__HH


/*  ------------------------------------------------------------------------------------------------------------------
包含的头文件
*/
#include "stdint.h"

//USB初始化
extern void xBSP_USB_Init(void);


//USB发送数据
extern uint8_t xBSP_USB_SendBuffer(uint8_t* buffer , uint32_t len);


//USB接收数据中断（注意这个函数是在中断中调用的）
extern void xBSP_USB_ReceiveDataCallBack(uint8_t* DataBuffer , uint32_t len);


#endif
/**************************************************END OF FILE**********************************************************/
