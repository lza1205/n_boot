#ifndef _N_CPU_H_
#define _N_CPU_H_

#ifdef _N_CPU_C_
#define _N_CPU_EXT_
#else
#define _N_CPU_EXT_ extern
#endif

#include "all.h"
#include "flash.h"

/******************************************************************
	FLASH 中的地址
******************************************************************/

#define N_BOOT_ADDR_BASE			0x8000000
#define N_BOOT_SIZE					0x3000

#define N_VAR_ADDR_BASE				0x8003000
#define N_VAR_SIZE					(1 * FLASH_PAGE_SIZE)

#define N_KENREL_ADDR_BASE			0x8003800


/* 中断向量表偏移地址 */
#define N_NVIC_OFFSET_ADDRR			(N_KENREL_ADDR_BASE - N_BOOT_ADDR_BASE)


//#define get_chat()		sys_uart1_getchar()
#define send_char(a)		sys_uart1_sendchar(a)

typedef  void (*pFunction)(void);


uint8_t __cpu_goto_app(uint32_t Addr);

void __cpu_disable_int(void);



#endif






