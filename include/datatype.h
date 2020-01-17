#ifndef _DATAPYTE_H_
#define _DATAPYTE_H_

typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned short INT16U;
typedef signed short INT16S;
typedef unsigned long INT32U;
typedef signed long INT32S;
typedef float FP32;
typedef double FP64;
#define SET_B(x,y) (x|=(1<<y))
#define CLR_B(x,y) (x&=~(1<<y))

#define NULL (void *)0

union ulong_type{
	INT32U hex;
	INT16U ss[2];
	INT8U bb[4];
};

#define uint8  INT8U
#define uint16 INT16U
#define sint8  INT8S
#define sint32 INT32S
#define uint32 INT32U


#endif

