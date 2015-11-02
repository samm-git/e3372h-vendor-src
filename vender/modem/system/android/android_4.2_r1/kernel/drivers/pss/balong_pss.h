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
#ifndef __BALONG_PSS_H
#define __BALONG_PSS_H


/*
 * BALONG SNAPSHOT driver data - see drivers/pss/balong_pss.c
 */


#include <linux/kprobes.h>
#include <linux/list.h>
#include <linux/spinlock_types.h>

struct pss_regs {
	int reg_offset;
	int sp_offset;
};

struct pss {
	struct list_head list;
	struct kprobe kp;
	unsigned int seq;
	
	unsigned int flags;
#define PSS_ENABLED		BIT(0)
#define PSS_PARA_STACK	BIT(1)
#define PSS_PARA_REG	BIT(2)
#define PSS_PARA_CUST	BIT(3)

	struct pss_regs arg;

	unsigned int count;
};
struct pss_record {
	/* with these two variables, the actual process scenario could be determined */
	unsigned long probe_addr; 
	unsigned long ret_addr;
	unsigned int timestamp;
	__be16  ipid;/* valid only for ipv4, set 0 for ipv6 */
	__be16 l3num;
	__be32 saddr;
	__be32 daddr;/* the last four bytes of the ip addr for IPv6 */
	unsigned int tcphdr[4]; /* the first 16 bytes of the tcphdr */
};

struct pss_flush_ops {
	unsigned int mtu;
	unsigned int (*pss_buffer_flush)(unsigned int rd_offset, 
		unsigned int wr_offset, char *buf, unsigned int size, unsigned int max);
};


struct pss_man {
	struct dentry *root;
	struct list_head list;
	struct timer_list timer;
	int 				first_record;
	int 				first_pss;
	struct net_device *wdev; /*wan device*/
	struct net_device *ldev; /*wan device*/
	__be32 dstip;
	spinlock_t lock;
	struct pss_record *records;
	unsigned int max; /* the maxium buffered record num */
	unsigned int cur_r; /* current read pointer of the buffer */
	unsigned int cur_w; /* current write pointer of the buffer */
	unsigned int full;
	unsigned int time_out;
	spinlock_t rw_lock;
	unsigned int seqnum;
	struct pss_flush_ops *ops;
	unsigned int debug;
#define PSS_DBG_TIME  BIT(0)
#define PSS_DBG_TCP  BIT(1)
#define PSS_DBG_UDP  BIT(2)
#define PSS_DBG_ICMP  BIT(3)

	unsigned int flags;
#define PSS_BUFFER_ROLLOVER BIT(0)
#define PSS_BUFFER_PROCESS_BUSY BIT(1)


};

extern void pss_set_flush_ops(struct pss_flush_ops *ops);
extern struct pss_man g_pss_man;

#define PSS_RECORD_MAX 0x100
#define PSS_ARM_SP 13

#endif
