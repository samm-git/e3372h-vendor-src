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
    问题单号                       时间                   修改人                        说明
******************************************************************************************************/
#ifndef _U_CDEV_H
#define _U_CDEV_H
#include "usb_platform_comm.h"
#define ACM_CDEV_PREFIX             "acm_"
#define ACM_CDEV_DRV_NAME           "acm_cdev"
#define ACM_CDEV_QUEUE_SIZE         16
#define WRITE_BUF_SIZE              8192
#define ACM_CDEV_DFT_RD_BUF_SIZE    2048
#define ACM_CDEV_DFT_RD_REQ_NUM     8
#define ACM_CDEV_DFT_WT_REQ_NUM     16
#define ACM_CDEV_AT_WT_REQ_NUM      256
#define ACM_CDEV_NAME_MAX   64




USB_INT gacm_cdev_setup(struct usb_gadget *g, USB_UINT count);
USB_VOID gacm_cdev_cleanup(USB_VOID);
USB_INT gacm_cdev_line_state(struct gserial *gser, USB_UINT32 state);
USB_INT gacm_cdev_suspend(struct gserial *gser);
USB_INT gacm_cdev_resume(struct gserial *gser);
USB_INT gacm_cdev_connect(struct gserial *gser, USB_UINT8 port_num);
USB_VOID gacm_cdev_disconnect(struct gserial *gser);
USB_VOID usb_cdev_init(USB_VOID);
USB_VOID usb_cdev_exit(USB_VOID);
#endif
