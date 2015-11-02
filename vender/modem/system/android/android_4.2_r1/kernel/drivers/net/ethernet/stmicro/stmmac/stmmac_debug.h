/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and 
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may 
 * *    be used to endorse or promote products derived from this software 
 * *    without specific prior written permission.
 * 
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef __STMMAC_DEBUG__
#define __STMMAC_DEBUG__

#include <linux/kernel.h>
#include <linux/skbuff.h>

/* gmac msg level */
#define GMAC_LEVEL_ERR	            BIT(0)
#define GMAC_LEVEL_WARNING          BIT(1)
#define GMAC_LEVEL_TRACE	        BIT(2)
#define GMAC_LEVEL_DBG	            BIT(3)
#define GMAC_LEVEL_INFO	            BIT(4)
#define GMAC_LEVEL_BUG	            BIT(5)
#define GMAC_LEVEL_CHIP_DBG         BIT(6)
#define GMAC_LEVEL_CHIP_INFO        BIT(7)
#define GMAC_LEVEL_SKB	            BIT(8)
#define GMAC_LEVEL_BYTE	            BIT(9)
#define GMAC_DWMAC_LIB_DBG	        BIT(10)
#define GMAC_LEVEL_RX_DBG	        BIT(11)
#define GMAC_LEVEL_TX_DBG	        BIT(12)

extern unsigned int gmac_msg_level;

#define GMAC_ERR(args)			do {if (gmac_msg_level & GMAC_LEVEL_ERR) printk args;} while (0)
#define GMAC_WARNING(args)			do {if (gmac_msg_level & GMAC_LEVEL_WARNING) printk args;} while (0)
#define GMAC_BUG(args,condition)          \
    do {\
        if (condition)\
        {\
            printk args;\
        }\
\
        if (gmac_msg_level & GNET_LEVEL_BUG)\
        {\
            BUG_ON(condition);\
        }\
    } while (0)

#define CONFIG_GMAC_DEBUG
#ifdef CONFIG_GMAC_DEBUG
#define GMAC_TRACE(args)		    do {if (gmac_msg_level & GMAC_LEVEL_TRACE) printk args;} while (0)
#define GMAC_DBG(args)			    do {if (gmac_msg_level & GMAC_LEVEL_DBG) printk args;} while (0)
#define GMAC_INFO(args)		        do {if (gmac_msg_level & GMAC_LEVEL_INFO) printk args;} while (0)
#define GMAC_CHIP_DBG(args)		    do {if (gmac_msg_level & GMAC_LEVEL_CHIP_DBG) printk args;} while (0)
#define GMAC_CHIP_INFO(args)		do {if (gmac_msg_level & GMAC_LEVEL_CHIP_INFO) printk args;} while (0)
#define DWMAC_LIB_DBG(args)         do {if (gmac_msg_level & GMAC_DWMAC_LIB_DBG) printk args;} while (0)
#define GMAC_RX_DBG(args)           do {if (gmac_msg_level & GMAC_LEVEL_RX_DBG) printk args;} while (0)
#define GMAC_TX_DBG(args)           do {if (gmac_msg_level & GMAC_LEVEL_TX_DBG) printk args;} while (0)


static inline void print_bytes(char *data, u32 len)
{
    if(gmac_msg_level & GMAC_LEVEL_BYTE)
    {
        u32 cnt = 0;

        printk("------pkt start------\n");
        for(cnt=0; cnt<len; cnt++)
        {
            printk("%02x ",data[cnt]);
        }
        printk("------pkt end  ------\n");
    }
}

static inline void print_pkt(unsigned char *buf, int len)
{
    if(gmac_msg_level & GMAC_LEVEL_SKB) {
    	int j;
    	printk("len = %d byte, buf addr: 0x%p", len, buf);
    	for (j = 0; j < len; j++) {
        	if ((j % 16) == 0)
    	    	printk("\n %03x:", j);
    	    printk(" %02x", buf[j]);
        }
        printk("\n");
    }
}

#else   /* CONFIG_GMAC_DEBUG */
#define GMAC_TRACE(args)
#define GMAC_DBG(args)
#define GMAC_INFO(args)
#define GMAC_CHIP_DBG(args)
#define GMAC_CHIP_INFO(args)
#define DWMAC_LIB_DBG(args)
#define GMAC_RX_DBG(args)
#define GMAC_TX_DBG(args)

static inline void print_bytes(char *data, u32 len)
{   return ;    }
static inline void print_skb(struct eth_dev *dev, struct sk_buff *skb)
{   return ;    }
static inline int gnet_rx_isdrop(struct sk_buff *skb)
{   return 0;   }
#endif  /* CONFIG_GMAC_DEBUG */

#endif /* __STMMAC_DEBUG__ */
