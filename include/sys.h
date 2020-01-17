#ifndef _SYS_STM32_H_
#define _SYS_STM32_H_

#ifdef _SYS_STM32_C_
#define _SYS_STM32_EXT_
#else
#define _SYS_STM32_EXT_ extern
#endif

#include "all.h"

void sys_rcc_init(void);
void sys_gpio_init(void);
void sys_timer2_init(void);

void sys_uart1_init(void);
INT8U sys_uart1_getchar(void);
void sys_uart1_sendchar(uint8_t data);




#endif
