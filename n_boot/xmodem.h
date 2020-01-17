
#ifndef _XMODEM_H_
#define _XMODEM_H_

//添加自己的数据类型定义
#include "all.h"


#define XM_DLY_1S	200		//在1S内xm_timer被调用的次数，小于65536
#define XM_RETRY	16		//retry次数

#define XM_SOH  	0x01	//Xmodem数据头
#define XM_STX  	0x02	//1K-xmodem数据头
#define XM_EOT  	0x04	//发送结束
#define XM_ACK  	0x06	//认可应答
#define XM_NAK  	0x15	//不认可应答
#define XM_CAN  	0x18	//丛机撤销传输
#define XM_EOF 		0x1A	//数据包填充

#define XM_OK		0
#define XM_ERR		-1

#define MAXRETRANS 	25

extern uint16 xmodem_timeout;

uint16 xm_crc16_ccitt(const uint8 *buf, sint32 len);
void xm_port_write(uint8 *ch);
sint8 xm_port_read(uint8 *ch);
void xm_timer(void);
void xm_disp(sint8 *str);
//返回实际接收的数据长度
sint32 xm_receive(uint8 *dest, sint32 destsz);
sint32 xm_transmit(uint8 *src, sint32 srcsz);







#endif /* _CRC16_H_ */
