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
/******************************************************************************************************
    问题单号              时间         修改人                        说明
******************************************************************************************************/
#ifndef _F_ACM_H
#define _F_ACM_H
#include "hw_pnp.h"
#include "usb_platform_comm.h"

/* string descriptors: */
#define ACM_CTRL_IDX	0
#define ACM_DATA_IDX	1
#define ACM_IAD_IDX	2

/**/
struct f_acm
{
    struct gserial			port;
    USB_UINT8				ctrl_id, data_id;
    USB_UINT8				port_num;

    USB_UINT8				pending;

    /* lock is mostly for pending and notify_req ... they get accessed
     * by callbacks both from tty (open/close/break) under its spinlock,
     * and notify_req.complete() which can't use that lock.
     */
    spinlock_t			lock;

    USB_INT                     support_notify;
    struct usb_ep*			notify;
    struct usb_request*		notify_req;
    USB_INT (*pending_notify)(struct f_acm* acm);

    /* cdc vendor flow control notify */
    USB_UINT             rx_is_on;
    USB_UINT             tx_is_on;

    struct usb_cdc_line_coding	port_line_coding;	/* 8-N-1 etc */

    /* SetControlLineState request -- CDC 1.1 section 6.2.14 (INPUT) */
    USB_UINT16				port_handshake_bits;
#define ACM_CTRL_RTS	(1 << 1)	/* unused with full duplex */
#define ACM_CTRL_DTR	(1 << 0)	/* host is ready for data r/w */

    /* SerialState notification -- CDC 1.1 section 6.3.5 (OUTPUT) */
    USB_UINT16				serial_state;
#define ACM_CTRL_OVERRUN	(1 << 6)
#define ACM_CTRL_PARITY		(1 << 5)
#define ACM_CTRL_FRAMING	(1 << 4)
#define ACM_CTRL_RI		(1 << 3)
#define ACM_CTRL_BRK		(1 << 2)
#define ACM_CTRL_DSR		(1 << 1)
#define ACM_CTRL_DCD		(1 << 0)
    enum acm_class_type class_type;
    USB_INT protocal;
};



typedef struct acm_ctx
{
    struct acm_name_type_tbl* cdev_name_type_var;
    struct acm_name_type_tbl* tty_name_type_var;
    struct acm_name_type_tbl* mdm_name_type_var;
} acm_ctx_t;

USB_VOID gacm_cleanup(USB_VOID);
acm_ctx_t* acm_get_ctx(USB_VOID);
USB_VOID usb_acm_init (USB_VOID);
#endif
