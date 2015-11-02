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

#ifndef _FASTIP_H_
#define _FASTIP_H_


#include <linux/fastip/typedefs.h>
#include <linux/fastip/fastip_osl.h>


#if(FEATURE_ON == MBB_FEATURE_FASTIP_IPV6)
#ifndef FASTIP_IPV6
#define FASTIP_IPV6
#endif
#endif

#define FASTIP_NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]

#define fastip_maccmp(a, b)	((((const uint16 *)(a))[0] ^ ((const uint16 *)(b))[0]) | \
	                 (((const uint16 *)(a))[1] ^ ((const uint16 *)(b))[1]) | \
	                 (((const uint16 *)(a))[2] ^ ((const uint16 *)(b))[2]))
	                 
#define FASTIP_PPPOE_ETYPE_OFFSET	12
#define FASTIP_PPPOE_VER_OFFSET	14
#define FASTIP_PPPOE_SESID_OFFSET	16
#define FASTIP_PPPOE_LEN_OFFSET	18

#define FASTIP_PPPOE_HLEN		20
#define FASTIP_PPPOE_PPP_HLEN		8 //PPPOE + PPP HEADER LEN

#define FASTIP_PPPOE_PROT_PPP		0x0021

#define FASTIP_CONN_CACHEN_INCR(s) ((s)++)
#define FASTIP_CONN_CACHEN_ADD(s, c) ((s) += (c))

#define FASTIP_MAXBR_CONN_CACHE_HOT		4
#define FASTIP_MAXBR_CONN_CACHE_HOTIF		4

#define FASTIP_BR_CONN_CACHE_HOT_HASH(da) 	((((uint8 *)da)[4] ^ ((uint8 *)da)[5]) & (FASTIP_MAXBRCHOT - 1))

#define NF_FASTIP_ENABLED (1 << 31)
#define NF_IP_PRE_ROUTING 0
#define NF_IP_POST_ROUTING 4

#ifdef FASTIP_IPV6
#ifndef FASTIP_IPV6_ADDR_LEN
#define   FASTIP_IPV6_ADDR_LEN                     (16)      /*IPV6¦Ì??¡¤3¡è?¨¨*/
#endif
#endif

#ifdef FASTIP_IPV6
#define FASTIP_IPADDR_U32_SZ		(FASTIP_IPV6_ADDR_LEN / sizeof(uint32))
#else
#define FASTIP_IPADDR_U32_SZ		1
#endif

#ifdef FASTIP_IPV6
/* IPV6 extension headers (options) */
#define FASTIP_IPV6_EXTHDR_HOP		0
#define FASTIP_IPV6_EXTHDR_ROUTING	43
#define FASTIP_IPV6_EXTHDR_FRAGMENT	44
#define FASTIP_IPV6_EXTHDR_AUTH	51
#define FASTIP_IPV6_EXTHDR_NONE	59
#define FASTIP_IPV6_EXTHDR_DEST	60

#define FASTIP_IPV6_EXTHDR(prot)	(((prot) == FASTIP_IPV6_EXTHDR_HOP) || \
	                         ((prot) == FASTIP_IPV6_EXTHDR_ROUTING) || \
	                         ((prot) == FASTIP_IPV6_EXTHDR_FRAGMENT) || \
	                         ((prot) == FASTIP_IPV6_EXTHDR_AUTH) || \
	                         ((prot) == FASTIP_IPV6_EXTHDR_NONE) || \
	                         ((prot) == FASTIP_IPV6_EXTHDR_DEST))

#define FASTIP_IPV6_MIN_HLEN 		40

#define FASTIP_IPV6_EXTHDR_LEN(eh)	((((struct fastip_ipv6_exthdr *)(eh))->hdrlen + 1) << 3)


struct fastip_ipv6_exthdr {
	uint8	nexthdr;
	uint8	hdrlen;
};

struct fastip_ipv6_exthdr_frag {
	uint8	nexthdr;
	uint8	rsvd;
	uint16	frag_off;
	uint32	ident;
};
#endif

/* copy an ethernet address in reverse order */
#define	ether_rcopy(s, d) \
do { \
	((uint16 *)(d))[2] = ((uint16 *)(s))[2]; \
	((uint16 *)(d))[1] = ((uint16 *)(s))[1]; \
	((uint16 *)(d))[0] = ((uint16 *)(s))[0]; \
} while (0)

typedef struct fastip_mark 
{
    uint32	value;
}fastip_mark_t;


typedef struct fastip_nat 
{
	uint32	ip;
	uint16	port;
} fastip_nat_t;

struct fastip_conn_tuple {
	uint32	sip[FASTIP_IPADDR_U32_SZ], dip[FASTIP_IPADDR_U32_SZ];
	uint16	sp, dp;
	uint8	proto;
};

typedef struct fastip_conn_tuple	fastip_conn_tuple_t;

struct fastip_br_conn_cache 
{
	struct	fastip_br_conn_cache		*next;		/* Pointer to br_conn_cache entry */
	struct	fastip_ether_addr	dhost;		/* MAC addr of host */
	uint16			vid;		/* VLAN id to use on txif */
	void			*txifp;		/* Interface connected to host */
	void			*txvifp;		/* vlan Interface connected to host*/		
	uint32			action;		/* Tag or untag the frames */
	uint32			live;		/* Counter used to expire the entry */
	uint32			hits;		/* Num frames matching br_conn_cache entry */
	uint64			*bytecnt_ptr;	/* Pointer to the byte counter */
};

typedef struct fastip_br_conn_cache	fastip_br_conn_cache_t;

struct fastip_br_conn_cache_hot 
{
	struct fastip_ether_addr	ea;	/* Dest address */
	fastip_br_conn_cache_t		*pbr_conn_cache;	/* br_conn_cache entry corresp to dest mac */
};

typedef struct fastip_br_conn_cache_hot fastip_br_conn_cache_hot_t;

struct fastip_ip_conn_cache {
	struct	fastip_ip_conn_cache		*next;		/* Pointer to ip_conn_cache entry */
	fastip_conn_tuple_t	tuple;		/* Tuple to uniquely id the flow */
	uint16			vid;		/* VLAN id to use on txif */
	struct	fastip_ether_addr	dhost;		/* Destination MAC address */
	struct	fastip_ether_addr	shost;		/* Source MAC address */
	void			*txif;		/* Target interface to send */
	uint32			action;		/* NAT and/or VLAN actions */
	fastip_br_conn_cache_t		*pbr_conn_cache;		/* br_conn_cache entry corresp to source mac */
	uint32			live;		/* Counter used to expire the entry */
	struct	fastip_nat		nat;		/* Manip data for SNAT, DNAT */
	struct	fastip_ether_addr	sa;		/* MAC address of sender */
	uint8			tos;		/* IPv4 tos or IPv6 traffic class field with ECN cleared */
	uint16			pppoe_sid;	/* PPPOE session to use */
	void			*ppp_ifp;	/* PPP interface handle */
	uint32			hits;		/* Num frames matching ip_conn_cache entry */
	uint64			*bytecnt_ptr;	/* Pointer to the byte counter */
	struct	fastip_mark	mark;		/* Mark value to use for the connection */
};

typedef struct fastip_ip_conn_cache	fastip_ip_conn_cache_t;

struct fastip_ppp_sk {
	struct pppox_sock 		*po;		/*pointer to pppoe socket*/
	unsigned char			pppox_protocol;	/*0:pppoe/1:pptp*/
	struct	fastip_ether_addr		dhost;		/*Remote MAC addr of host the pppox socket is bound to*/
};

typedef struct fastip_ppp {
	struct fastip_ppp_sk		psk;
	unsigned short			pppox_id;	/*PPTP peer call id if wan type is pptp, PPPOE session ID if wan type is PPPOE*/
} ctf_ppp_t;


#define CTFVLSTATS

#define FASTIP_ENAB(fastip_handle)    (((fastip_handle) != NULL) && (fastip_handle)->fastip_enab)
#define FASTIPQOS_ULDL_DIFFIF(fastip_handle)    (((fastip_handle) != NULL) && ((fastip_handle)->fastip_enab == (1 << 1)))

#define FASTIP_ACTION_TAG		(1 << 0)
#define FASTIP_ACTION_UNTAG	(1 << 1)
#define FASTIP_ACTION_SNAT		(1 << 2)
#define FASTIP_ACTION_DNAT		(1 << 3)
#define FASTIP_ACTION_SUSPEND	(1 << 4)
#define FASTIP_ACTION_TOS		(1 << 5)
#define FASTIP_ACTION_MARK		(1 << 6)
#define FASTIP_ACTION_BYTECNT	(1 << 7)
#define FASTIP_ACTION_PPPOE_ADD	(1 << 8)
#define FASTIP_ACTION_PPPOE_DEL	(1 << 9)

/* Hot bridge cache lkup */

#define FASTIP_HOTBRC_CMP(hbr_conn_cache, da, rxifp) \
({ \
	fastip_br_conn_cache_hot_t *bh = (hbr_conn_cache) + FASTIP_BRC_HOT_HASH(da); \
	((fastip_maccmp((bh)->ea.octet, (da)) == 0) && (bh->pbr_conn_cache->txifp != (rxifp))); \
})

/* Header prep for packets matching hot bridge cache entry */
#define CTF_HOTBRC_L2HDR_PREP(osh, hbr_conn_cache, prio, data, p) \
do { \
	uint8 *l2h; \
	fastip_br_conn_cache_hot_t *bh = (hbr_conn_cache) + FASTIP_BRC_HOT_HASH(data); \
	ASSERT(*(uint16 *)((data) + VLAN_TPID_OFFSET) == HTON16(FASTIP_ETHER_TYPE_8021Q)); \
	if (bh->pbr_conn_cache->action & FASTIP_ACTION_UNTAG) { \
		/* Remove vlan header */ \
		l2h = FASTIPPULL((p), FASTIP_VLAN_TAG_LEN); \
		ether_rcopy(l2h - FASTIP_VLAN_TAG_LEN + FASTIP_ETHER_ADDR_LEN, \
		            l2h + FASTIP_ETHER_ADDR_LEN); \
		ether_rcopy(l2h - FASTIP_VLAN_TAG_LEN, l2h); \
	} else { \
		/* Update vlan header */ \
		l2h = (data); \
		*(uint16 *)(l2h + VLAN_TCI_OFFSET) = \
		            HTON16((prio) << FASTIP_VLAN_PRI_SHIFT | bh->pbr_conn_cache->vid); \
	} \
} while (0)

#define	fastip_attach(n, m, c, a) \
	(fastip_attach_fn ? fastip_attach_fn(n, m, c, a) : NULL)
	
#define fastip_forward(fastip_handle, p, d)	(fastip_handle)->fn.forward(fastip_handle, p, d)
#define fastip_isenabled(fastip_handle, d)	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.isenabled(fastip_handle, d) : FALSE)
#define fastip_isbridge(fastip_handle, d)	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.isbridge(fastip_handle, d) : FALSE)
#define fastip_enable(fastip_handle, d, e, b)	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.enable(fastip_handle, d, e, b) : FASTIP_OK)
#define fastip_br_conn_cache_add(fastip_handle, b)	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.br_conn_cache_add(fastip_handle, b) : FASTIP_OK)
#define fastip_br_conn_cache_delete(fastip_handle, e)	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.br_conn_cache_delete(fastip_handle, e) : FASTIP_OK)
#define fastip_br_conn_cache_update(fastip_handle, b)	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.br_conn_cache_update(fastip_handle, b) : FASTIP_OK)
#define fastip_br_conn_cache_lkup(fastip_handle, e)	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.br_conn_cache_lkup(fastip_handle, e) : NULL)
#define fastip_ip_conn_cache_add(fastip_handle, i, v6)	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.ip_conn_cache_add(fastip_handle, i, v6) : FASTIP_OK)
#define fastip_ip_conn_cache_delete(fastip_handle, i, v6)	\
	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.ip_conn_cache_delete(fastip_handle, i, v6) : FASTIP_OK)
#define fastip_ip_conn_cache_count_get(fastip_handle, i) \
	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.ip_conn_cache_count_get(fastip_handle, i) : FASTIP_OK)
#define fastip_ip_conn_cache_delete_multi(fastip_handle, i, im, v6)	\
	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.ip_conn_cache_delete_multi(fastip_handle, i, im, v6) : FASTIP_OK)
#define fastip_ip_conn_cache_delete_range(fastip_handle, s, e, v6)	\
	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.ip_conn_cache_delete_range(fastip_handle, s, e, v6) : FASTIP_OK)
#define fastip_ip_conn_cache_action(fastip_handle, s, e, am, v6) \
	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.ip_conn_cache_action(fastip_handle, s, e, am, v6) : FASTIP_OK)
#define fastip_ip_conn_cache_lkup(fastip_handle, i, v6)	\
	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.ip_conn_cache_lkup(fastip_handle, i, v6) : NULL)
#ifdef FASTIP_IPV6
#define fastip_ip_conn_cache_lkup_l4proto(fastip_handle, iph, l4p)	(FASTIP_ENAB(fastip_handle) && (fastip_handle)->fn.ip_conn_cache_lkup_l4proto? \
	(fastip_handle)->fn.ip_conn_cache_lkup_l4proto((uint8 *)iph, l4p) : NULL)
#else
#define fastip_ip_conn_cache_lkup_l4proto(fastip_handle, iph, l4p)	(NULL)
#endif /* FASTIP_IPV6 */
#define fastip_dev_register(fastip_handle, d, b, e)	\
	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.dev_register(fastip_handle, d, b, e) : FASTIP_OK)
#define fastip_dev_vlan_add(fastip_handle, d, vid, vd)	\
	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.dev_vlan_add(fastip_handle, d, vid, vd) : FASTIP_OK)
#define fastip_dev_vlan_delete(fastip_handle, d, vid)	\
	(FASTIP_ENAB(fastip_handle) ? (fastip_handle)->fn.dev_vlan_delete(fastip_handle, d, vid) : FASTIP_OK)
#define fastip_detach(fastip_handle)			if (FASTIP_ENAB(fastip_handle)) (fastip_handle)->fn.detach(fastip_handle)
#define fastip_dump(fastip_handle)			if (FASTIP_ENAB(fastip_handle)) (fastip_handle)->fn.dump(fastip_handle)
#define fastip_dev_unregister(fastip_handle, d)	if (FASTIP_ENAB(fastip_handle)) (fastip_handle)->fn.dev_unregister(fastip_handle, d)

typedef struct fastip_pub	fastip_t;
typedef void (*fastip_detach_cb_t)(fastip_t *fastip_handle, void *arg);
typedef fastip_t * (*fastip_attach_t)(uint8 *name, uint32 *msg_level,
                                fastip_detach_cb_t cb, void *arg);
typedef void (*fastip_detach_t)(fastip_t *fastip_handle);
typedef int32 (*fastip_forward_t)(fastip_t *fastip_handle, void *p, void *rxifp);
typedef int32 (*fastip_isenabled_t)(fastip_t *fastip_handle, void *dev);
typedef int32 (*fastip_isbridge_t)(fastip_t *fastip_handle, void *dev);
typedef int32 (*fastip_br_conn_cache_add_t)(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache);
typedef int32 (*fastip_br_conn_cache_delete_t)(fastip_t *fastip_handle, uint8 *ea);
typedef int32 (*fastip_br_conn_cache_update_t)(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache);
typedef fastip_br_conn_cache_t * (*fastip_br_conn_cache_lkup_t)(fastip_t *fastip_handle, uint8 *da);
typedef void (*fastip_br_conn_cache_release_t)(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache);
typedef int32 (*fastip_ip_conn_cache_add_t)(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache, int32 v6);
typedef int32 (*fastip_ip_conn_cache_delete_t)(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache, int32 v6);
typedef int32 (*fastip_ip_conn_cache_count_get_t)(fastip_t *fastip_handle);
typedef int32 (*fastip_ip_conn_cache_delete_multi_t)(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache,
                                        fastip_ip_conn_cache_t *ip_conn_cachem, int32 v6);
typedef int32 (*fastip_ip_conn_cache_delete_range_t)(fastip_t *fastip_handle, fastip_ip_conn_cache_t *start,
                                        fastip_ip_conn_cache_t *end, int32 v6);
typedef int32 (*fastip_ip_conn_cache_action_t)(fastip_t *fastip_handle, fastip_ip_conn_cache_t *start,
                                  fastip_ip_conn_cache_t *end, uint32 action_mask, int32 v6);
typedef fastip_ip_conn_cache_t * (*fastip_ip_conn_cache_lkup_t)(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache, int32 v6);
typedef	uint8 * (*fastip_ip_conn_cache_lkup_l4proto_t)(uint8 *iph, uint8 *proto_num);
typedef void (*fastip_ip_conn_cache_release_t)(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache);
typedef int32 (*fastip_enable_t)(fastip_t *fastip_handle, void *dev, int32 enable, fastip_br_conn_cache_hot_t **br_conn_cache_hot);
typedef int32 (*fastip_dev_register_t)(fastip_t *fastip_handle, void *dev, int32 isbridge, int32 en);
typedef void (*fastip_dev_unregister_t)(fastip_t *fastip_handle, void *dev);
typedef int32 (*fastip_dev_vlan_add_t)(fastip_t *fastip_handle, void *dev, uint16 vid, void *vldev);
typedef int32 (*fastip_dev_vlan_delete_t)(fastip_t *fastip_handle, void *dev, uint16 vid);
typedef void (*fastip_dump_t)(fastip_t *fastip_handle);

typedef struct fastip_fn 
{
	fastip_detach_t		detach;
	fastip_forward_t		forward;
	fastip_isenabled_t		isenabled;
	fastip_isbridge_t		isbridge;
	fastip_br_conn_cache_add_t		br_conn_cache_add;
	fastip_br_conn_cache_delete_t	br_conn_cache_delete;
	fastip_br_conn_cache_update_t	br_conn_cache_update;
	fastip_br_conn_cache_lkup_t		br_conn_cache_lkup;
    fastip_br_conn_cache_release_t	br_conn_cache_release;
	fastip_ip_conn_cache_add_t		ip_conn_cache_add;
	fastip_ip_conn_cache_delete_t	ip_conn_cache_delete;
	fastip_ip_conn_cache_count_get_t	ip_conn_cache_count_get;
	fastip_ip_conn_cache_delete_multi_t	ip_conn_cache_delete_multi;
	fastip_ip_conn_cache_delete_range_t	ip_conn_cache_delete_range;
#ifdef FASTIP_IPV6
	fastip_ip_conn_cache_lkup_l4proto_t ip_conn_cache_lkup_l4proto;
#endif
	fastip_ip_conn_cache_action_t	ip_conn_cache_action;
	fastip_ip_conn_cache_lkup_t		ip_conn_cache_lkup;
    fastip_ip_conn_cache_release_t	ip_conn_cache_release;
	fastip_enable_t		enable;
	fastip_dev_register_t	dev_register;
	fastip_dev_unregister_t	dev_unregister;
	fastip_detach_cb_t		detach_cb;
	void			*detach_cb_arg;
	fastip_dev_vlan_add_t	dev_vlan_add;
	fastip_dev_vlan_delete_t	dev_vlan_delete;
	fastip_dump_t		dump;
} fastip_fn_t;

struct fastip_pub 
{
	int32			fastip_enab;		/* Global FASTIP enable/disable */
	fastip_fn_t		fn;		/* Exported functions */
};

extern fastip_t *fastip_kattach(uint8 *name);
extern void fastip_kdetach(fastip_t *priv_fastip_handle);
extern fastip_attach_t fastip_attach_fn;
extern fastip_t *_fastip_attach(uint8 *name, uint32 *msg_level,
                          fastip_detach_cb_t cb, void *arg);
extern fastip_t *kfastip_handle;

#endif /* _FASTIP_H_ */

