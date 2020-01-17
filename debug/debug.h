#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef _DEBUG_C_
#define _DEBUG_EXT_
#else
#define _DEBUG_EXT_ extern
#endif

#include "all.h"


void debug_flash(void);



#endif
