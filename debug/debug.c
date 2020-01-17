#define _DEBUG_C_
#include "all.h"

/***************************************************************
		该文件是 一个测试代码
***************************************************************/
#include "flash.h"

INT8U buf[64];

void debug_flash(void)
{
	flash_rd(0x8003000, buf, 64);
//	if(buf[0] != 0x87)
	{
		INT8U i;
		for(i = 0; i < 60; i ++)
		{
			buf[i] = i;
		}
		FLASH_Unlock();
		FLASH_ErasePage(0x8003000);
		FLASH_Lock();
		flash_wr(0x8003000, buf, 64);
	}
	flash_rd(0x8003000, buf, 64);
//	if(buf[0] != 0x88)
	{
		buf[0] = 0x87;
		buf[1] = 0x88;
		buf[2] = 0x89;
		FLASH_Unlock();
		FLASH_ErasePage(0x8003000);
		FLASH_Lock();
		flash_wr(0x8003000, buf, 64);
	}
	flash_rd(0x8003000, buf, 64);
}



