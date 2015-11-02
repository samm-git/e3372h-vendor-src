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
    问题单号              时间                 修改人                        说明
******************************************************************************************************/
#ifndef _MBB_USB_ADP_H
#define _MBB_USB_ADP_H
#include <linux/delay.h>
#include "usb_charger_manager.h"


typedef struct console* (*uart_console_cb_t)(void);

typedef int (*battery_notifier_call_chain_cb_t)(\
        unsigned long val, unsigned long v);
typedef  void (*stm_set_chg_type_cb_t)(chg_chgr_type_t chgr_type);
typedef void (*usb_register_notify_cb_t)(struct notifier_block* nb);
typedef void (*usb_unregister_notify_cb_t)(struct notifier_block* nb);
typedef void (*usb_switch_gatway_mode_cb_t)(unsigned int gmode);

typedef struct _usb_adp_ctx
{

    battery_notifier_call_chain_cb_t battery_notifier_call_chain_cb;
    stm_set_chg_type_cb_t stm_set_chg_type_cb;
    usb_register_notify_cb_t usb_register_notify_cb;
    usb_unregister_notify_cb_t usb_unregister_notify_cb;
    uart_console_cb_t  bsp_get_uart_console_cb;
    usb_switch_gatway_mode_cb_t usb_switch_gatway_mode_cb;
    struct delayed_work gatway_disconnect_work;    // 中断中不能直接调用切网关的回调，否则会核间通信失败
    unsigned int gatway_work_mode;
    /*we can add other callbacks here*/
} usb_adp_ctx_t;

usb_adp_ctx_t* usb_get_adp_ctx(void);

USB_INT usb_get_mmc_dev_idex(USB_VOID);

USB_INT usb_get_mmc_status(USB_VOID);

USB_INT Query_USB_PortType(USB_UINT* pucDialmode, USB_UINT* pucCdcSpec);

USB_INT Check_EnablePortName(USB_CHAR* PsBuffer, USB_ULONG* Length);

#endif
