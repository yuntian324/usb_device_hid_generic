/**************************************************START OF FILE*****************************************************/



/**
  ***********************************************************************************************************************
	如何移植本文件:

	1.修改debug 的定义值来使能或失能调试信息打印
	
	2.修改GET_DATA  定义的函数宏

	3.修改ReceiveCallBack 定义的函数宏
	
	4.修改Check  定义的函数宏
	
	5.根据应用的实际情况修改MAX_FRAME_LEN 的定义。

	6.根据应用的实际情况修改Head_H Head_L


	其它说明:

	1. GET_DATA  函数宏用来获取接收到的数据，获取失败时返回0 获取成功时返回1
	
	    参数Count 要获取数据的个数，pBuffer:获取到数据存放的空间

	2.ReceiveCallBack 函数当数据帧被成功解释后所调用，属于callback 函数 pBuffer帧头指针

	3.Check 函数宏用来反回16位的校验值，Count数据个数据 ，pBuffer校验数据指针
	

	作者: YWJ  QQ:872180981
     
***********************************************************************************************************************
 * */



/*  ------------------------------------------------------------------------------------------------------------------
包含头文件
*/
#include "DataUnpacking.h"
#include "stdint.h"
#include "CRC16.h"
#include "YJ_QUEUE.h"
#include "CMD_CallBack.h"
#include "Source.h"


/*  ------------------------------------------------------------------------------------------------------------------
宏定义
*/
#ifndef NULL
#define NULL  0
#endif

#define CHANNELCOUNT   CHANNELS_NUM       		//缓存区数量
#define MAX_FRAME_LEN  QUEUE_BUFFER_SIZE_USB  	//定义一帧数据的最大长度

/*定义帧头*/
#define Head_H 0xAA
#define Head_L 0x55

#define debug 0

/*发生解码错误时的跳转状态*/
#define STEP_ERR 6

#define offsetLimit(channel,offset)                 (0X000003FF&(offsetAA[channel]+(offset)))//偏移地址限制在0-1023范围内
#define getData(channel,offset)                     (pStart[channel][offsetLimit(channel,offset)])//获取1字节数据，channel:缓冲数组通道号，offset：所要读取数据相对首地址偏移量

#if(CONFIG_REGISTER_FUN == 0)
/*定义数据获取函数*/
#define GET_DATA(channel, Count, offset) 			QUEUE_GetBuffer(channel, Count, offset)		//获取数据函数
#define Check(pStart, offset, len)                  CRC16_CountP(pStart, offset, len)    //计算校验值的函数

#else //#if(CONFIG_REGISTER_FUN == 0)

/*定义数据获取函数*/
uint8_t (*GET_DATA)(uint32_t,uint8_t*);		//获取数据函数
void (*ReceiveCallBack)(uint8_t*);			//解包完成回调函数
uint16_t (*Check)(uint8_t*,int32_t);		//计算校验值的函数
#endif //#if(CONFIG_REGISTER_FUN == 0)

uint8_t* pStart[CHANNELCOUNT];//记录缓存数组首地址
uint16_t length[CHANNELCOUNT];//记录该帧数据长度
uint32_t offsetAA[CHANNELCOUNT];//记录AA地址相对缓存数组首地址便宜量（字节）



/*  ------------------------------------------------------------------------------------------------------------------
函数定义
*/
extern void CMD_Callback(uint8_t* pStart, uint16_t offsetAA, uint8_t source);
	
/*状态机处理函数列表*/
static void DUP_UnpackCASE0(uint8_t channel);
static void DUP_UnpackCASE1(uint8_t channel);
static void DUP_UnpackCASE2(uint8_t channel);
static void DUP_UnpackCASE3(uint8_t channel);
static void DUP_UnpackCASE4(uint8_t channel);
static void DUP_UnpackCASE5(uint8_t channel);
static void DUP_UnpackCASE6(uint8_t channel);
static void DUP_UnpackChannel(uint8_t channel);



/*  ------------------------------------------------------------------------------------------------------------------
变量定义
*/

/*解包完成回调函数列表*/
void (* ReceiveCallBackList[])(uint8_t* pStart, uint16_t offsetAA, uint8_t source) = 
{
    CMD_Callback,
};

/*状态机处理函数列表*/
void (* CaseFundList[])(uint8_t channel) = 
{
	DUP_UnpackCASE0,
	DUP_UnpackCASE1,
	DUP_UnpackCASE2,
	DUP_UnpackCASE3,
	DUP_UnpackCASE4,
	DUP_UnpackCASE5,
	DUP_UnpackCASE6,
};


/*当前状态机所处状态标识*/
uint8_t Step[CHANNELCOUNT];


/*当前缓冲区有效数据个数*/
#if (MAX_FRAME_LEN <= 0xFF)
uint8_t vaildDataCount[CHANNELCOUNT];
#elif  (MAX_FRAME_LEN <= 0xFFFF)
uint16_t vaildDataCount[CHANNELCOUNT];
#elif (MAX_FRAME_LEN <= 0xFFFFFFFF)
uint32_t vaildDataCount[CHANNELCOUNT];
#endif



/*
***********************************************************************************************************************
@函数名称	:	void DUP_Register_Fun(void* Get_data , void* RecCallBack ,void* check)

@功    能 	:	注册模块所使用到的函数指针

@参    数	:	获取数据函数 , 解包完成回调函数 ,计算校验值的函数

@返 回 值	：	NONE

@作    者 	：	YWJ
************************************************************************************************************************
*/

#if(CONFIG_REGISTER_FUN == 1)

void DUP_Register_Fun(void* Get_data , void* RecCallBack ,void* check)
{
	GET_DATA = (uint8_t (*)(uint32_t,uint8_t*))Get_data;
	ReceiveCallBack = (void (*)(uint8_t*))RecCallBack;
	Check = (uint16_t (*)(uint8_t*,int32_t))check;
}

#endif

/*
***********************************************************************************************************************
@函数名称	:	void DUP_init(void)

@功    能 	:	解包模块程序初始化

@参    数	:	NONE

@返 回 值	：	NONE

@作    者 	：	YWJ
************************************************************************************************************************
*/
void DUP_init(void)
{
    uint8_t channel;
    for(channel=0; channel<CHANNELCOUNT; channel++)
    {
        Step[channel] = 0;
        vaildDataCount[channel] = 0;   
		pStart[channel] = QueueBuff[channel].prx_buffer;
		QUEUE_ClearBuffer(channel);
    }
}


/*
***********************************************************************************************************************
@函数名称	:	void DUP_Unpack(void)

@功    能 	:	根据数据流解包函数

@参    数	:	NONE

@返 回 值	：	NONE

@作    者 	：	YWJ
************************************************************************************************************************
*/
void DUP_Unpack(void)
{
	DUP_UnpackChannel(CHANNEL_USB);
	DUP_UnpackChannel(CHANNEL_DSP1);
}

/*
***********************************************************************************************************************
@函数名称	:	DUP_UnpackChannel

@功    能 	:	根据数据流解包相应通道数据

@参    数	:	channel:数据通道

@返 回 值	：	NONE

@作    者 	：	YWJ WZB
************************************************************************************************************************
*/
static void DUP_UnpackChannel(uint8_t channel)
{
	static uint8_t STEPCP = 0; 
	
	#if debug
	if(STEPCP != Step)
	{
		printf("Step = %d\n", Step);
		STEPCP = Step;
	}
	#endif
	//状态机跳转
	do
	{
		STEPCP = Step[channel];
		CaseFundList[Step[channel]](channel);
	}
	while(STEPCP != Step[channel]);
}



/*
***********************************************************************************************************************
@函数名称	:	void DUP_UnpackCASE0(void)

@功    能 	:	状态机为0时，获取并判断帧头H

@参    数	:	channel:数据通道

@返 回 值	：	NONE

@作    者 	：	YWJ WZB
************************************************************************************************************************
*/
static void DUP_UnpackCASE0(uint8_t channel)
{
	//从缓冲区中获取一个数据
	if(GET_DATA(channel, 1, &offsetAA[channel]))
	{
		if(getData(channel, 0) == Head_H)
		{
			vaildDataCount[channel] = 1;
			Step[channel]++;
		}
	}
}

/*
***********************************************************************************************************************
@函数名称	:	void DUP_UnpackCASE0(void)

@功    能 	:	状态机为0时，获取并判断帧头L

@参    数	:	channel:数据通道

@返 回 值	：	NONE

@作    者 	：	YWJ WZB
************************************************************************************************************************
*/
static void DUP_UnpackCASE1(uint8_t channel)
{
	if(vaildDataCount[channel]<2)
	{
		if(!GET_DATA(channel, 1, NULL))
		{
			return;
		}
		vaildDataCount[channel] = 2;
	}
		
	if(getData(channel,1) == Head_L)
	{
		Step[channel]++;
	}
	else
	{
		Step[channel] = STEP_ERR;
	}
}

/*
***********************************************************************************************************************
@函数名称	:	void DUP_UnpackCASE2(void)

@功    能 	:	状态机为2时，获取示蔟长度，并判断是否合法

@参    数	:	channel:数据通道

@返 回 值	：	NONE

@作    者 	：	YWJ WZB
************************************************************************************************************************
*/
static void DUP_UnpackCASE2(uint8_t channel)
{
	//判断是否要从缓冲区里取数据
	if(vaildDataCount[channel]<4)
	{
		if(!GET_DATA(channel, (4-vaildDataCount[channel]), NULL))
		{
			//没有取到数据
			return;
		}
		vaildDataCount[channel] = 4;
	}

	length[channel] = (getData(channel,3)<<8)|getData(channel,2);

	if(length[channel] <= MAX_FRAME_LEN-6)
	{
		//数据长度合格
		Step[channel]++;
	}
	else
	{
		//数据长度不合格
		Step[channel] = STEP_ERR;
	}
}

/*
***********************************************************************************************************************
@函数名称	:	void DUP_UnpackCASE3(void)

@功    能 	:	状态机为3时，获取数据+CRC部分

@参    数	:	channel:数据通道

@返 回 值	：	NONE

@作    者 	：	YWJ WZB
************************************************************************************************************************
*/
static void DUP_UnpackCASE3(uint8_t channel)
{       
	if(vaildDataCount[channel] < length[channel]+6)
	{
		if(!GET_DATA(channel, ((length[channel]+6) - vaildDataCount[channel]), NULL))
		{
			//没有取到数据
			return;
		}	
		Step[channel]++;
		vaildDataCount[channel] = length[channel]+6;
	}
	else
	{
		Step[channel]++;
	}
}

/*
***********************************************************************************************************************
@函数名称	:	void DUP_UnpackCASE4(void)

@功    能 	:	状态机为4时,进行CRC的校验

@参    数	:	channel:数据通道

@返 回 值	：	NONE

@作    者 	：	YWJ WZB
************************************************************************************************************************
*/
static void DUP_UnpackCASE4(uint8_t channel)
{
	//所计算的校验值
	uint16_t CheckResult = 0;
	//帧发送过来的校验值
	uint16_t FrameCheck = 0;
	FrameCheck = getData(channel, length[channel]+4);
	FrameCheck <<=8; 
	FrameCheck |= getData(channel, length[channel]+5);
	CheckResult = Check(pStart[channel], offsetAA[channel]+2, length[channel]+2);
#if 1
	//屏蔽数据校验
	FrameCheck = CheckResult;    
#endif	
	if(FrameCheck == CheckResult)
	{
		//CRC数据校验正确
		Step[channel]++;
	}
	else
	{
		//CRC数据校验错误
		Step[channel] = STEP_ERR;
	}
}

/*
***********************************************************************************************************************
@函数名称	:	void DUP_UnpackCASE5(void)

@功    能 	:	状态机为5时，分发数据包。

@参    数	:	channel:数据通道

@返 回 值	：	NONE

@作    者 	：	YWJ WZB
************************************************************************************************************************
*/
static void DUP_UnpackCASE5(uint8_t channel)
{
	if((length[channel] <= MAX_FRAME_LEN-6)&&(length[channel]>0))
    {
        ReceiveCallBackList[channel](pStart[channel], offsetAA[channel], SourceGet(channel));
    }
    else 
    {
        //data length is zero.
    }
	//printf("%c" ,0xAA);
	//缓冲区内没有多余的数据
	if(vaildDataCount[channel] == length[channel] + 6)
	{
		vaildDataCount[channel] = 0;
		Step[channel] = 0;
	}
	else
	{
		//缓冲区内存在多余的数据
		uint32_t i = 1;
		offsetAA[channel] = offsetLimit(channel, i);
		vaildDataCount[channel] = i;
		Step[channel] = STEP_ERR;
	}
}


/*
***********************************************************************************************************************
@函数名称	:	void DUP_UnpackCASE6(void)

@功    能 	:	状态机为8时，数据解包发生错误时的处理。

@参    数	:	channel:数据通道

@返 回 值	：	NONE

@作    者 	：	YWJ WZB
************************************************************************************************************************
*/
static void DUP_UnpackCASE6(uint8_t channel)
{
	//寻找帧头
	uint32_t i = 1;
	for(;i<vaildDataCount[channel];i++)
	{
		if(pStart[channel][offsetAA[channel]+i] == 0xAA)
		{
			break;
		}
	}
	
	//数据前移
	offsetAA[channel] = offsetLimit(channel, i);
	vaildDataCount[channel] = vaildDataCount[channel] - i;
	
	if(vaildDataCount[channel] == 0)
	{
		Step[channel] = 0;
	}
	else
	{
		Step[channel] = 1;
	}
}


/**************************************************END OF FILE*****************************************************/
