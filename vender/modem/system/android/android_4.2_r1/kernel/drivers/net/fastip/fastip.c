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
/**********************问题单修改记录******************************************
 日    期          修改人         问题单号           修改内容


******************************************************************************/

/* 普通的IP首部为20字节 */

/*-------------------------------------------------------------------
 |      0        |       1        |       2        |       3        |
 |---------------|----------------|----------------|----------------|
 |0|1|2|3|4|5|6|7||0|1|2|3|4|5|6|7||0|1|2|3|4|5|6|7||0|1|2|3|4|5|6|7|
  --------------------------------------------------------------------
 | 协议  | 首部  |    服务类型    |            总长度               |
 | 版本  | 长度  |     (TOS)      |           (字节数)              |
  --------------------------------------------------------------------
 |            16位标识            | 标志 |         13位片偏移       |
  --------------------------------------------------------------------
 | 生存时间(TTL) |    8位协议     |        16位首部校验和           |
  --------------------------------------------------------------------
 |                           32位源IP地址                           |
  --------------------------------------------------------------------
 |                           32位目的IP地址                         |
  --------------------------------------------------------------------
 */
#include <linux/module.h>
#include <linux/ipv6.h>
#include <linux/fastip/fastip.h>
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_acct.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_zones.h>
#include <net/netfilter/nf_conntrack_timestamp.h>
#include <net/netfilter/nf_conntrack_timeout.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#include <net/ip.h>
#include <net/route.h>
#ifdef CONFIG_IPV6
#include <linux/ipv6.h>
#include <net/ipv6.h>
#include <net/ip6_route.h>
#define IPVERSION_IS_4(ipver) ((ipver) == 4)
#else
#if(FEATURE_ON == FASTIP_IPV6)
#define IPVERSION_IS_4(ipver)    1
#else
#define IPVERSION_IS_4(ipver)
#endif
#endif

#include "fastip_module.h"

#if (FEATURE_ON == MBB_FEATURE_FASTIP)

#define FASTIP__VERSION	"2013-12-05 10:32 v1.0"

#define NIP_ISMULTI(a) (((a) & 0x000000f0) == 0x000000e0)

/* Private action flags */
#define FASTIP_ACTION_FWD (1 << 31)

#define FASTIP_VID_MAX 32
#define FASTIP_VLC_MAX 8

#define FASTIP_NAME_SZ 8       /* 8 char names */

#define FASTIP_BR_CONN_CACHE_SZ 32
#define FASTIP_IP_CONN_CACHE_SZ 128

/* See if a is in the range of l to h */
#define FASTIP_REQ(a, l, h) ((((a) >= (l)) && ((a) <= (h)))    \
   || (((l) == FASTIP_NUM_0) && ((h) == FASTIP_NUM_0)))

/* Match x and y using the mask argument m */
#define FASTIP_NE(a, b, c) (((a) ^ (b)) & (c))

#define FASTIP_OSH(fastip_handle) ((fastip_info_t *)(fastip_handle))->osh
#define FASTIP_MSGLVL(fastip_handle) ((fastip_info_t *)(fastip_handle))->msg_level
#define FASTIP_BR_CONN_CACHEP(fastip_handle) ((fastip_info_t *)(fastip_handle))->br_conn_cache
#define FASTIP_IP_CONN_CACHEP(fastip_handle) ((fastip_info_t *)(fastip_handle))->ip_conn_cache
#define FASTIP_DEV_LIST_HEAD(fastip_handle) ((fastip_info_t *)(fastip_handle))->dev_list->head
#define FASTIP_VLDEV(fastip_handle, i, v) (((fastip_info_t *)(fastip_handle))->vlc[(i * FASTIP_VLC_MAX) + (v)])
#define FASTIP_BR_CONN_CACHE_HOT(fastip_handle, i, j) \
(((fastip_info_t *)(fastip_handle))->br_conn_cache_hot[(((i) & (FASTIP_MAXBR_CONN_CACHE_HOTIF - 1)) * FASTIP_MAXBR_CONN_CACHE_HOT) + (j)])
#define FASTIP_BR_CONN_CACHE_HOT_UPDATE(fastip_handle, ri, da, pbr_conn_cache) \
do { \
    FASTIP_BR_CONN_CACHE_HOT(fastip_handle, FASTIP_DEV_IFIDX(ri), FASTIP_BR_CONN_CACHE_HOT_HASH(da)).ea = \
        *(struct fastip_ether_addr *)(da); \
    FASTIP_BR_CONN_CACHE_HOT(fastip_handle, FASTIP_DEV_IFIDX(ri), FASTIP_BR_CONN_CACHE_HOT_HASH(da)).pbr_conn_cache = pbr_conn_cache; \
} while (0)

#define FASTIP_DEBUG
int debug_flag = 0;
#ifdef FASTIP_DEBUG
    #define FASTIP_TRACE_ERROR(fmt, args...) if (debug_flag){printf(fmt, ## args);}
    #define FASTIP_TRACE(fmt, args...) if (debug_flag){printf(fmt, ## args);}
#else
    #define FASTIP_TRACE_ERROR(fmt, args...)
    #define FASTIP_TRACE(fmt, args...)
#endif


#ifdef FASTIP_IPV6
#define IP_CONN_CACHE_TPL_CMP(p, v6, sip, dip, proto, sp, dp) \
    (!(((v6) ? memcmp(p->tuple.sip, sip, FASTIP_NUM_32) : \
        (p->tuple.sip[FASTIP_NUM_0] ^ sip[FASTIP_NUM_0]) | \
        (p->tuple.dip[FASTIP_NUM_0] ^ dip[FASTIP_NUM_0])) | \
       ((p->tuple.proto ^ proto) | \
        (p->tuple.sp ^ sp) | \
        (p->tuple.dp ^ dp))))
#else
#define IP_CONN_CACHE_TPL_CMP(p, v6, sip, dip, proto, sp, dp) \
    (!((p->tuple.sip[FASTIP_NUM_0] ^ sip[FASTIP_NUM_0]) | \
       (p->tuple.dip[FASTIP_NUM_0] ^ dip[FASTIP_NUM_0]) | \
       (p->tuple.proto ^ proto) | \
       (p->tuple.sp ^ sp) | \
       (p->tuple.dp ^ dp)))
#endif/* FASTIP_IPV6 */

#ifdef FASTIP_IPV6
#define IP_CONN_CACHE_HASH(v6, sip, dip, sp, dp, proto) \
    ((((v6) ? (sip[FASTIP_NUM_0] + sip[FASTIP_NUM_1] + sip[FASTIP_NUM_2] + sip[FASTIP_NUM_3] + \
               dip[FASTIP_NUM_0] + dip[FASTIP_NUM_1] + dip[FASTIP_NUM_2] + dip[FASTIP_NUM_3]) : \
               (sip[FASTIP_NUM_0] + dip[FASTIP_NUM_0])) + \
                sp + dp + proto) & (FASTIP_IP_CONN_CACHE_SZ - 1))
#else
#define IP_CONN_CACHE_HASH(v6, sip, dip, sp, dp, proto) \
    ((sip[FASTIP_NUM_0] + dip[FASTIP_NUM_0] + sp + dp + proto) & (FASTIP_IP_CONN_CACHE_SZ - FASTIP_NUM_1))
#endif/* FASTIP_IPV6 */

#ifdef FASTIP_IPV6
#define IS_V4ADDR(ipaddr) \
    ((ipaddr[FASTIP_NUM_1] | ipaddr[FASTIP_NUM_2] | ipaddr[FASTIP_NUM_3]) == 0)
#else
#define IS_V4ADDR(ipaddr) 1
#endif/* FASTIP_IPV6 */

#ifdef FASTIP_IPV6
#define IS_V6ADDR_NULL(addr) \
    (((addr)[FASTIP_NUM_0] | (addr)[FASTIP_NUM_1] | (addr)[FASTIP_NUM_2] | (addr)[FASTIP_NUM_3]) == 0)
#endif/* FASTIP_IPV6 */

#define FASTIP_EA_CMP(p, ea) \
    (!((((uint16 *)p->dhost.octet)[FASTIP_NUM_0] ^ ((uint16 *)(ea))[FASTIP_NUM_0]) | \
       (((uint16 *)p->dhost.octet)[FASTIP_NUM_1] ^ ((uint16 *)(ea))[FASTIP_NUM_1]) | \
       (((uint16 *)p->dhost.octet)[FASTIP_NUM_2] ^ ((uint16 *)(ea))[FASTIP_NUM_2])))

#define FASTIP_HASH(ea) \
    ((ea[FASTIP_NUM_5] + ea[FASTIP_NUM_4] + ea[FASTIP_NUM_3] + ea[FASTIP_NUM_2] + \
    ea[FASTIP_NUM_1]) & (FASTIP_BR_CONN_CACHE_SZ - FASTIP_NUM_1))

void *fastip_malloc(uint size)
{
    void *addr = NULL;

    if (NULL == (addr = kmalloc(size, GFP_KERNEL)))
    {
        return (NULL);
    }

    return (addr);
}

#define MALLOC(size) fastip_malloc(size)
#define MFREE(ptr) kfree(ptr)

typedef struct fastip_dev  fastip_dev_t;
typedef struct fastip_info fastip_info_t;

typedef struct fastip_dev_list
{
    fastip_dev_t    *head;      /* Pointer to head */
    fastip_lock_t   *lock;      /* Lock for devices list */
} fastip_dev_list_t;

struct fastip_info
{
    fastip_t                     fastip; /* Public structure */
    fastip_osl_t       *         osh;
    fastip_br_conn_cache_t  **   br_conn_cache;     /* Bridge cache table */
    fastip_lock_t   *            br_conn_cache_lock; /* Lock for bridge cache */
    fastip_br_conn_cache_hot_t  *br_conn_cache_hot; /* Hot bridge cache */
    fastip_ip_conn_cache_t  **   ip_conn_cache;     /* IP CONNECTION CACHE table */
    fastip_lock_t   *            ip_conn_cache_lock; /* Lock for ip conn cache */
    fastip_dev_list_t   *        dev_list; /* List of fastip enabled devices */
    void        **               vlc; /* VLAN device cache for fast lkup */
    uint32                       ip_conn_cache_count; /* Count of IP CONNECTION CACHE entries */
    uint32      *                msg_level; /* Message level pointer */
    uint8                        name[FASTIP_NAME_SZ]; /* Callers name for diag msgs */
    fastip_info_t   *            next; /* List of fastip context structs */
};

/* Device specific information */
typedef struct vlinfo
{
    uint16   vid;           /* VLAN Id of the device */
    uint16   reserv;
    void    *vldev;         /* VLAN device pointer */
} vlinfo_t;

struct fastip_dev
{
    fastip_dev_t    *next;      /* Pointer to next entry */
    int32           enabled; /* fastip enable/disable status */
    int32           is_br; /* True if device is bridge */
    fastip_t        *fastip_handle;        /* Pointer to fastip info */
    void            *dev;   /* Device identifier/pointer */
    vlinfo_t         vlinfo[FASTIP_VID_MAX]; /* VLAN info */
};

static void     _fastip_detach(fastip_t *fastip_handle);
static int32    _fastip_forward(fastip_t *fastip_handle, void *p, void *rxifp);
static int32     _fastip_isenabled(fastip_t *fastip_handle, void *dev);
static int32     _fastip_isbridge(fastip_t *fastip_handle, void *dev);
static int32    _fastip_br_conn_cache_add(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache);
static int32    _fastip_br_conn_cache_delete(fastip_t *fastip_handle, uint8 *ea);
static int32    _fastip_br_conn_cache_update(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache);
static fastip_br_conn_cache_t * _fastip_br_conn_cache_lkup(fastip_t *fastip_handle, uint8 *da);
static void     _fastip_br_conn_cache_release(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache);
static int32    _fastip_ip_conn_cache_add(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache, int32 v6);
static int32    _fastip_ip_conn_cache_delete(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache,
                                                int32 v6);
static int32    _fastip_ip_conn_cache_count_get(fastip_t *fastip_handle);
static fastip_ip_conn_cache_t * _fastip_ip_conn_cache_lkup(fastip_t *fastip_handle, int32 v6, uint32 *sip, uint32 *dip,
                                                            uint8 proto, uint16 sp, uint16 dp);
static fastip_ip_conn_cache_t * _fastip_ip_conn_cache_lkup_fn(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache,
                                                            int32 v6);
#ifdef FASTIP_IPV6
static uint8 *    _fastip_ip_conn_cache_lkup_l4proto_v6(uint8 *ip6, uint8 *proto_num);
#endif/* FASTIP_IPV6 */
static void     _fastip_ip_conn_cache_release(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache);
static int32    _fastip_enable(fastip_t *fastip_handle, void *dev, int32 enable,
                                fastip_br_conn_cache_hot_t **br_conn_cache_hot);
static void     _fastip_dump(fastip_t *fastip_handle);
static int32    _fastip_dev_vlan_add(fastip_t *fastip_handle, void *dev, uint16 vid, void *vldev);
static int32    _fastip_dev_vlan_delete(fastip_t *fastip_handle, void *dev, uint16 vid);
static int32    _fastip_dev_register(fastip_t *fastip_handle, void *dev, int32 isbridge, int32 enable);
static void     _fastip_dev_unregister(fastip_t *fastip_handle, void *dev);
static void     _fastip_dev_list_cleanup(fastip_t *fastip_handle);
static void     _fastip_ip_conn_cache_detach_pbr_conn_cache(fastip_t *fastip_handle,
                                                            fastip_br_conn_cache_t *pbr_conn_cache);
static void     _fastip_br_conn_cache_hot_invalidate(fastip_t *fastip_handle,
                                                    fastip_br_conn_cache_t *pbr_conn_cache);
static fastip_dev_t *_fastip_dev_lkup(fastip_t *fastip_handle, void *dev);
static void     _fastip_br_conn_cache_cleanup(fastip_t *fastip_handle);
static void     _fastip_ip_conn_cache_cleanup(fastip_t *fastip_handle);
static void     _fastip_detach_all(fastip_t *priv_fastip_handle);
static void     _fastip_br_conn_cache_delete_by_dev(fastip_t *fastip_handle, void *dev);
static void     _fastip_ip_conn_cache_delete_by_dev(fastip_t *fastip_handle, void *dev);
static int32    _fastip_ip_conn_cache_delete_multi(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache,
                                                                   fastip_ip_conn_cache_t *ip_conn_cachem, int32 v6);
static int32    _fastip_ip_conn_cache_delete_range(fastip_t *fastip_handle, fastip_ip_conn_cache_t *start,
                                                                   fastip_ip_conn_cache_t *end, int32 v6);
static int32    _fastip_ip_conn_cache_action(fastip_t *fastip_handle, fastip_ip_conn_cache_t *start,
                                                             fastip_ip_conn_cache_t *end, uint32 action_mark, int32 v6);

static fastip_info_t priv_fastip_handle;
static fastip_info_t *ci_head;

extern fastip_t *kfastip_handle;

/*fastip 运行开关*/
unsigned int fastip_run_flag = 0;
EXPORT_SYMBOL(fastip_run_flag);

/*fastip ttl开关*/
static int fastip_ttl_flag = 1 ;

/*fastip wifi流控生效开关*/
static int fastip_flow_ctl_flag = 0;
/* Default fastip message level */
static uint fastip_msg_level =
#ifdef FASTIP_DEBUG_ERR
    1;
#else
    0;
#endif

/***************************************内核通用函数***************************************/
/*-------------------------------------------------
函数原型： char *fastip_ether_ntoa(const struct fastip_ether_addr *addr, char *buf)
描    述： 格式化输出mac地址
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
char *fastip_ether_ntoa(const struct fastip_ether_addr *addr, char *buf)
{
    static const char template[] = "%02x:%02x:%02x:%02x:%02x:%02x";

    if ((NULL == buf) || (NULL == addr))
    {
        return NULL;
    }

    snprintf(buf, FASTIP_NUM_18, template,
             addr->octet[FASTIP_NUM_0] & FASTIP_NUM_FF, addr->octet[FASTIP_NUM_1] & FASTIP_NUM_FF, \
             addr->octet[FASTIP_NUM_2] & FASTIP_NUM_FF, addr->octet[FASTIP_NUM_3] & FASTIP_NUM_FF, \
             addr->octet[FASTIP_NUM_4] & FASTIP_NUM_FF, addr->octet[FASTIP_NUM_5] & FASTIP_NUM_FF);
    return (buf);
}

EXPORT_SYMBOL(fastip_ether_ntoa);



#ifndef FASTIP_IPV6
/*-------------------------------------------------
函数原型： void br_fdb_put(struct net_bridge_fdb_entry *ent)
描    述： 删除网桥转发记录,并刷新RCU
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void br_fdb_put(struct net_bridge_fdb_entry *ent)
{
    //if (atomic_dec_and_test(&ent->use_count)) 
    {
#if(FEATURE_ON == MBB_FEATURE_FASTIP)
        fastip_br_conn_cache_delete(kfastip_handle, ent->addr.addr);
#endif /* MBB_FEATURE_FASTIP */
        call_rcu(&ent->rcu, fdb_rcu_free);
    }
}
EXPORT_SYMBOL(br_fdb_put);
#endif 

/*-------------------------------------------------
函数原型： static void fastip_br_cache_init(fastip_br_conn_cache_t *br_conn_cache, \
    unsigned char *ea, struct net_device *rxdev)
描    述： 初始化网桥记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void fastip_br_cache_init(fastip_br_conn_cache_t *br_conn_cache, \
    unsigned char *ea, struct net_device *rxdev)
{
    memset(br_conn_cache, 0, sizeof(fastip_br_conn_cache_t));
    memcpy(br_conn_cache->dhost.octet, ea, ETH_ALEN);
#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
    if (rxdev->priv_flags & IFF_802_1Q_VLAN) 
    {
        br_conn_cache->txifp = (void *)(VLAN_DEV_INFO(rxdev)->real_dev);
        br_conn_cache->vid = VLAN_DEV_INFO(rxdev)->vlan_id;
        br_conn_cache->action = ((VLAN_DEV_INFO(rxdev)->flags & 1) ?
                     FASTIP_ACTION_TAG : FASTIP_ACTION_UNTAG);
    } 
    else 
#endif
    {
        br_conn_cache->txifp = (void *)rxdev;
#ifdef FASTIP_IPV6
        br_conn_cache->txvifp = NULL;
#endif

        br_conn_cache->action = FASTIP_ACTION_UNTAG;
    }

#ifdef FASTIP_DEBUG
    FASTIP_TRACE("mac %02x:%02x:%02x:%02x:%02x:%02x\n",
           br_conn_cache->dhost.octet[0], br_conn_cache->dhost.octet[1],
           br_conn_cache->dhost.octet[2], br_conn_cache->dhost.octet[3],
           br_conn_cache->dhost.octet[4], br_conn_cache->dhost.octet[5]);
    FASTIP_TRACE("vid: %d action %x\n", br_conn_cache->vid, br_conn_cache->action);
    FASTIP_TRACE("txif: %s\n", ((struct net_device *)br_conn_cache->txifp)->name);
#endif

    return;
}

#ifdef FASTIP_IPV6
/*-------------------------------------------------
函数原型： void fastip_br_cache_add(unsigned char *ea, struct net_device *rxdev)
描    述： 添加网桥记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_br_cache_add(unsigned char *ea, struct net_device *rxdev)
{
    fastip_br_conn_cache_t br_conn_cache_entry;

    /* Add br_conn_cache entry only if packet is received on fastip 
    * enabled interface
    */
    if (!fastip_isenabled(kfastip_handle, ((rxdev->priv_flags & IFF_802_1Q_VLAN) ?
                       vlan_dev_real_dev(rxdev) : rxdev)))
    {
        return;
    }

    fastip_br_cache_init(&br_conn_cache_entry, ea, rxdev);

#ifdef FASTIP_DEBUG
    FASTIP_TRACE("%s: Adding br_conn_cache entry\n", __FUNCTION__);
#endif

    /* Add the bridge cache entry */
    if (fastip_br_conn_cache_lkup(kfastip_handle, ea) == NULL)
        fastip_br_conn_cache_add(kfastip_handle, &br_conn_cache_entry);
    else
        fastip_br_conn_cache_update(kfastip_handle, &br_conn_cache_entry);

    return;
}
EXPORT_SYMBOL(fastip_br_cache_add);

/*-------------------------------------------------
函数原型： void fastip_br_cache_update(unsigned char *ea, struct net_device *rxdev)
描    述： 更新网桥记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_br_cache_update(unsigned char *ea, struct net_device *rxdev)
{
    fastip_br_conn_cache_t br_conn_cache_entry;

    /* Update br_conn_cache entry only if packet is received on fastip
     * enabled interface
     */
    if (!fastip_isenabled(kfastip_handle, ((rxdev->priv_flags & IFF_802_1Q_VLAN) ?
                       vlan_dev_real_dev(rxdev) : rxdev)))
        return;

    /* Initialize the new device and/or vlan info */
    fastip_br_cache_init(&br_conn_cache_entry, ea, rxdev);

#ifdef FASTIP_DEBUG
    FASTIP_TRACE("%s: Updating br_conn_cache entry\n", __FUNCTION__);
#endif

    /* Update the bridge cache entry */
    fastip_br_conn_cache_update(kfastip_handle, &br_conn_cache_entry);

    return;
}
EXPORT_SYMBOL(fastip_br_cache_update);
#else
/*-------------------------------------------------
函数原型： void fastip_br_conn_cache_add(unsigned char *ea, struct net_device *rxdev)
描    述： 添加网桥记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_br_cache_add(unsigned char *ea, struct net_device *rxdev)
{
    fastip_br_conn_cache_t br_conn_cache_entry;

    /* Add br_conn_cache entry only if packet is received on fastip 
     * enabled interface
    */

#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
    if (!fastip_isenabled(kfastip_handle, ((rxdev->priv_flags & IFF_802_1Q_VLAN) ?
                       VLAN_DEV_INFO(rxdev)->real_dev : rxdev)))
        return;
#else
    if (!fastip_isenabled(kfastip_handle, rxdev))
        return;

#endif

    fastip_br_cache_init(&br_conn_cache_entry, ea, rxdev);

    FASTIP_TRACE("%s: Adding br_conn_cache entry\n", __FUNCTION__);

    /* Add the bridge cache entry */
    if (fastip_br_conn_cache_lkup(kfastip_handle, ea) == NULL)
    {
        FASTIP_TRACE("fastip_br_conn_cache_add %s\n",ea);
        fastip_br_conn_cache_add(kfastip_handle, &br_conn_cache_entry);
    }
    else
    {
        FASTIP_TRACE("fastip_br_conn_cache_update %s\n",ea);
        fastip_br_conn_cache_update(kfastip_handle, &br_conn_cache_entry);
    }
    return;
}
EXPORT_SYMBOL(fastip_br_cache_add);

/*-------------------------------------------------
函数原型： void fastip_br_conn_cache_update(unsigned char *ea, struct net_device *rxdev)
描    述： 更新网桥记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_br_cache_update(unsigned char *ea, struct net_device *rxdev)
{
    fastip_br_conn_cache_t br_conn_cache_entry;

    /* Update br_conn_cache entry only if packet is received on fastip 
    * enabled interface
    */
#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
    if (!fastip_isenabled(kfastip_handle, ((rxdev->priv_flags & IFF_802_1Q_VLAN) ?
                       VLAN_DEV_INFO(rxdev)->real_dev : rxdev)))
        return;
#else
    if (!fastip_isenabled(kfastip_handle, rxdev))
        return;
#endif

    /* Initialize the new device and/or vlan info */
    fastip_br_cache_init(&br_conn_cache_entry, ea, rxdev);

    FASTIP_TRACE("%s: Updating br_conn_cache entry\n", __FUNCTION__);

    /* Update the bridge cache entry */
    fastip_br_conn_cache_update(kfastip_handle, &br_conn_cache_entry);

    return;
}
EXPORT_SYMBOL(fastip_br_cache_update);
#endif
/*-------------------------------------------------
函数原型： bool fastip_is_ip_conn_cache_allowed(struct sk_buff *skb, u_int32_t hooknum)
描    述： 是否允许加速
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
bool fastip_is_ip_conn_cache_allowed(struct sk_buff *skb, u_int32_t hooknum)
{
    struct net_device *dev;

    if (!FASTIP_ENAB(kfastip_handle))
    {
        FASTIP_TRACE("%s: kfastip_handle:%x not allow\n", __FUNCTION__ ,kfastip_handle);
        return FALSE;
    }
    if (hooknum == NF_INET_PRE_ROUTING || hooknum == NF_INET_POST_ROUTING) 
    {
        dev = skb->dev;
        if (dev->priv_flags & IFF_802_1Q_VLAN)
        {
            dev = vlan_dev_real_dev(dev);
        }
        /* Add ip_conn_cache entry if packet is received on fastip enabled interface
        * and the packet is not a defrag'd one.
        */
        if (fastip_isenabled(kfastip_handle, dev) && (skb->len <= dev->mtu))
        {
            skb->fastip_nfflags |= NF_FASTIP_ENABLED;
        }
    }

    /* Add the cache entries only if the device has registered and
    * enabled fastip.
    */
    if (skb->fastip_nfflags & NF_FASTIP_ENABLED)
    {
        return TRUE;
    }
    FASTIP_TRACE("%s: fastip_is_ip_conn_cache_allowed not allow\n", __FUNCTION__ );
    return FALSE;
}

EXPORT_SYMBOL(fastip_is_ip_conn_cache_allowed);

/*-------------------------------------------------
函数原型： void fastip_ip_conn_cachet_add(struct sk_buff *skb, u_int32_t hooknum,
                    struct nf_conn *ct, enum ip_conntrack_info ci,
                    struct nf_conntrack_tuple *manip)
描    述： 添加加速记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_ip_conn_cachet_add(struct sk_buff *skb, u_int32_t hooknum,
                    struct nf_conn *ct, enum ip_conntrack_info ci,
                    struct nf_conntrack_tuple *manip)
{
    fastip_ip_conn_cache_t ip_conn_cache_entry;
    struct hh_cache *hh;
    struct ethhdr *eth;
    struct iphdr *iph;
    struct tcphdr *tcph;
    u_int32_t daddr;
    struct rtable *rt;
    struct nf_conn_help *help;
    enum ip_conntrack_dir dir;
    __u8 ipver,protocol;
#ifdef CONFIG_IPV6
    struct ipv6hdr *ip6h = NULL;
#endif
    if ((skb == NULL) || (ct == NULL))
    {
         return;
    }

    /* Check fastip enabled */
    if (!fastip_is_ip_conn_cache_allowed(skb, hooknum))
    {
        FASTIP_TRACE("%s: fastip_is_ip_conn_cache_allowed not allow\n", __FUNCTION__);
        return;
    }
    /* We only add cache entires for non-helper connections and at
    * pre or post routing hooks.
    */
    help = nfct_help(ct);
    if ((help && help->helper) || (ct->fastip_flag & FASTIP_FLAGS_EXCLUDED) ||
        ((hooknum != NF_INET_PRE_ROUTING) && (hooknum != NF_INET_POST_ROUTING)))
    {
        FASTIP_TRACE("%s: nfct_help help:%x hooknum:%x\n", __FUNCTION__,
            help, hooknum);
        return;
    }

    iph = ip_hdr(skb);
    ipver = iph->version;
    FASTIP_TRACE("%s: ipver:%x\n", __FUNCTION__, ipver);
    /* Support both IPv4 and IPv6 */
    if (ipver == 4) 
    {
        tcph = ((struct tcphdr *)(((__u8 *)iph) + (iph->ihl << 2)));
        protocol = iph->protocol;
    }
#ifdef CONFIG_IPV6
    else if (ipver == 6) 
    {
        ip6h = (struct ipv6hdr *)iph;
        tcph = (struct tcphdr *)fastip_ip_conn_cache_lkup_l4proto(kfastip_handle, ip6h, &protocol);
        if (tcph == NULL)
        {
             return;
        }
    }
#endif
    else
    {
        return;
    }
    FASTIP_TRACE("%s: protocol protocol:%x\n", __FUNCTION__, protocol);
    if(protocol == IPPROTO_TCP)
    {
        if((ci!=IP_CT_ESTABLISHED)&&(ci !=(IP_CT_ESTABLISHED+IP_CT_IS_REPLY)))
        {
             return;
        }
        if(ct->proto.tcp.state >=TCP_CONNTRACK_FIN_WAIT &&
         ct->proto.tcp.state <= TCP_CONNTRACK_TIME_WAIT)
        {
             return;
        }
    }
    else if(protocol !=IPPROTO_UDP)
    {
        return;
    }

    dir = CTINFO2DIR(ci);

    if (ct->fastip_flag & (1 << dir))
    {
        FASTIP_TRACE("%s: ct->fastip_flag:%x (1 << dir):%x\n", __FUNCTION__,
            ct->fastip_flag, (1 << dir));
        return;
    }

    if(skb_dst(skb) == NULL)
    {
        if(IPVERSION_IS_4(ipver))
        {
            if((manip != NULL)&&(HOOK2MANIP(hooknum) == NF_NAT_MANIP_DST))
            {
                daddr = manip->dst.u3.ip;
            }
            else
            {
                daddr = iph->daddr;
            }
            ip_route_input(skb,daddr,iph->saddr,iph->tos,skb->dev);
        }
#ifdef CONFIG_IPV6
        else
        {
            ip6_route_input(skb);
        }
#endif
    }

    /* Ensure the packet belongs to a forwarding connection and it is
    * destined to an unicast address.
    */
    rt = (struct rtable *)skb_dst(skb);
    if ((rt == NULL) || (
#ifdef CONFIG_IPV6
            !IPVERSION_IS_4(ipver) ?
            ((rt->dst.input != ip6_forward) ||
            !(ipv6_addr_type(&ip6h->daddr) & IPV6_ADDR_UNICAST)) :
#endif
            ((rt->dst.input != ip_forward) || (rt->rt_type != RTN_UNICAST))) ||
            (rt->dst._neighbour == NULL) ||
            ((rt->dst._neighbour->nud_state &
                (NUD_PERMANENT|NUD_REACHABLE|NUD_STALE|NUD_DELAY|NUD_PROBE)) == 0))
    {
        #if 0
        printk("%s: skb_dst rt:%x rt->dst.input:%x ip_forward:%x rt->rt_type:%x RTN_UNICAST:%x\n", __FUNCTION__,
            rt, rt->dst.input, ip_forward, rt->rt_type, RTN_UNICAST);
        printk("%s: rt->dst._neighbour:%x rt->dst._neighbour->nud_state:%x NUD:%x\n", __FUNCTION__,
            rt->dst._neighbour, rt->dst._neighbour->nud_state, (NUD_PERMANENT|NUD_REACHABLE|NUD_STALE|NUD_DELAY|NUD_PROBE));
        #endif

        return;
    }
    memset(&ip_conn_cache_entry, 0, sizeof(ip_conn_cache_entry));

    /* Init the neighboring sender address */
    memcpy(ip_conn_cache_entry.sa.octet, eth_hdr(skb)->h_source, ETH_ALEN);

    /* If the packet is received on a bridge device then save
    * the bridge cache entry pointer in the ip cache entry.
    * This will be referenced in the data path to update the
    * live counter of br_conn_cache entry whenever a received packet
    * matches corresponding ip_conn_cache entry matches.
    */
    /*
    if ((skb->dev != NULL) && fastip_isbridge(kfastip_handle, skb->dev))
        ip_conn_cache_entry.pbr_conn_cache = fastip_br_conn_cache_lkup(kfastip_handle, eth_hdr(skb)->h_source);
    */
    ip_conn_cache_entry.pbr_conn_cache = NULL;
    hh = &(skb_dst(skb)->_neighbour->hh);
    if (hh != NULL) 
    {
        eth = (struct ethhdr *)(((unsigned char *)hh->hh_data) + 2);
        memcpy(ip_conn_cache_entry.dhost.octet, eth->h_dest, ETH_ALEN);
        memcpy(ip_conn_cache_entry.shost.octet, eth->h_source, ETH_ALEN);
    } 
    else 
    {
        memcpy(ip_conn_cache_entry.dhost.octet, rt->dst._neighbour->ha, ETH_ALEN);
        memcpy(ip_conn_cache_entry.shost.octet, skb_dst(skb)->dev->dev_addr, ETH_ALEN);
    }

    if(IPVERSION_IS_4(ipver))
    {
        ip_conn_cache_entry.tuple.sip[0] =iph->saddr;
        ip_conn_cache_entry.tuple.dip[0] =iph->daddr;
#ifdef CONFIG_IPV6
    } 
    else
    {
        memcpy(ip_conn_cache_entry.tuple.sip,&ip6h->saddr,sizeof(ip_conn_cache_entry.tuple.sip));
        memcpy(ip_conn_cache_entry.tuple.dip,&ip6h->daddr,sizeof(ip_conn_cache_entry.tuple.dip));

#endif
    }
    ip_conn_cache_entry.tuple.proto = protocol;
    ip_conn_cache_entry.tuple.sp = tcph->source;
    ip_conn_cache_entry.tuple.dp = tcph->dest;

    ip_conn_cache_entry.next = NULL;

    /* For vlan interfaces fill the vlan id and the tag/untag actions */
    if (skb_dst(skb)->dev->priv_flags & IFF_802_1Q_VLAN) {
        ip_conn_cache_entry.txif = (void *)(vlan_dev_real_dev(skb_dst(skb)->dev));
        ip_conn_cache_entry.vid = vlan_dev_vlan_id(skb_dst(skb)->dev);
        //ip_conn_cache_entry.action =( (vlan_dev_vlan_flags(skb_dst(skb)->dev) & 1) ?
        //                    FASTIP_ACTION_TAG : FASTIP_ACTION_UNTAG);
    } 
    else 
    {
        ip_conn_cache_entry.txif = skb_dst(skb)->dev;
        ip_conn_cache_entry.action = FASTIP_ACTION_UNTAG;
    }

    /* Update the manip ip and port */
    if (manip != NULL) 
    {
        if (HOOK2MANIP(hooknum) == NF_NAT_MANIP_SRC) 
        {
            ip_conn_cache_entry.nat.ip = manip->src.u3.ip;
            ip_conn_cache_entry.nat.port = manip->src.u.tcp.port;
            ip_conn_cache_entry.action |= FASTIP_ACTION_SNAT;
        } 
        else 
        {
            ip_conn_cache_entry.nat.ip = manip->dst.u3.ip;
            ip_conn_cache_entry.nat.port = manip->dst.u.tcp.port;
            ip_conn_cache_entry.action |= FASTIP_ACTION_DNAT;
        }
    }

    /* Do bridge cache lookup to determine outgoing interface
    * and any vlan tagging actions if needed.
    */
    if(!strcmp(((struct net_device *)ip_conn_cache_entry.txif)->name,"br0"))
    {
        if (fastip_isbridge(kfastip_handle, ip_conn_cache_entry.txif)) 
        {
            fastip_br_conn_cache_t *pbr_conn_cache;

            pbr_conn_cache = fastip_br_conn_cache_lkup(kfastip_handle, ip_conn_cache_entry.dhost.octet);
            FASTIP_TRACE("%s: fastip_br_conn_cache_lkup proto:%x pbr_conn_cache:%x\n", __FUNCTION__,
                ip_conn_cache_entry.tuple.proto,
                pbr_conn_cache);
            if (pbr_conn_cache == NULL)
            {
                return;
            }
            else 
            {
                ip_conn_cache_entry.action |= pbr_conn_cache->action;
                ip_conn_cache_entry.txif = pbr_conn_cache->txifp;
                ip_conn_cache_entry.vid = pbr_conn_cache->vid;
            }
        }
    }
#ifdef FASTIP_DEBUG
    if (IPVERSION_IS_4(ipver))
    {
        FASTIP_TRACE("%s: Adding ip_conn_cache entry for [%d]%u.%u.%u.%u:%u - %u.%u.%u.%u:%u\n", __FUNCTION__,
            ip_conn_cache_entry.tuple.proto,
            FASTIP_NIPQUAD(ip_conn_cache_entry.tuple.sip[0]), ntohs(ip_conn_cache_entry.tuple.sp),
            FASTIP_NIPQUAD(ip_conn_cache_entry.tuple.dip[0]), ntohs(ip_conn_cache_entry.tuple.dp));
    }
#ifdef CONFIG_IPV6
    else
    {
        FASTIP_TRACE("\n%s: Adding ip_conn_cache entry for [%d]\n"
            "%08x.%08x.%08x.%08x:%u => %08x.%08x.%08x.%08x:%u\n",
            __FUNCTION__, ip_conn_cache_entry.tuple.proto,
            ntohl(ip_conn_cache_entry.tuple.sip[0]), ntohl(ip_conn_cache_entry.tuple.sip[1]),
            ntohl(ip_conn_cache_entry.tuple.sip[2]), ntohl(ip_conn_cache_entry.tuple.sip[3]),
            ntohs(ip_conn_cache_entry.tuple.sp),
            ntohl(ip_conn_cache_entry.tuple.dip[0]), ntohl(ip_conn_cache_entry.tuple.dip[1]),
            ntohl(ip_conn_cache_entry.tuple.dip[2]), ntohl(ip_conn_cache_entry.tuple.dip[3]),
            ntohs(ip_conn_cache_entry.tuple.dp));
    }
#endif /* CONFIG_IPV6 */
    FASTIP_TRACE("sa %02x:%02x:%02x:%02x:%02x:%02x\n",
        ip_conn_cache_entry.shost.octet[0], ip_conn_cache_entry.shost.octet[1],
        ip_conn_cache_entry.shost.octet[2], ip_conn_cache_entry.shost.octet[3],
        ip_conn_cache_entry.shost.octet[4], ip_conn_cache_entry.shost.octet[5]);
    FASTIP_TRACE("da %02x:%02x:%02x:%02x:%02x:%02x\n",
        ip_conn_cache_entry.dhost.octet[0], ip_conn_cache_entry.dhost.octet[1],
        ip_conn_cache_entry.dhost.octet[2], ip_conn_cache_entry.dhost.octet[3],
        ip_conn_cache_entry.dhost.octet[4], ip_conn_cache_entry.dhost.octet[5]);
    FASTIP_TRACE("[%d] vid: %d action %x\n", hooknum, ip_conn_cache_entry.vid, ip_conn_cache_entry.action);
    if (manip != NULL)
    {
        FASTIP_TRACE("manip_ip: %u.%u.%u.%u manip_port %u\n",
            FASTIP_NIPQUAD(ip_conn_cache_entry.nat.ip), ntohs(ip_conn_cache_entry.nat.port));
    }
    FASTIP_TRACE("txif: %s\n", ((struct net_device *)ip_conn_cache_entry.txif)->name);
#endif
    fastip_ip_conn_cache_add(kfastip_handle, &ip_conn_cache_entry,!IPVERSION_IS_4(ipver));

    /* Update the attributes flag to indicate a fastip conn */
    ct->fastip_flag |= (FASTIP_FLAGS_CACHED | (1 << dir));


}

EXPORT_SYMBOL(fastip_ip_conn_cachet_add);

/*-------------------------------------------------
函数原型： int fasip_ip_conn_cachet_delete(struct nf_conn *ct, int ct_timeout)
描    述： 删除加速记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
int fasip_ip_conn_cachet_delete(struct nf_conn *ct, int ct_timeout)
{
    fastip_ip_conn_cache_t *ip_conn_cachet;
    struct nf_conntrack_tuple *orig, *repl;
    fastip_ip_conn_cache_t orig_ip_conn_cachet,repl_ip_conn_cachet;
    int ipaddr_sz;
    bool v6;

    if (!FASTIP_ENAB(kfastip_handle))
    {
         return 0;
    }

    orig = &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple;

    if ((orig->dst.protonum != IPPROTO_TCP) && (orig->dst.protonum != IPPROTO_UDP))
    {
         return 0;
    }

    repl = &ct->tuplehash[IP_CT_DIR_REPLY].tuple;

#ifdef CONFIG_IPV6
    v6=(orig->src.l3num == AF_INET6);
    ipaddr_sz = (v6)?sizeof(struct in6_addr):sizeof(struct in_addr);
#else
    v6=FALSE;
    ipaddr_sz=sizeof(struct in_addr);
#endif

    memset(&orig_ip_conn_cachet,0,sizeof(orig_ip_conn_cachet));
    memcpy(orig_ip_conn_cachet.tuple.sip,&orig->src.u3.ip,ipaddr_sz);
    memcpy(orig_ip_conn_cachet.tuple.dip,&orig->dst.u3.ip,ipaddr_sz);
    orig_ip_conn_cachet.tuple.proto = orig->dst.protonum;
    orig_ip_conn_cachet.tuple.sp = orig->src.u.tcp.port;
    orig_ip_conn_cachet.tuple.dp = orig->dst.u.tcp.port;

    memset(&repl_ip_conn_cachet,0,sizeof(repl_ip_conn_cachet));
    memcpy(repl_ip_conn_cachet.tuple.sip,&repl->src.u3.ip,ipaddr_sz);
    memcpy(repl_ip_conn_cachet.tuple.dip,&repl->dst.u3.ip,ipaddr_sz);
    repl_ip_conn_cachet.tuple.proto=repl->dst.protonum;
    repl_ip_conn_cachet.tuple.sp = repl->src.u.tcp.port;
    repl_ip_conn_cachet.tuple.dp=repl->dst.u.tcp.port;
    /* If the refresh counter of ip_conn_cache entry is non zero, it indicates
    * that the packet transfer is active and we should not delete
    * the conntrack entry.
    */
    if (ct_timeout) 
    {
        ip_conn_cachet = fastip_ip_conn_cache_lkup(kfastip_handle, &orig_ip_conn_cachet, v6);

        /* Postpone the deletion of ct entry if there are frames
        * flowing in this direction.
        */
        if ((ip_conn_cachet != NULL) && (ip_conn_cachet->live > 0)) 
        {
            ip_conn_cachet->live = 0;
            ct->timeout.expires = jiffies + ct->fastip_expire_jiffies;
            add_timer(&ct->timeout);
            return -1;
        }

        ip_conn_cachet = fastip_ip_conn_cache_lkup(kfastip_handle, &repl_ip_conn_cachet,v6);

        if ((ip_conn_cachet != NULL) && (ip_conn_cachet->live > 0)) 
        {
            ip_conn_cachet->live = 0;
            ct->timeout.expires = jiffies + ct->fastip_expire_jiffies;
            add_timer(&ct->timeout);
            return -1;
        }
    }

    /* If there are no packets over this connection for timeout period
    * delete the entries.
    */
    fastip_ip_conn_cache_delete(kfastip_handle, &orig_ip_conn_cachet,v6);

    fastip_ip_conn_cache_delete(kfastip_handle, &repl_ip_conn_cachet,v6);
    
#ifdef FASTIP_DEBUG
    FASTIP_TRACE("%s: Deleting the tuple %x %x %d %d %d\n",
            __FUNCTION__, orig->src.u3.ip, orig->dst.u3.ip, orig->dst.protonum,
           orig->src.u.tcp.port, orig->dst.u.tcp.port);
    FASTIP_TRACE("%s: Deleting the tuple %x %x %d %d %d\n",
           __FUNCTION__, repl->dst.u3.ip, repl->src.u3.ip, repl->dst.protonum,
           repl->dst.u.tcp.port, repl->src.u.tcp.port);
#endif

    return 0;
}

EXPORT_SYMBOL(fasip_ip_conn_cachet_delete);

/***************************************接口函数***************************************/
/*-------------------------------------------------
函数原型： fastip_t *_fastip_attach(uint8 *name, uint32 *msg_level, fastip_detach_cb_t cb, void *arg)
描    述： 创建并初始化fastip_t 操作句柄
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
fastip_t *_fastip_attach(uint8 *name, uint32 *msg_level, fastip_detach_cb_t cb, void *arg)
{
    fastip_info_t *fastip_handle = NULL;
    int32 size;
    fastip_osl_t *osh = 0xaabbccdd;

    ASSERT((osh != NULL) && (name != NULL) && (msg_level != NULL));

    /* Allocate memory for br_conn_cache and ip_conn_cache hash tables */
    if ((priv_fastip_handle.br_conn_cache == NULL) && (priv_fastip_handle.ip_conn_cache == NULL)
       && (priv_fastip_handle.dev_list == NULL) && (priv_fastip_handle.vlc == NULL) && (priv_fastip_handle.br_conn_cache_hot == NULL))
    {
        if ((priv_fastip_handle.br_conn_cache = MALLOC(FASTIP_BR_CONN_CACHE_SZ * sizeof(fastip_br_conn_cache_t *))) == NULL)
        {
            FASTIP_TRACE_ERROR("%s: br_conn_cache malloc failed\n", __FUNCTION__);
            return (NULL);
        }

        bzero((char *)priv_fastip_handle.br_conn_cache, (FASTIP_BR_CONN_CACHE_SZ * sizeof(fastip_br_conn_cache_t *)));

        if ((priv_fastip_handle.ip_conn_cache = MALLOC(FASTIP_IP_CONN_CACHE_SZ * sizeof(fastip_ip_conn_cache_t *))) == NULL)
        {
            FASTIP_TRACE_ERROR("%s: ip_conn_cache malloc failed\n", __FUNCTION__);
            MFREE(priv_fastip_handle.br_conn_cache);
            return (NULL);
        }

        bzero((char *)priv_fastip_handle.ip_conn_cache, (FASTIP_IP_CONN_CACHE_SZ * sizeof(fastip_ip_conn_cache_t *)));

        if ((priv_fastip_handle.dev_list = MALLOC(sizeof(fastip_dev_list_t))) == NULL)
        {
            FASTIP_TRACE_ERROR("%s: dev_list malloc failed\n", __FUNCTION__);
            MFREE(priv_fastip_handle.br_conn_cache);
            MFREE(priv_fastip_handle.ip_conn_cache);
            return (NULL);
        }

        bzero((char *)priv_fastip_handle.dev_list, sizeof(fastip_dev_list_t));

        size = sizeof(void *) * FASTIP_VLC_MAX * FASTIP_VLC_MAX;
        if ((priv_fastip_handle.vlc = MALLOC(size)) == NULL)
        {
            FASTIP_TRACE_ERROR("%s: vldev cache malloc failed\n", __FUNCTION__);
            MFREE(priv_fastip_handle.dev_list);
            MFREE(priv_fastip_handle.br_conn_cache);
            MFREE(priv_fastip_handle.ip_conn_cache);
            return (NULL);
        }

        bzero((char *)priv_fastip_handle.vlc, size);

        size = FASTIP_MAXBR_CONN_CACHE_HOTIF * FASTIP_MAXBR_CONN_CACHE_HOT * sizeof(fastip_br_conn_cache_hot_t);
        if ((priv_fastip_handle.br_conn_cache_hot = MALLOC(size)) == NULL)
        {
            FASTIP_TRACE_ERROR("%s: br_conn_cache hot malloc failed\n", __FUNCTION__);
            MFREE(priv_fastip_handle.vlc);
            MFREE(priv_fastip_handle.dev_list);
            MFREE(priv_fastip_handle.br_conn_cache);
            MFREE(priv_fastip_handle.ip_conn_cache);
            return (NULL);
        }

        bzero((char *)priv_fastip_handle.br_conn_cache_hot, size);

        /* Alloc and init the br_conn_cache ip_conn_cache and dev locks */
        FASTIP_LOCK_CREATE(priv_fastip_handle.osh, priv_fastip_handle.br_conn_cache_lock);
        FASTIP_LOCK_CREATE(priv_fastip_handle.osh, priv_fastip_handle.ip_conn_cache_lock);
        FASTIP_LOCK_CREATE(priv_fastip_handle.osh, priv_fastip_handle.dev_list->lock);
    }

    ASSERT((priv_fastip_handle.br_conn_cache != NULL) && (priv_fastip_handle.ip_conn_cache != NULL) && (priv_fastip_handle.dev_list != NULL));

    /* Allocate private info structure */
    if ((fastip_handle = MALLOC(sizeof(fastip_info_t))) == NULL)
    {
        FASTIP_TRACE_ERROR("%s: malloc failed\n", __FUNCTION__);
        return (NULL);
    }

    bzero((char *)fastip_handle, sizeof(fastip_info_t));

    fastip_handle->msg_level = msg_level ? msg_level : &fastip_msg_level;
    fastip_handle->osh = osh;
    strncpy(fastip_handle->name, name, FASTIP_NAME_SZ);
    fastip_handle->name[FASTIP_NAME_SZ - 1] = '\0';

    /* Point to the global ip_conn_cache and br_conn_cache tables */
    fastip_handle->br_conn_cache = priv_fastip_handle.br_conn_cache;
    fastip_handle->ip_conn_cache = priv_fastip_handle.ip_conn_cache;

    /* Point to the global vldev cache */
    fastip_handle->vlc = priv_fastip_handle.vlc;

    /* Point to the global br_conn_cache hot cache */
    fastip_handle->br_conn_cache_hot = priv_fastip_handle.br_conn_cache_hot;

    /* Initialize the locks */
    fastip_handle->br_conn_cache_lock = priv_fastip_handle.br_conn_cache_lock;
    fastip_handle->ip_conn_cache_lock = priv_fastip_handle.ip_conn_cache_lock;

    /* Initialize the dev list */
    fastip_handle->dev_list = priv_fastip_handle.dev_list;

    /* Initialize the public members */

    fastip_handle->fastip.fastip_enab = 1;

    fastip_handle->fastip.fn.detach    = _fastip_detach;
    fastip_handle->fastip.fn.forward   = _fastip_forward;
    fastip_handle->fastip.fn.isenabled = _fastip_isenabled;
    fastip_handle->fastip.fn.isbridge  = _fastip_isbridge;
    fastip_handle->fastip.fn.br_conn_cache_add = _fastip_br_conn_cache_add;
    fastip_handle->fastip.fn.br_conn_cache_delete = _fastip_br_conn_cache_delete;
    fastip_handle->fastip.fn.br_conn_cache_update = _fastip_br_conn_cache_update;
    fastip_handle->fastip.fn.br_conn_cache_lkup = _fastip_br_conn_cache_lkup;
    fastip_handle->fastip.fn.br_conn_cache_release = _fastip_br_conn_cache_release;
    fastip_handle->fastip.fn.ip_conn_cache_add = _fastip_ip_conn_cache_add;
    fastip_handle->fastip.fn.ip_conn_cache_delete = _fastip_ip_conn_cache_delete;
    fastip_handle->fastip.fn.ip_conn_cache_count_get = _fastip_ip_conn_cache_count_get;
    fastip_handle->fastip.fn.ip_conn_cache_delete_multi = _fastip_ip_conn_cache_delete_multi;
    fastip_handle->fastip.fn.ip_conn_cache_delete_range = _fastip_ip_conn_cache_delete_range;
    fastip_handle->fastip.fn.ip_conn_cache_action = _fastip_ip_conn_cache_action;
    fastip_handle->fastip.fn.ip_conn_cache_lkup = _fastip_ip_conn_cache_lkup_fn;
#ifdef FASTIP_IPV6
    fastip_handle->fastip.fn.ip_conn_cache_lkup_l4proto = _fastip_ip_conn_cache_lkup_l4proto_v6;
#endif/* FASTIP_IPV6 */
    fastip_handle->fastip.fn.ip_conn_cache_release = _fastip_ip_conn_cache_release;
    fastip_handle->fastip.fn.enable = _fastip_enable;
    fastip_handle->fastip.fn.dev_register   = _fastip_dev_register;
    fastip_handle->fastip.fn.dev_unregister = _fastip_dev_unregister;
    fastip_handle->fastip.fn.detach_cb = cb;
    fastip_handle->fastip.fn.detach_cb_arg = arg;
    fastip_handle->fastip.fn.dev_vlan_add = _fastip_dev_vlan_add;
    fastip_handle->fastip.fn.dev_vlan_delete = _fastip_dev_vlan_delete;
    fastip_handle->fastip.fn.dump = _fastip_dump;

    /* Add to the global list */
    fastip_handle->next = ci_head;
    ci_head = fastip_handle;
    printk("%s: name:%s fastip handle:%x\n", __FUNCTION__, name, (&fastip_handle->fastip));
    return (&fastip_handle->fastip);
}
EXPORT_SYMBOL_GPL(_fastip_attach);

/*-------------------------------------------------
函数原型： static void _fastip_detach(fastip_t *fastip_handle)
描    述： 释放fastip_t 操作句柄
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_detach(fastip_t *fastip_handle)
{
    fastip_info_t *p = NULL, *t;

    if (NULL == fastip_handle)
    {
        return;
    }

    /* Delete from global list */
    t = ci_head;
    while (1)
    {
        if (NULL == t)
        {
            return;
        }

        if (&t->fastip != fastip_handle)
        {
            p = t;
            t = t->next;
            continue;
        }

        /* Remove the entry from list */
        if (NULL != p)
        {
            p->next = t->next;
        }
        else
        {
            ci_head = t->next;
        }

        break;
    }

    ASSERT(&t->fastip == fastip_handle);

    /* Call back the driver before detach is complete */
    if (fastip_handle->fn.detach_cb != NULL)
    {
        fastip_handle->fn.detach_cb(fastip_handle, fastip_handle->fn.detach_cb_arg);
    }

    MFREE(fastip_handle);

    return;
}

/*-------------------------------------------------
函数原型： static void _fastip_detach_all(fastip_t *priv_fastip_handle)
描    述： 释放fastip_t 缓存记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_detach_all(fastip_t *priv_fastip_handle)
{
    fastip_info_t *t, *n;
    fastip_t *fastip_handle;

    /* Cleanup the registered device list */
    _fastip_dev_list_cleanup(priv_fastip_handle);

    /* Cleanup the fastip info list */
    t = ci_head;
    while (NULL != t)
    {
        n = t->next;

        /* Call back the driver before detach is complete */
        fastip_handle = (fastip_t *)&t->fastip;
        if (NULL != fastip_handle->fn.detach_cb)
        {
            fastip_handle->fn.detach_cb(fastip_handle, fastip_handle->fn.detach_cb_arg);
        }

        MFREE(fastip_handle);
        t = n;
    }

    /* Delete the bridge and ip cache entries */
    _fastip_br_conn_cache_cleanup(priv_fastip_handle);
    _fastip_ip_conn_cache_cleanup(priv_fastip_handle);

    ci_head = NULL;

    return;
}

/*-------------------------------------------------
函数原型： static vofastip_t *fastip_kattach(uint8 *name)
描    述： 初始化fastip 全局句柄
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
fastip_t *fastip_kattach(uint8 *name)
{
    static int32 attached = FALSE;
    fastip_info_t *fastip_handle = &priv_fastip_handle;

    if (attached)
    {
        return (&fastip_handle->fastip);
    }

    bzero((char *)fastip_handle, sizeof(fastip_info_t));

    fastip_handle->br_conn_cache = NULL;
    fastip_handle->ip_conn_cache = NULL;
    fastip_handle->dev_list = NULL;
    fastip_handle->vlc = NULL;
    fastip_handle->br_conn_cache_hot   = NULL;
    fastip_handle->ip_conn_cache_count = 0;

    fastip_handle->msg_level = &fastip_msg_level;
    fastip_handle->osh = NULL;
    strncpy(fastip_handle->name, name, FASTIP_NAME_SZ);
    fastip_handle->name[FASTIP_NAME_SZ - 1] = '\0';

    /* Initialize the public members */

    fastip_handle->fastip.fastip_enab = 1;

    fastip_handle->fastip.fn.detach    = _fastip_detach;
    fastip_handle->fastip.fn.forward   = _fastip_forward;
    fastip_handle->fastip.fn.isenabled = _fastip_isenabled;
    fastip_handle->fastip.fn.isbridge  = _fastip_isbridge;
    fastip_handle->fastip.fn.br_conn_cache_add = _fastip_br_conn_cache_add;
    fastip_handle->fastip.fn.br_conn_cache_delete = _fastip_br_conn_cache_delete;
    fastip_handle->fastip.fn.br_conn_cache_update = _fastip_br_conn_cache_update;
    fastip_handle->fastip.fn.br_conn_cache_lkup = _fastip_br_conn_cache_lkup;
    fastip_handle->fastip.fn.br_conn_cache_release = _fastip_br_conn_cache_release;
    fastip_handle->fastip.fn.ip_conn_cache_add = _fastip_ip_conn_cache_add;
    fastip_handle->fastip.fn.ip_conn_cache_delete = _fastip_ip_conn_cache_delete;
    fastip_handle->fastip.fn.ip_conn_cache_count_get = _fastip_ip_conn_cache_count_get;
    fastip_handle->fastip.fn.ip_conn_cache_delete_multi = _fastip_ip_conn_cache_delete_multi;
    fastip_handle->fastip.fn.ip_conn_cache_delete_range = _fastip_ip_conn_cache_delete_range;
    fastip_handle->fastip.fn.ip_conn_cache_action = _fastip_ip_conn_cache_action;
    fastip_handle->fastip.fn.ip_conn_cache_lkup = _fastip_ip_conn_cache_lkup_fn;
#ifdef FASTIP_IPV6
    fastip_handle->fastip.fn.ip_conn_cache_lkup_l4proto = _fastip_ip_conn_cache_lkup_l4proto_v6;
#endif/* FASTIP_IPV6 */
    fastip_handle->fastip.fn.ip_conn_cache_release = _fastip_ip_conn_cache_release;
    fastip_handle->fastip.fn.enable = _fastip_enable;
    fastip_handle->fastip.fn.dev_register   = _fastip_dev_register;
    fastip_handle->fastip.fn.dev_unregister = _fastip_dev_unregister;
    fastip_handle->fastip.fn.dev_vlan_add = _fastip_dev_vlan_add;
    fastip_handle->fastip.fn.dev_vlan_delete = _fastip_dev_vlan_delete;
    fastip_handle->fastip.fn.dump = _fastip_dump;

    attached = TRUE;

    FASTIP_TRACE("%s: attached %s\n", __FUNCTION__, name);

    return (&fastip_handle->fastip);
}

/*-------------------------------------------------
函数原型： static vofastip_t *fastip_kdetach(uint8 *name)
描    述： 释放fastip 全局句柄
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_kdetach(fastip_t *fastip_handle)
{
    fastip_info_t *pfastip_handle;

    pfastip_handle = (fastip_info_t *)fastip_handle;

    _fastip_detach_all(fastip_handle);

    /* Delete the br_conn_cache and ip_conn_cache locks */
    FASTIP_LOCK_DELETE(pfastip_handle->osh, pfastip_handle->br_conn_cache_lock);
    FASTIP_LOCK_DELETE(pfastip_handle->osh, pfastip_handle->ip_conn_cache_lock);
    FASTIP_LOCK_DELETE(pfastip_handle->osh, pfastip_handle->dev_list->lock);

    /* Free the memory alloced for br_conn_cache and ip_conn_cache hash tables */
    MFREE(pfastip_handle->br_conn_cache);
    MFREE(pfastip_handle->ip_conn_cache);
    MFREE(pfastip_handle->dev_list);
    MFREE(pfastip_handle->vlc);
    MFREE(pfastip_handle->br_conn_cache_hot);

    pfastip_handle->br_conn_cache = NULL;
    pfastip_handle->br_conn_cache_hot = NULL;
    pfastip_handle->ip_conn_cache = NULL;
    pfastip_handle->dev_list = NULL;
    pfastip_handle->vlc = NULL;
    pfastip_handle->ip_conn_cache_count = 0;

    return;
}

/*-------------------------------------------------
函数原型： static fastip_dev_t *_fastip_dev_lkup(fastip_t *fastip_handle, void *dev)
描    述： 查找fastip 句柄
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static fastip_dev_t *_fastip_dev_lkup(fastip_t *fastip_handle, void *dev)
{
    fastip_dev_t *ctd;

    /* Dont crash if list is nil */
    if (NULL == ((fastip_info_t *)(fastip_handle))->dev_list)
    {
        return NULL;
    }

    /* Device list lookup */
    ctd = FASTIP_DEV_LIST_HEAD(fastip_handle);
    while (NULL != ctd)
    {
        if (ctd->dev == dev)
        {
            return (ctd);
        }

        ctd = ctd->next;
    }

    return (NULL);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_isenabled(fastip_t *fastip_handle, void *dev)
描    述： 判断设备的加速功能是否启动
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_isenabled(fastip_t *fastip_handle, void *dev)
{
    fastip_dev_t *ctd;
    int32 enabled;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == dev))
    {
        return (FALSE);
    }
    
    FASTIP_DEV_LIST_LOCK(fastip_handle);
    ctd = _fastip_dev_lkup(fastip_handle, dev);
    enabled = ((ctd != NULL) ? ctd->enabled : FALSE);
    FASTIP_DEV_LIST_UNLOCK(fastip_handle);

    return (enabled);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_isbridge(fastip_t *fastip_handle, void *dev)
描    述： 判断设备是否为桥设备
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_isbridge(fastip_t *fastip_handle, void *dev)
{
    fastip_dev_t *ctd;
    int32 is_br;

    /* Validate the input params */
    ASSERT((fastip_handle != NULL) && (dev != NULL));

    FASTIP_DEV_LIST_LOCK(fastip_handle);
    ctd   = _fastip_dev_lkup(fastip_handle, dev);
    is_br = ((ctd != NULL) ? ctd->is_br : FALSE);
    FASTIP_DEV_LIST_UNLOCK(fastip_handle);

    return (is_br);
}


/*-------------------------------------------------
函数原型： static int32 _fastip_enable(fastip_t *fastip_handle, void *dev, int32 enable, fastip_br_conn_cache_hot_t **br_conn_cache_hot)
描    述： 启动设备加速功能
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_enable(fastip_t *fastip_handle, void *dev, int32 enable, fastip_br_conn_cache_hot_t **br_conn_cache_hot)
{
    fastip_dev_t *ctd;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == dev))
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (fastip_handle %p dev %p)\n",
                     __FUNCTION__, fastip_handle, dev);
        return (FASTIP_BADARG);
    }

    FASTIP_DEV_LIST_LOCK(fastip_handle);

    ctd = _fastip_dev_lkup(fastip_handle, dev);
    if (NULL == ctd)
    {
        FASTIP_DEV_LIST_UNLOCK(fastip_handle);
        FASTIP_TRACE_ERROR("%s: fastip dev %p not found\n",
                           __FUNCTION__, dev);
        return (FASTIP_NOTFOUND);
    }

    /* Update the state to enabled/disabled */
    ctd->enabled = enable;

    if (NULL != br_conn_cache_hot)
    {
        *br_conn_cache_hot = &FASTIP_BR_CONN_CACHE_HOT(fastip_handle, FASTIP_DEV_IFIDX (dev), 0);
    }

    FASTIP_DEV_LIST_UNLOCK(fastip_handle);
    FASTIP_TRACE("%s: _fastip_enable %p dev %s enabled:%d\n",
                     __FUNCTION__, fastip_handle, FASTIP_DEV_IFNAME(dev), ((ctd != NULL) ? ctd->enabled : FALSE));
    return (FASTIP_OK);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_dev_vlan_add(fastip_t *fastip_handle, void *dev, uint16 vid, void *vldev)
描    述： 添加VLAN设备加速
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_dev_vlan_add(fastip_t *fastip_handle, void *dev, uint16 vid, void *vldev)
{
    fastip_dev_t *ctd;
    int32 i;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == dev) || (FASTIP_VID_MAX < vid) || (NULL == vldev))
    {
        FASTIP_TRACE("%s: Invalid input (fastip_handle %p dev %p)",
                     __FUNCTION__, fastip_handle, dev);
        return (FASTIP_BADARG);
    }

    FASTIP_DEV_LIST_LOCK(fastip_handle);

    /* Add vlan only for registered devices */
    ctd = _fastip_dev_lkup(fastip_handle, dev);
    if (ctd == NULL)
    {
        FASTIP_DEV_LIST_UNLOCK(fastip_handle);
        FASTIP_TRACE_ERROR("%s: register fastip dev %p before adding vlan\n",
                           __FUNCTION__, dev);
        return (FASTIP_NOTFOUND);
    }

    /* Use the first empty slot and initialize the vlandev and vid */
    for (i = 0; i < FASTIP_VID_MAX; i++)
    {
        if (ctd->vlinfo[i].vldev == NULL)
        {
            ctd->vlinfo[i].vid   = vid;
            ctd->vlinfo[i].vldev = vldev;
            break;
        }
    }

    FASTIP_TRACE("%s: Adding vlandev %s with vid %d for %s\n",
                 __FUNCTION__, FASTIP_DEV_IFNAME(vldev), vid, FASTIP_DEV_IFNAME(dev));

    /* Add to the vldev cache for quick access while forwading */
    if ((FASTIP_DEV_IFIDX(dev) < FASTIP_VLC_MAX) && (vid < FASTIP_VLC_MAX))
    {
        FASTIP_TRACE("%s: Caching vlandev %s with vid %d at index %d\n",
                     __FUNCTION__, FASTIP_DEV_IFNAME(vldev), vid, FASTIP_DEV_IFIDX(dev));
        FASTIP_VLDEV(fastip_handle, FASTIP_DEV_IFIDX(dev), vid) = vldev;
    }

    FASTIP_DEV_LIST_UNLOCK(fastip_handle);

    return (FASTIP_OK);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_dev_vlan_delete(fastip_t *fastip_handle, void *dev, uint16 vid)
描    述： 根据VID删除vlan设备加速
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_dev_vlan_delete(fastip_t *fastip_handle, void *dev, uint16 vid)
{
    fastip_dev_t *ctd;
    int32 i;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == dev) || (FASTIP_VID_MAX < vid))
    {
        FASTIP_TRACE("%s: Invalid input (fastip_handle %p dev %p vid %d)",
                     __FUNCTION__, fastip_handle, dev, vid);
        return (FASTIP_BADARG);
    }

    FASTIP_DEV_LIST_LOCK(fastip_handle);

    /* Delete vlan only if the registered device exists */
    ctd = _fastip_dev_lkup(fastip_handle, dev);
    if (NULL == ctd)
    {
        FASTIP_DEV_LIST_UNLOCK(fastip_handle);
        FASTIP_TRACE_ERROR("%s: fastip dev %p doesn't exist\n",
                           __FUNCTION__, dev);
        return (FASTIP_NOTFOUND);
    }

    /* Clear the vlan info if the vid is configured on this device */
    for (i = 0; i < FASTIP_VID_MAX; i++)
    {
        if (ctd->vlinfo[i].vid == vid)
        {
            ctd->vlinfo[i].vid   = 0;
            ctd->vlinfo[i].vldev = NULL;
            break;
        }
    }

    if (FASTIP_VID_MAX == i)
    {
        FASTIP_DEV_LIST_UNLOCK(fastip_handle);
        return (FASTIP_NOTFOUND);
    }

    FASTIP_TRACE("%s: Deleted vlandev %s with vid %d and dev %s\n",
                 __FUNCTION__, FASTIP_DEV_IFNAME(ctd->vlinfo[i].vldev),
                 vid, FASTIP_DEV_IFNAME(dev));

    /* Delete the vldev cache entry */
    if ((FASTIP_DEV_IFIDX(dev) < FASTIP_VLC_MAX) && (vid < FASTIP_VLC_MAX))
    {
        FASTIP_TRACE("%s: Clearing vlc entry for %s with vid %d at index %d\n",
                     __FUNCTION__, FASTIP_DEV_IFNAME(FASTIP_VLDEV(fastip_handle, FASTIP_DEV_IFIDX(dev), vid)),
                     vid, FASTIP_DEV_IFIDX(dev));
        FASTIP_VLDEV(fastip_handle, FASTIP_DEV_IFIDX(dev), vid) = NULL;
    }

    FASTIP_DEV_LIST_UNLOCK(fastip_handle);

    return (FASTIP_OK);
}

/*-------------------------------------------------
函数原型： static void *_fastip_dev_vlan_find(fastip_t *fastip_handle, void *dev, uint16 vid)
描    述： 根据VID返回设备句柄
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void *_fastip_dev_vlan_find(fastip_t *fastip_handle, void *dev, uint16 vid)
{
    fastip_dev_t *ctd;
    int32 i;

    /* Find the vldev cache entry */
    if ((FASTIP_DEV_IFIDX(dev) < FASTIP_VLC_MAX) && (vid < FASTIP_VLC_MAX))
    {
        FASTIP_TRACE("%s: Found vlc entry for %s with vid %d at index %d\n",
                     __FUNCTION__, FASTIP_DEV_IFNAME(FASTIP_VLDEV(fastip_handle, FASTIP_DEV_IFIDX(dev), vid)),
                     vid, FASTIP_DEV_IFIDX(dev));
        return (FASTIP_VLDEV(fastip_handle, FASTIP_DEV_IFIDX(dev), vid));
    }

    /* Find the device */
    ctd = _fastip_dev_lkup(fastip_handle, dev);
    if (ctd == NULL)
    {
        FASTIP_TRACE_ERROR("%s: fastip dev %p doesn't exist\n", __FUNCTION__, dev);
        return (NULL);
    }

    for (i = 0; i < FASTIP_VID_MAX; i++)
    {
        if (ctd->vlinfo[i].vid == vid)
        {
            FASTIP_TRACE("%s: Found vlandev %s with vid %d and dev %s\n",
                         __FUNCTION__, FASTIP_DEV_IFNAME(ctd->vlinfo[i].vldev),
                         vid, FASTIP_DEV_IFNAME(dev));
            return (ctd->vlinfo[i].vldev);
        }
    }

    return (NULL);
}


/*-------------------------------------------------
函数原型： static int32 _fastip_dev_register(fastip_t *fastip_handle, void *dev, int32 br, int32 enable)
描    述： 注册设备加速功能
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_dev_register(fastip_t *fastip_handle, void *dev, int32 isbridge, int32 enable)
{
    fastip_dev_t *ctd;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == dev))
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (fastip_handle %p dev %p)\n",
                     __FUNCTION__, fastip_handle, dev);
        return (FASTIP_BADARG);
    }

    FASTIP_DEV_LIST_LOCK(fastip_handle);
    FASTIP_TRACE("%s:_fastip_dev_lkup dev:%s _fastip_enable:%d\n", __FUNCTION__, FASTIP_DEV_IFNAME(dev), enable);
    /* Check if entry is already registered */
    ctd = _fastip_dev_lkup(fastip_handle, dev);
    if (ctd != NULL)
    {
        FASTIP_DEV_LIST_UNLOCK(fastip_handle);
        FASTIP_TRACE_ERROR("%s: fastip dev %p already registered\n",
                           __FUNCTION__, dev);
        return (FASTIP_ERROR);
    }
    
    /* Allocate new entry and add to the list */
    if ((ctd = MALLOC(sizeof(fastip_dev_t))) == NULL)
    {
        FASTIP_DEV_LIST_UNLOCK(fastip_handle);
        FASTIP_TRACE_ERROR("%s: Out of memory allocating fastip dev entry\n",
                           __FUNCTION__);
        return (FASTIP_ERROR);
    }
    FASTIP_TRACE("%s:_fastip_dev_lkup dev:%s ctd:%x\n", __FUNCTION__, FASTIP_DEV_IFNAME(dev), ctd);
    /* Initialize the entry and add it to the list */
    bzero((char *)ctd, sizeof(fastip_dev_t));
    ctd->fastip_handle    = fastip_handle;
    ctd->dev   = dev;
    ctd->is_br = isbridge;
    ctd->next = FASTIP_DEV_LIST_HEAD(fastip_handle);
    FASTIP_DEV_LIST_HEAD(fastip_handle) = ctd;
    FASTIP_DEV_LIST_UNLOCK(fastip_handle);
    /*如果使能，直接启动加速功能，否则需要额外调用fastip_enable*/
    if (enable)
    {
        _fastip_enable(fastip_handle, dev, TRUE, NULL);
    }
    FASTIP_TRACE("%s: dev:%s _fastip_enable:%d\n", __FUNCTION__, FASTIP_DEV_IFNAME(dev), enable);
    return (FASTIP_OK);
}


/*-------------------------------------------------
函数原型： static void _fastip_dev_unregister(fastip_t *fastip_handle, void *dev)
描    述： 去注册设备加速功能
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_dev_unregister(fastip_t *fastip_handle, void *dev)
{
    fastip_dev_t *ctd, *prev = NULL;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == dev))
    {
        FASTIP_TRACE("%s: Invalid input (fastip_handle %p dev %p)",
                     __FUNCTION__, fastip_handle, dev);
        return;
    }

    FASTIP_DEV_LIST_LOCK(fastip_handle);

    /* Delete the bridge and ip cache entries corresponding
     * to this device.
     */
    _fastip_br_conn_cache_delete_by_dev(&priv_fastip_handle.fastip, dev);
    _fastip_ip_conn_cache_delete_by_dev(&priv_fastip_handle.fastip, dev);

    ctd = FASTIP_DEV_LIST_HEAD(fastip_handle);
    while (NULL != ctd)
    {
        if (ctd->dev != dev)
        {
            prev = ctd;
            ctd = ctd->next;
            continue;
        }

        /* Remove the entry from list */
        if (NULL != prev)
        {
            prev->next = ctd->next;
        }
        else
        {
            FASTIP_DEV_LIST_HEAD(fastip_handle) = ctd->next;
        }

        FASTIP_DEV_LIST_UNLOCK(fastip_handle);

        MFREE(ctd);

        return;
    }

    FASTIP_DEV_LIST_UNLOCK(fastip_handle);

    return;
}

/*-------------------------------------------------
函数原型： static void _fastip_dev_list_cleanup(fastip_t *fastip_handle)
描    述： 清除加速设备列表
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_dev_list_cleanup(fastip_t *fastip_handle)
{
    fastip_dev_t *ctd, *next;

    /* Validate the input param */
    if (NULL == fastip_handle)
    {
        return;
    }

    FASTIP_DEV_LIST_LOCK(fastip_handle);

    /* Delete entries of all the registered devices */
    ctd = FASTIP_DEV_LIST_HEAD(fastip_handle);
    while (NULL != ctd)
    {
        next = ctd->next;
        MFREE(ctd);
        ctd = next;
    }

    FASTIP_DEV_LIST_HEAD(fastip_handle) = NULL;

    FASTIP_DEV_LIST_UNLOCK(fastip_handle);
    return;
}


/*-------------------------------------------------
函数原型： static void _fastip_ip_conn_cache_detach_pbr_conn_cache(fastip_t *fastip_handle, fastip_br_conn_cache_t *pbr_conn_cache)
描    述：  根据pbr_conn_cache查找并清除对应的ip连接记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_ip_conn_cache_detach_pbr_conn_cache(fastip_t *fastip_handle, fastip_br_conn_cache_t *pbr_conn_cache)
{
    uint32 hash;
    fastip_ip_conn_cache_t *pip_conn_cache;

    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);

    for (hash = 0; hash < FASTIP_IP_CONN_CACHE_SZ; hash++)
    {
        pip_conn_cache = FASTIP_IP_CONN_CACHEP(fastip_handle)[hash];
        while (pip_conn_cache != NULL)
        {
            /* Clear pbr_conn_cache field in all the matching ip_conn_cache
             * entries.
             */
            if (pip_conn_cache->pbr_conn_cache == pbr_conn_cache)
            {
                pip_conn_cache->pbr_conn_cache = NULL;
            }

            pip_conn_cache = pip_conn_cache->next;
        }
    }

    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);
}


/*-------------------------------------------------
函数原型： static fastip_br_conn_cache_t * _fastip_br_conn_cache_lkup_ll(fastip_t *fastip_handle, uint8 *da)
描    述： 根据mac地址查找网桥缓存记录,在调用函数中添加锁保护
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static fastip_br_conn_cache_t * _fastip_br_conn_cache_lkup_ll(fastip_t *fastip_handle, uint8 *da)
{
    uint32 hash;
    fastip_br_conn_cache_t *pbr_conn_cache;

    hash = FASTIP_HASH(da);

    /* Bridge cache lookup */
    pbr_conn_cache = FASTIP_BR_CONN_CACHEP(fastip_handle)[hash];
    while (pbr_conn_cache != NULL)
    {
        if (FASTIP_EA_CMP(pbr_conn_cache, da))
        {
            return (pbr_conn_cache);
        }

        pbr_conn_cache = pbr_conn_cache->next;
    }

    return (NULL);
}


/*-------------------------------------------------
函数原型： static fastip_br_conn_cache_t *_fastip_br_conn_cache_lkup(fastip_t *fastip_handle, uint8 *da)
描    述： 根据mac地址查找网桥缓存记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static fastip_br_conn_cache_t *_fastip_br_conn_cache_lkup(fastip_t *fastip_handle, uint8 *da)
{
    fastip_br_conn_cache_t *pbr_conn_cache;

    FASTIP_BR_CONN_CACHE_LOCK(fastip_handle);

    pbr_conn_cache = _fastip_br_conn_cache_lkup_ll(fastip_handle, da);

    FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);

    return (pbr_conn_cache);
}

/*-------------------------------------------------
函数原型： static void _fastip_br_conn_cache_release(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache)
描    述： 释放网桥缓存处理锁
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_br_conn_cache_release(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache)
{
    ;//FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_br_conn_cache_add(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache)
描    述： 添加新的网桥缓存记录，在网桥添加fdb时进行调用
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_br_conn_cache_add(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache)
{
    uint32 hash;
    fastip_br_conn_cache_t *pbr_conn_cache;

#ifdef FASTIP_DEBUG
    char eabuf[FASTIP_ETHER_ADDR_STR_LEN];
#endif

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == br_conn_cache) || (NULL == FASTIP_BR_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (fastip_handle %p br_conn_cachee %p br_conn_cache %p)",
                     __FUNCTION__, fastip_handle, br_conn_cache, FASTIP_BR_CONN_CACHEP(fastip_handle));
        return (FASTIP_BADARG);
    }

    FASTIP_BR_CONN_CACHE_LOCK(fastip_handle);

    /* Check for duplicates */
    if (NULL != _fastip_br_conn_cache_lkup_ll(fastip_handle, br_conn_cache->dhost.octet))
    {
        FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);
        FASTIP_TRACE_ERROR("%s: Adding duplicate entry %s\n",
                     __FUNCTION__, fastip_ether_ntoa(&br_conn_cache->dhost, eabuf));
        return (FASTIP_ERROR);
    }

    /* Allocate new entry */
    if (NULL == (pbr_conn_cache = MALLOC(sizeof(fastip_br_conn_cache_t))))
    {
        FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);
        FASTIP_TRACE_ERROR("%s: Out of memory allocating br_conn_cache entry\n", __FUNCTION__);
        return (FASTIP_ERROR);
    }

    /* Initialize the bridge cache entry and add it to the hash table */
    *pbr_conn_cache = *br_conn_cache;

    hash = FASTIP_HASH(br_conn_cache->dhost.octet);
    pbr_conn_cache->next = FASTIP_BR_CONN_CACHEP(fastip_handle)[hash];
    FASTIP_BR_CONN_CACHEP(fastip_handle)[hash] = pbr_conn_cache;

    FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);

    FASTIP_TRACE("Adding br_conn_cache entry %s\n", fastip_ether_ntoa(&pbr_conn_cache->dhost, eabuf));

    return (FASTIP_OK);
}

/*-------------------------------------------------
函数原型： static void _fastip_br_conn_cache_hot_invalidate(fastip_t *fastip_handle, fastip_br_conn_cache_t *pbr_conn_cache)
描    述： 置网桥连接缓冲表hot为无效
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_br_conn_cache_hot_invalidate(fastip_t *fastip_handle, fastip_br_conn_cache_t *pbr_conn_cache)
{
    int32 i, j;

    /* Find and clear the hot br_conn_cache entry */
    for (i = 0; i < FASTIP_MAXBR_CONN_CACHE_HOTIF; i++)
    {
        for (j = 0; j < FASTIP_MAXBR_CONN_CACHE_HOT; j++)
        {
            if (FASTIP_EA_CMP(pbr_conn_cache, &FASTIP_BR_CONN_CACHE_HOT(fastip_handle, i, j).ea))
            {
                bzero(&FASTIP_BR_CONN_CACHE_HOT(fastip_handle, i, j), sizeof(fastip_br_conn_cache_hot_t));
            }
        }
    }
}

/*-------------------------------------------------
函数原型： static int32 _fastip_br_conn_cache_delete(fastip_t *fastip_handle, uint8 *ea)
描    述： 在网桥fdb状态更新时，删除网桥连接缓存记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_br_conn_cache_delete(fastip_t *fastip_handle, uint8 *ea)
{
    uint32 hash;
    fastip_br_conn_cache_t *prev = NULL, *pbr_conn_cache;

#ifdef FASTIP_DEBUG
    char eabuf[FASTIP_ETHER_ADDR_STR_LEN];
#endif

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == ea) || (NULL == FASTIP_BR_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE("%s: Invalid input (fastip_handle %p ea %p br_conn_cache %p)",
                     __FUNCTION__, fastip_handle, ea, FASTIP_BR_CONN_CACHEP(fastip_handle));
        return (FASTIP_BADARG);
    }

    hash = FASTIP_HASH(ea);
    pbr_conn_cache = FASTIP_BR_CONN_CACHEP(fastip_handle)[hash];

    FASTIP_BR_CONN_CACHE_LOCK(fastip_handle);

    /* Find the entry and delete */
    while (NULL != pbr_conn_cache)
    {
        if (!FASTIP_EA_CMP(pbr_conn_cache, ea))
        {
            prev = pbr_conn_cache;
            pbr_conn_cache = pbr_conn_cache->next;
            continue;
        }

        /* Remove the entry from hash list */
        if (NULL != prev)
        {
            prev->next = pbr_conn_cache->next;
        }
        else
        {
            FASTIP_BR_CONN_CACHEP(fastip_handle)[hash] = pbr_conn_cache->next;
        }

        /* Clear pbr_conn_cache field in all the matching ip_conn_cache entries */
        _fastip_ip_conn_cache_detach_pbr_conn_cache(fastip_handle, pbr_conn_cache);

        /* Invalidate hot br_conn_cache entry */
        _fastip_br_conn_cache_hot_invalidate(fastip_handle, pbr_conn_cache);

        FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);

        MFREE(pbr_conn_cache);

        FASTIP_TRACE("Deleting br_conn_cache entry %s\n", fastip_ether_ntoa(&pbr_conn_cache->dhost, eabuf));

        return (FASTIP_OK);
    }

    FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);

    FASTIP_TRACE("%s: Bridge cache entry not found\n", __FUNCTION__);

    return (FASTIP_NOTFOUND);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_br_conn_cache_update(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache)
描    述： 更新网桥缓存记录，在网桥下挂主机端口移出或其他状态更新时调用
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_br_conn_cache_update(fastip_t *fastip_handle, fastip_br_conn_cache_t *br_conn_cache)
{
    fastip_br_conn_cache_t *pbr_conn_cache;

#ifdef FASTIP_DEBUG
    char eabuf[FASTIP_ETHER_ADDR_STR_LEN];
#endif

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == br_conn_cache) || (NULL == FASTIP_BR_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE("%s: Invalid input (fastip_handle %p br_conn_cachee %p br_conn_cache %p)",
                     __FUNCTION__, fastip_handle, br_conn_cache, FASTIP_BR_CONN_CACHEP(fastip_handle));
        return (FASTIP_BADARG);
    }

    FASTIP_BR_CONN_CACHE_LOCK(fastip_handle);

    /* Check for duplicates */
    if ((pbr_conn_cache = _fastip_br_conn_cache_lkup_ll(fastip_handle, br_conn_cache->dhost.octet)) == NULL)
    {
        FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);
        FASTIP_TRACE_ERROR("%s: Bridge cache entry entry %s not found\n",
                           __FUNCTION__, fastip_ether_ntoa(&br_conn_cache->dhost, eabuf));
        return (FASTIP_NOTFOUND);
    }

    /* Update the bridge port and vlan information of cache entry */
    pbr_conn_cache->vid    = br_conn_cache->vid;
    pbr_conn_cache->txifp  = br_conn_cache->txifp;
    pbr_conn_cache->action = br_conn_cache->action;
    pbr_conn_cache->live   = 0;

    FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);

    FASTIP_TRACE("Updating br_conn_cache entry %s\n", fastip_ether_ntoa(&pbr_conn_cache->dhost, eabuf));

    return (FASTIP_OK);
}

/*-------------------------------------------------
函数原型： static fastip_ip_conn_cache_t * _fastip_ip_conn_cache_lkup_ll(fastip_t *fastip_handle, int32 v6, uint32 *sip, uint32 *dip,
                                                              uint8 proto, uint16 sp, uint16 dp)
描    述： 根据5元组查找ip连接缓存记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static fastip_ip_conn_cache_t * _fastip_ip_conn_cache_lkup_ll(fastip_t *fastip_handle, int32 v6, uint32 *sip, uint32 *dip,
                                                              uint8 proto, uint16 sp, uint16 dp)
{
    uint32 hash;
    fastip_ip_conn_cache_t *pip_conn_cache;

    hash = IP_CONN_CACHE_HASH(v6, sip, dip, proto, sp, dp);

    /* IP cache lookup */
    pip_conn_cache = FASTIP_IP_CONN_CACHEP(fastip_handle)[hash];
    while (NULL != pip_conn_cache)
    {
        if (IP_CONN_CACHE_TPL_CMP(pip_conn_cache, v6, sip, dip, proto, sp, dp))
        {
            return (pip_conn_cache);
        }

        pip_conn_cache = pip_conn_cache->next;
    }

    return (NULL);
}

/*-------------------------------------------------
函数原型： static fastip_ip_conn_cache_t *_fastip_ip_conn_cache_lkup(fastip_t *fastip_handle, int32 v6, uint32 *sip, uint32 *dip,
                                                          uint8 proto, uint16 sp, uint16 dp)
描    述： 根据5元组查找ip连接缓存记录，待处理完毕后由调用者释放锁
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static fastip_ip_conn_cache_t *_fastip_ip_conn_cache_lkup(fastip_t *fastip_handle, int32 v6, uint32 *sip, uint32 *dip,
                                                          uint8 proto, uint16 sp, uint16 dp)
{
    fastip_ip_conn_cache_t *pip_conn_cache;

    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);

    pip_conn_cache = _fastip_ip_conn_cache_lkup_ll(fastip_handle, v6, sip, dip, proto, sp, dp);

    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

    return (pip_conn_cache);
}

/*-------------------------------------------------
函数原型： static void _fastip_ip_conn_cache_release(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache)
描    述： 释放ip锁
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_ip_conn_cache_release(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache)
{
    ;//FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_ip_conn_cache_add(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache, int32 v6)
描    述： 添加ip缓存连接跟踪记录，在内核生成跟踪记录处进行调用
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_ip_conn_cache_add(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache, int32 v6)
{
    uint32 hash;
    uint32 *sip, *dip;
    uint16 sp, dp;
    uint8 proto;
    fastip_ip_conn_cache_t *pip_conn_cache;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == ip_conn_cache) || (NULL == FASTIP_IP_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE("%s: Invalid input (fastip_handle %p ip_conn_cachee %p pip_conn_cache %p)",
                     __FUNCTION__, fastip_handle, ip_conn_cache, FASTIP_IP_CONN_CACHEP(fastip_handle));
        return (FASTIP_BADARG);
    }

#ifndef FASTIP_IPV6
    if (v6)
    {
        return (FASTIP_BADARG);
    }
#endif/* FASTIP_IPV6 */


    sip   = ip_conn_cache->tuple.sip;
    dip   = ip_conn_cache->tuple.dip;
    proto = ip_conn_cache->tuple.proto;
    sp = ip_conn_cache->tuple.sp;
    dp = ip_conn_cache->tuple.dp;

    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);
    if (NULL != _fastip_ip_conn_cache_lkup_ll(fastip_handle, v6, sip, dip, proto, sp, dp))
    {
        FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);
        FASTIP_TRACE("%s: Adding duplicate entry\n", __FUNCTION__);
        return (FASTIP_ERROR);
    }

    if (NULL == (pip_conn_cache = MALLOC(sizeof(fastip_ip_conn_cache_t))))
    {
        FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);
        FASTIP_TRACE_ERROR("%s: Out of memory allocating ip_conn_cache entry\n", __FUNCTION__);
        return (FASTIP_ERROR);
    }

    *pip_conn_cache = *ip_conn_cache;
    pip_conn_cache->tos &= FASTIP_IPV4_TOS_DSCP_MASK;

    hash = IP_CONN_CACHE_HASH(v6, sip, dip, proto, sp, dp);
    pip_conn_cache->next = FASTIP_IP_CONN_CACHEP(fastip_handle)[hash];
    FASTIP_IP_CONN_CACHEP(fastip_handle)[hash] = pip_conn_cache;
    priv_fastip_handle.ip_conn_cache_count++;

    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

    FASTIP_TRACE("%s: IP conn cache entry added\n", __FUNCTION__);

    return (FASTIP_OK);
}


/*-------------------------------------------------
函数原型： static int32 _fastip_ip_conn_cache_delete(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache, int32 v6)
描    述： 删除ip缓存连接跟踪记录，在内核释放跟踪记录处进行调用，同时需要注意异常问题
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_ip_conn_cache_delete(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache, int32 v6)
{
    uint32 hash;
    fastip_ip_conn_cache_t *prev = NULL, *pip_conn_cache;
    uint32 *sip, *dip;
    uint16 sp, dp;
    uint8 proto;

    /* Validate the input params */
    if ((NULL == ip_conn_cache) || (NULL == fastip_handle) || (NULL == FASTIP_IP_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE("%s: Invalid input (fastip_handle %p ip_conn_cache %p)",
                     __FUNCTION__, fastip_handle, FASTIP_IP_CONN_CACHEP(fastip_handle));
        return (FASTIP_BADARG);
    }

#ifndef FASTIP_IPV6
    if (v6)
    {
        return (FASTIP_BADARG);
    }
#endif/* FASTIP_IPV6 */


    sip   = ip_conn_cache->tuple.sip;
    dip   = ip_conn_cache->tuple.dip;
    proto = ip_conn_cache->tuple.proto;
    sp   = ip_conn_cache->tuple.sp;
    dp   = ip_conn_cache->tuple.dp;
    hash = IP_CONN_CACHE_HASH(v6, sip, dip, proto, sp, dp);

    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);

    pip_conn_cache = FASTIP_IP_CONN_CACHEP(fastip_handle)[hash];

    /* Keep track of prev pointer for deletion */
    while (NULL != pip_conn_cache)
    {
        if (!IP_CONN_CACHE_TPL_CMP(pip_conn_cache, v6, sip, dip, proto, sp, dp))
        {
            prev = pip_conn_cache;
            pip_conn_cache = pip_conn_cache->next;
            continue;
        }

        /* Remove the entry from hash list */
        if (NULL != prev)
        {
            prev->next = pip_conn_cache->next;
        }
        else
        {
            FASTIP_IP_CONN_CACHEP(fastip_handle)[hash] = pip_conn_cache->next;
        }

        priv_fastip_handle.ip_conn_cache_count--;

        FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

        MFREE(pip_conn_cache);

        FASTIP_TRACE("%s: IP conn cache entry deleted\n", __FUNCTION__);

        return (FASTIP_OK);
    }

    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

    FASTIP_TRACE("%s: IP conn cache entry not found\n", __FUNCTION__);

    return (FASTIP_NOTFOUND);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_ip_conn_cache_count_get(fastip_t *fastip_handle)
描    述： 获取对应的ip缓存跟踪记录表统计
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_ip_conn_cache_count_get(fastip_t *fastip_handle)
{
    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == FASTIP_IP_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (fastip_handle %p)", __FUNCTION__, fastip_handle);
        return (FASTIP_BADARG);
    }

    return (priv_fastip_handle.ip_conn_cache_count);
}

/*-------------------------------------------------
函数原型： static fastip_ip_conn_cache_t * _fastip_ip_conn_cache_lkup_fn(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache,
                                                                int32 v6)
描    述： 返回ip缓存查找函数句柄
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static fastip_ip_conn_cache_t * _fastip_ip_conn_cache_lkup_fn(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache,
                                                                int32 v6)
{
    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == ip_conn_cache) || (NULL == FASTIP_IP_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE("%s: Invalid input (fastip_handle %p ip_conn_cachee %p pip_conn_cache %p)",
                     __FUNCTION__, fastip_handle, ip_conn_cache, FASTIP_IP_CONN_CACHEP(fastip_handle));
        return (NULL);
    }

#ifndef FASTIP_IPV6
    if (v6)
    {
        return (NULL);
    }
#endif/* FASTIP_IPV6 */


    return _fastip_ip_conn_cache_lkup(fastip_handle, v6, ip_conn_cache->tuple.sip, ip_conn_cache->tuple.dip,
                                      ip_conn_cache->tuple.proto, ip_conn_cache->tuple.sp, ip_conn_cache->tuple.dp);
}

/*-------------------------------------------------
函数原型： static void _fastip_br_conn_cache_cleanup(fastip_t *fastip_handle)
描    述： 清除bridge缓存连接跟踪记录，在设备进行detach时进行调研
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_br_conn_cache_cleanup(fastip_t *fastip_handle)
{
    int32 i;
    fastip_br_conn_cache_t *pbr_conn_cache, *next;

    if (NULL == fastip_handle)
    {
        return;
    }

    FASTIP_BR_CONN_CACHE_LOCK(fastip_handle);
    for (i = 0; i < FASTIP_BR_CONN_CACHE_SZ; i++)
    {
        pbr_conn_cache = FASTIP_BR_CONN_CACHEP(fastip_handle)[i];
        while (NULL != pbr_conn_cache)
        {
            next = pbr_conn_cache->next;
            MFREE(pbr_conn_cache);
            pbr_conn_cache = next;
        }

        FASTIP_BR_CONN_CACHEP(fastip_handle)[i] = NULL;
    }

    FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);

    return;
}


/*-------------------------------------------------
函数原型： static void _fastip_br_conn_cache_cleanup(fastip_t *fastip_handle)
描    述： 根据制定的设备，清除bridge缓存连接跟踪记录，在设备进行detach时进行调研
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_br_conn_cache_delete_by_dev(fastip_t *fastip_handle, void *dev)
{
    int32 i;
    fastip_br_conn_cache_t *pbr_conn_cache, *prev = NULL, *next;

    if (NULL == fastip_handle)
    {
        return;
    }

    FASTIP_BR_CONN_CACHE_LOCK(fastip_handle);
    for (i = 0; i < FASTIP_BR_CONN_CACHE_SZ; i++)
    {
        pbr_conn_cache = FASTIP_BR_CONN_CACHEP(fastip_handle)[i];
        prev = NULL;
        while (NULL != pbr_conn_cache)
        {
            if (pbr_conn_cache->txifp != dev)
            {
                prev = pbr_conn_cache;
                pbr_conn_cache = pbr_conn_cache->next;
                continue;
            }

            if (NULL != prev)
            {
                prev->next = pbr_conn_cache->next;
            }
            else
            {
                FASTIP_BR_CONN_CACHEP(fastip_handle)[i] = pbr_conn_cache->next;
            }

            next = pbr_conn_cache->next;

            /* Clear pbr_conn_cache field in all the matching ip_conn_cache entries */
            _fastip_ip_conn_cache_detach_pbr_conn_cache(fastip_handle, pbr_conn_cache);

            /* Invalidate hot br_conn_cache entry */
            _fastip_br_conn_cache_hot_invalidate(fastip_handle, pbr_conn_cache);

            MFREE(pbr_conn_cache);

            pbr_conn_cache = next;
        }
    }

    FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);

    return;
}

/*-------------------------------------------------
函数原型： static void _fastip_ip_conn_cache_cleanup(fastip_t *fastip_handle)
描    述： 清除ip缓存连接跟踪记录，在设备进行detach时进行调研
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_ip_conn_cache_cleanup(fastip_t *fastip_handle)
{
    int32 i;
    fastip_ip_conn_cache_t *next, *pip_conn_cache;

    if (NULL == fastip_handle)
    {
        return;
    }

    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);
    for (i = 0; i < FASTIP_IP_CONN_CACHE_SZ; i++)
    {
        pip_conn_cache = FASTIP_IP_CONN_CACHEP(fastip_handle)[i];
        while (NULL != pip_conn_cache)
        {
            next = pip_conn_cache->next;
            priv_fastip_handle.ip_conn_cache_count--;
            MFREE(pip_conn_cache);
            pip_conn_cache = next;
        }

        FASTIP_IP_CONN_CACHEP(fastip_handle)[i] = NULL;
    }

    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

    return;
}

/*-------------------------------------------------
函数原型： static void _fastip_ip_conn_cache_cleanup(fastip_t *fastip_handle)
描    述： 根据DEV 清除ip缓存连接跟踪记录，在设备进行detach时进行调研
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_ip_conn_cache_delete_by_dev(fastip_t *fastip_handle, void *dev)
{
    int32 i;
    fastip_ip_conn_cache_t *pip_conn_cache, *prev = NULL, *next;

    if (NULL == fastip_handle)
    {
        return;
    }

    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);
    for (i = 0; i < FASTIP_IP_CONN_CACHE_SZ; i++)
    {
        pip_conn_cache = FASTIP_IP_CONN_CACHEP(fastip_handle)[i];
        prev = NULL;
        while (NULL != pip_conn_cache)
        {
            if (pip_conn_cache->txif != dev)
            {
                prev = pip_conn_cache;
                pip_conn_cache = pip_conn_cache->next;
                continue;
            }

            if (NULL != prev)
            {
                prev->next = pip_conn_cache->next;
            }
            else
            {
                FASTIP_IP_CONN_CACHEP(fastip_handle)[i] = pip_conn_cache->next;
            }

            next = pip_conn_cache->next;

            priv_fastip_handle.ip_conn_cache_count--;

            MFREE(pip_conn_cache);
            pip_conn_cache = next;
        }
    }

    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

    return;
}


/*-------------------------------------------------
函数原型： static int32 _fastip_is_ip_conn_cache_in_range(fastip_ip_conn_cache_t *pip_conn_cache, fastip_ip_conn_cache_t *start,
                                                fastip_ip_conn_cache_t *end,
                                                int32 v6)
描    述： 判断入参或者五元组信息是否符合范围
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_is_ip_conn_cache_in_range(fastip_ip_conn_cache_t *pip_conn_cache, fastip_ip_conn_cache_t *start,
                                                fastip_ip_conn_cache_t *end,
                                                int32 v6)
{
    uint32 m;

    if ((NULL == pip_conn_cache) || (NULL == start))
    {
        return (FALSE);
    }

    /* Match the txif */
    if ((NULL != start->txif) && (pip_conn_cache->txif != start->txif))
    {
        return (FALSE);
    }

    /* See if the source mac address matches */
    if (!FASTIP_ETHER_ISNULLADDR(&start->sa)
       && memcmp(pip_conn_cache->sa.octet, start->sa.octet, FASTIP_ETHER_ADDR_LEN))
    {
        return (FALSE);
    }

    /* See if the dest mac address matches */
    if (!FASTIP_ETHER_ISNULLADDR(&start->dhost)
       && memcmp(pip_conn_cache->dhost.octet, start->dhost.octet, FASTIP_ETHER_ADDR_LEN))
    {
        return (FALSE);
    }

    /* See if the IP matches the specified version */
    if (IS_V4ADDR(pip_conn_cache->tuple.sip))
    {
        if (v6)
        {
            return (FALSE);
        }
    }
    else if (!v6)
    {
        return (FALSE);
    }

    /* Do exact match to start if end is not specified */
    if (NULL == end)
    {
        end = start;
    }

    /* See if the 5 tuple is in range */
#ifdef FASTIP_IPV6
    if (v6)
    {
        uint32 j;

        /* We ignore the sip/dip only when they are all 0 in start and end args */
        m = -1;
        if (!IS_V6ADDR_NULL(start->tuple.sip)
           || !IS_V6ADDR_NULL(end->tuple.sip))
        {
            for (j = 0; j < FASTIP_IPADDR_U32_SZ; j++)
            {
                m &= (pip_conn_cache->tuple.sip[j] >= start->tuple.sip[j])
               && (pip_conn_cache->tuple.sip[j] <= end->tuple.sip[j]);
            }
        }

        if (!IS_V6ADDR_NULL(start->tuple.dip)
           || !IS_V6ADDR_NULL(end->tuple.dip))
        {
            for (j = 0; j < FASTIP_IPADDR_U32_SZ; j++)
            {
                m &= (pip_conn_cache->tuple.dip[j] >= start->tuple.dip[j])
               && (pip_conn_cache->tuple.dip[j] <= end->tuple.dip[j]);
            }
        }
    }
    else
#endif/* FASTIP_IPV6 */
    {
        m  = FASTIP_REQ(pip_conn_cache->tuple.sip[0], start->tuple.sip[0], end->tuple.sip[0]);
        m &= FASTIP_REQ(pip_conn_cache->tuple.dip[0], start->tuple.dip[0], end->tuple.dip[0]);
    }

    m &= FASTIP_REQ(pip_conn_cache->tuple.sp, start->tuple.sp, end->tuple.sp);
    m &= FASTIP_REQ(pip_conn_cache->tuple.dp, start->tuple.dp, end->tuple.dp);
    m &= FASTIP_REQ(pip_conn_cache->tuple.proto, start->tuple.proto, end->tuple.proto);
    if (!m)
    {
        return (FALSE);
    }

    if (!v6)
    {
        m  = FASTIP_REQ(pip_conn_cache->nat.ip, start->nat.ip, end->nat.ip);
        m &= FASTIP_REQ(pip_conn_cache->nat.port, start->nat.port, end->nat.port);
        if (!m)
        {
            return (FALSE);
        }
    }

    return (TRUE);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_ip_conn_cache_delete_range(fastip_t *fastip_handle, fastip_ip_conn_cache_t *start,
                                                fastip_ip_conn_cache_t *end,
                                                int32 v6)
描    述： 根据入参删除对应符合条件制定范围的ip缓存连接跟踪记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_ip_conn_cache_delete_range(fastip_t *fastip_handle, fastip_ip_conn_cache_t *start,
                                                fastip_ip_conn_cache_t *end,
                                                int32 v6)
{
    uint32 c, i;
    fastip_ip_conn_cache_t *next, *prev = NULL, *pip_conn_cache;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == FASTIP_IP_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (fastip_handle %p)", __FUNCTION__, fastip_handle);
        return (FASTIP_BADARG);
    }

    if (NULL == start)
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (start %p end %p)",
                           __FUNCTION__, fastip_handle, FASTIP_IP_CONN_CACHEP(fastip_handle));
        return (FASTIP_BADARG);
    }

#ifndef FASTIP_IPV6
    if (v6)
    {
        return (FASTIP_BADARG);
    }
#endif/* FASTIP_IPV6 */


    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);
    c = priv_fastip_handle.ip_conn_cache_count;
    for (i = 0; i < FASTIP_IP_CONN_CACHE_SZ; i++)
    {
        prev = NULL;
        pip_conn_cache = FASTIP_IP_CONN_CACHEP(fastip_handle)[i];
        while (NULL != pip_conn_cache)
        {
            next = pip_conn_cache->next;

            if (!_fastip_is_ip_conn_cache_in_range(pip_conn_cache, start, end, v6))
            {
                goto notinrange;
            }

            /* Delete the entry and see if more matches exist */
            if (prev != NULL)
            {
                prev->next = pip_conn_cache->next;
            }
            else
            {
                FASTIP_IP_CONN_CACHEP(fastip_handle)[i] = pip_conn_cache->next;
            }

            priv_fastip_handle.ip_conn_cache_count--;

            FASTIP_TRACE("%s: Deleted fastip ip_conn_cache entry\n", __FUNCTION__);

            MFREE(pip_conn_cache);
            pip_conn_cache = next;
            continue;

notinrange:
            prev = pip_conn_cache;
            pip_conn_cache = next;
        }
    }

    c -= priv_fastip_handle.ip_conn_cache_count;
    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

    return (c == 0 ? FASTIP_NOTFOUND : FASTIP_OK);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_ip_conn_cache_action(fastip_t *fastip_handle, fastip_ip_conn_cache_t *start, fastip_ip_conn_cache_t *end,
                                          uint32 action_mask, int32 v6)
描    述： 更新ip缓存连接跟踪记录的状态
           FASTIP_ACTION_TAG, FASTIP_ACTION_UNTAG, FASTIP_ACTION_SNAT and FASTIP_ACTION_DNAT 不能在
           该函数变更状态
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_ip_conn_cache_action(fastip_t *fastip_handle, fastip_ip_conn_cache_t *start, fastip_ip_conn_cache_t *end,
                                          uint32 action_mask, int32 v6)
{
    uint32 c, i, j;
    fastip_ip_conn_cache_t *pip_conn_cache;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == FASTIP_IP_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (fastip_handle %p)", __FUNCTION__, fastip_handle);
        return (FASTIP_BADARG);
    }

    action_mask &= (FASTIP_ACTION_SUSPEND | FASTIP_ACTION_TOS | FASTIP_ACTION_MARK |
                    FASTIP_ACTION_BYTECNT);
    if ((NULL == start) || (0 == action_mask))
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (start %p end %p)",
                           __FUNCTION__, fastip_handle, FASTIP_IP_CONN_CACHEP(fastip_handle));
        return (FASTIP_BADARG);
    }

#ifndef FASTIP_IPV6
    if (v6)
    {
        return (FASTIP_BADARG);
    }
#endif/* FASTIP_IPV6 */


    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);
    j = priv_fastip_handle.ip_conn_cache_count;
    for (c = 0, i = 0; i < FASTIP_IP_CONN_CACHE_SZ; i++)
    {
        pip_conn_cache = FASTIP_IP_CONN_CACHEP(fastip_handle)[i];
        while (NULL != pip_conn_cache)
        {
            if (_fastip_is_ip_conn_cache_in_range(pip_conn_cache, start, end, v6))
            {
                uint32 action;

                action = (pip_conn_cache->action & ~action_mask);
                /* Change the suspend action flag */
                if (action_mask & FASTIP_ACTION_SUSPEND)
                {
                    action |= (start->action & FASTIP_ACTION_SUSPEND);
                }

                /* Change the TOS action flag and value */
                if (action_mask & FASTIP_ACTION_TOS)
                {
                    action |= (start->action & FASTIP_ACTION_TOS);
                    if (action & FASTIP_ACTION_TOS)
                    {
                        pip_conn_cache->tos = start->tos & FASTIP_IPV4_TOS_DSCP_MASK;
                    }
                }

                /* Change the mark action flag and value */
                if (action_mask & FASTIP_ACTION_MARK)
                {
                    action |= (start->action & FASTIP_ACTION_MARK);
                    if (action & FASTIP_ACTION_MARK)
                    {
                        pip_conn_cache->mark = start->mark;
                    }
                }

                /* Change the byte counter action flag and pointer */
                if (action_mask & FASTIP_ACTION_BYTECNT)
                {
                    action |= (start->action & FASTIP_ACTION_BYTECNT);
                    if (action & FASTIP_ACTION_BYTECNT)
                    {
                        pip_conn_cache->bytecnt_ptr = start->bytecnt_ptr;
                    }
                }

                pip_conn_cache->action = action;
                c++;
#ifdef FASTIP_DEBUG
                FASTIP_TRACE("%s: %s ", __FUNCTION__, \
                                (pip_conn_cache->tuple.proto == FASTIP_NUM_6) ? "tcp" : "udp");
                if (IS_V4ADDR(pip_conn_cache->tuple.sip))
                {
                    FASTIP_TRACE("%x:%x=>%x:%x", \
                                 NTOH32(pip_conn_cache->tuple.sip[FASTIP_NUM_0]), NTOH16(pip_conn_cache->tuple.sp),
                                 NTOH32(pip_conn_cache->tuple.dip[FASTIP_NUM_0]), NTOH16(pip_conn_cache->tuple.dp));
                }
                else
                {
                    FASTIP_TRACE("%x.%x.%x.%x:%x=>%x.%x.%x.%x:%x", \
                                 NTOH32(pip_conn_cache->tuple.sip[FASTIP_NUM_0]), \
                                 NTOH32(pip_conn_cache->tuple.sip[FASTIP_NUM_1]), \
                                 NTOH32(pip_conn_cache->tuple.sip[FASTIP_NUM_2]), \
                                 NTOH32(pip_conn_cache->tuple.sip[FASTIP_NUM_3]), \
                                 NTOH16(pip_conn_cache->tuple.sp), \
                                 NTOH32(pip_conn_cache->tuple.dip[FASTIP_NUM_0]), \
                                 NTOH32(pip_conn_cache->tuple.dip[FASTIP_NUM_1]), \
                                 NTOH32(pip_conn_cache->tuple.dip[FASTIP_NUM_2]), \
                                 NTOH32(pip_conn_cache->tuple.dip[FASTIP_NUM_3]), \
                                 NTOH16(pip_conn_cache->tuple.dp));
                }

                if (action & FASTIP_ACTION_TOS)
                {
                    FASTIP_TRACE(" tos=%x", pip_conn_cache->tos);
                }

                if (action & FASTIP_ACTION_MARK)
                {
                    FASTIP_TRACE(" mark=0x%x", FASTIP_MARK(pip_conn_cache->mark));
                }

                if (action & FASTIP_ACTION_BYTECNT)
                {
                    FASTIP_TRACE(" cntr at 0x%p", pip_conn_cache->bytecnt_ptr);
                }

                FASTIP_TRACE(" %s!\n",
                             (action & FASTIP_ACTION_SUSPEND) ? "suspend" : "forward");
#endif/* FASTIP_DEBUG */
            }

            pip_conn_cache = pip_conn_cache->next;
        }
    }

    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

    FASTIP_TRACE("%s: %d of %d updated!\n", __FUNCTION__, c, j);
    return (c == 0 ? FASTIP_NOTFOUND : FASTIP_OK);
}

/*-------------------------------------------------
函数原型： static int32 _fastip_ip_conn_cache_delete_multi(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache,
                                                fastip_ip_conn_cache_t *ip_conn_cachem,
                                                int32 v6)
描    述： 根据入参删除多条ip缓存连接跟踪记录
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_ip_conn_cache_delete_multi(fastip_t *fastip_handle, fastip_ip_conn_cache_t *ip_conn_cache,
                                                fastip_ip_conn_cache_t *ip_conn_cachem,
                                                int32 v6)
{
    uint32 c, i, n, j;
    fastip_ip_conn_cache_t *next, *prev = NULL, *pip_conn_cache;

    /* Validate the input params */
    if ((NULL == fastip_handle) || (NULL == FASTIP_IP_CONN_CACHEP(fastip_handle)))
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (fastip_handle %p)", __FUNCTION__, fastip_handle);
        return (FASTIP_BADARG);
    }

    if ((NULL == ip_conn_cache) || (NULL == ip_conn_cachem))
    {
        FASTIP_TRACE_ERROR("%s: Invalid input (ip_conn_cache %p ip_conn_cachem %p)",
                           __FUNCTION__, fastip_handle, FASTIP_IP_CONN_CACHEP(fastip_handle));
        return (FASTIP_BADARG);
    }

#ifndef FASTIP_IPV6
    if (v6)
    {
        return (FASTIP_BADARG);
    }
#endif/* FASTIP_IPV6 */


    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);
    c = priv_fastip_handle.ip_conn_cache_count;
    for (i = 0; i < FASTIP_IP_CONN_CACHE_SZ; i++)
    {
        prev = NULL;
        pip_conn_cache = FASTIP_IP_CONN_CACHEP(fastip_handle)[i];
        while (NULL != pip_conn_cache)
        {
            next = pip_conn_cache->next;

            /* Match the source mac address */
            n  = FASTIP_NE(ip_conn_cache->sa.octet[FASTIP_NUM_0], pip_conn_cache->sa.octet[FASTIP_NUM_0], \
                            ip_conn_cachem->sa.octet[FASTIP_NUM_0]);
            n |= FASTIP_NE(ip_conn_cache->sa.octet[FASTIP_NUM_1], pip_conn_cache->sa.octet[FASTIP_NUM_1], \
                            ip_conn_cachem->sa.octet[FASTIP_NUM_1]);
            n |= FASTIP_NE(ip_conn_cache->sa.octet[FASTIP_NUM_2], pip_conn_cache->sa.octet[FASTIP_NUM_2], \
                            ip_conn_cachem->sa.octet[FASTIP_NUM_2]);
            n |= FASTIP_NE(ip_conn_cache->sa.octet[FASTIP_NUM_3], pip_conn_cache->sa.octet[FASTIP_NUM_3], \
                            ip_conn_cachem->sa.octet[FASTIP_NUM_3]);
            n |= FASTIP_NE(ip_conn_cache->sa.octet[FASTIP_NUM_4], pip_conn_cache->sa.octet[FASTIP_NUM_4], \
                            ip_conn_cachem->sa.octet[FASTIP_NUM_4]);
            n |= FASTIP_NE(ip_conn_cache->sa.octet[FASTIP_NUM_5], pip_conn_cache->sa.octet[FASTIP_NUM_5], \
                            ip_conn_cachem->sa.octet[FASTIP_NUM_5]);

            if (n)
            {
                goto nomatch;
            }

            /* Match the dest mac address */
            n = FASTIP_NE(ip_conn_cache->dhost.octet[FASTIP_NUM_0], pip_conn_cache->dhost.octet[FASTIP_NUM_0],
                          ip_conn_cachem->dhost.octet[FASTIP_NUM_0]);
            n |= FASTIP_NE(ip_conn_cache->dhost.octet[FASTIP_NUM_1], pip_conn_cache->dhost.octet[FASTIP_NUM_1],
                           ip_conn_cachem->dhost.octet[FASTIP_NUM_1]);
            n |= FASTIP_NE(ip_conn_cache->dhost.octet[FASTIP_NUM_2], pip_conn_cache->dhost.octet[FASTIP_NUM_2],
                           ip_conn_cachem->dhost.octet[FASTIP_NUM_2]);
            n |= FASTIP_NE(ip_conn_cache->dhost.octet[FASTIP_NUM_3], pip_conn_cache->dhost.octet[FASTIP_NUM_3],
                           ip_conn_cachem->dhost.octet[FASTIP_NUM_3]);
            n |= FASTIP_NE(ip_conn_cache->dhost.octet[FASTIP_NUM_4], pip_conn_cache->dhost.octet[FASTIP_NUM_4],
                           ip_conn_cachem->dhost.octet[FASTIP_NUM_4]);
            n |= FASTIP_NE(ip_conn_cache->dhost.octet[FASTIP_NUM_5], pip_conn_cache->dhost.octet[FASTIP_NUM_5],
                           ip_conn_cachem->dhost.octet[FASTIP_NUM_5]);

            if (n)
            {
                goto nomatch;
            }

            /* Match the 5 tuple */
            for (j = 0; j < ((v6) ? FASTIP_IPADDR_U32_SZ : 1); j++)
            {
                n |= FASTIP_NE(ip_conn_cache->tuple.sip[j], pip_conn_cache->tuple.sip[j], ip_conn_cachem->tuple.sip[j]);
                n |= FASTIP_NE(ip_conn_cache->tuple.dip[j], pip_conn_cache->tuple.dip[j], ip_conn_cachem->tuple.dip[j]);
            }

            n |= FASTIP_NE(ip_conn_cache->tuple.sp, pip_conn_cache->tuple.sp, ip_conn_cachem->tuple.sp);
            n |= FASTIP_NE(ip_conn_cache->tuple.dp, pip_conn_cache->tuple.dp, ip_conn_cachem->tuple.dp);
            n |= FASTIP_NE(ip_conn_cache->tuple.proto, pip_conn_cache->tuple.proto, ip_conn_cachem->tuple.proto);

            if (n)
            {
                goto nomatch;
            }

            if (!v6)
            {
                n  = FASTIP_NE(ip_conn_cache->nat.ip, pip_conn_cache->nat.ip, ip_conn_cachem->nat.ip);
                n |= FASTIP_NE(ip_conn_cache->nat.port, pip_conn_cache->nat.port, ip_conn_cachem->nat.port);

                if (n)
                {
                    goto nomatch;
                }
            }

            /* Delete the entry and see if more matches exist */
            if (NULL != prev)
            {
                prev->next = pip_conn_cache->next;
            }
            else
            {
                FASTIP_IP_CONN_CACHEP(fastip_handle)[i] = pip_conn_cache->next;
            }

            FASTIP_TRACE("%s: Deleted fastip ip_conn_cache entry\n", __FUNCTION__);

            priv_fastip_handle.ip_conn_cache_count--;

            MFREE(pip_conn_cache);
            pip_conn_cache = next;
            continue;

nomatch:
            prev = pip_conn_cache;
            pip_conn_cache = next;
        }
    }

    c -= priv_fastip_handle.ip_conn_cache_count;
    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

    return (c == 0 ? FASTIP_NOTFOUND : FASTIP_OK);
}

/*-------------------------------------------------
函数原型： static fastip_ip_conn_cache_t *_fastip_forward_v4(fastip_t *fastip_handle, struct fastip_ipv4_hdr *ip)
描    述： 进行ipv4 加速
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static fastip_ip_conn_cache_t *_fastip_forward_v4(fastip_t *fastip_handle, struct fastip_ipv4_hdr *ip)
{
    fastip_ip_conn_cache_t *pip_conn_cache;
    uint8 *data;
    struct fastip_tcp_hdr *h;
    uint16 *chksum;
    uint16 upd_chksum;
    int32 action;

    if (FASTIP_IP_VER_4 != FASTIP_IP_VER(ip))
    {
        return NULL;
    }

    /* fastip 不支持分片 */
    if (0 != (ip->frag & HTON16(FASTIP_IPV4_FRAG_MORE | FASTIP_IPV4_FRAG_OFFSET_MASK)))
    {
        return NULL;
    }

    data = ((uint8 *)ip) + FASTIP_IPV4_HLEN(ip);
    h = (struct fastip_tcp_hdr *)data;

    if (NIP_ISMULTI(*((uint32 *)(ip->dst_ip))))
    {
        return NULL;
    }

    if (FASTIP_IP_PROT_TCP == ip->prot)
    {
        /*控制报文返回协议栈处理用于更新状态，不进行转发
         */
        if (h->hdrlen_rsvd_flags &
            HTON16(FASTIP_TCP_FLAG_RST | FASTIP_TCP_FLAG_FIN | FASTIP_TCP_FLAG_SYN))
        {
            return NULL;
        }

        chksum = (uint16 *)(data + FASTIP_TCP_CHKSUM_OFFSET);
        upd_chksum = 1;
    }
    else if (FASTIP_IP_PROT_UDP == ip->prot)
    {
        /* No need to update cksum if the udp cksum field
         * is zero.
         */
        chksum = (uint16 *)(data + FASTIP_UDP_CHKSUM_OFFSET);
        upd_chksum = *chksum;
    }
    else
    {
        return NULL;
    }

    pip_conn_cache = _fastip_ip_conn_cache_lkup_ll(fastip_handle, FALSE, (uint32 *)&ip->src_ip, (uint32 *)&ip->dst_ip,
                                                   ip->prot, h->src_port, h->dst_port);

    if (NULL == pip_conn_cache)
    {
        return NULL;
    }

    /* 获取NAT类型等信息 */
    action = pip_conn_cache->action;

    /* 被挂起的报文不进行转发*/
    if (action & FASTIP_ACTION_SUSPEND)
    {
        return NULL;
    }

    /* Do DSCP and checksum update */
    if ((action & FASTIP_ACTION_TOS)
       && (pip_conn_cache->tos != (ip->tos & FASTIP_IPV4_TOS_DSCP_MASK)))
    {
        uint16 ver_ihl_tos, ver_ihl_tos_new;

        ver_ihl_tos = *(uint16 *)ip;
        ip->tos = (ip->tos & ~FASTIP_IPV4_TOS_DSCP_MASK) | pip_conn_cache->tos;
        ver_ihl_tos_new = *(uint16 *)ip;
        ip->hdr_chksum = fastip_dev_csum((ver_ihl_tos ^ FASTIP_NUM_FFFF),
                                         ver_ihl_tos_new, ip->hdr_chksum);
    }

    if (action & FASTIP_ACTION_SNAT)
    {

        /* If supported cksum can be done in HW */
        if (upd_chksum)
        {
#if 1
            if (fastip_ttl_flag)
            {
                /* 过路由器后 ttl -1 防止异常*/
                if (FASTIP_NUM_1 < ip->ttl)
                {
                    ip->hdr_chksum  = fastip_dev_csum((ip->ttl ^ FASTIP_NUM_FFFF),
                                                     (ip->ttl - 1), ip->hdr_chksum );
                    ip->ttl--;
                }
            }
#endif
            *chksum = fastip_dev_csum(~(*((uint32 *)ip->src_ip)),
                                      pip_conn_cache->nat.ip,
                                      fastip_dev_csum(h->src_port ^ FASTIP_NUM_FFFF,
                                                      pip_conn_cache->nat.port, *chksum));
        }

        /* 修改源端口 */
        h->src_port = pip_conn_cache->nat.port;

        ip->hdr_chksum = fastip_dev_csum(~(*((uint32 *)ip->src_ip)),
                                         pip_conn_cache->nat.ip,
                                         ip->hdr_chksum);

        /* 更新源IP */
        *((uint32 *)ip->src_ip) = pip_conn_cache->nat.ip;
    }
    else if (action & FASTIP_ACTION_DNAT)
    {
        /* If supported cksum can be done in HW */
        if (upd_chksum)
        {
#if 1
            if (fastip_ttl_flag)
            {
                /* 过路由器后 ttl -1 防止异常*/
                if (FASTIP_NUM_1 < ip->ttl)
                {
                    ip->hdr_chksum  = fastip_dev_csum((ip->ttl ^ FASTIP_NUM_FFFF),
                                                     (ip->ttl - 1), ip->hdr_chksum );
                    ip->ttl--;
                }
            }
#endif
            *chksum = fastip_dev_csum(~(*((uint32 *)ip->dst_ip)),
                                      pip_conn_cache->nat.ip,
                                      fastip_dev_csum(h->dst_port ^ FASTIP_NUM_FFFF,
                                                      pip_conn_cache->nat.port, *chksum));
        }

        /* 修改目的端口 */
        h->dst_port = pip_conn_cache->nat.port;

        ip->hdr_chksum = fastip_dev_csum(~(*((uint32 *)ip->dst_ip)),
                                         pip_conn_cache->nat.ip,
                                         ip->hdr_chksum);

        /* 更新目的ip */
        *((uint32 *)ip->dst_ip) = pip_conn_cache->nat.ip;
    }

    return pip_conn_cache;
}

#ifdef FASTIP_IPV6

/*-------------------------------------------------
函数原型： static uint8 * _fastip_ip_conn_cache_lkup_l4proto_v6(uint8 *iph, uint8 *proto_num)
描    述： 返回IPV6报文中TCP/UDP 传输层头地址
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static uint8 * _fastip_ip_conn_cache_lkup_l4proto_v6(uint8 *iph, uint8 *proto_num)
{
    struct ipv6hdr *ip6;
    struct fastip_ipv6_exthdr *eh;
    uint8 next_hdr;
    uint32 hdr_len;

    if (FASTIP_IP_VER_6 != FASTIP_IP_VER(iph))
    {
        return (NULL);
    }

    ip6 = (struct ipv6hdr *)iph;
    eh = (struct fastip_ipv6_exthdr *)ip6;
	next_hdr = ip6->nexthdr;
    hdr_len = sizeof(struct ipv6hdr);
    for (;;)
    {
        if ((next_hdr == FASTIP_IP_PROT_TCP) || (next_hdr == FASTIP_IP_PROT_UDP))
        {
            /* Only TCP and UDP are currently supported */
            break;
        }

        /* Keep looking only if next header is Authentication,
         *  Destination option, or Routing extension headers.
         *  Hop-by-hop header must be inspected by each node.
         *  Fragmentation header is currently not supported.
         *  Data after ESP header is encrypted.
         *  No next header means no L4 payload.
         *  Other values are L4 other than the supported above.
         */
        if ((next_hdr != FASTIP_IPV6_EXTHDR_AUTH) && (next_hdr != FASTIP_IPV6_EXTHDR_DEST)
           && (next_hdr != FASTIP_IPV6_EXTHDR_ROUTING))
        {
            return NULL;
        }

        eh = (struct fastip_ipv6_exthdr *)((uint8 *)eh + hdr_len);
        hdr_len = (next_hdr == FASTIP_IPV6_EXTHDR_AUTH) ?
                  ((eh->hdrlen + FASTIP_NUM_2) << FASTIP_NUM_2) : FASTIP_IPV6_EXTHDR_LEN(eh);
        next_hdr = eh->nexthdr;
    }
    
    *proto_num = next_hdr;
    return ((uint8 *)eh + hdr_len);
}

/*-------------------------------------------------
函数原型： static fastip_ip_conn_cache_t * _fastip_forward_v6(fastip_t *fastip_handle, struct ipv6hdr *ip6)
描    述： 进行ipv6 加速
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static fastip_ip_conn_cache_t * _fastip_forward_v6(fastip_t *fastip_handle, struct ipv6hdr *ip6)
{
    fastip_ip_conn_cache_t *pip_conn_cache;
    uint8 protocol;
    struct fastip_tcp_hdr *h;

    h = (struct fastip_tcp_hdr *)_fastip_ip_conn_cache_lkup_l4proto_v6((uint8 *)ip6, &protocol);
    if (NULL == h)
    {
        return (NULL);
    }

    if (protocol == FASTIP_IP_PROT_TCP)
    {
        if (h->hdrlen_rsvd_flags & HTON16(FASTIP_TCP_FLAG_RST | FASTIP_TCP_FLAG_FIN | FASTIP_TCP_FLAG_SYN))
        {
            return (NULL);
        }
    }
    pip_conn_cache = _fastip_ip_conn_cache_lkup_ll(fastip_handle, TRUE, (uint32 *)&ip6->saddr, (uint32 *)&ip6->daddr,
                                                   protocol, h->src_port, h->dst_port);

    /* Suspend forwarding */
    if ((NULL == pip_conn_cache) || (pip_conn_cache->action & FASTIP_ACTION_SUSPEND))
    {
        return NULL;
    }

    /* 过路由器后 hop_limit -1 防止异常*/  
#if 1
    if (fastip_ttl_flag)
    {
        /* 过路由器后 ttl -1 防止异常*/
        if (FASTIP_NUM_1 < ip6->hop_limit)
        {
            ip6->hop_limit--;
        }
    }
#endif
    /* Update DSCP */
    if (pip_conn_cache->action & FASTIP_ACTION_TOS)
    {
        uint8 *ipv6body = (uint8 *)ip6;

        if ((ipv6body[FASTIP_NUM_0] & (FASTIP_IPV4_TOS_DSCP_MASK >> FASTIP_NUM_4)) != (pip_conn_cache->tos >> FASTIP_NUM_4))
        {
            ipv6body[FASTIP_NUM_0] = (ipv6body[FASTIP_NUM_0] & ~(FASTIP_IPV4_TOS_DSCP_MASK >> FASTIP_NUM_4)) |
                          (pip_conn_cache->tos >> FASTIP_NUM_4);
        }

        if ((ipv6body[FASTIP_NUM_1] & (FASTIP_IPV4_TOS_DSCP_MASK << FASTIP_NUM_4)) != (pip_conn_cache->tos << FASTIP_NUM_4))
        {
            ipv6body[FASTIP_NUM_1] = (ipv6body[FASTIP_NUM_1] & ~(FASTIP_IPV4_TOS_DSCP_MASK << FASTIP_NUM_4)) |
                          (pip_conn_cache->tos << FASTIP_NUM_4);
        }
    }

    return pip_conn_cache;
}

#endif/* FASTIP_IPV6 */


/*-------------------------------------------------
函数原型： static int32 _fastip_forward(fastip_t *fastip_handle, void *p, void *rxifp)
描    述： 对数据包进行处理，并且根据ip或者网桥信息进行数传加速
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 _fastip_forward(fastip_t *fastip_handle, void *p, void *rxifp)
{
    struct fastip_ethervlan_header *evh;
    struct fastip_ipv4_hdr *ip;
    uint8 *data;
    int16 vid_in = -1, vid;
    int32 action, prio = 0;
    uint16 ether_type;
    fastip_br_conn_cache_t *pbr_conn_cache;
    void *txifp;
    uint64 *bytecnt_ptr = NULL;

#ifdef FASTIP_IPV6
    struct ipv6hdr *ip6 = NULL;
#endif/* FASTIP_IPV6 */

    /*
     *  如果关闭加速，在此进行判断，不走加速
     */
    if (0 != fastip_run_flag)
    {
        return (FASTIP_ERROR);
    }

    FASTIP_TRACE("%s: fastip_handle:%x p:%x rxifp:%x\n", __FUNCTION__, fastip_handle, p, rxifp);
   
    /*
     *  如果数据包标记为不进行加速，在此进行判断，不走加速
     */
    if (FASTIPSKIPCONNCACHET(p))
    {
        return (FASTIP_ERROR);
    }

    data = FASTIPDATA(p);
    evh = (struct fastip_ethervlan_header *)data;

    FASTIP_TRACE("%s: data:%x evh:%x\n", __FUNCTION__, data, evh);
    /* 多播或广播包，不支持进行加速 */
    if (evh->ether_dhost[0] & 1)
    {
        return (FASTIP_ERROR);
    }
    FASTIP_TRACE("%s: rxifp name:%s FASTIPLEN:%d\n", __FUNCTION__, FASTIP_DEV_IFNAME(rxifp), FASTIPLEN(p));
    ether_type = evh->vlan_type;
    switch (ether_type)
    {
    case HTON16(FASTIP_ETHER_TYPE_8021Q):
        if (HTON16(FASTIP_ETHER_TYPE_IP) == evh->ether_type)
        {
            ip = (struct fastip_ipv4_hdr *)(data + FASTIP_ETHERVLAN_HDR_LEN);
#ifdef FASTIP_IPV6
        }
        else if (HTON16(FASTIP_ETHER_TYPE_IPV6) == evh->ether_type)
        {
            ip  = NULL;
            ip6 = (struct ipv6hdr *)(data + FASTIP_ETHERVLAN_HDR_LEN);
#endif/* FASTIP_IPV6 */
        }
        else
        {
            return (FASTIP_ERROR);
        }

        prio   = (NTOH16(evh->vlan_tag) & FASTIP_VLAN_PRI_MASK) >> FASTIP_VLAN_PRI_SHIFT;
        vid_in = NTOH16(evh->vlan_tag) & VLAN_VID_MASK;
        break;

    case HTON16(FASTIP_ETHER_TYPE_IP):
        ip   = (struct fastip_ipv4_hdr *)(data + FASTIP_ETHER_HDR_LEN);
        prio = (FASTIP_IPV4_TOS(ip) >> FASTIP_IPV4_TOS_PREC_SHIFT);
        break;
#ifdef FASTIP_IPV6
    case HTON16(FASTIP_ETHER_TYPE_IPV6):
        ip   = NULL;
        ip6  = (struct ipv6hdr *)(data + FASTIP_ETHER_HDR_LEN);
        prio = FASTIP_IPV6_TRAFFIC_CLASS(ip6);
        break;
#endif/* FASTIP_IPV6 */
    default:
        FASTIP_TRACE("%s: Non ip or vlan frame\n", __FUNCTION__);
        return (FASTIP_ERROR);
    }

    FASTIP_BR_CONN_CACHE_LOCK(fastip_handle);

    /* 查找网桥缓存记录 */
    if ((pbr_conn_cache = _fastip_br_conn_cache_lkup_ll(fastip_handle, evh->ether_dhost)) != NULL)
    {
        fastip_br_conn_cache_t *spbr_conn_cache;

        /* 回环报文不进行处理 */
        if (pbr_conn_cache->txifp == rxifp)
        {
            goto br_conn_cache_err;
        }

        action = pbr_conn_cache->action;
        txifp = pbr_conn_cache->txifp;
        vid = pbr_conn_cache->vid;
        FASTIP_CONN_CACHEN_ADD(pbr_conn_cache->hits, FASTIPPKTCCNT(p));

        /* 更新生存周期
         */
        if ((spbr_conn_cache = _fastip_br_conn_cache_lkup_ll(fastip_handle, evh->ether_shost)) != NULL)
        {
            spbr_conn_cache->live = fastip_getjiffies();
        }
        else
        {
            goto br_conn_cache_err;
        }

        /* 更新bytecount */
        if (action & FASTIP_ACTION_BYTECNT)
        {
            bytecnt_ptr = pbr_conn_cache->bytecnt_ptr;
        }

        /* 更新网桥缓存记录*/
        FASTIP_BR_CONN_CACHE_HOT_UPDATE(fastip_handle, rxifp, evh->ether_dhost, pbr_conn_cache);

        FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);
    }
    else
    {
        fastip_ip_conn_cache_t *pip_conn_cache;

        FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);

        FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);

#ifdef FASTIP_IPV6
        if (ip6)
        {
            pip_conn_cache = _fastip_forward_v6(fastip_handle, ip6);
        }
        else
#endif/* FASTIP_IPV6 */
        pip_conn_cache = _fastip_forward_v4(fastip_handle, ip);

        if (NULL == pip_conn_cache)
        {
            FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);
            return (FASTIP_ERROR);
        }

        /* 获取NAT类型 */
        action = pip_conn_cache->action;

        /* 修改目的mac地址
         */
        fastip_ether_copy(pip_conn_cache->dhost.octet, evh->ether_dhost);

        /* 修改源mac地址
         */
        fastip_ether_copy(pip_conn_cache->shost.octet, evh->ether_shost);

        txifp = pip_conn_cache->txif;
        vid = pip_conn_cache->vid;
        pip_conn_cache->live = fastip_getjiffies();
        FASTIP_CONN_CACHEN_INCR(pip_conn_cache->hits);

        /* 桥接口，更新记录生存周期
         */
        if (pip_conn_cache->pbr_conn_cache != NULL)
        {
            pip_conn_cache->pbr_conn_cache->live = fastip_getjiffies();
        }

        /* 复制MARK信息 */
        if (action & FASTIP_ACTION_MARK)
        {
            FASTIPSETMARK(p, FASTIP_MARK(pip_conn_cache->mark));
        }

        /*  获取bytecount指针，更新统计信息*/
        if (action & FASTIP_ACTION_BYTECNT)
        {
            bytecnt_ptr = pip_conn_cache->bytecnt_ptr;
        }

        FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);
    }

    /* 处理VLAN TAG */
    if (action & FASTIP_ACTION_TAG)
    {
        uint16 vlan_tag;

        vlan_tag = HTON16(prio << FASTIP_VLAN_PRI_SHIFT | vid);
        if (vid_in != -1)
        {
            FASTIP_TRACE("%s: Updating tag with vid %d\n", __FUNCTION__, vid);

            /* 更新VLAN统计 */
            FASTIP_VLAN_RXSTATS(fastip_handle, p, rxifp, vid_in);

            /* 更新VLAN TAG
             */
            evh->vlan_tag = vlan_tag;
        }
        else
        {
            FASTIP_TRACE("%s: Tagging with vid %d\n", __FUNCTION__, vid);

            /* 非VLAN包发给VLAN设备，添加VLAN TAG
             */
            data = FASTIPPUSH(p, FASTIP_VLAN_TAG_LEN);
            fastip_ether_copy(data + FASTIP_VLAN_TAG_LEN, data);
            fastip_ether_copy(data + FASTIP_ETHER_ADDR_LEN + FASTIP_VLAN_TAG_LEN, data + FASTIP_ETHER_ADDR_LEN);
            evh = (struct fastip_ethervlan_header *)data;
            evh->vlan_type = HTON16(FASTIP_ETHER_TYPE_8021Q);
            evh->vlan_tag   = vlan_tag;
            evh->ether_type = ether_type;
        }

        /* 更新VLAN统计 */
        FASTIP_VLAN_TXSTATS(fastip_handle, p, txifp, vid);
    }
    else if (action & FASTIP_ACTION_UNTAG)
    {
        if (vid_in != -1)
        {
            FASTIP_TRACE("%s: Untagging vid %d\n", __FUNCTION__, vid);

            /* 更新VLAN统计 */
            FASTIP_VLAN_RXSTATS(fastip_handle, p, rxifp, vid_in);

            /* VLAN包发给非VLAN设备，删除VLAN TAG.
             */
            data = FASTIPPULL(p, FASTIP_VLAN_TAG_LEN);
            ether_rcopy(data - FASTIP_VLAN_TAG_LEN + FASTIP_ETHER_ADDR_LEN, data + FASTIP_ETHER_ADDR_LEN);
            ether_rcopy(data - FASTIP_VLAN_TAG_LEN, data);
        }
    }

    FASTIPSETPRIO(p, prio);

    /* 更新统计计数 */
    if (bytecnt_ptr)
    {
        *bytecnt_ptr += FASTIPPKTCLEN(p);
    }

    FASTIP_TRACE("%s: fastip_dev_xmit txifp:%s\n", __FUNCTION__, ((struct net_device *)txifp)->name);

    if (fastip_flow_ctl_flag)
    {
        if (netif_queue_stopped((struct net_device *)txifp))
        {
            return (FASTIP_ERROR);
        }
    }
    
    fastip_dev_xmit(p, txifp,
                    (action & FASTIP_ACTION_TAG) ? FASTIP_ETHERVLAN_HDR_LEN : FASTIP_ETHER_HDR_LEN);

    return (FASTIP_OK);

br_conn_cache_err:
    FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);
    return (FASTIP_ERROR);
}

/*-------------------------------------------------
函数原型： static void _fastip_dump(fastip_t *fastip_handle)
描    述： 显示加速debug信息
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void _fastip_dump(fastip_t *fastip_handle)
{
    int32 i, j;
    fastip_ip_conn_cache_t *pip_conn_cache = NULL;
    fastip_br_conn_cache_t *pbr_conn_cache = NULL;
    fastip_dev_t *ctd = NULL;
    char eabuf[FASTIP_ETHER_ADDR_STR_LEN] = {0};

    if (!fastip_handle)
    {
        return;
    }

    printk("Fastip IP contrack caches num: %d \n", priv_fastip_handle.ip_conn_cache_count);
    printk("%-34s %-34sHits %5s %7s %7s\n",
           "SrcIP", "DestIP", "Proto", "SrcPort", "DstPort");
    FASTIP_IP_CONN_CACHE_LOCK(fastip_handle);
    for (i = 0; i < FASTIP_IP_CONN_CACHE_SZ; i++)
    {
        pip_conn_cache = FASTIP_IP_CONN_CACHEP(fastip_handle)[i];
        while (NULL != pip_conn_cache)
        {
            if (IS_V4ADDR(pip_conn_cache->tuple.sip))
            {
                printk("%u.%u.%u.%u\t\t\t  %u.%u.%u.%u\t\t\t ",
                       FASTIP_NIPQUAD(pip_conn_cache->tuple.sip[FASTIP_NUM_0]), FASTIP_NIPQUAD(pip_conn_cache->tuple.dip[FASTIP_NUM_0]));
            }
            else
            {
                printk("%08x.%08x.%08x.%08x %08x.%08x.%08x.%08x ", \
                       NTOH32(pip_conn_cache->tuple.sip[FASTIP_NUM_0]), NTOH32(pip_conn_cache->tuple.sip[FASTIP_NUM_1]), \
                       NTOH32(pip_conn_cache->tuple.sip[FASTIP_NUM_2]), NTOH32(pip_conn_cache->tuple.sip[FASTIP_NUM_3]), \
                       NTOH32(pip_conn_cache->tuple.dip[FASTIP_NUM_0]), NTOH32(pip_conn_cache->tuple.dip[FASTIP_NUM_1]), \
                       NTOH32(pip_conn_cache->tuple.dip[FASTIP_NUM_2]), NTOH32(pip_conn_cache->tuple.dip[FASTIP_NUM_3]));
            }

            printk("%u %-5s %-7.04x %-7.04x\n", pip_conn_cache->hits, \
                   ((pip_conn_cache->tuple.proto == FASTIP_NUM_6) ? "tcp" : "udp"), \
                   NTOH16(pip_conn_cache->tuple.sp), NTOH16(pip_conn_cache->tuple.dp));
            pip_conn_cache = pip_conn_cache->next;
        }
    }

    FASTIP_IP_CONN_CACHE_UNLOCK(fastip_handle);

    printk("\nFastip Bridge cache:\n");
    printk("Interface\t\t\t\tMacAddr\t\t\t\tHits\n");

    FASTIP_BR_CONN_CACHE_LOCK(fastip_handle);
    for (i = 0; i < FASTIP_BR_CONN_CACHE_SZ; i++)
    {
        pbr_conn_cache = FASTIP_BR_CONN_CACHEP(fastip_handle)[i];
        while (NULL != pbr_conn_cache)
        {
            printk("%s\t\t\t\t\%s\t\t%lu\n",
                   FASTIP_DEV_IFNAME(pbr_conn_cache->txifp), \
                   fastip_ether_ntoa(&pbr_conn_cache->dhost, eabuf), \
                   pbr_conn_cache->hits);
            pbr_conn_cache = pbr_conn_cache->next;
        }
    }

    printk("\nFastip Hot Bridge cache:\n");
    for (i = 0; i < FASTIP_MAXBR_CONN_CACHE_HOTIF; i++)
    {
        for (j = 0; j < FASTIP_MAXBR_CONN_CACHE_HOT; j++)
        {
            if (!FASTIP_ETHER_ISNULLADDR(&FASTIP_BR_CONN_CACHE_HOT(fastip_handle, i, j).ea))
            {
                printk("%d.%d: %s\n", i, j,
                       fastip_ether_ntoa(&FASTIP_BR_CONN_CACHE_HOT(fastip_handle, i, j).ea, eabuf));
            }
        }
    }
    
    printk("\nFastip Register dev:\n");

    /* Dont crash if list is nil */
    if (NULL == ((fastip_info_t *)(fastip_handle))->dev_list)
    {
        return NULL;
    }

    /* Device list lookup */
    ctd = FASTIP_DEV_LIST_HEAD(fastip_handle);
    while (NULL != ctd)
    {
        if (ctd->dev)
        {
            printk("\t%s ", FASTIP_DEV_IFNAME(ctd->dev));
        }
        ctd = ctd->next;
    }
    printk("\n");

    FASTIP_BR_CONN_CACHE_UNLOCK(fastip_handle);
}

/*-------------------------------------------------
函数原型： void fastip_show_dump()
描    述： 显示加速debug信息
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_show_dump()
{
    fastip_dump(kfastip_handle);
}

/*-------------------------------------------------
函数原型： void fastip_show_dump()
描    述： 显示加速debug信息
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_show_debug(int flag)
{
    debug_flag = flag;
}

/*-------------------------------------------------
函数原型： void fastip_stop(int flag)
描    述： fastip开关
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_start(int flag)
{
    fastip_run_flag = flag;
}

/*-------------------------------------------------
函数原型： void fastip_flow_ctl(int flag)
描    述： fastip wifi流控开关，1 开流控 2 关流控
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_flow_ctl(int flag)
{
    fastip_flow_ctl_flag = flag;
}

/*-------------------------------------------------
函数原型： void fastip_ttl(int flag)
描    述： fastip开关
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
void fastip_ttl(int flag)
{
    fastip_ttl_flag = flag;
}


/*-------------------------------------------------
函数原型： static int32 __init fastip_module_init(void)
描    述： 模块初始化
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 __init fastip_module_init(void)
{
    kfastip_handle = fastip_kattach("fastip");

    if (NULL == kfastip_handle)
    {
        printk("%s: fastip_kattach failed\n", __FUNCTION__);
        return -1;
    }
    printk("fastip ver:%s\n", FASTIP__VERSION);
    printk("%s: fastip_kattach kfastip_handle:%x\n", __FUNCTION__, kfastip_handle);
    fastip_attach_fn = _fastip_attach;

    return 0;
}

/*-------------------------------------------------
函数原型： static void __exit fastip_module_exit(void)
描    述： 模块去初始化
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void __exit fastip_module_exit(void)
{
    fastip_kdetach(kfastip_handle);
    fastip_attach_fn = NULL;
    kfastip_handle = NULL;
    return;
}

#else
/*-------------------------------------------------
函数原型： static int32 __init fastip_module_init(void)
描    述： 模块初始化
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static int32 __init fastip_module_init(void)
{
    return 0;
}

/*-------------------------------------------------
函数原型： static void __exit fastip_module_exit(void)
描    述： 模块去初始化
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static void __exit fastip_module_exit(void)
{
    return;
}

#endif
module_init(fastip_module_init);
module_exit(fastip_module_exit);
MODULE_LICENSE("Proprietary");

