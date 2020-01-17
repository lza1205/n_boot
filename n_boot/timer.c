//*================================================================================================
//* 文件名				: timer.c
//* 功能描述			: 定时器相关函数
//* 作者    			: 罗伟东
//* 版本				: 0.01
//* 建立日期、时间		: 2011/04/24 13:30
//* 最近修改日期、时间	: 
//* 修改原因			: 
//*================================================================================================
/*------------------------------------------ 头文件 ---------------------------------------------*/
#include "stm32f10x_conf.h"
#include "all.h"
#include "timer.h"
#include "xmodem.h"
/*-----------------------------------------------------------------------------------------------*/

/*----------------------------------------- 全局变量 --------------------------------------------*/
extern uint32_t	g_RequestTimes;	//发送文件请求次数
extern uint32_t	g_CheckType;
extern uint8_t	g_recTimeOutFlag;
extern uint32_t	g_starTimeTemp;
uint8_t	flag=0;
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------- 自定义数据类型 -----------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/

//*================================================================================================
//*											函	数	区
//*================================================================================================
/**
  * @brief  定时器3中断处理函数，在该函数中将串口接收状态机置于初始状态
  * @note   None
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
   //检测是否发生溢出更新事件
   if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		//清除TIM2的中断待处理位
	    TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
		g_recTimeOutFlag = TIME_OUT;
		if(g_starTimeTemp>0)
		{
		  g_starTimeTemp--;//定时器周期为100ms
		}
	}
}

/**
  * @brief  初始化USART接收数据定时器中断
  * @param  None
  * @retval None
  * @note  	None
  */
void USART1_Timer3_Interrupts_Config(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  初始化USART1接收定时器,防止USART接收数据超时 
  * @param  None
  * @retval None
  * @note  	该定时器可以防止在帧数据接收错误的时候其接收状态机出现错误状态
  */
void USART1_Timer3_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

    TIM_DeInit(TIM3);                              					//复位TIM2定时器
        
    /* TIM3 configuration */
    TIM_TimeBaseStructure.TIM_Period = 100;        					// 100ms    
    TIM_TimeBaseStructure.TIM_Prescaler = 36000;    				// 分频36000      
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  		// 时钟分频 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//计数方向向上计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Clear TIM3 update pending flag[清除TIM3溢出中断标志] */
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    /* Enable TIM3 Update interrupt [TIM3溢出中断允许]*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
	/* TIM3计数器清零*/
    TIM3->CNT=0;
	/* TIM3 enable counter [允许TIM3计数]*/
    TIM_Cmd(TIM3, DISABLE);  

	//初始化串口接收超时定时器中断
  	USART1_Timer3_Interrupts_Config();
}
/**
  * @brief  清除定时器计数器寄存器计数值并启动定时器
  * @param  None
  * @retval None
  * @note  	None
  */
void USART1_Timer3_Start(uint32_t time)
{
	TIM3->CNT=0;//清除计数器寄存器的值，可以减小丢帧的情况
	TIM3->ARR=time;
    /* Enable the TIM Counter */
	//TIM_Cmd(TIM3, ENABLE); 
    TIM3->CR1 |= ((uint16_t)0x0001);
}

/**
  * @brief  停止定时器并清除定时器的计数值 
  * @param  None
  * @retval None
  * @note  	None
  */
void USART1_Timer3_Stop(void)
{ 
    /* Disable the TIM Counter */
	//TIM_Cmd(TIM3, DISABLE);
	TIM3->CR1 &= ((uint16_t)0x03FE);
	TIM3->CNT=0;
}
