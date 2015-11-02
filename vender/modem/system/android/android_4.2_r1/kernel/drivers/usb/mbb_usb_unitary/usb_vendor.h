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
#ifndef __USB_VENDOR_H__
#define __USB_VENDOR_H__

#ifdef __cplusplus
extern "C" { /* allow C++ to use these headers */
#endif /* __cplusplus */
#include <product_config.h>
#include <linux/notifier.h>
#include "usb_hotplug.h"
#include "hw_pnp.h"
#include "hw_pnp_api.h"

#define USB_NOTIF_PRIO_ADP      0           /* adp has lowest priority */
#define USB_NOTIF_PRIO_FD       100         /* function drvier */
#define USB_NOTIF_PRIO_CORE     200         /* usb core */
#define USB_NOTIF_PRIO_HAL      300         /* hardware has highest priority */

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
#define USB_CHARGER_TYPE_CRADLE         8

#define USB_IS_SUPPORT_NV               1
#define USB_IS_SUPPORT_OM_DUMP          1
#define USB_IS_SUPPORT_PMU              1
#define USB_IS_SUPPORT_VSHELL           1
#define USB_IS_SUPPORT_WAKELOCK         1
#define USB_IS_SUPPORT_DYN_INIT         1


#define USB_FD_DEVICE_MAX               32
#define USB_CDEV_NAME_MAX               64

//#define GNET_MAX_NUM       (4)
//#define GNET_USED_NUM      (1)


#if !defined(CONFIG_PC_USE_HUAWEI_DRIVER)
#define USB_IF_SUBCLASS_BALONG      0x02
#else /* PC_USE_HUAWEI_DRIVER */
#define USB_IF_SUBCLASS_BALONG      0x03
#endif

typedef struct usb_enum_stat
{
    char* fd_name;                  /* function drvier file name */
    unsigned usb_intf_id;           /* usb interface id */
    unsigned is_enum;               /* whether the dev is enum */
} usb_enum_stat_t;

int power_on_dwc3_usb(void);
int power_off_dwc3_usb(void);
int bsp_usb_adapter_init(void);

int bsp_usb_is_support_charger(void);
int bsp_usb_is_support_pmu_detect(void);
void bsp_usb_wait_cdev_created(void);
void bsp_usb_clear_last_cdev_name(void);

/* notify interface */
void bsp_usb_register_notify(struct notifier_block* nb);
void bsp_usb_unregister_notify(struct notifier_block* nb);

/* usb status change interface*/
void bsp_usb_status_change(int status);
//int adp_usb_queue_delay_work(struct delayed_work *dwork, unsigned long delay);
int usb_balong_init(void);
void usb_balong_exit(void);
int bsp_usb_console_init(void);


/* usb enum done interface */
void bsp_usb_init_enum_stat(void);
int bsp_usb_is_all_enum(void);
int bsp_usb_is_all_disable(void);
#define bsp_usb_add_setup_dev(intf_id) \
    bsp_usb_add_setup_dev_fdname(intf_id, __FILE__)
void bsp_usb_add_setup_dev_fdname(unsigned intf_id, char* fd_name);
void bsp_usb_set_enum_stat(unsigned intf_id, int enum_stat);
void bsp_usb_set_last_cdev_name(char* dev_name);


/* usb nv feature functions */
/* 1: support, 0:not support */
int bsp_usb_is_support_wwan(void);
int bsp_usb_is_support_shell(void);

/* ncm host assemble function */
int ncm_set_host_assemble_param(int net_id, unsigned long host_timeout);

int  bsp_usb_usbid_proc();

#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif /* __cplusplus */

#endif    /* End of __USB_VENDOR_H__ */

