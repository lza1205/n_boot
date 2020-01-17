#define _N_COMMAND_C_

#include "command.h"
#include "boot.h"
#include "xmodem.h"

INT8U *b_name = "b";		//启动内核
INT8U *s_name = "s";		//发送 bin文件
INT8U *printf_var_name = "printfvar";
INT8U *setvar_name = "setvar";
INT8U *eraseflash_name = "eraseflash";

//static INT8U flash_buf[1024];

void s_fun(void)
{
	falsh_erase_len(gt_boot_var.kernel_addr_base, 10*0x400);
	send_str("系统已经准备就绪,请发送软件包。发送过程中请不要输入任何键。 \r\n");
	/* 关闭中断 */
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//使能接收中断
	
	xm_receive((void *)0, 10*1024);							//协议传输文件

//	flash_rd(gt_boot_var.kernel_addr_base, flash_buf, 1024);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能接收中断
	send_str("\r\n已成功烧写到 flash  \r\n");
}
void b_fun(void)
{
	send_str("内核启动中，请等待 .....................................\r\n");

	/* 将前面已经打开的中断都关闭掉 */
	__cpu_disable_int();
	
	__cpu_goto_app(gt_boot_var.kernel_addr_base);
}

void printf_var_fun(void)
{
	n_boot_printf("   timeout:            %x                       系统启动延时\r\n",gt_boot_var.start_timeout);
	n_boot_printf("   mach:               %x                      机器码\r\n",gt_boot_var.mach);
	n_boot_printf("   boot_base:          %x                 boot 存放地址\r\n",gt_boot_var.boot_addr_base);
	n_boot_printf("   var_base            %x                 var 存放地址\r\n",gt_boot_var.boot_var_addr_base);
	n_boot_printf("   kernel_base         %x                 kernel 存放地址\r\n",gt_boot_var.kernel_addr_base);	
}

INT8U *var_name[] = {
	"reset",
	"timeout",
	"mach",
	"boot_base",
	"boot_size",
	"var_base",
	"var_size",
	"kernel_base",
};

void setvar_fun(void)
{
	INT8U err;
	INT8U i;
	INT8U *p;
	INT32U *var_p;
	
	for(i = 0; i < sizeof(var_name); i++)	//一个个比对
	{
		err = __str_cmp(gp_comman_tag[1], var_name[i]);
		if(err != 0)
		{
			break;
		}
	}
	if(err == 0)
	{
		send_str("参数有误，请重新输入。\r\n");
		return ;
	}
#if 0
	for(i = 0; i < sizeof(var_name); i++)	//一个个比对
	{
		err = __str_cmp(gt_comman[3].tag_p, var_name[i]);
		if(err != 0)
		{
			gt_comman[3].tag_p = gt_comman[3].tag_p + err + 1;
			break;
		}
	}

/* 对参数做一次检查 */
	p = gt_comman[3].tag_p;
	while((*p) != '\0')
	{
		if((*p) == ' ')			//出现空格
		{
			send_str("参数个数不对，请重新输入。\r\n");
			return ;
		}
		p ++;
	}
	if(err == 0)
	{
		send_str("参数有误，请重新输入。\r\n");
		return ;
	}
#if 0
	switch(i)
	{
		case 0:
			gt_boot_var.start_timeout = str_to_hex(gt_comman[3].tag_p);
			break;
		case 0:
			gt_boot_var.mach = str_to_hex(gt_comman[3].tag_p);
			break;
		case 0:
			gt_boot_var.boot_addr_base= str_to_hex(gt_comman[3].tag_p);
			break;
		case 0:
			gt_boot_var.boot_var_addr_base= str_to_hex(gt_comman[3].tag_p);
			break;
		case 0:
			gt_boot_var.kernel_addr_base= str_to_hex(gt_comman[3].tag_p);
			break;
	}
#endif
#endif
	if(i == 0)
	{
		if(g_comman_num != 1)
		{
			send_str("参数个数不对，请重新输入。\r\n");
			return ;
		}
		default_var();
		return ;
	}
	if(g_comman_num != 2)
	{
		send_str("参数个数不对，请重新输入。\r\n");
		return ;
	}
	var_p = (INT32U *)&gt_boot_var;
	var_p += (i - 1);
	*var_p = str_to_hex(gp_comman_tag[2]);
	
	wr_boot_var();
	send_str("参数设置成功\r\n");
}

void eraseflash_fun(void)
{
	
}


void command_init(void)
{
	command_index = 0;
	gt_comman[command_index].name = b_name;
	gt_comman[command_index].com_fun= b_fun;
	command_index ++;

	gt_comman[command_index].name = s_name;
	gt_comman[command_index].com_fun= s_fun;
	command_index ++;

	gt_comman[command_index].name = printf_var_name;
	gt_comman[command_index].com_fun= printf_var_fun;
	command_index ++;

	gt_comman[command_index].name = setvar_name;
	gt_comman[command_index].com_fun= setvar_fun;
	command_index ++;

	gt_comman[command_index].name = eraseflash_name;
	gt_comman[command_index].com_fun= eraseflash_fun;
	command_index ++;
	
}



