/*
 * f_acm.c -- USB CDC serial (ACM) function driver
 *
 * Copyright (C) 2003 Al Borchers (alborchers@steinerpoint.com)
 * Copyright (C) 2008 by David Brownell
 * Copyright (C) 2008 by Nokia Corporation
 * Copyright (C) 2009 by Samsung Electronics
 * Author: Michal Nazarewicz (mina86@mina86.com)
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */
/**************************************************************************************************
    问题单号            时间              修改人                      说明
***************************************************************************************************/
/* #define VERBOSE_DEBUG */

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/etherdevice.h>
#include <linux/usb/composite.h>
//#include <bsp_usb.h>

#include "hw_pnp.h"
#include "usb_nv_get.h"
#include "u_serial.h"
#include "u_modem.h"
#include "u_cdev.h"
#include "f_acm.h"
#include "gadget_chips.h"

#include "usb_debug.h"
#include "usb_vendor.h"
#include "hw_pnp_adapt.h"

/*
 * This CDC ACM function support just wraps control functions and
 * notifications around the generic serial-over-usb code.
 *
 * Because CDC ACM is standardized by the USB-IF, many host operating
 * systems have drivers for it.  Accordingly, ACM is the preferred
 * interop solution for serial-port type connections.  The control
 * models are often not necessary, and in any case don't do much in
 * this bare-bones implementation.
 *
 * Note that even MS-Windows has some support for ACM.  However, that
 * support is somewhat broken because when you use ACM in a composite
 * device, having multiple interfaces confuses the poor OS.  It doesn't
 * seem to understand CDC Union descriptors.  The new "association"
 * descriptors (roughly equivalent to CDC Unions) may sometimes help.
 */


static struct acm_name_type_tbl g_acm_cdev_type_table[ACM_CDEV_COUNT] =
{
    /* name         type(prot id) */
    {"ttyGS0",          USB_IF_PROTOCOL_PCUI},
    {"acm_gps",         USB_IF_PROTOCOL_GPS},
    {"acm_4g_diag",     USB_IF_PROTOCOL_DIAG},
    {"acm_3g_diag",     USB_IF_PROTOCOL_3G_DIAG},
    {"acm_c_shell",     USB_IF_PROTOCOL_BLUETOOTH},
    {"acm_voice",       USB_IF_PROTOCOL_PCVOICE},
    {"acm_ctrl",        USB_IF_PROTOCOL_CTRL},
    {"unknown",         USB_IF_PROTOCOL_NOPNP}
};

static struct acm_name_type_tbl g_acm_tty_type_table[ACM_TTY_COUNT] =
{
    /* name             type(prot id) */
    {ACM_CONSOLE_NAME,  USB_IF_PROTOCOL_3G_GPS},
    {"unknown",         USB_IF_PROTOCOL_NOPNP}
};

static struct acm_name_type_tbl g_acm_mdm_type_table[ACM_MDM_COUNT] =
{
    /* name             type(prot id) */
    {"acm_modem",       USB_IF_PROTOCOL_MODEM},
    {"unknown",         USB_IF_PROTOCOL_NOPNP}
};
/* we can add other acm type ... */

static USB_INT g_acm_is_single_interface = ACM_IS_SINGLE_INTF;

static inline struct f_acm* func_to_acm(struct usb_function* f)
{
    return container_of(f, struct f_acm, port.func);
}

static inline struct f_acm* port_to_acm(struct gserial* p)
{
    return container_of(p, struct f_acm, port);
}

static inline USB_INT is_support_notify(enum acm_class_type type)
{
    switch (type)
    {
        case acm_class_cdev:
                return ACM_CDEV_SUPPORT_NOTIFY;
        case acm_class_tty:
            return ACM_TTY_SUPPORT_NOTIFY;
        case acm_class_modem:
            return ACM_MODEM_SUPPORT_NOTIFY;
        default:
            return 0;
    }
    return 0;
}

/*-------------------------------------------------------------------------*/

/* notification endpoint uses smallish and infrequent fixed-size messages */

#define GS_LOG2_NOTIFY_INTERVAL		5	/* 1 << 5 == 32 msec */
#define GS_NOTIFY_MAXPACKET		10	/* notification + 2 bytes */
#define ACM_IS_TTY(acm)    ((acm)->class_type == acm_class_tty)
#define ACM_IS_MDM(acm)    ((acm)->class_type == acm_class_modem)
#define ACM_IS_CDEV(acm)    ((acm)->class_type == acm_class_cdev)

/* interface and class descriptors: */

static inline USB_UINT8 ACM_GET_TYPE(struct f_acm* acm)
{
    switch (acm->class_type)
    {
        case acm_class_cdev:
            return (USB_UINT8)g_acm_cdev_type_table[acm->port_num].type;
        case acm_class_tty:
            return (USB_UINT8)g_acm_tty_type_table[acm->port_num].type;
        case acm_class_modem:
            return (USB_UINT8)g_acm_mdm_type_table[acm->port_num].type;
        default:
            return 0xFF;
    }
}

/*****************************************************************
Parameters    :  acm_desc
              class_type
Return        :
Description   :  从名字获取协议字段
*****************************************************************/
USB_INT  ACM_GET_PORT_NUM_FROM_NAME(USB_PCHAR acm_desc, USB_UINT class_type)
{
    USB_INT i = 0;

    switch (class_type)
    {
        case acm_class_cdev:
            for (i = 0; i < ACM_CDEV_COUNT; i++)
            {
                if (!strcmp(g_acm_cdev_type_table[i].name, "unknown"))
                {
                    return -1;
                }
                if (!strcmp(g_acm_cdev_type_table[i].name, acm_desc))
                {
                    return i;
                }
            }
            break;
        case acm_class_tty:
            for (i = 0; i < ACM_TTY_COUNT; i++)
            {
                if (!strcmp(g_acm_tty_type_table[i].name, "unknown"))
                {
                    return -1;
                }
                if (!strcmp(g_acm_tty_type_table[i].name, acm_desc))
                {
                    return i;
                }
            }
            break;
        case acm_class_modem:
            for (i = 0; i < ACM_MDM_COUNT; i++)
            {
                if (!strcmp(g_acm_mdm_type_table[i].name, "unknown"))
                {
                    return -1;
                }
                if (!strcmp(g_acm_mdm_type_table[i].name, acm_desc))
                {
                    return i;
                }
            }
            break;
        default:
            return -1;
    }

    return -1;
}

/*****************************************************************
Parameters    :  protocal
              class_type
Return        :
Description   :  从协议字段获取端口号
*****************************************************************/
USB_INT  ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_INT protocal, USB_UINT class_type)
{
    USB_INT i = 0;

    switch (class_type)
    {
        case acm_class_cdev:
            for (i = 0; i < ACM_CDEV_COUNT; i++)
            {
                if (USB_IF_PROTOCOL_NOPNP == g_acm_cdev_type_table[i].type)
                {
                    return -1;
                }
                if (protocal == g_acm_cdev_type_table[i].type)
                {
                    return i;
                }
            }
            break;
        case acm_class_tty:
            for (i = 0; i < ACM_TTY_COUNT; i++)
            {
                if (USB_IF_PROTOCOL_NOPNP == g_acm_tty_type_table[i].type)
                {
                    return -1;
                }
                if (protocal == g_acm_tty_type_table[i].type)
                {
                    return i;
                }
            }
            break;
        case acm_class_modem:
            for (i = 0; i < ACM_MDM_COUNT; i++)
            {
                if (USB_IF_PROTOCOL_NOPNP == g_acm_mdm_type_table[i].type)
                {
                    return -1;
                }
                if (protocal == g_acm_mdm_type_table[i].type)
                {
                    return i;
                }
            }
            break;
        default:
            return -1;
    }

    return -1;
}

/*****************************************************************
Parameters    :  acm
Return        :
Description   :  获取协议类型
*****************************************************************/
static inline USB_UINT8 ACM_GET_PROTOCOL_TYPE(struct f_acm* acm)
{
    switch (acm->class_type)
    {
        case acm_class_cdev:
            return (USB_UINT8)g_acm_cdev_type_table[acm->port_num].type;
        case acm_class_tty:
            return (USB_UINT8)g_acm_tty_type_table[acm->port_num].type;
        case acm_class_modem:
            return (USB_UINT8)g_acm_mdm_type_table[acm->port_num].type;
        default:
            return USB_IF_PROTOCOL_NOPNP;
    }
}

static USB_CHAR g_acm_dev_name[USB_CDEV_NAME_MAX] = {0};
static inline USB_PCHAR ACM_GET_NAME(struct f_acm* acm)
{
    char* ret = NULL;
    switch (acm->class_type)
    {
        case acm_class_cdev:
            snprintf(g_acm_dev_name, USB_CDEV_NAME_MAX, "/dev/%s",
                     g_acm_cdev_type_table[acm->port_num].name);
            return g_acm_dev_name;
        case acm_class_tty:
            snprintf(g_acm_dev_name, USB_CDEV_NAME_MAX, "/dev/%s%d",
                     g_acm_tty_type_table[acm->port_num].name, acm->port_num);
            return g_acm_dev_name;
        case acm_class_modem:
            snprintf(g_acm_dev_name, USB_CDEV_NAME_MAX, "/dev/%s",
                     g_acm_mdm_type_table[acm->port_num].name);
            return g_acm_dev_name;
        default:
            return ret;
    }
}

static struct usb_interface_assoc_descriptor
        acm_iad_descriptor =
{
    .bLength =		sizeof (acm_iad_descriptor),
    .bDescriptorType =	USB_DT_INTERFACE_ASSOCIATION,

    /* .bFirstInterface =	DYNAMIC, */
    .bInterfaceCount = 	2,	// control + data
    .bFunctionClass =	USB_CLASS_COMM,
    .bFunctionSubClass =	USB_CDC_SUBCLASS_ACM,
    .bFunctionProtocol =	USB_CDC_ACM_PROTO_AT_V25TER,
    /* .iFunction =		DYNAMIC */
};


static struct usb_interface_descriptor acm_control_interface_desc =
{
    .bLength =		USB_DT_INTERFACE_SIZE,
    .bDescriptorType =	USB_DT_INTERFACE,
    /* .bInterfaceNumber = DYNAMIC */
    .bNumEndpoints =	1,
    .bInterfaceClass =	USB_CLASS_COMM,
    .bInterfaceSubClass =	USB_CDC_SUBCLASS_ACM,
    .bInterfaceProtocol =	USB_CDC_ACM_PROTO_AT_V25TER,
    /* .iInterface = DYNAMIC */
};

static struct usb_interface_descriptor acm_data_interface_desc =
{
    .bLength =		USB_DT_INTERFACE_SIZE,
    .bDescriptorType =	USB_DT_INTERFACE,
    /* .bInterfaceNumber = DYNAMIC */
    .bNumEndpoints =	2,
    .bInterfaceClass =	USB_CLASS_CDC_DATA,
    .bInterfaceSubClass =	0,
    .bInterfaceProtocol =	0,
    /* .iInterface = DYNAMIC */
};

static struct usb_cdc_header_desc acm_header_desc =
{
    .bLength =		sizeof(acm_header_desc),
    .bDescriptorType =	USB_DT_CS_INTERFACE,
    .bDescriptorSubType =	USB_CDC_HEADER_TYPE,
    .bcdCDC =		cpu_to_le16(0x0110),
};

static struct usb_cdc_call_mgmt_descriptor
        acm_call_mgmt_descriptor =
{
    .bLength =		sizeof(acm_call_mgmt_descriptor),
    .bDescriptorType =	USB_DT_CS_INTERFACE,
    .bDescriptorSubType =	USB_CDC_CALL_MANAGEMENT_TYPE,
    .bmCapabilities =	0,
    /* .bDataInterface = DYNAMIC */
};

static struct usb_cdc_acm_descriptor acm_descriptor =
{
    .bLength =		sizeof(acm_descriptor),
    .bDescriptorType =	USB_DT_CS_INTERFACE,
    .bDescriptorSubType =	USB_CDC_ACM_TYPE,
    .bmCapabilities =	USB_CDC_CAP_LINE,
};

static struct usb_cdc_union_desc acm_union_desc =
{
    .bLength =		sizeof(acm_union_desc),
    .bDescriptorType =	USB_DT_CS_INTERFACE,
    .bDescriptorSubType =	USB_CDC_UNION_TYPE,
    /* .bMasterInterface0 =	DYNAMIC */
    /* .bSlaveInterface0 =	DYNAMIC */
};

static struct usb_interface_descriptor acm_single_interface_desc =
{
    .bLength =		USB_DT_INTERFACE_SIZE,
    .bDescriptorType =	USB_DT_INTERFACE,
    /* .bInterfaceNumber = DYNAMIC */
    /* .bNumEndpoints =	DYNAMIC, */
    .bInterfaceClass =	0xff,
    .bInterfaceSubClass =	0x02,
    .bInterfaceProtocol =	0x01,
    /* .iInterface = DYNAMIC */
};


/* full speed support: */

static struct usb_endpoint_descriptor acm_fs_notify_desc =
{
    .bLength =		USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =	USB_DT_ENDPOINT,
    .bEndpointAddress =	USB_DIR_IN,
    .bmAttributes =		USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =	cpu_to_le16(GS_NOTIFY_MAXPACKET),
    .bInterval =		1 << GS_LOG2_NOTIFY_INTERVAL,
};

static struct usb_endpoint_descriptor acm_fs_in_desc =
{
    .bLength =		USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =	USB_DT_ENDPOINT,
    .bEndpointAddress =	USB_DIR_IN,
    .bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor acm_fs_out_desc =
{
    .bLength =		USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =	USB_DT_ENDPOINT,
    .bEndpointAddress =	USB_DIR_OUT,
    .bmAttributes =		USB_ENDPOINT_XFER_BULK,
};

static struct usb_descriptor_header** acm_fs_cur_function;

static struct usb_descriptor_header* acm_fs_function_single[] =
{
    (struct usb_descriptor_header*)& acm_single_interface_desc,
    (struct usb_descriptor_header*)& acm_header_desc,
    (struct usb_descriptor_header*)& acm_descriptor,
    (struct usb_descriptor_header*)& acm_call_mgmt_descriptor,
    (struct usb_descriptor_header*)& acm_union_desc,
    (struct usb_descriptor_header*)& acm_fs_in_desc,
    (struct usb_descriptor_header*)& acm_fs_out_desc,
    NULL,
};

static struct usb_descriptor_header* acm_fs_function_gateway[] =
{
    (struct usb_descriptor_header*)& acm_single_interface_desc,
    (struct usb_descriptor_header*)& acm_fs_in_desc,
    (struct usb_descriptor_header*)& acm_fs_out_desc,
    NULL,
};

static struct usb_descriptor_header* acm_fs_function_single_notify[] =
{
    (struct usb_descriptor_header*)& acm_single_interface_desc,
    (struct usb_descriptor_header*)& acm_header_desc,
    (struct usb_descriptor_header*)& acm_descriptor,
    (struct usb_descriptor_header*)& acm_call_mgmt_descriptor,
    (struct usb_descriptor_header*)& acm_union_desc,
    (struct usb_descriptor_header*)& acm_fs_notify_desc,
    (struct usb_descriptor_header*)& acm_fs_in_desc,
    (struct usb_descriptor_header*)& acm_fs_out_desc,
    NULL,
};

static struct usb_descriptor_header* acm_fs_function[] =
{
    (struct usb_descriptor_header*)& acm_iad_descriptor,
    (struct usb_descriptor_header*)& acm_control_interface_desc,
    (struct usb_descriptor_header*)& acm_header_desc,
    (struct usb_descriptor_header*)& acm_call_mgmt_descriptor,
    (struct usb_descriptor_header*)& acm_descriptor,
    (struct usb_descriptor_header*)& acm_union_desc,
    (struct usb_descriptor_header*)& acm_fs_notify_desc,
    (struct usb_descriptor_header*)& acm_data_interface_desc,
    (struct usb_descriptor_header*)& acm_fs_in_desc,
    (struct usb_descriptor_header*)& acm_fs_out_desc,
    NULL,
};

/* high speed support: */

static struct usb_endpoint_descriptor acm_hs_notify_desc =
{
    .bLength =		USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =	USB_DT_ENDPOINT,
    .bEndpointAddress =	USB_DIR_IN,
    .bmAttributes =		USB_ENDPOINT_XFER_INT,
    .wMaxPacketSize =	cpu_to_le16(GS_NOTIFY_MAXPACKET),
    .bInterval =		GS_LOG2_NOTIFY_INTERVAL + 4,
};

static struct usb_endpoint_descriptor acm_hs_in_desc =
{
    .bLength =		USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =	USB_DT_ENDPOINT,
    .bmAttributes =		USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =	cpu_to_le16(512),
};

static struct usb_endpoint_descriptor acm_hs_out_desc =
{
    .bLength =		USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =	USB_DT_ENDPOINT,
    .bmAttributes =		USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =	cpu_to_le16(512),
};

static struct usb_descriptor_header** acm_hs_cur_function;

static struct usb_descriptor_header* acm_hs_function_single[] =
{
    (struct usb_descriptor_header*)& acm_single_interface_desc,
    (struct usb_descriptor_header*)& acm_header_desc,
    (struct usb_descriptor_header*)& acm_descriptor,
    (struct usb_descriptor_header*)& acm_call_mgmt_descriptor,
    (struct usb_descriptor_header*)& acm_union_desc,
    (struct usb_descriptor_header*)& acm_hs_in_desc,
    (struct usb_descriptor_header*)& acm_hs_out_desc,
    NULL,
};

static struct usb_descriptor_header* acm_hs_function_gateway[] =
{
    (struct usb_descriptor_header*)& acm_single_interface_desc,
    (struct usb_descriptor_header*)& acm_hs_in_desc,
    (struct usb_descriptor_header*)& acm_hs_out_desc,
    NULL,
};

static struct usb_descriptor_header* acm_hs_function_single_notify[] =
{
    (struct usb_descriptor_header*)& acm_single_interface_desc,
    (struct usb_descriptor_header*)& acm_header_desc,
    (struct usb_descriptor_header*)& acm_descriptor,
    (struct usb_descriptor_header*)& acm_call_mgmt_descriptor,
    (struct usb_descriptor_header*)& acm_union_desc,
    (struct usb_descriptor_header*)& acm_hs_notify_desc,
    (struct usb_descriptor_header*)& acm_hs_in_desc,
    (struct usb_descriptor_header*)& acm_hs_out_desc,
    NULL,
};

static struct usb_descriptor_header* acm_hs_function[] =
{
    (struct usb_descriptor_header*)& acm_iad_descriptor,
    (struct usb_descriptor_header*)& acm_control_interface_desc,
    (struct usb_descriptor_header*)& acm_header_desc,
    (struct usb_descriptor_header*)& acm_call_mgmt_descriptor,
    (struct usb_descriptor_header*)& acm_descriptor,
    (struct usb_descriptor_header*)& acm_union_desc,
    (struct usb_descriptor_header*)& acm_hs_notify_desc,
    (struct usb_descriptor_header*)& acm_data_interface_desc,
    (struct usb_descriptor_header*)& acm_hs_in_desc,
    (struct usb_descriptor_header*)& acm_hs_out_desc,
    NULL,
};


static struct usb_endpoint_descriptor acm_ss_in_desc =
{
    .bLength =		USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =	USB_DT_ENDPOINT,
    .bmAttributes =		USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_endpoint_descriptor acm_ss_out_desc =
{
    .bLength =		USB_DT_ENDPOINT_SIZE,
    .bDescriptorType =	USB_DT_ENDPOINT,
    .bmAttributes =		USB_ENDPOINT_XFER_BULK,
    .wMaxPacketSize =	cpu_to_le16(1024),
};

static struct usb_ss_ep_comp_descriptor acm_ss_bulk_comp_desc =
{
    .bLength =              sizeof acm_ss_bulk_comp_desc,
    .bDescriptorType =      USB_DT_SS_ENDPOINT_COMP,
};

static struct usb_descriptor_header** acm_ss_cur_function;

static struct usb_descriptor_header* acm_ss_function[] =
{
    (struct usb_descriptor_header*)& acm_iad_descriptor,
    (struct usb_descriptor_header*)& acm_control_interface_desc,
    (struct usb_descriptor_header*)& acm_header_desc,
    (struct usb_descriptor_header*)& acm_call_mgmt_descriptor,
    (struct usb_descriptor_header*)& acm_descriptor,
    (struct usb_descriptor_header*)& acm_union_desc,
    (struct usb_descriptor_header*)& acm_hs_notify_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    (struct usb_descriptor_header*)& acm_data_interface_desc,
    (struct usb_descriptor_header*)& acm_ss_in_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    (struct usb_descriptor_header*)& acm_ss_out_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    NULL,
};

static struct usb_descriptor_header* acm_ss_function_single[] =
{
    (struct usb_descriptor_header*)& acm_single_interface_desc,
    (struct usb_descriptor_header*)& acm_header_desc,
    (struct usb_descriptor_header*)& acm_descriptor,
    (struct usb_descriptor_header*)& acm_call_mgmt_descriptor,
    (struct usb_descriptor_header*)& acm_union_desc,
    (struct usb_descriptor_header*)& acm_ss_in_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    (struct usb_descriptor_header*)& acm_ss_out_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    NULL,
};

static struct usb_descriptor_header* acm_ss_function_gateway[] =
{
    (struct usb_descriptor_header*)& acm_single_interface_desc,
    (struct usb_descriptor_header*)& acm_ss_in_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    (struct usb_descriptor_header*)& acm_ss_out_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    NULL,
};

static struct usb_descriptor_header* acm_ss_function_single_notify[] =
{
    (struct usb_descriptor_header*)& acm_single_interface_desc,
    (struct usb_descriptor_header*)& acm_header_desc,
    (struct usb_descriptor_header*)& acm_descriptor,
    (struct usb_descriptor_header*)& acm_call_mgmt_descriptor,
    (struct usb_descriptor_header*)& acm_union_desc,
    (struct usb_descriptor_header*)& acm_hs_notify_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    (struct usb_descriptor_header*)& acm_ss_in_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    (struct usb_descriptor_header*)& acm_ss_out_desc,
    (struct usb_descriptor_header*)& acm_ss_bulk_comp_desc,
    NULL,
};



/* static strings, in UTF-8 */
static struct usb_string acm_string_defs[] =
{
    [ACM_CTRL_IDX].s = "CDC Abstract Control Model (ACM)",
    [ACM_DATA_IDX].s = "CDC ACM Data",
    [ACM_IAD_IDX ].s = "CDC Serial",
    {  /* ZEROES END LIST */ },
};

static struct usb_gadget_strings acm_string_table =
{
    .language =		0x0409,	/* en-us */
    .strings =		acm_string_defs,
};

static struct usb_gadget_strings* acm_strings[] =
{
    &acm_string_table,
    NULL,
};

/*-------------------------------------------------------------------------*/

/* ACM control ... data handling is delegated to tty library code.
 * The main task of this function is to activate and deactivate
 * that code based on device state; track parameters like line
 * speed, handshake state, and so on; and issue notifications.
 */

static USB_VOID acm_complete_set_line_coding(struct usb_ep* ep,
        struct usb_request* req)
{
    struct f_acm*	acm = ep->driver_data;

    if (req->status != 0)
    {
        DBG_I(MBB_ACM, "acm ttyGS%d completion, err %d\n",
              acm->port_num, req->status);
        return;
    }

    /* normal completion */
    if (req->actual != sizeof(acm->port_line_coding))
    {
        DBG_I(MBB_ACM, "acm ttyGS%d short resp, len %d\n",
              acm->port_num, req->actual);
        usb_ep_set_halt(ep);
    }
    else
    {
        struct usb_cdc_line_coding*	value = req->buf;

        /* REVISIT:  we currently just remember this data.
         * If we change that, (a) validate it first, then
         * (b) update whatever hardware needs updating,
         * (c) worry about locking.  This is information on
         * the order of 9600-8-N-1 ... most of which means
         * nothing unless we control a real RS232 line.
         */
        acm->port_line_coding = *value;
    }
}

static USB_INT acm_setup(struct usb_function* f, const struct usb_ctrlrequest* ctrl)
{
    struct f_acm*		acm = func_to_acm(f);
    struct usb_composite_dev* cdev = f->config->cdev;
    struct usb_request*	req = cdev->req;
    USB_INT			value = -EOPNOTSUPP;
    USB_UINT16			w_index = le16_to_cpu(ctrl->wIndex);
    USB_UINT16			w_value = le16_to_cpu(ctrl->wValue);
    USB_UINT16			w_length = le16_to_cpu(ctrl->wLength);

    /* composite driver infrastructure handles everything except
     * CDC class messages; interface activation uses set_alt().
     *
     * Note CDC spec table 4 lists the ACM request profile.  It requires
     * encapsulated command support ... we don't handle any, and respond
     * to them by stalling.  Options include get/set/clear comm features
     * (not that useful) and SEND_BREAK.
     */
    switch ((ctrl->bRequestType << 8) | ctrl->bRequest)
    {

            /* SET_LINE_CODING ... just read and save what the host sends */
        case ((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                | USB_CDC_REQ_SET_LINE_CODING:
            if (w_length != sizeof(struct usb_cdc_line_coding)
                || w_index != acm->ctrl_id)
            { goto invalid; }

            value = w_length;
            cdev->gadget->ep0->driver_data = acm;
            req->complete = acm_complete_set_line_coding;
            break;

            /* GET_LINE_CODING ... return what host sent, or initial value */
        case ((USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                | USB_CDC_REQ_GET_LINE_CODING:
            if (w_index != acm->ctrl_id)
            { goto invalid; }

            value = min_t(unsigned, w_length,
                          sizeof(struct usb_cdc_line_coding));
            memcpy(req->buf, &acm->port_line_coding, value);
            break;

            /* SET_CONTROL_LINE_STATE ... save what the host sent */
        case ((USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE) << 8)
                | USB_CDC_REQ_SET_CONTROL_LINE_STATE:
            if (w_index != (u16)acm->ctrl_id)
            { goto invalid; }

            value = 0;

            /* FIXME we should not allow data to flow until the
             * host sets the ACM_CTRL_DTR bit; and when it clears
             * that bit, we should return to that no-flow state.
             */
            acm->port_handshake_bits = w_value;
            if (ACM_IS_TTY(acm))
            { gserial_line_state(&acm->port, (u32)w_value); }
            else if (ACM_IS_MDM(acm))
            { gacm_modem_line_state(&acm->port, (u32)w_value); }
            else
            { gacm_cdev_line_state(&acm->port, (u32)w_value); }
#ifdef MBB_USB_UNITARY_Q
            if (acm->port.notify_modem)
            {
                acm->port.notify_modem(&acm->port, 0, w_value);
            }
#endif /*MBB_USB_UNITARY_Q */
            break;

        default:
        invalid:
            DBG_E(MBB_ACM, "invalid control req%02x.%02x v%04x i%04x l%d\n",
                  ctrl->bRequestType, ctrl->bRequest,
                  w_value, w_index, w_length);
    }

    /* respond with data transfer or status phase? */
    if (value >= 0)
    {
        DBG_E(MBB_ACM, "acm ttyGS%d req%02x.%02x v%04x i%04x l%d\n",
              acm->port_num, ctrl->bRequestType, ctrl->bRequest,
              w_value, w_index, w_length);
        req->zero = 0;
        req->length = value;
        value = usb_ep_queue(cdev->gadget->ep0, req, GFP_ATOMIC);
        if (value < 0)
            DBG_E(MBB_ACM, "acm response on ttyGS%d, err %d\n",
                  acm->port_num, value);
    }

    /* device either stalls (value < 0) or reports success */
    return value;
}

static USB_INT acm_set_alt(struct usb_function* f, USB_UINT intf, USB_UINT alt)
{
    struct f_acm*		acm = func_to_acm(f);
    struct usb_composite_dev* cdev = f->config->cdev;
    bool is_setting = 0;
    USB_INT iRet = 0;

    /* we know alt == 0, so this is an activation or a reset */

    /* if it is single interface, intf, acm->ctrl_id and acm->data_id
     * are the same, so we can setting data and notify interface in the same time.
     *
     * if it is multi interface, acm->ctrl_id and acm->data_id are different,
     * so the setting is go ahead in different times.
     */
    if (intf == acm->ctrl_id)
    {
        is_setting = 1;
        if (acm->notify)
        {
            if (acm->notify->driver_data)
            {
                DBG_I(MBB_ACM, "reset acm control interface %d\n", intf);
                usb_ep_disable(acm->notify);
            }
            else
            {
                DBG_I(MBB_ACM, "init acm ctrl interface %d\n", intf);
                if (config_ep_by_speed(cdev->gadget, f, acm->notify))
                { return -EINVAL; }
            }
            iRet = usb_ep_enable(acm->notify);
            if (iRet < 0)
            {
                return -EINVAL;
            }

            acm->notify->driver_data = acm;
        }
    }

    if (intf == acm->data_id)
    {
        is_setting = 1;
        if (acm->port.in->driver_data)
        {
            DBG_I(MBB_ACM, "reset acm ttyGS%d\n", acm->port_num);
            if (ACM_IS_TTY(acm))
            { gserial_disconnect(&acm->port); }
            if (ACM_IS_MDM(acm))
            { gacm_modem_disconnect(&acm->port); }
            else
            { gacm_cdev_disconnect(&acm->port); }
        }
        if (!acm->port.in->desc || !acm->port.out->desc)
        {
            DBG_I(MBB_ACM, "activate acm ttyGS%d\n", acm->port_num);
            if (config_ep_by_speed(cdev->gadget, f,
                                   acm->port.in) ||
                config_ep_by_speed(cdev->gadget, f,
                                   acm->port.out))
            {
                acm->port.in->desc = NULL;
                acm->port.out->desc = NULL;
                return -EINVAL;
            }
        }
        if (ACM_IS_TTY(acm))
        {
            gserial_connect(&acm->port, acm->port_num);
        }
        else if (ACM_IS_MDM(acm))
        {
            gacm_modem_connect(&acm->port, acm->port_num);

        }
        else if (ACM_IS_CDEV(acm))
        {
            gacm_cdev_connect(&acm->port, acm->port_num);
        }

#ifdef MBB_USB_UNITARY_Q
        gsmd_connect(&acm->port, 0);
#endif/*MBB_USB_UNITARY_Q*/

        bsp_usb_set_enum_stat(acm->data_id, 1);
    }

    if (!is_setting)
    { return -EINVAL; }

    return 0;
}

static USB_VOID acm_disable(struct usb_function* f)
{
    struct f_acm*	acm = func_to_acm(f);
    DBG_I(MBB_ACM, "acm ttyGS%d deactivated\n", acm->port_num);
    if (ACM_IS_TTY(acm))
    { gserial_disconnect(&acm->port); }
    else if (ACM_IS_MDM(acm))
    { gacm_modem_disconnect(&acm->port); }
    else
    { gacm_cdev_disconnect(&acm->port); }
    if (acm->notify)
    {
        usb_ep_disable(acm->notify);
        acm->notify->driver_data = NULL;
    }
    bsp_usb_set_enum_stat(acm->data_id, 0);
}

USB_VOID acm_suspend(struct usb_function* f)
{
    struct f_acm* acm = func_to_acm(f);

    if (ACM_IS_TTY(acm))
    { gserial_suspend(&acm->port); }
    else if (ACM_IS_MDM(acm))
    { gacm_modem_suspend(&acm->port); }
    else
    { gacm_cdev_suspend(&acm->port); }
}

USB_VOID acm_resume(struct usb_function* f)
{
    struct f_acm* acm = func_to_acm(f);

    if (ACM_IS_TTY(acm))
    { gserial_resume(&acm->port); }
    else if (ACM_IS_MDM(acm))
    { gacm_modem_resume(&acm->port); }
    else
    { gacm_cdev_resume(&acm->port); }
}

/*-------------------------------------------------------------------------*/

/**
 * acm_cdc_notify - issue CDC notification to host
 * @acm: wraps host to be notified
 * @type: notification type
 * @value: Refer to cdc specs, wValue field.
 * @data: data to be sent
 * @length: size of data
 * Context: irqs blocked, acm->lock held, acm_notify_req non-null
 *
 * Returns zero on success or a negative errno.
 *
 * See section 6.3.5 of the CDC 1.1 specification for information
 * about the only notification we issue:  SerialState change.
 */
static USB_INT acm_cdc_notify(struct f_acm* acm, USB_UINT8 type, USB_UINT16 value,
                              USB_PVOID data, USB_UINT length, bool is_vendor)
{
    struct usb_ep*			ep = acm->notify;
    struct usb_request*		req;
    struct usb_cdc_notification*	notify;
    const USB_UINT			len = sizeof(*notify) + length;
    USB_VOID*				buf;
    USB_INT				status;

    req = acm->notify_req;
    acm->notify_req = NULL;
    acm->pending = false;

    req->length = len;
    notify = req->buf;
    buf = notify + 1;

    notify->bmRequestType = USB_DIR_IN
                            | (is_vendor ? USB_TYPE_VENDOR : USB_TYPE_CLASS)
                            | USB_RECIP_INTERFACE;
    notify->bNotificationType = type;
    notify->wValue = cpu_to_le16(value);
    notify->wIndex = cpu_to_le16(acm->ctrl_id);
    notify->wLength = cpu_to_le16(length);
    if (length && data)
    {
        memcpy(buf, data, length);
    }

    /* ep_queue() can complete immediately if it fills the fifo... */
    spin_unlock(&acm->lock);
    status = usb_ep_queue(ep, req, GFP_ATOMIC);
    spin_lock(&acm->lock);

    if (status < 0)
    {

        DBG_E(MBB_ACM,
              "acm ttyGS%d can't notify serial state, %d\n",
              acm->port_num, status);
        acm->notify_req = req;
    }

    return status;
}

static USB_INT acm_notify_serial_state(struct f_acm* acm)
{
    USB_INT     status;

    spin_lock(&acm->lock);
    if (acm->notify_req)
    {
        DBG_I(MBB_ACM, "acm ttyGS%d serial state %04x\n",
              acm->port_num, acm->serial_state);
        status = acm_cdc_notify(acm, USB_CDC_NOTIFY_SERIAL_STATE,
                                0, &acm->serial_state, sizeof(acm->serial_state), 0);
    }
    else
    {
        acm->pending = true;
        acm->pending_notify = acm_notify_serial_state;
        status = 0;
    }
    spin_unlock(&acm->lock);
    return status;
}



static USB_INT acm_notify_flow_control(struct f_acm* acm)
{
    USB_INT status;
    USB_UINT16 value = (acm->rx_is_on ? 0x1 : 0x0) | (acm->tx_is_on ? 0x2 : 0x0);

    spin_lock(&acm->lock);
    if (acm->notify_req)
    {
        status = acm_cdc_notify(acm, USB_CDC_VENDOR_NTF_FLOW_CONTROL,
                                value, NULL, 0, 1);
    }
    else
    {
        acm->pending = true;
        acm->pending_notify = acm_notify_flow_control;
        status = 0;
    }
    spin_unlock(&acm->lock);
    return status;
}

static USB_VOID acm_cdc_notify_complete(struct usb_ep* ep, struct usb_request* req)
{
    struct f_acm*		acm = req->context;
    USB_UINT8			doit = false;

    /* on this call path we do NOT hold the port spinlock,
     * which is why ACM needs its own spinlock
     */
    spin_lock(&acm->lock);
    if (req->status != -ESHUTDOWN)
    { doit = acm->pending; }
    acm->notify_req = req;
    spin_unlock(&acm->lock);

    if (doit && acm->pending_notify)
    { acm->pending_notify(acm); }
}

/* connect == the TTY link is open */

static USB_VOID acm_connect(struct gserial* port)
{
    struct f_acm*		acm = port_to_acm(port);

    acm->serial_state |= ACM_CTRL_DSR | ACM_CTRL_DCD;
    (USB_VOID)acm_notify_serial_state(acm);
}

static USB_VOID acm_disconnect(struct gserial* port)
{
    struct f_acm*		acm = port_to_acm(port);

    acm->serial_state &= ~(ACM_CTRL_DSR | ACM_CTRL_DCD);
    (USB_VOID)acm_notify_serial_state(acm);
}

static USB_VOID acm_notify_state(struct gserial* port, u16 state)
{
    struct f_acm*		acm = port_to_acm(port);

    acm->serial_state = state;
    (USB_VOID)acm_notify_serial_state(acm);
}

static USB_VOID acm_flow_control(struct gserial* port, USB_UINT rx_is_on, USB_UINT tx_is_on)
{
    struct f_acm*		acm = port_to_acm(port);

    acm->rx_is_on = rx_is_on;
    acm->tx_is_on = tx_is_on;
    (USB_VOID)acm_notify_flow_control(acm);
}

static USB_INT acm_send_break(struct gserial* port, USB_INT duration)
{
    struct f_acm*		acm = port_to_acm(port);
    USB_UINT16			state;

    state = acm->serial_state;
    state &= ~ACM_CTRL_BRK;
    if (duration)
    { state |= ACM_CTRL_BRK; }

    acm->serial_state = state;
    return acm_notify_serial_state(acm);
}

/*-------------------------------------------------------------------------*/

/* ACM function driver setup/binding */
static USB_INT
acm_bind(struct usb_configuration* c, struct usb_function* f)
{
    struct usb_composite_dev* cdev = c->cdev;
    struct f_acm*		acm = func_to_acm(f);
    USB_INT			status;
    struct usb_ep*		ep;

    /* allocate instance-specific interface IDs, and patch descriptors */
    status = usb_interface_id(c, f);
    if (status < 0)
    { goto fail; }

    if (g_acm_is_single_interface)
    {
        acm->ctrl_id = acm->data_id = status;
        acm_single_interface_desc.bInterfaceNumber = status;
        acm_call_mgmt_descriptor.bDataInterface = status;
    }
    else
    {
        acm->ctrl_id = status;
        acm_iad_descriptor.bFirstInterface = status;

        acm_control_interface_desc.bInterfaceNumber = status;
        acm_union_desc .bMasterInterface0 = status;

        status = usb_interface_id(c, f);
        if (status < 0)
        { goto fail; }
        acm->data_id = status;

        acm_data_interface_desc.bInterfaceNumber = status;
        acm_union_desc.bSlaveInterface0 = status;
        acm_call_mgmt_descriptor.bDataInterface = status;
    }
    bsp_usb_add_setup_dev((unsigned)acm->data_id);

    /* don't need set for tty dev */
    if (!ACM_IS_TTY(acm))
    {
        bsp_usb_set_last_cdev_name(ACM_GET_NAME(acm));
    }

    status = -ENODEV;

    /* allocate instance-specific endpoints */
    ep = usb_ep_autoconfig(cdev->gadget, &acm_fs_in_desc);
    if (!ep)
    { goto fail; }
    acm->port.in = ep;
    ep->driver_data = cdev;	/* claim */

    ep = usb_ep_autoconfig(cdev->gadget, &acm_fs_out_desc);
    if (!ep)
    { goto fail; }
    acm->port.out = ep;
    ep->driver_data = cdev;	/* claim */

    if (acm->support_notify)
    {
        ep = usb_ep_autoconfig(cdev->gadget, &acm_fs_notify_desc);
        if (!ep)
        { goto fail; }
        acm->notify = ep;
        ep->driver_data = cdev;	/* claim */

        /* allocate notification */
        acm->notify_req = gs_alloc_req(ep,
                                       sizeof(struct usb_cdc_notification) + 2,
                                       GFP_KERNEL);
        if (!acm->notify_req)
        { goto fail; }

        acm->notify_req->complete = acm_cdc_notify_complete;
        acm->notify_req->context = acm;
    }
    else
    {
        acm->notify = NULL;
        acm->notify_req = NULL;
    }

    /* copy descriptors */
    f->descriptors = usb_copy_descriptors(acm_fs_cur_function);
    if (!f->descriptors)
    { goto fail; }

    /* support all relevant hardware speeds... we expect that when
     * hardware is dual speed, all bulk-capable endpoints work at
     * both speeds
     */
    if (gadget_is_dualspeed(c->cdev->gadget))
    {
        acm_hs_in_desc.bEndpointAddress =
            acm_fs_in_desc.bEndpointAddress;
        acm_hs_out_desc.bEndpointAddress =
            acm_fs_out_desc.bEndpointAddress;

        if (acm->support_notify)
        {
            acm_hs_notify_desc.bEndpointAddress =
                acm_fs_notify_desc.bEndpointAddress;
        }

        /* copy descriptors */
        f->hs_descriptors = usb_copy_descriptors(acm_hs_cur_function);
    }

    if (gadget_is_superspeed(c->cdev->gadget))
    {
        acm_ss_in_desc.bEndpointAddress =
            acm_fs_in_desc.bEndpointAddress;
        acm_ss_out_desc.bEndpointAddress =
            acm_fs_out_desc.bEndpointAddress;

        if (acm->support_notify)
        {
            acm_hs_notify_desc.bEndpointAddress =
                acm_fs_notify_desc.bEndpointAddress;
        }

        /* copy descriptors, and track endpoint copies */
        f->ss_descriptors = usb_copy_descriptors(acm_ss_cur_function);
        if (!f->ss_descriptors)
        { goto fail; }
    }
    /*
    DBG(cdev, "acm ttyGS%d: %s speed IN/%s OUT/%s NOTIFY/%s\n",
    		acm->port_num,
    		gadget_is_superspeed(c->cdev->gadget) ? "super" :
    		gadget_is_dualspeed(c->cdev->gadget) ? "dual" : "full",
    		acm->port.in->name, acm->port.out->name,
    		(acm->notify) ? acm->notify->name : "no_notify");
               */
    DBG_I(MBB_ACM, "acm ttyGS%d: %s speed IN/%s OUT/%s NOTIFY/%s\n",
          acm->port_num,
          gadget_is_superspeed(c->cdev->gadget) ? "super" :
          gadget_is_dualspeed(c->cdev->gadget) ? "dual" : "full",
          acm->port.in->name, acm->port.out->name,
          (acm->notify) ? acm->notify->name : "no_notify");
    return 0;

fail:
    if (acm->notify_req)
    { gs_free_req(acm->notify, acm->notify_req); }

    /* we might as well release our claims on endpoints */
    if (acm->notify)
    { acm->notify->driver_data = NULL; }
    if (acm->port.out)
    { acm->port.out->driver_data = NULL; }
    if (acm->port.in)
    { acm->port.in->driver_data = NULL; }

    ERROR(cdev, "%s/%p: can't bind, err %d\n", f->name, f, status);

    return status;
}

static USB_VOID
acm_unbind(struct usb_configuration* c, struct usb_function* f)
{
    struct f_acm*		acm = func_to_acm(f);

    /* clear the string id index */
    if (acm_string_defs[ACM_CTRL_IDX].id)
    {
        acm_string_defs[ACM_CTRL_IDX].id = 0;
        acm_string_defs[ACM_DATA_IDX].id = 0;
        acm_string_defs[ACM_IAD_IDX].id = 0;
    }

    if (gadget_is_dualspeed(c->cdev->gadget))
    { usb_free_descriptors(f->hs_descriptors); }
    if (gadget_is_superspeed(c->cdev->gadget))
    { usb_free_descriptors(f->ss_descriptors); }
    usb_free_descriptors(f->descriptors);
    if (acm->notify)
    { gs_free_req(acm->notify, acm->notify_req); }
    kfree(acm);
}

/* Some controllers can't support CDC ACM ... */
static inline bool can_support_cdc(struct usb_configuration* c)
{
    /* everything else is *probably* fine ... */
    return true;
}



static inline USB_VOID acm_set_config_vendor(struct f_acm* acm)
{

    mbb_usb_nv_info_st* usb_nv_ctx = usb_nv_get_ctx();
    if (g_acm_is_single_interface)
    {
        if (acm->support_notify)
        {
            /* bulk in + bulk out + interrupt in */
            acm_single_interface_desc.bNumEndpoints = USB_NUM_3;
            acm_fs_cur_function = acm_fs_function_single_notify;
            acm_hs_cur_function = acm_hs_function_single_notify;
            acm_ss_cur_function = acm_ss_function_single_notify;
        }
        else
        {
            acm_single_interface_desc.bNumEndpoints = USB_NUM_2;
            /* bulk in + bulk out */
            if (GATEWAY_OS_FLAG == usb_pnp_system_type_get())
            {
                acm_fs_cur_function = acm_fs_function_gateway;
                acm_hs_cur_function = acm_hs_function_gateway;
                acm_ss_cur_function = acm_ss_function_gateway;
            }
            else
            {
                acm_fs_cur_function = acm_fs_function_single;
                acm_hs_cur_function = acm_hs_function_single;
                acm_ss_cur_function = acm_ss_function_single;
            }
        }
        if (usb_pnp_port_style_stat())
        {
            acm_single_interface_desc.bInterfaceClass =    VENDOR_PRO;
            acm_single_interface_desc.bInterfaceSubClass = VENDOR_PRO;
            acm_single_interface_desc.bInterfaceProtocol = VENDOR_PRO;
        }
        else
        {
            acm_single_interface_desc.bInterfaceClass = VENDOR_PRO;
            if ((LINUX_OS_FLAG == usb_pnp_system_type_get())
                || (GATEWAY_OS_FLAG == usb_pnp_system_type_get()))
            {
                acm_single_interface_desc.bInterfaceSubClass = USB_SUBCLASS_LINUX;
            }
            else
            {
                acm_single_interface_desc.bInterfaceSubClass = USB_SUBCLASS_CODE;
            }


            if (0 != acm->protocal)
            {
                acm_single_interface_desc.bInterfaceProtocol = acm->protocal;
            }
            else
            {
                acm_single_interface_desc.bInterfaceProtocol = ACM_GET_PROTOCOL_TYPE(acm);
            }
        }
    }
    else
    {
        if (usb_pnp_port_style_stat())
        {
            acm_control_interface_desc.bInterfaceClass =    VENDOR_PRO;
            acm_control_interface_desc.bInterfaceSubClass = VENDOR_PRO;
            acm_control_interface_desc.bInterfaceProtocol = VENDOR_PRO;
        }
        else
        {
            acm_control_interface_desc.bInterfaceClass = VENDOR_PRO;
            if ((LINUX_OS_FLAG == usb_pnp_system_type_get())
                || (GATEWAY_OS_FLAG == usb_pnp_system_type_get()))
            {
                acm_single_interface_desc.bInterfaceSubClass = USB_SUBCLASS_LINUX;
            }
            else
            {
                acm_control_interface_desc.bInterfaceSubClass = USB_SUBCLASS_CODE;
            }

            if (0 != acm->protocal)
            {
                acm_control_interface_desc.bInterfaceProtocol = acm->protocal;
            }
            else
            {
                acm_control_interface_desc.bInterfaceProtocol = ACM_GET_PROTOCOL_TYPE(acm);
            }
        }

        acm_fs_cur_function = acm_fs_function;
        acm_hs_cur_function = acm_hs_function;
        acm_ss_cur_function = acm_ss_function;

    }
}

/**
 * acm_bind_config - add a CDC ACM function to a configuration
 * @c: the configuration to support the CDC ACM instance
 * @port_num: /dev/ttyGS* port this interface will use
 * Context: single threaded during gadget setup
 *
 * Returns zero on success, else negative errno.
 *
 * Caller must have called @gserial_setup() with enough ports to
 * handle all the ones it binds.  Caller is also responsible
 * for calling @gserial_cleanup() before module unload.
 */
USB_INT acm_bind_config(struct usb_configuration* c, USB_UINT port_num,
                        enum acm_class_type type, struct android_usb_function* f)
{
    struct f_acm*	acm = NULL;
    USB_INT		status;

    if (!can_support_cdc(c))
    { return -EINVAL; }

    /* REVISIT might want instance-specific strings to help
     * distinguish instances ...
     */

    /* maybe allocate device-global string IDs, and patch descriptors */
    if (acm_string_defs[ACM_CTRL_IDX].id == 0)
    {
        status = usb_string_id(c->cdev);
        if (status < 0)
        { return status; }
        acm_string_defs[ACM_CTRL_IDX].id = status;

        acm_control_interface_desc.iInterface = status;

        status = usb_string_id(c->cdev);
        if (status < 0)
        { return status; }
        acm_string_defs[ACM_DATA_IDX].id = status;

        acm_data_interface_desc.iInterface = status;

        status = usb_string_id(c->cdev);
        if (status < 0)
        { return status; }
        acm_string_defs[ACM_IAD_IDX].id = status;

        acm_iad_descriptor.iFunction = status;
        //需要讨论
        //acm_single_interface_desc.iInterface = status;
    }

    /* allocate and initialize one new instance */
    acm = kzalloc(sizeof * acm, GFP_KERNEL);
    if (!acm)
    { return -ENOMEM; }

    spin_lock_init(&acm->lock);

    acm->port_num = port_num;
    acm->class_type = type;
    acm->support_notify = is_support_notify(type);

    acm->port.connect = acm_connect;
    acm->port.disconnect = acm_disconnect;
    acm->port.notify_state = acm_notify_state;
    acm->port.flow_control = acm_flow_control;
    acm->port.send_break = acm_send_break;

    acm->port.func.name = "acm";
    acm->port.func.strings = acm_strings;
    /* descriptors are per-instance copies */
    acm->port.func.bind = acm_bind;
    acm->port.func.unbind = acm_unbind;
    acm->port.func.set_alt = acm_set_alt;
    acm->port.func.setup = acm_setup;
    acm->port.func.disable = acm_disable;
    acm->port.func.suspend = acm_suspend;
    acm->port.func.resume = acm_resume;

    if (NULL != f)
    {
        acm->protocal = f->property;
    }

    acm_set_config_vendor(acm);

    status = usb_add_function(c, &acm->port.func);
    if (status)
    { kfree(acm); }
    return status;
}



static acm_ctx_t  acm_ctx_var =
{
    .cdev_name_type_var = ( struct acm_name_type_tbl* )& g_acm_cdev_type_table,
    .tty_name_type_var = (struct acm_name_type_tbl*)& g_acm_tty_type_table,
    .mdm_name_type_var = (struct acm_name_type_tbl*)& g_acm_mdm_type_table,
};

acm_ctx_t*  acm_get_ctx(void)
{
    return &acm_ctx_var;
}


static USB_UINT n_acm_cdev_ports = ACM_CDEV_USED_COUNT;
static USB_UINT n_acm_tty_ports = ACM_TTY_USED_COUNT;
static USB_UINT n_acm_mdm_ports = ACM_MDM_USED_COUNT;
static USB_UINT n_acm_ports_init = 0;

#define MAX_ACM_INSTANCES 4
struct acm_function_config
{
    USB_INT instances;
};


static USB_INT acm_function_init(struct android_usb_function* f, struct usb_composite_dev* cdev)
{
    USB_INT status;

    f->config = kzalloc(sizeof(struct acm_function_config), GFP_KERNEL);
    if (!f->config)
    {
        return  ( - ENOMEM);
    }

    if (n_acm_ports_init > 0)
    {
        DBG_E(MBB_ACM, "acm_function_init has been init\n");
        return 0;
    }
    else
    {
        n_acm_ports_init++;

    }
    if (n_acm_tty_ports > 0)
    {
        status = gserial_setup(cdev->gadget, n_acm_tty_ports);
        if (status < 0)
        { return status; }
    }
    if (n_acm_cdev_ports > 0)
    {
        status = gacm_cdev_setup(cdev->gadget, n_acm_cdev_ports);
        if (status < 0)
        {
            goto fail_clean_tty;
        }
    }
    if (n_acm_mdm_ports > 0)
    {
        status = gacm_modem_setup(cdev->gadget, n_acm_mdm_ports);
        if (status < 0)
        {
            goto fail_clean_cdev;
        }
    }
    return 0;

fail_clean_cdev:
    if (n_acm_cdev_ports > 0)
    {
        gacm_cdev_cleanup();
    }

fail_clean_tty:
    if (n_acm_tty_ports > 0)
    {
        gserial_cleanup();
    }
    return status;
}

static USB_VOID acm_function_cleanup(struct android_usb_function* f)
{
#if 0
    if (n_acm_tty_ports > 0)
    { gserial_cleanup(); }

    if (n_acm_cdev_ports > 0)
    { gacm_cdev_cleanup(); }
#endif
    kfree(f->config);
    f->config = NULL;
    n_acm_ports_init = 0;
}

static USB_INT
acm_function_bind_config(struct android_usb_function* f, struct usb_configuration* c)
{
    USB_INT port_num = 0;
    USB_INT ret = 0;

    DBG_I(MBB_ACM, "f->name:%s\n", f->name);
    if (!strcmp(f->name, "pcui") && n_acm_cdev_ports > 0)
    {
        port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_PCUI, acm_class_cdev);
        if (port_num < 0)
        {
            ret = port_num;
        }
        else
        {
            DBG_T(MBB_PNP, "binding '%s' to config '%s'/%p\n",
                  f->name,
                  c->label, c);
            acm_bind_config(c, port_num, acm_class_cdev, f);
        }
    }
    else if (!strcmp(f->name, "gps") && n_acm_cdev_ports > 0)
    {
        port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_GPS, acm_class_cdev);
        if (port_num < 0)
        {
            ret = port_num;
        }
        else
        {
            DBG_T(MBB_PNP, "binding '%s' to config '%s'/%p\n",
                  f->name,
                  c->label, c);
            acm_bind_config(c, port_num, acm_class_cdev, f);
        }
    }
    else if (!strcmp(f->name, "3g_gps") && n_acm_cdev_ports > 0)
    {
        port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_3G_GPS, acm_class_cdev);
        if (port_num < 0)
        {
            ret = port_num;
        }
        else
        {
            DBG_T(MBB_PNP, "binding '%s' to config '%s'/%p\n",
                  f->name,
                  c->label, c);
            acm_bind_config(c, port_num, acm_class_cdev, f);
        }
    }
    else if (!strcmp(f->name, "4g_diag") && n_acm_cdev_ports > 0)
    {
        port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_DIAG, acm_class_cdev);
        if (port_num < 0)
        {
            ret = port_num;
        }
        else
        {
            DBG_T(MBB_PNP, "binding '%s' to config '%s'/%p\n",
                  f->name,
                  c->label, c);
            acm_bind_config(c, port_num, acm_class_cdev, f);
        }
    }
    else if (!strcmp(f->name, "3g_diag") && n_acm_cdev_ports > 0)
    {
        port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_3G_DIAG, acm_class_cdev);
        if (port_num < 0)
        {
            ret = port_num;
        }
        else
        {
            DBG_T(MBB_PNP, "binding '%s' to config '%s'/%p\n",
                  f->name,
                  c->label, c);
            acm_bind_config(c, port_num, acm_class_cdev, f);
        }

    }
    else if (!strcmp(f->name, "c_shell") && n_acm_cdev_ports > 0)
    {
        port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_BLUETOOTH , acm_class_cdev);
        if (port_num < 0)
        {
            ret = port_num;
        }
        else
        {
            DBG_T(MBB_PNP, "binding '%s' to config '%s'/%p\n",
                  f->name,
                  c->label, c);
            acm_bind_config(c, port_num, acm_class_cdev, f);
        }
    }
    else if (!strcmp(f->name, "pcsc") && n_acm_cdev_ports > 0)
    {
        //port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_3G_GPS,acm_class_tty);
        //acm_bind_config(c, port_num, acm_class_tty);
    }
    else if (!strcmp(f->name, "voice") && n_acm_cdev_ports > 0)
    {
        //port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_3G_GPS,acm_class_tty);
        //acm_bind_config(c, port_num, acm_class_tty);
    }
    else if (!strcmp(f->name, "gps_ctl") && n_acm_cdev_ports > 0)
    {
        port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_CTRL , acm_class_cdev);
        if (port_num < 0)
        {
            ret = port_num;
        }
        else
        {
            DBG_T(MBB_PNP, "binding '%s' to config '%s'/%p\n",
                  f->name,
                  c->label, c);
            acm_bind_config(c, port_num, acm_class_cdev, f);
        }
    }
    else if (!strcmp(f->name, "a_shell") && n_acm_tty_ports > 0)
    {
        port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_3G_GPS, acm_class_tty);
        if (port_num < 0)
        {
            ret = port_num;
        }
        else
        {
            DBG_T(MBB_PNP, "binding '%s' to config '%s'/%p\n",
                  f->name,
                  c->label, c);
            acm_bind_config(c, port_num, acm_class_tty, f);
        }
    }
    else if (!strcmp(f->name, "3g_modem") || !strcmp(f->name, "modem"))
    {
        /*modem 已实现*/
        port_num = ACM_GET_PORT_NUM_FROM_PROTOCAL(USB_IF_PROTOCOL_MODEM, acm_class_modem);
        if (port_num < 0)
        {
            ret = port_num;
        }
        else
        {
            DBG_T(MBB_PNP, "binding '%s' to config '%s'/%p\n",
                  f->name,
                  c->label, c);
            acm_bind_config(c, port_num, acm_class_modem, f);
        }
    }
    return ret;
}

static ssize_t acm_instances_show(struct device* dev, struct device_attribute* attr, USB_CHAR* buf)
{
    return snprintf(buf, MAX_SHOW_LEN, "%d\n", n_acm_cdev_ports + n_acm_tty_ports + n_acm_mdm_ports);
}

static ssize_t acm_instances_store(struct device* dev, struct device_attribute* attr, const USB_CHAR*  buf, size_t size)
{
    struct android_usb_function* f = dev_get_drvdata(dev);
    struct acm_function_config* config = f->config;
    USB_INT value;

    (USB_VOID)sscanf(buf, "%d", &value);
    if (value > MAX_ACM_INSTANCES)
    {
        value = MAX_ACM_INSTANCES;
    }

    config->instances = n_acm_cdev_ports + n_acm_tty_ports;
    return size;
}

static DEVICE_ATTR(instances, S_IRUGO | S_IWUSR, acm_instances_show, acm_instances_store);
static struct device_attribute* acm_function_attributes[] =
{
    &dev_attr_instances,
    NULL
};


static struct android_usb_function acm_function =
{
    .name       = "acm",
    .init           = acm_function_init,
    .cleanup     = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};

static struct android_usb_function acm_3g_modem_function =
{
    .name        = "3g_modem",
    .init        = acm_function_init,
    .cleanup     = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes  = acm_function_attributes,
};

static struct android_usb_function acm_modem_function =
{
    .name        = "modem",
    .init        = acm_function_init,
    .cleanup     = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes  = acm_function_attributes,
};

static struct android_usb_function acm_pcui_function =
{
    .name     = "pcui",
    .init     = acm_function_init,
    .cleanup  = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};
static struct android_usb_function acm_gps_function =
{
    .name        = "gps",
    .init           = acm_function_init,
    .cleanup     = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};
static struct android_usb_function acm_4g_diag_function =
{
    .name           = "4g_diag",
    .init               = acm_function_init,
    .cleanup        = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};

static struct android_usb_function acm_3g_diag_function =
{
    .name       = "3g_diag",
    .init           = acm_function_init,
    .cleanup       = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};
static struct android_usb_function acm_c_shell_function =
{
    .name           = "c_shell",
    .init               = acm_function_init,
    .cleanup = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};

static struct android_usb_function acm_a_shell_function =
{
    .name           = "a_shell",
    .init               = acm_function_init,
    .cleanup         = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};

static struct android_usb_function acm_pcsc_function =
{
    .name           = "pcsc",
    .init               = acm_function_init,
    .cleanup        = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};

static struct android_usb_function acm_voice_function =
{
    .name           = "voice",
    .init              = acm_function_init,
    .cleanup        = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};

static struct android_usb_function acm_ctl_function =
{
    .name           = "gps_ctl",
    .init              = acm_function_init,
    .cleanup        = acm_function_cleanup,
    .bind_config = acm_function_bind_config,
    .attributes = acm_function_attributes,
};

USB_VOID usb_acm_init (USB_VOID)
{
    DBG_I(MBB_ACM, "%s entry!\n" , __func__ );

    pnp_register_usb_support_function(&acm_function);
    pnp_register_usb_support_function(&acm_3g_modem_function);
    pnp_register_usb_support_function(&acm_pcui_function);
    pnp_register_usb_support_function(&acm_gps_function);
    pnp_register_usb_support_function(&acm_4g_diag_function);
    pnp_register_usb_support_function(&acm_3g_diag_function);
    pnp_register_usb_support_function(&acm_c_shell_function);
    pnp_register_usb_support_function(&acm_a_shell_function);
    pnp_register_usb_support_function(&acm_pcsc_function);
    pnp_register_usb_support_function(&acm_voice_function);
    pnp_register_usb_support_function(&acm_modem_function);
    pnp_register_usb_support_function(&acm_ctl_function);
}
USB_VOID gacm_cleanup(USB_VOID)
{
    DBG_I(MBB_ACM, "%s entry!\n", __func__ );
    if (n_acm_tty_ports > 0)
    {
        gserial_cleanup();
    }

    if (n_acm_cdev_ports > 0)
    {
        gacm_cdev_cleanup();
    }

    if (n_acm_mdm_ports > 0)
    {
        gacm_modem_cleanup();
    }

}

