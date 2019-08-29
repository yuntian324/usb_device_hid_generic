/**
  ************************************************************************************************************************************************************

	其它说明:

	1. void DUP_init(void) 用来初始化本模块所使用的相关数据

	2.void DUP_Unpack(void) 用于解释数据包的状态机，应定时调用或周期性调用

	3.DUP_Register_Fun : 用于注册回调函数等函数
	

	作者: YWJ  QQ:872180981
     
  ************************************************************************************************************************************************************
 * */
#ifndef HH__DATAUNPACKING__HH
#define HH__DATAUNPACKING__HH


#include "stdint.h"



//定义注册函数的方式，为1时使用函数指针方式，为0时使用宏定义
#define CONFIG_REGISTER_FUN 0



/*数据解包模块的初始化*/
extern void DUP_init(void);

/*数据解包*/
extern void DUP_Unpack(void);


#if(CONFIG_REGISTER_FUN == 1)

extern void DUP_Register_Fun(void* Get_data , void* RecCallBack ,void* check);

#endif


#endif


/**************************************************END OF FILE**********************************************************/


