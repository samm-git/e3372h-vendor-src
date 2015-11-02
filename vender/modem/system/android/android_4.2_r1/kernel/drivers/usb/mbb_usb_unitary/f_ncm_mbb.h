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

/**************************************************************************************************
    问题单号               时间               修改人                      说明  
***************************************************************************************************/

#ifndef _MBB_NCM_H
#define _MBB_NCM_H

#include "u_ether.h"
#include "drv_ncm.h"

#ifdef CONFIG_NCM_MBIM_SUPPORT
#define NCM_PKT_TYPE_ETH    0
#define NCM_PKT_TYPE_RAW    1
#define NCM_PKT_TYPE_DSS    2

typedef struct _ncm_skb_cb_s
{
    USB_UINT8 PktType;            /* IP数据包类型: 0 - 以太网数据包; 1 - RAW IP数据包; 2 - DSS数据包 */
    USB_UINT8 PdpChannel;         /* 多路PDP下，当前SKB所属的PDP会话编号，以主机的Session ID为准 */
}ncm_skb_cb_s;

/* 执行多PDP组包操作的下水线: 1:高于1路PDP进行组包; 0: 永久执行PDP组包，即使只有1路 */
#define MBIM_MPDP_LOW_WATERLINE   1

typedef struct ncm_mpdp_ndp_info_s
{
    USB_UINT32    tx_pkt_num;       /* 需要发送的数据包的数量 */
    USB_UINT32    offset;           /* 在NCM数据包中的位置偏移量 */
    USB_UINT32    next_ndp;         /* 下一个NDP的位置偏移量 */
    USB_PUCHAR    data_gram_ptr;    /* 数据包信息指针地址 */
}ncm_mpdp_ndp_info_t;
#endif

typedef struct ncm_vendor_ctx
{
    USB_BOOL is_mbim_mode;              /* if mbim mode now */
    USB_BOOL connect;
    USB_BOOL flow_control;
    NCM_IOCTL_CONNECTION_SPEED_S speed;
    struct completion notify_done;  /* notify completion */
    USB_BOOL tx_task_run;               /* tx task run */
    struct task_struct *tx_task;    /* tx task */
    struct completion tx_task_stop; /* notify tx task stoped */
    struct sk_buff_head tx_frames;  /* hold tx frames from vendor */
    struct completion tx_kick;      /* kick tx task */
    USB_VOID *ncm_priv;                 /* hold f_ncm */
    USB_VOID *vendor_priv;              /* hold app_ctx */

    /* stat info */
    USB_ULONG stat_rx_total;
    USB_ULONG stat_rx_einval;
    USB_ULONG stat_tx_total;
    USB_ULONG stat_tx_cancel;
    USB_ULONG stat_tx_xmit;
    USB_ULONG stat_tx_xmit_fail;
    USB_ULONG stat_tx_post;
    USB_ULONG stat_tx_drop;
    USB_ULONG stat_tx_nochl;
    USB_ULONG stat_tx_nodev;
    USB_ULONG stat_notify_timeout;
}ncm_vendor_ctx_t;

/* ncm debug mask */
#define NCM_DBG_MASK_AT                     BIT(0)
#define NCM_DBG_MASK_RX                     BIT(1)
#define NCM_DBG_MASK_TX                     BIT(2)
#define NCM_DBG_MASK_NTF                    BIT(3)

extern USB_UINT32 gNcmDbgMask;
#define NCM_DBG_MASK_CHECK(_mask) (unlikely(_mask == (gNcmDbgMask & _mask)))
#define NCM_DBG_PRT(fmt, args...) \
    printk(KERN_ERR "[NCM DBG] %s :" fmt , __FUNCTION__, ## args);

#define NCM_DBG_PRINT_AT(fmt, args...) if (NCM_DBG_MASK_CHECK(NCM_DBG_MASK_AT)) {NCM_DBG_PRT(fmt, ## args);}
#define NCM_DBG_PRINT_RX(fmt, args...) if (NCM_DBG_MASK_CHECK(NCM_DBG_MASK_RX)) {NCM_DBG_PRT(fmt, ## args);}
#define NCM_DBG_PRINT_TX(fmt, args...) if (NCM_DBG_MASK_CHECK(NCM_DBG_MASK_TX)) {NCM_DBG_PRT(fmt, ## args);}
#define NCM_DBG_PRINT_NTF(fmt, args...) if (NCM_DBG_MASK_CHECK(NCM_DBG_MASK_NTF)) {NCM_DBG_PRT(fmt, ## args);}

USB_VOID ncm_free_ntbnode(ntb_node_t *ntb_node);
ntb_node_t *ncm_alloc_ntbnode(USB_UINT32 size, gfp_t gfp_msk);
USB_UINT32 ncm_get_ntbnode(tx_ctx_t *ctx, USB_UINT32 size, gfp_t gfp_msk);
USB_VOID ncm_put_ntbnode(tx_ctx_t *ctx);
USB_BOOL ncm_get_bypass_state(USB_VOID);
USB_VOID usb_ncm_init(USB_VOID);

#endif

