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

*****************************************************************************************************/
#ifndef _DWC_OTG_EXTERN_CHARGE_H
#define _DWC_OTG_EXTERN_CHARGE_H

#include <linux/workqueue.h>
#include <linux/gpio.h>
#include "usb_platform_comm.h"
#include <product_config.h>
#include <linux/wakelock.h>
#define GPIO_0_17                     (17)
#define GPIO_2_19                     (83)
#define GPIO_2_20                     (84)

#define GPIO_2_23                       (87)
#define GPIO_2_31	                  (95) 
#if (FEATURE_ON == MBB_CHG_PLATFORM_9X25)
/*高通平台GPIO端口号*/
#define GPIO_OTG_ID_DET      GPIO_0_17
#define GPIO_OTG_ID_SET      GPIO_2_19
#define GPIO_DMDP_CONNECT    GPIO_2_20
#else
/*balong平台GPIO端口号*/


#define GPIO_OTG_ID_DET      OTG_ID_DET_GPIO
#define GPIO_OTG_ID_SET      OTG_ID_SET_GPIO
#define GPIO_DMDP_CONNECT    DMDP_CONNECT_GPIO

#endif

#define GPIO_HIGH 1
#define GPIO_LOW 0


typedef struct otg_charger
{
    USB_BOOL  ext_chg_id_ins;
    wait_queue_head_t wait_wq;
    struct wake_lock otg_chg_wakelock;
    struct work_struct otg_chg_notify_work;
} otg_charger_st;
USB_VOID usb_otg_extern_charge_event_notify( USB_BOOL  usbid_on_off);
USB_VOID usb_otg_extern_charge_init(USB_VOID);
USB_VOID usb_otg_extern_charge_remove(USB_VOID);



#endif
