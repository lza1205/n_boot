
#include "stm32f10x.h"
#include "ALL.h"
#include "xmodem.h"


//移植时，需要修改该函数
//串口收发，实用查询方式。
void xm_port_write(uint8 *ch)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);   
	USART1->DR = *ch;

}

//串口接收函数，需要移植
sint8 xm_port_read(uint8 *ch)
{
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)
	{  
		*ch = USART_ReceiveData(USART1);
		return 1;
	}
	return 0;
}

//在定时中断里调用该函数
//定时时间5ms
void xm_timer(void)
{
	xmodem_timeout++;
}


