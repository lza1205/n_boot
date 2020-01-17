#ifndef _N_COMMAND_H_
#define _N_COMMAND_H_

#ifdef _N_COMMAND_C_
#define _N_COMMAND_EXT_
#else
#define _N_COMMAND_EXT_ extern
#endif

#include "all.h"

#include "sys.h"
#include "cpu_h.h"
//#include "my_list.h"

/* 命令行结构体 */
struct command_t{
	INT8U *name;
	void (*com_fun)(void);
	INT8U tag_num;
	INT8U *tag_p;
//	struct list_head list;
};



_N_COMMAND_EXT_ struct command_t gt_comman[50];
_N_COMMAND_EXT_ INT8U command_index;
_N_COMMAND_EXT_ INT8U *gp_comman_tag[10];		//命令行参数
_N_COMMAND_EXT_ INT8U g_comman_num;		//命令行参数个数




void command_init(void);



#endif




