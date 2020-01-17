#ifndef _N_FLASH_H_
#define _N_FLASH_H_

#ifdef _N_FLASH_C_
#define _N_FLASH_EXT_
#else
#define _N_FLASH_EXT_ extern
#endif

#include "all.h"


#define FLASH_PAGE_SIZE			(2 * 1024)		/* һҳ2k */


void flash_rd(INT32U addr, INT8U *buf, INT32U len);
void flash_wr(INT32U addr, INT8U *buf, INT32U len);
void flash_erase_page(INT32U addr);
void falsh_erase_len(INT32U addr, INT32U len);






#endif


