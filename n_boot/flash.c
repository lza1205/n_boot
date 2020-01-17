#define _N_FLASH_C_

#include "all.h"
#include "flash.h"

/*********************************************************************
时间:2014.3.23
作者: lza1205

	flash 的相关操作
	该文件是cpu相关的代码，不同平台需要修改


*********************************************************************/

/* 从flash中读出数据 */
void flash_rd(INT32U addr, INT8U *buf, INT32U len)
{
	while(len)
	{
		(*buf++) = (*(INT8U *)addr);
		len -= 1;
		addr += 1;
	}
}

/* 向flash中写入数据 */
void flash_wr(INT32U addr, INT8U *buf, INT32U len)
{
	//INT8U l;
	union ulong_type ul_t;
	INT32U tmp_len;
	if(len % 4 == 0)
		tmp_len = len;
	else
		tmp_len = len + (4 - len%4);
	FLASH_Unlock();
	while(tmp_len)
	{
		ul_t.bb[0] = (*buf++);
		ul_t.bb[1] = (*buf++);
		ul_t.bb[2] = (*buf++);
		ul_t.bb[3] = (*buf++);
		FLASH_ProgramWord(addr, ul_t.hex);
		addr += 4;
		tmp_len -= 4;
	}
	FLASH_Lock();
}


/* 擦除 */
void flash_erase_page(INT32U addr)
{
	FLASH_Unlock();
	FLASH_ErasePage(addr);	
	FLASH_Lock();
}

/* 擦除一块连续的flash */
void falsh_erase_len(INT32U addr, INT32U len)
{
	INT32S tmp_len;
	tmp_len = len;
	while(tmp_len > 0)
	{
		flash_erase_page(addr);
		addr += FLASH_PAGE_SIZE;
		tmp_len -= FLASH_PAGE_SIZE;
	}
}

