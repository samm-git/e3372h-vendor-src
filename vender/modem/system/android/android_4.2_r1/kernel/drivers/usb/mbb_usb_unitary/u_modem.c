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
/*
 * u_modem.c - utilities for USB gadget "modem port" support
 *
 */
/**************************************************************************************************
    问题单号          时间            修改人                   说明
***************************************************************************************************/

/* #define VERBOSE_DEBUG */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>

#include "drv_acm.h"
#include "drv_udi.h"
#include "u_serial.h"
#include "u_modem.h"
#include "f_acm.h"
#include "usb_debug.h"
#include "usb_platform_comm.h"


/*òààμ?ó?úDè?óè?mbb_usb_adp.h ;by w */
extern struct class *tty_class;


/* cdev driver */
struct u_modem_driver {
	struct kref kref;           /* Reference management */
	struct cdev cdev;
    dev_t dev_no;
    struct module *owner;
	const USB_CHAR *driver_name;
	const USB_CHAR *name;
	USB_INT	name_base;              /* offset of printed name */
	USB_INT	major;                  /* major device number */
	USB_INT	minor_start;            /* start of minor device number */
	USB_INT	num;                    /* number of devices allocated */

    struct workqueue_struct *acm_work_queue;

};

struct gs_acm_modem_sglist_info {
    struct scatterlist *sg_list;
    USB_UINT valid;
	USB_UINT item_num;
	USB_UINT index;
	USB_UINT total_size;
	struct gs_acm_modem_sglist_info *next;
    struct sk_buff_head skb_list;
    struct gs_acm_modem_port *port;
};

struct gs_acm_modem_sglist_ctx {
	struct gs_acm_modem_sglist_info sg_list_array[ACM_MODEM_SG_LIST_NUM];
	USB_UINT submit_pos;
    USB_UINT done_pos;
    USB_UINT item_num_threshold;
    USB_UINT buffer_size_threshold;
    USB_UINT tx_timeout;
    USB_UINT tx_timeout_on;
};

/*
 * The port structure holds info for each port, one for each minor number
 * (and thus for each /dev/ node).
 */
struct gs_acm_modem_port {
	spinlock_t port_lock;               /* guard port_* access */
	struct gserial *port_usb;
	USB_UINT open_count;
	bool openclose;	                    /* open/close in progress */
	USB_UINT port_num;
	wait_queue_head_t close_wait;       /* wait for last close */
    ACM_EVENT_CB_T event_notify_cb;
    USB_UINT16 line_state;
    USB_UINT16 line_state_change;
    USB_INT sr_change;
    USB_INT is_suspend;
    USB_PCHAR in_name;
    USB_PCHAR out_name;
    struct gs_acm_modem_sglist_ctx sglist_ctx;
    bool ps_recv_flow;
    MODEM_MSC_STRU cur_flow_msg;

    USB_CHAR read_domain[4];
	struct list_head read_pool;         /* free read req list */
    struct list_head read_done_queue;   /* done read req list */
    struct list_head read_queue_in_usb; /* reqs in usb core */
    struct sk_buff_head skb_free_list;  /* skb list can be reused */
    struct sk_buff_head skb_done_list;  /* skb list in using */

	USB_INT read_started;
	USB_INT read_allocated;
    USB_INT read_req_enqueued;
    USB_INT read_completed;
    USB_UINT read_req_num;
    USB_UINT read_buf_size;
    struct usb_request	*reading_req;
    wait_queue_head_t read_wait;
    ACM_READ_DONE_CB_T read_done_cb;

    USB_CHAR write_domain[4];
	struct list_head write_pool;        /* free write req list */
    USB_UINT write_req_num;
	USB_INT write_started;
	USB_INT write_allocated;
    USB_INT write_completed;
    wait_queue_head_t write_wait;
    ACM_WRITE_DONE_CB_T write_done_cb;
    ACM_FREE_CB_T write_done_free_cb;
    ACM_MODEM_MSC_READ_CB_T read_sig_cb;
    ACM_MODEM_REL_IND_CB_T rel_ind_cb;

	struct delayed_work	rw_work;
	struct usb_cdc_line_coding port_line_coding;	/* 8-N-1 etc */
    USB_UINT is_realloc;
    wait_queue_head_t realloc_wait;

    USB_CHAR debug_tx_domain[4];
    USB_UINT stat_write_async_call;
    USB_UINT stat_write_submit;
    USB_UINT stat_tx_submit;
    USB_UINT stat_tx_submit_fail;
    USB_UINT stat_write_bytes;
    USB_UINT stat_tx_submit_bytes;
    USB_UINT stat_tx_done;
    USB_UINT stat_tx_done_fail;
    USB_UINT stat_tx_done_bytes;
    USB_UINT stat_tx_no_req;
    USB_UINT stat_tx_suspend;
    USB_UINT stat_tx_fifo_full;
    USB_UINT stat_tx_inv_param;
    USB_UINT stat_tx_disconnect;
    USB_UINT stat_timeout_tx;
    USB_UINT stat_direct_tx;

    USB_CHAR debug_rx_domain[4];
    USB_UINT stat_get_buf_call;
    USB_UINT stat_ret_buf_call;
    USB_UINT stat_rx_submit;
    USB_UINT stat_rx_submit_fail;
    USB_UINT stat_rx_disconnect;
    USB_UINT stat_rx_no_req;
    USB_UINT stat_rx_done;
    USB_UINT stat_rx_done_fail;
    USB_UINT stat_rx_done_bytes;
    USB_UINT stat_rx_done_disconnect;
    USB_UINT stat_rx_done_schdule;
    USB_UINT stat_rx_callback;
    USB_UINT stat_rx_cb_not_start;
    USB_UINT stat_alloc_skb;
    USB_UINT stat_alloc_skb_fail;
    USB_UINT stat_free_skb;

    USB_CHAR debug_port_domain[4];
    USB_UINT stat_port_is_connect;
    USB_UINT stat_port_connect;
    USB_UINT stat_port_disconnect;
    USB_UINT stat_enable_in_fail;
    USB_UINT stat_enable_out_fail;
    USB_UINT stat_notify_sched;
    USB_UINT stat_notify_on_cnt;
    USB_UINT stat_notify_off_cnt;
    USB_UINT stat_read_sig_cnt;
};

struct gs_acm_modem_rw_priv {
    struct list_head list;
    struct usb_request *req;
    USB_PVOID rw_ctx;
    struct kiocb *iocb;
};

static struct acm_modem_port_manager {
    USB_CHAR name_domain[4];
	struct mutex	open_close_lock;			/* protect open/close */
	struct gs_acm_modem_port	*port;
    struct device	*cdev;
} gs_acm_modem_ports[ACM_MDM_COUNT];

static struct gs_acm_evt_manage gs_modem_write_evt_manage;
static struct gs_acm_evt_manage gs_modem_read_evt_manage;
static struct gs_acm_evt_manage gs_modem_sig_stat_evt_manage;


static USB_UINT	gs_acm_modem_n_ports;
static USB_UINT gs_modem_drv_invalid;
static USB_UINT gs_modem_port_num_err;


static struct u_modem_driver *gs_modem_driver;

static USB_VOID gs_acm_modem_free_request(struct usb_ep *ep, struct usb_request	*req);
static struct usb_request* gs_acm_modem_alloc_request(struct usb_ep *ep, USB_UINT buf_size, USB_UINT is_mtrans);
static USB_VOID gs_acm_modem_free_requests(struct usb_ep *ep, struct list_head *head,\
							 USB_INT *allocated, struct gs_acm_modem_port *port);
static USB_VOID gs_acm_modem_write_complete(struct usb_ep *ep, struct usb_request *req);
static USB_VOID gs_acm_modem_read_complete(struct usb_ep *ep, struct usb_request *req);
static struct sk_buff *gs_acm_modem_alloc_skb(struct gs_acm_modem_port *port, USB_UINT size);
static USB_VOID gs_acm_modem_free_skb(struct gs_acm_modem_port *port, struct sk_buff *skb);
static USB_VOID gs_acm_modem_free_skb_list(struct gs_acm_modem_port *port,
                    struct sk_buff_head *skb_list);
/*-------------------------------------------------------------------------*/

/*
 * gs_acm_modem_start_tx
 *
 * This function finds available write requests, calls
 * usb_ep_queue to send the data.
 *
 */
static USB_INT gs_acm_modem_start_tx(struct gs_acm_modem_port *port,
                                 struct gs_acm_modem_sglist_info *sglist_info)
{
	struct list_head *pool = &port->write_pool;
	USB_INT status = 0;
    struct usb_request *req;
    struct gs_acm_modem_rw_priv *write_priv;
    struct scatterlist *sg_list_item;

    while (port->sglist_ctx.done_pos != sglist_info->next->index) {

        if (unlikely(list_empty(pool))) {
            port->stat_tx_no_req++;
            return -EAGAIN;
        }

        if (0 == sglist_info->item_num) {
            break;
        }

        /* get a write req from the write pool */
        req = list_entry(pool->next, struct usb_request, list);
        list_del_init(&req->list);
        port->write_started++;

        /* mark the last sg item */
        sg_list_item = sglist_info->sg_list + sglist_info->item_num - 1;
        sg_mark_end(sg_list_item);

        /* fill the req */
    	req->buf = NULL;
        req->sg = sglist_info->sg_list;
        req->num_sgs = sglist_info->item_num;
        req->length = sglist_info->total_size;
        write_priv = (struct gs_acm_modem_rw_priv *)req->context;
        write_priv->rw_ctx = (void*)sglist_info;

        if (!port->port_usb) {
            status = -ENODEV;
            port->stat_tx_disconnect++;
            goto tx_fail_restore;
        }

        /* check zlp */
        if (req->length % port->port_usb->in->maxpacket == 0) {
            req->zero = 1;
        }
        else {
            req->zero = 0;
        }

        /* drop lock while we call out; the controller driver
		 * may need to call us back (e.g. for disconnect)
		 */
        spin_unlock(&port->port_lock);
    	status = usb_ep_queue(port->port_usb->in, req, GFP_ATOMIC);
    	spin_lock(&port->port_lock);
        if (status) {
    		port->stat_tx_submit_fail++;
    		goto tx_fail_restore;
    	}
        port->stat_tx_submit_bytes += sglist_info->total_size;
        port->stat_tx_submit++;

        /* move the submit pos */
        sglist_info = sglist_info->next;
        port->sglist_ctx.submit_pos = sglist_info->index;
    }

    return 0;

tx_fail_restore:
    list_add_tail(&req->list, pool);
    port->write_started--;
	return status;
}

/*
 * Context: caller owns port_lock, and port_usb is set
 */
static USB_INT gs_acm_modem_start_rx(struct gs_acm_modem_port *port)
{
    struct list_head	*pool = &port->read_pool;
    struct usb_ep		*out = NULL;
    struct usb_request	*req;
    struct gs_acm_modem_rw_priv *rw_priv;
    struct sk_buff      *new_skb;

	if (!port->port_usb) {
        port->stat_rx_disconnect++;
		goto start_rx_ret;
    }

    out = port->port_usb->out;
    
start_rx_beg:
    while (!list_empty(pool)) {
        USB_INT status;

        req = list_entry(pool->next, struct usb_request, list);
        list_del_init(&req->list);
        port->read_started++;

        rw_priv = (struct gs_acm_modem_rw_priv *)req->context;
        /*
         * if req length is 0, it need to alloc a new skb
         * else the req is already alloced, we can reuse the skb
         */
        if (0 == req->length) {
            new_skb = gs_acm_modem_alloc_skb(port, port->read_buf_size);
            if (!new_skb) {
                goto start_rx_ret;
            }
            req->complete = gs_acm_modem_read_complete;
            req->buf = new_skb->data;
            rw_priv->rw_ctx = (void*)new_skb;
        }
        req->length = port->read_buf_size;

        /* drop lock while we call out; the controller driver
         * may need to call us back (e.g. for disconnect)
         */
        spin_unlock(&port->port_lock);
        status = usb_ep_queue(out, req, GFP_ATOMIC);
        spin_lock(&port->port_lock);

        if (status) {
            /* if usb queue fail, we can reuse the skb in the rw_priv */
            list_add(&req->list, pool);
            port->read_started--;
            port->stat_rx_submit_fail++;
			goto start_rx_ret;
		}

        list_add_tail(&rw_priv->list, &port->read_queue_in_usb);
        port->read_req_enqueued++;
        port->stat_rx_submit++;

		/* abort immediately after disconnect */
		if (!port->port_usb) {
            port->stat_rx_disconnect++;
			goto start_rx_ret;
        }
	}

    /* if there are no read req in usb core,
     * get the read done req and submit to usb core
     */
    if (port->port_usb && 0 == port->read_req_enqueued) {
        struct list_head *queue = &port->read_done_queue;

        if (!list_empty(queue)) {
            req = list_entry(queue->prev, struct usb_request, list);
            list_move(&req->list, pool);
            port->read_started--;

            /* go to beginning of the function,
             * re-submit the read req
             */
            port->stat_rx_no_req++;
            goto start_rx_beg;
        }
    }

start_rx_ret:
	return port->read_started;
}

static USB_INT gs_acm_modem_get_read_buf(struct gs_acm_modem_port *port,
                                     ACM_WR_ASYNC_INFO *read_info)
{
    struct list_head *queue = &port->read_done_queue;
    struct usb_request	*req = NULL;
    struct sk_buff *skb;
    USB_INT status;
    USB_ULONG flags;

    spin_lock_irqsave(&port->port_lock, flags);
    if (!list_empty(queue)) {
        req = list_first_entry(queue, struct usb_request, list);
    }

    if (NULL == req) {
        read_info->pBuffer = NULL;
        read_info->u32Size = 0;
        status = -EAGAIN;
    }
    else {
        struct gs_acm_modem_rw_priv *rw_priv;
        rw_priv = (struct gs_acm_modem_rw_priv *)req->context;
        skb = (struct sk_buff *)rw_priv->rw_ctx;
        /* copy skb info to usr */
        skb_put(skb, req->actual);

        read_info->pBuffer = (char*)skb;
        read_info->u32Size = req->actual;

        /* reset the req member and move it to read pool list */
        req->length = 0;
        req->buf = NULL;
        rw_priv->rw_ctx = NULL;
        list_move(&req->list, &port->read_pool);
        port->read_started--;
        status = 0;
    }
    spin_unlock_irqrestore(&port->port_lock, flags);
    return status;
}

static USB_INT gs_acm_modem_ret_read_buf(struct gs_acm_modem_port *port,
                                     ACM_WR_ASYNC_INFO *read_info)
{
    struct sk_buff *free_skb = (struct sk_buff *)read_info->pBuffer;

    gs_acm_modem_free_skb(port, free_skb);
    return 0;
}

static USB_INT gs_acm_modem_write_signal(struct gs_acm_modem_port *port,
                                     MODEM_MSC_STRU* modem_msc)
{
    USB_UINT capability = 0;
    struct gserial	*gser = port->port_usb;

    if (!gser) {
        return -ENODEV;
    }

    if (SIGNALCH == modem_msc->OP_Cts) {
        if (SIGNALNOCH == modem_msc->ucCts) {
            /* disable ps flow flag */
            port->ps_recv_flow = RECV_DISABLE;
        }
        else {
            /* enable ps flow flag */
            port->ps_recv_flow = RECV_ENABLE;
        }
    }

    /* notify flow control to host */
    if (SIGNALCH == modem_msc->OP_Cts) {
        if (SIGNALNOCH == modem_msc->ucCts) {
            /* enable rx flow control */
            gser->flow_control(gser, RECV_DISABLE, SEND_ENABLE);
        }
        else {
            /* disable rx flow control */
            gser->flow_control(gser, RECV_ENABLE, SEND_ENABLE);
        }
    }
    if((SIGNALCH == modem_msc->OP_Ri)
		||(SIGNALCH == modem_msc->OP_Dsr )
		||(SIGNALCH == modem_msc->OP_Dcd )) {

	    if ((SIGNALCH == modem_msc->OP_Ri)
	        && (HIGHLEVEL == modem_msc->ucRi)) {
	        capability |= U_ACM_CTRL_RING;
	    };

	    /* DSR SIGNAL CHANGE */
	    if ((SIGNALCH == modem_msc->OP_Dsr)
	        && (HIGHLEVEL == modem_msc->ucDsr)){
	        capability |= U_ACM_CTRL_RTS;
	    };

	    /* DCD SIGNAL CHANGE */
	    if ((SIGNALCH == modem_msc->OP_Dcd)
	        && (HIGHLEVEL == modem_msc->ucDcd)) {
	        capability |= U_ACM_CTRL_DTR;
	    };

        gser->notify_state(gser, (USB_UINT16)capability);
    }

    return 0;
}


static USB_VOID gs_acm_modem_notify_cb(struct gs_acm_modem_port *port)
{
    ACM_EVENT_CB_T event_cb = NULL;
    ACM_MODEM_MSC_READ_CB_T read_sig_cb = NULL;
    USB_UINT16 line_state;
    bool old_dtr;
    MODEM_MSC_STRU* new_flow_msg = &port->cur_flow_msg;
    USB_ULONG flags;

    spin_lock_irqsave(&port->port_lock, flags);
    line_state = port->line_state;
    old_dtr = port->cur_flow_msg.ucDtr;

    if (port->line_state_change) {
        event_cb = port->event_notify_cb;
        read_sig_cb = port->read_sig_cb;

        if (line_state & U_ACM_CTRL_DTR) {
            new_flow_msg->ucDtr = HIGHLEVEL;
        }
        else {
        	new_flow_msg->ucDtr = LOWLEVEL;
        }

        if (line_state & U_ACM_CTRL_RTS) {
        	new_flow_msg->ucRts = HIGHLEVEL;
        }
        else {
        	new_flow_msg->ucRts = LOWLEVEL;
        }
        port->line_state_change = 0;
    }

    /* if modem is suspended, pull down the DTR signal */
    if (port->sr_change) {
        read_sig_cb = port->read_sig_cb;
        event_cb = port->event_notify_cb;
        new_flow_msg->ucDtr = LOWLEVEL;
        port->sr_change = 0;
        line_state = 0;
    }

    spin_unlock_irqrestore(&port->port_lock, flags);

    if (event_cb) {
        if (line_state) {
            port->stat_notify_on_cnt++;
        }
        else {
            port->stat_notify_off_cnt++;
        }
        event_cb((ACM_EVT_E)(line_state & U_ACM_CTRL_DTR));
    }

    if(old_dtr != new_flow_msg->ucDtr && read_sig_cb) {
        new_flow_msg->OP_Dtr = SIGNALCH;
        port->stat_read_sig_cnt++;
        read_sig_cb(new_flow_msg);
        new_flow_msg->OP_Dtr = SIGNALNOCH;
    }

    return;
}


static USB_VOID gs_acm_modem_read_cb(struct gs_acm_modem_port *port)
{
    struct list_head *queue = &port->read_done_queue;
    struct usb_request	*req = NULL;
    ACM_READ_DONE_CB_T read_cb = NULL;
    USB_ULONG flags;
    USB_INT status = 0;

    for (;;) {
        spin_lock_irqsave(&port->port_lock, flags);
        if (port->read_completed <= 0) {
            spin_unlock_irqrestore(&port->port_lock, flags);
            return;
        }
        port->read_completed--;

        if (!list_empty(queue)) {
            req = list_first_entry(queue, struct usb_request, list);
            status= req->status;

            /* if there are data in queue, prepare the read callback */
            if (!req->status && port->open_count) {
                read_cb = port->read_done_cb;
            }
            else {
                list_move(&req->list, &port->read_pool);
                port->read_started--;
            }
        }

        /* submit the next read req */
        if (-ESHUTDOWN != status && port->port_usb) {
		    gs_acm_modem_start_rx(port);
        }
        else {
            port->stat_rx_cb_not_start++;
        }
        spin_unlock_irqrestore(&port->port_lock, flags);

        if (NULL != read_cb) {
            port->stat_rx_callback++;
            read_cb();
        }
    }
}

static USB_VOID gs_acm_modem_timeout_tx_cb(struct gs_acm_modem_port *port)
{
    struct gs_acm_modem_sglist_info *sglist_info;
    USB_ULONG flags;

    spin_lock_irqsave(&port->port_lock, flags);
    port->stat_timeout_tx++;
    sglist_info = &port->sglist_ctx.sg_list_array[port->sglist_ctx.submit_pos];
    (USB_VOID)gs_acm_modem_start_tx(port, sglist_info);
    port->sglist_ctx.tx_timeout_on = 0;
    spin_unlock_irqrestore(&port->port_lock, flags);
}

/*
 * rw workqueue takes data out of the RX queue and hands it up to the TTY
 * layer until it refuses to take any more data (or is throttled back).
 * Then it issues reads for any further data.
 */
static USB_VOID gs_acm_modem_rw_push(struct work_struct *work)
{
	struct gs_acm_modem_port *port;

    /* notify callback */
    while (NULL != (port = gs_acm_evt_get(&gs_modem_sig_stat_evt_manage))) {
        gs_acm_modem_notify_cb(port);
    }

    /* read callback */
    while (NULL != (port = gs_acm_evt_get(&gs_modem_read_evt_manage))) {
        gs_acm_modem_read_cb(port);
    }

    /* timeout tx callback */
    while (NULL != (port = gs_acm_evt_get(&gs_modem_write_evt_manage))) {
        gs_acm_modem_timeout_tx_cb(port);
    }

    /* other callback ... */

    return;
}

static USB_VOID gs_acm_modem_read_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct gs_acm_modem_port	*port = ep->driver_data;
    struct gs_acm_modem_rw_priv *rw_priv;

	/* Queue all received data until the tty layer is ready for it. */
	spin_lock(&port->port_lock);

    if (!req->status) {
        port->stat_rx_done++;
        port->stat_rx_done_bytes += req->actual;
    }
    else {
        port->stat_rx_done_fail++;
    }

    rw_priv = (struct gs_acm_modem_rw_priv *)req->context;
    list_del_init(&rw_priv->list);
    port->read_req_enqueued--;

    if (port->port_usb && req->actual && !req->status) {
    	list_add_tail(&req->list, &port->read_done_queue);
        port->stat_rx_done_schdule++;
        port->read_completed++;
        gs_acm_evt_push(port, &gs_modem_read_evt_manage);
        queue_delayed_work(gs_modem_driver->acm_work_queue, &port->rw_work, 0);
    }
    else {
        gs_acm_modem_free_skb(port, (struct sk_buff *)rw_priv->rw_ctx);
        req->length = 0;
        req->buf = NULL;
        list_add_tail(&req->list, &port->read_pool);

        port->read_started--;
        port->stat_rx_done_disconnect++;
    }
	spin_unlock(&port->port_lock);
}

static USB_VOID gs_acm_modem_write_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct gs_acm_modem_port	*port = ep->driver_data;
    struct gs_acm_modem_rw_priv *write_priv;
    struct gs_acm_modem_sglist_info *sglist_info;

	spin_lock(&port->port_lock);
    write_priv = (struct gs_acm_modem_rw_priv *)req->context;
    sglist_info = (struct gs_acm_modem_sglist_info *)write_priv->rw_ctx;

    if (!req->status) {
        port->stat_tx_done++;
        port->stat_tx_done_bytes += req->actual;
    }
    else {
        port->stat_tx_done_fail++;
    }

    port->write_completed++;

    gs_acm_modem_free_skb_list(port, &sglist_info->skb_list);

    sglist_info->item_num = 0;
    sglist_info->total_size = 0;

    /* move the done pos */
    port->sglist_ctx.done_pos = sglist_info->index;

    list_add_tail(&req->list, &port->write_pool);
    port->write_started--;

	spin_unlock(&port->port_lock);
}

/* ---------------- modem skb allocation ---------------- */

static struct sk_buff *
gs_acm_modem_alloc_skb(struct gs_acm_modem_port *port, USB_UINT size)
{
    struct sk_buff *new_skb;

    new_skb = dev_alloc_skb(size);
    if (unlikely(!new_skb)) {
        port->stat_alloc_skb_fail++;
    }

    return new_skb;
}

static USB_VOID
gs_acm_modem_free_skb(struct gs_acm_modem_port *port, struct sk_buff *skb)
{
    dev_kfree_skb_any(skb);
}

static USB_VOID gs_acm_modem_free_skb_list(struct gs_acm_modem_port *port,
                    struct sk_buff_head *skb_list)
{
    struct sk_buff *skb, *skbnext;

    skb_queue_walk_safe(skb_list, skb, skbnext) {
        gs_acm_modem_free_skb(port, skb);
    }
    __skb_queue_head_init(skb_list);
}

/* ---------------- modem req managemenet --------------- */
static USB_VOID gs_acm_modem_free_request(struct usb_ep *ep, struct usb_request	*req)
{
    if (req->context) {
        kfree(req->context);
        req->context = NULL;
    }
	gs_free_req(ep, req);
}

static USB_VOID gs_acm_modem_free_requests(struct usb_ep *ep, struct list_head *head,
							 USB_INT *allocated, struct gs_acm_modem_port *port)
{
	struct usb_request	*req;
	struct gs_acm_modem_rw_priv *rw_priv;

	while (!list_empty(head)) {
		req = list_entry(head->next, struct usb_request, list);
		list_del_init(&req->list);

        rw_priv = (struct gs_acm_modem_rw_priv *)req->context;
        if (req->buf) {
    		gs_acm_modem_free_skb(port, (struct sk_buff *)rw_priv->rw_ctx);
            req->length = 0;
            req->buf = NULL;
        }
        gs_acm_modem_free_request(ep, req);
		if (allocated)
			(*allocated)--;
	}
}

static struct usb_request*
gs_acm_modem_alloc_request(struct usb_ep *ep, USB_UINT buf_size, USB_UINT is_mtrans)
{
    struct gs_acm_modem_rw_priv *rw_priv;
    struct usb_request	*req;

    req = gs_alloc_req(ep, buf_size, GFP_ATOMIC);

    if (!req) {
        DBG_E(MBB_ACM,"=================== gs_alloc_req fail, line:%d\n", __LINE__);
		return NULL;
    }

    rw_priv = (struct gs_acm_modem_rw_priv *)
              kzalloc(sizeof(struct gs_acm_modem_rw_priv), GFP_ATOMIC);
    if (!rw_priv) {
        DBG_E(MBB_ACM,"==================== kzalloc fail, line:%d\n", __LINE__);
        gs_free_req(ep, req);
        return NULL;
    }
    req->context = (void*)rw_priv;
    if (is_mtrans) {
        req->sg_mode = USB_REQUEST_M_TRANSFER;
    }
    rw_priv->req = req;
    INIT_LIST_HEAD(&rw_priv->list);

    return req;
}

static USB_INT gs_acm_modem_alloc_requests(struct usb_ep *ep, struct list_head *head,
		USB_VOID (*fn)(struct usb_ep *, struct usb_request *),
		USB_INT *allocated, USB_UINT buf_size, USB_UINT buf_num, USB_UINT is_mtrans)
{
	USB_INT			i;
	struct usb_request	*req;

	USB_INT n = allocated ? buf_num - *allocated : buf_num;

	for (i = 0; i < n; i++) {

        req = gs_acm_modem_alloc_request(ep, buf_size, is_mtrans);
        if (!req)
            return list_empty(head) ? -ENOMEM : 0;
		req->complete = fn;
		list_add_tail(&req->list, head);
		if (allocated)
			(*allocated)++;
	}
	return 0;
}

/*
 * Context: holding port_lock;
 */
static USB_INT gs_acm_modem_prepare_io(struct gs_acm_modem_port *port)
{
    struct list_head	*head = &port->read_pool;
	struct usb_ep		*ep = port->port_usb->out;
	USB_INT			status;

    /* out ep use block buffer */
    status = gs_acm_modem_alloc_requests(ep, head, gs_acm_modem_read_complete,
		&port->read_allocated, 0, port->read_req_num, 0);
	if (status)
		return status;

    /* in ep use sg_list multitranfer mode */
	status = gs_acm_modem_alloc_requests(port->port_usb->in, &port->write_pool,
			gs_acm_modem_write_complete, &port->write_allocated,
			0, port->write_req_num, 0);
	if (status) {
		gs_acm_modem_free_requests(ep, head, &port->read_allocated, port);
		return status;
	}

    return 0;
}

/*-------------------------------------------------------------------------*/

static inline USB_INT gs_acm_modem_get_port_num(struct inode *inode)
{
    USB_INT		port_num;

    if (!gs_modem_driver) {
        gs_modem_drv_invalid++;
		return -ENXIO;
    }

    port_num = inode->i_rdev - gs_modem_driver->dev_no;

    if (port_num >= gs_acm_modem_n_ports) {
        gs_modem_port_num_err++;
		return -ENXIO;
    }

    return port_num;
}

static USB_INT gs_acm_modem_open(struct inode *inode, struct file *filp)
{
	USB_INT	port_num;
	struct gs_acm_modem_port	*port;
	USB_INT	status;
    USB_ULONG flags;

    port_num = gs_acm_modem_get_port_num(inode);
    if (port_num < 0) {
        return port_num;
    }

	do {
		mutex_lock(&gs_acm_modem_ports[port_num].open_close_lock);
		port = gs_acm_modem_ports[port_num].port;
		if (!port) {
		    mutex_unlock(&gs_acm_modem_ports[port_num].open_close_lock);
			return -ENODEV;
		}
		else {
			spin_lock_irqsave(&port->port_lock, flags);

			/* already open?  Great. */
			if (port->open_count) {
				status = 0;
				port->open_count++;

			/* currently opening/closing? wait ... */
			} else if (port->openclose) {
				status = -EBUSY;

			/* ... else we do the work */
			} else {
				status = -EAGAIN;
				port->openclose = true;
			}
			spin_unlock_irqrestore(&port->port_lock, flags);
		}
		mutex_unlock(&gs_acm_modem_ports[port_num].open_close_lock);

		switch (status) {
		default:
			/* fully handled */
			return status;
		case -EAGAIN:
			/* must do the work */
			break;
		case -EBUSY:
			/* wait for EAGAIN task to finish */
			msleep(10);
			/* REVISIT could have a waitchannel here, if
			 * concurrent open performance is important
			 */
			break;
		}
	} while (status != -EAGAIN);

    spin_lock_irqsave(&port->port_lock, flags);
	port->open_count = 1;
	port->openclose = false;

#if ACM_MODEM_SUPPORT_NOTIFY
    /* if connected, start the I/O stream */
	if (port->port_usb) {
        struct gserial	*gser = port->port_usb;
		if (gser->connect)
			gser->connect(gser);
	}
#endif
    spin_unlock_irqrestore(&port->port_lock, flags);

	return 0;
}

static USB_INT gs_acm_modem_close(struct inode *inode, struct file *file)
{
	struct gs_acm_modem_port *port;
    USB_ULONG flags;
    USB_INT status;

    status = gs_acm_modem_get_port_num(inode);
    if (status < 0)
        return status;
    port = gs_acm_modem_ports[status].port;

	spin_lock_irqsave(&port->port_lock, flags);

	if (port->open_count != 1) {
		if (port->open_count == 0) {
			WARN_ON(1);
            return -EBADF;
        }
		else {
			--port->open_count;
        }
		goto exit;
	}

	/* mark port as closing but in use; we can drop port lock
	 * and sleep if necessary
	 */
	port->openclose = true;
	port->open_count = 0;

	/* restore the state */
	memset(&port->cur_flow_msg, 0, sizeof(MODEM_MSC_STRU));
	port->ps_recv_flow = 0;
	port->line_state = 0;
	port->line_state_change = 0;

#if ACM_MODEM_SUPPORT_NOTIFY
    {
        struct gserial	*gser;
    	gser = port->port_usb;
    	if (gser && gser->disconnect)
    		gser->disconnect(gser);
    }
#endif

	port->openclose = false;

	wake_up_interruptible(&port->close_wait);
exit:
	spin_unlock_irqrestore(&port->port_lock, flags);
    return 0;
}

static USB_INT gs_acm_modem_write_base(struct gs_acm_modem_port *port,
                                   struct sk_buff* skb)
{
	USB_INT status = 0;
    USB_ULONG flags;
    struct gs_acm_modem_sglist_info *sglist_info;
    struct scatterlist *sg_list_item;

    if (unlikely(NULL == skb || 0 == skb->len)) {
        port->stat_tx_inv_param++;
        return -EINVAL;
    }
    if(NULL == port || NULL == port->port_usb)
    {
         return -EINVAL;
    }

    port->stat_write_bytes += skb->len;
    spin_lock_irqsave(&port->port_lock, flags);

    /* if suspend, could not send data */
    if (unlikely(port->is_suspend)) {
        port->stat_tx_suspend++;
        status = -ESHUTDOWN;
        goto write_base_exit;
    }

    /* get current sglist info */
    sglist_info = &port->sglist_ctx.sg_list_array[port->sglist_ctx.submit_pos];

    /* check tx fifo pos */
    if (port->sglist_ctx.done_pos == sglist_info->next->index) {
        port->stat_tx_fifo_full++;
        status = -EAGAIN;
        goto write_base_exit;
    }

    /* skb enqueue */
    __skb_queue_tail(&sglist_info->skb_list, skb);

    /* fill the sg_list */
    sg_list_item = sglist_info->sg_list + sglist_info->item_num;
    sg_list_item->page_link = 0;
    sg_set_buf(sg_list_item, (const USB_PVOID)skb->data, skb->len);

    /* update the sglist info */
    sglist_info->item_num++;
    sglist_info->total_size += skb->len;

#if 0
    /* check zlp */
    if (skb->len % port->port_usb->in->maxpacket == 0) {
        sg_list_item = sglist_info->sg_list + sglist_info->item_num;
        sg_list_item->page_link = 0;
        /* we use pre skb data and length to 0, to send zlp */
        sg_set_buf(sg_list_item, (const void *)skb->data, 0);
        sglist_info->item_num++;
    }
#endif

    /* check whether can be submit to usb core*/
    if ((sglist_info->item_num >= port->sglist_ctx.item_num_threshold)
        || (sglist_info->total_size > port->sglist_ctx.buffer_size_threshold)
        /*|| (sglist_info->item_num == (ACM_MODEM_SG_LIST_ITEM_NUM - 1))*/) {

        port->stat_direct_tx++;
        status = gs_acm_modem_start_tx(port, sglist_info);
        if (status) {
            goto write_base_exit;
        }
    }
    else if (sglist_info->item_num && !port->sglist_ctx.tx_timeout_on) {
        port->sglist_ctx.tx_timeout_on = 1;
        gs_acm_evt_push(port, &gs_modem_write_evt_manage);
        queue_delayed_work(gs_modem_driver->acm_work_queue,
            &port->rw_work, port->sglist_ctx.tx_timeout);
    }

write_base_exit:
    spin_unlock_irqrestore(&port->port_lock, flags);
	return status;
}

/**
 *	gs_acm_modem_write		-	write method for tty device file
 *	@file: acm file pointer
 *	@buf: user data to write
 *	@count: bytes to write
 *	@ppos: unused
 *
 *	Write data to a acm device.
 *
 */
static ssize_t gs_acm_modem_write(struct file *file, const USB_CHAR __user *buf,
						          size_t count, loff_t *ppos)
{
    struct inode *inode = file->f_path.dentry->d_inode;
    USB_INT		port_num;
	struct gs_acm_modem_port	*port;
    USB_INT status;

    if (unlikely(NULL == buf)) {
        DBG_E(MBB_ACM,"%s invalid param buf:%p, count:%d\n", __FUNCTION__, buf, count);
        return -EFAULT;
    }

    if (get_fs() != KERNEL_DS) {
        DBG_E(MBB_ACM, "can't support in usr space\n");
        return -ENOTSUPP;
    }

    port_num = gs_acm_modem_get_port_num(inode);
    if (port_num < 0)
        return port_num;

    port = gs_acm_modem_ports[port_num].port;

    port->stat_write_submit++;
    status = gs_acm_modem_write_base(port, (struct sk_buff*)buf);

    return (ssize_t)status;
}

/**
 *	gs_acm_modem_read		-	read method for modem device file
 *	@file: acm file pointer
 *	@buf: user data to read
 *	@count: bytes to read
 *	@ppos: unused
 *
 *	read data form a acm device: we don't support
 *
 */
static ssize_t gs_acm_modem_read(struct file *file, USB_CHAR __user *buf,
						         size_t count, loff_t *ppos)
{
    return -ENOTSUPP;
}

static USB_INT32 gs_acm_modem_ioctl(struct file *file, USB_UINT cmd, USB_ULONG arg)
{
    struct inode *inode = file->f_path.dentry->d_inode;
    ACM_WR_ASYNC_INFO *rw_info;
    ACM_READ_BUFF_INFO *rd_info;
    struct gs_acm_modem_port	*port;
    USB_INT	status;

    status = gs_acm_modem_get_port_num(inode);

    if (get_fs() != KERNEL_DS) {
        DBG_E(MBB_ACM,"can't support in usr space\n");
        return -ENOTSUPP;
    }
    
    if (status < 0) {
        DBG_E(MBB_ACM,"gs_acm_modem_get_port_num err, status=%d\n",status);
        return status;
    }

    port = gs_acm_modem_ports[status].port;
    if (!port) {
        printk(KERN_ERR "modem is disconnect\n");
        return -ESHUTDOWN;
    }

    /* init the return status */
    status = 0;
    switch (cmd) {
	case ACM_IOCTL_SET_READ_CB:
	case UDI_IOCTL_SET_READ_CB:
	    port->read_done_cb = (ACM_READ_DONE_CB_T)arg;
	    break;

	case ACM_IOCTL_SET_WRITE_CB:
	case UDI_IOCTL_SET_WRITE_CB:
	    port->write_done_cb = (ACM_WRITE_DONE_CB_T)arg;
	    break;

	case ACM_IOCTL_SET_EVT_CB:
        port->event_notify_cb = (ACM_EVENT_CB_T)arg;
	    break;

	case ACM_IOCTL_SET_FREE_CB:
	    port->write_done_free_cb = (ACM_FREE_CB_T)arg;
	    break;

	case ACM_IOCTL_WRITE_ASYNC:
		if (0 == arg) {
            DBG_E(MBB_ACM,"gs_acm_modem_ioctl ACM_IOCTL_WRITE_ASYNC invalid param\n");
			return -EFAULT;
		}
		rw_info = (ACM_WR_ASYNC_INFO *)arg;
		port->stat_write_async_call++;
		status = gs_acm_modem_write_base(port, (struct sk_buff*)rw_info->pBuffer);
	    break;

	case ACM_IOCTL_GET_RD_BUFF:
		if (0 == arg) {
            DBG_E(MBB_ACM,"gs_acm_modem_ioctl ACM_IOCTL_GET_RD_BUFF invalid param\n");
		    return -EFAULT;
		}
		port->stat_get_buf_call++;
		status = gs_acm_modem_get_read_buf(port, (ACM_WR_ASYNC_INFO*)arg);
	    break;

	case ACM_IOCTL_RETURN_BUFF:
		if (0 == arg) {
                 DBG_E(MBB_ACM,"gs_acm_modem_ioctl ACM_IOCTL_RETURN_BUFF invalid param\n");
		    return -EFAULT;
		}
		port->stat_ret_buf_call++;
		status = gs_acm_modem_ret_read_buf(port, (ACM_WR_ASYNC_INFO*)arg);
	    break;

	case ACM_IOCTL_RELLOC_READ_BUFF:
	    if (0 == arg) {
		    pr_err("gs_acm_modem_ioctl ACM_IOCTL_RETURN_BUFF invalid param\n");
		    return -EFAULT;
		}
	    rd_info = (ACM_READ_BUFF_INFO*)arg;
	    port->read_buf_size = (unsigned)rd_info->u32BuffSize;
	    break;

    case ACM_IOCTL_SEND_BUFF_CAN_DMA:
    case ACM_IOCTL_WRITE_DO_COPY:
		/* modem always return ok */
	    break;

    case ACM_MODEM_IOCTL_SET_MSC_READ_CB:
        port->read_sig_cb = (ACM_MODEM_MSC_READ_CB_T)arg;
	    break;

    case ACM_MODEM_IOCTL_MSC_WRITE_CMD:
        status = gs_acm_modem_write_signal(port, (MODEM_MSC_STRU*)arg);
	    break;

    case ACM_MODEM_IOCTL_SET_REL_IND_CB:
        port->rel_ind_cb = (ACM_MODEM_REL_IND_CB_T)arg;
	    break;

	default:
        status = -1;
        break;
	}

    return status;
}

static const struct file_operations gs_acm_modem_fops = {
	.llseek		= no_llseek,

	.read		=       gs_acm_modem_read,
	.write		=       gs_acm_modem_write,
	.unlocked_ioctl	=   gs_acm_modem_ioctl,
	.open		=       gs_acm_modem_open,
	.release	=       gs_acm_modem_close,
};

/*-------------------------------------------------------------------------*/

static USB_INT g_modem_sg_cnt = 0;

static USB_INT gs_acm_modem_port_alloc(USB_UINT port_num, struct usb_cdc_line_coding *coding)
{
	struct gs_acm_modem_port	*port;
    USB_UINT i;
    struct gs_acm_modem_sglist_info *sglist_info;

	port = kzalloc(sizeof(struct gs_acm_modem_port), GFP_KERNEL);
	if (port == NULL)
		return -ENOMEM;

    for (i = 0; i < ACM_MODEM_SG_LIST_NUM; i++) {
        sglist_info = &port->sglist_ctx.sg_list_array[i];
        sglist_info->sg_list = kzalloc(ACM_MODEM_SG_LIST_ITEM_NUM *
            sizeof(struct scatterlist), GFP_KERNEL);
            g_modem_sg_cnt++;
        if (!sglist_info->sg_list) {
            goto port_alloc_fail;
        }
        skb_queue_head_init(&sglist_info->skb_list);

        sglist_info->port = port;
		sglist_info->item_num = 0;
		sglist_info->total_size = 0;
		sglist_info->index = i;
		sglist_info->next =
            &port->sglist_ctx.sg_list_array[(i + 1)%ACM_MODEM_SG_LIST_NUM];
    }
    port->sglist_ctx.buffer_size_threshold = ACM_MODEM_SIZE_THRE;
    port->sglist_ctx.item_num_threshold = ACM_MODEM_NUM_THRE;
    port->sglist_ctx.tx_timeout = ACM_MODEM_TIMEOUT_THRE;

    gs_acm_evt_init(&gs_modem_write_evt_manage, "modem_write");
    gs_acm_evt_init(&gs_modem_read_evt_manage, "modem_read");
    gs_acm_evt_init(&gs_modem_sig_stat_evt_manage, "modem_sig_stat");

	spin_lock_init(&port->port_lock);
	init_waitqueue_head(&port->close_wait);

    INIT_DELAYED_WORK(&port->rw_work, gs_acm_modem_rw_push);

	INIT_LIST_HEAD(&port->read_pool);
	INIT_LIST_HEAD(&port->read_done_queue);
    INIT_LIST_HEAD(&port->read_queue_in_usb);
	INIT_LIST_HEAD(&port->write_pool);

    init_waitqueue_head(&port->write_wait);
    init_waitqueue_head(&port->read_wait);
    init_waitqueue_head(&port->realloc_wait);

	port->port_num = port_num;
	port->port_line_coding = *coding;

    port->read_buf_size = ACM_MODEM_DFT_RD_BUF_SIZE;
    port->read_req_num = ACM_MODEM_DFT_RD_REQ_NUM;
    port->write_req_num = ACM_MODEM_DFT_WT_REQ_NUM;

	gs_acm_modem_ports[port_num].port = port;

    /* mark the asic string for debug */
    snprintf(gs_acm_modem_ports[port_num].name_domain, 4, "mdm%d", port_num);
    snprintf(port->read_domain, 4, "%d_rd", port_num);
    snprintf(port->write_domain, 4, "%d_wt", port_num);
    snprintf(port->debug_tx_domain, 4, "dtx%d", port_num);
    snprintf(port->debug_rx_domain, 4, "drx%d", port_num);
    snprintf(port->debug_port_domain, 4, "dpt%d", port_num);

	return 0;

port_alloc_fail:

    for (i = 0; i < ACM_MODEM_SG_LIST_NUM; i++) {
        sglist_info = &port->sglist_ctx.sg_list_array[i];
        if (sglist_info->sg_list) {
            kfree(sglist_info->sg_list);
        }
    }

    if (port)
        kfree(port);
    return -ENOMEM;
}

static struct u_modem_driver *gs_acm_modem_alloc_driver(USB_INT lines)
{
	struct u_modem_driver *driver;

	driver = kzalloc(sizeof(struct u_modem_driver), GFP_KERNEL);
	if (driver) {
		kref_init(&driver->kref);
		driver->num = lines;
	}
	return driver;
}

static USB_INT gs_acm_modem_register_driver(struct u_modem_driver *driver)
{
    USB_INT error;
    dev_t dev;

    DBG_I(MBB_ACM,"++++ alloc_chrdev_region dev:0x%x, minor_start:%d, num:%d, name:%s\n", 
            (USB_UINT)&dev, (USB_UINT)driver->minor_start, driver->num, driver->name);
    error = alloc_chrdev_region(&dev, driver->minor_start,
						driver->num, driver->name);
    if (error < 0) {
		return error;
	}
	driver->major = MAJOR(dev);
	driver->minor_start = MINOR(dev);

    cdev_init(&driver->cdev, &gs_acm_modem_fops);
    driver->cdev.owner = driver->owner;

    DBG_I(MBB_ACM,"++++ cdev_add cdev:0x%x, dev:0x%x, num:%d\n", 
            (USB_UINT)&driver->cdev, (USB_UINT)dev, driver->num);
    error = cdev_add(&driver->cdev, dev, driver->num);
    if (error) {
		unregister_chrdev_region(dev, driver->num);
		return error;
	}
    driver->dev_no = dev;

    return 0;
}

static USB_VOID gs_acm_modem_unregister_driver(struct u_modem_driver *driver)
{
    DBG_I(MBB_ACM,"---- cdev_del cdev:0x%x\n", (USB_UINT)&driver->cdev);
    cdev_del(&driver->cdev);
    DBG_I(MBB_ACM,"++++ alloc_chrdev_region dev_no:0x%x, num:%d\n", 
                gs_modem_driver->dev_no, gs_modem_driver->num);
    return unregister_chrdev_region(gs_modem_driver->dev_no, gs_modem_driver->num);
}

static acm_ctx_t* acm_ctx =NULL;

static struct device *gs_acm_modem_register_device(struct u_modem_driver *driver,
                        USB_UINT index, struct device *device)
{
	USB_CHAR name[ACM_MODEM_NAME_MAX];
       
	dev_t dev = MKDEV(driver->major, driver->minor_start) + index;

	if (index >= driver->num) {
             DBG_E(MBB_ACM,"Attempt to register invalid tty line number (%d).\n", index);
		return ERR_PTR(-EINVAL);
	}
    snprintf(name, ACM_MODEM_NAME_MAX, "%s",
                   acm_ctx->mdm_name_type_var[index].name);

	return device_create(tty_class, device, dev, NULL, name);
}

static USB_VOID gs_acm_modem_unregister_device(struct u_modem_driver *driver,
                                           USB_UINT index)
{
	device_destroy(tty_class,
		MKDEV(driver->major, driver->minor_start) + index);
}

/**
 * gacm_modem_setup - initialize cdev driver for one or more gs_acm_modem_ports
 * @g: gadget to associate with these gs_acm_modem_ports
 * @count: how many gs_acm_modem_ports to support
 * Context: may sleep
 *
 * Returns negative errno or zero.
 */
USB_INT gacm_modem_setup(struct usb_gadget *g, USB_UINT count)
{
	USB_UINT			i;
	struct usb_cdc_line_coding	coding;
	USB_INT				status;

	if (count == 0 || count > ACM_MDM_COUNT)
		return -EINVAL;

	gs_modem_driver = gs_acm_modem_alloc_driver(count);
	if (!gs_modem_driver)
		return -ENOMEM;

    gs_modem_driver->owner = THIS_MODULE;
	gs_modem_driver->driver_name = ACM_MODEM_DRV_NAME;
	gs_modem_driver->name = ACM_MODEM_PREFIX;

	coding.dwDTERate = cpu_to_le32(115200);
	coding.bCharFormat = 8;
	coding.bParityType = USB_CDC_NO_PARITY;
	coding.bDataBits = USB_CDC_1_STOP_BITS;

	/* alloc and init each port */
	for (i = 0; i < count; i++) {
		mutex_init(&gs_acm_modem_ports[i].open_close_lock);
		status = gs_acm_modem_port_alloc(i, &coding);
		if (status) {
			count = i;
			goto setup_fail;
		}
	}
	gs_acm_modem_n_ports = count;

	/* register the driver ... */
	status = gs_acm_modem_register_driver(gs_modem_driver);
	if (status) {
             DBG_E(MBB_ACM,"%s: cannot register, err %d\n",
				__func__, status);
		goto setup_fail;
	}

	/* register devices ... */
	for (i = 0; i < count; i++) {
		struct device	*cdev;

		cdev = gs_acm_modem_register_device(gs_modem_driver, i, &g->dev);
		if (IS_ERR(cdev)){
                    DBG_W(MBB_ACM,"%s: no classdev for port %d, err %ld\n",
				__func__, i, PTR_ERR(cdev));
            goto setup_fail;
        }
        gs_acm_modem_ports[i].cdev = cdev;
	}

    gs_modem_driver->acm_work_queue = create_singlethread_workqueue("acm_mdm");
    if (!gs_modem_driver->acm_work_queue)
    {
        status = -ENOMEM;
        goto setup_fail;
    }

	return status;

setup_fail:
    if (gs_modem_driver->acm_work_queue)
        destroy_workqueue(gs_modem_driver->acm_work_queue);

	while (count){
	    --count; /* change for coverity */
        if (gs_acm_modem_ports[count].port)
            kfree(gs_acm_modem_ports[count].port);

        if (gs_acm_modem_ports[count].cdev)
            gs_acm_modem_unregister_device(gs_modem_driver, count);
    }

    gs_acm_modem_unregister_driver(gs_modem_driver);

    if (gs_modem_driver){
        kfree(gs_modem_driver);
	    gs_modem_driver = NULL;
    }
	return status;
}

static USB_INT gs_acm_modem_closed(struct gs_acm_modem_port *port)
{
	USB_INT cond;
    USB_ULONG flags;

	spin_lock_irqsave(&port->port_lock, flags);
	cond = (port->open_count == 0) && !port->openclose;
	spin_unlock_irqrestore(&port->port_lock, flags);
	return cond;
}

/**
 * gacm_modem_cleanup - remove cdev-over-USB driver and devices
 * Context: may sleep
 *
 * This is called to free all resources allocated by @gserial_setup().
 * Accordingly, it may need to wait until some open /dev/ files have
 * closed.
 *
 * The caller must have issued @gserial_disconnect() for any gs_acm_modem_ports
 * that had previously been connected, so that there is never any
 * I/O pending when it's called.
 */
USB_VOID gacm_modem_cleanup(USB_VOID)
{
	USB_UINT i,j;
	struct gs_acm_modem_port *port;
    struct gs_acm_modem_sglist_info *sglist_info;
    USB_ULONG flags;


	if (!gs_modem_driver) {
           DBG_W(MBB_ACM,"gacm_modem_cleanup gs_modem_driver is NULL\n");
		return;
	}
#if 1
	/* start sysfs and /dev/ttyGS* node removal */
	for (i = 0; i < gs_acm_modem_n_ports; i++)
		gs_acm_modem_unregister_device(gs_modem_driver, i);
#endif

	for (i = 0; i < gs_acm_modem_n_ports; i++) {
		/* prevent new opens */
		mutex_lock(&gs_acm_modem_ports[i].open_close_lock);
		port = gs_acm_modem_ports[i].port;
		gs_acm_modem_ports[i].port = NULL;
		mutex_unlock(&gs_acm_modem_ports[i].open_close_lock);

		/* wait for old opens to finish */
		wait_event(port->close_wait, gs_acm_modem_closed(port));

		WARN_ON(port->port_usb != NULL);

		spin_lock_irqsave(&port->port_lock, flags);
        for (j = 0; j < ACM_MODEM_SG_LIST_NUM; j++) {
            sglist_info = &port->sglist_ctx.sg_list_array[j];

            /* if there are skb remain in list, free it */
            if (sglist_info->item_num) {
                gs_acm_modem_free_skb_list(port, &sglist_info->skb_list);
                sglist_info->item_num = 0;
                sglist_info->total_size = 0;
            }
            if (sglist_info->sg_list) {
                g_modem_sg_cnt--;
                kfree(sglist_info->sg_list);
            }
        }
        spin_unlock_irqrestore(&port->port_lock, flags);

        cancel_delayed_work_sync(&port->rw_work);
		kfree(port);
	}
	gs_acm_modem_n_ports = 0;

#if 1
    if (gs_modem_driver->acm_work_queue) {
        flush_workqueue(gs_modem_driver->acm_work_queue);
        destroy_workqueue(gs_modem_driver->acm_work_queue);
        gs_modem_driver->acm_work_queue = NULL;
    }

    gs_acm_modem_unregister_driver(gs_modem_driver);
#endif

    if (gs_modem_driver){
        kfree(gs_modem_driver);
	    gs_modem_driver = NULL;
    }
}

USB_INT gacm_modem_line_state(struct gserial *gser, USB_UINT state)
{
    struct gs_acm_modem_port *port = gser->ioport;
    USB_ULONG flags;
    USB_UINT16 line_state;

    if (!port) {
        return -ESHUTDOWN;
    }

	spin_lock_irqsave(&port->port_lock, flags);
	line_state = port->line_state;

	/* if line state is change notify the callback */
	if (line_state != (USB_UINT16)(state & U_ACM_CTRL_DTR) ||
        line_state != (USB_UINT16)(state & U_ACM_CTRL_RTS)) {

        port->line_state =
            (USB_UINT16)((state & U_ACM_CTRL_DTR) | (state & U_ACM_CTRL_RTS));
        port->line_state_change = 1;
    }
	spin_unlock_irqrestore(&port->port_lock, flags);

    /* host may change the state in a short time, delay it, use the last state */
	if (port->line_state_change) {
        port->stat_notify_sched++;
        gs_acm_evt_push(port, &gs_modem_sig_stat_evt_manage);
	    queue_delayed_work(gs_modem_driver->acm_work_queue, &port->rw_work, 50);
	}
	return 0;
}

USB_INT gacm_modem_suspend(struct gserial *gser)
{
    struct gs_acm_modem_port *port = gser->ioport;
    USB_ULONG flags;

    if (!port) {
        return -ESHUTDOWN;
    }

	spin_lock_irqsave(&port->port_lock, flags);
    port->is_suspend = 1;
    port->sr_change = 1;
	spin_unlock_irqrestore(&port->port_lock, flags);

	if (port->sr_change) {
        port->stat_notify_sched++;
        gs_acm_evt_push(port, &gs_modem_sig_stat_evt_manage);
	    queue_delayed_work(gs_modem_driver->acm_work_queue, &port->rw_work, 0);
	}
	return 0;
}

USB_INT gacm_modem_resume(struct gserial *gser)
{
    struct gs_acm_modem_port *port = gser->ioport;
    USB_ULONG flags;

    if (!port) {
        return -ESHUTDOWN;
    }

	spin_lock_irqsave(&port->port_lock, flags);
    port->is_suspend = 0;
    gs_acm_modem_start_rx(port);
	spin_unlock_irqrestore(&port->port_lock, flags);
	return 0;
}


/**
 * gacm_modem_connect - notify TTY I/O glue that USB link is active
 * @gser: the function, set up with endpoints and descriptors
 * @port_num: which port is active
 * Context: any (usually from irq)
 *
 * This is called activate endpoints and let the TTY layer know that
 * the connection is active ... not unlike "carrier detect".  It won't
 * necessarily start I/O queues; unless the TTY is held open by any
 * task, there would be no point.  However, the endpoints will be
 * activated so the USB host can perform I/O, subject to basic USB
 * hardware flow control.
 *
 * Caller needs to have set up the endpoints and USB function in @dev
 * before calling this, as well as the appropriate (speed-specific)
 * endpoint descriptors, and also have set up the TTY driver by calling
 * @gserial_setup().
 *
 * Returns negative errno or zero.
 * On success, ep->driver_data will be overwritten.
 */
USB_INT gacm_modem_connect(struct gserial *gser, USB_UINT8 port_num)
{
	struct gs_acm_modem_port	*port;
	USB_ULONG	flags;
	USB_INT		status;

	if (!gs_modem_driver || port_num >= gs_acm_modem_n_ports) {
        DBG_E(MBB_ACM,"gacm_modem_connect fail drv:%p, port_num:%d, n_ports:%d\n",
                 gs_modem_driver, port_num, gs_acm_modem_n_ports);
        BUG();
		return -ENXIO;
    }

	/* we "know" gserial_cleanup() hasn't been called */
	port = gs_acm_modem_ports[port_num].port;

    /* mask the not ready interrupt for usb netcard class function driver */
    gser->out->enable_xfer_in_progress = 1;

	/* activate the endpoints */
	status = usb_ep_enable(gser->in);
	if (status < 0) {
        port->stat_enable_in_fail++;
		return status;
    }

	status = usb_ep_enable(gser->out);
	if (status < 0) {
        port->stat_enable_out_fail++;
		goto fail_out;
    }

	/* then tell the tty glue that I/O can work */
	spin_lock_irqsave(&port->port_lock, flags);
    gser->in->driver_data = port;
    gser->out->driver_data = port;
	gser->ioport = (void*)port;
	port->port_usb = gser;

	/* REVISIT unclear how best to handle this state...
	 * we don't really couple it with the Linux TTY.
	 */
	gser->port_line_coding = port->port_line_coding;

    /* prepare requests */
    gs_acm_modem_prepare_io(port);

	/* if it's already open, start I/O ... and notify the serial
	 * protocol about open/close status (connect/disconnect).
	 * don't need to notify host now ...
	 */
#if ACM_MODEM_SUPPORT_NOTIFY
	if (port->open_count) {
		if (gser->connect)
			gser->connect(gser);
	} else {
		if (gser->disconnect)
			gser->disconnect(gser);
	}
#endif

    /* start read requests */
    gs_acm_modem_start_rx(port);

	spin_unlock_irqrestore(&port->port_lock, flags);
	port->in_name = (char*)gser->in->name;
	port->out_name = (char*)gser->out->name;
    port->stat_port_connect++;
    port->stat_port_is_connect = 1;
	return status;

fail_out:
	usb_ep_disable(gser->in);
	gser->in->driver_data = NULL;
	port->stat_port_is_connect = 0;
	return status;
}

/**
 * gacm_modem_disconnect - notify TTY I/O glue that USB link is inactive
 * @gser: the function, on which gserial_connect() was called
 * Context: any (usually from irq)
 *
 * This is called to deactivate endpoints and let the TTY layer know
 * that the connection went inactive ... not unlike "hangup".
 *
 * On return, the state is as if gserial_connect() had never been called;
 * there is no active USB I/O on these endpoints.
 */
USB_VOID gacm_modem_disconnect(struct gserial *gser)
{
	struct gs_acm_modem_port	*port = gser->ioport;
	USB_ULONG	flags;

	if (!port) {
        BUG();
		return;
    }

	/* disable endpoints, aborting down any active I/O */
	usb_ep_disable(gser->out);
	usb_ep_disable(gser->in);

    spin_lock_irqsave(&port->port_lock, flags);

    port->port_usb = NULL;
	gser->ioport = NULL;
    gser->out->driver_data = NULL;
    gser->in->driver_data = NULL;

	gs_acm_modem_free_requests(gser->out, &port->read_pool, NULL, port);
	gs_acm_modem_free_requests(gser->out, &port->read_done_queue, NULL, port);
	gs_acm_modem_free_requests(gser->in, &port->write_pool, NULL, port);
	port->read_allocated = 0;
	port->write_allocated = 0;

    port->stat_port_disconnect++;
    port->stat_port_is_connect = 0;

    port->is_suspend = 0;
    port->sr_change = 0;
    spin_unlock_irqrestore(&port->port_lock, flags);
}

USB_VOID usb_modem_init(USB_VOID)
{
    DBG_I(MBB_ACM,"%s : entry!\n",__func__);
    acm_ctx = acm_get_ctx();
    return;
}

USB_VOID usb_modem_exit(USB_VOID)
{
    return;
}

static USB_VOID acm_modem_dump_ep_info(struct gs_acm_modem_port *port)
{
    USB_PCHAR find;
    USB_UINT ep_num;

    if (port->stat_port_is_connect) {
        DBG_I(MBB_ACM,"in ep name:\t\t <%s>\n", port->in_name);
        find = strstr(port->in_name, "ep");
        if (find) {
            /* skip "ep" */
            find += 2;
            ep_num = simple_strtoul(find, NULL, 0);
            DBG_I(MBB_ACM,"in ep num:\t\t <%d>\n", ep_num * 2 + 1);
        }
        DBG_I(MBB_ACM,"out ep name:\t\t <%s>\n", port->out_name);
        find = strstr(port->out_name, "ep");
        if (find) {
            /* skip "ep" */
            find += 2;
            ep_num = simple_strtoul(find, NULL, 0);
            DBG_I(MBB_ACM,"out ep num:\t\t <%d>\n", ep_num * 2);
        }
    }
    else {
        DBG_E(MBB_ACM,"the acm dev is not connect\n");
    }
}

/* ----- functions for debug ----- */
USB_VOID acm_modem_set_timeout(USB_INT port_num, USB_INT timeout)
{
    struct gs_acm_modem_port	*port;

    if (!gs_modem_driver || port_num >= gs_acm_modem_n_ports) {
        DBG_E(MBB_ACM,"gacm_dump fail drv:%p, port_num:%d, n_ports:%d\n",
                 gs_modem_driver, port_num, gs_acm_modem_n_ports);
		return;
    }

	port = gs_acm_modem_ports[port_num].port;
	port->sglist_ctx.tx_timeout = timeout;
	return;
}

USB_VOID acm_modem_dump(USB_INT port_num)
{
    struct gs_acm_modem_port	*port;
 
    if (!gs_modem_driver || port_num >= gs_acm_modem_n_ports) {
        DBG_E(MBB_ACM,"gacm_dump fail drv:%p, port_num:%d, n_ports:%d\n",
                 gs_modem_driver, port_num, gs_acm_modem_n_ports);
        
		return;
    }

	port = gs_acm_modem_ports[port_num].port;

    DBG_E(MBB_ACM,"=== dump stat dev ctx info ===\n");
    DBG_E(MBB_ACM,"build version:            %s\n", __VERSION__);
    DBG_E(MBB_ACM,"build date:               %s\n", __DATE__);
    DBG_E(MBB_ACM,"build time:               %s\n", __TIME__);
    DBG_E(MBB_ACM,"dev name                  %s\n", 
            acm_ctx->mdm_name_type_var[port_num].name);
   
    DBG_E(MBB_ACM,"gs_modem_drv_invalid      %d\n", gs_modem_drv_invalid);
    DBG_E(MBB_ACM,"gs_modem_port_num_err     %d\n", gs_modem_port_num_err);
    DBG_E(MBB_ACM,"open_count                %d\n", port->open_count);
    DBG_E(MBB_ACM,"openclose                 %d\n", port->openclose);
    DBG_E(MBB_ACM,"port_num                  %d\n", port->port_num);
    DBG_E(MBB_ACM,"line_state                %d\n", port->line_state);
    
    DBG_E(MBB_ACM,"line_state_change         %d\n", port->line_state_change);
    
    acm_modem_dump_ep_info(port);

    mdelay(10);

    DBG_E(MBB_ACM,"\n=== dump stat read info ===\n");
    DBG_E(MBB_ACM,"read_started              %d\n", port->read_started);
    DBG_E(MBB_ACM,"read_allocated            %d\n", port->read_allocated);
    DBG_E(MBB_ACM,"read_req_enqueued         %d\n", port->read_req_enqueued);
    DBG_E(MBB_ACM,"read_req_num              %d\n", port->read_req_num);
    DBG_E(MBB_ACM,"read_buf_size             %d\n", port->read_buf_size);
    DBG_E(MBB_ACM,"read_completed            %d\n", port->read_completed);
    DBG_E(MBB_ACM,"\n=== dump rx status info ===\n");
    
    DBG_E(MBB_ACM,"stat_get_buf_call         %d\n", port->stat_get_buf_call);
    DBG_E(MBB_ACM,"stat_ret_buf_call         %d\n", port->stat_ret_buf_call);
    DBG_E(MBB_ACM,"stat_rx_submit            %d\n", port->stat_rx_submit);
    DBG_E(MBB_ACM,"stat_rx_submit_fail       %d\n", port->stat_rx_submit_fail);
    DBG_E(MBB_ACM,"stat_rx_disconnect        %d\n", port->stat_rx_disconnect);
    DBG_E(MBB_ACM,"stat_rx_no_req            %d\n", port->stat_rx_no_req);
    DBG_E(MBB_ACM,"stat_rx_done              %d\n", port->stat_rx_done);
    DBG_E(MBB_ACM,"stat_rx_done_fail         %d\n", port->stat_rx_done_fail);
    DBG_E(MBB_ACM,"stat_rx_done_bytes        %d\n", port->stat_rx_done_bytes);
    DBG_E(MBB_ACM,"stat_rx_done_disconnect   %d\n", port->stat_rx_done_disconnect);
    DBG_E(MBB_ACM,"stat_rx_done_schdule      %d\n", port->stat_rx_done_schdule);
    DBG_E(MBB_ACM,"stat_rx_callback          %d\n", port->stat_rx_callback);
    DBG_E(MBB_ACM,"stat_rx_cb_not_start      %d\n", port->stat_rx_cb_not_start);
    DBG_E(MBB_ACM,"stat_alloc_skb            %d\n", port->stat_alloc_skb);
    DBG_E(MBB_ACM,"stat_alloc_skb_fail       %d\n", port->stat_alloc_skb_fail);
    DBG_E(MBB_ACM,"stat_free_skb             %d\n", port->stat_free_skb);

    
    mdelay(10);
    
    DBG_E(MBB_ACM,"\n=== dump stat write info ===\n");
    DBG_E(MBB_ACM,"write_req_num             %d\n", port->write_req_num);
    DBG_E(MBB_ACM,"write_started             %d\n", port->write_started);
    DBG_E(MBB_ACM,"write_allocated           %d\n", port->write_allocated);
    DBG_E(MBB_ACM,"\n=== dump tx status info ===\n");

    DBG_E(MBB_ACM,"stat_write_async_call     %d\n", port->stat_write_async_call);
    DBG_E(MBB_ACM,"stat_write_submit         %d\n", port->stat_write_submit);
    DBG_E(MBB_ACM,"stat_write_bytes          %d\n", port->stat_write_bytes);
    DBG_E(MBB_ACM,"stat_tx_submit            %d\n", port->stat_tx_submit);
    DBG_E(MBB_ACM,"stat_tx_submit_fail       %d\n", port->stat_tx_submit_fail);
    DBG_E(MBB_ACM,"stat_tx_submit_bytes      %d\n", port->stat_tx_submit_bytes);
    DBG_E(MBB_ACM,"stat_tx_done              %d\n", port->stat_tx_done);
    DBG_E(MBB_ACM,"stat_tx_done_fail         %d\n", port->stat_tx_done_fail);
    DBG_E(MBB_ACM,"stat_tx_done_bytes        %d\n", port->stat_tx_done_bytes);
    DBG_E(MBB_ACM,"stat_tx_no_req            %d\n", port->stat_tx_no_req);
    DBG_E(MBB_ACM,"stat_tx_inv_param         %d\n", port->stat_tx_inv_param);
    DBG_E(MBB_ACM,"stat_tx_disconnect        %d\n", port->stat_tx_disconnect);
    DBG_E(MBB_ACM,"stat_tx_fifo_full         %d\n", port->stat_tx_fifo_full);
    DBG_E(MBB_ACM,"stat_tx_suspend           %d\n", port->stat_tx_suspend);
    DBG_E(MBB_ACM,"stat_timeout_tx           %d\n", port->stat_timeout_tx);
    DBG_E(MBB_ACM,"stat_direct_tx            %d\n", port->stat_direct_tx);

    mdelay(10);
    
    DBG_E(MBB_ACM,"\n=== dump port status info ===\n");
    DBG_E(MBB_ACM,"stat_port_connect         %d\n", port->stat_port_connect);
    DBG_E(MBB_ACM,"stat_port_disconnect      %d\n", port->stat_port_disconnect);
    DBG_E(MBB_ACM,"stat_enable_in_fail       %d\n", port->stat_enable_in_fail);
    DBG_E(MBB_ACM,"stat_enable_out_fail      %d\n", port->stat_enable_out_fail);
    DBG_E(MBB_ACM,"stat_notify_sched         %d\n", port->stat_notify_sched);
    DBG_E(MBB_ACM,"stat_notify_on_cnt        %d\n", port->stat_notify_on_cnt);
    DBG_E(MBB_ACM,"stat_notify_off_cnt       %d\n", port->stat_notify_off_cnt);
    DBG_E(MBB_ACM,"stat_read_sig_cnt         %d\n", port->stat_read_sig_cnt);
    DBG_E(MBB_ACM,"sr_change                 %d\n", port->sr_change);
    DBG_E(MBB_ACM,"is_suspend                %d\n", port->is_suspend);


}

USB_VOID acm_modem_dump_sglist(USB_INT port_num)
{
    struct gs_acm_modem_port	*port;
    USB_INT i;
    
    struct gs_acm_modem_sglist_info *sglist_info;

    
    if (!gs_modem_driver || port_num >= gs_acm_modem_n_ports) 
    {
        DBG_E(MBB_ACM,"gacm_dump fail drv:%p, port_num:%d, n_ports:%d\n",
                gs_modem_driver, port_num, gs_acm_modem_n_ports);
        return;
    }

	port = gs_acm_modem_ports[port_num].port;

    DBG_E(MBB_ACM,"\n=== dump port sglist_ctx info ===\n");
    DBG_E(MBB_ACM,"buffer_size_threshold     %d\n", port->sglist_ctx.buffer_size_threshold);
    DBG_E(MBB_ACM,"item_num_threshold        %d\n", port->sglist_ctx.item_num_threshold);
    DBG_E(MBB_ACM,"submit_pos                %d\n", port->sglist_ctx.submit_pos);
    DBG_E(MBB_ACM,"done_pos                  %d\n", port->sglist_ctx.done_pos);
    DBG_E(MBB_ACM,"tx_timeout                %d\n", port->sglist_ctx.tx_timeout);
    DBG_E(MBB_ACM,"tx_timeout_on             %d\n", port->sglist_ctx.tx_timeout_on);
    DBG_E(MBB_ACM,"\n");

	for (i = 0; i < ACM_MODEM_SG_LIST_NUM; i++) {
        sglist_info = &port->sglist_ctx.sg_list_array[i];
        DBG_E(MBB_ACM,"=== dump sglist info[%d], addr:<0x%x> ===\n", i, (USB_UINT)sglist_info);
        DBG_E(MBB_ACM,"sg_list                   0x%x\n", (USB_UINT)sglist_info->sg_list);
        DBG_E(MBB_ACM,"skb_list.qlen             %d\n", sglist_info->skb_list.qlen);
        DBG_E(MBB_ACM,"skb_list.prev             0x%x\n", (USB_UINT)sglist_info->skb_list.prev);
        DBG_E(MBB_ACM,"skb_list.next             0x%x\n", (USB_UINT)sglist_info->skb_list.next);
        DBG_E(MBB_ACM,"item_num                  %d\n", sglist_info->item_num);
        DBG_E(MBB_ACM,"total_size                %d\n", sglist_info->total_size);
        DBG_E(MBB_ACM,"index                     %d\n", sglist_info->index);
        DBG_E(MBB_ACM,"next                      0x%x\n", (USB_UINT)sglist_info->next);
        DBG_E(MBB_ACM,"\n");
    }
}

USB_VOID acm_modem_dump_msg(USB_INT port_num)
{
    struct gs_acm_modem_port	*port;

    if (!gs_modem_driver || port_num >= gs_acm_modem_n_ports) {

        DBG_E(MBB_ACM,"gacm_dump fail drv:%p, port_num:%d, n_ports:%d\n",
                gs_modem_driver, port_num, gs_acm_modem_n_ports);
		return;
    }

	port = gs_acm_modem_ports[port_num].port;
    DBG_I(MBB_ACM,"\n=== dump port flow msg info ===\n");

    DBG_E(MBB_ACM,"ps_recv_flow              %d\n", port->ps_recv_flow);
    DBG_E(MBB_ACM,"OP_Dtr                    %d\n", port->cur_flow_msg.OP_Dtr);
    DBG_E(MBB_ACM,"OP_Dsr                    %d\n", port->cur_flow_msg.OP_Dsr);
    DBG_E(MBB_ACM,"OP_Cts                    %d\n", port->cur_flow_msg.OP_Cts);
    DBG_E(MBB_ACM,"OP_Rts                    %d\n", port->cur_flow_msg.OP_Rts);
    DBG_E(MBB_ACM,"OP_Ri                     %d\n", port->cur_flow_msg.OP_Ri);
    DBG_E(MBB_ACM,"OP_Dcd                    %d\n", port->cur_flow_msg.OP_Dcd);
    DBG_E(MBB_ACM,"OP_Fc                     %d\n", port->cur_flow_msg.OP_Fc);
    DBG_E(MBB_ACM,"OP_Brk                    %d\n", port->cur_flow_msg.OP_Brk);
    DBG_E(MBB_ACM,"OP_Spare                  %d\n", port->cur_flow_msg.OP_Spare);
    DBG_E(MBB_ACM,"ucDtr                     %d\n", port->cur_flow_msg.ucDtr);
    DBG_E(MBB_ACM,"ucDsr                     %d\n", port->cur_flow_msg.ucDsr);
    DBG_E(MBB_ACM,"ucCts                     %d\n", port->cur_flow_msg.ucCts);
    DBG_E(MBB_ACM,"ucRts                     %d\n", port->cur_flow_msg.ucRts);
    DBG_E(MBB_ACM,"ucRi                      %d\n", port->cur_flow_msg.ucRi);
    DBG_E(MBB_ACM,"ucDcd                     %d\n", port->cur_flow_msg.ucDcd);
    DBG_E(MBB_ACM,"ucFc                      %d\n", port->cur_flow_msg.ucFc);
    DBG_E(MBB_ACM,"ucBrk                     %d\n", port->cur_flow_msg.ucBrk);
    DBG_E(MBB_ACM,"ucBrkLen                  %d\n", port->cur_flow_msg.ucBrkLen);
}

USB_VOID acm_modem_dump_evt(USB_VOID)
{
    gs_acm_evt_dump_info(&gs_modem_write_evt_manage);
    gs_acm_evt_dump_info(&gs_modem_read_evt_manage);
    gs_acm_evt_dump_info(&gs_modem_sig_stat_evt_manage);
}

USB_INT32 acm_modem_write_test(USB_UINT32 buf_size)
{
    struct sk_buff *skb_tx = NULL;
    struct gs_acm_modem_port *port = gs_acm_modem_ports[0].port;

    if ((NULL == port) || (USB_TEST_PKT_LEN_MIN > buf_size) || (USB_TEST_PKT_LEN_MAX < buf_size))
    {
        DBG_E(MBB_ACM, "wrong param\n");
        return -1;
    }
    
    skb_tx = dev_alloc_skb(buf_size);
    if (NULL == skb_tx)
    {
        DBG_E(MBB_ACM, "alloc skb failed\n");
        return -1;
    }
    
    skb_put(skb_tx, buf_size);
    memset(skb_tx->data, USB_NUM_DD, buf_size);  

    return (USB_UINT32)gs_acm_modem_write_base(port, skb_tx);
}

