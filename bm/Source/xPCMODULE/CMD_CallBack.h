

/**************************************************START OF FILE*****************************************************/


#ifndef HH__CMD_CALLBACK__HH
#define HH__CMD_CALLBACK__HH



#include "stdint.h"
#define getSingleData(pS, offset)         (pS[0X000003FF&offset])//��ȡ1�ֽ����ݣ�pS:�����׵�ַָ�룬offset����Ҫ��ȡ��������׵�ַƫ����


extern void CMD_Callback(uint8_t* pStart, uint16_t offsetAA, uint8_t source);






#endif


/**************************************************END OF FILE**********************************************************/
