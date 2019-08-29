

/**************************************************START OF FILE*****************************************************/


#ifndef HH__CMD_CALLBACK__HH
#define HH__CMD_CALLBACK__HH



#include "stdint.h"
#define getSingleData(pS, offset)         (pS[0X000003FF&offset])//获取1字节数据，pS:数组首地址指针，offset：所要读取数据相对首地址偏移量


extern void CMD_Callback(uint8_t* pStart, uint16_t offsetAA, uint8_t source);






#endif


/**************************************************END OF FILE**********************************************************/
