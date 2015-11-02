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

#ifndef _FAST_ON_OFF_H
#define _FAST_ON_OFF_H

#include "usb_platform_comm.h"
#include <drv_chg.h>
#include <drv_onoff.h>

#ifdef USB_FAST_ON_OFF
USB_INT usb_fast_on_off_stat(USB_VOID);
USB_VOID fast_on_off_init(USB_VOID);
USB_VOID fast_on_off_exit(USB_VOID);
#else
static inline USB_INT usb_fast_on_off_stat(USB_VOID)
{
    return 0;
}

static inline USB_VOID fast_on_off_init(USB_VOID)
{
    return ;
}

static inline USB_VOID fast_on_off_exit(USB_VOID)
{
    return ;
}

#endif

#endif
