/*
 * u_serial.h - interface to USB gadget "serial port"/TTY utilities
 *
 * Copyright (C) 2008 David Brownell
 * Copyright (C) 2008 by Nokia Corporation
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

/**************************************************************************************************
    问题单号          时间           修改人                      说明
**************************************************************************************************/

#ifndef __U_SERIAL_H
#define __U_SERIAL_H

#include <linux/usb/composite.h>
#include <linux/usb/cdc.h>
#include "usb_debug.h"
#include "usb_platform_comm.h"
#define U_ACM_CTRL_DTR	(1 << 0)
#define U_ACM_CTRL_RTS	(1 << 1)
#define U_ACM_CTRL_RING (1 << 3)
#define SHELL_OPEN_FLAG  2 //shell 端口使能

/*
 * One non-multiplexed "serial" I/O port ... there can be several of these
 * on any given USB peripheral device, if it provides enough endpoints.
 *
 * The "u_serial" utility component exists to do one thing:  manage TTY
 * style I/O using the USB peripheral endpoints listed here, including
 * hookups to sysfs and /dev for each logical "tty" device.
 *
 * REVISIT at least ACM could support tiocmget() if needed.
 *
 * REVISIT someday, allow multiplexing several TTYs over these endpoints.
 */
struct gserial
{
    struct usb_function		func;

    /* port is managed by gserial_{connect,disconnect} */
    USB_PVOID                    ioport;

    struct usb_ep*			in;
    struct usb_ep*			out;

    /* REVISIT avoid this CDC-ACM support harder ... */
    struct usb_cdc_line_coding port_line_coding;	/* 9600-8-N-1 etc */

    /* notification callbacks */
    USB_VOID (*connect)(struct gserial* p);
    USB_VOID (*disconnect)(struct gserial* p);
    USB_VOID (*notify_state)(struct gserial* p, USB_UINT16 state);
    USB_VOID (*flow_control)(struct gserial* p, USB_UINT rx_is_on, USB_UINT tx_is_on);
    USB_INT (*send_break)(struct gserial* p, USB_INT duration);
    
    /* notification changes to modem */
    USB_VOID (*notify_modem)(USB_PVOID gser, USB_UINT8 portno, USB_INT ctrl_bits);
};

struct acm_name_type_tbl {
    USB_PCHAR name;
    USB_INT type;//接口协议类型
};

enum acm_class_type {
    acm_class_cdev,
    acm_class_tty,
    acm_class_modem,
    acm_class_unknown           /* last item */
};

/* utilities to allocate/free request and buffer */
struct usb_request *gs_alloc_req(struct usb_ep *ep, USB_UINT len, gfp_t flags);
USB_VOID gs_free_req(struct usb_ep *, struct usb_request *req);

/* port setup/teardown is handled by gadget driver */
USB_INT gserial_setup(struct usb_gadget *g, USB_UINT n_ports);
USB_VOID gserial_cleanup(USB_VOID);

/* connect/disconnect is handled by individual functions */
USB_INT gserial_connect(struct gserial *, USB_UINT8 port_num);
USB_VOID gserial_disconnect(struct gserial *);
USB_INT gserial_line_state(struct gserial *, USB_UINT state);

USB_INT gserial_suspend(struct gserial *);
USB_INT gserial_resume(struct gserial *);

/* functions are bound to configurations by a config or gadget driver */

USB_INT gser_bind_config(struct usb_configuration *c, USB_UINT32 port_num);
USB_INT obex_bind_config(struct usb_configuration *c, USB_UINT32 port_num);

/* -------------- acm support cap --------------- */
/* max dev driver support */
#define ACM_CDEV_COUNT      8
#define ACM_TTY_COUNT       2
#define ACM_MDM_COUNT       2

/* dev count we used (the value must < ACM_XXX_COUNT) */
/* modify to 5 for cshell */
#define ACM_CDEV_USED_COUNT      7
#define ACM_TTY_USED_COUNT       1
#define ACM_MDM_USED_COUNT       1

#if ACM_CDEV_USED_COUNT >= ACM_CDEV_COUNT
    #error "ACM_CDEV_USED_COUNT error"
#endif

#if ACM_TTY_USED_COUNT >= ACM_TTY_COUNT
    #error "ACM_TTY_USED_COUNT error"
#endif

#if ACM_MDM_USED_COUNT >= ACM_MDM_COUNT
    #error "ACM_MDM_COUNT error"
#endif

/* acm config ... */
#define ACM_IS_SINGLE_INTF          1

/* acm cdev config ... */
#define ACM_CDEV_SUPPORT_NOTIFY     0

/* acm tty config ... */
#define ACM_CONSOLE_IDX     0
#define ACM_CONSOLE_NAME    "uw_tty"
#define ACM_TTY_SUPPORT_NOTIFY      0

/* we can change the console enable checker here */
#define ACM_IS_CONSOLE_ENABLE() gs_acm_is_console_enable()

/* acm modem config ... */
#define ACM_MODEM_SG_LIST_NUM       64
#define ACM_MODEM_SG_LIST_ITEM_NUM  64
#define ACM_MODEM_SIZE_THRE         640 *1024
#define ACM_MODEM_NUM_THRE          ACM_MODEM_SG_LIST_ITEM_NUM / 2
#define ACM_MODEM_TIMEOUT_THRE      0   /* jiffies */
#define ACM_MODEM_SUPPORT_NOTIFY    1

/* --- acm evt managment --- */
struct gs_acm_evt_manage {
     void* port_evt_array[ACM_CDEV_COUNT + 1];
     int port_evt_pos;
     char* name;
     spinlock_t evt_lock;
};

USB_INT usb_serial_init(USB_VOID);

static inline USB_VOID gs_acm_evt_init(struct gs_acm_evt_manage* evt, USB_CHAR* name)
{
    spin_lock_init(&evt->evt_lock);
    evt->port_evt_pos = 0;
    evt->name = name;
    memset(evt->port_evt_array, 0, sizeof(evt->port_evt_array));
}

#define gs_acm_evt_push(port, evt) __gs_acm_evt_push((USB_PVOID)port, evt)
static inline USB_VOID __gs_acm_evt_push(USB_PVOID port, struct gs_acm_evt_manage* evt)
{
    USB_ULONG flags;
    USB_INT add_new = 1;
    USB_INT i;

    spin_lock_irqsave(&evt->evt_lock, flags);
    for (i = 0; i <= evt->port_evt_pos; i++) {
        if (evt->port_evt_array[i] == port) {
            add_new = 0;
            break;
        }
    }
    if (add_new) {
        evt->port_evt_array[evt->port_evt_pos] =  port;
        evt->port_evt_pos++;
    }
    spin_unlock_irqrestore(&evt->evt_lock, flags);
}

static inline USB_PVOID gs_acm_evt_get(struct gs_acm_evt_manage* evt)
{
    USB_ULONG  flags;
    struct gs_acm_cdev_port* ret_port = NULL;

    spin_lock_irqsave(&evt->evt_lock, flags);
    if (evt->port_evt_pos > 0) {
        ret_port = evt->port_evt_array[evt->port_evt_pos-1];
        evt->port_evt_array[evt->port_evt_pos-1] = NULL;
        evt->port_evt_pos--;
    }
    spin_unlock_irqrestore(&evt->evt_lock, flags);

    return ret_port;
}

static inline USB_VOID gs_acm_evt_dump_info(struct gs_acm_evt_manage* evt)
{
    USB_INT i;
    struct gs_acm_cdev_port* port;

    pr_emerg("--- dump evt_pos:%d, name:%s ---\n",
        evt->port_evt_pos, (evt->name) ? (evt->name) : ("NULL"));
    for (i = 0; i <= evt->port_evt_pos; i++) {
        port = evt->port_evt_array[i];
        if (port) {
            pr_emerg("port[%d]_0x%x : %s\n", (u32)port,
                evt->port_evt_pos, (evt->name) ? (evt->name) : ("NULL"));
        }
    }
}

#endif /* __U_SERIAL_H */

