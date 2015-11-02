/*
 * u_ether.c -- Ethernet-over-USB link layer utilities for Gadget stack
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

/* #define VERBOSE_DEBUG */
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/device.h>
#include <linux/ctype.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/ip.h>
#include "u_ether.h"
#include "rndis.h"
#include "f_ncm_mbb.h"

#if (FEATURE_ON == MBB_CTF_COMMON)
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <linux/fake/typedefs.h>
#include <linux/fake/osl.h>
#include <linux/fake/linux_osl.h>
#include <linux/fake/ctf/hndctf.h>
#endif

#if (FEATURE_ON == MBB_FEATURE_FASTIP)
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <linux/fastip/fastip.h>
#include <linux/ip.h>
#endif
/*
 * This component encapsulates the Ethernet link glue needed to provide
 * one (!) network link through the USB gadget stack, normally "usb0".
 *
 * The control and data models are handled by the function driver which
 * connects to this code; such as CDC Ethernet (ECM or EEM),
 * "CDC Subset", or RNDIS.  That includes all descriptor and endpoint
 * management.
 *
 * Link level addressing is handled by this component using module
 * parameters; if no such parameters are provided, random link level
 * addresses are used.  Each end of the link uses one address.  The
 * host end address is exported in various ways, and is often recorded
 * in configuration databases.
 *
 * The driver which assembles each configuration using such a link is
 * responsible for ensuring that each configuration includes at most one
 * instance of is network link.  (The network layer provides ways for
 * this single "physical" link to be used by multiple virtual links.)
 */

#define UETH__VERSION    "29-May-2008"

USB_UINT32 gnet_msg_level = GNET_LEVEL_ERR;
USB_UINT32 gnet_dbg_level = GNET_DBG_RX2ADS;

#ifdef USB_DUAL_CORE_CPE
static struct eth_dev *the_dev[GNET_MAX_NUM] = {0};
#else
static struct eth_dev *the_dev[GNET_USED_NUM] = {0};
#endif
static USB_UINT geth_idx = 0;

struct sk_buff_head *g_rx_skb_free_q = NULL;
struct sk_buff_head *g_rx_skb_done_q = NULL;

/* IP align for ecm is ECM_IP_ALIGN_LENGTH,
  for other cdc, this value should be 0 */
USB_UINT g_usb_net_ip_align = 0;  

#define USB_NET_IP_ALIGN          g_usb_net_ip_align
#define ECM_IP_ALIGN_LENGTH       (2 + max(32, L1_CACHE_BYTES))

/*-------------------------------------------------------------------------*/

#define RX_EXTRA    20    /* bytes guarding against rx overflows */

#define DEFAULT_QLEN    2    /* double buffering by default */

#ifdef CONFIG_USB_GADGET_DUALSPEED
static USB_UINT qmult = 32;
module_param(qmult, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(qmult, "queue length multiplier at high/super speed");
#else    /* full speed (low speed doesn't do bulk) */
#define qmult        1
#endif

#ifdef CONFIG_USB_GADGET_DUALSPEED
static USB_UINT qmult_rx_ecm = 0;
static USB_UINT qmult_rx_rndis = 0;
static USB_UINT qmult_rx_ncm = 0;
module_param(qmult_rx_ecm, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(qmult_rx_ecm, "ecm rx queue length multiplier at high/super speed");

module_param(qmult_rx_rndis, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(qmult_rx_rndis, "rndis rx queue length multiplier at high/super speed");

module_param(qmult_rx_ncm, uint, S_IRUGO|S_IWUSR);
MODULE_PARM_DESC(qmult_rx_ncm, "ncm rx queue length multiplier at high/super speed");
#else    /* full speed (low speed doesn't do bulk) */
#define qmult_rx        1
#endif

#if (FEATURE_ON == MBB_CTF_COMMON)
static ctf_t *usb_cih __attribute__ ((aligned(32))) = NULL;/* ctf instance handle */
static osl_t *usb_fake_osh = NULL;
USB_UINT g_ifctf = 0;
#endif

#if(FEATURE_ON == MBB_FEATURE_FASTIP)
fastip_t  __attribute__ ((aligned(32))) *usb_fastip_handle  = NULL;
#endif /* MBB_FEATURE_FASTIP */

#if (FEATURE_ON == MBB_CTF_COMMON)
USB_VOID usb_ctf_detach(ctf_t *ci, USB_VOID *arg)
{
    usb_cih = NULL;
    return;
}

USB_INT usb_ctf_forward(struct sk_buff *skb)
{
    /* use slow path if ctf is disabled */
    if (!CTF_ENAB(usb_cih))
    {
        return (BCME_ERROR);
    }
    /* try cut thru first */
    if (BCME_ERROR != ctf_forward(usb_cih, skb, skb->dev))
    {
        return (BCME_OK);
    }

    /* clear skipct flag before sending up */
    PKTCLRSKIPCT(NULL /* et->osh*/, skb);

    return (BCME_ERROR);
}

USB_VOID eth_open_test()
{
    if(!usb_cih)
    {
        DBG_E(MBB_NET, "ctf of usb isn't attached! \n");
    }
    else
    {
        DBG_E(MBB_NET, "ctf of usb is attached! \n");
    }
}
EXPORT_SYMBOL(eth_open_test);

#endif /* CTF */


#if (FEATURE_ON == MBB_FEATURE_FASTIP)
static USB_VOID fastip_detach_usb(fastip_t *ci, USB_VOID *arg)
{
    usb_fastip_handle = NULL;
    return;
}

USB_INT fastip_forward_usb(struct sk_buff *skb)
{

    /* use slow path if fastip is disabled */
    if (!FASTIP_ENAB(usb_fastip_handle))
    {
        return (FASTIP_ERROR);
    }

    /* try fastip first */
    if (fastip_forward(usb_fastip_handle, skb, skb->dev) != FASTIP_ERROR)
    {
        return (FASTIP_OK);
    }
    /* clear skipct flag before sending up */
    FASTIPCLRSKIPCT(skb);

    return (FASTIP_ERROR);
}

#endif /* MBB_FEATURE_FASTIP */

/* for dual-speed hardware, use deeper queues at high/super speed */
static inline USB_INT32 qlen(struct usb_gadget *gadget)
{
    if (gadget_is_dualspeed(gadget) && (gadget->speed == USB_SPEED_HIGH ||
                        gadget->speed == USB_SPEED_SUPER))
        return qmult * DEFAULT_QLEN;
    else
        return DEFAULT_QLEN;
}

#ifndef DWC3_TRB_NUM
#define DWC3_TRB_NUM 128 /* should be consistent with the definition at core.h of dwc3 */
#endif
static inline USB_INT32 qlen_rx(struct usb_gadget *gadget, USB_UINT32 cdc_type)
{
    USB_UINT32 qmult_rx = qmult_rx_ecm; /* Use Ecm as default */
    
    if (GNET_CDC_TYPE_ECM == cdc_type)
    {
        qmult_rx_ecm = (GNET_RX_MEM_PREALLOC) / (GNET_ECM_OUT_MAXSIZE_PER_TRANSFER * DEFAULT_QLEN);
        qmult_rx = qmult_rx_ecm;
    }
    else if (GNET_CDC_TYPE_RNDIS == cdc_type)
    {
        qmult_rx_rndis = (GNET_RX_MEM_PREALLOC) / (GNET_RNDIS_OUT_MAXSIZE_PER_TRANSFER * DEFAULT_QLEN);
        qmult_rx = qmult_rx_rndis; 
    }
    else if (GNET_CDC_TYPE_NCM == cdc_type)
    {
        qmult_rx_ncm = (GNET_RX_MEM_PREALLOC) / (GNET_NCM_OUT_MAXSIZE_PER_TRANSFER * DEFAULT_QLEN);
        qmult_rx = qmult_rx_ncm;
    }

    GNET_INFO(("unify gnet_qlen_rx : type = %lu, qmult_rx = %lu\n", cdc_type, qmult_rx));  
    
    /* it's useless if the num of rx request is greater than trb num */
    if (gadget_is_dualspeed(gadget)
      && (gadget->speed == USB_SPEED_HIGH || gadget->speed == USB_SPEED_SUPER))
    {
        return min_t(size_t, qmult_rx * DEFAULT_QLEN, DWC3_TRB_NUM);
    }
    else
    {
        return min_t(size_t, USB_NUM_4 * DEFAULT_QLEN, DWC3_TRB_NUM);
    }
}

/*-------------------------------------------------------------------------*/

/* REVISIT there must be a better way than having two sets
 * of debug calls ...
 */

#undef DBG
#undef VDBG
#undef ERROR
#undef INFO

#define xprintk(d, level, fmt, args...) \
    printk(level "%s: " fmt , (d)->net->name , ## args)

#ifdef DEBUG
#undef DEBUG
#define DBG(dev, fmt, args...) \
    xprintk(dev , KERN_DEBUG , fmt , ## args)
#else
#define DBG(dev, fmt, args...) \
    do { } while (0)
#endif /* DEBUG */

#ifdef VERBOSE_DEBUG
#define VDBG    DBG
#else
#define VDBG(dev, fmt, args...) \
    do { } while (0)
#endif /* DEBUG */

#define ERROR(dev, fmt, args...) \
    xprintk(dev , KERN_ERR , fmt , ## args)
#define INFO(dev, fmt, args...) \
    xprintk(dev , KERN_INFO , fmt , ## args)

USB_VOID set_gnet_msg(USB_UINT32 level)
{
    gnet_msg_level = level;
}
EXPORT_SYMBOL(set_gnet_msg);

USB_UINT32 get_gnet_msg(USB_VOID)
{
    printk("level : %lx\n",gnet_msg_level);
    return gnet_msg_level;
}
EXPORT_SYMBOL(get_gnet_msg);

USB_INT32 gnet_get_netmode(struct eth_dev *dev)
{
    if (dev->ctx.cdc_bypass)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
EXPORT_SYMBOL(gnet_get_netmode);

/*-------------------------------------------------------------------------*/

/* NETWORK DRIVER HOOKUP (to the layer above this driver) */

static USB_INT ueth_change_mtu(struct net_device *net, USB_INT new_mtu)
{
    struct eth_dev    *dev = netdev_priv(net);
    USB_ULONG          flags;
    USB_INT          status = 0;

    /* don't change MTU on "live" link (peer won't know) */
    spin_lock_irqsave(&dev->lock, flags);
    if (dev->port_usb)
        status = -EBUSY;
    else if (new_mtu <= ETH_HLEN || new_mtu > ETH_FRAME_LEN)
        status = -ERANGE;
    else
        net->mtu = new_mtu;
    spin_unlock_irqrestore(&dev->lock, flags);

    return status;
}

static USB_VOID eth_get_drvinfo(struct net_device *net, struct ethtool_drvinfo *p)
{
    struct eth_dev    *dev = netdev_priv(net);

    strlcpy(p->driver, "g_ether", sizeof p->driver);
    strlcpy(p->version, UETH__VERSION, sizeof p->version);
    strlcpy(p->fw_version, dev->gadget->name, sizeof p->fw_version);
    strlcpy(p->bus_info, dev_name(&dev->gadget->dev), sizeof p->bus_info);
}

/* REVISIT can also support:
 *   - WOL (by tracking suspends and issuing remote wakeup)
 *   - msglevel (implies updated messaging)
 *   - ... probably more ethtool ops
 */

static const struct ethtool_ops ops = {
    .get_drvinfo = eth_get_drvinfo,
    .get_link = ethtool_op_get_link,
};

static USB_VOID defer_kevent(struct eth_dev *dev, USB_INT32 flag)
{
    if (test_and_set_bit(flag, &dev->todo))
        return;
    if (!schedule_delayed_work(&dev->work, 2))
    {
        dev->ctx.stats.rx_kevent_busy[flag]++;
    }
    else
    {
        dev->ctx.stats.rx_kevent_ok[flag]++;
    }
}

static USB_VOID defer_rx(struct eth_dev *dev)
{
    if (!schedule_work(&dev->rx_work)) {
        dev->ctx.stats.rx_kevent_busy[WORK_RX_UPTASK]++;
    }
    else {
        dev->ctx.stats.rx_kevent_ok[WORK_RX_UPTASK]++;
    }
}

static USB_VOID rx_complete(struct usb_ep *ep, struct usb_request *req);

static USB_INT32
rx_submit(struct eth_dev *dev, struct usb_request *req, gfp_t gfp_flags)
{
    struct net_device *net = dev->net;
    struct sk_buff    *skb;
    USB_INT32       retval = -ENOMEM;
    size_t        size = 0;
    struct usb_ep    *out;
    USB_ULONG         flags;

    spin_lock_irqsave(&dev->lock, flags);
    if (dev->port_usb)
        out = dev->port_usb->out_ep;
    else
        out = NULL;
    spin_unlock_irqrestore(&dev->lock, flags);

    if (!out)
        return -ENOTCONN;


    /* Padding up to RX_EXTRA handles minor disagreements with host.
     * Normally we use the USB "terminate on short read" convention;
     * so allow up to (N*maxpacket), since that memory is normally
     * already allocated.  Some hardware doesn't deal well with short
     * reads (e.g. DMA must be N*maxpacket), so for now don't trim a
     * byte off the end (to force hardware errors on overflow).
     *
     * RNDIS uses internal framing, and explicitly allows senders to
     * pad to end-of-packet.  That's potentially nice for speed, but
     * means receivers can't recover lost synch on their own (because
     * new packets don't only start after a short RX).
     */
    size += sizeof(struct ethhdr) + net->mtu + RX_EXTRA;
    size += dev->port_usb->header_len;
    size += out->maxpacket - 1;
    size -= size % out->maxpacket;

    if (dev->port_usb->is_fixed)
        size = max_t(size_t, size, dev->port_usb->fixed_out_len);

    skb = gnet_get_rx_skb(dev, size, gfp_flags);

    if (skb == NULL) {
        dev->ctx.stats.rx_no_skb++;
        goto enomem;
    }

    /* Some platforms perform better when IP packets are aligned,
     * but on at least one, checksumming fails otherwise.  Note:
     * RNDIS headers involve variable numbers of LE32 values.
     */
#if !defined(CONFIG_GNET_PREALLOC_RX_MEM)
    skb_reserve(skb, USB_NET_IP_ALIGN);
#endif
    req->buf = skb->data;
    req->length = size;
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
    if ((GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type) || (GNET_CDC_TYPE_ECM == dev->ctx.cdc_type))
    {
        GNET_TRACE(("unify-rndis-ecm %s : set req length as skb size\n", __FUNCTION__));
        req->length = dev->ctx.rx_skb_size;
    }
#endif
    req->complete = rx_complete;
    req->context = skb;
    
    retval = usb_ep_queue(out, req, gfp_flags);

    GNET_TRACE(("rx_submit : ep queue req=%p, retval=%ld\n", req, retval));
    
    if (retval == -ENOMEM)
enomem:
        defer_kevent(dev, WORK_RX_MEMORY);
    if (retval) {
        if (skb)
        {
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
            gnet_recycle_rx_skb(dev, skb);
#else
            dev_kfree_skb_any(skb);
#endif
        }

        dev->ctx.stats.rx_req_submitfail++;

        spin_lock_irqsave(&dev->req_lock, flags);
        list_add(&req->list, &dev->rx_reqs);
        spin_unlock_irqrestore(&dev->req_lock, flags);
    }
    else
    {
        atomic_inc(&dev->rx_qlen);
    }

    return retval;
}

USB_VOID gnet_set_rx_protect_num(USB_UINT num, USB_INT idx)
{
    struct eth_dev *dev;

    if (idx < 0 || idx >= GNET_USED_NUM) {
        return;
    }

    dev = the_dev[idx];
    dev->ctx.protect_num = num;
}

USB_VOID gnet_rx_protect_init(struct eth_dev *dev)
{
    dev->ctx.protect_jiff = jiffies;
    dev->ctx.protect_num = GNET_RX_PROTECT_PKT;
}

USB_INT gnet_rx_need_protect(struct eth_dev *dev, USB_UCHAR* data)
{
    static USB_UINT s_rx_pkt = 0;

    s_rx_pkt++;

    /* calc time is over */
    if (time_after_eq(jiffies, dev->ctx.protect_jiff + GNET_RX_PROTECT_TIME)) {
        dev->ctx.protect_jiff = jiffies;
        s_rx_pkt = 0;
    }
    /* if the rx pkt reach the max pos, start protect */
    else if (s_rx_pkt >= dev->ctx.protect_num) {
        struct iphdr *ip = (struct iphdr *)(data + ETH_HLEN);

        /* we just protect for udp prot */
        if (ip->protocol == IPPROTO_UDP) {
            dev->ctx.stats.rx_skb_protect_drop++;
            return 1;
        }
    }

    return 0;
}

static USB_INT32 gnet_rx_transmit_packet(struct eth_dev *dev, struct sk_buff *skb)
{
    struct net_device *net = NULL;
    USB_INT32 status = 0;
    
    if (!dev || !skb)
    {
        GNET_ERR(("gnet_rx_transmit_packet invalid param\n"));
        return -1;
    }

    net = dev->net;
    net->stats.rx_packets++;
    net->stats.rx_bytes += skb->len;

#if (FEATURE_ON == MBB_CTF_COMMON)
    skb->dev = net;
    if (0 == g_ifctf)
    {
        if (usb_cih && (BCME_ERROR != usb_ctf_forward(skb)))
        {
            return NET_RX_SUCCESS;
        }
    }
#endif

#if (FEATURE_ON == MBB_FEATURE_FASTIP)
    skb->dev = net;
    /* try fastip before netif_rx */
    if (usb_fastip_handle && fastip_forward_usb(skb) != FASTIP_ERROR)
    {
        /* 统计网卡接收数据信息 */
        return NET_RX_SUCCESS;
    }
#endif

    skb->protocol = eth_type_trans(skb, net);

    /* no buffer copies needed, unless hardware can't use skb buffers.*/
    status = netif_rx(skb);
            
    return status;
}

static USB_VOID rx_complete(struct usb_ep *ep, struct usb_request *req)
{
    struct sk_buff    *skb = req->context, *skb2;
    struct eth_dev    *dev = ep->driver_data;
    struct net_device *net = dev->net;
    USB_INT32       status = req->status;

    GNET_TRACE(("rx_complete <- req=%p, status=%ld\n", req, status));

    dev->ctx.stats.rx_completed++;

    switch (status) {
    /* normal completion */
    case 0:
        if(gnet_dbg_level & GNET_DBG_DROPREQ)
        {
            dev->ctx.stats.rx_req_manudrop++;
            break;
        }

        dev->ctx.stats.rx_complete_ok++;
        if(!req->actual)
        {
            dev->ctx.stats.rx_zlp++;
            GNET_DBG(("rx received zlp\n"));
            break;
        }
        print_bytes(dev, req->buf, 64, __FUNCTION__);

        skb_put(skb, req->actual);

        if (dev->unwrap) {
            USB_ULONG    flags;

            spin_lock_irqsave(&dev->lock, flags);
            if (dev->port_usb) {
                status = dev->unwrap(dev->port_usb,
                            skb,
                            &dev->rx_frames);
            } else {
                dev_kfree_skb_any(skb);
                status = -ENOTCONN;
            }
            spin_unlock_irqrestore(&dev->lock, flags);
            if(status)
            {
                dev->ctx.stats.rx_unwrap_fail++;
            }
        } else {
            skb_queue_tail(&dev->rx_frames, skb);
        }
        skb = NULL;

        skb2 = skb_dequeue(&dev->rx_frames);
        while (skb2) {
            if (status < 0
                    || ETH_HLEN > skb2->len
                    || skb2->len > ETH_FRAME_LEN) {
                net->stats.rx_errors++;
                net->stats.rx_length_errors++;
                dev->ctx.stats.rx_packets_err++;
                GNET_TRACE(("rx length %d\n", skb2->len));
                dev_kfree_skb_any(skb2);
                goto next_frame;
            }

            print_skb(dev, skb2, __FUNCTION__);

            /* check if user-defined drop */
            if(unlikely(gnet_rx_isdrop(skb2)))
            {
                dev_kfree_skb_any(skb2);
                status = 0;
                goto next_frame;
            }

#if 0
            if (dev->ctx.cdc_bypass)
            {
                /* dfs control for stick bypass mode */
                netif_rx_dfs_control();
            
                if (!dev->handle_rx)
                {
                    GNET_ERR(("dev->handle_rx not registered!\n"));
                    dev_kfree_skb_any(skb2);
                    status = 0;    
                    goto next_frame;
                }
                
                dev->ctx.stats.rx_bytes += skb2->len;
                dev->ctx.stats.rx_packets++;

                if(!(gnet_dbg_level & GNET_DBG_RX2ADS))   /* don't call cb directly */
                {
                    if(skb_queue_len(&dev->rx_frames_to_ads) <= dev->rx_frames_waterline)
                    {
                        skb_queue_tail(&dev->rx_frames_to_ads, skb2);
                    }
                    else
                    {
                        dev->ctx.stats.rx_packets_overwline++;
                        dev_kfree_skb_any(skb2);
                    }

                    status = 0;
                }
                else
                {
                    status = dev->handle_rx(dev->port_usb, skb2);
                    if(status)
                    {
                        dev->ctx.stats.rx_packets_fail++;
                        dev->ctx.stats.rx_bytes_fail += skb2->len;
                        dev_kfree_skb_any(skb2);
                        status = 0;
                    }
                }

                goto next_frame;
            }

#if (FEATURE_ON == MBB_CTF_COMMON)
            skb2->dev = net;
            if (0 == g_ifctf)
            {
                if (usb_cih && (BCME_ERROR != usb_ctf_forward(skb2)))
                {
                    net->stats.rx_packets++;
                    net->stats.rx_bytes += skb2->len;
                    dev->ctx.stats.rx_bytes += skb2->len;
                    dev->ctx.stats.rx_packets++;
                    status = NET_RX_SUCCESS;
                    goto next_frame;
                }
            }
#endif

#if (FEATURE_ON == MBB_FEATURE_FASTIP)
            skb2->dev = net;
            /* try fastip before netif_rx */
            if (usb_fastip_handle && fastip_forward_usb(skb2) != FASTIP_ERROR)
            {
                /* 统计网卡接收数据信息 */
                dev->ctx.stats.rx_bytes += skb2->len;
                dev->ctx.stats.rx_packets++;
                net->stats.rx_packets++;
                net->stats.rx_bytes += skb2->len;
                status = NET_RX_SUCCESS;
                goto next_frame;
            }
#endif

            skb2->protocol = eth_type_trans(skb2, net);
            net->stats.rx_packets++;
            net->stats.rx_bytes += skb2->len;
            dev->ctx.stats.rx_bytes += skb2->len;
            dev->ctx.stats.rx_packets++;

            /* no buffer copies needed, unless hardware can't
             * use skb buffers.
             */
            status = netif_rx(skb2);
#endif

            dev->ctx.stats.rx_bytes += skb2->len;
            dev->ctx.stats.rx_packets++;

            if (dev->ctx.cdc_bypass)
            {
                /* dfs control for stick bypass mode */
                netif_rx_dfs_control();             
            }
            
            if (!(gnet_dbg_level & GNET_DBG_RX2ADS))
            {
                if(skb_queue_len(&dev->rx_frames_to_ads) <= dev->rx_frames_waterline)
                {
                    skb_queue_tail(&dev->rx_frames_to_ads, skb2);
                }
                else
                {
                    dev->ctx.stats.rx_packets_overwline++;
                    dev_kfree_skb_any(skb2);
                }

                status = 0; 
                goto next_frame;
            }
            else if (dev->ctx.cdc_bypass)
            {                    
                if (!dev->handle_rx)
                {
                    GNET_ERR(("dev->handle_rx not registered!\n"));
                    dev_kfree_skb_any(skb2);
                }
                else 
                {      
                    status = dev->handle_rx(dev->port_usb, skb2);
                    if(status)
                    {
                        dev->ctx.stats.rx_packets_fail++;
                        dev->ctx.stats.rx_bytes_fail += skb2->len;
                        dev_kfree_skb_any(skb2);
                    }
                }

                status = 0;    
                goto next_frame;
            }
                
            status = gnet_rx_transmit_packet(dev, skb2);

next_frame:
            skb2 = skb_dequeue(&dev->rx_frames);
        }

        if (!(gnet_dbg_level & GNET_DBG_RX2ADS))
        {
            defer_rx(dev);
        }

        break;

    /* software-driven interface shutdown */
    case -ECONNRESET:        /* unlink */
    case -ESHUTDOWN:        /* disconnect etc */
        DBG_I(MBB_NET, "rx shutdown, code %ld\n",status);
        goto quiesce;

    /* for hardware automagic (such as pxa) */
    case -ECONNABORTED:        /* endpoint reset */
        DBG_I(MBB_NET, "rx %s reset\n",ep->name);
        defer_kevent(dev, WORK_RX_MEMORY);
        
quiesce:

        gnet_recycle_rx_skb(dev, skb);

        goto clean;

    /* data overrun */
    case -EOVERFLOW:
        net->stats.rx_over_errors++;
        /* FALLTHROUGH */

    default:
        net->stats.rx_errors++;
        DBG_I(MBB_NET, "rx status %ld\n", status);
        break;
    }

    if (skb)
    {
        gnet_recycle_rx_skb(dev, skb);
    }

    if ((dev->ctx.cdc_bypass && (!dev->handle_rx)) || (!dev->ctx.cdc_bypass && !netif_running(net)))
    {
        DBG_I(MBB_NET, "rx_complete : rx process stopped\n");
clean:
        spin_lock(&dev->req_lock);
        list_add(&req->list, &dev->rx_reqs);
        spin_unlock(&dev->req_lock);
        req = NULL;
    }

    atomic_dec(&dev->rx_qlen);

    if (req)
        rx_submit(dev, req, GFP_ATOMIC);
}

static USB_INT32 prealloc(struct list_head *list, struct usb_ep *ep, USB_UINT32 n)
{
    USB_UINT32        i;
    struct usb_request    *req;

    if (!n)
        return -ENOMEM;

    /* queue/recycle up to N requests */
    i = n;
    list_for_each_entry(req, list, list) {
        if (i-- == 0)
            goto extra;
    }
    while (i--) {
        req = usb_ep_alloc_request(ep, GFP_ATOMIC);
        if (!req)
            return list_empty(list) ? -ENOMEM : 0;
        list_add(&req->list, list);
    }
    return 0;

extra:
    /* free extras */
    for (;;) {
        struct list_head    *next;

        next = req->list.next;
        list_del(&req->list);
        usb_ep_free_request(ep, req);

        if (next == list)
            break;

        req = container_of(next, struct usb_request, list);
    }
    return 0;
}

static USB_INT32 alloc_requests(struct eth_dev *dev, struct gether *link, USB_UINT32 n_rx, USB_UINT32 n_tx)
{
    USB_INT32   status;

    GNET_TRACE(("%s line %d:alloc %ld rx request, %ld tx request\n", __FUNCTION__, __LINE__, n_rx, n_tx));

    /* alloc tx requests */
    spin_lock(&dev->tx_req_lock);
    status = prealloc(&dev->tx_reqs, link->in_ep, n_tx);
    if (status < 0)
        goto fail_tx;
    status = alloc_tx_ctx(dev, n_tx);
    if (status < 0)
        goto fail_tx;
    spin_unlock(&dev->tx_req_lock);
    dev->tx_req_num = n_tx;

    /* alloc rx requests */
    spin_lock(&dev->req_lock);
    status = prealloc(&dev->rx_reqs, link->out_ep, n_rx);
    if (status < 0)
        goto fail_rx;
    spin_unlock(&dev->req_lock);
    dev->rx_req_num = n_rx;

    GNET_INFO(("unify gnet_alloc_requests : type = %lu\n", dev->ctx.cdc_type));
    if (GNET_CDC_TYPE_NCM == dev->ctx.cdc_type)
    {
        dev->rx_req_waterline = n_rx / GNET_NCM_RX_REQ_WATERLINE_DIV;
    }
    else if (GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type)
    {
        dev->rx_req_waterline = n_rx / GNET_RNDIS_RX_REQ_WATERLINE_DIV;
    }
    else
    {
        /* Use ecm as default */
        dev->rx_req_waterline = n_rx / GNET_ECM_RX_REQ_WATERLINE_DIV;
    }
    
#ifdef CONFIG_GNET_PREALLOC_RX_MEM
    dev->ctx.rx_skb_waterline = min_t(size_t,dev->ctx.rx_skb_waterline,dev->rx_req_waterline);
#endif

    return status;
fail_tx:
    DBG_I(MBB_NET, "can't alloc tx requests\n");
    spin_unlock(&dev->tx_req_lock);
    return status;
fail_rx:
    DBG_I(MBB_NET, "can't alloc rx requests\n");
    spin_unlock(&dev->req_lock);
    return status;
}

static USB_VOID rx_ads_process(struct eth_dev *dev, gfp_t gfp_flags)
{
    struct sk_buff    *skb;
    USB_INT32 ret;
    USB_INT32 status = 0;
    struct net_device *net = dev->net;
    
    while(NULL != (skb = skb_dequeue(&dev->rx_frames_to_ads)))
    {
        if (!dev->ctx.cdc_bypass)
        {
            if (status < 0)
            {
                net->stats.rx_errors++;
                net->stats.rx_length_errors++;
                dev->ctx.stats.rx_packets_err++;
                GNET_TRACE(("rx length %d\n", skb->len));
                dev_kfree_skb_any(skb);           
            }
            else
            {
                status = gnet_rx_transmit_packet(dev, skb);
            }
            continue;
        }
        
        if(dev->handle_rx)
        {
            ret = dev->handle_rx(dev->port_usb,skb);
            if(ret)
            {
                dev->ctx.stats.rx_packets_fail++;
                dev->ctx.stats.rx_bytes_fail += skb->len;
                dev_kfree_skb_any(skb);
            }
        }
        else
        {
            dev_kfree_skb_any(skb);
        }
    }
}

static USB_VOID rx_fill(struct eth_dev *dev, gfp_t gfp_flags)
{
    struct usb_request    *req;
    USB_ULONG              flags;

    /* fill unused rxq slots with some skb */
    spin_lock_irqsave(&dev->req_lock, flags);
    while (!list_empty(&dev->rx_reqs)) {
        if(atomic_read(&dev->rx_qlen) >= dev->rx_req_waterline)
        {
            break;
        }

        req = container_of(dev->rx_reqs.next,
                struct usb_request, list);
        list_del_init(&req->list);
        spin_unlock_irqrestore(&dev->req_lock, flags);

        if (rx_submit(dev, req, gfp_flags) < 0) {
            defer_kevent(dev, WORK_RX_MEMORY);
            return;
        }

        spin_lock_irqsave(&dev->req_lock, flags);
    }
    spin_unlock_irqrestore(&dev->req_lock, flags);
}

static USB_VOID eth_work(struct work_struct *data)
{
    struct eth_dev    *dev = container_of(data, struct eth_dev, work.work);
    if (test_and_clear_bit(WORK_RX_MEMORY, &dev->todo)) {
        if (dev->ctx.cdc_bypass || (netif_running(dev->net)))
            rx_fill(dev, GFP_KERNEL);
    }

    if (dev->todo)
    {
        GNET_DBG(("work done, flags = 0x%lx\n", dev->todo));
    }
        
}

static USB_VOID eth_rx_work(struct work_struct *work)
{
    struct eth_dev    *dev = container_of(work, struct eth_dev, rx_work);
    rx_ads_process(dev, GFP_KERNEL);
}

static USB_VOID tx_complete(struct usb_ep *ep, struct usb_request *req)
{
    tx_ctx_t *ctx = (tx_ctx_t *)req->context;
    struct eth_dev    *dev = ep->driver_data;
    struct net_device *net = dev->net;

    USB_ULONG flags;

    GNET_TRACE(("tx_complete -> req=%p, status=%d\n", req, req->status));

    switch (req->status) {
    case 0:
        dev->ctx.stats.tx_complete_ok++;
        break;

    default:
        DBG_I(MBB_NET, "tx err %d\n", req->status);

        /* FALLTHROUGH */
    case -ECONNRESET:        /* unlink */
    case -ESHUTDOWN:        /* disconnect etc */
        dev->ctx.stats.tx_complete_fail++;
        break;
    }

    gnet_recycle_ctx_info(ctx,  (0 == req->status));  /* memory recycle */

    /* request enqueue */
    spin_lock_irqsave(&dev->tx_req_lock, flags);
    list_add_tail(&req->list, &dev->tx_reqs);
    spin_unlock_irqrestore(&dev->tx_req_lock, flags);

    atomic_dec(&dev->tx_qlen);

    if ((!dev->ctx.cdc_bypass) && (netif_carrier_ok(net) && netif_queue_stopped(net)))
    {
        dev->ctx.stats.tx_wake_queue++;
        netif_wake_queue(net);
    }
}

static inline USB_INT32 is_promisc(USB_UINT16 cdc_filter)
{
    return cdc_filter & USB_CDC_PACKET_TYPE_PROMISCUOUS;
}

tx_ctx_t *skb_to_sgs(struct gether *port, tx_ctx_t *ctx)
{
#define TRB_CACHE_QUIRK_UPDATE_BYTES(trbs,bytes,maxpacket)\
    do\
    {\
        if((bytes) >= (maxpacket))\
        {\
            (bytes) &= ((maxpacket)-1);\
            (trbs) = (bytes)?1:0;\
        }\
    }while(0)

    struct eth_dev *dev = port->ioport;
    struct sk_buff    *skb_loop = NULL;
    struct sk_buff    *skb_next = NULL;
    struct sk_buff    *skb_last = NULL;
    struct scatterlist *sgl = NULL;

    USB_UINT32 cnt = 0;
    USB_UINT32 chain_node_cnt = 0;
    USB_UINT32 trbs_for_quirk = 0;
    USB_UINT32 bytes_for_quirk = 0;
    USB_UINT32 maxpacket = port->in_ep->maxpacket;
    USB_UINT32 pkt_cnt_per_transfer = 0;
    USB_UINT32 pkt_len_per_transfer = 0;

    GNET_TRACE(("%s: enter\n", __FUNCTION__));
    /* alloc scatter-gather list */
    if(gnet_get_sgnode(ctx, ctx->pkt_cnt + dev->ctx.sg_node_reserve, GFP_ATOMIC))
    {
        dev->ctx.stats.tx_no_sg++;
        GNET_ERR(("%s: fail to alloc sgnode\n", __FUNCTION__));
        return NULL;
    }

    /* assemble skb list */
    ctx->sg_node->actual = 0;

    /* always use the sg node from the beginning */
    sgl = &ctx->sg_node->sg_tbl.sgl[0];

    bytes_for_quirk = 0;
    trbs_for_quirk = 0;
    pkt_cnt_per_transfer = 0;
    pkt_len_per_transfer = 0;

    skb_queue_walk_safe(&ctx->pkt_list,skb_loop,skb_next)
    {
        if ((dev->ctx.quirks & GNET_RNDIS_QUIRK_DWC_TRB_CACHE) 
          && (GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type))
        {
            GNET_TRACE(("unify-rndis skb_to_sgs : rndis inc bytes_for_quirk\n"));
            bytes_for_quirk += skb_loop->len;
            if(bytes_for_quirk >= maxpacket)
            {
                TRB_CACHE_QUIRK_UPDATE_BYTES(trbs_for_quirk,bytes_for_quirk,maxpacket);
            }
            else
            {
                trbs_for_quirk++;    /* try to add current trb */
                if((USB_CDC_NCM_DWC_CACHE_TRB_NUM - 1) == trbs_for_quirk)
                {
                    struct rndis_packet_msg_type *header = NULL;
                    USB_UINT32 msg_len = 0;

                    if (unlikely((NULL == skb_last) || (NULL == skb_last->data)))
                    {
                        GNET_ERR(("%s: skb_last is NULL\n", __FUNCTION__));
                        return NULL;                        
                    }
                    
                    header = (struct rndis_packet_msg_type *)skb_last->data;
                    msg_len = le32_to_cpu(header->MessageLength);
                    
                    bytes_for_quirk -= skb_loop->len;   /* give up current skb */
                    /* modify last skb */
                    msg_len += maxpacket - bytes_for_quirk;
                    header->MessageLength = cpu_to_le32(msg_len);

                    ctx->sg_node->actual += maxpacket - bytes_for_quirk;
                    pkt_len_per_transfer += maxpacket - bytes_for_quirk;

                    /* ugly to clean the page_link of sg here */
                    sgl->page_link = 0;

                    sg_set_buf(sgl, (USB_VOID *)dev->ctx.dwc_trb_cache_buf, maxpacket - bytes_for_quirk);
                    sgl++;
                    bytes_for_quirk += maxpacket - bytes_for_quirk;  /* fill dummy skb */
                    TRB_CACHE_QUIRK_UPDATE_BYTES(trbs_for_quirk,bytes_for_quirk,maxpacket);

                    trbs_for_quirk++;
                    bytes_for_quirk += skb_loop->len;
                    TRB_CACHE_QUIRK_UPDATE_BYTES(trbs_for_quirk,bytes_for_quirk,maxpacket);

                    dev->ctx.stats.tx_quirk++;
                }
            }
        }

        ctx->sg_node->actual += skb_loop->len;
        if (GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type)
        {
            GNET_TRACE(("unify-rndis skb_to_sgs : rndis inc pkt_len_per_transfer\n"));
            pkt_len_per_transfer += skb_loop->len;
        }

        skb_last = skb_loop;

        /* ugly to clean the page_link of sg here */
        sgl->page_link = 0;

        sg_set_buf(sgl, (const USB_VOID *)skb_loop->data, skb_loop->len); /* let usb core flush cache */

        if (GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type)
        {
            GNET_TRACE(("unify-rndis skb_to_sgs : rndis inc pkt_cnt_per_transfer\n"));
            pkt_cnt_per_transfer++;

            /* For RNDIS, we should mark the boundary of one transfer */
            if ((pkt_cnt_per_transfer > (GNET_RNDIS_MAX_PKT_PER_TRANSFER - 1))
                || (pkt_len_per_transfer > (GNET_RNDIS_IN_MAXSIZE_PER_TRANSFER - 1600)))
            {
                if(!(pkt_len_per_transfer % maxpacket))
                {
                    dev->ctx.stats.tx_expand_zlp++;
                    sg_set_buf(sgl, (const void *)skb_loop->data, skb_loop->len+1); /* add 1 byte for zero packet */
                    ctx->sg_node->actual += 1;
                }
                sgl++;
            
                sgl->page_link = ((USB_ULONG)(sgl + 1) | 0x01) & ~0x02;
                sgl++;

                chain_node_cnt++;

                bytes_for_quirk = 0;
                trbs_for_quirk = 0;
                pkt_cnt_per_transfer = 0;
                pkt_len_per_transfer = 0;
            }
            else
            {
                if(!(pkt_len_per_transfer % maxpacket) 
                    && skb_queue_is_last(&ctx->pkt_list,skb_loop))
                {
                    dev->ctx.stats.tx_expand_zlp1++;
                    sg_set_buf(sgl, (const void *)skb_loop->data, skb_loop->len+1); /* add 1 byte for zero packet */
                    ctx->sg_node->actual += 1;
                }

                sgl++;
            }            
        }
        else
        {
            if(!(skb_loop->len % maxpacket))
            {
                dev->ctx.stats.tx_expand_zlp++;
                sg_set_buf(sgl, (const void *)skb_loop->data, skb_loop->len+1); /* add 1 byte for zero packet */
                ctx->sg_node->actual += 1;
            }

            sgl++;        
        }
        
        /* count the gathered packets statistics */
        if (GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type)
        {
            GNET_TRACE(("unify-rndis skb_to_sgs : rndis inc tx_gathered_bytes\n"));
            dev->ctx.stats.tx_gathered_bytes += skb_loop->len - RNDIS_PACKET_HEADER_LEN;
        }
        else
        {
            dev->ctx.stats.tx_gathered_bytes += skb_loop->len;
        }
    }

    cnt = ((USB_ULONG)sgl - (USB_ULONG)(&ctx->sg_node->sg_tbl.sgl[0])) / (sizeof(struct scatterlist));

    ctx->sg_node->sg_tbl.nents = cnt - chain_node_cnt;

    /* set the last flag */
    sg_mark_end((sgl-1));

    return ctx;
}
/**
 * eth_gather_pkt - gather skb to exploit the usb bus efficiency
 * @port: associated function link
 * @skb: sk_buff to be transmitted by usb.
 * Context: soft irq
 *
 * This is the dev->wrap function routine for RNDIS & ECM, which does NOT just "wrap" the packet.
 * The main purpose of this routine is to gather the skbs from TCP/IP protocol suite,
 * and deliver to the usb core in batch. While gathering packets, we call the link->wrap
 * to "wrap" every packet.
 *
 * Returns:
 *  NULL                :failed, the input skb is released.
 *  GNET_SKB_MAGIC      :pkts gathering done, to be enqueued to the usb core
 *  Other valid value   :pkts gathering.
 */
struct sk_buff *eth_gather_pkt(struct gether *port, struct sk_buff *skb)
{
    struct eth_dev *dev = port->ioport;
    //struct net_device *net = dev->net;
    struct usb_request *req;
    tx_ctx_t *ctx;
    USB_ULONG flags;

    GNET_TRACE(("%s: enter\n", __FUNCTION__));

    if(skb) /* caller is xmit */
    {
        /* wrap the packet according to the corresponding function wrap mechanism, e.g rndis */
        if (port->wrap)
            skb = port->wrap(port, skb);

        if (unlikely(!skb))
        {
            //skb already freed
            GNET_TRACE(("%s: function driver wrap fail\n", __FUNCTION__));
            return NULL;
        }

        spin_lock_irqsave(&dev->tx_req_lock, flags);

        /* get req tx_ctx */
        req = list_first_entry(&dev->tx_reqs, struct usb_request, list);
        ctx = (tx_ctx_t *)req->context;

        if(skb_queue_empty(&ctx->pkt_list))
        {
            /* start timer */
            GNET_TRACE(("%s: 1st skb, start timer\n", __FUNCTION__));
            dev->ctx.timer.expires = jiffies + dev->ctx.timeout;  /* jiffies unit */
            if (!timer_pending(&dev->ctx.timer))
            {   
                add_timer(&dev->ctx.timer);
                dev->ctx.stats.tx_timer_activated++;
            }
        }

        /* pkt enqueue */
        skb->used_check = GNET_MAGIC_SKB_USED;
        __skb_queue_tail(&ctx->pkt_list, skb);
        
        /* update packets info */
        ctx->pkt_cnt++;
        ctx->pkt_len += skb->len;

        if((!skb->psh) && (ctx->pkt_cnt < dev->ctx.pkt_cnt_waterline)
            && (ctx->pkt_len < dev->ctx.pkt_len_waterline))
        {
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            GNET_TRACE(("%s: put skb into list only\n", __FUNCTION__));
            /* this is a bad trick, as skb may be realloced. We should NOT assume the
                skb is the same as the input parameter skb
            */
            return (struct sk_buff *)GNET_MAGIC_WRAPING;
        }
        else
        {
            /*wrap aggregation in case of waterline reach*/
            del_timer(&dev->ctx.timer); /* delete timer */
            dev->ctx.stats.tx_psh += skb->psh;
            list_del(&req->list);
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            dev->ctx.stats.tx_timer_activated--;
            GNET_TRACE(("%s: waterline reach,ctx=%p\n",__FUNCTION__,ctx));
        }
    }
    else    /* caller is timeout */
    {
        GNET_TRACE(("%s:caller is timeout\n",__FUNCTION__));

        /* check the done queue, it is possible that the normal wrap waterline and
            timer to trigger the packet send process by collision
        */
        spin_lock_irqsave(&dev->tx_req_lock, flags);
        if(!list_empty_careful(&dev->tx_done_reqs))    /* ready to send */
        {
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            /* This should NOT happen, as timeout routine shouldn't preempt over the
             * tx xmit routine. So if happened, just to skip this xmit, as normal xmit
             * will dispose the done ctx.
             */
            return (struct sk_buff *)GNET_MAGIC_WRAPED;
        }

        if(list_empty_careful(&dev->tx_reqs))  /* no tx ctx */
        {
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            dev->ctx.stats.tx_bug_noreq++;
            GNET_ERR(("%s: 700: no tx req found\n", __FUNCTION__));
            return (struct sk_buff *)GNET_MAGIC_WRAPING;
        }

        /* get req & tx_ctx */
        req = list_first_entry(&dev->tx_reqs, struct usb_request, list);
        ctx = (tx_ctx_t *)req->context;
        if(skb_queue_empty(&ctx->pkt_list))
        {
            spin_unlock_irqrestore(&dev->tx_req_lock, flags);
            GNET_ERR(("%s:800:timer triggered but not skb in tx_ctx_free_q, error\n", __FUNCTION__));
            return (struct sk_buff *)GNET_MAGIC_WRAPING;
        }

        list_del(&req->list);
        spin_unlock_irqrestore(&dev->tx_req_lock, flags);
    }

    if(skb_to_sgs(port, ctx))  /* wrap skbs according to rndis protocol */
    {
        /* push ctx to done queue */
        spin_lock_irqsave(&dev->tx_req_lock, flags);
        list_add_tail(&req->list, &dev->tx_done_reqs);
        spin_unlock_irqrestore(&dev->tx_req_lock, flags);
        GNET_TRACE(("%s:skbs to sg ok.\n",__FUNCTION__));
        return (struct sk_buff *)GNET_MAGIC_WRAPED;
    }
    else
    {
        spin_lock_irqsave(&dev->tx_req_lock, flags);
        gnet_recycle_ctx_info(ctx, 0);  /* memory recycle */
        list_add_tail(&req->list, &dev->tx_reqs);;  /* req enqueue */
        spin_unlock_irqrestore(&dev->tx_req_lock, flags);
        GNET_ERR(("%s:skbs to sg fail.\n",__FUNCTION__));
        return NULL;
    }
}

static netdev_tx_t eth_send_pkt(struct eth_dev *dev)
{
    struct gether *port = NULL;
    struct usb_ep   *in;
    struct net_device *net = dev->net;
    struct usb_request    *req = NULL;
    tx_ctx_t *ctx;
    USB_ULONG   flags = 0;
    USB_INT32    retval = 0;
    USB_UINT32   length;

    /* get request */
    spin_lock_irqsave(&dev->tx_req_lock, flags);

    if (list_empty_careful(&dev->tx_done_reqs)) {
        spin_unlock_irqrestore(&dev->tx_req_lock, flags);
        dev->ctx.stats.tx_no_donereq++;
        return NETDEV_TX_OK;
    }

    req = list_first_entry(&dev->tx_done_reqs, struct usb_request, list);
    list_del(&req->list);

    if (list_empty_careful(&dev->tx_reqs)) {
        if (!dev->ctx.cdc_bypass)
        {
            netif_stop_queue(net);
        }
        dev->ctx.stats.tx_prestop_queue++;
    }

    ctx = (tx_ctx_t *)req->context;

    spin_unlock_irqrestore(&dev->tx_req_lock, flags);

    length = ctx->sg_node->actual;
    if (GNET_CDC_TYPE_NCM == dev->ctx.cdc_type)
    {
        GNET_TRACE(("unify-ncm : gnet_eth_send_pkt ntb len %lu\n", ctx->ntb->actual));
        length += ctx->ntb->actual;    
    }

    /* fill req */
    req->num_sgs = ctx->sg_node->sg_tbl.nents;

    GNET_TRACE(("%s:gathered sg num =%d in req\n",__FUNCTION__,req->num_sgs));

    spin_lock_irqsave(&dev->lock, flags);
    port = dev->port_usb;
    if(!port || !(in = port->in_ep))
    {
        spin_unlock_irqrestore(&dev->lock, flags);
        dev->ctx.stats.dev_no_port++;
        GNET_ERR(("%s: !port || !(in = port->in_ep\n", __FUNCTION__));
        goto req_enqueue;
    }

    /* NCM requires no zlp if transfer is dwNtbInMaxSize */
    if (dev->port_usb->is_fixed &&
        length == dev->port_usb->fixed_in_len &&
        (length % in->maxpacket) == 0)
        req->zero = 0;
    else if(dev->port_usb->is_fixed &&
        length < dev->port_usb->fixed_in_len &&
        (length % in->maxpacket) == 0)
    {
        req->zero = 1;
        dev->ctx.stats.tx_zlp++;
    }
    else
        req->zero = 0;

    /* throttle high/super speed IRQ rate back slightly */
    if (gadget_is_dualspeed(dev->gadget))
        req->no_interrupt = (dev->gadget->speed == USB_SPEED_HIGH ||
                     dev->gadget->speed == USB_SPEED_SUPER)
            ? ((atomic_read(&dev->tx_qlen) % qmult) != 0)
            : 0;
    retval = usb_ep_queue(in, req, GFP_ATOMIC);

    GNET_TRACE(("eth_send_pkt : ep queue req=%p, retval=%ld\n", req, retval));
    
    switch (retval) {
    case 0:
        net->trans_start = jiffies;
        atomic_inc(&dev->tx_qlen);
        break;

    default:
        dev->ctx.stats.tx_req_dropped++;
        DBG_I(MBB_NET, "tx queue err %ld\n", retval);
        break;
    }

    if (retval) {
        spin_unlock_irqrestore(&dev->lock, flags);
        goto req_enqueue;
    }

    spin_unlock_irqrestore(&dev->lock, flags);
    return NETDEV_TX_OK;

req_enqueue:
    /* tx_reqs enqueue */
    spin_lock_irqsave(&dev->tx_req_lock, flags);
    gnet_recycle_ctx_info((tx_ctx_t *)req->context,0);
    if (list_empty(&dev->tx_reqs))
    {
        if (!dev->ctx.cdc_bypass)
        {
            netif_start_queue(net);
            dev->ctx.stats.tx_wake_queue++;
        }
    }
    list_add(&req->list, &dev->tx_reqs);
    spin_unlock_irqrestore(&dev->tx_req_lock, flags);

    return NETDEV_TX_OK;
}

/* recycle the buffer resource when failed to resume the gadget*/
static USB_VOID gnet_wakeup_fail_recycle(struct eth_dev *dev)
{
    struct usb_request    *req;
    USB_ULONG flags;

    spin_lock_irqsave(&dev->tx_req_lock, flags);
    req = list_first_entry(&dev->tx_done_reqs, struct usb_request, list);
    list_del(&req->list);

    gnet_recycle_ctx_info((tx_ctx_t *)req->context,0);

    list_add(&req->list, &dev->tx_reqs);
    spin_unlock_irqrestore(&dev->tx_req_lock, flags);
}

static netdev_tx_t eth_wrap_and_send(struct eth_dev *dev, struct sk_buff *skb)
{
    struct sk_buff *skb2;
    USB_ULONG flags;

    GNET_TRACE(("%s: enter\n", __FUNCTION__));

    spin_lock_irqsave(&dev->lock, flags);
    if (dev->wrap && dev->port_usb) {
        skb2 = dev->wrap(dev->port_usb, skb);
        if((struct sk_buff *)GNET_MAGIC_WRAPING == skb2)
        {
            spin_unlock_irqrestore(&dev->lock, flags);
            return NETDEV_TX_OK;
        }
        else if((struct sk_buff *)GNET_MAGIC_WRAPED == skb2)
        {/* wrapped OK */
            spin_unlock_irqrestore(&dev->lock, flags);
            /* check whether the gadget is suspended.
           if so, try to wake up the host */
            if (unlikely(dev->is_suspend)) {
                if (gnet_wakeup_gadget(dev)) {
                    /* wakeup failed, recycle the buffer resource */
                    gnet_wakeup_fail_recycle(dev);
                    return NETDEV_TX_OK;
                }
            }
        }
        else
        {
            /* if failed, recycle mem in wrap api */
            spin_unlock_irqrestore(&dev->lock, flags);
            dev->ctx.stats.dev_wrap_fail++;
            GNET_ERR(("%s:wrap failed\n", __FUNCTION__));
            return NETDEV_TX_OK;
        }
    }
    else {
        spin_unlock_irqrestore(&dev->lock, flags);
        dev->ctx.stats.dev_no_wrap++;
        dev->net->stats.tx_dropped++;
        if(skb){
            dev_kfree_skb_any(skb);
        }

        GNET_ERR(("%s: dev->wrap or dev->port_usb null(%lx,%lx)\n",
                    __FUNCTION__,(USB_UINT32)dev->wrap,(USB_UINT32)dev->port_usb));
        return NETDEV_TX_OK;
    }

    return eth_send_pkt(dev);
}

USB_VOID gnet_timeout_handle(USB_ULONG data)
{
    struct eth_dev  *dev = (struct eth_dev *)data;

    (USB_VOID)eth_wrap_and_send(dev, NULL);

    /* count the tx triggered by timer */
    dev->ctx.stats.tx_by_timer++;
}

static netdev_tx_t eth_start_xmit(struct sk_buff *skb,
                    struct net_device *net)
{
    struct eth_dev  *dev = netdev_priv(net);
    struct usb_ep   *in;
    USB_ULONG   flags;
    USB_UINT16 cdc_filter;

    print_skb(dev, skb, __FUNCTION__);
    dev->ctx.stats.tx_total++;

    if (GNET_MAGIC_SKB_USED == skb->used_check) {
        dev->ctx.stats.tx_skb_check_err++;
        return NETDEV_TX_OK;
    }

    spin_lock_irqsave(&dev->lock, flags);
    if (dev->port_usb) {
        in = dev->port_usb->in_ep;
        cdc_filter = dev->port_usb->cdc_filter;
    } else {
        in = NULL;
        cdc_filter = 0;
    }
    spin_unlock_irqrestore(&dev->lock, flags);

    if (!in) {
        dev->ctx.stats.tx_inep_null++;
        net->stats.tx_dropped++;
        dev_kfree_skb_any(skb);
        return NETDEV_TX_OK;
    }

    /* apply outgoing CDC or RNDIS filters */
    if (!is_promisc(cdc_filter)) {
        USB_UINT8        *dest = skb->data;

        if (is_multicast_ether_addr(dest)) {
            USB_UINT16    type;

            /* ignores USB_CDC_PACKET_TYPE_MULTICAST and host
             * SET_ETHERNET_MULTICAST_FILTERS requests
             */
            if (is_broadcast_ether_addr(dest))
                type = USB_CDC_PACKET_TYPE_BROADCAST;
            else
                type = USB_CDC_PACKET_TYPE_ALL_MULTICAST;
            if (!(cdc_filter & type)) {
                dev->ctx.stats.tx_filt_out++;
                net->stats.tx_dropped++;
                dev_kfree_skb_any(skb);
                return NETDEV_TX_OK;
            }
        }
        /* ignores USB_CDC_PACKET_TYPE_DIRECTED */
    }

    spin_lock_irqsave(&dev->tx_req_lock, flags);
    /*
     * this freelist can be empty if an interrupt triggered disconnect()
     * and reconfigured the gadget (shutting down this queue) after the
     * network stack decided to xmit but before we got the spinlock.
     */
    if (list_empty(&dev->tx_reqs)) {
        spin_unlock_irqrestore(&dev->tx_req_lock, flags);
        dev_kfree_skb_any(skb);
        if (!dev->ctx.cdc_bypass)
        {
            netif_stop_queue(net);
            dev->ctx.stats.tx_stop_queue++;
        }
        dev->ctx.stats.tx_no_req++;

        return NETDEV_TX_OK;
    }
    spin_unlock_irqrestore(&dev->tx_req_lock, flags);

    return eth_wrap_and_send(dev, skb);
}

netdev_tx_t eth_vendor_start_xmit(struct sk_buff *skb,
                    struct net_device *net)
{
    struct eth_dev *dev = netdev_priv(net);

    if(unlikely(dev->is_suspend && !(dev->gadget->rwakeup))) {
        dev_kfree_skb_any(skb);
        dev->ctx.stats.tx_drop_suspend++;
        return NETDEV_TX_OK;
    }

    return eth_start_xmit(skb, net);
}

/*-------------------------------------------------------------------------*/

static USB_VOID eth_start(struct eth_dev *dev, gfp_t gfp_flags)
{
    DBG_T(MBB_NET, "%s\n", __func__);

    dev->ctx.stats.dev_start++;
    atomic_set(&dev->rx_qlen, 0);

    /* fill the rx queue */
    rx_fill(dev, gfp_flags);

    /* and open the tx floodgates */
    atomic_set(&dev->tx_qlen, 0);

    if((!dev->ctx.cdc_bypass) && (NULL != dev->net))
    {
        netif_wake_queue(dev->net);
        dev->ctx.stats.tx_wake_queue++;
    }    
}

USB_VOID eth_suspend(struct eth_dev *dev)
{
    if (NULL == dev)
    {
        DBG_E(MBB_NET, "%s: empty dev\n", __func__);
        return;
    }
    
    DBG_T(MBB_NET, "%s\n", __func__);

    dev->ctx.stats.dev_suspend++;
    
    if (!dev->gadget->rwakeup) 
    {
        if ((!dev->ctx.cdc_bypass) && (NULL != dev->net))
        {
            GNET_INFO(("%s: stop queue\n", __func__));
            netif_stop_queue(dev->net);
            dev->ctx.stats.tx_stop_queue++;
        }
    }

    dev->is_suspend = 1;
}

USB_VOID eth_resume(struct eth_dev *dev)
{
    if (NULL == dev)
    {
        DBG_E(MBB_NET, "%s: empty dev\n", __func__);
        return;
    }
    
    DBG_T(MBB_NET, "%s\n", __func__);

    dev->ctx.stats.dev_resume++;
    
    eth_start(dev, GFP_ATOMIC);
    
    /* if (!dev->gadget->rwakeup) 
    {
        if ((!dev->ctx.cdc_bypass) && (NULL != dev->net))
        {
            GNET_INFO(("%s: start queue\n", __func__));
            netif_start_queue(dev->net);
        }

        dev->ctx.stats.tx_wake_queue++;
    } */
    dev->is_suspend = 0;
}

static USB_INT eth_open(struct net_device *net)
{
    struct eth_dev    *dev = netdev_priv(net);
    struct gether    *link;
#if (FEATURE_ON == MBB_CTF_COMMON)
    USB_INT msglevel = 1;
#endif
    DBG_T(MBB_NET, "%s\n", __func__);
    dev->ctx.stats.dev_open++;
    
    if (netif_carrier_ok(net))
        eth_start(dev, GFP_KERNEL);

    spin_lock_irq(&dev->lock);
    link = dev->port_usb;
    if (link && link->open)
        link->open(link);
    spin_unlock_irq(&dev->lock);

#if (FEATURE_ON == MBB_CTF_COMMON)
    if(!usb_cih) 
    {
        DBG_I(MBB_NET, "Init CTF for USB.\n");
        usb_fake_osh = (osl_t *)0xdeadbeaf; /* watch here, it might die */
        usb_cih = ctf_attach(usb_fake_osh, net->name, &msglevel, usb_ctf_detach, NULL /* et*/ );
        if (usb_cih)
        {
            DBG_I(MBB_NET, "Successful attach CTF for USB. usb_cih = 0x%08x ctf_attach:%x\n", 
                (USB_UINT)usb_cih, (USB_UINT)ctf_attach_fn);
        }

        if ((ctf_dev_register(usb_cih, net, FALSE) != BCME_OK) ||
            (ctf_enable(usb_cih, net, TRUE,NULL) != BCME_OK)) 
        {
            DBG_E(MBB_NET, "ctf_dev_register() failed for USB.\n");
        }
        else
        {
            DBG_I(MBB_NET, "Register CTF for USB successful.\n");
        }
    }
#endif


    return 0;
}

static USB_INT eth_stop(struct net_device *net)
{
    struct eth_dev    *dev = netdev_priv(net);
    USB_ULONG          flags;

    DBG_T(MBB_NET, "%s\n", __func__);
    dev->ctx.stats.dev_stop++;
    
    if (!dev->ctx.cdc_bypass)
    {
        netif_stop_queue(net);
        dev->ctx.stats.tx_stop_queue++;
    }

    DBG_I(MBB_NET, "stop stats: rx/tx %ld/%ld, errs %ld/%ld\n",
        net->stats.rx_packets, net->stats.tx_packets,
        net->stats.rx_errors, net->stats.tx_errors);

    /* ensure there are no more active requests */
    spin_lock_irqsave(&dev->lock, flags);
    if (dev->port_usb) {
        struct gether    *link = dev->port_usb;

        if (link->close)
            link->close(link);

        /* NOTE:  we have no abort-queue primitive we could use
         * to cancel all pending I/O.  Instead, we disable then
         * reenable the endpoints ... this idiom may leave toggle
         * wrong, but that's a self-correcting error.
         *
         * REVISIT:  we *COULD* just let the transfers complete at
         * their own pace; the network stack can handle old packets.
         * For the moment we leave this here, since it works.
         */
        /* usb_ep_disable(link->in_ep);
        usb_ep_disable(link->out_ep);
        if (netif_carrier_ok(net)) {
            DBG_I(MBB_NET, "host still using in/out endpoints\n");
            (USB_VOID)usb_ep_enable(link->in_ep);
            (USB_VOID)usb_ep_enable(link->out_ep);
        } */
    }
    spin_unlock_irqrestore(&dev->lock, flags);

#if (FEATURE_ON == MBB_CTF_COMMON)
    ctf_dev_unregister(usb_cih, net);
    usb_cih = NULL;
#endif /* CTF */

#if (FEATURE_ON == MBB_FEATURE_FASTIP)
    fastip_dev_unregister(usb_fastip_handle, dev->net);
    usb_fastip_handle = NULL;
#endif /* MBB_FEATURE_FASTIP */

    return 0;
}

USB_INT32 eth_add_tx_req(struct eth_dev *dev)
{
    struct usb_request *req = NULL;
    tx_ctx_t *tx_ctx = NULL;
    sg_node_t *sg_node = NULL;
    ntb_node_t *ntb_node = NULL;

    DBG_T(MBB_NET, "%s\n", __FUNCTION__);
    
    req = usb_ep_alloc_request(dev->port_usb->in_ep, GFP_ATOMIC);
    if(!req)
    {            
        DBG_E(MBB_NET, "%s: alloc req failed\n", __FUNCTION__);
        goto fail;
    }

    tx_ctx = (tx_ctx_t *)kzalloc(sizeof(tx_ctx_t), GFP_ATOMIC);
    if(!tx_ctx)
    {            
        DBG_E(MBB_NET, "%s: alloc tx_ctx failed\n", __FUNCTION__);
        goto fail;
    }
    
    sg_node = alloc_sg_node(dev->ctx.sg_node_nents, GFP_ATOMIC);
    if(!sg_node)
    {            
        DBG_E(MBB_NET, "%s: alloc sg_node failed\n", __FUNCTION__);
        goto fail;
    }
    tx_ctx->sg_node = sg_node;

    if (GNET_CDC_TYPE_NCM == dev->ctx.cdc_type)  
    {
        /* alloc ntb node */
        ntb_node = ncm_alloc_ntbnode(dev->ctx.ntb_node_size, GFP_ATOMIC);
        if(!ntb_node)
        {        
            DBG_E(MBB_NET, "%s: alloc ntb_node failed\n", __FUNCTION__);
            goto fail;
        }
        tx_ctx->ntb = ntb_node;
    }

    skb_queue_head_init(&tx_ctx->pkt_list);
    tx_ctx->dev = dev;
    tx_ctx->req = req;
    req->complete = tx_complete;
    req->context = (USB_VOID *)tx_ctx;
    req->length = 1;    /* dummy, just for usb_gadget_map_request para check */
    req->sg = tx_ctx->sg_node->sg_tbl.sgl;
    if (GNET_CDC_TYPE_ECM == dev->ctx.cdc_type)
    {
        req->sg_mode = USB_REQUEST_M_TRANSFER;  /* For ecm, we treat sg as a special use */
    }

    spin_lock(&dev->tx_req_lock);
    list_add(&req->list, &dev->tx_reqs);
    dev->tx_req_num++;
    spin_unlock(&dev->tx_req_lock);
    
    return 0;
    
fail :

    if (tx_ctx)
    {
        free_tx_ctx(tx_ctx);
    }

    if (req)
    {
        usb_ep_free_request(dev->port_usb->in_ep, req);
    }

    return -ENOMEM;
}

USB_UINT32 eth_get_net_speed(USB_VOID)
{
    mbb_usb_nv_info_st *usb_nv_ctx = usb_nv_get_ctx();
    if (NULL != usb_nv_ctx)
    {
        return usb_nv_ctx->net_speed_info.net_speed;
    }

    return 0;
}

/*-------------------------------------------------------------------------*/

/* initial value, changed by "ifconfig usb0 hw ether xx:xx:xx:xx:xx:xx" */
static USB_CHAR *dev_addr[GNET_MAX_NUM]={
    "0A:5B:8F:27:9A:64",
    "0A:5B:8F:27:9A:65",
    "0A:5B:8F:27:9A:66",
    "0A:5B:8F:27:9A:67"
    };
module_param_array(dev_addr, charp, NULL, S_IRUGO);
MODULE_PARM_DESC(dev_addr, "Device Ethernet Address");

/* this address is invisible to ifconfig */
static USB_CHAR *host_addr[GNET_MAX_NUM]={
    "0C:5B:8F:27:9A:64",
    "0C:5B:8F:27:9A:65",
    "0C:5B:8F:27:9A:66",
    "0C:5B:8F:27:9A:67"
};
module_param_array(host_addr, charp, NULL, S_IRUGO);
MODULE_PARM_DESC(host_addr, "Host Ethernet Address");

static USB_VOID gether_ether_addr_init(USB_VOID)
{
    /* may get from nv someday */
    return ;
}

static USB_INT32 get_ether_addr(const USB_CHAR *str, USB_UINT8 *dev_addr)
{
    if (str) {
        USB_UINT32    i;

        for (i = 0; i < 6; i++) {
            USB_UCHAR num;

            if ((*str == '.') || (*str == ':'))
                str++;
            num = hex_to_bin(*str++) << 4;
            num |= hex_to_bin(*str++);
            dev_addr [i] = num;
        }
        if (is_valid_ether_addr(dev_addr))
            return 0;
    }
    random_ether_addr(dev_addr);
    return 1;
}

static const struct net_device_ops eth_netdev_ops = {
    .ndo_open        = eth_open,
    .ndo_stop        = eth_stop,
    .ndo_start_xmit        = eth_start_xmit,
    .ndo_change_mtu        = ueth_change_mtu,
    .ndo_set_mac_address     = eth_mac_addr,
    .ndo_validate_addr    = eth_validate_addr,
};

static struct device_type gadget_type = {
    .name    = "gadget",
};

static USB_VOID eth_stats_clear(struct eth_dev  *dev)
{
    memset((USB_VOID *)&dev->ctx.stats, 0 , sizeof(dev->ctx.stats));
}

static ssize_t
gnet_show(struct device *pdev, struct device_attribute *attr, USB_CHAR *buf)
{
    struct eth_dev *dev = container_of(attr,struct eth_dev,dev_attr);
    static struct eth_dev *dev_dbg = NULL;
    USB_INT count = 0;
    USB_INT showkern = 0;
    struct usb_ep *notify=NULL;
    USB_UINT32 *header_room_resved_ptr = NULL;
    USB_INT32 i;

    if (gnet_msg_level & GNET_LEVEL_KERN)
    {
        showkern = 1;
        if (!dev_dbg)
        {
            dev_dbg = (struct eth_dev *)kzalloc(sizeof(struct eth_dev), GFP_ATOMIC);
            if (!dev_dbg)
            {
                DBG_T(MBB_NCM, "alloc dev_dbg failed\n");
                return 0;
            }
        }

        memcpy(dev_dbg, dev, sizeof(struct eth_dev));
        dev = dev_dbg;
    }
    else
    {
        if (dev_dbg)
        {
            kfree(dev_dbg);
            dev_dbg = NULL;
        }
    }
        
#define GNET_SHOW_PRINT(fmt, args...) \
    if (showkern) \
        printk( KERN_ERR ""fmt , ## args); \
    else \
        count += snprintf(buf+count, PAGE_SIZE, ""fmt , ## args);

    GNET_SHOW_PRINT("|-+balong gnet %d dump:\n",dev->ctx.eth_idx);
    GNET_SHOW_PRINT("| |--gnet mode             :%s\n",gnet_get_netmode(dev)?"NETWORK BYPASS MODE":"NETWORK MODE");
    GNET_SHOW_PRINT("| |--tx ep name            :%s\n",dev->port_usb?dev->port_usb->in_ep->name:"UNKNOWN");
    GNET_SHOW_PRINT("| |--rx ep name            :%s\n",dev->port_usb?dev->port_usb->out_ep->name:"UNKNOWN");
    notify = dev->port_usb ? dev->port_usb->notify : NULL;
    GNET_SHOW_PRINT("| |--notify ep name        :%s\n",notify?notify->name:"UNKNOWN");
    GNET_SHOW_PRINT("| |--is_suspend            :%d\n",dev->is_suspend);
    GNET_SHOW_PRINT("| |--tx_qlen               :%u\n",atomic_read(&dev->tx_qlen));
    GNET_SHOW_PRINT("| |--tx_req_num            :%lu\n",dev->tx_req_num);
    GNET_SHOW_PRINT("| |--rx_qlen               :%u\n",atomic_read(&dev->rx_qlen));
    GNET_SHOW_PRINT("| |--rx_req_num            :%lu\n",dev->rx_req_num);
    GNET_SHOW_PRINT("| |--rx_req_waterline      :%lu\n",dev->rx_req_waterline);
    GNET_SHOW_PRINT("| |--rx_doclone            :%u\n",dev->doclone);
    GNET_SHOW_PRINT("| |--rx_frames_waterline   :%lu\n",dev->rx_frames_waterline);
#ifdef CONFIG_GNET_PREALLOC_TX_MEM
    GNET_SHOW_PRINT("| |--sg_node_nents         :%lu\n",dev->ctx.sg_node_nents);
    GNET_SHOW_PRINT("| |--ntb_node_size         :%lu\n",dev->ctx.ntb_node_size);
#endif

    if (GNET_CDC_TYPE_NCM == dev->ctx.cdc_type) 
    {
        header_room_resved_ptr = dev->port_usb ? dev->port_usb->header_room_resved_ptr : NULL;
    }
    GNET_SHOW_PRINT("| |--skb hdr resved len    :%lu\n",
         header_room_resved_ptr ? (*header_room_resved_ptr) : 0xffffffff);    /* 0xffffffff means invalid */

#ifdef CONFIG_GNET_PREALLOC_RX_MEM
    GNET_SHOW_PRINT("| |--rx_skb_num            :%lu\n",dev->ctx.rx_skb_num);
    GNET_SHOW_PRINT("| |--rx_skb_size           :%lu\n",dev->ctx.rx_skb_size);
    GNET_SHOW_PRINT("| |--rx_skb_waterline      :%lu\n",dev->ctx.rx_skb_waterline);
    GNET_SHOW_PRINT("| |--rx_skb_free           :%u\n",dev->ctx.rx_skb_free_q?dev->ctx.rx_skb_free_q->qlen:0xffffffff);
    GNET_SHOW_PRINT("| |--rx_skb_done           :%u\n",dev->ctx.rx_skb_done_q?dev->ctx.rx_skb_done_q->qlen:0xffffffff);
#endif
    GNET_SHOW_PRINT("| |--timeout               :%lu\n",dev->ctx.timeout);
    GNET_SHOW_PRINT("| |--protect_num            :%lu\n",dev->ctx.protect_num);
    GNET_SHOW_PRINT("| |--pkt_cnt_waterline     :%lu\n",dev->ctx.pkt_cnt_waterline);
    GNET_SHOW_PRINT("| |--pkt_len_waterline     :%lu\n",dev->ctx.pkt_len_waterline);
    GNET_SHOW_PRINT("| |--gnet_msg_level        :%08lx\n",gnet_msg_level);
    GNET_SHOW_PRINT("| |--gnet_dbg_level        :%08lx\n",gnet_dbg_level);
    GNET_SHOW_PRINT("| |--quirks                :%08lx\n",dev->ctx.quirks);
    GNET_SHOW_PRINT("| |--dwc_trb_cache_buf     :%08lx\n",(USB_UINT32)dev->ctx.dwc_trb_cache_buf);
    GNET_SHOW_PRINT("| |--stat->dev_no_wrap     :%lu\n",dev->ctx.stats.dev_no_wrap);
    GNET_SHOW_PRINT("| |--stat->dev_no_port     :%lu\n",dev->ctx.stats.dev_no_port);
    GNET_SHOW_PRINT("| |--stat->dev_wrap_fail   :%lu\n",dev->ctx.stats.dev_wrap_fail);
    GNET_SHOW_PRINT("| |--stat->dev_disconnect  :%lu\n",dev->ctx.stats.dev_disconnect);
    GNET_SHOW_PRINT("| |--stat->dev_connect     :%lu\n",dev->ctx.stats.dev_connect);
    GNET_SHOW_PRINT("| |--stat->dev_open        :%lu\n",dev->ctx.stats.dev_open);
    GNET_SHOW_PRINT("| |--stat->dev_start       :%lu\n",dev->ctx.stats.dev_start);
    GNET_SHOW_PRINT("| |--stat->dev_stop        :%lu\n",dev->ctx.stats.dev_stop);
    GNET_SHOW_PRINT("| |--stat->dev_suspend     :%lu\n",dev->ctx.stats.dev_suspend);
    GNET_SHOW_PRINT("| |--stat->dev_resume      :%lu\n",dev->ctx.stats.dev_resume);
    GNET_SHOW_PRINT("| |--stat->tx_inep_null    :%lu\n",dev->ctx.stats.tx_inep_null);
    GNET_SHOW_PRINT("| |--stat->tx_skb_check_err:%lu\n",dev->ctx.stats.tx_skb_check_err);    
    GNET_SHOW_PRINT("| |--stat->tx_skb_tailrom_lack    :%lu\n",dev->ctx.stats.tx_skb_tailrom_lack);
    GNET_SHOW_PRINT("| |--stat->tx_skb_tailrom_expand  :%lu\n",dev->ctx.stats.tx_skb_tailrom_expand);
    GNET_SHOW_PRINT("| |--stat->tx_zlp                 :%lu\n",dev->ctx.stats.tx_zlp);
    GNET_SHOW_PRINT("| |--stat->tx_expand_zlp          :%lu\n",dev->ctx.stats.tx_expand_zlp);
    GNET_SHOW_PRINT("| |--stat->tx_expand_zlp1         :%lu\n",dev->ctx.stats.tx_expand_zlp1);
    GNET_SHOW_PRINT("| |--stat->tx_quirk               :%lu\n",dev->ctx.stats.tx_quirk);
    GNET_SHOW_PRINT("| |--stat->tx_skb_tailrom_lack    :%lu\n",dev->ctx.stats.tx_skb_tailrom_lack);
    GNET_SHOW_PRINT("| |--stat->tx_skb_tailrom_expand  :%lu\n",dev->ctx.stats.tx_skb_tailrom_expand);    
    GNET_SHOW_PRINT("| |--stat->tx_filt_out     :%lu\n",dev->ctx.stats.tx_filt_out);
    GNET_SHOW_PRINT("| |--stat->tx_drop_suspend :%lu\n",dev->ctx.stats.tx_drop_suspend);
    GNET_SHOW_PRINT("| |--stat->tx_no_ctx       :%lu\n",dev->ctx.stats.tx_no_ctx);
    GNET_SHOW_PRINT("| |--stat->tx_no_sg        :%lu\n",dev->ctx.stats.tx_no_sg);
    GNET_SHOW_PRINT("| |--stat->tx_no_ntb       :%lu\n",dev->ctx.stats.tx_no_ntb);
    GNET_SHOW_PRINT("| |--stat->tx_ntb_overflow :%lu\n",dev->ctx.stats.tx_ntb_overflow);
    GNET_SHOW_PRINT("| |--stat->tx_no_req       :%lu\n",dev->ctx.stats.tx_no_req);
    GNET_SHOW_PRINT("| |--stat->tx_noreq_inwrap :%lu\n",dev->ctx.stats.tx_noreq_inwrap);
    GNET_SHOW_PRINT("| |--stat->tx_bug_noreq    :%lu\n",dev->ctx.stats.tx_bug_noreq);
    GNET_SHOW_PRINT("| |--stat->tx_bug_nopkt    :%lu\n",dev->ctx.stats.tx_bug_nopkt);
    GNET_SHOW_PRINT("| |--stat->tx_no_donereq   :%lu\n",dev->ctx.stats.tx_no_donereq);
    GNET_SHOW_PRINT("| |--stat->tx_stop_queue   :%lu\n",dev->ctx.stats.tx_stop_queue);
    GNET_SHOW_PRINT("| |--stat->tx_prestop_queue:%lu\n",dev->ctx.stats.tx_prestop_queue);
    GNET_SHOW_PRINT("| |--stat->tx_wake_queue   :%lu\n",dev->ctx.stats.tx_wake_queue);
    GNET_SHOW_PRINT("| |--stat->tx_req_dropped  :%lu\n",dev->ctx.stats.tx_req_dropped);
    GNET_SHOW_PRINT("| |--stat->tx_complete_ok  :%lu\n",dev->ctx.stats.tx_complete_ok);
    GNET_SHOW_PRINT("| |--stat->tx_complete_fail:%lu\n",dev->ctx.stats.tx_complete_fail);
    GNET_SHOW_PRINT("| |--stat->tx_by_timer     :%lu\n",dev->ctx.stats.tx_by_timer);
    GNET_SHOW_PRINT("| |--stat->tx_timer_activated:%lu\n",dev->ctx.stats.tx_timer_activated);
    GNET_SHOW_PRINT("| |--stat->tx_skb_realloc  :%lu\n",dev->ctx.stats.tx_skb_realloc);
    GNET_SHOW_PRINT("| |--stat->tx_gathered_bytes:%lu\n",dev->ctx.stats.tx_gathered_bytes);
    GNET_SHOW_PRINT("| |--stat->tx_total        :%lu\n",dev->ctx.stats.tx_total);
    GNET_SHOW_PRINT("| |--stat->tx_ok_bytes     :%lu\n",dev->ctx.stats.tx_ok_bytes);
    GNET_SHOW_PRINT("| |--stat->tx_ok_pkts      :%lu\n",dev->ctx.stats.tx_ok_pkts);
    GNET_SHOW_PRINT("| |--stat->tx_err_pkts     :%lu\n",dev->ctx.stats.tx_err_pkts);
    GNET_SHOW_PRINT("| |--stat->tx_err_bytes    :%lu\n",dev->ctx.stats.tx_err_bytes);
    GNET_SHOW_PRINT("| |--stat->tx_psh          :%lu\n",dev->ctx.stats.tx_psh);
    GNET_SHOW_PRINT("| |--stat->rx_zlp          :%lu\n",dev->ctx.stats.rx_zlp);
    GNET_SHOW_PRINT("| |--stat->rx_no_skb       :%lu\n",dev->ctx.stats.rx_no_skb);
    GNET_SHOW_PRINT("| |--stat->rx_req_submitfail  :%lu\n",dev->ctx.stats.rx_req_submitfail);
    GNET_SHOW_PRINT("| |--stat->rx_req_manudrop :%lu\n",dev->ctx.stats.rx_req_manudrop);
    GNET_SHOW_PRINT("| |--stat->rx_skb_protect_drop:%lu\n",dev->ctx.stats.rx_skb_protect_drop);
    GNET_SHOW_PRINT("| |--stat->rx_recycle_delay:%lu\n",dev->ctx.stats.rx_skb_recycle_delay);
    GNET_SHOW_PRINT("| |--stat->rx_completed    :%lu\n",dev->ctx.stats.rx_completed);
    GNET_SHOW_PRINT("| |--stat->rx_complete_ok  :%lu\n",dev->ctx.stats.rx_complete_ok);
    GNET_SHOW_PRINT("| |--stat->rx_unwrap_fail  :%lu\n",dev->ctx.stats.rx_unwrap_fail);
    GNET_SHOW_PRINT("| |--stat->rx_packets      :%lu\n",dev->ctx.stats.rx_packets);
    GNET_SHOW_PRINT("| |--stat->rx_bytes        :%lu\n",dev->ctx.stats.rx_bytes);
    GNET_SHOW_PRINT("| |--stat->rx_packets_fail :%lu\n",dev->ctx.stats.rx_packets_fail);
    GNET_SHOW_PRINT("| |--stat->rx_bytes_fail   :%lu\n",dev->ctx.stats.rx_bytes_fail);
    GNET_SHOW_PRINT("| |--stat->rx_packets_err  :%lu\n",dev->ctx.stats.rx_packets_err);
    GNET_SHOW_PRINT("| |--stat->rx_packets_overwline :%lu\n",dev->ctx.stats.rx_packets_overwline);
    for(i=0; i<WORK_RX_BOTTROM; i++)
    {
        GNET_SHOW_PRINT("| |--stat->rx_kevent_busy[%ld]  :%lu\n",i,dev->ctx.stats.rx_kevent_busy[i]);
        GNET_SHOW_PRINT("| |--stat->rx_kevent_ok[%ld]    :%lu\n",i,dev->ctx.stats.rx_kevent_ok[i]);
    }
    return count;
}

#define CMD_LEN (32)
struct gnet_sysfs_cmd
{
    USB_CHAR   name[CMD_LEN];
    USB_UINT32 format; /* 0:none 1:dec 2:hex */
};

static struct gnet_sysfs_cmd g_gnet_cmdtbl[]=
{
    {"gnet_msg_level",2},
    {"gnet_dbg_level",2},
    {"pkt_cnt_waterline",1},
    {"pkt_len_waterline",1},
    {"rx_req_waterline",1},
    {"rx_skb_waterline",1},
    {"timeout",1},
    {"quirks",2},
    {"rx_doclone",1},
    {"clrstats",0},
    {"help",0},
    {"rx_frames_waterline",1}
};

USB_VOID gnet_sysfs_help(USB_VOID)
{
    USB_UINT32 loop;
    printk("gnet sysfs cmdlist:\n");
    for(loop=0; loop<sizeof(g_gnet_cmdtbl)/sizeof(struct gnet_sysfs_cmd); loop++)
    {
        printk("->%s\n",g_gnet_cmdtbl[loop].name);
    }
}

static ssize_t
gnet_store(struct device *pdev, struct device_attribute *attr,
                    const USB_CHAR *buf, size_t size)
{

    struct eth_dev        *dev = container_of(attr,struct eth_dev,dev_attr);
    USB_UINT32 loop;
    USB_UINT32 cmd_value = 0;
    USB_CHAR   cmd_str[CMD_LEN] = {0};

    (void)sscanf(buf, "%s",cmd_str);

    for(loop=0; loop<sizeof(g_gnet_cmdtbl)/sizeof(struct gnet_sysfs_cmd); loop++){
        if(!memcmp(cmd_str, g_gnet_cmdtbl[loop].name,strlen(g_gnet_cmdtbl[loop].name))){
            break;
        }
    }

    if(sizeof(g_gnet_cmdtbl)/sizeof(struct gnet_sysfs_cmd) == loop)
    {
        printk("unsupport cmd(%s)!\n",cmd_str);
        return size;
    }

    if(2 == g_gnet_cmdtbl[loop].format)
    {
        (void)sscanf(buf, "%s %lx",cmd_str,&cmd_value);
    }
    else if(1 == g_gnet_cmdtbl[loop].format)
    {
        (void)sscanf(buf, "%s %ld",cmd_str,&cmd_value);
    }
    else
    {
        ;
    }

    switch(loop)
    {
        case 0:
            gnet_msg_level = cmd_value;
            break;
        case 1:
            gnet_dbg_level = cmd_value;
            break;
        case 2:
            dev->ctx.pkt_cnt_waterline = cmd_value;
            break;
        case 3:
            dev->ctx.pkt_len_waterline = cmd_value;
            break;
        case 4:
            dev->rx_req_waterline = cmd_value;
            break;
        case 5:
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
            dev->ctx.rx_skb_waterline = cmd_value;
#endif
            break;
        case 6:
            dev->ctx.timeout = cmd_value;
            break;
        case 7:
            dev->ctx.quirks = cmd_value;
            break;
        case 8:
            gnet_set_rx_clone(dev,(USB_UINT32)(cmd_value & 0x1));
            break;
        case 9:
            eth_stats_clear(dev);
            break;
        case 10:
            gnet_sysfs_help();
            break;
        case 11:
            dev->rx_frames_waterline = cmd_value;
            break;

        default:
            break;
    }

    return size;
}

/**
 * gether_setup - initialize one ethernet-over-usb link
 * @g: gadget to associated with these links
 * @ethaddr: NULL, or a buffer in which the ethernet address of the
 *    host side of the link is recorded
 * Context: may sleep
 *
 * This sets up the single network link that may be exported by a
 * gadget driver using this framework.  The link layer addresses are
 * set up using module parameters.
 *
 * Returns negative errno, or zero on success
 */
USB_INT gether_setup(struct usb_gadget *g, USB_UINT8 ethaddr[ETH_ALEN])
{
    return gether_setup_name(g, ethaddr, "usb");
}

/**
 * gether_setup_name - initialize one ethernet-over-usb link
 * @g: gadget to associated with these links
 * @ethaddr: NULL, or a buffer in which the ethernet address of the
 *    host side of the link is recorded
 * @netname: name for network device (for example, "usb")
 * Context: may sleep
 *
 * This sets up the single network link that may be exported by a
 * gadget driver using this framework.  The link layer addresses are
 * set up using module parameters.
 *
 * Returns negative errno, or zero on success
 */
USB_INT gether_setup_name(struct usb_gadget *g, USB_UINT8 ethaddr[ETH_ALEN],
        const USB_CHAR *netname)
{
    struct eth_dev        *dev;
    struct net_device    *net;
    USB_INT           status;
#if(FEATURE_ON == MBB_FEATURE_FASTIP)
    USB_INT32 msglevel = 1;
#endif
    USB_UINT32   gnet_pkt_num_waterline = 0;
    USB_UINT32   gnet_out_max_per_trans = 0;
    USB_UINT32   gnet_pkt_len_waterline = 0;
    USB_UINT32   gnet_quick_dwc_trb_cache = 0;

    if (the_dev[geth_idx])
    {
         DBG_E(MBB_PNP,"the_dev[geth_idx] :%p,geth_idx:%u\n",the_dev[geth_idx],geth_idx);
         return -EBUSY;
    }
        

    net = alloc_etherdev(sizeof *dev);
    if (!net)
        return -ENOMEM;

    dev = netdev_priv(net);
    spin_lock_init(&dev->lock);
    spin_lock_init(&dev->req_lock);
    spin_lock_init(&dev->tx_req_lock);
    INIT_DELAYED_WORK(&dev->work, eth_work);
    INIT_LIST_HEAD(&dev->tx_done_reqs);
    INIT_LIST_HEAD(&dev->tx_reqs);
    INIT_LIST_HEAD(&dev->rx_reqs);

    skb_queue_head_init(&dev->rx_frames);

    dev->rx_frames_waterline = RX_FRAMES_MAX;
    skb_queue_head_init(&dev->rx_frames_to_ads);
    INIT_WORK(&dev->rx_work, eth_rx_work);
    dev->ctx.cdc_bypass = 0;

    if (!strcmp(netname, "ecm"))
    {
        dev->ctx.cdc_type = GNET_CDC_TYPE_ECM;
        gnet_pkt_num_waterline = GNET_ECM_PKT_NUM_WATERLINE;
        gnet_out_max_per_trans = GNET_ECM_OUT_MAXSIZE_PER_TRANSFER;
        gnet_pkt_len_waterline = GNET_ECM_PKT_LEN_WATERLINE;
        gnet_quick_dwc_trb_cache = GNET_ECM_QUIRK_DWC_TRB_CACHE;
    }
    else if (!strcmp(netname, "rndis"))
    {
        dev->ctx.cdc_type = GNET_CDC_TYPE_RNDIS;
        gnet_pkt_num_waterline = GNET_RNDIS_PKT_NUM_WATERLINE;
        gnet_out_max_per_trans = GNET_RNDIS_OUT_MAXSIZE_PER_TRANSFER;
        gnet_pkt_len_waterline = GNET_RNDIS_PKT_LEN_WATERLINE;
        gnet_quick_dwc_trb_cache = GNET_RNDIS_QUIRK_DWC_TRB_CACHE;
    } 
    else if ((!strcmp(netname, "ncm")) || (!strcmp(netname, "usb")))
    {
        if (ncm_get_bypass_state())
        {
            dev->ctx.cdc_bypass = 1;
        }
        
        dev->ctx.cdc_type = GNET_CDC_TYPE_NCM;
        gnet_pkt_num_waterline = GNET_NCM_PKT_NUM_WATERLINE;
        gnet_out_max_per_trans = GNET_NCM_OUT_MAXSIZE_PER_TRANSFER;
        gnet_pkt_len_waterline = GNET_NCM_PKT_LEN_WATERLINE;
        gnet_quick_dwc_trb_cache = GNET_NCM_QUIRK_DWC_TRB_CACHE;
    } 
    else
    {
        GNET_INFO(("unify gether_setup_name : invalid netname %s\n", netname));
        return -EINVAL;
    }

    /* 路由模式下上行使用队列处理 */
    if (!dev->ctx.cdc_bypass)
    {
        gnet_dbg_level &= ~(0xFFFFFFFF & GNET_DBG_RX2ADS); 
    }
    
    DBG_T(MBB_NET, "gether_setup_name : netname=%s, cdc_type=%lu, bypass=%lu, net_param=%lu,%lu,%lu,%lu\n",
      netname, dev->ctx.cdc_type, dev->ctx.cdc_bypass, gnet_pkt_num_waterline, 
      gnet_out_max_per_trans, gnet_pkt_len_waterline, gnet_quick_dwc_trb_cache);

#ifdef CONFIG_GNET_PREALLOC_TX_MEM
    dev->ctx.sg_node_nents = gnet_pkt_num_waterline;
    dev->ctx.ntb_node_size = (USB_CDC_NCM_NDP32_INDEX_MIN +
        USB_CDC_NCM_NDP32_MIN + GNET_NCM_PKT_NUM_WATERLINE * 8);
#ifdef CONFIG_NCM_MBIM_SUPPORT
    /* M-PDP 需要更多的NDP空间,NTH16=12字节，NTH32=16字节，NDP16=12字节，NDP32=24字节 */
    if (usb_nv_get_ctx()->winblue_profile.MBIMEnable)
    {
        dev->ctx.ntb_node_size += (GNET_MAX_NUM * USB_CDC_NCM_NDP32_MIN);
    }
#endif
#endif  /* CONFIG_GNET_PREALLOC_TX_MEM */

#ifdef CONFIG_GNET_PREALLOC_RX_MEM
    /* init rx skb queue */
    //skb_queue_head_init(&dev->ctx.rx_skb_free_q);
    //skb_queue_head_init(&dev->ctx.rx_skb_done_q);
    dev->ctx.rx_skb_size = gnet_out_max_per_trans;
    dev->ctx.rx_skb_num = GNET_RX_MEM_PREALLOC/gnet_out_max_per_trans;
    dev->ctx.rx_skb_waterline = dev->ctx.rx_skb_num/4;
#endif

    /* init timer */
    init_timer(&dev->ctx.timer);
    dev->ctx.timer.function = gnet_timeout_handle;
    dev->ctx.timer.data = (USB_ULONG)dev;
    dev->ctx.timeout = USB_CDC_GNET_TIMEOUT;

    dev->ctx.pkt_cnt_waterline = gnet_pkt_num_waterline;
    dev->ctx.pkt_len_waterline = gnet_pkt_len_waterline;

    dev->ctx.quirks = gnet_quick_dwc_trb_cache;
    if(dev->ctx.quirks & gnet_quick_dwc_trb_cache)
    {
        dev->ctx.sg_node_reserve =
            dev->ctx.sg_node_nents/(USB_CDC_NCM_DWC_CACHE_TRB_NUM-2) + 2;
    }

    if (GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type)
    {
        GNET_INFO(("unify-rndis gether_setup_name : set rndis sg_node_reserve\n"));
        dev->ctx.sg_node_reserve += (gnet_pkt_num_waterline / GNET_RNDIS_MAX_PKT_PER_TRANSFER - 1);    
    }

    dev->ctx.sg_node_nents += dev->ctx.sg_node_reserve + 1;

    /* network device setup */
    dev->net = net;
    snprintf(net->name, sizeof(net->name), "usb%d", geth_idx);

    gether_ether_addr_init();
    
    if (get_ether_addr(dev_addr[geth_idx], net->dev_addr))
    {
        GNET_INFO(("using random %s ethernet address\n", "self"));
    }
    
    if (get_ether_addr(host_addr[geth_idx], dev->host_mac))
    {
        GNET_INFO(("using random %s ethernet address\n", "host"));
    }

    if (ethaddr)
        memcpy(ethaddr, dev->host_mac, ETH_ALEN);

    net->netdev_ops = &eth_netdev_ops;

    if (GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type)
    {
        GNET_INFO(("unify-rndis gether_setup_name : set rndis headroom\n"));
        net->hard_header_len = ETH_HLEN;
        net->needed_headroom = RNDIS_PACKET_HEADER_LEN;
        net->needed_tailroom = GNET_RNDIS_ALIGN_LEN;
    }

/* Please set these values very carefully, as it would impact the network
 * core process.
 * Current policy:
 *     1 ONLY set this feature to make the clone skb go through the network
 *  stack without re-copying for RNDIS & ECM.
 *     2 For NCM, ONLY set this feature when ncm has enough headroom
 *  reserved or works at the bypass mode.
 */
#ifdef CONFIG_GNET_PREALLOC_RX_MEM
    if ((GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type)
        || (GNET_CDC_TYPE_ECM == dev->ctx.cdc_type))
    {
        GNET_INFO(("unify-rndis-ecm gether_setup_name : gnet_set_rx_clone\n"));
        gnet_set_rx_clone(dev,1);
    }
#endif

    SET_ETHTOOL_OPS(net, &ops);

    /* two kinds of host-initiated state changes:
     *  - iff DATA transfer is active, carrier is "on"
     *  - tx queueing enabled if open *and* carrier is "on"
     */
    netif_carrier_off(net);

    dev->gadget = g;
    SET_NETDEV_DEV(net, &g->dev);
    SET_NETDEV_DEVTYPE(net, &gadget_type);

    snprintf(dev->dev_sysfs_name, sizeof(dev->dev_sysfs_name),"gnet%d",geth_idx);
    dev->dev_attr.show = gnet_show;
    dev->dev_attr.store = gnet_store;
    dev->dev_attr.attr.name = dev->dev_sysfs_name;
    dev->dev_attr.attr.mode = S_IRUGO | S_IWUSR;
    status = device_create_file(&g->dev, &dev->dev_attr);
    if(status < 0)
    {
        dev_dbg(&g->dev, "device_create_file failed, %d\n", status);
        DBG_E(MBB_NET,"device_create_file failed\n");
        free_netdev(net);
        return status;
    }

    if (dev->ctx.cdc_bypass)
    {
        status = 0;    /* no need to register netdev */
    }
    else
    {
        status = register_netdev(net);
        DBG_I(MBB_NET,"register_netdev:%d\n",status);
    }

    if (status < 0) {
        dev_dbg(&g->dev, "register_netdev failed, %d\n", status);
        free_netdev(net);
        (USB_VOID)device_remove_file(&g->dev, &dev->dev_attr);
    } else {
        GNET_INFO(("MAC %pM\n", net->dev_addr));
        GNET_INFO(("HOST MAC %pM\n", dev->host_mac));
        dev->ctx.eth_idx = geth_idx;
        the_dev[geth_idx] = dev;
        geth_idx++;
    }

#if (FEATURE_ON == MBB_FEATURE_FASTIP)
    if (!usb_fastip_handle) 
    {
        DBG_I(MBB_NET, "usb fastip init\n");
        DBG_I(MBB_NET, "usb fastip devname %s\n", net->name);
        usb_fastip_handle = fastip_attach(net->name, &msglevel, fastip_detach_usb, NULL);
        if (usb_fastip_handle)
        {
            DBG_I(MBB_NET, "\n usb_fastip_handle attach ok !!!!!! cih = 0x%x \n", usb_fastip_handle );
        }
        if (fastip_dev_register(usb_fastip_handle, net, FALSE, TRUE) != FASTIP_OK) 
        {
            DBG_I(MBB_NET, "usb fastip_dev_register() failed\n");
        }
        else
        {
            DBG_I(MBB_NET, "usb fastip register ok\n");
        }
    }
#endif /* MBB_FEATURE_FASTIP */
    return status;
}

/**
 * gether_cleanup - remove Ethernet-over-USB device
 * Context: may sleep
 *
 * This is called to free all resources allocated by @gether_setup().
 */
USB_VOID gether_cleanup(USB_VOID)
{
    struct eth_dev *dev = NULL;
    USB_INT32 loop;

    for(loop=0; loop<GNET_USED_NUM; loop++)
    {
        if(!the_dev[loop])
        {
            continue;
        }

        dev = the_dev[loop];

        (USB_VOID)device_remove_file(&dev->gadget->dev, &dev->dev_attr);
        if (!dev->ctx.cdc_bypass)
        {
            unregister_netdev(dev->net);
        }
        else
        {       
            flush_work(&dev->rx_work);
        }

        flush_delayed_work_sync(&dev->work);
        free_netdev(dev->net);

        geth_idx--;
        the_dev[loop] = NULL;
    }

    GNET_BUG(("%s line %d:geth_idx unmatch(%d)!\n", __FUNCTION__,__LINE__,geth_idx),(0 != geth_idx));
    geth_idx = 0;

    return ;
}

/**
 * gether_connect - notify network layer that USB link is active
 * @link: the USB link, set up with endpoints, descriptors matching
 *    current device speed, and any framing wrapper(s) set up.
 * Context: irqs blocked
 *
 * This is called to activate endpoints and let the network layer know
 * the connection is active ("carrier detect").  It may cause the I/O
 * queues to open and start letting network packets flow, but will in
 * any case activate the endpoints so that they respond properly to the
 * USB host.
 *
 * Verify net_device pointer returned using IS_ERR().  If it doesn't
 * indicate some error code (negative errno), ep->driver_data values
 * have been overwritten.
 */
struct net_device *gether_connect(struct gether *link)
{
    struct eth_dev        *dev = the_dev[link->port_no];
    USB_INT32           result = 0;

    if (!dev)
        return ERR_PTR(-EINVAL);

    DBG_T(MBB_NET, "%s\n", __func__);
    dev->ctx.stats.dev_connect++;

    if (GNET_CDC_TYPE_ECM == dev->ctx.cdc_type)
    {
        g_usb_net_ip_align = ECM_IP_ALIGN_LENGTH;
    }
    else
    {
        g_usb_net_ip_align = 0;
    }
  
    link->in_ep->driver_data = dev;
    /* mask the not ready interrupt for usb nic class function driver */
    link->in_ep->masknotready = gnet_get_netmode(dev)?0:1;/* if bypass mode,enable not ready intr. */
    link->in_ep->xfer_cmd_monitor = 
        (GNET_DBG_XFERMON == (gnet_dbg_level & GNET_DBG_XFERMON));

    result = usb_ep_enable(link->in_ep);
    if (result != 0) {
        DBG_I(MBB_NET, "enable %s --> %ld\n",
            link->in_ep->name, result);
        goto fail0;
    }

    link->out_ep->driver_data = dev;
    /* mask the not ready interrupt for usb nic class function driver */
    link->out_ep->enable_xfer_in_progress = 1;
    result = usb_ep_enable(link->out_ep);
    if (result != 0) {
        DBG_I(MBB_NET, "enable %s --> %ld\n",
            link->out_ep->name, result);
        goto fail1;
    }


    if (result == 0)
        result = alloc_requests(dev, link, qlen_rx(dev->gadget, dev->ctx.cdc_type), qlen(dev->gadget));

#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
    if (result == 0)
        result = alloc_rx_mem(dev);
#endif  /* CONFIG_GNET_PREALLOC_RX_MEM */

    if (result == 0) {
        dev->zlp = link->is_zlp_ok;
        DBG_I(MBB_NET, "qlen %ld\n", qlen(dev->gadget));

        dev->header_len = link->header_len;
        dev->unwrap = link->unwrap;

        if ((GNET_CDC_TYPE_RNDIS == dev->ctx.cdc_type) 
          || (GNET_CDC_TYPE_ECM == dev->ctx.cdc_type))
        {
            GNET_INFO(("unify-rndis-ecm %s : set wrap handler\n", __FUNCTION__));
            dev->wrap = eth_gather_pkt;
        }
        else
        {
            GNET_INFO(("unify-ncm-bypass %s : set wrap handler\n", __FUNCTION__));
            dev->wrap = link->wrap;
        }

        dev->handle_rx = link->handle_rx;

        spin_lock(&dev->lock);
        dev->port_usb = link;
        link->ioport = dev;

        if (!dev->ctx.cdc_bypass)
        {
            if (netif_running(dev->net)) {
                if (link->open)
                    link->open(link);
            } else {
                if (link->close)
                    link->close(link);
            }
        }
        
        spin_unlock(&dev->lock);

        if (dev->ctx.cdc_bypass)
        {
            eth_start(dev, GFP_ATOMIC);
        }
        else
        {
            netif_carrier_on(dev->net);
            if (netif_running(dev->net))
                eth_start(dev, GFP_ATOMIC);
        }

    /* on error, disable any endpoints  */
    } else {
        (USB_VOID) usb_ep_disable(link->out_ep);
fail1:
        (USB_VOID) usb_ep_disable(link->in_ep);
    }
fail0:
    /* caller is responsible for cleanup on error */
    if (result < 0)
        return ERR_PTR(result);

    /* init the rx protect */
    gnet_rx_protect_init(dev);

    return dev->net;
}

/**
 * gether_disconnect - notify network layer that USB link is inactive
 * @link: the USB link, on which gether_connect() was called
 * Context: irqs blocked
 *
 * This is called to deactivate endpoints and let the network layer know
 * the connection went inactive ("no carrier").
 *
 * On return, the state is as if gether_connect() had never been called.
 * The endpoints are inactive, and accordingly without active USB I/O.
 * Pointers to endpoint descriptors and endpoint private data are nulled.
 */
USB_VOID gether_disconnect(struct gether *link)
{
    struct eth_dev        *dev = link->ioport;
    struct usb_request    *req;

    WARN_ON(!dev);
    if (!dev)
        return;

    DBG_T(MBB_NET, "%s\n", __func__);

    if (!dev->ctx.cdc_bypass)
    {
        netif_stop_queue(dev->net);
        netif_carrier_off(dev->net);
        dev->ctx.stats.tx_stop_queue++;
    }

    del_timer(&dev->ctx.timer); /* delete timer */
    dev->ctx.stats.dev_disconnect++;

    /* disable endpoints, forcing (synchronous) completion
     * of all pending i/o.  then free the request objects
     * and forget about the endpoints.
     */
    usb_ep_disable(link->in_ep);

    spin_lock(&dev->tx_req_lock);

    while (!list_empty(&dev->tx_reqs)) {
        req = container_of(dev->tx_reqs.next,
                    struct usb_request, list);
        list_del(&req->list);

        spin_unlock(&dev->tx_req_lock);
        if(req->context)
        {
            free_tx_ctx((tx_ctx_t *)req->context);
            req->context = NULL;
        }

        usb_ep_free_request(link->in_ep, req);

        spin_lock(&dev->tx_req_lock);
    }

    spin_unlock(&dev->tx_req_lock);

    /* free the dwc_trb_cache_buf */
    if(dev->ctx.dwc_trb_cache_buf)
    {
        kfree(dev->ctx.dwc_trb_cache_buf);
        dev->ctx.dwc_trb_cache_buf = NULL;
    }
    
    link->in_ep->driver_data = NULL;
    link->in_ep->desc = NULL;

    usb_ep_disable(link->out_ep);
    spin_lock(&dev->req_lock);
    while (!list_empty(&dev->rx_reqs)) {
        req = container_of(dev->rx_reqs.next,
                    struct usb_request, list);
        list_del(&req->list);

        spin_unlock(&dev->req_lock);
        usb_ep_free_request(link->out_ep, req);
        spin_lock(&dev->req_lock);
    }
    spin_unlock(&dev->req_lock);
    link->out_ep->driver_data = NULL;
    link->out_ep->desc = NULL;

    free_rx_mem(dev);

    /* finish forgetting about this USB link episode */
    dev->header_len = 0;
    dev->unwrap = NULL;
    dev->wrap = NULL;
    dev->handle_rx = NULL;

    dev->port_usb = NULL;
    link->ioport = NULL;
    dev->is_suspend = 0;
    spin_unlock(&dev->lock);
}

USB_VOID free_sg_node(sg_node_t *sg_node)
{
    if(sg_node)
    {
        if(sg_node->sg_tbl.sgl)
        {
            sg_free_table(&sg_node->sg_tbl);
        }

        kfree(sg_node);
    }
}

sg_node_t *alloc_sg_node(USB_UINT32 nents, gfp_t gfp_msk)
{
    sg_node_t *sg_node;

    sg_node = (sg_node_t *)kzalloc(sizeof(sg_node_t), gfp_msk);
    if(!sg_node)
    {
        goto fail;
    }

    if(sg_alloc_table(&sg_node->sg_tbl, nents, gfp_msk))
    {
        goto fail;
    }

    INIT_LIST_HEAD(&sg_node->entry);

    return sg_node;

fail:
    free_sg_node(sg_node);

    return NULL;
}

USB_INT gnet_get_sgnode(tx_ctx_t *ctx, USB_UINT32 nents, gfp_t gfp_msk)
{
#if defined(CONFIG_GNET_PREALLOC_TX_MEM)
    return 0;
#else
    sg_node_t *sg_node = NULL;

    sg_node = alloc_sg_node(nents, gfp_msk);
    if(!sg_node)
        return -ENOMEM;

    ctx->sg_node = sg_node;

    return 0;
#endif
}

USB_VOID gnet_put_sgnode(tx_ctx_t *ctx)
{
#if defined(CONFIG_GNET_PREALLOC_TX_MEM)
#else
    if(ctx->sg_node)
    {
        free_sg_node(ctx->sg_node);
        ctx->sg_node = NULL;        
    }
#endif
}

USB_VOID free_tx_ctx(tx_ctx_t *tx_ctx)
{
    if(tx_ctx)
    {
        if(!skb_queue_empty(&tx_ctx->pkt_list))
        {
            gnet_recycle_ctx_info(tx_ctx, 0);
        }
    
#ifdef CONFIG_GNET_PREALLOC_TX_MEM
        if(tx_ctx->sg_node)
        {
            free_sg_node(tx_ctx->sg_node);
            tx_ctx->sg_node = NULL;
        }
#endif

        if ((NULL != tx_ctx->dev) 
          && (GNET_CDC_TYPE_NCM == tx_ctx->dev->ctx.cdc_type))
        {
            GNET_INFO(("unify-ncm gnet_free_tx_ctx free ntb node\n"));
            if(tx_ctx->ntb)
            {
                ncm_free_ntbnode(tx_ctx->ntb);
                tx_ctx->ntb = NULL;
            }
        }
        
        kfree(tx_ctx);
    }
}

USB_INT alloc_tx_ctx(struct eth_dev *dev, USB_UINT32 n)
{
    USB_INT   status = 0;
    tx_ctx_t    *tx_ctx = NULL;
    sg_node_t *sg_node = NULL;
    ntb_node_t *ntb_node = NULL;
    struct usb_request *req,*req_next;
    gfp_t gfp_msk = GFP_ATOMIC;

    if(!dev->ctx.dwc_trb_cache_buf)
    {
        dev->ctx.dwc_trb_cache_buf = kzalloc(1024, gfp_msk);
        if(!dev->ctx.dwc_trb_cache_buf)
        {
            GNET_ERR(("alloc dwc_trb_cache_buf failed\n"));
            return -ENOMEM;
        }
    }

    /* alloc tx ctx for tx req */
    list_for_each_entry_safe(req, req_next, &dev->tx_reqs, list)
    {
        /* alloc tx ctx node */
        tx_ctx = (tx_ctx_t *)kzalloc(sizeof(tx_ctx_t), gfp_msk);
        if(!tx_ctx)
            goto fail;

#ifdef CONFIG_GNET_PREALLOC_TX_MEM
        /* alloc sg node */
        sg_node = alloc_sg_node(dev->ctx.sg_node_nents, gfp_msk);
        if(!sg_node)
        {
            kfree(tx_ctx);
            goto fail;
        }

        tx_ctx->sg_node = sg_node;

        if (GNET_CDC_TYPE_NCM == dev->ctx.cdc_type)  
        {
            /* alloc ntb node */
            ntb_node = ncm_alloc_ntbnode(dev->ctx.ntb_node_size, gfp_msk);
            GNET_INFO(("unify-ncm gnet_alloc_tx_ctx alloc ntb node %p\n", ntb_node));
            if(!ntb_node)
            {
                free_sg_node(sg_node);
                tx_ctx->sg_node = NULL;
                kfree(tx_ctx);            
                goto fail;
            }
            tx_ctx->ntb = ntb_node;
        }
#endif  /* CONFIG_GNET_PREALLOC_TX_MEM */

        skb_queue_head_init(&tx_ctx->pkt_list);
        tx_ctx->dev = dev;
        tx_ctx->req = req;
        req->complete = tx_complete;
        req->context = (USB_VOID *)tx_ctx;
        req->length = 1;    /* dummy, just for usb_gadget_map_request para check */
        req->sg = tx_ctx->sg_node->sg_tbl.sgl;
        if (GNET_CDC_TYPE_ECM == dev->ctx.cdc_type)
        {
            GNET_INFO(("unify-ecm gnet_alloc_tx_ctx set ecm as USB_REQUEST_M_TRANSFER\n"));
            req->sg_mode = USB_REQUEST_M_TRANSFER;  /* For ecm, we treat sg as a special use */
        }
    }

    goto done;

fail:
    /* free all tx ctx */
    list_for_each_entry_safe(req, req_next, &dev->tx_reqs, list)
    {
        if(req->context)
        {
            free_tx_ctx((tx_ctx_t *)req->context);
            req->context = NULL;
        }
    }

    if(dev->ctx.dwc_trb_cache_buf)
    {
        kfree(dev->ctx.dwc_trb_cache_buf);
        dev->ctx.dwc_trb_cache_buf = NULL;
    }

    status = -ENOMEM;
done:
    return status;
}

USB_VOID gnet_recycle_ctx_info(tx_ctx_t *ctx, USB_UINT32 success)
{
    struct eth_dev  *dev = ctx->dev;
    struct net_device *net = dev->net;
    struct sk_buff    *skb_loop = NULL;
    struct sk_buff    *skb_next = NULL;

    if(success)
    {
        dev->ctx.stats.tx_ok_bytes += ctx->pkt_len;
        dev->ctx.stats.tx_ok_pkts += ctx->pkt_cnt;
        net->stats.tx_bytes += ctx->pkt_len;
        net->stats.tx_packets += ctx->pkt_cnt;
    }
    else
    {
        dev->ctx.stats.tx_err_bytes += ctx->pkt_len;
        dev->ctx.stats.tx_err_pkts += ctx->pkt_cnt;
        net->stats.tx_dropped += ctx->pkt_cnt;
    }

    skb_queue_walk_safe(&ctx->pkt_list,skb_loop,skb_next)
    {
        skb_loop = __skb_dequeue(&ctx->pkt_list);
        if(NULL == skb_loop)
        {
            break;
        }
        skb_loop->used_check = 0;
        dev_kfree_skb_any(skb_loop);
    }

    gnet_put_sgnode(ctx);

    if ((NULL != ctx->dev) 
      && (GNET_CDC_TYPE_NCM == ctx->dev->ctx.cdc_type))
    {
        GNET_TRACE(("unify-ncm gnet_recycle_ctx_info put ntb\n"));
        ncm_put_ntbnode(ctx);
    }

    ctx->pkt_cnt = 0;
    ctx->pkt_len = 0;
}

USB_VOID gnet_skb_refresh(struct sk_buff *skb)
{
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
    skb->data = skb->head + USB_NET_IP_ALIGN;
    skb_reset_tail_pointer(skb);
    skb->len = 0;
    skb->cloned = 0;
#endif
}

struct sk_buff *gnet_get_rx_skb(struct eth_dev  *dev, USB_UINT32 size, gfp_t gfp_msk)
{
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
    struct sk_buff *skb = NULL;
    struct sk_buff *skb_next = NULL;
    USB_ULONG       flags;
    USB_UINT32      recycle_cnt = 0;

    spin_lock_irqsave(&dev->ctx.rx_skb_free_q->lock, flags);
    skb = __skb_dequeue(dev->ctx.rx_skb_free_q);
    if(skb) {
        spin_unlock_irqrestore(&dev->ctx.rx_skb_free_q->lock, flags);
        return skb;
    }

    /* we have locked the local irq in rx_skb_free_q.lock */
    spin_lock(&dev->ctx.rx_skb_done_q->lock);
    skb_queue_walk_safe(dev->ctx.rx_skb_done_q, skb, skb_next)
    {
        if(skb_cloned(skb))
        {
            /* contine to recycle skb when no free skb to use
             * in the non-interrupt process context
             */
            if (skb_queue_empty(dev->ctx.rx_skb_free_q) && !in_interrupt()) {
                continue;
            }
            else {
                break;
            }
        }

        __skb_unlink(skb, dev->ctx.rx_skb_done_q);

        gnet_skb_refresh(skb);

        __skb_queue_tail(dev->ctx.rx_skb_free_q, skb);

        if(++recycle_cnt >= dev->ctx.rx_skb_waterline)
        {
            break;
        }
    }
    skb = __skb_dequeue(dev->ctx.rx_skb_free_q);
    spin_unlock(&dev->ctx.rx_skb_done_q->lock);

    spin_unlock_irqrestore(&dev->ctx.rx_skb_free_q->lock, flags);
    return skb;
#else
    return alloc_skb(size + USB_NET_IP_ALIGN, gfp_msk);
#endif
}

USB_VOID gnet_recycle_rx_skb(struct eth_dev  *dev, struct sk_buff *skb)
{
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
    if(skb_cloned(skb))
    {
        dev->ctx.stats.rx_skb_recycle_delay++;
        return;
    }

    gnet_skb_refresh(skb);

    skb_queue_tail(dev->ctx.rx_skb_free_q, skb);
#else
    dev_kfree_skb_any(skb);
#endif
}

USB_VOID gnet_put_rx_skb_to_done(struct eth_dev  *dev, struct sk_buff *skb)
{
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
    skb_queue_tail(dev->ctx.rx_skb_done_q, skb);
#else
#endif
}

USB_VOID free_rx_mem(struct eth_dev *dev)
{
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
    struct sk_buff *skb = NULL, *skb_next = NULL;
    USB_ULONG flags;

    if ((NULL == dev->ctx.rx_skb_done_q) || (NULL == dev->ctx.rx_skb_free_q))
    {
        GNET_ERR(("%s: done_q=%p, free_q=%p\n", __FUNCTION__, 
          dev->ctx.rx_skb_done_q, dev->ctx.rx_skb_free_q));
        return;
    }
    
    spin_lock_irqsave(&dev->ctx.rx_skb_done_q->lock, flags);
    skb_queue_walk_safe(dev->ctx.rx_skb_done_q, skb, skb_next)
    {
        if(skb_cloned(skb))
        {
            GNET_DBG(("%s:skb %08x now cloned, stay in done queue.\n",__FUNCTION__,(u32)skb));
            continue;
        }

        __skb_unlink(skb, dev->ctx.rx_skb_done_q);

        gnet_skb_refresh(skb);

        skb_queue_tail(dev->ctx.rx_skb_free_q, skb);
    }
    spin_unlock_irqrestore(&dev->ctx.rx_skb_done_q->lock, flags);

    //while(NULL != (skb = skb_dequeue(&dev->ctx.rx_skb_free_q)))
        //dev_kfree_skb_any(skb);
#endif

    if (!(gnet_dbg_level & GNET_DBG_RX2ADS))
    {
        struct sk_buff *skb_queued = NULL;
        GNET_INFO(("%s : free %u queued skb.\n", __FUNCTION__, skb_queue_len(&dev->rx_frames_to_ads)));
        while(NULL != (skb_queued = skb_dequeue(&dev->rx_frames_to_ads)))
        {
            dev_kfree_skb_any(skb_queued);
        }
    }
}

static USB_VOID usb_destroy_eth_rx_mem(USB_VOID)
{
    struct sk_buff *skb = NULL;
    
    if(NULL != g_rx_skb_done_q)
    {
        while(NULL != (skb = skb_dequeue(g_rx_skb_done_q)))
        {
            dev_kfree_skb_any(skb);
        }

        kfree(g_rx_skb_done_q);
        g_rx_skb_done_q = NULL;
    }
    
    if(NULL != g_rx_skb_free_q)
    {
        while(NULL != (skb = skb_dequeue(g_rx_skb_free_q)))
        {
            dev_kfree_skb_any(skb);
        }        

        kfree(g_rx_skb_free_q);
        g_rx_skb_free_q = NULL;  
    }    
}

static USB_INT usb_prealloc_eth_rx_mem(struct eth_dev *dev, USB_UINT32 cdc_type_hint)
{
#ifdef CONFIG_GNET_PREALLOC_RX_MEM
    struct sk_buff     *skb = NULL;
    USB_UINT32          cnt = 0;
    static USB_UINT32   cdc_type = 0;
    static USB_UINT32   rx_size = 0;
    static USB_UINT32   rx_num = 0;
    USB_UINT32   set_cdc_type = 0;
    USB_UINT32   rx_skb_size = 0;
    USB_UINT32   rx_skb_num = 0;
    int flags = GFP_ATOMIC;
  
    if (!in_interrupt())
    {
        flags = GFP_KERNEL;
    }
    if (NULL == dev)
    {
        /* 使用网口默认参数申请内存 */
        if (GNET_CDC_TYPE_ECM == cdc_type_hint)
        {
            set_cdc_type = GNET_CDC_TYPE_ECM;
            rx_skb_size = GNET_ECM_OUT_MAXSIZE_PER_TRANSFER;
            rx_skb_num = GNET_RX_MEM_PREALLOC / GNET_ECM_OUT_MAXSIZE_PER_TRANSFER;  
        }
        else if (GNET_CDC_TYPE_RNDIS == cdc_type_hint)
        {
            set_cdc_type = GNET_CDC_TYPE_RNDIS;
            rx_skb_size = GNET_RNDIS_OUT_MAXSIZE_PER_TRANSFER;
            rx_skb_num = GNET_RX_MEM_PREALLOC / GNET_RNDIS_OUT_MAXSIZE_PER_TRANSFER;
        }
        else if (GNET_CDC_TYPE_NCM == cdc_type_hint)
        {
            set_cdc_type = GNET_CDC_TYPE_NCM;
            rx_skb_size = GNET_NCM_OUT_MAXSIZE_PER_TRANSFER;
            rx_skb_num = GNET_RX_MEM_PREALLOC / GNET_NCM_OUT_MAXSIZE_PER_TRANSFER;
        }
        else
        {
            DBG_E(MBB_NET, "%s can not alloc rx mem for type %u\n", __FUNCTION__, cdc_type);
            return -EINVAL;
        }
    }
    else
    {
        set_cdc_type = dev->ctx.cdc_type;
        rx_skb_size = dev->ctx.rx_skb_size;
        rx_skb_num = dev->ctx.rx_skb_num;           
    }
    
    DBG_T(MBB_NET, "type:%lu->%lu, size:%lu->%lu, num:%lu->%lu, ptr=%p\n", 
        cdc_type, set_cdc_type, rx_size, rx_skb_size, rx_num, rx_skb_num, g_rx_skb_free_q);

    /* 申请内存的大小和数量一样，不必重新申请 */
    if ((rx_size == rx_skb_size) && (rx_num == rx_skb_num))
    {
        cdc_type = set_cdc_type;
    }
    
    if (cdc_type != set_cdc_type)
    {
        GNET_INFO(("unify %s, cdc type %lu to %lu \n", __FUNCTION__, cdc_type, set_cdc_type));
        cdc_type = set_cdc_type;
        rx_size = rx_skb_size;
        rx_num = rx_skb_num;
        usb_destroy_eth_rx_mem();
    }
    
    if(!g_rx_skb_free_q)
    {
        g_rx_skb_free_q = kzalloc(sizeof(struct sk_buff_head), flags);
        if(!g_rx_skb_free_q)
        {
            goto nomem;
        }

        skb_queue_head_init(g_rx_skb_free_q);

        DBG_T(MBB_NET, "%s : allocmem num=%lu, size=%lu\n", 
          __FUNCTION__, rx_skb_num, rx_skb_size);
        
        /* prealloc rx skbs */
        for(cnt=0; cnt < rx_skb_num; cnt++)
        {
            skb = alloc_skb(rx_skb_size, flags);
            if(!skb)
            {
                DBG_E(MBB_NET, "%s: fail to alloc skb\n", __FUNCTION__);
                goto nomem;
            }

            skb_queue_tail(g_rx_skb_free_q, skb);
        }        
    }

    if(!g_rx_skb_done_q)
    {
        g_rx_skb_done_q = kzalloc(sizeof(struct sk_buff_head), flags);
        if(!g_rx_skb_done_q)
        {
            goto nomem;
        }

        skb_queue_head_init(g_rx_skb_done_q);
    }
    
    return 0;
        
nomem:

    usb_destroy_eth_rx_mem();

    return -ENOMEM;
#else
    return 0;
#endif
}

USB_INT eth_prealloc_rx_mem(USB_UINT32 cdc_type)
{
    return usb_prealloc_eth_rx_mem(NULL, cdc_type);
}

USB_INT alloc_rx_mem(struct eth_dev *dev)
{
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
    USB_INT ret = 0;
    
    ret = usb_prealloc_eth_rx_mem(dev, 0);
    if(ret)
    {
        return ret;
    }

    dev->ctx.rx_skb_free_q = g_rx_skb_free_q;
    dev->ctx.rx_skb_done_q = g_rx_skb_done_q;
#endif
    return 0;
}

/**
 * make_clone_skb_writable
 * @skb: sk_buff to be written by tcp/ip layer.
 * Context: all
 *
 * This is the clone skb optimazation function routine for NCM/RNDIS/ECM, which revise
 * the skb state to permit the tcp/ip layer modifing the cloned skb data buffer as
 * needed as possible without making another copy of the data buffer.
 *
 * Returns:none
 */

USB_VOID make_clone_skb_writable(struct sk_buff *skb)
{
#if defined(CONFIG_GNET_PREALLOC_RX_MEM)
    skb_header_release(skb);
    /* ip_hdr + ip_options + tcp_hdr  = 80 at most */
    skb->hdr_len = skb_headroom(skb) + 80;
#endif
}

/**
 * gnet_copy_skb
 * @skb: sk_buff to be copied
 * @offset: src offset from the beginning of skb->data
 * @len: total length to be copied
 * context: in irq
 *
 * This is the skb receive copy routine for NCM. According to the current
 * implementation, ncm driver prepares 16KB buffer to receive the
 * aggregated packets, it would be better to clone the skb to the tcp/ip
 * protocol. However, in ip_forward, ip_cow seems to COPY the cloned
 * skb in an unexpected way, so before the clone bug is fixed, we do the
 * copy at the receive routine in advance.
 */
struct sk_buff * gnet_copy_skb(struct sk_buff *skb, USB_INT32 offset, USB_INT32 len)
{
    struct sk_buff *n;
    USB_UINT32      hdrlen = 0;
#ifdef CONFIG_NCM_MBIM_SUPPORT
    ncm_skb_cb_s   *skb_cb = (ncm_skb_cb_s *)skb->cb;
#endif

    if (skb->len <= (len + offset)) {
        GNET_ERR(("====ERROR:%s skb->len=%d should be greater than len=%ld \
            + offset=%ld====\n",__FUNCTION__, skb->len, len, offset));
        return NULL;
    }

#ifdef CONFIG_NCM_MBIM_SUPPORT
    /* add headroom for raw ip packet */
    hdrlen = (NCM_PKT_TYPE_RAW == skb_cb->PktType) ? ETH_HLEN : 0;
#endif
    
    /* reserve 8 bytes tailroom and some headroom*/
    n = dev_alloc_skb(len + 8 + hdrlen);
    if(!n) {
        GNET_ERR(("====WARNING:%s memery not sufficient====\n",__FUNCTION__));
        return n;
    }

    /* Set the tail pointer and length */
    skb_put(n, (len + hdrlen));
#ifdef CONFIG_NCM_MBIM_SUPPORT
    /* Set header */
    if (hdrlen)
    {
        skb_pull(n, hdrlen);
    }
#endif
    /* NO need to copy skb header, as not filled this moment, so just copy the buffer*/
    if (skb_copy_bits(skb, offset, n->data, len)) {
        GNET_ERR(("====WARNING:%s failure to copy skb buff====\n",__FUNCTION__));
        dev_kfree_skb_any(n);
        n = NULL;
    }

    return n;
}
/**
 * gnet_set_rx_clone
 * context: any
 *
 * This function is to be used by ncm to indicate doing the clone when receiving packets.
 * If enough headroom has been reserved when PC sends frames, it would be very
 * efficient to set these features to make the skb go throungh the network
 * stack without doing the buffer copying.
 */
USB_VOID gnet_set_rx_clone(struct eth_dev *dev, USB_UINT32 doclone)
{
    struct net_device *net = dev->net;

    dev->doclone = doclone?1:0;

    if(!dev->doclone)
    {
        net->features &=  ~NETIF_F_NOCLONE_CHECK;
        net->hw_features &= ~NETIF_F_NOCLONE_CHECK;
    }
    else
    {
        net->features |=  NETIF_F_NOCLONE_CHECK;
        net->hw_features |= NETIF_F_NOCLONE_CHECK;
    }
}

/*
os_set_thread_attr
    设置任务调度优先级,调度算法,性能调试使用
pid:
    look for ps cmd
priority:
    0~99
sched:
    SCHED_NORMAL        0
    SCHED_FIFO          1
    SCHED_RR            2
*/
USB_INT32 os_set_thread_attr(USB_INT32 pid, USB_INT32 priority, USB_INT32 sched)
{
    struct sched_param param;
    struct pid *pstPid;
    struct task_struct *pstTask;

    param.sched_priority = priority;
    pstPid = find_get_pid(pid);
    pstTask = pid_task(pstPid, PIDTYPE_PID);
    if (sched > 2 || sched < 0)
    {
        sched = SCHED_FIFO;
    }
    sched_setscheduler(pstTask, sched, &param);
    return 0;
}


USB_INT32 ncm_eth_mem_dump(USB_CHAR *buffer, USB_UINT32 buf_size)
{
    USB_UINT32 need_size;
    struct eth_dev_ctx*  ctx;
    USB_UINT32 i;
    USB_CHAR *cur = buffer;

    /* no devs exist */
    if (0 == geth_idx) {
        return 0;
    }

    need_size = geth_idx * sizeof(struct eth_dev_ctx) + sizeof(USB_INT32);
    /* no room left */
    if (need_size > buf_size) {
        return -1;
    }

    /* record port num */
    *((USB_INT32 *)cur) = geth_idx;
    cur += sizeof(USB_INT32);

    for (i = 0; i < geth_idx && i < GNET_USED_NUM; i++) {
        if (the_dev[i]) {
            ctx = &the_dev[i]->ctx;
            memcpy(cur, ctx, sizeof(struct eth_dev_ctx));
            cur += sizeof(struct eth_dev_ctx);
        }
    }

    return (USB_INT32)need_size;
}

USB_INT32 gnet_eth_xmit_test(USB_UINT32 buf_size)
{
    struct sk_buff *skb_tx = NULL;
    struct eth_dev *dev = the_dev[0];

    if ((NULL == dev) || (NULL == dev->net) 
      || (USB_TEST_PKT_LEN_MIN > buf_size) || (USB_TEST_PKT_LEN_MAX < buf_size))
    {
        DBG_E(MBB_NET, "wrong param\n");
        return -1;
    }
    
    skb_tx = dev_alloc_skb(buf_size);
    if (NULL == skb_tx)
    {
        DBG_E(MBB_NET, "alloc skb failed\n");
        return -1;
    }
    
    skb_put(skb_tx, buf_size);
    memset(skb_tx->data, USB_NUM_CC, buf_size);  

    return (USB_INT32)eth_start_xmit(skb_tx, dev->net);
}

