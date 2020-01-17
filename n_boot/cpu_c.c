#define _N_CPU_C_

#include "cpu_h.h"

/************************************************************************
时间:2014.3.23
作者: lza1205

	基于stm32 的boot 该文件是cpu相关的代码，不同平台需要修改

************************************************************************/
uint8_t __cpu_goto_app(uint32_t Addr)
{
	pFunction Jump_To_Application;
	__IO uint32_t JumpAddress; 

	if (((*(__IO uint32_t*)Addr) & 0x2FFE0000 ) == 0x20000000)
	{ 
		/* 地址要偏移4 */
		JumpAddress = *(__IO uint32_t*) (Addr + 4);
		Jump_To_Application = (pFunction) JumpAddress; 

		/* 使用app的栈 */
		__set_MSP(*(__IO uint32_t*) Addr);

		/* 跳转到用户函数入口地址 */
		Jump_To_Application();
	}
	return 1;
}



/*********************************************************
	经过测试，在跳转之前，要先将前面已经打开的中断都屏蔽掉
	因为跳转过去后，使用的是新的向量表，而新的向量表可能
	没有对应的中断，这样会出错
*********************************************************/
void __cpu_disable_int(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //中断占先等级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  //中断响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;			  //打开中断
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	//通道设置为串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //中断占先等级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //中断响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;			  //打开中断
	NVIC_Init(&NVIC_InitStructure);
}


