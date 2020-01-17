//*================================================================================================
//* 文件名				: timer.h
//* 功能描述			: 定时器相关配置
//* 作者    			: 罗伟东
//* 版本				: 0.01
//* 建立日期、时间		: 2011/04/24 14:44
//* 最近修改日期、时间	: 
//* 修改原因			: 
//*================================================================================================
#ifndef	__TIMER_H
#define __TIMER_H

#include "stm32f10x_conf.h"

#define	TIMER_ONE_TIME	0x01
#define TIMER_CYCLE		0x02

extern void USART1_Timer3_Config(void);
extern void USART1_Timer3_Stop(void);
extern void USART1_Timer3_Start(uint32_t time);

#endif
