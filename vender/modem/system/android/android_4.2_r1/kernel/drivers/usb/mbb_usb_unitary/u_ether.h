/*
 * u_ether.h -- interface to USB gadget "ethernet link" utilities
 *
 * Copyright (C) 2003-2005,2008 David Brownell
 * Copyright (C) 2003-2004 Robert Schwebel, Benedikt Spranger
 * Copyright (C) 2008 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/**************************************************************************************************
    问题单号           时间           修改人                    说明  
***************************************************************************************************/

#ifndef __U_ETHER_H
#define __U_ETHER_H

#include <linux/err.h>
#include <linux/if_ether.h>
#include <linux/usb/composite.h>
#include <linux/usb/cdc.h>

#include "gadget_chips.h"
#include "bsp_usb.h"
#include "usb_nv_get.h"
#include "hw_pnp.h"
#include "usb_notify.h"
#include "usb_debug.h"
#include "usb_vendor.h"

struct eth_dev;

#define    WORK_RX_MEMORY                      (0)
#define    WORK_RX_UPTASK                      (1)
#define    WORK_RX_BOTTROM                     (WORK_RX_UPTASK + 1)
#define    RX_FRAMES_MAX                       (2048)

/* gnet msg level */
#define GNET_LEVEL_ERR        BIT(0)
#define GNET_LEVEL_TRACE      BIT(1)
#define GNET_LEVEL_DBG        BIT(2)
#define GNET_LEVEL_INFO       BIT(3)
#define GNET_LEVEL_BUG        BIT(4)
#define GNET_LEVEL_SKB        BIT(5)
#define GNET_LEVEL_BYTE       BIT(6)
#define GNET_LEVEL_KERN       BIT(7)

extern USB_UINT32 gnet_msg_level;

/* gnet dbg level */
#define GNET_DBG_DROPUDP    BIT(0)
#define GNET_DBG_DROPALL    BIT(1)
#define GNET_DBG_DROPREQ    BIT(2)
#define GNET_DBG_RX2ADS     BIT(3)
#define GNET_DBG_RXUNWRAP   BIT(4)
#define GNET_DBG_TX2TASK    BIT(5)
#define GNET_DBG_XFERMON    BIT(6)

#define GNET_RX_PROTECT_PKT        120
#define GNET_RX_PROTECT_TIME       1

extern USB_UINT32 gnet_dbg_level;

#define GNET_ERR(args)            do {if (gnet_msg_level & GNET_LEVEL_ERR) printk args;} while (0)
#define GNET_BUG(args,condition)          \
    do {\
        if (condition)\
        {\
            printk args;\
        }\
\
        if (gnet_msg_level & GNET_LEVEL_BUG)\
        {\
            BUG_ON(condition);\
        }\
    } while (0)

#ifdef CONFIG_GNET_DEBUG
#define GNET_TRACE(args)        do {if (gnet_msg_level & GNET_LEVEL_TRACE) printk args;} while (0)
#define GNET_DBG(args)          do {if (gnet_msg_level & GNET_LEVEL_DBG) printk args;} while (0)
#define GNET_INFO(args)         do {if (gnet_msg_level & GNET_LEVEL_INFO) printk args;} while (0)
static inline USB_VOID print_bytes(struct eth_dev *dev, USB_CHAR *data, USB_UINT32 len, const USB_CHAR *tag)
{
    if(gnet_msg_level & GNET_LEVEL_BYTE)
    {
        USB_UINT32 cnt = 0;

        printk("------pkt start %s------\n", tag);
        for(cnt=0; cnt<len; cnt++)
        {
            printk("%02x ",data[cnt]);
        }
        printk("------pkt end  ------\n");
    }
}
static inline USB_VOID print_skb(struct eth_dev *dev, struct sk_buff *skb, const USB_CHAR *tag)
{
    if(gnet_msg_level & GNET_LEVEL_SKB)
    {
        USB_UINT32 cnt = 0;
        USB_UINT32 len = skb->len<=64?skb->len:64;

        printk("%s - skb data(len %d):\n", tag, skb->len);
        for(cnt=0; cnt<len; cnt++)
        {
            printk("%02x ",skb->data[cnt]);
        }
        printk("\n");
    }
}
static inline USB_INT32 gnet_rx_isdrop(struct sk_buff *skb)
{
    if(likely(!(gnet_dbg_level & (GNET_DBG_DROPALL | GNET_DBG_DROPUDP))))
    {
        return 0;
    }
    else if((gnet_dbg_level & GNET_DBG_DROPUDP
            && 0x11 == skb->data[0x17]) 
            || (gnet_dbg_level & GNET_DBG_DROPALL))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#else   /* CONFIG_GNET_DEBUG */
#define GNET_TRACE(args)
#define GNET_DBG(args)
#define GNET_INFO(args)
static inline USB_VOID print_bytes(struct eth_dev *dev, USB_CHAR *data, USB_UINT32 len, const USB_CHAR *tag)
{   return ;    }
static inline USB_VOID print_skb(struct eth_dev *dev, struct sk_buff *skb, const USB_CHAR *tag)
{   return ;    }
static inline USB_INT32 gnet_rx_isdrop(struct sk_buff *skb)
{   return 0;   }
#endif  /* CONFIG_GNET_DEBUG */

/* match with pc usb driver */
//#define GNET_CLASS_TYPE_HUAWEI
//#define GNET_SINGLE_INTERFACE

/* common macro for gnet */
#define GNET_RX_MEM_PREALLOC                (1*824*1024)

/* SKB MAGIC MACRO */
#define GNET_MAGIC_WRAPING                  (0x55AA5AA5)
#define GNET_MAGIC_WRAPED                   (GNET_MAGIC_WRAPING + 1)

#define GNET_MAGIC_SKB_USED                 (0xbeef)

/* it should be very caution to use the 1 jiffy timer */
#define USB_CDC_GNET_TIMEOUT                (2) /* jiffies unit */

#define GNET_CDC_TYPE_ECM                   1
#define GNET_CDC_TYPE_NCM                   2
#define GNET_CDC_TYPE_RNDIS                 3

/***** Parameters for RNDIS  *****/
#define GNET_RNDIS_MAX_PKT_PER_TRANSFER           (10)
#define GNET_RNDIS_MAX_TRANSFER                   (0x4000) /* 16k */
#define GNET_RNDIS_ALIGN_FACTOR                   (2) /* word alignment */
#define GNET_RNDIS_ALIGN_LEN                      (1 << GNET_RNDIS_ALIGN_FACTOR)

/* definitions for rndis optimation */
#define GNET_RNDIS_OUT_MAXSIZE_PER_TRANSFER       (2*1024) /* 2k */
#define GNET_RNDIS_IN_MAXSIZE_PER_TRANSFER        (GNET_RNDIS_MAX_TRANSFER)
#define GNET_RNDIS_PKT_LEN_WATERLINE              (120*1024)/* meaningless actually */
#define GNET_RNDIS_PKT_NUM_WATERLINE              (40)

#define GNET_RNDIS_QUIRK_DWC_TRB_CACHE            BIT(0)
#define GNET_RNDIS_RX_REQ_WATERLINE_DIV           (2)

/***** Parameters for NCM  *****/
#define GNET_NCM_MAX_PKT_PER_TRANSFER           (50)
#define GNET_NCM_OUT_MAXSIZE_PER_TRANSFER       (16*1024)
#define GNET_NCM_IN_MAXSIZE_PER_TRANSFER        (128*1024) /* 128k bytes */
#define GNET_NCM_PKT_LEN_WATERLINE              (60*1024)
#define GNET_NCM_PKT_NUM_WATERLINE              (50)

#define GNET_NCM_QUIRK_DWC_TRB_CACHE            BIT(0)
#define GNET_NCM_RX_REQ_WATERLINE_DIV           (4)

#define USB_CDC_NCM_NTB_IN_MAXSIZE          (128*1024)
#define USB_CDC_NCM_NDP_IN_DIVISOR          (4)
#define USB_CDC_NCM_NDP_IN_REMAINDER        (2)
#define USB_CDC_NCM_NDP_IN_ALIGNMENT        (4)

#define USB_CDC_NCM_NDP_OUT_DIVISOR         (4)
#define USB_CDC_NCM_NDP_OUT_REMAINDER       (2)
#define USB_CDC_NCM_NDP_OUT_ALIGNMENT       (4)

#define USB_CDC_NCM_NDP16_MIN               (0x0010)
#define USB_CDC_NCM_NDP32_MIN               (0x0020)

#define GNET_NCM_ROOM_FOR_NTB_INMAXSIZE     (4096)

#ifndef CONFIG_USB_CDC_NCM_DWC_CACHE_TRB_NUM
#undef USB_CDC_NCM_DWC_CACHE_TRB_NUM
#define USB_CDC_NCM_DWC_CACHE_TRB_NUM       (16)
#endif

/***** Parameters for ECM  *****/
#define GNET_ECM_MAX_PKT_PER_TRANSFER           (1)
#define GNET_ECM_OUT_MAXSIZE_PER_TRANSFER       (2*1024)
#define GNET_ECM_IN_MAXSIZE_PER_TRANSFER        (2*1024)
#define GNET_ECM_PKT_LEN_WATERLINE              (120*1024)/* meaningless actually */
#define GNET_ECM_PKT_NUM_WATERLINE              (40)

#define GNET_ECM_QUIRK_DWC_TRB_CACHE            (0)
#define GNET_ECM_RX_REQ_WATERLINE_DIV           (2)

typedef struct
{
    struct list_head    entry;
    USB_VOID * addr;
    USB_UINT32 size;
    USB_UINT32 actual;
}ntb_node_t;

typedef struct
{
    struct list_head    entry;
    struct sg_table sg_tbl;
    USB_UINT32 actual;
}sg_node_t;

typedef struct
{
    struct eth_dev *dev;
    struct sk_buff_head    pkt_list;
    sg_node_t *sg_node;
    ntb_node_t *ntb;
    struct usb_request *req;

    USB_UINT32 pkt_cnt;
    USB_UINT32 pkt_len;
}tx_ctx_t;

typedef struct
{
    USB_ULONG dev_no_wrap;
    USB_ULONG dev_no_port;
    USB_ULONG dev_wrap_fail;
    USB_ULONG dev_disconnect;
    USB_ULONG dev_connect;
    USB_ULONG dev_open;
    USB_ULONG dev_start;
    USB_ULONG dev_stop;
    USB_ULONG dev_suspend;
    USB_ULONG dev_resume;

    /* tx */
    USB_ULONG tx_total;
    USB_ULONG tx_ok_pkts;
    USB_ULONG tx_ok_bytes;
    USB_ULONG tx_err_pkts;
    USB_ULONG tx_err_bytes;
    USB_ULONG tx_inep_null;
    USB_ULONG tx_dropped;
    USB_ULONG tx_filt_out;
    USB_ULONG tx_drop_suspend;
    USB_ULONG tx_no_ctx;
    USB_ULONG tx_no_sg;
    USB_ULONG tx_no_ntb;
    USB_ULONG tx_ntb_overflow;
    USB_ULONG tx_no_req;
    USB_ULONG tx_noreq_inwrap;
    USB_ULONG tx_bug_noreq;
    USB_ULONG tx_bug_nopkt;
    USB_ULONG tx_no_donereq;
    USB_ULONG tx_stop_queue;
    USB_ULONG tx_prestop_queue;
    USB_ULONG tx_wake_queue;
    USB_ULONG tx_req_dropped;
    USB_ULONG tx_complete_ok;
    USB_ULONG tx_complete_fail;
    USB_ULONG tx_by_timer;
    USB_ULONG tx_timer_activated;
    USB_ULONG tx_skb_realloc;
    USB_ULONG tx_gathered_bytes;
    USB_ULONG tx_psh;
    USB_ULONG tx_skb_check_err;
    USB_ULONG tx_skb_tailrom_lack;
    USB_ULONG tx_skb_tailrom_expand;
    USB_ULONG tx_zlp;
    USB_ULONG tx_expand_zlp;
    USB_ULONG tx_expand_zlp1;
    USB_ULONG tx_quirk;
    
    /* rx */
    USB_ULONG rx_zlp;
    USB_ULONG rx_no_skb;
    USB_ULONG rx_req_submitfail;
    USB_ULONG rx_req_manudrop;
    USB_ULONG rx_skb_protect_drop;
    USB_ULONG rx_skb_recycle_delay;
    USB_ULONG rx_completed;
    USB_ULONG rx_complete_ok;
    USB_ULONG rx_unwrap_fail;
    USB_ULONG rx_packets;
    USB_ULONG rx_bytes;
    USB_ULONG rx_packets_fail;
    USB_ULONG rx_bytes_fail;
    USB_ULONG rx_packets_err;
    USB_ULONG rx_packets_overwline;
    USB_ULONG rx_kevent_busy[WORK_RX_BOTTROM];   
    USB_ULONG rx_kevent_ok[WORK_RX_BOTTROM];
}gnet_stat_t;

struct eth_dev_ctx
{
    USB_UINT eth_idx;
    USB_UINT32 sg_node_nents;
    USB_UINT32 sg_node_reserve;

    USB_UINT32 ntb_node_size;
    USB_UINT8 *dwc_trb_cache_buf;
#if CONFIG_GNET_PREALLOC_RX_MEM
    struct sk_buff_head *rx_skb_free_q;
    struct sk_buff_head *rx_skb_done_q;
    USB_UINT32 rx_skb_num;
    USB_UINT32 rx_skb_waterline;
    USB_UINT32 rx_skb_size;
#endif
    struct timer_list   timer;
    USB_UINT32 timeout;
    USB_UINT32 pkt_cnt_waterline;
    USB_UINT32 pkt_len_waterline;
    USB_UINT32 quirks;
    USB_UINT32 dbg_flags;
    USB_UINT32 protect_num;
    USB_ULONG  protect_jiff;
    USB_UINT32 cdc_type;
    USB_UINT32 cdc_bypass;
    gnet_stat_t stats;
};

struct eth_dev {
    /* lock is held while accessing port_usb
     * or updating its backlink port_usb->ioport
     */
    spinlock_t        lock;
    struct gether        *port_usb;

    struct net_device    *net;
    struct usb_gadget    *gadget;

    spinlock_t        req_lock;    /* guard {rx,tx}_reqs */
    spinlock_t        tx_req_lock;    /* guard tx_reqs */
    struct list_head    tx_reqs, rx_reqs;
    struct list_head    tx_done_reqs;

    atomic_t        tx_qlen;
    atomic_t        rx_qlen;
    USB_UINT32 tx_req_num;
    USB_UINT32 rx_req_num;
    USB_UINT32 rx_req_waterline;

    struct sk_buff_head    rx_frames;

    USB_UINT32        header_len;
    struct sk_buff        *(*wrap)(struct gether *, struct sk_buff *skb);
    USB_INT32           (*unwrap)(struct gether *,
                        struct sk_buff *skb,
                        struct sk_buff_head *list);

    USB_INT32(*handle_rx)(struct gether *port,
                 struct sk_buff *skb);
    struct sk_buff_head    rx_frames_to_ads;
    USB_UINT32 rx_frames_waterline;
    struct work_struct rx_work;

    struct delayed_work    work;

    USB_ULONG        todo;
    USB_UINT32            zlp:1;
    USB_UINT32         doclone:1;
    USB_UINT32         is_suspend:1;
    USB_UINT32         remote_wakeup_en:1;
    USB_UINT8            host_mac[ETH_ALEN];

    struct eth_dev_ctx  ctx;
    struct device_attribute dev_attr;
    USB_CHAR dev_sysfs_name[64];
};

/*
 * This represents the USB side of an "ethernet" link, managed by a USB
 * function which provides control and (maybe) framing.  Two functions
 * in different configurations could share the same ethernet link/netdev,
 * using different host interaction models.
 *
 * There is a current limitation that only one instance of this link may
 * be present in any given configuration.  When that's a problem, network
 * layer facilities can be used to package multiple logical links on this
 * single "physical" one.
 */
struct gether {
    struct usb_function        func;

    /* updated by gether_{connect,disconnect} */
    struct eth_dev            *ioport;

    /* endpoints handle full and/or high speeds */
    struct usb_ep            *in_ep;
    struct usb_ep            *out_ep;
    struct usb_ep            *notify;
    
    USB_UINT32 *header_room_resved_ptr;

    USB_BOOL                is_zlp_ok;

    USB_UINT16                cdc_filter;

    /* hooks for added framing, as needed for RNDIS and EEM. */
    USB_UINT32                header_len;
    /* NCM requires fixed size bundles */
    USB_BOOL                is_fixed;
    USB_UINT32                fixed_out_len;
    USB_UINT32                fixed_in_len;
    struct sk_buff            *(*wrap)(struct gether *port,
                        struct sk_buff *skb);
    USB_INT32               (*unwrap)(struct gether *port,
                        struct sk_buff *skb,
                        struct sk_buff_head *list);

    USB_INT32 (*handle_rx)(struct gether *port, struct sk_buff *skb);

    /* called on network open/close */
    USB_VOID                (*open)(struct gether *);
    USB_VOID                (*close)(struct gether *);
    
    USB_UINT32 port_no;    /* indicates which function port */
};

#define    DEFAULT_FILTER    (USB_CDC_PACKET_TYPE_BROADCAST \
            |USB_CDC_PACKET_TYPE_ALL_MULTICAST \
            |USB_CDC_PACKET_TYPE_PROMISCUOUS \
            |USB_CDC_PACKET_TYPE_DIRECTED)

/************** 外部函数声明 *****************/
USB_VOID mbim_flag_proc_init(USB_VOID);

/************** 内部函数声明 *****************/

/* netdev setup/teardown as directed by the gadget driver */
USB_INT gether_setup(struct usb_gadget *g, USB_UINT8 ethaddr[ETH_ALEN]);
USB_VOID gether_cleanup(USB_VOID);
/* variant of gether_setup that allows customizing network device name */
USB_INT gether_setup_name(struct usb_gadget *g, USB_UINT8 ethaddr[ETH_ALEN],
        const USB_CHAR *netname);

/* connect/disconnect is handled by individual functions */
struct net_device *gether_connect(struct gether *);
USB_VOID gether_disconnect(struct gether *);

/* Some controllers can't support CDC Ethernet (ECM) ... */
static inline USB_BOOL can_support_ecm(struct usb_gadget *gadget)
{
    if (!gadget_supports_altsettings(gadget))
        return false;

    /* Everything else is *presumably* fine ... but this is a bit
     * chancy, so be **CERTAIN** there are no hardware issues with
     * your controller.  Add it above if it can't handle CDC.
     */
    return true;
}

USB_INT rndis_bind_config_vendor(struct usb_configuration *c, USB_UINT8 ethaddr[ETH_ALEN],
                USB_UINT32 vendorID, const USB_CHAR *manufacturer);

USB_VOID free_sg_node(sg_node_t *sg_node);
sg_node_t *alloc_sg_node(USB_UINT32 nents, gfp_t gfp_msk);
USB_INT gnet_get_sgnode(tx_ctx_t *ctx, USB_UINT32 nents, gfp_t gfp_msk);
USB_VOID gnet_put_sgnode(tx_ctx_t *ctx);
struct sk_buff *gnet_get_rx_skb(struct eth_dev  *dev, USB_UINT32 size, gfp_t gfp_msk);
USB_VOID gnet_recycle_rx_skb(struct eth_dev  *dev, struct sk_buff *skb);
USB_VOID gnet_put_rx_skb_to_done(struct eth_dev  *dev, struct sk_buff *skb);
USB_VOID free_tx_ctx(tx_ctx_t *tx_ctx);
USB_INT alloc_tx_ctx(struct eth_dev *dev, USB_UINT32 n);
USB_VOID free_rx_mem(struct eth_dev *dev);
USB_INT alloc_rx_mem(struct eth_dev *dev);
USB_VOID gnet_recycle_ctx_info(tx_ctx_t *ctx, USB_UINT32 success);
USB_VOID gnet_timeout_handle(USB_ULONG data);
USB_VOID make_clone_skb_writable(struct sk_buff *skb);
struct sk_buff * gnet_copy_skb(struct sk_buff *skb, USB_INT32 offset, USB_INT32 len);
USB_INT gnet_rx_need_protect(struct eth_dev *dev, USB_UCHAR* data);
USB_VOID gnet_set_rx_clone(struct eth_dev *dev, USB_UINT32 doclone);
USB_VOID eth_suspend(struct eth_dev *dev);
USB_VOID eth_resume(struct eth_dev *dev);
USB_INT32 eth_add_tx_req(struct eth_dev *dev);
USB_UINT32 eth_get_net_speed(USB_VOID);
USB_INT eth_prealloc_rx_mem(USB_UINT32 cdc_type);

static inline struct eth_dev *func_to_ethdev(struct usb_function *f)
{
    struct gether *port = container_of(f, struct gether, func);
    return port->ioport;
}

/**
 * gnet_wakeup_gadget - wakeup the gadget to normal mode
 * @dev: the gent dev
 * Context: may soft irq
 *
 * This function invokes the gadget-ops->wakeup to resume the gadget.
 *
 * Returns:
 *  0 : success;
 *  Other : failed to wakeup the host.
 */
static inline USB_INT32 gnet_wakeup_gadget(struct eth_dev *dev)
{
    if (dev->gadget->ops && dev->gadget->ops->wakeup) {
        return dev->gadget->ops->wakeup(dev->gadget);
    }

    return 1;
}

#endif /* __U_ETHER_H */

