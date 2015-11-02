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
#ifndef __USB_HOTPLUG_H__
#define __USB_HOTPLUG_H__

#ifdef __cplusplus
extern "C" { /* allow C++ to use these headers */
#endif /* __cplusplus */

#include <linux/notifier.h>
#include <linux/workqueue.h>
//#include <product_config.h>
#include "usb_platform_comm.h"

/*
 * usd to support usb event notify
 * connect pc:
 * insert(1) -> chargertype iden (2) -> enum_done(3) -> remove(0)/disable(-1) +
 *   ^___________________________________________________________|
 *
 * connect charger (must pmu support):
 * insert(1) -> remove(0) +
 *   ^____________________|
 *
 * connect peripheral(such as udisk):
 * perip_insert(4) -> perip_remove(5)
 *   ^_______________________^
 */
#define USB_BALONG_DEVICE_INSERT    1
#define USB_BALONG_CHARGER_IDEN     2
#define USB_BALONG_ENUM_DONE        3
#define USB_BALONG_PERIP_INSERT     4
#define USB_BALONG_PERIP_REMOVE     5
#define USB_BALONG_DEVICE_REMOVE    0
/* if the version is not support pmu detect
 * and all the device is disable, we assume that the usb is remove,
 */
#define USB_BALONG_DEVICE_DISABLE   (0xF1)

#define USB_BALONG_HOTPLUG_IDLE     0
#define USB_BALONG_HOTPLUG_INSERT   1
#define USB_BALONG_HOTPLUG_REMOVE   2

/*
 * charger type define
 */
#define USB_CHARGER_TYPE_HUAWEI         1
#define USB_CHARGER_TYPE_NOT_HUAWEI     2
#define USB_CHARGER_TYPE_INVALID        0

#define USB_CDEV_NAME_MAX               64


typedef USB_INT (*usb_notifier_call_chain_cb_t)(unsigned long val, unsigned long v);
typedef  USB_INT (*usb_poweroff)(USB_VOID);
typedef  USB_INT (*usb_poweron)(USB_VOID);
typedef  USB_INT (*usb_entry)(USB_VOID);
typedef  USB_VOID (*usb_exit)(USB_VOID);
typedef USB_VOID (*usbid_proc)(USB_VOID);
typedef USB_VOID (*usb_wait_enumdone)(USB_VOID);
typedef USB_VOID (*usb_clear_enumdone)(USB_VOID);
typedef USB_INT (*usb_check_wireless_chg)(USB_VOID);
typedef USB_INT (*usb_wireless_chg_remove)(USB_VOID);
/*USB插拔需要适配的接口*/
typedef struct _usb_hotplug_hanlder
{
    usb_poweroff usb_poweroff_cb;                   /*USB下电 不支持 usb下电赋空*/
    usb_poweron usb_poweron_cb;                     /*USB上 电 不支持 usb上 电赋空*/
    usb_entry       usb_entry_cb;                   /*USB插入回调*/
    usb_exit         usb_exit_cb;                   /*USB拔出回调*/
    usbid_proc     usbid_proc_cb;                   /*USB ID是否通过GPIO控制*/
    usb_wait_enumdone  usb_wait_enumdone_cb;        /*USB 等待  是否枚举完成*/
    usb_clear_enumdone usb_clear_enumdone_cb;       /*USB 清空 是否枚举完成*/
    usb_check_wireless_chg  usb_check_wireless_chg_cb; /*判断是否无线充电*/
    usb_wireless_chg_remove  usb_wireless_chg_remove_cb; /*通知无线充电拔出*/
    struct workqueue_struct* usb_notify_wq_extern;      /*外部已存在usb的workqueue*/
    /*we can add other callbacks here*/
} usb_hotplug_hanlder_t;

//usb_adp_ctx_t* usb_get_adp_ctx(void);
void bsp_usb_status_change_ex(int status);
void bsp_usb_status_update_ex(int status);
int usb_get_vbus_status(void);
int usb_set_vbus_status(int value);
void set_id_delta_time(int mstime);

/* notify interface */
void bsp_usb_register_notify(struct notifier_block* nb);
void bsp_usb_unregister_notify(struct notifier_block* nb);

/* usb status change interface*/
void bsp_usb_status_change(int status);
USB_INT adp_usb_queue_delay_work(struct delayed_work* dwork, USB_ULONG delay);

USB_INT usb_hotplug_init(USB_VOID);
USB_VOID bsp_usb_set_charger_type(USB_INT type);

USB_VOID usb_wake_lock(USB_VOID);
USB_VOID usb_wake_unlockUSB_VOID(USB_VOID);

USB_ULONG usb_get_insert_timestamp(USB_VOID);
USB_ULONG usb_get_remove_timestamp(USB_VOID);

USB_INT usb_get_hotplug_status(USB_VOID);

USB_INT usb_get_hotplug_old_status(USB_VOID);

USB_VOID* usb_get_notifier_handle(USB_VOID);

USB_INT usb_get_vbus_status(USB_VOID);
USB_VOID usb_wake_unlock(USB_VOID);
USB_VOID usb_wake_lock(USB_VOID);
#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif /* __cplusplus */

#endif    /* End of __USB_HOTPLUG_H__ */

