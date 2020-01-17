#ifndef _N_BOOT_H_
#define _N_BOOT_H_

#ifdef _N_BOOT_C_
#define _N_BOOT_EXT_
#else
#define _N_BOOT_EXT_ extern
#endif

#include "all.h"

#include "sys.h"
#include "cpu_h.h"
//#include "my_list.h"


/* boot 系统参数 */
struct boot_var{
	INT32U start_timeout;		//内核启动延时
	INT32U mach;				//机器ID

	INT32U boot_addr_base;		//boot地址
	INT32U boot_size;			//boot代码段大小
	INT32U boot_var_addr_base;	//boot 参数地址
	INT32U boot_var_size;		//boot参数段大小
	INT32U kernel_addr_base;	//AP 内核地址

};


#define START_TIMEOUT 5					/* 启动延时 5 秒*/

#define RX_BUF_SIZE		(2*1024)			/* 接收缓冲区的大小 */

_N_BOOT_EXT_ struct boot_var gt_boot_var;		//系统参数

//_N_BOOT_EXT_ INT8U gt_boot_var.start_timeout;		//内核启动延时

_N_BOOT_EXT_ volatile INT32S g_delay_ms;		//延时
_N_BOOT_EXT_ volatile INT8U g_compel_out;	//强制从延时中退出

_N_BOOT_EXT_ volatile INT8U uart_rx_buf[RX_BUF_SIZE];	//uart 接收缓冲区
_N_BOOT_EXT_ volatile INT8U uart_rx_index;		//写入下标

_N_BOOT_EXT_ volatile INT8U uart_rx_ok;			//已经接收到一个回车符，可以对数据进行处理了

_N_BOOT_EXT_ INT8U flash_buf[1024];			//读出flash 的缓存


_N_BOOT_EXT_ volatile void (*receive_input)(INT8U res);		//输入处理函数指针


void n_boot_memu(void);
void rd_boot_var(void);
void wr_boot_var(void);
void default_var(void);

volatile void receive_command(INT8U res);

void translate_tag(void);
void translate_command(void);


void send_str_prefix(INT8U *str);
void send_str(INT8U *str);
void send_var(INT32U var);
void n_boot_printf(INT8U *str, INT32U var);

void get_str(void);

INT8U __str_cmp(INT8U *str1, INT8U *str2);
INT32U str_to_hex(INT8U *str);




#endif






