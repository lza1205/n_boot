

#include "xmodem.h"
#include "datatype.h"
#include "boot.h"

static sint32 last_error = 0;
uint16 xmodem_timeout = 0;

/****************Portting Start *******************/


void xm_outbyte(uint8 trychar)
{
	uint8 buf[2];
	buf[0] = trychar;
	xm_port_write(buf);
}

//void xm_timer(void)
//{
//	xmodem_timeout++;
//}

uint8 xm_inbyte(uint32 time_out)
{
	uint8 ch;

	last_error = 0;

	//超时管理
	xmodem_timeout = 0;
	while(xmodem_timeout < time_out)
	{
		if(xm_port_read(&ch) == 1)
			return ch;
	}

	last_error = 1;
	return ch;
}

void *xm_memcpy(void *destaddr, void const *srcaddr, sint32 len)
{
  	uint8 *dest = destaddr;
  	uint8 const *src = srcaddr;

  	while(len-- > 0)
   		*dest++ = *src++ ;
  	return destaddr;
}

void *xm_memset(void *src, sint32 c, sint32 count)
{
	 uint8 *tmpsrc=(uint8*)src;
	 while(count--)
	  	*tmpsrc++ =(uint8)c;
	 return src;
}

/****************Portting End*******************/
static sint32 xm_check(sint32 crc, const uint8 *buf, sint32 sz)
{
	if (crc) 
	{
		uint16 crc = xm_crc16_ccitt(buf, sz);
		uint16 tcrc = (buf[sz]<<8)+buf[sz+1];
		if (crc == tcrc)
			return 1;
	}
	else 
	{
		sint32 i;
		uint8 cks = 0;
		for (i = 0; i < sz; ++i) 
		{
			cks += buf[i];
		}
		if (cks == buf[sz])
		return 1;
	}

	return 0;
}

static void xm_flushinput(void)
{
	//xm_inbyte(0);
	;
}

void xm_disp(sint8 *str)
{
	while(*str)
	{
		xm_outbyte(*str++);	
	}
}


uint8 xm_buf[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
sint32 xm_receive(uint8 *dest, sint32 destsz)
{

	uint8 *p;
	sint32 bufsz, crc = 0;
	uint8 trychar = 'C';
	uint8 packetno = 1;
	sint32 i, c, len = 0;
	sint32 retry, retrans = MAXRETRANS;

	INT32U dec_addr;
	dec_addr = gt_boot_var.kernel_addr_base;

	xm_disp("\n\rStart Xmodem Receive!\n\r");
	for(;;) 
	{
		for( retry = 0; retry < XM_RETRY; ++retry) 
		{
			if (trychar) 
				xm_outbyte(trychar);
			c = xm_inbyte((XM_DLY_1S)<<1);
			if (last_error == 0) 
			{
				switch (c) 
				{
					case XM_SOH:
						bufsz = 128;
						goto start_recv;
					case XM_STX:
						bufsz = 1024;
						goto start_recv;
					case XM_EOT:
						xm_flushinput();
						xm_outbyte(XM_ACK);
						return len; /* normal end */
					case XM_CAN:
						c = xm_inbyte(XM_DLY_1S);

						if (c == XM_CAN) 
						{
							xm_flushinput();
							xm_outbyte(XM_ACK);
							return -1; /* canceled by remote */
						}
						break;
					default:
						break;
				}
			}
		}
		if (trychar == 'C') 
		{ 
			trychar = XM_NAK; 
			continue; 
		}
		xm_flushinput();
		xm_outbyte(XM_CAN);
		xm_outbyte(XM_CAN);
		xm_outbyte(XM_CAN);
		return -2; /* sync error */

	start_recv:
		if (trychar == 'C') crc = 1;
		trychar = 0;
		p = xm_buf;
		*p++ = c;
		for (i = 0;  i < (bufsz+(crc?1:0)+3); ++i) 
		{
			c = xm_inbyte(XM_DLY_1S);

			if (last_error != 0) 
				goto reject;
			*p++ = c;
		}

		if (xm_buf[1] == (uint8)(~xm_buf[2]) && 
			(xm_buf[1] == packetno || xm_buf[1] == (uint8)packetno-1) &&
			xm_check(crc, &xm_buf[3], bufsz)) 
		{
			if (xm_buf[1] == packetno)	
			{
//				sint32 count = destsz - len;
//				if (count > bufsz) 
//					count = bufsz;
//				if (count > 0) 
				{
					//xm_memcpy (&dest[len], &xm_buf[3], count);
/******************************************************************************
					烧写至 flash
******************************************************************************/					
					flash_wr(dec_addr, &xm_buf[3], bufsz);
					flash_rd(dec_addr, flash_buf, bufsz);
					
					dec_addr += bufsz;
//					len += count;
				}
				++packetno;
				retrans = MAXRETRANS+1;
			}
			if (--retrans <= 0) 
			{
				xm_flushinput();
				xm_outbyte(XM_CAN);
				xm_outbyte(XM_CAN);
				xm_outbyte(XM_CAN);
				return -3; /* too many retry error */
			}
			xm_outbyte(XM_ACK);
			continue;
		}
	reject:
		xm_flushinput();
		xm_outbyte(XM_NAK);
	}
}

sint32 xm_transmit(uint8 *src, sint32 srcsz)
{
	uint8 xm_buf[1030]; /* 1024 for XModem 1k + 3 head chars + 2 crc + nul */
	sint32 bufsz, crc = -1;
	uint8 packetno = 1;
	sint32 i, c, len = 0;
	sint32 retry;

	for(;;) {
		for( retry = 0; retry < 16; ++retry) 
		{
			c = xm_inbyte((XM_DLY_1S)<<1);
			if (last_error == 0) 
			{
				switch (c) 
				{
					case 'C':
						crc = 1;
						goto start_trans;
					case XM_NAK:
						crc = 0;
						goto start_trans;
					case XM_CAN:
						c = xm_inbyte(XM_DLY_1S);
						if (c == XM_CAN) 
						{
							xm_outbyte(XM_ACK);
							xm_flushinput();
							return -1; /* canceled by remote */
						}
						break;
					default:
						break;
				}
			}
		}
		xm_outbyte(XM_CAN);
		xm_outbyte(XM_CAN);
		xm_outbyte(XM_CAN);
		xm_flushinput();
		return -2; /* no sync */

		for(;;) 
		{
		start_trans:
			xm_buf[0] = XM_SOH; bufsz = 128;
			xm_buf[1] = packetno;
			xm_buf[2] = ~packetno;
			c = srcsz - len;
			if (c > bufsz) c = bufsz;
			if (c >= 0) 
			{
				xm_memset (&xm_buf[3], 0, bufsz);
				if (c == 0) 
				{
					xm_buf[3] = XM_EOF;
				}
				else 
				{
					xm_memcpy (&xm_buf[3], &src[len], c);
					if (c < bufsz) xm_buf[3+c] = XM_EOF;
				}
				if (crc) 
				{
					uint16 ccrc = xm_crc16_ccitt(&xm_buf[3], bufsz);
					xm_buf[bufsz+3] = (ccrc>>8) & 0xFF;
					xm_buf[bufsz+4] = ccrc & 0xFF;
				}
				else 
				{
					uint8 ccks = 0;
					for (i = 3; i < bufsz+3; ++i) 
					{
						ccks += xm_buf[i];
					}
					xm_buf[bufsz+3] = ccks;
				}
				for (retry = 0; retry < MAXRETRANS; ++retry) 
				{
					for (i = 0; i < bufsz+4+(crc?1:0); ++i) 
					{
						xm_outbyte(xm_buf[i]);
					}
					c = xm_inbyte(XM_DLY_1S);
					if (last_error == 0 ) 
					{
						switch (c) 
						{
							case XM_ACK:
								++packetno;
								len += bufsz;
								goto start_trans;
							case XM_CAN:
								c = xm_inbyte(XM_DLY_1S);
								if ( c == XM_CAN) 
								{
									xm_outbyte(XM_ACK);
									xm_flushinput();
									return -1; /* canceled by remote */
								}
								break;
							case XM_NAK:
							default:
								break;
						}
					}
				}
				xm_outbyte(XM_CAN);
				xm_outbyte(XM_CAN);
				xm_outbyte(XM_CAN);
				xm_flushinput();
				return -4; /* xmit error */
			}
			else 
			{
				for (retry = 0; retry < 10; ++retry) 
				{
					xm_outbyte(XM_EOT);
					c = xm_inbyte((XM_DLY_1S)<<1);
					if (c == XM_ACK) break;
				}
				xm_flushinput();
				return (c == XM_ACK)?len:-5;
			}
		}
	}
}



