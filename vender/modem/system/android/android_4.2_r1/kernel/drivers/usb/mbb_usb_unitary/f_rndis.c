/*
 * f_rndis.c -- RNDIS link function driver
 *
 * Copyright (C) 2003-2005,2008 David Brownell
 * Copyright (C) 2003-2004 Robert Schwebel, Benedikt Spranger
 * Copyright (C) 2008 Nokia Corporation
 * Copyright (C) 2009 Samsung Electronics
 *                    Author: Michal Nazarewicz (mina86@mina86.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
/**************************************************************************************************
    问题单号          时间           修改人                      说明
***************************************************************************************************/

/* #define VERBOSE_DEBUG */

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/etherdevice.h>

#include <linux/atomic.h>

#include "rndis.h"
#include "adapt/hw_net_dev.h"
#include "hw_pnp.h"
/*
 * This function is an RNDIS Ethernet port -- a Microsoft protocol that's
 * been promoted instead of the standard CDC Ethernet.  The published RNDIS
 * spec is ambiguous, incomplete, and needlessly complex.  Variants such as
 * ActiveSync have even worse status in terms of specification.
 *
 * In short:  it's a protocol controlled by (and for) Microsoft, not for an
 * Open ecosystem or markets.  Linux supports it *only* because Microsoft
 * doesn't support the CDC Ethernet standard.
 *
 * The RNDIS data transfer model is complex, with multiple Ethernet packets
 * per USB message, and out of band data.  The control model is built around
 * what's essentially an "RNDIS RPC" protocol.  It's all wrapped in a CDC ACM
 * (modem, not Ethernet) veneer, with those ACM descriptors being entirely
 * useless (they're ignored).  RNDIS expects to be the only function in its
 * configuration, so it's no real help if you need composite devices; and
 * it expects to be the first configuration too.
 *
 * There is a single technical advantage of RNDIS over CDC Ethernet, if you
 * discount the fluff that its RPC can be made to deliver: it doesn't need
 * a NOP altsetting for the data interface.  That lets it work on some of the
 * "so smart it's stupid" hardware which takes over configuration changes
 * from the software, and adds restrictions like "no altsettings".
 *
 * Unfortunately MSFT's RNDIS drivers are buggy.  They hang or oops, and
 * have all sorts of contrary-to-specification oddities that can prevent
 * them from working sanely.  Since bugfixes (or accurate specs, letting
 * Linux work around those bugs) are unlikely to ever come from MSFT, you
 * may want to avoid using RNDIS on purely operational grounds.
 *
 * Omissions from the RNDIS 1.0 specification include:
 *
 *   - Power management ... references data that's scattered around lots
 *     of other documentation, which is incorrect/incomplete there too.
 *
 *   - There are various undocumented protocol requirements, like the need
 *     to send garbage in some control-OUT messages.
 *
 *   - MS-Windows drivers sometimes emit undocumented requests.
 */

struct f_rndis {
    struct gether            port;
    USB_UINT8                ctrl_id, data_id;
    USB_UINT8                ethaddr[ETH_ALEN];
    USB_UINT32               vendorID;
    const USB_CHAR          *manufacturer;
    USB_INT32                config;

    struct usb_ep           *notify;
    struct usb_request      *notify_req;
    atomic_t                 notify_count;
    USB_UINT32               curr_alt;
};

static inline struct f_rndis *func_to_rndis(struct usb_function *f)
{
    return container_of(f, struct f_rndis, port.func);
}

/* peak (theoretical) bulk transfer rate in bits-per-second */
static USB_UINT32 bitrate(struct usb_gadget *g)
{
    USB_UINT32 speed = eth_get_net_speed();
    if (0 < speed)
    {
        return speed;
    }
    return 300 * 1000 * 1000;   /* V7R2的理论峰值速率为300M */
}

/*-------------------------------------------------------------------------*/

/*
 */

#define LOG2_STATUS_INTERVAL_MSEC    5    /* 1 << 5 == 32 msec */
#define STATUS_BYTECOUNT             8    /* 8 bytes data */

/* interface descriptor: */
static struct usb_interface_descriptor rndis_control_intf = {
    .bLength =        sizeof rndis_control_intf,
    .bDescriptorType =    USB_DT_INTERFACE,

    /* .bInterfaceNumber = DYNAMIC */
    /* status endpoint is optional; this could be patched later */
    .bNumEndpoints =    1,
    .bInterfaceClass =    0xe0,
    .bInterfaceSubClass =   0x01,
    .bInterfaceProtocol =   0x03,
    /* .iInterface = DYNAMIC */
};

static struct usb_cdc_header_desc header_desc = {
    .bLength =        sizeof header_desc,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubType =    USB_CDC_HEADER_TYPE,

    .bcdCDC =        cpu_to_le16(0x0110),
};

static struct usb_cdc_call_mgmt_descriptor call_mgmt_descriptor = {
    .bLength =        sizeof call_mgmt_descriptor,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubType =    USB_CDC_CALL_MANAGEMENT_TYPE,

    .bmCapabilities =    0x00,
    .bDataInterface =    0x01,
};

static struct usb_cdc_acm_descriptor rndis_acm_descriptor = {
    .bLength =        sizeof rndis_acm_descriptor,
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubType =    USB_CDC_ACM_TYPE,

    .bmCapabilities =    0x00,
};

static struct usb_cdc_union_desc rndis_union_desc = {
    .bLength =        sizeof(rndis_union_desc),
    .bDescriptorType =    USB_DT_CS_INTERFACE,
    .bDescriptorSubType =    USB_CDC_UNION_TYPE,
    /* .bMasterInterface0 =    DYNAMIC */
    /* .bSlaveInterface0 =    DYNAMIC */
};

/* the data interface has two bulk endpoints */

static struct usb_interface_descriptor rndis_data_intf = {
    .bLength =        sizeof rndis_data_intf,
    .bDescriptorType =    USB_DT_INTERFACE,

    /* .bInterfaceNumber = DYNAMIC */
    .bNumEndpoints =    2,
    .bInterfaceClass =    USB_CLASS_CDC_DATA,
    .bInterfaceSubClass =    0,
    .bInterfaceProtocol =    0,
    /* .iInterface = DYNAMIC */
};


static struct usb_interface_assoc_descriptor
rndis_iad_descriptor = {
    .bLength =        sizeof rndis_iad_descriptor,
    .bDescriptorType =    USB_DT_INTERFACE_ASSOCIATION,

    .bFirstInterface =    0, /* XXX, hardcoded */
    .bInterfaceCount =     2,    // control + data
    .bFunctionClass =    0xe0,
    .bFunctionSubClass =    0x01,
    .bFunctionProtocol =    0x03,
    /* .iFunction = DYNAMIC */
};

/* full speed support: */

static struct usb_endpoint_descriptor fs_notify_desc = {
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =    cpu_to_le16(STATUS_BYTECOUNT),
    .bInterval =        1 << LOG2_STATUS_INTERVAL_MSEC,
};

static struct usb_endpoint_descriptor fs_in_desc = {
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor fs_out_desc = {
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_OUT,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
};

static struct usb_descriptor_header *eth_fs_function[] = {
    (struct usb_descriptor_header *) &rndis_iad_descriptor,

    /* control interface matches ACM, not Ethernet */
    (struct usb_descriptor_header *) &rndis_control_intf,
    (struct usb_descriptor_header *) &header_desc,
    (struct usb_descriptor_header *) &call_mgmt_descriptor,
    (struct usb_descriptor_header *) &rndis_acm_descriptor,
    (struct usb_descriptor_header *) &rndis_union_desc,
    (struct usb_descriptor_header *) &fs_notify_desc,

    /* data interface has no altsetting */
    (struct usb_descriptor_header *) &rndis_data_intf,
    (struct usb_descriptor_header *) &fs_in_desc,
    (struct usb_descriptor_header *) &fs_out_desc,
    NULL,
};

/* high speed support: */

static struct usb_endpoint_descriptor hs_notify_desc = {
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =    cpu_to_le16(STATUS_BYTECOUNT),
    .bInterval =        LOG2_STATUS_INTERVAL_MSEC,
};

static struct usb_endpoint_descriptor hs_in_desc = {
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =    cpu_to_le16(512),
};

static struct usb_endpoint_descriptor hs_out_desc = {
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_OUT,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =    cpu_to_le16(512),
};

static struct usb_descriptor_header *eth_hs_function[] = {
    (struct usb_descriptor_header *) &rndis_iad_descriptor,

    /* control interface matches ACM, not Ethernet */
    (struct usb_descriptor_header *) &rndis_control_intf,
    (struct usb_descriptor_header *) &header_desc,
    (struct usb_descriptor_header *) &call_mgmt_descriptor,
    (struct usb_descriptor_header *) &rndis_acm_descriptor,
    (struct usb_descriptor_header *) &rndis_union_desc,
    (struct usb_descriptor_header *) &hs_notify_desc,

    /* data interface has no altsetting */
    (struct usb_descriptor_header *) &rndis_data_intf,
    (struct usb_descriptor_header *) &hs_in_desc,
    (struct usb_descriptor_header *) &hs_out_desc,
    NULL,
};

/* super speed support: */

static struct usb_endpoint_descriptor ss_notify_desc = {
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =    cpu_to_le16(STATUS_BYTECOUNT),
    .bInterval =        LOG2_STATUS_INTERVAL_MSEC,
};

static struct usb_ss_ep_comp_descriptor ss_intr_comp_desc = {
    .bLength =        sizeof ss_intr_comp_desc,
    .bDescriptorType =    USB_DT_SS_ENDPOINT_COMP,

    /* the following 3 values can be tweaked if necessary */
    /* .bMaxBurst =        0, */
    /* .bmAttributes =    0, */
    .wBytesPerInterval =    cpu_to_le16(STATUS_BYTECOUNT),
};

static struct usb_endpoint_descriptor ss_in_desc = {
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_IN,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =    cpu_to_le16(1024),
};

static struct usb_endpoint_descriptor ss_out_desc = {
    .bLength =        USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =    USB_DT_ENDPOINT,

    .bEndpointAddress =    USB_DIR_OUT,
    .bmAttributes =        USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =    cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor ss_bulk_comp_desc = {
    .bLength =        sizeof ss_bulk_comp_desc,
    .bDescriptorType =    USB_DT_SS_ENDPOINT_COMP,

    /* the following 2 values can be tweaked if necessary */
    /* .bMaxBurst =        0, */
    /* .bmAttributes =    0, */
};

static struct usb_descriptor_header *eth_ss_function[] = {
    (struct usb_descriptor_header *) &rndis_iad_descriptor,

    /* control interface matches ACM, not Ethernet */
    (struct usb_descriptor_header *) &rndis_control_intf,
    (struct usb_descriptor_header *) &header_desc,
    (struct usb_descriptor_header *) &call_mgmt_descriptor,
    (struct usb_descriptor_header *) &rndis_acm_descriptor,
    (struct usb_descriptor_header *) &rndis_union_desc,
    (struct usb_descriptor_header *) &ss_notify_desc,
    (struct usb_descriptor_header *) &ss_intr_comp_desc,

    /* data interface has no altsetting */
    (struct usb_descriptor_header *) &rndis_data_intf,
    (struct usb_descriptor_header *) &ss_in_desc,
    (struct usb_descriptor_header *) &ss_bulk_comp_desc,
    (struct usb_descriptor_header *) &ss_out_desc,
    (struct usb_descriptor_header *) &ss_bulk_comp_desc,
    NULL,
};

/* string descriptors: */

static struct usb_string rndis_string_defs[] = {
    [0].s = "RNDIS Communications Control",
    [1].s = "RNDIS Ethernet Data",
    [2].s = "RNDIS",
    {  } /* end of list */
};

static struct usb_gadget_strings rndis_string_table = {
    .language =        0x0409,    /* en-us */
    .strings =        rndis_string_defs,
};

static struct usb_gadget_strings *rndis_strings[] = {
    &rndis_string_table,
    NULL,
};

/*-------------------------------------------------------------------------*/
static struct sk_buff *rndis_wrap_single(struct gether *port,
                    struct sk_buff *skb)
{
    struct sk_buff *skb2;
    USB_INT32 pad_len;
    GNET_TRACE(("%s:enter\n", __FUNCTION__));
    GNET_TRACE(("%s:skb_headroom=%d\n", __FUNCTION__, skb_headroom(skb)));

    /* fix me: do we need to check skb clone? */
    if (skb_headroom(skb) <= RNDIS_PACKET_HEADER_LEN)
    {
        struct eth_dev *dev = port->ioport;
        dev->ctx.stats.tx_skb_realloc++;
        GNET_TRACE(("%s:skb_headroom: %d not enough,HEADER_LEN:%d, skb_pad:%d\n", __FUNCTION__, skb_headroom(skb), RNDIS_PACKET_HEADER_LEN, NET_SKB_PAD));
        skb2 = skb_realloc_headroom(skb, sizeof(struct rndis_packet_msg_type));
        dev_kfree_skb_any(skb);
        skb = skb2;
    }

    if (skb)
    {
        /* take into account the RNDIS Header when counting the pad */
        pad_len = ALIGN(skb->len + RNDIS_PACKET_HEADER_LEN, GNET_RNDIS_ALIGN_LEN) - skb->len - RNDIS_PACKET_HEADER_LEN;

        if(pad_len > 0)
        {
            if(skb_pad(skb, pad_len))
            {
                /* got error here; */
                return NULL;
            }
            skb_put(skb, pad_len);
        }
        /* add the rndis header */
        rndis_add_hdr(skb,pad_len);

        GNET_TRACE(("%s:pad_len=%ld,tail_room=%d, skb len=%d\n", __FUNCTION__, pad_len,skb_tailroom(skb),skb->len));
    }

    return skb;


}

#ifdef CONFIG_GNET_PREALLOC_RX_MEM
static USB_INT32 rndis_unwrap_array(struct gether *port,
              struct sk_buff *skb,
              struct sk_buff_head *list)
{
    USB_INT32 total_len;
    USB_INT32 ret = -1;
    USB_INT32 single_len;
    struct sk_buff *skb2;
    total_len = skb->len;

    GNET_TRACE(("%s: 100: enter,total_len=%ld\n", __FUNCTION__,total_len));

    do{
        GNET_TRACE(("%s: 200: big skb len : %d\n", __FUNCTION__, skb->len));

        skb2 = skb_clone(skb, GFP_ATOMIC);

        if(NULL == skb2)
        {
            goto rndis_unwrap_array_err;
        }
        ret = rndis_rm_hdr(port, skb2, list, &single_len);
        if(ret)
        {
            goto rndis_unwrap_array_err;
        }

        total_len -= single_len;

        /* shold be larger than RNDIS Header Len for valid packets */
        if(total_len > RNDIS_PACKET_HEADER_LEN)
        {
            skb_pull(skb,single_len);
        }
        else
        {
            //last packet, leave it
            GNET_TRACE(("%s: 500: last packet\n", __FUNCTION__));
        }
    }while(total_len > 0);


    gnet_put_rx_skb_to_done(port->ioport,skb);

    return ret;

rndis_unwrap_array_err:
    GNET_TRACE(("%s: 900: rndis_unwrap_array_err\n", __FUNCTION__));

    skb_queue_purge(list);

    gnet_recycle_rx_skb(port->ioport, skb);

    return ret;
}
#endif

static USB_VOID rndis_response_available(USB_VOID *_rndis)
{
    struct f_rndis            *rndis = _rndis;
    struct usb_request        *req = rndis->notify_req;
    __le32                 *data = req->buf;
    USB_INT32               status;
    struct eth_dev         *dev = rndis->port.ioport;

    if (atomic_inc_return(&rndis->notify_count) != 1)
        return;

    /* Send RNDIS RESPONSE_AVAILABLE notification; a
     * USB_CDC_NOTIFY_RESPONSE_AVAILABLE "should" work too
     *
     * This is the only notification defined by RNDIS.
     */
    data[0] = cpu_to_le32(1);
    data[1] = cpu_to_le32(0);

    /* check whether the gadget is suspended.
       if so, try to wake up the host */
    if (unlikely(dev->is_suspend)) {
        if (gnet_wakeup_gadget(dev)) {
            /* wakeup failed, reset the notify state */
            atomic_dec(&rndis->notify_count);
            return;
        }
    }

    status = usb_ep_queue(rndis->notify, req, GFP_ATOMIC);
    if (status) {
        atomic_dec(&rndis->notify_count);
        DBG_E(MBB_RNDIS, "notify/0 --> %ld\n", status);
    }
}

static USB_VOID rndis_response_complete(struct usb_ep *ep, struct usb_request *req)
{
    struct f_rndis            *rndis = req->context;
    USB_INT32               status = req->status;

    /* after TX:
     *  - USB_CDC_GET_ENCAPSULATED_RESPONSE (ep0/control)
     *  - RNDIS_RESPONSE_AVAILABLE (status/irq)
     */
    switch (status) {
    case -ECONNRESET:
    case -ESHUTDOWN:
        /* connection gone */
        atomic_set(&rndis->notify_count, 0);
        break;
    default:
        DBG_I(MBB_RNDIS, "RNDIS %s response error %ld, %u/%u\n",
            ep->name, status,
            req->actual, req->length);
        /* FALLTHROUGH */
    case 0:
        if (ep != rndis->notify)
            break;

        /* handle multiple pending RNDIS_RESPONSE_AVAILABLE
         * notifications by resending until we're done
         */
        if (atomic_dec_and_test(&rndis->notify_count))
            break;
        status = usb_ep_queue(rndis->notify, req, GFP_ATOMIC);
        if (status) {
            atomic_dec(&rndis->notify_count);
            DBG_E(MBB_RNDIS, "notify/1 --> %ld\n", status);
        }
        break;
    }
}

static USB_VOID rndis_command_complete(struct usb_ep *ep, struct usb_request *req)
{
    struct f_rndis            *rndis = req->context;
    USB_INT32               status;

    /* received RNDIS command from USB_CDC_SEND_ENCAPSULATED_COMMAND */
//    spin_lock(&dev->lock);
    status = rndis_msg_parser(rndis->config, (USB_UINT8 *) req->buf);
    if (status < 0)
        DBG_E(MBB_RNDIS, "RNDIS command error %ld, %u/%u\n",
            status, req->actual, req->length);
//    spin_unlock(&dev->lock);
}

static USB_INT
rndis_setup(struct usb_function *f, const struct usb_ctrlrequest *ctrl)
{
    struct f_rndis        *rndis = func_to_rndis(f);
    struct usb_composite_dev *cdev = f->config->cdev;
    struct usb_request    *req = cdev->req;
    USB_INT           value = -EOPNOTSUPP;
    USB_UINT16            w_index = le16_to_cpu(ctrl->wIndex);
    USB_UINT16            w_value = le16_to_cpu(ctrl->wValue);
    USB_UINT16            w_length = le16_to_cpu(ctrl->wLength);

    /* composite driver infrastructure handles everything except
     * CDC class messages; interface activation uses set_alt().
     */
    switch ((ctrl->bRequestType << 8) | ctrl->bRequest) {

    /* RNDIS uses the CDC command encapsulation mechanism to implement
     * an RPC scheme, with much getting/setting of attributes by OID.
     */
    case ((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
            | USB_CDC_SEND_ENCAPSULATED_COMMAND:
        if (w_value || w_index != rndis->ctrl_id)
            goto invalid;
        /* read the request; process it later */
        value = w_length;
        req->complete = rndis_command_complete;
        req->context = rndis;
        /* later, rndis_response_available() sends a notification */
        break;

    case ((USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
            | USB_CDC_GET_ENCAPSULATED_RESPONSE:
        if (w_value || w_index != rndis->ctrl_id)
            goto invalid;
        else {
            USB_UINT8 *buf;
            USB_UINT32 n;

            /* return the result */
            buf = rndis_get_next_response(rndis->config, &n);
            if (buf) {
                memcpy(req->buf, buf, n);
                req->complete = rndis_response_complete;
                req->context = rndis;
                rndis_free_response(rndis->config, buf);
                value = n;
            }
            /* else stalls ... spec says to avoid that */
        }
        break;

    default:
invalid:
        DBG_I(MBB_RNDIS, "invalid control req%02x.%02x v%04x i%04x l%d\n",
            ctrl->bRequestType, ctrl->bRequest,
            w_value, w_index, w_length);
    }

    /* respond with data transfer or status phase? */
    if (value >= 0) {
        DBG_I(MBB_RNDIS, "rndis req%02x.%02x v%04x i%04x l%d\n",
            ctrl->bRequestType, ctrl->bRequest,
            w_value, w_index, w_length);
        req->zero = (value < w_length);
        req->length = value;
        value = usb_ep_queue(cdev->gadget->ep0, req, GFP_ATOMIC);
        if (value < 0)
            DBG_E(MBB_RNDIS, "rndis response on err %d\n", value);
    }

    /* device either stalls (value < 0) or reports success */
    return value;
}


static USB_INT rndis_set_alt(struct usb_function *f, USB_UINT intf, USB_UINT alt)
{
    struct f_rndis        *rndis = func_to_rndis(f);
    struct usb_composite_dev *cdev = f->config->cdev;

    DBG_T(MBB_RNDIS, "rndis_set_alt: intf=%u, alt=%u, ctrl_id=%u, data_id=%u\n",
        intf, alt, rndis->ctrl_id, rndis->data_id);
        
    /* we know alt == 0 */

    if (intf == rndis->ctrl_id) {
        if (rndis->notify->driver_data) {
            DBG_E(MBB_NCM, "reset rndis control %u\n", intf);
            usb_ep_disable(rndis->notify);
        }
        if (!rndis->notify->desc) {
            DBG_E(MBB_NCM, "init rndis ctrl %u\n", intf);
            if (config_ep_by_speed(cdev->gadget, f, rndis->notify))
                goto fail;
        }
        (USB_VOID)usb_ep_enable(rndis->notify);
        rndis->notify->driver_data = rndis;

        bsp_usb_set_enum_stat(rndis->data_id, 1);
    } else if (intf == rndis->data_id) {
        struct net_device    *net;

        if (rndis->port.in_ep->driver_data) {
            DBG_I(MBB_RNDIS, "reset rndis\n");
            gether_disconnect(&rndis->port);
        }

        if (!rndis->port.in_ep->desc || !rndis->port.out_ep->desc) {
            DBG_I(MBB_RNDIS, "init rndis\n");
            if (config_ep_by_speed(cdev->gadget, f,
                           rndis->port.in_ep) ||
                config_ep_by_speed(cdev->gadget, f,
                           rndis->port.out_ep)) {
                rndis->port.in_ep->desc = NULL;
                rndis->port.out_ep->desc = NULL;
                goto fail;
            }
        }

        /* Avoid ZLPs; they can be troublesome. */
        rndis->port.is_zlp_ok = false;

        /* RNDIS should be in the "RNDIS uninitialized" state,
         * either never activated or after rndis_uninit().
         *
         * We don't want data to flow here until a nonzero packet
         * filter is set, at which point it enters "RNDIS data
         * initialized" state ... but we do want the endpoints
         * to be activated.  It's a strange little state.
         *
         * REVISIT the RNDIS gadget code has done this wrong for a
         * very long time.  We need another call to the link layer
         * code -- gether_updown(...bool) maybe -- to do it right.
         */
        rndis->port.cdc_filter = 0;

        DBG_I(MBB_RNDIS, "RNDIS RX/TX early activation ... \n");
        net = gether_connect(&rndis->port);
        if (IS_ERR(net))
            return PTR_ERR(net);

        rndis->port.notify = rndis->notify;
                    
        usb_notify_syswatch(DEVICE_ID_USB,USB_ENABLE);
        /*设置连接状态为断开状态*/
        USB_ETH_LinkStatSet( TRUE );
        DBG_I(MBB_RNDIS, "U_RNDIS:link stat =%d\n ",USB_ETH_LinkStatGet());

        rndis_set_param_dev(rndis->config, net,
                &rndis->port.cdc_filter);
    } else
        goto fail;

    /* flag curr_alt 1 if rndis rx/tx activated */
    rndis->curr_alt = (unsigned)(intf == rndis->data_id);

    return 0;
fail:
    rndis->curr_alt = 0;
    return -EINVAL;
}

static USB_VOID rndis_disable(struct usb_function *f)
{
    struct f_rndis        *rndis = func_to_rndis(f);

    if (!rndis->notify->driver_data)
        return;

    DBG_T(MBB_RNDIS, "%s\n", __func__);

    rndis_uninit(rndis->config);
    gether_disconnect(&rndis->port);

    usb_ep_disable(rndis->notify);
    rndis->notify->driver_data = NULL;

    bsp_usb_set_enum_stat(rndis->data_id, 0);
}

static USB_VOID rndis_suspend(struct usb_function *f)
{
    struct eth_dev *dev = func_to_ethdev(f);
    struct f_rndis *rndis = func_to_rndis(f);

    DBG_T(MBB_RNDIS, "%s\n", __func__);
    
    if ((!rndis->curr_alt) && (!dev)) {
        DBG_I(MBB_RNDIS, "rndis function disabled, skip the rndis adapter suspend process\n");
        return;
    }

    eth_suspend(dev);
}

static USB_VOID rndis_resume(struct usb_function *f)
{
    struct eth_dev *dev = func_to_ethdev(f);
    struct f_rndis *rndis = func_to_rndis(f);

    DBG_T(MBB_RNDIS, "%s\n", __func__);
    
    if ((!rndis->curr_alt) && (!dev)) {
        DBG_I(MBB_RNDIS, "rndis function disabled, skip the rndis adapter resume process\n");
        return;
    }

    eth_resume(dev);
}

/*-------------------------------------------------------------------------*/

/*
 * This isn't quite the same mechanism as CDC Ethernet, since the
 * notification scheme passes less data, but the same set of link
 * states must be tested.  A key difference is that altsettings are
 * not used to tell whether the link should send packets or not.
 */

static USB_VOID rndis_open(struct gether *geth)
{
    struct f_rndis        *rndis = func_to_rndis(&geth->func);
    struct usb_composite_dev *cdev = geth->func.config->cdev;

    DBG_T(MBB_RNDIS, "%s\n", __func__);

    rndis_set_param_medium(rndis->config, NDIS_MEDIUM_802_3,
                bitrate(cdev->gadget) / 100);
    rndis_signal_connect(rndis->config);
}

static USB_VOID rndis_close(struct gether *geth)
{
    struct f_rndis        *rndis = func_to_rndis(&geth->func);

    DBG_T(MBB_RNDIS, "%s\n", __func__);

    rndis_set_param_medium(rndis->config, NDIS_MEDIUM_802_3, 0);
    rndis_signal_disconnect(rndis->config);
}

/*-------------------------------------------------------------------------*/

/* ethernet function driver setup/binding */

static USB_INT
rndis_bind(struct usb_configuration *c, struct usb_function *f)
{
    struct usb_composite_dev *cdev = c->cdev;
    struct f_rndis        *rndis = func_to_rndis(f);
    USB_INT           status;
    struct usb_ep        *ep;

    DBG_T(MBB_RNDIS, "%s\n", __func__);
    
    /* allocate instance-specific interface IDs */
    status = usb_interface_id(c, f);
    if (status < 0)
        goto fail;
    rndis->ctrl_id = status;
    rndis_iad_descriptor.bFirstInterface = status;

    rndis_control_intf.bInterfaceNumber = status;
    rndis_union_desc.bMasterInterface0 = status;

    status = usb_interface_id(c, f);
    if (status < 0)
        goto fail;
    rndis->data_id = status;

    bsp_usb_add_setup_dev((USB_UINT32)status);

    rndis_data_intf.bInterfaceNumber = status;
    rndis_union_desc.bSlaveInterface0 = status;

    status = -ENODEV;

    /* allocate instance-specific endpoints */
    ep = usb_ep_autoconfig(cdev->gadget, &fs_in_desc);
    if (!ep)
        goto fail;
    rndis->port.in_ep = ep;
    ep->driver_data = cdev;    /* claim */

    ep = usb_ep_autoconfig(cdev->gadget, &fs_out_desc);
    if (!ep)
        goto fail;
    rndis->port.out_ep = ep;
    ep->driver_data = cdev;    /* claim */

    /* NOTE:  a status/notification endpoint is, strictly speaking,
     * optional.  We don't treat it that way though!  It's simpler,
     * and some newer profiles don't treat it as optional.
     */
    ep = usb_ep_autoconfig(cdev->gadget, &fs_notify_desc);
    if (!ep)
        goto fail;
    rndis->notify = ep;
    ep->driver_data = cdev;    /* claim */

    status = -ENOMEM;

    /* allocate notification request and buffer */
    rndis->notify_req = usb_ep_alloc_request(ep, GFP_KERNEL);
    if (!rndis->notify_req)
        goto fail;
    rndis->notify_req->buf = kmalloc(STATUS_BYTECOUNT, GFP_KERNEL);
    if (!rndis->notify_req->buf)
        goto fail;
    rndis->notify_req->length = STATUS_BYTECOUNT;
    rndis->notify_req->context = rndis;
    rndis->notify_req->complete = rndis_response_complete;

    /* copy descriptors, and track endpoint copies */
    f->descriptors = usb_copy_descriptors(eth_fs_function);
    if (!f->descriptors)
        goto fail;

    /* support all relevant hardware speeds... we expect that when
     * hardware is dual speed, all bulk-capable endpoints work at
     * both speeds
     */
    if (gadget_is_dualspeed(c->cdev->gadget)) {
        hs_in_desc.bEndpointAddress =
                fs_in_desc.bEndpointAddress;
        hs_out_desc.bEndpointAddress =
                fs_out_desc.bEndpointAddress;
        hs_notify_desc.bEndpointAddress =
                fs_notify_desc.bEndpointAddress;

        /* copy descriptors, and track endpoint copies */
        f->hs_descriptors = usb_copy_descriptors(eth_hs_function);
        if (!f->hs_descriptors)
            goto fail;
    }

    if (gadget_is_superspeed(c->cdev->gadget)) {
        ss_in_desc.bEndpointAddress =
                fs_in_desc.bEndpointAddress;
        ss_out_desc.bEndpointAddress =
                fs_out_desc.bEndpointAddress;
        ss_notify_desc.bEndpointAddress =
                fs_notify_desc.bEndpointAddress;

        /* copy descriptors, and track endpoint copies */
        f->ss_descriptors = usb_copy_descriptors(eth_ss_function);
        if (!f->ss_descriptors)
            goto fail;
    }

    rndis->port.open = rndis_open;
    rndis->port.close = rndis_close;

    status = rndis_register(rndis_response_available, rndis);
    if (status < 0)
        goto fail;
    rndis->config = status;

    rndis_set_param_medium(rndis->config, NDIS_MEDIUM_802_3, 0);
    rndis_set_host_mac(rndis->config, rndis->ethaddr);

    if (rndis->manufacturer && rndis->vendorID &&
            rndis_set_param_vendor(rndis->config, rndis->vendorID,
                           rndis->manufacturer))
        goto fail;

    /* NOTE:  all that is done without knowing or caring about
     * the network link ... which is unavailable to this code
     * until we're activated via set_alt().
     */

    DBG_I(MBB_RNDIS, "RNDIS: %s speed IN/%s OUT/%s NOTIFY/%s\n",
            gadget_is_superspeed(c->cdev->gadget) ? "super" :
            gadget_is_dualspeed(c->cdev->gadget) ? "dual" : "full",
            rndis->port.in_ep->name, rndis->port.out_ep->name,
            rndis->notify->name);
    return 0;

fail:
    if (gadget_is_superspeed(c->cdev->gadget) && f->ss_descriptors)
        usb_free_descriptors(f->ss_descriptors);
    if (gadget_is_dualspeed(c->cdev->gadget) && f->hs_descriptors)
        usb_free_descriptors(f->hs_descriptors);
    if (f->descriptors)
        usb_free_descriptors(f->descriptors);

    if (rndis->notify_req) {
        kfree(rndis->notify_req->buf);
        usb_ep_free_request(rndis->notify, rndis->notify_req);
    }

    /* we might as well release our claims on endpoints */
    if (rndis->notify)
        rndis->notify->driver_data = NULL;
    if (rndis->port.out_ep)
        rndis->port.out_ep->driver_data = NULL;
    if (rndis->port.in_ep)
        rndis->port.in_ep->driver_data = NULL;

    DBG_E(MBB_RNDIS, "%s: can't bind, err %d\n", f->name, status);

    return status;
}

static USB_VOID
rndis_unbind(struct usb_configuration *c, struct usb_function *f)
{
    struct f_rndis        *rndis = func_to_rndis(f);

    DBG_T(MBB_RNDIS, "%s\n", __func__);
    
    rndis_deregister(rndis->config);
    rndis_exit();

    if (gadget_is_superspeed(c->cdev->gadget))
        usb_free_descriptors(f->ss_descriptors);
    if (gadget_is_dualspeed(c->cdev->gadget))
        usb_free_descriptors(f->hs_descriptors);
    usb_free_descriptors(f->descriptors);

    kfree(rndis->notify_req->buf);
    usb_ep_free_request(rndis->notify, rndis->notify_req);

    kfree(rndis);
}

/* Some controllers can't support RNDIS ... */
static inline bool can_support_rndis(struct usb_configuration *c)
{
    /* everything else is *presumably* fine */
    return true;
}

/**
 * rndis_bind_config - add RNDIS network link to a configuration
 * @c: the configuration to support the network link
 * @ethaddr: a buffer in which the ethernet address of the host side
 *    side of the link was recorded
 * Context: single threaded during gadget setup
 *
 * Returns zero on success, else negative errno.
 *
 * Caller must have called @gether_setup().  Caller is also responsible
 * for calling @gether_cleanup() before module unload.
 */
USB_INT
rndis_bind_config(struct usb_configuration *c, USB_UINT8 ethaddr[ETH_ALEN])
{
    DBG_T(MBB_RNDIS, "%s\n", __func__);
    return rndis_bind_config_vendor(c, ethaddr, 0, NULL);
}

static inline USB_VOID rndis_reset_values(struct f_rndis *gadget_rndis)
{
    gadget_rndis->port.fixed_out_len = GNET_RNDIS_OUT_MAXSIZE_PER_TRANSFER;
    gadget_rndis->port.fixed_in_len = GNET_RNDIS_IN_MAXSIZE_PER_TRANSFER;

    /* RNDIS activates when the host changes this filter */
    gadget_rndis->port.cdc_filter = 0;

    /* RNDIS has special (and complex) framing */
    gadget_rndis->port.header_len = sizeof(struct rndis_packet_msg_type);
}


USB_INT
rndis_bind_config_vendor(struct usb_configuration *c, USB_UINT8 ethaddr[ETH_ALEN],
                USB_UINT32 vendorID, const USB_CHAR *manufacturer)
{
    struct f_rndis    *rndis;
    USB_INT      status;

    if (!can_support_rndis(c) || !ethaddr)
        return -EINVAL;

    /* setup RNDIS itself */
    status = rndis_init();
    if (status < 0)
        return status;

    /* maybe allocate device-global string IDs */
    if (rndis_string_defs[0].id == 0) {

        /* control interface label */
        status = usb_string_id(c->cdev);
        if (status < 0)
            return status;
        rndis_string_defs[0].id = status;
        rndis_control_intf.iInterface = status;

        /* data interface label */
        status = usb_string_id(c->cdev);
        if (status < 0)
            return status;
        rndis_string_defs[1].id = status;
        rndis_data_intf.iInterface = status;

        /* IAD iFunction label */
        status = usb_string_id(c->cdev);
        if (status < 0)
            return status;
        rndis_string_defs[2].id = status;
        rndis_iad_descriptor.iFunction = status;
    }

    /* allocate and initialize one new instance */
    status = -ENOMEM;
    rndis = kzalloc(sizeof *rndis, GFP_KERNEL);
    if (!rndis)
        goto fail;

    memcpy(rndis->ethaddr, ethaddr, ETH_ALEN);
    rndis->vendorID = vendorID;
    rndis->manufacturer = manufacturer;

    rndis_reset_values(rndis);
    rndis->port.wrap = rndis_wrap_single;
#ifdef CONFIG_GNET_PREALLOC_RX_MEM
    rndis->port.unwrap = rndis_unwrap_array;
#else
    rndis->port.unwrap = rndis_rm_hdr;
#endif    /* end of CONFIG_GNET_PREALLOC_RX_MEM */

    rndis->port.func.name = "rndis";
    rndis->port.func.strings = rndis_strings;
    /* descriptors are per-instance copies */
    rndis->port.func.bind = rndis_bind;
    rndis->port.func.unbind = rndis_unbind;
    rndis->port.func.set_alt = rndis_set_alt;
    rndis->port.func.setup = rndis_setup;
    rndis->port.func.disable = rndis_disable;

    rndis->port.func.suspend = rndis_suspend;
    rndis->port.func.resume = rndis_resume;

    status = usb_add_function(c, &rndis->port.func);
    if (status) {
        kfree(rndis);
fail:
        rndis_exit();
    }
    return status;
}

struct rndis_function_config
{
    USB_UINT8      ethaddr[ETH_ALEN];
    USB_UINT32     vendorID;
    USB_CHAR       manufacturer[USB_NUM_256];
    /* "Wireless" RNDIS; auto-detected by Windows */
    USB_BOOL    wceis;
};

static USB_INT rndis_function_init(struct android_usb_function* f, struct usb_composite_dev* cdev)
{
    f->config = kzalloc(sizeof(struct rndis_function_config), GFP_KERNEL);
    if (!f->config)
    { 
        return -ENOMEM; 
    }
    return 0;
}

static USB_VOID rndis_function_cleanup(struct android_usb_function* f)
{
    kfree(f->config);
    f->config = NULL;
}

static USB_INT
rndis_function_bind_config(struct android_usb_function* f, struct usb_configuration* c)
{
    USB_INT ret;
    struct rndis_function_config* rndis = f->config;

    if (!rndis)
    {
        DBG_E(MBB_RNDIS, "%s: rndis_pdata\n", __func__);
        return -1;
    }

    DBG_I(MBB_RNDIS, "%s MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
            rndis->ethaddr[USB_NUM_0], rndis->ethaddr[USB_NUM_1], rndis->ethaddr[USB_NUM_2],
            rndis->ethaddr[USB_NUM_3], rndis->ethaddr[USB_NUM_4], rndis->ethaddr[USB_NUM_5]);

    ret = gether_setup_name(c->cdev->gadget, rndis->ethaddr, "rndis");
    if (ret)
    {
        DBG_E(MBB_RNDIS, "%s: gether_setup failed\n", __func__);
        return ret;
    }

    if (rndis->wceis)
    {
        /* "Wireless" RNDIS; auto-detected by Windows */
        rndis_iad_descriptor.bFunctionClass =
            USB_CLASS_WIRELESS_CONTROLLER;
        rndis_iad_descriptor.bFunctionSubClass = USB_NUM_0x01;
        rndis_iad_descriptor.bFunctionProtocol = USB_NUM_0x03;
        rndis_control_intf.bInterfaceClass =
            USB_CLASS_WIRELESS_CONTROLLER;
        rndis_control_intf.bInterfaceSubClass =     USB_NUM_0x01;
        rndis_control_intf.bInterfaceProtocol =     USB_NUM_0x03;
    }

    return rndis_bind_config_vendor(c, rndis->ethaddr, rndis->vendorID,
                                    rndis->manufacturer);
}

static USB_VOID rndis_function_unbind_config(struct android_usb_function* f, struct usb_configuration* c)
{
    /* clear the string id index */
    if (rndis_string_defs[0].id)
    {
        rndis_string_defs[0].id = 0;
        rndis_string_defs[1].id = 0;
        rndis_string_defs[2].id = 0;
    }

    gether_cleanup();
}

static ssize_t rndis_manufacturer_show(struct device* dev, struct device_attribute* attr, USB_CHAR *buf)
{
    struct android_usb_function* f = dev_get_drvdata(dev);
    struct rndis_function_config* config = f->config;
    return snprintf(buf, MAX_SHOW_LEN, "%s\n", config->manufacturer);
}

static ssize_t rndis_manufacturer_store(struct device* dev, struct device_attribute* attr, const USB_CHAR *buf, size_t size)
{
    return size;
}

static DEVICE_ATTR(manufacturer, S_IRUGO | S_IWUSR, rndis_manufacturer_show, rndis_manufacturer_store);

static ssize_t rndis_wceis_show(struct device* dev, struct device_attribute* attr, USB_CHAR *buf)
{
    struct android_usb_function* f = dev_get_drvdata(dev);
    struct rndis_function_config* config = f->config;
    return snprintf(buf, MAX_SHOW_LEN, "%d\n", config->wceis);
}

static ssize_t rndis_wceis_store(struct device* dev, struct device_attribute* attr, const USB_CHAR *buf, size_t size)
{
    struct android_usb_function* f = dev_get_drvdata(dev);
    struct rndis_function_config* config = f->config;
    USB_INT32 value;

    if (sscanf(buf, "%ld", &value) == 1)
    {
        config->wceis = value;
        return size;
    }
    return -EINVAL;
}

static DEVICE_ATTR(wceis, S_IRUGO | S_IWUSR, rndis_wceis_show, rndis_wceis_store);

static ssize_t rndis_ethaddr_show(struct device* dev, struct device_attribute* attr, USB_CHAR *buf)
{
    struct android_usb_function* f = dev_get_drvdata(dev);
    struct rndis_function_config* rndis = f->config;
    return snprintf(buf,MAX_SHOW_LEN, "%02x:%02x:%02x:%02x:%02x:%02x\n",
                   rndis->ethaddr[USB_NUM_0], rndis->ethaddr[USB_NUM_1], rndis->ethaddr[USB_NUM_2],
                   rndis->ethaddr[USB_NUM_3], rndis->ethaddr[USB_NUM_4], rndis->ethaddr[USB_NUM_5]);
}

static ssize_t rndis_ethaddr_store(struct device* dev, struct device_attribute* attr, const USB_CHAR *buf, size_t size)
{

    return size;

}

static DEVICE_ATTR(ethaddr, S_IRUGO | S_IWUSR, rndis_ethaddr_show, rndis_ethaddr_store);

static ssize_t rndis_vendorID_show(struct device* dev, struct device_attribute* attr, USB_CHAR *buf)
{
    struct android_usb_function* f = dev_get_drvdata(dev);
    struct rndis_function_config* config = f->config;
    return snprintf(buf, MAX_SHOW_LEN, "%04lx\n", config->vendorID);
}

static ssize_t rndis_vendorID_store(struct device* dev, struct device_attribute* attr, const USB_CHAR *buf, size_t size)
{
    struct android_usb_function* f = dev_get_drvdata(dev);
    struct rndis_function_config* config = f->config;
    USB_INT32 value;

    if (sscanf(buf, "%04lx", &value) == 1)
    {
        config->vendorID = value;
        return size;
    }
    return -EINVAL;
}

static DEVICE_ATTR(vendorID, S_IRUGO | S_IWUSR, rndis_vendorID_show, rndis_vendorID_store);

static struct device_attribute* rndis_function_attributes[] =
{
    &dev_attr_manufacturer,
    &dev_attr_wceis,
    &dev_attr_ethaddr,
    &dev_attr_vendorID,
    NULL
};

static struct android_usb_function rndis_function =
{
    .name         = "rndis",
    .init            = rndis_function_init,
    .cleanup      = rndis_function_cleanup,
    .bind_config  = rndis_function_bind_config,
    .unbind_config  = rndis_function_unbind_config,
    .attributes     = rndis_function_attributes,
};

USB_VOID usb_rndis_init(USB_VOID)
{
    DBG_I(MBB_RNDIS, "usb_rndis_init enter\n");
    pnp_register_usb_support_function(&rndis_function);
}

