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
/**************************************************************问题单修改记录***********************************************************************
日    期           修改人           问题单号               修改内容
*****************************************************************************************************************************************************/

#ifndef __HW_NET_DEV_H
#define __HW_NET_DEV_H

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

#include <linux/delay.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include "../usb_config.h"
#include "../usb_platform_comm.h"
#ifdef CONFIG_BUS_PCI
#include <linux/pci.h>
#else
#include <linux/dma-mapping.h>
#endif
#include <linux/kernel.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/unaligned.h>
#include <linux/netdevice.h>
//#include <linux/netdevice_balong.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/wireless.h>
#include <linux/if_arp.h>
#include <net/iw_handler.h>

//#include <huawei_types.h>
//#include <huawei_sync.h>
//#include <mach/BSP.h>
/*
typedef struct net_rx_item_t {
    struct sk_buff *skb;
    mem_desc_t memdesc;
    uint32_t length;
    sg_list_item_t *sg_item;
    struct list_head list;
    BSP_U8 * u32RcvBuff;
} net_rx_item_t;
*/
#ifndef TRUE
#define TRUE                (1)
#endif

#ifndef FALSE
#define FALSE               (0)
#endif

typedef int                     bool_t;

enum RATE_TYPE_ENUM
{
    GSM_RATE = 0,
    GPRS_RATE,
    EDGE_RATE,
    WCDMA_RATE,
    HSDPA_RATE,
    RATE_TYPE_BUTT
};
typedef struct
{
    unsigned char gsm_connect_rate;
    unsigned char gprs_connect_rate;
    unsigned char edge_connect_rate;
    unsigned char wcdma_connect_rate;
    unsigned char hspda_connect_rate;
} nv_huawei_connect_display_rate_type;

typedef enum
{
    UWE_OK              =  0,
    UWE_NOTSTARTED ,
    UWE_INPROGRESS,
    UWE_PERM ,
    UWE_NOENT,
    UWE_IO ,
    UWE_NXIO,
    UWE_NOMEM,
    UWE_BUSY,
    UWE_NODEV,
    UWE_INVAL,
    UWE_NOTSUP,
    UWE_TIMEDOUT,
    UWE_SUSPENDED,
    UWE_UNKNOWN,
    UWE_TEST_FAILED,
    UWE_STATE,
    UWE_STALLED ,
    UWE_PARAM,
    UWE_ABORTED,
    UWE_SHORT,
    UWE_EXPIRED,
} result_t;

#define en_NV_Item_Default_Rate     50027
#define DEFAULT_RATE                21600000
uint32_t net_get_connect_rate(uint32_t ratetype);
void    USB_ETH_LinkStatSet( bool_t status );
bool_t USB_ETH_LinkStatGet(void);

#endif /* __HW_NET_DEV_H */

