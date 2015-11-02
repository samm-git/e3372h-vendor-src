/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2013-2015. All rights reserved.
 *
 * mobile@huawei.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _fastip_osl_h_
#define _fastip_osl_h_

#include <linux/spinlock.h>

typedef spinlock_t fastip_lock_t; 
typedef spinlock_t fastip_osl_t;

#define	FASTIPPKTCCNT(skb)		(1)
#ifndef FASTIPPKTCLEN
#define	FASTIPPKTCLEN(skb)		FASTIPLEN(skb)
#endif
#include <linux/fastip/typedefs.h>

/* ASSERT */

#ifdef __GNUC__
	#define GCC_VERSION \
		(__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
	#if GCC_VERSION > 30100
		#define ASSERT(exp)	do {} while (0)
	#else
		/* ASSERT could cause segmentation fault on GCC3.1, use empty instead */
		#define ASSERT(exp)
	#endif /* GCC_VERSION > 30100 */
#endif /* __GNUC__ */

/* the largest reasonable packet buffer driver uses for ethernet MTU in bytes */
#define	FASTIPBUFSZ	2048   /* largest reasonable packet buffer, driver uses for ethernet MTU */

/*
 * BINOSL selects the slightly slower function-call-based binary compatible osl.
 * Macros expand to calls to functions defined in linux_osl.c .
 */
#include <linux/version.h>           /* use current 2.4.x calling conventions */
#include <linux/kernel.h>       /* for vsn/printf's */
#include <linux/string.h>       /* for mem*, str* */

#define	printf(fmt, args...)	printk(fmt , ## args)

/* bcopy's: Linux kernel doesn't provide these (anymore) */
#define	bcopy(src, dst, len)	memcpy((dst), (src), (len))
#define	bcmp(b1, b2, len)	memcmp((b1), (b2), (len))
#define	bzero(b, len)		memset((b), '\0', (len))


#define	FASTIPDATA(skb)		(((struct sk_buff*)(skb))->data)
#define	FASTIPLEN(skb)		(((struct sk_buff*)(skb))->len)
#define FASTIPHEADROOM(skb)		(FASTIPDATA(skb)-(((struct sk_buff*)(skb))->head))
#define FASTIPTAILROOM(skb)		skb_tailroom((struct sk_buff*)(skb))
#define	FASTIPNEXT(skb)		(((struct sk_buff*)(skb))->next)
#define	FASTIPSETNEXT(skb, x)		(((struct sk_buff*)(skb))->next = (struct sk_buff*)(x))
#define	FASTIPSETLEN(skb, len)	__skb_trim((struct sk_buff*)(skb), (len))
#define	FASTIPPUSH(skb, bytes)	skb_push((struct sk_buff*)(skb), (bytes))
#define	FASTIPPULL(skb, bytes)	skb_pull((struct sk_buff*)(skb), (bytes))
#define	FASTIPTAG(skb)			((void*)(((struct sk_buff*)(skb))->cb))
#define FASTIPSETPOOL(skb, x, y)	do {} while (0)
#define FASTIPPOOL(skb)		FALSE
#define FASTIPSHRINK(m)		(m)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 22)

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)

#define	FASTIPSKIPCT	(1 << 2)
#define	FASTIPCHAINED	(1 << 3)
#define	FASTIPSETSKIPCT(skb)	(((struct sk_buff*)(skb))->fastip_packet_flags |= FASTIPSKIPCT)
#define	FASTIPCLRSKIPCT(skb)	(((struct sk_buff*)(skb))->fastip_packet_flags &= (~FASTIPSKIPCT))
#define	FASTIPSKIPCONNCACHET(skb)	(((struct sk_buff*)(skb))->fastip_packet_flags & FASTIPSKIPCT)
#define	FASTIPSETCHAINED(skb)	(((struct sk_buff*)(skb))->fastip_packet_flags |= FASTIPCHAINED)
#define	FASTIPCLRCHAINED(skb)	(((struct sk_buff*)(skb))->fastip_packet_flags &= (~FASTIPCHAINED))
#define	FASTIPISCHAINED(skb)	(((struct sk_buff*)(skb))->fastip_packet_flags & FASTIPCHAINED)

#else
#define	FASTIPSKIPCT	(1 << 18)
#define	FASTIPCHAINED	(1 << 19)

#define	FASTIPSETSKIPCT(skb)	(((struct sk_buff*)(skb))->mac_len |= FASTIPSKIPCT)
#define	FASTIPCLRSKIPCT(skb)	(((struct sk_buff*)(skb))->mac_len &= (~FASTIPSKIPCT))
#define	FASTIPSKIPCONNCACHET(skb)	(((struct sk_buff*)(skb))->mac_len & FASTIPSKIPCT)
#define	FASTIPSETCHAINED(skb)	(((struct sk_buff*)(skb))->mac_len |= FASTIPCHAINED)
#define	FASTIPCLRCHAINED(skb)	(((struct sk_buff*)(skb))->mac_len &= (~FASTIPCHAINED))
#define	FASTIPISCHAINED(skb)	(((struct sk_buff*)(skb))->mac_len & FASTIPCHAINED)
#endif /* 2.6.36 */
#else /* 2.6.22 */

#define	FASTIPSKIPCT	(1 << 2)
#define	FASTIPCHAINED	(1 << 3)
#define	FASTIPSETSKIPCT(skb)	(((struct sk_buff*)(skb))->__unused |= FASTIPSKIPCT)
#define	FASTIPCLRSKIPCT(skb)	(((struct sk_buff*)(skb))->__unused &= (~FASTIPSKIPCT))
#define	FASTIPSKIPCONNCACHET(skb)	(((struct sk_buff*)(skb))->__unused & FASTIPSKIPCT)
#define	FASTIPSETCHAINED(skb)	(((struct sk_buff*)(skb))->__unused |= FASTIPCHAINED)
#define	FASTIPCLRCHAINED(skb)	(((struct sk_buff*)(skb))->__unused &= (~FASTIPCHAINED))
#define	FASTIPISCHAINED(skb)	(((struct sk_buff*)(skb))->__unused & FASTIPCHAINED)

#endif /* 2.6.22 */

#define FASTIP_MARK(m)				(m.value)

#define	FASTIPLINK(skb)			(((struct sk_buff*)(skb))->prev)
#define	FASTIPSETLINK(skb, x)		(((struct sk_buff*)(skb))->prev = (struct sk_buff*)(x))
#define	FASTIPPRIO(skb)			(((struct sk_buff*)(skb))->priority)
#define	FASTIPSETPRIO(skb, x)		(((struct sk_buff*)(skb))->priority = (x))
#define FASTIPSUMNEEDED(skb)		(((struct sk_buff*)(skb))->ip_summed == CHECKSUM_HW)
#define FASTIPSETSUMGOOD(skb, x)		(((struct sk_buff*)(skb))->ip_summed = \
						                ((x) ? CHECKSUM_UNNECESSARY : CHECKSUM_NONE))

#define FASTIPSHARED(skb)                 (((struct sk_buff*)(skb))->cloned)

#ifdef CONFIG_NF_CONNTRACK_MARK
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
#define FASTIPMARK(p)                     (((struct sk_buff *)(p))->mark)
#define FASTIPSETMARK(p, m)               ((struct sk_buff *)(p))->mark = (m)
#else /* !2.6.0 */
#define FASTIPMARK(p)                     (((struct sk_buff *)(p))->nfmark)
#define FASTIPSETMARK(p, m)               ((struct sk_buff *)(p))->nfmark = (m)
#endif /* 2.6.0 */
#else /* CONFIG_NF_CONNTRACK_MARK */
#define FASTIPMARK(p)                     0
#define FASTIPSETMARK(p, m)
#endif /* CONFIG_NF_CONNTRACK_MARK */


#endif

