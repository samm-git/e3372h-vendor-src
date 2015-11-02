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
#ifndef _FASTIP_MODULE_H_
#define _FASTIP_MODULE_H_
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/if_vlan.h>
#include <asm/checksum.h>
#include <linux/fastip/fastip.h>

/*FASTIP模块创建/删除锁接口*/
#define FASTIP_LOCK_CREATE(osh, lock) \
    { \
        if (sizeof(fastip_lock_t)) { \
            lock = KMALLOC(sizeof(osh)); \
            ASSERT(lock != NULL); \
            spin_lock_init(lock); \
        } \
    }

#define FASTIP_LOCK_DELETE(osh, lock) \
    if (sizeof(fastip_lock_t)) \
    { \
        KFREE(lock); \
    }
    
/*FASTIP模块自旋锁操作宏定义*/
#define FASTIP_BR_CONN_CACHE_LOCK(ci) spin_lock_bh(((fastip_info_t *)(ci))->br_conn_cache_lock)
#define FASTIP_IP_CONN_CACHE_LOCK(ci) spin_lock_bh(((fastip_info_t *)(ci))->ip_conn_cache_lock)
#define FASTIP_BR_CONN_CACHE_UNLOCK(ci) spin_unlock_bh(((fastip_info_t *)(ci))->br_conn_cache_lock)
#define FASTIP_IP_CONN_CACHE_UNLOCK(ci) spin_unlock_bh(((fastip_info_t *)(ci))->ip_conn_cache_lock)
#define FASTIP_DEV_LIST_LOCK(ci) spin_lock_bh(((fastip_info_t *)(ci))->dev_list->lock)
#define FASTIP_DEV_LIST_UNLOCK(ci) spin_unlock_bh(((fastip_info_t *)(ci))->dev_list->lock)

/*FASTIP模块设备宏定义*/
#define FASTIP_DEV_IFNAME(dev) (((struct net_device *)dev)->name)
#define FASTIP_DEV_IFIDX(dev) (((struct net_device *)dev)->ifindex)

/*FASTIP模块VLAN流量接口定义*/
#if defined (CTFVLSTATS) && (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
 #define FASTIP_VLAN_RXSTATS(ci, p, dev, vid) \
    do { \
            struct net_device *vldev; \
            FASTIP_DEV_LIST_LOCK(ci); \
            if ((vldev = _fastip_dev_vlan_find(ci, dev, vid)) != NULL) \
            { \
                struct sk_buff *skb = (struct sk_buff *)p; \
                struct net_device_stats *stats; \
                stats = vlan_dev_get_stats((struct net_device *)vldev); \
                stats->rx_packets += FASTIPPKTCCNT(p); \
                stats->rx_bytes += FASTIPPKTCLEN(p); \
            } \
            FASTIP_DEV_LIST_UNLOCK(ci); \
        } while (0)

 #define FASTIP_VLAN_TXSTATS(ci, p, dev, vid) \
    do { \
            struct net_device *vldev; \
            FASTIP_DEV_LIST_LOCK(ci); \
            if ((vldev = _fastip_dev_vlan_find(ci, dev, vid)) != NULL) \
            { \
                struct sk_buff *skb = (struct sk_buff *)p; \
                struct net_device_stats *stats; \
                stats = vlan_dev_get_stats((struct net_device *)vldev); \
                stats->tx_packets += FASTIPPKTCCNT(p); \
                stats->tx_bytes += FASTIPPKTCLEN(p); \
            } \
            FASTIP_DEV_LIST_UNLOCK(ci); \
        } while (0)
#else /* FASTVLSTATS */
 #define FASTIP_VLAN_RXSTATS(ci, p, dev, vid)
 #define FASTIP_VLAN_TXSTATS(ci, p, dev, vid)
#endif /* FASTVLSTATS */

#define FASTIP_VLAN_RXSTATS(ci, p, dev, vid)
#define FASTIP_VLAN_TXSTATS(ci, p, dev, vid)
#define KMALLOC(x) kmalloc(x, GFP_KERNEL)
#define KFREE(x) kfree(x)

/*-------------------------------------------------
函数原型： static inline uint16 fastip_dev_csum(uint32 oldval, uint32 newval, uint16 oldcksum)
描    述： csum校验和计算函数
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static inline uint16 fastip_dev_csum(uint32 oldval, uint32 newval, uint16 oldcksum)
{
    uint32 diffs[] = { oldval, newval };

    return csum_fold(csum_partial((char *)diffs, sizeof(diffs), oldcksum ^ FASTIP_NUM_FFFF));
}

/*-------------------------------------------------
函数原型： static inline void fastip_dev_xmit(void *p, void *txif, int mhdr_len)
描    述： 系统dev_queue_xmit 发包函数接口
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static inline void fastip_dev_xmit(void *p, void *txif, int mhdr_len)
{
    struct sk_buff *skb = (struct sk_buff *)p;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(FASTIP_NUM_2, FASTIP_NUM_6, FASTIP_NUM_22)
    skb_reset_mac_header(skb);
#else
    skb->mac.raw = skb->data;
#endif
    skb->dev = (struct net_device *)txif;

    /* QoS support */
    {
        skb->protocol = *(__be16 *)&skb->data[mhdr_len - FASTIP_ETHER_TYPE_LEN];
#if LINUX_VERSION_CODE >= KERNEL_VERSION(FASTIP_NUM_2, FASTIP_NUM_6, FASTIP_NUM_22)
        skb_set_network_header(skb, mhdr_len);
#else
        skb->nh.raw = skb->mac.raw + mhdr_len;
#endif
    }

    skb->dev->netdev_ops->ndo_start_xmit(skb, skb->dev);

    return;
}

/*-------------------------------------------------
函数原型： static inline uint32 fastip_getjiffies(void)
描    述： 获取系统jiffies
输    入： 		
输    出： 无
返 回 值： 无
--------------------------------------------------*/
static inline uint32 fastip_getjiffies(void)
{
    return (uint32)jiffies;
}

#endif

/* _FASTIP_MODULE_H_ */
