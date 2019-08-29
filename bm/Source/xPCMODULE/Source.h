
/**************************************************START OF FILE*****************************************************/



#ifndef HH__SOURCE__HH
#define HH__SOURCE__HH



/*  ------------------------------------------------------------------------------------------------------------------
包含头文件
*/
#include "stdint.h"

//数据来源
#define SOURCE_USB   0x01
#define SOURCE_DSP1  0x02
#define SOURCE_KEY   0X80 //下位机按键或编码器等修改参数
#define SOURCE_ALL   (SOURCE_USB|SOURCE_DSP1|SOURCE_KEY)

extern uint8_t SourceGet(uint8_t channel);
extern uint8_t SourceTransform(uint8_t source);

#endif


/**************************************************END OF FILE**********************************************************/


